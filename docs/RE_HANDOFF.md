# TH10 handoff — 2026-10-31

## Start here

Follow [AGENTS.md](../AGENTS.md), then read the selected owner's source and
ledger rows. [RE_WORKFLOW.md](RE_WORKFLOW.md) defines the bounded exact loop;
[ORACLES.md](ORACLES.md) defines acceptance. Call local tools directly.
There is no running compiler/linker job or unfinished source edit.

[PROGRESS.md](PROGRESS.md) is generated from the ledgers and is the count authority:

| Measure | Current count |
| --- | ---: |
| Reviewed authored owned bytes exact | 115,040 / 275,890 |
| Authored exact owners | 538 |
| Total canonical exact owners / codegen bytes | 1,418 / 183,174 |
| Source mappings / authored mappings / authored exact backlog | 1,221 / 703 / 165 |
| Reviewed boundaries / authored functions | 1,819 / 747 |

The denominator and source/origin inventory remain provisional. Exact codegen
proves neither original names/class/TU ownership nor product/runtime/portability.

## Latest reviewed authored exact checkpoint

[HintOwnerView::Initialize @418E30](evidence/hint-initialize-split-exact-20261031/README.md) is now full **131/131 exact** with all13 real callback, Chain, supervisor flag, file-path and `LoadHintFile` fields. A bounded source partition puts the parser and its four static helpers in normal-compiled `HintLoad.cpp` while preserving the actual three-stack-input call rather than changing ABI manually. Two independent targeted native cold runs protect lifecycle **785/785**, loader `/GL` **366/366**, loader `/GL /GS` **312/312**, and the separate Caution/FileSystem guards 180/180. Full loader1498 remains nonexact. Exact authored progress is **115,040 / 275,890 across 538 owners**.


## Earlier accepted checkpoints (historical snapshots)

The entries below record cumulative totals at each earlier acceptance. Their
per-checkpoint counts are chronology, not current progress. Use the top-level
counts and generated PROGRESS.md as the only current ledger totals; do not add
these historical numerators together.

[AnmRenderManagerView::PreloadSurface @447EC0](evidence/anm-preload-surface-exact-20261031/README.md) is now full **169/169 exact** with all10 real Stage-linked fields, correcting prior fabricated `AnmErrorLoggerView::Log` to original `th10::g_GameErrorContext.Fatal @44B8E0` and original `filePath[260]` stack frame. Two independent selected StageCreate colds preserve new169 and existing stage initializer522, **691/691** in one shared real source graph. Authored exact rises to **114,909 / 275,890 over 537 owners**. No full cold replay, no original-source/TU/runtime claim.


[ReplayAllocateFrameData @42AA50](evidence/replay-frame-allocation-exact-20261031/README.md) now passes complete **139/139 exact** with its true operator-new REL32, ESI stage/stack manager/RET4. A natural typed cursor-slot publication fixes the original store/LEA scheduling, while two independent selected two-input `ReplayManager::BeginStage`/`ReplayStageHelpers` colds prove the complete owner. Existing ReplayClearStageFrameData93 and ReplayRestoreStagePosition83 remain exact in the same new graph. This increases authored exact to **114,740 / 275,890 over 536 owners**, without admitting the nonexact 582-byte BeginStage parent or running full cold replay.


[BulletManagerView::~BulletManagerView @405F70](evidence/script-viewer-bullet-destructor-exact-20261031/README.md) is now a **complete 236/236 authored exact parent**, all23 target fields bound. The explicit one-state 38-byte generated Bullet-array-EH protocol passed a real-image negative suite with28 rejection controls, and two independent **3-source** targeted cold links. The second also preserves the old Bomb137 via its separate 2-source graph (**373/373** across both producer groups). Only the parent236 receives credit; the generated EH helper/element destructor do not. Authored exact increases to **114,601 / 275,890** over **535** owners.


[ScriptViewerBombOwnerView::~ScriptViewerBombOwnerView @405620](evidence/script-viewer-bomb-cleanup-exact-20261031/README.md) is now complete **137/137 exact**, with all15 original Chain, critical-section, Supervisor state and bomb global fields. The actual `ScriptViewerReleaseBomb`/`Chain.cpp` two-input graph cold-replays twice independently; the previously open EH-protected BulletManager destructor is now independently admitted as full236/236 in a separate verified three-input graph; this earlier 137-byte claim remains unchanged. Authored exact rises to **114,365 / 275,890** across **534** owners.


