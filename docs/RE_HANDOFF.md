# TH10 reconstruction handoff

Updated 2026-10-23 after Player startup's real-dependency/source repair.
Reconstruction remains **active-incomplete**. The maintained initializer now
emits1435 bytes in its measured17-input graph, but is not exact: complete replay
of49 independently bound address fields matches1123/leaves312 differences.
The required .sht loader is source-present, not a standalone exact claim. Headers,
layouts, canonical recipes and exact ledgers remain unchanged. Checkpoint prefix:
`gpt-6.1-sol: ...`.

## Current state

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target 7020 | 7020 bytes; 6993 matching, 27 ordinary differences; 189 fields/62 local destinations; 171-byte float callee exact |
| ANM ExecuteScript / target 9964 | 9964 bytes; nonexact; case 7541/7995, 90/92 target-sized groups, gap 2 |
| Enemy dispatcher / target 14416 | 14336 bytes; frame 2C4; 181 selectors/108 groups; case 3245/11128, 45 target-sized groups, gap 328 |
| StageExecuteScript / target 1868 | 1868 bytes; frame 8C; nonexact; case 1443/1619, 13/15 target-sized groups, gap 2 |
| PlayerInitialize / target 1435 | 1435 bytes in current17-input graph;1123 matching/312 differences after all49 independent bindings; nonexact |
| Authored / source / exact | 734 functions/273579 bytes; source 614/248174; exact 409/74716 |
| Source-present nonexact / absent | 205 / 120 authored functions |
| Overall exact | 1252 functions/141647 bytes; 1098 source mappings |
| Windows product / runtime | Open; semantic reconstruction and portability not started |

Case scores, masked structural scores and size proximity grant no exactness
credit. Live counts are in [PROGRESS.md](PROGRESS.md). The 1776 reviewed candidates
include 518 excluded and 524 indeterminate origins; the denominator remains open.
Large-function exact work remains the priority, without small-function count gains.

## Resume Player's complete initializer

Read the [real-dependency packet](evidence/player-startup-real-dependencies/README.md)
first. Source now uses actual Chain/resource/log services and the required
.sht loader426520/229. The real linked loader uses ESI Player/EAX filename and
restores both target log-call paths. Existing shared selection storage at
GameScoreState474C40+28/+2C restores target-like character reloads. Eight existing
Float3 value operations recover the12-byte scratch; chained extent stores repair
six target bytes with no losses. Original type/data ownership remains unknown.

All49 native/target address destinations are independently identified and their
complete inventories agree. The 1435-byte parent matches1123 bytes after replay;
no ordinary instruction or local control destination is masked. Preserve the
[full312-byte residual](evidence/player-startup-real-dependencies/parent-differences.json).
Do not infer source identity from a coincident neighboring diagnostic anchor.

Next attack callback setup/VM receiver, history addressing, first data-pointer
scheduling, one commuted float operand and tail timer/index schedule. Preserve
all1435 bytes and every field. The shared VM initializer still uses nativeEDX /
targetESI and is200/202 bytes; prior formal/caller controls remain exclusions.
Node reorder changes four still-different bytes without repairs/losses. Partial
views, flag snapshot, index/multiply order and PlayerOb1 are whole-parent neutral.
Pointer history, option-value locals and timer members
regress. Do not rerun those forms without a new premise or substitute a small
loader exact claim for the large-owner objective.

Private inputs/producers: `.analysis/gpt-6.1-sol/player-services-20261023`.
Current-state recovery uses manifest-final.json, stage-common-final.py,
probe-maintained.py and bound-parent-final.py. Preserve historical pins.
Retain `build/gpt-6.1-sol/player-services-20261023/maintained-17`, full-services-17
support cache and prior-chain-canonical. Cleanup records identical replacements
for removed duplicate supports. The
[earlier candidate packet](evidence/player-startup-candidates/README.md) is historical;
its patches belong to source040f1e8 and are not current-source patches.

## Other large-owner routes and closed controls

