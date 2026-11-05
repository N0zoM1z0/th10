# TH10 EnemyBeginSpell: typed world position and spell-history rows

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EnemyBeginSpell` at `0x00409280` has a **2,432-byte** complete target.
The complete ECL dispatcher at `0x0040E770` is independently **14,416 bytes**.
Both remain **NONEXACT**.

The spell-start visual effect sources its position from full enemy
`+0x1068`—the existing independently layout-checked
`EnemyFullObjectView::runtime` at `+0x103C`, then
`EnemyRuntimeView::worldMotion.position` at runtime `+0x2C`. The source
now reads this real 3D world-position member, preserving the old 12-byte
bitwise struct copy into `EnemyGameStateView::effectPosition`.

The original retail `EnemyBeginSpell` instructions at `0x409336` and
`0x40936B` multiply the statistics group index by `0x437C`; at
`0x40937A`, `0x409389`, `0x4093AA` and `0x4093B8` they access
entry-relative `+0x84` as the capped capture count. The two distinct
spell-history entry locations are group-strided base `+0x5A4` and the
shared overall base `+0x19A8C`, both with `0x90` spell-index stride.
The maintained `EnemySpellHistoryEntryView` therefore models the actual
name starting at `+0` and `captureCount` at `+0x84`, with opaque eight-byte
suffix and native `sizeof`/`offsetof` assertions. The original separate
statistic-record reload, `strcpy` ordering, capped count and second
increment path are unchanged; this is **not** an invented original name.

The original VC7.1 /GL **eleven-source** graph was linked with the
modified full source; unmodified support objects are reused only under
source/header SHA-256 checks. Full PDB bodies after the typed changes:

* EnemyBeginSpell: **2,424** candidate bytes vs 2,432 target,
  108 linked fields at unchanged offsets/types, **0** ordinary
  differences relative to the preceding full candidate.
* EnemyRuntimeView::DispatchEclInstruction: **14,352** candidate bytes
  vs 14,416 target, 716 linked fields at unchanged offsets/types,
  **0** ordinary candidate differences.

These are candidate-to-candidate codegen preservation results, not an
original-target match; no new canonical exact credit. The local complete
artifact is `build/gpt-web/20261105-followup-ecl/spell-typed-history-record/`.
