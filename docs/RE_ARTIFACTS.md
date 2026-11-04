# TH10 private reconstruction artifacts — current storage policy (2026-11-04)

See [current agent handoff](RE_HANDOFF.md), [ledger progress](PROGRESS.md)
and the [2026-11-04 audited cleanup](evidence/handoff-storage-20261104/README.md).
This page describes what is **expanded now**, what is in a **local archive**,
and how to recover it. Older cleanup campaigns are historical, **not** an
instruction to assume their input hashes still match current source.

## What must be kept

The following are **not** expendable scratch outputs:

- `resources/th10.exe`: legally provided, original Japanese v1.00a target,
  verified by `scripts/verify-target.py`; never commit or replace it.
- `.tools/`: pinned VC7.1 SP1 compiler/linker/headers, Wine and related
  toolchain; deleting it would block native function replays.
- `ghidra-project/`, `_reference/`, `resources/private/`, `.analysis/archive/`
  and existing independent D3D research inputs, where present. They contain
  unique local project/input or archived evidence, not reproducible PE
  outputs. Transfer separately if handing off to a different workspace.
- `src/`, `config/`, `scripts/`, `docs/` and the Git repository/history.
- `build/match-linked/`: **retained** selected accepted canonical producers
  (recreatable from `config/match-units.toml` and pinned tools, but useful
  for current comparisons and handoff).
- `.analysis/gpt-web/`: **retained** recent experiment scripts, source
  snapshots and JSON/LOG receipts, including Nov 3 ANM/ECL/laser work.
- `build/gpt-web/`: **retained** selected near-miss/warm experiment outputs;
  source/header epoch must be checked before any reuse. Not an exact Oracle.

Older accepted proofs under `docs/evidence/` are versioned in Git. Their
references to archived `.analysis/gpt-6.1-sol/` paths describe *former*
expanded states, **not live paths after the latest cleanup**. Codegen exact
credit comes from accepted complete-owner ledgers and canonical replay, not
from finding an old PDB with a familiar name.

## Current compressed archive — old source-epoch compiler graphs

Two previously expanded October 30–31 source-pinned trees have been compressed
and **removed from their expanded directories**:

| Former root | Status after 2026-11-04 cleanup |
| --- | --- |
| `.analysis/gpt-6.1-sol/` | Archived, not expanded |
| `build/gpt-6.1-sol/` | Archived, not expanded |

Their combined **11,061 regular files / 333,340,746 file-content bytes** are
in the local, ignored
`.analysis/archive/historical-compiler-graphs-20261104.tar.zst`
(**20,253,646 bytes**, SHA-256
`7917cbed088dbff5d12ec914e33975c79fc9999bc6ded0266b94da0a8472e513`).
Every archived member's **size and SHA-256** was compared to the
pre-cleanup source file before deletion; 11,061/11,061 matched. The ignored,
per-file recovery manifest lives at
`.analysis/maintenance-20261104/historical-compiler-graphs-manifest.jsonl.gz`.
The tracked [machine summary](evidence/handoff-storage-20261104/cleanup-summary.json)
records archive identity, verification and reclaimed bytes.

Inspect or restore *specific* archived paths from the repository root:

```sh
zstd -t .analysis/archive/historical-compiler-graphs-20261104.tar.zst
tar --zstd -tf .analysis/archive/historical-compiler-graphs-20261104.tar.zst \
  | grep 'anm-executor-inline-context-20261030' | head
# Example: restore one historical manifest, not entire old sources over src/:
tar --zstd -xf .analysis/archive/historical-compiler-graphs-20261104.tar.zst \
  -C . .analysis/gpt-6.1-sol/anm-executor-inline-context-20261030/executor-graph.json
```

For an old compiled image, its map/PDB/objects must be restored together
and compared to the **historical** recorded input hashes, source order,
compiler profiles and linker context. Older archives contain previous-source
state, and restoring them **does not make them valid current-source results**.
Never extract historical `src/` or `config/` over the live worktree.
When the target is a currently maintained function, the better default is
rebuilding a named **scoped** unit using the current sources and pins.

The cleanup also removed **unarchived but automatically regenerable**
`build/pycache/` and `build/probe-ltcg/` outputs. They can be recreated by
repository Python or the targeted `probe-ltcg-backlog.py` command; they
contain no required source or target input. The total measured reduction
in `.analysis` plus `build` was **347,042,395 bytes (~331 MiB)** including
archive creation, expanded-tree deletion and cache pruning. Afterward,
`.analysis` was about **207 MiB** and `build` about **125 MiB**.
Do **not** rerun the same cache cleanup and add those bytes to the tally
again if normal compiler activity regenerates them.

## Earlier protected private archives

These archives remain **unchanged and locally preserved**:

| Existing archive | Historical record |
| --- | --- |
| `.analysis/archive/historical-analysis-20261028.tar.zst` | Private index `.analysis/handoff-20261028/historical-file-index.jsonl.gz`; earlier source epoch |
| `.analysis/archive/terminal-experiments-20261030.tar.xz` | [2026-10-30 cleanup](evidence/maintenance-20261030/cleanup-summary.json) |
| `.analysis/archive/terminal-experiments-20261031.tar.xz` | [2026-10-31 cleanup](evidence/maintenance-20261031/README.md) |

Some older docs link to private inventory paths using a relative URL; those
only resolve in the original workspace. Historical cleanup and archive notes
remain in [2026-11-03 audit](evidence/handoff-storage-20261103/README.md),
[2026-11-02 audit](evidence/handoff-storage-20261102/README.md), and
[2026-10-31 maintenance](evidence/maintenance-20261031/README.md).
For a newly connected agent working with only Git, those ignored archives
are **not downloadable from Git** and must be transferred separately by an
authorized person. Do not delete them merely because a previous candidate
is now known nonexact.

## Guardrails for future pruning

1. Inspect `git status`, verify the exact private target, and leave active
   source changes alone. Don't prune multiple agents' state blindly.
2. Prefer regenerable `build/gpt-web/` and cache outputs. For source-epoch
   research files, archive with a per-file checksum manifest **before**
   removing expanded copies. Keep separate archive checksum and retention
   instructions in a tracked report.
3. Never prune `build/match-linked/` automatically or infer its current
   exactness from size. A canonical scoped replay can overwrite/rebuild
   its selected output; only a complete target-bound receipt is evidence.
4. Do not use the default full cold replay as a storage diagnostic; pass
   only affected `--unit` selectors. Run the seven-stage CI after source,
   ledger or reference-document edits. Keep this page and `RE_HANDOFF.md`
   current when changing what is expanded/archived.
