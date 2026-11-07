# Enemy ECL spell ID difficulty bias: modulo-32-bit arithmetic

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EnemyRuntimeView::DispatchEclInstruction` has three difficulty-adjusted
spell-start cases: add the current difficulty, difficulty minus one, or
difficulty minus two to the script's spell ID. Disassembling the *original*
full owner at `0x00410EE9..0x00410F06` gives the following instructions:

- `0x00410EE9..0x00410EF2`: read `g_EnemyDifficulty` and
  `LEA ESI,[ESI+EAX-2]`.
- `0x00410EF4..0x00410EFE`: read the same word and
  `LEA ESI,[ESI+ECX-1]`.
- `0x00410F00`: `ADD ESI,DWORD PTR [g_EnemyDifficulty]`.

These are **32-bit word** operations, including wrapped signed-limit cases,
not C++ signed-overflow or widened arithmetic. The maintained cases now
convert the original index and difficulty to `unsigned int`, apply modulo-
32-bit additions/subtractions, and publish the same word back to the native
signed spell ID. All operand reads, conditional switches and the real
`EnemyBeginSpell` call are unchanged. No script-validation or range guard is
invented; the conversion back to signed is a checked VC7.1/i386 model, not
a claim of C++ portability.

## Source-pinned native and scoped Oracle evidence

Only `EnemyEclDispatcher.cpp` was freshly recompiled and linked after the
previous source-pinned 11-translation-unit baseline (whose unchanged input
objects were hash-verified). Full complete-owner comparison with that
baseline yields **zero ordinary byte differences and identical field
positions/types/widths** for:

| PDB-owned function | Candidate bytes | Bound candidate fields |
| --- | ---: | ---: |
| Enemy ECL dispatcher | 14,352 | 716 |
| EnemyBeginSpell | 2,432 | 108 |
| EnemyFinalizeDeath | 162 | 9 |
| EnemyConfigureViewportOwner | 330 | 10 |
| EnemyFullObjectView constructor | 672 | 5 |

The native **original dispatcher is 14,416 bytes**, so the 14,352-byte
candidate remains NONEXACT, regardless of complete current-candidate
equality. This change gets no new exact ledger credit. A separately selected
canonical cold replay of the actual operand helpers:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument \
  --unit enemy-read-float-argument \
  --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
```

passes **73/73 existing exact bytes** with all bound real callees.
Seven-stage lightweight CI and whitespace checks pass. No whole cold replay,
complete product or runtime claim is made. The private current-build link,
source-PIN records and entire PDB owner proof are retained under
`build/gpt-web/20261107-enemy-flag-shift/spell-index-word-wrap/` and
`.analysis/gpt-web/20261107-enemy-flag-shift/`.
