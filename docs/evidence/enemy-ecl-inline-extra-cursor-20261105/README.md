# TH10 ECL full dispatcher: preserve native codegen with per-access typed extra cursor

Target: verified Japanese TH10 v1.00a executable, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The full original `EnemyRuntimeView::DispatchEclInstruction` at `0x0040E770`
occupies **14,416 bytes**; the maintained function is still NONEXACT.

## Actual source recovery and rejected alternative

`ENEMY_ECL_SET_BULLET_EXTRA` (`0x199`) indexes an overlapping sequence of
24-byte `EnemyBulletExtraView` entries. The runtime origin is the already
verified pattern offset `0x2C4` plus the pattern field origin `0x20`:
`0x2E4`. There are 22 records per 0x210-byte pattern stride. The five
value/float field stores had five independent opaque pointer conversions;
all now use a single **forced-inline typed cursor function**
`EnemyExtraRecordAt(int baseAddress)` with that same checked offset and
existing signed integer base lifetime. The sixth `floatB` store deliberately
retains the original eight-byte unit indexed expression: the candidate
otherwise loses the target-relevant SIB address encoding. This is a
view of already shared storage, not a new heap array or a bounds check.

We also actually compiled an alternative that hoisted one persistent
extra-record pointer *before* all five argument reads. Although the PDB
function size stayed 14,352 bytes, its subsequent linked-field positions
moved and violated the original graph's field-geometry contract. That trial
was discarded and source restored. The installed approach instead creates
its typed pointer expression at **each use**, preserving the original
intervening VM reads and observable field ordering.

## Fresh full owner comparison and targeted exact guards

An independent pinned VC7.1 SP1 `/GL` compile of the current **11 actual
translation units** established a fresh source/header-pinned baseline.
The changed dispatcher translation unit was recompiled with the unchanged
10 support objects reused only after verifying the exact source and all
shared header hashes. Full linked-PDB owner measurements versus that same
source epoch:

- ECL dispatcher: **14,352/14,352 candidate bytes**, **716** same-position,
  same-type candidate linked fields, **0** ordinary candidate differences.
  The original target is **14,416**, so **still NONEXACT (64-byte gap)**.
- `EnemyBeginSpell`: **2,432/2,432 candidate bytes**, **108** same-geometry
  fields and no ordinary candidate differences. Still NOT target-exact.
- Enemy death finalizer 162 bytes and viewport owner 330 bytes: unchanged
  candidate codegen and all associated candidate fields.

Five selected original-bound exact replays of the installed modified source
pass: `enemy-ecl-resource-load-package` 239, `enemy-ecl-dispatch-forwarder`
11, `enemy-configure-interrupt` 48, `enemy-read-int-argument` 16 and
`enemy-read-raw-float-argument` 26 — **340/340 previously credited bytes**.
There is **zero new exact admission**. This does not assert original source
spelling, complete product closure or runtime equivalence, and does not
normalize the dispatcher's nonexact 64-byte gap.

Machine-readable source pins, full owner extents, field comparisons and
selected exact receipts are in `native-result.json`. Native private images,
map, PDB and object witnesses remain ignored under
`build/gpt-web/20261105-ecl-extra-slot/`. No full historical cold replay.
