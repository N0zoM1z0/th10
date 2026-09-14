# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`, target `target:th10-main`, analysis provider `th10-ghidra`.
- Current session starting HEAD: `444eb1a63d4e3a91c4bfcad90c63da25d55c4a02`, clean, branch `main`, ahead of `origin/main` by two local `gpt-web:` commits.
- Completed session checkpoint: `bd9b2e3 gpt-5.6-sol: promote exact PbgFile accessors`.
- Completed session checkpoint: `9b5e7eb gpt-5.6-sol: add exact replay workflow`.
- Completed session checkpoint: `4ed34ee gpt-5.6-sol: promote exact PbgArchive lifecycles`.
- Completed session checkpoint: `621add9 gpt-5.6-sol: promote exact LZSS state reset`.
- Completed session checkpoint: `88800b6 gpt-5.6-sol: report exact reconstruction backlog`.
- Completed session checkpoint: `96f17c6 gpt-5.6-sol: harden COFF extent diagnostics`.
- Completed session checkpoint: `d59f975 gpt-5.6-sol: batch probe exact backlog`.
- Completed session checkpoint: `26af714 gpt-5.6-sol: add LTCG linked-image diagnostics`.
- Completed session checkpoint: `f5a0c4f gpt-5.6-sol: add canonical LTCG replay`.
- Completed session checkpoint: `40c96ec gpt-5.6-sol: promote exact PbgArchive LTCG helpers`.
- Completed session checkpoint: `e4c9077 gpt-5.6-sol: promote exact LZSS LTCG helpers`.
- Completed session checkpoint: `37ebb83 gpt-5.6-sol: promote remaining exact LTCG helpers`.
- Completed session checkpoint: `ead21f9 gpt-5.6-sol: rank exact reconstruction backlog`.
- Planned current checkpoint subject: `gpt-5.6-sol: recover ECL core lifecycles`. Final commit hash is intentionally not self-recorded before the commit exists; recover it from live Git after checkpoint.
- Recovery found no staged, unstaged, or untracked files. The ignored private target, existing `.analysis/`, toolchain, Wine prefix, Ghidra project, and build caches were preserved.
- Current campaign: `.analysis/gpt-5.6-sol/20260914-ecl-core-lifecycle/`. The backlog-ranking, final-structural, Lzss, PbgArchive, canonical-replay, linked-diagnostic, and earlier session campaigns are checkpointed separately; earlier `gpt-web` campaigns remain ignored evidence and were not treated as current authority without replay.
- This session has not pushed. The exact-reconstruction campaign remains active/incomplete.

## Recovery and authority

The session inspected branch/HEAD/history, complete tracked/untracked state, the prior handoff, and the two commits ahead of `origin/main`. Clean HEAD `444eb1a...` was adopted as live authority.

All requested repository and Factory guidance was re-read from the live repository shell before tracked reconstruction work. No requested path was missing.

