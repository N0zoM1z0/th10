# Transition effect host contracts

These are LOCAL-ONLY semantic tests, not a target build, target execution, exactness claim, scheduler reconstruction, or proof of 32-bit layout/private calling conventions. No reference implementation is copied into the fixture. Every proposed body and selector is read from `src/TransitionEffect.cpp` at test generation time.

## Run

In an installed repository containing the pinned dependencies:

```
python3 scripts/transition_effect_host_test.py
python3 scripts/transition_effect_host_test.py --sanitize --out .analysis/transition-effect-host/sanitized
python3 scripts/transition_effect_mutations.py
```

The default is repository-local `src`; there is no silent remote or machine-specific fallback. The runners enforce the seven strict LF-normalized dependency hashes from version 2 of `config/transition-effect-host-inputs.json` before generating or compiling. Only byte sequences CRLF are changed to LF. Ordinary whitespace, bare CR, BOM, and every other content change are rejected; there is no raw-hash fallback. The configuration retains original observed raw hashes as provenance only. Execution manifests retain raw and normalized hashes, byte counts, and CRLF/bare-CR counts for the source, header, fixture, and other inputs, alongside the extracted timer/range-helper/adapted-Chain hashes, compiler command, output, and return codes. Extraction uses this same explicit byte normalization, never universal-newline decoding. Only local host g++ is invoked.

## Actual bodies and declared seams

The 2026-10-19 dependency refresh changes only the Chain declaration/definition
pins for the reviewed `UnregisterElem` fastcall spelling. The complete native
114-byte owner and RemoveAll 82 pass two cold comparisons; see the
[browser lifecycle evidence](evidence/script-viewer-native-lifecycle/README.md).
The host fixture already erases `__fastcall`, so its algorithms and seams are
unchanged. The pin file retains the original raw hashes and receipt as historical
provenance and records the two reviewed normalized-hash updates separately.

A second 2026-10-19 refresh updates only the AnmManager.cpp normalized pin
after primary-alpha tangent initialization moved before endpoint stores; see
[the full native evidence](evidence/anm-primary-alpha-local/README.md).
The extracted `AnmRandomU32InRange` body is byte-identical under the existing
normalization, and timer/viewport headers are unchanged. The pin file records
that equality and both normalized file hashes separately from the historical
raw provenance. Extraction rules and rejection tests are unchanged.

- The proposed source is included completely, with includes removed and only allocation/deletion operations redirected to host instrumentation. The proposed header is included with its 32-bit physical-layout assertion omitted. Host structures have native host pointers. No target ABI or layout is thereby established.
- Canonical `Chain.cpp` is included. Registration, insertion, and unregistration algorithms remain unchanged. `AllocElem`'s hard-coded 32-bit field lvalues are translated to the same named fields, with pinned exact replacement counts. Its `new ChainElem` and `free(elem)` operations are instrumented. `RemoveAllFromList`'s raw `listBase+0x18` next-pointer read is explicitly adapted to a host ListNode passed by the fixture. No claim is made that this adapted parameter has the original ABI.
- Canonical `RandomMath.cpp`, `Rng.hpp`, the complete inline `AnmVmTimerView` struct, and the complete `AnmRandomU32InRange` body are extracted from pinned sources. The RNG oracle checks use fixed independently calculated seed/output values. No separate effect reference implementation is used.
- Host allocation uses malloc plus placement construction and fills effect storage with `0xa5` before the actual constructor, so a missing factory zero-fill is observable. Nodes are actually released, not kept alive in a quarantine. This fixture choice does not prove ISO-defined initialization: the canonical timer constructor reads its flags member before the factory memset. The compiler's uninitialized-member warning is retained. Sanitizer success does not resolve this inherited constructor/object-lifetime concern.
- A `std::recursive_mutex` models recursive Windows critical-section behavior and records nesting. It is not a Windows critical section or a concurrency proof. Registration snapshots observe partial publication at the real canonical insertion lock boundary; they do not schedule concurrent engine callbacks. The final frames-before-kind write order is additionally checked as an explicitly labeled source-order check.
- The renderer is an argument/event-observation seam. It records rectangle, color, viewport, captured manager flush, and later global device/manager observations. It does not execute the real rectangle renderer, D3D device, or validate physical global aliases.
- The scheduler is an explicitly named observed-protocol seam for statuses 0, 1, and 7 only. It caches next before a callback, runs update unlocked, invokes the delete callback while locked, and observes both recursive cleanup cycles. It is not the missing canonical calc/draw dispatcher, and its successful traversal does not establish real-engine asynchronous lifetime closure.

