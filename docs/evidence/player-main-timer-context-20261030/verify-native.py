#!/usr/bin/env python3
"""Reproduce the complete nonexact Player main receipt, including its table."""

import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM, CS_OP_MEM
from linked_image import (
    PEImage, linked_code_fields, linked_functions, map_publics, verify_capstone,
)


def inventory(directory):
    image_path = directory / "source.exe"
    map_path = directory / "source.map"
    image = PEImage(image_path)
    owners = [f for f in linked_functions(
        image_path, map_path, directory / "source.pdb")["functions"]
        if f["symbol"] == "?PlayerUpdateCallbackBody@@YGHPAUPlayer@@@Z"]
    assert len(owners) == 1
    owner = owners[0]
    assert owner["extent_source"] == "pdb-section-contribution"
    assert owner["map_object"] == "support-06-Player.ltcg.obj"
    address, size = int(owner["address"], 0), owner["size"]
    raw = image.read_address(address, size)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    prefix = list(decoder.disasm(raw, address))
    switches = [i for i in prefix if i.mnemonic == "jmp"
                and i.operands[0].type == CS_OP_MEM
                and i.operands[0].mem.index != 0]
    assert len(switches) == 1
    table_address = switches[0].operands[0].mem.disp
    table_offset = table_address - address
    assert 0 < table_offset < size and size - table_offset == 20
    instructions = list(decoder.disasm(raw[:table_offset], address))
    assert sum(i.size for i in instructions) == table_offset
    starts = {i.address - address for i in instructions}
    table = [int.from_bytes(raw[p:p + 4], "little") - address
             for p in range(table_offset, size, 4)]
    assert all(offset in starts for offset in table)
    report = linked_code_fields(image, map_publics(map_path), address, size)
    assert report["normalization_complete"]
    normalized = bytearray(raw)
    identities = []
    for field in report["fields"]:
        offset, width = field["offset"], field["width"]
        normalized[offset:offset + width] = b"\0" * width
        destination = field["candidate_target"]
        identity = {"offset": offset, "width": width, "type": field["type"]}
        if address <= destination < address + size:
            identity["destination"] = {"local_offset": destination - address}
        else:
            symbols = sorted(s for s in field["candidate_symbols"]
                             if not s.startswith("_th10_"))
            assert symbols
            identity["destination"] = {"symbols": symbols}
        identities.append(identity)
    branches = []
    for instruction in instructions:
        if not instruction.group(CS_GRP_JUMP):
            continue
        if instruction.operands[0].type != CS_OP_IMM:
            assert instruction.address == switches[0].address
            continue
        destination = instruction.operands[0].imm - address
        assert destination in starts
        branches.append({"offset": instruction.address - address,
                         "destination": destination,
                         "mnemonic": instruction.mnemonic})
    return {
        "complete_owned_bytes": size,
        "code_and_alignment_bytes": table_offset,
        "decoded_instructions": len(instructions),
        "normalized_sha256": hashlib.sha256(normalized).hexdigest(),
        "field_identities": identities,
        "fixed_local_branches": branches,
        "table": {"offset": table_offset, "entry_offsets": table},
        "returns": [{"offset": i.address - address, "operand": i.op_str}
                    for i in instructions if i.mnemonic == "ret"],
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--artifact-dir", type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / "build")
    proof = json.loads(Path(__file__).with_name("results.json").read_text())
    assert hashlib.sha256((ROOT / "resources/th10.exe").read_bytes()).hexdigest() == proof["target_sha256"]
    verify_capstone(tomllib.loads((ROOT / "config/tools.lock.toml").read_text())["capstone"])
    target = PEImage(ROOT / "resources/th10.exe")
    operand = proof["readonly_operand"]
    operand_address = int(operand["address"], 0)
    assert int.from_bytes(target.read_address(operand_address, 4), "little") == int(operand["word"], 0)
    section = next(s for s in target.sections if
                   target.image_base + s.rva <= operand_address < target.image_base + s.rva + s.virtual_size)
    assert section.name == operand["section"] and section.characteristics == int(operand["section_flags"], 0)
    assert not section.characteristics & 0x80000000
    actual = inventory(directory)
    assert actual == proof["native_reproduction"]["after"], "native receipt differs"
    print(json.dumps({"result": "native-candidate-reproduced-nonexact",
                      "candidate_size": actual["complete_owned_bytes"],
                      "target_size": 3112,
                      "field_count": len(actual["field_identities"]),
                      "fixed_local_branches": len(actual["fixed_local_branches"]),
                      "table_entries": len(actual["table"]["entry_offsets"]),
                      "exact_credit": 0}))


if __name__ == "__main__":
    main()
