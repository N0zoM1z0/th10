# TH10 exact reconstruction handoff

Updated 2026-09-24. This is a current recovery snapshot, not a chronological
session log. Historical target facts belong in docs/KNOWLEDGE_BASE.md and
accepted implementation history belongs in Git.

## Authority and recovery

- Target: original Japanese TH10 v1.00a.
- Canonical executable: resources/th10.exe.
- Required SHA-256:
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Branch: main.
- Latest source checkpoint for the complete Enemy laser caller graph:
  `1d53446 gpt-web: recover Enemy laser type1 collision callers`.
- Earlier giant-owner measurements below belong to explicitly selected LTCG
  support graphs. The associated `.analysis/` files are no longer present;
  regenerate a focused probe before using a measurement as live feedback.
- Commit and push substantive checkpoints as requested by the active operator;
  this work uses the `gpt-6-luna-max:` prefix.
- Decompiler output, adjacent games, ignored build products and .analysis/
  artifacts are hypothesis/evidence only. They never establish exactness by
  themselves.

Before editing:

    git status --short --branch
    git diff --check
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --execute
    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/report-reconstruction-status.py

For Factory work, also require a passing th10-ghidra preflight for the
attested target before treating new disassembly/decompilation as evidence.

Do not copy static repository-wide inventory counts from this file into a new
report. Regenerate them with the tracking scripts because source-present and
exact-unit counts change independently of the three giant owners. The tracked
candidate boundaries are reviewed, but 90 origins are indeterminate and
unresolved `.text` gaps leave the final authored denominator open. Whole-product
Windows i386 closure is still open; semantic and port stages have not started.

## Active exact frontiers

The three large owners remain non-exact. Equal size, equal table order,
equal selector bytes, or source/semantic coverage is not exactness.

| Owner | Target | Last recorded diagnostic context (replay required) | Status |
| --- | ---: | --- | --- |
| EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770 | 14,416 bytes | 14,228 bytes; pre-table 13,572/13,760; 746/11,556 normalized comparable bytes; selector 181/181; physical order matches; typed 0x1A8 speed fields, rank-float shared tails and target 0x119 positive-value tests retained | non-exact |
| AnmRenderManagerView::ExecuteScript @ 0x0043EE30 | 9,587-byte executable owner | selected source-shape diagnostic: contribution 9,960 with pre-table 9,584/9,588 and 705/8,608 agreement; exact-creator split context separately yields 10,040 with pre-table 9,664 | non-exact; context-sensitive |
| EclVmContext::Run @ 0x0044E1A0 | 7,020 bytes | 7,020/7,020; pre-table 6,692/6,692; 945/6,264 normalized agreement, normalization incomplete | non-exact |

Use the per-owner sections below for the selected context and open problems. Do
not mix measurements from different LTCG support graphs.

## Small-function roadmap

Keep each helper in a source module supported by its TH10 caller path. Local
view names describe maintained code and do not establish the original class or
production translation unit. Do not collect unrelated helpers in a generic
`GameSmallFunctions.cpp`.

