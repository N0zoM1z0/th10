# TH10 exact-reconstruction handoff

## Campaign state

The exact-reconstruction campaign remains active and incomplete. This conversation
reviewed one hard archive-owner packet centered on the target archive object at
`0x00497990` and the source bodies around `0x00434C10-0x004352C8`. It closed the
physical archive-object extent, recovered the PBG entry layout and THA1 parser,
challenged the function denominator with four Ghidra-missed authored source
bodies plus one compiler-generated deletion dispatcher, and established the
first maintained TH10 reconstruction source mappings. It did not establish any
canonical exact function, per-function normal-COFF/LTCG object ownership, whole
Windows i386 product closure, runtime validation, semantic reconstruction, or
portability claim.

Repository selection was exactly `th10`, analysis provider exactly
`th10-ghidra`, and target exactly `target:th10-main`. The private operator target
remained at `resources/th10.exe`; it was verified in place and was never
modified, replaced, relocated, staged, or committed. No `/mnt` search and no
`TH10_TARGET_PATH` override were used.

Session starting HEAD was
`2f4a3350fc61132e7eb5e340f657527965ae5500` on `main`, with zero staged,
unstaged, untracked, or conflicted paths. At session entry `origin/main` already
matched that HEAD: the remote-tracking reflog records an external
`2026-09-12T19:52:24+08:00 update by push`. This session did not perform that
push and does not infer who did.

The intended local checkpoint subject for this packet is
`gpt-web: reconstruct TH10 archive owner seam`. The live Git commit containing
this handoff is authoritative after closeout. No push is performed by this
session.

## Recovery gate and preserved state

The mandatory Factory authority, repository-list, and live TH10 status calls
were completed before editing. Recovery inspection covered branch, HEAD,
upstream relation, recent commits, porcelain-v2 status, complete staged and
unstaged diffs, all untracked paths, ignored analysis/tool/build/target/provider
state, the previous handoff, and the remote-tracking reflog.

No tracked or untracked interrupted work required recovery. Ignored/private
state was classified and preserved:

- `resources/th10.exe`: operator-supplied private target; preserve/exclude.
- `.tools/`: shared tool selectors plus game-bound Wine state; preserve/exclude.
- `ghidra-project/`: target-bound provider state; preserve/exclude.
- `.analysis/bootstrap/`: pre-existing/legacy analysis; preserve without
  granting evidence authority.
- earlier `.analysis/gpt-web/...` roots: previous bounded/checkpointed diagnostic
  campaigns; preserve.
- `build/`: generated/open build state; no recovery edit was required.
- `.analysis/gpt-web/20260912-archive-owner/`: this conversation's owned bounded
  compiler/source-shape diagnostic campaign.

No reset, stash, target edit, bulk deletion, or cleanup of unknown/shared state
was used to manufacture a clean tree.

All prompt-named repository and Factory guidance was mounted and read before
editing: `AGENTS.md`, `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`,
`docs/ORACLES.md`, `docs/TOOLS.md`, both named Factory contracts, `ontology.md`,
`verification-planes.md`, `agent-autonomy.md`, `new-game-bootstrap.md`, and
`worktree-recovery-and-analysis-artifacts.md`. `AGENTS.md` additionally led to
`semantic-reconstruction.md`, which was read. The repository-local
`.agents/skills/th10-reconstruction/SKILL.md` body was also located and read
explicitly; no skill injection was assumed. No required guidance path was
missing.

Several repository/provider requests failed at the Factory transport layer.
Every failure was followed by live repository recovery before retry. Notable
cases were the initial combined recovery query, a combined guidance read, the
combined preflight shell, the first standalone toolchain-execution request, and
one adjacent PbgFile source query. None left unexpected tracked/untracked work.
A local ledger-update Python script also failed once because it incorrectly read
`fieldnames` from a file handle instead of the `csv.DictReader`; recovery showed
that it had written no ledger state, after which the corrected script was run.
The first archive-source compile failed because `<stddef.h>` was absent for
`offsetof`; live status was checked and the natural missing include was added
before recompiling. These are engineering/recovery events, not target facts.

## Identity and mandatory preflights

The private target passed `python3 scripts/verify-target.py` with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 image base `0x00400000`,
entry `0x004537DC`, four sections, Rich checksum `0x3152A748`, and dominant
build 6030.

