# TH10 exact-reconstruction handoff

## Campaign state

The exact-reconstruction campaign remains active and incomplete. This browser
conversation reviewed one hard `PbgFile` / pack-file owner packet centered on
`0x00435300-0x0043599D`. It recovered the complete target `CPbgFile` vtable and
0x0C object layout, challenged the denominator with eleven Ghidra-missed target
entries, established maintained `CPbgFile` source, and reviewed the connected
archive-aware file helpers that feed the `ResFile` owner seam. It did **not**
establish any canonical exact function, original translation-unit ownership,
whole Windows i386 product closure, runtime validation, semantic reconstruction,
or portability claim.

Repository selection was exactly `th10`, analysis provider exactly
`th10-ghidra`, and target exactly `target:th10-main`. The private operator target
remained at `resources/th10.exe`; it was verified in place and was never
modified, replaced, relocated, staged, or committed. No `/mnt` search and no
`TH10_TARGET_PATH` override were used.

Session starting HEAD was
`084a1ee3f5741f7325cad933534b14190ec0d7d0` on `main`. At entry it was clean:
zero staged, zero unstaged, zero untracked, zero conflicts. Its upstream was
`origin/main` at `2f4a3350fc61132e7eb5e340f657527965ae5500`, so local state was
ahead 1 / behind 0 before this packet. This session never pushed.

The intended local checkpoint subject is:

`gpt-web: reconstruct TH10 PbgFile backend seam`

The live Git HEAD after closeout is authoritative. No push is performed by this
session.

## Recovery gate and preserved state

The mandatory Factory authority, repository-list, and live TH10 status calls
were performed before editing. Recovery inspection covered branch, HEAD,
upstream relation, recent commits, porcelain-v2 status, complete staged and
unstaged diffs, all untracked paths, relevant ignored state, `.analysis/`,
`build/`, and the prior handoff.

No recoverable interrupted tracked/untracked work existed. The following state
was classified and preserved:

- `resources/th10.exe`: operator-supplied ignored private target; preserve and
  exclude from Git.
- `.tools/`: shared toolchain selectors and game-bound Wine state; preserve.
- `ghidra-project/`: target-bound provider state; preserve.
- `.analysis/bootstrap/` and earlier `.analysis/gpt-web/...`: pre-existing or
  checkpointed diagnostic state; preserve without granting authority.
- `build/`: reproducible generated/open build state; no recovery edit required.

Entry `.analysis` size was **80,734 bytes**. No old campaign, target, provider
database, Wine prefix, toolchain, or unknown/shared artifact was deleted.

All prompt-named repository and Factory guidance was actually read before
editing: `AGENTS.md`, `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`,
`docs/ORACLES.md`, `docs/TOOLS.md`, both named Factory contracts,
`ontology.md`, `verification-planes.md`, `agent-autonomy.md`,
`new-game-bootstrap.md`, and `worktree-recovery-and-analysis-artifacts.md`.
`AGENTS.md` also required `semantic-reconstruction.md`, which was read. The
repository-local `.agents/skills/th10-reconstruction/SKILL.md` body was read
explicitly; no skill injection was assumed. No required path was missing.

One initial read-only packet/preflight shell exited before any preflight because
its Python inspection attempted to access a nonexistent `origin` field in
`config/functions.csv`. Live repository status was re-read and remained clean
before the corrected command ran. One initial multi-address Ghidra request and
one adjacent committed-source search failed at the Factory network layer; each
was followed by live recovery/status inspection before retry. A Truth Kernel
snapshot request later returned `another factory operation owns <operator-path>`;
that attempt was treated as unavailable and no inference was made from it. A
final post-checkpoint refresh later succeeded and is recorded below.

## Identity and mandatory preflights

The private target passed `python3 scripts/verify-target.py` with:

- SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, four sections
- image base `0x00400000`
- entry point `0x004537DC`
- Rich checksum `0x3152A748`
- dominant build 6030

`python3 scripts/verify-toolchain.py --execute` passed the pinned VC7.1 SP1
normal-COFF, C++ `/GL`/LTCG, resource, and PE32 i386 execution smoke. Target
Rich records still establish 131 normal C, 15 normal C++, and 52 LTCG C++
inputs at build 6030. Per-function profile, physical object owner, original TU
partition, production libraries/resources, and link order remain unknown.