| Priority | Candidate | Evidence and next step |
| --- | --- | --- |
| Completed | `Lzss::DeleteString @ 0x00436210` | `src/Lzss.cpp`; exact linked-PE replay covers the complete 80-byte PDB extent and all seven linkage fields. Original production owner remains unknown. |
| Completed | `AnmOpcodeVectorView::FromAngleMagnitude @ 0x00408750` | Maintained in `src/AnmManager.cpp`; caller `0x00406D90` is the ANM VM opcode handler. Exact body replay covers all 30 relocation-free bytes. |
| Completed | `AnmScriptVectorView::FromAngleMagnitude @ 0x00441EF0` | Maintained in `src/AnmManager.cpp`; direct caller `0x0043EE30` is the ANM script executor. Exact body replay covers all 30 relocation-free bytes. |
| Completed | `EnemyRuntimeVectorView::FromAngleMagnitude @ 0x0044C5D0` | Maintained in `src/Enemy.cpp`; called by `EnemyRuntimeUpdate @ 0x0040DC80`. Exact body replay covers all 30 relocation-free bytes. |
| Completed | `PlayerShotBoundsView::IsOutsidePlayfield @ 0x00428D70` | Maintained in `src/Player.cpp`; called from `PlayerUpdateShots @ 0x00428280`. Exact linked-PE replay covers all 91 bytes and four bounds references; target values are X `(-192, 192)` and Y `(0, 448)`. |
| Completed | `EnemyLaserVectorView::IsOutsidePlayfield @ 0x0041F7A0` | Maintained in `src/EnemyLaser.cpp`; two callers are the mapped type-0/type-1 collision owners at `0x0041D880` and `0x0041EB00`. The third caller `0x0041D3D0` remains unassigned. Exact linked-PE replay covers 91 bytes and four bounds references. |
| Completed | `AnmRenderManagerView::FindVm @ 0x004491C0` | Maintained in `src/AnmManager.cpp`; Ghidra reports 43 direct callers, including `MarkVmForDeletion @ 0x004492A0`. Two canonical LTCG builds replay the complete 73-byte PDB contribution exactly, with no link fields. Original production TU remains unknown. |
| Completed | `AnmVmIdView::{GetVm, SetFlag2, ClearFlag2, Release} @ 0x00449450, 0x00449590, 0x004495E0, 0x00449630` | Grouped in `src/AnmVmId.cpp` by their TH10 class declaration and caller paths. TH095 has an analogous `AnmVmId.cpp`; TH08 keeps its older grouping in `AnmManager.cpp`. Two independent canonical linked-PE builds reproduce their complete 21/67/67/21-byte PDB contributions and both linkage fields for each method. This maintained placement does not claim either game as the original production TU. |
| Completed | `AnmVmIdView::SetInterrupt @ 0x00449470` | Added to `src/AnmVmId.cpp`; FrontEnd and GUI interrupt helpers call it. Two independent canonical linked-PE builds reproduce its complete 65-byte PDB contribution, including the internal alignment gap and both linkage fields. The maintained class/source placement does not establish the original production TU. |
| Completed | `EnemyRuntimeView::ResolveIntArgument @ 0x00412A10` | Maintained in `src/EnemyEclDispatcher.cpp` with the ECL dispatcher caller. Two cold canonical linked-PE replays under the real `DispatchEclInstruction` entry and ECL/ANM support inputs reproduce the complete 75-byte target extent, with full decoding, raw equality, and no linked fields. This codegen exactness is scoped to the declared compiler context; original source/outlining origin and production TU remain unknown. |
| Completed | `PlayerUpdateCallback @ 0x00426500` | The seven-byte registered update adapter is colocated in `src/Player.cpp` with `PlayerUpdateCallbackBody` and the paired draw-chain callback. Two cold canonical normal-COFF replays reproduce the full extent and its sole REL32 to `0x00425730`. Codegen exactness does not determine whether the target wrapper was authored or emitted by LTCG; original origin and production TU remain unknown. |
| Completed | AnmRenderManagerView::GetVmPosition @ 0x004493C0 | The canonical normal-COFF unit anm-manager-get-vm-position reproduces all 27 bytes and its FindVm REL32 in two cold builds. Ghidra does not mark a function at this address; the accepted boundary and match rows already close this helper. |
| Defer | `CMemoryPbgFile::Seek @ 0x004364F0` | Keep the natural virtual method in `src/ResFile.cpp`. Target uses `ECX this`, two stack arguments, and `RET 8`; its 107-byte body dispatches `FILE_BEGIN/CURRENT/END` and shares the current/end pointer update. The if-chain candidate was 125 bytes with 6/107 comparable bytes. A switch over the target seek origin emits 117 bytes with 12/107 comparable bytes in the selected linked probe. Reordering its `case` labels to `FILE_END`, `FILE_CURRENT`, `FILE_BEGIN` leaves the fixed normal-COFF output identical at 117 bytes and 12/107 comparable. Single-result and shared-base refactors emitted 118 and 122 bytes, respectively, and were reverted. Current backlog ranking remains 117 bytes versus target 107, with 12/107 comparable bytes; repeated syntax/order changes have not improved it. Reopen only with new source/control-flow/compiler evidence. |
| Defer | `CMemoryPbgFile::Open @ 0x00436400` | Keep the virtual method in `src/ResFile.cpp`. Target calls 0x435800 with filename in ECX plus a stack null, then calls 0x4358E0 with filename in ECX; Ghidra's provisional signatures are `__thiscall (void*, byte*)` and `__fastcall (uint*)`, with only these two callsites in the current xref results (0x43640C/0x436416). The target call setup supports those argument locations but does not establish helper source owners or original declarations. Do not guess declarations from provisional signatures. The selected `/GL` probe with `PbgArchive.cpp` support emits 51 bytes versus 46 (8/38 comparable bytes). Reopen with source-owner or compiler evidence. |
| Defer | `ReplayManager::Destroy @ 0x004297B0` | The maintained null-safe delete candidate lives in `src/ReplayManager.cpp`. Target extent is 18 bytes with a private ESI object and plain `RET`; direct callers `0x004236F0` and `0x00433570` still have no maintained source owners. A selected `/GL` entry build emits 27 bytes versus 18 (2/14 comparable bytes; two differing fields). The probe exposed a stale local `void CloseWriteFile()` declaration conflicting with `FileSystem.hpp`'s `int` declaration; that duplicate is removed, and the module now compiles. Reopen after mapping one real caller path. |
| Completed | `GuiSetLivesDisplayCount @ 0x00413790` | Its complete 77-byte PDB-owned contribution from maintained `src/Gui.cpp` is raw-equal and fully decoded, with zero linked fields, in two independent cold VC7.1 SP1 replays. The canonical graph uses `PlayerResetRuntimeState @ 0x00424D90` as primary entry and `Gui.cpp` as support; target references are three calls and one tail jump. Other callers `0x004188A0` and `0x004269D0` remain unmapped. Exactness is scoped to this declared compiler context; original physical TU remains unknown. |
| Defer | `EclVmHost::SpawnThread @ 0x004500D0` | Its unique target caller is `EclVmContext::Run @ 0x0044E1A0`, which calls it twice. Target `SpawnThread` is 142 bytes and passes the new context in EAX to `StartSubroutine`; the caller-supported natural `/GL` candidate is 140 bytes with 122/130 comparable bytes, and retains the context in ESI. The all-source ranking's 142-byte/129-of-130 candidate used a different default link entry, so it is not the target caller context. Keep semantic source in `src/EclVm.cpp`; don't force the private register ABI. Reopen with new source-expression or optimizer-lifetime evidence. |
| Defer | `EclVmContext::ReadInt @ 0x0044FDB0` | Factory-attested xrefs show 116 direct calls: 9 from `EclVmContext::Run`, 106 from `EnemyRuntimeView::DispatchEclInstruction`, and 1 from `EnemyRuntimeView::ReadIntArgument`. The target entry is EDX=context plus one stack index and `RET 4`; representative paths include Run moving ESI to EDX, dispatcher paths loading the context from runtime `+4` and pushing indices 0 or 0xB, and the argument adapter following runtime to ECL state `+0x14D8` to context `+4`. The selected `/GL /GS` graph emits all 144 bytes; 136/140 comparable bytes match with the `__ftol2` displacement treated as a linkage field, while three register-choice fields differ (candidate ECX, target ESI). A mutable local-output test emitted 115/144 and regressed `Run` to 7024/7020 bytes; it was reverted. The separate `poppedValue` rewrite that changed the entry/return ABI is also closed. Keep the current 144-byte candidate diagnostic-only; reopen only with new target-supported source-expression or register-lifetime evidence. |
| Defer | `AnmSetVmScriptIndexAndExecute @ 0x004496D0` | All three target calls are inside `EnemyRuntimeUpdate`, at 0x0040E089, 0x0040E0D3, and 0x0040E374; they pass the managed-VM ID array and a selected script index. The current source candidate is in `src/AnmManager.cpp` and performs shared-manager `FindVm`, null return, then `SetAndExecuteScriptIdx`. With real Enemy support it emits 42 bytes versus the 36-byte target (1/28 comparable bytes; three differing fields); a manager-member form emitted 40/36 (2/29). Target uses a private EAX VM-ID pointer plus one stack script argument and `RET 4`, so exactness remains unresolved pending linked-image extent/call-ABI evidence. The adjacent `SetAndExecuteScriptIdx @ 0x0043E8B0` remains a separate 287-byte near match at 249/271 comparable bytes. |
| Defer | `PlayerCreate @ 0x00425020` | The selected natural `/GL` candidate is 66 bytes versus the 69-byte target (12/49 comparable bytes; five differing fields). Its direct caller `0x00417870` is a large startup/system-initialization sequence with no maintained source owner; map that path before changing this constructor. |
| Defer | `EnemyEclResourceView::LoadFile @ 0x0040CD20` | The target calls `0x0044B360` with filename in EAX, `sizeOut/mode` on the stack, and `RET 8`; `Enemy.cpp` keeps a three-value logical declaration without claiming that private ABI. The real same-TU `EnemyManagerView::Initialize` `/GL` candidate is 97 bytes versus 95 (50/79 normalized comparable bytes; four linkages). The helper's source convention and production context remain unknown, so keep the current abstraction and exactness open. |
| Defer | `AnmVmView::StartPrimaryAlphaInterpolation @ 0x00442300` | The actual `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` context emits 128 bytes versus 130 (14/126 comparable bytes), although the adjacent primary/secondary color interpolation helpers are exact. Do not infer alpha source shape from those siblings. |
| Defer | `AnmRenderManagerView::ReleaseAnm @ 0x004477D0` | A semantic implementation already exists in `src/AnmManager.cpp`; do not add a duplicate. The 55-byte target uses private EBX manager and ESI index state, while the fixed normal-COFF member candidate is 62 bytes with 3/47 comparable bytes and two REL32 fields. Its only direct caller, ECL worker `0x0040BD80`, remains source-unmapped. Keep exactness open until that caller/compiler context is mapped; do not force the private register ABI. |
| Defer | `OnDrawHighPriority @ 0x00401520` | Target has one code call to `AsciiManagerDrawGuiStrings @ 0x00401A50` and a callback-registration data xref at 0x004011DA; maintained `AsciiManagerView::Initialize` registers it. The natural member-method experiment kept the callback wrapper at 10 bytes but used EDI where target saves EBX; the renderer body was 416/570 bytes and likewise used EDI instead of the target private EBX manager. The experiment was reverted. Keep deferred until source/lifetime evidence supports another natural lowering. |
| Defer | `AsciiManagerCreate @ 0x00401440` | The target is 69 bytes; the source-linked `/GL` candidate is 70. Replacing the source destructor call with explicit destruction and `operator delete` emitted the same candidate. Do not repeat that spelling change; first recover the target's owner/call context. |
| Defer | `SetVmPendingInterrupt @ 0x00449210` | Its sole direct caller is `PlayerUpdateMovementAndOptions @ 0x004250B0`; target checks the ID, then calls this helper with the ID and interrupt 1. Current Player source performs the corresponding check and state update through `FindPlayerVm` and `PlayerSetManagedVmDeleteState(&player->modeVmId, 1)`. A direct call through `g_AnmRenderManagerView` was tried and reverted; with the real Player support source, `/GL` emitted 67 bytes versus 54 with 3/50 comparable bytes. Preserve the helper in `src/AnmManager.cpp`; reopen for target-supported private-ABI evidence. |
| Defer | `AnmRenderManagerView::SetVmPendingInterruptAndExecute @ 0x00449250` | Target has 15 call sites across nine callers, six mapped to FrontEnd owners. `/GL` builds with `FrontEnd.cpp` support under direct `UpdateDifficulty` and `UpdateOptions` entries both emit a 72-byte PDB contribution versus 76 target bytes. Target places a four-byte `LEA ESP,[ESP]` at `0x0044927C` before the child loop; a natural guarded do/while rewrite still emits 72 bytes. Keep the source semantic and exactness open; don't add manual padding. |
| Defer | `SetVmPosition @ 0x004492F0` | Five callers include `GuiMessageVmView::Run`, `PlayerUpdateMovementAndOptions`, and `EnemyRuntimeUpdate`. `Gui.cpp` reaches the method through `SetMessageSide`, but a `/GL` graph with GUI support emits 102/90 bytes and only 5/86 comparable bytes. Keep it in `src/AnmManager.cpp`; do not reshape the natural manager call without better lowering evidence. |
| Defer | `MarkLoadedVmsForDeletion @ 0x004493E0` | Eight callers include maintained FrontEnd and ANM-release paths. The natural list traversal in `src/AnmManager.cpp`, with `FrontEnd.cpp` support, emits 82/92 bytes and 18/92 comparable bytes. Revisit after compiler-context evidence explains the register and loop-shape differences. |
| Defer | `0x004086B0` playfield-bounds sibling, 91 bytes | Its callers `0x00408030` and `0x00408100` span mixed object/update paths. Leave it unmapped until TH10 evidence establishes a shared source owner or a coherent module for its caller path. |
| Completed | `ItemVectorView::FromAngleMagnitude @ 0x0041BEB0` | TH10-local ownership is now established: `0x0041AED0` allocates `0x21CEC0` bytes and vector-constructs `0x896` records of `0x3F0` bytes from manager `+0x14`; `0x0041AD90` registers update/draw callbacks, while `0x0041AFD0` iterates those records and `0x0041BB00` creates them. The maintained x87 helper reproduces all 30 bytes in normal COFF and in two independent canonical linked-PE cold replays. `ItemVectorView` is descriptive, not a recovered original symbol. |
| Completed | `GuiScoreView::Add @ 0x00409D90` | The target adds `amount / 10` to `this+4`, stores the sum, and caps values at or above `1000000000` to `999999999`. `GuiMessageVmView::Run` is a real caller, and the pre-existing `GuiScoreView::Add` in `src/Gui.cpp` becomes target-shaped when the equivalent comparison is spelled `score >= 1000000000`. Two independent canonical linked-PE cold replays in that real `/GL` caller context reproduce all 46 relocation-free bytes raw-equal. Maintained placement does not prove the original production TU. |
| Completed | `GameScoreStateView::AddFaith @ 0x0041BE80` | The target updates global state `0x00474C40+0xC` by signed `amount / 10` and caps at `99999`. TH10 itself identifies the maintained semantics: item-update paths pass 10, 100, and difficulty-scaled 5000/8000/10000; the GUI renders the same stored value as five digits and uses it in stage-clear score; boss timeout subtracts 3000 with a floor of 5000. In the adjacent polar-helper `/GL` context the helper becomes exactly 46 bytes; two independent canonical cold replays reproduce all bytes raw-equal. The maintained type/method names are descriptive. |
| Next | ItemManager callback gap `0x0041B8E0`, `0x0041BA00`, `0x0041BA30` | Re-auditing raw target bytes disproved the old apparent gap after `0x0041AFD0`: `0x0041B8E0-0x0041B9F8` is a draw-side item loop, `0x0041BA00-0x0041BA2F` is the registered update callback, and `0x0041BA30-0x0041BA4C` is the registered draw callback/tail path, with `CC` separators. These entries are missing from the old candidate inventory and should be reviewed as small/medium ItemManager functions before returning to unrelated large bodies. |
| Completed | `AnmRenderManagerView::PreloadAnm @ 0x00447280` | The same-TU `AsciiManagerView::Initialize @ 0x00401110` entry reproduces the complete 81-byte PDB contribution and all three linked fields in two independent canonical linked-PE cold builds. Directly testing the stop byte's `0x80` bit naturally emits the target `JS` branch; an equivalent `while` rewrite leaves codegen unchanged. Ten other target callers remain source-unmapped, and original source ownership stays unknown. |
| Defer | `AnmRenderManagerView::ServicePreloadedAnms @ 0x00447700` | The sole observed caller `0x0041FF80` pushes the manager pointer, and the target callee consumes it with `RET 4`; a pinned VC7.1 ABI smoke supports a nonstatic `__stdcall` member. The maintained source now expresses that ABI and the observed 33-slot scan, release sequence, and post-clear null-slot write. Its selected `/GL` image is the correct 135-byte extent with 83/123 comparable bytes and three resolved linkage fields, but target and candidate use different registers for manager base and entry. Exact neighbors `FindVm` and `GetVmPosition` replayed cleanly after the header change. Reopen only with new caller/compiler evidence; do not force register allocation. |
| Defer | AnmLoadedView::SetAndExecuteScriptIdx @ 0x0043E8B0 | Its sole target caller is AnmSetVmScriptIndexAndExecute @ 0x004496D0. Selecting that direct same-TU caller as /GL entry still emits 287 bytes with 249/271 comparable bytes and four linkages; target keeps script index in EDI and script pointer in ESI, while the natural candidate swaps them. Keep source natural and exactness open. |
| Defer | AnmRenderManagerView::SetVmWorldPosition @ 0x00449350 | Four target callers contribute five call sites. Its body keeps the position input in ESI. The 114-byte same-TU candidate (18/91 comparable bytes, five linkages) used AsciiManagerView::Initialize as entry, which is not a direct caller. Keep it diagnostic and revisit after the real caller graph or private-register evidence improves. |

