# TH10 reconstruction handoff

Start with [AGENTS.md](../AGENTS.md), then read the selected owner's source
and ledger rows. [PROGRESS.md](PROGRESS.md) holds the generated totals.
Use [matches.csv](../config/matches.csv) to select remaining work and
[match-units.toml](../config/match-units.toml) to find the exact recipe.

## Resume

The active owner is `EnemyGameStateView::~EnemyGameStateView` at
`0x00408AF0` (402 bytes). Start from the real `CreateEnemyGameState`
caller and link the actual ANM and Chain owners. The target takes the
receiver on the stack, keeps one ANM manager across the three VM
deletions, and unwinds arrays of 2, 8 and 5 VMs.

Keep the original Japanese TH10 v1.00a target at the read-only path
`resources/th10.exe`. Use pinned VC7.1 SP1 build 6030, a single available
CPU at nice 10, and explicit unit selectors. Repository commands run
locally. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) identifies the retained inputs.

### Parked owners

`ScriptViewerView::~ScriptViewerView` at `0x0040A1A0` has a strict
412-byte cold match with 44 bound fields and both unwind actions in
`ScriptViewerDestructSmall`. The factory and eleven related entries pass
in the same image. The standalone VM destructor, GUI and ASCII
destructors, GUI cleanup, startup worker destructor, enemy spell panel
constructor and ANM render manager constructor pass in their affected
cold links. The stage constructor and destructor guards still need to
qualify the inline VM destructor in `src/StageLifecycle.cpp`, where the
stage link now emits it. Finish those guards before promoting
`script-viewer-destructor` to the match ledger.

`EclVmContext::StartSubroutine` at `0x0044DF70` remains at 536/550 bytes
with all four call fields resolved in the `ecl-vm-start-subroutine`
comparison. Its latest source separates encoded operands from their
resolved values and uses one typed descriptor reader. The two-source
cold link reproduces the same 14 register-only differences and passes
`ecl-vm-spawn-thread` and `ecl-vm-host-run`; the separate
`ecl-vm-run` link also passes. The allocator and late tail merge findings
are in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md).

Use `scripts/check-frontier.py FRONTIER` for an existing nonexact entry in
[frontiers.toml](../config/frontiers.toml).

## Recent exact checkpoints

| Unit | Extent | Reproducible link |
| --- | ---: | --- |
| `laser-type0-bounce` | 410 B | `LaserLineUpdate`, 17 fields and the real type-0 virtual slot. Both spawn calls take the manager in ESI and the request in EDI. |
| `startup-worker-destructor` | 417 B | `StartupDestructorCore`, 40 fields and nine local branches. Both member unwind actions bind to their concrete destructors. |
| `frontend-stage-scores-draw` | 419 B | `FrontEndReplaySaveDraw`, 16 fields and 17 local branches. The callback supplies EDI and the formatter takes the position in EBX. |
| `input-initialize-devices` | 439 B | `InputDeviceCore`, 29 fields and ten local branches. The worker passes the device owner in EAX, and Log uses ECX. |
| `laser-line-initialize` | 442 B | `LaserLineInitCore`, 10 fields and four local branches. The virtual receiver uses ECX, and the VM starter receives ESI/EAX. |
| `replay-manager-load-replay` | 450 B | `ReplayLoadCore`, 18 fields and 14 local branches. The archive opener takes its name in EAX and size pointer on the stack. `Load` and `Initialize` pass in their own links. |
| `ascii-manager-destructor` | 467 B | `GuiDestructorCore`, 38 fields, eight local branches and two embedded VM unwind actions. The receiver is on the stack with two RET 4 exits. |
| `gui-cleanup-resources` | 476 B | `GuiDestructorCore`, 17 fields, an EBP GUI pointer and the shared startup runtime flags. |
| `text-render-alternate` | 509 B | `TextBufferLifecycle`, 32 fields, outlined LTCG FillColor and normal-COFF InvertAlpha. |
| `script-viewer-popup-draw` | 540 B | `ScriptViewerPopupCreate`, 15 fields and the EDI draw callback receiver. Assign the decremented count after publishing the cursor. |
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
