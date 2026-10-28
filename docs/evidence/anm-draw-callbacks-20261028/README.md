# ANM chain callbacks and complete manager construction, 2026-10-28

This direct-local checkpoint recovers all twenty registered ANM update/draw
callbacks and replaces the large manager constructor's unresolved Chain facades
with the observed allocations, flags and real Chain calls. It admits **585 exact
bytes**, including **379 authored bytes**. The complete **2315-byte constructor
remains nonexact** and receives no exact credit.

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The disk target, pinned VC7.1 execution and direct-local read-only Ghidra
six-sample identity checks pass. No Factory operation is used. Toolchain files
may reside in shared installed directories; invocation is through local scripts.

## New owners and origin

The callbacks occupy previously untracked code between CaptureToSurface and
UpdatePrimaryVms. Each complete extent densely decodes, ends in RET or its own
tail JMP, has one DATA reference from the2315-byte constructor, and overlaps no
existing owner. The63 inter-owner CC padding bytes are excluded. The private
Ghidra database has labels rather than function definitions at these entries;
that absence neither supplies nor rejects ownership. Raw target decode, the
constructor's callback stores and reference review establish the focused rows.

| Entry | Address | Bytes | Origin disposition |
| --- | --- | ---: | --- |
| OnUpdatePrimary | 0x4485D0 | 7 | Indeterminate |
| OnUpdateSecondary | 0x4485E0 | 7 | Indeterminate |
| DrawLayer0 | 0x4485F0 | 12 | Indeterminate |
| DrawLayer1 | 0x448600 | 15 | Indeterminate |
| DrawLayer2 | 0x448610 | 15 | Indeterminate |
| DrawLayer3 | 0x448620 | 115 | Authored, medium confidence |
| DrawLayer4 | 0x4486A0 | 15 | Indeterminate |
| DrawLayer5 | 0x4486B0 | 15 | Indeterminate |
| DrawLayer6 | 0x4486C0 | 15 | Indeterminate |
| DrawLayer7 | 0x4486D0 | 15 | Indeterminate |
| DrawLayer8 | 0x4486E0 | 15 | Indeterminate |
| DrawLayer9 | 0x4486F0 | 15 | Indeterminate |
| DrawLayer10 | 0x448700 | 15 | Indeterminate |
| DrawLayer11 | 0x448710 | 15 | Indeterminate |
| DrawLayer12 | 0x448720 | 15 | Indeterminate |
| DrawLayer13 | 0x448730 | 15 | Indeterminate |
| DrawLayer14 | 0x448740 | 48 | Authored, medium confidence |
| DrawLayer15 | 0x448770 | 72 | Authored, medium confidence |
| DrawLayer16 | 0x4487C0 | 72 | Authored, medium confidence |
| DrawLayer19 | 0x448810 | 72 | Authored, medium confidence |

Read-only Ghidra queries cover every Capstone-decoded instruction start in all
20 extents:20 constructor entry references, one internal branch and zero outside
references to interior instruction starts. This does not exclude every possible
computed runtime reference. The global `report-boundary-inventory.py
--check-ledger` fails on a preexisting evidence-text discrepancy at row3,
0x401100. Its generated inventory is retained privately; the established global
ledger is not overwritten and no global boundary-audit pass is claimed.

Five stateful callbacks have target-specific logic. Layer3 publishes Main's
first camera, calls the real0x421480 configurator, reloads the active pointer and
device, stores viewport flag0, and conditionally flushes/clears the full DWORD
fog cache before render state28=0. Layer14 clears both camera shake components
and both components in the global renderer, then calls the incoming manager.
Layers15/16/19 use Main's second screen camera and store viewport flag1. All
callbacks return the observed update/layer result. The fifteen plain adapters
could be source-written or compiler/LTCG-generated; their206 bytes stay origin
indeterminate even though their complete native contributions are exact.

## Large constructor feedback

The entire constructor0x445900-0x44620A was compared early and after each
meaningful dependency/source change. These are contribution sizes, not ordinary
byte mismatch counts:

| Model | Translation units | Native bytes | Target bytes |
| --- | ---: | ---: | ---: |
| Retained source-current Main graph before this batch | 11 | 1284 | 2315 |
| Actual callback definitions, old constructor facades | 12 | 1284 | 2315 |
| Real ChainElem allocation, flags and Chain Add calls | 12 | 1884 | 2315 |
| Actual Chain.cpp support definitions | 13 | 2408 | 2315 |
| AnmManager.cpp additionally compiled with /EHs | 13 | 2308 | 2315 |

The constructor source now allocates twenty real ChainElem objects, sets their
callback, clears added/deleted callbacks, sets the low two flag bits while
preserving other flags, stores `this`, and calls the real two calculation /
eighteen drawing registrations at the observed priorities. Target decode shows
the same allocation/initialization sequence. Existing Chain source annotations
are unchanged. TH095 supplied the historical source-family hypothesis only;
every accepted callback, offset, priority and side effect is TH10-local.

