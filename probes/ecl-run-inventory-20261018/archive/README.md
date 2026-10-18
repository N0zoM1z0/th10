# Run7020 discrepancy inventory archive

Source-only archival packet. Contains exact reviewed inventory/localization collectors, exact authored tests, synthetic-only historical fixtures, fixed schemas, scopes, actual count-only receipts/results and manifests. No native image, object, PDB, target bytes, assembly, register data, field arrays or ordered native traces are included.

Read ACTUAL_RESULT.md and ACTUAL_LOCALIZATION_RESULT.md first. The baseline whole-owner raw discrepancy is787 positions;139 prefix differences lie outside recognized candidate fields. Decimal-opcode30's selector-associated lexical interval contains118 of those139. These are conditional structural investigation leads, never semantic ownership/equivalence or exactness credit.

Reviewed collector hashes:
- inventory.py:7994fd0c444eb909e11acdfa7443ea4f498c8b935b24dc564e82ab6cfe0e1ade
- localization.py:e9417b47e2e07afbdd1d1046918ff6edeaed30d864b75524087a3da9ef6746e3

Actual parent invocations:
- inventory:e8404d04f7554c088119f7331971d970
- localization:632e317d46f84bc188b81bdc916b4400

Local synthetic replay: python REPLAY_TESTS.py
Wrapper safety regression: python TEST_REPLAY_WRAPPER.py

Replay rejects optimized Python before any resource changes or copies and requires exact recursive file membership, rejects symlinks, checks mandatory source/test dependencies, and verifies every archive-manifest file hash before preparing its temporary copy.

Replay makes a temporary source-only copy and rewrites exactly two historical fixture import paths. Archived collectors and original tests remain byte-identical; no verifier inspect/main path touches native inputs during the tests. Suites contain36 and48 tests respectively. The fixtures include historical source modules only to support synthetic ownership/table tests. Their presence is not authorization to execute a historical verifier or baseline build runner.

Current clean repository HEAD at localization was65e3fd3c127c7efe190ed9ac2cd9b82d31bd2725. Historical baseline manifest HEAD remainsa4edda20e434cd40ee99ea3b6b68673a80c3e3be. No new remote operation, build, source mutation, exactness claim or adoption is authorized by this archival packet.
