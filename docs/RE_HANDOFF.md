# TH10 reconstruction handoff

Start with [AGENTS.md](../AGENTS.md), then read the selected owner's source
and ledger rows. [PROGRESS.md](PROGRESS.md) holds the generated totals.
Use [matches.csv](../config/matches.csv) to select remaining work and
[match-units.toml](../config/match-units.toml) to find the exact recipe.

## Resume

The active owner is `EclVmContext::StartSubroutine` at `0x0044DF70`
in `src/EclVm.cpp`. It is the largest remaining authored owner, with
542 of 550 bytes matching in `EclVmHostRunEnemy`. Its four call fields
bind. The remaining differences are register choices in the operand loop.
The integer operand and both integer result stores match through +0xC5.
The float operand is loaded into ECX where the target uses EDX.
Its allocation gets EAX and ECX preferences from the index and instruction
base, while the caller-context conflict cancels the EAX preference. The
read-only C2 trace and its completed debugger link are retained under
`.analysis/ecl-start-subroutine/terra-float-costs2.*`.
The loop latch assigns its destination cursor before the instruction
pointer, which leaves the latter with ECX and EDX available. The target
uses ECX for the cursor and EAX for the instruction pointer. See
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) for traces and cold replay scope.
For nonexact candidates, compile and compare only the affected unit.
Run selected cold replays after the owner reaches a complete exact match.

Keep the original Japanese TH10 v1.00a target at the read-only path
`resources/th10.exe`. Use pinned VC7.1 SP1 build 6030, a single available
CPU at nice 10, and explicit unit selectors. Repository commands run
locally. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) identifies the retained inputs.

Use `scripts/check-frontier.py FRONTIER` for an existing nonexact entry in
[frontiers.toml](../config/frontiers.toml).

## Recent exact checkpoints

| Unit | Extent | Reproducible link |
| --- | ---: | --- |
| `hint-cleanup-owner` | 346 B | `HintCleanupOwner`, 22 fields and the complete EAX-receiver list cleanup callee. The factory and GameManager cleanup pass in their own graphs. |
| `enemy-full-object-destructor` | 386 B | `EclVmHostRunEnemy`, seven fields. The inherited virtual declaration restores the stack receiver. Both VM searches share the same match path. The generated 31-byte deleting wrapper matches both calls and vtable slot five. |
| `script-viewer-destructor` | 412 B | `ScriptViewerDestructSmall` with `/EHs`, 44 fields and two member unwind actions. The factory and eleven related entries pass in the same image. The Stage factory, constructor and destructor pass in their separate cold link. |
| `player-start-power-transition` | 397 B | `PlayerStartPowerTransition`, 17 fields. The in-class indexed invalidator preserves the 81-byte outline in the same link and keeps the panel in ESI across its call. The two Player update calls retain the stack receiver. |
| `enemy-spell-panel-destructor` | 402 B | `SpellPanelDestructor` with `/EHs`, 36 fields and three VM-array unwind actions. The receiver is on the stack with RET 4. |
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

## Known comparison issues

The retained `ascii-manager-initialize` image reports a call-binding
difference at `+0x108` for `AnmVmView::Initialize`. The high-priority draw
callback has a separate cold proof in `StageViewport`.

The retained `gui-message-destructor` image in `GameCleanupContext`
matches 89 of 97 bytes. Relinking the source from before the `GuiView`
destructor work reproduced the same differences. The direct GameManager
cleanup and ScriptViewer creation and release callers passed their
comparisons with the final GUI source.
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) records source and compiler findings.
