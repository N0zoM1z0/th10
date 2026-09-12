# TH10 exact-reconstruction handoff

## Campaign state

The exact-reconstruction campaign remains active and incomplete. This session
reviewed a hard file-loader/archive/sound-owner packet centered on
`0x0044B360-0x0044B4C7`, reviewed the immediately adjacent file-existence helper,
and expanded the authored denominator with three target code bodies that Ghidra
did not model as functions. The packet also used one fixed pinned-VC7.1
normal-COFF versus `/GL`+`/LTCG` diagnostic to test the loader's private machine
ABI without tuning flags for a desired answer.

Repository selection was exactly `th10`, analysis provider exactly
`th10-ghidra`, and target exactly `target:th10-main`. The private operator target
remained at `resources/th10.exe`; it was verified in place and was never
modified, replaced, relocated, staged, or committed. No `/mnt` search or
`TH10_TARGET_PATH` override was used.

Session starting HEAD was
`c6978d346ac7cb28a9dc9391a117419dee126431` on `main`. The tracked/untracked tree
was clean at entry: zero staged, unstaged, untracked, or conflicted paths. The
checkpoint containing this handoff should use subject
`gpt-web: review TH10 file loader seam`; the live Git HEAD containing this file
is the authoritative ending checkpoint. No push was requested or executed by
this session.

At entry the local branch was one commit ahead of `origin/main`. During
read-only packet work, `origin/main` moved externally to the same starting HEAD.
Work was paused and recovery was repeated: HEAD, complete staged/unstaged diffs,
untracked paths, and `.analysis` were unchanged. The remote-tracking reflog
records `2026-09-12T19:27:59+08:00 update by push`. This session did not perform
that push and does not infer who did it.

## Recovery gate and preserved state

The mandatory `factory_describe`, repository-list, and live TH10 status calls
were completed before editing. Recovery inspection covered branch, HEAD,
upstream relation, recent commits, porcelain-v2 status, complete staged and
unstaged diffs, all untracked paths, relevant ignored analysis/tool/build/target
/provider state, and the previous handoff.

