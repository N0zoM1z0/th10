# Popup-number tick/update: complete 134-byte exact function + 7-byte adapter

**Target:** original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The original `0x0042B6F0..0x0042B775` owner is **134 bytes**, and the newly
inventoried callback at `0x0042B9A0..0x0042B9A6` is **7 bytes**. The complete
real-source native LTCG copies pass **134/134** and **7/7** respectively, not
just a prefix or a same-length comparison. Neither function was declared
exact in the earlier popup-draw hypothesis packet.

## Recovered source semantics and revised origin

`ScriptViewerPopupOwnerView::Update` in
`src/ScriptViewerPopupCreate.cpp` traverses **723** fixed-size `0x40` popup
records from enclosing owner offset `+0x3C4`. Every active record decrements
its position Y by `g_AnmGameSpeed * 0.5f`, executes the genuine
`AnmVmTimerView::Tick` on its record timer at `+0x1C`, and becomes inactive
once `timer.current > 60`. Exactly these reads and writes, including the
float/int timer conversion and the constant-range fast path, appear in the
target. There are no added range checks, null guards, or alternative expiry
semantics. The existing `0xB884` storage layout and **134-byte exact
constructor** remain intact. The body is game-specific, actively mutates
frame data and is independently called by the update callback. The old
"indeterminate constructor-shaped" label at `0x42B6F0` is contradicted by
the target arithmetic and callback edge, so its reviewed origin is corrected
to **authored game**; this does **not** assert original class/TU naming.

The previously missing `0x42B9A0` **7-byte independent** callback contains
`MOV EAX,ECX; JMP 0x42B6F0`. It ends at the original jump instruction;
following CC padding runs through `0x42B9AF`. The preceding 540-byte Draw
owner at `0x42B780..0x42B99B` and following 167-byte numeric emitter at
`0x42B9C0..0x42BA66` do not overlap. Its fastcall adapter may be compiler
outlining or an authored callback; origin therefore remains **indeterminate**.

## Strict real-source proof and cold replay

The true four-TU source graph uses `ScriptViewerPopupCreate.cpp`, `Chain.cpp`,
`ScriptViewerLifecycle.cpp`, and `AnmManager.cpp` under the pinned VC7.1 SP1
`/GL /GS /EHs` profile. `Update` has six original target-bound fields:

| Original offset | Role | Original target |
| --- | --- | --- |
| +0x19 | `g_AnmGameSpeed` global | 0x00476F78 |
| +0x1F | `0.5f` | 0x00470B0C |
| +0x36 | `0.99f` | 0x00470B68 |
| +0x45 | `1.01f` | 0x00470B64 |
| +0x55 | `1.0f` | 0x00470AFC |
| +0x67 | real `__ftol2` | 0x00463B2C |

The callback's sole `+0x03` REL32 directly binds the Update function at
`0x0042B6F0`. Both images have the complete PDB extents and all internal
branches normalized only under the canonical comparator's verified
instructions. **Two independent selected cold links** reproduce every byte,
with distinct PE SHA-256 digests and PDB GUIDs (retained in
[native-summary.json](native-summary.json)). The first group also safeguards
the existing popup constructor **134/134**, including its global binding.
No historical all-unit cold replay or full-game build was run.

The 145-byte popup initializer, 71-byte popup factory and 540-byte Draw body
are still **NONEXACT** and gain no codegen credit from this result. The factory
produces 71 native bytes but differs in ordinary register/control-flow bytes;
its 5 native linked fields are not a proved target-field identity contract.
Complete product closure, original source provenance, gameplay runtime and
portability remain separate questions.

Reproduce with bounded selected units only:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-popup-update --unit script-viewer-popup-update-callback \
  --unit script-viewer-popup-constructor --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-popup-update --unit script-viewer-popup-update-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```
