# TH10 EnemySpawn: native active-enemy and spawn-counter INC words

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Original `EnemySpawn @0x0040CFB0` is a complete **577-byte** function.
It constructs and links a new enemy into the manager list, then increments
two signed 32-bit counters: `activeEnemyCount` and `spawnCounter` at manager
offsets +0x60 and +0x64. The original i386 at function offsets
`+0x1E4` and `+0x1E6` emits `INC EDX` and `INC ECX`, respectively, so
both values advance as native low 32-bit words even through `INT_MAX`.
The previous maintained C++ `++manager->activeEnemyCount` and
`++manager->spawnCounter` invoked undefined signed overflow at that bound.

The maintained source now uses explicit unsigned-word `+1u` operations
followed by publication to the original signed fields, retaining the
existing list insertion, effect-script selection, constructor and resource
handling, error paths and signed use of the counters (including the death
sound parity). No cap, new condition, or malformed-game-state validation
has been added; the source models the target's Win32 arithmetic.

## Three complete 14-TU original-codegen experiments

The real canonical `enemy-spawn` build graph was first **cold rebuilt from
current source**: ScriptViewer primary `/GL /GS /EHs`, together with 13
support sources including Enemy `/GL`, AnmManager, Chain, FrontEnd, Player,
Gui, BulletLifecycle, InputState, Main, ScriptViewerWorker/Lifecycle,
ItemManager and AnmVmId. Every source/header SHA and the emitted support
objects were pinned before changing any source.

Three separate natural C++ source variants were recompiled into that
graph, changing only the Enemy TU: active count alone, spawn counter alone,
and the installed combination. Each reproduced the complete **577/577
original EnemySpawn bytes, all 12 target-bound fields**. The adjacent
`EnemyManagerClear` remained **118/118** with two fields, and
`EnemyManagerSuspendCallbacks` **28/28**. The combined version was installed.

A separate selected **cold replay from installed source**, in the real
14-TU ScriptViewer graph and the small canonical manager-update graph,
verifies all five affected old-exact owners:

| Previously exact owner | Original bytes | Original fields |
| --- | ---: | ---: |
| EnemySpawn | 577/577 | 12/12 |
| EnemyManagerClear | 118/118 | 2/2 |
| EnemyManagerSuspendCallbacks | 28/28 | 0 |
| EnemyManagerUpdate | 166/166 | 5/5 |
| EnemyManagerUpdateCallback | 10/10 | 1/1 |
| **Selected total** | **899/899** | **20/20** |

These were already exact codegen owners: no newly accepted owner or
codegen byte credit is claimed, nor a full-game product/runtime proof.
Only the two affected graph families were replayed, not the historical
full cohort.

Selected reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-spawn --unit enemy-manager-clear \
  --unit enemy-manager-suspend-callbacks \
  --unit enemy-manager-update --unit enemy-manager-update-callback --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Private driver, complete PE/PDB/MAP and receipts:
`.analysis/gpt-web/20261108-enemy-spawn-counter-native.py`,
`.analysis/gpt-web/20261107-active/enemy-spawn-count-results.json`,
`.analysis/gpt-web/20261108-enemy-spawn-current-replay.json`, and
`build/gpt-web/20261108-enemy-spawn-native-counts/`.
