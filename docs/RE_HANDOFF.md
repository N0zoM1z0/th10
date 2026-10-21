# TH10 reconstruction handoff

Updated 2026-10-21. This file describes the current checkpoint. Historical
experiments remain in Git, the linked evidence packs and
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md); they are not a live work queue.

## Resume safely

- Target only original Japanese TH10 v1.00a at ignored `resources/th10.exe`:
  SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Recover tracked and untracked work before editing. The pre-existing untracked
  `docs/evidence/d3d-adapter-format/` is independent, unadopted work; preserve it.
- Use `scripts/repo-python`. Inspect `AGENTS.md`, [RE_WORKFLOW.md](RE_WORKFLOW.md)
  and [ORACLES.md](ORACLES.md) before target-dependent work.
- Current operator-authorized checkpoints use English `gpt-6.1-sol: ...`
  messages and are pushed to `origin/main` after validation.
  The operator requests direct local tools, not Factory MCP. This session uses
  Bash and, when needed, read-only Ghidra headless.
- Serialize native builds at nice10 or lower priority. Ghidra review uses
  read-only/noanalysis, rollback, one CPU and 768 MiB. Verify disk identity and
  complete mapped text before/after temporary metadata work:
  401000..465A80 inclusive, 412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  The old 64A80-byte hash is a truncated prefix, not complete mapped text.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current checkpoint

The operator has reopened reconstruction for both large and small functions,
including ANM ExecuteScript, ECL Run and Enemy DispatchEclInstruction. This work is active;
none of the three is exact yet. Re-evaluate old source/API assumptions against
fresh target and compiler evidence. The first bounded changes and measurements
are recorded in [large-owner call order](evidence/large-owner-call-order/README.md).

Live totals are generated in [PROGRESS.md](PROGRESS.md): 1776 reviewed
candidates, 735 authored functions owning 273576 bytes, 1085 source mappings,
1235 canonical exact functions overall. Authored exact is **394 functions,
72153/273576 bytes (26.37%)**. The 208 mapped authored nonexact functions,
523 indeterminate origins and unresolved inventory gaps remain open; the
authored denominator is provisional. The SVG now visibly fills the authored
bar from current classified exact bytes and labels the ratio provisional.
The complete-game denominator remains unknown.

The latest source checkpoint restores PlayerUpdateShots' actual pending3 call
at428370->40C4D0, replacing an unresolved misleading DeleteState3 alias. Two
menu helpers pass their existing typed IDs to FindVm. Complete target876/native738
caller decode verifies the direct helper entry and biased iterator identity:
full-shot+20/+24 versus+40/+4 both reach ID+44, stride5C. The callers remain
nonexact; no ordinary score is given to unequal extents.

Current two-/five-TU helpers remain65/63/all2. Both input pointers are EAX;
the residual bytes are the loaded ID payload in ECX versus target EAX. The
prior private inputECX interpretation was wrong. Stdcall and consistent member
controls are restored. Canonical row404/all16 cold-replays; current33-input
large diagnostic recompiles Player once/reuses ten objects/fresh anchors and
retains46 helpers5335/all142. Run7020/all189/62locals stays27, exact171-byte
callee and all7020 target-replayed bytes equal the prior payload. Its generic
decoder flag is false while strict table-field/selector coverage remains complete.
Current ANM7478/7995/90groups/gap2 and Enemy14464/frame2C4/19/42groups/gap650
are remeasured. Original large goals and later phase gates remain open.

See [pending3 source/caller proof](evidence/player-pending3-route/README.md).
Current additional carrier: `build/gpt-6.1-sol/ecl-square-order-20261021/current-player`.
The prior real-player33-input source pin is historical after this Player change;
its earlier measurements retain their original scope. The accepted ten-TU
canonical carrier below is unchanged. Four owned terminal native directories
are pruned/4379425 bytes; source/producer/comparison snapshots and selected5TU
caller failure remain. Recovery: `.analysis/gpt-6.1-sol/ecl-square-order-20261021/RECOVERY.md`.
No new exact credit: the visible authored progress bar remains26.37%.

The preceding bounded checkpoint closes PlayerSetShotVmInterrupt2 at40C480,
complete65 bytes and both address fields, without changing maintained C++.
Fresh local Ghidra confirms eight callers and pending-only root-child propagation.
Two independent existing three-TU Player constructor cold builds freeze23
inputs and reproduce the full new65-byte owner plus the existing273-byte
Player reset guard/all eight combined fields. Only these shared-artifact units
are replayed. See [Player interrupt evidence](evidence/player-shot-interrupt/README.md).
ANM restart remains287 bytes/265 matching replay bytes/all four fields; initializing
helper remains197/203 in the real eleven-TU context. Three private natural
source controls fail to improve it and preserve all three large native bodies
raw. No source/profile/entry change is adopted. Two neutral native directories
and one unexecuted source are pruned/2603728 bytes; retained source/cache pins
remain current. Recovery: `.analysis/gpt-6.1-sol/anm-script-restart-20261021/RECOVERY.md`.
The original three large goals remain active/nonexact.

The preceding child/creator investigation corrects three stale90/91 caller comments:
source and target both route88->448D00,90->448F60,91->448E30,92->449090. No C++
routing change is needed. A new bounded source/target/PE-PDB caller checker
validates optional GetInt then creator/GetVm/InsertAfter across eight static
paths per image. It grants no parent/callee byte credit. Synthetic24-layout/
192-path and16 strict negatives, plus four actual source/copied-native negatives,
pass. See [child/creator evidence](evidence/anm-child-creator-context/README.md).

