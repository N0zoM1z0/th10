# TH10 enemy ECL bullet protection radius: typed binary32 storage

Original target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target's `ENEMY_ECL_SET_BULLET_PROTECTION_RANGE` computes the square
of its floating operand and writes a 32-bit float to EnemyRuntimeView +0x1454.
Bullet-pattern emission compares the same field against 0.0f and against
squared X/Y distance to the player. Its actual field is therefore a float,
not the previously declared anonymous `int` accessed via a cast. Recover
`bulletProtectionRadiusSquared` as an explicitly offset-checked float,
then use that member for both original interpreter paths.

Fresh, all-11-source VC7.1 /GL native compilation preserved the **entire**
14,352-byte generated dispatcher and the positions and types of all 716
candidate linkage fields, with **zero ordinary candidate-to-candidate byte
differences** against the pre-change source epoch. Companion owners:
`EnemyBeginSpell` 2,432/108 fields, death-finalizer 162/9,
viewport 330/10, full constructor 672/5, also with zero ordinary
candidate changes. Native build and complete PDB/MAP comparison are retained
privately at `build/gpt-web/20261105-next-boss/protection-float-typed/`.

Selected original-bound **cold** exact replays passed: runtime constructor
145/145, ECL state reset 71/71 and ECL context constructor 15/15
(total **231/231** bytes). The seven-stage lightweight CI also passed.
The original target dispatcher is 14,416 bytes: the 64-byte physical
candidate deficit remains, so this is a source/type recovery checkpoint,
**not** a new exact function, full-product or runtime claim.
