# Hint lifecycle source contracts

## Bounded contribution

This proposal adds the genuine constructor (418D40/237), initializer
(418E30/131), terminal cleanup (418EE0/346), no-argument factory (419090/67),
registered calc adapter (4198A0/10), and registered draw adapter (4198B0/6).
It builds on the maintained loader, exporter, ConsumeEntries (419120), and
Update (419650). The calc callback calls that real Update; the draw callback's
return 1 is the complete observed target contribution. The separate unregistered
419890 return-1 contribution is not substituted for the registered address.

Methods use natural C++ interfaces. The optimized EDX constructor and EBX
initializer observations do not justify dummy parameters, private-ABI forcing,
assembly, or optimization tricks. Cleanup is explicitly stdcall and terminal:
it does not delete the owner, reset chain pointers, or repair freed sentinel
links. There is deliberately no automatic C++ destructor that would cause
unrequested cleanup of stack test owners or a second cleanup after the explicit
factory failure branch. The separately observed scalar-deleting-shaped
4190E0/419100 contributions and the larger destruction owner417C80 remain open.

The factory uses ordinary new/delete like the target owner allocation. Its
nonzero-initializer branch retains the native owner-null guard around cleanup
and delete (4190B8/BA), while the Initialize call stays unconditional. The
branch is retained naturally, without fabricated address
escapes to defeat optimization. Maintained Initialize always returns zero, so
an optimizing compiler may remove cleanup/export from a factory-root graph.
That is a diagnostic outcome, not a reason to distort the source. Original
allocation-failure behavior is not host-test evidence: the target calls its
initializer even after null allocation, whereas modern throwing new may throw.
No null-owner, failed-chain-allocation, or exception path is executed by fixtures.

## Payload and ownership identity

HintLinkView::payload is void*: list sentinel payloads are the HintOwnerView
object, whereas real entry-node payloads are HintEntryView objects. Each actual
entry traversal casts only real nodes reached from sentinel.next. Constructor
and clone entry self-pointers remain intact. No sentinel is passed off as an
entry. x86 owner size remains 0x1A8 with flags+0, unknown+4, chain pointers+8/+C,
counters+10/+14, eight loaded sentinels+18 and eight export sentinels+78. Existing
frame fields and entry size/offset checks are retained; the additional chain
field offsets are checked. Host pointer sizes are not x86 layout validation.

The single declaration for target477814 is now
HintOwnerView *g_GameOwner477814 in Hint.hpp. GameManager no longer declares an
incompatible GameChainOwnerView pointer or the generic GameCreateStageOwner
factory. Its actual startup edge calls CreateHintOwner. Its chain-flag action
accesses the real HintOwnerView chain fields, not a cast to an unrelated prefix.
No new global storage definition is introduced.

## Canonical dependency bindings

- th10::g_Chain:491BE4; Chain::AllocElem/AddToCalcChain/AddToDrawChain and
  UnregisterElem are reused. Convenience RegisterCalc/RegisterDraw set flag2,
  contrary to Hint initialization, and are not used.
- g_MainSupervisorView:491C28; hintMode+142=491D6A;
  criticalSections[0]+64C=492274; criticalSectionLockCounts[0]+6F4=49231C.
  Main.hpp precedes Hint.hpp in Hint.cpp so its Windows-import shielding applies.
- Existing Chain.cpp aliases th10::g_CriticalSections and th10::g_ChainNestCounter
  must bind to that SAME Main storage, at +64C and +6F4. They are not new objects.
  Read-only src/config/scripts audit at7c845941 confirms the existing manifest
  maps these decorated aliases to492274 and49231C. GameErrorContext also uses
  the same lock-array symbol with addend48. This proposal deliberately does
  not alter the accepted Chain bodies.
- g_AnmRenderManagerView:491C10; actual manager MarkVmForDeletion and its FindVm
  implement the existing two-list lookup/root-child deletion semantics.
- g_GameOwner477814:477814 keeps its basename but changes C++ pointer type;
  the audit found no existing src/config/scripts manifest binding for that
  owner or generic factory. Future native C++ decorated-name bindings must use
  the corrected type. No second
  independent Hint global is permitted.
- CreateHintOwner replaces GameCreateStageOwner at the genuine startup edge
  corresponding to CALL417AE5->419090. Existing mapping that only knows the old
  generic factory must be reconciled, not left as a parallel definition.

Cleanup snapshots g_Chain separately for each chain before entering the lock,
then enters/increments/unregisters/leaves/decrements. It snapshots the ANM
manager once after both chains, skips zero IDs, and clears each ID after marking
it even if no VM was found. Mode2 export precedes list freeing; clearing the
published owner is last. No synchronization ordering or pointer clearing is
silently repaired.

## Evidence and limits

The authoritative observations for this proposal are the complete saved native
core decodes and callback/factory decode, plus the original bounded lifecycle
plan. The target identity was audited there. Source tests establish bounded
portable contracts with instrumented dependencies; they do not execute target
machine code, establish runtime registration ABI, prove physical globals,
recover original class names/TU ownership, or confer codegen/whole-image credit.
Canonical replay still owns exactness. A native factory diagnostic should report
all linked extents and actual global/REL32 targets, including whether cleanup
and exporter survived optimization, rather than reporting similarity alone.