Fresh repository preflight passed on `444eb1a...`:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`

The execute toolchain path passed pinned VC7.1 SP1 build6030 normal COFF, C++ `/GL`, resource and PE32-i386 link smoke through headless Wine. Native `th10-ghidra` operation schemas were rediscovered and `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command`; an earlier refresh also passed. The latest operation-discovery attempt reported that the native implementation differed from its operator binding and returned no semantic result, so this packet uses direct hash-attested target evidence instead. Provider failures were treated as unavailable evidence, never as mismatch or rollback.

Target remains the ignored operator file `resources/th10.exe`: size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`, dominant Rich build6030. It was not modified, moved, staged or committed. `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Current packet: ECL core lifecycle and resource base

Direct TH10 review establishes a reusable ECL host around the full object's
embedded `0x1018` context. The two callback-subroutine transition paths call a
45-byte allocation-list release at `0x0040C6E0` and then the 71-byte execution
reset at `0x0040C730`. Maintained `EnemyRuntimeUpdate` now expresses both calls.
The release helper is canonical exact; reset is source-present and remains
non-exact at 69 linked bytes versus 71 target bytes.

The neighboring ECL resource base is now source-complete for its reviewed
lifecycle seam. Base vtable slot one is a five-byte default `LoadPackage`
returning zero; the five-byte indexed getter returns one of 32 script pointers
from `+0x0C`; the 34-byte constructor clears the complete `0x1098` object and
installs vtable `0x0046D0F0`; and the 36-byte destructor restores that vtable,
frees lookup owner `+0x8C`, and clears it. The embedded-context constructor
independently clears the two storage cursors at context `+0x1008/+0x100C`.

Two cold linked builds replay all six newly exact functions across **140
bytes**. The three nonzero-field units declare both release calls, the base
vtable fields, and the direct free call exhaustively; omitted-field negative
manifests are rejected. Repository exact coverage is now **39 functions / 1,555
bytes**. Production TU partition and linked-image ownership remain unknown.

## Completed packet: dual-codegen exact-backlog ranking

`scripts/rank-exact-backlog.py` now turns the two existing batch probes into one
fresh selection tool. It obtains the current authored backlog once, runs both
normal COFF and LTCG diagnostics, requires both reports to match every current
address/name/source/size key exactly, and rejects a run if the target, ledgers,
tools, or maintained source tree changes between the two probes. Its explicit
distance combines relative complete-extent size delta with the mismatch ratio
over non-link-field bytes. Profile recommendation and `ltcg_gain` remain triage
signals with `acceptance_authority=none`.

A full target-bound run covers all **53** current authored backlog functions.
Both lanes report **53 mismatches**, zero structural-exact candidates, and zero
unresolved keys. The leading bounded experiments are:

1. `Lzss::AddString`, LTCG: candidate/target **512/516 bytes**, **51/408**
   comparable mismatches, 27 linked fields, distance **0.133**;
2. `CPbgFile::Write`, normal COFF: **67/63**, **10/59**, one relocation,
   distance **0.233**;
3. `EnemyManagerCreate`, LTCG: **94/95**, **22/75**, five linked fields,
   distance **0.304**.

The ranking does not make any function exact or determine its production
profile/physical owner. It gives later sessions a reproducible reason to choose
a source/codegen experiment and preserves both lanes' raw metrics in JSON.

## Completed packet: final structural-exact LTCG promotions

The final seven unreviewed fields from the original linked diagnostic are now
bound to TH10-local targets. The Enemy link uses `g_EnemyManager` at
`0x00477704`, derived/base ECL-resource vtables at `0x0046D0B4` and
`0x0046D0F0`, and the already reviewed `free` entry at `0x00452422`. The
PbgFile link uses the reviewed `strchr` entry at `0x00452730`, complete
45-byte `strrchr` entry at `0x00452930`, and the `GetModuleFileNameA` IAT slot
at `0x004660DC`.

Two independent cold linked builds reproduce:

- `EnemyManagerView::EnemyManagerView`: **29/29 bytes**, one DIR32 field;
- `EnemyEclResourceView::EnemyEclResourceView`: **34/34 bytes**, one DIR32;
- `EnemyEclResourceView::~EnemyEclResourceView`: **36/36 bytes**, one DIR32
  and one REL32;
- `CPbgFile::GetFullFilePath`: **121/121 bytes**, one DIR32 and two REL32.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared fields. Replaying all seven fields leaves zero differences
across all **220 bytes**. Negative manifests omitting the Enemy global DIR32 or
the PbgFile `strrchr` REL32 are rejected. The `strrchr` boundary is independently
reviewed and classified library/CRT; `strchr` keeps origin/boundary state open
because its optimized body shares a backward return tail outside the current
Ghidra candidate. A fresh Enemy/PbgFile backlog probe reports 18 mismatches and
zero structural-exact candidates. All 13 structural candidates / 715 bytes from
the original full LTCG diagnostic have now been promoted through canonical
replay; none remains a diagnostic-only exact candidate.

## Completed packet: canonical Lzss linked fields

All 24 linked fields across the four structural-exact Lzss candidates resolve
to the already target-proven `g_LzssTree` owner at `0x00477858`. Their only
addends are `+0` (`parent`), `+4` (`left`), `+8` (`right`), and `+0x18008`
(`tree[0x2000].right`). The last address is four bytes inside the final
0x0C-byte node of the independently bounded `0x2001`-node / `0x1800C`-byte
array; it is not promoted as a separate global.

Two independent cold linked builds reproduce:

- `Lzss::InitTree`: **36/36 bytes**, four DIR32 fields;
- `Lzss::ContractNode`: **79/79 bytes**, eight DIR32 fields;
- `Lzss::ReplaceNode`: **117/117 bytes**, nine DIR32 fields;
- `Lzss::FindNextNode`: **49/49 bytes**, three DIR32 fields.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared field offsets, types and widths. Replaying the reviewed
tree base/addends leaves zero differences across all **281 bytes**. Together
with the normal-COFF exact `InitEncoderState`, this seam has five exact helpers
/ 323 bytes in two explicitly separate artifact contexts. The result does not
establish a production source/data TU, physical compiler owner, or product
closure. A fresh Lzss backlog probe after the promotions reports only three
mismatches and no remaining structural-exact candidate for this source.

## Completed packet: canonical PbgArchive linked fields

The remaining three structural-exact PbgArchive helpers each have one external
REL32 field. Direct hash-attested target review closes `0x00452706-0x00452717`
as an 18-byte CRT `malloc` wrapper with 94 direct call sites, and
`0x0046054E-0x004605B6` as a 105-byte CRT `_stricmp` wrapper with 11 direct
call sites. Both boundaries end at RET and meet either CC padding or the next
independent CRT function. Their behavior and surrounding CRT owners justify
library/CRT exclusion; the exact original CRT library members remain unknown.

Two independent cold canonical linked builds reproduce all three functions:

- `PbgArchive::GetEntryDecompressedSize`: **51/51 bytes**, one `__stricmp`
  REL32 at `+0x15` replayed to `0x0046054E`;
- `PbgArchive::FindEntry`: **53/53 bytes**, one `__stricmp` REL32 at `+0x18`
  replayed to `0x0046054E`;
- `PbgArchive::CopyFileName`: **47/47 bytes**, one `_malloc` REL32 at `+0x11`
  replayed to `0x00452706`.

All three complete `source.ltcg.obj` PDB contributions decode without gaps and
contain exactly their declared fields. The 151 replayed bytes have zero
differences. Together with the two zero-field pointer helpers, this shared
PbgArchive `/GL` context now has five linked-PE exact functions / 214 bytes.
The diagnostic anchor image remains non-runnable and receives no product or
physical-owner credit. A fresh Factory Ghidra operation-discovery attempt was
unavailable because the native implementation differed from its operator
binding; it returned no semantic evidence, so this packet relies on direct
target and compiler/linker evidence only. A fresh PbgArchive backlog probe after
the promotions reports only seven mismatches and no remaining structural-exact
candidate for this source.

## Completed packet: canonical LTCG exact replay

The exact-unit graph now supports `artifact_kind="linked-pe"` alongside normal
COFF. Shared `scripts/ltcg_link.py` owns the fixed cold `/GL` compile and link
context. `build-match-unit.py` validates the artifact-specific manifest and
builds one image/map/PDB set per shared context. `compare-linked-function.py`
then rebinds PE/map/PDB identity, selects one unique `source.ltcg.obj` PDB
contribution, requires its full manifest size, verifies the pinned Capstone
5.0.6 component hashes, decodes the entire extent, rejects missing or extra
linked fields, validates declared semantic symbols through map publics or the
generated unique anchor aliases, replays target addresses, and compares every
byte. `replay-exact-units.py` cold-builds and checks both artifact kinds.

Two independent cold canonical builds reproduce
`PbgArchive::SeekPastInt` at `0x00435270` across **12/12 bytes** and
`PbgArchive::SeekPastString` at `0x00435280` across **51/51 bytes**. Their
complete PDB-owned extents decode without gaps, contain zero linked fields, and
are raw-equal to all **63 target bytes**. A separate focused `/OPT:REF` test
emitted a 16-byte ordinary stack-ABI `SeekPastInt`; the declared `/OPT:NOREF`
source context is required to recover the target's 12-byte private-register
form. These two accepted claims bind only the declared bounded LTCG context.
The non-runnable anchor image is not a reconstructed product, and production
TU partition, physical owner, link order, data owners, and whole-build closure
remain unknown.

## Completed packet: linked LTCG backlog diagnostics

The repository now parses MSF 7/PDB DBI section contributions directly and joins them to VC7.1 linker-map publics and PE raw bytes. It fails closed unless the PDB RSDS GUID/age matches the PE CodeView record and map timestamp, image base, public start, and section layout all agree. `verify-toolchain.py --execute` exercises this PE/PDB extent path in addition to the existing compiler, resource, and link smoke.

`scripts/probe-ltcg-backlog.py` cold-compiles one source at a time with fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, then performs an iterative diagnostic link. Missing externals receive distinct non-runnable data anchors, allowing every linked field to retain a unique candidate-symbol identity without manufacturing executable stubs. PE HIGHLOW fields and decoded external control-flow operands are masked only for structural comparison. The harness, every result, and the report itself explicitly carry no acceptance authority.

The complete authored backlog scan covers all **66 functions across nine sources**. It found **13 structural-exact diagnostics / 715 bytes** and 53 mismatches: three Enemy lifecycle bodies, four Lzss tree helpers, five PbgArchive helpers, and `CPbgFile::GetFullFilePath`. The two PbgArchive pointer-advance helpers have no linked fields and reproduce all **63 raw bytes**. The other 11 candidates contain 34 resolved fields requiring explicit symbol/target review before any canonical linked unit can exist. Two independent cold PbgArchive passes reproduced the same result/extent/field signature. At checkpoint `26af714` all 13 remained diagnostics; later canonical packets have promoted the five PbgArchive and four Lzss candidates, leaving four structural candidates under review.

## Completed packet: batch normal-COFF backlog diagnostics

Checkpoint `d59f975` added `scripts/probe-exact-backlog.py`. It cold-compiles each selected source once under fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`, enumerates strict COFF functions, joins conservative source-name hints only when one external symbol matches, and runs every authored source backlog entry through target-bound probe comparison. Unresolved mappings, true size/byte mismatches and structural-exact results are separate states; the entire report has `acceptance_authority=none`.

