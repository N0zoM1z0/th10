# TH10 agent handoff — 2026-10-29

## Start here

Current checkpoint restores Main's actual lifetime and complete293-byte shutdown.
Five new complete owners518 bytes/all42 fields pass two independent30-input colds:
425 authored and93 indeterminate. Typed configuration construction, automatic
worker destruction, FPS teardown and MIDI timer cleanup are source-backed.
WinMain1422/1389 and Options3196/3188 remain nonexact. The next concrete WinMain
lead is its103-byte Chain destructor438880; the old window-init label is wrong.

Read [AGENTS.md](../AGENTS.md), this handoff and the relevant source/ledger rows.
Consult [workflow](RE_WORKFLOW.md) and [Oracle policy](ORACLES.md) for the lane
being used. [Historical sessions](RE_HANDOFF_HISTORY_20261028.md) are reference
material, not a startup reading requirement.

Operator direction:

- Advance substantial functions and useful leaves; compare complete source early.
- Use local tools directly, including `scripts/ghidra.py`; no Factory provider.
- English local commits: `gpt-6.1-sol: ...`, specified author/coauthor in AGENTS.
  Save substantive progress and never push.
- Recover dirty work. Only replay new/affected exact units, not the whole cohort.
- Compile/link serially, one CPU at nice10, bounded with retained exit logs.
- No routine host failure/null/exception scenarios, sanitizer/leak sweeps or
  mutation campaigns. Default CI is seven lightweight repository checks.
- Keep complete extents, every field, fixed local targets, callee bindings and
  ABI strict. Size equality/near matches grant no partial exact credit.

Target: original Japanese TH10 v1.00a, ignored/read-only `resources/th10.exe`,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Use `scripts/repo-python`; verify the target before target work and attest local
Ghidra before using its observations. No `/mnt` search or target substitution.

## Current facts

Authoritative counts are generated in [PROGRESS.md](PROGRESS.md):
**86,034/275,729 reviewed authored owned bytes exact, 465 exact authored owners**;
678 authored source mappings. Total: 1819 candidates, 1193 mappings, 1337 exact.
The denominator remains provisional; product build/runtime closure is still open.

Latest [Main lifetime proof](evidence/main-lifetime-20261029/README.md):

- Construction421F00/71, implicit destruction41FF60/22, game shutdown420270/293,
  FPS deletion413450/84 and timer destruction4203C0/48 pass every byte/all42 fields
  twice. The new22-byte destructor has one registered-thunk entry ref and no
  interior refs. Construction/destruction remain origin-indeterminate;425 authored
  and93 indeterminate bytes added with no authored denominator change.
- Main now constructs the real configuration+120 before worker+62C. The consumed
  packed4 view covers the784h-byte clear and preserves all offsets; original
  allocation stride/tail/packing declaration remain unproven. Global491C28 has
  real static construction/member destruction in the native graph.
- Shutdown preserves global/receiver ownership and stops/frees mode, FPS, render,
  text, input, timer and archive resources. Timer StopTimer member context recovers
  293 from291; destructor48 retains both period-end calls. The archive source uses
  its existing pinned /GL /EHsc /Ob1 recipe, with the complete86-byte callee exact.
- Eighteen explicit old same-image guards8415/all371 pass; the changed Hint
  interface and FrontEnd CreateVm also pass their original recipes. The new graph
  has CreateVm90/95 and OpenFile312/360; these are not callee-byte exact claims.
  No historical cohort or comparator/host-scenario campaign was run.
- Full Options controller is3196/3188, not the historical1128-byte candidate.
  Natural member role is a complete native-to-native neutral control and restored.
  GameManager destructor role still1120/1105 and is restored. WinMain1422/1389,
  mode release133/128, reset125/125 and D3D init1152/1159 remain nonexact.
- Actual WinMain438DAB passes its48-byte Chain to438880/103. This stops Main worker,
  removes both sentinel lists and destroys callback slots under EH. Restore the
  real Chain lifecycle next; its old generic window-initialization label is wrong.

