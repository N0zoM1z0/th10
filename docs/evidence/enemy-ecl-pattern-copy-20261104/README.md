# Enemy ECL boss: restore the complete typed bullet-pattern copy

Original target is the attested Japanese TH10 v1.00a
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This modifies the *real* 14,416-byte physical
`EnemyRuntimeView::DispatchEclInstruction` target at `0x0040E770`; the
whole target function remains **NONEXACT**.

The `ENEMY_ECL_COPY_BULLET_PATTERN` opcode previously formed source and
destination addresses through `runtimeAddress + 0x2C4 + index * 0x210`.
The typed `EnemyRuntimeView::bulletPatterns[8]` is already independently
asserted at runtime offset `0x2C4`; its `EnemyBulletPatternView` and the
actual aggregate-copy view are both 0x210 bytes. The maintained source
now copies through `&bulletPatterns[uVar23]` and
`&bulletPatterns[uVar22]` while retaining exactly the original full
0x210-byte aggregate assignment, including its physical word layout.
This replaces opaque arithmetic with the actual runtime array owner;
it is not a new duplicate type or a partial pattern transfer.

## Native producer and rejected controls

A selected pinned VC7.1 SP1 `/GL` Enemy update graph containing 11 real
source inputs was compiled with the modified dispatcher. Changed source
input `Player.cpp` was compiled afresh, and unchanged support reuse was
SHA checked. The complete PDB contribution is **14,352 bytes** and all
**716 decoded native address fields** retain the same offsets and types.
After normalizing only the candidate's own address fields, all ordinary
bytes compare equal against the preceding typed-array source checkpoint.
This proves source-codegen preservation, **not** equality of the candidate
to the 14,416-byte original. No new canonical exact credit is added.

Two additional *real compiled* controls attempted to convert the FIRE
BULLET_PATTERN launch-offset vector reads to naturally typed array indexing:
one used a scoped named pointer and one accessed the array fields directly.
Both emitted **14,368** PDB bytes (16-byte regression) with different
field geometry, so neither was retained. An initial unscoped pointer
variant was rejected at compile time by VC7.1 (`C2360`: a switch label
would skip its initializer). The original launch-offset arithmetic remains
in source pending better register-lifetime evidence. Successful aggregate
copy and failed launch-offset controls are separately recorded in
[receipt.json](receipt.json) with actual source fingerprints.

Affected canonical helpers `enemy-drop-item-counts-core` **503/503** and
`enemy-read-raw-int-argument` **17/17** passed selected cold replay after
the source was restored to the accepted variant (**520/520 total**).
Normal repository CI passed. No whole-game cold replay, product build or
runtime correctness claim.

Private artifact locations:
`.analysis/gpt-web/20261104-live/typed-pattern-copy-native-result.json`
for the full-owner comparison, and
`build/gpt-web/20261104-live-enemy/typed-pattern-copy/` for its PE/MAP/PDB.
