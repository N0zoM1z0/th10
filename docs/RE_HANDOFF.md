# TH10 reconstruction handoff

Status: **active-incomplete exact reconstruction campaign**. This conversation
reviewed the retained resource-backed `PbgArchive` load seam at
`0x00434CA0-0x00434D03`, corrected the maintained archive backend type to the
observed polymorphic interface, added natural maintained source, and gathered
compiler feedback without promoting exactness. It did **not** establish any
canonical exact function, original identifier/TU ownership, whole Windows i386
product closure, runtime validation, semantic reconstruction, or portability
claim.

## Recovery baseline

Selection was exactly repository `th10`, provider `th10-ghidra`, target
`target:th10-main`. The private operator target remained
`resources/th10.exe`; it was verified in place and was never modified, replaced,
relocated, staged, or committed. No `/mnt` search and no `TH10_TARGET_PATH`
override were used.

Session starting HEAD was
`9cb6cf4a902e2869a28f52b9ae7300669b6be047` on `main`, with upstream
`origin/main` at `2f4a3350fc61132e7eb5e340f657527965ae5500`: ahead 3 / behind 0.
Entry worktree was clean: zero staged, zero unstaged, zero untracked, zero
conflicts. Entry `.analysis/` size was **90,490 bytes** and `build/` was about
5.0 MiB of preserved generated/open state.

The mandatory recovery inspection covered recent log, branch/HEAD/upstream,
porcelain-v2, complete staged/unstaged diffs, all untracked paths, relevant
ignored state, `.analysis/`, `build/`, and this handoff. No recoverable
interrupted tracked or untracked work existed. Preserved state classification:

- `resources/th10.exe`: operator-supplied ignored private target; preserve and
  exclude from Git.
- `.tools/`: shared immutable tool selectors plus game-bound Wine state;
  preserve.
- `ghidra-project/`: target-bound provider state; preserve.
- earlier `.analysis/` campaigns/bootstrap: checkpointed, reproducible, shared,
  or legacy evidence; preserve unless current-session ownership is explicit.
- `build/`: reproducible generated/open state; no recovery edit required.

No unknown/unrelated dirty path was reset, overwritten, deleted, or staged.

Before editing, all prompt-named repository/Factory guidance was actually read:
`AGENTS.md`, `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`,
`docs/TOOLS.md`, both required Factory contracts, `ontology.md`,
`verification-planes.md`, `agent-autonomy.md`, `new-game-bootstrap.md`, and
`worktree-recovery-and-analysis-artifacts.md`. `AGENTS.md` additionally required
`semantic-reconstruction.md`, which was read. The repository-local
`.agents/skills/th10-reconstruction/SKILL.md` was explicitly read; no skill
injection was assumed. No required guidance path was missing.

## Mandatory identity/preflight

Before target-dependent work these repository checks passed:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Target remained original Japanese TH10 v1.00a:

- size 487,936 bytes;
- SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`;
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`;
- PE32 i386, four sections, image base `0x00400000`, entry `0x004537DC`;
- Rich checksum `0x3152A748`, dominant build 6030.

The pinned VC7.1 SP1 execution smoke passed normal C/C++ COFF, C++ LTCG,
resource compilation, and PE32 linking under headless Wine. This still proves
only tool availability/artifact families. Per-function profile/object owner,
original TU partition, production libraries/resources, and link order remain
unknown.

Native Ghidra schemas were rediscovered via
`factory_list_analysis_operations(th10-ghidra)`. Mandatory `check {}` passed
with `attestation.status=passed`, exact `target:th10-main`, and
`attestation.provider_transport=factory-native-command`. Every useful Ghidra
result in this packet carried the same passed target/native attestation and
`exactness_credit=none`.

Entry ledger state was:

- candidates: **1,227**;
- origin/boundary pending: **1,159**;
- reviewed authored: **62 functions / 10,711 bytes**;
- exclusions: **6**;
- source-present mappings: **35**;
- canonical exact: **0 functions / 0 bytes**;
- canonical normal-COFF match units: **0**.

## Hard packet selection

The selected packet was the prior handoff's central ownership continuation:
`0x00434CA0-0x00434D03`, initially a 100-byte Ghidra candidate in
`unknown/review`. It was chosen because it directly constructs the newly
reviewed resource-backed `IPbgFile` implementation and shares archive parser,
filename, cleanup, and storage ownership with reviewed `PbgArchive::Load`, not
because it is an easy exact candidate.