The complete current run covers all **66 authored, source-present, non-exact functions across nine sources**. All 66 map uniquely to a COFF symbol and all 66 are real mismatches; there are zero unresolved and zero structural-exact results. Thus the fixed normal profile has no additional exact unit waiting for ledger-only promotion. This does not reject alternate normal profiles or LTCG ownership.

Probe relocation reporting now also handles a candidate relocation that crosses a shorter target extent: it retains the object addend, marks target encoding/candidate unavailable with `target_extent_complete=false`, and reports a normal size mismatch instead of aborting the batch. Canonical comparison remains strict. No source behavior, exact ledger row, or whole-build state changed.

## Completed packet: strict COFF extent diagnostics

Checkpoint `96f17c6` added decorated-symbol/extent/relocation enumeration and removed the unsafe probe fallback that could slice a caller-requested target length beyond the candidate COMDAT. Missing candidate bytes are now explicit differences. The corrected diagnostic reports `PbgArchive::GetEntryDecompressedSize` as a true 25-byte object against the 51-byte target, and all **20 canonical units / 700 bytes** still cold-replay exact under the strict extent rule.

## Completed packet: exact-work backlog reporting

Checkpoint `88800b6` added `scripts/report-exact-backlog.py`. It joins and validates the function, origin, source-mapping and exact ledgers, with source/module/state filters and JSON output. The 90 source mappings split into **20 canonical exact**, **66 authored non-exact functions / 26,540 bytes**, and **four origin-review Player callbacks / 38 bytes**. Progress and live status now expose the authored/origin-pending split.