Latest [Main configuration proof](evidence/main-configuration-20261029/README.md):

- MainLoadConfiguration420870..420A85 owns534 bytes and60 address fields. Reset,
  file read/copy/free, six byte-domain/version/size checks, key-map publication,
  options/logging, vsync and unconditional writeback/error reporting are complete.
  Adds534 authored exact bytes and one mapping; reviewed denominator unchanged.
- Existing WinMain438B5C pushes Main and supplies EBX path. Natural stdcall source
  replaces the old cdecl extern; first530/534 had two plain RETs, corrected534
  reproduces target RET4 and every ordinary byte through the actual caller.
- Every configuration field binds Main491C28+120; the default table binds474E88
  plus0/4/8/C/10. Nearest anchor labels are not semantic evidence. Original key
  data/log storage, identifiers, TU and product/runtime remain open.
- Two independent canonical29-input colds pass the new owner and seven selected
  direct/ABI guards1217 bytes. Reset remains125/125 with nine differences;
  WinMain1438/1389 is still nonexact and unchanged in size from the prior graph.
  No full cohort, comparison-infrastructure change or host scenario is claimed.

Latest [GameManager launch proof](evidence/game-manager-launch-20261029/README.md):

- Factory4180E0/107, callback417C70/12 and Main launcher420EA0/118 pass all13
  fields in two independent canonical28-input colds. The callback was missing
  from inventory; all-byte refs find only its owning factory entry reference.
  It remains origin-indeterminate;225 authored and12 indeterminate bytes added,
  authored denominator unchanged. Four mappings362 include nonexact reset125.
- Actual GameManager60 constructs timer/config before its full clear; it calls
  device slot14, stores mode+5C, publishes477810/flag4 and launches the real
  initializer. The callback forwards its result with plain RET; initializer
 1039/1009 and cleanup1120/1105 remain nonexact in this context.
- Main worker+62C is the existing shared1C worker; old closeRequestBlocked+63C
  is its running field. Launcher holds critical section6/counter6 and expands
  Stop/beginthreadex under lock. Natural EDI receiver/entry+context/RET8 survives
  every ordinary byte. Existing Supervisor113 now uses the actual field and passes.
- Configuration constructor127 includes an imposed returned receiver; void reset
  is125/125 with nine ordinary prologue/temp-register differences. Unsigned byte
  layout fixes format/hint ordering. Free-function role is neutral and restored;
  bool+1B is rejected by actual loader's three-value domain. No consumed return
  contract is established; do not manufacture a zero return to force registers.
- Forty-four explicit affected old contracts10922/all402 pass per image. No full
  historical replay, comparator change or host scenario. Device operand binds
  Main491C28+8, irrespective of misleading nearest diagnostic-stub labels.

Latest [FrontEnd launch proof](evidence/frontend-launch-20261029/README.md):

- Four new authored owners520/all37 and retargeted old helper65/all2 pass two
  independent canonical27-input colds. Three new mappings cover455 target bytes;
  existing65-byte FrontEnd mapping becomes its member name. New168-byte callback
  is game-authored by its actual game-specific control/refs, independently of
  compiler equality; denominator increases168. All-byte refs find only owning
  branches and the two factory entry data references. Boundaries/CC gaps are strict.
- 477820 is the existing Startup object, correcting the prior statistics label.
  The actual wait uses frames+3EC; its unchanged3F0 view moves to Startup.hpp.
  Registration preloads slots25/26 through447280, not synchronous LoadAnm447080.
  Both actual registered callbacks retain their old7-byte exact adapters.
- Natural member receivers recover CreateVm's stack-owner/EDI/RET4 and the
  interrupt method's EAX-owner/ECX-index/SI ABI. Explicit value ID copy fixes its
  remaining operand differences. No register annotations or fake arguments.
- The1128-byte parent moves372 ->1100 ->1076 through complete in-place logic and
  real callees. Mode2 returns to replay screen12/4315C0, not practice11/431EE0.
  Raw demo loop has a local index, contradicting the decompiler's global-loop
  presentation. Draw-task flag2 replaces the old transition navigation label.
  The parent remains nonexact; no partial code/table credit. Inline void setter
  and mode-spelling controls are byte-neutral and parked; setter restored.
