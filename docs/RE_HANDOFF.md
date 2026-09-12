# TH10 reconstruction handoff

Status: **active-incomplete exact reconstruction campaign**. This checkpoint
reviews the shared archive/replay/score transform cohort centered on
`0x00435DC0` and `0x0044B0D0`, adds maintained decompression/decrypt source,
closes the shared LZSS dictionary storage extent, and expands the denominator
with two Ghidra-missed LZSS state helpers. It does **not** establish canonical
function exactness, original translation-unit ownership, whole Windows i386
product closure, runtime validation, semantic reconstruction, or portability
readiness.

## Recovery baseline

Exact selection for this session:

- repository: `th10`
- provider: `th10-ghidra`
- target: `target:th10-main`
- branch: `main`
- starting HEAD: `45e5c727731c14489ced198721c6bf1f5b8783c9`
- starting upstream: `origin/main` at
  `2f4a3350fc61132e7eb5e340f657527965ae5500`
- starting relation: ahead 4 / behind 0
- starting worktree: 0 staged / 0 unstaged / 0 untracked / 0 conflicts
- starting `.analysis/`: 94,869 bytes
- starting `build/`: approximately 5.0 MiB of generated/open state

The mandatory recovery review inspected recent history, branch/HEAD/upstream,
porcelain-v2, complete staged/unstaged diffs, all untracked paths, relevant
ignored state, `.analysis/`, `build/`, and the prior handoff. There was no
recoverable interrupted tracked/untracked work.

Preserved classifications:

- `resources/th10.exe`: operator-supplied ignored private target; preserve and
  never stage/modify/relocate.
- `.tools/`: shared immutable tool selectors plus game-bound mutable tool/Wine
  state; preserve.
- `ghidra-project/`: target-bound provider state; preserve.
- prior `.analysis/` campaigns/bootstrap: checkpointed, reproducible, shared, or
  legacy evidence; preserve unless current-session ownership is explicit.
- `build/`: reproducible/open build output; preserve.

No unknown or unrelated work was reset, overwritten, deleted, staged, or
committed. `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

Before editing, the session fully reread `AGENTS.md`, this handoff,
`docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, `docs/TOOLS.md`, both required Factory
contracts, Factory ontology/verification/autonomy/bootstrap/recovery guidance,
`docs/semantic-reconstruction.md`, and the repository-local
`.agents/skills/th10-reconstruction/SKILL.md`. No required guidance path was
missing and no injected skill body was assumed.

## Mandatory preflight

Before target-dependent analysis these passed:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Target identity remained original Japanese TH10 v1.00a:

- size 487,936 bytes
- SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, four sections, image base `0x00400000`, entry `0x004537DC`
- Rich checksum `0x3152A748`, dominant build 6030

The pinned VC7.1 SP1 execute smoke passed normal C/C++ COFF, C++ LTCG,
resources, and PE32 linking under headless Wine. This proves tool availability,
not a per-function profile or original TU owner.

Native Ghidra schemas were rediscovered with
`factory_list_analysis_operations(th10-ghidra)`. Mandatory `check {}` passed
with `attestation.status=passed`, exact `target:th10-main`, and
`provider_transport=factory-native-command`. Every useful Ghidra result in this
packet carried the same passed target/native attestation and
`exactness_credit=none`.

Entry ledger state was:

- candidates: 1,227
- origin/boundary pending: 1,158
- reviewed authored: 63 functions / 10,811 bytes
- exclusions: 6
- source-present mappings: 36
- canonical exact: 0 functions / 0 bytes
- canonical normal-COFF match units: 0

## Hard packet selection

The selected hard frontier followed the prior archive-resource checkpoint into
the two unresolved transforms already consumed by maintained
`src/PbgArchive.cpp`:

- `0x00435DC0-0x00435F9A`, 475 bytes, initially `unknown/review`
- `0x0044B0D0-0x0044B212`, 323 bytes, initially `unknown/review`