`python3 scripts/verify-toolchain.py --execute` passed the pinned VC7.1 SP1
normal-COFF, C++ `/GL`/LTCG, resource, and PE32 i386 execution smoke. Target
Rich records still establish 131 normal C, 15 normal C++, and 52 LTCG C++
inputs at build 6030. Per-function compiler profile, physical owner, original TU
partition, production libraries/resources, and link order remain unknown.

The mandatory repository preflights passed before target-dependent work:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Native Ghidra operation schemas were rediscovered before use. The mandatory
`check {}` passed with `attestation.status=passed`, exact target
`target:th10-main`, and
`attestation.provider_transport=factory-native-command`. Every useful Ghidra
result in this packet carried the same target/native-transport attestation and
`exactness_credit=none`. A second Ghidra `check {}` after the dirty source/ledger
work also passed with the same private target hash, layout, entry point, and
mapped-byte samples.

## Packet selection and balance

The selected packet followed the previous handoff and was deliberately a hard
owner/data-boundary cohort rather than a small-leaf strategy:

- `0x00434C30-0x00434C9B` archive load;
- `0x00434D10-0x00434D65` archive release;
- `0x00434DD0-0x00434EE8` archive read/transform/decompression path;
- `0x00434F30-0x00434F64` archive entry lookup;
- the directly connected parser/entry lifecycle helpers needed to close the
  physical owner;
- `0x00435800-0x004358D5` only as caller context, not promoted in this packet.

The observable goal was to determine whether the `0x00497990` storage, archive
entry format, backend virtual interface, lifecycle, and private ABIs could be
closed with TH10-local evidence strongly enough to justify maintained candidate
source without pretending to know the original source file or compiler owner.
The result is yes for source presence and no for exactness/object ownership.

This follows two previous central Main/startup/FileSystem hard packets. The
conversation did not switch to the easiest unreviewed functions merely to
improve counts.

## Complete TH10 archive-object storage at `0x00497990`

The global archive object is now physically bounded to exactly `0x10` bytes:

```text
[0x00497990, 0x004979A0)
+0x00  PbgArchiveEntry *entries
+0x04  int entryCount
+0x08  char *archiveFilename
+0x0C  polymorphic file-backend pointer
```

This is stronger than the previous used-prefix claim. Archive lifecycle code
uses exactly those four dwords. A dedicated shutdown wrapper at the end of the
image passes exactly `0x00497990` to the release body, while independent densely
referenced storage begins at `0x004979A0`.

The target also contains an array of twenty separate 0x10-byte archive objects at
`0x004923B0`. Static initialization passes `0x00434C10` and `0x00434C20` as the
constructor/destructor pair to the MSVC vector constructor/destructor machinery
with element size `0x10` and count `0x14`. Thus the 0x10-byte object size is a
TH10-local physical fact, not a transferred adjacent-game layout.

`PbgArchive` is retained as a maintained/proposed C++ type name because the
behavior and adjacent committed source agree. It is not an original-symbol,
original-TU, or object-file ownership claim.

## Reviewed archive lifecycle and query source bodies

### `0x00434C10-0x00434C1F` — proposed `PbgArchive::PbgArchive`

This 16-byte CC-delimited body was missing from the Ghidra function inventory.
The target vector-constructor setup references it directly for the twenty
0x10-byte archive objects. It clears all four object fields. A natural pinned
VC7.1 normal-COFF compile of maintained source is also 16 bytes and fully
byte-identical to this target body, but the per-function normal-COFF/LTCG owner
is unknown, so no canonical exact row is created.

### `0x00434C20-0x00434C29` — proposed `PbgArchive::~PbgArchive`

The static vector-destructor setup references this existing Ghidra candidate.
The target receives the object in ECX, moves it to ESI, calls the release body,
and returns. The source-level name is adjacent-supported after the TH10 lifecycle
role was established.

### `0x00434C30-0x00434C9B` — proposed `PbgArchive::Load`

The unique target startup caller supplies `ECX=0x00497990` and
`EAX="th10.dat"`. The callee therefore has a private/interprocedural machine ABI
of live-in ECX archive plus EAX filename, not a source-level fastcall claim. It
releases prior state, allocates a 0x0C-byte CPbgFile-shaped backend, invokes the
header parser, copies the archive filename, reopens the backend, and returns a
boolean result in AL.

### `0x00434D10-0x00434D65` — proposed `PbgArchive::Release`

This existing candidate consumes the archive through live-in ESI. It frees and
clears the archive filename, invokes the MSVC vector destructor for 0x10-byte
entries and frees the array, deletes the polymorphic backend through vtable slot
`+0x1C`, then clears entries/count/backend. The machine ABI is recorded as a
target fact independently from the natural member-function declaration.