## Completed packet: canonical LZSS state-reset backlog

Checkpoint `621add9` promoted the reviewed `Lzss::InitEncoderState` body at `0x00435FD0-0x00435FF9`. Two cold builds reproduced **42/42 bytes and three DIR32 relocations**: dictionary base `0x0048F868`, plus `g_LzssTree` base `0x00477858` with object addends `+4` and `+0x18010`. Ghidra still misses the standalone helper; raw target code and the encoder's inline reset establish its extent and behavior.

## Completed packet: canonical PbgArchive lifecycle backlog

Checkpoint `4ed34ee` promoted `PbgArchive::PbgArchive`, `PbgArchiveEntry::~PbgArchiveEntry`, and `PbgArchiveEntry::PbgArchiveEntry`. Two cold shared-object passes reproduced **51/51 bytes and one REL32 `free` relocation**. Fresh Ghidra review modeled only the entry destructor; raw boundaries and reviewed vector-constructor references remain the evidence for the two inventory-missed constructors.

## Completed packet: exact replay workflow and ResFile backlog

Checkpoint `9b5e7eb` promoted six `CMemoryPbgFile` methods across **210/210 bytes and four relocations** and added `scripts/replay-exact-units.py`. The tool cold-builds one shared object per source/profile and strictly compares every selected unit. The then-current 16 canonical units used three shared outputs instead of compiling the same three translation units 16 times. Manifest validation rejects malformed extents/relocations, overlapping target ranges and inconsistent output sharing; ledger validation joins canonical units to exact, source, function and origin records and requires both exact-unit sets to agree.

