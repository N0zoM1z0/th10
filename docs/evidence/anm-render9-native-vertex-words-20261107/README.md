# ANM ExecuteScript render-mode 9: native segment-count and vertex cursors

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `AnmRenderManagerView::ExecuteScript @0x0043EE30` owns
**9,964 bytes** and 350 independently original-bound code/data fields.

For renderMode 9 generated vertices, `vm->intVar0` controls the number of
segments without a checked maximum. The original i386 decrements the
32-bit integer with wraparound before the floating-point denominator and
loop bound; subsequent generated-vertex writes advance flat 32-bit address
words. The earlier C++ used signed `vertexCount - 1` and advanced
`AnmRenderVertexView *vertex` with `++vertex`, potentially invoking signed
integer overflow and out-of-array pointer arithmetic. The installed C++
now computes all three segment-minus-one uses as native unsigned subtraction
cast back to signed and advances both complete `AnmRenderVertexView` strides
through modulo-32-bit Win32 address words. It does not bound unchecked
vertex writes or promise portable pointer behavior.

## Direct current-source linked verification

A fresh five-input VC7.1 SP1 `/GL /GS` graph compiled the original source,
followed by separate native trials of first-vertex, second-vertex, both
and combined count variants; no target byte was copied into source. The
installed source at SHA-256
`0de0bdff08ca81f78882b72f91e4bbdc1d7ab44305984ab9968aa66dd299ed6a`
was recompiled as a **complete** 9,964-byte PDB contribution in its
actual five-TU graph. The full original comparison is still
**9,962/9,964 bytes**, with all **350/350 original-bound fields** and the
only two mismatches at `+0x113,+0x115` (opcode-91 register choices). The
changed mode-9 loop does not resolve these opcode-91 bytes. This entire
owner remains **NONEXACT**, and no new original exactness is claimed.

Selected affected native original-bound functions were independently cold
replayed from installed source (only these selectors, not all historical
units): `anm-vm-timer-add` **88/88**, `anm-vm-timer-decrement` **20/20**,
`anm-int3-scale` **56/56**; **164/164** total complete original bytes in
2 native graphs. Default lightweight CI passed seven stages.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-vm-timer-add --unit anm-vm-timer-decrement \
  --unit anm-int3-scale --json
scripts/repo-python scripts/ci.py
git diff --check
```

Ignored local evidence: `.analysis/gpt-web/20261107-active/anm-current.json`,
`anm-vertex-results.json`, `anm-vertex-count-results.json`,
`anm-installed-vertex-word.json`, `anm-render9-exact-replay.json`; cold PDB,
MAP, PE under `build/gpt-web/20261107-active-anm-graph/`.