Observable goals were to close:

- physical function extent and adjacent padding;
- callers/references and retained-code status;
- machine ABI/resource-name flow;
- backend allocation/type ownership;
- success/failure cleanup behavior;
- relation to reviewed `PbgArchive::Load`, `ParseHeader`, `CopyFileName`, and
  `Release`;
- natural maintained source and fixed compiler feedback without inventing
  original TU/profile/exactness.

## `0x00434CA0-0x00434D03` result

The target establishes a complete **100-byte** CC-delimited body:

- ordinary reviewed `PbgArchive::Load` ends at `0x00434C9B`;
- `0x00434C9C-0x00434C9F` is four bytes of `CC` padding;
- alternate body starts exactly at `0x00434CA0` and ends at the `RET` at
  `0x00434D03`;
- `0x00434D04-0x00434D0F` is `CC` padding before reviewed
  `PbgArchive::Release` at `0x00434D10`.

Machine ABI is observed directly, not inferred from Ghidra's provisional
fastcall signature:

- live-in ECX -> 0x10-byte `PbgArchive` object, saved to ESI;
- live-in EAX -> resource-name pointer, saved to EDI;
- no stack argument is consumed at the entry boundary;
- both exits use plain `RET`.

Target behavior is a source-shaped sibling of ordinary `PbgArchive::Load`:

1. call reviewed `PbgArchive::Release`;
2. allocate exactly 0x10 bytes;
3. zero backend fields `+0x04/+0x08/+0x0C` and install vtable `0x0046F328`;
4. store that polymorphic backend at archive `+0x0C`;
5. call reviewed `PbgArchive::ParseHeader(resourceName)`;
6. on parser success, call reviewed `CopyFileName(resourceName)` and store it at
   archive `+0x08`;
7. return true when the copied name is non-null;
8. otherwise call `Release` and return false.

Unlike ordinary `PbgArchive::Load`, the success path does **not** invoke backend
`Open` a second time after parsing. That is consistent with the reviewed
resource-backed backend: its virtual Open already copies the complete Win32
resource into its owned 0x10-byte memory-file storage, allowing later archive
seeks/reads without reopening.

`factory_analysis_call(callers)` reports zero callers. Attested `xrefs_to`
reports no entry reference, bounded target disassembly finds no direct call or
literal VA reference, and a whole-target raw search finds zero little-endian
`0x00434CA0` VA occurrences. The function is therefore a retained out-of-line
source-shaped body with no observed retail entry reference, not an active call
node in the current image.

Maintained source names it **`PbgArchive::LoadFromResource`**. This name is
explicitly descriptive; no original TH10 C++ identifier or physical original TU
is claimed.

## Resource-directory negative evidence

The target resource directory was inspected directly with `objdump -x`.
The exact retail executable contains only:

- type 3 (`ICON`), resource ID 1;
- type 5 (`DIALOG`), resource ID 203;
- type 14 (`GROUP_ICON`), named `IDI_ICON3`.

There is **no resource type 10 (`RT_RCDATA`)**. The reviewed resource-backed
Open calls `FindResourceA(NULL, name, RT_RCDATA)`, so this exact retail image has
no resource instance that could make the retained archive-resource load path
succeed. Combined with the no-reference audit, no concrete resource name can be
recovered from current product usage. This negative fact does not prove an
original TU, source identifier, or compiler owner.

## Maintained source/type correction

Tracked source changes are intentionally small:

- `src/PbgArchive.hpp`
  - adds descriptive `PbgArchive::LoadFromResource(const char *)`;
  - changes archive `m_FileAbstraction` from `CPbgFile *` to `IPbgFile *`.
- `src/PbgArchive.cpp`
  - includes `ResFile.hpp`;
  - implements natural `LoadFromResource` using `new CWin32ResourcePbgFile`.

The backend pointer type correction is target-driven. Archive `+0x0C` is
observed to own both the ordinary 0x0C `CPbgFile` backend and the 0x10
resource-derived backend, and all consumers use the `IPbgFile` virtual slots.
The pointer-size/layout of the 0x10 archive object is unchanged.

Exactly one existing candidate is newly source-present/authored in this packet:

- `0x00434CA0` -> descriptive `PbgArchive::LoadFromResource`.

No source mapping is added for the compiler/explicit-source-ambiguous derived
lifecycle bodies at `0x00436640` or `0x00436680`.

