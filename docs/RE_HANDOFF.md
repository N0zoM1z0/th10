# TH10 reconstruction handoff

Updated 2026-10-23 for handoff after Player vector-value ordering and artifact
compaction. Reconstruction remains
**active-incomplete**. Maintained PlayerInitialize keeps1435 bytes and now matches
1359/leaves76 differences after49 independent address bindings. Headers, layouts,
ABI declarations, canonical recipes and exact ledgers are unchanged. Checkpoint
prefix: `gpt-6-astra-medium: ... ~`.

## Current state

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target 7020 | 7020 bytes; 6993 matching,27 ordinary differences;189 fields/62 local destinations;171-byte float callee exact |
| ANM ExecuteScript / target 9964 | 9964 bytes; nonexact; case7541/7995,90/92 target-sized groups,gap2 |
| Enemy dispatcher / target 14416 | 14336 bytes; frame2C4;181 selectors/108 groups;case3245/11128,45 target-sized groups,gap328 |
| StageExecuteScript / target 1868 | 1868 bytes; frame8C;nonexact;case1443/1619,13/15 target-sized groups,gap2 |
| PlayerInitialize / target 1435 | 1435 bytes in current17-input graph;1359 matching/76 differences after all49 independent bindings;nonexact |
| Authored / source / exact | 734 functions/273579 bytes;source614/248174;exact409/74716 |
| Source-present nonexact / absent | 205 /120 authored functions |
| Overall exact | 1252 functions/141647 bytes;1098 source mappings |
| Windows product / runtime | Open;semantic reconstruction and portability not started |

Case scores, masked structural scores and size proximity grant no exactness
credit. Live counts are in [PROGRESS.md](PROGRESS.md). The1776 reviewed candidates
include518 excluded and524 indeterminate origins; the denominator remains open.
Large-function exact work remains the priority, without small-function count gains.

## Resume Player's complete initializer

Read the [current vector-order packet](evidence/player-initialization-vector-order/README.md)
and preserve the [complete76-byte residual/all49 fields](evidence/player-initialization-vector-order/parent-differences.json).
The [owner-capture packet](evidence/player-initialization-owner-capture/README.md)
retains the preceding1355/80 baseline.
The prior [parameter/type](evidence/player-initialization-parameter-controls/README.md),
[caller/publication](evidence/player-initialization-caller-controls/README.md),
[ordering](evidence/player-initialization-ordering/README.md) and
[real-dependency](evidence/player-startup-real-dependencies/README.md) packets retain
historical source hashes; their old1195/1170/1123 baselines are not current.

The maintained change captures flags and the actual callback owner before node
publication. One scoped volatile pointer observation replaces the first compiler
barrier; four initializer barriers remain. The first manager load uses EAX and
the short absolute encoding. Compiler alignment grows naturally from six to seven
bytes; no padding is supplied by source. Across200 changed replayed bytes there
are164 repairs/four losses, net160 versus1195/240. Original qualifiers, barriers,
threading and class/TU ownership remain unknown. No CPU fence or false ABI is used.

Actual Chain/resource/log services, .sht loader426520/229, shared selection
storage474C40+28/+2C and Float3 values retain the1435-byte owner/12-byte scratch.
The loader is source-present, not exact. All49 fields are independently bound;
no ordinary/local bytes are masked. Do not bind shared-storage members to
coincident neighboring diagnostic anchors.

The new component-order value construction and one scoped Y-operand observation
repair four further ordinary bytes without losses, reaching1359/76. The actual
three-component result is used and the helper inlines inside the complete parent.
Its original qualifier/source structure is unknown; no helper target is mapped.

Private and freshly compiled maintained Player objects reproduce519 complete
public PDB owners raw/140370 bytes. The /OPT:NOREF diagnostic also retains an
unmapped static helper copy; later code shifts40h. Old/new raw bytes change in160
public owners:159 differences are confined to independently observed link-field
extents in both images, and only PlayerInitialize has ordinary differences.
This classification does not bind every semantic destination or prove target-wide
exactness. Static/unowned fragments and product/runtime remain outside scope. Eight complete
shared guards pass1753/all99. Maintained reproduction compiles Player once and
reuses16 pinned unchanged inputs, not an independent whole-graph cold pair.

