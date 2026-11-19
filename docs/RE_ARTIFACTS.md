# TH10 private inputs and reclaimable storage

**Updated 2026-11-19.** The current task brief is [RE_HANDOFF.md](RE_HANDOFF.md). Keep the original target and active reconstruction contexts; remove reproducible scratch only after its useful findings have been recorded.

## Preserve

- `resources/th10.exe` — original Japanese v1.00a target, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep read-only and uncommitted.
- `.tools/` (about 585 MiB), including this game's Wine prefix, pinned VC7.1 compiler/linker and shared-runtime links.
- `ghidra-project/` (about 9.7 MiB), `_reference/`, maintained `src/`, `scripts/`, `config/`, Git history and reflogs.
- `build/match-linked/` (about 183 MiB): 271 configured output directories, 244 present and 27 not yet built. Keep every present linked PE/MAP/PDB and support-object context. Also keep `build/match/` and `build/probe-backlog/`.
- Five unresolved diagnostic workspaces under `build/gpt-web/` (about 4.5 MiB):
  - `20261108-enemy-dispatch-rank-tail-final/`
  - `20261108-enemy-runtime-aggregate-sum/`
  - `20261109/anm-execute-child91-aggregate/`
  - `20261109/ecl-run-length-pointer-order/`
  - `20261109/playerhud-restored-natural-loader/`
- `.analysis/` (about 6.4 MiB), including active ANM/ECL/Enemy decompiler exports, `.analysis/factory-runtime/`, the D3D adapter work and evidence, `.analysis/gpt-5.6-sol/20260917-origin-review/runtime-origin-review.json`, and the four optional trial generators in `.analysis/gpt-web/`.
- `docs/evidence/` compiler contexts and binding contracts referenced by the reconstruction workflow.

## Cleanup on 2026-11-19

Removed **232** superseded experiment files (230 top-level `build/try-*` logs/source snapshots/reproducible comparisons plus two temporary camera relink logs), reclaiming **322,448 bytes**. Kept the two selected cold-link result JSON files for `EnemyMotionView::AdvanceMotion` and `EclVmScriptDatabase::AddScriptData`; their findings are summarized in `KNOWLEDGE_BASE.md` and `RE_HANDOFF.md`. No `build/match-linked/` context, target, toolchain, analyzer project, D3D work or canonical ledger was deleted.

Do not remove the five diagnostic workspaces or the four `.analysis/gpt-web/` trial generators without first replacing the current unresolved-owner evidence. Do not run Git garbage collection or expire reflogs for storage cleanup. Keep new scratch below `.analysis/` and build outputs below `build/`; no replacement archives, deletion manifests or recurring cleanup reports are needed.