### `0x00434DD0-0x00434EE8` — proposed `PbgArchive::ReadDecompressEntry`

The physical extent is 281 bytes. Ghidra reports 278 CFG body addresses because
its decompiler labels the real signed-mod correction at `0x00434E80-0x00434E84`
as unreachable; those instructions are target code, not padding or data.

The machine boundary consumes filename through live-in ECX and two stack
arguments (archive object and output buffer), with `ret 8` on every exit. The
body finds the archive entry, derives compressed size from the next record's data
offset, reads through the backend, computes an 8-bit filename checksum, selects
one of eight 0x0C-byte decrypt profiles, applies the target data transform, and
conditionally invokes the decompression helper when compressed and decompressed
sizes differ.

The maintained source deliberately preserves a target error behavior that may
look unattractive: when seek/read fails, the current non-null compressed buffer
is freed even if it is the caller-provided output buffer. Reconstruction does not
silently "fix" that behavior.

### `0x00434EF0-0x00434F22` — proposed `PbgArchive::GetEntryDecompressedSize`

This 51-byte existing candidate uses live-in EAX archive and EBX filename,
performs the same 0x10-byte case-insensitive entry lookup, and returns entry
`+0x08` or zero.

### `0x00434F30-0x00434F64` — proposed `PbgArchive::FindEntry`

This 53-byte existing candidate walks `entries/count` at object `+0/+4` in
0x10-byte strides and returns the `_stricmp` match. Its target machine inputs are
live-in EAX archive and EBX filename.

## THA1 header parser and entry-table ownership

### `0x00434F70-0x004350CD` — proposed `PbgArchive::ParseHeader`

The target machine boundary is live-in ESI archive plus one stack filename,
returning with `ret 4`. It opens and reads a 16-byte header through the file
backend, decrypts the header with XOR value/increment `0x1B/0x37`, and requires
magic `0x31414854` (`THA1`). It decodes:

- file-table decompressed size by subtracting `123456789`;
- file-table compressed size by subtracting `987654321`;
- entry count by subtracting `135792468`.

It calculates the file-table offset from archive size minus compressed-table
size, seeks to that offset, reads the compressed table, decrypts it with
`0x3E/0x9B` and chunk size `0x80`, decompresses it, and invokes the entry-table
builder. The target does not test the return of the file-table seek; maintained
source preserves that observed behavior.

TH095 committed source later supplied unusually close naming/source-shape
corroboration, including the THA1 format and constants, but the TH10 constants,
control flow, ABI, and layout above were established from TH10 first.

### `0x004350D0-0x00435216` — proposed `PbgArchive::AllocEntries`

The target allocates `count + 1` records of exactly 0x10 bytes and invokes the
entry constructor/destructor through the compiler array-construction machinery.
Each decoded file-table entry copies:

```text
+0x00  filename pointer
+0x04  dataOffset
+0x08  decompressedSize
+0x0C  third metadata dword
```

The extra sentinel record stores the archive data/table boundary at `+0x04` and
zero at `+0x08`. `ReadDecompressEntry` uses
`entry[1].dataOffset - entry.dataOffset` as compressed size. The target copies
entry `+0x0C` but no reviewed retail reader consumes it, so maintained source
uses the neutral name `unconsumedMetadata`; its meaning remains unknown.

The target machine body has three stack arguments and `ret 0x0C`; a source-level
member `this` is unused/elided and is not reverse-invented into the machine ABI.

### `0x00435220-0x0043524E` — proposed `PbgArchive::CopyFileName`

This 47-byte existing candidate receives filename through live-in EDI, allocates
`strlen + 1`, and copies the NUL-terminated filename. Maintained source uses
natural `strcpy`, matching the observed byte-copy behavior.

### `0x00435250-0x00435269` — proposed `PbgArchiveEntry::~PbgArchiveEntry`

The target entry destructor receives ECX `this`, frees the filename when non-null,
clears the field, and returns. A natural pinned normal-COFF compile is also 26
bytes and matches all 22 comparable non-relocation bytes. This remains diagnostic
source-shape evidence with zero exactness authority because physical compiler
ownership is unresolved.

## Ghidra-missed retained archive helpers and compiler exclusion

The packet challenged the candidate denominator rather than trusting the initial
Ghidra inventory.