Mandatory repository preflights passed before target-dependent work:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Native Ghidra operation schemas were rediscovered. The mandatory `check {}`
passed with `attestation.status=passed`, exact target `target:th10-main`, and
`attestation.provider_transport=factory-native-command`. Every useful Ghidra
result used the same target/native transport and carried
`exactness_credit=none`.

## Packet selection and balance

This packet followed the previous handoff and deliberately attacked a central
owner/vtable/data-boundary cohort rather than a small-leaf count strategy:

- Ghidra-missed constructor around `0x00435300`;
- `0x00435320` deleting destructor;
- `0x00435340` destructor;
- `0x00435370` `Open`;
- `0x00435460` `Close`;
- the raw vtable methods between `0x00435490-0x00435574`;
- `0x00435580` whole-file reader;
- `0x00435610` path helper;
- base-interface lifecycle/compiler bodies through `0x004356E3`;
- central archive lookup `0x004356F0`;
- Ghidra-missed archive-load helper `0x004357D0`;
- archive-or-disk read/size helpers `0x00435800` and `0x004358E0`.

The observable outcome sought was to separate the physical `CPbgFile` owner
from higher pack/file archive helpers, close the complete vtable/layout, expose
hidden denominator entries, and determine whether natural maintained source can
compile under both target-observed compiler artifact families without guessing
per-function ownership.

The packet is a hard-frontier attempt because it includes a 239-byte Win32 open
method, a 137-byte virtual whole-file path, 214/190-byte archive/disk fallback
helpers, two polymorphic vtables, compiler-generated lifecycle helpers, and
cross-owner `ResFile` callers. It was not selected for easy exact wins.

## Complete `CPbgFile` storage and vtables

TH10 target construction sites establish a `CPbgFile` object of exactly 0x0C
bytes:

- `+0x00`: vptr
- `+0x04`: Win32 `HANDLE`
- `+0x08`: desired access

The derived target vtable at `0x0046F230` contains **nine** slots:

1. `+0x00` -> `0x00435370` `Open`
2. `+0x04` -> `0x00435460` `Close`
3. `+0x08` -> `0x00435490` `Read`
4. `+0x0C` -> `0x004354D0` `Write`
5. `+0x10` -> `0x00435510` `Tell`
6. `+0x14` -> `0x00435530` `GetSize`
7. `+0x18` -> `0x00435550` `Seek`
8. `+0x1C` -> `0x00435320` compiler deleting destructor
9. `+0x20` -> `0x00435580` `ReadWholeFile`

The ninth slot corrected the prior declaration-only `src/PbgFile.hpp`, which
had stopped after the inherited virtual destructor. The base vtable at
`0x0046F210` contains seven pure-virtual slots followed by the base deleting
destructor at `+0x1C`.

All vtable slot statements are direct TH10 rdata/xref facts. TH08/TH095 source
was consulted only after the TH10 slot order was recovered and corroborates the
source-level names/order; it does not transfer target ownership or exactness.

## Reviewed authored `CPbgFile` bodies

### `0x00435300-0x00435314` — proposed `CPbgFile::CPbgFile`

Ghidra missed this 21-byte CC-delimited body. It consumes the object through
live-in EAX, writes vptr `0x0046F230`, writes `INVALID_HANDLE_VALUE` at `+4`,
and clears `+8`. Maintained natural source uses an ordinary constructor; fixed
normal COFF adds an ECX-to-EAX entry move, while the target consumes live-in EAX.
This is source-shape/private-ABI evidence, not an exactness or LTCG-owner claim.

### `0x00435320-0x0043533D` — compiler deleting destructor

The derived vtable points here at `+0x1C`. The body calls the reviewed
non-deleting destructor, tests deleting flags, conditionally invokes operator
delete, and returns with `ret 4`. It is tracked as
`compiler_generated / exclude`, not authored code and not source-present.

### `0x00435340-0x0043536D` — proposed `CPbgFile::~CPbgFile`

The body installs the derived vptr, closes a live handle, resets handle/access,
then installs the base vptr. Natural maintained source is a destructor that
calls `Close()`. A fixed normal-COFF target-bound diagnostic is
`structural-exact` across **34/34 comparable non-relocation bytes**, but physical
normal-COFF versus LTCG ownership remains unresolved, so no canonical exact row
was added.

