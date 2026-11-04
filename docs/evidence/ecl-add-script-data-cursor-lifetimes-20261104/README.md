# TH10 ECL 589-byte AddScriptData: recover native copy/cursor lifetimes

Target: verified Japanese TH10 v1.00a (`0x00450220`, full **589 bytes**),
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This is an actual source change to `src/EclVm.cpp`, not only a diagnostic.

The original ECL subroutine database insertion retains a prior sorted-entry
table pointer and movable input-offset cursor, copies previous 8-byte entries
using **REP MOVSD**, and tests the prior table pointer immediately after the
copy before freeing it. An earlier candidate's `memcpy` correctly used REP
MOVSD but lost this pointer lifetime and unconditionalized the free, emitting
only **547 bytes**, with REP MOVSD at `+0xED` instead of original `+0xFB`.

The maintained source keeps the actual `memcpy` and free semantics, modelling
the **pointer objects themselves** as reloadable locals:
`EclVmSubroutineEntry *volatile previousEntries` and
`unsigned int *volatile offsets`. The target's observable pointer/stack reloads
motivate these qualifiers; they do not claim inter-thread mutation or original
source spelling. The native compiler now emits a **570-byte complete PDB owner**,
with REP MOVSD at **`+0xFC`**, followed by the original type of conditional
branch to skip `free` when the pointer is null. This is a structural improvement,
**not** a 589-byte target match, and adds **zero exact credit**.

Real pinned VC7.1 SP1 `/GL` experiments used the five actual Enemy, FileSystem,
PbgArchive, EclVm and GameErrorContext sources; `EclVm.cpp` was freshly
compiled, while unaffected support objects were reused only after source+object
SHA-256 checks. Explicit word and record loops were also tried: the record loop
reached 580 bytes but incorrectly removed REP MOVSD. Some name/count volatile
combinations inflated the size without retaining target register scheduling.
`native-result.json` contains source/image identities, original and candidate
copy instruction offsets, comparison limits and the entire tested-shape summary.

Target-bound exact regressions on the installed source passed:
`EnemyEclResourceView::LoadFile` **95/95**,
`EclVmScriptDatabase::FindSubroutine` **136/136**, and
`EnemyEclResourceView::LoadPackage` **239/239** (**470/470** bytes).
No all-unit cold replay, native product build, historical-source attribution,
runtime equivalence or portability is asserted. The 589-byte function remains
explicitly **NONEXACT**.
