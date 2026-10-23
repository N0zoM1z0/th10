# TH10 reconstruction handoff

Updated 2026-10-23 after Player caller/type controls and publication scheduling.
Reconstruction remains **active-incomplete**. The maintained initializer now
emits1435 bytes in its measured17-input graph, but is not exact: complete replay
of49 independently bound address fields matches1195/leaves240 differences.
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
| PlayerInitialize / target 1435 | 1435 bytes in current17-input graph;1195 matching/240 differences after all49 independent bindings; nonexact |
| Authored / source / exact | 734 functions/273579 bytes; source 614/248174; exact 409/74716 |
| Source-present nonexact / absent | 205 / 120 authored functions |
| Overall exact | 1252 functions/141647 bytes; 1098 source mappings |
| Windows product / runtime | Open; semantic reconstruction and portability not started |

Case scores, masked structural scores and size proximity grant no exactness
credit. Live counts are in [PROGRESS.md](PROGRESS.md). The 1776 reviewed candidates
include 518 excluded and 524 indeterminate origins; the denominator remains open.
Large-function exact work remains the priority, without small-function count gains.

## Resume Player's complete initializer

Read the [current caller/type controls](evidence/player-initialization-caller-controls/README.md),
then the historical [ordering](evidence/player-initialization-ordering/README.md)
and [real-dependency](evidence/player-startup-real-dependencies/README.md) packets.
Actual Chain/resource/log services, .sht loader426520/229, existing selection
storage474C40+28/+2C and Float3 values retain the target1435-byte owner/12-byte scratch.
All49 independent address fields are replayed; no ordinary/local bytes are masked.
Keep the [complete240-byte residual](evidence/player-initialization-caller-controls/parent-differences.json).
Do not bind shared-storage members to coincident neighboring diagnostic anchors.

Two additional compiler-only barriers recover draw-node memory-form flags and
late call-argument setup. Over178 changed bytes there are32 repairs/seven losses,
net25 versus276ada7. Five initializer barriers emit no CPU fences; original
source barriers/threading remain unknown. Eight full shared guards pass1753/all99.
Private/maintained builds reproduce519 complete public PDB owners raw; this does
not cover static/unowned fragments or prove target equality/product behavior.

Three coherent private17-input graphs test Chain insertion members, Player
Initialize as a member, and actual AnmVmView embedding. All inputs recompile;
every complete parent is baseline-neutral and all guards pass. A24-input graph
adds seven real caller TUs to cover all27 maintained explicit VM member calls,
and is also complete-parent neutral. This does not close the target28-call/11-owner
inventory. The VM wrapper stays200/nativeEDX versus202/targetESI, with core273 exact.
The first embedding mapping is caught by an offset assertion: color+300 is the
current ANM secondaryColor view, not primaryColor+2FC. The corrected type is
measured and neutral. No header/interface change is adopted.

Of240 remaining differences,197 precede offset150;21 are in150..1FF,11 in200..3FF
and11 in400..59A. Prioritize callback/VM receiver and address lifetimes: the first
flags/global pair still allocates different registers and adds a longer global
load. Consistent insertion-parameter reordering and natural vector-operator
return shape are unexecuted; the former requires every consumer and signature
binding to move together, and the older direct-constructor control does not test
the latter. Later constant placement, a commuted vector operand, scratch-store
order and selection-index registers remain. Early callback
field publication, volatile flags and removing the first node barrier regress.
Do not repeat indexed controls without a new premise or replace the large-owner
objective with a small helper claim. Preserve all1435 bytes and49 fields.

Current producers: `.analysis/gpt-6.1-sol/player-chain-members-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and
bound-parent-final.py. Both maintained-17 and highlight-publication-17 under the
corresponding build campaign remain complete. Historical pins are immutable;
closed controls/snapshots require archive restoration before old consumers run.
Retain the prior player-schedule maintained17 carrier and player-services
maintained/full-services support caches plus prior-chain-canonical. The older
[source candidates](evidence/player-startup-candidates/README.md) belong to040f1e8.

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

The [current cleanup receipt](evidence/player-initialization-caller-controls/cleanup-summary.json)
records15 completed control builds and four closed source/header snapshots:
665 files/33.40MB become0.73MB, reclaiming32.67MB of payload. Every archive member
is length/hash-verified and all37241 protected file hashes are unchanged. Both
current carriers, selected sources, compact reports and producers remain expanded.
Restore archived controls/snapshots from the repository root when needed:

```sh
tar -xJf .analysis/player-caller-cleanup-20261023/completed-controls.tar.xz -C .
```

The [current validation receipt](evidence/player-initialization-caller-controls/validation-summary.json)
states actual build/check scope. The previous
[ordering cleanup](evidence/player-initialization-ordering/README.md) retains its
separate338-file gzip archive and restoration command, saving18.17MB at that checkpoint.
The [previous cleanup receipt](evidence/handoff-final-20261023/README.md) records
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

Restore the earlier RNG snapshot archive from the repository root; optionally append one
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

Current source verifies the complete1435-byte Player parent after49 independent
bindings and519 private/maintained public owners raw. Eight complete shared
guards pass1753/all99. Maintained reproduction compiles Player and reuses16 pinned
inputs; the three private interface/type graphs each freshly compile all17 once.
No independent full-graph pair, new exact acceptance, canonical cold, full CI,
product closure or runtime validation is claimed. The older07b1be2 checkpoint
separately cold-replayed the original14-input Chain recipe4/838/all58. Different
Stage/caller graphs are not interchangeable. Original classes/TUs, flags and
ownership remain independently unknown.
