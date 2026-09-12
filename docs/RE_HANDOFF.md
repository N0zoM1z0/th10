# TH10 reconstruction handoff

Status: **active-incomplete exact reconstruction campaign**. This checkpoint
reviews the ResFile memory/resource backend owner seam and adds maintained
source. It does **not** establish canonical function exactness, original source
identifiers, original translation-unit ownership, whole-product closure,
runtime validation, or semantic/portable phase readiness.

## Recovery and checkpoint baseline

- Repository: `th10`
- Analysis provider: `th10-ghidra`
- Target identity: `target:th10-main`
- Branch: `main`
- Starting HEAD: `0ec8087410343f24f6ea5826ed31b606751c0e50`
- Starting upstream: `origin/main` at
  `2f4a3350fc61132e7eb5e340f657527965ae5500`
- Starting relation: ahead 2 / behind 0
- Starting worktree: 0 staged, 0 unstaged, 0 untracked, 0 conflicts
- Starting `.analysis/`: 87,227 bytes
- Starting `build/`: approximately 5.0 MiB; retained generated/open state

The mandatory recovery gate found no interrupted tracked or untracked work.
Preserved ignored/private state was classified as follows:

- `resources/th10.exe`: operator-supplied private target; preserve/exclude.
- `.tools/`: shared tool selection plus game-bound Wine/tool state; preserve.
- `ghidra-project/`: target-bound provider state; preserve.
- `.analysis/`: prior checkpointed/reproducible diagnostics plus current scratch;
  preserve except explicitly current-session reproducible scratch.
- `build/`: reproducible/open build output; preserve.

No unrelated or unknown dirty path was reset, overwritten, staged, or deleted.
The private target was not modified, moved, or committed. `/mnt` was not
searched and `TH10_TARGET_PATH` was not set.

