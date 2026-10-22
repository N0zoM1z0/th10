# Archive-or-disk size and stack cleanup

2026-10-22, starting `5e8f6f3`, original Japanese TH10 v1.00a only.

The complete 190-byte GetSize owner at4358E0..43599D is now reproduced by
canonical `filesystem-path-size`. Only these 190 bytes receive new exact credit.
[Facts](facts.json) bind source, recipes, artifacts, fields and diagnostics;
[validation](validation-summary.json) records completed checks.

## Source and compiler context

Read and GetSize consume the existing ordinary first-slash suffix helper.
FindEntry's unchanged implementation moves into PbgArchive.hpp as an ordinary
inline definition; its logical compilation-TU mapping remains PbgArchive.cpp.
Fields, virtuals, globals, signatures and target bytes do not change.

The new recipe retains GL/GS and uses EHsc, with GL/Ob1 PbgArchive support and
normal-COFF ResFile support. Explicitly inline FindEntry remains available
inside the 51-byte size helper while GetSize retains its target-observed call.
These are reproduced compiler inputs, not original production flags or TU proof.
No noinline attribute, register forcing, padding or fabricated return is used.

Both final actual four-TU cold builds pass the complete owner and all13 fields.
Each image additionally passes five complete guards: FindArchive212, caller46,
GetEntryDecompressedSize51, FindEntry53 and file destructor46, totaling408/all14
fields. Separate affected canonical colds retain FindArchive212 and both archive
helpers104, totaling316/all9 in their unchanged original recipes. All1243 old
unit dictionaries remain unchanged; only one new unit is added.

## Exception entry binding and the header correction

`stack_file_eh_handler = true` is a narrow DIR32 opt-in. Its parent-named,
unaliased MAP entry must join the complete generated18-byte PDB contribution
in that same parent object/module. The eight-byte action loads ECX from
EBP-0x18 and tail-jumps to the real qualified46-byte file destructor. The
ten-byte entry selects immutable FuncInfo and dispatches CxxFrameHandler.
Both images must retain one state, next state -1 and that exact cleanup action.
Allocation/static/stack selectors cannot be mixed. This supplies address
identity, without generated-code, metadata-owner or runtime credit.

The inherited metadata reader incorrectly inspected eight words. Actual Read
FuncInfo ends exactly at its native readonly section boundary after seven
words/28 bytes. In both images, GetSize's next word belongs to Read's UnwindMap.
Both supported19930520 readers now stop at28 bytes. Each synthetic suite has
four positives, covering the section end and three unrelated neighbor values;
stack-file45 and allocation37 rejection controls pass. A cached181-byte existing
allocation-EH parent still compares exactly under the corrected reader; that is
an Oracle regression check, not a new current-source cold.

Earlier paired builds preceded the metadata correction or an EOF-only cleanup.
Acceptance here uses the final two runs with identical373-input pins matching
the checkpoint. The compiler recipes remain unchanged. Fresh111-group CI passes
with those same373 inputs unchanged. Cleanup removes1999267 logical file bytes
from proven superseded experiment/snapshot outputs; final carriers are retained.

Fresh direct local Ghidra reviews the path owners, callees, raw caller, EH
action/entry regions and metadata words with all412289 mapped bytes unchanged
before/after readOnly/noanalysis rollback, one CPU and768MiB heap. Absent
constructor, size/caller or EH function records stay raw contexts; no metadata
or target bytes are patched. Ghidra names/types remain provisional.

## Executed controls and the remaining Read difference

| Private model | Read / GetSize bytes | Result |
| --- | ---: | --- |
| Read suffix helper |204 /242|One archive-reader call; cleanup stays outlined|
| Both suffix helpers |204 /243|Lookup/caller guards retained|
| Archive Ob1 alone |204 /181|Entry-size child becomes16/51; reject|
| Class-visible destructor |204 /242|No cleanup inlining|
| Explicit Close |231 /242|Extra release flow; reject|
| All-input EHsc |214 /248|Cleanup inlines; register differences remain|
| File-only EHsc |204 /242|Callee flag alone does not recover cleanup|
| Parent/normal EHsc |214 /248|All three complete path bodies raw-identical to all-input EHsc|
| Scoped result with EHsc |214 /248|No new match|
| Inline FindEntry, archive Ob1, EHsc |214 /190|Complete GetSize candidate and existing guards pass|

These ten models have explicit compile/reuse scopes in the facts. They are
distinct from the final actual maintained-source colds. Planned but unexecuted
models receive no evidence. Adjacent TH08/TH095 destructor declarations and TH09
file code were inspected as hypotheses only; they supply no TH10 exact premise.

Read now admits a complete strict comparison:214 bytes, all14 fields including
its EH protocol bound, 200 bytes equal and14 ordinary register/return-order
differences. It remains nonexact. The routing projection's suffix control now
mutates the actual reader argument;112 ASan/UBSan assertions and eight compiled
strict controls pass. Its archive/disk seams do not prove native class lifetime,
Windows API behavior or engine runtime. Whole product closure remains open;
semantic reconstruction and portability have not started.

## Recovery

Campaign: `.analysis/gpt-6.1-sol/filesystem-wrapper-codegen-20261022/`.
Keep the first final cold in
`build/gpt-6.1-sol/filesystem-wrapper-codegen-20261022/accepted-cold-1` and second
at `build/match-linked/FileSystemPathSize`, plus compact producer/source/results
and the affected canonical carriers. Old experiment manifests freeze historical
inputs and must not cause a source rollback or a blind producer rerun.
Preserve nine independent D3D files and other provider/legacy state.
