# Reconstruction artifacts and restoration

This index describes private local working state; the
[current handoff](RE_HANDOFF.md) is the continuation entry. Archives below are
ignored and are not part of a Git clone. Keep the archives and their inventories
when transferring this workspace. Historical input pins remain immutable.

## Expanded continuation inputs

The current Player producer is
`.analysis/gpt-6.1-sol/player-selection-storage-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and
bound-parent-final.py. Its initial manifest and older producers bind historical
source, not the current source.

| Carrier | Purpose |
| --- | --- |
| `build/gpt-6.1-sol/player-value-lifetimes-20261023/current-callers-27` | Current-source27-input caller control;1380/55,unadopted context |
| `build/gpt-6.1-sol/player-value-lifetimes-20261023/current-winmain-entry-27` | Same27 inputs/actual WinMain entry;1380/55,not product closure |
| `build/gpt-6.1-sol/player-selection-storage-20261023/maintained-17` | Current1380/1435,55 differences,all49 bindings |
| `build/gpt-6.1-sol/player-selection-storage-20261023/draw-typed-chain-owner` | Selected private witness reproducing current bytes |
| `build/gpt-6.1-sol/player-flag-lifetimes-20261023/maintained-17` | Previous1370/65 cache |
| `build/gpt-6.1-sol/player-flag-lifetimes-20261023/first-timer-commit-barrier` | Previous selected timer-commit witness |
| `build/gpt-6.1-sol/player-residual-publication-20261023/maintained-17` | Previous1359/76 cache |
| `build/gpt-6.1-sol/player-stage-caller-20261023/max0-ordered-named-result` | Previous selected vector-order witness |
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

The latest ECL campaign is .analysis/gpt-6.1-sol/ecl-typed-values-20261023.
Its manifest, actual plans/patches, strict parent/guard reports, producers and
unexecuted format-before-math/format-before-frame sources remain expanded.
Eight closed images/private source proposals/bulky structural reports and two
failed-link trees are archived. The maintained EnemyItemDrops carrier remains.
Thirteen older Player services control images are also archived; maintained-17,
full-services-17, bound-services-reviewed-15 and prior-chain-canonical are kept.

Repeated src/config/scripts/build subtrees from19 terminal historical workers
(Bullet1454, ECL baseline, shot-type/replay/difficulty/ANM pairs and Enemy spell
controls) are archived. Outer manifests, receipts, runners and resource inputs
remain. Restore the historical subtrees before using those workers; preserving
an old result does not make it current or passing.

## Archive index

All sizes below are decimal payload MB. Each receipt binds the archive digest,
member lengths/hashes, producer and preservation scope. Compaction preserves the
exact historical files; it does not make a stale report current or grant exactness.

| Archive group | Receipt | Before → archive |
| --- | --- | ---: |
| Closed ECL/Player controls and19 historical worker snapshot families | [Latest handoff cleanup](evidence/handoff-ecl-cleanup-20261023/cleanup-summary.json) |109.04 →1.57 |
| Player value/copy controls and refused entry partial output | [Value/caller controls](evidence/player-initialization-value-controls/cleanup-summary.json) |29.93 →0.48 |
| Player draw-owner/table controls,14 images/14 source proposals | [Draw owner](evidence/player-initialization-draw-owner/cleanup-summary.json) |28.14 →0.41 |
| Player timer-commit controls and rejected intrinsic attempt | [Timer commit](evidence/player-initialization-timer-commit/cleanup-summary.json) |18.85 →0.40 |
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
tar -xJf .analysis/handoff-ecl-cleanup-20261023/closed-probes-and-snapshots.tar.xz -C .
tar -xJf .analysis/player-value-controls-cleanup-20261023/completed-controls.tar.xz -C .
tar -xJf .analysis/player-draw-owner-cleanup-20261023/completed-controls.tar.xz -C .
tar -xJf .analysis/player-timer-commit-cleanup-20261023/completed-controls.tar.xz -C .
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