Before editing, the session fully reread `AGENTS.md`, this handoff,
`docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, `docs/TOOLS.md`, both required Factory
contracts, the Factory ontology/verification/autonomy/bootstrap/recovery docs,
`docs/semantic-reconstruction.md`, and
`.agents/skills/th10-reconstruction/SKILL.md`. No guidance path was missing.

## Mandatory preflight

All repository preflights passed before target-dependent editing:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

Target identity remained:

- size 487,936 bytes;
- SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`;
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`;
- PE32 i386, four sections, image base `0x00400000`, entry `0x004537DC`;
- Rich checksum `0x3152A748`, dominant build 6030.

The toolchain execute smoke passed normal COFF, C++ LTCG, resources, and PE32
linking under the pinned VC7.1 SP1 candidate.

`factory_list_analysis_operations(th10-ghidra)` was rediscovered before use.
`factory_analysis_call(th10-ghidra, check, {})` passed with exact target
`target:th10-main`, target hash/layout/entry agreement, and
`attestation.provider_transport=factory-native-command`. Every useful Ghidra
result in this packet carried the same passed attestation and
`exactness_credit=none`.

Pre-packet ledger state was:

- 1,219 candidates;
- 1,163 origin/boundary pending;
- 52 authored functions / 10,162 authored bytes;
- 4 exclusions;
- 25 source-present mappings;
- 0 canonical exact functions / 0 exact authored bytes.

## Packet selection and bounded scope

The hard packet followed the prior PbgFile handoff into the ResFile owner seam.
The initial routing range was `0x004362B0-0x00436767`, centered on the
Ghidra-missed body at `0x00436400`. The observable outcome was to close target
vtable ownership, object layout, raw retained methods, and machine ABI before
adding any maintained source.

Target evidence narrowed the real owner packet to:

- first vtable `0x0046F308-0x0046F327`;
- second/derived vtable `0x0046F328-0x0046F347`;
- raw/reviewed method bodies `0x00436370-0x0043655A`;
- derived lifecycle/Open bodies `0x00436640-0x00436767`;
- direct derived allocation/vptr construction at `0x00434CA0-0x00434CD4`.

Nearby candidates were deliberately **not** absorbed merely because of address
proximity:

- `0x004362B0` and `0x00436330` manipulate an unrelated global indexed/tree-like
  structure rooted near `0x00477858/5C/60`;
- `0x00436570` is a `CreateProcessA` helper;
- raw `0x00436560` has no useful target xref and was not promoted into this owner.

Those candidates remain unknown/review where they were already tracked.

## Target-local ResFile owner result

### Memory-backed implementation

The first target vtable at `0x0046F308` has exactly the eight `IPbgFile` slots:

1. `0x00436400` — Open
2. `0x00436430` — Close
3. `0x00436460` — Read
4. `0x00436370` — Write
5. `0x004364D0` — Tell
6. `0x004364E0` — GetSize
7. `0x004364F0` — Seek
8. `0x004363A0` — MSVC deleting destructor

The raw constructor `0x00436380-0x00436391` writes that vtable and clears three
fields. Together with all virtual-method accesses this closes the target layout
to exactly 0x10 bytes:

- `+0x00`: vptr;
- `+0x04`: DWORD size;
- `+0x08`: current byte pointer;
- `+0x0C`: owned malloc buffer/base pointer.

Maintained source uses the descriptive name `CMemoryPbgFile`. This is **not** an
original identifier claim.

Reviewed behavior:

- `0x00436400-0x0043642D`: calls the already reviewed archive-or-disk data and
  size helpers, stores data/size/current, ignores its second stack argument, and
  returns whether data is non-null.
- `0x00436430-0x00436454`: frees the owned buffer when non-null and clears all
  three state fields.
- `0x00436460-0x004364CB`: copies requested bytes when available or the remaining
  tail, advances current, and returns bytes copied.
- `0x00436370-0x00436374`: read-only `Write`; always returns false with `ret 8`.
- `0x004364D0-0x004364D6`: current minus data base.
- `0x004364E0-0x004364E3`: returns size.
- `0x004364F0-0x0043655A`: target-specific `FILE_BEGIN/CURRENT/END` seek logic.
  The maintained source preserves the observed unusual strict bounds: current-
  relative negative movement has no lower-bound check, exact end is rejected in
  the observed begin/current cases, and `-size` is rejected for `FILE_END`.
- `0x004363C0-0x004363F0`: authored cleanup body; installs the derived vptr,
  frees/clears owned data, then restores the `IPbgFile` vptr.
- `0x004363A0-0x004363BD`: compiler-generated scalar deleting destructor;
  excluded rather than source-present.

### Win32-resource derived implementation

The second vtable at `0x0046F328` shares Close/Read/Write/Tell/GetSize/Seek with
the memory implementation. It overrides only:

- slot +0x00: `0x004366C0-0x00436767`;
- deleting destructor slot +0x1C: `0x00436660-0x0043667D`.

A direct target allocation path at `0x00434CA0` calls `operator new(0x10)`,
clears `+0x04/+0x08/+0x0C`, and writes vptr `0x0046F328`, independently proving
that the derived object has no added storage. Maintained source uses descriptive
name `CWin32ResourcePbgFile`.

`0x004366C0` first closes existing storage, then loads `RT_RCDATA` through
`FindResourceA`, `LoadResource`, `LockResource`, and `SizeofResource`, mallocs
and copies the bytes, sets current to the copied buffer, and calls
`FreeResource`. Direct disassembly confirms that the reviewed failure exits also
return true after cleanup; maintained source preserves that target behavior.
The second stack argument is ignored.

The derived lifecycle bodies remain deliberately split by origin evidence:

- `0x00436660`: high-confidence MSVC deleting destructor, compiler-generated and
  excluded;
- raw `0x00436640-0x00436651`: constructor-shaped, but may be a compiler-
  synthesized implicit derived constructor with inlined base construction;
  remains unknown/review and has no source mapping;
- `0x00436680-0x004366B0`: destructor-shaped, but may be an implicit derived
  destructor with inlined base destruction; remains unknown/review and has no
  source mapping.

Explicit source versus compiler-synthesized lifecycle cannot be chosen from the
current TH10 evidence, so unknown is retained.

## Denominator challenge

Eight complete CC-delimited target entries missing from the Ghidra denominator
were added:

- `0x00436370-0x00436374`
- `0x00436380-0x00436391`
- `0x00436400-0x0043642D`
- `0x00436460-0x004364CB`
- `0x004364D0-0x004364D6`
- `0x004364E0-0x004364E3`
- `0x004364F0-0x0043655A`
- `0x00436640-0x00436651`

Seven are reviewed authored memory-file source bodies. The derived constructor-
shaped `0x00436640` remains unknown/review. Two pre-existing Ghidra candidates
were newly classified compiler-generated deleting destructors, while the
existing `0x00436680` lifecycle body stays unknown/review.

A bounded whole-`.text` direct-call/literal scan found no ordinary direct calls
to these raw virtual methods/constructors. The observed direct relations are the
vtable data references, deleting-destructor calls to non-deleting destructors,
and direct vptr stores. This is optimizer/owner-context evidence but does not by
itself prove LTCG ownership.

## Maintained source and compiler feedback

Added:

- `src/ResFile.hpp`
- `src/ResFile.cpp`

The source deliberately uses descriptive reconstruction class/helper names. It
does not claim recovered original C++ identifiers or a physical original TU.
The archive-or-disk helpers are declared descriptively because their target
behavior is reviewed but their original identifiers/source-level convention/TU
ownership remain unknown.

The same natural source compiled successfully with the pinned VC7.1 SP1
candidate as:

- normal COFF: `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`;
- C++ LTCG diagnostic: the same profile plus `/GL`.

Normal-COFF target-bound probe comparisons produced:

| Maintained body | Result | Comparable bytes |
| --- | --- | ---: |
| `CMemoryPbgFile` constructor | mismatch | 0 / 14 |
| `CMemoryPbgFile` destructor | structural-exact | 37 / 37 |
| memory Open | mismatch | 8 / 38 |
| Close | structural-exact | 33 / 33 |
| Read | structural-exact | 108 / 108 |
| Write | structural-exact | 5 / 5 |
| Tell | structural-exact | 7 / 7 |
| GetSize | structural-exact | 4 / 4 |
| Seek | mismatch | 6 / 107 |
| Win32-resource Open | mismatch | 8 / 144 |

The constructor, both Opens, and Seek have target/object extent equality where
shown by the comparator but retain substantial code-shape/register/control-flow
differences. These mixed results are evidence against treating the whole owner
as one demonstrated standalone normal-COFF unit. `/GL` compilation succeeds,
but there is no accepted target-bound linked-image extent Oracle for LTCG.

All probe results have `acceptance_authority=none`. No canonical match unit and
no exact ledger row were created.

Compact retained compiler evidence is:

- `.analysis/gpt-web/20260912-resfile-owner/compiler-shape-report.txt`

The normal and LTCG `.obj` files plus the detailed JSON comparator output were
current-session reproducible scratch. After their compact results were retained,
they were deleted. No legacy/unknown analysis state, provider state, target,
toolchain, or Wine state was removed.

`.analysis/` sizes observed in this packet:

- session entry: 87,227 bytes;
- campaign peak after compiler objects/reports: 132,807 bytes;
- after current-session scratch cleanup: 88,867 bytes before final manifest
  closeout.

No file approached the 64 MiB individual review threshold and the campaign was
far below 256 MiB.

## Source-path and adjacent-game discipline

The target string `.\\src\\pack\\ResFile.cpp:82 BYTE` remains direct evidence
that that source path participated in the original build. This packet extended
the negative reference check: attested xrefs, bounded text-immediate scans, and
raw little-endian pointer scans for every address across the string range found
no code reference. Therefore maintained `src/ResFile.cpp` is a reconstruction
source location, **not** a claim that any mapped function has established
original `src/pack/ResFile.cpp` physical ownership.

Adjacent repositories were consulted only after TH10 established the local
vtable/layout evidence, and only committed HEAD content/history was searched:

- TH08: `a45e99fb1942714e6edded20847e32a654d56f97`, clean;
- TH09: `3b4fbc82d7012fa99f68e5d1dec9c06a5463b342`, unrelated dirty tracked
  `config/match-units.toml`, `src/ScoreData.cpp`, and
  `src/SupervisorDeletedCallback.cpp` preserved and not read as hypothesis
  material;
- TH095: `a1b4bc7174e5fd445b339a8d3bd6147591524ebc`, unrelated untracked
  `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`,
  and `scripts/runtime-diff.py` preserved and not read as hypothesis material.

No committed adjacent repository supplied a ResFile implementation or original
class name. TH10 naming therefore remains descriptive rather than transferred.

## Ledger/source result

Tracked state after this packet is:

- 1,227 function candidates;
- 1,159 origin/boundary pending;
- 62 authored functions;
- 10,711 authored bytes;
- 6 exclusions;
- 35 source-present authored mappings;
- 0 canonical exact functions / 0 exact authored bytes.

Relative to the packet start, this is:

- +8 denominator candidates;
- +10 reviewed authored functions;
- +549 reviewed authored bytes;
- +2 compiler-generated exclusions;
- +10 maintained source mappings;
- +0 canonical exact functions.

Tracked files changed by this packet are expected to be:

- `src/ResFile.hpp`
- `src/ResFile.cpp`
- `config/functions.csv`
- `config/function-origins.csv`
- `config/reccmp-functions.csv`
- `config/implemented.csv`
- `docs/KNOWLEDGE_BASE.md`
- `docs/PROGRESS.md`
- `docs/RE_HANDOFF.md`
- `resources/progress.svg`

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` have no
packet diff.