These are bounded exact-reconstruction tasks only. Exact helper replay does not
close the whole-product Windows i386 build gate.

## Enemy dispatcher: current recovery point

Use the maintained source at HEAD. Historical Enemy measurements in
KNOWLEDGE_BASE and Git explain how the source got here; they are not alternate
baselines to restore. The 14,228-byte typed-field measurement below is the last
recorded result for its selected four-source linked graph, not a fresh replay
of the later all-real-caller graph. Its named scratch reports were pruned.
Linked-PE probe reports have no exactness authority.

| Measure | Selected candidate | Target |
| --- | ---: | ---: |
| Complete contribution | 14,228 | 14,416 |
| Pre-table span | 13,572 | 13,760 |
| Stack frame allocation | 0x2C4 | 0x2C4 |
| Normalized comparable bytes | 746 / 11,556 | 11,556 / 11,556 |
| Selector bytes | 181 / 181 | 181 / 181 |
| Physical selector-group order | matches | matches |
| Suffix | 43 | 43 |
| START_SPELL physical group | 214 | 219 |

Retained source facts:

- Dispatcher float reads use the generic `EclVmContext::ReadFloat` boundary;
  target `0x00412A60` is an unreferenced adapter. Do not restore the older
  explicit `EclVmContext *context` local merely because it scored 696/11,536;
  that variant introduced false adapter calls.