Current ten-TU context passes41/45 complete guards4978/all120; its four creators
are complete74/73 and unsupported before scoring. The existing canonical
creator recipe is a separate three-TU /GL/no-GS context, not this diagnostic.
Appending only real Player.cpp
recompiles that one TU and reuses ten verified objects, freezing33 inputs. It
restores initializer ECX/two-stack/RET8 and all four73-byte creator guards:
45 complete helpers5270/all140. Initializer207/217 and world116/118 stay nonexact.
Fresh direct Ghidra lists46 initializer incoming calls; no complete caller/source
closure is claimed. The additional eleven-TU diagnostic is
`build/gpt-6.1-sol/anm-child-routing-20261021/real-player`; it does not supersede
the accepted canonical carrier/source pins below.

Eight controls compile18 game TUs total, including one consistent full11-TU
private vector-type cold. Direct temporary child91 regresses aligned ANM to7460;
named script operand and most initializer/copy forms preserve their entire
native bodies. The private storage-base cold preserves all five measured bodies
raw. Flag-before-third changes only its207-byte helper. None is adopted. All
Run7020/all189/62locals retain the identical complete target-replayed bytes and27
residuals;171-byte callee stays exact. Fresh eleven-TU ANM stays7478/7995,90/92,
gap2; dispatcher14464/frame2C4/19sentinels/42target-sized groups/gap650. No parent
or case exact credit. All three original large goals remain active.
Eleven owned terminal/unexecuted directories are pruned/8444651 bytes. Recovery:
`.analysis/gpt-6.1-sol/anm-child-routing-20261021/RECOVERY.md`. Original C++/headers/
profiles/recipes/exact totals and independent D3D work are unchanged.
All105 standard CI checks pass in one invocation. Final two-image routing,
16 synthetic negatives,45 full helper guards5270/all140, fresh current ANM
and expected Run7020/27, toolchain/tracking/progress/docs/whitespace gates pass.
All33 maintained inputs,15 canonical artifacts and32 retained native artifacts
remain current after CI. No full historical or extra canonical replay is run.

The preceding bounded large-owner investigation tests the ANM timer tail.
An unchanged private ANM positive and two ordinary full Tick expansions (scale
first versus subframe += scale) preserve every raw byte of all three complete
native bodies9964/7020/14464. Each retains35 helpers4424/all118. ANM stays7478/
7995 case-aligned,90/92 target-sized groups,gap2. Adding /Op only to the ANM
support grows its owner to10140, scale56->82 and RGB820->769;33 guards3548/all98
remain exact. The initial cached-anchor link is terminal-failed on8 new math
symbols; private anchor recovery (11 total added symbols) reuses all game
objects and completes. Run stays27 with all7020 target-replayed bytes equal;
Op dispatcher's21 unbound fields receive no ordinary score/identity claim.
No source/header/profile/recipe/ledger is adopted. The circle-cancel source and
current canonical carrier pins below remain live. See
[ANM timer evidence](evidence/anm-timer-sequencing/README.md).
Three terminal private duplicate images are pruned/3124040 bytes; the completed
Op failure and full source/compile/guard receipts remain. Recovery is
`.analysis/gpt-6.1-sol/anm-timer-sequencing-20261021/RECOVERY.md`.
All104 standard CI checks pass in one invocation. Fresh current ANM9964
case measurement remains7478/7995,90/92,gap2; complete Run/all189/62locals
retains27 ordinary differences. Final toolchain/tracking/progress/docs/diff
checks pass; all32 raw inputs,15 current canonical artifacts and the retained
profile failure artifacts remain current after CI. No extra canonical replay
is needed for unchanged production source/interfaces.

The latest exact checkpoint closes EnemyCancelManagerView::CancelBulletPattern at
408100..408203: **260 new authored exact bytes/all5 fields**. This existing
reviewed authored owner gains one source mapping; its boundary/disposition and
denominator do not change. The old <= comparison excluded unordered inputs,
unlike target TEST AH,5/JNP. Negated > restores that path. Ordinary derived/base
float3 copying retains Z and the full frame18; traversing actual BulletRuntimeView
with literal0.5 recovers EAX manager, EBX point and RET12. Four private source
controls execute; only the typed record/vector is adopted. Original class/TU/
profile/data ownership remain unknown. See
[circle-cancel proof](evidence/enemy-circle-cancel/README.md).

Two independent full ten-TU GL/GS colds freeze32 equal recursive raw inputs
(the real BulletManager.hpp adds one). Each passes35 complete owners4424/all118:
new260 plus34 prior guards4164/all113. Canonical cold2 selects only new260,
one shared group; no full historical replay. The current carrier remains
`build/match-linked/EnemyItemDrops`, pinned by this new proof. Independent cold1:
`build/gpt-6.1-sol/enemy-circle-cancel-20261021/maintained-cold1`.
Both colds have identical raw complete circle/ANM/Run/dispatcher bodies.
ANM9964 and Run7020 retain complete native equality to the preceding carrier
after verified field rewrites. Current Run/all189/62locals stays27 ordinary
differences,171-byte float guard exact. Dispatcher is now14464/14416, frame2C4,
19sentinels,42/108 target-sized groups and absolute case gap650 (previous14468,
40/108,gap652). Case-aligned matched comparable bytes2980/11152 are diagnostic
only. All three original large owners remain active/nonexact.

Fresh direct Ghidra observes five direct calls and full260-byte body followed
by twelveCC. Complete mapped-text identity is unchanged. Four strict native/
template controls reject;6480 live-object host scenarios and ten intended-
assertion mutants pass under sanitizers. Six inspected terminal/unexecuted
owned directories are pruned/6719057 bytes; accepted colds,279-byte failure,
source/producer/strict/guard reports and copied-native controls remain.
Private recovery is
`.analysis/gpt-6.1-sol/enemy-circle-cancel-20261021/RECOVERY.md`.
All104 standard CI checks pass in one invocation. The six affected existing
normal-COFF helpers pass197 bytes in one additional shared compile under their
original profile. Total selected canonical replay7 units457 bytes/two groups;
32 raw inputs,30 linked artifacts and one additional COFF object remain current.
Final focused260/all5, expected Run7020/27 and standard toolchain/tracking/
progress/docs/whitespace gates pass. Independent D3D
work is untouched; native-product/runtime/semantic/port gates stay open.

