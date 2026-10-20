#!/usr/bin/env python3
"""Target-only review of result text owners, callback and compressed switch."""
from collections import Counter
import csv, json, struct, tomllib
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM
from linked_image import verify_capstone
from target_identity import pe_bytes_at, resolve_target, verify_target
ROOT = Path(__file__).resolve().parents[1]


def require(condition, message):
    if not condition: raise ValueError(message)


def main():
    path=resolve_target(); identity,problems=verify_target(path)
    require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb') as stream:
        verify_capstone(tomllib.load(stream)['capstone'])
    raw=path.read_bytes(); decoder=Cs(CS_ARCH_X86,CS_MODE_32); decoder.detail=True
    def decode(address,size):
        result=list(decoder.disasm(pe_bytes_at(raw,address,size),address))
        require(sum(i.size for i in result)==size,'incomplete decode')
        return result
    name=decode(0x4224C0,404); rows=decode(0x422660,1035); callback=decode(0x422AA0,7)
    for code,calls,pop in [(name,{0x401630:3},16),(rows,{0x401630:6,0x4224C0:2,0x452BAA:3},4)]:
        require(Counter(i.operands[0].imm for i in code if i.mnemonic=='call' and i.operands[0].type==X86_OP_IMM)==calls,'call set differs')
        require(all(i.operands[0].imm==pop for i in code if i.mnemonic=='ret'),'stack cleanup differs')
        starts={i.address for i in code}
        require(all(i.operands[0].imm in starts for i in code if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM),'branch leaves complete owner')
    require([(i.mnemonic,i.op_str) for i in callback]==[('push','ecx'),('call','0x422660'),('ret','')],'draw callback differs')
    require([(i.mnemonic,i.op_str) for i in decode(0x42217A,5)]==[('push','0x422aa0')],'draw callback registration differs')
    require(pe_bytes_at(raw,0x422654,12)==b'\xcc'*12 and pe_bytes_at(raw,0x422A86,10)==b'\xcc'*10 and pe_bytes_at(raw,0x422AA7,9)==b'\xcc'*9,'neighbor padding differs')
    require(pe_bytes_at(raw,0x422A6B,1)==b'\x90','tail alignment differs')
    pointers=struct.unpack('<4I',pe_bytes_at(raw,0x422A6C,16))
    require(pointers==(0x422698,0x42278D,0x422883,0x422A52),'pointer table differs')
    indices=pe_bytes_at(raw,0x422A7C,10)
    require(list(indices)==[0,1,2,3,3,3,3,0,1,2],'compressed selector differs')
    require(set(pointers)<={i.address for i in rows},'table destination is not local code')
    guard=decode(0x42267C,28)
    require([(i.mnemonic,i.op_str) for i in guard[:2]]==[('add','eax, -0xa'),('cmp','eax, 9')],'phase guard differs')
    require(any(i.mnemonic=='ja' and i.operands[0].imm==0x422A52 for i in guard),'unsigned upper guard differs')
    require(any(i.mnemonic=='movzx' and i.operands[-1].type==X86_OP_MEM and i.operands[-1].mem.disp==0x422A7C for i in guard),'selector read differs')
    require(any(i.mnemonic=='jmp' and i.operands[0].type==X86_OP_MEM and i.operands[0].mem.disp==0x422A6C for i in guard),'indexed jump differs')
    observed={i.address:(i.mnemonic,i.op_str) for i in rows}
    for a,pair in {
        0x42266F:('mov','dword ptr [ecx + 0x8988], 1'),
        0x422A55:('mov','dword ptr [ecx + 0x8988], 0'),
        0x4226E5:('mov','esi, dword ptr [eax + 0x18]'),
        0x422803:('mov','edx, dword ptr [0x477838]'),
        0x42281D:('mov','ecx, dword ptr [0x474c7c]'),
        0x422990:('mov','ecx, dword ptr [0x474c68]'),
        0x422996:('mov','edx, dword ptr [0x474c6c]'),
        0x42299C:('mov','ebp, dword ptr [0x47783c]'),
        0x4229A7:('mov','ecx, dword ptr [0x474c74]'),
        0x4229C1:('movsx','edx, byte ptr [ecx + 0x1c]'),
        0x4229E2:('movsx','eax, byte ptr [ecx + 0x1d]'),
    }.items(): require(observed.get(a)==pair,'sampling operand differs')
    for a,value in [(0x46E19C,b'No.%.2d %s %.2d/%.2d/%.2d %s %s %s\0'),
                    (0x46E148,b'No.%.2d          %.2d/%.2d/%.2d %s %s %s\0'),
                    (0x46E1C8,b'%s\0'),(0x46E1C4,b'_\0'),(0x46E1C0,b'%c\0')]:
        require(pe_bytes_at(raw,a,len(value))==value,'format identity differs')
    for a,value in [(0x470C10,9.),(0x470B48,16.),(0x470C0C,18.)]:
        require(pe_bytes_at(raw,a,4)==struct.pack('<f',value),'name-entry constant differs')
    with (ROOT/'config/function-boundaries.csv').open(newline='') as stream:
        ledger={int(r['address'],0):r for r in csv.DictReader(stream)}
    for a,size in [(0x4224C0,404),(0x422660,1062),(0x422AA0,7)]:
        require(ledger[a]['state']=='reviewed' and int(ledger[a]['span_end'],0)==a+size-1,'physical ledger differs')
    print(json.dumps({'result':'passed','authority':'target observations only; no codegen/runtime credit','target':identity,
        'owners':[{'address':'0x4224c0','owned_bytes':404},{'address':'0x422660','code_bytes':1035,'owned_bytes':1062,'ghidra_body_bytes':1026},{'address':'0x422aa0','owned_bytes':7,'origin':'indeterminate'}],
        'authored_denominator_correction':27,'pointer_table':[hex(a) for a in pointers],'selector':list(indices)},indent=2))


if __name__=='__main__': main()
