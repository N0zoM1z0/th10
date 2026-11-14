#!/usr/bin/env python3
"""Draft a linked unit's linkage list: target field offsets/targets, with symbol
names resolved from the candidate image and map. Review, then let
compare-linked-function.py decide.
usage: scripts/repo-python scripts/diagnostics/triage/linkages-from-candidate.py IMAGE SYMBOL_SUBSTR TARGET_VA SIZE
"""
import sys, re, struct, pathlib, subprocess
img, sym, tva, size = sys.argv[1], sys.argv[2], int(sys.argv[3], 0), int(sys.argv[4], 0)
mp = next(pathlib.Path(img).parent.glob('*.map'))
syms = []
for l in open(mp, errors='replace'):
    p = l.split()
    if len(p) >= 3 and re.fullmatch(r'[0-9a-f]{8}', p[2]) and re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{8}', p[0]):
        syms.append((int(p[2], 16), p[1]))
syms.sort(key=lambda t: (t[0], 'probe_anchor' not in t[1]))
cva = next(a for a, n in syms if sym in n)
b = open(img, 'rb').read(); o = struct.unpack_from('<I', b, 0x3c)[0]
n = struct.unpack_from('<H', b, o + 6)[0]; opt = struct.unpack_from('<H', b, o + 20)[0]; base = struct.unpack_from('<I', b, o + 52)[0]
secs = []
for i in range(n):
    s = o + 24 + opt + 40 * i; vs, va, rs, rp = struct.unpack_from('<IIII', b, s + 8); secs.append((base + va, max(vs, rs), rp))
def rd(va):
    for sva, sz, rp in secs:
        if sva <= va < sva + sz: return struct.unpack_from('<I', b, rp + va - sva)[0]
def name(v):
    best = None
    for a, nm in syms:
        if a <= v: best = (a, nm)
        else: break
    return best
tf = subprocess.run([sys.executable, str(pathlib.Path(__file__).with_name('target-fields.py')), hex(tva), hex(size)], capture_output=True, text=True).stdout
for line in tf.splitlines():
    m = re.search(r'offset = 0x([0-9A-F]+), type = "(\w+)".*target = 0x([0-9A-F]+)', line)
    off, kind, tgt = int(m[1], 16), m[2], int(m[3], 16)
    raw = rd(cva + off)
    dest = (cva + off + 4 + raw) & 0xffffffff if kind == 'REL32' else raw
    a, nm = name(dest)
    add = dest - a
    if kind == 'REL32' and add: print('# WARN rel32 into middle', hex(dest), nm, add)
    ad = f', addend = 0x{add:X}' if add else ''
    tbase = tgt - add
    print(f'  {{ offset = 0x{off:02X}, type = "{kind}", symbol = "{nm}", target = 0x{tbase:08X}{ad} }},')
