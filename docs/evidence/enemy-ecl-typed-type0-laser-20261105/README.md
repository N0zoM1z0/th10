# TH10 complete ECL dispatcher: restore type-0 laser request members

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete `EnemyRuntimeView::DispatchEclInstruction` original owner at
`0x0040E770` is **14,416 bytes**. The two `FIRE_LASER_A` opcode variants
previously wrote the `EnemyLaserRequestScratch` through anonymous halfword,
float-word, and integer-word casts. They now write the **real reviewed**
`EnemyLaserType0RequestView` fields (type +0x24, color +0x26, angle +0x0C,
speed +0x20, initial/maximum/terminal lengths +0x14/+0x10/+0x18,
width +0x1C, and flags +0x28). The existing `EnemyLaser.hpp` native size and
`offsetof` assertions establish these positions independently. The original
per-operand evaluation sequence, clearing loop, optional 0x6C-word pattern
copy, flag bit transition and `EnemyFireLaser` call are retained.

The real VC7.1 `/GL` full **eleven-source** graph compiles the modified
14KB dispatcher and reuses ten SHA-verified unchanged support objects. Its
complete dispatcher remains **14,352 candidate bytes** with all **716**
linked fields at identical offsets/types, but there is **one ordinary
candidate-to-candidate byte difference**, at candidate `+0x271F`.
The changed SIB operand in the unrelated spell-name decoder is
`8A 0C 0E` (`[esi+ecx]`) -> `8A 0C 31` (`[ecx+esi]`). These addresses
are semantically equivalent; the latter SIB encoding is independently
observed in the corresponding **original** TH10 decoder, so the source
change has a grounded code-generation improvement. This does **not** mean
that the two owner offsets are identical (the original dispatcher is 64
bytes longer), nor that the whole dispatcher is exact.

Other independently complete native owners in the same graph—
`EnemyBeginSpell` 2,432 bytes/108 fields, `EnemyFinalizeDeath` 162/9,
`EnemyConfigureViewportOwner` 330/10, and the **context-specific** drop
core 505/27—have zero ordinary candidate differences and identical fields.
Target-endorsed selected exact replays also confirm `enemy-find-laser`
**22/22**, `enemy-set-laser-position` **23/23**, and `enemy-finalize-death`
**162/162**, totaling **207/207 previously credited bytes**. No new exact
owner credit is claimed. Lightweight 7-stage CI and whitespace checks pass.
No full historical cold replay, game-product closure or runtime test.

Linked artifact: ignored `build/gpt-web/20261105-laser-type0/full-ecl-boss/`.
See `native-result.json` for source hash and complete comparison rows.