## Compiler feedback and lifecycle evidence

The modified `src/PbgArchive.cpp` compiled successfully with the pinned VC7.1
SP1 build-6030 candidate under both:

```text
/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc
/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL
```

Normal-COFF target-bound probe results:

- `PbgArchive::LoadFromResource`: object 100 bytes / target 100 bytes,
  **mismatch**, 10/72 comparable bytes.
  - standalone source reads the resource name through the ordinary stack ABI;
  - it calls the synthesized/base memory-file constructor;
  - target receives resource name in EAX and inlines the 0x10-byte construction.
- implicit `CWin32ResourcePbgFile` constructor: object 18 bytes / target raw
  `0x00436640` 18 bytes, **mismatch**, 1/10 comparable bytes.
  - normal source calls the base constructor;
  - target uses private EAX input and directly performs the base-field/vptr
    writes.
- automatically generated `CWin32ResourcePbgFile` deleting destructor: object
  30 bytes / target `0x00436660` 30 bytes, **structural-exact 22/22** comparable
  non-relocation bytes.

These observations are compiler/source-shape diagnostics only. Their
`acceptance_authority` is none, and the same maintained archive source also
compiles under `/GL`. No per-function normal-COFF/LTCG physical ownership is
established. No canonical match unit or exact ledger row was added.

The lifecycle origin boundary remains conservative:

- `0x00436660` stays compiler-generated/excluded, now with stronger pinned
  compiler corroboration;
- `0x00436640` remains `unknown/review`: compiler synthesis is plausible but an
  explicit original source constructor cannot be excluded;
- `0x00436680` remains `unknown/review`: maintained source declares no explicit
  derived destructor and the fixed normal compile emits no separate derived
  non-deleting destructor symbol, but that does not prove the target body's
  source origin.

Compact compiler/target evidence is retained at:

`.analysis/gpt-web/20260912-archive-resource-load/compiler-shape-report.txt`

## Adjacent-game discipline

Adjacent games were consulted only after TH10 established the local resource
backend/archive behavior, and only committed HEAD content was read.

Observed states during consultation:

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch
  `port/portable-64bit`, clean.
- TH09: HEAD `d8938ad72cef5bc7e06fa443703d6749c526257f`, branch `main`, with
  unrelated dirty `config/match-units.toml` and `src/ScoreData.cpp`; no
  uncommitted content was used.
- TH095: HEAD `a1b4bc7174e5fd445b339a8d3bd6147591524ebc`, branch `main`, with
  unrelated untracked runtime/analysis files; no uncommitted content was used.

Committed adjacent PbgArchive source exposes ordinary `Load`, but no matching
resource-load overload or original class identifier was found. TH08/TH095
`PbgMemory.hpp` contains only allocation macros and does not supply the TH10
resource-backend class name. Therefore the maintained name remains descriptive,
not cross-game-transferred.

## Ledger/verification-plane state after this packet

After progress regeneration:

- tracked candidates: **1,227**;
- origin/boundary pending: **1,158**;
- reviewed authored: **63 functions / 10,811 bytes**;
- exclusions: **6**;
- source-present authored mappings: **36**;
- canonical exact: **0 functions / 0 bytes**;
- canonical normal-COFF match units: **0**.

Relative to session entry:

- candidates: +0;
- reviewed authored: +1 function / +100 bytes;
- exclusions: +0;
- source mappings: +1;
- canonical exact: +0.

Verification planes remain independent:

- **source presence:** 36 mapped authored functions; one new mapping here.
- **exactness:** 0 canonical exact functions/bytes. Equal extents and
  structural probe equality are diagnostic only.
- **whole Windows i386 build:** open; `config/build.toml` remains unchanged.
- **runtime validation:** not started.
- **semantic/port stages:** not started.
- **Factory acceptance:** one pre-checkpoint snapshot refresh was unavailable
  because another Factory operation owned the shared `<operator-path>`. No
  current acceptance/rejection is inferred and no packet fact is claimed
  accepted.

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` have no
packet diff.

## Analysis artifacts

Session entry `.analysis/`: **90,490 bytes**.

Current campaign:

`.analysis/gpt-web/20260912-archive-resource-load/`

Peak observed `.analysis/` after normal and `/GL` objects: **132,675 bytes**.
No file exceeded 64 MiB and the campaign was far below 256 MiB.

Current-session normal/LTCG object files were deleted after the target-bound
probe results were summarized. They are reproducible from tracked source and the
pinned toolchain. The current campaign retains only the compact compiler report
and its manifest. Post-cleanup `.analysis/` before final manifest closeout was
**93,172 bytes**. No older campaign, target, provider database, toolchain, Wine
prefix, or unknown/shared artifact was removed.

## Validation actually performed

Already run successfully in this packet:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
scripts/compile-probe.sh src/PbgArchive.cpp <normal.obj> /TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc
scripts/compile-probe.sh src/PbgArchive.cpp <ltcg.obj> /TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL
python3 scripts/progress.py
python3 scripts/progress.py --check
git diff --check
```

