# Exact Chain frame dispatchers

Reviewed2026-10-22 from `4c4c98a`. Both maintained frame dispatchers are now
canonical exact: **Calc308 bytes/23 fields and Draw268 bytes/21 fields**. Two
independent complete14-TU historical-compiler cold builds reproduce every owned
byte and all44 linked fields. This adds **two authored exact functions/576 bytes**.
[Evidence](evidence.json) records the complete second replay, first-cold snapshot,
source models and independent target-derived field declarations.

Only original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, is used.
Read-only/noanalysis Ghidra review uses rollback, one CPU and768 MiB heap. Full
mapped text401000..465A80 inclusive,412289 bytes, retains SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`
before and after review. Counter references establish byte accesses; they do
not establish original signedness, volatility, class ownership or source names.

## Source model and compiler controls

The existing TH10 MainSupervisor view already models seven critical sections
at+64Ch and seven unsigned byte counters at+6F4h, from base491C28. Thus the first
lock and counter are492274 and49231C, the same target addresses previously
modeled by separate Chain globals. The adopted dispatchers access these existing
members directly. Other Chain helpers and their scalar declarations are retained;
no new global definition or type change is introduced. Source case4 precedes
case3, reproducing the target return-block/table ordering.

| Private model | Complete Calc / Draw sizes | Outcome |
| --- | --- | --- |
|Inline scalar lock wrappers|316 /272|Does not recover target sizes|
|Include Main.hpp without changing scalar accesses|316 /272|Header inclusion alone does not recover sizes|
|Existing MainSupervisor member accesses|308 /268|Sizes recover; wrong3/4 local table destinations still rejected|
|Indexed member-based lock wrappers|308 /268|Same unresolved destination ordering; extra wrappers unnecessary|
|Member accesses plus4-before3 source order|308 /268|Strict complete replay exact; adopted|
|Signed-byte scalar hypothesis|No linked image|Source compiled, cached unsigned-symbol anchor response lacks the new signed decorated name; no signedness/codegen conclusion|

Each linked private model recompiles one Chain object and reuses13 hash-verified
game objects; none is called a full cold. The TH09 wrapper source is hypothesis
material only. Actual TH10 offsets, ABI, target instructions and compiler output
establish the adopted bounded claim. The winning model preserves the existing
unsigned type and reproduces the target direct memory decrement and resulting
branch/alignment layout. No volatile, target-byte copy, inert padding, fabricated
return, forced private ABI or removed `/GL` is used. Byte equality does not
identify the original spelling, source class, physical TU or production flags.

## Complete target contract

The [preceding ownership review](../chain-dispatchers/README.md) establishes the
full ranges449C00..449D33/308 and449D40..449E4B/268. These include code through
RET4, internal/post-RET alignment and eight/six guarded absolute jump-table words.
All14 destinations independently land on target-decoded instruction starts in
the same owner. Target PE base relocations are absent; the field declarations
are independently derived from target instructions/tables and reviewed globals.

The canonical manifest enumerates every imported, global, callee and local field.
MainSupervisor fields bind the reviewed491C28 base plus64Ch/6F4h addends, rather
than an unrelated diagnostic-anchor name at an overlapping address. Every local
field preserves its destination offset inside the complete PDB contribution.
All table words are compared; there is no larger artificial compare extent.
The generic decoder consumes307/263 bytes including portions of table data;
complete authoritative DIR32 fields bridge the remaining suffix. This is full
owned-byte coverage, not a307/263 instruction-only claim. No comparator or
coverage rule is changed.

A negative source model keeps equal308/268 sizes but swaps return blocks and
corresponding table destinations. The unchanged strict comparator rejects its
Calc field at+120h (candidate D4h versus target DEh) and Draw at+100h (B4h versus
BEh). It cannot normalize away a different local destination. This provides a
control against granting exactness from sizes or masked structural totals alone.

The physical interface remains a stack owner/RET4 and EAX processed count, with
fastcall ECX callback arguments. Count/null/enable behavior and all status cases
remain as previously target-reviewed. Drawing6/7 remain default results; Calc6
restarts and Calc7 invokes the optional locked delete callback. Existing source
and host assertions retain these behaviors.

## Actual-source cold acceptance

The canonical context uses real ScriptViewerCreate as entry, ScriptViewer plus
13 maintained support TUs, Chain as the PDB owner, `/GL /GS /EHs` primary profile
and the existing Enemy support override. All identical build keys share the
existing canonical output directory. An initial attempt to give the new pair a
separate directory was rejected before compilation; the graph invariant remains
unchanged. It is a declared reproduced context, not original TU/profile proof.

The first actual-source full14-TU cold passes four units772 bytes: new576 and
existing Unregister114/RemoveAll82. Its complete outputs are copied before the
second independent cold. The second recompiles all14 TUs and again passes both
new complete owners576/all44. A separate affected registration cold passes
RegisterCalc43/Draw43. Thus six unique units858 bytes are validated, and only the
new576 bytes receive additional credit. The first source of truth remains the
canonical byte/field Oracle, not Ghidra or host execution.

[Validation](validation-summary.json) records host1375 ASan/UBSan assertions,
eight strict faulty-code controls and19 EOL checks. Two MainSupervisor member
views are explicitly projected onto the same existing host lock/counter roles,
exactly eight occurrences each, with counts retained in the manifest. This is a
host storage-role projection, not native MainSupervisor layout or Windows lock
execution. Only the active Chain.cpp LF pin changes; other six dependencies and
CRLF-to-LF-only rejection rules remain unchanged.

The fresh full CI passes all108 groups with all368 frozen source/test/config
inputs unchanged. Classified cleanup removes20,432,288 logical file bytes from
12 owned reproducible output trees. Both actual cold carriers, one equal-sized
rejected local-table model and compact evidence remain; nine independent D3D
files and shared provider state are preserved.

All1238 prior canonical unit definitions and the existing large32 source-input/
15 artifact pins are checked unchanged. The whole canonical manifest hash changes
only by adding the two units, so the old whole-file fingerprint is historical.
Neither changed Chain.cpp nor any other changed compiler input enters the prior
ten-TU large graph. No new large cold,35-guard replay or Run measurement is claimed.

Authored exact is now399 functions/72963 bytes; overall1240/139473. Authored source
610, nonexact backlog211, source-absent124, denominator734/273579 and unknown524
otherwise retain their prior meanings. Original data storage aliases, global
ownership/static initialization and faithful Windows i386 product/runtime remain
open. Semantic reconstruction and portability have not started. Private recovery:
`.analysis/gpt-6.1-sol/chain-counter-codegen-20261022/RECOVERY.md`.

The subsequent [insertion matches](../chain-insertion-native/README.md) change
only two other Chain bodies and refresh the host projection count to twelve.
Their first actual-source cold freshly preserves both runners, 576 bytes/all44
fields. That batch supersedes the shared second carrier; the receipts and first
snapshot above retain historical source/context scope.
