# Scene camera initializer: bounded diagnostic

Target 0x4216F0–0x4218C0 is a complete 465-byte reviewed authored owner.
The first complete existing-type model emits **451/465**, and one grounded
real rocking-vector type proposal emits **487/465**. Both structurally reject.
No maintained source mapping, exact bytes, denominator or phase changes follow.
Authored exact remains 75782/273658, 416 owners. No new full-CI/runtime claim.

## Target and source evidence

The complete body is straight-line code with no calls, branches, local targets,
owned tables or internal alignment. RET is at4218C0; the following15 CC bytes
are outside this owner. Its sole decoded direct caller is4201CB inside the
186-byte startup owner4201B0. Both original compiler context and original TU
are unknown. The ledger source mapping is blank, but historical semantic dump
`DecomphelpLeafAccessors.cpp:Fn004216F0` exists; this is not a claim of no prior
source definition.

The existing AnmFloat3View constructor naturally retains the repeated XYZ
stack writes and reads; neither model manufactures them. Existing
StageCameraStateView is118h bytes. Target bases491D7C and491E94 are separated
by118h and correspond to MainSupervisor offsets154h and26Ch, as already
observed by the ASCII viewport view. Allocation ownership remains open.
Both positions are(0,0,1000), directions(0,0,0), up vectors(0,1,0), rocking
triples(0,0,0), FOV bits3F060A92. Gameplay viewport is(0,0,640,480,0,1), flag1;
background viewport is(32,16,384,448,0,1), flag0. Other camera fields are untouched.

First-model source retains the three real scalar rocking fields. Review
rejected an unwritten-to-native vector lvalue cast onto those scalar objects;
the compiled first source uses scalar assignments. Both models use fixed-size
memcpy from real unsigned locals for the opaque four-byte flags, avoiding an
unsigned-int lvalue over a char array. The second isolated header replaces the
three scalar rocking fields with a genuine AnmFloat3View member, not an aliasing
cast; offset3C, component offsets4/8, FOV48 and size118 are asserted. No other
consumer or maintained shared header changes. This tests a type hypothesis,
not a known original class declaration.

First source restores constructor traffic but differs in initial scheduling
and emits scalar immediate rocking stores. The real-vector model restores
rocking constructor/copy traffic yet grows to487 and still differs in
scheduling. No independent evidence justifies another source spelling, register,
ABI or flag panel. Park; reopen only with new target lifetime/constructor or
actual caller/compiler-context evidence and a falsifiable full-owner prediction.

## Native scope and bindings

Both compile one isolated source, default pinned VC7.1 LTCG profile plus /GS,
rooted at the actual initializer itself. This is a smallest diagnostic recipe,
not original production-context proof; no manufactured caller or external call
is added. Native jobs run serially, one CPU at nice10, timeout180, exit0.
Fresh target/toolchain/tracking and Factory-native Ghidra attestations pass.

[Role bindings](role-field-bindings.json) include every40 target and candidate
absolute-memory field with explicit source role, symbol base and addend. The
target lacks PE relocation entries, so the supplemental observer decodes its
absolute operands rather than accepting an empty relocation inventory. The
original first observer reports zero target relocation fields and is retained
unchanged; that is not a zero-target-address-fields claim. All extents densely
decode; every local-target inventory is empty. Unequal extents reject before
ordinary-byte scoring or canonical acceptance.

The harness gives unresolved symbols four-byte data anchors. Camera anchors
therefore overlap at403000/403004, unlike target storage. These images are
non-runnable, never executed, and not physical-storage or runtime proofs.
Bindings follow source operation provenance, not nearest-symbol guesses;
`__fltused` at gameplay+8 is not the camera Z owner. Existing canonical guards
are not linked by this isolated graph and are not claimed replayed. Maintained
source, recipes and prior proofs are untouched; frozen124/125 input files,
including all nine protected D3D evidence files, pass after both runs.

## Reproduction and retention

Source proposals and the isolated header patch are adjacent. Exact producers
retain the original ignored paths under `.analysis/scene-camera-init-20261025`
and refuse to overwrite existing build directories. To repeat, materialize the
proposals at those paths and choose a distinct build directory in a copied
producer. Do not rerun into the frozen first proof. `inventory.py` independently
rechecks both retained PE/PDB-derived complete owners and all input hashes.
Original build logs/exits and PE/MAP/PDB artifacts remain at the paths in
[retained hashes](retained-artifacts.json). Diagnostic proposals are not
production source or canonical match units.

Independent read-only review confirms both complete source models, bit payloads,
field provenance, whole extents and zero local targets. Vector's first
non-address scheduling difference is target+1E; first model differs at+0D.
Neither observation justifies another optimizer spelling panel.

Focused final checks pass: retained full-inventory/input-freeze replay,
toolchain declaration check, tracking, progress regeneration and diff whitespace.
Progress markdown/SVG are unchanged. No full CI was run for these isolated
proposals/documentation; existing production code and canonical recipes did not
change. Final check completed2026-10-25 09:07:54 UTC, exit0.