- [Earlier Player VM binding and caller audit](evidence/player-initializer-binding/README.md):
  private/maintained 17-TU images agree across 366 complete public PDB owners.
  Preserve both carriers under `build/gpt-6.1-sol/stage-initializer-formals-20261023`.
  The audit confirms 28 direct target calls in 11 reviewed owners; source has 27
  explicit member calls. Owner 0x404450 lacks source and two raw E8 hints lack
  boundary proof. Caller closure remains open.
- [Stage packed-color improvement](evidence/stage-color-storage/README.md),
  source checkpoint 2b03d2b: nine changed bytes, five target repairs, no
  target-equal losses. Opcode13 uses native EDX versus target EAX; opcode12 uses
  native CL versus target DL. Earlier ECX notes for opcode13 are superseded.
  Preserve `build/match-linked/StageRealCallback` and before-state witnesses
  under `build/gpt-6.1-sol/stage-script-values-20261023`.
- [Four integer controls](evidence/stage-integer-lifetimes/README.md) and
  [six vector/visibility controls](evidence/stage-vector-lifetimes/README.md)
  did not repair Stage allocation. Member argument reversal and full caller
  visibility were neutral; ordinary free/cdecl was rejected. Do not repeat
  without a new hypothesis. `global-forceinline` was not executed.
- ECL, ANM and Enemy exclusions are in the
  [historical handoffs](RE_HANDOFF_HISTORY_20261023.md). Seven old ECL labels were
  incorrect: use the [source-route audit](evidence/ecl-scratch-route-audit/README.md)
  and actual patches. Preserve `build/match-linked/EnemyItemDrops` and unadopted
  14396-byte/gap298 candidate
  `build/gpt-6-astra-medium/enemy-selection-graph-20261022/scoped-rank-shared-aim`.

## Cleanup, preservation and restoration

The [final cleanup receipt](evidence/handoff-final-20261023/README.md) records
13 losslessly archived historical RNG snapshot trees, 26 removed duplicate
Player objects and Git object repacking. Snapshot contents were verified per
file before removal. Historical RNG producers, receipts and compact results
remain expanded; restore required trees before using old consumers.
Git refs and the reachable/reflog commit list are unchanged by packing.

All canonical linked outputs, current large-owner candidates, their required
source snapshots, toolchains, Wine state, Ghidra database, original target and
game data are retained. All nine unrelated untracked
`docs/evidence/d3d-adapter-format/` files, including `.gitattributes`, remain
unstaged and hash-unchanged. Unknown-origin legacy files are retained.

Restore the new private archive from the repository root; optionally append one
exact original member path from the inventory:

```sh
tar -xzf .analysis/handoff-final-20261023/historical-rng-snapshots.tar.gz -C .
```

Earlier archives remain unchanged, with separate restoration commands:
[329 compiler reports](evidence/handoff-cleanup-20261023/README.md) and
[28 GUI reports](evidence/handoff-cleanup-20261022/README.md).
Never restore old maintained source or weaken historical pins to rerun a probe.

## Resume safely and validation scope

Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md),
relevant source/ledger rows and this handoff. Recover status/diffs first.
Only the ignored original Japanese 1.00a `resources/th10.exe` is valid, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

Operator preferences: direct local tools without Factory MCP, serial nice10
native builds, readOnly/noanalysis/rollback Ghidra with maxcpu1/heap768m, and no
subagents. Attest the database before new target analysis. Full mapped text
401000..465A80 inclusive is 412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.

This source batch verifies the complete1435-byte Player parent after49 independent
bindings and all519 uniquely enumerated public owners between private/maintained
compilations. Seven complete shared guards pass1480/all99 in the current17-input
image. One separate original-recipe14-input cold passes four Chain owners838/all58.
Full CI, new exact acceptance, product closure and runtime validation are not
claimed. Earlier17-input Stage/caller images have different context and are not
interchangeable with this graph. Original classes/TUs, flags and ownership remain
independently unknown.
