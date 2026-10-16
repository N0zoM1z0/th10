#!/usr/bin/env python3
"""Read-only Options call topology; run with scripts/repo-python from repo root.

Example:
  scripts/repo-python scripts/report-options-owner-topology.py \
    --image build/probe-ltcg/EXISTING/source.exe \
    --map build/probe-ltcg/EXISTING/source.map \
    --pdb build/probe-ltcg/EXISTING/source.pdb

No compilation, byte-match score, offset alignment, or exactness claim is made.
Only JSON is written to stdout. Never commit executables, PDBs, or binary dumps.
"""
from __future__ import annotations

import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import re
import sys
import struct
import tomllib

# Also support a reviewed copy outside scripts/, invoked from the repository root.
sys.dont_write_bytecode = True
sys.path.insert(0, str(Path.cwd() / "scripts"))
from linked_image import (LinkedImageError, PEImage, linked_functions,
                          map_publics, verify_capstone)

EXPECTED_TARGET_SHA256 = "2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040"

OWNERS = (("RefreshOptionsDisplay", 0x42E5A0, 3999, 3999),
          ("UpdateOptions", 0x42D920, 3188, 3168))
MAP_LINE = re.compile(r"^\s*([0-9A-Fa-f]{4}):([0-9A-Fa-f]{8})\s+"
                      r"(\S+)\s+([0-9A-Fa-f]{8})\s+(.+)$")


def hx(value):
    return f"0x{value:08X}"


def symbols_from_map(path, image):
    """Public records are a list, not an address->names dictionary.

    Read static/private rows too, validating each row's VA against PE sections.
    No nearest-symbol attribution: a name is attached only at its exact VA.
    """
    records = map_publics(path)
    by_address = defaultdict(set)
    private = set()
    for record in records:
        by_address[int(record["address"])].add(str(record["symbol"]))
    in_static = False
    for line in Path(path).read_text(encoding="cp1252").splitlines():
        if "Static symbols" in line:
            in_static = True
        match = MAP_LINE.match(line)
        if not match:
            continue
        section, offset, name, address, _ = match.groups()
        address = int(address, 16)
        section_number = int(section, 16)
        # Absolute symbols do not have a PE section mapping.
        if not section_number:
            continue
        expected = image.image_base + image.section(section_number).rva + int(offset, 16)
        if address != expected:
            raise LinkedImageError(f"map VA mismatch for {name}")
        by_address[address].add(name)
        if in_static:
            private.add(address)
    return {a: sorted(names) for a, names in by_address.items()}, private