- Thirty-four explicit affected old contracts10143/all351 pass in each image.
  VM dependencies, lifecycle, callbacks, displays, Startup and worker interfaces
  are preserved. No full historical replay, comparator or host-scenario campaign.

Latest [FrontEnd lifecycle proof](evidence/frontend-lifecycle-20261029/README.md):

- Five new complete owners1159/all75 plus existing nullable delete18/all2 pass
  independent canonical25-input colds. Constructor204 and new size6 retain
  indeterminate origin; FrontEnd destructor491, Ending destructor362 and factory96
  retain authored origin.
  The authored denominator is unchanged. Four new mappings cover797 bytes; Ending
  already had its mapping.
- Sole vtable46ECF0 points to42C5B0, a size query returning sizeof5ACC; the
  destructor is nonvirtual. Target worker is+5AB0, tasks+C/+10, flags+4.
  Plain ID storage avoids default zero writes before the full clear. Existing
  UI and ScriptViewer contracts retain their complete bytes.
- Static fastcall/cdecl and node-wrapper controls were neutral for four remaining
  destructor bytes. Ordinary chain->UnregisterElem(element) naturally selects the
  actual promoted element-ECX/chain-EDX interface and solves all four. Every source
  call site uses it. Its canonical prior four units772 pass a separate cold.
- The handler binder adds only this named destructor with literal member+5AB0;
  Ending remains fixed+D0. Complete same-module24-byte region, immutable metadata,
  saved-this+4 and actual16-byte worker cleanup stay strict. Five positives/41
  rejection controls and the prior Ending constructor cold pass. Address binding
  grants no generated-code, EH-runtime or product credit. Explicit old same-image
  guards pass in both FrontEnd colds; public proof records the counts.

Latest [Supervisor mode proof](evidence/supervisor-modes-20261029/README.md):

- New complete exact owners: update41FF80/113, generic unlock421C50/25,
  BGM fades421E00/225 and nullable deletes418150/18,42CDB0/18. The first three
  retain authored origin; the last two remain indeterminate. Existing23-byte
  stage helper421B70 is retargeted from unchanged legacy source to the typed
  GameScoreState member, with no new bytes/mappings or origin change.
- Two independent canonical22-input colds pass all six selected owners422/all18.
  Thirty-four explicitly selected older same-image contracts6535/all221 pass
  per image. No full historical replay, comparator or host-scenario campaign.
- Full mode boundary is4218D0..421B69: code572 + tables64/16 + byte map14.
  Every table slot reaches a body instruction; all-byte refs find only owning
  reads. Raw MOVZX base421B5C supersedes Ghidra's provisional421B5B association.
  Six CC bytes precede independent421B70. Reviewed authored denominator gains94
  owned data bytes; the nonexact parent gets no exact/table credit.
- Seven new complete source mappings cover1269 target bytes. Registration's real
  callback escape changes update107/privateESI into113/ECX. Fade225 is Sound
  owner492590 maintenance, superseding old generic-cleanup navigation labels.
  Capturing the current sound per test recovers225 from273 and preserves reloads
  between calls. Stack-owner/RET4 cleanup interfaces are independently checked.

Latest [Startup state-field proof](evidence/startup-state-field-20261029/README.md):

- Registered callback41FEB0/63 and all3 fields pass two independent canonical
  nineteen-input colds. State slot491FB8 is existing Supervisor base491C28+390.
  Correctly using nextGameMode removes the old independent state external and
  recovers the direct final flag clear; no register or profile control is used.
- Seven prior explicit selectors556/all33 pass in both images, plus23 older
  same-image contracts6235/all179. The EDX update copy41FD90 remains nonexact;
  existing origin and boundary decisions are retained. No authored credit.

Latest [statistics cleanup proof](evidence/statistics-cleanup-20261029/README.md):