- Opcode `0x1A8` writes through the recovered `EnemyBulletPatternView` array.
  Its four float operands have natural case-local source lifetimes, and target
  stores prove typed float speed fields at pattern `+0x18` and `+0x1C`.
  Maintaining those fields as an overlay on the existing 0x210-byte words view
  makes VC7.1 form the destination in the target shape (`LEA` of scaled index
  plus runtime, then stores at `+0x2DC/+0x2E0`). Under the selected linked
  context this keeps the owner at 14,228/14,416, changes the case from 171 to
  173 bytes versus target 206, and raises normalized agreement from 735 to
  746/11,556. The target still homes the three saved float operands at
  ESP+0x10/+0x30/+0x18 while the candidate uses +0x18/+0x40/+0x38; it also
  consumes and rematerializes the rank factor where the candidate keeps it live
  on x87. This remains a non-exact checkpoint. The raw-offset spelling is
  superseded.
- Rank-float selection uses the two target-observed shared
  ReadFloat/ResolveFloat/store tails. Do not restore a private third 0x15E tail.
- Opcode `0x119` uses the target's strict-positive tests (`0.0 < value`). This
  changed the broad heuristic score from the preceding rank-tail checkpoint's
  731 to 730/11,556 at that checkpoint while fixing unordered/NaN behavior and
  branch shape. The later retained 0x1A8 source work supersedes that whole-owner
  score; the old 731 score is not a better checkpoint.