def switch_tail(image, address, size, decoder):
    """Validate a five-entry suffix table with an unmodified, bounded index.

    This is a narrow compiler-pattern recognizer, not whole-function CFG proof.
    Allow up to four PUSHes before JA and 12 index-preserving straight-line
    instructions between JA and dispatch.
    """
    from capstone import CS_GRP_CALL, CS_GRP_INT, CS_GRP_IRET, CS_GRP_JUMP, CS_GRP_RET
    from capstone.x86_const import X86_OP_REG, X86_OP_IMM, X86_OP_MEM
    if size < 20:
        return None
    table = address + size - 20
    raw = image.read_address(address, size)
    insns = list(decoder.disasm(raw[:-20], address))
    if sum(i.size for i in insns) != size - 20:
        return None
    boundaries = {i.address for i in insns}
    entries = struct.unpack("<5I", raw[-20:])
    if not all(e in boundaries for e in entries):
        return None
    alias_groups = (
        {"eax", "ax", "al", "ah"}, {"ebx", "bx", "bl", "bh"},
        {"ecx", "cx", "cl", "ch"}, {"edx", "dx", "dl", "dh"},
        {"esi", "si", "sil"}, {"edi", "di", "dil"},
        {"ebp", "bp", "bpl"}, {"esp", "sp", "spl"},
    )
    flow_groups = {CS_GRP_CALL, CS_GRP_INT, CS_GRP_IRET, CS_GRP_JUMP, CS_GRP_RET}
    for index, cmp in enumerate(insns):
        if (cmp.mnemonic != "cmp" or len(cmp.operands) != 2
                or cmp.operands[0].type != X86_OP_REG
                or cmp.operands[1].type != X86_OP_IMM or cmp.operands[1].imm != 4):
            continue
        register = cmp.operands[0].reg
        register_name = decoder.reg_name(register)
        # A 16/8-bit comparison does not bound a 32-bit dispatch index.
        if register_name not in {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"}:
            continue
        aliases = next(group for group in alias_groups if register_name in group)
        guard_index = index + 1
        pushes = []
        # PUSH itself preserves EFLAGS; no other pre-guard opcode is admitted.
        while guard_index < len(insns) and insns[guard_index].mnemonic == "push" and len(pushes) < 4:
            push = insns[guard_index]
            try:
                _, writes = push.regs_access()
            except Exception:
                break
            if aliases.intersection(decoder.reg_name(reg) for reg in writes):
                break
            pushes.append(hx(push.address))
            guard_index += 1
        if guard_index >= len(insns):
            continue
        branch = insns[guard_index]
        if (branch.mnemonic != "ja" or len(branch.operands) != 1
                or branch.operands[0].type != X86_OP_IMM
                or branch.operands[0].imm not in boundaries):
            continue
        intervening = []
        for jump in insns[guard_index + 1:guard_index + 14]:
            if (jump.mnemonic == "jmp" and len(jump.operands) == 1
                    and jump.operands[0].type == X86_OP_MEM):
                mem = jump.operands[0].mem
                dispatch_path = {i.address for i in insns[index:guard_index + 1 + len(intervening)]}
                dispatch_path.update((cmp.address, branch.address, jump.address))
                if (mem.base == 0 and mem.index == register and mem.scale == 4
                        and mem.disp & 0xFFFFFFFF == table
                        and branch.operands[0].imm not in dispatch_path):
                    return {"table_address": hx(table), "entries": list(map(hx, entries)),
                            "entry_count": 5, "bound_site": hx(cmp.address),
                            "unsigned_above_site": hx(branch.address),
                            "dispatch_site": hx(jump.address),
                            "index_register": register_name,
                            "pre_guard_push_sites": pushes,
                            "index_preserving_sites": intervening}
                break
            if flow_groups.intersection(jump.groups) or jump.mnemonic in {"sysenter", "sysexit", "syscall", "sysret", "hlt", "ud2"}:
                break
            try:
                _, writes = jump.regs_access()
            except Exception:
                break  # Unavailable register-effect evidence must fail closed.
            if aliases.intersection(decoder.reg_name(reg) for reg in writes):
                break
            intervening.append(hx(jump.address))
    return None


def inventory(image, address, physical_size, code_size, names, decoder, known_code=True):
    """Linear opcode inventory over a stated extent, not reachability analysis."""
    tail = None if known_code else switch_tail(image, address, physical_size, decoder)
    if tail:
        code_size = physical_size - 20
    raw = image.read_address(address, physical_size)
    instructions = list(decoder.disasm(raw[:code_size], address))
    decoded = sum(ins.size for ins in instructions)
    if decoded != code_size and known_code:
        raise LinkedImageError(f"incomplete decode at {hx(address + decoded)}: "
                               f"{decoded}/{code_size} bytes")
    from capstone.x86_const import X86_INS_CALL, X86_OP_IMM
    calls = defaultdict(list)
    indirect = []
    for ins in instructions:
        if ins.id != X86_INS_CALL:
            continue
        if len(ins.operands) == 1 and ins.operands[0].type == X86_OP_IMM:
            calls[ins.operands[0].imm & 0xFFFFFFFF].append(hx(ins.address))
        else:
            indirect.append({"site": hx(ins.address), "operand": ins.op_str})
    return {
        "address": hx(address),
        "physical_extent": {"size": physical_size, "end_exclusive": hx(address + physical_size),
                            "sha256": hashlib.sha256(raw).hexdigest()},
        "code_extent": ({"size": code_size, "basis": "specified-target-extent"}
                        if known_code else
                        {"size": code_size, "basis": "validated-five-entry-switch-tail", "evidence": tail}
                        if tail else {"size": None, "basis": "unknown; physical extent is not a code proof"}),
        "decoded_span": {"size": code_size, "end_exclusive": hx(address + code_size),
                        "decoded_bytes": decoded,
                        "sha256": hashlib.sha256(raw[:code_size]).hexdigest()},
        "inventory_status": ("specified-target-code" if known_code else
                             "validated-tail-excluded-linear-span" if tail else
                             "unverified-physical-span; potential data/padding artifacts"),
        "decode_complete": decoded == code_size,
        "undecoded_bytes": code_size - decoded,
        "decode_stop_address": hx(address + decoded),
        "counts_are_trusted_code_inventory": known_code or bool(tail),
        "excluded_tail_bytes": physical_size - code_size,
        "direct_call_count": sum(map(len, calls.values())),
        "direct_targets": [{"address": hx(a), "names": names.get(a, []),
                            "count": len(sites), "sites": sites}
                           for a, sites in sorted(calls.items())],
        "indirect_call_count": len(indirect), "indirect_calls": indirect,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("image", "map", "pdb"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--target", type=Path, default=Path("resources/th10.exe"))
    args = parser.parse_args()
    lock = tomllib.loads(Path("config/tools.lock.toml").read_text())["capstone"]
    decoder_identity = verify_capstone(lock)
    import capstone
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    candidate, target = PEImage(args.image), PEImage(args.target)
    if target.sha256 != EXPECTED_TARGET_SHA256:
        raise LinkedImageError("unsupported target SHA256: hardcoded Options extents require "
                               + EXPECTED_TARGET_SHA256 + "; got " + target.sha256)
    linked = linked_functions(args.image, args.map, args.pdb)
    names, private_addresses = symbols_from_map(args.map, candidate)
    target_names = {a: [name] for name, a, _, _ in OWNERS}
    # PDB gives physical contributions, including those without public symbols.
    extents = defaultdict(set)
    for record in linked["functions"] + linked["unowned_code_contributions"]:
        extents[int(str(record["address"]), 0)].add(int(record["size"]))
    owners = []
    owner_addresses = set()
    for name, address, physical, code in OWNERS:
        matches = [f for f in linked["functions"]
                   if f["symbol"].startswith("?" + name + "@FrontEndControllerView@@")]
        if len(matches) != 1:
            raise LinkedImageError(f"expected one linked owner {name}, found {len(matches)}")
        function = matches[0]
        start, size = int(function["address"], 0), int(function["size"])
        owner_addresses.add(start)
        owners.append({"name": name,
                       "target": inventory(target, address, physical, code, target_names, decoder),
                       "candidate": {"symbol": function["symbol"],
                                     "extent_source": function["extent_source"],
                                     **inventory(candidate, start, size, size, names, decoder, known_code=False)}})
    helpers = {}
    for owner in owners:
        outlined = []
        for call in owner["candidate"]["direct_targets"]:
            address = int(call["address"], 0)
            # A conservative, explicit selector; other direct callees remain in inventory.
            is_helper = address in private_addresses or any("Options" in n for n in call["names"])
            if not is_helper or address in owner_addresses:
                continue
            outlined.append({"address": call["address"], "names": call["names"],
                             "call_count": call["count"], "sites": call["sites"]})
            if address in helpers:
                continue
            sizes = extents.get(address, set())
            helper = {"names": call["names"], "address": hx(address)}
            if len(sizes) == 1:
                size = next(iter(sizes))
                helper.update({"extent_source": "pdb-section-contribution-exact-start",
                               **inventory(candidate, address, size, size, names, decoder, known_code=False)})
            else:
                helper["extent_status"] = "unknown-or-ambiguous; no next-symbol size inference"
            helpers[address] = helper
        owner["candidate"]["outlined_helper_edges"] = outlined
    report = {
        "schema": "th10-options-owner-topology-v1", "diagnostic_only": True,
        "exactness_credit": 0,
        "limitations": [
            "Independent opcode inventories; no cross-image raw-offset linkage claims.",
            "Linear decoding is not a reachable control-flow proof; unknown candidate code extents use the physical span, so inventories may include data/padding artifacts.",
            "Target UpdateOptions has a known 20-byte trailing table excluded from decoding.",
            "Target callees remain unnamed except known owner entries; candidate names are exact map VA matches.",
            "Helper topology covers direct owner calls to static/private or Options-named symbols, then one-level helper inventories; indirect calls and tail jumps are not graph edges.",
            "Outline presence or absence alone does not prove correct inlining or semantic equivalence."],
        "inputs": {"image": str(args.image), "image_sha256": candidate.sha256,
                   "map": str(args.map), "map_sha256": hashlib.sha256(args.map.read_bytes()).hexdigest(),
                   "pdb": str(args.pdb), "pdb_sha256": hashlib.sha256(args.pdb.read_bytes()).hexdigest(),
                   "target": str(args.target), "target_sha256": target.sha256},
        "capstone": decoder_identity, "owners": owners,
        "candidate_helpers": [helpers[a] for a in sorted(helpers)],
    }
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    try:
        main()
    except (LinkedImageError, OSError, KeyError, ValueError) as exc:
        sys.exit(f"options topology diagnostic failed: {exc}")
