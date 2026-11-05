# TH10 ECL interpreter: 16-bit indirect operand selector in full instruction header

Target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The variable-length `EclVmInstruction` header stores the indirect operand
selection word at **+0x08**. The existing scalar `operandFlags` is a 16-bit
word; before this change, the common `IsOperandIndirect` reader accessed its
untyped scalar directly, and no maintained field described its encoded role.
This source checkpoint adds `EclVmOperandIndirectMaskView` (one unsigned
16-bit word) and overlays it on `operandFlags` without adding any storage or
changing the serialized instruction layout. The common predicate now reads
`instruction->operandMask.bits`. Compile-time assertions bind both aliases to
+0x08 and the complete mask to two bytes. This view expresses the known
wire-level meaning; original source names and unused bits remain unknown.

## Current-source native evidence, not an exact admission

A genuine pinned VC7.1 SP1 two-source `/GL` link of `src/EclVm.cpp` and
`src/Enemy.cpp` with the original-bound 189-field manifest produces a complete
`EclVmContext::Run` PDB owner of **7,020 bytes**. All **189** independently
verified target fields, including local jump tables/destinations, pass; the
complete normal instruction comparison is **6,997/7,020** matching bytes, with
the same **23 non-field mismatches**. The full `Run` remains **NONEXACT**.

Six explicitly selected original-target exact leaf replays for `ReadInt`,
`ReadFloat`, `ReadIntValue`, `ReadFloatValue`, `ResolveInt` and `ResolveFloat`
pass **773/773** bytes. This is a scoped replay, not the historical full cold
cohort. `scripts/ci.py` passed all seven checks; `git diff --check` passed.

Private witnesses and reproducible commands:
`.analysis/gpt-web/20261105-ecl-mask/verify.py`,
`build/gpt-web/20261105-ecl-mask/current/native-result.json`,
`.analysis/gpt-web/20261105-ecl-mask/selected-exact.json`. These are scoped
compiler and target-byte witnesses, not a complete game or runtime claim.
