# TH10 shared 0x3C item-drop record across constructor, ECL and scatter core

Original target: Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`Enemy.cpp` previously carried an independent local 0x3C-byte drop record,
while `EnemyEclDispatcher.cpp` had a different local definition at the same
position in each runtime. Cross-checking real consumers established one layout:
main drop type +0, eleven counts starting at +4, unclassified word +0x30,
float X/Y scatter radii at +0x34/+0x38. The **one shared**
`EnemyItemDropCountRadiusView` now lives in `Enemy.hpp` with native size and
field offset assertions. The full-object constructor clears this record and
initializes both radii to 32.0f in the original order. The ECL dispatcher's
`SET_DROP_AREA` branch sets the corresponding real members. The 503-byte
item scatter core reads those same counts/radii. The externally observed
`int *` argument ABI remains unchanged. No hypothetical original C++ member
function or unreviewed additional vtable owner has been introduced.

Three separately selected canonical original-target replays from the **final
installed source** remain exact: `enemy-runtime-constructor` 145/145,
`enemy-drop-item-counts` 48/48 and `enemy-drop-item-counts-core` 503/503,
for **696/696 previously accepted bytes**. These functions were already exact,
so this batch receives **no new codegen credit**.

For the public-header change, a real eleven-source pinned VC7.1 /GL linked
graph was rebuilt: Enemy.cpp, EnemyEclDispatcher.cpp, EclVm.cpp and Player.cpp
were recompiled; seven support objects with unmodified source/header inputs
were reused. The **complete** `EnemyBeginSpell` candidate remains 2,432 bytes,
108 unchanged linkage fields, zero ordinary differences versus the prior
candidate. The complete ECL dispatcher remains 14,352 bytes, with 716 unchanged
fields, against its 14,416-byte original target and is still NONEXACT. The
330-byte camera owner and 672-byte constructor candidate also preserve their
respective codegen. The original constructor's reviewed 673-byte inclusive
boundary remains NONEXACT. Importantly the drop core in this **different**
large LTCG graph is 505 bytes with 27 fields (not exact); its canonical
503-byte exact result above comes from its registered dedicated caller graph.
Never conflate those contexts or promote target equality from similar size.

Seven-stage lightweight CI and diff validation passed. No historical full
cold replay, full-game build or runtime claim. The ignored local graph is at
`build/gpt-web/20261105-shared-drops/complete-enemy-scatter-direct/`; selected
replay and final source hashes are in `native-result.json`.
