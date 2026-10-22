# Handoff cleanup receipt — 2026-10-22

The operator requested cleanup and handoff and cleared the active goal. The last
already-running ECL experiment completed; no additional experiment was started.
Maintained game code, exact ledgers, canonical build recipes, private target,
Ghidra database and toolchains are unchanged.

## Removed generated carriers

The six [ECL interaction trials](../ecl-neg-storage-interaction/README.md) are
non-improving candidates. Their patches, source copies, producers and complete
reports remain available. All 96 new native artifact files were hash-verified
against the producer records before deletion: 6399286 bytes reclaimed. The
per-file inventory is in that evidence packet.

## Compressed historical reports

28 expanded historical GUI JSON reports (each at least 256 KiB), including the
old 51 MB ASan diagnostic, were archived losslessly. All archive members were
read back and checked for exact length and SHA-256 before deleting their expanded
copies. This preserves evidence while reducing 130873625 bytes to 6501731 bytes,
saving 124371894 bytes. The archive is private ignored working state, not a Git
artifact. [The inventory](archive-inventory.json) records every original path,
length and hash, plus the archive hash.

Archive location:
`.analysis/handoff-cleanup-20261022/historical-gui-reports.tar.gz`

Restore all archived reports from the repository root before rerunning a
historical consumer that needs them:

```sh
tar -xzf .analysis/handoff-cleanup-20261022/historical-gui-reports.tar.gz -C .
```

To restore only one report, append its exact original `.analysis/...json` path
from the inventory to that command. Source snapshots, patches, small reports,
producers and logs were not removed by this archive operation. The cleanup
producer remains in `.analysis/handoff-cleanup-20261022/archive-reports.py`.

## Retained and verified

- Canonical linked outputs, including `build/match-linked/EnemyItemDrops`.
- Useful unresolved ANM, ECL and Enemy candidate carriers, especially the Enemy
  scoped-rank/shared-aim candidate and its inputs.
- All nine unrelated untracked `docs/evidence/d3d-adapter-format/` files, with
  their existing hashes checked and no staging.
- Original target, Ghidra database, toolchains and archive/game data.

Combined net reduction from carrier deletion and report compression:
130771180 bytes (about 130.8 MB decimal, 124.7 MiB), excluding filesystem block
rounding and the small new documentation/inventory files. Focused native guards
passed; final checks cover toolchain declarations, tracking, progress generation,
documentation links and whitespace. No full CI or new exact credit is claimed.
