# TH10 reconstruction handoff

Updated 2026-10-23 for the operator-requested cleanup and handoff. Reconstruction
remains **active-incomplete**. Maintained game source is still 040f1e8: Player's
startup VM call uses the real shared ANM initializer. Subsequent Player candidates
are private, measured and **not adopted**. No source, header, layout, compiler
profile, canonical recipe or exact ledger changes in this batch.
Current checkpoint prefix: `gpt-6.1-sol: ...`.

## Current state

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target 7020 | 7020 bytes; 6993 matching, 27 ordinary differences; 189 fields/62 local destinations; 171-byte float callee exact |
| ANM ExecuteScript / target 9964 | 9964 bytes; nonexact; case 7541/7995, 90/92 target-sized groups, gap 2 |
| Enemy dispatcher / target 14416 | 14336 bytes; frame 2C4; 181 selectors/108 groups; case 3245/11128, 45 target-sized groups, gap 328 |
| StageExecuteScript / target 1868 | 1868 bytes; frame 8C; nonexact; case 1443/1619, 13/15 target-sized groups, gap 2 |
| PlayerInitialize / target 1435 | 1116 bytes/nonexact in maintained 17-TU graph; latest private 14-TU candidate 1388 bytes, not adopted |
| Authored / source / exact | 734 functions/273579 bytes; source 613/247945; exact 409/74716 |
| Source-present nonexact / absent | 204 / 121 authored functions |
| Overall exact | 1252 functions/141647 bytes; 1097 source mappings |
| Windows product / runtime | Open; semantic reconstruction and portability not started |

Case scores, masked structural scores and size proximity grant no exactness
credit. Live counts are in [PROGRESS.md](PROGRESS.md). The 1776 reviewed candidates
include 518 excluded and 524 indeterminate origins; the denominator remains open.
Large-function exact work remains the priority, without small-function count gains.

## Resume Player's complete initializer

Read the [new Player candidate packet](evidence/player-startup-candidates/README.md)
first. It reconciles the final completed job and preserves five independently
applicable patches, actual input/artifact hashes and complete-parent reports.

| Private 14-TU trial | Complete Player bytes | State |
| --- | --- | --- |
| typed-nodes: existing Chain allocation and insertion APIs | 1116 | Measured, not adopted |
| value-extent-left: eight existing Float3 value operations | 1366 | Measured, not adopted |
| value-current-data: also reload optionData after timer setup | 1388 | Measured, not adopted |
| value-pos-left / bound-services | Unknown | Prepared, not executed |

All three measured trials pass four complete Chain guards, 838 bytes/all 58
fields, and expose five real callback/VM call edges. The target and both value
candidates reserve 12 scratch bytes; the original scalar source does not.
This supports further source-shape investigation, not whole-parent exactness.

Next measure the prepared real resource/error bindings, then inspect the full
1435-byte parent residual. The declaration-only option-data loader targets
0x426520/229 and lacks its real source/caller binding. Treat it as a dependency
of this large owner, not a separate small-function campaign. Establish every
target field independently before whole-owner exact acceptance.

Private inputs/producers:
`.analysis/gpt-6.1-sol/player-chain-bindings-20261023`.
Complete latest native carrier:
`build/gpt-6.1-sol/player-chain-bindings-20261023/value-current-data-14`.
Earlier trials retain images, maps, PDBs and unique Player objects; only their
26 duplicate reused objects are removed. Existing manifests retain their pins.
Do not overwrite completed trial directories or relax pins.

## Other large-owner routes and closed controls

- [Maintained Player VM binding and caller audit](evidence/player-initializer-binding/README.md):
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

This documentation/artifact batch reruns cached canonical Chain comparison for
four full owners/838 bytes/all 58 fields, target/toolchain/tracking/progress,
documentation hygiene, Git connectivity and whitespace. It does not cold-build
new game source, repeat full CI or claim new exact/product/runtime validation.
The preceding current-source canonical Chain cold is recorded in the Player VM
packet. Its four contracts reject unequal extents in the 17-TU graph; only the
declared canonical 14-TU replay is accepted. Original classes/TUs, flags,
ownership, product and runtime stay independently unknown.
