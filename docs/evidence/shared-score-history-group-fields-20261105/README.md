# TH10 score-state group selectors shared by spell, player, game and replay

Original Japanese TH10 v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

At original `EnemyEndSpell` addresses `0x00409CCE` and `0x00409CD3`, the
code reads global dwords at `0x00474C68` and `0x00474C6C`. The
`GameScoreStateView` global at `0x00474C40` establishes these as +0x28/+0x2C;
the two values select one of the statistics rows by `groupA*3 + groupB`.
These are the same reviewed fields used by the `EnemyBeginSpell` group
selector, the player resource/shot option initializer, the GameManager
starting-stage statistics lookup and ReplayManager's character/shot variant.
The maintained code previously duplicated anonymous reinterpret-casts of a
20-byte opaque block in five translation units.

The existing 0x14-byte `unknown28` array remains available as a union
alias, with `statisticsGroupA` and `statisticsGroupB` in a named overlay
and 12 opaque bytes following. Compile-time `offsetof` assertions secure
both offsets and the start of the remaining opaque bytes; the overall
score-state object retains its original reviewed 0x5C-byte layout. Actual
`EnemyEndSpell`, `PlayerInitialize`, `GameManager` and `ReplayManager`
call sites now read the established named members, **without** caching
values across any previous reload or adding unobserved bounds checks.
The existing original ABI and original source spelling are not asserted.

Two actually selected VC7.1 `/GL` target-bound canonical replays from the
changed source pass completely: `enemy-end-spell` **399/399 exact** and
`script-viewer-player-create` **69/69 exact**, including each registered
original linkage obligation. Both units had prior exact credit: no newly
exact function is claimed.

Separately, real VC7.1 normal COFF comparison compiled before/after source
variants for *all three* affected larger TUs `Player.cpp`, `GameManager.cpp`
and `ReplayManager.cpp`. Their original and updated object files contain
79/79, 24/24, and 23/23 functions, respectively, with **zero complete
candidate functions differing** in ordinary bytes or relocation geometry.
These are candidate-to-candidate comparisons only and cannot substitute
for target-bound `/GL` exactness of the nonexact long owners.

Seven-stage lightweight repository CI and whitespace checks passed; there
was no full historical cold replay, product build or runtime validation.
Current source SHA pins, selected replay and normal object counts are in
`native-result.json`. Local source comparisons (ignored):
`build/gpt-web/20261105-score-groups/normal-comparisons/`.