Canonical exact dependencies already protected around this owner include
`EnemyMarkPendingInterrupt` (65 bytes), `EnemySoundQueueView::QueueSoundSample`
(123), `QueueSoundCue` (149), `EnemyDropVectorView::FromAngleRadii` (30), and
`EnemyRuntimeView::EnemyRuntimeView` (145). The ANM render-layer creator family
also remains independently exact in its canonical context. Re-run the focused
exact-unit gate after changing this translation unit; giant-owner diagnostics do
not override accepted small exact units.

Open problems (independent; select one bounded experiment):

1. **Private integer-reader ABI.** Target `ReadIntArgument @ 0x00412A00` is a
   16-byte adapter with private `EAX=runtime`, `ECX=index`. The maintained
   callgraph has the target-observed 30 direct adapter calls and a 16-byte
   candidate adapter, but its private registers are reversed. This difference
   propagates into rank/difficulty integer cases and whole-function coloring.
2. **Shared-tail physical ownership.** `AIM_BULLET_AT_PLAYER 0x1AE` reaches the
   same float Resolve/store tail as rank/difficulty selection in the target;
   semantic routing is recovered, but VC7.1 still places the shared block under
   the wrong physical group. Moving equivalent C labels alone does not change
   this.
3. **`SET_BULLET_COUNT_BY_DIFFICULTY 0x1B4`.** Target has two local difficulty
   trees whose branches select immediate indices before one generic ReadInt join
   per tree; the second join also changes the owner register lifetime. The
   retained group is 140/151 bytes. Several locally closer 128..226-byte source
   variants worsen whole-owner agreement and are superseded.