The preceding checkpoint closes EnemyCancelBulletRecord408030..4080F6,
199/all4, with branch-local state/animation transitions. Its31-input source and
mutable canonical carrier pins are now historical; its independent cold1 at
`build/gpt-6.1-sol/enemy-cancel-record-lifetimes-20261021/maintained-cold1`
is retained. This prior body remains a complete199-byte guard in both current
images. See [cancel-record proof](evidence/enemy-cancel-record-lifetimes/README.md).

The preceding ECL diagnostic executes eight source/profile controls, adopts
none, and retains27 current ordinary differences. Six controls preserve every
native/replayed byte; direct Pop addressing and unsigned comparison storage
fail strict binding/extent checks. Older two-TU allocator color weights remain
unverified in this ten-TU graph. See
[ECL control evidence](evidence/ecl-value-lifetimes/README.md). Its source and
mutable-carrier pins are now historical after the cancel-record source change.

The preceding exact checkpoint closes EnemyFinalizeDeath40E5F0..40E691:
**162 new authored exact bytes/all9 fields**. Real QueueSoundCue and
CreateVmAtWorldVariant0 replace receiverless free aliases; noinline on the
existing shared ExtendFaithTimer and explicit position/drop-block lifetimes
recover target ESI/EBP, dead parameter-home reuse for the ANM result, and RET4.
Ordinary source/annotation is compatible evidence; original declaration/TU/
profile remain unknown. Two independent complete ten-TU GL/GS colds through
real dispatcher freeze31 equal raw inputs and each pass30 complete owners
3656/all104. Canonical cold2 selects only new162 plus modified existing Faith93
in its prior two-TU no-GS recipe: two units255 bytes/two groups. No boundary,
origin, source-map or denominator expansion. See
[death-service proof](evidence/enemy-finalize-services/README.md).

The preceding death-service proof pins the earlier contents of
`build/match-linked/EnemyItemDrops`; those mutable-cache/source pins are now
historical. Its independent cold1 remains
`build/gpt-6.1-sol/enemy-finalize-real-services-20261021/maintained-cold1`.
Complete ANM9964, Run7020 and dispatcher14468 native bodies retain equality
after verified field rewrites; fresh complete Run/all189/62locals stays27
ordinary differences. The three original large goals stay active/nonexact.
World creator116/118, Spawn795/819, RuntimeUpdate2356/2412 and item update2232/2320
receive no exact credit. All nine observed timer references are direct calls;
no original noinline annotation is inferred. Dense162 endsRET4 before14CC.

Host projection passes432 live-storage/order scenarios and7 strict mutants
under sanitizers; three native/template controls reject. Four inspected owned
terminal directories are pruned/5342450 bytes; final colds and165-byte failure
remain. Private recovery is
`.analysis/gpt-6.1-sol/enemy-finalize-real-services-20261021/RECOVERY.md`.
An initial canonical group preflight rejects conflicting output directories
before compiling; corrected recipe uses the existing same-graph output.
Source/profile/entry/field contracts remain unchanged. No failed cold is counted.
Unrelated D3D evidence is untouched.
All102 standard CI checks pass in one invocation; final focused255/all11,
toolchain/tracking/progress/docs/whitespace gates pass. All31 raw inputs and31
final native artifacts remain hash-current after CI.

The preceding checkpoint closes EnemyDropItemCounts40C9A0/48/all3 and
EnemyDropItemCountsCore40C9D0/503/all27: **551 new authored exact bytes**.
All four item callers now use the real ItemManager receiver; the core uses one
ordinary unsigned conversion per draw, in-place three-float storage, outlined
WrapAngle, direct count indexing and ordinary stdcall. Eleven kinds are processed
and twelve words cleared. Two independent full ten-TU GL/GS colds through real
dispatcher with actual ItemManager.cpp freeze31 equal raw inputs; each passes28
complete owners3401/all93. Canonical cold2 selects only the new pair and modified
normal-COFF WrapAngle94: three units645 bytes/two groups. No denominator or
source-map expansion. Three native/template controls and four host mutants reject;
228 maintained-body host scenarios pass under sanitizers. See
[item-scatter proof](evidence/enemy-item-scatter/README.md).

Current large carrier is `build/match-linked/EnemyItemDrops`, independently
corroborated by `build/gpt-6.1-sol/enemy-tail-packet-20261021/final-cold1`.
The proof pins31 current raw inputs, four artifacts and nine support objects per
cold. Earlier30-input nine-TU and16-input two-TU pins are historical after the
Enemy source edits. Full ANM9964 and Run7020 retain native equality after verified
fields; the current complete Run7020/all189/62locals diagnostic retains27 ordinary
differences. Dispatcher14468/14416 changes two ordinary bytes but retains frame2C4,
19 sentinels,40/108 target-sized groups and gap652. No large exact credit.
Real Item Spawn795/819 and FinalizeDeath246/162 remain nonexact. Fresh target
prologue has packet ESI, unlike one old private ECX assumption; later branches
reload opcode after operand calls, so cached opcode is unsupported. Native1B4's
third ReadInt is shared from1AA; the physical105/106 site count is not a missing
source read. All three original large exact goals remain active.

Sixteen inspected terminal native/host directories are pruned/17808307 bytes;
private sources/receipts, final colds and selected failures remain. The private
pack is `.analysis/gpt-6.1-sol/enemy-tail-packet-20261021/`; old probe.py assumes
pre-adoption source hashes and must not be rerun over accepted source. No Factory
MCP, no subagents; unrelated D3D evidence is untouched.
All101 standard CI checks pass in one invocation. Final focused645-byte
comparisons, toolchain/tracking/progress/docs/whitespace gates pass;31 raw inputs
and27 final native artifact hashes remain unchanged after CI.

