# D3D real callees, 2026-10-24

## Accepted complete owners

Main.cpp source SHA-256:
`34af603b4fbfa66a9a94802c3383dcd108e80a57b8f3de7d27513a275654b577`.

Both reviewed bodies are now maintained source and canonically exact:
MainClearPresentTwice **135 bytes / all 8 fields**, and MainSetScreenColor
**108 bytes / all 11 fields**. The helper first passed fresh canonical cold
replay at 18:05:14 UTC. After the strict callee-unit capability was implemented,
a fresh wrapper cold build passed at 18:15:52 UTC, including complete recursive
proof of the real static helper in that same image. All frozen inputs were
preserved, and wrapper credit was added only after that measured result.

This batch adds **243 authored exact bytes and two source-present owners**.
Totals: 411 authored exact functions / 74959 bytes out of 273579 reviewed
authored bytes; 616 authored source owners; 1254 overall exact functions /
141890 bytes; 1100 source mappings. Progress and SVG reflect those totals.

## Copied canonical receipts

These are unchanged JSON receipts, not binaries or disassembly:

- [Initial helper cold replay](helper-canonical-replay.json), 3959 bytes,
  SHA-256 `949f9c8df296b9dfd2d77c8f1338a5ef364714a2ab1f0446867499f3beae1361`.
  Historical image:
  `6dee02d604d25577ac7d9f2e42d6543ce049e7b617005b9aae52d94c1f0f963d`.
- [Fresh wrapper cold replay](wrapper-canonical-replay.json), 10761 bytes,
  SHA-256 `3524045edca58d9b5fb1092783dd3135e831c58e7784306097de0b89dbd28b92`.
- [Helper replay in the wrapper build](wrapper-build-helper-replay.json),
  3959 bytes, SHA-256
  `af53453d4e75a4a3dc860fc2927d319f398f4f6b12f8366db58be11a34f398c1`.

The last two receipts bind the same fresh image:
`62a4968556263c050d60b6ab4da87016d54cfa9627ac06cb39ed112da876428c`.
The initial helper receipt keeps its historical image identity; it is not
silently repinned to this later build. Units are `main-clear-present-twice`
and `main-set-screen-color`.

## Evidence sequence and remaining parent limits

The same three-TU graph-only baseline preceded implementation and preserved all
thirteen prior exact guard owners. Independent replay of the first cold
implementation matched helper 135/all 8, wrapper 108/all 11 and real
FlushVertexBuffer 133/all 4. Implementation image:
`254c5b8e72c15bc962ae5f7a8a6a8c440976ec1aae492f193d957b7672a28316`.

The D3D parent inlines the wrapper and is **1238 versus 1159 bytes, nonexact**.
There is no direct D3D-to-wrapper call. Exact standalone callees do not establish
that missing original call boundary, make the parent exact, or turn its extent
difference into an ordinary-byte mismatch count.

The original width audit could not account for inlined SetViewport cleanup and
is retained. Separate v04 validation explicitly accounts for its eight-byte
cleanup and preserves flag/stack checks; it grants no parent exactness.
Diagnostic global aliases normalizing to one target address do not establish
shared runtime storage, whole-product ownership or runtime closure.

## Strict static callee-unit capability

The old static-callee lane correctly rejected the wrapper because its real
135-byte helper has eight fields rather than a zero-field raw-equal body.
That historical rejection is preserved. The applied `static_callee_unit`
extension now requires explicit same-artifact, same-graph complete recursive
proof, qualified identity and extent agreement, exhaustive fields and an actual
direct CALL to the proven helper. Cycle, identity, extent and field checks
fail closed; legacy zero-field behavior is unchanged.

The maintained `scripts/test-linked-static-callee-unit.py` suite passes 110
tests. This resolves wrapper canonical acceptance, not D3D parent/runtime or
shared-global closure. The new test stage is included in the complete
112-stage CI pass below.

## Retained evidence and validation status

Ignored packet roots, not supplied by a Git clone:

- `.analysis/gpt-dots/d3d-real-callee-graph-20261024/`: graph-only baseline,
  frozen inputs and thirteen guard results.
- `.analysis/gpt-dots/d3d-real-callee-implementation-20261024/`: implementation
  producer, original unresolved audit, v04 supplemental audit and controls.
- `.analysis/gpt-dots/d3d-callee-independent-native-20261024/proof.json`:
  complete body replays and explicit ownership/parent limits.
- `.analysis/gpt-dots/d3d-callee-canonical-promotion-20261024/`:
  `root-canonical-helper.json`, `root-canonical-wrapper.json`,
  `root-wrapper-build-helper-replay.json`, freezes/input-preservation receipts,
  and the historical wrapper rejection. Earlier proposal notes are superseded
  by the measured canonical results, not additional acceptance evidence.
- `.analysis/gpt-dots/d3d-callee-regression-20261024/receipt.json`:
  fresh new-source ten-TU regression passed at 18:11:20 UTC, six complete
  owners / 1436 bytes / all 69 fields.

Full CI with the new stage passed **all 112 stages** at 18:28:40 UTC, exit 0,
against this source/tool revision. The frozen input hashes remained unchanged.
Logs and input receipts are retained under
`.analysis/gpt-dots/d3d-callees-checkpoint-20261024/`; see the durable
[validation summary](validation-summary.json). Only CI-result documentation
changed afterward and received separate hygiene/link/whitespace checks.
Tooling/tests and callees/evidence/progress are separate local checkpoints.
See the [current handoff](../../RE_HANDOFF.md).
The nine unrelated files under `docs/evidence/d3d-adapter-format/` are untouched;
their distinct stopped experiment is not resumed.
