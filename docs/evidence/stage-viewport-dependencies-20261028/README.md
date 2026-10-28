# Main viewport dependencies in the Stage graph, 2026-10-28

This checkpoint separates two different target entries previously represented
by one `MainConfigureGameplayViewport(int)` declaration. It also recovers the
full-word fog cache and the actual fog-disable call in Main's rendering callback.
The complete Stage draw and Main Render parents remain nonexact.

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Disk verification, pinned VC7.1 execution, tracking and read-only local Ghidra
six-sample attestation passed. No Factory operation was used this turn.

## Target-local recovery

- Stage early draw calls0x405300 at0x402AEC. That60-byte owner accepts the
  supervisor inESI and camera index inEBX, selects the record at
  supervisor+0x154+index*0x118, publishes+0x384, calls0x421480, reloads the
  active pointer and owner device, sets the viewport and stores the index at+0x388.
- Main Render calls0x421480 directly at0x4393FF, withEDI=0x491E94. This is a
  **287-byte camera configurator**, not the60-byte selector. Its dimensions are
  unsigned; the eye/at use positive half-width/height, up is(0,-1,0), eye Z uses
  tan(pi/20), perspective uses pi/10, near1 and far10000. Both matrix setters
  are real D3DX imports; transforms2/3 and post-call renderer reload are retained.
- Fog disable0x420CD0/53 usesEDI supervisor, reads/tests a **DWORD** at+0x750,
  flushes before clearing the word, and returns the COM result on the nonzero
  path. Its zero path explicitly returns0. `long` models this observed result;
  original source return spelling remains unknown.
- Main Render0x4394C2 writes **DWORD 0xFF** to0x492378, then calls fog disable
  at0x4394CC. The old `cachedRenderState` byte declaration and `MainPrepareDraw`
  facade were incorrect. Main.hpp now has a four-byte `cachedFogEnabled` at+0x750,
  without moving the following0x778 timestamp. No byte alias is retained.

Fresh Ghidra xrefs find one selector call, two fog-disable calls, and six camera
calls. The four additional camera sites belong to the ANM layer path; this
checkpoint does not reconstruct or modify those callers. Target observations,
compiler results, maintained-source coverage and product/runtime remain separate.

## Native feedback

The first complete ten-TU StageCreate graph retains the canonical `/GL /GS`
profile and adds only the actual MainViewport.cpp support. It reports:

| Complete owner | First source | Capture/return correction | Target |
| --- | ---: | ---: | ---: |
| MainConfigureCamera | 294 | 290 | 287 |
| MainConfigureGameplayViewport | 60 | 60 | 60 |
| StageDisableSupervisorFog | 50 | 53 | 53 |
| StageDrawEarly | 1098 | 1098 | 1099 |
| StageDrawLate | 933 | 933 | 929 |

These are **full contribution sizes**, not byte-match scores. The second camera
model builds eye/at independently and captures aspect explicitly; scheduling and
FP temporary differences remain. No forced ABI, registers, inline annotation,
padding or compiler-profile change is introduced. Do not promote the290-byte
camera, the1098-byte early draw, or the933-byte late draw from numerical proximity.

A separate actual Main WinMain graph adds MainViewport.cpp to the existing
Main/screenshots ten-source context, retaining its AnmInt3Scale-only `/Ob1`
override. Complete Main Render is422/425 and camera290/287, both nonexact.
The three same-image guards Present236, release43 and screenshot service111
pass **390 bytes / all22 fields**. The new real call identities and full-word
cache store are checked in that retained image; its full Render is not accepted.

## Exact scope and source coverage

The two complete small dependencies are admitted only after two independent
final-source cold links and strict comparisons:

| Owner | Target | Bytes | Fields |
| --- | --- | ---: | ---: |
| MainConfigureGameplayViewport | 0x405300 | 60 | 1 |
| StageDisableSupervisorFog | 0x420CD0 | 53 | 2 |

The common graph is StageScript.cpp plus StageLifecycle, StageDraw, Chain,
AnmManager, AnmVmCreate, AnmVmId, RandomMath, FileSystem and MainViewport, with
StageCreate as the real entry. The selected PDB owner is
`support-08-MainViewport.ltcg.obj`. All113 owned bytes and all three semantic
fields are compared; the fog branch's local destination is not normalized.
The public camera call binds its entry address only: it grants **no camera-byte
credit**, and its complete290-byte native body remains nonexact.

Five existing Stage contracts (update304, Tick72 and three7-byte adapters)
retain397 bytes/all18 fields in both candidate images and both final cold links.
The original nine-TU recipes are not migrated and the full exact set is not
replayed. New source coverage is **400 bytes / three already-authored owners**;
new exact coverage is **113 bytes / two owners**. No denominator, boundary,
origin, physical production ownership, data-storage or runtime promotion follows.

## Host checks and limitations

The actual MainViewport.cpp bodies run against synthetic D3DX/COM sinks.
Checks cover unsigned high-bit dimensions, exact argument payloads, camera
indices0/1, post-call active-pointer reload, renderer/device replacement,
null renderer, full-word cache tests with only high bits set, and the observed
0/COM-result return paths. Four intentionally wrong source models (signed width,
byte-only cache, lost HRESULT and stale camera pointer) must assertion-fail.
The three Stage tests also pass after updating the explicit supervisor argument.
These are host source contracts, not i386 ABI, native x87 or D3DX runtime proof.

`report-main-corridor.py --check` rejects the source marker inventory. The same
failure is independently reproduced using the unmodified `a39ed78` source and
header: the old reporter expects ten markers while Main already contains fourteen.
No weakening or unrelated reporter change is included. Full-CI outcome is
reported separately in [results.json](results.json), not inferred from focused
checks. Product/runtime remain open.

The thirteen affected CI stages pass through the new opt-in `--only STAGE`
selector, including the four host groups, ledger/graph/progress checks and
documentation validation. No selection still schedules all119 stages: a dry
schedule control proves all118 old labels retain their order with only the new
Main-camera test added. An unknown stage returns1, rather than passing zero
tests. Dry scheduling is not full test execution; no full-CI pass is claimed.

Reproduce the two-unit cold group with:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit main-select-viewport --unit main-disable-fog --json
scripts/repo-python scripts/test-main-viewport.py
scripts/repo-python scripts/test-stage-draw-passes.py
scripts/repo-python scripts/test-stage-camera.py
scripts/repo-python scripts/test-stage-draw.py
```

Keep compiler jobs serial, one CPU at nice10, bounded with terminal logs.
Working producers and complete reports live under
`.analysis/gpt-6.1-sol/stage-viewport-20261028/`; canonical final artifacts are in
`build/match-linked/StageViewport/`, with the first final PE/MAP/PDB preserved
under `build/gpt-6.1-sol/stage-viewport-20261028/cold-a-frozen/`.
