#!/usr/bin/env python3
"""Check four ANM child call routes without granting parent or callee exactness."""
from __future__ import annotations

import argparse
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys
import tomllib

from capstone import (CS_ARCH_X86, CS_MODE_32, CS_GRP_CALL, CS_GRP_JUMP,
                      CS_GRP_RET, CS_GRP_INT, CS_GRP_IRET, CS_GRP_PRIVILEGE, Cs)
from capstone.x86 import X86_OP_IMM
from linked_image import PEImage, linked_functions

ROOT = Path(__file__).resolve().parents[1]
CHILD_OPCODES = (88, 90, 91, 92)
EXECUTE_SYMBOL = "?ExecuteScript@AnmRenderManagerView@@SGHPAUAnmVmView@@@Z"


def module(filename, name):
    spec = importlib.util.spec_from_file_location(name, ROOT / "scripts" / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def source_factories(text):
    """Require one named creator in each complete source case, independently."""
    text = re.sub(r"//[^\n]*|/\*.*?\*/", "", text, flags=re.S)
    result = {}
    for opcode in CHILD_OPCODES:
        matches = list(re.finditer(
            rf"(?m)^\s*case\s+ANM_OP_CREATE_CHILD_{opcode}\s*:", text))
        if len(matches) != 1:
            raise ValueError(f"source child{opcode} does not have one case")
        rest = text[matches[0].end():]
        following = re.search(r"(?m)^\s*case\b", rest)
        body = rest[:following.start()] if following else rest
        names = re.findall(r"\b(CreateVmVariant[0-3])\s*\(", body)
        if len(names) != 1:
            raise ValueError(f"source child{opcode} does not have one creator")
        result[opcode] = names[0]
    return result


def call_paths(image, start, lower, upper):
    """Follow direct branches within a closed region; external CALLs return."""
    if not lower <= start < upper:
        raise ValueError("case entry leaves the child region")
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(lower, upper - lower), lower))
    if sum(i.size for i in instructions) != upper - lower:
        raise ValueError("child region is not completely decoded")
    by_address = {i.address: i for i in instructions}
    pending = [(start, (), frozenset())]
    paths = []
    states = 0
    while pending:
        pc, calls, visited = pending.pop()
        states += 1
        if states > 4096:
            raise ValueError("child CFG exceeds the bounded state budget")
        if not lower <= pc < upper:
            paths.append({"calls": calls, "exit": pc})
            continue
        if pc in visited:
            raise ValueError("child CFG contains a cycle")
        if pc not in by_address:
            raise ValueError("child branch reaches an instruction interior")
        instruction = by_address[pc]
        seen = visited | {pc}
        following = pc + instruction.size
        mnemonic = instruction.mnemonic
        control_groups = {CS_GRP_CALL, CS_GRP_JUMP, CS_GRP_RET,
                          CS_GRP_INT, CS_GRP_IRET, CS_GRP_PRIVILEGE}
        if (control_groups.intersection(instruction.groups)
                and mnemonic != "call" and not mnemonic.startswith("j")):
            raise ValueError("unsupported child control flow")
        if mnemonic == "call" or mnemonic.startswith("j"):
            if len(instruction.operands) != 1 or instruction.operands[0].type != X86_OP_IMM:
                raise ValueError("child CFG contains indirect control flow")
            destination = instruction.operands[0].imm & 0xffffffff
            if mnemonic == "call":
                if lower <= destination < upper:
                    raise ValueError("internal child CALL is not modeled")
                pending.append((following, calls + (destination,), seen))
            elif mnemonic == "jmp":
                pending.append((destination, calls, seen))
            else:
                pending.extend([(following, calls, seen), (destination, calls, seen)])
        elif mnemonic.startswith(("ret", "loop", "iret")) or mnemonic in {
                "int", "int3", "ud0", "ud1", "ud2", "hlt", "syscall", "sysenter"}:
            raise ValueError("unsupported child control flow")
        else:
            pending.append((following, calls, seen))
    if not paths:
        raise ValueError("child CFG has no bounded exit")
    return paths


def require_calls(paths, creator, getter, resolver, insert):
    expected = [(creator, resolver, insert), (getter, creator, resolver, insert)]
    if any(tuple(path["calls"]) not in expected for path in paths):
        raise ValueError("child calls differ from creator -> GetVm -> InsertAfter")
    return len(paths)