### `0x00435370-0x0043545E` — proposed `CPbgFile::Open`

The derived vtable points here at slot 0. The target:

- closes prior state;
- scans mode bytes for `r`, `w`, or `a`;
- uses `GENERIC_READ`/`GENERIC_WRITE` and the observed Win32 creation modes;
- deletes the old path for `w`;
- resolves a full path through `0x00435610`;
- calls `CreateFileA` with `FILE_SHARE_READ` and
  `FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN`;
- seeks to `FILE_END` for append mode.

Its machine ABI is ECX `this` plus two stack args and `ret 8`. The target has a
security-cookie frame and calls `GetFullFilePath` through a private
EAX-filename/ECX-buffer boundary. The fixed standalone normal-COFF source probe
therefore mismatches, while the same natural source also compiles successfully
as `/GL`. Profile/owner/exactness remain unknown.

### `0x00435460-0x00435481` — proposed `CPbgFile::Close`

Slot `+0x04`. Invalid handle is a no-op; otherwise `CloseHandle` runs and the
handle/access fields are reset. Normal-COFF diagnostic: **30/30 comparable
bytes**, `structural-exact`, authority none.

### Ghidra-missed virtual methods

Raw target/vtable review added five denominator entries that Ghidra had not
modeled as functions:

- `0x00435490-0x004354C8`, 57 bytes — proposed `CPbgFile::Read`; slot `+0x08`;
  requires `GENERIC_READ`, calls `ReadFile`, returns bytes read. Normal-COFF
  diagnostic: **53/53 comparable bytes**, structural-exact.
- `0x004354D0-0x0043550E`, 63 bytes — proposed `CPbgFile::Write`; slot `+0x0C`;
  requires `GENERIC_WRITE`, calls `WriteFile`, returns byte-count equality.
  Fixed normal COFF has a small post-call code-shape mismatch.
- `0x00435510-0x00435528`, 25 bytes — proposed `CPbgFile::Tell`; slot `+0x10`;
  invalid handle -> 0, otherwise `SetFilePointer(..., FILE_CURRENT)`.
  Diagnostic: **21/21 comparable bytes**, structural-exact.
- `0x00435530-0x00435544`, 21 bytes — proposed `CPbgFile::GetSize`; slot `+0x14`;
  invalid handle -> 0, otherwise `GetFileSize`. Diagnostic: **17/17 comparable
  bytes**, structural-exact.
- `0x00435550-0x00435574`, 37 bytes — proposed `CPbgFile::Seek`; slot `+0x18`;
  invalid handle -> false, otherwise `SetFilePointer` and true. Diagnostic:
  **33/33 comparable bytes**, structural-exact.

Boundary review explicitly includes callee-pop immediate operands: `Read` ends
at `0x004354C8`, `Write` at `0x0043550E`, and `Seek` at `0x00435574`; following
bytes are CC padding.

### `0x00435580-0x00435608` — proposed `CPbgFile::ReadWholeFile`

This is the ninth derived virtual slot at `+0x20`, after the inherited deleting
destructor. It requires `GENERIC_READ`, obtains size, enforces the caller max,
allocates with `malloc`, saves/restores position, and reads through virtual
methods. On a seek failure after allocation the retail target returns null
without freeing that allocation; maintained source preserves this observed
behavior instead of silently fixing it. Normal COFF differs in register
allocation and seek-origin/global handling; `/GL` compiles successfully. No
exactness credit.

### `0x00435610-0x00435688` — proposed `CPbgFile::GetFullFilePath`

The reviewed `Open` caller supplies filename in live-in EAX and output buffer in
ECX. A drive-qualified path is copied directly; otherwise the module directory
is fetched and the filename appended. Natural maintained static source matches
the target semantics, including the observed path-end control flow, but the
standalone normal-COFF static-argument ABI does not match the private target
boundary. No source calling-convention or LTCG ownership claim is made.

## Base-interface denominator challenge

Three CC-delimited Ghidra-missed raw bodies were added to the candidate ledger
without forcing origin:

- `0x00435690-0x00435696`: destructor-shaped base-vptr store through ECX. An
  empty virtual `IPbgFile` destructor in fixed normal COFF has the same 3/3
  comparable bytes, but original explicit/defaulted source origin and physical
  ownership remain unknown.
