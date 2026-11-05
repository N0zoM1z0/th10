# TH10 extended Enemy ECL: shared instruction header and pattern byte cursor

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The Enemy-specific dispatcher at `0x0040E770` and the generic VM at
`0x0044E1A0` consume the same variable-length ECL instruction format.
`EnemyEclInstructionView` formerly described header words at +0 and +6 as
unknown and stored the 16-bit indirect-operand mask only as a primitive word.
It now uses `time`, `size`, `difficultyMask`, `operandCount` and the shared
`EclVmOperandIndirectMaskView` at +8, preserving the raw `operandMask` alias.
Compile-time `offsetof` checks compare its fields to independently reviewed
`EclVmInstruction` at every relevant wire header offset, with the payload
still beginning at +0x10. This does not reinterpret the variable-length
stream as a fixed-length allocation or guess original identifiers.

The real `EnemyRuntimeView::DispatchEclInstruction` source also now binds
byte-scaled bullet-pattern accesses through the scoped, fully inline
`EnemyPatternAtRuntimeByte(int byteAddress)` helper. The byte cursor remains
integer-typed at its original expression sites; the helper applies the
already layout-asserted `bulletPatterns` offset before accessing the concrete
0x210-byte pattern fields. The original instruction/read sequencing and
codegen-sensitive pointer lifetime remain intact; no large typed pointer
is hoisted across VM reads.

## Bounded actual native comparison

Fresh pinned VC7.1 SP1 **11-translation-unit** `/GL` build with the changed
header and dispatcher source, compared against the preceding complete native
source graph, retained the entire **14,352-byte** candidate ECL dispatcher
with **716** same-position/type linkage fields and **zero differing ordinary
bytes** after normalizing candidate fields. The complete BeginSpell (2,432
bytes, 108 fields), death-finalizer (162, 9), viewport (330, 10) and
full Enemy constructor (672, 5) likewise preserve size, field geometry and
ordinary bytes. A separately selected, original-target-bound strict replay
of `enemy-read-int-argument` also passed **16/16** bytes; this uses an
explicit `--unit` selector, not a full historical cold cohort.

Private reproducer and result:
`.analysis/gpt-web/20261105-enemy-shared-header-pattern/verify.py`,
`build/gpt-web/20261105-enemy-shared-header-pattern/current/native-result.json`,
`.analysis/gpt-web/20261105-enemy-shared-header-pattern/read-int-exact.json`.

**No new exact credit:** the original extended dispatcher is **14,416**
bytes; its 14,352-byte complete candidate remains **NONEXACT**. Equal
candidate bytes are a source-regression guarantee, not original-byte equality.
No native product/runtime/portability claim is made.
