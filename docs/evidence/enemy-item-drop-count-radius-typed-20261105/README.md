# TH10 complete 503-byte enemy item-scatter core: typed mixed count/radii record

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EnemyDropItemCountsCore` at original `0x0040C9D0` has a complete **503-byte**
canonical exact target, called from the separately exact 48-byte
`EnemyDropItemCounts`. The 60-byte scatter parameter previously appeared
only as `int *`, with 11 loop counts read at integer word indices 1–11 and
ellipse radii read by casting the same pointer to float words 13 and 14.
The installed narrow `EnemyItemDropCountRadiusView` overlays these exact
fields, including explicitly opaque word 0 and word +0x30. It names the
11 counts at +0x04 and the two binary32 radii at +0x34/+0x38, and checks
their native `offsetof` and record size 0x3C at compile time. The public
callee ABI remains the observed raw `int *` pointer, not an invented
historical C++ declaration. Original order of RNG reads, the x87
FSINCOS helper, 11 kind loops, and item spawning is unchanged.

Two separate source trials were compiled: typing the radii alone, then
both counts and radii. Both passed selected canonical **complete target**
VC7.1 `/GL /GS` linked replays, including all original linkage fields.
In the retained final version `enemy-drop-item-counts-core` remains
**503/503 exact** and its `enemy-drop-item-counts` parent **48/48 exact**
(551/551 previously credited bytes, no new exact credit). The complete
14KB ECL interpreter is not claimed target-exact and no new whole-function
boss match is inferred from these two accepted units.

Only selected cold replay and seven-stage lightweight CI were used, not
the full historical cold replay or a runnable game build. Source hashes
and selected receipts are recorded in `native-result.json`.
