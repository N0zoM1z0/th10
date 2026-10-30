#!/usr/bin/env python3
"""Verify the complete native hit candidate receipt, with zero exact credit."""

import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM
from linked_image import (
    PEImage, linked_code_fields, linked_functions, map_publics, verify_capstone,
)


def inventory(directory, expected):
    image_path = directory / "source.exe"
    map_path = directory / "source.map"
    pdb_path = directory / "source.pdb"
    image = PEImage(image_path)
    functions = linked_functions(image_path, map_path, pdb_path)["functions"]
    owners = [f for f in functions if f["symbol"] == expected["owner"]["symbol"]]
    assert len(owners) == 1
    owner = owners[0]
    assert owner["map_object"] == expected["owner"]["map_object"]
    assert owner["extent_source"] == "pdb-section-contribution"
    address, size = int(owner["address"], 0), owner["size"]
    code = bytearray(image.read_address(address, size))
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, address))
    assert sum(i.size for i in instructions) == size
    starts = {i.address - address for i in instructions}
    report = linked_code_fields(image, map_publics(map_path), address, size)
    assert report["normalization_complete"]
    identities = []
    for field in report["fields"]:
        offset, width = field["offset"], field["width"]
        code[offset:offset + width] = b"\0" * width
        destination = field["candidate_target"]
        identity = {"offset": offset, "width": width, "type": field["type"]}
        if address <= destination < address + size:
            identity["destination"] = {"local_offset": destination - address}
        else:
            symbols = sorted(s for s in field["candidate_symbols"] if not s.startswith("_th10_"))
            assert symbols
            identity["destination"] = {"symbols": symbols}
        identities.append(identity)
    branches = []
    for instruction in instructions:
        if not instruction.group(CS_GRP_JUMP):
            continue
        assert instruction.operands[0].type == CS_OP_IMM
        destination = instruction.operands[0].imm - address
        assert destination in starts
        branches.append({
            "offset": instruction.address - address,
            "destination": destination,
            "mnemonic": instruction.mnemonic,
        })
    returns = [{"offset": i.address - address, "operand": i.op_str}
               for i in instructions if i.mnemonic == "ret"]
    return {
        "complete_owned_bytes": size,
        "decoded_instructions": len(instructions),
        "normalized_sha256": hashlib.sha256(code).hexdigest(),
        "field_identities": identities,
        "fixed_local_branches": branches,
        "returns": returns,
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
    expected = proof["native_reproduction"]["after"]
    actual = inventory(directory, expected)
    assert all(actual[key] == expected[key] for key in actual), "native receipt differs"
    print(json.dumps({
        "result": "native-candidate-reproduced-nonexact",
        "candidate_size": actual["complete_owned_bytes"],
        "target_size": proof["target_owner_review"]["size"],
        "field_count": len(actual["field_identities"]),
        "fixed_local_branches": len(actual["fixed_local_branches"]),
        "exact_credit": 0,
    }))


if __name__ == "__main__":
    main()