4. **FireLaser cross-owner LTCG ABI.** Target EnemyFireLaser @ 0x0041C510
   receives ESI=manager, EDI=request, plus one stack type argument and RET 4
   across ten calls in six owners. The entire caller topology is now
   source-present: the dispatcher supplies four calls, while src/EnemyLaser.cpp
   supplies EnemyLaserBoundaryStateView::UpdateBoundary @ 0x0041CFD0,
   type-0 box/circle collision @ 0x0041D880/0x0041DD80, and type-1 box/circle
   collision @ 0x0041EB00/0x0041EFA0. The independently exact
   EnemyLaserVectorView::FromAngleMagnitude @ 0x0041F800 remains 30/30.
   The type-1 owners preserve the target 256-byte hit bitmap and 12-unit scan;
   they keep the leading unhit prefix in the current laser and respawn later
   gaps through zero-initialized 0x77-dword type-0 requests carrying angle,
   width, terminal-distance, type/color and speed 8.0f.
   In the first complete all-real-caller graph, central EnemyFireLaser remains
   154/153 with 29/137 normalized agreement and the dispatcher remains
   14,320/14,416 with 699/11,556. Therefore missing caller coverage is no
   longer a viable explanation for the private ABI mismatch. Free-function
   parameter-order variants keep the wrong ESI=manager/EAX=type/stack-request
   ABI. Member-method variants recover RET 4 but still keep type in EAX and
   request on the stack. Rewriting the central type selection as switch(type)
   reproduces the target sub/dec branch shape and leaves type on the stack, but
   rotates the remaining registers to EDI=manager/ESI=laser/stack-request.
   The next frontier is the Type0/Type1 constructor private ABI: target
   constructors keep the allocated object in EBX and use ESI=-2, whereas the
   the recorded diagnostic constructors keep object in EDX and use EBX=-2. Do not
   fake EDI=request with synthetic dependencies; recover the constructor/source
   lifetime that naturally produces the target register coloring.