## Verification planes

### Source presence

35 reviewed authored functions have maintained source mappings after this
packet, including ten new ResFile-family mappings. Source presence is not an
original-TU or exactness claim.

### Function exactness

Still 0 canonical exact functions and 0 exact authored bytes. There are 0
configured canonical normal-COFF match units. Structural probe equality has no
acceptance authority and was not promoted.

### Whole Windows i386 build

Still explicitly open. `python3 scripts/build.py --check` passes the target-bound
open skeleton. An honest `python3 scripts/build.py` returns expected RC=2 with
compile flags, TU partition, libraries, resources, and link order still unknown.
This is not whole-product closure.

### Runtime

Not started. No reconstructed faithful Windows i386 product is closed yet, so no
runtime scenario is claimed.

### Factory accepted facts

A pre-checkpoint `factory_get_accepted_snapshot(th10)` refresh was attempted and
was temporarily unavailable because another Factory operation owned the shared
`<operator-path>`. No new acceptance is claimed from that unavailable check.
The previous handoff's older accepted `target_attested` fact is not reused as
proof for this packet. A post-commit closeout refresh may be reported separately
if the shared path becomes available.

## Commands/checks actually exercised

Target/repository gates:

- `python3 scripts/verify-target.py` — pass
- `python3 scripts/verify-toolchain.py --execute` — pass
- `python3 scripts/verify-toolchain.py --check` — pass
- `python3 scripts/validate-tracking.py --require-target` — pass
- `python3 scripts/report-reconstruction-status.py` — pass
- `python3 scripts/progress.py` and `--check` — pass
- `python3 scripts/build-match-unit.py --check` — pass, 0 units
- `python3 scripts/build.py --check` — pass, explicitly open
- `python3 scripts/build.py` — expected RC=2/open
- `python3 scripts/ci.py` — pass
- `git diff --check` — pass

