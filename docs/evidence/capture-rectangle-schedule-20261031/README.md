# Complete capture request scheduling — 2026-10-31

`AnmRenderManagerView::RequestCaptureRectangle @ 0x00424480..0x00424521`
(162 bytes) remains **NONEXACT**. This checkpoint improves its independently
bound complete-owner candidate from **139/162** to **148/162** in the same
actual two-input `/GL` graph, without assuming a provisional source note was
correct or giving partial exact credit. The target is original Japanese TH10
v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## Real callee interface and all owned bytes

The real result-menu pause caller and `src/AnmManager.cpp` support produce the
complete 162-byte `src/FrontEnd.cpp` PDB contribution, including 54 decoded
instructions, the negative-index early return, four integer conversions,
all capture-field stores and final `RET 20`. `FindVm` at `0x004491C0` has an
**EDX** input for the manager here. A standalone FrontEnd compile emits ECX
and must not be used as a false native acceptance context. Adding genuine
AnmManager support, without register annotations, recovers EDX and removes
one mismatch. This same context preserves the actual `FindVm` call body.

All six external fields are reviewed and strictly bound: the renderer global
`g_AnmRenderManagerView @ 0x00491C10`, actual `FindVm @ 0x004491C0`,
and four distinct calls to CRT `__ftol2 @ 0x00463B2C`. No local branch
or physical tail is cropped or masked. `verify-native.py` runs the unchanged
strict linked comparator over the **entire** owner; it must reject, not
accept, the remaining 14 ordinary byte differences.

The original maintained source wrote the destination X before source X and
destination Y. A complete A/B in the same `ResultMenuOpenPause`-rooted
two-input `/GL` graph showed **23 ordinary differences** after authentic
binding. Moving destination X after those two independent capture-field
assignments, without changing any values or outside calls, recovers nine
ordinary codegen bytes: **148/162**. This is a compiler scheduling model, not
proof of original C++ statement order. The 14 residual offsets are
`+64`, `+65`, `+67`, `+6B`, `+6C`, `+6E`, `+73`, `+7A`, `+7B`, `+7D`,
`+81`, `+84`, `+8F`, `+92`. They arise from the remaining destination store
schedule and register choices; all six address roles compare equal.

## Scoped independent source and callee preservation

The selected model is reproducible in a second fresh **two-input cold link**
with the same compiler profile and real entry. Its complete proof still
rejects with exactly 14 ordinary differences; a separately linked earlier
candidate records 23 differences. An independent affected canonical cold
replay selects **only** `result-menu-open-game-over` and
`game-complete-stage-transition`, which both bind the actual capture callee
through their production-like graph. They pass **411+411=822** complete
existing exact bytes, one shared artifact. No other FrontEnd units are
replayed merely because they share a TU.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python docs/evidence/capture-rectangle-schedule-20261031/verify-native.py \
  --artifact-dir build/gpt-web/newturn-front-end/final-capture-cold
scripts/repo-python scripts/replay-exact-units.py \
  --unit result-menu-open-game-over --unit game-complete-stage-transition --json
scripts/repo-python scripts/ci.py
```

The private experiments under `.analysis/gpt-web/newturn/` tested real caller
roots, `/GS` as a control, scoped read/write barriers, local aliases, and
field order permutations. Most barriers increased the complete mismatch
count, and changing caller roots without the genuine callee source left ECX
wrong. Adjacent-order controls from the best candidate plateau at 14; the
machine shape of independently observed fields, not a historical hypothesis,
remains the acceptance authority. The native PE, MAP, PDB, toolchain caches
and targets are ignored, never committed. Original source/TU ownership,
complete Windows product, runtime equivalence and portability remain open.
