# TH10 ANM renderMode 9: native vertex-index INC word

Original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`AnmRenderManagerView::ExecuteScript @0x0043EE30` is the **complete
9,964-byte** original interpreter, with 350 independently reviewed
linked fields. Its renderMode 9 path uses an unchecked script-supplied
vertex count and increments a signed local loop counter. On i386 the
integer INC wraps as a native unsigned 32-bit word. Ordinary C++
`++vertexIndex` would have undefined signed-overflow semantics if the
counter reaches `INT_MAX`. The source now uses
`vertexIndex = static_cast<int>(static_cast<unsigned int>(vertexIndex) + 1u)`
while retaining the signed loop comparison, script-count minus-one, two
existing native low-word generated-vertex pointer steps, float geometry,
texture coordinates, order of calls and tail copy. This is a faithful
Win32 word model, not a bounds check or portable invalid-script guarantee.

## Two real compiler controls and entire-owner comparison

The installed source baseline was cold built from the current five-input
VC7.1 SP1 `/GL /GS` graph: AnmManager.cpp, RandomMath.cpp,
AnmVmCreate.cpp, AnmVmId.cpp and AnmInt3Scale.cpp (latter `/Ob1`). The
baseline proves **9,962/9,964 original bytes and 350/350 original-bound
field bindings**, with only opcode 91 differences at `+0x113,+0x115`.

Two separate source variants changed only the renderMode 9 increment:
explicit signed publication after unsigned-word addition, and unsigned
reference increment. Both were actually recompiled and linked in the
full five-TU context against the original executable. Each preserved the
**complete 9,964-byte PDB owner**, **9,962/9,964** strict original bytes,
all **350 original address/call fields**, and exactly the same two
opcode-91 register mismatches. The simplest unsigned-word assignment is
installed. The owner therefore remains **NONEXACT**; no new exact owner
or codegen credit is claimed.

A fresh selected current-source cold replay of the affected existing exact
units in two genuine native contexts passed:

| Already exact unit | Original bytes |
| --- | ---: |
| `anm-vm-timer-add` | 88/88 |
| `anm-vm-timer-decrement` | 20/20 |
| `anm-int3-scale` | 56/56 |
| **Total** | **164/164** |

Reproduce the selected units only (not the all-unit historical replay):

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-vm-timer-add --unit anm-vm-timer-decrement \
  --unit anm-int3-scale --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored complete-source/PE/MAP/PDB native evidence:
`.analysis/gpt-web/20261107-active/anm-current.json`,
`anm-render9-index-word-trials.py`, `anm-render9-index-word-results.json`,
`anm-render9-counter-selected-replay.json`, and
`build/gpt-web/20261107-anm-render9-index-word/`.
The installed `src/AnmManager.cpp` SHA-256 is
`3ab26d964232575916055f24a0b09dffb32bc059b0a8fced7751d815a290f677`.
No whole executable or runtime equivalence is established.
