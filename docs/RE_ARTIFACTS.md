# TH10 private reconstruction artifacts — current storage policy (2026-11-05)

See [current agent handoff](RE_HANDOFF.md), [ledger progress](PROGRESS.md)
and the [latest verified source-epoch cleanup](evidence/handoff-current-cleanup-20261105/README.md).
The [earlier November 5 handoff cleanup](evidence/handoff-maintenance-20261105/README.md),
[first November 5 selective archive](evidence/handoff-storage-20261105/README.md)
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
- `build/gpt-web/`: **437 currently expanded witness files totaling
  19,819,118 file-content bytes**, from the prior 22 protected path
  prefixes **plus eight more current-source witness prefixes**. An additional
  1,016 transient files have been verified, compressed and pruned in the
  [latest cleanup](evidence/handoff-current-cleanup-20261105/README.md),
  after the earlier **1,581** and **7,243** archived generated files.
  Candidate PE/PDB/OBJ extents are **not exact codegen credit**. See the
  [current source-epoch retention receipt](evidence/handoff-current-cleanup-20261105/cleanup-summary.json).
  These counts are a **2026-11-05 cleanup snapshot**, not a promise that
  later recompiles leave the expanded inventory unchanged.

Older accepted proofs under `docs/evidence/` are versioned in Git. Their
references to archived `.analysis/gpt-6.1-sol/` paths describe *former*
expanded states, **not live paths after the latest cleanup**. Codegen exact
credit comes from accepted complete-owner ledgers and canonical replay, not
from finding an old PDB with a familiar name.

## Latest 2026-11-05 source-epoch pruning (current inventory snapshot)

Following actual later ANM, Enemy, Bullet, Sound and ECL builds, the earlier
**311-file** snapshot grew to **1,453 files / 56,594,554 payload bytes**.
A [new independently verified cleanup](evidence/handoff-current-cleanup-20261105/README.md)
retained all 22 previously protected prefixes and additionally retained:

- `20261105-anm-flags-halfword/current-5-tu` (original-bound 9,964-byte ANM)
- `20261105-enemy-time-lifecycle/{fresh-11-tu,two-source}`
- `20261105-continuation-bullet/runtime-angle-overlay`
- `20261105-ecl-thread-flag-bit/full-run`
- `20261105-vector-accel/{expiry-first-volatiles,expiry-barrier-memory-reload}`
- `20261105-continuation-sound/epoch-current`

Under ignored `build/gpt-web/` only, **1,016 other generated files**
(**36,775,436 uncompressed content bytes**) were individually size/SHA
verified in the new local compressed tar; each original was rehashed before
its expanded copy was removed. A genuine member extraction was checked.
The 247 regenerated Python `.pyc` files in `build/pycache` were also deleted.
Exactly **437 original linked witness files / 19,819,118 payload bytes**
remain expanded. Measured combined `build` plus `.analysis` allocated-block
savings, **including** the new archive, was **42,033,152 bytes (~40.1 MiB)**.
Do not add that number to prior savings from a different baseline.

Newest archive (private/ignored, must be transferred separately if needed):

    .analysis/archive/gpt-web-transient-frontiers-20261105.tar.zst

Compressed **2,310,433 bytes**, SHA-256
`d99f7572d6d1ab66efac495c5091c32b5b4499bc3ea9102fbd7f26b004d130a6`.
The private per-file manifest is
`.analysis/maintenance-next-agent-20261105/transient-graphs-manifest.jsonl.gz`,
SHA-256 `a230b5fe213938bb29421d14f2a476da5ff20d322108f3c609b78396c1b947fc`.
To verify or restore a single old member **without overwriting** current
source-epoch outputs:

```sh
zstd -t .analysis/archive/gpt-web-transient-frontiers-20261105.tar.zst
mkdir -p /tmp/th10-restore-evidence
zstd -dc .analysis/archive/gpt-web-transient-frontiers-20261105.tar.zst |
  tar -xf - -C /tmp/th10-restore-evidence -- \
    build/gpt-web/20261105-anm-const-layer-ref/link-aliases.rsp
```

For detailed initial/final measured disk blocks, surviving witnesses,
verified SHA-256 and the exact archive entry list, see
[tracked machine receipt](evidence/handoff-current-cleanup-20261105/cleanup-summary.json).

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

## Earlier 2026-11-05 next-agent cleanup (historical 311-file snapshot)

At that historical checkpoint, `build/gpt-web` expanded from the prior
13 witness prefixes / 199 files to **1,892 files**.
The second, independent [verified handoff cleanup](evidence/handoff-maintenance-20261105/README.md)
archived **1,581** regenerated/older-source experiment files with
**72,321,019 bytes** of original content. All were hashed before archiving;
every regular member was independently size/SHA-256 verified from the
compressed tar; every original file was rehashed again before guarded unlink.
The old 13 protected prefixes plus nine current frontier prefixes were
left expanded **at that point in time**: **311 files / 14,079,534 payload
bytes**. Later builds and the source-epoch cleanup above superseded those
**current-inventory** numbers. A verified
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