## Completed packet: canonical PbgFile exact backlog

Checkpoint `bd9b2e3` promoted six already reviewed PbgFile methods. Current `src/PbgFile.cpp` compiled from absent output paths twice and replayed exact across **220/220 bytes and nine DIR32 relocations**: destructor 46 bytes/three relocations, `Close` 34/one, `Read` 57/one, `Tell` 25/one, `GetSize` 21/one, and `Seek` 37/one. It also extended diagnostic probe output with target-derived relocation candidates; these remain non-authoritative until a reviewed canonical unit passes.

## Completed previous hard packet: Enemy ECL dispatcher context seam

This packet deliberately stayed on the central 14,416-byte `EnemyRuntimeView::DispatchEclInstruction` owner at `0x0040E770-0x00411FBF` instead of pivoting to easier leaf functions. It bounded opcodes `0x15D-0x164`, the nested difficulty tables, and the adjacent integer argument seam. The observable outcome was structural: recover context ownership/ABI and denominator gaps needed for a natural future dispatcher, without writing a partial `default`-return switch and falsely promoting the dispatcher to source-present.

Direct TH10 table decoding maps the eight selected opcodes to case entries `0x004118BB`, `0x004118E2`, `0x0041198B`, `0x004119FC`, `0x00411A3E`, `0x00411B23`, `0x0041009B`, and `0x004100EA`. `0x15F` uses rank interpolation with target constants `1024.0f` and `1/2048.0f`; `0x162` performs the integer analogue `(high-low)*(rank+0x400)/0x800 + low` with the target signed truncation-toward-zero correction. Difficulty cases reach five-entry tables at `0x00411F98` and `0x00411FAC`. These observations are provisional semantic/target evidence, not exactness credit.

The strongest ownership result is the ECL execution context embedded in full Enemy `+0x008..+0x101F`, exactly `0x1018` bytes, with the active-context pointer at full `+0x004`. Context `+0x004` is the current ECL instruction; its target-observed header has opcode at `+0x004`, variable-operand mask at `+0x008`, and dword operands at `+0x010`. Generic typed-operand helpers use context `+0x1008` as operand-stack state, `+0x100C` in local-storage addressing, and context `+0x1014` as the operand-resolver owner. Constructor machine code stores the full Enemy itself at full `+0x101C` = context `+0x1014`, directly binding the generic context to the already-reviewed Enemy operand vtable. Maintained `src/Enemy.hpp/.cpp` now express this exact-size context/instruction view while preserving all full-object offsets.