Two further ECL thread-expression controls recompile only EclVm.cpp and reuse
nine verified objects. Both retain28 complete helpers3401/all93. Naming the
resolved thread ID moves its CALL field247->245: complete7020 is unbound, with
no ordinary-difference score. Naming only the raw unsigned ID retains every
byte of all three native large bodies and all7020 target-replayed Run bytes;
27 differences remain. No source adopted. One terminal duplicate control is
pruned/1040840 bytes; failure retained. See the item-scatter proof and private
`.analysis/gpt-6.1-sol/ecl-thread-expression-lifetimes-20261021/RECOVERY.md`.

The preceding checkpoint closes ReplayFile::Open44B6B0/137/all12. Ordinary
stdcall declaration/definition recover target RET4 and reuse of the no-longer-live
path home for the failure message pointer. The old135-byte cdecl form remains
wrong even in the real LoadReplay graph. Two independent complete FileSystem +
ReplayManager GL/GS colds through real LoadReplay freeze18 equal raw inputs and
strictly pass five owners496/all47 each. Canonical replay selects only12 directly
affected FileSystem/ReplayManager units1541 bytes in their original contexts.
Three controls reject; no boundary/origin/source-map/denominator expansion.
The two-TU loader improves468->458/450 but is still nonexact. See
[read-open proof](evidence/replay-open-context/README.md).

FileSystem.hpp changes one of30 actual nine-TU inputs. Only AnmManager.cpp
consumes it. Necessary support recompilation plus eight hash-verified reused
objects and relink retain all three complete native bodies after verified field
rewrites;21 complete target guards retain2415/all52. Run7020/all189 stays27
differences in this real graph; no large exact credit or new independent large
cold was claimed there. That preceding carrier/pins are historical after the
current item-scatter source changes.

The distinct integer-value signature flag/context/value, unlike old
context/flag/value, is tested in a consistent copied two-TU cold. Seven complete
helper guards919/all17 pass, but full Run7020/all189/62locals stays26 differences
and is completely byte-equal to the baseline after verified field rewrites.
Null guard before const cursor declaration instead yields61 differences at7020,
with the same seven guards. Both rejected; production EclVm/Enemy untouched.
Three terminal native control directories/802006 bytes are pruned with identities
and exact source/report recovery retained. All three original large goals stay
active. See the private ecl-integer-value-context-20261021 recovery note.

The preceding checkpoint admits the missing screenshot callback420540..42066D
and closes its complete302 bytes/all30 fields. The prior308-byte unresolved
gap is now302 owned bytes plus fourCC before/twoCC after; no prior extent is
overwritten. Natural cdecl callback source uses real FileSystem open/write/close
definitions, preserves target failure behavior, frees/clears both buffers and
clears the thread handle. Fresh direct Ghidra shows one external entry data
reference42083F and no external interior reference; metadata is rolled back.
Two independent complete ten-TU GL/GS colds through real WinMain freeze24 equal
raw inputs and pass18 full owners3764/all214 each. Canonical cold2 selects only
new worker302 plus affected parent504: two units806 bytes/one group. Wrong info
offset and copied-native14->15 header-count controls reject. An old warm fallback
alias for the newly defined callback is also rejected; fresh harnesses pass
without changing the Oracle. Main now has24/24 tracked source-present authored
owners,20 exact, four nonexact/3206 bytes. Original class/name/TU/profile and
product/runtime/semantic/port gates remain open. See
[worker proof](evidence/main-screenshot-worker/README.md).
All100 standard CI checks pass in one invocation. Final focused/toolchain/
tracking/progress/docs/diff gates pass. Four owned terminal ECL control builds
are pruned/1024282 bytes; complete source/receipts and two failures remain.
Both worker cold proofs, the stale-alias failure and current large carrier are
retained. Unrelated D3D work is untouched.

Six private ECL lifetime/consumer controls are rejected; production EclVm and
Enemy source/header stay unchanged. Fresh full comparison verifies current
retained two-TU Run7020/all189/62-local,171-byte callee exact,26 ordinary bytes:
four thread-register bytes, four commuted addressing bytes,16 stack homes and
two common-latch bytes. It is not only a stack swap. Snapshots39, shared integer
result27, rebound cursor7052 and shared POD record6940 do not improve it.
Explicit arithmetic and named angle-result controls preserve its COMPLETE
target-replayed bytes, beyond equal scores. Private source/producer/receipts
are in `.analysis/gpt-6.1-sol/ecl-operation-lifetimes-20261021/RECOVERY.md`.
Main.cpp is not in the actual nine-TU large graph; all30 raw inputs and four
artifacts/eight support objects remain pinned current. No extra large rebuild
is required. All three large exact goals remain active/nonexact with their
prior complete native measurements unchanged. The current large carrier stays
`build/gpt-6.1-sol/main-surface-outlines-20261021/large-current`.

The preceding checkpoint closes surface release438A30/43, screenshot request
service438A60/111, Present4391F0/236, screenshot420670/504 and
CaptureToTexture448270/240: **1134 new authored exact bytes/all39 fields**,
with three new source mappings. Natural real bodies recover private registers
and ESI save lifetimes; the two release calls and FileCheck remain explicitly
outlined. CaptureToTexture reloads its texture after Flush/GetBackBuffer,
matching both target owner loads. Its finalC22000 is the complete three-byte
RET32, correcting the old two-padding-byte claim without changing240 ownership.
Two independent full ten-TU GL/GS colds through real WinMain freeze24 equal
inputs and strictly replay16 complete owners3325/all172 each. Canonical cold2
selects only the five new units and affected original FileCheck111/OpenFile360:
seven units1605 bytes across three groups. Main now has23/23 source-present
authored owners,19 exact and four nonexact owning3206 bytes. See
[screenshot context proof](evidence/main-screenshot-context/README.md).
At that checkpoint the302-byte screenshot callback was only independently
reviewed for address identity; its inventory/source/exact admission is now
completed in the newer worker proof above. Keep the observed allocation/format
error behavior; no runtime, product or original-owner/profile claim is made.

