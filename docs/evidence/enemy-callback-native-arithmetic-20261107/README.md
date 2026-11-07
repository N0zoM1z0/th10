# TH10 Enemy callback: native life, countdown and penalty arithmetic

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EnemyResolveRuntimeCallback @0x004127A0` is a complete **593-byte
original function**, called twice from the large original 2,412-byte
`EnemyRuntimeUpdate @0x0040DC80` owner.

## Source changes

The actual `EnemyCallback.cpp` translation unit now explicitly includes
`AnmManager.hpp` before `Enemy.hpp`, which needs the complete
`AnmFloat3View` layout. Previously an independent VC7.1 normal-COFF
compile of this source failed in dozens of transitive headers; with this
include, it builds and exposes its complete callback function for direct
comparison.

Three real target x86 arithmetic paths use low-32-bit `SUB`/`ADD`, not
portable checked signed arithmetic:

- `lifeToNextCallback = life - lifeThreshold` is a 32-bit difference;
- the countdown computes `timerThreshold - current + 59` as native
  32-bit operations, then performs the **original signed** division by 60;
- the global timeout budget subtracts 3,000 before retaining the
  original signed 5,000 minimum check.

The source now models these intermediate values as unsigned Win32 words,
then republishes signed values at the original comparison/division sites.
There is no clamp, new error path or claim that corrupt values are safe.
The eight-record scan, timer reset, callback names, player-mask cleanup
and record disabling are unchanged.

## Entire-function native comparisons

Four natural VC7.1 source variants were normal-COFF compiled separately:
life only, countdown only, penalty only and all three together. All
produced the same complete **598-byte normal-COFF candidate**, with
seven relocations and unchanged comparative bytes. The combined source
was installed and **recompiled** in a real two-TU `/GL` producer:
`Enemy.cpp` (entry `EnemyRuntimeUpdate`) plus `EnemyCallback.cpp`.

| Whole PDB candidate owner | Before | Installed | Ordinary differences vs baseline |
| --- | ---: | ---: | ---: |
| `EnemyResolveRuntimeCallback` | 606 bytes, 7 candidate fields | 606 bytes, 7 fields | 0 |
| `EnemyRuntimeUpdate` | 2,371 bytes, 69 candidate fields | 2,371 bytes, 69 fields | 0 |

Both complete field offset/type/width geometries are also identical.
**The original callback has 593 bytes and the original runtime has 2,412
bytes. Both remain NONEXACT.** The 7 and 69 fields are *candidate* fields;
none is promoted to an original-bound exact-owner obligation by this
comparison. The source change is a Win32 arithmetic correctness and
compilation-recovery checkpoint, not a game-product or runtime proof.

Private reproduce/inspection inputs: 
`.analysis/gpt-web/20261107-active/enemy-callback-word-trials.py`,
`enemy-callback-word-results.json`, `enemy-callback-linked.py`, and
`build/gpt-web/20261107-active-enemy-callback/{baseline,native-words}/`.
No full historical cold replay was run; no canonical exact owner is
sourced by this callback TU. The lightweight seven-stage repository CI
and `git diff --check` remain the selected verification.
