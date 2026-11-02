# TH10: unified five-source ResultMenu lifecycle graph (2026-11-02)

The four already accepted, *source-authored* ResultMenu lifecycle functions
were previously proven in separate, selected source graphs. Their physical
owner sources now coexist in **one actual VC7.1 SP1 `/GL /GS /EHs`
linked PE/PDB**: `ResultMenuLifecycle.cpp`, `Chain.cpp`, `AnmManager.cpp`,
`ReplayManager.cpp`, and `ResultMenuCleanup.cpp`.

| Complete original function | Bytes reproduced | Target-bound external fields |
| --- | ---: | ---: |
| `ResultMenuView::ResultMenuView` at `0x4220E0` | **101/101** | 1 |
| `ResultMenuView::Initialize` at `0x422150` | **142/142** | 9 |
| `GameCleanupController422220` at `0x422220` | **311/311** | 18 |
| `GameCreateController` at `0x422360` | **67/67** | 5 |
| **Total, one selected actual artifact** | **621/621** | **33** |

All original local branch destinations in the four complete owners are
compared, including the 16 in cleanup. The cleanup uses the actual Replay
destructor, both Chain unregister operations, and the actual ANM manager VM
lookup. The factory's new, constructor, initialize, cleanup and delete calls
resolve to the real compiled symbols; their identities are not inferred from
name proximity or masked address locations.

Two independently selected cold links rebuild all five sources and replay
**621/621**, using distinct PE SHA-256 and PDB GUID identities. The exact
independent receipts are in [receipts.json](receipts.json). The canonical
`result-menu-constructor`, `result-menu-initialize`, `result-menu-creator`
and `result-menu-cleanup` units now use the same selected integrated graph,
not several independently cold artifacts. There is **zero additional exact
function or byte credit** from this consolidation: it strengthens source
integration evidence for the existing four owners.

`ResultMenuMarkTasks422200` remains an independent **28-byte nonexact**
source-present target. The original production physical translation-unit
separation, complete game executable link, runtime behavior and portability
are *not* claimed.

Scoped replay (no full historical replay):

```sh
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit result-menu-creator --unit result-menu-constructor \
  --unit result-menu-initialize --unit result-menu-cleanup --json
scripts/repo-python scripts/ci.py
```
