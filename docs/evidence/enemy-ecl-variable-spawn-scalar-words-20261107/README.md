# TH10 Enemy ECL: native scalar stream after counted spawn names

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original `EnemyRuntimeView::DispatchEclInstruction @0x0040E770` owns
**14,416 bytes**; the current pinned VC7.1 SP1 11-TU `/GL /GS` native
candidate owns **14,352 bytes with 716 candidate address fields**.
The large dispatcher is still **NONEXACT**, zero new canonical exact credit.

## Verified source semantics

Enemy creation ECL opcodes use a counted, variable-length inline subroutine
name followed by up to six four-byte scalar words. The maintained wire
`EnemyEclInstructionView::scalarOperands[1]` is only a first-word overlay
at independently established `+0x10`, not an actual one-element script
allocation. There were 28 active indexed reads of its `real`/`integer`
members with dynamic `iVar26 + 0..5` indices. C++ pointer arithmetic on
that declared one-element array can be undefined for perfectly ordinary
later script operands. Signed `iVar26 + 1..5` may also overflow for
malformed script-provided byte counts.

The new local force-inline `EnemySpawnScalarWordAt(instructionWord, index)`
forms the native low 32-bit instruction address plus real member offset
and the four-byte word index, and views the unchanged ECL `EclVmScalar`
integer/float bits. Each scalar-index expression converts the signed
cursor to unsigned **before** adding a fixed word offset. Evaluation order,
argument flag indices, word representations and all distinct opcode cases
remain unchanged; no range guard, alternate object, ABI override, heap
allocation, new target instructions or game-data modifications.
This is explicitly an i386 native-word source model, not a claim that
out-of-bounds data access is generally safe or that it is portable C++.

## Actual complete native trials and installed-source comparison

The eleven inputs and their 62 C++ headers were pinned in a fresh current
source native producer graph after the preceding pattern-index changes.
Three distinct VC7.1 source variants replaced only first-word reads,
only following `+1..+5` reads, or **all 28** reads. Each was compiled
and linked with a complete PDB source contribution. The all-reads version
was installed into the real repository and **recompiled independently**.
For every one of the five entire candidate owners, complete code bytes
and field offset/type/width geometry remain identical to the fresh base:

| Full PDB owner | Native bytes | Candidate fields | Ordinary differences |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

**This is candidate-to-candidate preservation, not comparison success
against the original 14,416-byte dispatcher.** Its original completeness
and physical source TU remain unknown. The five selected existing exact
Enemy ECL argument leaves were cold replayed separately from the actual
installed source and retain **87/87 original-bound bytes**. This selected
replay did not run the full historical cohort.

Private native evidence and reproducibility:
`.analysis/gpt-web/20261107-turn3/enemy-spawn-stream-word-probe.py`,
`enemy-spawn-stream-results.json`, `enemy-spawn-installed-native.log`,
`enemy-spawn-stream-exact-guards.json`;
`build/gpt-web/20261107-session-th10-enemy-v2/installed-spawn-stream-words/`
contains complete PE/MAP/PDB and five-owner comparisons.
No full game product or runtime proof is claimed.