Because AnmManager.cpp changed, the actual nine-TU large graph recompiles that
one support and reuses eight hash-verified unchanged objects. All30 current
inputs and full artifacts are pinned in the new proof. Complete ANM9964,
Run7020 and dispatcher14468 native bodies remain equal after verified link
field rewrites. Existing target differences persist: ANM90/92 groups/gap2,
Run27 ordinary differences/all189 fields (separate two-TU26), Enemy gap652,
frame2C4 and19 sentinels. The current large carrier is
`build/gpt-6.1-sol/main-surface-outlines-20261021/large-current`.
Only Main.cpp, FileSystem.cpp and the bounded capture helper in AnmManager.cpp
change; shared headers remain unchanged. The three original large exact goals
remain active. The old checksum proof's large-input pins are now historical.
All100 standard CI checks pass in80+20 segments. The first invocation stops
at the strict whole-file ANM host pin after80 passed; whole-file reversal and
unchanged extracted RNG proof justify only that LF pin refresh. The actual same
CI implementation resumes81..100; no uninterrupted pass is claimed. Final
toolchain, tracking, progress and diff gates pass. Cleanup removes five owned
terminal native builds/2602263 bytes, retaining both cold proofs, two failures
and the current large carrier. Unrelated D3D evidence remains untouched.

The preceding checkpoint closes CalcExecutableChecksum43A1C0/207/all9 fields.
Two independent complete five-TU colds replay11 owners2191/all133 per image;
the new context above retains every one of those guards. Its private negative
ANM source/interface controls remain useful evidence, not a live work queue.

At the preceding checkpoint, eight ANM source/interface controls were rejected. The complete four actual
getters and Float2/Float3/alpha/RGB evaluators pass as part of28 owners5180/all168.
Member and ordinary free stdcall GetFloatVar both reproduce424/all44, so source
ownership is not recovered by the private ESI protocol. A full nine-TU ordinary
unused-receiver manager-member ExecuteScript control also preserves its entire
native9964-byte owner and Run7020, with28 guards intact. G6/G7 and one natural
local-pointer rename are neutral. Wrapper/cdecl variants regress. Do not repeat
these hypotheses or infer target exactness from native equality; no ANM source
change from those controls was adopted. The source/control/artifact identities are in the checksum
proof and private `.analysis/gpt-6.1-sol/anm-execute-flags-20261021/RECOVERY.md`.

The preceding checkpoint closes WindowProc4390E0/265 and retained
ActivateWindow43A1B0/8:273 new authored exact bytes/all17 fields. A real
MidiOutputView::UnprepareHeader call plus common final DefWindowProcA source
return recovers the target private MIDI arguments and four live callback
parameters. Standard LRESULT/stdcall source and /GL /GS remain unchanged.
Two full Main/GameErrorContext/FileSystem/Midi colds through real WinMain freeze
14 identical inputs and strictly replay nine owners1624/all98 per image. The
second cold replays the two new units together and separately replays affected
CreateGameWindow341 in its original context: three units614 bytes total.
MainConfigurationDialogProc233 gains source but stays nonexact even at the same
size. Main has source for all20 reviewed authored owners;14 are exact and six
remain nonexact. See [window proof](evidence/main-window-callback/README.md).
One changed header input requires a new primary-only large-graph rebuild with
eight verified unchanged support objects. Complete large native bodies remain
equal after verified field rewrites,21 existing guards2415/all52 pass, and
ANM9964/gap2, Run7020/all189/27 differences and Enemy14468/gap652 remain nonexact.
The then-current large carrier was
`build/gpt-6.1-sol/main-window-callback-20261021/large-header-context`;
source/artifact pins are in the window proof. Product/runtime gates remain open.
All100 standard CI checks pass in one invocation; final focused gates pass.
Cleanup removes20 owned terminal native directories/5801419 bytes, retaining
the first actual final cold, canonical cold, MIDI negative and current large
carrier. Source deltas, input pins and reports remain. Three large goals stay active.

The preceding checkpoint closes CheckForRunningGameInstance439FF0/442 and
ResolveShortcut43A290/272:714 new authored exact bytes, one new source mapping.
The real SDK COM body recovers its private EBX/EDI arguments naturally. The
caller restores its Fatal receiver, uses the target byte-wide GameWindow+28
path flag, and keeps the nonnull-title path first. Two independent complete
Main/GameErrorContext/FileSystem /GL /GS colds through real WinMain strictly
replay714/all38 plus four existing guards558/all40,1272/all78 per image.
The second cold canonically replays only the two new units in one build.
Target GUIDs/imports, full boundaries and byte-field accesses are independently
reviewed; cdecl, swapped-GUID and wrong-addend controls fail. Stdcall and
fastcall source produce the same private body, so original annotation/TU/profile
remain unknown. Main source closure still lacks8-byte ActivateWindow and
233-byte dialog code. WinMain1311/1389, rendering1051/1159, checksum199/207 and
OpenFile297/360 are separate nonexact observations in this three-TU carrier.
See [startup source proof](evidence/main-startup-shortcut/README.md).
All100 standard CI checks pass in one invocation. Cleanup removes9 owned
terminal native directories/2126392 bytes, retaining both final cold carriers,
one cdecl failure and verified source/control deltas. Final focused gates pass.
The original ANM/Enemy/ECL C++ sources are unchanged, but Main.hpp changes
one of30 nine-TU inputs. Only EnemyEclDispatcher.cpp consumes it. A necessary
primary-TU recompile with eight hash-verified unchanged support objects keeps
all3 complete large native bodies equal after verified field rewrites;21
existing complete guards retain2415/all52. Current ANM9964/gap2, Run7020/all189
with27 differences and dispatcher14468/gap652/frame2C4/19 sentinels persist.
This is an incremental dependency check, separate from both startup colds;
all three large exact goals stay active.

