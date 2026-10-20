#!/usr/bin/env python3
"""Audit TH10 title/main-menu extents, guarded tables and direct call edges."""
from collections import Counter
import csv
import json
from pathlib import Path
import struct
import tomllib
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM
from linked_image import verify_capstone
from target_identity import pe_bytes_at, resolve_target, verify_target

ROOT = Path(__file__).resolve().parents[1]
OWNERS = [(0x42D300,268,288), (0x42D420,1195,1280), (0x42C850,69,69)]
TABLES = [
    (0x42D40C, (0x42D315,0x42D383,0x42D3A0,0x42D404,0x42D3EF),0),
    (0x42D8CC, (0x42D436,0x42D4CA,0x42D516,0x42D8C2,0x42D726),1),
    (0x42D8E0, (0x42D630,)*5+(0x42D673,0x42D6C2,0x42D700),1),
    (0x42D900, (0x42D749,0x42D757,0x42D7CE,0x42D822,0x42D84A,0x42D872,0x42D89A,0x42D8B6),1),
]
CALLS = [
    {0x4491C0:1,0x42C670:2,0x448D00:1,0x42C620:3,0x42C770:1,0x43DC90:1,0x409E50:1,0x42C5C0:1},
    {0x42C850:1,0x4491C0:1,0x42C670:2,0x42C620:6,0x449250:2,0x42C770:8,0x44BEA0:2,0x43DC90:7,0x42C750:3,0x42C6D0:5,0x42C5C0:7,0x44BE20:6,0x40AD20:1},
    {},
]


def require(condition, message):
    if not condition: raise ValueError(message)


def main():
    path=resolve_target(None); identity,problems=verify_target(path)
    require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb') as stream: verify_capstone(tomllib.load(stream)['capstone'])
    raw=path.read_bytes(); decoder=Cs(CS_ARCH_X86,CS_MODE_32); decoder.detail=True
    with (ROOT/'config/function-boundaries.csv').open(newline='') as stream:
        boundaries={int(r['address'],0):r for r in csv.DictReader(stream)}
    instructions=[]
    for (start,code,owned),calls in zip(OWNERS,CALLS):
        ins=list(decoder.disasm(pe_bytes_at(raw,start,code),start)); instructions.append(ins)
        require(sum(i.size for i in ins)==code and ins[-1].mnemonic=='ret','incomplete code extent')
        require(all(i.operands[0].imm in {x.address for x in ins} for i in ins if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM),'external/noninstruction branch')
        require(Counter(i.operands[0].imm for i in ins if i.mnemonic=='call' and i.operands[0].type==X86_OP_IMM)==calls,'call set differs')
        require(all(not i.operands for i in ins if i.mnemonic=='ret'),'unexpected stack cleanup')
        row=boundaries[start]
        require(row['state']=='reviewed' and int(row['span_end'],0)==start+owned-1,'ledger boundary differs')
    for address,destinations,owner in TABLES:
        require(struct.unpack('<'+'I'*len(destinations),pe_bytes_at(raw,address,4*len(destinations)))==destinations,'table differs')
        require(set(destinations)<={i.address for i in instructions[owner]},'noninstruction destination')
    # Each selector is guarded by an unsigned upper bound before its indexed JMP.
    for address,bound,table in [(0x42D304,4,0x42D40C),(0x42D423,4,0x42D8CC),
                                (0x42D620,7,0x42D8E0),(0x42D736,7,0x42D900)]:
        ins=list(decoder.disasm(pe_bytes_at(raw,address,25),address))
        require(ins[0].mnemonic=='cmp' and ins[0].operands[-1].imm==bound and any(i.mnemonic=='ja' for i in ins[1:4]),'selector guard differs')
        require(any(i.mnemonic=='jmp' and i.operands[0].type==X86_OP_MEM and i.operands[0].mem.scale==4 and i.operands[0].mem.disp==table for i in ins),'indexed table operand differs')
    require(pe_bytes_at(raw,0x42D8CB,1)==b'\x90','internal alignment differs')
    require(pe_bytes_at(raw,0x42C895,11)==b'\xcc'*11,'progress-query padding differs')
    for site,destination in [(0x42D065,0x42D300),(0x42D0BD,0x42D420)]:
        call=list(decoder.disasm(pe_bytes_at(raw,site,5),site))
        require(len(call)==1 and call[0].mnemonic=='call' and call[0].operands[0].imm==destination,'dispatcher edge differs')
    offsets=[op.mem.disp for i in instructions[2] for op in i.operands if op.type==X86_OP_MEM]
    require(offsets==list(range(0x1D888,0x1D88E)),'six progress byte offsets differ')
    print(json.dumps({'authority':'target observations only; no exact/runtime credit','target':identity,
        'owners':[dict(entry=hex(a),code_bytes=c,owned_bytes=o) for a,c,o in OWNERS],
        'tables':[dict(address=hex(a),destinations=[hex(x) for x in ds]) for a,ds,_ in TABLES],
        'authored_denominator_correction':105,'profile_clear_bytes':offsets,'result':'passed'},indent=2))


if __name__=='__main__': main()