## Regression gates

Run existing test-hint-load.py, test-hint-export.py, test-hint-frame.py, and
test-hint-frame-arithmetic.py against this tree, at both ordinary and optimized
host configurations supported by their runners. Preserve every prior mutation
control. The fixture changes must use actual constructor extraction and correct
object lifetimes; copying a temporary owner leaves stale sentinel self-pointers
and is not an acceptable reset strategy.

Run the lifecycle runner's unchanged baseline and every strict intended-CHECK
mutation. Compile failure, sanitizer output, timeout, signal, or the wrong CHECK
must be reported as a harness failure, never as a mutation kill. Test dependency
substitutions remain confined to isolated fixtures, including the explicit
failed-initializer seam needed to exercise the otherwise unreachable factory
failure arm. No seam may become a production or diagnostic contribution.

Affected wider regressions: GameManagerInitialize startup factory sequencing and
GameManager reset chain flags; Chain registration/removal units using the shared
Main lock aliases; ANM FindVm/MarkVmForDeletion units. Their unchanged source
should be exercised with the corrected Hint declaration and updated fixture
bindings. Run any source provenance/lint gate that validates GameManager's
shared-global declarations. Do not run unrelated compiler-option sweeps.

## Local validation of this proposal

The new lifecycle baseline and all65 intended-CHECK controls pass at O0/O1 with
ASan/UBSan. Independent review also passes all65 with Python -O and C++ O2 under
ASan/UBSan. The source integration/null-guard audit is reported separately.
The original22 loader,12 exporter,49 frame controls all pass at O0/O2, preserving
their prior sanitizer settings and failure classifiers. The unchanged arithmetic
model passes normal Python and Python -O. A separate32-bit host syntax-only
check passes the actual header's layout assertions with calling-convention
macros erased; this establishes no native calling-convention evidence.

After generating the CRLF-preserving patch against byte-identical7c845941
baselines, git apply --check and application succeeded, all12 files matched
proposal bytes, and all four suite baselines passed again from the applied tree.
No target runtime or exactness claim follows from these checks.


## Native integration and canonical migration (2026-10-16)

The actual factory entry compiled using Hint.cpp and ordered support
FileSystem.cpp, Chain.cpp, AnmManager.cpp, AnmVmCreate.cpp, AnmVmId.cpp,
RandomMath.cpp and EnemyEclDispatcher.cpp, with the pinned normal LTCG profile.
Actual i386 headers and layout assertions passed. Main.hpp's Windows SDK
defines small as a macro; the scale-category local was renamed smallScale,
with corresponding mutation anchors. This compatibility fix changes no logic.

Diagnostic PE SHA-256:
0358f6da0a09cdc1998cfa92932a4316568698249e1b79fb30201e598febe6af.
Owned sizes: constructor 197 versus target 237, Initialize 131 versus 131,
cleanup 222 versus 346, factory 67 versus 67, update adapter 7 versus 10,
draw callback 6 versus 6, ConsumeEntries 1222 versus 1317, Update 541 versus
571. Equal sizes alone are not matches. In particular Initialize remains
nonexact: the compiler uses a private loader path-argument convention.
No forced ABI or caller-retention device was introduced.

Canonical hint-create-owner covers the complete 67-byte factory. Its five
REL32 fields at 0x07, 0x15, 0x20, 0x2E and 0x34 bind the reviewed allocation,
constructor, initialization, cleanup and release targets respectively.
Two independent cold links replay all 67 bytes exactly. This does not make
the nonexact callees, allocation-failure runtime, or whole lifecycle exact.

Canonical hint-draw-callback covers the existing six-byte constant-return
function, now retained through the real registration address escape at
0x00418E5A. This replaces game-decomphelp-fulltu-004198b0 and its old
isolated-leaf mapping; it adds no six-byte credit. The old leaf body remains
a noncanonical historical candidate, while the unique active target mapping
is HintOwnerView::OnDraw. Its original authored/generated origin remains
indeterminate. The constructor and update adapter likewise retain their
preexisting indeterminate origin.

The shared factory producer's two cold PE hashes are:
- 9f62553cdc246cfcf576672566d5e846b3697dcbb7c719f9daf9100b5385b8b3
- e9aac874936673d2c6b5545a8bfb658e55afdd1f7a8457380fc9d45cf89cf8eb

Both units replay 73 bytes together. Factory acceptance is checked separately
against the final committed snapshot; local replay alone is not an accepted
receipt. All three affected previous canonical units also remain exact:
GameManager draw 31, GameManager update callback 7, and hint list cleanup 79,
for 117 bytes of regression coverage.

The parent-integrated source passes all 65 lifecycle controls and all 49
frame controls with strict intended-CHECK evidence. All 22 loader and 12 exporter controls also pass against this source, as
does lightweight public CI. The optimized-Python/C++ O2 lifecycle baseline
passes as a separate check. No original game
execution or physical alias closure is implied: Main and Chain declarations
retain explicit same-target-address obligations, while the nonruntime
diagnostic's unresolved-data anchors are not real shared runtime storage.
