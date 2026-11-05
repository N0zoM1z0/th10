# TH10 private reconstruction artifacts — current storage policy (2026-11-05)

See [current agent handoff](RE_HANDOFF.md), [ledger progress](PROGRESS.md)
and the [2026-11-05 final handoff cleanup](evidence/handoff-maintenance-20261105/README.md).
The [first November 5 selective archive](evidence/handoff-storage-20261105/README.md)
and [November 4 cleanup](evidence/handoff-storage-20261104/README.md)
are separately verified **historical events**, not today's live inventory.
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
- `build/gpt-web/`: currently **311 expanded witness files totaling
  14,079,534 file-content bytes**, spanning the 13 retained early-November
  frontier prefixes **plus nine newer selected checkpoint prefixes**.
  Another 7,243 historical files remain recoverable in the first November 5
  archive, and a further **1,581 old/intermediate experiments** are recoverable
  in the [later 2026-11-05 verified archive](evidence/handoff-maintenance-20261105/README.md).
  All of these are candidate artifacts, **not exact credit**. See the latest
  [retention inventory](evidence/handoff-maintenance-20261105/cleanup-summary.json).

Older accepted proofs under `docs/evidence/` are versioned in Git. Their
references to archived `.analysis/gpt-6.1-sol/` paths describe *former*
expanded states, **not live paths after the latest cleanup**. Codegen exact
credit comes from accepted complete-owner ledgers and canonical replay, not
from finding an old PDB with a familiar name.

## 2026-11-05 verified archive of regenerable experiment images

The selected early-November experiment trees under build/gpt-web were cleaned
**without deleting their only recoverable copy**. The new local, ignored
compressed archive is:

    .analysis/archive/gpt-web-regenerable-experiments-20261105.tar.zst

It contains **7,243 independently attested regular files / 254,774,405
expanded payload bytes** and occupies **13,907,599 bytes**. Archive SHA-256:

    c3c38c20fa7c6f303911973988b4582f0d012a62f0dd2c9fbde24fcc9f03ebde

The per-file original path, size and SHA-256 manifest is *private, ignored*:

    .analysis/maintenance-20261105/gpt-web-regenerable-manifest.jsonl.gz

Manifest SHA-256:

    48e168d4b8eb432f1eb493a58c55f2a63b15fc7b1cf0e2dcf719c309b91fe1e4

Before deleting any expanded copy, the compression was decoded as a tar
stream and **every one of the 7,243 members** independently matched the
pre-deletion file size and SHA-256. All source files were then **re-hashed
once more immediately before deletion**. A separate smoke check restored
one archived file to a temporary directory, verified its byte size and
hash, then removed only that temporary copy. The target, toolchain,
Ghidra project, .analysis/gpt-web research scripts and JSON receipts,
all existing archives, and the whole build/match-linked canonical tree
were *not touched*.

At the time of this **first** Nov 5 archive, thirteen frontier witness
subtrees remained expanded, including then-current ECL, ANM, ECL
AddScriptData, Chain allocator, Text, Sound, Popup and Item Draw graphs.
That description is **historical**: subsequent agent work expanded many
additional local graphs, and a second guarded archive retained the current
full ECL/ANM/ECL Run checkpoints. The exact original selection is still
recorded in
[evidence/handoff-storage-20261105/cleanup-summary.json](evidence/handoff-storage-20261105/cleanup-summary.json).
Other old README/source experiment paths may no longer be expanded.
The old date in a directory name does **not** by itself prove which
source/hash epoch it represents.

### Selective read/restore without an all-unit rebuild

Run from the TH10 repository root. These commands need the **private
archive** on disk; they do not work in a Git clone without that transfer.

    zstd -t .analysis/archive/gpt-web-regenerable-experiments-20261105.tar.zst
    zstd -dc .analysis/archive/gpt-web-regenerable-experiments-20261105.tar.zst |
      tar -tf - | grep '^build/gpt-web/20261104-current-popup-creator/' | head

