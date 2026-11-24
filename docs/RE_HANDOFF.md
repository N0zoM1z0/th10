# TH10 reconstruction handoff

Start with [AGENTS.md](../AGENTS.md), then read the selected owner's source and
ledger rows. Exact reconstruction uses the pinned VC7.1 SP1 build 6030 compiler
and linker. [PROGRESS.md](PROGRESS.md) gives the ledger totals.
Product closure, runtime semantics and portability remain open.

Use the original Japanese TH10 v1.00a executable at the private, read-only
path `resources/th10.exe`. [The target manifest](../config/target.toml) gives
its pinned SHA-256. [Match units](../config/match-units.toml) define the
compiler recipes, [matches](../config/matches.csv) record exact owners, and
[frontiers](../config/frontiers.toml) record nonexact baselines. Reuse a
comparison only for its source and link graph.

## Recent exact checkpoints

| Unit | Complete extent | What to keep in the comparison |
| --- | ---: | --- |
| `hint-load-file` | 1,498 B | `HintReadLine`, 70 fields and the complete split, color, and duplicate-entry paths. The reader, splitter, constructor and list append pass in the same link. |
| `frontend-practice-update` | 1,968 B | `FrontEndPractice`, 125 fields, four phase destinations and the complete keyboard and cancellation loops. The existing Options update passes its own graph. |
| `hint-export-automatic` | 2,247 B | `HintWriteToOpenFile`, 181 fields, both name lookup tails and the nine outlined writer calls. The 76-byte writer also passes. |
| `player-update-movement` | 1,656 B | `ClaudeReplayInitLoadScript`, 54 fields and two eight-entry direction tables. Player update, hit response, option rebuild and state transition also pass. |
| `midi-load-track-file` | 448 B | `MidiStopPlayback`, six fields and the direct `ReleaseTracks` callee. Direct byte accesses in the endian helpers preserve the header copies and track loop. |
| `player-handle-collision-hit` | 627 B | `ClaudeReplayInitLoadScript`, 21 fields and two inline hit-effect creation sites. Player update, option rebuild and state transition pass in the same link. |
| `anm-render-manager-constructor` | 2,315 B | `StartupWorkerLifecycle`, 116 fields and three VM cleanup actions. The same link contains the exact 273-byte initializer. |
| `replay-playback-frame-control-callback` | 68 B | `FrontEndOptionsRefresh`, two DIR32 fields and the four short branches to the shared return. |
| `rebuild-player-options` | 2,520 B | `ClaudeReplayInitLoadScript`, 96 fields and six switch destinations. The link also covers Player update and state transition. |
| `front-end-update-music-room` | 2,796 B | `MusicRoomHelpers`, 106 fields and four phase destinations. The same link covers both line readers and row interpolation. |
| `player-update-callback-body` | 3,112 B | `ClaudeReplayInitLoadScript`, 112 fields and five state entries. The separate seven-byte adapter also passed its replay. |
| `frontend-options-update` | 3,188 B | `FrontEndOptions`, 129 fields and five switch destinations. Check the refresh and constant-interrupt members in the same link. |
| `enemy-dispatch-ecl` | 14,416 B | `EnemyEclDispatcher`, 726 fields, 118 jump destinations and 181 opcode selectors. The selected replay includes dialog EH and the drop-flush body. |
| `ecl-vm-run` | 7,020 B | `GameManagerUpdate`, 189 fields, switch data and 62 fixed local destinations. |
| `anm-execute-script` | 9,964 B | `GameManagerUpdate`, 350 fields and 95 fixed local destinations. Per-case child attachment controls allocation before tail merging. |

## Open owner

`FrontEndControllerView::UpdateReplay` at `0x004315C0` is the active owner.
The `frontend-replay-update` recipe produces the full 1,492-byte body and
six phase destinations in the replay-loader `/EHs` graph. That graph keeps
both replay file loads outlined and restores the 0x188-byte frame. The
current comparison has 183 differing bytes. Start with the phase-2 input
snapshot and the register rotation after creating the backdrop.

The [knowledge base](KNOWLEDGE_BASE.md) records target layouts, compiler behavior
and source forms worth reusing. Use `_reference/th10-decomp` for concrete
source hypotheses and verify them in the repository's TH10 graph.

## Resume commands

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout -k 10s 540s taskset -c 2 nice -n 10 scripts/repo-python scripts/check-frontier.py FRONTIER
timeout -k 10s 540s taskset -c 2 nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit UNIT
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
timeout -k 10s 540s taskset -c 2 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit EXACT_UNIT --json
scripts/repo-python scripts/ci.py && git diff --check
```

Use an available CPU for `taskset`. Select new and affected exact units
individually. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) lists the retained private
inputs and comparison contexts.
