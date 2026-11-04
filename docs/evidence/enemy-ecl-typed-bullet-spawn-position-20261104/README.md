# TH10 Enemy ECL dispatcher: genuine bullet-pattern spawn coordinates

Original target: Japanese TH10 1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The complete original EnemyRuntimeView::DispatchEclInstruction starts at
0x0040E770 and owns 14,416 bytes; it **remains NONEXACT**. This is a real
source-recovery change inside the large opcode dispatcher.

## Recovered native position fields

The FIRE_BULLET_PATTERN opcode stores the computed world-space launch X/Y/Z
at exactly runtime +0x2C8/+0x2CC/+0x2D0. They are the first three floats
at +0x04/+0x08/+0x0C within one of the eight actual 0x210-byte
EnemyBulletPatternView records. The source now models these as
spawnPositionX, spawnPositionY and spawnPositionZ in the existing
EnemyBulletPatternFieldsView (not a duplicate struct). It adds compiler
layout assertions for their three native offsets and retains all other
0x210-byte record layout obligations.

All five opcode accesses (three writes, two subsequent protection-distance
reads) now use those real record members. They preserve the already matched
word widths, floating-point arithmetic order, target protection branch
and native BulletSpawnDescriptorView call address. No new field storage,
runtime bounds checking, stack padding, register-forcing or target byte
patches were introduced.

## Whole native compiler proof and rejected probe

In the pinned MSVC 7.1 SP1 11-source Enemy update /GL graph, the edited
whole PDB-owned dispatcher remains **14,348 candidate bytes**. All **716**
decoded native address fields retain their exact candidate offsets and
types, and all ordinary candidate bytes compare equal to the immediately
preceding selected source epoch after both candidates' identical fields
are normalized. The independently recorded edited-source SHA is in
[retained-native.json](retained-native.json), with retained native
PE/MAP/PDB under
build/gpt-web/20261104-live-enemy/native-bullet-spawn-position-20261104/.

A separate experiment typed a few g_Player draw-position reads using the
already checked Player drawPosition at +0x3C0; the whole candidate became
**14,352** bytes and changed address-field positions. Restoring the
remaining standalone cursor read did not repair that optimizer schedule.
Those typed Player expressions were **rejected**; the installed source
keeps the old target-preserving player-pointer expressions, and the
failed actual link is recorded in [rejected-player-native.json](rejected-player-native.json).

This is full-owner **candidate-to-candidate preservation**, not byte-exact
equality with the original 14,416-byte target. No new canonical exact
function or partial byte credit is claimed. Original translation-unit
provenance, complete executable and runtime equivalence remain open.
Only explicitly selected affected already exact units were cold-replayed;
the historical all-unit cold replay was not invoked.