- `0x00435270-0x0043527B` is a 12-byte CC-delimited authored retained body,
  proposed `PbgArchive::SeekPastInt`. It receives a pointer-to-pointer through
  live-in EAX, advances it four bytes, and returns the dword at the new cursor.
  `AllocEntries` contains the same operation inline. Normal COFF is 16 bytes
  because it first loads the normal stack argument.
- `0x00435280-0x004352B2` is a 51-byte CC-delimited authored retained body,
  proposed `PbgArchive::SeekPastString`. It consumes pointer-to-pointer through
  live-in EDX, advances past the NUL string, rounds to a four-byte boundary, and
  returns the updated pointer. `AllocEntries` contains the same logic inline.
- `0x004352C0-0x004352C8` is a nine-byte CC-delimited authored entry constructor,
  directly referenced by the target vector-constructor iterator. Natural normal
  COFF from maintained source is nine bytes and fully byte-identical.
- `0x00434C10-0x00434C1F`, described above, is the fourth newly added authored
  body missed by Ghidra.
- `0x00434D70-0x00434DCC` is **not** promoted to authored code. It is a 93-byte
  CC-delimited MSVC scalar/vector deleting-destructor dispatcher using deletion
  flags and the 0x10-byte entry destructor. The pinned normal compiler
  automatically emits the same class of deleting-destructor COMDAT from the
  maintained entry source. It is tracked as `compiler_generated / exclude`.

Tiny raw return stubs around `0x004352D0/0x004352E0/0x004352F0` remain outside
this authored promotion because their origin/ownership has not been established.

## File-backend interface evidence

The reviewed archive methods call a polymorphic backend at object `+0x0C`.
TH10 target virtual calls establish the relevant slot roles:

- vtable `+0x00`: Open;
- `+0x08`: Read;
- `+0x14`: GetSize;
- `+0x18`: Seek;
- `+0x1C`: deleting destructor.

Adjacent committed PbgFile declarations corroborate the complete slot sequence as
Open, Close, Read, Write, Tell, GetSize, Seek, destructor. `PbgArchive::Load`
allocates a 0x0C-byte backend and initializes the expected vptr/handle/access-like
storage. Maintained `src/PbgFile.hpp` contains only this minimal target-facing
interface and declarations; the backend implementation remains deliberately
absent and is the next owner packet.

## Maintained source and source-present plane

This is the first TH10 checkpoint to establish maintained source presence. New
tracked files are:

- `src/PbgFile.hpp` — minimal target-verified backend interface/declarations;
- `src/PbgArchive.hpp` — TH10-observed header, entry, and 0x10-byte archive
  layouts plus archive declarations;
- `src/PbgArchive.cpp` — natural archive lifecycle/parser/read/source bodies.

Fourteen reviewed authored archive target functions are mapped in
`config/reccmp-functions.csv` and listed in `config/implemented.csv`:

```text
PbgArchive::PbgArchive
PbgArchive::~PbgArchive
PbgArchive::Load
PbgArchive::Release
PbgArchive::ReadDecompressEntry
PbgArchive::GetEntryDecompressedSize
PbgArchive::FindEntry
PbgArchive::ParseHeader
PbgArchive::AllocEntries
PbgArchive::CopyFileName
PbgArchiveEntry::~PbgArchiveEntry
PbgArchive::SeekPastInt
PbgArchive::SeekPastString
PbgArchiveEntry::PbgArchiveEntry
```

This mapping means maintained reconstruction source exists for those target
functions. It does **not** claim the original TH10 source file/TU, object-file
ownership, normal-COFF/LTCG ownership, build-graph ownership, or exactness.
`config/build.toml` remains explicitly open and receives no source entry from
this packet.

The source contains no target byte arrays, no fake returns, no inert padding, and
no target patch/oracle-specific code.

## Pinned VC7.1 source-shape feedback

Maintained `src/PbgArchive.cpp` was compiled with one fixed normal profile:

```text
/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc
```

and with the same exact source/profile plus `/GL`. Both compilation modes
succeeded under the pinned VC7.1 SP1 build-6030 toolchain.

A compact target-bound diagnostic is retained at
`.analysis/gpt-web/20260912-archive-owner/compiler-shape-report.txt`:

- `PbgArchiveEntry::PbgArchiveEntry` target size 9, normal-COFF size 9, all bytes
  equal;
- `PbgArchive::PbgArchive` target size 16, normal-COFF size 16, all bytes equal;
- `PbgArchiveEntry::~PbgArchiveEntry` target/normal size 26, all 22 comparable
  non-relocation bytes equal;
