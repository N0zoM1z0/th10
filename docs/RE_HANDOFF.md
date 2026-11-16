# TH10 reconstruction — agent handoff

**Updated 2026-11-16.** Resume from the current `main` HEAD. The source
checkpoint before this documentation pass was `89729769`
(`gpt-web: document TH10 ECL owner measurements`). The tree was clean before
this pass; keep the work local and never push. [AGENTS.md](../AGENTS.md) is the
operational authority, and [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) contains the
durable reverse-engineering details.

## First actions

- Inspect `git status --short --branch`, `git diff` and `git log -8 --oneline`.
  Preserve any work already present; do not reset it.
- Before target-dependent work, run
  `scripts/repo-python scripts/verify-target.py`.
- `config/matches.csv`, `config/match-units.toml` and
  `config/functions.csv` are the ownership/acceptance ledgers. `PROGRESS.md` is
  generated; do not edit it by hand.
- Use the commit identity and `gpt-web:` format specified in `AGENTS.md`.
  Never push.

## Current comparison baseline

The broad `player-handle-collision-hit` graph has 36 support TUs and writes to
`build/match-linked/ClaudeReplayInitLoadScript/`. Its current PE/MAP/PDB was
linked on 2026-11-16 at 14:43, after rebuilding `EclVm.cpp` from the restored
clean source. The `AnmManager.cpp` support object also postdates its source.
This is the useful shared graph for Player, Enemy, ANM and ECL comparisons.

The canonical `PlayerHandleCollisionHit` owner remains accepted at 627/627
bytes with 21 fields and a selected cold replay. In the current linked graph,
the ECL stack leaves compare byte-exact: `Pop` 122/122, `Push` 96/96,
`EnterFrame` 64/64 and `LeaveFrame` 41/41.

## Open owners

| Owner | Current measurement | Next useful action |
| --- | --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @0x40E770` | In the current 36-TU graph: candidate extent 14,492 vs target 14,416; 10,568 differing bytes; `REGRESSED` vs the stored 10,416 residual set. Best separate 12-TU `/GS` diagnostic: 1,545/11,568 comparable bytes. | Continue the rank-five lower bands and operand/register schedule in `build/gpt-web/20261108-enemy-dispatch-rank-tail-final/`. Keep the 12-TU score diagnostic-only; reconcile with the complete 36-TU graph. |
| `AnmRenderManagerView::ExecuteScript @0x43EE30` | Full 9,964-byte extent, two residual bytes at +0x113/+0x115; frontier `OK` (no regression). Retail loads `[ebp+0x20]` into EDX and pushes EDX; candidate uses ECX. | Isolate opcode 91's call-site register lifetime without changing adjacent opcode bodies. The prior local/volatile/direct-read/inline-assembly trials were reverted. |
| `EclVmContext::Run @0x44E1A0` | Full 7,020-byte extent, 23 differing bytes; frontier `REGRESSED` because residual offsets moved. Current examples: +0xEB/+0xF1 register choice, +0x129/+0x12B pointer register, +0x1F9/+0x241 SIB order, +0x403..+0x4CD spill homes, +0xE6D/+0xE79/+0x1584 SIB order and +0x19E5/+0x19E7 tail register. | Reconcile the current residual set against the stored frontier. Do not use `--update` just to accept moved mismatches. Preserve the exact ECL stack leaves. |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange @0x4078E0` | Full 335-byte extent, two residual bytes at +0xD7/+0xDD; frontier `OK`. Retail uses EDX for the angle argument; candidate uses EAX. | Revisit only with a new, falsifiable register-liveness hypothesis. `register` hints were neutral; removing the angle-copy local added two differences; inline assembly regressed the whole owner. All trials were reverted. |
| `GuiMessageVmView::Run @0x415E90` | Latest recorded candidate: 4,463 bytes against a 4,480-byte target; 1,047/1,219 normalized instructions. Measurement epoch 2026-11-14. | Continue portrait operand order, inline clear-count increments and the stage-6/7 merged tails. See the detailed note in `KNOWLEDGE_BASE.md`. |
| `ResultMenuUpdateResult @0x4236F0` | Latest recorded candidate: 3,228 bytes against a 3,227-byte target; 784/907 normalized instructions. Measurement epoch 2026-11-13. | Keep the separate state bodies and resolve the GUI score receiver/frame without adding copies. See `KNOWLEDGE_BASE.md`. |

`check-frontier` status `OK` means only that the recorded residual set did not
regress; it does not mean the owner is exact. `REGRESSED` means the current
candidate differs from the recorded residual offsets, even if the raw mismatch
count is unchanged or lower. Do not refresh a frontier to conceal moved bytes.

## Bounded rebuild and comparison loop

For the shared graph, rebuild only changed translation units:

```sh
scripts/repo-python scripts/diagnostics/triage/quick-relink.py player-handle-collision-hit --dirty src/Changed.cpp
scripts/repo-python scripts/check-frontier.py --no-build --show ecl-run
scripts/repo-python scripts/compare-linked-function.py --unit player-handle-collision-hit --json
```

List every changed TU after source edits; if a shared header changes, include
every affected TU. The relinker reuses all other cached objects, so an omitted
source can silently test stale code. Compare the complete owner, including
resolved fields and relocations. Only a zero-difference candidate proceeds to
an explicit `scripts/repo-python scripts/replay-exact-units.py --unit UNIT
--json` replay. Do not run the full historical cohort or a full cold replay
for routine iteration.

## Retained artifacts

Preserve the original target, `.tools/` and this game's Wine prefix,
`ghidra-project/`, `_reference/`, canonical ledgers, maintained source/scripts,
`build/match/`, `build/match-linked/`, the five diagnostic contexts under
`build/gpt-web/`, the four trial drivers under `.analysis/gpt-web/`, the
independent origin-review result and unrelated `.analysis/d3d-*` work. The
precise retention and scratch-cleanup policy is in [RE_ARTIFACTS.md](RE_ARTIFACTS.md).
