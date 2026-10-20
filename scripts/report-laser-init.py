#!/usr/bin/env python3
"""Target-only laser Initialize/Motion0040 boundaries, fields and neighbors."""
from collections import Counter
import csv,json,struct,tomllib
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM
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
    owners={a:decode(a,z)for a,z in [(0x41C880,54),(0x41C8C0,442),(0x41D170,329)]}
    for a,code in owners.items():
        starts={i.address for i in code};require(code[-1].mnemonic=='ret','missing final RET')
        require(all(i.operands[0].imm in starts for i in code if i.group(CS_GRP_JUMP)and i.operands[0].type==X86_OP_IMM),'branch leaves owner')
    require(owners[0x41C880][-1].op_str=='8'and owners[0x41C8C0][-1].op_str=='4','stack cleanup differs')
    require(struct.unpack('<I',pe_bytes_at(raw,0x46DA64,4))[0]==0x41C8C0 and struct.unpack('<I',pe_bytes_at(raw,0x46DA90,4))[0]==0x41D170,'virtual slots differ')
    require(Counter(i.op_str for i in owners[0x41C8C0]if i.mnemonic=='call')=={'0x404f30':2,'0x43ee30':2,'0x41f800':1},'initializer call protocol differs')
    require(Counter(i.op_str for i in owners[0x41D170]if i.mnemonic=='call')=={'0x43dc90':1,'0x41f800':1,'0x463b2c':1},'motion call protocol differs')
    observed={i.address:(i.mnemonic,i.op_str)for code in owners.values()for i in code}
    for a,pair in {
        0x41C8CB:('lea','ebp, [ebx + 0x424]'),0x41C8D1:('mov','ecx, 0x77'),
        0x41C8D8:('rep movsd','dword ptr es:[edi], dword ptr [esi]'),
        0x41C8DA:('movsx','ecx, word ptr [ebx + 0x448]'),
        0x41C8E1:('movsx','eax, word ptr [ebx + 0x44a]'),
        0x41C8ED:('mov','dword ptr [ebx + 0xc], edi'),
        0x41C8F0:('add','eax, dword ptr [ecx*4 + 0x474170]'),
        0x41C8F7:('mov','edx, dword ptr [0x47781c]'),
        0x41C8FD:('mov','ecx, dword ptr [edx + 0x458]'),
        0x41C903:('lea','esi, [ebx + 0x600]'),
        0x41C910:('mov','word ptr [ebx + 0x904], di'),
        0x41C91C:('test','byte ptr [ebx + 0x44c], 1'),
        0x41C943:('and','eax, 0xfc63ffff'),0x41C948:('or','eax, 0x600000'),
        0x41C953:('movsx','eax, word ptr [ebx + 0x44a]'),
        0x41C960:('lea','esi, [ebx + 0x9ac]'),0x41C966:('add','eax, 0x103'),
        0x41C972:('mov','word ptr [ebx + 0xcb0], di'),
        0x41C996:('and','ecx, 0xfc7fffff'),0x41C99C:('or','ecx, 0x400000'),
        0x41C9DD:('mov','dword ptr [ebx + 0x414], 0x1e'),
        0x41C9E7:('mov','dword ptr [ebx + 0x418], 0x41f00000'),
        0x41C9F1:('mov','dword ptr [ebx + 0x410], 0x1d'),
        0x41C9FB:('fld','dword ptr [ebx + 0x438]'),
        0x41CA3E:('mov','dword ptr [ebx + 0x40c], 0x18'),
        0x41CA48:('mov','dword ptr [ebx + 0x44], edx'),
        0x41CA4D:('test','ah, 0x41'),0x41CA52:('mov','dword ptr [ebx + 0x4c], 0x3c23d70a'),
        0x41D174:('mov','eax, dword ptr [esi + 0x11c]'),
        0x41D17A:('cmp','dword ptr [esi + 0xf8], eax'),
        0x41D18C:('mov','edi, dword ptr [esi + 0x40c]'),
        0x41D199:('mov','ecx, 0x492590'),
        0x41D1B4:('mov','dword ptr [esi + 0x124], edx'),
        0x41D1BC:('and','dword ptr [esi + 0x404], 0xffffffbf'),
        0x41D1C3:('fld','dword ptr [esi + 0x10c]'),
        0x41D1D5:('fst','dword ptr [esi + 0x48]'),
        0x41D1FF:('mov','dword ptr [esi + 0x100], 0x476f78'),
        0x41D22E:('fmul','dword ptr [esi + 0xfc]'),
        0x41D234:('fidiv','dword ptr [esp + 4]'),
        0x41D257:('mov','ecx, dword ptr [esi + 0x100]'),
    }.items():require(observed.get(a)==pair,'operand differs at '+hex(a))
    for a,x in [(0x470B68,.99),(0x470B64,1.01),(0x470AFC,1.)]:require(pe_bytes_at(raw,a,4)==struct.pack('<f',x),'timer constant differs')
    for a,z in [(0x41C87C,4),(0x41C8B6,10),(0x41CA7A,6),(0x41D16A,6),(0x41D2B9,7)]:require(pe_bytes_at(raw,a,z)==b'\xcc'*z,'neighbor padding differs')
    with (ROOT/'config/function-boundaries.csv').open(newline='')as f:ledger={int(r['address'],0):r for r in csv.DictReader(f)}
    for a,z in [(0x41C880,54),(0x41C8C0,442),(0x41D170,329)]:require(ledger[a]['state']=='reviewed'and int(ledger[a]['span_end'],0)==a+z-1,'physical ledger differs')
    text=pe_bytes_at(raw,0x401000,0x64A81)
    refs=[0x401000+off for off in range(len(text)-4)if text[off]==0xe8 and 0x401000+off+5+struct.unpack_from('<i',text,off+1)[0]==0x41C880]
    require(not refs and struct.pack('<I',0x41C880)not in raw,'unreferenced neighbor status differs')
    print(json.dumps({'result':'passed','authority':'target observations only; no codegen/runtime credit','target':identity,'new_authored_owned_bytes':771,'unreferenced_unknown_owner_bytes':54,'owners':[{'address':'0x41c8c0','size':442,'slot':4,'request_copy_bytes':476},{'address':'0x41d170','size':329,'slot':48,'mask':'0x40'}],'unknown_neighbor':'0x41c880; no static CALL pattern or literal VA; no source mapping'},indent=2))


if __name__=='__main__':main()
