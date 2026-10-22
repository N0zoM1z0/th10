# TH10 reconstruction handoff

Updated 2026-10-22. Exact reconstruction remains **active-incomplete**. Latest
ANM common-tail and source-ABI controls complete five actual hypotheses;
maintained source and exact totals remain unchanged. Authored exact409/74716,
overall1250/141226.
Source remains613/247945 owned bytes; nonexact204, absent121.
Archive source15/15, exact15/15;
FileSystem remains source13/13, exact11/13.
See [ANM common-tail/ABI evidence](evidence/anm-common-tail-abi-controls/README.md),
[ANM branch controls](evidence/anm-operand-branch-controls/README.md),
[resource load proof](evidence/pbgarchive-load-from-resource/README.md),
[header proof](evidence/pbgarchive-parse-header/README.md),
[entry reader proof](evidence/pbgarchive-read-decompress-entry/README.md),
[registration proof](evidence/filesystem-load-archive/README.md),
[archive load proof](evidence/pbgarchive-load/README.md),
[read proof](evidence/filesystem-path-read/README.md),
[size/EH proof](evidence/filesystem-path-size/README.md),
[lookup proof](evidence/filesystem-find-archive/README.md) and the preceding
[archive routing](evidence/filesystem-archive-routing/README.md) source checkpoint.

## Resume safely

