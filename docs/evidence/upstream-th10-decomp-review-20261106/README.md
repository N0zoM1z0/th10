# Updated th10-decomp reference review — 2026-11-06

The existing MIT reference was pinned at
[`c6791d8` (2026-10-18)](https://github.com/levzzz5154/th10-decomp/commit/c6791d82dc9f02e55eca45a5314f900a85481efd).
GitHub main and a local fetch agree on
[`d1f3121` (2026-11-01)](https://github.com/levzzz5154/th10-decomp/commit/d1f31212514781c928fba145d8207397c8fb5650).
The GitHub comparison reports 42 commits and 264 changed paths. Native Git
diff finds 24 changed files under `matching2/msvc/src/`. The old checkout is
preserved at `_reference/th10-decomp`; the new ignored, detached worktree is
`_reference/th10-decomp-d1f3121`. These share the existing sparse checkout;
omitted manifest files were read directly from their pinned Git objects.
No upstream build scripts were executed.

## Useful source hypotheses

| Local owner / original address | Upstream change and practical use | Limit before local acceptance |
| --- | --- | --- |
| `TransitionShakeLinear`, `43C550`, 439 bytes | A named, explicitly float-converted product before division changes the x87 dependency graph and delays `FIDIV`. The new upstream recipe drops its synthetic callback entry and uses an ordinary TU. First local experiment below. | Our real initializer/callback graph and RNG representation must still match completely. |
| `EclVmScriptDatabase::AddScriptData`, `450220`, 589 bytes | Grow the name count through an `int&` helper, then read the stored count for allocation. Upstream also models Parse as vtable slot 0, with LoadChunks in slot 1. | Verify the real virtual interface locally. Their free argument uses `(old == previous) ? old : previous`, an identity selector intended to retain two register candidates. Do not adopt that artificial pressure control. |
| `EndingTextVmView::Run`, `40BD80`, 1,568 bytes | Plain animation-manager reads replace the old volatile macro. The ID passed as an expression, a small indexed ring loop and consistent text-writer interface recover scheduling without extra state. | Its accepted cluster includes special handle/text-writer stand-ins and relocated donor bodies. Use our maintained callees and real interfaces. |
| `ReplayManager::SaveReplay`, `429B60`, 1,681 bytes | Scope the compressor output parameter around compression/free; use independent write-count locals; preserve stage-count/score/slow-rate store order and genuine imported API declarations. The version formatter argument order was also corrected upstream. | Their score address is escaped by a synthetic function, and the entry is synthetic. Inspect our actual score users and call graph; do not copy the escape context. |
| `ReplayManager::Initialize`, `428F60`, 1,344 bytes | Six old barriers and a raw-address reload disappear after correcting the alias model of RNG/application state. This is a useful warning against accumulating scheduler controls. | Their graph retains another barrier and uses an escape function, parked factory and seven callee stand-ins. None is a local acceptance recipe. |
| Type-0 circle and type-1 box/circle collision, `41DD80` / `41EB00` / `41EFA0`, 1,238 / 1,171 / 1,108 bytes | Declare sample counters before copying the origin; use the appropriate sample pointer and scalar/vector expression; simplify the box guard to one hit-count test. Crucially, the visible polar helper preserves a register needed across the call. These are substantial next owners. | Reproduce with complete real polar, item, ANM and laser-spawn implementations. Pointer spelling alone did not close upstream's opaque-callee graph. |
| Bullet spawn/transform, `4067D0` / `406D90` | A real `4.0f` literal replaces an external float and an inert address expression; a per-command request is scoped inside its switch case, with explicit meaningful store order. These are natural alternatives to register/temporary perturbations. | Upstream regions are 1,472 / 1,604 bytes versus our reviewed 1,413 / 1,454 owners. Their regions include tables. Independently resolve the ownership difference before comparing or crediting bytes. |
| `MergeJoystickInput`, `44A190`, 833 bytes | Ordinary locals, direct capability reads and the Y-dead-zone expression recover spills naturally; a synthetic frame-filling record is removed. | The upstream caller is synthetic and supplies a private ECX argument convention. Only arithmetic/local-lifetime hypotheses transfer. |
| `AnmRenderManagerView::ExecuteScript`, `43EE30`, 9,964 bytes | Float-vector operations, strip-loop count use, wait-path source structure and scale-helper parameter order are new leads. | The cluster uses 29 stand-ins plus a global escape function; its scale stand-in order differs from the real helper body. Our current two-byte opcode-91 mismatch is not proved closed by this update. |

Pinned primary notes and source diffs:

- [Camera change](https://github.com/levzzz5154/th10-decomp/commit/bda121ab287c0378539ca9d92bd436600f524a09)
- [ECL name table](https://github.com/levzzz5154/th10-decomp/commit/6696a2752cf7c86dcae465741b17c7c895a6472e)
- [Ending executor](https://github.com/levzzz5154/th10-decomp/commit/8924bc645d60b0b4f14088c1cd1de890e7d659e7)
- [Replay save](https://github.com/levzzz5154/th10-decomp/commit/360749c5fa9a382df098b1083ca017c64fa188cb)
- [Replay initialization](https://github.com/levzzz5154/th10-decomp/commit/2e800958c775bf3ededded5e3424a9362b616de8)
- [Laser split analysis](https://github.com/levzzz5154/th10-decomp/blob/d1f31212514781c928fba145d8207397c8fb5650/matching2/reference/recovery_notes/laser_splits_0041eb00_0041efa0_exact_2026_10_25.md)
- [Bullet literal analysis](https://github.com/levzzz5154/th10-decomp/blob/d1f31212514781c928fba145d8207397c8fb5650/matching2/reference/recovery_notes/projectile_spawn_cell_004067d0_accepted_2026_10_14.md)
- [Joystick change](https://github.com/levzzz5154/th10-decomp/commit/8799666243c155c79e7fc93656b0c844ce2aed49)
- [ANM change](https://github.com/levzzz5154/th10-decomp/commit/aa3c5cad6865a0d2dfe86c0b1cc45010ff0ad5c2)

## What the upstream numbers mean

The pinned regions manifest reduces equivalent regions from 23 to 7,
promoting 16 entries. At review start fifteen of those entry addresses were
nonexact in our ledger; `GuiView::UpdateStageElements @414900` was already exact.
[inventory.json](inventory.json) records the address/source crosswalk and
manifest hash. Entries establish where to investigate, with no local credit.

Subsequent local complete-owner proofs accepted
[SaveReplay1681](../replay-save-1681-exact-20261106/README.md) and
[Ending Run1568](../ending-run-1568-exact-20261106/README.md), leaving13 of those
promoted entry addresses open. The inventory's review-start flags remain
historical; current exact counts come from the local generated ledger.

Upstream's current README reports 91.3% exact game bytes and an estimated
99.6% after weighting matching instructions within equivalent functions.
Its 100% recovered figure covers 2,322 regions / 322,064 bytes. Its EXACT
build substitutes assembly for the remaining equivalents, and some regular
source owners use pinned GCC/Clang profiles. These definitions, region
boundaries and compiler contexts differ from our confirmed-authored ledger.
We did not reproduce those build claims. The checked-in HANDOFF is dated
October 25 and still says 85.8% / 15 equivalents; it is stale relative to the
November 1 README and manifest.

The seven remaining upstream equivalent entries are ECL core `44E1A0`,
enemy dispatcher `40E770`, font-atlas fill `436DA0`, name entry `432CB0`,
item update `41AFD0`, ANM texture fill `4465B0`, and ECL child start `44DF70`.
The update therefore supplies no upstream exact solution for our large ECL
and enemy dispatcher frontiers.

## Local camera experiment

[camera-experiments.json](camera-experiments.json) pins the original target,
current baseline producer graph and every source/header input. The existing
14-input Gui/transition graph retains its pinned `/GL` profile and real
`GuiView::UpdateStageElements` entry; no upstream compiler/context recipe is
transplanted. The reviewed callback boundary and source mapping are unchanged.

The fresh baseline emits the complete **439 bytes**, with **412/439 matching**
and **20/20 original-bound fields**. Its 27 ordinary differences are the
instruction shuffle caused by `FIDIV` at `+B6` instead of target `+CE`.
The target's 19 absolute fields and one relative CRT call were independently
decoded from the original complete owner, then bound to the maintained
globals/constants/`__ftol2`; all fixed local branches remain compared.

The named outer-converted product emits 439 bytes but moves `FIDIV` earlier
to `+AA`; two DIR32 fields move from target `+AD/+B2` to `+B1/+B6`. The strict
comparator rejects its field inventory. A follow-up with explicit inner and
outer conversions and ordinary nonconst float locals gives the same complete
native body. This follow-up recompiled only the changed TU and performed a
nonincremental link, reusing the thirteen other objects after verifying all
unchanged source/header pins. The first trial rebuilt all fourteen inputs.
No independent acceptance cold was warranted for either rejected result.

Both controls are parked and the source is restored byte for byte. The
restored source's fresh baseline image also passes the affected existing
initializer guard, **396/396 bytes / 37 fields**. This does not credit the
callback. There are **zero new exact bytes**, no match/denominator changes,
no new comparison mechanism and no runtime claim. The original `/GL` graph's
different optimizer IR is a possible explanation; it has not been proved.
Further spelling sweeps require new evidence.

## Provenance and retained evidence

The reference remains MIT, copyright (c) 2026 levzzz. The retained license is
[th10-decomp-MIT.txt](../../licenses/th10-decomp-MIT.txt). Any adopted source
must cite its pinned source path/commit and preserve the applicable notice.
Original target identity, complete ownership, every field, callee ABI and
current-source cold reproducibility remain independent local requirements.

Private fetch log: `.analysis/gpt-web/upstream-th10-decomp-fetch-20261106.log`.
Review/native witnesses: `.analysis/gpt-web/20261106-upstream-review/` and
`build/gpt-web/20261106-upstream-review/`. The original target and old reference
checkout are unchanged. This review changes no comparator or toolchain.