- Destructor42AEF0/45 and nullable global release42AF90/76 are complete/exact,
  all7 fields. Five affected Startup selectors also pass435/all26 in both colds;
  23 older same-image contracts6235/all179 pass separately.
- Three previously omitted CC-delimited whole extents add186 reviewed bytes.
  Scalar deleting42AFE0/65 is compiler-excluded. The two maintained/exact bodies
  remain origin indeterminate; no authored numerator or denominator change.
- Startup now deletes the typed statistics owner instead of duplicating its
  buffer cleanup. The complete parent remains433/417; real member destruction
  does not by itself solve the normal EH state write or register lifetimes.

Latest [startup lifecycle proof](evidence/startup-launcher-20261029/README.md):

- Constructor41F850/128, RegisterTasks41FAC0/140, factory41FD00/69,
  ReleaseStartupResources41F930/91 and Draw adapter41FEF0/7 pass two independent
  nineteen-input colds. All26 fields and complete extents are strict; no byte or
  ABI exception is introduced. Twenty-three old contracts6235/all179 pass in
  each new image, including worker Start/Stop/destructor and the streaming owners.
- The actual object has worker+10, VM+30, scalar id+3DC and whole3F0 clear.
  Wrapper-id construction introduced a zero absent from the target. A retained
  worker reference recovers its actual ESI launch lifetime. The callback still
  escapes unchanged to _beginthreadex; this does not fix thread's ECX homing.
- Five omitted CC-delimited extents are now reviewed: scalar delete41FD50/27 is
  compiler-excluded; nullable delete41FD70/18, duplicate update41FD90/63,
  registered update41FEB0/63 and Draw adapter41FEF0/7 stay origin indeterminate.
  Existing Decomphelp already contains the update helpers; its TU is untouched.
  Constructor128 also retains its old indeterminate origin. Authored denominator
  is unchanged. Nine new mappings cover1198 bytes, including the legacy helper.

Latest [stream creation/startup proof](evidence/sound-disk-startup-20261029/README.md):

- The first actual startup-context comparison leaves disk681/702 and memory651/651
  nonexact. Saving/checking each COM HRESULT separately recovers both complete
  owners, including disk's eleven internal alignment bytes and both EH protocols.
- Two independent canonical16-input colds pass all2130 bytes/all60 fields. The
  first also passes two old COFF destructors147/all6. Thirteen relevant old linked
  contracts pass3932/all112 in the new image, including real FillBufferWithSound,
  queue, StopBgm, ReopenBgm, loader join and Log interfaces.
- The allocation EH opt-in can explicitly pin another aligned negative disp8 slot;
  both new handlers require EBP-48 literally in both images. Default EBP-10 is
  unchanged. Five synthetic positives/45 rejections and the retained181-byte
  default-slot Replay contract pass; no EH-runtime or generated-code credit.
- Eight new source mappings cover2019 target bytes. Existing origin rows and the
  reviewed denominator are unchanged; exact codegen does not settle SDK ancestry.

Latest [primary-format proof](evidence/sound-primary-format-20261029/README.md):
SetPrimaryBufferFormat44C7E0..44C8E3 is260/all3 exact in two independent
final-source canonical Sound+ZWave colds and a retained real15-input Main image.
The first cold also passes four old Sound/streaming guards and the affected
setup-thread interface:2509/all70/six owners. This establishes exactness of
existing maintained source; original origin/TU and product/runtime stay separate.

Latest [Sound proof](evidence/sound-queues-20261028/README.md):

| Complete owner | Bytes | Fields | Canonical unit |
| --- | ---: | ---: | --- |
| ProcessQueues `43DDF0` | 1452 | 43 | `sound-process-queues` |
| Streaming Reset `44DAD0` | 213 | 4 | `streaming-sound-reset` |

Two independent Sound+ZWave cold images pass 2192 bytes/all 60 fields, including
two old guards. The real Main 15-input context also passes both plus five old
guards: 2582 bytes/all 82 fields. It recompiled three affected inputs and reused
12 verified objects; this is not full-product or whole-Main exactness. The queue
owns 1419 code + 1 NOP + 32 table bytes, resolving the old adjacent-table
uncertainty. New SDK-family 19/52-byte candidates at `44D3E0`/`44D400` stay
indeterminate, unmapped and nonexact.