They were selected because both feed reviewed archive parser/read paths and have
shared non-archive replay/score-like callers. The observable goals were to
recover physical extents, ABIs, algorithms, shared data ownership, source shape,
and compiler feedback while continuing to challenge the authored denominator.
This is not a small-function count strategy.

## `0x00435DC0-0x00435F9A` — maintained `DecompressData`

The target physical extent is complete:

- previous function ends at `0x00435DB4`
- `0x00435DB5-0x00435DBF` is `CC` padding
- decoder begins at `0x00435DC0`
- final `ret 0x10` occupies `0x00435F98-0x00435F9A`
- `0x00435F9B-0x00435F9F` is `CC` padding before a separate retained helper

Machine ABI is four stack arguments, callee-pop 16 bytes:

1. input pointer
2. compressed input size
3. output pointer or null
4. output allocation size

All four target call sites use that exact shape:

- `0x0042A317`
- `0x0042B08C`
- reviewed `PbgArchive::ReadDecompressEntry` at `0x00434EC2`
- reviewed `PbgArchive::ParseHeader` at `0x0043505E`

Target behavior is a LZSS-style bitstream decoder:

- input bit mask starts at `0x80`
- flag bit 1 selects an 8-bit literal
- flag bit 0 selects a 13-bit dictionary offset
- offset 0 terminates the stream
- the following 4-bit length field represents actual copy length minus three
- dictionary head starts at 1 and wraps with mask `0x1FFF`
- each output byte is mirrored into the shared dictionary
- when output is null the function calls `malloc(outputSize)` and returns null on
  allocation failure
- trailing bits are consumed through the same fetch/bit-mask path until the mask
  returns to `0x80`

Maintained name `DecompressData` is adjacent-supported after the TH10 algorithm
was recovered. It is not an original-symbol or original-TU claim.

## Shared LZSS dictionary storage

TH10 target-local evidence closes the ring storage itself:

- dictionary base: `0x0048F868`
- all decoder and encoder indexing uses mask `0x1FFF`
- neighboring encoder `0x004359B0` explicitly clears all 0x2000 dictionary bytes
  before encoding
- `0x0048F868 + 0x2000 = 0x00491868`
- independent densely referenced storage begins exactly at `0x00491868`

Therefore the shared encoder/decompressor dictionary is physically bounded to
exactly `0x0048F868-0x00491867`, 0x2000 bytes. Maintained source defines one
`g_DecompressionRing[0x2000]` with a compile-time size assertion. The original
source/data TU owner remains unknown.

The adjacent encoder/tree context was inspected only to avoid a false decoder-
private ownership claim. Existing candidates remain pending unless explicitly
listed below.

## Ghidra-missed retained LZSS helpers

Boundary review found two complete source-shaped bodies that were absent from
the candidate denominator.

### `0x00435FA0-0x00435FC3` — proposed `Lzss::InitTree`

- 36-byte CC-delimited body
- machine input: root through live-in EAX, plain `ret`
- stores root in the sentinel tree node
- sets `tree[root].parent = 0x2000`
- clears `tree[root]` child links
- `0x00435FC4-0x00435FCF` is `CC` padding
- Ghidra models no containing function and reports no entry xref
- neighboring encoder contains equivalent initialization semantics inline

A natural fixed normal-COFF helper probe is also 36 bytes, but standalone source
loads root from the ordinary stack ABI while target consumes EAX. This is
source-shape/private-ABI evidence only. The helper is reviewed authored but is
not source-mapped yet.

### `0x00435FD0-0x00435FF9` — proposed `Lzss::InitEncoderState`

- 42-byte CC-delimited, no-argument body
- clears exactly 0x2000 dictionary bytes using `rep stosd`
- clears all 0x2001 three-dword tree nodes rooted at `0x00477858`
- `0x00435FFA-0x00435FFF` is `CC` padding before `0x00436000`
- Ghidra models no containing function and reports no entry xref
- neighboring encoder inlines the same reset logic

A natural fixed normal-COFF helper probe is also 42 bytes and matches all 30
comparable non-relocation bytes. This has `acceptance_authority=none`; physical
normal-COFF ownership is not established. The helper is reviewed authored but
is intentionally not source-mapped until the wider encoder/tree owner is
reconstructed.

