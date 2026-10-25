# Separate Spt Test script loader: bounded native pilot, 2026-10-25

**No new exact or maintained-source credit.** Complete first source is407/261;
the unchanged-source actual filesystem context is261/261 but nonexact. Same
extent is not equality. Callback16 is diagnostic exact-shaped, not admitted
source or authored credit. Totals remain416 authored exact owners,
75782/273658 bytes. No source, canonical recipe, boundary or origin ledger changes.

## Target and source model

Verified original Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Raw load42BE20..42BF24 is261 dense code bytes endingRET, then11 CC bytes.
Its global callback42BE10..42BE1F is16 bytes: saveESI, load[477844], call
load, return0 and restoreESI. No context argument or fabricated update caller.
The real update loads callback address42BE10 into EDI at42C1BA, then calls
44C1C0 at42C1C5.
The newly observed pair remains outside the ledger pending separate admission;
load behavior supports authored origin, callback authored-versus-outlined origin
remains indeterminate. Exact-shaped code alone cannot decide origin.

Source uses the existing SptTestView, active EnemyEclHostBaseView virtual scalar
delete at slot+14, CRTfree452422, actual FileSystem::OpenFile44B360, and existing
concrete EnemyEclResourceBaseView deriving from EclVmScriptDatabase. The original
1463-byte concrete declaration/assertions are moved byte-for-byte from candidate
Enemy.cpp to a shared candidate header; no duplicate class or method definition.
Production files are untouched. Original class/TU and runtime safety are unknown.

The260-byte local path receives sprintf("%s", selected script name) before
cleanup. Preserve duplicate activeVm clearing, conditional scriptData free/clear,
load result publication, unchecked new1098, virtual AddScriptData slot0, count
reload and zero selection clamped to count-1 only for negative nonzero count,
state1 and return0. No bounds/error repair, noinline, forced ABI, padding, copied
code or extra volatile is added. Existing constructor volatile tail stores remain.

**Constructor prose correction:** existing functions.csv40C830 describes vtable
installation after clearing. Raw34 and inline42BEB1..CC instead install46D0F0
first, zero tail1090/1094, then memset all1098 bytes, erasing that vptr. The next
load operation dereferences it through virtual slot0. The proposal deliberately
preserves this observation; a dormant bug is only a hypothesis, not a runtime
conclusion. Vtable46D0F0 slots are450220 AddScriptData and40C810 LoadPackage;
cleanup42BDD0 additionally corroborates this concrete layout. Existing exact
constructor bytes/metrics are unchanged; this note corrects interpretation only.

## Complete native observations

Both jobs are serial, oneCPU nice10, bounded and exit0. Every source input and
all nine protected D3D evidence files remain hash-identical. Pinned VC7.1 `/GL
/GS /EHs` and actual global callback root are unchanged between runs.

| Model | Real source graph | Load | Callback | Required full guards |
|---|---|---:|---:|---|
| First | SptTest + AnmManager + FileSystem |407/261 rejected extent|16/all2 diagnostic exact|Draw531/all32, ASCII100/all4, constructor34/all1 exact|
| Actual filesystem context | above + unchanged Main + PbgArchive |261/261 nonexact|16/all2 diagnostic exact|same665 bytes/all37 exact|

First naturally specializes/inlines archive-only OpenFile, generating24 fields
and13 local branches. Existing Main CalcExecutableChecksum supplies a real mode1
caller; the canonical filesystem graph hypothesis recovers a distinct external
OpenFile call with natural filename-EAX ABI. The complete second loader has all8
fields and all6 fixed local destinations, but constructor tail-store scheduling
and count/zero register lifetime still differ. Its OpenFile358/360 independently
rejects; no successful canonical360 replay is claimed for this context.
Draw callback becomes5 bytes in this root context; it is not a guard or new claim.
Original canonical Draw/callback recipes and proofs remain untouched.

[Native summary](native-summary.json) and [role bindings](role-field-bindings.json)
retain full owner/field/local scope. Target PE relocation absence does not mean
zero absolute fields: supplemental inventory validates decoded absolute operands
against explicit target bindings (load8 and callback2). Candidate first's direct
anchored Win32 calls are not invented target thunks: their target import-slot
roles are recorded, direct entry unknown. Other first24 fields and every second8
field have explicit symbol base/addend identity. No nearest-anchor aliases.
The non-runnable unresolved-symbol image is never executed or runtime-validated.
Neither owner has an owned switch table or EH handler; complete decoded extents
endRET and padding stays excluded. Constructor null and erased-vptr failures stay.

No independent evidence justifies another scheduling/register/profile or source
spelling panel. Park. Reopen only with new TH10 constructor/capture/lifetime or
actual compiler-context evidence and a falsifiable whole-owner prediction.

## Reproduction and evidence retention

`prepare.py` materializes all source at the original isolated `.analysis` path
from checkpointcb894e0 and performs the byte-preserving shared-header extraction.
`first.py` and `context.py` retain exact build recipes, freeze inputs and refuse
to overwrite existing output. Copy producers to distinct paths/output directories
for a new run; never replace the frozen first proof. `compare.py` derives complete
PE/PDB extents, retains all branches/fields and strictly replays declared guards.
The first target observer omitted stripped DIR32 records; separate full-field
logs record its correction, not an absence of target fields or new native run.
[Retained hashes](retained-artifacts.json) bind original builds and diagnostics.

Independent review and final focused validation are recorded below. No new full
CI or runtime claim is made: production sources and canonical recipes did not
change. Prior full-CI scopes remain unchanged.

Independent read-only review confirms the natural source, byte-preserved concrete
class move, complete raw boundaries, all8 load/all2 callback bindings, all6 fixed
locals in the second image, and all three665-byte guards. The second loader
remains nonexact; callback remains diagnostic only. No EH state owner beyond
the /GS cookie was observed. Source/origin/whole-build/runtime gates stay closed.

Final focused checks pass: `final-check.py` replays both retained image contracts,
confirms59 retained artifact hashes and234 frozen inputs per image unchanged;
toolchain declarations, tracking, progress regeneration and whitespace pass.
Progress markdown/SVG are byte-unchanged. Details: [validation](validation-summary.json).