## Immediate continuation: complete configuration and actual callers

GameConfigurationView::ResetDefaults418C40 is a complete125-byte candidate with
nine ordinary differences. Constructor127 imposed an unused returned receiver;
void reset matches length, leaving prologue scheduling and EAX/ECX temporaries
for600/flag1. Actual420870 loader allows three values at+1B, so retain a byte
field. Free-function and bool controls are rejected/restored. The existing byte
view's original types stay provisional. All five actual calls fail to establish
a consumed return contract; do not add return0 merely to influence allocation.

Concrete caller/context leads: Main configuration load420870 and Main init421F00
both call418C40; replay-object creation428FEA also constructs this same34-byte
configuration at object+14. Restore actual source/context before more register
or spelling panels. Device slot14 stays a typed offset interface, without claiming
an original SDK/TU declaration. GameManager factory107 and Main launcher118 are
now complete/exact. GameManager initializer1039/1009 and cleanup1120/1105 remain
nonexact. Current header is a target-backed Main view, not full constructor or
storage closure; preserve all known offsets and unrelated D3D evidence.

FrontEnd update42CDF0 remains1076/1128 (1059 code +1 alignment +68 table). Retain
its corrected replay-return, local demo search, cursor clamps and actual VM
interfaces.491C00's FrontEnd mode now uses the shared g_MainModeTransitionCounter
symbol; the complete native control is byte-neutral. This closes that alias
without exact credit. Do not reopen the old inline setter, unsigned mode or
branch-placement controls. Source-visible actual callers are the useful premise.
Supervisor mode4218D0 remains678/666 and registration420470 remains204/204 with
register differences. ReleaseModeOwners4203F0/128 and Main added/draw callbacks
remain unreconstructed. Larger maintained owners receive no partial credit.

Current warm context:
`.analysis/gpt-6.1-sol/frontend-controller-20261029/final-cold-b-context.json`.
It is the28-input Main graph, adding GameManagerLifecycle to the prior27 inputs.
Keep recursive include/profile pins. Diagnose by compiling changed/new inputs;
canonical acceptance compiles all declared inputs. Only select new/affected units.
A native interior address may coincide with an unrelated tiny data anchor:
factory device is Main+8, and default binding loads are474E88+0/4/8/C/10. Bind
semantic bases/addends, not accidental nearest-public labels. No product storage
or runnable-image claim follows from the diagnostic anchor layout.

## Startup residuals



Complete Startup.cpp now contains the real3F0 layout, constructor128, task
registration140, factory69, teardown417 proposal, release91, Draw220 and callback
bodies. Initial constructor135/launcher150 recover128/140 with scalar id storage
and a retained worker-subobject reference. No forced registers or padding.

Teardown first376/417 omitted EH; explicit /EHs retains the observed two-state
frame but gives434, then433 after ASCII capture before clearing the startup
global. /EHsc on Startup and separately on the VM implementation are neutral and
restored. Current declaration uses /EHs only for Startup; prior profiles remain
unchanged. Target and native unwind observations agree on saved thisEBP+4,
worker+10 and VM+30, with states[-1,0]; this is diagnostic metadata evidence,
not an accepted handler binding or parent match. Normal-path state updates,
zero-register lifetime and argument scheduling still differ. Park flag controls.

Draw is218/220 in the current28-input context (210/220 in the prior22-input
context), with different hidden-return/frame reuse. Named result temporaries
produce222 and are restored. OnUpdate is now63/63 exact through the state-field
repair above. Raw EDX counterpart41FD90 and registered ECX copy41FEB0 have the
same transitions, but their original
member/adapter/LTCG relationship stays unknown. An ordinary member Update plus
an inlined fastcall adapter produced67/63 for both before the state-field repair
and is restored.
Readiness bitfield remains67; volatile readiness also remains67 and breaks the
exact constructor (132/128), so both are restored. Explicit throw() on the VM
destructor leaves teardown433 and the extra normal-path state write; restored.
Park these controls pending new evidence. No authored-byte or partial
exact credit follows from their game-specific logic or legacy source presence.
The factory's actual caller4218D0 now has complete666-byte source and corrected
table ownership, as above. The unresolved MainCreateCallbackOwner seam is still
not identified with this startup factory.