## `0x0044B0D0-0x0044B212` — maintained `FileSystem::Decrypt`

The target physical extent is complete:

- previous candidate ends at `0x0044B0CA`
- `0x0044B0CB-0x0044B0CF` is `CC` padding
- transform begins at `0x0044B0D0`
- final `ret 0x14` occupies `0x0044B210-0x0044B212`
- `0x0044B213-0x0044B21F` is `CC` padding before candidate `0x0044B220`

Ghidra's provisional five-parameter signature misses one real machine input.
Raw TH10 callers establish the actual boundary:

- initial XOR seed in live-in AL
- five stack arguments, callee-pop 20 bytes:
  1. data pointer
  2. size
  3. XOR increment
  4. chunk size
  5. max bytes

All six target call sites have that same AL-plus-five-stack shape:

- `0x0042A2E8`: seed `0xAA`, increment `0xE1`, chunk `0x400`
- `0x0042A2FF`: seed `0x3D`, increment `0x7A`, chunk `0x80`
- `0x0042B064`: seed `0xAC`, increment `0x35`, chunk `0x10`
- `0x00434EAD`: filename-checksum-selected archive profile
- `0x00434FCA`: seed `0x1B`, increment `0x37`, chunk `0x10`
- `0x00435050`: seed `0x3E`, increment `0x9B`, chunk `0x80`

The natural source-level declaration retains six explicit parameters; the
private AL placement is recorded as machine ABI rather than falsely written as a
source calling convention.

TH10 behavior:

- scratch copy size is `min(maxBytes, size)`
- a small untransformed tail is selected when `size % chunkSize < chunkSize/4`,
  then adjusted by size parity
- transformed size excludes that tail
- the bounded prefix is copied to scratch
- for each chunk, scratch is consumed forward while output bytes are written
  backward from alternating positions at the chunk end
- every transformed byte XORs with the evolving key and then increments the key
- remaining size/maxBytes shrink by the processed chunk
- scratch is freed and the original data pointer is returned

Maintained `FileSystem::Decrypt` is adjacent-supported naming after TH10-local
ABI/behavior recovery; it is not an original-symbol/TU claim.

## Maintained source and compiler feedback

Added tracked source:

- `src/Decompress.hpp`
- `src/Decompress.cpp`
- `src/FileSystem.hpp`
- `src/FileSystem.cpp`

Updated `src/PbgArchive.cpp` to include those headers instead of carrying local
forward declarations. No original TU assignment was added to `config/build.toml`.

The maintained source compiles successfully with the pinned VC7.1 SP1 candidate
as both fixed-profile normal COFF and the same profile plus `/GL`. Updated
`src/PbgArchive.cpp` also compiles under both artifact families.

Normal-COFF target-bound diagnostic results:

- `DecompressData`: object 475 bytes / target 475 bytes, mismatch,
  53/459 comparable bytes for the maintained version
- one deliberately bounded historical local-declaration-order probe, same fixed
  flags and semantics: 475 bytes, mismatch, 186/459 comparable bytes
- `FileSystem::Decrypt`: object 323 bytes / target 323 bytes, mismatch,
  13/315 comparable bytes
- natural `InitTree` helper: object/target both 36 bytes, mismatch because of
  ordinary stack argument versus target live-in EAX
- natural `InitEncoderState`: object/target both 42 bytes, structural-exact
  30/30 comparable non-relocation bytes

The declaration-order decoder probe was performed once to distinguish a simple
maintained-source-shape issue from broader profile/context effects. It improved
similarity but remained clearly non-exact. No further source/profile fishing was
performed. Tracked decoder source remains the simpler version containing only
TH10-observed behavior rather than importing a discarded adjacent checksum or
oracle-specific local aliases.

All of these are diagnostic results with `acceptance_authority=none`.
Normal-COFF versus LTCG physical ownership remains unknown and no canonical
match unit or exact row was added.