- `SeekPastInt` target is 12 bytes versus 16 normal-COFF bytes because the target
  consumes the pointer through live-in EAX rather than loading `[esp+4]`;
- `SeekPastString` target is 51 bytes versus 61 normal-COFF bytes, with live-in
  EDX/private boundary differences;
- the normal compiler automatically emits a PbgArchiveEntry vector deleting
  destructor, corroborating the compiler-generated classification of raw
  `0x00434D70`.

These observations do **not** create a canonical normal-COFF match. The target
contains LTCG inputs and the physical owner of these functions is not established.
No `config/matches.csv` or `config/match-units.toml` row is added, and the `/GL`
hypothesis is not stripped merely to fit the standalone COFF Oracle.

## Adjacent-game hypothesis discipline

Adjacent source was consulted only after TH10 target observations established the
archive subsystem. Only committed `HEAD:` content was used for evidence. Material
states were:

- TH08 committed HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch
  `port/portable-64bit`, clean when consulted;
- TH09 committed source was materially consulted at HEAD
  `6f15efd70d7e155b6b23ad0cddbb2ff049f183c3`; unrelated live dirty state was
  excluded. Later in this conversation the live TH09 checkout externally moved
  to `caa38688bcc88d5a711dee96cfd712d2261c4b99`; that later HEAD was observed
  during closeout but was not substituted retroactively for the consulted source;
- TH095 committed HEAD `2c1acc0402ddd793f77dd405718202bbe38e2477` was used for
  archive/PbgFile source-shape corroboration. Its current checkout contains
  unrelated modified/untracked work; only committed HEAD content was used.

Adjacent archive source corroborated names, natural C++ organization, virtual
slot naming, and the THA1/PBG source shape. It did not transfer TH10 addresses,
extents, physical ownership, ABI, exactness, source mappings, or completion.
Where source generations differed, TH10 target behavior controlled the maintained
implementation.

## Ledger and verification-plane state

After this packet and generated progress refresh:

| Plane | State |
| --- | --- |
| Tracked candidates | **1,208** |
| Origin/boundary pending | **1,169** |
| Reviewed authored | **37 functions / 8,709 bytes** |
| Reviewed exclusions | **2** |
| Source-present authored mappings | **14** |
| Canonical exact functions / bytes | **0 / 0** |
| Canonical normal-COFF match units | **0** |
| Whole Windows i386 build | **open** |
| Runtime validation | **not started** |
| Semantic/port stages | **not started** |
| Ghidra authority | provisional, `exactness_credit=none` |

Relative to session entry, this packet reviews ten existing authored archive
candidates, adds four new authored denominator candidates, adds one
compiler-generated exclusion candidate, and adds 1,427 reviewed authored bytes.
It also adds the first fourteen maintained source mappings. Source presence and
exactness remain separate: exactness is still zero.

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` are
unchanged. The whole-build graph therefore remains honestly open rather than
claiming the newly maintained source has a known production TU/profile/link owner.

## Truth Kernel acceptance plane

A current `factory_get_accepted_snapshot(th10)` succeeded during this packet.
The acceptance registry exposed exactly one accepted TH10 fact: the existing
`target_attested` claim for `target:th10-main`, backed by an older accepted target
identity receipt whose source commit is
`9f7886f72adcd1880c05e34c62408cf9b1dad966`.

No current archive boundary, origin, source-present, object-ownership,
compile-succeeded, codegen-exact, or whole-build claim from this conversation is
accepted in the Truth Kernel. This session did not submit a replay. The newly
maintained dirty source and ledgers therefore have **zero new Factory acceptance
credit** even though repository-local validation passes.

## Commands and validation actually run

Recovery/guidance work used the mandatory Factory authority/repository/status
calls plus bounded live Git/ignored-state inspection and all required guidance
reads. Target/provider work used discovered `th10-ghidra` schemas for `check`,
`function`, `decompile`, `callers`, `callees`, `disassemble`, and `xrefs_to`,
plus bounded direct target `objdump`/Python PE and raw-data inspection.

Historical-compiler feedback used `scripts/compile-probe.sh`. A disposable
archive-source probe first failed because `<stddef.h>` was missing for `offsetof`;
the missing standard include was added and the same natural source then compiled
successfully as normal COFF and `/GL`. After source semantics were corrected from
TH10 target evidence, the tracked `src/PbgArchive.cpp` was again compiled
successfully in both modes. No flag sweep or source mutation was used to force a
target register allocation.

Before handoff, the following repository checks passed:

```text
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