StartupThread stays292/299 after real callback escape and launcher definitions.
Its seven missing ECX homing/allocation/pop bytes and error-store scheduling are
still unexplained. Do not force an unused argument or callback ABI. The actual
_beginthreadex callback type and the observed plain RET stay separate.

Active recipes/listings/logs: `.analysis/gpt-6.1-sol/frontend-controller-20261029/`.
Its `final-cold-b-context.json` records the current28-input warm graph. The
private producer compares recursive include and profile pins, rebuilding only
affected/new inputs. Cold acceptance reuses no objects. Superseded
private images are removed after retaining source snapshots, receipts/listings;
only current canonical images and the independent first witness are retained.

Sound Initialize755/786 and LoadBgm304/298 remain parked. The prior nested-success
Initialize control763 was neutral and restored. Disk caller249/241 remains
nonexact despite its exact702-byte callee. Source/byte/origin/storage/runtime
claims remain independent.

## Other open work

| Owner | Current native/target extent | State |
| --- | --- | --- |
| Sound PreloadBgm / LoadBgm | 369/369; 304/298 | Nonexact; parked scratch-register controls |
| Sound sample loader | 684/684 | Nonexact; size equality grants nothing |
| WinMain / Render | 1533/1389; 419/425 | Nonexact, real queue call now connected |
| Ending Run | 1564/1568 | Nonexact; outer destructor362 is now exact |
| ANM manager constructor | 2305/2315 | Nonexact; counter/operator-view and neutral loop controls rejected |
| Large ANM/ECL interpreters | See exact-first method and evidence | Parked residuals; require a new premise |

See [durable knowledge](KNOWLEDGE_BASE.md) and
[exact-first method](TH10_EXACT_FIRST_METHOD_20261024.md) for established facts.
Do not turn old hypotheses into facts or replay parked spelling controls without
new target/callee/capture/context evidence. Selecting another substantial owner
is allowed when it has a concrete uncertainty and a complete comparison path.

## Artifacts and checkpoint

Current active artifacts and the verified historical archive are indexed in
`.analysis/README.md`; cleanup audit is `.analysis/handoff-20261028/`.
Newest logs/receipts are `.analysis/gpt-6.1-sol/frontend-controller-20261029/`;
the independent first witness is
`build/gpt-6.1-sol/frontend-controller-20261029/cold-a/` and the second is canonical
`build/match-linked/GameManagerLifecycle/`. Prior FrontEnd and Supervisor
witnesses remain retained. Source/include hashes agree.
Earlier SoundDiskStartup and primary-format witnesses remain retained separately.
Canonical `build/match-linked`, `build/match` and `build/match-runtime` caches,
current Sound witnesses and warm Main inputs are retained. Obsolete host/probe
builds are removed. Old analysis is hash-verified before compression/removal;
extract only a needed historical subtree. Old scratch paths are not guaranteed
live. The private target, toolchain links, Ghidra database and unrelated D3D
adapter evidence are untouched.

Cleanup reduced `.analysis/` from 738 MiB to about 128 MiB and `build/` from
478 MiB to about 81 MiB after lightweight CI regenerated its Python cache.
The historical archive retains 26,144 leaves in about
117 MiB; every archived file was hash-verified before its working copy was
removed. The audit records all 198 removed build roots and protected-file checks.
The cleanup checkpoint passed all seven lightweight CI stages; no native build
or new exact comparison was run for this documentation/artifact batch.

For ordinary checkpoints run the affected native Oracle and lightweight
`scripts/repo-python scripts/ci.py`, then `git diff --check`. Regenerate progress
only when its ledger inputs change. Product, dual-runtime semantic and port
validation belong to later phases. Do not revive retired test requirements.
