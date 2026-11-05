# TH10 score-file header and tail initializer: typed native storage

The sole original target is Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `GameStatisticsOwner::Load42B030` at `0x0042B030` is
423 bytes; the independent `InitializeStatisticsTail42ADD0` at
`0x0042ADD0` is 142 bytes.

The score-file header reads and writes `magic == 0x30314854` at +0
and `formatVersion == 3` in the lower 16 bits of +8. The real
`GameStatisticsFileHeader` now expresses those members without an
integer-to-halfword alias cast, with size + field-offset assertions.
The tail initializer likewise writes `magic == 0x5453` into the first
16-bit word and zero into the second halfword; `GameStatisticsTail`
now types both members, preserving the exact 0x448-byte record layout.
The unobserved higher format-version word is deliberately opaque.

After all these **actual installed source** changes, the selected
canonical VC7.1 `/GL` exact replays pass **423/423** for
`game-statistics-load` and **142/142** for
`game-statistics-initialize-tail`; this verifies every byte of both
complete owners and their canonical linkage obligations. There are
**565/565 accepted bytes**, but no newly admitted function because
both owners were exact before this source clarification.

The neighboring factory and constructor remain separate NONEXACT
frontiers: reference linked PDB candidates 98 vs target 97 for
`CreateStatistics42AF20`, and 137 vs target 132 for
`GameStatisticsOwner::GameStatisticsOwner`. Their equalities are **not**
claimed. Seven-stage lightweight CI, declared toolchain check and
`git diff --check` pass. No all-unit replay, runtime or product closure.
See `native-result.json` for installed source hashes and selected proof.
