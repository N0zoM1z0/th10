# GameManager real worker dependency, 2026-10-25

## One measured source/dependency model

After [delete/timer checkpoint cdc54c74](../game-manager-delete-tick-20261025/README.md),
include the existing ScriptViewer.hpp view, remove GameFinishWorker and replace
its two calls with ScriptViewerWorkerView::Stop through the unchanged global
byte array. Only **ScriptViewerWorker.cpp** joins the prior GameManager.cpp
OnUpdate diagnostic graph. Both TUs retain **/GL without /GS**; entry, harness
and link profile stay unchanged. This is an actual graph change, not a canonical
recipe migration or evidence of the original production TU partition.

Root, cold build and comparison exits are all 0. Complete GameManagerUpdateBody
changes **1581 to 1575 bytes versus target 1544**, still nonexact. Inventory
retains **114 fields / 55 local branches / 36 calls**; these are not accepted
parent bindings or mismatch-byte counts. Independent review passes, preserving
94 original artifacts, all 55 aligned branch roles and both real **ESI Stop
receiver edges**. Only two field identities change, facade to Stop. Four
ordinary instruction regions change: two PUSH-to-MOV-ESI receivers, removal
of the first caller cleanup, and replacement of the second load/cleanup/test
sequence by a direct memory test. These are before/after roles, not target
local-address normalization or parent acceptance.

Three same-image manager guards remain **127 bytes / all 2 fields exact**;
Tick remains **72 / all 4**, and Stop at 0x44C150 is **101 / all 3**, bound to the
actual worker TU's PDB contribution. The original OnDraw canonical recipe also
cold-passes **31 / all 1** in a separate image. All are existing credit.
**Zero new exact bytes, source owners or denominator bytes.**

## Identities and limits

- Current GameManager.cpp SHA-256:
  `cf8c462512ec0c3b95f8273029038bdf2201a35543c79964bd4bd067194bc044`
- Two-TU OnUpdate image:
  `70c33f67484fc89cd8310403cc04852a5ae4f05f0d5505898690f13d16969240`
- Separate canonical OnDraw image:
  `5ef296f6a73c31b798c5ba910092cb5046a5636ad0d39e8053591d561e75bc09`

Stop's existing noinline declaration, maintained since 46853f1, is unchanged;
no new register/outline hint is added. Its original annotation and physical TU
remain unknown. The byte-array view does not establish object construction,
lifetime or storage ownership. Independent Ready/Cancelled aliases remain
unchanged; no canonical global identity or runtime closure follows.

[Summary JSON](summary.json) records the exact graph and receipt identities.
The ignored packet is
`.analysis/gpt-dots/game-real-worker-candidate-20261025`, including separate
`ondraw/comparison.json`. Preparation-time notes remain historical; subsequent
root execution is established by the durable exits and comparison receipts.
No target payload or disassembly is copied here.

The independent receipt SHA-256 is
`ebcc5bf7877a34db072350fb42778d3dc3765d44c5b802c82dd6cec2cb4c9fb5`.
Three reviewer observer errors (EOL comparison, cleanup-position expectation
and guard-size split) remain recorded beside the successful same-image review;
they are not native failures and required no rebuild. Root's focused teardown
baseline also passes 70 cases.

**All 112 target-independent CI stages pass for this worker checkpoint**,
verified 2026-10-25 02:35:47 UTC. In
`.analysis/gpt-dots/game-worker-checkpoint-20261025`, root-ci.exit and
post-ci-inputs.exit are both 0, with all **1491 frozen source, documentation
and protected-file inputs preserved**. The root-ci.log SHA-256 is
`a91e9eba5d9448f51a739dc6b8412895261dd64bc7902444d7021689af551a73`;
[Summary JSON](summary.json) records the pin manifest and receipt hashes.
The earlier MCP timeout affected transport only; the durable CI run succeeded.
This documentation-only result update follows the successful freeze. The
02:16:22 UTC pass remains separately scoped to prior checkpoint cdc54c74.
Neither CI result creates whole-owner exact credit.
GUI substitution remains rejected and the unsupported bitfield-declaration
lead stays parked. Parent exactness and runtime/shared-global closure remain open.
