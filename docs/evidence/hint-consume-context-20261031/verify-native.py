#!/usr/bin/env python3
"""Reproduce the complete nonexact Hint consumption owner; admit no match."""

import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM, CS_OP_MEM
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, verify_capstone

ADDRESS, SIZE = 0x419120, 1317
SYMBOL = "?ConsumeEntries@HintOwnerView@@QAEHPAUHintLinkView@@H@Z"


def decode(image, address, size):
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, size), address))
    assert sum(i.size for i in instructions) == size
    return instructions


def target_fields(image, instructions):
    """Enumerate independently: the original PE has no relocation directory."""
    def mapped(value):
        return any(image.image_base + s.rva <= value <
                   image.image_base + s.rva + s.virtual_size for s in image.sections)
    fields = []
    for i in instructions:
        offset = i.address - ADDRESS
        if i.group(CS_GRP_JUMP):
            assert i.operands[0].type == CS_OP_IMM
            assert ADDRESS <= i.operands[0].imm < ADDRESS + SIZE
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


def geometry(instructions, address):
    starts = {i.address for i in instructions}
    branches, returns = [], []
    for i in instructions:
        if i.group(CS_GRP_JUMP):
            assert i.operands[0].type == CS_OP_IMM and i.operands[0].imm in starts
            branches.append(dict(offset=i.address - address, mnemonic=i.mnemonic,
                                 destination=i.operands[0].imm - address))
        if i.mnemonic == "ret":
            returns.append(dict(offset=i.address - address, operand=i.op_str))
    return dict(instructions=len(instructions), local_branches=branches, returns=returns)


def observations(directory, roles):
    verify_capstone(tomllib.loads((ROOT / "config/tools.lock.toml").read_text())["capstone"])
    target = PEImage(ROOT / "resources/th10.exe")
    assert target.sha256 == "2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040"
    image = PEImage(directory / "source.exe")
    functions = linked_functions(directory / "source.exe", directory / "source.map",
                                 directory / "source.pdb")["functions"]
    owners = [f for f in functions if f["symbol"] == SYMBOL]
    assert len(owners) == 1
    owner = owners[0]
    assert owner["map_object"] == "source.ltcg.obj"
    assert owner["extent_source"] == "pdb-section-contribution" and owner["size"] == 1308
    address, size = int(owner["address"], 0), owner["size"]
    publics = map_publics(directory / "source.map")
    names = {p["symbol"]: p["address"] for p in publics}
    ti, ni = decode(target, ADDRESS, SIZE), decode(image, address, size)
    tf = target_fields(target, ti)
    nf = linked_code_fields(image, publics, address, size)
    assert nf["normalization_complete"] and len(tf) == len(nf["fields"]) == len(roles) == 34
    tc, nc = bytearray(target.read_address(ADDRESS, SIZE)), bytearray(image.read_address(address, size))
    bindings, callees = [], []
    for t, n, role in zip(tf, nf["fields"], roles):
        assert t == {k: role[k] for k in ("offset", "width", "type", "target")}
        assert n["type"] == t["type"] and n["width"] == 4
        symbol = role["symbol"]
        assert n["candidate_target"] == names[symbol]
        if symbol.startswith("__real@"):
            assert image.read_address(names[symbol], 4) == target.read_address(t["target"], 4)
        if t["type"] == "REL32" and symbol not in ("__ftol2", "??3@YAXPAX@Z"):
            found = [f for f in functions if f["symbol"] == symbol
                     and int(f["address"], 0) == names[symbol] and f["size"] > 0]
            assert len(found) == 1
            callees.append(dict(symbol=symbol, size=found[0]["size"],
                                extent_source=found[0]["extent_source"]))
        tc[t["offset"]:t["offset"] + 4] = bytes(4)
        nc[n["offset"]:n["offset"] + 4] = bytes(4)
        bindings.append({**role, "native_offset": n["offset"]})
    # Keep the complete local destination inventories. For the three bounded
    # matching regions only, also prove each relative operand's real destination
    # under the explicit correspondence; never accept a blanket branch mask.
    regions = []
    for ts, te, ns in ((0, 0x10D, 0), (0x1F6, 0x28B, 0x1ED), (0x2A2, SIZE, 0x299)):
        tr = [i for i in ti if ts <= i.address - ADDRESS < te]
        nr = [i for i in ni if ns <= i.address - address < ns + te - ts]
        assert len(tr) == len(nr)
        tb, nb = bytearray(tc[ts:te]), bytearray(nc[ns:ns + te - ts])
        destinations = []
        for t, n in zip(tr, nr):
            assert t.address - ADDRESS - ts == n.address - address - ns and t.size == n.size
            if not t.group(CS_GRP_JUMP):
                continue
            assert n.group(CS_GRP_JUMP) and t.mnemonic == n.mnemonic
            td, nd = t.operands[0].imm - ADDRESS, n.operands[0].imm - address
            expected = 0x1BF if td == 0x1C8 else (td - 9 if td >= 0x1F6 else td)
            assert nd == expected, (hex(t.address - ADDRESS), hex(td), hex(nd), hex(expected))
            destinations.append(dict(target=td, native=nd))
            to, no = t.address - ADDRESS - ts + t.imm_offset, n.address - address - ns + n.imm_offset
            assert t.imm_size == n.imm_size
            tb[to:to + t.imm_size] = bytes(t.imm_size)
            nb[no:no + n.imm_size] = bytes(n.imm_size)
        assert tb == nb
        regions.append(dict(target_start=ts, native_start=ns, size=te - ts,
                            normalized_sha256=hashlib.sha256(tb).hexdigest(),
                            bound_destinations=destinations))
    tg, ng = geometry(ti, ADDRESS), geometry(ni, address)
    assert len(tg["local_branches"]) == len(ng["local_branches"]) == 42
    assert tg["returns"] == [dict(offset=0x522, operand="8")]
    assert ng["returns"] == [dict(offset=0x519, operand="8")]
    assert ni[0].mnemonic == "sub" and ni[0].op_str == "esp, 0x3c"
    assert tc != nc
    return dict(result="complete-native-candidate-nonexact", target_size=SIZE,
                native_size=size, target_geometry=tg, native_geometry=ng,
                target_normalized_sha256=hashlib.sha256(tc).hexdigest(),
                native_normalized_sha256=hashlib.sha256(nc).hexdigest(),
                field_bindings=bindings, actual_authored_callees=callees,
                bound_regions=regions, exact_credit=0)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--artifact-dir", required=True, type=Path)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / "build")
    proof = json.loads(Path(__file__).with_name("results.json").read_text())
    actual = observations(directory, proof["target_field_roles"])
    assert actual == proof["native_reproduction"]
    print(json.dumps(dict(result=actual["result"], native_size=actual["native_size"],
                          target_size=SIZE, fields=34, local_destinations=42, exact_credit=0)))


if __name__ == "__main__":
    main()