All repository instructions and Factory guidance named by the session prompt
were mounted and read before editing, including `AGENTS.md`, `docs/RE_HANDOFF.md`,
`docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, `docs/TOOLS.md`, both named Factory
contracts, `ontology.md`, `verification-planes.md`, `agent-autonomy.md`,
`new-game-bootstrap.md`, and `worktree-recovery-and-analysis-artifacts.md`.
`AGENTS.md` additionally led to `semantic-reconstruction.md`, which was read.
The repository-local `.agents/skills/th10-reconstruction/SKILL.md` body was also
located and read explicitly; no skill injection was assumed. No required
guidance path was missing.

No tracked or untracked interrupted work required recovery. Ignored/private
state was classified and preserved:

- `resources/th10.exe`: operator-supplied private target; preserve/exclude.
- `.tools/`: immutable-tool selectors plus game-bound Wine state;
  preserve/exclude.
- `ghidra-project/`: target-bound provider state; preserve/exclude.
- `.analysis/bootstrap/`: pre-existing/legacy analysis; preserve without
  granting evidence authority.
- earlier `.analysis/gpt-web/...` roots: prior checkpointed diagnostics;
  preserve.
- `.analysis/gpt-web/20260912-file-loader-owner/`: this session's bounded
  compiler-diagnostic campaign; see the artifact section.
- `build/`: generated/open build state; no recovery edit was required.

No reset, stash, target edit, bulk deletion, or cleanup of unknown/shared state
was used to manufacture a clean tree.

Several read-only Factory repository/provider calls encountered transport-layer
connection failures. After every such failure, live repository status was
re-read before retry; where relevant, staged/unstaged state, untracked paths, and
analysis inventory were re-inspected. No unexpected filesystem side effect was
found. These failures did not change the target or tracked work.

## Identity and mandatory preflights

The private target passed `python3 scripts/verify-target.py` with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 image base `0x00400000`,
entry `0x004537DC`, four sections, Rich checksum `0x3152A748`, and dominant
build 6030.

`python3 scripts/verify-toolchain.py --execute` passed the pinned VC7.1 SP1
normal-C, normal-C++, C++ LTCG, resource, and PE32 i386 execution smoke tests.
The target Rich facts remain 131 normal C, 15 normal C++, and 52 LTCG C++ inputs
at build 6030. Per-function normal-COFF versus LTCG ownership, exact production
flags, TU partition, libraries, resources, and link order remain unknown.

The following mandatory repository preflights passed before edits:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

Native Ghidra operations were rediscovered before use. The mandatory
`factory_analysis_call(th10-ghidra, check, {})` passed with
`attestation.status=passed`, target `target:th10-main`, and
`attestation.provider_transport=factory-native-command`. Observed SHA-256, MD5,
size, image base, image size, entry point, and distributed mapped-byte samples
matched the private target. Every useful Ghidra result used in this packet also
carried the same passed target/native-transport attestation and
`exactness_credit=none`.

## Packet selection and balance

The packet was selected directly from the prior handoff rather than by smallest
extent: the central file loader `0x0044B360-0x0044B4C7` has calls from many
subsystems, is used by the previously reviewed executable-checksum path, owns a
mixed register/stack machine ABI, and crosses the archive-versus-real-file data
boundary. The observable goal was to close its physical extent, machine ABI,
archive storage dependencies, side effects, and error paths and to decide
whether source/TU/profile evidence was strong enough for production source.

The packet grew only along evidence-connected boundaries. `xrefs_to` found 17
actual calls to the loader while Ghidra assigned only 15 containing callers; the
two extra calls exposed real Ghidra-missed sound/startup bodies. A third
Ghidra-missed thread launcher was then found through direct target thread-start
linkage and an inlined duplicate. These bodies were reviewed because they
challenge the denominator and explain loader ownership; unrelated small
FileSystem leaves were intentionally left pending.

The previous checkpoint was already a hard Main/startup/private-ABI packet. This
session remained on a hard owner/data-boundary frontier rather than accumulating
easy function-count wins.

## Reviewed `0x0044B360-0x0044B4C7` — proposed `FileSystem::OpenFile`

The reviewed target extent is exactly 360 bytes. The final target return is
`ret 8` through `0x0044B4C7`; `0x0044B4C8-0x0044B4CF` is eight bytes of `CC`
padding before independent `0x0044B4D0`.

The target machine boundary is not the provisional Ghidra source signature:

- path/filename is supplied through live-in EAX and copied into ESI;
- the first stack argument is the optional size-output pointer;
- the second stack argument is the mode/external-resource flag;
- both exits use `ret 8` and therefore consume the two stack slots.

Source-level calling convention and physical normal-COFF/LTCG owner remain
unknown. The ledger records the machine boundary rather than inventing a source
`__stdcall`/fastcall declaration.

The body enters the file-system critical section at `0x004922A4` and increments
the active-count byte at `0x0049231E`. In mode zero it strips `\\` and `/` path
components, searches the archive entry array rooted at `0x00497990` using
case-insensitive filenames and 0x10-byte records, reads decompressed size from
entry `+0x08`, optionally writes that size, allocates output, and calls
`0x00434DD0` to read/decode the selected archive entry. In nonzero mode it calls
`CreateFileA` with read-only/share-read/open-existing/sequential-normal flags,
gets the size, allocates, reads, optionally writes actual bytes read, closes the
handle, and returns the allocation. All success/failure exits release the same
critical section and decrement the active count.

`FileSystem::OpenFile` is retained only as a proposed semantic name. Committed
TH08, TH09, and TH095 source contains a strongly corresponding source shape, but
TH10 target behavior and layout were established first. No TH10 string/xref was
found for `FileSystem`, `global.cpp`, or `src/core` in this packet, so no TU or
source-file mapping is justified.

## TH10 archive-owner context rooted at `0x00497990`

Target-local evidence now establishes a coherent archive object-like used prefix
through `+0x0F`:

- `+0x00`: entry-array pointer;
- `+0x04`: entry count;
- `+0x08`: heap-owned archive filename;
- `+0x0C`: polymorphic file backend.

`0x00434D10` frees/zeros the filename, destroys/frees the 0x10-byte entry array,
releases the backend, and clears the count. `0x00434F30` searches the `+0/+4`
entry range case-insensitively. `0x00434DD0` uses the backend to open/seek/read a
selected record, computes compressed size from adjacent entry data offsets,
reads decompressed size from `+0x08`, applies the target data transform, and
conditionally reaches the decompression helper.

For each 0x10-byte entry, TH10 itself establishes filename at `+0x00`, data
offset at `+0x04`, and decompressed size at `+0x08`. TH08/TH09 committed
`PbgArchiveEntry` layouts and archive source corroborate the same shape only
after these TH10 facts were established. The complete TH10 object extent, field
`+0x0C` semantics for each entry, original type identifier, physical TU, and
normal-COFF/LTCG owner remain unknown. `PbgArchive` is therefore descriptive
adjacent-supported terminology, not an original-symbol claim.

The archive helpers were not promoted from pending in this session; they form
the preferred next hard packet.

## Reviewed `0x0044B4D0-0x0044B53E` — proposed file-existence helper

This 111-byte authored body uses the same file-system critical section and active
count as the loader, attempts a read-only `CreateFileA`, closes a valid handle,
and returns 1 for existence or 0 for failure. The machine boundary consumes one
stack argument and both exits use `ret 4`. `0x0044B53F` is one byte of `CC`
padding before `0x0044B540`.

Direct target xrefs include reviewed `GameWindow::Present`, reviewed startup
`GameWindow::CheckForRunningGameInstance`, and file/sound startup work.
`FileSystem::CheckIfFileAlreadyExists` is an adjacent-supported proposed name
only. The callee-pop target shape is not promoted to a source-level `__stdcall`
claim because the physical compiler/LTCG context remains unresolved.

## Denominator expansion — startup and sound worker bodies missed by Ghidra

Three complete CC-delimited authored bodies were added to the candidate
denominator.

### `0x0041F990-0x0041FABA` — proposed `Supervisor::StartupThread`

This 299-byte body begins after `0x0041F98B-0x0041F98F` CC padding and ends at
`0x0041FABA`; `0x0041FABB-0x0041FABF` is CC padding before the existing
`0x0041FAC0` candidate. Ghidra does not model it as a function.

`0x0041FAC0` stores the address in an object field and passes it to
`_beginthreadex` with the same object as the thread argument. Its unique caller,
`0x0041FD00`, first allocates 0x3F0 bytes, invokes `0x0041F850`, and then calls the thread launcher; that size is
not used to infer an original type because adjacent Supervisor layouts vary
substantially by game. The worker loads `sig.anm` and `text.anm`, reports a
text-initialization failure, then loads `../../bgm/thbgm.fmt`, checks `thbgm.dat`,
and reports BGM initialization failure. It also enters the reviewed file-loader
and file-existence paths. The target body returns with plain `ret`; because it is
used as a runtime thread start address, the source callback convention remains
unresolved rather than inferred from the API typedef.

The earlier working hypothesis `SoundPlayerWorkerThread` was rejected during
diff review because the target owns broad text/animation plus BGM startup work.
Committed TH08, TH09, and TH095 instead place the corresponding text/BGM sequence
inside `Supervisor::StartupThread`. That recurring name is retained only as an
adjacent-supported proposed name; TH10 original identifier, object type, TU,
physical normal-COFF/LTCG owner, and source mapping remain unknown.

### `0x0043D080-0x0043D0EA` — proposed `SoundDataLoaderThread`

This 107-byte body is passed as the thread start address by target
`0x00420100`. It loops exactly 0x25 times over the target filename-pointer table
at `0x00474B40`; direct target decoding shows all 37 names are `se_*.wav` sound
effects. Each path is passed through reviewed `0x0044B360`, and the resulting
buffer is stored in the array at `0x004977C0`. The body exits early when the
sound stop state is 2; after all loads it repeatedly calls `Sleep(1)` while the
state remains zero. On a failed load it reports the target Sound-file error with
the failed name.

The target KERNEL32 IAT maps `0x004660AC` to `Sleep` and `0x00466118` to
`CreateThread`. The thread body returns with plain `ret` and does not implement a
standard callee-pop thread callback boundary; no source-level API callback ABI is
claimed. Committed TH095 `SoundDataLoaderThread` source corroborates the role and
name only.

### `0x0043D0F0-0x0043D111` — proposed `StartSoundLoadThread`

This 34-byte CC-delimited body allocates a local thread-id slot, calls
`CreateThread(NULL, 0, 0x0043D080, 0x00492590, 0, &threadId)`, stores the returned
handle at `0x004977AC`, and returns it. `0x0043D112-0x0043D11F` is CC padding
before the existing `0x0043D120` candidate.

Ghidra has no modeled direct entry xref to the retained body, but
`0x00420100` contains the same launcher sequence inline. This retained-body plus
inline-copy pattern is analogous to previously reviewed Main retained bodies and
supports authored origin without claiming original source/TU ownership.
`StartSoundLoadThread` is an adjacent-supported proposed name only.

A nearby raw body at `0x0043D050-0x0043D074` looks like a deletion/destructor
wrapper and was deliberately not promoted: compiler-generated versus authored
ownership has not been reconciled. Accuracy takes precedence over denominator
size.

## Fixed VC7.1 loader-ABI diagnostic

A single natural three-argument diagnostic was kept fixed rather than tuned for
the target. The source is
`.analysis/gpt-web/20260912-file-loader-owner/open-file-callconv-probe.cpp` and
uses all three inputs of `OpenFileProbe(path, sizeOut, mode)` plus a non-inlined
path-value helper.

Under `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG`, normal VC7.1 COFF passes all three
arguments on the stack; the caller cleans 12 bytes. The same exact source with
one added `/GL` compile and one pinned linker `/LTCG` pass promotes the three
surviving inputs to EDX/EDI/ESI and the callee returns with plain `ret`.
Therefore this fixed diagnostic does **not** reproduce TH10's live-in-EAX plus
two-stack-argument `ret 8` boundary.

This is a useful negative source-shape/profile result, not a disproof of LTCG in
a different whole-program context. No alternate flags or source mutations were
tried to force the desired register allocation. The diagnostic has
`exactness_credit=none` and creates no source mapping or canonical match unit.

## Adjacent-game hypothesis record

Adjacent repositories were consulted only after TH10 target evidence suggested
the FileSystem/archive/sound seam. Only committed `HEAD:` content was used.
Observed states when materially consulted were:

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch
  `port/portable-64bit`, clean.
- TH09: HEAD `6f15efd70d7e155b6b23ad0cddbb2ff049f183c3`, branch `main`, with unrelated untracked `src/ScoreData.cpp`. No uncommitted TH09 content was used.
- TH095: HEAD `f282620ec1a22be1c4257181ed672dfcd2b43a7e`, branch `main`, dirty with
  unrelated `src/ScreenEffect.cpp`, `src/ScreenEffect.hpp`, and unrelated
  untracked runtime/reconstruction files. No uncommitted TH095 content was used.

Committed TH08/TH09/TH095 `FileSystem::OpenFile` shapes corroborate basename
stripping, archive lookup/read-decompress behavior, and the external
CreateFile/GetFileSize/ReadFile path. Committed TH08/TH09 archive layouts
corroborate the TH10-local 0x10-byte entry shape and four-dword archive state
prefix. Committed TH08/TH09/TH095 startup code corroborates the proposed
`Supervisor::StartupThread` role for `0x0041F990`; committed TH095 sound code
corroborates `SoundDataLoaderThread` and `StartSoundLoadThread`. These observations do not
transfer addresses, extents, physical ownership, ABI, TU, or exactness into
TH10.

## Ledger and verification-plane state

After the packet and generated progress refresh:

| Plane | State |
| --- | --- |
| Tracked candidates | 1,203 |
| Origin/boundary pending | 1,179 |
| Reviewed authored | 23 functions / 7,282 bytes |
| Reviewed exclusions | 1 |
| Source-present authored mappings | 0 |
| Canonical exact functions / bytes | 0 / 0 |
| Normal-COFF canonical match units | 0 |
| Whole Windows i386 build | open |
| Runtime validation | not started |
| Semantic/port stages | not started |
| Ghidra authority | provisional, `exactness_credit=none` |
| Truth Kernel accepted snapshot | unavailable on two refresh attempts: the first was blocked by the shared operator-path lock and the second failed at the Factory transport layer; no new acceptance claimed |

The packet reviewed two pre-existing candidates and added three new denominator
candidates, for 911 newly reviewed authored bytes. It did not add production
source, source mappings, signatures, canonical match units, or exact rows.

Tracked files changed by the packet before the handoff update are limited to:

- `config/function-origins.csv`
- `config/functions.csv`
- `docs/KNOWLEDGE_BASE.md`
- `docs/PROGRESS.md`
- `resources/progress.svg`

This handoff itself adds `docs/RE_HANDOFF.md` to the checkpoint. No `src/` file,
`config/implemented.csv`, `config/reccmp-functions.csv`, `config/matches.csv`,
`config/match-units.toml`, or `config/build.toml` is changed.

## Commands and validation actually run

Recovery and guidance work included Factory repository/status/description calls,
complete Git diff/status/untracked inspection, ignored-state inventory, all
prompt-named guidance reads, the additional semantic-reconstruction guidance,
and explicit repo-local skill-body reading.

Target/provider analysis used discovered native Ghidra `check`, `function`,
`decompile`, `callers`, `callees`, `disassemble`, `xrefs_to`, and
`search_strings` operations. Useful results were accepted only with passed
`target:th10-main` and `factory-native-command` attestation. Direct target
inspection used bounded `objdump` and small read-only Python PE/VA parsers. The
optional Python `pefile` module was unavailable; a minimal read-only PE import
parser was used instead and directly mapped the `Sleep`/`CreateThread` IAT
slots.

The pinned compiler diagnostic actually ran:

- `scripts/compile-probe.sh` once for normal COFF with
  `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG`;
- the same command once with one additional `/GL`;
- the hash-pinned VC7.1 linker once with `/LTCG /nodefaultlib` and a diagnostic
  entry point;
- bounded `objdump`/map extraction into the retained compiler-shape report.

No second profile or flag search was used after the LTCG result failed to
reproduce the target ABI.

Post-edit focused validation passed:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --check`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/progress.py --check`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `git diff --check`

An honest `python3 scripts/build.py` milestone returned expected RC=2 with
`whole build unavailable: compile flags, TU partition, libraries, resources, and link order remain unknown`.
This remains an explicit open whole-product state, not a failed exact-function
Oracle and not a runtime result.

Several read-only Factory calls failed at the transport layer during the packet.
Each failure was followed by live-status recovery before retry. Two Truth Kernel
accepted-snapshot refreshes were unavailable: the first because another operation
owned the shared operator path, and the second because the Factory transport
connection failed. Live repository status was re-read after the second failure.
Neither unavailable read is converted into acceptance, rejection, or a target fact.

## Analysis artifacts

`.analysis/` measured **68,494 bytes** at session entry. This session owns only
`.analysis/gpt-web/20260912-file-loader-owner/`.

After compiler feedback and reference review, the retained current-session
artifacts are:

- `open-file-callconv-probe.cpp`: compact fixed natural source reproducer;
- `compiler-shape-report.txt`: normal-COFF and linked-LTCG disassembly/map
  summary;
- `manifest.json`: target binding, producer/disposition metadata, and result
  summary.

The following current-session intermediates were deleted only after the retained
report was confirmed to contain the needed result and no durable file depended
on the binary artifact:

- `open-file-normal.obj`
- `open-file-ltcg.obj`
- `open-file-ltcg.exe`
- `open-file-ltcg.map`

No current campaign artifact exceeds 64 MiB. The post-cleanup pre-checkpoint
`.analysis` size was **76,299 bytes**. After the first local checkpoint, the
ignored manifest was marked `checkpointed`, bound to that commit with
`pushed=false`, and `.analysis` measured **76,462 bytes**. This handoff is then
amended into the same logical checkpoint and the ignored manifest is rebound to
the amended live HEAD; the authoritative final size is re-read after that rebind
and must agree with the closeout report.

No old campaign, bootstrap analysis, Ghidra project, target, Wine prefix,
toolchain, or unknown/shared artifact was deleted.

## Remaining unknowns and next evidence-connected packet

The principal unresolved facts after this packet are:

- physical normal-COFF versus LTCG ownership of `0x0044B360`, its callers, and
  the archive helpers;
- original source-level ABI of the loader and sound thread bodies;
- TH10 source/TU ownership for FileSystem, archive, and Sound code;
- complete `0x00497990` archive-object extent and the original names/types;
- the purpose/ownership of archive-entry `+0x0C`;
- the full transform/decompression ownership reached by `0x00434DD0`;
- the raw `0x0043D050-0x0043D074` deletion/destructor-like body, whose authored
  versus compiler-generated origin remains unknown;
- exact production flags, TU partition, libraries, resources, and link order;
- all source-present and canonical exact ledgers, which remain empty.

The preferred next hard packet remains directly connected to the reviewed loader
rather than selecting an easier FileSystem leaf:

1. `0x00434C30-0x00434C9B` — 108 bytes, archive load/lifecycle entry;
2. `0x00434D10-0x00434D65` — 86 bytes, archive release/state cleanup;
3. `0x00434DD0-0x00434EE8` — 278 bytes, archive read/transform/decompression
   path called by `FileSystem::OpenFile`;
4. `0x00434F30-0x00434F64` — 53 bytes, archive entry lookup;
5. `0x00435800-0x004358D5` — 214 bytes only as needed, because it is the other
   direct caller of `0x00434DD0` and may be required to resolve owner/ABI.

Observable outcome: close the archive object's real owner and used/complete
layout, recover the helper machine ABIs and decompression/data-transform
boundary, and determine whether the target finally provides enough physical
owner/profile evidence for maintainable production `PbgArchive`/FileSystem
source. If it does not, keep source presence at zero and checkpoint the reviewed
boundary/origin/data facts without forcing a mapping.

This next packet is not chosen for size: it contains the central 278-byte archive
read/decompression routine and its lifecycle/data-owner dependencies, and is the
hardest directly connected owner question exposed by the current loader packet.

Immediately after the first local checkpoint, before this handoff-only closeout
amendment, the tracked/untracked worktree was clean with zero staged, unstaged,
untracked, or conflicted paths; `main` was one commit ahead of the externally
updated `origin/main` and zero behind. The final amended checkpoint is re-audited
for the same clean state. No push is performed.
