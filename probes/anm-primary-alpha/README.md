# Bounded primary-alpha source hypothesis and native diagnostics

This archive contains source-only recipes and sanitized evidence. No complete AnmManager source copies, target binary, generated PE/PDB/MAP/OBJ, disassembly or ordered native traces are included.

## Current result and limits

At clean baseline e534a6f6c77115017e613721353308a41cde9b6a, one unchanged-source direct ExecuteScript /GL /GS link measures complete ExecuteScript9948/9964 and primary-alpha128/130. The sole candidate changes the three byte formals to int and explicitly byte-normalizes their assignments; both caller expressions remain unchanged. Its complete owners measure ExecuteScript9948/9964 and alpha130/130. Both artifacts independently pass existing Initialize273 and SetupVertexBuffer472 canonical guards. Matching alpha extent is not byte equality, original ABI proof or exact credit. A read-only full130-byte owner comparison finds94 raw equal bytes and36 mismatches. Candidate decode identifies one4-byte DIR32 field; even allowing every byte at those four positions to change leaves at least32 mismatches. This is not normalized comparison and makes no target-global-absence claim. The candidate is parked/unadopted, with no further build or normalization authorized. A separate fresh read-only ExecuteScript diagnostic now confirms83/92 equal physical group-span lengths. This is size-only evidence, without instruction equivalence, normalization or exactness; see execute-span-lengths/ACTUAL_RESULT.md.

See docs/evidence/anm-primary-alpha-2026-10-18 from repository root for the actual baseline/candidate receipts and limits. Production source was not changed by these isolated links. Math/TH09 remain outside scope.

## Portable host replay without source duplicates

Requires Python3, GNU patch with --binary support, and g++ for the full host run (the --check-only path needs no compiler). From any directory, use normal Python3 and an absolute path to this script:

python3 /path/to/repository/probes/anm-primary-alpha/host/replay_host.py --source-root /path/to/exact-baseline-repository

The supplied repository must contain src/AnmManager.cpp/.hpp at the pinned baseline hashes in host/identities.json. A current tree containing the proposed candidate must not be silently treated as baseline: use the pinned baseline checkout/source input explicitly. The adapter copies only these two files into TemporaryDirectory, applies the frozen mixed-newline patch with --binary/--fuzz=0, verifies candidate hashes, reverses a temporary copy and verifies original hashes, then runs the unmodified reviewed host extraction test. No source file in the supplied repository or archive is written. Generated code/executables are temporary and removed. Use --check-only to perform byte reconstruction without compiling host code; --output writes an optional chosen result file.

The .cpp retains CRLF; .hpp retains LF. The full host run passes781926 Cartesian cases and nine functional mutation controls. It uses the actual source helper/timer declarations and defined static initialization; it establishes byte-normalization/value semantics, not target layout, native execution, LTCG ABI or exactness.

## Synthetic runner tests

python3 -B -m unittest discover -s /path/to/repository/probes/anm-primary-alpha/baseline -p 'test_*.py'
python3 -B -m unittest discover -s /path/to/repository/probes/anm-primary-alpha/candidate -p 'test_*.py'

These retained test_runner/test_owner suites are self-contained and mock native actions. Run them as separate processes because both recipes intentionally use module name run. Each suite currently has20 tests. The local-path planning generator and local-only input/overlay fixtures are deliberately omitted. Their useful byte/identity gate is retained in the portable host adapter and in the frozen runners themselves.

## Frozen native recipe, environment-bound

baseline/run.py and candidate/run.py plus their manifests are exact reviewed execution packets; do not regenerate or modify them to reuse old approval. Their fixed historical compiler/Wine roots are deliberate environment bindings, not claims of general cross-machine portability. No native build is launched by the portable host tests. Archived INVOCATION files record parent-only historical execution, not instructions to rerun completed experiments automatically. Fresh native activity requires a new parent gate, new ignored run directory and current source/tool verification. Candidate has exactly two reviewed raw source overlays generated internally; no full source overlay copies are archived.

Reference provenance and target-authored historical ABI notes are evidence only. TH08 inspired the type hypothesis; its code does not prove TH10's original signature. No wrapper/attribute/register-forcing/padding/profile/TU sweep was added.

## Fresh ExecuteScript span-length replay tests

Set TH10_SCRIPT_ROOT to /path/to/repository/scripts and TH10_SOURCE_ROOT to /path/to/exact-baseline-repository, then run python3 -B -m unittest discover -s /path/to/archive/execute-span-lengths -p 'test_*.py'. Its12 synthetic/source tests open no native artifacts. The frozen measure.py is a historical read-only remote diagnostic, not part of this host test execution. New native diagnostic runs require a separate parent gate and verified frozen baseline artifact.
