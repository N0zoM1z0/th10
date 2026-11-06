# Type-1 box cutter: real virtual owner and vector recovery — 2026-11-06

`EnemyLaserType1View::CheckCollisionBox @41EB00..41EF92` now implements the
existing virtual slot in the real type-1 runtime view. The disconnected
`EnemyLaserType1CollisionView` declaration/body is removed for this box method;
that provisional view still owns the separate circle candidate. Reviewed
ownership remains **1,171 bytes**, source presence is unchanged and **zero new
exact bytes** are claimed.

## Original target facts and source changes

All facts below were rechecked by dense decoding of the hash-verified original
Japanese TH10 v1.00a and direct reads of its vtable data. No new Ghidra
observation or boundary change is needed.

- `46DA10+18` contains `41EB00`. The existing type-1 header already declares
  this virtual method, and `Update` calls that slot. The body now implements
  that declaration with the actual ECX receiver and `RET C` interface.
- The whole target decodes to 1,171 bytes, ending with `RET C` at `+490`.
  Its prologue aligns ESP to eight bytes and allocates `0x344` bytes. Typed
  vector construction/assignment now preserves the real intermediate XYZ
  results and copies for origin, half-size, minimum, maximum and sample.
  The existing `AnmFloat3View` API is reused; no common header changes.
- Target `+1AE/+1D4` reject an ordered lower-bound violation; `+1C1/+1E7`
  reject an ordered upper-bound violation. Unordered comparisons pass these
  four rejects. The old positive `minimum <= point <= maximum` source rejected
  unordered values. The maintained body now uses the observed negated rejects.
- Target `+16E` clears sample Z **after** adding the initial vector to position.
  That explicit clear was missing. Step Z is still cleared before the polar
  call, and each loop still performs the target's Z addition.
- The live type-1 fields at `444/464/466` are now read through the existing
  request's `maximumLength/type/color`. The gap request remains zeroed over
  exactly `0x77` dwords, with speed eight and all genuine fields preserved.
- A single typed sample pointer feeds both the bounds receiver and Item Spawn
  argument. Native comparison confirms that this removes an extra `LEA` before
  Item Spawn and restores its call field at `+229`, the manager field at `+239`
  and the world-creator call at `+24F`.

The natural counter/vector structure was suggested by the MIT source
`matching2/msvc/src/game/laser/laser_rectangle_split.cpp` at upstream
[`d1f3121`](https://github.com/levzzz5154/th10-decomp/blob/d1f31212514781c928fba145d8207397c8fb5650/matching2/msvc/src/game/laser/laser_rectangle_split.cpp).
Copyright (c) 2026 levzzz; [retained license](../../licenses/th10-decomp-MIT.txt).
No upstream section controls, synthetic caller, stand-in callee or ABI model
is adopted. Maintained names/views do not establish original C++ spelling or
object-lifetime/portability facts.

## Complete native comparison and remaining frontier

The first current-source baseline is compiled early in a real four-input
graph: EnemyLaser, AnmManager, ItemManager and EnemyEclDispatcher. Pinned
VC7.1 `/GL /GS` and existing linker profiles are retained. Compiler/link jobs
are serial, CPU-bound, nice10 and time-limited. Follow-up probes recompile the
changed TU and nonincrementally relink unchanged source/header-pinned inputs.

| Complete candidate | Native extent | Fields | Observation |
| --- | ---: | ---: | --- |
| Previous collision-view body, fresh baseline | 1,061 | 22 | Old source and current four-input graph; historical 914/1,002 figures are other epochs. |
| Real virtual owner plus typed vectors | 1,209 | 20 | Recovers target `0x344` frame and vector temporaries; one extra sample-address reload. |
| Shared sample pointer, installed source | 1,209 | 20 | Removes that reload; the first 13 target field positions and semantic symbols agree. Ordinary register/stack encodings still differ. |
| Original complete owner | 1,171 | 19 | Six direct calls, including the out-of-line laser factory. |

The strict complete-owner comparator rejects the installed candidate because
its PDB contribution is `0x4B9`, against the reviewed `0x493`. Field-position
agreement is a diagnostic only. The target factory call at `+46A` is absent:
the current optimizer inlines the type-0 factory branch, introducing allocation
and constructor calls. The emitted standalone factory is 153 bytes versus the
151-byte target, the world creator is 116 versus 118, and Item Spawn code is
795 versus 793 (its complete physical owner also contains alignment/tables).
These callee/ABI contexts and loop/register scheduling remain open. No partial
owner, normalized-prefix or size-only exact credit is taken.

The installed image independently passes the full existing polar helper
**30 bytes** and playfield-bounds helper **91 bytes / four fields**.
The actual type-1 Update caller separately cold-passes **832/832 bytes / 34
fields** with the explicit `laser-type1-update` selector. No historical
cohort or host simulation/mutation campaign is run.

[native-result.json](native-result.json) pins current sources/headers, all 19
independently decoded target fields, complete candidate fields and artifacts,
strict rejection and the affected guard results. Private reproducible native
scripts/logs and disassemblies stay under
`.analysis/gpt-web/20261106-laser-type1-box/`; diagnostic builds stay under
`build/gpt-web/20261106-laser-type1-box/`. The selected caller's canonical
artifact remains `build/match-linked/LaserType1Update/`.