To restore a selected missing experiment directory (do not extract the
whole archive or overwrite existing witnesses without checking):

    zstd -dc .analysis/archive/gpt-web-regenerable-experiments-20261105.tar.zst |
      tar -xf - --wildcards 'build/gpt-web/20261104-current-popup-creator/*'

The tar contains only ignored build/gpt-web paths, never src/, config/,
tools or the original target. The compressed manifest and the tracked
cleanup summary permit full independent SHA-256 checks before using any
restored PE/MAP/PDB. In particular, restored objects from former source
epochs must not be silently mixed into a current VC7.1 graph.

The exact **post-archive** measured reduction in build + .analysis was
**258,570,524 bytes (246.6 MiB)**, including a separately measured
5,809,259 bytes of regenerated Python-build cache removal. Because this
measurement began **after** the new archive was written, it is not an
all-in net claim. Subtracting the new 13,907,599-byte archive gives
approximately **244,662,925 bytes (233.3 MiB) saved overall**, with
additional small manifest/audit-file overhead excluded from that
estimate. The stage-specific before/after du bytes are saved in the
machine-readable cleanup receipt. Do not add the Nov 4 savings to the
Nov 5 savings without accounting for different baselines.

## Later 2026-11-05 next-agent cleanup (current expanded inventory)

After many more VC7.1 source/field experiments, `build/gpt-web` expanded
again from the prior 13 witness prefixes / 199 files to **1,892 files**.
The second, independent [verified handoff cleanup](evidence/handoff-maintenance-20261105/README.md)
archived **1,581** regenerated/older-source experiment files with
**72,321,019 bytes** of original content. All were hashed before archiving;
every regular member was independently size/SHA-256 verified from the
compressed tar; every original file was rehashed again before guarded unlink.
The old 13 protected prefixes plus nine current frontier prefixes were
left expanded: **311 files / 14,079,534 payload bytes**. A verified
single-member restoration confirmed selective recoverability.

New ignored private zstd tar:

    .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst

Compressed size **3,292,104 bytes**; SHA-256:

    b62afedcc09bc2f4e0dbe0724824afa19d71642df4e2fc95828b9f630c50301c

Ignored per-file path/size/SHA-256 manifest:

    .analysis/maintenance-handoff-20261105/extra-graphs-manifest.jsonl.gz

Manifest SHA-256:

    1b94eaa3df6851aaf972595b717b86cae9c2726e1993f98057a27400c8fcc2ce

Inspect or selectively restore a *single* missing old experiment path,
never replace a current-source graph blindly:

```sh
zstd -t .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst
zstd -dc .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst |
  tar -tf - | grep '20261105-anm-layer-probe' | head
mkdir -p /tmp/th10-restore-check
zstd -dc .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst |
  tar -xf - -C /tmp/th10-restore-check -- \
    build/gpt-web/20261105-anm-layer-probe/volatile-layer-value/link-aliases.rsp
```

Separately, **247** automatically regenerable `.pyc` files under
`build/pycache/` were deleted. The measured disk-block reduction across
`build` and `.analysis`, **including the newly created archive**, is
**77,815,808 bytes (~74.2 MiB)** between the recorded immediate before
and after checkpoints. This number must **not** be added to the earlier
November cleanup baseline as though the two measurements were contiguous.

All earlier archives, the private target and tools, source, Ghidra,
`_reference`, current `.analysis/gpt-web` research, tracked evidence and
the full `build/match-linked` canonical tree were preserved. Git alone
cannot transfer the private archive or target. Consult the exact retained
paths and byte totals in
[evidence/handoff-maintenance-20261105/cleanup-summary.json](evidence/handoff-maintenance-20261105/cleanup-summary.json).

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
in .analysis plus build **at the Nov 4 source epoch** was
**347,042,395 bytes (~331 MiB)** including that earlier archive,
expanded-tree deletion and cache pruning. Immediately after the Nov 4
cleanup only, .analysis was about **207 MiB** and build about
**125 MiB**; these are historical intermediate measurements, not current
expanded-storage figures after the Nov 5 selective archive.
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
