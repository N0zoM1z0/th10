# Reconstruction artifacts and restoration

This index describes private local working state; the
[current handoff](RE_HANDOFF.md) is the continuation entry. Archives below are
ignored and are not part of a Git clone. Keep the archives and their inventories
when transferring this workspace. Historical input pins remain immutable.

## Expanded continuation inputs

The current Player producer is
`.analysis/gpt-6.1-sol/player-residual-publication-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and
bound-parent-final.py. Its initial manifest and older producers bind historical
source, not the current source.

| Carrier | Purpose |
| --- | --- |
| `build/gpt-6.1-sol/player-residual-publication-20261023/maintained-17` | Current1359/1435,76 differences,all49 bindings |
| `build/gpt-6.1-sol/player-stage-caller-20261023/max0-ordered-named-result` | Selected private witness reproducing current bytes |
| `build/gpt-6.1-sol/player-stage-caller-20261023/maintained-17` | Previous1355/80 cache |
| `build/gpt-6.1-sol/player-stage-caller-20261023/calc-snapshot-before-owner` | Previous owner-capture private witness |
| `build/gpt-6.1-sol/player-stage-caller-20261023/all-wrapper-callers-27` | Closed caller-visibility graph retained as a support cache |
| `build/gpt-6.1-sol/stage-initializer-formals-20261023` | Earlier VM-binding private/maintained witnesses |
| `build/match-linked/StageRealCallback` | Maintained Stage diagnostic graph |
| `build/match-linked/EnemyItemDrops` | Maintained large ECL/ANM/Enemy diagnostic graph |
| `build/gpt-6-astra-medium/enemy-selection-graph-20261022/scoped-rank-shared-aim` | Unadopted14396-byte Enemy candidate |

Other required Player caches remain under player-chain-members, player-schedule
and player-services campaigns. Their manifests record actual dependencies;
carrier names alone do not establish compatible inputs. All canonical linked
outputs, current large-owner candidates and required source snapshots remain.
Tools, Wine state, the private Ghidra database, original target and game data
are retained. The nine unrelated D3D files remain untracked and unstaged.
Unknown-origin artifacts and incomplete probe caches are retained.

## Archive index

All sizes below are decimal payload MB. Each receipt binds the archive digest,
member lengths/hashes, producer and preservation scope. Compaction preserves the
exact historical files; it does not make a stale report current or grant exactness.

| Archive group | Receipt | Before → archive |
| --- | --- | ---: |
| Historical complete LTCG probe caches,85 trees | [Handoff compaction](evidence/handoff-artifact-compact-20261023/README.md) |64.85 →2.28 |
| Player vector-order controls,16 builds/15 private source proposals | [Vector ordering](evidence/player-initialization-vector-order/cleanup-summary.json) |32.06 →0.42 |
| Player owner-capture controls and two type snapshots | [Owner capture](evidence/player-initialization-owner-capture/cleanup-summary.json) |34.16 →0.88 |
| Earlier Player controls/snapshots and host outputs | [Earlier handoff](evidence/handoff-ready-20261023/README.md) |133.71 →1.74 |
| Player caller controls and source/header snapshots | [Caller controls](evidence/player-initialization-caller-controls/cleanup-summary.json) |33.40 →0.73 |
| Player ordering controls | [Ordering](evidence/player-initialization-ordering/README.md) |24.99 →6.82 |
| Historical RNG snapshot trees | [RNG cleanup](evidence/handoff-final-20261023/README.md) |51.22 →10.54 |
| Historical compiler reports | [Compiler reports](evidence/handoff-cleanup-20261023/README.md) |190.63 →12.62 |
| Historical GUI reports | [GUI reports](evidence/handoff-cleanup-20261022/README.md) |130.87 →6.50 |

The RNG receipt also records removed duplicate objects/Python caches and Git
packing. Packing preserved refs and reachable/reflog history; no pruning or
history rewrite was requested. Other receipts separately record intermediate
removal. Do not add those historical savings to the current batch's savings.

## Restore only required historical inputs

Run the appropriate command from the repository root. Append one exact original
member path from the inventory to restore a single file. Inspect member names
with `tar -tJf` for xz or `tar -tzf` for gzip. Restore only ignored paths; never
extract old maintained source into src/ or relax source/target pins.

```sh
tar -xJf .analysis/handoff-artifact-compact-20261023/historical-probe-caches.tar.xz -C .
tar -xJf .analysis/player-vector-order-cleanup-20261023/completed-controls.tar.xz -C .
tar -xJf .analysis/player-owner-capture-cleanup-20261023/completed-controls.tar.xz -C .
tar -xJf .analysis/handoff-ready-20261023/player-completed-controls.tar.xz -C .
tar -xJf .analysis/handoff-ready-20261023/historical-host-checks.tar.xz -C .
tar -xJf .analysis/player-caller-cleanup-20261023/completed-controls.tar.xz -C .
tar -xzf .analysis/handoff-player-ordering-20261023/control-builds.tar.gz -C .
tar -xzf .analysis/handoff-final-20261023/historical-rng-snapshots.tar.gz -C .
tar -xzf .analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz -C .
tar -xzf .analysis/handoff-cleanup-20261022/historical-gui-reports.tar.gz -C .
```

Historical consumers may refer to files moved into these archives. Restore those
files before rerunning the consumer. Rebuilding from today's source is a new
experiment and does not reproduce a historical source-bound receipt.
