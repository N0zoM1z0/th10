# TH10 typed drop counts, reserved word and main-item reset — preserve exact caller/core

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained `EnemyDropItemCountsCore` and `EnemyDropItemCounts` now use the
same 0x3C-byte `EnemyItemDropCountRadiusView` established from enemy
construction and ECL handling. The 503-byte core's final clear is expressed
by the real `countsByKind[11]` and `unknown030` members, clearing exactly
**48 bytes starting at packet +4**, not the subsequent +0x34/+0x38 ellipse
radii. The 48-byte caller reads and zeroes `mainItemType` through the typed
record, retaining its original raw `int *` external ABI. All 11 kinds, RNG
steps, item spawn ordering and C++ integer comparisons remain unchanged.

The original-target pinned VC7.1 `/GL /GS` selected canonical replays pass
`enemy-drop-item-counts` **48/48** and `enemy-drop-item-counts-core`
**503/503**. These functions were exact before the edit; there is no new
exact credit. A separate full **eleven-source /GL ECL graph**, recompiling
the changed dispatcher and hash-checking ten reused support objects, preserves
the prior candidate bodies: full `EnemyBeginSpell` **2,432 bytes**/108 fields,
ECL dispatcher **14,352 bytes**/716 fields, `EnemyConfigureViewportOwner`
**330 bytes**/10 fields and `EnemyFullObjectView` constructor **672-byte
candidate**/5 fields, all with unchanged linked field geometry and zero
ordinary candidate-to-candidate differences. The original ECL dispatcher is
14,416 bytes; that owner remains NONEXACT. The 503-byte item core in this
different ECL graph still emits **505** candidate bytes, not a canonical
503-byte proof; do not conflate the two compiler contexts.

Only the affected exact units and local full-owner graphs were checked, plus
seven-stage lightweight CI and whitespace validation. No historical full
cold replay, product/runtime claim or exact-owner promotion.
Full local graph: `build/gpt-web/20261105-drop-finish/complete-enemy-drop-finish/`;
selected source hashes and comparisons: `native-result.json`.
