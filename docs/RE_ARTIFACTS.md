# TH10 ignored artifacts — live next-agent storage and recovery

**Remeasured 2026-11-09 after the selective native-trial cleanup.**
The 2026-11-09 table below is the current allocation. The 2026-11-08 and
2026-11-07 measurements and deletion audits are preserved as historical
snapshots, not current disk usage. Use [RE_HANDOFF.md](RE_HANDOFF.md) for
the current nonexact frontiers and [PROGRESS.md](PROGRESS.md) for canonical
exact counts.

## Preserve for the new agent

- **Mandatory private inputs:** read-only `resources/th10.exe` (Japanese
  1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`),
  `.tools/` (VC7.1 SP1/Wine), `ghidra-project/`, `_reference/`, private
  resource packs, and every `.analysis/archive/` historical archive.
- **Native evidence:** the complete **`build/match-linked/` canonical
  graph**, current accepted cold images where retained, `.analysis/gpt-web/`
  source snapshots, experiments, JSON receipts, and native MAP/log text.
- **Tracked truth:** `src/`, `config/`, `scripts/`, `docs/`, full Git
  history; all canonical counts come from `config/functions.csv`,
  `config/matches.csv`, and generated `docs/PROGRESS.md`.

These ignored resources **are not in a normal Git clone**. Transfer them
separately if the successor uses a different machine, checking their
SHA-256 and VC7.1 profile. Never mix an old PE/PDB with newer source
hashes, and **never delete the original EXE, private toolchain, canonical
match-linked graph, Ghidra DB, reference tree or historical archives**.

## Current measured storage and selective native prune (2026-11-09)

| Path | Current allocated bytes (du -s -B1) | Approx. MiB | Retention |
| --- | ---: | ---: | --- |
| build/gpt-web/ | **313,556,992** | 299.03 | Eight selected diagnostic roots, nested installed/canonical witnesses, MAP/LOG/RSP/JSON/source snapshots and drivers |
| build/match-linked/ | **134,434,816** | 128.21 | **Preserved canonical build graph** |
| .analysis/gpt-web/ | **132,583,424** | 126.44 | **Preserved source snapshots, drivers and comparison results** |
| .analysis/archive/ | **216,694,784** | 206.66 | **All historical compressed archives preserved** |
| .tools/ | **611,430,400** | 583.11 | **Pinned compiler/toolchain preserved** |
| _reference/ | **315,129,856** | 300.53 | **Original research/reference preserved** |
| ghidra-project/ | **10,088,448** | 9.62 | **Original private Ghidra project preserved** |
| build/pycache/ | removed | — | 250 generated .pyc files removed |

The 2026-11-09 prune reviewed ignored native outputs in build/gpt-web/ and
removed **2,622** obsolete .obj/.exe/.pdb files from non-preserved trial
roots: **2,239 .obj, 191 .exe, 192 .pdb**, totaling **222,426,440
payload bytes**. Allocated usage fell from **540,475,392** to
**313,556,992 bytes**, reclaiming **226,918,400 allocated bytes**. All
MAP/LOG/RSP/JSON/source snapshot files and experiment drivers were retained.

Eight useful diagnostic roots remain intact:
- enemy-runtime-full-ltcg-callers/
- 20261108-enemy-runtime-aggregate-sum/
- 20261108-enemy-dispatch-rank-tail-final/
- 20261108-anm-initialize-vm-gameui/
- 20261108-anm-initialize-vm-gameui-callers/
- 20261108-enemy-begin-spell-20261105-full-snapshot/
- 20261108-anm-binder-complete-real-callers/
- 20261108-anm-binder-real-callers/

Nested installed-cold, accepted-exact and canonical-cold directories,
plus root-level .obj inputs, were preserved. A further **290 Python
bytecode cache files** were removed (5,243,328 payload bytes), including the
4.6 MB build/pycache/ payload. The original target, canonical
build/match-linked/, .analysis/archive/, .tools/, _reference/ and
ghidra-project/ were measured before and after and remained unchanged.
The original target still hashes to
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The private file-by-file audit is
.analysis/maintenance-next-agent-20261109/removed-files.jsonl
(SHA-256 3b9f4f3346ccca96e1bd0556dc48e1c0fb70be44b719b222d8686f0ee99c5bdc);
cleanup-summary.json records the complete before/after measurements.
No backup archive was created. **The manifest is an audit, not a restore
medium.** Rebuild a removed trial from its retained driver/source snapshot.

## Previous measured storage and second deletion audit (2026-11-08; historical)

At that 2026-11-08 checkpoint, the second one-time prune had removed only
rebuildable ignored `.obj/.exe/.pdb` outputs from the fixed 451-root
`build/gpt-web/` snapshot. It reviewed 448 roots and preserved three selected
enemy witness graphs. The retained tree then occupied **262,840,320
allocated bytes (250.7 MiB)**.

| Path | Recorded allocated bytes at that checkpoint (`du -s -B1`) | Approx. MiB | Retention |
| --- | ---: | ---: | --- |
| `build/gpt-web/` | **262,840,320** | 250.7 | Trial logs/MAPs/sources, three witness graphs, 12 installed-cold files and three root `.obj` inputs |
| `build/match-linked/` | **128,806,912** | 122.8 | **Preserved complete canonical build graph** |
| `.analysis/gpt-web/` | **103,477,248** | 98.7 | Preserved drivers, source snapshots, JSON/MAP/logs |
| `.analysis/archive/` | **216,694,784** | 206.7 | **All historical compressed archives preserved** |
| `.tools/` | **611,315,712** | 583.0 | **Pinned compiler/toolchain preserved** |
| `_reference/` | **315,129,856** | 300.5 | **Original research/reference preserved** |
| `ghidra-project/` | **10,088,448** | 9.6 | **Original private Ghidra project preserved** |
| `build/pycache/` | **5,312,512** | 5.1 | Regenerable Python bytecode cache |

The cleanup removed **12,051 native files** with 869,252,724 payload bytes
and reclaimed **886,214,656 allocated bytes (845.2 MiB)**, reducing
`build/gpt-web/` from **1,149,054,976 bytes (1,095.8 MiB)** to the current
262,840,320 bytes. It retained these three roots intact:

- `build/gpt-web/enemy-runtime-full-ltcg-callers/` — reusable 11-TU base graph;
- `build/gpt-web/20261108-enemy-runtime-aggregate-sum/` — current best
  EnemyRuntimeUpdate/EnemyAdvanceMotion witness;
- `build/gpt-web/20261108-enemy-dispatch-rank-tail-final/` — latest dispatcher
  witness.

The 448 reviewed roots retain their non-native MAP/log/response/source/JSON
files where present. Nested `installed-cold`, `accepted-exact` and
`canonical-cold` paths were excluded. Sixty native files remain: 45 in the
three selected witness graphs, 12 under two `installed-cold` paths and three
root-level `.obj` inputs. The separate `.analysis/gpt-web/` driver and source-
snapshot tree was not pruned. The canonical graph, original target, private
toolchain, Ghidra/reference inputs, `.analysis/gpt-web/` and every historical
archive were verified unchanged. The tracked [cleanup evidence](evidence/next-agent-storage-prune-20261108b/README.md)
and [machine-readable summary](evidence/next-agent-storage-prune-20261108b/cleanup-summary.json)
record the before/after measurements and protected identities.

The private per-file deletion inventory is
`.analysis/maintenance-next-agent-20261108b/removed-native.jsonl`
(SHA-256 `53406dc8c0da3d57bf543859482cdb6950e12a4beb6b25c14735e6636a9100eb`);
its adjacent `cleanup-result.json` contains the complete private audit.
No archive was made. **The manifest is an audit, not a backup.** Rebuild a
removed trial from its retained driver and source snapshot if needed.

The opt-in script
[`scripts/prune-next-agent-native-20261108b.py`](../scripts/prune-next-agent-native-20261108b.py)
was dry-run first and **its `--apply` has already completed once**. Do not
rerun it. It is pinned to the reviewed root inventory and is not a general
purpose cleanup tool.

## Previous 2026-11-08 first prune (historical; superseded by the second pass)

The earlier same-day bounded pass removed **3,786** regenerable `.obj/.exe/.pdb`
files from 57 allowlisted ignored `build/gpt-web/` roots. It reclaimed
**274,890,752 allocated bytes (262.2 MiB)**, taking the tree from
572,641,280 to 297,750,528 bytes at that earlier point in time. Its private
manifest is `.analysis/maintenance-next-agent-20261108/removed-native.jsonl`
(SHA-256 `a309a6e1c748e2a388a4d5c1f3a0f54f8f3600018e1cb27edc187a0c652a539f`);
its adjacent `cleanup-result.json` is the corresponding receipt. Its tracked [historical note](evidence/next-agent-storage-prune-20261108/README.md)
records that pass. The first script
`scripts/prune-next-agent-native-20261108.py` was already applied; do not
rerun it. This historical snapshot is not the current allocation.

## Previous 2026-11-07 allocation snapshot (historical; superseded)


At the older 2026-11-07 source/disk epoch, after a *different*
reviewed deletion batch (not the current allocation):

| Directory | Allocated bytes | Approximate MiB | Policy |
| --- | ---: | ---: | --- |
| `build/gpt-web/` | 296,566,784 | 282.8 | Rebuildable probe outputs; selected witnesses remain |
| `build/match-linked/` | 127,463,424 | 121.6 | **All preserved** |
| `.analysis/gpt-web/` | 74,817,536 | 71.4 | Trial source, results, drivers and compact receipts preserved |
| `.analysis/archive/` | 216,694,784 | 206.7 | **All nine compressed archives preserved** |
| `.tools/` | 610,549,760 | 582.3 | Pinned compiler/toolchain, **all preserved** |
| `_reference/` | 315,129,856 | 300.5 | Original research reference, **all preserved** |
| `ghidra-project/` | 10,088,448 | 9.6 | Original database, **all preserved** |
| `build/pycache/` | 5,287,936 | 5.0 | Generated Python bytecode; may be cleared later |

These numbers are allocated `du -s -B1` values. Only
`build/gpt-web/` was modified during this new binary prune; the protected
canonical/native/toolchain/archives were verified unchanged by allocation
and representative SHA-256 identities. `build/gpt-web/` immediately
before this batch measured **468,172,800 bytes** (446.5 MiB).

## Previous rejected native experiment cleanup (2026-11-07; historical)

- **21 specifically reviewed rejected/superseded probe families** under
  ignored `build/gpt-web/`, not a blanket build-directory deletion.
- **2,063** `.exe`, `.obj`, `.pdb` files removed, totaling **168,744,891
  payload bytes**. Exact measured allocated reclamation:
  **171,606,016 bytes (163.7 MiB)**, from 468,172,800 to 296,566,784.
- The file-by-file path, size and SHA-256 deletion audit is the private
  ignored `.analysis/maintenance-next-agent-20261107/removed-native.jsonl`
  (SHA-256 `92741ed493aee801bf09218f796c9ac380499b412ff02dff2b3326f3cf326710`).
  The protected original/canonical hashes and before/after accounting are in
  `.analysis/maintenance-next-agent-20261107/cleanup-result.json`.
- The inspected, **allowlisted** one-time driver is
  `scripts/prune-rejected-native-trials.py`. Default mode was a dry run;
  its `--apply` completed once. **Do not rerun `--apply`**. All relevant
  trial scripts, JSON, MAP and logs were left in place.
- **No new backup archive was created.** A list of deleted SHA hashes is
  proof of what was deleted, **not a way to restore an old binary**. If a
  rejected control becomes relevant, rebuild with the retained
  `.analysis/gpt-web/` driver, the matching old source snapshot and pinned
  compiler/profile; use a separate output directory. Do not cite a deleted
  `.exe/.pdb` as if it were present.

[Machine-readable cleanup summary](evidence/next-agent-storage-prune-20261107/cleanup-summary.json)
records the scope and measured sizes. A prior independent cleanup deleted
1,342 other rejected native binaries and Python caches at an **older**
source checkpoint; those historical numbers remain in the
[earlier 2026-11-07 evidence](evidence/agent-handoff-maintenance-20261107/README.md).
Likewise the [2026-11-06 handoff cleanup](evidence/agent-handoff-maintenance-20261106/README.md)
and [older archives](evidence/handoff-storage-20261106/README.md)
are historical, not the latest storage inventory.

## Private archive registry

These ignored archives were verified present and had SHA-256s rechecked on
2026-11-07. They remain **unmodified**; none was duplicated in this cleanup.

| Private `.analysis/archive/` filename | SHA-256 | Provenance |
| --- | --- | --- |
| `gpt-web-handoff-controls-20261106.tar.zst` | `574dcaa16c912444b241974f8b43593f3ef5ae2f8b181e6a86064b2ba70b44ec` | Handoff controls: 731 verified native members, **14,075,697 bytes**; [cleanup receipt](evidence/agent-handoff-maintenance-20261106/README.md) |
| `gpt-web-rejected-native-20261106.tar.zst` | `cd9d92d88606ca46060eed0154c140d3971e6bfdcb6436b9db05df31a3992ed5` | Earlier verified 367-member native trial archive, **3,158,740 bytes** |
| `gpt-web-transient-frontiers-20261105.tar.zst` | `d99f7572d6d1ab66efac495c5091c32b5b4499bc3ea9102fbd7f26b004d130a6` | [Nov 5 current-source-era cleanup](evidence/handoff-current-cleanup-20261105/README.md) |
| `gpt-web-regenerable-experiments-20261105.tar.zst` | `c3c38c20fa7c6f303911973988b4582f0d012a62f0dd2c9fbde24fcc9f03ebde` | [Earlier selected archive](evidence/handoff-storage-20261105/README.md) |
| `gpt-web-extra-graphs-20261105.tar.zst` | `b62afedcc09bc2f4e0dbe0724824afa19d71642df4e2fc95828b9f630c50301c` | [Handoff maintenance](evidence/handoff-maintenance-20261105/README.md) |
| `historical-compiler-graphs-20261104.tar.zst` | `7917cbed088dbff5d12ec914e33975c79fc9999bc6ded0266b94da0a8472e513` | [Nov 4 detailed cleanup](evidence/handoff-storage-20261104/README.md) |
| `historical-analysis-20261028.tar.zst` | `b7cf15be7f5f243e6b5bc0aeb9a0fa69924da3364dac8e27e5390aff27426e67` | Earlier private analysis source-epoch archive |
| `terminal-experiments-20261030.tar.xz` | `d1fb67af025897e2facf375b8f6e1a62646bd0fcea303a95586a791befd2179c` | Earlier terminal experiments |
| `terminal-experiments-20261031.tar.xz` | `8b2b1c573c499083cd632691d9085b5e17307dea922c9d4ae0303e698964e917` | Earlier terminal experiments |

## Selective recovery (old archived experiments only)

For the historical 2026-11-06 archive, extract to a private staging tree,
not into live `build/match-linked/` or a current-source compiler directory:

```sh
zstd -t .analysis/archive/gpt-web-handoff-controls-20261106.tar.zst
mkdir -p .analysis/native-restore-handoff
zstd -dc .analysis/archive/gpt-web-handoff-controls-20261106.tar.zst |
  tar -xf - -C .analysis/native-restore-handoff -- \
    build/gpt-web/20261106-replay-initialize/continues-snapshot/source.pdb
```

Compare each restored member to the original private manifest and old
profile/source pins. This archive **does not contain** the 2,063 new
unarchived deletions. Rebuild those from their retained drivers instead.
To verify current sources, run
`scripts/repo-python scripts/verify-handoff-pins.py`. For documentation or
storage-only changes run `scripts/repo-python scripts/ci.py` and
`git diff --check`; **do not cold replay all units**. Use targeted selected
cold builds only after a real source change. Never push.
