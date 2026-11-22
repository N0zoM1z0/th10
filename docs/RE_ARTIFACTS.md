# TH10 private inputs and reclaimable storage

**Updated 2026-11-22.** Start with [RE_HANDOFF.md](RE_HANDOFF.md). Keep the private target, the toolchain, the analyzer database and the configured comparison outputs. Remove only unconfigured or reproducible trials, after checking `config/match-units.toml` and the worktree.

## Retain

- `resources/th10.exe`: the private original Japanese TH10 v1.00a target (SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`), read-only and ignored.
- `.tools/` (about 585 MiB), including the VC7.1 SP1 toolchain and this game's Wine prefix; `ghidra-project/` (about 10 MiB) and the shared Ghidra runtime config; `_reference/` (about 70 MiB, source only).
- Maintained `src/`, `scripts/`, `config/`, Git history and `docs/evidence/`, including the D3D adapter/binding evidence and `.analysis/d3d-*`; the origin-review input under `.analysis/gpt-5.6-sol/20260917-origin-review/`; the optional `.analysis/gpt-web/` generators.
- `build/match-linked/` (about 185 MiB): 273 configured output directories, 246 of them built, and no unconfigured directory. The comparator reads each directory's `source.exe`, map, PDB and objects, so keep them whole. Also keep `build/match/` and `build/probe-backlog/`.
- In `.analysis/` (about 6 MiB): the read-only Ghidra exports (`th10-*`, `dispatch-ecl-*`, `game-reset-gui.c`, `transition-shake-linear.c`) and `startscript/replay-units.txt`, the replay selector list cited by the handoff. All exports can be regenerated from the Ghidra project.

## Reclaimed

- 2026-11-20: about 14 MiB of unconfigured linked-PE trials, duplicate Ghidra exports, stale assembly dumps, Python bytecode and old Ghidra log content.
- 2026-11-22: about 33 MiB in total:
  - two unconfigured linked-PE trial outputs;
  - about 900 loose experiment files in `.analysis/`: source snapshots, build logs, compare JSON and candidate disassembly for bullet, transform, ECL and ANM trials;
  - the `runtime-baseline-backup` source copy;
  - the three dated `build/gpt-web/` diagnostic workspaces;
  - the `build/anm-caller-abi-test` probe.

  The useful findings are in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md).

Do not run Git GC, expire reflogs, or purge the retained directories just to shrink the repository. Do not create replacement archives or deletion manifests.
