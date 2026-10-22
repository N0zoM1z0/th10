# TH10 reconstruction handoff

Updated 2026-10-22. Exact reconstruction remains **active-incomplete**. Complete
FileSystem GetSize4358E0 is now exact190/all13 fields after two final independent
full four-TU colds. Authored exact403/73627, overall1244/140137. Source remains
613/247945 owned bytes; nonexact210, absent121. FileSystem source13/13, exact9/13.
See [size/EH proof](evidence/filesystem-path-size/README.md),
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
210 source-present/nonexact,121 source-absent. Authored exact **403/73627 bytes**;
overall exact1244/140137 bytes. Unknown origins524 keep the whole-game denominator
unresolved. Boundary/origin/source/exact/product/runtime remain separate claims.

## Latest size and stack-file EH checkpoint

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

Current Read is214/200 with all14 fields/EH bound, leaving14 ordinary register/
return-order differences. Its EHs context is204; do not mix those measurements.
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

The three FileSystem owners above are source-present. FindArchive212 and
GetSize190 now have complete canonical exact contracts. Next investigate Read's
remaining14 ordinary register/return-order differences in its current214-byte
EHsc/Ob1 caller graph, preserving all14 fields and the six whole guards. Its
archive call and cleanup inlining are recovered; do not repeat the ten executed
models or treat the EHs204 measurement as this context. Use target-supported
ordinary-source/caller hypotheses with complete field/EH binding before credit;
keep /GL and do not force registers or returns. The old File.cpp string has
no code xref and still does not establish original source ownership.

The remaining source-absent queue is121 owners:118 GameUnassigned,2 Player and
1 Startup. Neither FileSystem source completeness nor its host contracts close
the broader objective. Continue bounded exact/source recovery and preserve
all independent phase gates.

Whole Windows i386 product/runtime closure is open: globals, static init,
resources, libraries, link order and exercised paths still need audit. Semantic
reconstruction and portability have not started. The broad goal remains active.
