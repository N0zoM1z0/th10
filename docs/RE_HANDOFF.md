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
| `replay-manager-begin-stage` | 582 B | `ReplayBeginStageCore`, 25 fields and ten local branch destinations. The position helper has the same EAX and ECX inputs in this link and matches all 83 bytes in its default-profile cold link. |
| `spt-test-update` | 926 B | `SptTestUpdate`, 39 fields, VM lifetime and two-pass file enumeration. The registered callback and the interpreter, factory and worker calls match in the same image. |
| `laser-type1-clear` | 623 B | `LaserType1Clear`, 29 fields and the virtual ECX receiver. The polar helper, script loader, registrar and full item allocator match in the same image. |
| `ending-initialize` | 584 B | `EndingInitialize`, 30 fields and the allocation cleanup slot at EBP+4. The actual factory, text constructor, both callbacks and chain registrations match. |
| `laser-line-clear` | 609 B | `LaserLineClear`, 22 fields and the virtual ECX receiver. The polar helper, script loader and full item allocator match in the same image. |
| `wave-stream-refill` | 637 B | `SoundInitBuffers`, five call fields and both stack arguments. The BGM worker and queue entry match in the same image. |
| `supervisor-update-game-mode` | 666 B | `SupervisorModes`, 60 fields, 20 jump entries and 14 selector bytes. The registered update, chain registration and lifecycle guards match in the same image. |
| `gui-score-notice` | 668 B | `GuiScoreNoticeCore`, 35 fields and seven switch destinations. The spell-end caller, caption factory, script initializer and sprite setter pass in the same image. |
| `enemy-full-constructor` | 673 B | Two-source `EnemyFullConstructor` link through `EnemySpawn`, five fields and the ESI receiver. The base, embedded context and runtime constructors pass independent replays. The caller passes in its own link. |
| `gui-message-constructor` | 683 B | `EnemyEclDispatcher`, 21 fields, four stored-ID lookups and the 2,000-record bullet sweep. The actual allocation caller, its EH binding and the ECL dispatcher match in the same image. |
| `frontend-practice-draw` | 689 B | `FrontEndReplaySaveDraw`, 24 fields and the EDI receiver. The score rows, play totals and three neighboring draw paths pass. The score-entry update also passes in its own graph. |
| `ascii-manager-draw-strings` | 738 B | `StageViewport`, 30 fields, both fonts, shadow drawing and viewport restoration. The callback, camera, flush and renderer pass in the same image. |
| `sound-player-initialize` | 786 B | `SoundSetupRealMain`, 23 fields, DirectSound device and buffer setup, and the 37-sample preload. The setup thread, sample loader and chunk walker pass. |
| `item-manager-spawn` | 819 B | `ItemManagerSpawn`, 18 fields, three jump slots and eleven selector bytes. The normal pool and delayed ring match. Both laser callers, the polar helper, VM creator and loader pass. |
| `frontend-replay-draw` | 825 B | `FrontEndReplaySaveDraw`, 33 fields, the 25-slot list and the seven-stage detail view. Both neighboring draw paths, the callback and the text helpers pass. |
| `frontend-score-entry-draw` | 826 B | `FrontEndReplaySaveDraw`, 35 fields and the two keyboard text paths. The score reads, replay save draw, title callback, formatter and string queue match. |
| `merge-joystick-input` | 833 B | `InputPolling`, 21 fields and four RET 8 exits. The WinMM midpoint and dead zone calculations match. The keyboard caller and both button helpers pass their comparisons. |
| `frontend-practice-records` | 850 B | `FrontEndPractice`, 14 fields and all three centered text calls. The VM result uses an output reference, and the palette reads the capture counter independently. The practice update replays in the same image. |
| `frontend-keyconfig-update` | 880 B | `FrontEndKeyConfig`, 67 fields and five phase destinations. The EBX callback path, pad poller, 200-byte binding editor and display refresh match. The practice update passes its own link. |
| `enemy-read-float-operand` | 884 B | `EnemyReadIntOperand`, 84 fields and 51 selector destinations. The integer reader, three RNG helpers, angle helper and both lvalue resolvers match. All four operand vtable slots retain their bindings. |
| `bullet-runtime-update` | 885 B | `BulletRuntimeTick`, 39 fields and the ECX transform call. The registered callback, manager update, transform and both spawners also match in the same image. |
| `frontend-difficulty-update` | 888 B | `FrontEndStageSelect`, 62 fields and five phase destinations. The caller-owned backdrop output and both child-VM paths match. The factory and registrar, character and stage updates also match in the same image. |
| `frontend-replay-save-draw` | 955 B | `FrontEndReplaySaveDraw`, 42 fields, the animated name entry and 25 replay rows. The draw callback, formatter and string queue match in the same image. |
| `frontend-character-update` | 956 B | `FrontEndStageSelect`, 53 fields, five phases and four child calls sharing one result slot. Both lookup entry points, the title dispatcher, and the adjacent selection handlers pass. |
| `frontend-stage-update` | 1,052 B | `FrontEndStageSelect`, 80 fields, five phases and two numeric-key ladders. The real EDX keyboard snapshot, title dispatcher and 12 other guards match in the same image. |
| `result-menu-draw-rows` | 1,062 B | `ResultMenuRows`, 56 fields, four jump destinations and ten selector bytes. The callback, name-entry overlay, formatter and string queue also match in the same image. |
| `laser-type1-circle-split` | 1,108 B | `LaserType1CircleSplit`, 29 fields, inlined viewport tests and the ordered fragment lengths. The script loader and primary VM registrar also match in the same image. |
| `frontend-update-dispatcher` | 1,128 B | `FrontEndReplay`, 99 fields, 17 jump destinations, the idle-demo state and the EAX callback. The five linked menus, the Practice menu and the replay destructor keep their exact code. |
| `main-init-d3d-rendering` | 1,159 B | `MainInitD3dRendering`, 100 fields, the full HAL/REF fallback and selected back-buffer format. The logs, reset and screen-color functions match in the same image. WinMain matches in its own link. |
| `laser-type1-box-split` | 1,171 B | `LaserType1BoxSplit`, 19 fields, the virtual ECX receiver and the zeroed fragment request. Polar, playfield bounds and the world-VM creator also match in the same image. |
| `laser-line-circle-trim` | 1,238 B | `LaserLineCircleTrim`, 33 fields, inline viewport tests and the VM allocation, script load, and registration calls. The rectangle trim, allocator and three direct Player consumers also match in their own links. |
| `laser-line-box-trim` | 1,269 B | `LaserLineBoxTrim`, 23 fields, the ECX receiver, the XYZ sample loop and all 476 bytes of each fragment request. Polar, bounds and world-VM creation also match in the same link. |
| `frontend-main-menu-return` | 1,280 B | `FrontEndReplay`, 103 fields, the EBX receiver and three complete switch tables. The phase setter and eight known exact units pass their affected comparisons. |
| `hint-consume-entries` | 1,317 B | `HintLifecycle`, 34 fields and all three world-VM paths. The per-frame update and registered callback replay in the same build. |
| `result-menu-update-pause` | 1,360 B | `ResultMenuPause`, 79 fields and five phase destinations. The input word and repeat view share 0x474E36. The scheduler callback and dispatcher pass in the same link. |
| `laser-line-run-commands` | 1,356 B | `LaserLineCommands`, 31 fields, five jump destinations and 64 selector bytes. The virtual receiver is ECX, and `laser-line-update` passes its own link. |
| `frontend-score-entry-update` | 1,400 B | `FrontEndReplay`, 80 fields and four phase destinations. The score insert uses the EDI receiver and both indexed name copies match. |
| `frontend-shot-type-update` | 1,424 B | `FrontEndReplay`, 90 fields and five phase destinations. The child lookup, VM flag and interrupt owners pass in the same link. |
| `player-initialize` | 1,435 B | `PlayerInitialize`, 49 fields, the EBX receiver from `PlayerCreate` and both callback registrations. The factory and option-file loader compare in their own link graphs. |
| `frontend-replay-save-update` | 1,460 B | `FrontEndReplay`, 86 fields and five phase destinations. Replay loading, saving, slot destruction and both music calls also pass in the same link. |
| `frontend-replay-update` | 1,492 B | `FrontEndReplay`, 91 fields and six phase destinations. The `/EHs` link preserves the two calls to the exact replay-file loader. |
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

## Current verification

The replay frame allocator, clear and position helpers passed selected
cold replay. The process-frame family and stage-state restoration passed
their warm comparisons and have selected cold jobs in progress.

A fresh build of the larger GameManager graph exposes a prior GUI-reset
call ABI difference at +0x10E. Relinking the pre-recovery replay source
into that graph reproduces the same diff.

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
