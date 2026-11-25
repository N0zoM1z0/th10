# TH10 reconstruction handoff

Start with [AGENTS.md](../AGENTS.md), then read the selected owner's source
and ledger rows. [PROGRESS.md](PROGRESS.md) holds the generated totals.
Use [matches.csv](../config/matches.csv) to select remaining work and
[match-units.toml](../config/match-units.toml) to find the exact recipe.

## Resume

The largest remaining reviewed authored owner is
`AsciiManagerDrawGuiStrings` at `0x00401A50`, 570 bytes, in
`src/AnmManager.cpp`. Start from the real `ascii-manager-on-draw-high`
recipe. Its 10-byte callback already binds the renderer and establishes
the EBX receiver. The regular-string renderer at `0x00401760` is a
separate exact owner. Record the new selection here before editing.
Continue that owner through a strict cold match before switching.

Keep the original Japanese TH10 v1.00a target at the read-only path
`resources/th10.exe`. Use pinned VC7.1 SP1 build 6030, a single available
CPU at nice 10, and explicit unit selectors. Repository commands run
locally. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) identifies the retained inputs.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout -k 10s 540s taskset -c 2 nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit UNIT
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
timeout -k 10s 540s taskset -c 2 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit EXACT_UNIT --json
scripts/repo-python scripts/ci.py && git diff --check
```

Use `scripts/check-frontier.py FRONTIER` for an existing nonexact entry in
[frontiers.toml](../config/frontiers.toml). Product closure, runtime semantics
and portability remain open.

## Recent exact checkpoints

| Unit | Extent | Reproducible link |
| --- | ---: | --- |
| `gui-destructor` | 574 B | `GuiDestructorCore` with `/EHsc` for all five inputs. All 37 fields, 18 local branches and seven unwind actions match. The receiver is on the stack and the function returns with RET 4. |
| `game-reset-gui` | 1,965 B | `GameManagerUpdate`, 52 fields and 73 local branches. Both GameManager calls and the ScriptViewer call passed cold with an EDI receiver. |
| `replay-manager-begin-stage` | 582 B | `ReplayBeginStageCore`, 25 fields and ten local branches. The position helper uses EAX and ECX here and matches 83 bytes in its default-profile cold link. |
| `ending-initialize` | 584 B | `EndingInitialize`, 30 fields and an EBP+4 allocation cleanup slot. The link includes the real PBG opener, text constructor and both registrations. |
| `gui-score-notice` | 668 B | `GuiScoreNoticeCore`, 35 fields and seven switch destinations. The spell-end caller passes in the same graph. |
| `enemy-dispatch-ecl` | 14,416 B | `EnemyEclDispatcher`, 726 fields, 118 jump destinations and 181 opcode selectors. The selected replay includes dialog allocation EH. |
| `ecl-vm-run` | 7,020 B | `GameManagerUpdate`, 189 fields, switch data and 62 local branches. |
| `anm-execute-script` | 9,964 B | `GameManagerUpdate`, 350 fields and 95 local branches. Per-case child attachment drives allocation before tail merging. |

## Known comparison issue

The `gui-message-destructor` warm guard in `GameCleanupContext` differed by
eight bytes. Relinking the source from before the `GuiView` destructor work
reproduced the same eight differences. The direct GameManager cleanup and
ScriptViewer creation and release callers passed their own warm comparisons
with the final GUI source. Use the unit's own recipe for further checks.
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) records source and compiler findings.
