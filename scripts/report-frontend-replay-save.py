#!/usr/bin/env python3
"""Audit the complete target replay-save owners; grants no codegen exactness.

Ghidra's provisional body ranges omit internal padding and the update table.
This focused target audit keeps those physical bytes, selector/table ownership,
entry call sites and adjacent padding separate from source and compiler claims.
"""
from collections import Counter
import csv, json, struct, tomllib
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM, X86_REG_EAX
from linked_image import verify_capstone
from target_identity import pe_bytes_at, resolve_target, verify_target
ROOT = Path(__file__).resolve().parents[1]
CALLS = {
    0x433570: {0x409e50:1,0x40ad20:1,0x420a90:1,0x420b10:1,
        0x4294a0:2,0x4296f0:2,0x4297b0:1,0x429b60:1,0x42c5c0:1,
        0x42c620:5,0x42c670:1,0x42c6d0:2,0x42c770:2,0x43dc90:7,
        0x44be70:1,0x44bea0:7,0x4524a1:2,0x452ae8:2,0x452b71:1,0x458ea5:1},
    0x433b30: {0x401630:6,0x452baa:2},
}


def require(condition, message):
    if not condition: raise ValueError(message)


def main():
    target = resolve_target(None); identity, problems = verify_target(target)
    require(not problems, '; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb') as f:
        decoder_identity = verify_capstone(tomllib.load(f)['capstone'])
    raw = target.read_bytes(); decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    def decode(start, size):
        result = list(decoder.disasm(pe_bytes_at(raw, start, size), start))
        require(sum(i.size for i in result) == size, 'incomplete dense decode')
        return result
    records = []
    for start, size, physical_end in [(0x433570,1440,0x433b23),(0x433b30,955,0x433eea)]:
        instructions = decode(start, size)
        require(instructions[-1].mnemonic == 'ret' and instructions[-1].operands[0].imm == 4, 'RET4 changed')
        calls = Counter(i.operands[0].imm for i in instructions if i.mnemonic == 'call' and i.operands[0].type == X86_OP_IMM)
        require(calls == CALLS[start], 'direct call inventory changed')
        # Every ordinary immediate branch must land on a decoded local start.
        starts = {i.address for i in instructions}
        for i in instructions:
            if i.group(CS_GRP_JUMP) and i.operands and i.operands[0].type == X86_OP_IMM:
                require(i.operands[0].imm in starts, 'external/non-instruction branch')
        with (ROOT/'config/function-boundaries.csv').open(newline='') as f:
            rows = {int(r['address'],0): r for r in csv.DictReader(f)}
        require(int(rows[start]['span_end'],0) == physical_end and rows[start]['state'] == 'reviewed', 'physical ledger differs')
        records.append({'entry':hex(start),'code_bytes':size,'owned_bytes':physical_end-start+1,'calls':{hex(k):v for k,v in sorted(calls.items())}})
    table = struct.unpack('<5I', pe_bytes_at(raw,0x433b10,20))
    require(table == (0x433599,0x43362f,0x43364d,0x4337c9,0x433a33), 'five-phase table changed')
    guard = decode(0x433586,9)
    require(len(guard)==2 and guard[0].mnemonic=='cmp' and guard[0].operands[0].reg==X86_REG_EAX and guard[0].operands[1].imm==4 and guard[1].mnemonic=='ja' and guard[1].operands[0].imm==0x433af9,'unsigned phase guard changed')
    jump = decode(0x433592,7)[0]
    require(jump.mnemonic=='jmp' and jump.operands[0].type==X86_OP_MEM and jump.operands[0].mem.index==X86_REG_EAX and jump.operands[0].mem.scale==4 and jump.operands[0].mem.disp==0x433b10,'table reference changed')
    for start, count in [(0x433b24,12),(0x433eeb,5)]:
        require(pe_bytes_at(raw,start,count)==b'\xcc'*count,'neighbor padding changed')
    for site, callee in [(0x42d19a,0x433570),(0x42d29c,0x433b30)]:
        pair = decode(site-1,6)
        require(len(pair)==2 and pair[0].mnemonic=='push' and pair[1].mnemonic=='call' and pair[1].operands[0].imm==callee,'dispatcher stack edge changed')
    require(struct.unpack('<I',pe_bytes_at(raw,0x42d214+16*4,4))[0]==0x42d199,'update screen16 table edge changed')
    require(struct.unpack('<I',pe_bytes_at(raw,0x42d2b4+7*4,4))[0]==0x42d29b,'draw screen16 table edge changed')
    print(json.dumps({'authority':'target observations only; no source/runtime/codegen promotion','target':identity,'decoder':decoder_identity,'owners':records,'phase_destinations':[hex(x)for x in table],'result':'passed'},indent=2))

if __name__ == '__main__': main()