- `0x004356C0-0x004356C6`: constructor-shaped base-vptr store through live-in
  EAX. Standalone normal COFF needs an ECX-to-EAX move. Origin remains unknown.
- `0x004356D0-0x004356E3`: retained helper returning the byte after the first
  slash, or the original pointer. It has no established direct entry xref or
  original identifier. Origin remains unknown.

`0x004356A0-0x004356BE` is different: base vtable `+0x1C` points directly to it,
and its flag/delete pattern is the MSVC scalar deleting destructor. The pinned
normal compiler emits the same 31-byte helper with 23/23 comparable
non-relocation bytes. It is classified `compiler_generated / exclude`.

Keeping the three raw bodies `unknown/review` is intentional denominator
pressure, not incomplete bookkeeping.

## Connected pack/file archive helpers

These functions are behaviorally connected to the backend but are **not**
assigned to the `CPbgFile` source mapping.

### `0x004356F0-0x004357C3`

Reviewed authored central archive lookup. It derives `<first path component>.dat`
and searches the active array of 0x10-byte `PbgArchive` objects rooted at
`0x004923B0`, using active count `0x00477850`. Both archive-aware read and size
helpers call it.

### `0x004357D0-0x004357F3`

Ghidra-missed 36-byte retained authored body. It selects the next archive object
from `0x004923B0 + count * 0x10`, calls reviewed `PbgArchive::Load` using the
live-in archive filename, increments `0x00477850` on success, and returns a
boolean. No original identifier or source owner is claimed.

### `0x00435800-0x004358D5`

Reviewed authored archive-or-disk read helper. Machine ABI is ECX path plus one
stack output-buffer argument, `ret 4`. A matching archive uses the path suffix
and `PbgArchive::ReadDecompressEntry`; fallback constructs a temporary
`CPbgFile`, opens disk, and calls `ReadWholeFile`.

### `0x004358E0-0x0043599D`

Reviewed authored required sibling size helper. It uses the same archive lookup;
archive hits call reviewed `PbgArchive::GetEntryDecompressedSize`, while disk
fallback opens a temporary `CPbgFile` and calls `GetFileSize`.

Both `0x00435800` and `0x004358E0` are directly consumed by the raw owner seam at
`0x00436400-0x0043642B`, which stores data/size state into the surrounding
ResFile-like object. This makes the higher pack/file helper cohort a central
owner seam rather than unrelated easy leaves.

## Source-path evidence and original TU limits

Attested target string search found:

- `0x0046F254`: `.\src\pack\File.cpp:233 void`
- `0x0046F348`: `.\src\pack\ResFile.cpp:82 BYTE`

These strings prove those source paths participated in the original build.
`xrefs_to` returned no code reference to either string, so they **do not** map
any reviewed target function to an original translation unit. In particular,
maintained `src/PbgFile.cpp` is a reconstruction source location, not a claim
that the reviewed methods were physically owned by original
`src/pack/File.cpp`.

## Maintained source and compiler feedback

Tracked maintained source now includes:

- updated `src/PbgFile.hpp` with the target-verified ninth virtual
  `ReadWholeFile` and static `GetFullFilePath` declaration;
- new `src/PbgFile.cpp` with natural maintainable implementations of constructor,
  destructor, `Open`, `Close`, `Read`, `Write`, `Tell`, `GetSize`, `Seek`,
  `ReadWholeFile`, and `GetFullFilePath`.

Exactly eleven reviewed authored `CPbgFile` target bodies are source-present and
mapped through `config/reccmp-functions.csv` / `config/implemented.csv`. The
compiler-generated deleting destructor and unresolved `IPbgFile` raw bodies are
not source mappings.

The same tracked source compiled successfully through the pinned compiler as:

```text
/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc
/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL
```

The normal object was compared with `scripts/compare-coff-function.py` in probe
mode. Relocation-masked structural-exact results were obtained for the
non-deleting destructor, `Close`, `Read`, `Tell`, `GetSize`, and `Seek`, while
other bodies retained real shape/context differences. The compact report is:

`.analysis/gpt-web/20260912-pbgfile-backend/compiler-shape-report.txt`

All probe results have `acceptance_authority=none`. No `match-units.toml` row,
`matches.csv` row, or canonical exact claim was added. `/GL` was not stripped to
fit the standalone normal-COFF Oracle.

