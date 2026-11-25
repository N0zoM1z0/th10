# TH10 reconstruction handoff

Start with [AGENTS.md](../AGENTS.md), then read the selected owner's source
and ledger rows. [PROGRESS.md](PROGRESS.md) holds the generated totals.
Use [matches.csv](../config/matches.csv) to select remaining work and
[match-units.toml](../config/match-units.toml) to find the exact recipe.

## Resume

The active owner is `EclVmContext::StartSubroutine` at `0x0044DF70`
in `src/EclVm.cpp`. The target owns 550 bytes and receives the destination
in EAX with two stack arguments and RET 8. The two-source
`EclVmHostRunEnemy` link emits the full extent and binds all four calls.
The strict comparison matches 459 bytes. The same image matches
`ecl-vm-spawn-thread` and `ecl-vm-host-run`.

Keep the original Japanese TH10 v1.00a target at the read-only path
`resources/th10.exe`. Use pinned VC7.1 SP1 build 6030, a single available
CPU at nice 10, and explicit unit selectors. Repository commands run
locally. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) identifies the retained inputs.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout -k 10s 120s taskset -c 2 nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-start-subroutine
scripts/repo-python .analysis/ecl-start-subroutine/compare.py EclVmHostRunEnemy
scripts/repo-python scripts/compare-linked-function.py --unit ecl-vm-start-subroutine --json
scripts/repo-python scripts/compare-linked-function.py --unit ecl-vm-spawn-thread --json
scripts/repo-python scripts/ci.py && git diff --check
```

The caller reload after the argument loop puts the caller in EDX and
the four-byte push size in EBX. From the loop's last branch at +0x114
through RET 8, the return-frame and lookup instructions now align.
Resolve the zero and cursor registers in the prefix, the destination
cursor setup at +0x64 and the conversion path beginning at +0xAD.
The earlier ordinary caller parameter gave a closer prefix and widened
the return-frame block by one byte. Sharing the two branches' final push
through a pointer lost a separate capacity check.

Use `scripts/check-frontier.py FRONTIER` for an existing nonexact entry in
[frontiers.toml](../config/frontiers.toml). Product closure, runtime semantics
and portability remain open.

## Recent exact checkpoints

| Unit | Extent | Reproducible link |
| --- | ---: | --- |
| `ascii-manager-draw-gui-strings` | 570 B | `StageViewport`, 28 fields, both quad paths and the EBX draw callback. |
| `gui-destructor` | 574 B | `GuiDestructorCore` with `/EHsc` for all five inputs. All 37 fields, 18 local branches and seven unwind actions match. The receiver is on the stack and the function returns with RET 4. |
| `game-reset-gui` | 1,965 B | `GameManagerUpdate`, 52 fields and 73 local branches. Both GameManager calls and the ScriptViewer call passed cold with an EDI receiver. |
| `replay-manager-begin-stage` | 582 B | `ReplayBeginStageCore`, 25 fields and ten local branches. The position helper uses EAX and ECX here and matches 83 bytes in its default-profile cold link. |
| `ending-initialize` | 584 B | `EndingInitialize`, 30 fields and an EBP+4 allocation cleanup slot. The link includes the real PBG opener, text constructor and both registrations. |
| `gui-score-notice` | 668 B | `GuiScoreNoticeCore`, 35 fields and seven switch destinations. The spell-end caller passes in the same graph. |
| `enemy-dispatch-ecl` | 14,416 B | `EnemyEclDispatcher`, 726 fields, 118 jump destinations and 181 opcode selectors. The selected replay includes dialog allocation EH. |
| `ecl-vm-run` | 7,020 B | `GameManagerUpdate`, 189 fields, switch data and 62 local branches. |
| `anm-execute-script` | 9,964 B | `GameManagerUpdate`, 350 fields and 95 local branches. Per-case child attachment drives allocation before tail merging. |

## Known comparison issue

The `ascii-manager-initialize` factory graph reports a call-binding
difference at +0x108. Relinking before and after the GUI draw source
change produced the same diagnostic. The 10-byte high-priority draw
callback now has its own cold proof in `StageViewport`.

The `gui-message-destructor` warm guard in `GameCleanupContext` differed by
eight bytes. Relinking the source from before the `GuiView` destructor work
reproduced the same eight differences. The direct GameManager cleanup and
ScriptViewer creation and release callers passed their own warm comparisons
with the final GUI source. Use the unit's own recipe for further checks.
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) records source and compiler findings.
