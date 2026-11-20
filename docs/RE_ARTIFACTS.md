# TH10 private inputs and reclaimable storage

**Updated 2026-11-20.** Start with [RE_HANDOFF.md](RE_HANDOFF.md). Temporary compiler logs, duplicate decompiles and unused experimental artifacts can be regenerated; keep the actual target and current verification inputs.

## Retain

- `resources/th10.exe`: private original Japanese TH10 v1.00a target (SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`), read-only and ignored.
- `.tools/` (roughly 585 MiB), including the VC7.1 SP1 toolchain and this game's Wine prefix; `ghidra-project/` (roughly 10 MiB) and the shared Ghidra runtime config; `_reference/` (roughly 68 MiB).
- Maintained `src/`, `scripts/`, `config/`, Git history and `docs/evidence/`, including D3D adapter/binding evidence; the origin-review input under `.analysis/gpt-5.6-sol/20260917-origin-review/`.
- All **configured** `build/match-linked/` linked PE/MAP/PDB and object contexts: 272 configured output directories, **245 currently present**, 27 not yet built. Retain `build/match/`, `build/probe-backlog/` and the five active unresolved `build/gpt-web/` diagnostic workspaces. Preserve the four optional `.analysis/gpt-web/` generators and the active `.analysis/d3d-*` evidence.
- Selected original Ghidra exports in `.analysis/` remain available, including `th10-bullet-transform-decomp.c`, `th10-bullet-transform-disasm.txt`, `th10-run-decomp.c` and `th10-dispatch-decomp.c`. All can also be regenerated read-only from the retained target project.

## Cleanup performed

The 2026-11-20 handoff cleanup reclaimed approximately **3.5 MiB (3,674,270 regular-file bytes)**: two unconfigured, unreferenced linked-output experiments (`BulletRuntimeUpdateTransform`, `AnmManagerRemoveVm`); identical duplicate top-level decompiler exports; obsolete transform alignment/trial scripts and reports; old Python bytecode; and old Ghidra application-log content (retaining the final 64 KiB). No configured match unit, game input, toolchain, analyzer project, D3D evidence, or canonical ledger was deleted.

The remaining dominant storage is required reconstruction context: the toolchain/Wine prefix, configured linked comparison contexts, and reference source. Do not run Git GC, expire reflogs, or purge those directories just to make the repository smaller. If a future owner replaces a private comparison context, record the actionable finding in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) first and clean only demonstrably unreferenced temporary outputs. Do not create replacement archives or deletion manifests.
