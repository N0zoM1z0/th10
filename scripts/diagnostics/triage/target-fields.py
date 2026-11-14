#!/usr/bin/env python3
"""List absolute (DIR32) and outbound REL32 field candidates of a target function.

Heuristic: verify with compare-linked-function.py. Optional SYMBOL=BASE:SIZE or
SYMBOL@ADDR arguments name the fields.
usage: scripts/repo-python scripts/diagnostics/triage/target-fields.py VA SIZE [SYM...]
"""
import pathlib
import sys, struct, capstone
from capstone import x86
exe = (pathlib.Path(__file__).resolve().parents[3] / 'resources/th10.exe').read_bytes()
va, size = int(sys.argv[1], 0), int(sys.argv[2], 0)
# extra args: SYMBOL=BASE:SIZE (data range, emits target=BASE addend=delta) or SYMBOL@ADDR (exact address)
syms = []
for a in sys.argv[3:]:
    if '=' in a:
        n, r = a.rsplit('=', 1); b, z = r.split(':'); syms.append((n, int(b, 0), int(z, 0)))
    else:
        n, b = a.rsplit('@', 1); syms.append((n, int(b, 0), 1))
def name(v):
    for n, b, z in syms:
        if b <= v < b + z:
            return n, b, v - b
    return '?', v, 0
def fmt(rel, kind, v):
    n, b, d = name(v)
    ad = f', addend = 0x{d:X}' if d else ''
    return f'  {{ offset = 0x{rel:02X}, type = "{kind}", symbol = "{n}", target = 0x{b:08X}{ad} }},'
off = va - 0x401000 + 0x400
code = exe[off:off + size]
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
lo, hi = 0x401000, 0x49c000
for ins in md.disasm(code, va):
    rel = ins.address - va
    raw = bytes(ins.bytes)
    if ins.group(x86.X86_GRP_BRANCH_RELATIVE) or ins.mnemonic in ('call', 'jmp') and ins.operands and ins.operands[0].type == x86.X86_OP_IMM:
        t = ins.operands[0].imm
        if len(raw) >= 5 and not (va <= t < va + size):
            print(fmt(rel+len(raw)-4, 'REL32', t))
        continue
    for k in range(len(raw) - 3):
        v = struct.unpack_from('<I', raw, k)[0]
        if lo <= v < hi and (ins.disp_offset == k or ins.imm_offset == k):
            print(fmt(rel+k, 'DIR32', v))
