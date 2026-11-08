# TH10 enemy teardown: native manager DEC and special-slot word address

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `EnemyFullObjectView::~EnemyFullObjectView @0x0040DAE0` is a
**386-byte** stack-bound teardown owner. It unlinks an enemy from the
manager list, clears its special-manager slot, releases managed VM IDs and
player tracking references. At +0x5C original i386 emits
`DEC dword ptr [ECX+0x60]`, decreasing the manager's signed 32-bit
`activeEnemyCount` as a low-32-bit word. The earlier maintained source
used ordinary signed `--activeEnemyCount`, which invokes undefined
signed overflow at `INT_MIN`. The natural C++ now subtracts 1u from an
unsigned representation, publishing the resulting low 32-bit word to
this same signed field, without changing the original list ordering.

The `managerSlot` is taken from enemy runtime data and used directly as
an index into the special-enemy table, without a range check. Target
machine code addresses the entry as a low-32-bit base plus four times
the index (`MOV [ECX+EAX*4+0x10],EBX`). The source now explicitly forms
that Win32 dword pointer address, rather than a C++ out-of-bounds
array expression; both original conditions and the NULL store remain.
These are target-specific unchecked arithmetic recoveries, not bounds
repairs, portable 64-bit pointer behavior or runtime safety guarantees.

## Four separate real 14-TU native codegen trials

The actual reviewed `enemy-spawn` graph was cold-built from **current**
source (ScriptViewerLifecycle primary and 13 real supports, including
Enemy, AnmManager, Player and BulletManager). Four variants were linked
in that complete VC7.1 SP1 `/GL /GS` graph by recompiling only the Enemy
TU: unsigned-word DEC, unsigned alias DEC, native special-slot address,
and both together. All four retained the complete **385-byte source
candidate destructor** with the same **seven candidate relocation fields**
and *zero changes to any nonfield candidate code bytes*. The original
owner is **386 bytes**, and the candidate compares only **55/357
ordinary original bytes** with 302 ordinary differences; it remains
explicitly **NONEXACT**, with no additional codegen credit.

All four trials independently protected the real **577/577** original
`EnemySpawn` owner, its **12 target-bound fields**, the **118/118**
`EnemyManagerClear` with two fields, and **28/28** callback suspension.
The combined source was installed and formatted. A separate installed-
source selected cold rebuild verified those three owners plus the
complete manager update/callback pair in the correct other context:

| Existing exact owner | Original bytes | Target fields |
| --- | ---: | ---: |
| EnemySpawn | 577/577 | 12 |
| EnemyManagerClear | 118/118 | 2 |
| EnemyManagerSuspendCallbacks | 28/28 | 0 |
| EnemyManagerUpdate | 166/166 | 5 |
| EnemyManagerUpdateCallback | 10/10 | 1 |
| **Total** | **899/899** | **20/20** |

The 385-byte destructor was separately inspected from that installed
source-linked PE/PDB and found to retain its same seven candidate fields
and 55/357 ordinary match. **No original-bound exact credit** is assigned
to it, nor do these proofs imply a completed TH10 executable.

Selected reproduction, avoiding the historical full cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-spawn --unit enemy-manager-clear \
  --unit enemy-manager-suspend-callbacks --unit enemy-manager-update \
  --unit enemy-manager-update-callback --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Private source-trial/PE/MAP/PDB evidence:
`.analysis/gpt-web/20261108-enemy-destructor-native-count.py`,
`.analysis/gpt-web/20261107-active/enemy-destructor-native-words-results.json`,
`.analysis/gpt-web/20261108-enemy-dtor-installed-replay.json`, and
`build/gpt-web/20261108-enemy-destructor-words/`.
Installed `src/Enemy.cpp` SHA-256:
`bcbf59bf58b94432c0a1e350433e6715600c5bef6646e814d5b82e935d13f0bb`.
