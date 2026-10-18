# Exploratory alpha typing candidate: parent-gated packet

This is one candidate only. The parent has now authorized exactly one isolated native run; see PARENT_GATE.md and the exact ready-identity review. Preparation itself performs no remote/native execution. Production remains e534a6f6c77115017e613721353308a41cde9b6a, unchanged.

## Frozen baseline and scope

The baseline packet at ../anm-execute-baseline-plan remains byte-for-byte unchanged: runner a59bfec32f6f52a5ac722f79ef05736b5ae36fd65f4ef7d95703d672f145cb63; manifest 9738c814281e531774a97d3700ed5bf865ce5cbe7cd87b210a4a3f7092f71f56. Its fresh successful receipt reported 10.339 seconds, complete ExecuteScript 9948 bytes and primary alpha 128 bytes. Both canonical guards passed on that baseline artifact. Target sizes are 9964 and 130. Historical group counts were not remeasured.

Only the ignored candidate source tree receives two source transformations. The runner first checks every original production pin and copies raw inputs. It then verifies the two original raw source hashes, performs fixed unique byte replacements in memory, verifies BOTH resulting approved hashes before writing either file, and verifies the written results. The C++ implementation preserves CRLF and the header preserves LF. No full duplicate sources are transferred. All production and copied non-overlay input pins are rechecked after execution; copied overlay pins must match the candidate hashes. Include edges and external headers remain unchanged. No production, attributes, config, driver, profile or TU changes.

The source-only change widens mode, initial and final formals from unsigned char to int and explicitly narrows the three stored bytes. Duration, callers, write order, tangents and timer reset are unchanged. Independent prior semantic review covered 781926 host cases and nine compiled mutants; these do not establish native exactness.

## Exact symbol derivation

The baseline symbol is ?StartPrimaryAlphaInterpolation@AnmVmView@@QAEXHEEE@Z. In ordinary 32-bit MSVC member-function mangling, QAEX encodes the existing public, non-const, thiscall void method prefix; H is int and E is unsigned char. The duration H remains, and the three E primitive formal encodings become H, yielding exactly ?StartPrimaryAlphaInterpolation@AnmVmView@@QAEXHHHH@Z. Primitive H encodings are repeated directly. No alternate symbol, fallback, size search, sum, or crop is permitted. The ExecuteScript symbol remains unchanged. Both owners must be unique complete PDB-attributed contributions under the unchanged strict attribution predicates.

## Execution mechanics and evidence limits

The same four-TU graph, entry, profiles, exact canonical manifest guards (Initialize273 and SetupVertexBuffer472), same-artifact checks, path-validation, resource bounds, one-shot claiming, durable receipts, process-group cleanup and status-only polling are retained. One CPU, nice10, CPU120 seconds, wall150 seconds, inherited unlimited Wine address space. One cold_link call. No runtime execution, native tools or native image reading during host preparation/tests.

The new receipt label is candidate_valid. It requires both guards exact on this candidate artifact and both complete owners. It does not mean ExecuteScript9964 or primary-alpha130 exactness, and even equal sizes do not establish matching bytes. No acceptance authority or production adoption. Before adoption, every affected canonical graph requires review.

TH08 int-formal/byte-storage source motivates this hypothesis. The upstream TH10 byte-formal helper is only a link-context model. Neither establishes the original TH10 signature. See ../th08_alpha_source_provenance.md and ../th10_alpha_upstream_provenance.md.

## Host verification

Thirty-two host-only tests pass, including inherited synthetic owner attribution, guard validation, process cleanup, path checks, one-shot and status tests; plus candidate raw-byte materialization, line endings, source drift, symlink, candidate pin, original-production postcheck, strict scope and owner-symbol tests. No native execution occurred. Test doubles intercept native entry points.