## Adjacent-game hypothesis discipline

Only committed adjacent source was used, and only after TH10 target evidence
established the backend behavior/vtable.

Observed adjacent states while consulted:

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch
  `port/portable-64bit`, clean in the observed status. Committed
  `src/pbg/PbgFile.hpp/.cpp` corroborated names and natural source shape.
- TH09: HEAD `caa38688bcc88d5a711dee96cfd712d2261c4b99`, branch `main`, with
  unrelated dirty tracked paths in `config/function-origins.csv`,
  `config/functions.csv`, `docs/KNOWLEDGE_BASE.md`, and `src/ScoreData.cpp`.
  Only committed HEAD search results were used; no uncommitted content was
  consulted.
- TH095: HEAD `2c1acc0402ddd793f77dd405718202bbe38e2477`, branch `main`, with
  unrelated dirty `src/FrontEndController.cpp`, `src/Supervisor.hpp`, plus
  unrelated untracked analysis/runtime files. Only committed HEAD
  `src/pbg/PbgFile.hpp/.cpp` was used.

TH10 differs materially where observed: for example, its `ReadWholeFile` target
uses `malloc/free`; TH08 committed source uses `GlobalAlloc/GlobalFree`, while
TH095 committed source corroborates the later `malloc/free` shape. TH10 target
facts always win.

## Ledger and verification-plane state

After this packet and generated progress refresh:

- tracked candidates: **1,219**
- origin/boundary pending: **1,163**
- confirmed authored: **52 functions / 10,162 bytes**
- classified exclusions: **4**
- source-present authored mappings: **25**
- canonical exact functions: **0**
- canonical exact authored bytes: **0**
- normal-COFF canonical match units: **0**

This packet added eleven target candidates to the denominator. Eight of those
were reviewed immediately (six authored `CPbgFile`/archive bodies and two
compiler helpers), while three deliberately remain `unknown/review`. Across
existing plus new candidates, the packet added **15 reviewed authored functions
and 1,453 authored bytes**, **2 compiler exclusions**, and **11 source-present
mappings**.

Verification planes remain separate:

- **source presence:** 25 reviewed authored mappings total; eleven new
  `CPbgFile` mappings this packet.
- **exactness:** 0 canonical exact functions/bytes. Structural probe equality is
  diagnostic only.
- **whole Windows i386 build:** open. `config/build.toml` remains unchanged and
  honest.
