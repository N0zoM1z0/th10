# TH10 next-agent native-trial storage prune (2026-11-08, second pass)

A fixed, reviewed snapshot of `build/gpt-web/` contained 451 immediate
experiment roots. The one-time cleanup removed only rebuildable `.obj`,
`.exe` and `.pdb` outputs from 448 roots. It retained the reusable base graph
and the two latest selected Enemy witnesses:

- `build/gpt-web/enemy-runtime-full-ltcg-callers/` — pinned 11-TU enemy base
  graph used for targeted rebuilds;
- `build/gpt-web/20261108-enemy-runtime-aggregate-sum/` — best measured
  `EnemyRuntimeUpdate` candidate (2,412 bytes; 1,582/2,136 comparable bytes)
  and `EnemyAdvanceMotion` candidate (168 bytes; 133/140 ordinary bytes);
- `build/gpt-web/20261108-enemy-dispatch-rank-tail-final/` — latest dispatcher
  candidate (14,416 bytes; 1,252/11,536 comparable bytes).

**12,051 native files** were removed. Their payload total was
869,252,724 bytes; allocated reclamation was **886,214,656 bytes
(about 845.2 MiB)**. The allocated `build/gpt-web/` footprint fell from
1,149,054,976 bytes (about 1,095.8 MiB) to 262,840,320 bytes
(about 250.7 MiB). The 448 reviewed roots retain their non-native MAP,
log, response, generated-source and JSON files where present. Nested
`installed-cold`, `accepted-exact` and `canonical-cold` paths were excluded.
Sixty native files remain: 45 in the three selected witness graphs, 12 under
two `installed-cold` paths, and three root-level `.obj` inputs. Their paths
and SHA-256 values are included in `cleanup-summary.json`. The separate
`.analysis/gpt-web/` driver/source-snapshot tree was not pruned.

`build/match-linked/`, `resources/th10.exe`, `.tools/`, `_reference/`,
`ghidra-project/`, `.analysis/archive/` and the three selected witness graphs
were SHA/size protected and verified unchanged. No historical archive was
created. The per-file deletion inventory remains in the ignored private
manifest `.analysis/maintenance-next-agent-20261108b/removed-native.jsonl`
(SHA-256 recorded in `cleanup-summary.json`). **That manifest is an audit,
not a backup.** Regenerate a removed binary from its retained source snapshot
and driver if it becomes relevant again.

The fixed-snapshot one-time script is
`scripts/prune-next-agent-native-20261108b.py`. It is dry-run by default and
its `--apply` has already completed; **do not rerun `--apply`**. It refuses a
changed root inventory, a dirty Git tree, unsafe paths or an existing report.

The earlier 2026-11-08 cleanup (3,786 files from 57 roots) is a separate,
older pass; its private receipt is `.analysis/maintenance-next-agent-20261108/`.
