#!/usr/bin/env python3
"""Audit TH10 MIDI dispatch, compressed-table ownership and timer protocols."""
from collections import Counter
import csv
import json
from pathlib import Path
import struct
import tomllib
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM,X86_OP_MEM
from linked_image import verify_capstone
from target_identity import pe_bytes_at,resolve_target,verify_target

ROOT=Path(__file__).resolve().parents[1]
OWNERS=[(0x43A730,26,26),(0x43ACE0,132,132),(0x43AF30,473,473),
        (0x43B110,1323,1670),(0x43B7A0,140,140),(0x43B8B0,1,1)]
CALLS=[{}, {}, {0x456640:4,0x456B80:2,0x463B2C:2,0x43B7A0:1,0x43B110:1,0x43ACE0:1},
       {0x43AEB0:1,0x452706:2,0x43A730:2,0x4662A0:1,0x46629C:1,
        0x452422:2,0x456640:2,0x456B80:1,0x463B2C:1,0x466298:1},
       {0x463B2C:1,0x466298:1},{}]
TABLES=[
    (0x43B63C,(0x43B38D,0x43B3A6,0x43B178,0x43B287),0x43B64C,113,3,
     {0:0,16:0,32:0,48:0,64:1,80:1,96:0,112:2}),
    (0x43B6C0,(0x43B3EE,0x43B3BB,0x43B431,0x43B41F,0x43B5D8),0x43B6D4,65,4,
     {0:0,16:1,48:2,64:3}),
    (0x43B718,(0x43B44B,0x43B502,0x43B56E,0x43B461,0x43B4EC,0x43B4C0,0x43B4D6,0x43B5D8),0x43B738,94,7,
     {0:0,2:1,4:2,7:3,10:4,91:5,93:6}),
]


def require(condition,message):
    if not condition:raise ValueError(message)


def main():
    path=resolve_target(None);identity,problems=verify_target(path);require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb')as f:verify_capstone(tomllib.load(f)['capstone'])
    raw=path.read_bytes();decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    with (ROOT/'config/function-boundaries.csv').open(newline='')as f:
        rows={int(r['address'],0):r for r in csv.DictReader(f)}
    decoded=[]
    for (entry,code,owned),expected in zip(OWNERS,CALLS):
        ins=list(decoder.disasm(pe_bytes_at(raw,entry,code),entry));decoded.append(ins)
        require(sum(i.size for i in ins)==code and ins[-1].mnemonic=='ret','incomplete executable extent')
        actual=Counter((i.operands[0].imm if i.operands[0].type==X86_OP_IMM else i.operands[0].mem.disp)
                       for i in ins if i.mnemonic=='call')
        require(actual==expected,'call inventory differs')
        starts={i.address for i in ins}
        require(all(i.operands[0].imm in starts for i in ins if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM),'external/noninstruction branch')
        require(rows[entry]['state']=='reviewed' and int(rows[entry]['span_end'],0)==entry+owned-1,'ledger extent differs')
        returns=[i for i in ins if i.mnemonic=='ret']
        cleanup=8 if entry==0x43B110 else 4 if entry==0x43B7A0 else 0
        require(all((i.operands[0].imm if i.operands else 0)==cleanup for i in returns),'stack cleanup differs')
    starts={i.address for i in decoded[3]}
    for table,destinations,index_table,count,default,overrides in TABLES:
        require(struct.unpack('<'+'I'*len(destinations),pe_bytes_at(raw,table,4*len(destinations)))==destinations,'pointer table differs')
        require(set(destinations)<=starts,'table leaves owner code')
        expected=bytes(overrides.get(i,default)for i in range(count))
        require(pe_bytes_at(raw,index_table,count)==expected,'complete compressed index differs')
    for address,bound,index_table,pointer_table in [(0x43B159,112,0x43B64C,0x43B63C),
            (0x43B28B,64,0x43B6D4,0x43B6C0),(0x43B434,93,0x43B738,0x43B718)]:
        ins=list(decoder.disasm(pe_bytes_at(raw,address,32),address))
        require(ins[0].mnemonic=='cmp' and ins[0].operands[-1].imm==bound,'guard bound differs')
        require(any(i.mnemonic=='ja'for i in ins[1:4]),'unsigned guard missing')
        require(any(i.mnemonic=='movzx' and i.operands[-1].type==X86_OP_MEM and i.operands[-1].mem.disp==index_table for i in ins),'compressed index operand missing')
        require(any(i.mnemonic=='jmp' and i.operands[0].type==X86_OP_MEM and i.operands[0].mem.disp==pointer_table for i in ins),'pointer-table operand missing')
    require(pe_bytes_at(raw,0x43B63B,1)==b'\x90' and pe_bytes_at(raw,0x43B6BD,3)==b'\x8d\x49\x00' and pe_bytes_at(raw,0x43B715,3)==b'\x8d\x49\x00','owned alignment differs')
    require(pe_bytes_at(raw,0x43B796,10)==b'\xcc'*10 and pe_bytes_at(raw,0x43B8B1,15)==b'\xcc'*15,'adjacent padding differs')
    require(struct.unpack('<II',pe_bytes_at(raw,0x46F80C,8))==(0x43AF30,0x43B8B0),'derived/base timer vtable slots differ')
    thunk=list(decoder.disasm(pe_bytes_at(raw,0x43A6E0,11),0x43A6E0))
    require([i.mnemonic for i in thunk]==['mov','mov','call','ret'] and thunk[-1].operands[0].imm==20,'timer virtual trampoline differs')
    multiply=next(i for i in decoded[3]if i.address==0x43B368)
    require(multiply.mnemonic=='imul' and multiply.operands[-1].imm==0x101,'target tempo recurrence differs')
    print(json.dumps({'authority':'target observations only; no source/exact/runtime grant','target':identity,
        'owners':[dict(entry=hex(a),code_bytes=c,owned_bytes=o)for a,c,o in OWNERS],
        'parser_owned_data_and_alignment':347,'tempo_multiplier':257,
        'compressed_tables':[dict(pointer_table=hex(a),index_table=hex(b),entries=n)for a,_,b,n,_,_ in TABLES],
        'timer_trampoline':'0x43A6E0','derived_slot':'0x46F80C','base_slot':'0x46F810','result':'passed'},indent=2))


if __name__=='__main__':main()
