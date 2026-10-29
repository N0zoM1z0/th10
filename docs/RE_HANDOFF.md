# TH10 agent handoff — 2026-10-29

## Start here

Current checkpoint adds complete disk702 and memory651 streaming creators,
PreparePlayback295, resource preload82, Open173 and constructors118/109: **2130
new exact bytes / seven owners /60 fields**. The startup chain is now maintained
in Startup.cpp/Sound.cpp/ZWave.cpp. Outer startup and disk caller stay nonexact.

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
**82,718/275,467 reviewed authored owned bytes exact, 446 exact authored owners**;
655 authored source mappings. Total: 1807 candidates, 1157 mappings, 1306 exact.
The denominator remains provisional; product build/runtime closure is still open.

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

## Immediate continuation: outer startup and disk caller

Current maintained source has eight new complete mappings. Startup41F990/299
is292 bytes; OpenDiskBgm43D690/241 is249. Both remain nonexact. The parent's
actual callee graph is now present and exact; no guessed private ABI is needed.
The target startup's seven missing bytes are ECX stack homing/allocation/pops;
its error-state store also schedules differently. An ordinary-member startup
control is byte-neutral and restored. Do not force an unused argument, calling
convention or stack padding. The actual launcher41FAC0/140 stores the callback
and passes the worker to _beginthreadex; its complete source/escape context is
not yet recovered and is a concrete next investigation.

Disk caller's manager-null block is early in the target, with two short checks;
the candidate merges it into a cold tail and has different return scheduling.
A separate manager gate/positive device block is neutral and restored. Parent
and caller are independent claims: exact702 does not make caller241 exact.

Active recipes/listings/logs:
`.analysis/gpt-6.1-sol/sound-disk-startup-20261029/`. The private producer derives
changed inputs from recursive include pins: current diagnostics recompile Main,
Sound, ZWave and Startup, reusing twelve unchanged objects. Cold acceptance reuses
none. Final-source cold contexts are retained with receipts; old15-input warm
Sound caches predate the Sound.hpp changes and cannot reuse Main unchecked.

Sound Initialize43CD30 remains755/786. The old pending cbSize producer and eight
prior controls are complete; this continuation's nested-success control is763,
byte-neutral against the previous success-first shape, and restored. Preserve
explicit cbSize zero and park CFG/order controls without new evidence. LoadBgm
is304/298 and data-loader thread112/107 in the new graph, both nonexact. The two
new exact creators do not grant partial credit to any caller.

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
New cold logs/receipts are `.analysis/gpt-6.1-sol/sound-disk-startup-20261029/`;
the first independent native witness is
`build/gpt-6.1-sol/sound-disk-startup-20261029/cold-a/` and the second is the
canonical `build/match-linked/SoundDiskStartup/`. Source/include hashes agree.
The earlier primary-format witnesses remain retained separately.
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
