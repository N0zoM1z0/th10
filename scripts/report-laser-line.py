#!/usr/bin/env python3
"""Replay target observations for laser line owners, vtable and omitted parser."""
from collections import Counter
import csv,json,struct,tomllib
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM,X86_OP_MEM
from linked_image import verify_capstone
from target_identity import pe_bytes_at,resolve_target,verify_target
ROOT=Path(__file__).resolve().parents[1]


def require(condition,message):
    if not condition:raise ValueError(message)


def main():
    path=resolve_target();identity,problems=verify_target(path);require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb')as f:verify_capstone(tomllib.load(f)['capstone'])
    raw=path.read_bytes();d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
    def decode(a,z):
        code=list(d.disasm(pe_bytes_at(raw,a,z),a));require(sum(i.size for i in code)==z,'incomplete decode');return code
    owners={a:decode(a,z)for a,z in [(0x41CA80,1269),(0x41D2C0,269),(0x41D3D0,1001),(0x41D7C0,173),(0x4267F0,471)]}
    for a,code in owners.items():
        starts={i.address for i in code};require(code[-1].mnemonic=='ret','owner does not end in RET')
        require(all(i.operands[0].imm in starts for i in code if i.group(CS_GRP_JUMP)and i.operands[0].type==X86_OP_IMM),'branch leaves complete owner')
    vtable=struct.unpack('<19I',pe_bytes_at(raw,0x46DA60,76))
    require(vtable==(0x41CA80,0x41C8C0,0x41D3D0,0x41D7C0,0x41D870,0x41E260,0x41D880,0x41DD80,0x41E4D0,0x41BF90,0x41D2C0,0x41BFB0,0x41D170,0x41BFD0,0x41BFE0,0x41CFD0,0x41C000,0x41C010,0x41C020),'known-slot interface differs')
    update=owners[0x41D3D0];calls=[i for i in update if i.mnemonic=='call']
    require([i.operands[0].mem.disp for i in calls if i.operands[0].type==X86_OP_MEM]==[0,0x24,0x28,0x2C,0x30,0x34,0x38,0x3C,0x40,0x44,0x48,0x18],'virtual order differs')
    require(Counter(i.operands[0].imm for i in calls if i.operands[0].type==X86_OP_IMM)=={0x44BF40:2,0x41F800:2,0x41F7A0:2,0x4267F0:1,0x448DB0:1,0x43DD10:1,0x43EE30:2},'update direct boundaries differ')
    observed={i.address:(i.mnemonic,i.op_str)for code in owners.values()for i in code}
    for a,pair in {
        0x41CA84:('cmp','dword ptr [ebp + 0x400], 0x12'),
        0x41CAA6:('lea','eax, [ecx + ecx*2 + 0x8a]'),
        0x41CAAD:('mov','edx, dword ptr [ebp + eax*8 + 0x10]'),
        0x41CABF:('cmp','dword ptr [esi + 0x14], ebx'),
        0x41CAC4:('cmp','dword ptr [ebp + 0x404], ebx'),
        0x41CB08:('dec','eax'),0x41CB09:('cmp','eax, 0x3f'),
        0x41CB0C:('ja','0x41cf50'),
        0x41CB12:('movzx','ecx, byte ptr [eax + 0x41cf8c]'),
        0x41CB19:('jmp','dword ptr [ecx*4 + 0x41cf78]'),
        0x41D44E:('test','dword ptr [ebp + 0x404], 0x8000c00'),
        0x41D6C8:('mov','edx, dword ptr [0x477834]'),
        0x41D707:('mov','ecx, 5'),0x41D70C:('idiv','ecx'),
        0x41D712:('mov','eax, dword ptr [0x4776f0]'),
        0x41D717:('mov','ecx, dword ptr [eax + 0x3e0b50]'),
        0x41D725:('push','0x1b2'),
        0x41D73B:('mov','edx, dword ptr [ebp + 0x24]'),
        0x41D73F:('mov','ebx, 0x1c'),0x41D744:('mov','esi, 0x492590'),
        0x41D759:('fdiv','dword ptr [ecx + 0x34]'),
        0x41D782:('fdiv','dword ptr [edx + 0x30]'),
        0x41D799:('fld','dword ptr [ebp + 0x4c]'),
        0x41D7D8:('fstp','dword ptr [esi + 0x934]'),
        0x41D7E7:('fstp','dword ptr [esi + 0x938]'),
        0x41D7F0:('mov','dword ptr [esi + 0x93c], eax'),
        0x41D833:('mov','ecx, dword ptr [0x491c10]'),
        0x41D845:('fstp','dword ptr [esi + 0xce0]'),
        0x426950:('mov','eax, dword ptr [ecx + 0x4310]'),
    }.items():require(observed.get(a)==pair,'operand differs at '+hex(a))
    pointers=struct.unpack('<5I',pe_bytes_at(raw,0x41CF78,20));require(pointers==(0x41CB20,0x41CB47,0x41CBF3,0x41CC80,0x41CF50),'parser pointers differ')
    indices=list(pe_bytes_at(raw,0x41CF8C,64));expected=[4]*64
    for k,v in [(0,0),(15,1),(31,2),(63,3)]:expected[k]=v
    require(indices==expected and set(pointers)<={i.address for i in owners[0x41CA80]},'parser selector domain differs')
    require([(i.mnemonic,i.op_str)for i in decode(0x41CA96,10)]==[('lea','esp, [esp]'),('lea','ecx, [ecx]')],'parser internal alignment differs')
    require([(i.mnemonic,i.op_str)for i in decode(0x41CF75,3)]==[('lea','ecx, [ecx]')],'parser tail alignment differs')
    for a,z in [(0x41CA7A,6),(0x41CFCC,4),(0x41D3CD,3),(0x41D7B9,7),(0x41D86D,3),(0x4269C7,9)]:require(pe_bytes_at(raw,a,z)==b'\xcc'*z,'neighbor padding differs')
    polar=decode(0x41F800,30)
    require([i.op_str for i in polar if i.mnemonic=='fstp']==['dword ptr [eax]','dword ptr [eax + 4]'],'polar output shape differs')
    player=owners[0x4267F0]
    require([i.operands[0].imm for i in player if i.mnemonic=='call']==[0x426CF0],'Player hit boundary differs')
    require(all(i.op_str=='0xc'for i in player if i.mnemonic=='ret'),'Player stack cleanup differs')
    require([(i.mnemonic,i.op_str)for i in decode(0x42695A,6)]==[('push','ecx'),('call','0x426cf0')]and decode(0x426CF0,627)[-1].op_str=='4','Player hit stack boundary differs')
    require([i.operands[0].imm for i in owners[0x41D2C0]if i.mnemonic=='call']==[0x463B2C],'motion conversion boundary differs')
    for a,x in [(0x470B40,-192.),(0x470B3C,192.),(0x470B04,0.),(0x470B38,448.),(0x470B4C,224.),(0x470B48,16.),(0x470C58,.0001),(0x470B68,.99),(0x470B64,1.01),(0x470AFC,1.),(0x470CA8,1.5),(0x470CA4,-1.5)]:require(pe_bytes_at(raw,a,4)==struct.pack('<f',x),'constant identity differs')
    with (ROOT/'config/function-boundaries.csv').open(newline='')as f:ledger={int(r['address'],0):r for r in csv.DictReader(f)}
    for a,z in [(0x41CA80,1356),(0x41D2C0,269),(0x41D3D0,1001),(0x41D7C0,173),(0x4267F0,471)]:require(ledger[a]['state']=='reviewed'and int(ledger[a]['span_end'],0)==a+z-1,'physical ledger differs')
    print(json.dumps({'result':'passed','authority':'target observations only; no codegen/runtime credit','target':identity,'parser':{'address':'0x41CA80','ghidra_body_bytes':1259,'code_span_bytes':1269,'internal_alignment_bytes':10,'tail_alignment_bytes':3,'pointer_bytes':20,'selector_bytes':64,'owned_bytes':1356,'source':'absent'},'vtable':[hex(a)for a in vtable],'mapped_authored_source_bytes':1645,'mapped_indeterminate_source_bytes':269,'polar_Z_prefill':'unknown'},indent=2))


if __name__=='__main__':main()