5. **Whole-function spill/stack coloring.** Large laser/spawn scratch objects are
   uniformly eight bytes below target stack offsets despite the correct 0x2C4
   frame and understood object sizes/order. At dispatcher entry the target
   homes runtime/current-instruction/opcode state at ESP+0x30/+0x1C/+0x10,
   whereas the recorded candidate uses +0x40/+0x14/+0x1C. In 0x1A8 the target
   later reuses the runtime's +0x30 home for a saved float operand; the
   case-local checkpoint similarly reuses the candidate runtime's +0x40 home.
   This ties the remaining rank-speed stack mismatch directly to whole-owner
   lifetime allocation. Treat it as a lifetime/spill symptom, not missing
   padding or dummy locals. Simple declaration permutations and `#pragma
   var_order` are byte-identical to the baseline and are closed directions.
   The PLAY_ANM 0x107 three-byte gap is another symptom of the same
   entry-register state problem.

Known negative/superseded experiment classes that should not be repeated without
new target evidence:

- volatile/dummy owner reloads, fake dependencies, padding, or dummy locals;
- direct exposure of recovered Spawn/EndSpell/screen-effect bodies to the
  dispatcher solely to steer LTCG;
- the 2026-09-22 EndSpell/GUI full-seam experiments, which produced only
  14,180/14,416 and at best 669/11,568 (later variants also had incomplete
  normalization), well below the retained checkpoint;
- changing `EnemyRuntimeView::owner` from the full host pointer to the base ECL
  host type solely for alias coloring; it produced byte-identical owner metrics;
- source-order-only moves of spawn cases or shared-tail labels;
- replacing all ReadInt calls with direct generic reads, or forcing a synthetic
  local/static/private wrapper ABI;
- memcpy/aggregate rewrites chosen only because a short local sequence looks
  closer; several caused unrelated cross-case folding and worse whole-owner
  diagnostics;
- changing accepted exact ANM creator ABI/TU shape to improve this giant owner.

Useful supporting owners remain non-exact: `EnemySpawn @ 0x0040CFB0` is
577/577 in its focused diagnostic but not byte-exact, and
`EnemyFullObjectView::EnemyFullObjectView @ 0x0040D830` is 672/673 with
137/653 normalized comparable bytes.

### Focused Enemy probe

Set `analysis_dir` to a new `.analysis/<agent>/<campaign>` directory and create
its manifest before writing these outputs, per the Factory artifact policy.

    scripts/repo-python scripts/probe-ltcg-backlog.py --source src/EnemyEclDispatcher.cpp --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmVmCreate.cpp' --profile-flag=/GS --json > "$analysis_dir/enemy-probe.json"

    scripts/repo-python scripts/report-ecl-dispatch-table.py --candidate build/probe-ltcg/src_EnemyEclDispatcher.cpp/source.exe --candidate-function-address "$candidate_address" --json > "$analysis_dir/enemy-layout.json"