def inspect_routes(image, table, factories, helper_addresses):
    points = sorted(set(table))
    entries = {op: table[op + 1] for op in CHILD_OPCODES}
    selected = set(entries.values())
    lower, final = min(selected), max(selected)
    upper = next((address for address in points if address > final), None)
    if len(selected) != 4 or upper is None:
        raise ValueError("four distinct bounded child case entries are required")
    if {p for p in points if lower <= p < upper} != selected:
        raise ValueError("child cases are not one contiguous physical group")
    routes = []
    for opcode in CHILD_OPCODES:
        paths = call_paths(image, entries[opcode], lower, upper)
        count = require_calls(paths, factories[opcode], *helper_addresses)
        routes.append({
            "opcode": opcode,
            "entry": f"0x{entries[opcode]:08X}",
            "creator": f"0x{factories[opcode]:08X}",
            "static_paths": count,
            "paths": [{"calls": [f"0x{x:08X}" for x in p["calls"]],
                       "exit": f"0x{p['exit']:08X}"} for p in paths],
        })
    return {"region_start": f"0x{lower:08X}", "region_end_exclusive": f"0x{upper:08X}",
            "routes": routes}


def compare(source, artifact_dir=None):
    manifest = tomllib.loads((ROOT / "config/match-units.toml").read_text())
    units = manifest["units"]
    table_module = module("report-anm-execute-table.py", "child_routing_table")
    target = module("probe-ltcg-backlog.py", "child_routing_target").verified_target()
    if manifest["target_sha256"] != target.sha256:
        raise ValueError("canonical bindings name a different target")
    enums, _, problems = table_module.parse_source(source, table_module.DEFAULT_HEADER)
    if problems:
        raise ValueError("; ".join(problems))
    expected_enums = {f"ANM_OP_CREATE_CHILD_{op}": op for op in CHILD_OPCODES}
    if any(enums.get(name) != number for name, number in expected_enums.items()):
        raise ValueError("child opcode numbers differ from the reviewed selectors")
    named = source_factories(source.read_text())

    def selected_symbol(prefix):
        found = [u for u in units.values() if u.get("symbol", "").startswith(prefix)]
        identities = {(u["symbol"], u["target_address"], u["size"]) for u in found}
        if len(identities) != 1:
            raise ValueError(f"expected one canonical symbol for {prefix}")
        return found[0]

    creators = {name: selected_symbol(f"?{name}@AnmLoadedView@@")
                for name in set(named.values())}
    helpers = [selected_symbol(prefix) for prefix in (
        "?GetIntVar@AnmVmView@@", "?GetVm@AnmVmIdView@@",
        "?InsertAfter@AnmVmLayerNodeView@@")]
    target_table = struct.unpack("<94I", target.read_address(
        table_module.JUMP_TABLE_ADDRESS, 94 * 4))
    target_routes = inspect_routes(
        target, target_table,
        {op: creators[name]["target_address"] for op, name in named.items()},
        [u["target_address"] for u in helpers])
    result = {
        "scope": "Four source/target call routes only; no parent/callee exactness or runtime claim",
        "assumptions": "CALLs return; conditional predicates unconstrained; bounded child region",
        "result": "passed", "target_sha256": target.sha256,
        "source": str(source), "source_factories": named, "target": target_routes,
    }
    if artifact_dir is not None:
        image_path = artifact_dir / "source.exe"
        image = PEImage(image_path)
        functions = linked_functions(
            image_path, artifact_dir / "source.map", artifact_dir / "source.pdb")["functions"]

        def owner(symbol):
            found = [f for f in functions if f["symbol"] == symbol]
            if len(found) != 1:
                raise ValueError(f"candidate lacks one PDB owner for {symbol}")
            return found[0]

        execute = owner(EXECUTE_SYMBOL)
        address = int(execute["address"], 0)
        layout = table_module.candidate_case_layout(
            image_path, address, execute["size"], target_table,
            {number: name for name, number in enums.items()})
        if not layout["physical_order_matches"]:
            raise ValueError("candidate physical case order differs")
        candidate_table = struct.unpack("<94I", image.read_address(
            int(layout["jump_table_address"], 0), 94 * 4))
        creator_owners = {name: owner(u["symbol"]) for name, u in creators.items()}
        helper_owners = [owner(u["symbol"]) for u in helpers]
        result["candidate"] = inspect_routes(
            image, candidate_table,
            {op: int(creator_owners[name]["address"], 0) for op, name in named.items()},
            [int(f["address"], 0) for f in helper_owners])
        result["candidate"].update(
            image=str(image_path), image_sha256=image.sha256, execute_owner=execute,
            creator_owners=creator_owners,
            qualification="Entry/address binding only; whole callee bytes must be checked separately",
        )
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=ROOT / "src/AnmManager.cpp")
    parser.add_argument("--artifact-dir", type=Path)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        result = compare(args.source.resolve(), args.artifact_dir.resolve()
                         if args.artifact_dir else None)
    except (OSError, ValueError, KeyError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        for opcode, creator in result["source_factories"].items():
            print(f"child{opcode}: {creator}")
        print("ANM child call routing passed; no exactness credit")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