## Coverage

The suite exercises all nine selector entries and all eleven distinct update/draw callbacks; caller draw priority and calc priority 14; update-then-draw publication; zero-fill, flags, payload/timer initialization order; null factory allocation; fade fractional/negative/zero durations, pause, abort, and wrapping end comparisons; persistent dim/release boundaries; pulse signed repeat count and return 0; all rectangle geometries/color masks; viewport reset and global reload sequencing; canonical timer fractional/near-unit paths; duplicated-second-half kind-1 RNG versus genuine kind-8 range RNG; attack/hold/decay boundaries and signed comparisons/unsigned conversions; cleanup pre-entry chain capture and between-cycle chain/draw reload; null destruction and no implicit deletion on Unregister/RemoveAll.

Mode 4 is tested by recording the update address before dispatch, observing the live owner's stale pointer value afterward, and never dereferencing that freed node. Its owner and draw node remain alive. Test teardown explicitly unregisters the remaining draw node and releases the owner; it does not present this as production lifetime behavior. A stale absent pointer can miss the canonical list search; address reuse can instead target an unrelated node. Neither universal immediate UAF nor safe repeat-cleanup is claimed.

Invalid kind and failed node registration remain target crash/precondition domains. The harness does not invent safe-success cases or intentionally trigger those undefined behaviors. Nonfinite/out-of-range float-to-integer conversion is also outside these positive checks.

## Numeric fidelity gate

All ordinary float assertions use strict equality with selected finite host values; no epsilon tolerance, volatile trick, or compiler flag tuning is used. Some unsigned-decay checks are explicitly strict host-float observations rather than universal x87 equivalence claims.

A separate `precision_gap` scenario preserves the known counterexample: kind 8, post-tick subframe 1, attack 3, amplitude 10 produces strict-host-float `3.3333334922790527`, while evaluating the target extended-precision division/multiply and finally storing float produces `3.3333332538604736`. It prints `KNOWN_GAP` and asserts that the difference remains visible. A passing suite therefore explicitly does **not** mean universal finite-FP fidelity. One mutation is a negative control against losing this gap observation, not evidence that the discrepant production arithmetic is target-exact.

## Strict mutations and sanitizers

Each mutation requires one unique function marker, one unique selected source block, and one occurrence of each scoped edit anchor. Every mutant must compile. A kill requires exit code 86, empty stdout, and stderr exactly equal to its named `ASSERT:<identifier>` followed by one newline. Leading/trailing noise, extra blank lines, or additional output are rejected. Compile errors, arbitrary crashes, other assertions, timeouts, unchanged/surviving mutants, and missing/duplicate anchors are not accepted as kills. The runner first requires the unchanged full baseline to pass. Its report retains each mutant's full expected/observed result and hash.

ASan and UBSan are optional host observations. This container runs under tracing, which makes LeakSanitizer itself fail; sanitizer runs explicitly use `ASAN_OPTIONS=detect_leaks=0`, recorded in the manifest. The fixture's allocation ledger independently asserts that each test releases every tracked allocation. That ledger is bounded to the fixture and is not a general leak detector.

Generated harnesses, binaries, mutant source copies, logs, and manifests live only under `.analysis/transition-effect-host/`; exclude that directory from a proposed repository patch. The reusable authored artifacts are this document, the fixture, and the three scripts.

Runner-only operational bounds: mutation compilation defaults to one worker (`--jobs 1`), and baseline/mutant executions each have a 15-second timeout. Timeout or compilation failure fails the run and is never credited as a mutation kill. These bounds do not change effect or fixture semantics.

## Line-ending portability check

Run `python3 scripts/transition_effect_eol_test.py --canonical src` for strict pin tests covering LF, CRLF, mixed line endings, whitespace/content changes, bare CR, and legacy-schema rejection. This host-only portability rule accepts ordinary Git `eol=crlf` checkouts without relaxing dependency identity. It does not rewrite any production input.

The2026-10-19 [allocator/scheduler review](evidence/effect-allocation-abi/README.md)
updates the exact-one host allocation rewrite to the actual ordinary-new source
expression and the init_flags function marker to its callee-pop declaration.
Three dependency LF pins change only for reviewed calling-convention annotations;
full-file reversal proof is recorded. All843 baseline assertions, the focused
init_flags negative control and all19 unchanged EOL guards pass. This host seam
does not establish native ordinary-new exception or failure behavior.
