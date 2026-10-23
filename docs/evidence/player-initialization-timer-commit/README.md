# Player initialization: timer commit scheduling

2026-10-23, starting d586573. The complete1435-byte PlayerInitialize4247F0
improves1359/76 to **1370 matching/65 differences**, after all49 independent
address bindings. No small-function mapping or exact credit is added.

The [draw-owner successor](../player-initialization-draw-owner/README.md)
now reaches1380/55. This packet's1370 source pins and commands are historical.

## Maintained source and full reproduction

[The source patch](maintained.patch) adds a compiler-only read/write barrier
between the actual previous=-2 and current=-1 stores of the first timer.
OR ESI,-1 moves from+19C to target+1B0, and all21 timer-region differences vanish.
Five initializer barriers remain. The scoped owner/vector observations and the
earlier vector repair are retained. This emits no CPU fence, fake value, extra
call or source padding. Original barriers, qualifiers and concurrency are unknown.
The zero-context patch binds the starting source; use `git apply --unidiff-zero`.

Across34 changed replayed bytes,21 target bytes improve and10 target-equal bytes
are lost; three already-different positions also change. The losses are later
vector scratch-store scheduling. The net improvement is11, not21. All remaining
[65 differences and49 bindings](parent-differences.json) are retained. No ordinary
instruction or local destination is masked or cropped. Prefix37, timer0,
vector-region10 and tail18 differences remain.

Eight complete shared guards pass1753 bytes/all99 fields. Private and freshly
compiled maintained Player objects reproduce519 complete public PDB owners
raw/140370 bytes. Versus1359, only PlayerInitialize changes; the other518 public
owners remain raw-identical. [The comparison](public-reproduction.json) binds
both observations. Static/unowned fragments, runtime and target-wide equality
are outside scope. Maintained reproduction rebuilds Player once and reuses16
pinned objects, not a whole-graph cold pair. VM wrapper200/nativeEDX versus
202/targetESI and original source/TU ownership remain open.

## Executed controls

[Evidence](evidence.json) records11 complete1435-byte images and12 compiler
invocations:11 accepted object compiles and one rejected intrinsic object.
All11 complete parents replay49 fields and pass all eight full guards.

| Control | Complete match /1435 | Outcome |
| --- | ---: | --- |
| Draw-owner capture, ordinary / ordered read |1203 each|Regressive; current flags/owner allocation is not preserved |
| Draw-node publication barrier |1358|Two repairs/three losses |
| First-timer commit barrier |1370|Selected;21 repairs/10 losses |
| Ordered cold character read |1351|Regressive |
| Write-only first-timer barrier |1370|Whole replay equal to selected; unadopted |
| Volatile previous-field store |1358|Regressive |
| Two real named vector returns |1370|Whole replay equal to selected; unadopted |
| Calc-snapshot write barrier / actual Player G7 |1370 each|Whole replay equal to selected; unadopted |
| Fresh maintained reproduction |1370|All519 complete public owners reproduce private bytes |
| Read-only timer barrier |No image|Pinned VC7.1 C4163: _ReadBarrier unavailable as intrinsic; object rejected before link |

G7 is present in the actual compiled Player profile with GL/GS/EHs retained.
No unsupported intrinsic is converted to an opaque diagnostic helper.
Headers, layouts, ABI declarations, canonical recipes and exact ledgers stay
unchanged; only one candidate-ledger notes cell records the current result.

## Recovery, cleanup and validation

Current producers: `.analysis/gpt-6.1-sol/player-flag-lifetimes-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and bound-parent-final.py.
The initial1359 manifest remains immutable. Retain both maintained-17 and selected
first-timer-commit-barrier carriers under the corresponding build campaign.

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-flag-lifetimes-20261023/bound-parent-final.py maintained-17
```

Expected mismatch1370/1435,49 fields. Restore required ignored paths before old
consumers, never historical source into src/ and never weaken pins.
[Cleanup](cleanup-summary.json) archives nine closed images, one rejected
intrinsic object tree and ten private source proposals:249 files/18.85MB become
0.40MB, reclaiming18.45MB. All24811 protected-file hashes are unchanged during
compaction. Current carriers, required old caches, source witnesses, manifests,
reports, target, tools, Wine, database and unrelated D3D work are retained.

```sh
tar -xJf .analysis/player-timer-commit-cleanup-20261023/completed-controls.tar.xz -C .
```

[Validation](validation-summary.json) records the focused scope and fresh
read-only, full-mapped-text Ghidra attestation. No independent cold pair, full CI,
new exact acceptance, product closure or runtime validation is claimed.
Authored409/74716 and overall1252/141647 exact counts remain unchanged.
The reconstruction campaign remains active-incomplete.
