#!/usr/bin/env python3
"""Target-only review of laser Clear and its real Item Spawn interface."""
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


def main():
    path=resolve_target();identity,problems=verify_target(path);require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb')as f:verify_capstone(tomllib.load(f)['capstone'])
    raw=path.read_bytes();d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
    code=list(d.disasm(pe_bytes_at(raw,0x41e260,609),0x41e260));require(sum(i.size for i in code)==609,'incomplete owner decode')
    starts={i.address for i in code};require(all(i.operands[0].imm in starts for i in code if i.group(CS_GRP_JUMP)and i.operands[0].type==X86_OP_IMM),'branch leaves owner')
    require([i.op_str for i in code if i.mnemonic=='ret']==['4','4'],'return ABI differs')
    require(Counter(i.op_str for i in code if i.mnemonic=='call')=={'0x41f800':1,'0x449950':1,'0x43e710':1,'0x41bb00':1},'call protocol differs')
    require(struct.unpack('<I',pe_bytes_at(raw,0x46da74,4))[0]==0x41e260,'virtual slot14 differs')
    obs={i.address:(i.mnemonic,i.op_str)for i in code}
    for a,pair in {
        0x41e269:('push','0x40c00000'),0x41e298:('mov','dword ptr [esp + 0x24], esi'),
        0x41e2e2:('test','ah, 5'),0x41e2f8:('movsx','ebx, word ptr [edx + 0x44a]'),
        0x41e2ff:('mov','ecx, dword ptr [0x47781c]'),0x41e305:('mov','ebp, dword ptr [ecx + 0x458]'),
        0x41e310:('lea','ebx, [ebx + ebx + 0x11]'),0x41e32b:('or','edx, 0x40000000'),
        0x41e33d:('mov','dword ptr [edi + 0x20], esi'),0x41e340:('fstp','dword ptr [edi + 0x340]'),
        0x41e34c:('mov','dword ptr [edi + 0x348], edx'),0x41e358:('fstp','dword ptr [edi + 0x344]'),
        0x41e36c:('mov','esi, dword ptr [0x491c10]'),0x41e374:('mov','ecx, dword ptr [esi + 0x72dad4]'),
        0x41e3a8:('mov','ecx, dword ptr [esi + 0x732454]'),
        0x41e3e4:('test','ah, 0x41'),0x41e3fb:('test','ah, 1'),
        0x41e412:('test','ah, 0x41'),0x41e429:('test','ah, 1'),
        0x41e42e:('mov','eax, dword ptr [0x477818]'),0x41e433:('push','0x3f19999a'),
        0x41e438:('push','0xbfc90fdb'),0x41e43d:('push','-1'),0x41e43f:('push','8'),
        0x41e46e:('fld','dword ptr [esp + 0x24]'),0x41e472:('fadd','dword ptr [esp + 0x30]'),
        0x41e48e:('fcomp','dword ptr [ecx + 0x40]'),0x41e493:('test','ah, 5'),
        0x41e4a3:('mov','dword ptr [edx + 0xc], 1'),0x41e4b3:('mov','dword ptr [edx + 0xc], 1'),
    }.items():require(obs.get(a)==pair,'operand differs at '+hex(a))
    for a,x in [(0x470cd8,12),(0x470d1c,6),(0x470b4c,224),(0x470b48,16),(0x470bcc,32),(0x470b40,-192),(0x470b3c,192),(0x470b04,0),(0x470b38,448)]:require(pe_bytes_at(raw,a,4)==struct.pack('<f',x),'constant differs at '+hex(a))
    require(pe_bytes_at(raw,0x41e256,10)==b'\xcc'*10,'preceding padding differs')
    require(pe_bytes_at(raw,0x41e4c1,15)==b'\xcc'*15,'following padding differs')
    require(pe_bytes_at(raw,0x41bb00,15)==bytes.fromhex('53558b6c240c5633db83fd08578bf0'),'Item Spawn receiver/kind ABI differs')
    rows={int(r['address'],0):r for r in csv.DictReader((ROOT/'config/functions.csv').open())}
    require(int(rows[0x41e260]['size'])==609 and int(rows[0x41e260]['span_end'],0)==0x41e4c0,'owner extent differs')
    shared=[]
    for a,call in [(0x41d880,0x41daa8),(0x41dd80,0x41df27),(0x41eb00,0x41ed28),(0x41efa0,0x41f147)]:
        z=int(rows[a]['size']);ins=list(d.disasm(pe_bytes_at(raw,a,z),a));idx=next(j for j,i in enumerate(ins)if i.address==call)
        require(ins[idx].mnemonic=='call'and ins[idx].op_str=='0x41bb00','collision Item callee differs')
        require(any(i.mnemonic=='mov'and i.op_str=='eax, dword ptr [0x477818]'for i in ins[max(0,idx-12):idx]),'collision Item receiver differs')
        shared.append({'owner':hex(a),'call':hex(call),'global':'0x477818','callee':'0x41bb00'})
    print(json.dumps({'result':'passed','authority':'target only; no codegen/runtime credit','target':identity,'owner':{'address':'0x41e260','size':609,'slot':20,'initial_sample':6,'stride':12,'resource':'0x47781c+458','item_receiver':'0x477818','retirement_field':'0xc'},'shared_item_calls':shared},indent=2))


if __name__=='__main__':main()
