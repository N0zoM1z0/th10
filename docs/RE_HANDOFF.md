# TH10 agent handoff — 2026-10-29

## Start here

Current checkpoint restores the complete Supervisor mode call graph and adds
399 exact bytes:363 authored plus36 indeterminate. The existing23-byte stage
helper moves to typed source without extra credit. Actual mode owner4218D0 is
666 bytes (old ledger omitted94 table bytes); its complete candidate678/666 and
registration204/204 remain nonexact. No partial owner credit is granted.

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
**83,381/275,561 reviewed authored owned bytes exact, 452 exact authored owners**;
665 authored source mappings. Total: 1815 candidates, 1175 mappings, 1319 exact.
The denominator remains provisional; product build/runtime closure is still open.

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

## Immediate continuation: Supervisor mode and actual lifecycle callees

The complete666-byte mode source remains678, with one common FrontEndCreate call
but different block/epilogue placement, branch distances and stage-store scheduling.
Initial686 becomes678 through the outer change guard; grouped/fallthrough/common
cleanup controls694 are restored. Generic normal-unlock spelling is byte-neutral
and restored. Park these block controls. Registration420470 is actually global/
no-argument; correcting the old pointer seam gives204/204, but EBX/EBP allocations
still differ. Direct-global versus retained pointer spelling is neutral; park it.

The next useful premise is actual lifecycle callee source, not register/flag
sweeps. FrontEndCreate42CD50/96 is still opaque. Fresh raw constructor42C920/204
writes a vtable at+0, constructs cursor/timer state and worker+5AB0, then clears
5ACC and sets flag+4 bit2/global47784C. The current FrontEndController view ends
at5AB0; do not append a worker or introduce virtual layout without reconciling
all observed offsets and the491-byte cleanup42CB60. Constructor/raw/decompile
leads are retained in the active campaign. GameManagerCreate4180E0/107 is also
opaque; its logical stack mode/RET4 and owner mode+5C are independently observed.
ReleaseModeOwners4203F0/128 and added/draw callbacks remain declarations only.

The source-backed actual callback41FF80 and stage method421B70 now reproduce
complete bytes/fields. This does not prove the mode callee's bytes, original
class/TU or physical ownership of all legacy global aliases. ScoreState fields
selectedStage+3C/nextStage+40 preserve the existing5C layout. GameManager's
existing60-byte view is shared through GameManager.hpp; its old adapters pass.

Current warm context: `.analysis/gpt-6.1-sol/supervisor-modes-20261029/final-cold-b-context.json`.
It is the22-input Main graph with actual Ending/GameManager/SupervisorModes
supports. Preserve all pinned per-source profiles and recursive include hashes.
Only rebuild changed/new inputs during diagnosis; cold acceptance reuses none.

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

Draw is210/220 in the new22-input context (218/220 in the retained19-input
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

Active recipes/listings/logs: `.analysis/gpt-6.1-sol/supervisor-modes-20261029/`.
Its `final-cold-b-context.json` records the current22-input warm graph. The
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
| Ending Run / outer destructor | 1564/1568; 362/362 | Nonexact; parked register/return-buffer panels |
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
Newest logs/receipts are `.analysis/gpt-6.1-sol/supervisor-modes-20261029/`;
the independent first witness is
`build/gpt-6.1-sol/supervisor-modes-20261029/cold-a/` and the second is canonical
`build/match-linked/SupervisorModes/`. Source/include hashes agree.
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
