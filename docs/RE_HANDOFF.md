# TH10 reconstruction handoff

Start with [AGENTS.md](../AGENTS.md), then read the selected owner's source
and ledger rows. [PROGRESS.md](PROGRESS.md) holds the generated totals.
Use [matches.csv](../config/matches.csv) to select remaining work and
[match-units.toml](../config/match-units.toml) to find the exact recipe.

## Resume

The sole active owner is `CreateStatistics42AF20` at `0x0042AF20`, 97 bytes
in `src/GameStatisticsLoad.cpp`. Its prepared `game-statistics-create` unit
already passes all 97 bytes and nine fields in the constructor's eight-input
WinMain cold graph, including the complete one-allocation EH protocol.
Accept this ready candidate from its retained complete proof; no executable
input has changed since that cold graph. Next accept the 71-byte popup factory
from its initializer's four-input cold proof, then resume StartSubroutine.
The operator explicitly approved this remaining order in the current session.

The operator has authorized resuming `EclVmContext::StartSubroutine` at
`0x0044DF70` in `src/EclVm.cpp` after the two ready factories above. Its
retained frontier is 542 of 550 bytes matching in `EclVmHostRunEnemy`.
Its four call fields
bind. The remaining differences are register choices in the operand loop.
The integer operand and both integer result stores match through +0xC5.
The float operand is loaded into ECX where the target uses EDX.
The direct integer store no longer needs a compiler barrier in this form.
Its allocation gets EAX and ECX preferences from the index and instruction
base, while the caller-context conflict cancels the EAX preference. The
read-only C2 trace and its completed debugger link are retained under
`.analysis/ecl-start-subroutine/terra-float-costs2.*`.
The loop latch assigns its destination cursor before the instruction
pointer, which leaves the latter with ECX and EDX available. C2 creates
their global-register candidates in read order and queues them in that
same order. An instruction-walk counter supplies their secondary queue
order, with 106 for the cursor and 105 for the instruction. Reversing the
reads assigns the desired registers but changes the machine-code schedule;
raising the instruction candidate's priority in a debugger control also
changes that schedule. A volatile count read restores it in the control,
but the count still uses EAX instead of ECX. The next useful experiment
must address both candidate order and count register choice: in the
volatile-count form, C2 selects the count's EAX on a zero-cost tie with
ECX and has no register preference for that value. The target
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
| `frontend-draw-dispatch` | 116 B | `FrontEndReplaySaveDraw`, two-input draw-callback cold, all 14 fields and eight fixed switch destinations. Source unchanged. Callback7 and complete draw callees419/689/825/826/955 pass in the same image. |
| `game-statistics-constructor` | 132 B | `GameStatisticsConstructor`, eight-input WinMain cold, five fields and EAX owner/RET. Source unchanged; real file/archive consumers restore OpenFile360. Factory97/9 and its allocation EH, loader423, finalizer564, tail142 and record274 pass in the same image. |
| `script-viewer-popup-initialize` | 145 B | `ScriptViewerPopupCreate`, four-input canonical cold, 11 fields and EAX owner/RET. Reload the saved resource after both registrations. Direct factory71/5 and all selected popup, Chain and concrete VM initializer guards pass; factory remains a prepared candidate for later selection. |
| `enemy-laser-spawn` | 153 B | `LaserLineUpdate`, four-input canonical cold, four fields and ESI/EDI/stack type with RET4. Current member source unchanged; replaces the stale free-helper label. Bounce410/17, update1001/31 and both constructors202/2 and219/2 pass in the same image. |
| `script-viewer-emit-value-popup` | 167 B | `ScriptViewerPopupCreate`, four-input canonical cold, one speed field and ESI/EDI/EAX/stack color/RET4. Keep zero fixup after the sign branch and reuse the timer helper. Popup guards pass; actual ItemManager caller2320/154 and same-image callee167/1 pass its 41-input cold graph. |
| `frontend-assign-keyconfig-binding` | 200 B | `FrontEndKeyConfig`, 21-input canonical cold, three fields and EAX/ECX/EDX with RET. Source unchanged. The real key editor caller passes 880/67; display refresh2452 and sound queue123 pass in the same image. |
| `entity-start-script` | 202 B | `StagePrepareAnimationSlots`, five-input canonical cold, both calls and ESI VM/EAX index/stack resource/RET4. Natural `AnmVmView::Initialize()` replaces the dummy-argument free wrapper; initializer273 remains raw-exact. Direct initializer consumers, stage214, stage dispatcher1868 and StartSpell2432 pass selected canonical cold comparisons. |
| `anm-render-manager-destructor` | 206 B | `AnmCopyTextureRect`, two-input canonical cold, all 11 fields and stack manager/RET4. Cache next before RemoveVm in both lists. Three-state EH, 79-byte generated region and 34-byte VM cleanup pass. RemoveVm passes 268 in this graph and its own cold; WinMain passes 1,389/146 after the changed source is recompiled in its canonical graph. |
| `game-prepare-animation-slots` | 214 B | `StagePrepareAnimationSlots`, five-input canonical cold, sole real VM-starter call and stack owner/RET4. Source unchanged. The retained 1,544-byte GameManager caller passes all 114 fields; the starter now has its own 202-byte exact recipe. |
| `anm-copy-texture-rect` | 227 B | `AnmCopyTextureRect`, two-input canonical cold, both call fields and EDX/EAX/ECX plus four stack words / RET16. Fresh lookups cross the flush and first surface query. The 133-byte flush callee passes four fields in the same graph and its own canonical cold. |
| `script-viewer-popup-owner-destructor` | 232 B | `ScriptViewerPopupCreate`, four-input canonical cold, 22 fields and stack receiver/RET4. Single VM-member EH: 21-byte region, 28-byte FuncInfo and real 34-byte cleanup. Constructor134 and release18 pass; the VM EH verifier self-tests pass. |
| `player-constructor` | 244 B | `ScriptViewerLifecycle-cold-2`, 14-input canonical cold, sole g_Player field and ESI receiver/EAX result. Nested option-state construction restores the last flag clear. The Player factory passes 69/5 and the standalone option constructor remains 10/10. |
| `player-shot-track-enemy` | 266 B | `PlayerShotTracking`, two-input canonical cold, all 15 fields and unchanged source. Real EnemyWrapAngle support keeps speed on the x87 stack. Same-image angle helpers pass 80/4 and 94/6; acquisition passes its own canonical cold entry at 53/1. |
| `enemy-start-spell` | 2,432 B | `EnemyStartSpell`, 39-input canonical cold, 108 fields and RET 0x10. The real ECL caller and same-image owner pass selected strict replays. Both creation paths retain by-value appearance inputs. |
| `game-statistics-initialize-record` | 274 B | `GameStatisticsTail`, eight fields, matching literal payload and stack record / RET4. Three same-image guards and six selected typed-record consumers pass cold. The independent StartSpell regression has its own accepted repair. |
| `anm-loaded-set-execute-script-idx` | 287 B | `AnmSetVmScriptIndex`, four fields and EDX/EAX/ECX inputs. The sole 36-byte handle caller passes all three fields in the same independent cold link. |
| `main-configure-camera` | 287 B | `StageViewport`, 12 fields and EDI receiver. All six direct consumers pass selected cold comparisons in three graphs. |
| `frontend-main-menu-update` | 288 B | `FrontEndReplay`, 20 fields including five switch entries. The real dispatcher passes 1,128 bytes/99 fields in the same cold link. |
| `gui-load-data` | 289 B | `GuiCreation-cold-20261025`, 16 fields and six local branches. Both direct callers pass: GUI Initialize 147/15 and GameManager Initialize 1,009/102 in its own link. |
| `laser-type1-initialize` | 309 B | `LaserType1InitCore`, seven fields. Unchanged natural source with the real VM starter and script loader; independent cold replay passes. |
| `frontend-insert-score` | 316 B | `ResultMenuClose`, 18 fields and the EDI table receiver. Both direct callers pass their own selected cold links. |
| `player-destructor` | 325 B | `ScriptViewerDestructSmall`, 30 fields and the real 34-byte VM cleanup. Five selected lifecycle/field consumers pass their own cold links. |
| `anm-copy-surface-to-backbuffer2` | 325 B | `BulletHorizontalWrap`, seven fields and EBX index/stack manager/RET 28. Both selected same-image guards pass. |
| `anm-copy-surface-to-backbuffer` | 330 B | `BulletHorizontalWrap`, seven fields and EBX/EAX/RET 16. The same-image absolute-direction Bullet guard passes 335 bytes and eight fields. |
| `bullet-update-absolute-direction` | 335 B | `BulletHorizontalWrap`, eight fields and the ESI receiver. The direct runtime caller passes its own cold link at 885 bytes and 39 fields. |
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
