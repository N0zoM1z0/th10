# Handoff cleanup — 2026-10-23

The operator requested code/document cleanup, a smaller workspace and a precise
handoff. Source checkpoint bd2b5a4 is retained. Maintained game code and headers,
compiler profiles, canonical recipes, ledgers and progress counts are unchanged.
The reconstruction campaign remains **active-incomplete**. No new game compile
or Ghidra analysis was started and no TH10 native producer is left running.

The [current handoff](../../RE_HANDOFF.md) now incorporates the recovered
[parameter/partition controls](../player-initialization-parameter-controls/README.md),
distinguishes completed and unexecuted experiments, and identifies the actual
opaque GamePrepareAnimationSlots caller dependency. The older caller packet's
parameter/vector suggestions are superseded explicitly. Generated dumps remain
private; only compact observations and result/provenance summaries are tracked.

## Verified local compaction

| Archived outputs | Files | Before | Archive | Reclaimed payload |
| --- | ---: | ---: | ---: | ---: |
| Closed Player controls and snapshots | 831 | 41.71 MB | 0.79 MB | 40.92 MB |
| Historical host validation outputs | 12495 | 92.00 MB | 0.95 MB | 91.05 MB |
| Total | 13326 | 133.71 MB | 1.74 MB | 131.97 MB |

Sizes are decimal payload bytes, excluding small new documentation and inventory
files. Allocated-block reduction is163.81 MB.

The [cleanup summary](cleanup-summary.json) records exact sizes, archive digests,
selected roots and protected-file checks. Nineteen closed Player build trees and
five full private source/header snapshots are losslessly archived. Two useful
19-input graphs and their VM-owner snapshot remain expanded alongside the current
maintained17-input graph and its selected private witness. Fifteen historical
host-test checks/ output trees are also archived; every recorded completion
.exit is zero, and their source, producers and other reports remain expanded.

Each archive member is read back and checked for length/SHA-256 before any
expanded tree is removed. All other protected files are hash-checked before and
after. The full per-member inventory, protection manifest and cleanup producer
are under `.analysis/handoff-ready-20261023`; archives are ignored local state.
Restore from the repository root before using a historical consumer:

```sh
tar -xJf .analysis/handoff-ready-20261023/player-completed-controls.tar.xz -C .
tar -xJf .analysis/handoff-ready-20261023/historical-host-checks.tar.xz -C .
```

Append an exact original member path from archive-members.json.gz to restore one
file. Previous archives/restoration commands in the current handoff remain valid.
Canonical linked outputs, current large-owner candidates, required support
caches, the original target, game data, toolchains, Wine prefixes and private
Ghidra database are retained. All nine unrelated untracked D3D files remain
unstaged and hash-unchanged; unknown-origin legacy files are retained.

[Validation](validation-summary.json) records actual checks. Full CI and broad
cold replay are omitted under the operator's focused-validation preference for
this documentation/artifact-only change. No new exactness, product, runtime,
semantic or portability credit is claimed.
