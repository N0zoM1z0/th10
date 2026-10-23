# Player startup VM binding and Stage caller context

2026-10-23, starting 0902294. PlayerInitialize's embedded VM call now uses the
existing full ANM initializer instead of a declaration-only placeholder. This
repairs one large-owner call binding; neither Player nor Stage becomes exact.

## Target-backed source change

At 0x4248B1, the complete 1435-byte PlayerInitialize calls 0x404F30. The preceding
instructions supply VM=Player+0x14, the resource pointer at Player+0x10 and script 0.
Fresh readOnly/noanalysis Ghidra and independent decoding from the reviewed
parent entry agree. The full mapped 412289-byte hash remains
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.

The old PlayerInitializeEmbeddedDrawVm interface had no implementation. In the
measured linked graph it resolved to a non-runnable data anchor. The maintained
source removes that unused declaration and calls
AnmVmView::InitializeForLoadedScript through the existing Player VM/resource
boundary views. Existing assertions retain VM offset 0x14, size 0x3AC and relevant
field offsets. No shared header, layout, public signature, ABI annotation or
compiler profile changes. Original class identity and object lifetime remain
unproven by these views.

The corrected native Player parent has one direct CALL to the unique, real
200-byte ANM implementation; its former placeholder disappears from unresolved
symbols without introducing a replacement unresolved symbol. Private and
maintained Player compilations reuse the same 16 verified game objects. All 366
complete public MAP/PDB-owned functions retain identical extents and raw bytes
between those two images. Static/unowned fragments are outside that proof.

Player remains 1116 versus target 1435. Masked structural positions fall 126->98
of 1267 under this particular graph; these are diagnostic scores, not an exact
target comparison or a byte-improvement claim. The source change is retained
because the actual target callee is independently established, rather than
preserving the higher score of an unimplemented interface.

## Experiments and caller audit

| Actual context | Complete Stage bytes / guards | Result |
| --- | --- | --- |
| Member script/resource argument order swapped consistently |1868;11/987/all 36|Complete Stage and initializer raw-neutral after explicit source-identity checks|
| Ordinary free-function counterpart, VM first |1872;11/987/all 36|Rejected; native initializer198/plain RET versus target 202/RET4|
| All 26 then-maintained named callers,16 fresh game TUs |1868;11/987/all 36|Baseline-equal after 13 independently bound fields|
| Add Player with its old placeholder,17 TUs |1868;11/987/all 36|Player 1116, zero direct edges to the real initializer|
| Corrected private Player,17 TUs |1868;11/987/all 36|Player 1116, one real initializer edge|
| Maintained corrected Player,17 TUs |1868;11/987/all 36|All 366 public owned bodies raw-equal to private image|

All source-kind/formal controls convert the actual declaration, definition and
26 source calls consistently in private snapshots; all nine selected game TUs
are recompiled. The 16-TU control compiles all 26 named calls without source
changes. Player before/after controls each compile only Player and reuse 16
objects; they are not independent full cold pairs. /GL /GS stays enabled.
No unexecuted free-function order or calling-convention variant is credited.

The [xref audit](initializer-xrefs.json) independently confirms 28 direct calls
inside 11 reviewed physical owners. Five references have no Ghidra function
owner but do have reviewed ledger owners; the old 23/eight-owner description was
not a complete caller inventory. Before this change the source had 26 explicit
calls to the shared member; afterward it has 27. Counts alone do not prove
call-site or runtime closure. The 214-byte owner 0x404450 still has no mapped source.
Two further raw E8 hints, 0x41BE65 and 0x4415D5, lack owner/instruction-boundary proof
and are not promoted to calls, functions or authored coverage.

## Validation and retained state

The measured 17-TU before/after images each pass 11 whole Stage guards 987/all 36
plus the existing raw 273-byte VM Initialize contract: 12 checks/1260/all 36.
Four Chain contracts reject in both 17-TU images because their complete native
extents differ from those contracts; they are not cropped or counted as passes.
Their separate canonical 14-TU cold replay uses the original recipe and current
source and passes all four complete contracts: 838 bytes/all 58 fields. The prior
canonical carrier is hash-archived before replacement. This is one current-source
cold for existing claims, not new exact credit or a new acceptance cold pair.
See [validation](validation-summary.json) for the actual checks.

Of 366 before/after public owners, 353 are verified equal raw or after independent
address replay. Twelve more retain unresolved address bindings; Player changes
field inventory. This is not a proof that every other byte is unchanged. The
separate private-to-maintained 366-owner equality above is complete for that
declared public-owner scope. No full CI, runtime or product closure is claimed.

[Evidence](evidence.json) binds sources, actual compilations, complete PDB extents,
the real caller edge, failed/rejected scopes and preservation limits. Adjacent
patches preserve the tested transformations. Private recovery is
`.analysis/gpt-6.1-sol/stage-initializer-formals-20261023`; historical producers
pin the before-state. Use the explicit final manifest for maintained replay;
do not restore source or relax historical pins. Exact counts stay unchanged.

Hash-verified cleanup removes 81 superseded native files/3964133 bytes and 160
unchanged, uncompiled source-snapshot copies/2470106 bytes. All modified and
compiled private inputs remain. Retain the before/after 17-TU native carriers,
before-Player source and previous/current canonical Chain witnesses. The
[cleanup inventory](cleanup-inventory.json) records every removed file.

Next investigate the remaining real dependency bindings in the 1435-byte Player
initializer, retaining complete-parent evidence. Keep Stage, ANM, ECL and Enemy
open and do not repeat the closed local-variable/formal-order controls.