The preceding checkpoint closes the retained45-byte InitD3DInterface439700 owner.
Calling the existing GameErrorContext::Fatal restores its omitted error-context
receiver and target EDI save/setup/restore. Two independent complete Main.cpp +
GameErrorContext.cpp /GL /GS colds through real WinMain freeze9 inputs and
strictly reproduce45/all5 fields plus the same-image existing Fatal201/all11
guard. The second cold uses the new canonical main-init-d3d-interface recipe.
Only45 authored bytes are credited; no denominator/source-map changes.
WinMain1298->1307/1389 remains nonexact. Supervisor491C28+4 is bound from the
known base and actual layout, not a coinciding neighboring data-anchor name.
See [D3D error-context proof](evidence/main-d3d-fatal/README.md).
All100 CI checks are covered in92+8 segments after correcting a missing new
validation-file link; code and recipes remain frozen. Final focused checks pass.
Cleanup removes17 owned terminal native directories/4073413 bytes; selected
proof/failure carriers and source/control inputs remain.

Fresh ECL controls leave production EclVm/Enemy source unchanged. A completed
read-only pinned-c2 stack-packing observation plus complete Run7020/all189
replay retains26 differences in the two-TU graph. FloatResult/logicalResult,
FORMAT offset and X share one color(weight36); scratch/Y/DX share another
(weight9). This is not just an equal-weight X/Y slot tie. Scoped float results
restore geometry/FORMAT homes but move integer arithmetic/logical homes and
retain32 ordinary differences. Direct/scoped results, bool types, operand
references/unions, installer declarations and the real callback support are
not accepted. Native/control recovery is under
`.analysis/gpt-6.1-sol/ecl-operand-representation-20261021/`.
FileSystem/PbgArchive real-callee and /Ob1 diagnostics also remain nonexact;
no caller is invented for retained LoadArchive4357D0, which has no raw direct
or literal-entry reference. These controls remain separate from canonical
recipes. All three original large functions remain active.

The preceding checkpoint supplies real AnmInt3Scale.cpp in the dispatcher graph,
migrating the six shared canonical recipes from eight to nine TUs. Only that
support uses the already reproduced /GL /GS /Ob1 profile; no C++ source or
semantic target bindings change. Two independent full nine-TU colds strictly
replay21 existing complete owners2415 bytes/all52 fields with all30 source pins
equal. RGB Evaluate780->820 and scale56 now match in this carrier. ANM
ExecuteScript remains9964 but aligned diagnostic positions improve7250->7478
of7995;90/92 target-sized spans and absolute gap2 remain. Run7020/all189,
62 local destinations retains27 ordinary differences; dispatcher14468/14416
retains gap652,40/108 target-sized spans, frame2C4 and19 sentinels. No new exact
or denominator credit. See [real dependency proof](evidence/enemy-real-scale-support/README.md).
ECL diagnostics now accept an explicit context manifest while retaining
canonical target bindings/callee templates; wrong-target and poisoned-template
controls pass. New carrier identities supersede the eight-TU image identities.
The separate RGB and other canonical contexts remain unchanged. Remaining
anchors keep the diagnostic image non-runnable; all three large owners are open.

The preceding checkpoint changes only two final rank-count reads to local generic
host views. Target EBX owner reloads for indices10/8 and the rank-five interval's
eleven integer calls/five returns are recovered. Two independent full eight-TU
colds reproduce the dispatcher at14468/14416 and retain19 complete existing
exact owners1539/all32, with all29 source pins equal. Rank-five is434/420;
absolute case-size gap652 instead of690, with40/108 target-sized spans,
frame2C4 and19 sentinels. Root generic integer calls103->105 versus target106;
the remaining physical-call deficit is specifically1B4, target3/candidate2.
Native-to-native case comparison binds internal destinations explicitly and
finds101/108 old/new spans equal, leaving seven changed or unbound. Two copied
byte/destination controls and a stale-field-cache control reject false equality.
Run7020/all189 retains27 ordinary differences. No new exact or denominator
credit. See [rank-count evidence](evidence/enemy-rank-count-owner/README.md).
The canonical `build/match-linked/AnmReleaseSlot` carrier has this newer proof's
identities. Difficulty-count host and conditional-call controls recover its
third call but regress rank-five to nine calls/four returns and are restored.
Generic macro/prologue views are neutral; biased pattern-pointer, direct-rank
and boolean controls are rejected. All five adapter bodies are unchanged.
The eight-TU carrier still anchored AnmInt3View::operator* despite the maintained
AnmInt3Scale.cpp body. The separate RGB recipe uses /GL /GS /Ob1 for that support. Adding the real known body in a separate declared dispatcher
diagnostic graph was subsequently tested in the real dependency proof above.
It establishes the declared bounded context, without production closure.
All100 standard CI checks pass. Cleanup reclaims9148041 bytes from nine
owned superseded native directories and eight cloned analysis files.

The preceding source checkpoint closes five Enemy operand adapters at412A00/16,
412A60/14,412A70/14,412A80/17 and412AA0/26:87 bytes total. Viewing the loaded
full owner as EclVmHost before reading activeContext recovers the target
intermediate loads, private registers and scheduling. Both target-proven
dereferences remain; runtime itself is not the host. Two independent full
eight-TU colds pass19 complete owners1539 bytes/all32 fields, with all29 source
pins equal. Five origins remain indeterminate, so authored exact stays375/68104.
The canonical carrier shares `build/match-linked/AnmReleaseSlot`; older carrier
identities are historical. Source/API and /GL /GS profiles are retained.
Dispatcher is now14404/14416,40/108 same-sized cases, absolute case-size gap690,
frame2C4 and19 sentinel comparisons. Polar399/405 and other differences remain.
Run7020 retains27 ordinary differences/all189 fields in this actual eight-TU
graph; the separate two-TU26 measurement remains distinct. ANM source is unchanged
and its bytes are not replayed in this bounded checkpoint. See
[operand host-view proof](evidence/enemy-operand-host-view/README.md).
Separate-TU and definition-order integer-adapter controls are neutral and
restored; the direct raw-float expression has the wrong ECX receiver.
All three original large owners remain open. The direct integer-read macro
was subsequently tested in the rank-count proof above and was byte-neutral.
All100 standard CI checks pass. Inspected cleanup reclaims8639051 bytes from
seven superseded native directories and29 intermediate analysis files.