`python3 scripts/build-match-unit.py --check` reports zero canonical units. An
honest `python3 scripts/build.py` invocation returned the expected RC=2 with:

```text
whole build unavailable: compile flags, TU partition, libraries, resources, and link order remain unknown
```

This is an open product plane, not an exact-function failure and not runtime
validation.

## Analysis artifacts

`.analysis/` measured **76,462 bytes** at session entry. The current archive
campaign reached **166,193 bytes** before checkpoint cleanup while normal and
`/GL` objects plus source probes were retained for comparison.

After reference review, only current-session reproducible intermediates were
removed:

- `archive-source-probe.cpp`;
- `archive-normal.obj`;
- `archive-ltcg.obj`;
- `tracked-normal.obj`;
- `tracked-ltcg.obj`.

Current retained scratch under
`.analysis/gpt-web/20260912-archive-owner/` is:

- `compiler-shape-report.txt` — compact target/compiler source-shape result;
- `manifest.json` — target/source/toolchain binding and artifact dispositions.

After the retained manifest was updated with the available Truth Kernel snapshot and tracked-source bindings, the pre-checkpoint `.analysis` size was **80,564 bytes**. After the first local checkpoint, the ignored manifest was marked `checkpointed`, bound to that commit with `pushed=false`, and `.analysis` measured **80,734 bytes**. This handoff-only closeout amendment is then folded into the same logical checkpoint; the manifest is rebound to the amended live HEAD and the final size is re-read during cold closeout. No current campaign artifact exceeds 64 MiB, and the campaign is far below the 256 MiB review threshold. No old campaign, bootstrap state, provider database, target, Wine prefix, toolchain, or unknown/shared artifact was deleted.

## Remaining unknowns and next evidence-connected packet

Important unresolved facts are:

1. per-function normal-COFF versus LTCG physical ownership for the archive core;
2. the original TH10 archive/FileSystem source-file and translation-unit
   partition;
3. semantic meaning of `PbgArchiveEntry +0x0C` beyond "copied third metadata
   dword, no reviewed retail consumer";
4. implementation and physical ownership of the CPbgFile backend;
5. owners/constants for the PBG decrypt-profile table, transform routine
   `0x0044B0D0`, and decompression routine `0x00435DC0`;
6. higher-level fallback opener `0x00435800` and its archive/file owner;
7. production compile flags, libraries, resources, source graph, and link order;
8. all canonical exact claims, which remain zero.

The preferred next hard packet is the directly connected **PbgFile backend and
archive/disk fallback seam**, not an easier unrelated leaf:

- raw `0x00435300` — constructor-shaped 0x0C backend initializer, not currently a
  Ghidra function;
- `0x00435320-0x0043533D` — deleting destructor;
- `0x00435340-0x0043536D` — destructor;
- `0x00435370-0x0043545E` — backend Open;
- `0x00435580-0x00435608` — backend read/allocation helper;
- `0x004356F0-0x004357C3` — archive/path lookup helper;
- `0x00435800-0x004358D5` — archive-versus-real-file fallback opener as needed
  for owner/ABI closure.

The observable goal is to replace the declaration-only `src/PbgFile.hpp` boundary
with target-backed maintained backend source, reconcile its vtable/lifetime and
private ABIs, determine which helper owns disk/archive path resolution, and test
whether that source can be connected to the archive code without pretending to
know the original production TU/profile. The first validation step in the next
conversation remains the mandatory recovery/preflight plus a passed
`th10-ghidra check {}` before inspecting these addresses.

This next packet is structurally central because it closes the unresolved
polymorphic backend on which every newly mapped archive method depends. It is not
selected because of function size or likely exactness.

The campaign remains active and incomplete. Fourteen source-present mappings,
several strong normal-COFF source-shape matches, a future high reviewed ratio, or
even a later 99.5% moving objective do not authorize GPT-web to declare the exact
phase complete.

## Local checkpoint closeout

The first local checkpoint for this packet was
`d2bc9a53e81bea419098888dbea07254394e14a2`, subject
`gpt-web: reconstruct TH10 archive owner seam`. Immediately after that commit the
tracked/untracked worktree was clean with zero staged, unstaged, untracked, or
conflicted paths; `main` was one commit ahead of `origin/main` and zero behind.
No push was performed.

The closeout-only handoff update is amended into that same logical checkpoint,
so the final live Git commit containing this file is the authoritative ending
HEAD. After amendment, the ignored campaign manifest is rebound to that final
HEAD and the same clean/ahead relation is re-audited.
