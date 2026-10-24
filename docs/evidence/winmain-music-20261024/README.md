# WinMain music caller recovery, 2026-10-24

## Current result

Two separately measured models after **b33dda1** leave WinMain **1423 versus
1389 bytes, nonexact**, with 153 fields, 72 calls and 48 local branches. These
are complete-owner extents and inventory counts, not mismatch-byte counts.
This batch adds **zero exact bytes and zero source owners**; metrics are unchanged.
All 112 target-independent CI stages passed for this source, exit 0, with
frozen inputs preserved, verified at 21:39:42 UTC. Only CI-result documentation
changed afterward. The separate automatic boundary-ledger divergence remains open.

Current `src/Main.cpp` SHA-256:
`e49f06a2c64f0eb6e2577237253a0b3bf0773d14fc51e991fcb2ced19d7be4f0`.
Current image SHA-256:
`832cc185996620c09694424cd4ef0f5b06589323f3c3152aa1c8f8ccecadbcb1`.
[Machine-readable summary](summary.json) pins both source transitions, images,
original receipts and independent reviews without copying binary/disassembly data.

| Separate model | Bytes / fields / calls / locals | Measured outcome |
| --- | --- | --- |
| Post-stop reload/null gate | 1438 / 153 / 72 / 49 | Fresh musicOwner load/null test gates destruction after stop. Final null publication remains outside the inner gate and inside the original outer gate. |
| Natural StopPlayback member call | 1423 / 153 / 72 / 48 | Only MainStopMusic becomes MidiOutputView::StopPlayback; the actual EDI receiver now naturally matches the target. |

Both native runners exit **0**. The first model's independent replay passes,
preserving **115 original artifacts**. The second independent replay also passes
with original files unchanged; all preceding properties and intact guards hold.
Neither result establishes a complete accepted WinMain binding comparison.

## Callee result and unchanged guards

The actual StopPlayback body is **129 bytes with all five field identities
bound**, but retains **13 ordinary scheduling-byte differences**. That count is
valid only for this complete equal-extent, fully bound callee, not the parent.
Independent baseline comparison shows all ordinary callee bytes are unchanged
from the preceding image: the new real caller did not remove those differences.
Receiver identity comes from actual caller def-use and callee field access,
not an invented source calling convention or forced register hint.

Same-image UnprepareHeader is **79 bytes / all 3 fields exact**. Both models
preserve nine complete guards totaling **1758 bytes / all 94 fields**, plus
Log/Fatal/WriteFile totaling **620 bytes / all 43 fields**. Startup, instance
publication, device recheck, captured registry teardown and logging properties
remain checked. These preserve existing contracts, not new canonical credit or
recertification of different graphs.

Music control-flow checks cover initial-null, stop-produced-null and retained-
nonnull paths. Five target mutations and the previous missing-gate native image
are rejected. These decoded control-flow/source-slot checks do not establish
runtime behavior of the remaining destroy facade or overlapping data anchors.
Prior failed audits and supplemental proofs remain unchanged in their original
[logging](../winmain-logging-20261024/README.md) and
[lifecycle](../winmain-lifecycle-20261024/README.md) evidence packets.

## Retained receipts and remaining work

Ignored packets below `.analysis/gpt-dots/`, not supplied by a Git clone:

- `winmain-music-recheck-20261024/`: `trial-receipt.json`, `trial-root.exit`,
  full inventories, `trial-music-proof.json` and `audit-tests.json`.
- `winmain-music-independent-review-20261024/review-receipt.json`:
  independent first-model replay with 115 artifacts preserved.
- `winmain-stop-playback-20261024/`: `trial-receipt.json`, `trial-root.exit`,
  `trial-stop-report.json`, full callee/parent inventories and preceding guards.
- `winmain-stop-playback-20261024/independent-review/`: `receipt.json` and
  `callee-baseline-comparison.json`, separating parent/receiver validation from
  unchanged nonexact callee codegen.

Full CI passed on frozen final inputs before checkpointing. Further parent/callee
work requires a new target-backed falsifiable model, not repeating neutral
spelling controls. Original source identities, production TU and shared-global/
runtime closure remain unproven; neither owner receives exact credit here.