`0x00412A00-0x00412A0F` and `0x00412A10-0x00412A5A` are now boundary/ABI-reviewed integer argument wrappers. Both take `EnemyRuntimeView*` in EAX and argument index in ECX; the first forwards to the generic int reader and the second resolves a writable int argument address. Complete target scans find all 30 and 9 direct calls respectively inside the central dispatcher, but whether these are source-written helpers or compiler/LTCG outlines remains unknown, so origin stays `unknown/review` and neither is source-present.

Raw corridor review also found three initial-inventory misses: `0x00412A60-0x00412A6D` (14B), `0x00412A70-0x00412A7D` (14B), and `0x00412A80-0x00412A90` (17B). They are CC-delimited helper-shaped bodies adjacent to the seam, but fresh Ghidra xrefs plus complete direct-REL32 and absolute-pointer scans find no target references. They are therefore added only as new `unknown/review` candidates; authored/source/exact denominators do not increase.

Adjacent-game hypothesis provenance for this packet: clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`; clean committed TH09 HEAD `f1d4c1e332828a0cb8b9308b61b79ce77f86e4d0`; TH095 live tree was dirty (4 paths), so only committed HEAD `dee282c33dbb75ca0545d3115cd421a6e4f6a7bd` was consulted. Adjacent source supplied only ECL operand source-family hypotheses after TH10-local recovery; no adjacent addresses, opcodes, layouts, owners, or exactness were transferred.

Pinned VC7.1 SP1 build6030 compiles the refined Enemy source under fixed normal and `/GL`. A separately compiled clean-HEAD baseline and the refined normal object emit byte-identical 586-byte `EnemyConstruct` COMDAT contents with the same four relocation sites; both fresh target diagnostics are non-exact at `63/657` comparable bytes. The old `64/657` note is superseded. This is structural source feedback only, not constructor or dispatcher exactness.

The first cold canonical replay after adding the header declarations correctly failed on the int lvalue resolver because VC7.1 renumbered compiler-internal `$L...` symbols by exactly +37. Clean-HEAD versus revised COFF symbol review proved every int/float resolver jump-table/case label kept the same section offset, and the canonical relocation offsets, types and target VAs are unchanged. `config/match-units.toml` therefore refreshes only those generated object-symbol names. A second cold deletion/rebuild/replay then returns exact 6/6, 11/11, 80/80 and 80/80 for all four configured units. The failed receipt is retained as recovery evidence; exactness was not assumed across the manifest update.

## Completed hard packet: Enemy ECL operands

The packet reconstructed the four vtable-owned operand methods immediately following the previously reviewed 14,416-byte Enemy ECL dispatcher physical owner. Their boundaries/origin were already reviewed authored-game denominator entries at packet entry; this packet adds maintained source and exactness evidence rather than inflating the authored denominator.

| Address | Maintained source | Target bytes | Final state |
| --- | --- | ---: | --- |
| `0x00411FC0-0x004122F3` | `EnemyFullObjectView::ReadIntOperand` | 820 | source-present, non-exact |
| `0x00412300-0x0041234F` | `EnemyFullObjectView::ResolveIntOperand` | 80 | source-present, canonical exact |
| `0x00412350-0x004126C3` | `EnemyFullObjectView::ReadFloatOperand` | 884 | source-present, non-exact |
| `0x004126D0-0x0041271F` | `EnemyFullObjectView::ResolveFloatOperand` | 80 | source-present, canonical exact |

The reviewed full Enemy vtable at `0x0046D0C0` owns these entries in slots 1-4. Raw target review closes each `RET 4`, adjacent padding and each resolver jump table. Ghidra does not model the four vtable entries as standalone functions; its semantic view remains provisional and carries no exactness credit.

## Operand protocol and layout refinement

The two lvalue resolvers and the target spawn copy independently close the 0x20-byte block at full Enemy `+0x1138..+0x1157` / runtime `+0x0FC..+0x11B` as four writable integer ECL variables followed by four writable float ECL variables. Spawn executes `REP MOVSD` for exactly eight dwords from request `+0x20` to full Enemy `+0x1138`, so maintained `EnemyEclVariableView { int integers[4]; float floats[4]; }` is shared by `EnemyRuntimeView` and `EnemySpawnRequestView`. This refines the prior neutral eight-dword transport block without changing the reviewed object extents.

Both readers preserve the target's exact 51-selector domain `-10000..-9950`. TH10-local cases cover RNG U32/unit/signed-unit values, signed random angle scaling, current/offset/base motion values, Player position and angle-to-point helpers, update timer, runtime flag bit `0x10000`, the four int/four float variables, special-enemy coordinates, managed-VM id resolution followed by signed VM word `+0x38A`, rank, difficulty, velocity angle, life and difficulty equality tests.

The integer and float readers deliberately remain separate because TH10 contains type-dependent behavior elsewhere. Direct table entry sends integer selectors `-9996` and `-9976` to target `0x0041219A` while `ECX` still names this Enemy, so both readers expose this Enemy's world-Y and the integer form applies the target float-to-int helper. Only selector `-9962` enters through `0x00412192`, replaces `ECX` with the manager primary special-enemy pointer, and then falls through to `0x0041219A`; selector `-9963` resolves the corresponding special-enemy X directly. Maintained source preserves this target control flow rather than normalizing it from adjacent games.

TH10-local writers establish `0x00474C74` as a `0..4` difficulty index and `0x00474C98` as a signed rank-like value restored from replay and adjusted/clamped to `[-0x400,+0x400]`; maintained source uses descriptive names `g_EnemyDifficulty` and `g_EnemyRank`, not original-identifier claims. Helper evidence independently identifies RNG helpers at `0x0044B9E0/0x0044BB20/0x0044BB90`, Player angle-to-point helper `0x00426660`, float-to-int helper `0x00463B2C`, and VM id resolve-or-clear helper `0x00449450`. Source does not add a target-absent null guard around the VM lookup.

Committed adjacent source was consulted only after TH10 switch tables/storage were independently decoded. TH08 observed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean; adjacent material only corroborated the broad typed int/float/lvalue ECL source family. No selector, address, layout, ABI, owner or exactness fact was transferred. Dirty adjacent untracked content was not used.

## Compiler and exactness feedback

Final maintained `src/Enemy.cpp` compiles with pinned VC7.1 SP1 build6030 under fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`.