[GameCleanupLinked41C1C0 @41C1C0](evidence/enemy-laser-linked-cleanup-exact-20261031/README.md) is now full **195/195 exact** with all16 real IAT/callee/supervisor-state fields, including base+0x64C/+0x6F4, through the actual `GameCreateOwner41C290`-rooted 2-TU EnemyLaserManager/Chain producer. Two independently rebuilt selected colds pass; exact authored becomes **114,228 / 275,890, 533 owners**. This is a complete medium owner, not a padded prefix or a full-game replay.


[ResultMenuClose @423510](evidence/result-menu-close-exact-20261031/README.md) now passes **89/89 exact** with real ESI receiver, two exact `EnemyMarkPendingInterrupt` call fields and two original ANM-speed global references. The final canonical recipe uses the genuine **two-input** `ResultMenuUpdateResult`/`VmIdInterrupt` graph, where two independent selected cold links strictly reproduce **89/89** and all four fields. An older 15-input `ResultTransitions` cold had also protected two existing 411-byte menu callers (**911/911**) but its next retry hit a transient Wine path fault; that historical graph is corroboration, not the current acceptance dependency. Reviewed authored numerator reaches **114,033/275,890**, 532 owners, without full cold replay.


[TransitionEffectView::Cleanup @43CB00](evidence/transition-cleanup-exact-20261031/README.md) is now complete **127/127 exact**, all 14 original call/global fields bound, under the actual StageCreate link. Two independent selected cold links pass cleanup alone, then both cleanup and existing StageLifecycle initializer (**649/649**). The authored exact numerator becomes **113,944 / 275,890**, **531 owners**. No full cold replay or source/ABI changes were required.


[Three complete lifecycle owners](evidence/script-viewer-graph-lifecycle-exacts-20261031/README.md) now replay **69/69 PlayerCreate**, **95/95 EnemyManagerCreate** and **161/161 ANM AllocateVm** under one real `ScriptViewerCreate` lifecycle producer. All14 original target fields and the unique source/PDB owners bind; two independent scoped cold builds pass first **626/626** across three new and two existing guards, then **325/325** across only the three new functions. This raises reviewed authored exact to **113,817/275,890**, **530 authored exact owners**; the denominator is still provisional.


[AsciiManagerCreate @401440](evidence/ascii-factory-exact-20261031/README.md) is now complete **69/69 exact** (five allocation/constructor/initializer/destructor/deallocation REL32 fields) in the genuine three-input ASCII initialization graph. The old 70-byte one-TU candidate had the wrong destructor private receiver; neither source ABI nor factory code needed alteration. Two independent selected colds preserve the new69 and previous initializer330 + high-draw10, **409/409 exact** across34 fields. Reviewed authored exact progress rises to113,492/275,890 across527 owners.


[AsciiManagerView::OnDrawHighPriority](evidence/ascii-high-draw-exact-20261031/README.md) is now full **10/10 exact** with its real `AsciiManagerDrawGuiStrings` REL32, recovered by a natural VM pointer in the genuine GUI renderer. The selected 3-TU `AsciiManagerCreate` source graph yields the target EBX receiver; two independent selected cold links preserve new callback10 and prior Initialize330, **340/340** across all29 fields. The larger 570-byte renderer stays **NONEXACT** despite improving from54/458 to160/458 comparable positions. This new callback raises reviewed authored exact to113,492/275,890 across527 owners.


[ResultMenuUpdate @4223F0](evidence/result-menu-update-exact-20261031/README.md) is now complete **206/206 exact**, including the formerly excluded 27 bytes of physical jump/selector tables. All16 external/local fields and five table destinations bind in two independent selected cold links. The previous provisional boundary stopped at the RET after 179 bytes; the corrected extent ends at 4224BD, two CC bytes before the next owner. Exact authored progress increases to 113,492/275,890 and one new owner; the reviewed denominator is still provisional.


[GameConfigurationView::ResetDefaults @418C40](evidence/game-configuration-reset-exact-20261031/README.md)
now passes all **125 bytes and all five fields** from its real
MainLoadConfiguration caller graph. Moving the existing `colorMode = 0`
assignment ahead of numeric defaults recovers the target codegen; the prior
order was 116/125 nonexact in the same graph. An incorrect double-addend
manifest was independently repaired. Two **selected** cold replays pass;
no historical whole-cohort replay is required. Current reviewed authored
exact total is 113,492/275,890 across 526 owners; the denominator remains
provisional.