Compiler diagnostics:

- `scripts/compile-probe.sh` normal VC7.1 COFF for `src/ResFile.cpp` — pass
- same maintained source with `/GL` — pass
- `scripts/compare-coff-function.py` probe-mode comparisons for ten reviewed
  target bodies — six structural-exact diagnostics, four mismatches; authority
  none as detailed above.

Target analysis included attested `function`, `decompile`, `xrefs_to`,
`xrefs_from`, and bounded `disassemble` calls plus bounded direct target
`objdump`/literal scans. No Ghidra output was treated as exactness evidence.

## Remaining unknowns and blockers

- Original class identifiers for the two ResFile-family implementations.
- Whether raw derived lifecycle bodies `0x00436640` and `0x00436680` came from
  explicit source or compiler-synthesized implicit lifecycle methods.
- Per-function normal-COFF versus LTCG physical ownership for the ResFile seam.
- Original source identifiers/TU ownership/source-level convention for reviewed
  archive-or-disk helpers `0x00435800` and `0x004358E0`.
- Why the target memory-file `Seek` has the observed code shape under its actual
  owner/profile despite equal normal-COFF extent.
- Translation-unit partition, libraries, resources, link order, and production
  whole-build graph.
- Existing unresolved PbgFile base lifecycle/slash-helper rows and broader Main
  raw gaps from earlier packets.

## Next evidence-connected hard packet

Prefer `0x00434CA0-0x00434D03`, currently a 100-byte Ghidra candidate still
`unknown/review`.

This is not an easy-function selection. Direct TH10 target bytes show that it:

- is an independent CC-delimited body after reviewed `PbgArchive::Load`
  (`0x00434C30-0x00434C9B`), not an interior block;
- releases prior archive state;
- allocates exactly 0x10 bytes;
- clears the three ResFile-family state fields;
- writes the newly reviewed Win32-resource vtable `0x0046F328`;
- stores that backend in the archive owner at `+0x0C`;
- continues through the reviewed archive parse/filename path.

The next packet should first close callers, machine ABI, exact boundary/padding,
resource-name flow, failure cleanup, and its relation to `PbgArchive::Load` and
`ParseHeader`. Only then test whether maintained `PbgArchive`/ResFile source can
represent it naturally and whether any source mapping/origin promotion is
justified. Re-audit the archive backend owner before making a TU/profile claim.

Packet-selection balance remains hard-frontier weighted: the preceding packet
closed the central PbgFile polymorphic backend and this packet closed two linked
ResFile vtables plus hidden methods and object ownership. The next 100-byte
archive/resource owner body continues that ownership chain rather than selecting
a small independent leaf for function-count progress.

No push is authorized or intended. The checkpoint commit for this handoff must
remain local and use an English `gpt-web:` subject.
