# ECL control-flow audit

## Status and scope

Optional manual source-projection test; no CI integration. Production source is unchanged. The audit does **not** establish full semantic equivalence, native ABI correctness, or a match percentage. `source_change_justified: false`.

Source pins (SHA-256):
- `src/EclVm.cpp`: `8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb`
- `src/EclVm.hpp`: `a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551`

## Reproduce the host projection

From the repository root, with Python 3 and g++ available:

```sh
python3 scripts/test-ecl-control-projection.py --source-root .
```

Optionally pass `--output-dir /new/empty/directory`. Default output is a unique system temporary directory. The script prints its location and writes generated harnesses, binaries, `test.log`, and `manifest.json` there; do not commit those outputs. Nonempty output directories and optimized Python (`-O`/`-OO`) are rejected. Source pins are checked before and after execution. Success is printed only after all test, mutation, and hash gates pass.

Observed locally and in a separate independent run: **35 fixture invocations pass; 11 mutants fail test assertions**. Compile errors, sanitizer errors, crashes, and timeouts do not count as mutation kills. Compilation uses C++17, `-O0`, and nonrecovering undefined-behavior sanitizer.

### Source behavior exercised

- Entry-null and TERMINATE suppress the ordinary time increment.
- The due-time gate precedes difficulty filtering; due masked instructions skip execution and advance, including CALL/RETURN/TERMINATE.
- Default dispatch stops automatic processing only for exactly -1; 0, +1, and -2 proceed. Callback changes to cursor, live size, time, and difficulty are observed. Switching host.activeContext alone does not rebind Run's cursor.
- Successful CALL bypasses common advancement; failed CALL suppresses the ordinary tick.
- RETURN advances the restored instruction using its size, restores time, clears an empty frame, and pops time before rejecting a restored null cursor.
- Actual Pop/LeaveFrame source bodies exercise underflow behavior; malformed RETURN fixtures are labeled separately and do not establish valid-script conformance.

Negative controls independently change live cursor reload, CALL advancement, default return comparison, default-stop tick, difficulty predicate, ordinary tick, RETURN time pop, CALL-failure tick, RETURN null-check ordering, empty-RETURN cursor clearing, and inclusive due-time comparison.

### Projection limits

Run's scoped control slices, Pop/LeaveFrame bodies, and instruction declaration are extracted verbatim. Other switch arms are omitted. Host/context/stack scaffolding is handwritten. Validation used an eight-byte-pointer host; RETURN records use four time bytes plus sizeof(pointer). The production target has four-byte pointers; multilib was unavailable in the test environment. No production ABI assertions were altered. Native layouts, MSVC behavior, frame-word interoperability, floating-point scheduling, and the full interpreter remain untested.

StartSubroutine is a deterministic **concrete-helper outcome stub**, not a callback boundary. It checks this/0 arguments, selects the configured cursor, resets time, and returns the configured status. Actual StartSubroutine argument evaluation, frame construction, lookup, and active-context restoration are untested. The +1 failure fixture is synthetic; the current source helper returns 0/-1. DispatchEclInstruction alone supplies callback mutations. A non--1 callback result with null cursor would dereference null in the source common tail and is intentionally not executed or hardened.

## Separate bounded target observations

The following are provenance observations from read-only repository-command receipts on 2026-10-17, not results of the host projection. Intermediate auditors and target data are not distributed with this test. The final bounded dependency verifier is preserved separately under `scripts/audits/ecl-control/`; it reports narrow facts, not full equivalence.

All four receipts report exit 0, untruncated output, unchanged clean repository HEAD `446283beb5b5dcf1d561e495d54d8f119e638db6`, pinned target SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, and `source_change_justified: false`:

- Preflight: `repository-command:7963ebc72ed4424eb4f7ba5a0d85c684`
- Routing: `repository-command:0477f22593504b778c2321d2e53f9c74`
- Refined shapes: `repository-command:82f39ed7c7d64317a6ed3dbfefe2222e`
- Strict dependencies: `repository-command:9a508d9261be44aa918f5244254d5d07`

Structural gates covered four owners, 2,011 decoded instructions, 279 direct branches, 131 direct calls, one indirect call, 88 selector entries, and 60 jump entries. These counts do not prove behavior.

The refined receipt establishes only local call-result branch dependencies: CALL compares the StartSubroutine result with zero, with the unequal successor returning -1 through a tail without an explicit nonstack store; default compares an indirect-call result with -1, with the equal successor returning zero through such a tail and the unequal successor reaching the common NOP entry. The earlier routing receipt's stronger-sounding `return_without_nonstack_store` label is superseded by `return_without_explicit_nonstack_store`. This is normal-execution-only bounded evidence: faulting accesses are not modeled, call effects are not excluded, and receiver/argument identity and prefix time gates are unresolved.

The strict-dependency receipt additionally establishes a generic straight-line same-slot unsigned-size pointer advance: one pointer load, one unsigned 16-bit size origin, one addition, one store to the original cell, and zero intervening stores. This identifies a dependency pattern, not an EclVmContext field. CALL's equal-successor analysis stops at a floating operation; its continuation remains unresolved.

**UNRESOLVED:** common-advance context/cursor identity; difficulty-to-common routing; CALL success continuation; RETURN restoration semantics; helper register contracts; StartSubroutine cross-callback reloads. The common block's established generic pointer dependency does not identify its fields. Neither these receipts nor the host projection justify a production change.


## Reproduce the bounded target verifier

From the repository root, using its pinned Python environment and existing target image:

```sh
scripts/repo-python -B scripts/audits/ecl-control/audit_ecl_control_dependencies.py --self-test
scripts/repo-python -B scripts/audits/ecl-control/test_ecl_control_dependencies.py
scripts/repo-python -B scripts/audits/ecl-control/audit_ecl_control_dependencies.py --root . --target resources/th10.exe
```

The first two commands exercise synthetic gates without target bytes. The final command requires the existing pinned target, configuration, linked-image helper, and decoder environment. It is read-only and emits bounded JSON facts without target bytes, instruction text, or register traces. It does not compile or rebuild the native project. No target file is included. Do not use optimized Python. The integrated verifier is frozen at SHA-256 `91b3d866ebbfec97425ca8b42c6eeeb80ab9d7bc4c127d3d6d7975591a089743`; the synthetic fixture and tests import the integrated verifier directly to cover its logic.
