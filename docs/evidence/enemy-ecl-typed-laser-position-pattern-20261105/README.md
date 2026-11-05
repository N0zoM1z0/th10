# TH10 complete ECL boss: typed laser positions and copied pattern destinations

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The full 14,416-byte original `EnemyRuntimeView::DispatchEclInstruction`
includes the `FIRE_LASER` and `FIRE_STRAIGHT_LASER_2` branches. Their native
scratch requests are already reviewed as `EnemyLaserType0RequestView`
(0x1DC bytes) and `EnemyLaserType1RequestView` (0x1F8 bytes), overlaid in
`EnemyLaserRequestScratch`. Six anonymous `float*` indexed stores that
previously wrote three float3 position coordinates are now explicit
`type0.position.x/y/z` and `type1.position.x/y/z` accesses, preserving
read order, bit values and the existing vector-scratch lifetime.

The two optional 0x6C-word bullet pattern transfers also now name their
actual destination members instead of writing through `unsigned int*`
plus raw offsets: `type0.patternWords` at +0x2C (with a native `sizeof`
length) and `type1.patternWords` at +0x48. The latter retains its original
explicit 0x6C-step copy loop and the former the original `memcpy` shape.
A native `offsetof` assertion checks both positions and both pattern bases;
no unknown laser fields are reclassified or invented.

A pinned original VC7.1 `/GL` full eleven-source link recompiles the modified
ECL dispatcher, reusing ten SHA-verified unchanged support objects. The
**complete** ECL dispatcher remains **14,352 candidate bytes** versus
**14,416 original**, with **716** linked fields at identical offsets/types
and **zero** ordinary candidate-to-candidate differences. The same graph
preserves `EnemyBeginSpell` 2,432/108, `EnemyFinalizeDeath` 162/9,
camera 330/10 and context-specific drop core 505/27. These are
candidate-preservation comparisons, **not** a claim that the 14KB original
is now exact.

Affected selected canonical original-target replays still pass
`enemy-set-laser-position` 23/23, `enemy-find-laser` 22/22, and
`enemy-finalize-death` 162/162 (**207/207 previously exact bytes**).
Seven-stage lightweight CI and whitespace validation pass; no whole
historical cold replay, product closure, or runtime acceptance was run.
Complete source pins and local receipts are in `native-result.json`;
compiled artifact: ignored
`build/gpt-web/20261105-laser-positions/full-ecl-boss/`.
