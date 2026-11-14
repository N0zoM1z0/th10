#!/usr/bin/env python3
"""Triage only, never exact acceptance: align a linked candidate function's
normalized instructions (addresses masked) against the TH10 target.

usage: scripts/repo-python scripts/diagnostics/triage/idiff.py IMAGE.exe SYMBOL_SUBSTR TARGET_VA [SIZE] [--show N]
"""
import sys, re, difflib, struct, capstone, pathlib
root = pathlib.Path(__file__).resolve().parents[3]
img, sym, tva = sys.argv[1], sys.argv[2], int(sys.argv[3], 0)
tsize = int(sys.argv[4], 0) if len(sys.argv) > 4 and not sys.argv[4].startswith('--') else None
show = int(sys.argv[sys.argv.index('--show') + 1]) if '--show' in sys.argv else 40
def pe(path):
    b = open(path, 'rb').read(); o = struct.unpack_from('<I', b, 0x3c)[0]
    n = struct.unpack_from('<H', b, o + 6)[0]; opt = struct.unpack_from('<H', b, o + 20)[0]
    base = struct.unpack_from('<I', b, o + 52)[0]; secs = []
    for i in range(n):
        s = o + 24 + opt + 40 * i
        vs, va, rs, rp = struct.unpack_from('<IIII', b, s + 8)
        secs.append((base + va, max(vs, rs), rp))
    return b, secs
def read(path, va, size):
    b, secs = pe(path)
    for sva, sz, rp in secs:
        if sva <= va < sva + sz: return b[rp + va - sva: rp + va - sva + size]
mp = next(pathlib.Path(img).parent.glob('*.map'))
cva = None
for line in open(mp, errors='replace'):
    p = line.split()
    if len(p) >= 3 and sym in p[1] and re.fullmatch(r'[0-9a-f]{8}', p[2]): cva = int(p[2], 16); break
assert cva, 'symbol not in map'
addrs = sorted({int(l.split()[2], 16) for l in open(mp, errors='replace') if len(l.split()) >= 3 and re.fullmatch(r'[0-9a-f]{8}', l.split()[2])})
cnext = next((a for a in addrs if a > cva), cva + 0x10000)
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
def norm(code, base, size):
    out = []
    for ins in md.disasm(code, base):
        if ins.address >= base + size: break
        t = f'{ins.mnemonic} {ins.op_str}'
        t = re.sub(r'0x[0-9a-f]{5,8}', 'ADDR', t)
        out.append((ins.address - base, t, ins.size))
    return out
tcode = read(str(root / 'resources/th10.exe'), tva, tsize or 0x8000)
T = norm(tcode, tva, tsize or 0x8000)
if not tsize:
    tsize = next((o for o, t, s in T if t.startswith('int3')), T[-1][0])
    T = [x for x in T if x[0] < tsize]
C = norm(read(img, cva, cnext - cva), cva, cnext - cva)
while C and C[-1][1].startswith('int3'): C.pop()
cend = C[-1][0] + C[-1][2]
sm = difflib.SequenceMatcher(None, [t for _, t, _ in T], [t for _, t, _ in C], autojunk=False)
same = sum(b.size for b in sm.get_matching_blocks())
print(f'target {tsize} bytes/{len(T)} ins; candidate {cend} bytes/{len(C)} ins; identical-normalized ins {same}/{len(T)}')
k = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    k += 1
    if k > show: continue
    print(f'--- {tag} T+{T[i1][0] if i1 < len(T) else tsize:#x} C+{C[j1][0] if j1 < len(C) else cend:#x}')
    for o, t, _ in T[i1:i2][:8]: print(f'   T {o:#06x} {t}')
    for o, t, _ in C[j1:j2][:8]: print(f'   C {o:#06x} {t}')
print('hunks', k)
