# Actual unchanged-source ANM baseline, 2026-10-18

Command repository-command:33484bc750dd4640b2de88ae1609e49b completed at10:33:38UTC on unchanged clean HEAD e534a6f6c77115017e613721353308a41cde9b6a. One cold link took10.339seconds, with89 unresolved data anchors. All current input pins and clean-source postchecks passed; process group cleanup completed.

Complete same-start PDB-owned ExecuteScript is9948 bytes against target9964; primary-alpha is128 against130. These freshly reproduce the historical sizes. Selector groups were not remeasured: no fresh83/92 claim is made. Neither large owner was tested for exactness; zero exact credit and no runtime execution.

Both canonical guards pass in the same image: Initialize273/273 raw-equal and SetupVertexBuffer472/472 fully normalized exact (raw unequal because linkage addresses differ). This is a valid diagnostic baseline, not fresh Factory acceptance.

Image SHA256: 0d354f972c1a554aa98ad0b6ccf28c860d54f757fbbe521594e5213d98c72dff
ExecuteScript complete owner SHA256: aa000c8ba3d351e877404e9830fbf77ba303fcb02c70923e3a8ae3cb2aeee775
Primary-alpha complete owner SHA256: f5e9c491716b9bb4b93bfed8426d3b86dfd533e87e2a4f3c1b101d56c3912039

ACTUAL_BASELINE_RESULT.json binds the complete sanitized result, command/receipt hashes, timestamps, reviewed runner/manifest hashes and source/artifact identities. The tool reports upstream alignment0/0 before and after; this run created no commit or push, and no explanation for the external alignment change is inferred.

## Compact archival strategy

Archive source-only recipe plus sanitized evidence, not generated native artifacts: frozen run.py and ready manifest.proposed.json, their exact hashes, ACTUAL_BASELINE_RESULT.json, raw metadata-only remote receipt, review disposition, invocation/resource/stop rules, and synthetic test_runner.py/test_owner.py. Keep the target/EXE/PDB/MAP/OBJ, compiler logs and private guard JSON only in ignored remote scratch while the current candidate comparison needs them; never commit or export native payloads.

The manifest already contains all83 raw source pins,11-file ANM include closure, exact four-TU graph/profile, target/tool pins and both complete canonical guard definitions. A short archival README can point to the committed source/config/tool files and these pins. Do not duplicate all source files just for this diagnostic. Archive original baseline and later candidate as separate immutable outcomes sharing the baseline recipe, with explicit two-file overlay hashes for the candidate. Do not overwrite the baseline receipt or use its guard pass for a different candidate image.

The local prepare_plan.py imports a planning helper via /workspace/shared and test_inputs.py has a local snapshot path; do not claim these are a portable committed replay recipe as-is. Either omit the regeneration helper and local-only closure fixture (frozen runner already verifies closure at execution), or adapt them in a separately reviewed source-only archival change to repository-relative paths. Retain the already reviewed runner unchanged. Any later canonical registration requires its own justified full-owner linkage/data specification and acceptance process; this diagnostic adds none.

Do not rebuild this successful baseline. Preserve the current ignored remote artifact for the sole independently reviewed two-file parameter-width candidate comparison. The candidate preparation has no authorization from this note to execute or alter production.