The latest diagnostic checkpoint repairs Run table coverage when Capstone stops
inside the jump table instead of near the end of the selector bytes. Complete
PE fields plus explicit unlinked suffix coverage still compare every owned byte;
16 synthetic tests and three copied-image controls guard rejection behavior.
A fresh full two-TU /GL cold confirms Run7020/all189 fields with26 ordinary
differences. Fifteen same-image existing guards retain1587 bytes/all21 fields.
All source/type/API controls are restored. EnemySpawn cold-replays577/all12;
all100 standard CI checks pass. Cleanup removes2,592,154 bytes from10 owned
obsolete directories. StartSubroutine is currently551/550;
the older522 diagnostic is stale. No new exact credit. See
[current ECL evidence](evidence/ecl-current-dependencies/README.md).

The latest source checkpoint reconstructs ASCII camera/viewport effects where
an earlier interface had only a declaration. Both draw owners now capture the
viewport index before flush, publish/configure the camera, reload active/device
state, set the viewport and then write the configured flag. Native offsets are
asserted partial views; allocated storage aliases remain open. Newline paths
now skip spacing, and each regular glyph marks dirty bit8 after copying
dimensions. Real StageDraw support gives regular726/738 and GUI572/570
bytes; high callback7/10 remains nonexact. Eighteen same-image complete guards
pass6596 bytes/all266 fields; two existing canonical units cold-replay337 bytes.
Host viewport checks pass72 scenarios/eight strict mutants; glyph checks pass198/three,
all with sanitizers. No new exact credit. All99 standard CI checks pass; cleanup removes9,058,987
bytes from10 owned superseded native directories, in addition to the explicitly
cancelled earlier CI fixture. See
[viewport source evidence](evidence/ascii-viewport-transitions/README.md).
Original large-owner work remains active; integer signatures, FORMAT state,
and two FLIP_X controls from this round are restored. The new two-TU ECL baseline supersedes its old carrier; the eight-TU Enemy
carrier has since been cold-rechecked in the operand host-view proof above.

The preceding checkpoint changes ReadFloatValue to ordinary explicit-context
static source, updating its three real callers. Two independent full two-TU
cold builds preserve Run7020/all189 fields and reduce ordinary differences
28->26. The existing171-byte callee retains canonical exactness in two further
single-TU colds; all16 existing ECL units retain1617 bytes. The actual eight-TU
Enemy graph reduces Run29->27 and retains identical complete dispatcher14524
and raw-float forwarder26 hashes. No new exact credit is claimed. The original
static declaration and production TU remain unknown. See
[float call order](evidence/ecl-float-value-call-order/README.md). All97 standard CI
checks pass. Cleanup removes3,376,996 bytes from five owned superseded outputs.

The preceding checkpoint closes HintLinkView::Append41AB10/52, formerly
maintained as free HintAppendLink. Ordinary member source recovers the complete
raw-equal EAX-head/EDX-node body, including ESI and loop alignment; cdecl versus
fastcall on the free source was machine-code neutral. Two full four-TU colds
replay seven complete owners1038 bytes/all36 fields, with all18 input pins and
seven owner hashes equal. Original class/name/TU remain unknown. See
[list member proof](evidence/hint-list-member/README.md). The current carrier
is still `build/match-linked/HintReadLine`, now pinned by this newer proof.
Loader1533/1498 and constructor51/nonexact remain open. An aggregate link-init
control grows the constructor to57 and is restored.

The preceding checkpoint closes HintReadLine419F40/260 through the real
four-TU Hint/Main/FileSystem /GL /GS graph. Ordinary stdcall source recovers
both RET12 exits. Two full colds replay six complete owners986 bytes/all36
fields, including the existing OpenFile360/all26 guard. All18 source pins and
six owner hashes agree. The host parser suite explicitly models x87 evaluation
and still kills22 strict mutants; the original game control word/runtime remain
unknown. Packed BGRA storage reduces the nonexact constructor72->51 bytes;
float-expression source and the dependency context reduce LoadHintFile to1533
versus1498. See [line-reader proof](evidence/hint-read-line/README.md).
The current carrier is `build/match-linked/HintReadLine`; prior HintLoad pins
are historical. The constructor's origin stays indeterminate with no credit.

The preceding checkpoint closes HintSplitLine41A050/75. An explicit
colonless early return recovers target branch layout. Two full cold builds
replay the complete static owner and all4 fields, plus three existing guards
291 bytes/all2 fields. Explicit static tail JMP binding additionally requires
the whole raw-equal trim callee and maintained target source/name mapping;
the original static CALL default remains unchanged. See
[split proof](evidence/hint-split-tail/README.md). The canonical HintLoad
carrier and14 source pins in this proof supersede the prior helper carrier.
The ECL two-float value-object control grows Run7020->7024/frame108->110;
it changes the second spill to10C instead of target14 and is restored.

The preceding checkpoint closes HintTrimWhitespace41A0A0/114 and
HintLookupValue4198C0/98 through the real LoadHintFile two-TU graph. Two
independent cold builds reproduce both complete static owners raw-equal and
retain ClearHintLists79/all2 fields. Natural stdcall lookup source recovers
the target callee cleanup; whitespace source already matched. Host parser
checks pass with22 strict mutants. See [helper proof](evidence/hint-small-helpers/README.md).
Seven restored negative large-owner build directories were pruned (5,399,291
bytes); their source controls and reports remain. The three large owners
remain nonexact.

