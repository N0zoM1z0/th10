# GUI creation family — 2026-10-25

## Result and scope

**283 new authored exact bytes / two complete owners**: factory `414830` (136)
and **Initialize**, not constructor, `413980` (147). Separate corrected isolated,
capture-corrected and canonical cold links reproduce both complete extents,
25 fields and six fixed local targets. The allocation EH binding independently
checks its handler/action, saved allocation, scalar delete, FuncInfo and unwind
map; generated EH support gets no separate authored credit.

Complete new authored source is **572 bytes / three owners**. `LoadData413A20`
remains 289/289 nonexact, with all16 fields and six fixed locals explicit.
Actual constructor `413810` is independently **origin-indeterminate**: 367 target
bytes versus525 emitted; all23 fields, no local branches. Its six arrays/43 VMs,
ID members and timer are constructed naturally before full memset, flags and
global publication. Existing ID constructor stores are deliberately retained;
there is no constructor exact/authored credit. Existing destructor remains parked.
All four preexisting reviewed boundaries/origins are unchanged: denominator +0.
Authored exact **77034/275055,424 owners**; authored source **634/252003**.

## Experiments and evidence

- First invocation stopped on misspelled shorthand `Preload`; original error,
  source snapshot and exit1 are retained. Only the actual member spelling
  `PreloadAnm` was corrected before the first completed image.
- First complete graph: factory136 exact; Initialize147 differs at logger
  receiver/cleanup; LoadData289 remains nonexact; ctor525 remains nonexact.
  Original post-link JSON Path-key failure/exit1 was retained and the existing
  image read without a duplicate native run.
- One real dependency correction replaces the existing ANM logger facade with
  maintained `th10::GameErrorContext::Log` plus its TU. This naturally restores
  target ECX and four-byte caller cleanup; Initialize147 becomes exact.
- Independent raw review identifies filename capture at413AD6 before buffer
  clear413ADA and stage read413AAC before spell stores413AB2/AB8. One isolated
  capture model fixes both. LoadData still differs in register/store scheduling;
  unsupported spelling/register/profile trials are parked.
- Separate canonical cold source graph passes both exact owners and **30 whole
  guards /3738 bytes**: all28 original ScriptViewer lifecycle/manager guards
  (including Create67, InitializeManagers101, LoadThread16, Update1251), plus
  same-image Preload81 and actual Log. Original recipes/artifacts remain intact.

The actual root is ScriptViewerCreate → LoadThread → InitializeManagers →
ScriptViewerCreateGui. Preserve its proven14 TUs, `/GL /GS /EHs` and existing
Enemy `/GL`-only override; append actual FileSystem/PbgArchive/Logger dependencies
for17 TUs. No new ABI/noinline/profile control. One CPU, nice10, bounded serial
native runs, frozen input hashes and explicit terminal logs throughout.

See [complete field/local inventory](inventory.json),
[canonical full proof](canonical-compare.json),
[validation summary](validation-summary.json), and
[retained artifact hashes](retained-artifacts.json). `first.patch`, `logger.patch`
and `captures.patch` preserve complete isolated models against8703e417.
Raw Ghidra receipt is retained in `.analysis/gui-creation-20261025/ghidra-raw.json`.
Field roles bind actual source bases/addends, especially stage-state474C40+4/+28;
neighboring four-byte diagnostic anchors are not runtime storage owners.

## Validation and limits

`test_th10_gui_creation_contracts.py` executes the extracted complete bodies with
explicit dependency seams, finite failures/publications/chain/timer cases, live
filename/stage alias witnesses, and five rejected source mutations. Legacy GUI
fixtures have explicit setup-only constructor seams; their tested bodies are
unchanged. Host tests are not target ABI, null-new execution, SEH, game runtime,
Windows product closure, or portability proof. Full CI result is recorded in
validation-summary.json when terminal; no phase-gate promotion follows.

Final validation: all117 full-CI stages pass, terminal0 verified12:55:01 UTC;
all2683 frozen inputs (including protected9 and prior retained artifacts) unchanged.
The first CI stopped at stage78 because the laser-line host fixture also needed
a GUI setup-only constructor seam; original exit1/freeze2676 is retained. Header
include-graph audit finds no transitive Gui.hpp consumers; laser-type1 only holds
a pointer and its focused compile passes. One documented laser setup seam fixes
the failure without production changes or disabled tests. Separate corrected
full CI is green; MCP timeouts were recovered without duplicate executions.
Only result documentation/hashes changed afterward.
