# Complete Stage camera — 2026-10-29

Stage camera4215A0..4216E9 adds **330 authored exact bytes**. Two independent
canonical ten-input cold links reproduce the complete PDB owner and all10
address fields. Its existing source mapping is renamed to
`StageCameraStateView::ConfigureCamera`; mappings and the reviewed authored
denominator stay fixed. [Results](results.json) retain source/include hashes,
profiles, field-contract hash, artifact identities and distinct PDB GUIDs.

The complete owner flushes pending sprites, constructs view and projection
matrices, installs transforms2/3, computes and normalizes direction × up into
cameraRight, then reloads the renderer and publishes the two shake values. Near
and far planes are30 and1800. Width/height are unsigned; both occurrences of the
compiler's binary32 unsigned correction constant at470B98 are independently
raw-equal. D3DX calls keep their actual stdcall boundaries. The native camera
receiver is EDI and the owner ends in plain RET. Target calls403A48,4028C1,
402B78,402D0D and40183B/401872/4019FF corroborate that receiver convention.
The original source class name and TU remain unknown.

A standalone three-component constructor gave the correct330-byte size but
changed cross-product operand loads and the temporary copy. An ordinary inline
cross helper first computes a local vector and then copies it to the output;
that restores the copy sequence but leaves two multiplication operand pairs
reversed. A local device pointer, member camera operation and direct XYZ
construction of the look-at target complete the source context. The last direct
XYZ change turns the full330-byte nonmatch into a full match. There is no forced
calling convention/register, volatile spill, padding, code-byte patch, noinline
annotation or profile change. Original source spelling is not established by
this reproduction.

Both colds select `stage-configure-camera`, `main-select-viewport` and
`main-disable-fog`. Five existing same-image contracts pass260/all9: viewport
selection60, fog disable53, two registered draw adapters7 each and real vertex
flush133. The last three use their canonical complete byte/field contracts in
the new ten-input image; their original recipes remain separate. The30-input
Main recipe is also cold-rebuilt with explicit WinMain/Render selectors:
1389+425 bytes/all180 fields remain exact. No historical cohort is replayed.

The neighboring default camera421480 remains **282/287, nonexact**. Its source
now retains width/height rounding stores and places the float conversion on the
tan result, recovering the target FPTAN/FDIVR form without duplicating height.
Aspect computation still occurs after LookAt instead of before it; register and
store scheduling also differ. Temporary vectors, double aspect, /Op, named
halves, double-tan and a parameter aggregate are measured nonmatches. /Op is
restored; no volatile source control is adopted. These attempts add no exact
credit. Final Stage drawing cores also remain1102/1099 and933/929, nonexact.
Earlier private diagnostics accidentally read interior402A50/402EA0 instead of
402850/402CA0 for those two drawing cores; those ordinary comparisons are
invalid and discarded. Camera comparisons and PDB sizes were unaffected.

The local pinned SDK's d3dx8math.inl supplies an alias-safe cross-product shape
lead; this is not a claim that TH10 uses D3D8. Its forceinline control was neutral
and restored. The MIT [reference source](https://github.com/levzzz5154/th10-decomp/blob/f51eec86d81064788b6d2ff80c0be28d96095d27/matching2/msvc/src/game/mother/camera_apply.cpp)
supplies member/target-construction/device-local hypotheses only; the
[license](../../licenses/th10-decomp-MIT.txt) is retained. Its synthetic caller,
placement, attributes, compiler profiles and acceptance claims are not imported.
Local complete target-bound comparisons decide this result.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit stage-configure-camera --unit main-select-viewport --unit main-disable-fog --json
scripts/repo-python scripts/replay-exact-units.py --unit main-winmain --unit main-render --json
scripts/repo-python scripts/ci.py
git diff --check
```

The comparator is unchanged. Data/global alias ownership, one-image exact
closure, product build/runtime and portability remain separate open phases.
