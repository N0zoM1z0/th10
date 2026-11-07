# TH10 Enemy ECL: native pattern-copy, rank-speed and offset-vector index words

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete original `EnemyRuntimeView::DispatchEclInstruction @0x0040E770`
occupies **14,416 bytes**. The maintained pinned 11-TU VC7.1 `/GL /GS`
compiler graph emits **14,352 bytes** and 716 **candidate** field positions.
The giant owner remains **NONEXACT**: no new target exact credit.

## Source reconstruction

The script controls indices into an eight-entry 0x210-byte bullet pattern
array and a parallel 12-byte XYZ offset array. The original i386 code uses
low 32-bit IMUL/LEA/ADD words for these addresses. C++ `array[index]`
would form invalid/out-of-bounds array pointers for unchecked script indices
or negative indices. The existing `EnemyPatternAtRuntimeIndex` now also
handles both source/destination pattern-copy addresses and two rank-speed
stores, preserving their distinct argument/read positions. A new natural
`EnemyPatternOffsetAtRuntimeIndex` forms the 12-byte vector address by
unsigned native-word multiply and addition from the independently checked
`offsetof(EnemyRuntimeView, bulletPatternOffsets) == 0x1344`. The two
zeroing stores for initialized pattern offsets and two opcode
SET_BULLET_OFFSET coordinate stores use this same native view. No extra
branch, clamp, storage, fake ABI, assembler, or target executable patch.
These target-specific pointer casts are not portable or malformed-script
runtime-safety guarantees.

## Fresh source-pinned, full-owner native trials

A new baseline was compiled for the **current installed eleven source
files and sixty-two header pins** (including recently changed ECL inputs)
under pinned VC7.1 SP1. Six real codegen controls separately tested the
pattern copy, rank speed, vector x/y stores, initialization zero stores,
combined vector expressions, and all changes combined. An initial vector
trial suffered a Wine link page fault and was rerun successfully; no
failure result was counted as passing. The installed *combined* source
was then recompiled and linked independently against the same current
baseline, checking full PDB contributions and candidate field geometry:

| Complete current compiled owner | Bytes | Candidate address fields | Ordinary differences |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

All compared candidate extents and field offset/type/width geometry remain
unchanged. This **candidate-to-candidate result is not original-bound
exactness**, and the large original dispatcher is still 64 bytes longer.

Only the five directly related previously exact Enemy ECL argument leaves
were selected for a current-installed-source original-bound cold replay:
`enemy-read-int-argument`, `enemy-read-float-argument`,
`enemy-resolve-float-argument`, `enemy-read-raw-int-argument`,
`enemy-read-raw-float-argument`. All **87/87** original bytes pass.
No full historical cold replay or full-game runtime test was run.

Ignored local evidence and native images:
`.analysis/gpt-web/20261107-turn3/enemy-pattern-array-views.py`,
`enemy-pattern-index-results.json`, `enemy-installed-native.log`,
`enemy-typed-index-exact-guards.json`, and
`build/gpt-web/20261107-session-th10-enemy-v2/installed-all-typed-pattern-indices/`.
Original TU ownership and game runtime behavior remain unproved.