Compact retained evidence:

`.analysis/gpt-web/20260912-archive-transform/compiler-shape-report.txt`

## Adjacent-game discipline

Adjacent source was consulted only after TH10 target facts were established and
only committed HEAD content was used.

Observed adjacent states:

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch
  `port/portable-64bit`, clean. Committed `src/pbg/Lzss.cpp` corroborates the
  13/4-bit LZSS algorithm and helper names; committed `Global.cpp` corroborates
  the transform family.
- TH09: HEAD `96c24de34e12b8dc8bd6a24875a7bfb31a55cdfe`, branch `main`, ahead 4,
  clean at consultation. Committed `LzssDecode.cpp` and `FileDecrypt.cpp`
  corroborate natural source shape; no uncommitted content was used.
- TH095: HEAD `a1b4bc7174e5fd445b339a8d3bd6147591524ebc`, branch `main`, ahead 2,
  with unrelated untracked `EnemyManagerUpdate.i`,
  `config/runtime-scenarios.json`, `droid.resume.txt`, and
  `scripts/runtime-diff.py`. Only committed `Decompress.cpp`, `FileSystem.cpp`,
  and LZSS source was consulted.

TH10 target facts always take precedence. Adjacent addresses, ownership,
exactness, declaration-order hacks, and completion claims were not transferred.

## Ledger and verification planes after this packet

After generated progress refresh:

- tracked candidates: **1,229**
- origin/boundary pending: **1,156**
- reviewed authored: **67 functions / 11,687 bytes**
- exclusions: **6**
- source-present authored mappings: **38**
- canonical exact: **0 functions / 0 bytes**
- canonical normal-COFF match units: **0**

Relative to session entry:

- candidates: +2
- reviewed authored: +4 functions / +876 bytes
- exclusions: +0
- source mappings: +2
- canonical exact: +0

Verification planes remain independent:

- **source presence:** 38 maintained authored mappings, two new here.
- **exactness:** 0 canonical exact functions/bytes. Equal extents and structural
  probe equality are diagnostics only.
- **whole Windows i386 build:** open; no original production source ownership
  has been added to `config/build.toml`.