The preceding checkpoint closes RGB Evaluate441600/820. Two independent
five-TU /GL /GS canonical colds reproduce all17 target fields. Only the new
`src/AnmInt3Scale.cpp` support TU uses /Ob1; its migrated56-byte operator
retains existing credit. Twelve same-image existing guards retain4579 bytes
and183 fields. Each image therefore passes14 complete owners5455 bytes/all203
fields. Actual-source host checks pass13 scenarios,9 strict mutants and
ASan/UBSan/leaks. See [RGB proof](evidence/anm-rgb-interpolation/README.md).
Earlier Float2/Float3 and alpha exact units remain intact in these guards.
Original physical source/TU/profile identity is still unknown. All97 standard
CI steps pass after the final source freeze; current raw inputs and native
artifact hashes remain unchanged. Cleanup removes about977 MB of verified
obsolete outputs; historical source/report evidence remains.

The whole Windows i386 product graph is open. Diagnostic anchor images are
not runnable products; host tests are not original/native-product execution.
Semantic reconstruction and portability have not started.

## Open large owners

These are scoped historical diagnostics, not exact claims. Rebuild the actual
graph after any dependency changes before comparing scores.

| Owner | Last measured context | Remaining work |
| --- | --- | --- |
| ANM ExecuteScript43EE30/9964 | Current two full ten-TU circle-cancel colds have identical complete9964 bodies; equality to the preceding carrier requires verified field rewrites. Fresh timer-batch diagnosis remeasures7478/7995,90/92 groups and gap2 | Nonexact. Registers, flag scheduling, intervals61(+1)/2(−1) and common tail remain open. No target exact credit from native equality. |
| ECL Run44E1A0/7020 | Current full ten-TU circle-cancel graph: complete7020/all189/62locals,6993 matched,171-byte float callee exact | Nonexact,27 ordinary differences. Historical separate two-TU26 is not current. Thread argument, SIB, stack-home and common-latch differences remain. [Current evidence](evidence/enemy-circle-cancel/README.md). |
| Enemy DispatchEclInstruction40E770/14416 | Current two full ten-TU circle-cancel colds:14464/14416, frame2C4,19sentinels,181 selectors/108 groups in target order;42 target-sized groups, absolute gap650 | Nonexact. Instruction lifetime, storage, register scheduling and stack homes remain open. Historical105/106 integer-call counts and1B4/1AA shared-tail addresses have not been freshly counted here; do not use them as current missing-read evidence. No parent or case exact credit. |

Old rejected controls are scoped observations, not proof that an API or source
shape is correct. Require a concrete target hypothesis and compare corresponding
instructions; an unchanged unaligned score does not establish code neutrality.
Gain new TH10 caller, lifetime, partition or profile evidence. Keep target
observations, cross-game hypotheses, compiler observations and unknowns distinct.
For the dispatcher, opcode1B4 already contains three genuine source integer
reads. A preceding complete bounded CFG review found exactly three generic
ReadInt calls on every selected static return path in those two images: native404A4C is
physically inside1AA and shared by1B4; target41054F/410580/4105BE stay in1B4.
Calls are assumed returning and predicates unconstrained; no argument/runtime
equivalence is claimed. The earlier two-versus-three region count is layout,
not a missing read. Review tail sharing and packet lifetime before adding one.
The reused integer iVar27 mixes the initial packet and unrelated later values.
A distinct typed packet was already tested in the preceding private nine-TU
context and regressed to14476/gap662; do not describe that control as untested
or assume a new context fixes it without fresh target lifetime evidence.

## Reproduction and retained local state

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-cancel-bullet-pattern --json
scripts/repo-python scripts/test-enemy-circle-cancel.py --mutations
scripts/repo-python scripts/report-anm-execute-table.py --check
```

The latest operator instruction explicitly requests only necessary affected
replays, serialized resource-conscious builds and periodic artifact cleanup.
Replay only the necessary affected units; no full historical replay was run
for this checkpoint. Existing guard recipes retain their original contexts;
the RGB pack records explicit additional same-image guard overrides. The
scale recipe alone was migrated to the five-TU graph.

The retained canonical RGB carrier is `build/match-linked/AnmRgbInterpolation`.
Its source pins are historical after the ASCII changes; the current diagnostic
carrier and focused guards are in the newer ASCII proof.
Verify all12 raw project-input hashes and four artifact hashes against
`docs/evidence/anm-rgb-interpolation/native-summary.json` cold2 before reuse.
Older ANM carrier/source hashes are historical. The replaced
`AnmSetupVertexBuffer` carrier and nine discovery build directories are pruned.
Cold1 output was superseded by cold2; both complete reports and identities
remain in the private pack. Any rebuild replaces this cache and its pins.

Private review/recovery material is under
`.analysis/gpt-6.1-sol/th10-anm-rgb-value-graph-20261020/`: final reports,
source pins, target/Ghidra review, compact controls and verified source deltas.
Read its `RECOVERY.md` before using old runners: they can restore the old
starting source in `finally`; do not rerun them over accepted work.

The issue reference remains an ignored shallow sparse checkout at
`_reference/th10-decomp`, levzzz5154/th10-decomp,
commit `c6791d82dc9f02e55eca45a5314f900a85481efd` (MIT).
Adjacent TH08/TH09/TH095 and reference source are hypotheses only.

## Checkpoint completion

Latest focused caller/payload diagnostics are in the pending3 proof; earlier
routing/creator diagnostics remain in the child/creator proof;
exact results are in
the circle-cancel proof; death-service/item-scatter/read-open/worker/RGB results retain
their historical scope. Run the standard gates
before the next commit; regenerate progress after ledger changes:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

Clean only inspected, attributable artifacts after retaining reproducible
evidence. Preserve unknown work, D3D evidence, private target, databases,
toolchains and reference checkout. Final validation and cleanup totals are in
[the current validation summary](evidence/player-pending3-route/validation-summary.json).
