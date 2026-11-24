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

## Open owners

| Owner | Where to resume |
| --- | --- |
| `MidiOutputView::LoadTrackFile` (448 B) | The 448-byte `midi-load-track-file` baseline has six differing ordinary bytes. The older 446-byte cursor-returning probe recovers the stack homes. See the MIDI notes in the [knowledge base](KNOWLEDGE_BASE.md) and the working notes at `.analysis/midi-event/results.txt`. |

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