Sound PreloadBgm has a separate [bounded nonexact correction](evidence/sound-preload-reg-contract-20261031/README.md)
that recovers the real GetFmtIndexByName ECX input; do not admit its
369-byte owner merely because it has a full-size candidate.

## Recent Hint exact source checkpoint

[Hint frame Update419650..41988A](evidence/hint-frame-exact-20261031/README.md)
is complete571/all17 exact, committed in `9bd5b01`. It has169 instructions,
23 fixed local branches, EBX owner and RET at23A. Repeating the width-array
expression at its three genuine uses makes ordinary CSE/loop reduction select
width base17C; the target store/reloads, manager field72 and alignment follow.
No extra induction pointer, register forcing, padding, header or flag changes.

The existing update callback10/all1 remains exact and origin-indeterminate.
Warm and zero-reuse eight-input cold agree; independent canonical selects only
`hint-frame-update` and `hint-update-callback`,581/all18 in one producer.
Same-image factory/draw guards also pass. Actual ConsumeEntries remains
1308/1317 NONEXACT and passes its previous full proof in both final images.

Historical build/cache context:
.analysis/gpt-6.1-sol/hint-frame-loop-20261031/final-cold-context.json.
It records 36 source/header pins, but only 34/36 match current sources;
src/AnmManager.cpp and src/Hint.cpp changed in later accepted work. Do not
reuse its image or objects as a current-source witness. After the Hint source
split, two independent canonical selected colds freshly replayed
hint-frame-update + hint-update-callback as 581/581 exact, with distinct
PDB GUIDs 1121e78937b9ca49814888c78c86888c and
7a82b94de2986a41833ad56d687e784e. The normal canonical producer remains
build/match-linked/HintLifecycle.

The complete 9,964-byte ANM ExecuteScript now strictly reproduces **9,962 bytes** using a local timer-read helper and a scoped radial Z read. The earlier child-variant contexts are historical; the latest post-PreloadSurface context gptweb-post-preload-20261031-context.json has **13/13 current source/header pins**. Its five-source cold and all350 field bindings confirm the remaining two bytes; see [the large-owner proof](evidence/anm-executor-two-byte-frontier-20261031/README.md). Exact totals are unchanged because the whole owner still rejects.

## Open owners worth resuming

These are complete measurements at the linked proof's source epoch, not fresh
comparisons of every owner against today's source. Choose one bounded owner.

| Owner | Last complete snapshot | Evidence / concrete remaining lead |
| --- | --- | --- |
| Sound PreloadBgm43D7D0 |365/369 NE|[Read-register experiment](evidence/sound-preload-barrier-20261031/README.md): ten bound fields and all 121 instructions match except four EAX/EDX register bytes; affected existing units cold-pass 1641/all |
| Hint ConsumeEntries419120 |1308/1317 NE|[Consumption proof](evidence/hint-consume-context-20261031/README.md): small-ID result/register order and common script PUSH; actual world creator116/118 remains NE |
| GUI message Run |4472/4480 NE|[Latest state view](evidence/gui-message-state-view-20261031/README.md): counter placement, cursor and remaining register/read schedules |
| Capture RequestCaptureRectangle424480 |148/162 NE|[Full strict context](evidence/capture-rectangle-schedule-20261031/README.md): real FindVm EDX callee recovered; 23 to 14 remaining store-schedule differences; two existing actual callers cold-pass 822/all |
| GUI DrawStageElements415800 |724/724 NE|[Latest arithmetic proof](evidence/gui-draw-read-order-20261031/README.md): first rectangle LEA targetD6/nativeEC, addition field targetEC/nativeE8 |
| GameManager cleanup / update |1098/1105 and1538/1544 NE|[Cleanup controls](evidence/gui-message-teardown-20261031/README.md): transition flag publication/block placement and VM-call/private helper interfaces |
| ECL AddScriptData450220 |547/589 NE|[Caller-context record](evidence/maintenance-20261030/README.md): actual Enemy caller restores ABI; validation-slot reload, old-table guard and insertion lifetimes remain open |
| Enemy float operand reader |876/884 NE|[Operand proof](evidence/enemy-operands-exact-20261030/README.md): float family/source ordering controls remain unresolved |
| ANM ExecuteScript |9962/9964 NE|[New arithmetic proof](evidence/anm-executor-two-byte-frontier-20261031/README.md): Z read and timer x87 schedule recovered; only child91 EDX/ECX push remain; all350 fields/95 locals strictly bound |
| Ending Run |1568/1568 NE|[Ending proof](evidence/ending-native-record-20261030/README.md): old complete comparison retains54 ordinary differences; current inputs have changed |

