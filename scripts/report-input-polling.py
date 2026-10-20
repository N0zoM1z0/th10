#!/usr/bin/env python3
"""Target-only audit of input polling, complete record stride and button helpers."""
import csv,json,tomllib
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM
from linked_image import verify_capstone
from target_identity import pe_bytes_at,resolve_target,verify_target
ROOT=Path(__file__).resolve().parents[1]


def require(ok,message):
    if not ok:raise ValueError(message)


def main():
    path=resolve_target();identity,errors=verify_target(path);require(not errors,'; '.join(errors))
    with (ROOT/'config/tools.lock.toml').open('rb')as f:verify_capstone(tomllib.load(f)['capstone'])
    raw=path.read_bytes();decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    owners={};observed={}
    for address,size,returns in [(0x44a130,37,['','']),(0x44a160,43,['4','4']),(0x44a190,833,['8']*4),(0x44a4e0,264,['']),(0x44a5f0,850,[''])]:
        code=list(decoder.disasm(pe_bytes_at(raw,address,size),address));require(sum(i.size for i in code)==size,'incomplete decode '+hex(address));starts={i.address for i in code}
        require(all(i.operands[0].imm in starts for i in code if i.group(CS_GRP_JUMP)and i.operands[0].type==X86_OP_IMM),'external branch '+hex(address))
        require([i.op_str for i in code if i.mnemonic=='ret']==returns,'return protocol '+hex(address));owners[hex(address)]={'size':size,'instructions':len(code)};observed.update({i.address:(i.mnemonic,i.op_str)for i in code})
    for a,pair in {
        0x44a146:('or','word ptr [edx], cx'),0x44a149:('cmp','byte ptr [eax], 0'),
        0x44a14e:('movzx','eax, si'),0x44a165:('mov','eax, 1'),
        0x44a16a:('shl','eax, cl'),0x44a179:('or','word ptr [edx], cx'),
        0x44a180:('movzx','eax, si'),
        0x44a1ad:('imul','eax, eax, 0x6a'),0x44a1b0:('add','eax, 0x474e30'),
        0x44a1de:('setne','cl'),0x44a205:('mov','cx, word ptr [ebx + 0x58]'),
        0x44a23e:('mov','cx, word ptr [ebx + 0x5e]'),0x44a259:('mov','cx, word ptr [ebx + 0x5c]'),
        0x44a274:('mov','bx, word ptr [ebx + 0x68]'),0x44a296:('imul','eax, eax, 0x194'),
        0x44a35b:('jmp','0x44a360'),0x44a35d:('lea','ecx, [ecx]'),
        0x44a36e:('cmp','edi, 0x190'),0x44a376:('cmp','eax, 0x8007001e'),
        0x44a39c:('mov','ecx, 0x44'),0x44a3b5:('push','0x110'),
        0x44a3c0:('jge','0x44a3dc'),
        0x44a4fa:('mov','ecx, 0x20'),0x44a4ff:('mov','edi, 0x497bb0'),
        0x44a522:('push','0'),0x44a556:('cmp','eax, 0x20'),
        0x44a593:('cmp','esi, 0x190'),0x44a5b2:('push','0x110'),
        0x44a5b8:('call','dword ptr [edx + 0x24]'),
        0x44a5bb:('mov','ecx, 0x20'),0x44a5c0:('lea','esi, [esp + 0x70]'),
        0x44a603:('imul','esi, esi, 0x6a'),0x44a615:('add','esi, 0x474e30'),
        0x44a61b:('test','eax, eax'),0x44a61d:('je','0x44a8b3'),
        0x44a628:('test','ah, 2'),0x44a636:('call','dword ptr [0x466208]'),
        0x44a76b:('cmp','eax, 0x8007001e'),0x44a782:('test','eax, eax'),
        0x44a784:('je','0x44a796'),
        0x44a8b3:('push','0'),0x44a8b5:('push','ebx'),0x44a8b6:('mov','ecx, edi'),
        0x44a8b8:('call','0x44a190'),0x44a8c5:('mov','word ptr [esi + 2], cx'),
        0x44a8c9:('mov','word ptr [esi], ax'),0x44a8d2:('mov','word ptr [esi + 4], 0'),
        0x44a8d8:('lea','ecx, [esi + 0xa]'),0x44a8db:('mov','edi, 0x10'),
        0x44a8e9:('cmp','word ptr [ecx], 0x1a'),0x44a8ef:('or','word ptr [esi + 4], dx'),
        0x44a8f3:('add','word ptr [ecx], 0xfff8'),0x44a8fa:('mov','word ptr [ecx], 0'),
        0x44a922:('mov','word ptr [esi + 6], di'),0x44a926:('mov','word ptr [esi + 8], cx'),
    }.items():require(observed.get(a)==pair,'operand differs '+hex(a))
    # This caller does not inspect the raw query's HRESULT before copying.
    getter=list(decoder.disasm(pe_bytes_at(raw,0x44a4e0,264),0x44a4e0));after=[i for i in getter if 0x44a5bb<=i.address<0x44a5cb]
    require(not any(i.group(CS_GRP_JUMP)for i in after),'raw query now branches on HRESULT')
    for address,count in [(0x44a129,7),(0x44a155,11),(0x44a18b,5),(0x44a4d1,15),(0x44a5e8,8),(0x44a942,14)]:require(pe_bytes_at(raw,address,count)==b'\xcc'*count,'neighbor padding '+hex(address))
    rows={int(r['address'],0):r for r in csv.DictReader((ROOT/'config/functions.csv').open())}
    for address,size in [(0x44a190,833),(0x44a4e0,264),(0x44a5f0,850)]:require(int(rows[address]['size'])==size and int(rows[address]['span_end'],0)==address+size-1,'ledger extent '+hex(address))
    print(json.dumps({'result':'passed','authority':'target observations only; no codegen/product/runtime credit','target':identity,'owners':owners,'record_stride':106,'gameplay_counter_offset':10,'debug_counter_offset':56,'mapping_offset':88,'retry_limit':'initial Acquire plus at most400 retries; no state fetch after Poll failure','origin_of_unreferenced_helpers':'indeterminate; source identity is descriptive'},indent=2))


if __name__=='__main__':main()