True final normal `/Gy` extents versus target are:

- `ReadIntOperand`: **800 / 820 bytes**; target-window diagnostic `mismatch`, **115/452 comparable bytes**, 92 object relocations, `acceptance_authority=none`.
- `ResolveIntOperand`: **80 / 80 bytes**.
- `ReadFloatOperand`: **800 / 884 bytes**; target-window diagnostic `mismatch`, **95/540 comparable bytes**, 86 object relocations, `acceptance_authority=none`.
- `ResolveFloatOperand`: **80 / 80 bytes**.

The two 80-byte lvalue resolvers each have an explicit canonical `artifact_kind="coff"` unit. Each complete owned extent carries five DIR32 relocations: one jump-table relocation plus four local case-target relocations. Current-source clean builds and repeated target-bound comparison return `result="exact"`, 80/80 bytes, with all relocation targets matching the target. These are the only new exact promotions in this packet.

The two readers remain source-present/non-exact. Successful `/GL` compilation preserves a possible production LTCG/interprocedural context but gives no standalone exactness credit.

Because all current Enemy exact units share `src/Enemy.cpp`, the final integration gate deletes/rebuilds and compares every configured exact unit after the operand edits:

- `enemy-manager-draw-callback`: exact 6/6, zero relocations;
- `enemy-ecl-dispatch-forwarder`: exact 11/11, one relocation;
- `enemy-ecl-int-lvalue-resolver`: exact 80/80, five relocations;
- `enemy-ecl-float-lvalue-resolver`: exact 80/80, five relocations.

Total repository canonical exact coverage after the Enemy packet was **4 functions / 177 bytes**.

## Ledger and verification planes

Current ledger:

- candidates: **1269**
- origin/boundary pending: **1130**
- authored: **125 / 53,138 bytes**
- excluded: **14**
- source-present: **97 / 27,489 bytes**
- canonical exact: **39 / 1,555 bytes**

Current ECL packet delta from `ead21f9`: **+6 functions / +140 exact bytes** and seven source mappings. The authored source backlog is now **54 functions**. Session exact delta from `444eb1a...` is **+35 functions / +1,378 bytes**.

**Source presence:** seven ECL lifecycle/resource functions became source-present.

**Exactness:** repository canonical exact coverage is 39 functions / 1,555 bytes. The complete mixed set cold-replays through nine artifact builds across five source files. This exact lane does not establish production object ownership or whole-build closure.

**Whole build:** actual final `python3 scripts/build.py` returned **RC2 / explicitly open**. Production compiler flags, TU partition, libraries, resources and link order remain unknown. `build.py --check`, tracking, toolchain execution and public CI pass this honest open state.

**Runtime:** not performed. There is no closed faithful reconstructed Windows-i386 product to execute.

**Factory Truth:** no Factory replay or acceptance submission was performed by this session. Repository canonical exact results and Factory accepted facts remain separate states.

## Scratch and recovery artifacts

The overall recovery/resume entry inventory earlier in this conversation was **365 regular files / 4,218,280 logical bytes / 0 files >64 MiB**. At the clean `2053a39e...` hard-packet boundary, `.analysis/` contained exactly **319 regular files / 3,709,138 logical bytes / 4,464 KiB allocated / 0 files >64 MiB**. Legacy/unknown analysis, other campaigns, Ghidra state, toolchains, Wine state and the private target were preserved.

The hard campaign `.analysis/gpt-web/20260914-enemy-dispatcher-hard/` exits with **28 files / 257,221 bytes**. It retains the manifest, bounded dispatch/table/helper disassemblies, top-level and nested table maps, clean-HEAD baseline source/object evidence, final normal and `/GL` objects, constructor/read-int/read-float diagnostics, local-label shift receipt, exact replay receipts (including the intentional failed pre-refresh receipt), and whole-build logs. Superseded current-packet `Enemy.context.*` scratch and reproducible `build/match/*.obj` outputs were removed only after final receipts existed.

The previous Web checkpoint's final `.analysis/` inventory was **347 regular files / 3,966,359 logical bytes / 4,796 KiB allocated / 0 files >64 MiB**. No legacy/unknown artifact was bulk-deleted, and the ignored private `resources/th10.exe` was not modified, moved, staged or committed.

The linked-diagnostic, canonical replay, PbgArchive, Lzss, final-structural, backlog-ranking, and current ECL lifecycle campaigns each retain one compact manifest; full JSON reports, objects, linked images, maps and PDBs are reproducible below ignored `build/`. No copied target was created.

## Next hard frontier

Continue the ECL core before returning to the general ranked backlog. The immediate bounded seam is the six-slot base host vtable at `0x0046D0D8`, its constructor/destructor family at `0x0040C710/0x0040C780/0x0040C7B0`, and the default typed operand methods at `0x0040C5E0/0x0040C690/0x0040C6A0/0x0040C6B0/0x0040C6C0`. Model the base/derived relationship only after the target-local object layout and all Ghidra-missed boundaries are entered into the denominator. The 71-byte reset is the closest connected non-exact source experiment.

After the reviewed backlog is exhausted, return to the central Enemy dispatcher with a second opcode cohort that shares the typed execution context and argument wrappers. Keep `0x0040E770-0x00411FBF` source-absent until a complete maintainable switch representation is defensible. The real production link graph remains a major infrastructure gap; canonical bounded linked-image units do not close it.

The adjacent `0x00412AA0` and later ECL helper corridor is also relevant as ABI/owner context, but smaller size alone is not a reason to abandon the central owner. The three newly discovered unreferenced `0x00412A60/70/80` candidates require origin evidence before any authored promotion.

The exact-reconstruction campaign remains active/incomplete. Faithful Windows-i386 whole-build closure, runtime validation, Factory acceptance, semantic reconstruction and portability remain open.