- Original Japanese v1.00a only, ignored `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md),
  inspect status/diffs and recover dirty work before editing.
- Preserve nine independent untracked `docs/evidence/d3d-adapter-format/` files.
  Their hashes are checked; never stage or clean them with this batch.
- Use `scripts/repo-python`. Operator preference: direct local tools, no Factory
  MCP; native builds serial nice10; Ghidra readOnly/noanalysis/rollback, one CPU,
  768 MiB heap. No subagents without explicit authorization.
- Full mapped text401000..465A80 inclusive is412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  Older64A80-byte hashes cover only a truncated prefix.
- Authorized English `gpt-6.1-sol: ...` checkpoints are committed and pushed to
  `origin/main` after validation. Replay only necessary affected units.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current source and exact progress

[Generated progress](PROGRESS.md):1776 reviewed candidates,734 authored functions/
273579 owned bytes;1097 source mappings. Authored source613/247945 bytes,
204 source-present/nonexact,121 source-absent. Authored exact **409/74716 bytes**;
overall exact1250/141226 bytes. Unknown origins524 keep the whole-game denominator
unresolved. Boundary/origin/source/exact/product/runtime remain separate claims.

## Latest bounded ANM common-tail and ABI checkpoint

Starting pushed/remote-verifiedca13d55, preflight passes target/toolchain/tracking.
All108 maintained sources,32 large-source/header and15 baseline native files
remain unchanged. The initial373 source/test/config inputs are unchanged until
the deliberate ANM ledger-note qualification; only that note changes before
the final373-input CI snapshot. Keep9 unrelated untracked D3D files unstaged.

Fresh direct readOnly/noanalysis Ghidra reviews the existing entry/incoming
references, FlipX52 and entire common-tail1441. Rollback/maxcpu1/heap768 MiB;
all412289 mapped text bytes remain unchanged. No provisional signature, body
or original source ABI is promoted. Actual loop backedges/forward JMP establish
target960+13skipped+468=1441 versus baseline960+12skipped+468=1440. This explains
span length only; all skipped fill remains owned/compared. The initial early-RET
locator was invalid and superseded. Navigation omitting address fields and only
the measured fill gives372 instructions each and one slow-Tick FLD/FADD pair
difference, without target equality credit. FlipX target EAX XOR5 versus native
EDX XOR6 explains its52/53 bytes.

Three actual one-ANM-GL/nine-reused-object controls keep9964/7541of7995/90of92/
gap2. Raw compound FlipX still changes4 ordinary radial Y/Z bytes despite equal
case scores. Scale-initialized timer accumulation is raw-neutral; TU-local
postfix forwarding is full-body baseline-equal after291 independently identified
changed address fields are replayed. All3 pass35 whole guards4424/all118 and
8 bounded child paths, with ECL7020/6993/27/189/62/171 preserved.

Two distinct private32-file source overlays each cold-compile all10 game-source
TUs, serial nice10 with GL/GS and existing profiles; only2 pinned harness files
are reused. Default static emits9948/6770of7995/four plain RETs, still stack VM.
Ordinary member emits9964/7541of7995/four RET4s and is whole-body baseline-equal
after287 changed address fields are independently replayed. These are distinct
hypotheses, not a same-candidate acceptance pair. Each passes35 base guards and
the complete ECL diagnostic; the8 child-path checks are not run for these2.

The additional complete canonical caller43E7E0/194/all3 passes for baseline and
the first3 controls, but rejects default-static197 and member200 at actual PDB
extents. Both interfaces are rejected. TH09 member versus provisional TH095
static remains inconclusive hypothesis material; current static-stdcall is a
maintained boundary view, not proven original membership/source convention.
No maintained source, header, signature, recipe, match or origin/boundary state
changes. Only the ANM ledger note is qualified; no new exact/case credit.

Compact scope and results: [packet](evidence/anm-common-tail-abi-controls/README.md).
Private recovery: `.analysis/gpt-6.1-sol/anm-common-tail-20261022/RECOVERY.md`.
Hash-checked cleanup removes50 files/3129383 logical bytes:3 superseded private
native carriers and2 superseded locator reports. Retain both distinct10-source
ABI carriers/their32-file source views, baseline32/15, compact producers/reports,
corrected loop evidence and9 independent D3D files. All111 final CI groups pass
with actual exit0 and all373 final source/test/config inputs unchanged, including
108 maintained source and32/15 baseline pins. Focused diagnosis, toolchain,
tracking, source gate, progress, staged documentation/public-tree and whitespace
checks pass. See the packet validation summary. Historical private producers
pin pre-note inputs; never restore current source or notes to rerun them.

Next rotate away from compound flags, slow-accumulator/postfix forwarding and
default-static/member hypotheses. Investigate a distinct49/53/54 operand-lifetime
or linked-context route, preserving the full194-byte caller as an extra guard.
Whole Windows i386 product/runtime stays open; semantic/port stages unstarted.

## Previous bounded ANM operand-form checkpoint

Starting pushed/remote-verifiede19a98f, all108 maintained source/header files,
373 source/test/config inputs,32 large-source/header pins,15 baseline artifacts
and9 independent D3D files are verified. Fresh direct readOnly/noanalysis Ghidra
reviews the existing ANM entry/incoming references,49/53/54 and final timer,
with rollback/maxcpu1/heap768 MiB and all412289 mapped bytes unchanged.
Ghidra's provisional signature and body ranges remain unpromoted.

Three actual one-ANM-GL/nine-reused-source-object controls: conditional stores
10012/7066of8001, literal-first value selection9964/7439of7995 and ordinary inline
helper9836/7277of7999. Same-sized groups86/90/87 of92; aggregate gaps70/2/130.
The inline hint leaves8 actual helper calls. None is adopted; baseline remains
9964/7541of7995/90of92/gap2. This is a bounded source-form plateau, not an
exhausted function, unique-original-source proof or a blocker.

Each image passes35 complete linked guards4424/all118 and8 bounded child paths.
Five guards originate from separate canonical COFF contracts, unchanged. ECL
remains7020/6993/all189/62 with complete171-byte callee exact;7019 decoded plus
the reviewed one-byte selector covers the full owner despite the generic false
normalization flag. All3 whole ECL raw bodies equal baseline. Enemy14352 versus
14416: literal-first raw-equal; the other two have only changed CALL fields to
independently identical map callees. This is baseline preservation only.

TH08/TH09/provisional TH095 source reads support the variable-first component
form as hypothesis material only. No cross-game facts or original TU/flags,
data ownership, whole product/runtime or semantic/port credit is promoted.
The [branch-control packet](evidence/anm-operand-branch-controls/README.md)
contains complete scope, source patches and final validation. Private recovery:
`.analysis/gpt-6.1-sol/anm-branch-stores-20261022/RECOVERY.md`.
Hash-checked cleanup removes32 files/2083152 logical bytes in two superseded
carriers. Keep the canonical baseline, first10012-byte failure, compact producers/
sources/reports and9 independent D3D files. Current409/74716/1250 remains.
Fresh111-group CI exits0 with all373 final inputs,108 maintained sources and
32/15 baseline pins unchanged. Focused case diagnosis, toolchain/tracking,
generated progress, documentation links and whitespace complete successfully;
the case diagnosis remains noncanonical and mismatching.
Next investigate a distinct common-tail2 lifetime or global register-allocation
context; do not repeat these three or prior raw-input/named-result/helper/timer
forms. Source and exact gates remain independent of the open product/runtime.

## Previous complete resource archive load checkpoint

Starting pushed/remote-verified73fbf4c, fresh local Ghidra reviews the existing
100-byte retained body and vtable reference, with no incoming reference observed.
ReadOnly/noanalysis/rollback, maxcpu1/heap768 MiB; all412289 mapped text bytes
remain unchanged. Complete target decode independently covers36 instructions,
five local branches and all6 fields:5 REL32 calls and1 DIR32 resource vtable.
The original PE has no base relocations; decoded operands and semantic target
observations establish its fields. Original callers remain unknown.

The old normal-COFF ResFile context emits161 with base-constructor/EH handling;
the separate Ob2/Gz context emits280. One actual source-unchanged ResFile GL
support compile, reusing3 source objects, recovers100/all6 and9 whole existing
guards956/all52. It exposes the existing constructor to optimization. This is
one measured hypothesis, not original production flags or physical TU proof.

Two independent actual full4TU allGL/GS/EHsc canonical colds, archiveOb1, each
reproduce new100/all6 and9 whole guards956/all52:1056/all58 total. All1249 prior
canonical dictionaries remain unchanged. Source/header/interface/layout/ABI and
all13 old-context files are unchanged. Only100 new bytes count; constructors,
callees and vtable data owners gain no credit. Archive now15/15 authored exact
under its separately declared contexts, without a whole-product claim.

Keep first actual cold
`build/gpt-6.1-sol/archive-resource-load-20261022/accepted-cold-1` and second
`build/match-linked/PbgArchiveResourceLoad`, plus compact producers/results.
Private recovery: `.analysis/gpt-6.1-sol/archive-resource-load-20261022/RECOVERY.md`.
Hash-checked cleanup removes11 files/187410 logical bytes: the superseded
one-support carrier and rejected empty-argument preflight log. The successful
complete attestation, actual colds, old-context proofs and9 D3D files remain.
The32 large-source/header and15 native baseline hashes freshly pass only;
no new large build or comparison is claimed. All111 final CI groups pass with
exit0 and all373 source/test/config inputs unchanged throughout. Final focused
100/all6, toolchain/tracking, Archive source gate, generated progress,
documentation links and whitespace pass. Final validation status:
[resource load validation](evidence/pbgarchive-load-from-resource/validation-summary.json).
Whole Windows i386 product/runtime and semantic/port gates stay open.

## Previous complete archive header checkpoint

Starting pushed/verified62e6cd1, target observations correct ParseHeader seek to
literal FILE_BEGIN=0, guard temporary frees on failure and place backend-null
assignment after the conditional delete. Existing header/interface/layout/global
declarations remain unchanged. The original mode-array/data-owner or constant-
folding explanation is unknown. The preceding fresh local Ghidra/full412289-byte
attestation covers this complete350-byte target and its caller context.

Four actual one-support/three-reused controls: literal origin351, late state351
(neutral), guarded cleanup351 and identical corrected source under archiveOb2
350/all10 exact. The first three retain10 existing whole functions1237/all62.
Instruction routing points to an extra receiver push in the Ob1 context; it is
not an equality proof. Ob2 emits the actual existing allocator327/all10 exact
under the supported allocation-EH address binder. No static/header experiment
is executed and no helper ownership or duplicate327 credit is inferred.

Two actual independent full4TU canonical colds pass350/all10 plus8 complete
guards1230/all48, totaling1580/all58. The new Ob2/Gz context does not reproduce
GetSize263,Load231,registration35; those are explicitly outside its guard success.
Original recipes stay unchanged. Corrected maintained source hash equals the
old Ob1 current-source control, retaining10/1237/all62 with1 support recompiled,
not a new full Ob1 cold. Original-profile Load108 separately cold-passes.
All1248 prior canonical dictionaries remain unchanged. Only350 new bytes count.

Keep first new cold
`build/gpt-6.1-sol/archive-header-context-20261022/accepted-cold-1`, second at
`build/match-linked/PbgArchiveParseHeader`, old-context preservation and compact
producers/results. Previous reader original-context13 files are archived under
its prior-original-profile path before Load's original recipe overwrites the
shared output. Private recovery:
`.analysis/gpt-6.1-sol/archive-header-context-20261022/RECOVERY.md`.
All111 CI groups complete with the final success marker and the observed native
process terminated; all373 frozen source/test/config inputs remain unchanged.
The original exec handle was lost during recovery, so no direct CI exit status
is claimed. Hash-checked cleanup removes40 files/647797 logical bytes in four
superseded control carriers. Both actual colds and9 independent D3D files remain.
Final focused comparison, toolchain/tracking, Archive source gate, generated
progress, documentation links and whitespace pass. Final checks and cleanup are in the
[header validation](evidence/pbgarchive-parse-header/validation-summary.json).
The different compiler contexts do not close the whole production graph.
Source/TU/production flags/data owners/runtime and semantic/port gates stay open.

## Previous complete archive entry reader checkpoint

Starting b5fc8a9, fresh local readOnly/noanalysis Ghidra reviews both transforms,
all6 decrypt/3 encrypt recorded call sites and two archive callers, with all
412289 mapped bytes unchanged before/after rollback,maxcpu1/heap768. AL seed,
five stack arguments and RET20 are target machine facts, not original source
annotations. Supplementary untyped address surveys establish no data owners.

A consistent four-TU /Gz control retains GL/GS/EHsc, archiveOb1 and normal ResFile.
Transforms become286/RET20 versus targets323/307 and remain nonexact. The reader
becomes281 bytes but fails exhaustive field inventory9 versus10. Direct indexed
profile arguments restore all10 fields, leaving35 ordinary byte differences.
Initializing the checksum after filename length recovers all281 bytes/all10.
The latter two controls each recompile only archive support/reuse3 pinned source
objects. Late-buffer and combined-lifetime forms are declared but unexecuted.
The accepted source changes no signature/header/layout/global or inline control.

Two actual independent full4TU canonical colds pass281/all10 plus9 existing whole
guards956/all52, totaling1237/all62 each. Separate affected original-profile
path Read214/GetSize190 cold passes404/all27, preserving existing recipes.
All1247 prior canonical dictionaries remain unchanged. Only281 new authored
bytes are credited; decrypt/decompress callees and profile data gain no credit.
Original source/TU/flags/class and product/runtime remain unknown.

Keep new first cold at
`build/gpt-6.1-sol/filesystem-transform-context-20261022/accepted-cold-1` and second
at `build/match-linked/PbgArchiveReadDecompressEntry`. The different complete /Gz
build group uses its own output. The previous original-profile canonical13 files
are archived at the registration packet's cold-2 path before the affected replay;
current `FileSystemPathSize` now contains that fresh original-profile replay.
Private recovery: `.analysis/gpt-6.1-sol/filesystem-transform-context-20261022/RECOVERY.md`.
Fresh111-group CI passes with all373 final code/test/config inputs unchanged.
Hash-checked cleanup removes33 files/493754 logical bytes in three superseded
private control carriers; actual colds and compact proofs remain. The32 large
source/header and15 native baseline hashes remain unchanged; no large replay
is claimed. Final validation and cleanup are recorded in the
[reader packet](evidence/pbgarchive-read-decompress-entry/validation-summary.json).
Old pre-adoption producer pins are historical: never restore source or rerun
them blindly. Preserve9 independent D3D files and all independent phase gates.

## Previous complete archive registration checkpoint

Starting pushed/remote-verified fdbe829, a normal success-first LoadArchive
branch replaces the previous early failure return. The private one-primary/
three-reused probe passes36/all4 plus eight whole guards920/all48; two other
declared forms are not executed. No signature/header/global/layout/profile or
ABI forcing is introduced. Maintained source exactly adopts that private form.

Two independent actual four-TU canonical colds each pass36/all4 and eight
existing whole guards920/all48, totaling956/all52. All1246 prior canonical unit
dictionaries remain unchanged. Target raw review independently covers all11
instructions and four fields, with unchanged boundary/origin rows. The earlier
fresh local Ghidra review attests all412289 mapped bytes under readOnly/noanalysis
rollback;4357D0 still has no Ghidra function record and none is manufactured.
Only36 new authored bytes are credited. The preceding39 and34-byte diagnostics
remain historical, not limits on possible natural source forms.

Keep first actual cold
`build/gpt-6.1-sol/filesystem-load-archive-20261022/cold-1` and second canonical
`build/match-linked/FileSystemPathSize`. Prior archive108 second cold is archived
and pinned at its private cold-2 path before replacement, preserving both old
archive108 and Read pairs. Compact private recovery is
`.analysis/gpt-6.1-sol/filesystem-load-archive-20261022/RECOVERY.md`.
Fresh111-group CI passes with all373 final source/test/config inputs unchanged.
Hash-checked cleanup removes12 files/1642902 logical bytes: the superseded
private one-input carrier and two duplicate ledger snapshots. Both current
colds, old Read/archive108 pairs and9 independent D3D files remain. Fresh32/15
large input/artifact hashes pass only; no large build/comparison is claimed.
Final CI and cleanup are recorded in
[validation](evidence/filesystem-load-archive/validation-summary.json).
Product/runtime, original names/TU/flags and semantic/port gates remain open.

## Previous complete archive load checkpoint

Starting pushed/remote-verified cd0038f, the existing natural PbgArchive::Load
source reproduces the complete434C30..434C9B/108 owner in the current path graph.
Two pinned Read carriers first supply cached comparisons; two subsequent actual
four-TU canonical colds independently pass108/all7 and seven whole same-image
guards812/all41, totaling920/all48 each. All1245 prior canonical unit dictionaries
remain unchanged. Source, headers, ABI declarations, profiles and infrastructure
are unchanged. Explicit pdb_source binds the archive support contribution.

Fresh local readOnly/noanalysis Ghidra reviews the loader/callees and attests
all412289 mapped text bytes unchanged before/after rollback, maxcpu1/heap768.
Startup's recorded call42011F preloads ECX497990/EAX th10.dat. Independently
decoded4357DF also calls Load, despite its missing Ghidra function/xref record.
Never infer that the recorded startup xref is the only target call. Seven fields
are independently bound: two Release calls, allocation, parser, filename copy,
vtable and mode0. Callee/data ownership and runtime receive no duplicate credit.

Initial separate-output preflight rejects the identical build group before any
compilation. The accepted unit shares FileSystemPathSize as required. Prior
Read cold2 is archived/pinned at its private build path before overwriting the
canonical carrier; both historical Read colds remain. Keep current archive
cold1 at `build/gpt-6.1-sol/archive-load-exact-20261022/cold-1` and cold2 in the
shared canonical directory, plus compact producers/results. Private recovery:
`.analysis/gpt-6.1-sol/archive-load-exact-20261022/RECOVERY.md`.
Fresh111-group CI passes with373 frozen inputs unchanged during execution.
Afterward only unrelated optional quoting in two CSV files is restored to HEAD
format; all1776 function rows and1246 match rows are exactly equal as parsed,
with the other371 inputs unchanged. Final tracking/progress/Oracle checks pass;
the format-only proof and final snapshot are recorded in
[validation](evidence/pbgarchive-load/validation-summary.json).
Only108 new authored bytes are credited; broader product/semantic/port gates
remain open.

## Previous complete read checkpoint

Starting9cd7b17, five actual controls isolate ordinary result lifetime. G7,
direct suffix expression, archive-first declarations and a real normal-caller
link entry leave Read214/200/all14. One result shared by archive/disk branches
recovers all214 bytes and14 fields, including the existing stack-file EH binder.
Only this natural source form and three host mutation selectors are adopted;
signatures/layouts/globals/profiles remain unchanged. Host112/eight compiled
strict controls pass. Private experiments reuse attested objects; they are not
full cold evidence.

Two actual independent four-TU canonical builds each reproduce Read214 and
existing GetSize190/all27 fields, plus five complete guards408/all14, totaling
812/all41. A separate original-recipe FindArchive cold also passes212/all7.
All1244 previous unit dictionaries remain unchanged. Only214 new authored bytes
are credited. The Ghidra review is readOnly/noanalysis/rollback, maxcpu1/heap768,
and independently checks all412289 mapped text bytes before/after. No generated
EH-code, metadata owner, original class/TU/flags or product/runtime claim follows.

Fresh111-group CI passes with all373 final code/test/config inputs unchanged.
Hash-verified cleanup removes50 files/808687 logical bytes across five obsolete
private model carriers. Both actual colds and all9 independent D3D files remain.
The32 large-source and15 native baseline pins remain unchanged; no new large
build or comparison is claimed. Final CI and cleanup details are recorded in
the [read validation](evidence/filesystem-path-read/validation-summary.json).
Private recovery: `.analysis/gpt-6.1-sol/filesystem-read-registers-20261022/`.
Keep the actual first cold at its matching build path and the second canonical
carrier `build/match-linked/FileSystemPathSize`, compact source/producers/results
and the affected FindArchive canonical carrier. Old manifest/model pins describe
historical pre-adoption inputs; never restore source or rerun producers blindly.
Preserve all9 independent untracked D3D files.

## Previous size and stack-file EH checkpoint

Starting5e8f6f3, ten private models recover single-call Read and the complete
GetSize shape. Read/GetSize consume the existing suffix helper; FindEntry's
unchanged implementation becomes an ordinary inline header definition. The new
EHsc recipe retains GL and uses GL/Ob1 archive support plus normal ResFile.
No layout, virtual, global, signature, forced ABI or target byte changes.

The explicit stack-file binder joins a unique parent-named MAP entry to its
complete same-parent18-byte PDB region, validates EBP-0x18/destructor tail and
immutable one-state FuncInfo. An inherited extra-word assumption is corrected:
the supported header is28 bytes, with native Read ending at the readonly-section
boundary. GetSize's following word belongs to Read's UnwindMap. Four positives
each and45 stack/37 allocation rejections pass. Cached replay-manager-load181
still passes under the corrected reader; this is not a current-source cold.

The two initial colds are superseded. Both final actual four-TU colds pass
GetSize190/all13 and five additional whole guards408/all14. Existing canonical
FindArchive212 and archive53/51 independently cold-pass316/all9 in unchanged
recipes. All1243 previous unit dictionaries are unchanged. Only190 new bytes
are credited; EH-code/metadata owners and native runtime remain unknown.
Routing112/eight strict controls pass after updating the suffix-argument mutant.
Fresh111-group CI passes with all373 code/test/config inputs unchanged after
the corrected28-byte protocol and EOF-clean header. Earlier cold pairs and the
interrupted EOF CI are superseded. Classified cleanup removes1999267 logical
bytes; both final carriers and all9 independent D3D files remain unchanged.

Before the subsequent shared-result checkpoint, Read was214/200 with all14
fields/EH bound and14 ordinary register/return-order differences. Its older EHs
context was204; do not mix those measurements.
The ten-TU32 source/header and15 native pins freshly remain unchanged; no large
owner cold or comparison is claimed. Old broad source inventories are historical.

Private recovery: `.analysis/gpt-6.1-sol/filesystem-wrapper-codegen-20261022/`.
Keep first final cold in
`build/gpt-6.1-sol/filesystem-wrapper-codegen-20261022/accepted-cold-1` and second
at `build/match-linked/FileSystemPathSize`, compact source/producers/results and
the affected canonical carriers. Earlier model inputs are frozen historical
states: never restore maintained source or blindly rerun their producers.
The [size packet](evidence/filesystem-path-size/README.md) records validation.

## Previous exact archive lookup checkpoint

Starting5b539f9 dirty work is recovered: the ordinary suffix helper and pointer
iteration in FileSystem.cpp, plus its host selector/control update. Seven actual
private controls culminate in212/all7 zero-difference; they reuse three support
objects and are not full colds. No unexecuted planned model receives evidence.

Canonical `filesystem-find-archive` then independently cold-compiles all four
actual source inputs twice: GL/GS/EHs FileSystem, PbgArchive, PbgFile and the
target-supported normal-COFF ResFile caller. Both complete212-byte PDB owners and
all7 semantic fields pass; each actual image also retains caller46/all2. The
whole extent includes loop alignment and the final MOV/JMP, not only through a
RET. Both candidate literals and target independently contain five-byte `.dat`.
Only one212-byte exact gain is credited; the inline helper and indeterminate
4356D0 owner gain no source/origin/exact claim. Original class/TU/profile/data
owners and product/runtime remain unknown. Read/GetSize bodies remain unchanged,
with retained236/242 versus214/190. No broad historical or large-owner replay.

Private recovery is `.analysis/gpt-6.1-sol/filesystem-codegen-20261022/`.
Keep first actual cold `build/gpt-6.1-sol/filesystem-codegen-20261022/cold-1` and
second canonical `build/match-linked/FileSystemArchiveLookup`, source/context
pins, producers, compact controls and the preceding routing baseline. Cleanup
removes70 files/1150942 logical bytes in seven superseded private build trees.
All9 independent D3D files and other canonical/provider/legacy state stay intact.
Fresh110-group CI passes with all371 code/test/config inputs unchanged. Its
actual-source routing group passes112 assertions/eight compiled strict controls.
Current canonical PDB owners independently confirm212/236/242; the latter two
remain nonexact. Toolchain execution/check, tracking, progress, FileSystem source
gate and whitespace pass. Validation is recorded in the
[lookup packet](evidence/filesystem-find-archive/README.md).

## Previous FileSystem source checkpoint

Target4356F0/212,435800/214,4358E0/190 now map to ArchiveOrDiskPathView
FindArchive/Read/GetSize. The physical path-call declaration moves from
ResFile.cpp to a shared header; PbgArchive gains a nonvirtual read-only filename
accessor. Existing fields, virtuals and archive globals are retained. These are
descriptive boundary views, not original class/TU, object lifetime or data owners.

Lookup replaces the first slash and suffix with `.dat`, or compares a slash-free
path unchanged, then case-sensitively scans16-byte active archive rows. Read/size
use the first-slash suffix or original path. Matching archive failures do not
fall back to disk. The optional buffer is forwarded only to archives. Disk Open
returns are ignored; size/read and real CPbgFile destruction still run. Mode0
is the existing `r` pointer. Current source preserves these target observations.

Fresh local read-only/noanalysis Ghidra reviews all3 extents and attests412289
mapped bytes unchanged before/after rollback. Ghidra lacks a436400 caller record;
use raw46-byte context and its independent exact normal-COFF proof, not an
invented Ghidra boundary. No private metadata edits are saved.

Two actual four-TU builds succeed, first allGL then with the already exact
normal-COFF ResFile caller. New helper extents220/236/242 remain nonexact versus
212/214/190; positional field guesses are not semantic bindings. Three selected
canonical units cold-replay113/all3: memory Open46, archive constructor16 and
entry size51. An additional same-image normal caller passes46/all2 fields.
No broad replay of unchanged consumers or new exact unit is claimed.

Actual source passes112 ASan/UBSan assertions and8 compiled functional controls;
dependencies are explicit host seams, without native class/lifetime/Windows or
engine-runtime proof. Existing statistics-save229 scenarios/128 masks pass.
All110 CI groups pass with371 source/test/config inputs unchanged. Current108
native inputs are pinned. Older broad107 inventories below are historical;
do not restore source to satisfy them. The large ten-TU32 source/header and15
artifact pins freshly remain unchanged; no new large-owner comparison is claimed.

Private recovery: `.analysis/gpt-6.1-sol/filesystem-archive-routing-20261022/RECOVERY.md`.
Retain its normal-caller carrier, current producers/input pins and compact
evidence. Clean only identified superseded current outputs; preserve nine
independent D3D files, canonical carriers and provider/legacy state.

The [insertion proof](evidence/chain-insertion-native/README.md) closes complete
Calc449AE0..449B62 and Draw449B70..449BF2 owners131 each, densely decoded through
RET4. The source reuses existing supervisor CS[0]/unsigned-byte counter[0] views;
no new global, type, signature, algorithm, volatile or padding. Physical ESI element,
EDI signed priority and stack owner remain. Added callbacks use ECX argument before
locking, retain their result and are cleared; new equal-priority elements precede
old ones. Original source class/TU/profile and physical storage aliases are unknown.

The earlier [runner proof](evidence/chain-dispatcher-native/README.md), committed
and pushed as10798f4, closes Calc449C00..449D33/308 and Draw449D40..449E4B/268,
including alignment and14 local table words. That576-byte gain is separate from
the prior62-byte table ownership correction and current262-byte insertion gain.

Both runners cache next before callbacks; null callbacks are uncounted, disabled
nonnull callbacks(mask0x2, bit1) counted. Callbacks run unlocked with ECX argument.
0 unregisters only;2 retries with enabled-mask recheck;3/4/5 force1/0/-1 and break.
Calc6 resets/restarts;7 invokes an optional locked delete callback without implicit
unregister. Draw6/7 and out-of-range values are defaults. Arbitrary mutation,
invalid-pointer success and Windows concurrency are outside the established scope.

The [construction review](evidence/chain-source-closure/README.md) retains two
0x24-byte heads in the0x48-byte owner, partial ownership-bit initialization and
untouched argument words. Constructor449AA0/59 stays origin-indeterminate and
nonexact; implicit versus explicit outer construction is unknown. AllocElem74
still emits69 in its reviewed context. The491BE4 physical g_MainCallbackOwner/
g_Chain alias and production data owner remain open.

## Prior scoped checkpoints and recovery

The [ECL layout packet](evidence/ecl-run-layout-controls/README.md) revalidates the
entire ten-TU enemy-drop-item-counts recipe,32 source/header and15 native pins
against the previous actual cold. All107 maintained native source/header files
remain byte-identical. No new full cold is performed. Fresh local readOnly/
noanalysis Ghidra reviews12 residual spans and HostRun callers with all412289
mapped bytes unchanged before/after; transaction rollback/maxcpu1/heap768 MiB.

Complete Run is7020/6993 with189 bound fields/62 local destinations and the
complete171-byte ReadFloatValue exact. Its27 differences are4 thread-register,
5 commuted-addressing,16 stack-home and2 common-latch register bytes. All60
jump words and88 selectors are checked. Generic decoding7019 plus the explicit
one-byte selector suffix is complete coverage despite the generic flag.

Sixteen actual controls retain the same entire target-replayed Run hash
830db1c50fc4f1228d0bfbf99f08c51e9d9e36043990d2900820c8b55600a472.
These cover wider scopes, scalar scratch sharing, named math destinations,
Run-definition order, cursor reference, EHs/G6/G7/GS-off profiles(retaining GL),
support order and real/external entries. Twelve compile only ECL/reuse9 verified
objects; four link all10 reused objects. Fifteen pass nine selected whole ECL
same-image guards1137/all18. HostRun as external entry produces151 versus146
and that guard batch is rejected; no full nine-guard success is claimed for it.
All tested neutral forms remain candidates, with no original-source/type/flag/
TU/ownership inference. The earlier11 thread controls also remain historical.

The maintained diagnostic now selects one explicit existing repository-local
ECL input compiled exactly once by the chosen context. Default src/EclVm.cpp
behavior remains. Its stable observed source hash is not producer attestation.
Canonical bindings, complete callee,189/62 and all data rules remain unchanged.
Actual default/private CLI comparisons retain the complete27 differences; a
private context without the selector is rejected. Three valid/eight invalid
source selections and16 existing table/selector controls pass. Historical
private copies are bytewise proven to alter only two source paths and ROOT;
new work uses the maintained option. Old producers froze pre-routing inputs:
do not blindly rerun them or restore historical source.

The previous Chain insertion batch independently passed two actual14-TU colds:
new262/all14 fields plus first-cold current runners576/all44, four units838.
Its retained carriers and compact proofs keep their source/context scope. Current
game source, headers, canonical recipes and exact counts are unchanged, so no
new Chain cold, host1375/eight controls/19 EOL replay or large35-guard replay is
claimed in this ECL batch. Full CI below includes those host checks.

All109 full CI groups pass with all369 frozen source/test/config inputs unchanged.
Classified cleanup removes27654818 logical file bytes across29 owned
trees and95 superseded files. The canonical baseline, one private API
positive and one actual HostRun extent failure remain. Results:
[validation](evidence/ecl-run-layout-controls/validation-summary.json).
Private recovery: `.analysis/gpt-6.1-sol/ecl-run-register-pressure-20261022/RECOVERY.md`
and manifest.json. Keep the unchanged canonical ten-TU baseline, one private
reference-cursor source/API carrier and the HostRun151/146 extent failure;
retain compact source transformations and reports. Remove duplicate neutral
outputs and superseded private diagnostic copies after preserving facts.
Nine independent D3D files and provider/legacy unknown state remain untouched.

Global boundary metadata drift remains open:1116 rows,49 state and107 confidence
differences, zero geometry differences. First failure401100 provenance. Unowned
indirect tables16. Ordinary tracking/CI does not waive this separate gate.

## Large owners and next route

| Owner / target | Current retained measurement | Remaining work |
| --- | --- | --- |
|ANM43EE30 /9964|9964 bytes;7541/7995 aligned positions;90/92 target-sized groups; aggregate gap2|Child91, temporaries49/53/54, FlipX61, common-tail2 and Tick order|
|ECL44E1A0 /7020|7020/6993,27 differences;189 fields/62 locals;171-byte float callee exact|Thread argument, SIB, homes and latch|
|Enemy40E770 /14416|14352 bytes; frame2C4;181 selectors/108 groups;42 target-sized groups; aggregate gap566|Laser ABI, shared tails and packet/register/stack lifetime|

The current ECL scope/storage/order/profile family is a measured plateau, not
proof that Run is exhausted. The subsequent child/flag controls below also
reach a bounded plateau; rotate to ANM groups49/53/54 or common-tail2, using
the retained target-bound32/15 inputs and strict local destinations. Keep
ECL27 open; do not repeat these16 neutral forms or
substitute the old two-TU26 result. Original data owners, source types, names,
physical TU and production flags remain unknown despite close output.

The subsequent ANM child/flag batch freshly revalidates all32/15 baseline pins,
107 native inputs and369 code/test/config inputs. Local read-only Ghidra
checks all412289 mapped bytes with rollback. Child91 default construction then
assignment creates an extra copy and grows47->55. Explicit script/layer locals
are whole-ANM raw-neutral. Two FlipX bitfield orders are identical across all
three large bodies to each other and grow FlipX61 versus target52/current53.
Their diagnostic totals regress to7309/8001 and7338/8000 respectively; baseline
remains7541/7995. All four pass35 whole guards4424/all118 and Run189/62 with the
exact171-byte callee; no full native cold or separate ANM host campaign is claimed.
Five normal-COFF canonical contracts supply only additional linked guards here.
One stale cancel-record mapping label in the old diagnostic template is refreshed
from current canonical metadata; extent and fields are unchanged.

Private recovery: `.analysis/gpt-6.1-sol/anm-child-tail-20261022/RECOVERY.md`.
Retain the canonical baseline, compact transformations, producer and one actual
child91-assignment failure; remove duplicate neutral/regressing carriers after
recording their facts. All109 CI groups pass with369 frozen code/test/config
inputs unchanged before/after. Cleanup removes6427081 logical bytes from33
owned paths; target/toolchain, tracking, progress and whitespace checks pass.
Nine independent D3D files remain untouched. Next groups49/53 use target
EDX/EAX/ECX argument MOV/PUSH carriers versus native ECX/EDX/EAX;54 uses the
first two corresponding registers (6/6/4 ordinary differences, equal sizes).
Earlier named float result locals were neutral. Investigate a distinct lifetime
or common-tail hypothesis without repeating those locals or these four forms.
This is a bounded plateau, not
an exhausted function or a blocker. Current authored401/73225 remains unchanged.

The [operand lifetime controls](evidence/anm-operand-lifetime-controls/README.md)
subsequently execute three distinct input/helper/timer forms. Input snapshots
grow ANM10076, helper calls shrink it9836, and an explicit timer tail reproduces
all three complete baseline large bodies raw. Each image retains35 whole
guards4424/all118, eight child paths and full ECL27/189/62/171. No source is
adopted. The current369 frozen code/test/config inputs are identical to the
preceding109-group CI run; the ANM documentation checkpoint484f421 reused that
result. The final Chain batch now independently reruns all109 groups successfully
with the same369 inputs unchanged.
Private recovery: `.analysis/gpt-6.1-sol/anm-argument-carriers-20261022/RECOVERY.md`.

Coverage now rotates to Chain allocator449ED0/74 versus current69. Target tail
loads flags into ECX, ORs1 and stores after the callback write; current native
uses a memory OR, accounting for five bytes. Constructor AND scheduling also
differs. Private flags-snapshot, ordinary callback-helper and untouched-source
G7 controls remain69 while all four existing Chain guards838/all58 pass.
These are candidates, not new exact units or proof of original compiler flags.
The direct-tools campaign is `.analysis/gpt-6.1-sol/chain-allocation-codegen-20261022/`;
use its live process handles and current results to recover, not the state label.

The final allocation packet includes five actual controls: flags snapshot, G7,
callback helper and compound heap bit remain69, while Og disabled grows161 and
rejects all four guards. Ignoring only the sole observed CALL field3..6, every
optimized69-byte body is identical to the retained69-byte baseline. The helper's
raw difference is a changed linked owner/CALL displacement, not ordinary code.
No allocator comparison passes its74-byte PDB extent, and no normal-COFF
substitution or original flag/TU inference is made. Full read-only local Ghidra
attests412289 mapped bytes before/after and reviews the allocator/constructor.
All107 native inputs and23 original snapshot files remain unchanged. Full109
CI, target/toolchain, tracking, generated progress and whitespace checks pass;
all369 code/test/config inputs remain unchanged. Cleanup removes5338060 logical
bytes from21 owned paths, retaining compound-bit69 and Og-disabled161 failures.
The ANM batch separately removed4289158 bytes from22 owned paths. Canonical
carriers, prior snapshots, nine independent D3D files and provider/legacy remain.

The three path owners and registration wrapper now have complete canonical
contracts: lookup212, Read214, size190 and LoadArchive36. Remaining FileSystem
nonexact owners are Decrypt323 and Encrypt307, totaling630 bytes; use the origin
ledger's subsystem fallback when counting, since neither has a function-row
module. Inspect current target/caller and operand-lifetime evidence before a
transform experiment. Alternatively rotate to the large-owner routes above
without repeating neutral models. Keep /GL and do not force registers or
returns. The old File.cpp string still has no code xref and establishes no
original source ownership.

The original-profile transform contributions were284 each/RET, versus target
Decrypt323/Encrypt307/RET20. The subsequent consistent /Gz probe produces286 each
with RET20 and keeps9 whole guards956/all52 exact; current reader acceptance
retains that context. This establishes one reproduced cleanup context, not the
original convention/TU/flags or transform exactness. Former ParseHeader355 and
private351 measurements are superseded by its complete350/all10 context above.
Continue TH10-local transform/caller investigation without repeating the measured
/Gz/reference/direct-field/late-checksum family. Archive's15 reviewed authored
owners now have exact contracts under their declared contexts. Continue either
the two FileSystem transforms or a distinct large-owner lifetime hypothesis;
source, physical ownership and product/runtime gates remain independent.
Existing allocator327 stays a normal-COFF canonical claim even though a full
linked guard matches it here. Neither surface proves production ownership.
TH09/TH095
headers/implementation suggest ordinary data-first signatures only; they do not
prove TH10 types/flags/ABI. TH095 provisional packed locals and unused writes
are not TH10 evidence and must not be imported as padding. Compact scope/results
are in the current transform-context private directory; older PDB extents retain
their historical source/profile scope.

PbgArchive::Load434C30 retains its complete108/all7 canonical contract. Its two
cached comparisons in the preceding Read packet were routing evidence, not a
new cold or gain in that earlier batch. The subsequent36 source/acceptance batch
is now adopted and independently verified above. Historical Read34 and older39
wrapper extents are not current measurements. Private model/acceptance manifests
freeze their own historical inputs; never restore source or rerun blindly.

The remaining source-absent queue is121 owners:118 GameUnassigned,2 Player and
1 Startup. Neither FileSystem source completeness nor its host contracts close
the broader objective. Continue bounded exact/source recovery and preserve
all independent phase gates.

Whole Windows i386 product/runtime closure is open: globals, static init,
resources, libraries, link order and exercised paths still need audit. Semantic
reconstruction and portability have not started. The broad goal remains active.