Probe-mode `scripts/compare-coff-function.py` was run for `0x00434CA0`,
`0x00436640`, and `0x00436660` with the results above.

After the source/ledger/handoff edits, the current tracked source was compiled
again as both fixed-profile normal COFF and `/GL`; both passed. The full
checkpoint surface was then rerun successfully: `verify-target.py`,
`verify-toolchain.py --check`, tracking, progress, the 0-unit match graph, the
open build graph, reconstruction status, public CI, and `git diff --check`. The
honest `python3 scripts/build.py` returned the expected RC=2/open result with
compile flags, TU partition, libraries, resources, and link order still unknown.
`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` remained
unchanged. A post-edit Ghidra `check {}` also passed exact target/native
transport attestation.

## Remaining unknowns/blockers

- Original identifier and physical original TU for `0x00434CA0`.
- Per-function normal-COFF versus LTCG physical ownership for the archive and
  resource-backend seam.
- Explicit-source versus compiler-synthesized lifecycle origin of
  `0x00436640` and `0x00436680`.
- Concrete resource name for the retained resource-load method: none is
  recoverable from this target because it has no entry reference and no
  `RT_RCDATA` resource.
- Original identifiers/source ownership for reviewed `0x00435800` and
  `0x004358E0` archive/disk helpers.
- Production TU partition, compile profiles, libraries, exact resources, link
  order, whole-build closure, and runtime behavior.

## Next evidence-connected hard packet

Prefer the **archive transform cohort** centered on:

- `0x00435DC0-0x00435F9A` — 475 bytes, currently `unknown/review`;
- `0x0044B0D0-0x0044B212` — 323 bytes, currently `unknown/review`.

Attested Ghidra caller analysis shows the two functions have the **same four
callers**:

- `0x0042A200`;
- `0x0042B030`;
- reviewed `PbgArchive::ReadDecompressEntry` at `0x00434DD0`;
- reviewed `PbgArchive::ParseHeader` at `0x00434F70`.

Maintained `src/PbgArchive.cpp` currently declares these semantics externally as
`DecompressData` and `FileSystem::Decrypt`. The next packet should first recover
both target extents, ABIs, caller argument shapes, tails/padding, tables/data
owners, and the relationship between the two non-archive callers. Focused
TH08/TH095 `Lzss`/decrypt source may then be used only as a hypothesis after
TH10 facts are established. This is intentionally a hard-frontier cohort (475 +
323 bytes, cross-subsystem callers, compression/transform data ownership), not
the easiest remaining function.

The first next-session commands after mandatory recovery/preflight should be
attested `function`/`decompile`/`callers`/`callees`/`disassemble` queries for
`0x00435DC0` and `0x0044B0D0`, followed by bounded raw target boundary/data
inspection.

## Checkpoint closeout

The first local checkpoint for this packet was
`3d9dfd7ddaba1c85fc65e603ce36e787350bc241`, subject
`gpt-web: reconstruct TH10 archive resource load`. Immediately after that
commit, the tracked/untracked worktree was clean with zero staged, unstaged,
untracked, or conflicted paths. `main` was ahead 4 / behind 0 relative to
`origin/main`. No push was performed.

The ignored campaign manifest was then marked `checkpointed`, bound to that
checkpoint with `pushed=false`, and `.analysis/` measured **94,869 bytes**
after the final-size field was written. The current campaign retained only the
2,210-byte compact compiler/target report and its manifest; all current-session
normal/LTCG object outputs had already been removed as reproducible scratch.

This handoff-only closeout update is amended into the same logical checkpoint.
The final live Git HEAD after that amend is authoritative; the ignored manifest
is rebound to that final hash. Final repository status, Ghidra attestation,
Truth Kernel availability, and `.analysis/` size are re-read without changing
tracked reconstruction conclusions.
