#!/usr/bin/env python3
"""Review the type1 Clear extent, bounds, integer result and preserved polar EDX."""
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
    raw=path.read_bytes();decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    code=list(decoder.disasm(pe_bytes_at(raw,0x41f400,623),0x41f400))
    require(sum(i.size for i in code)==623,'incomplete Clear decode')
    starts={i.address for i in code}
    require(all(i.operands[0].imm in starts for i in code if i.group(CS_GRP_JUMP)and i.operands[0].type==X86_OP_IMM),'branch leaves owner')
    require([i.op_str for i in code if i.mnemonic=='ret']==['4','4'],'return ABI differs')
    require(Counter(i.op_str for i in code if i.mnemonic=='call')=={'0x41f800':1,'0x449950':1,'0x43e710':1,'0x4489d0':1,'0x41bb00':1},'call protocol differs')
    require(struct.unpack('<I',pe_bytes_at(raw,0x46da24,4))[0]==0x41f400,'slot14 differs')
    obs={i.address:(i.mnemonic,i.op_str)for i in code}
    for address,pair in {
        0x41f40e:('xor','edx, edx'),0x41f415:('mov','dword ptr [esp + 0x10], edx'),
        0x41f419:('mov','dword ptr [esp + 0xc], 0x40c00000'),
        0x41f434:('mov','dword ptr [esp + 0x24], edx'),
        0x41f47e:('test','ah, 5'),0x41f494:('mov','eax, dword ptr [esp + 0x14]'),
        0x41f49e:('inc','eax'),0x41f49f:('mov','dword ptr [esp + 0x14], eax'),
        0x41f4ab:('test','ah, 0x41'),0x41f4c6:('test','ah, 1'),
        0x41f4d9:('fst','dword ptr [esp + 0x18]'),
        0x41f4e5:('test','ah, 0x41'),0x41f500:('test','ah, 1'),
        0x41f509:('movsx','ebx, word ptr [ebp + 0x466]'),
        0x41f510:('mov','ecx, dword ptr [0x47781c]'),
        0x41f516:('mov','esi, dword ptr [0x491c10]'),
        0x41f51c:('mov','edi, dword ptr [ecx + 0x458]'),
        0x41f522:('lea','ebx, [ebx + ebx + 0x11]'),
        0x41f52b:('mov','edx, dword ptr [esp + 0x18]'),
        0x41f545:('or','ecx, 0x40000000'),
        0x41f551:('mov','dword ptr [esi + 0x20], 0'),
        0x41f558:('mov','dword ptr [esi + 0x348], eax'),
        0x41f55e:('fstp','dword ptr [esi + 0x340]'),
        0x41f564:('mov','eax, esi'),0x41f566:('mov','ecx, edi'),
        0x41f568:('mov','dword ptr [esi + 0x344], edx'),
        0x41f573:('mov','edx, dword ptr [0x491c10]'),
        0x41f579:('mov','ebx, esi'),0x41f57b:('lea','eax, [esp + 0x18]'),
        0x41f588:('test','eax, eax'),
        0x41f59e:('test','ah, 0x41'),0x41f5b5:('test','ah, 1'),
        0x41f5cc:('test','ah, 0x41'),0x41f5e3:('test','ah, 1'),
        0x41f5e8:('mov','eax, dword ptr [0x477818]'),
        0x41f5ed:('push','0x3f19999a'),0x41f5f2:('push','0xbfc90fdb'),
        0x41f5f7:('push','-1'),0x41f5f9:('push','8'),
        0x41f61c:('fld','dword ptr [esp + 0x24]'),
        0x41f620:('fadd','dword ptr [esp + 0x30]'),
        0x41f63c:('fcomp','dword ptr [ebp + 0x40]'),0x41f641:('test','ah, 5'),
        0x41f64a:('mov','eax, dword ptr [esp + 0x14]'),
        0x41f651:('mov','dword ptr [ebp + 0xc], 1'),
        0x41f65f:('mov','dword ptr [ebp + 0xc], 1'),0x41f666:('mov','eax, edx'),
    }.items():require(obs.get(address)==pair,'operand differs at '+hex(address))
    polar=list(decoder.disasm(pe_bytes_at(raw,0x41f800,30),0x41f800))
    require(sum(i.size for i in polar)==30,'incomplete polar decode')
    require(not any(i.mnemonic=='call'or any(i.reg_name(r)in ('edx','dx','dl','dh')for r in i.regs_access()[1])for i in polar),'polar no longer preserves EDX')
    require([i.op_str for i in polar if i.mnemonic=='fstp']==['dword ptr [eax]','dword ptr [eax + 4]'],'polar write set differs')
    require(polar[-1].mnemonic=='ret'and polar[-1].op_str=='8','polar return ABI differs')
    for address,value in [(0x470cd8,12),(0x470d1c,6),(0x470b4c,224),(0x470b48,16),(0x470bcc,32),(0x470b40,-192),(0x470b3c,192),(0x470b04,0),(0x470b38,448)]:
        require(pe_bytes_at(raw,address,4)==struct.pack('<f',value),'constant differs at '+hex(address))
    require(pe_bytes_at(raw,0x41f3f4,12)==b'\xcc'*12,'preceding padding differs')
    require(pe_bytes_at(raw,0x41f66f,1)==b'\xcc','following padding differs')
    require(pe_bytes_at(raw,0x41f670,7)==bytes.fromhex('83ec188b44241c'),'next query entry differs')
    rows={int(r['address'],0):r for r in csv.DictReader((ROOT/'config/functions.csv').open())}
    require(int(rows[0x41f400]['size'])==623 and int(rows[0x41f400]['span_end'],0)==0x41f66e,'owner extent differs')
    print(json.dumps({'result':'passed','authority':'target observations only; no native/runtime credit','target':identity,'owner':{'address':'0x41f400','size':623,'slot':20,'initial_sample':6,'stride':12,'outer_margin':16,'item_margin':32,'count_includes_offscreen':True,'result':'integer EAX','polar_preserves_edx':True,'step_z':0,'creation':'direct allocator/binder/outlined registrar','vm_offset_field':'0x340'},'unreviewed_frontier':'slot20 query41F670 remains separately untracked'},indent=2))


if __name__=='__main__':main()