After the probe, set `candidate_address` to the linked entry address from its
JSON; do not hard-code an old candidate address. When the worktree contains
unrelated edits, use committed HEAD snapshots for support translation units
rather than silently incorporating dirty source.

## ANM executor: recovery point

The maintained source covers opcodes -1..92 and remains non-exact. Two
diagnostic contexts are intentionally kept distinct:

- the selected source-shape checkpoint is contribution 9,960 with pre-table
  9,584 versus target 9,588 and 705/8,608 agreement;
- the exact-creator split ExecuteScript context is 10,040 bytes with pre-table
  9,664. It is useful for understanding caller/TU effects, not as a replacement
  baseline.

Both retain all 92 physical groups in target order. Do not compare their total
sizes as if they were the same link graph.

The four render-layer creators Variant0/2/1/3 are independently canonical
exact 73-byte units with the target EDI hidden-return ABI. Variant0 remains a
natural AnmLoadedView member, but its maintained definition lives in
src/AnmVmCreate.cpp; that is a maintained TU boundary, not an original
filename claim.

Open executor problems are still block placement, float temporary homes,
END/stop ownership, and child/helper private ABI effects. FindVm/VM-id value
semantics should not be regressed to raw integer APIs merely to change layout.
After a focused probe, use scripts/report-anm-execute-table.py and compare
physical groups, not just contribution size.

## Generic ECL runner: recovery point

The earlier `b3a74c1` checkpoint restored target 0x0044DF70 as
EclVmContext::StartSubroutine(caller, firstArgument) rather than a
three-argument global helper. The member spelling naturally reproduces the
target-observed RET 8 exits: the receiver is private and only the two explicit
arguments are callee-popped.

In the selected real-host /GL diagnostic:

| Measure | Candidate | Target |
| --- | ---: | ---: |
| Complete Run contribution | 7,020 | 7,020 |
| Pre-table span | 6,692 | 6,692 |
| Normalized comparable bytes | 945 / 6,264 | 6,264 / 6,264 |
| Physical opcode-group order | matches | matches |

Normalization is still incomplete, so equal contribution/pre-table/order is
not exactness. StartSubroutine itself remains 522/550 and SpawnThread remains
close but non-exact. The open classes are x87 arithmetic stack homes, format
opcode 0x1E metadata/cursor registers, and remaining private register choices
around ReadInt/stack operations. All accepted EclVm exact units must continue
to replay zero-difference while iterating on Run.

## Exact regression gates

While iterating on the three large owners, replay exact units sourced from the
changed translation units:

    scripts/repo-python scripts/replay-exact-units.py --source src/EnemyEclDispatcher.cpp

    scripts/repo-python scripts/replay-exact-units.py --source src/EclVm.cpp

    scripts/repo-python scripts/replay-exact-units.py --source src/AnmManager.cpp

Before a checkpoint:

    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/validate-tracking.py
    scripts/repo-python scripts/progress.py
    scripts/repo-python scripts/ci.py
    git diff --check

Checkpoint commits follow the active operator's prefix instruction. The active
operator has requested pushes for substantive progress in this session; other
sessions must follow their current operator authorization and repository rules.

## Local analysis retention

.analysis/ is disposable scratch and is never exactness authority.
KNOWLEDGE_BASE.md may retain historical .analysis/... provenance paths even
after those local files are pruned.

Keep only:

- one current probe/layout pair per active giant owner;
- scratch needed for an unresolved experiment that will be resumed immediately;
- small provider/boundary caches that are expensive to regenerate locally.

Delete after a checkpoint:

- superseded probe/layout JSON and duplicate whole-owner scans;
- copied source snapshots, temporary support TUs, patches and disassembly dumps
  whose conclusion is already in tracked docs or Git;
- old PDB/map/PE/COFF products and historical linked-image snapshots;
- interrupted/zero-byte outputs;
- negative experiments invalidated by later source/call-graph changes.

The 2026-09-23 audit found no retained file artifacts under `.analysis/`.
Historical probe/layout names above and in KNOWLEDGE_BASE are provenance, not
files to open or baselines to trust without a fresh source-bound replay.

Historical paths named by KNOWLEDGE_BASE are provenance labels and may be absent
after their conclusions have been compacted into tracked evidence. Do not treat
a missing historical scratch file as loss of exactness authority.

Do not preserve scratch merely because an old KNOWLEDGE_BASE row names it.
Accepted exactness lives in config/match-units.toml, config/matches.csv,
tracked ledgers, and successful canonical replay, not in .analysis/.
