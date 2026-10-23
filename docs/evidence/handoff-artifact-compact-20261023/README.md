# Handoff artifact compaction — 2026-10-23

The operator requested code/document cleanup and smaller local artifacts.
Recovered Player work is checkpointed at `2c1c753`: the complete1435-byte
initializer matches1359 bytes with76 remaining differences/all49 bindings.
This cleanup changes documentation and ignored caches; game source, headers,
recipes, exact ledgers and progress counts remain unchanged. The reconstruction
campaign remains active-incomplete.

The [handoff](../../RE_HANDOFF.md) keeps current metrics, the active large-owner
route, exclusions and validation limits. Historical archive details now live in
one [artifact and restoration index](../../RE_ARTIFACTS.md). Earlier evidence
packets and their input pins are preserved.

## Verified cache compaction

[The receipt](cleanup-summary.json) records85 complete historical
build/probe-ltcg trees,1229 files:64851867 bytes become2278404 bytes,
reclaiming62573463 bytes/about62.57 MB. This is separate from the preceding
Player controls'31.64 MB saving. Sizes exclude small inventories and new docs.

Selected caches contain complete diagnostic PE/map/PDB outputs and compiler
objects from the repository's LTCG probe family. All direct cache references in
tracked files and scanned private producers/input manifests remain expanded;
32 other trees are retained, including seven incomplete caches. Historical
result dumps are not exhaustively scanned. Their consumers may require restore.
These generated diagnostic images are not the original target or a game product.

Every archive member is read back and checked for exact length/SHA-256 before
expanded files are removed. All24600 protected-file hashes match before and
after compaction. Current candidates, canonical outputs, manifests, producers,
original target, prior archives and all nine unrelated D3D files are retained.
Tools, Wine, database and game data are not modified. No TH10 native producer
is active during compaction; unrelated game processes are left alone.

Private producer, member inventory and protection manifest are retained under
`.analysis/handoff-artifact-compact-20261023`. Restore from the repository root
before using a historical consumer; append an exact member path for one file:

```sh
tar -xJf .analysis/handoff-artifact-compact-20261023/historical-probe-caches.tar.xz -C .
```

[Validation](validation-summary.json) records the focused cached Oracle and
repository checks. No new reconstruction experiment, game compilation or Ghidra
analysis is started for this cleanup. Toolchain smoke is executed for recovery;
full CI and broad cold replay are omitted under the operator's focused-validation
preference. No new exact, product, runtime, semantic or portability credit is claimed.
