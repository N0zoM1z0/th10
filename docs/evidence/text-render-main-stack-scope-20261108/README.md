# TH10 554-byte main text upload: C++ local lifetime controls the exact stack frame

Original is TH10 v1.00a Japanese, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`TextHelperView::RenderTextToTexture @0x00437DB0` is a **complete 554-byte**
original owner. Before this change, the maintained source compiled to
554 bytes with **390/414 ordinary bytes** matching; all 24 different
ordinary bytes were stack displacements caused by `SUB ESP,0x18`
versus original `SUB ESP,0x14`. The original reuses the completed GDI
font-restoration stack home for the later surface and uploaded rectangle.

The maintained source now places the late `D3d9RectView sourceRect` and
`D3d9SurfaceView *surface` upload work inside a separate natural C++
block. The narrowed live ranges let the pinned VC7.1 SP1 compiler
emit the original **20-byte** stack frame without an assembly patch,
ABI cast, extra local, changed GDI call or error handling. The font
selection tree, two SelectObject restores, both text-shadow passes,
both alpha inversions, bleed, source rectangle, D3DX upload and
conditional surface release are unchanged.

## Real whole-source tests and original-bound fields

Five source-level alternatives were tested in the normal-COFF producer
(firstly font-phase scoping, initialized surface, moving its declaration,
late rectangle scoping, and declaration placement). **Only the late
rectangle scope** changes the frame to 20 bytes while keeping the complete
554-byte owner; from the formatted installed source, normal COFF proves
**414/414 comparable ordinary bytes**, with **35 COFF relocations**.

A full native **hybrid** 3-TU source graph, `TextFontRelease.cpp` `/GL`
with real `TextRenderer.cpp` compiled as normal VC7.1 COFF and
`RandomMath.cpp` `/GL`, retains the original caller and natural
member-function `ECX` ABI. It proves **554/554 original-bound bytes and
all 35 original fields**. These bindings were derived independently
from original PE instructions and the source COFF relocation symbols:
15 global GDI font slots, 7 buffer-field references with their exact
addends, the remaining base-object uses, GDI API pointers, two
`InvertAlpha` calls, alpha-bleed call and the D3DX upload. Each
referent, addend, call destination and field offset passed the strict
linked-PE comparator, not only masked ordinary-byte comparison.

The all-`/GL` TextFontRelease and TextBufferLifecycle contexts **do not**
produce the same call ABI: their LTCG optimizer chooses an EAX receiver
for `InvertAlpha`, causing 11 ordinary bytes to diverge from the target,
even though the stack frame is now correct. That context must not be
used to claim this owner exact. The actual private hybrid comparison
receipt is at `.analysis/gpt-web/20261107-active/text-main-original-fields.json`;
full current-source PE/MAP/PDB at
`build/gpt-web/20261107-text-main-hybrid/cold1`.

The **six previously accepted** text renderer exact owners were separately
rebuilt in their respective canonical graphs and pass **2,476/2,476**
original bytes, including the complete 946-byte alpha-bleed and
149-byte font release. In the mixed graph, the 149-byte font-release
caller differs by one byte; the already accepted font-release unit
continues to use its own correct all-`/GL` graph. This source change
must not overwrite that existing compiler context.

## Canonical exact admission — two distinct current-source cold builds

The previously provisional 554-byte source owner is now **accepted** as
`text-render-main` in `config/match-units.toml`, the sorted
`config/matches.csv` exact ledger, and `config/functions.csv` with its
reviewed authored origin. All 35 original relocation fields are
independently matched to real symbols, actual target addresses and exact
record-member addends. They are not masked as anonymous unknown dwords.

Two separate canonical `replay-exact-units.py --unit text-render-main`
source builds independently verify **554/554 code bytes, 35/35 fields**:

| Selected verification | New render owner | Other exact text owners | Strict total |
| --- | ---: | ---: | ---: |
| Cold 1, seven units / four artifact graphs | 554/554 | 2,476/2,476 | **3,030/3,030** |
| Cold 2, render + separate font release | 554/554 | 149/149 | **703/703** |

The main hybrid PE SHA-256s differ:
`8b25539bea7caed24ddffaf198ba2a7c2b2eca185a9b450add9467d88081ebe7`
and `35b68f1b086768750880c3605ab231a325d5092837472181faa6fc60079be919`.
The original source SHA remains pinned to
`0adc88ef7764554082d1feea916cbc9e211d5c5aa9dddb491f0174ac05522c75`.
Canonical current totals rise from **1,499 to 1,500 exact owners**, and
from **196,265 to 196,819 exact original bytes**: **+1 authored exact
owner, +554 bytes**. This acceptance is a **codegen equivalence claim**,
not an assertion of original C++ source spelling, physical source TU,
full executable product build, or runtime equivalence.

Focused exact guard:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit text-render-main --unit text-font-release-resources \
  --unit text-create-buffer --unit text-try-allocate-buffer \
  --unit text-release-buffer --unit text-fill-color \
  --unit text-buffer-apply-alpha-bleed --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Source SHA-256: `0adc88ef7764554082d1feea916cbc9e211d5c5aa9dddb491f0174ac05522c75`.

Independent canonical acceptance receipts:
`.analysis/gpt-web/20261107-active/text-main-canonical-cold-1.json`,
`text-main-canonical-cold-2.json` and private fully decoded
`text-main-original-fields.json`.
