#!/usr/bin/env python3
"""Target-only type1 laser lifecycle, request layout and state-table audit."""
from collections import Counter
import csv,json,struct,tomllib
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM
from linked_image import verify_capstone
from target_identity import pe_bytes_at,resolve_target,verify_target
ROOT=Path(__file__).resolve().parents[1]


def require(ok,message):
    if not ok:raise ValueError(message)


def observe():
    path=resolve_target();identity,problems=verify_target(path);require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb')as f:verify_capstone(tomllib.load(f)['capstone'])
    raw=path.read_bytes();d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
    sizes={0x41e5c0:309,0x41e700:816,0x41ea40:173};owners={a:list(d.disasm(pe_bytes_at(raw,a,z),a))for a,z in sizes.items()}
    for a,code in owners.items():
        require(sum(i.size for i in code)==sizes[a],'incomplete code decode')
        starts={i.address for i in code};require(code[-1].mnemonic=='ret','missing final RET')
        require(all(i.operands[0].imm in starts for i in code if i.group(CS_GRP_JUMP)and i.operands[0].type==X86_OP_IMM),'branch leaves owner')
    require([i.op_str for i in owners[0x41e5c0]if i.mnemonic=='ret']==['4'],'initializer stack ABI differs')
    for a in [0x41e700,0x41ea40]:require(all(not i.op_str for i in owners[a]if i.mnemonic=='ret'),'callback stack ABI differs')
    require(Counter(i.op_str for i in owners[0x41e5c0]if i.mnemonic=='call')=={'0x404f30':2,'0x43ee30':2},'initializer call sequence differs')
    require(Counter(i.op_str for i in owners[0x41e700]if i.mnemonic=='call')=={'0x44bc10':1,'0x405410':3,'0x41f800':1,'0x4267f0':1,'dword ptr [edx + 0x18]':1,'0x448db0':1,'0x43dd10':1,'0x43ee30':2},'Update call sequence differs')
    require(Counter(i.op_str for i in owners[0x41ea40]if i.mnemonic=='call')=={'0x44bc10':1,'0x4451c0':2},'Draw call sequence differs')
    obs={i.address:(i.mnemonic,i.op_str)for code in owners.values()for i in code}
    for a,pair in {
        0x41e5c0:('mov','edx, dword ptr [0x47781c]'),0x41e5d0:('lea','ebx, [ebp + 0x424]'),0x41e5d6:('mov','ecx, 0x7e'),
        0x41e5df:('movsx','ecx, word ptr [ebp + 0x464]'),0x41e5e6:('movsx','eax, word ptr [ebp + 0x466]'),
        0x41e5ed:('mov','dword ptr [ebp + 0xc], 3'),0x41e5f4:('mov','edi, dword ptr [ecx*4 + 0x474170]'),
        0x41e601:('lea','esi, [ebp + 0x61c]'),0x41e621:('test','byte ptr [ebp + 0x468], 2'),
        0x41e648:('and','eax, 0xfc63ffff'),0x41e64d:('or','eax, 0x600000'),0x41e665:('lea','esi, [ebp + 0x9c8]'),
        0x41e69d:('and','ecx, 0xfc7fffff'),0x41e6a3:('or','ecx, 0x400000'),
        0x41e6c6:('mov','eax, dword ptr [ebp + 0x43c]'),0x41e6cc:('mov','edx, dword ptr [ebp + 0x450]'),
        0x41e6d7:('mov','dword ptr [ebp + 0x40c], 0x18'),0x41e6e4:('mov','dword ptr [ebp + 0x44], 0x40000000'),
        0x41e70f:('fnstsw','ax'),0x41e73b:('fld','dword ptr [0x476f78]'),0x41e757:('test','byte ptr [ebp + 0x468], 1'),
        0x41e760:('mov','edx, dword ptr [0x477704]'),0x41e766:('mov','eax, dword ptr [edx + 0x10]'),0x41e76d:('add','eax, 0x1068'),
        0x41e78b:('fmul','dword ptr [ebp + 0x430]'),0x41e797:('fmul','dword ptr [ebp + 0x434]'),0x41e7a3:('fmul','dword ptr [ebp + 0x438]'),
        0x41e7c8:('add','eax, -2'),0x41e7cb:('cmp','eax, 3'),0x41e7d4:('jmp','dword ptr [eax*4 + 0x41ea30]'),
        0x41e7de:('cmp','ecx, dword ptr [ebp + 0x454]'),0x41e800:('mov','eax, dword ptr [ebp + 0x458]'),
        0x41e82c:('cmp','eax, dword ptr [ebp + 0x45c]'),0x41e845:('mov','eax, dword ptr [ebp + 0x460]'),
        0x41e8fa:('fmul','dword ptr [0x470b0c]'),0x41e908:('fmul','dword ptr [0x470cc4]'),
        0x41e926:('mov','ecx, dword ptr [0x477834]'),0x41e983:('idiv','ecx'),0x41e989:('mov','eax, dword ptr [0x4776f0]'),
        0x41e9d0:('fdiv','dword ptr [ecx + 0x34]'),0x41e9f9:('fdiv','dword ptr [edx + 0x30]'),
        0x41ea18:('test','ah, 0x44'),0x41ea52:('lea','edi, [esi + 0x61c]'),0x41ea58:('fstp','dword ptr [esi + 0x950]'),
        0x41ea8e:('or','eax, 4'),0x41eaab:('test','ah, 0x44'),0x41eac5:('fstp','dword ptr [esi + 0xcfc]'),
    }.items():require(obs.get(a)==pair,'operand differs at '+hex(a))
    table=struct.unpack('<4I',pe_bytes_at(raw,0x41ea30,16));starts={i.address for i in owners[0x41e700]}
    require(all(a in starts for a in table),'state table leaves code');require(raw.count(struct.pack('<I',0x41ea30))==1,'other table base reference')
    for offset,a in [(4,0x41e5c0),(8,0x41e700),(12,0x41ea40)]:require(struct.unpack('<I',pe_bytes_at(raw,0x46da10+offset,4))[0]==a,'virtual entry differs')
    for a,z in [(0x41e5b2,14),(0x41e6f5,11),(0x41eaed,3)]:require(pe_bytes_at(raw,a,z)==b'\xcc'*z,'neighbor padding differs')
    for a,x in [(0x470b4c,224),(0x470b48,16),(0x470b04,0),(0x470c18,.1),(0x470b0c,.5),(0x470cc0,.800000011920929),(0x470cc4,.3333333432674408)]:require(pe_bytes_at(raw,a,4)==struct.pack('<f',x),'constant differs')
    return {'result':'passed','authority':'target only; no codegen/runtime credit','target_sha256':identity['sha256'],'owners':[{'address':hex(a),'code_size':z,'owned_size':z+(16 if a==0x41e700 else 0)}for a,z in sizes.items()],'state_table':{'address':'0x41ea30','selector_states':[2,3,4,5],'targets':[hex(a)for a in table]},'new_authored_bytes':498}


def main():
    report=observe();rows={int(r['address'],0):r for r in csv.DictReader((ROOT/'config/functions.csv').open())};bounds={int(r['address'],0):r for r in csv.DictReader((ROOT/'config/function-boundaries.csv').open())}
    for f in report['owners']:
        a=int(f['address'],0);z=f['owned_size'];require(int(rows[a]['size'])==z and int(rows[a]['span_end'],0)==a+z-1,'physical owner ledger differs');require(bounds[a]['state']=='reviewed'and int(bounds[a]['span_end'],0)==a+z-1,'boundary ledger differs')
    print(json.dumps(report,indent=2))


if __name__=='__main__':main()
