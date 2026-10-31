#!/usr/bin/env python3
"""Verify complete Hint frame evidence and the exact registered callback."""

import argparse
import copy
import hashlib
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM, CS_OP_MEM
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, verify_capstone

ADDRESS, SIZE = 0x419650, 571
SYMBOL = "?Update@HintOwnerView@@QAEHXZ"


def decode(image, address, size):
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, size), address))
    assert sum(i.size for i in instructions) == size
    return instructions


def geometry(instructions, address):
    starts = {i.address for i in instructions}
    branches = []
    for i in instructions:
        if i.group(CS_GRP_JUMP):
            assert i.operands[0].type == CS_OP_IMM and i.operands[0].imm in starts
            branches.append(dict(offset=i.address - address, mnemonic=i.mnemonic,
                                 destination=i.operands[0].imm - address))
    return dict(instructions=len(instructions), branches=branches,
                returns=[dict(offset=i.address - address, operand=i.op_str)
                         for i in instructions if i.mnemonic == "ret"])


def enumerate_target_fields(image, instructions):
    def mapped(value):
        return any(image.image_base + s.rva <= value <
                   image.image_base + s.rva + s.virtual_size for s in image.sections)
    fields = []
    for i in instructions:
        offset = i.address - ADDRESS
        if i.group(CS_GRP_JUMP):
            continue
        if i.mnemonic == "call":
            assert i.operands[0].type == CS_OP_IMM and i.imm_size == 4
            fields.append(dict(offset=offset + i.imm_offset, width=4,
                               type="REL32", target=i.operands[0].imm))
        elif i.disp_size == 4:
            operands = [o for o in i.operands if o.type == CS_OP_MEM
                        and o.mem.base == 0 and mapped(o.mem.disp)]
            if operands:
                assert len(operands) == 1
                fields.append(dict(offset=offset + i.disp_offset, width=4,
                                   type="DIR32", target=operands[0].mem.disp))
        elif i.imm_size == 4:
            operands = [o for o in i.operands if o.type == CS_OP_IMM and mapped(o.imm)]
            if operands:
                assert len(operands) == 1
                fields.append(dict(offset=offset + i.imm_offset, width=4,
                                   type="DIR32", target=operands[0].imm))
    return fields


def observations(directory, proof):
    verify_capstone(tomllib.loads((ROOT / "config/tools.lock.toml").read_text())["capstone"])
    target = PEImage(ROOT / "resources/th10.exe")
    assert target.sha256 == proof["target_sha256"]
    image = PEImage(directory / "source.exe")
    functions = linked_functions(directory / "source.exe", directory / "source.map",
                                 directory / "source.pdb")["functions"]
    owners = [f for f in functions if f["symbol"] == SYMBOL]
    assert len(owners) == 1
    owner = owners[0]
    assert owner["map_object"] == "source.ltcg.obj"
    assert owner["extent_source"] == "pdb-section-contribution" and owner["size"] == SIZE
    address = int(owner["address"], 0)
    publics = map_publics(directory / "source.map")
    names = {p["symbol"]: p["address"] for p in publics}
    ti, ni = decode(target, ADDRESS, SIZE), decode(image, address, SIZE)
    fields = linked_code_fields(image, publics, address, SIZE)
    tf, roles = enumerate_target_fields(target, ti), proof["target_field_roles"]
    assert fields["normalization_complete"] and len(tf) == len(fields["fields"]) == len(roles) == 17
    tc, nc = bytearray(target.read_address(ADDRESS, SIZE)), bytearray(image.read_address(address, SIZE))
    bindings = []
    for t, n, r in zip(tf, fields["fields"], roles):
        assert t == {k: r[k] for k in ("offset", "width", "type", "target")}
        assert n["width"] == 4 and n["type"] == t["type"]
        assert n["candidate_target"] == names[r["symbol"]]
        if r["symbol"].startswith("__real@"):
            assert image.read_address(names[r["symbol"]], 4) == target.read_address(r["target"], 4)
        if r["type"] == "REL32" and r["symbol"] != "__ftol2":
            found = [f for f in functions if f["symbol"] == r["symbol"]
                     and int(f["address"], 0) == names[r["symbol"]]]
            assert len(found) == 1 and found[0]["size"] == 1308
        tc[t["offset"]:t["offset"] + 4] = bytes(4)
        nc[n["offset"]:n["offset"] + 4] = bytes(4)
        bindings.append({**r, "native_offset": n["offset"]})
    regions = []
    for start, end in ((0, 0x3C), (0x4E, 0x62), (0xE2, SIZE)):
        # Local branch operands remain literal: their actual destinations must
        # also match. Only independently bound external fields are normalized.
        assert tc[start:end] == nc[start:end]
        regions.append(dict(start=start, size=end - start,
                            normalized_sha256=hashlib.sha256(tc[start:end]).hexdigest()))
    assert tc != nc
    assert geometry(ni, address)["returns"] == [dict(offset=570, operand="")]
    manifest = tomllib.loads((ROOT / "config/match-units.toml").read_text())
    unit = copy.deepcopy(proof["callback_unit"])
    unit["output_dir"] = str(directory.relative_to(ROOT))
    manifest["units"]["hint-update-callback"] = unit
    compare = runpy.run_path(str(ROOT / "scripts/compare-linked-function.py"))["compare_unit"]
    callback = compare("hint-update-callback", _unit_manifest=manifest)
    assert callback["result"] == "exact" and not callback["first_differences"]
    assert callback["size"] == 10 and len(callback["linkages"]) == 1
    assert any(f["symbol"] == SYMBOL and int(f["address"], 0) ==
               int(callback["linkages"][0]["candidate_target"], 0) for f in functions)
    return dict(result="complete-frame-nonexact-callback-exact", size=SIZE,
                target_geometry=geometry(ti, ADDRESS), native_geometry=geometry(ni, address),
                field_bindings=bindings, bound_regions=regions,
                target_normalized_sha256=hashlib.sha256(tc).hexdigest(),
                native_normalized_sha256=hashlib.sha256(nc).hexdigest(),
                callback=dict(result="exact", size=10, fields=1),
                frame_exact_credit=0, authored_exact_credit=0, callback_codegen_credit=10)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--artifact-dir", required=True, type=Path)
    directory = parser.parse_args().artifact_dir.resolve()
    directory.relative_to(ROOT / "build")
    proof = json.loads(Path(__file__).with_name("results.json").read_text())
    actual = observations(directory, proof)
    assert actual == proof["native_reproduction"]
    print(json.dumps(dict(result=actual["result"], frame_size=SIZE, frame_fields=17,
                          callback_exact_bytes=10, authored_exact_credit=0)))


if __name__ == "__main__":
    main()