- **runtime validation:** not started.
- **semantic/port stages:** not started.
- **Truth Kernel acceptance:** an earlier current-session request was unavailable due
  shared `<operator-path>` ownership, but the final post-checkpoint refresh succeeded.
  The accepted snapshot contains exactly one accepted fact, `target_attested`, from
  source commit `9f7886f72adcd1880c05e34c62408cf9b1dad966`. No PbgFile, archive, source-present,
  ownership, or exactness fact from this packet is accepted.

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` have no
tracked diff in this packet.

## Validation actually run

The following checks were run successfully after source/ledger work:

```text
scripts/compile-probe.sh src/PbgFile.cpp <normal.obj> /TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc
scripts/compile-probe.sh src/PbgFile.cpp <ltcg.obj> /TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --check
python3 scripts/validate-tracking.py --require-target
python3 scripts/progress.py --check
python3 scripts/build-match-unit.py --check
python3 scripts/build.py --check
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
git diff --check
```

The pinned toolchain had already passed `python3 scripts/verify-toolchain.py
--execute` at session preflight.

The honest whole-build command was also exercised:

```text
python3 scripts/build.py
```

It returned expected **RC=2** with the open-state message that compile flags, TU
partition, libraries, resources, and link order remain unknown. This is not
whole-product closure and is not reported as a successful reconstructed build.

## Analysis artifacts

Entry `.analysis` size: **80,734 bytes**.

Current campaign directory:

`.analysis/gpt-web/20260912-pbgfile-backend/`

Peak observed `.analysis` size during normal/LTCG objects plus detailed JSON was
**138,115 bytes**, far below the 256 MiB review threshold. No file exceeded
64 MiB.

After validation, only the compact compiler report and manifest are retained;
current pre-checkpoint `.analysis` size after the results manifest update is **87,069 bytes**. The current-session
normal object, `/GL` object, and detailed comparator JSON were deleted only
after their results were reviewed and summarized. They are reproducible from
tracked source and the pinned toolchain. No old scratch tree, target, provider
state, toolchain, or unknown/shared artifact was removed.

The manifest must be rebound to the final checkpoint hash during closeout; its
final byte size and `.analysis` total must be re-read afterward.

## Remaining unknowns and blockers

- Per-function normal-COFF versus LTCG physical ownership remains unknown,
  including for the six small methods that are structurally identical in the
  fixed normal-COFF diagnostic.
- Original ownership of the observed `src/pack/File.cpp` path is not tied to any
  function because its target string has no code xref.
- `0x00435690`, `0x004356C0`, and `0x004356D0` remain explicit
  `unknown/review` denominator entries.
- Original identifiers for `0x004356F0`, `0x004357D0`, `0x00435800`, and
  `0x004358E0` remain unknown.
- The active archive-count/array initialization owner beyond the reviewed
  helper remains only partially bounded.
- Production TU partition, exact compile profiles, libraries, resources, and
  link order remain open.
- Whole-product build and runtime closure remain open.
- The final Truth Kernel snapshot is available but contains only the older
  `target_attested` fact; no current PbgFile packet claim is accepted.

## Next evidence-connected hard packet

Prefer the **ResFile owner seam** around `0x004362B0-0x00436767`, centered on the
Ghidra-missed authored-looking body at `0x00436400-0x0043642B` that directly
calls reviewed `0x00435800` and `0x004358E0`. Existing nearby candidates include:

- `0x004362B0-0x00436324` — 117 bytes
- `0x00436330-0x00436360` — 49 bytes
- `0x004363A0-0x004363BD` — 30 bytes
- `0x004363C0-0x004363F0` — 49 bytes
- raw `0x00436400-0x0043642B` — missing from the current denominator
- `0x00436430-0x00436454` — 37 bytes
- `0x00436570-0x00436638` — 201 bytes
- `0x00436660-0x0043667D` — 30 bytes
- `0x00436680-0x004366B0` — 49 bytes
- `0x004366C0-0x00436767` — 168 bytes

The target string `.\src\pack\ResFile.cpp:82 BYTE` and rdata/vtable context near
`0x0046F308` make this a direct owner continuation, but the string has no xref
and is not a function mapping. The next packet should first close its vtable,
raw gaps, ctor/dtor/compiler helpers, object layout, and machine ABIs before
consulting adjacent source or assigning names/source mappings.

This next packet is intentionally not the easiest remaining function: it is the
consumer/owner of the newly reviewed archive-or-disk helpers, includes a
Ghidra-missed central body and multiple 100-200-byte methods, and can challenge
both denominator and data ownership. Do not detour back to small structural
COFF wins merely because they are easy to count.

## Local checkpoint closeout

The first local checkpoint for this packet was
`944eb6955935b7e597626f1d85531e329a3afe90`, subject
`gpt-web: reconstruct TH10 PbgFile backend seam`. Immediately after that commit
the tracked/untracked worktree was clean with zero staged, unstaged, untracked,
or conflicted paths. `main` was ahead 2 / behind 0 relative to `origin/main`.
No push was performed.

The ignored campaign manifest was then marked `checkpointed`, bound to that
checkpoint with `pushed=false`, and `.analysis` measured **87,227 bytes** with
only the compact 2,609-byte compiler-shape report and the campaign manifest
retained in the current campaign directory. The handoff-only closeout update is
amended into the same logical checkpoint. The final live Git HEAD after that
amend is authoritative and the ignored manifest is rebound to it; final status,
Ghidra attestation, Truth Kernel availability, and `.analysis` size are re-read
without changing tracked reconstruction content.

### Final Truth Kernel refresh

After the checkpoint and final Ghidra attestation,
`factory_get_accepted_snapshot(th10)` succeeded. The accepted snapshot
`source_commit` remained `9f7886f72adcd1880c05e34c62408cf9b1dad966`. The snapshot contained exactly
one accepted fact and exactly one accepted claim, `target_attested`, for
`target:th10-main`. Therefore the earlier operator-path lock was transient, but
no PbgFile boundary, source-presence, ownership, compile, or exactness claim from
this packet has Truth Kernel acceptance.