The current packet records18 complete1435-byte images/18 new object compilations,
all replayed with49 independent fields and eight full guards. Ordered calc flags
and an intervening snapshot barrier regress. All five first-timer store-order
permutations regress, confined to the timer region. Timer-scale identity and
actual Player G5 profile are whole-parent neutral. Direct ordered-Y constructors
move the read too early; a real named component-order return fixes its operands.
Subtraction/POD/timer helpers and an ordinary inline declaration preserve the
selected1359 bytes and are not adopted as additional changes. The previous
owner-capture packet retains its19-image/55-object scope, including rejected
1438/1446-byte controls. Do not add gains from independent experiments.

The private missing-Stage-caller dependency, actual GameManager caller and full
wrapper-caller source graph produce20/21/27-input old-source controls, all neutral
at1195. An actual WinMain-entry relink is also neutral. The214-byte helper is not
adopted; no small-function source/exact gain is claimed. These graphs do not prove
all target call-site identities or transfer their result to a different graph.

A new target-attested read-only review finds Ghidra's21 incoming binder references
incomplete: six additional raw CALLs in complete95/118-byte creator owners give a
verified lower bound27. All eight position creators correctly call43E710. Do not
change those calls based on the incomplete reference list. The expanded native
image has21 direct child calls with a different owner distribution; counts alone
do not close callers. Inspect real operands and missing/differently folded owners.
The VM wrapper remains200/nativeEDX versus202/targetESI; its core273 guard is exact.

Of76 remaining differences,37 precede150,21 lie in150..1FF,7 in200..3FF and11 in
400..59A. Prioritize callback/VM receiver lifetimes, the early timer constant,
the vector scratch-store ordering and selector registers.
Earlier neutral controls require a new premise before repetition. Keep every
owned byte and field; the large parent remains the objective.

Current producers: `.analysis/gpt-6.1-sol/player-residual-publication-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and bound-parent-final.py.
Keep complete maintained-17 under the new build campaign and selected
max0-ordered-named-result under the older player-stage-caller build campaign;
actual paths are recorded in each report. Keep earlier maintained1355,
calc-snapshot-before-owner and all-wrapper-callers-27 witnesses unchanged.

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-residual-publication-20261023/bound-parent-final.py maintained-17
```

Expected mismatch1359/1435,49 fields. Initial/historical manifests are immutable;
restore archived ignored paths before historical consumers, never old source
into src/ and never weaken pins. Earlier required caches remain retained:
player-chain-members maintained/highlight-publication17, player-schedule
maintained17, player-services maintained/full-services and prior-chain-canonical.

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

## Artifact recovery

Code checkpoint `2c1c753` retains the measured Player improvement. The
[handoff cleanup](evidence/handoff-artifact-compact-20261023/README.md) archives
85 complete historical LTCG probe caches:1229 files/64.85MB become2.28MB,
reclaiming62.57MB. All24600 protected-file hashes are unchanged during compaction.
The preceding Player batch separately reclaimed31.64MB. No further game-source,
header, recipe or ledger change is made for cleanup.

Use the [artifact and restoration index](RE_ARTIFACTS.md) for current expanded
carriers, every archive path and restore command. Restore required ignored paths
before historical consumers; never restore old source over src/ or weaken pins.
All nine unrelated untracked D3D files remain unstaged and hash-unchanged.

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

Current source verifies the complete1435-byte Player parent at1359/76 after49
independent bindings and reproduces519 private/maintained public owners raw.
Eight complete shared guards pass1753/all99. Maintained reproduction compiles
Player and reuses16 pinned inputs; the complete18-image experiment matrix and
all18 new object compilations are recorded in the current packet. No independent
full-graph pair, new exact acceptance, canonical cold, full CI, product closure
or runtime validation is claimed. The older07b1be2 checkpoint separately
cold-replayed the original14-input Chain recipe4/838/all58. Different Stage/caller
graphs are not interchangeable. Original classes/TUs, flags and ownership remain
independently unknown.
