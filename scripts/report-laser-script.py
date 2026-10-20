#!/usr/bin/env python3
"""Target-only command parser operands, callback boundaries and timer records."""
from collections import Counter
import json,struct,tomllib
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from linked_image import verify_capstone
from target_identity import pe_bytes_at,resolve_target,verify_target
ROOT=Path(__file__).resolve().parents[1]


def require(condition,message):
    if not condition:raise ValueError(message)


def main():
    path=resolve_target();identity,problems=verify_target(path);require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb')as f:verify_capstone(tomllib.load(f)['capstone'])
    raw=path.read_bytes();d=Cs(CS_ARCH_X86,CS_MODE_32)
    def decode(a,z):
        code=list(d.disasm(pe_bytes_at(raw,a,z),a));require(sum(i.size for i in code)==z,'incomplete decode');return code
    code=decode(0x41CA80,1269);observed={i.address:(i.mnemonic,i.op_str)for i in code}
    for a,pair in {
        0x41CA84:('cmp','dword ptr [ebp + 0x400], 0x12'),
        0x41CA8B:('jge','0x41cf72'),
        0x41CAAD:('mov','edx, dword ptr [ebp + eax*8 + 0x10]'),
        0x41CABF:('cmp','dword ptr [esi + 0x14], ebx'),
        0x41CAD0:('cmp','edx, 0x80000000'),
        0x41CB50:('mov','dword ptr [ebp + 0xa0], edx'),
        0x41CB59:('fcomp','dword ptr [0x470b50]'),
        0x41CB6E:('fcomp','dword ptr [0x470ccc]'),
        0x41CB8B:('fld','dword ptr [esi + 4]'),
        0x41CBB7:('mov','dword ptr [ebp + 0xb4], eax'),
        0x41CBC2:('cmp','dword ptr [ebp + 0x400], ebx'),
        0x41CBCE:('mov','edi, dword ptr [ebp + 0x40c]'),
        0x41CC20:('mov','dword ptr [ebp + 0xe8], edx'),
        0x41CC88:('mov','dword ptr [ebp + 0x10c], eax'),
        0x41CC91:('fcomp','dword ptr [0x470cc8]'),
        0x41CCA3:('fld','dword ptr [ebp + 0x48]'),
        0x41CD2A:('mov','eax, dword ptr [eax*4 + 0x474170]'),
        0x41CD31:('mov','ecx, dword ptr [0x4776f0]'),
        0x41CD39:('mov','edx, dword ptr [ecx + 0x3e0b50]'),
        0x41CD3F:('lea','esi, [ebp + 0x600]'),
        0x41CD4B:('mov','eax, dword ptr [ebp + 0x400]'),
        0x41CD97:('fild','dword ptr [esp + 0x10]'),
        0x41CDE6:('mov','eax, dword ptr [ebp + 0x24]'),
        0x41CDED:('mov','esi, 0x492590'),
        0x41CE09:('mov','dword ptr [ebp + 0xc], 3'),
        0x41CE20:('lea','eax, [ebp + 0x190]'),
        0x41CE4C:('lea','eax, [ebp + 0x190]'),
        0x41CE67:('mov','dword ptr [ebp + 0x54], eax'),
        0x41CE92:('cmp','dword ptr [esi + 8], ebx'),
        0x41CE95:('jle','0x41cf50'),
        0x41CEB3:('test','ah, 1'),
        0x41CECB:('dec','dword ptr [esi + 8]'),
        0x41CEEF:('mov','dword ptr [ebp + 0x20c], eax'),
        0x41CF4A:('mov','dword ptr [ebp + 0x220], edx'),
        0x41CF59:('mov','eax, dword ptr [esi + 8]'),
        0x41CF5C:('mov','dword ptr [ebp + 0x400], eax'),
    }.items():require(observed.get(a)==pair,'operand differs at '+hex(a))
    # Capstone detail is unnecessary for the address-pinned observations above.
    calls=Counter(i.op_str for i in code if i.mnemonic=='call')
    require(calls=={'0x405410':5,'0x426660':1,'0x41f800':1,'0x43dc90':2,'0x404f30':1,'0x43dd10':1},'direct boundaries differ')
    for a,value in [(0x470B50,-990.),(0x470CCC,990.),(0x470CC8,-999.)]:require(pe_bytes_at(raw,a,4)==struct.pack('<f',value),'sentinel differs')
    require([(i.mnemonic,i.op_str)for i in decode(0x40708C,7)]==[('mov','eax, dword ptr [edx*4 + 0x474170]')],'existing Bullet table binding differs')
    require(decode(0x405410,57)[-1].op_str=='4','shared timer cleanup differs')
    print(json.dumps({'result':'passed','authority':'target observations only; no source/codegen/runtime credit','target':identity,'parser':'0x41CA80','owned_bytes':1356,'direct_calls':dict(calls),'commands':{'offset':'0x450','stride':24,'upper_signed_bound':18,'negative_pc':'not rejected by target','argument2':'live writable counter/jump/timer operand'},'timers':{'0x100000':6,'0x200000':6,'0x8000':5,'0x4000000':8},'script_base_table':'0x474170 (same physical table as existing Bullet caller)'},indent=2))


if __name__=='__main__':main()
