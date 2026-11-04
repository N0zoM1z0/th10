# TH10 full enemy ECL boss: recover native three-axis bullet-spawn storage

Target is original Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
The complete target dispatcher at 0x0040E770 has 14,416 bytes and remains
**NONEXACT**; the actual source changed is src/EnemyEclDispatcher.cpp
plus the established bullet-pattern layout in src/Enemy.hpp.

The 0x210-byte EnemyBulletPatternView record contains the native three
float spawn coordinates at offsets +0x04/+0x08/+0x0C. Before this change
these were three opaque dwords in the source declaration, and the ECL
FIRE_BULLET_PATTERN opcode used five raw runtime+index*0x210 loads/stores
(+0x2C8/+0x2CC/+0x2D0). The real record now exposes
spawnPositionX/Y/Z with compile-time offset checks. The opcode uses
the actual fields for all three stores and both squared-distance reads,
while retaining its original operand evaluation, exponent/float behavior,
guard and bullet-spawn call. No new shadow storage or variable-length copy.

## Whole-owner native proof and rejected control

The selected VC7.1 SP1 real 11-source Enemy runtime update /GL graph compiled
the edited ECL owner, with unchanged support pins SHA-checked. The complete
candidate PDB contribution remains **14,348 bytes**, has **716** linked
address fields in exactly the previous positions/types, and **zero** ordinary
byte changes versus the preceding candidate, after candidate-field masking.
The actual source SHA-256 and image path are in native-result.json.

A separate attempt also replaced three direct Player world position reads
with the already mapped Player::drawPosition member. That source variant
was physically compiled, but changed the full-owner size to **14,352**
and address field geometry. It was rejected *only at those uses* and is
recorded in rejected-player-reads.json. Target assembly and prior source
hypotheses were not assumed correct: the selected code was checked from
a fresh source hash and actual compiled result. The retained raw Player
reads keep the original optimizer lifetime, not an inferred layout change.

This remains 14,348 candidate vs 14,416 target: **no whole-original
exactness, no new canonical match, no partial-byte credit**. No full replay,
runtime/game-product or historical TU claim.