The GUI stage updater is already complete3836-byte exact; do not reopen the
old four-byte residual. See [its accepted proof](evidence/gui-update-exact-20261031/README.md).
The integer operand reader is already exact too. Use the ledgers before
selecting a similarly named old candidate.

Parked Hint models: separate width induction spills; width-derived slot indexing
adds arithmetic; local width address and owner/slot lookup forms are neutral;
typed position view selects the wrong x-based cursor. New repeated-width source
has superseded that loop frontier. For consumption, lookup-value projection and
receiver/endpoint spelling controls are neutral. Reopen only with new evidence
and a distinguishing prediction.

## Cache and artifact recovery

[RE_ARTIFACTS.md](RE_ARTIFACTS.md) lists expanded working contexts, the verified
private archives and selective restoration. Ended campaigns are archived with
per-file hashes; their old paths do not imply expanded or reusable inputs.
The current [cleanup audit](evidence/maintenance-20261031/README.md) records scope,
storage savings and source-pin mismatches. In particular:

- GUI Run remains a 4472/4480 nonexact candidate; its reviewed 64-pin context
  is now 60/64 (AnmManager.cpp, FrontEnd.cpp, Hint.cpp, Sound.cpp changed).
  Its image is historical until rebuilt.
- ECL actual-caller context differs at AnmManager.hpp, Enemy.cpp and GameScoreState.hpp.
  The pin-equal one-source entry is a different graph with the wrong ABI.
- Cleanup/message contexts are historical: the retained cleanup-leaf sample
  is 74/84 pins and the reviewed message-pairs sample is 78/84. The latest
  Ending final-source context is 57/70 (13 changed inputs). Rebuild before reuse.
- Hint frame's retained 36-pin context is now 34/36 (changed AnmManager.cpp
  and Hint.cpp); the accepted frame/callback pair was freshly cold-replayed
  twice against the current Hint split (581/581 each run).
- Latest GUI draw context is 14/15 (changed AnmManager.cpp); treat its
  candidate image as historical until rebuilt.
- The ANM executor's latest post-PreloadSurface context is 13/13 current pins;
  older child/radial variant contexts have changed AnmManager.cpp and are not
  reusable current-source artifacts.

Caution424650 calls AddAutomaticEntry41A120, not ConsumeEntries or Update.
Its extra replay in older Hint campaigns was unnecessary and is not a gate.
Preserve original target, Ghidra, toolchain/Wine, reference checkouts and the
[D3D adapter evidence](evidence/d3d-adapter-format/README.md). That D3D hypothesis
is unadopted; do not rerun its consumed one-shot probe. Music parser qualification
requires the real `build/match-linked/MusicRoomHelpers` producer path.

## Validation and history

Verify the target before target work; attest Ghidra before using observations.
Keep compiler/linker jobs serial, CPU0/nice10, bounded and exit-logged. Compare
complete owners, every field, fixed locals, required callees and actual ABI.
Replay only new/affected exact owners with explicit `--unit` selectors.

Default `scripts/repo-python scripts/ci.py` runs seven lightweight checks.
Select comparator self-tests only for a changed mechanism. Historical host
failure/lifecycle/sanitizer/mutation reports are not current gates. Product
closure, semantic dual-Oracle work and portability remain later phases.
Regenerate progress when ledger inputs change; finish with `git diff --check`
and a local English `gpt-web:` commit using the specified author/trailer.
Never push.

The previous858-line handoff is recoverable with
`git show 9bd5b01:docs/RE_HANDOFF.md`. Older detail remains in
[the dated handoff](RE_HANDOFF_HISTORY_20261030.md), linked evidence and Git.
Old sizes, open statuses, test lists and “current” cache labels do not override
current ledgers, source pins or the latest complete owner proof.
