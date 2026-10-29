# Player damage bounds and caller audit — 2026-10-30

The complete Player damage owner428630 remains **nonexact1175/1170**. This
checkpoint recovers the target0x40 frame and more of its shot/bounds schedule;
it adds **zero exact bytes**. The [full record](results.json) pins sources,
profiles, complete PDB extents, controls and the affected exact helper checks.

## Target observations and source hypothesis

The previous ten-input source emits1162 bytes with frame2C. Target min XY is
stored at ESP+38/+3C, target max at+2C/+30, shot min at+44/+48 and shot max X
at+20; shot max Y remains on x87 until comparison. The twelve-byte spacing is
consistent with full vector value homes. The old source used four independent
target-bound scalars and an invented two-component shot-bound record.

The candidate now uses the existing PlayerFloat3 type for all four bound values.
The planar test consumes only X/Y; Z is neither read nor assigned. This is a
temporary-type hypothesis, not proof of the original vector class. A consumed
shot pointer advances through128 entries, and each min-X assignment precedes
the Y half-extent computation. Those operations restore target frame40 and early
pointer/FPU scheduling while preserving descriptor reloads, rounded shot minima
and max-X, retained max-Y and late rotated target-size reads.

The iterator alone emits1175/frame30. Full-vector records restore frame40, and
the final complete cold candidate remains1175 bytes. Address-normalized aligned
instructions improve247/355 to276/355. This diagnostic masks addresses and is
not a byte-exact or partial-owner claim. The candidate exceeds the target extent;
bound-slot order, accumulation, factory temporaries and later geometry/helper
scheduling remain unequal. In particular candidate target-min/max and shot-min/max
homes are still permuted relative to target. No boundary or match is promoted.

## Caller audit and parked controls

Moving EnemyRuntimeUpdate's real eight-entry countdown earlier is byte-neutral
and restored. The old Player callee already saves/restores EBX on paths that use
it, so simple EBX destruction does not explain the caller's missing early count.
The new callee source also leaves the complete runtime candidate unchanged after
link-field normalization:2136/2136 ordinary bytes agree with its prior candidate,
and all89 decoded local jump destinations retain their relative offsets. This
preserves an existing nonexact candidate, not the original game's exact owner.
EnemyRuntimeUpdate remains2412/2412 with the previously recorded647/671 alignment.

Outer scalar declaration order is neutral. Shared bound scope/declaration order,
an existing vector type with an empty constructor and the alternative accumulation
expression are also neutral in the recovered-frame candidate. All are restored;
repeat only with new discriminating evidence. The remaining stack-home permutation
must not be hidden by normalization.

## Native verification

One independent cold rebuild compiles all ten game sources with zero cached
objects, retaining36 recursive project pins and the established explicit profiles.
The complete damage contribution fails extent equality; the runtime caller stays
nonexact. The directly used old PlayerSetShotVmInterrupt2 helper passes all65 bytes
and both target-bound fields in that same image. An explicit canonical cold replay
then preserves the same65/2 in its unchanged three-input constructor recipe.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit player-set-shot-vm-interrupt2 --json
scripts/repo-python scripts/ci.py
git diff --check
```

No header/interface, comparator, profile, match, origin, boundary or progress count
changes. No historical cohort or host failure scenarios run. Product/runtime and
original physical TU/type ownership remain open. Current cache and native records:
`.analysis/gpt-6.1-sol/enemy-residual-context-20261030/final-context.json`.