- **runtime validation:** not started.
- **semantic/port phases:** not started.
- **Factory acceptance:** a pre-checkpoint snapshot refresh was unavailable
  because another Factory operation owned the shared `<operator-path>`. No
  current acceptance/rejection is inferred and no packet fact is claimed
  accepted.

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` remain
unchanged in this packet.

## Analysis artifacts

Session entry `.analysis/`: **94,869 bytes**.

Current campaign:

`.analysis/gpt-web/20260912-archive-transform/`

Peak observed `.analysis/`: **187,316 bytes**, far below the 256 MiB campaign
review threshold; no individual file approached 64 MiB.

After reviewing compiler results, all current-session normal/LTCG objects,
detailed comparator JSON, and temporary helper/declaration-order probe source
were deleted as explicitly owned reproducible scratch. Only the compact
2,383-byte report and manifest remain in this campaign. Post-cleanup
`.analysis/` was **98,645 bytes** before final manifest closeout. No prior
campaign, target, provider state, toolchain, Wine prefix, or unknown/shared
artifact was removed.

## Commands/checks actually exercised so far

Entry/preflight:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Target analysis used attested `function`, `decompile`, `callers`, `callees`,
`xrefs_to`, and `disassemble` operations plus bounded raw target `objdump` and
literal scans. One read-only Factory pagination call failed at the transport
layer; live repository status was re-read immediately, remained unchanged, and
the same durable output page was then retrieved successfully.

Compiler feedback used `scripts/compile-probe.sh` for fixed normal and `/GL`
profiles and `scripts/compare-coff-function.py` in diagnostic/probe mode. No
`/GL` hypothesis was stripped to fit the normal-COFF Oracle.

After the final tracked source/ledger/handoff state, the affected sources were
compiled again in a command-local temporary directory: `Decompress.cpp`,
`FileSystem.cpp`, and `PbgArchive.cpp` all passed both fixed-profile normal COFF
and the same profile plus `/GL`. The cold checkpoint surface then passed:
`verify-target.py`, `verify-toolchain.py --check`, tracking, generated progress,
the 0-unit normal-COFF match graph, the explicitly open build graph,
reconstruction status, public CI, and `git diff --check`. The honest
`python3 scripts/build.py` returned expected RC=2/open because compile flags, TU
partition, libraries, resources, and link order remain unknown. A post-edit Ghidra `check {}` also passed exact target/native-transport
attestation.

## Remaining unknowns/blockers

- Per-function normal-COFF versus LTCG physical ownership for decoder/decrypt.
- Original identifiers and original translation units for the maintained
  `DecompressData` and `FileSystem::Decrypt` names.
- Original data/source owner for the target-proven 0x2000-byte shared dictionary.
- Complete physical/source ownership of the LZSS tree beginning at `0x00477858`.
- The full encoder/tree helper cohort remains boundary/origin pending except for
  the two retained init helpers reviewed here.
- `0x0044B220` appears to be a related transform sibling but remains unreviewed.
- Production TU partition, exact compiler profiles, libraries, resources, link
  order, whole-build closure, and runtime behavior remain unknown/open.

## Next evidence-connected hard packet

Prefer the **LZSS encoder/tree owner cohort**, now directly connected to the
shared dictionary recovered in this packet:

- `0x004359B0-0x00435DB4` — 1,029 ledger bytes, provisional encoder behavior
- reviewed retained `0x00435FA0-0x00435FC3` — proposed `Lzss::InitTree`
- reviewed retained `0x00435FD0-0x00435FF9` — proposed `Lzss::InitEncoderState`
- `0x00436000-0x00436203` — 516 bytes, AddString/match-search behavior
- `0x00436210-0x0043625F` — 80 bytes, DeleteString-like behavior
- `0x00436260-0x004362AE` — 79 bytes, ContractNode-like behavior
- `0x004362B0-0x00436324` — 117 bytes, ReplaceNode-like behavior
- `0x00436330-0x00436360` — ledger 49 bytes / Ghidra 43 body addresses,
  FindNextNode-like behavior; boundary must be reconciled before promotion

The earlier ResFile packet correctly excluded `0x004362B0/0x00436330` from the
ResFile owner, but current target-local evidence identifies their indexed globals
as the LZSS tree rather than an unrelated anonymous structure. That refined
routing should be used next.

The next packet should first close raw boundaries/ABIs/padding and the physical
`0x00477858` tree extent, then compare the full cohort against committed adjacent
LZSS source. If source presence is justified, map the two already-reviewed init
helpers together with the encoder/tree owner rather than cherry-picking their
small structural wins.

This is intentionally another hard frontier: more than 1.8 KiB of central
compression code plus shared data ownership and private register ABIs. An
alternate connected route, if the encoder cohort is blocked, is the unreviewed
`0x0044B220` transform sibling next to reviewed `FileSystem::Decrypt`.

## Checkpoint closeout

The first local checkpoint for this packet was
`4620038d45f6414ef4e181678ba8817763cf5ee8`, subject
`gpt-web: reconstruct TH10 archive transform seam`. Immediately after that
commit the tracked/untracked worktree was clean with zero staged, unstaged,
untracked, or conflicted paths. `main` was ahead 5 / behind 0 relative to
`origin/main`. No push was performed.

The ignored campaign manifest was then marked `checkpointed`, bound to that
checkpoint with `pushed=false`, and `.analysis/` measured **99,128 bytes**
after writing its final-size field. The current campaign retained only the
2,383-byte compact compiler/target report and its manifest; all current-session
objects, detailed JSON, and temporary probe source had already been removed as
reproducible scratch.

This handoff-only closeout update is amended into the same logical checkpoint.
The final live Git HEAD after that amend is authoritative and the ignored
manifest is rebound to it. Final repository status, cold validation, Ghidra
attestation, Truth Kernel availability, and `.analysis/` size are re-read
without changing tracked reconstruction conclusions.
