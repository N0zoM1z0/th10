# TH10 bomb owner: exact scheduler registration and 0x48-byte factory

**Target:** Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete **84-byte** original owner at `0x004055C0..0x00405613`
registers the Bomb update/draw callbacks with the game chain; the complete
**85-byte** owner at `0x004056B0..0x00405704` allocates and constructs the
0x48-byte Bomb owner. Both are now full-byte **EXACT** under real-source
VC7.1 SP1 LTCG, with all external target identities strictly bound.

## Reconstructed lifecycle and independently checked boundaries

`src/ScriptViewerOwners.hpp` now describes the measured 0x48-byte bomb owner
with its actual `AnmVmTimerView` at +0x14 and timer flags at +0x24. It also
retains update/draw task pointers at +0x08/+0x0C and the state/cursor fields
consumed by the adjacent 223-byte update owner. `src/ScriptViewerBombCreate.cpp`
implements an ordinary C++ constructor, registration and failure-cleanup
factory. In the original factory the timer's default constructor clears flag
bit 0 at +0x24 **before** 18 DWORDs are zeroed. The owner flag bit 1 and the
real global `g_ScriptViewerBomb` at `0x004776EC` are published before
registration. Its null-allocation branch and conditional destructor/scalar
`operator delete` remain intact, not simulated or optimized away.

The registration body uses `th10::Chain::AllocElem` twice, retaining the
original **17/34** priorities, `heapAllocated` independent `flags |= 2`,
receiver stored in each element's `arg`, and the actual
`AddToCalcChain`/`AddToDrawChain` calls. The first callback target is the
independent `0x00405840`; the second is the already preserved six-byte
`th10::leaf::Fn00405850`. The source presently treats the first callback and
its 223-byte bomb Update body as real external interfaces to be reconstructed;
this exactness does **not** confer exact credit on either.

## Two source-selected real cold links

The reviewed canonical `script-viewer-bomb-register-tasks` proof compares
all **84 bytes** with **8** strictly target-bound external fields, including
the two callback addresses, two allocation calls, two chain globals and the
calc/draw registration calls. `script-viewer-bomb-create` compares all
**85 bytes** with **5** bound fields: `operator new`, the Bomb owner global,
registration, actual Bomb destructor and `operator delete`. Both PDB extents
are complete, with internal destinations reviewed by the strict comparator.

The selected real two-source VC7.1 `/GL /GS /EHs` graph uses
`ScriptViewerBombCreate.cpp`, `Chain.cpp` and `ScriptViewerLifecycle.cpp`.
The first limited cold also preserves the independently accepted **137/137**
Bomb destructor (15 bound fields), for **306/306** selected bytes. A second
independent cold over the new **169/169** bytes again passes; PE hashes and
PDB GUIDs differ. Receipt identities and exact source hashes are pinned in
[native-summary.json](native-summary.json).

No full cold replay, product link or runtime test was performed. The exact
bytes do not prove the original class spelling, physical translation unit,
compiler ancestry or ABI beyond the measured target and selected graph. Bomb
update @`0x00405750` (**223 bytes**) and the fastcall adapter @`0x00405840`
(**10 bytes**) remain separate follow-up targets.

Reproduce only selected relevant owners:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-bomb-register-tasks --unit script-viewer-bomb-create \
  --unit script-viewer-bomb-destructor --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-bomb-register-tasks --unit script-viewer-bomb-create --json
scripts/repo-python scripts/ci.py
git diff --check
```
