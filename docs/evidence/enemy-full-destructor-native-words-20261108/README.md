# TH10 EnemyFullObjectView destructor: native count decrement and special slot words

Original target is Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EnemyFullObjectView::~EnemyFullObjectView @0x0040DAE0` is **386 bytes** in the
retail executable, reached from the scalar-deleting wrapper, and is distinct
from the **300-byte exact** EnemyManagerView destructor. Original instruction
`DEC dword ptr [ECX+0x60]` at `+0x5C` decrements the signed 32-bit manager
active-enemy count modulo 2^32, matching EnemySpawn's two native counter
increments. The maintained source formerly used signed `--activeEnemyCount`,
undefined at signed underflow. The new source subtracts 1u from its unsigned
word and republishes it to the same signed field without a new check.

The special-enemy slot is indexed by a script-derived managerSlot. Original
i386 forms the low-32-bit pointer table address and stores NULL. The maintained
`specialEnemySlots[managerSlot]` previously formed a potentially invalid C++
array pointer for out-of-range signed indices. The new cast computes a native
32-bit base plus unsigned index times the actual four-byte pointer stride.
Neither operation adds a cap or claims portable 64-bit behavior.

## Verified whole-function experiment, no credit inflation

A source-current genuine 14-TU `/GL /GS /EHs` ScriptViewerCreate graph was
cold built first, with the Enemy.cpp support TU compiled under its real `/GL`
override. Four separate natural C++ experiments then recompiled the Enemy
support object and linked the same real source graph: modulo-32-bit decrement,
unsigned decrement alias, native special-slot address, and both operations.
All four preserve the **complete 385-byte PDB-owned candidate** with **all
seven candidate relocation fields** and **zero candidate ordinary-byte
regressions**; the same-image exact owners also remain unchanged.

The installed combination was independently cold-replayed against the
original across the **two** affected canonical graph families:

| Previously exact owner | Bytes | Original fields |
| --- | ---: | ---: |
| EnemySpawn | 577/577 | 12 |
| EnemyManagerClear | 118/118 | 2 |
| EnemyManagerSuspendCallbacks | 28/28 | 0 |
| EnemyManagerUpdate | 166/166 | 5 |
| EnemyManagerUpdateCallback | 10/10 | 1 |
| **Total** | **899/899** | **20/20** |

The installed FullObject destructor is **NONEXACT**: target **386 bytes**,
candidate **385 bytes**, seven candidate fields only, with **55/357**
ordinary target-comparable bytes in the current 14-TU graph. No 300-byte
EnemyManager destructor result is attributed to this separate owner. No new
exact function or original-byte codegen credit, no historical full replay,
and no complete executable/runtime equivalence are claimed.

Reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-spawn --unit enemy-manager-clear \
  --unit enemy-manager-suspend-callbacks \
  --unit enemy-manager-update --unit enemy-manager-update-callback --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored trial source and complete PDB/PE/MAP evidence:
`.analysis/gpt-web/20261108-enemy-destructor-native-count.py`,
`.analysis/gpt-web/20261107-active/enemy-destructor-native-words-results.json`,
`.analysis/gpt-web/20261108-enemy-dtor-installed-replay.json`,
`build/gpt-web/20261108-enemy-destructor-words/`.
Source SHA-256 `bcbf59bf58b94432c0a1e350433e6715600c5bef6646e814d5b82e935d13f0bb`.