The target's EH prologue and EH array-construction calls motivate the isolated
AnmManager `/EHs` profile in this new diagnostic context. `/GL /GS` remains on;
AnmInt3Scale retains its existing `/Ob1` override. Older recipes and global
profiles are not migrated. Remaining constructor differences include EH frame
order/stack locals, a pre-clear capture-ID member constructor store, VM flag
initialization, loop pointer/counter allocation and Chain flag-store scheduling.
The diagnostic anchor for FS storage and complex constructor EH metadata do not
establish a valid exact contract. The near-equal extent grants no parent credit.
The capture-ID representation/default-construction boundary is a new concrete
lead; avoid broad ID constructor changes without caller evidence.

## Strict callback proof and recovery

The new canonical units are `anm-callback-update-primary`,
`anm-callback-update-secondary` and `anm-callback-layer-N` for the eighteen
listed layers. They share the actual WinMain13-TU graph, real `WinMain@16` entry,
pinned VC7.1 SP1 build6030 and PDB owner
`support-10-AnmDrawCallbacks.ltcg.obj`. The diagnostic unresolved-symbol data
anchors do not make a runnable game image or prove production TU ownership.

Two independently cold-compiled source sets reproduce all20 complete callback
contributions: **585 bytes/all52 fields**, with exact PDB extents and complete
field coverage. Call fields bind the reviewed0x448860/0x448900/0x448980,
0x421480 and0x442F50 entries. Absolute fields bind the known ANM global or
supervisor base0x491C28 plus reviewed member addends; incidental equal-address
anchor names are not used as semantic identities. The camera callee itself is
still nonexact and receives no collateral credit.

The second invocation compiled all13 objects successfully but failed linking
with LNK1104 because Wine crash text contaminated a `winepath` result. The
original `cold.log` and exit1 are preserved. A separate bounded recovery checks
single-line absolute Windows paths and source/object hashes, resumes linking
those same objects without recompiling them, validates PE/MAP/PDB entry identity,
and passes all20 strict contracts. It is a recovered second cold compilation,
not an originally successful cold invocation. See [results.json](results.json).

Both final images also pass six same-image existing contracts: Main Present,
ANM-surface release and screenshot service390/all22, plus ANM UpdatePrimary149,
UpdateSecondary123 and DrawLayer67/all7, totaling **729 bytes/all29 fields**.
Older contexts are not migrated and the complete prior cohort is not replayed.

## Source checks and scope

The actual callback bodies run against synthetic camera/COM observers, including
high-only cache bits, active-pointer/device replacement, incoming/global manager
separation, ordering and return propagation. Four deliberate source mutants
(byte cache, stale viewport, omitted Y clear and wrong screen flag) must fail
specific assertions. The actual constructor body runs with explicit VM,
allocation and Chain observation seams, checking4117 member constructions,
4096 Initialize calls, all20 callback identities/priorities/flags/arguments, quad
stores, caches and final shader call. Three mutants (lost flag bit1, wrong update
callback and wrong priority) must assertion-fail. These host contracts do not
prove target allocation initialization, i386 layout/ABI or native runtime.

The first focused CI attempt stops at the strict transition/Chain host input
pin because the complete AnmManager.cpp file changed. Its extracted RNG helper
is unchanged, all six other active dependencies match, and restoring only the
previous complete constructor reproduces the entire prior LF-normalized file.
The reviewed input receipt updates only that active LF pin and records the
inverse/helper checks. Historical raw receipts, host algorithms and strict
normalization/rejection rules remain intact. Final validation includes the
affected transition suite, its EOL rejection controls and actual Chain dispatcher
controls; the failed first selection remains separately recorded.

This adds20 source owners/585 bytes, of which5/379 are authored; new totals are
**77,823 authored exact bytes / 275,434 reviewed authored bytes,434 owners**.
Source-present authored owners646. New CI stages cover the two host groups;
default collection has121 stages, while only the affected explicit selection
is run here. No full-suite, whole-product, runtime, semantic-gate or port claim.

All16 selected CI stages pass, including the two new host groups, Main camera,
transition baseline,19 EOL/pin controls, eight Chain dispatcher mutations,
supervisor bindings, ledgers, source completeness, graphs, progress and
documentation/whitespace. The transition host baseline retains its documented
kind8 FP rounding gap; passing its source contract does not resolve that gap.

Reproduce the focused cold group using repeated `--unit` options for the20
canonical names; shared context is built once by `replay-exact-units.py`.
Host checks are `scripts/test-anm-draw-callbacks.py` and
`scripts/test-anm-manager-construction.py`, invoked with `scripts/repo-python`.
Keep native jobs serial, one CPU, nice10 and bounded with retained terminal logs.
Working producers/receipts are below
`.analysis/gpt-6.1-sol/anm-camera-callers-20261028/`; first final artifacts are in
`build/gpt-6.1-sol/anm-camera-callers-20261028/eh-members/`, recovered second ones
in `build/match-linked/AnmDrawCallbacks/`.
