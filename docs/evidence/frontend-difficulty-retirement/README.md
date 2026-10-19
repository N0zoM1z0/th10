# Difficulty initialization retirement hypothesis

This archive records one unadopted, reference-inspired source experiment in `FrontEndControllerView::UpdateDifficulty`. In INITIALIZE, the candidate changes only `DeleteVm(controller, 0x62)` to `DeleteVm(controller, rootIndex)`, immediately before creating the root. `rootIndex` is already captured from the difficulty. The edit adds five source bytes, preserves CRLF, and leaves the header, ABI, helpers, call order, state break and timing unchanged. It starts from the pristine FrontEnd baseline without Replay or ShotType overlays.

## Recorded outcome

A fresh baseline/candidate pair produced whole same-start PDB-owned contributions of **840 / 828 bytes**, respectively, against the known **888-byte target size**. Both start at **4214128 (0x00404D70)**. Neither size matches the target. The target Difficulty body was not separately extracted, decoded or compared by this experiment. The retained helper guard performed full-image identity reads and loaded the target image for its four helper comparisons. A 12-byte size reduction is not an accuracy improvement. The raw comparison found798 differing positions in828 overlapping bytes and12 unmatched baseline bytes; these differences remain unclassified.

The genuine single-TU `FrontEndCursorPush` graph and `UpdateDifficulty` entry remained fixed: `/GL`, no `/GS`, existing canonical link profile and unresolved-symbol anchor mechanism. The producer used the full same-start `source.ltcg.obj` PDB contribution, without cropping, trimming, summing helpers, forced retention or profile searches. Independent jump-table coverage is not asserted. Exactly two cold links ran, one per arm. Both worker cleanup and shared-runtime sampling completed; eight shared unmanaged Wine processes remained, with zero pending grace counts. This is not a zero-process or global service-cleanliness claim.

A later read of those existing images, with no rebuild, verified four complete helpers in each arm: Push66, Pop47, Move128 and IsRepeated33. All eight checks were **raw exact**, with empty declared linkages and no data spans; no relocation normalization was allowed. These preservation checks do not establish UpdateDifficulty exactness, runtime behavior, general Factory acceptance or production adoption.

The once-only pair and guard claims are consumed. The retained guard added exactly comparator/config/target inputs to each isolated arm tree. Original preflight absence inventories are historical and cannot authorize another run. Production source and clean HEAD4a113613d9f4a04b8b70ff739f2965461d6352c6 were unchanged. Sanitized command identities, result details, image/owner/report hashes and current canonical rows are in `evidence.json`; generated images, PDBs, target bytes, raw function bodies, private logs and full source copies are intentionally absent.

## Conditional source argument

`DeleteVm` requests `pendingInterrupt=1` for a resolved handle and clears the selected controller slot even when lookup misses. It does not prove destruction. The model covers difficulty0–4, both backdrop branches, null and unresolved handles, live non-head nodes and a bounded acyclic successor chain, plus the ordinary Character-return slot effects. On the modeled normal Difficulty→Character→Difficulty route, the old difficulty-root handle is not explicitly cleared. If that handle survives, remains resolvable, and has not been externally replaced, selecting the captured root slot requests retirement before its new creation.

Those are premises, not observed runtime facts. The model assumes stable successful creation with distinct handles and valid cursor0 fixtures. It does not execute C++, Win32, ANM scripts/callbacks, allocation failures, external slot mutation or asynchronous destruction. No actual leak, recovered resource, effective retirement or upstream ABI equivalence is claimed. Other states/helpers remain byte-identical rather than being reimplemented in the model.

## Local reproduction

From this directory run:

`PYTHONDONTWRITEBYTECODE=1 python -B test_candidate.py --baseline-root /absolute/pinned/baseline/root`

The seven source/model tests require exact raw baseline CPP SHA256 `c26c9c1c00b3caa98a01fc210efdbb898fe6e8cace5a4d84a8072cd3f8a90d5d` and header SHA256 `8b4e0e13312bce0b9bab71e16daff79cfa402d55a61e55cca53bd359edc7832b`. `reconstruct.py` performs a context-scoped in-memory edit and exact inverse check. The resulting CPP SHA256 must be `addb9de47649a892f4241cf20b1992b165ce7b91afa0e0097b4c0ffff19fe463`. No import-time source reads or output writes are required. Tests compare the regenerated minimal patch byte-for-byte and verify CRLF preservation; there is no LF normalization.

`candidate.patch` contains CRLF source/context lines with LF diff metadata. Local `.gitattributes` marks only that patch `-text`; archive Python/Markdown/JSON files use LF. The archive has no compiler, linker, runtime, remote-deployment or commit launcher.

## Provenance

The authored inspiration is pinned to levzzz5154/th10-decomp commit `4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a`, source blob `68cf2ffad8fc917992ec87b5fa532425210c5ad4`. `provenance.json` retains source-review and response hashes. The original one-index patch and model copy no upstream implementation, pragma, helper or control annotation; no c679 reference material is used. The source package's verified upstream root MIT notice is retained byte-identically as contextual attribution. This archive does not expand the source package's licensing verification or claim that similarly named upstream retirement helpers have identical semantics.

This is a local evidence proposal. Any deployment, native rerun, target comparison, candidate revision or adoption requires separate authorization and review.
