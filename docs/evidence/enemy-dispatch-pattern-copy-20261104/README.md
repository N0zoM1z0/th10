# TH10 enemy ECL 14,416-byte boss: typed complete bullet template copy

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Actual edited owner: `EnemyRuntimeView::DispatchEclInstruction`
(original `0x0040E770`, complete target **14,416** bytes, NONEXACT).

The `ENEMY_ECL_COPY_BULLET_PATTERN` opcode previously copied a manually
computed `(runtimeAddress + 0x2C4 + index * 0x210)` block through the
local `EnemyBulletPatternCopyView` (132 words, 0x210 bytes). The source
now selects the actual `bulletPatterns[index]` element from the established
`EnemyRuntimeView` layout on both sides of that exact 0x210-byte copy.
It preserves the explicit 132-word copy type and does not substitute a
partial subset, change indexing or invent a shadow template array.

## Compiler outcome and rejected control

The genuine 11-source Enemy runtime-update VC7.1 SP1 /GL graph was linked,
recompiling the edited dispatcher and currently changed Player source and
SHA-checking reusable unchanged objects. The full PDB contribution is
**14,352 bytes**, and its **716 decoded linked address-field locations
and types** remain identical to the preceding native candidate. After
field normalization, the compiled full candidate is byte-for-byte identical
against that previous compiled source epoch. This does NOT establish
14,416-byte target equality and creates no additional canonical exact
byte credit.

A separate attempted rewrite of the `FIRE_BULLET_PATTERN` coordinate loads
as `bulletPatternOffsets[index].{x,y,z}` was tested using the same true
whole-owner compiler. The direct C++ array form increased the contribution
to **14,368** bytes and changed field geometry, while an initialized local
pointer outside a switch-case block failed compilation (`C2360`). Those
controls were rejected, and the previous byte-preserving launch schedule
was explicitly retained. Their native outputs remain in ignored
`.analysis/gpt-web/20261104-live/typed-pattern-fire*-native-result.json`.

The source hash and whole owner comparison are recorded in
[native-result.json](native-result.json). Reproduction uses the ignored
`large-enemy-typed-link.py` and `build/gpt-web/20261104-live-enemy/typed-pattern-copy/`
PE/MAP/PDB. No full cohort cold replay, run-time correctness claim or
original-source TU attribution is implied.
