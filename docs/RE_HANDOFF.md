# TH10 agent handoff — 2026-10-29

## Start here

Current checkpoint reconstructs the startup object's real worker/VM members,
construction, task registration, factory and cleanup. Five complete owners add
435 exact bytes/all26 fields: **300 authored** plus135 origin-indeterminate.
Destructor433/417, Draw218/220 and thread292/299 remain nonexact.

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
**83,018/275,467 reviewed authored owned bytes exact, 449 exact authored owners**;
660 authored source mappings. Total: 1812 candidates, 1166 mappings, 1311 exact.
The denominator remains provisional; product build/runtime closure is still open.

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

## Immediate continuation: startup teardown and callback representation

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

Draw218/220 has different hidden-return/frame reuse. Named result temporaries
produce222 and are restored. OnUpdate67/63 is nonexact; raw EDX counterpart41FD90
and ECX registered copy41FEB0 have the same transitions, but their original
member/adapter/LTCG relationship stays unknown. An ordinary member Update plus
an inlined fastcall adapter is an untested source model supported by these paired
bodies; it must be compared without a forced receiver ABI. No authored-byte or partial
exact credit follows from their game-specific logic or legacy source presence.
The factory's actual caller is4218D0, independently queried. Do not identify the
unresolved MainCreateCallbackOwner seam with this factory without new evidence.

StartupThread stays292/299 after real callback escape and launcher definitions.
Its seven missing ECX homing/allocation/pop bytes and error-store scheduling are
still unexplained. Do not force an unused argument or callback ABI. The actual
_beginthreadex callback type and the observed plain RET stay separate.

Active recipes/listings/logs: `.analysis/gpt-6.1-sol/startup-launcher-20261029/`.
`final-cold-b-context.json` records the current nineteen-input warm graph. The
private producer compares recursive include and profile pins, rebuilding only
Startup and new/changed supports. Cold acceptance reuses no objects. Superseded
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
Newest logs/receipts are `.analysis/gpt-6.1-sol/startup-launcher-20261029/`;
the independent first witness is
`build/gpt-6.1-sol/startup-launcher-20261029/cold-a/` and the second is canonical
`build/match-linked/StartupWorkerLifecycle/`. Source/include hashes agree.
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
