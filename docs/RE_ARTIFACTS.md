# TH10 private inputs and reclaimable storage

**Updated 2026-11-22.** Start with [RE_HANDOFF.md](RE_HANDOFF.md). Keep the private target, the toolchain, the analyzer database, configured comparison outputs and the scratch that open owners still need. Remove only unconfigured or reproducible trials, after checking `config/match-units.toml` and the worktree.

## Retain

- `resources/th10.exe`: the private original Japanese TH10 v1.00a target (SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`), read-only and ignored.
- `.tools/` (about 585 MiB), including the VC7.1 SP1 toolchain and this game's Wine prefix; `ghidra-project/` (about 10 MiB) and the shared Ghidra runtime config; `_reference/` (about 68 MiB).
- Maintained `src/`, `scripts/`, `config/`, Git history and `docs/evidence/`, including the D3D adapter/binding evidence and `.analysis/d3d-*`; the origin-review input under `.analysis/gpt-5.6-sol/20260917-origin-review/`; the optional `.analysis/gpt-web/` generators.
- Every configured `build/match-linked/` directory. There are 273 configured output directories: 246 are built and 27 have not been built yet. No unconfigured directory remains. Also keep `build/match/`, `build/probe-backlog/` and the three `build/gpt-web/` diagnostic workspaces.
- The original Ghidra exports in `.analysis/` (`th10-bullet-transform-decomp.c`, `th10-bullet-transform-disasm.txt`, `th10-run-decomp.c`, `th10-dispatch-decomp.c`, the `AdvanceTransformProgram.*` target dumps). They can be regenerated read-only from the Ghidra project.

## Scratch still pending cleanup

`.analysis/` holds about 955 loose files (about 30 MiB). Most are source snapshots, build logs and compare JSON from the failed `AdvanceTransformProgram`, `SpawnSingleBullet`, `UpdateAbsoluteDirectionChange` and earlier bullet experiments. The useful findings are already in the handoff and [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). Delete each owner's snapshots once that owner is exact or parked.

## Reclaimed

- 2026-11-20: about 14 MiB of unconfigured linked-PE trials, duplicate Ghidra exports, stale assembly dumps, Python bytecode and old Ghidra log content.
- 2026-11-22: about 2.8 MiB. This removed the two unconfigured trial outputs `BulletRuntimeDeactivateRandom` and `BulletTransformProgramSameTU`, plus 132 `BulletCancelArea` snapshots and disassembly dumps that the exact `CancelBulletsInArea` supersedes.

Do not run Git GC, expire reflogs, or purge the retained directories just to shrink the repository. Do not create replacement archives or deletion manifests.
