# TH10 next-agent handoff, source-epoch audit and verified transient cleanup

Audit date: **2026-11-05**, following the first handoff correction commit
`87a1806`. See [current next-agent handoff](../../RE_HANDOFF.md),
[private artifact/restore policy](../../RE_ARTIFACTS.md), and the machine-
readable [full cleanup receipt](cleanup-summary.json). This task **did not
change source code, canonical match ledgers, exactness or runtime behavior**.

## What was corrected for the incoming agent

The old `RE_HANDOFF.md` front page called `0dd8b20` the most recent code
checkpoint, even though current installed sources were already at
`2061ddf`. The new handoff now pins that HEAD and the seven checked source
SHA-256 values. It consolidates the latest full ANM `ExecuteScript`
**9,962/9,964**, Enemy ECL dispatcher **14,352 candidate versus 14,416
original**, ECL `Run` **6,997/7,020**, and Bullet absolute direction
**333/335 preliminary ordinary positions**; every one is **NONEXACT**.
The `EnemyBeginSpell` candidate is **2,432**, not the older 2,424.
The three latest actual source recoveries preserve selected existing exact
unit batches of **1,323**, **432**, and **312 bytes**, not newly counted
functions. The canonical ledger remains **1,482 exact functions /
189,797 exact codegen bytes**. The failed x87/register experiments are
recorded with their source status and exact binding limitations; old
experiment logs are historical, not the live scorecard.

The previous artifact README called **311 retained expanded files /
14,079,534 bytes** "current". This was accurate *immediately after the
previous cleanup*, but became stale as subsequent VC7.1 builds expanded
`build/gpt-web` to **1,453 files / 56,594,554 content bytes**. The
latest full inventory is below.

## Safely archived and pruned generated build outputs

- Reused the **22 path prefixes** specifically protected in the prior
  cleanup, and additionally kept **8 current critical witness prefixes**:
  ANM five-source original-bound, Enemy two-source and full eleven-source,
  Bullet seven-source, ECL VM thread-signal, two vector-acceleration
  305/303-byte diagnostic frontiers, and Sound current five-source.
  These remain available in full as PE, OBJ, PDB, MAP and receipts.
- Before deletion, independently SHA-256-hashed **1,016 other generated
  regular files** inside ignored `build/gpt-web`, totaling
  **36,775,436 original payload bytes**.
- Wrote and compressed those exact files into a private zstd tar,
  confirmed zstd integrity, then **decompressed and checked every one of
  the 1,016 member paths, byte lengths and SHA-256 hashes** against the
  private original-path manifest. Also extracted one member to a real
  temporary file and verified it again.
- **Rehashed every original again just before unlinking**, removed exactly
  those 1,016 verified expanded copies, removed empty directories, and
  deleted **247 purely regenerable `build/pycache/*.pyc`** files
  (4,581,025 payload bytes). No source, `.analysis/gpt-web` script/JSON,
  other archive, original game, toolchain, Git history or canonical
  `build/match-linked` producer was deleted.
- Current expanded `build/gpt-web`: **437 regular witness files** with
  **19,819,118 payload bytes**. Actual before-to-after **allocated disk
  block reduction for `build` plus `.analysis`**, *including the new
  compressed archive*, is **42,033,152 bytes (about 40.1 MiB)**. This is
  one separately measured cleanup, not cumulative savings from earlier
  November campaigns.

### Private archive and recovery

Archive (ignored; *not* in Git):

    .analysis/archive/gpt-web-transient-frontiers-20261105.tar.zst

Compressed **2,310,433 bytes**; SHA-256:

    d99f7572d6d1ab66efac495c5091c32b5b4499bc3ea9102fbd7f26b004d130a6

Private per-file original-path/size/SHA-256 manifest:

    .analysis/maintenance-next-agent-20261105/transient-graphs-manifest.jsonl.gz

Manifest SHA-256:

    a230b5fe213938bb29421d14f2a476da5ff20d322108f3c609b78396c1b947fc

To inspect or restore **one historical member** without replacing current
binaries (from TH10 repository root):

```sh
zstd -t .analysis/archive/gpt-web-transient-frontiers-20261105.tar.zst
zstd -dc .analysis/archive/gpt-web-transient-frontiers-20261105.tar.zst |
  tar -tf - | grep '^build/gpt-web/20261105-anm-const-layer-ref/'
mkdir -p /tmp/th10-restore-evidence
zstd -dc .analysis/archive/gpt-web-transient-frontiers-20261105.tar.zst |
  tar -xf - -C /tmp/th10-restore-evidence -- \
    build/gpt-web/20261105-anm-const-layer-ref/link-aliases.rsp
```

The tested member's SHA-256 is
`d5e21f3c0fb22b4a002eb312ea303b2dda8739ae9d41f3621fbd56431cb410ee`.
Do not extract old objects over the installed current-source graph; if the
source/header epochs differ, rebuild a specifically named graph. The
reproducible cleanup/verification driver and detailed log are retained
under ignored `.analysis/maintenance-next-agent-20261105/`.

**Important for handoff:** Git transfers the tracked source, evidence README
and JSON cleanup receipt, **not** ignored private archives, original EXE,
Wine/VC7.1 toolchain or Ghidra. Another machine needs an explicitly
arranged transfer of those inputs and their SHA-256 verification. No
historical full cold replay was executed.
