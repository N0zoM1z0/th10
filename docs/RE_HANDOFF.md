# TH10 reconstruction handoff

Updated 2026-10-23 after Player draw-owner publication. Reconstruction remains
**active-incomplete**. Maintained PlayerInitialize keeps1435 bytes and now matches
1380/leaves55 differences after49 independent address bindings. Headers, layouts,
ABI declarations, canonical recipes and exact ledgers are unchanged. Checkpoint
prefix: `gpt-6.1-sol: ...`.

## Current state

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target 7020 | 7020 bytes; 6993 matching,27 ordinary differences;189 fields/62 local destinations;171-byte float callee exact |
| ANM ExecuteScript / target 9964 | 9964 bytes; nonexact; case7541/7995,90/92 target-sized groups,gap2 |
| Enemy dispatcher / target 14416 | 14336 bytes; frame2C4;181 selectors/108 groups;case3245/11128,45 target-sized groups,gap328 |
| StageExecuteScript / target 1868 | 1868 bytes; frame8C;nonexact;case1443/1619,13/15 target-sized groups,gap2 |
| PlayerInitialize / target 1435 | 1435 bytes in current17-input graph;1380 matching/55 differences after all49 independent bindings;nonexact |
| Authored / source / exact | 734 functions/273579 bytes;source614/248174;exact409/74716 |
| Source-present nonexact / absent | 205 /120 authored functions |
| Overall exact | 1252 functions/141647 bytes;1098 source mappings |
| Windows product / runtime | Open;semantic reconstruction and portability not started |

Case scores, masked structural scores and size proximity grant no exactness
credit. Live counts are in [PROGRESS.md](PROGRESS.md). The1776 reviewed candidates
include518 excluded and524 indeterminate origins; the denominator remains open.
Large-function exact work remains the priority, without small-function count gains.

## Resume Player's complete initializer

Read the [draw-owner packet](evidence/player-initialization-draw-owner/README.md)
and preserve the [complete55-byte residual/all49 fields](evidence/player-initialization-draw-owner/parent-differences.json).
The [timer-commit packet](evidence/player-initialization-timer-commit/README.md)
retains1370/65, [vector ordering](evidence/player-initialization-vector-order/README.md)
retains1359/76, and [owner capture](evidence/player-initialization-owner-capture/README.md)
retains1355/80. Older1195/1170/1123 inputs and commands are historical.

Only the draw-registration argument now reads existing th10::g_Chain. Its TH10
identity491BE4 and the retained calc-owner view are independently bound. This
moves the actual node-argument publication into the target position: complete
1435-byte replay improves1370/65 to1380/55, ten repairs/no target-equal losses.
Five initializer barriers, owner/vector scoped observations, history copy and
twelve-byte Float3 scratch remain. No header, layout, ABI, profile, canonical
recipe or exact-ledger change is made. No small helper receives target credit.

Do not infer an original C++ type or ownership from this result. Typing the old
manager declaration alone, including its ordered access, reproduces the old1370
bytes. The result depends on the current symbol/alias context. Two distinct
native diagnostic anchors independently bind the same TH10 global; this is not
proof that their runtime storage aliases. Original qualifiers, classes, TUs,
barriers and product data ownership remain unknown. No CPU fence is added.

Private and freshly compiled maintained Player objects reproduce519 complete
public PDB owners raw/140370 bytes. Only PlayerInitialize changes versus1370;
the other518 public owners are raw-identical. Static/unowned fragments and
runtime remain outside scope. Eight complete shared guards pass1753/all99.
Maintained reproduction compiles Player once and reuses16 pinned unchanged
inputs, not an independent whole-graph cold pair. All49 target fields remain
independently bound and every ordinary/local byte is compared without cropping.
Shared selection storage remains GameScoreState474C40+28/+2C; never use coincident
neighboring diagnostic anchors as semantic members. The.sht loader426520/229 is
source-present, not exact.

The current batch records16 complete1435-byte images/16 new Player object
compilations, each with49 fields and eight full guards. Matrix-view, actual
matrix-declaration and real filename-value helper forms produce identical1247
replays:12 repairs/135 losses versus1370, despite correcting the cold selection
block. Adding the selected draw owner to a matrix form yields1248, not additive
gains. Expanded indexing and two real memcpy value copies reproduce1370.
Typed draw-owner snapshots give1214, a following barrier1229. Typed calc-owner
access gives1370, ordered calc flags1193 and a calc snapshot barrier1210.
The old _ReadBarrier/C4163 refusal remains valid; do not link an opaque substitute.

Fresh disk/Ghidra table bytes agree at476FC0/24: pl00a,b,c.sht followed by
pl01a,b,c.sht. The stride3 code and six pointers do not establish original C++
array dimensions or data authorship. Native alias/type hypotheses stay separate
from these TH10 observations. New manifests bind the current1370 receipt and
required hashes; obsolete copied candidate-score hints are excluded without
changing historical input pins.

The prior missing-Stage-caller, GameManager and27-input wrapper graphs remain
neutral at their own1195 baseline. Actual WinMain entry is also neutral there.
They do not close target call-site identities or transfer to different source.
The target-attested binder audit finds Ghidra's21 incoming references incomplete:
six extra CALLs in complete95/118-byte creators give a verified lower bound27.
All eight position creators call43E710 correctly. The expanded native image has
21 direct child calls with a different owner distribution; counts alone do not
close callers. The VM wrapper remains200/nativeEDX versus202/targetESI; its273-byte
core guard is exact. Inspect operands and missing/differently folded owners.

Of55 differences,27 precede150,none lie in150..1FF,10 lie in200..3FF and18 in
400..59A. Prioritize calc/draw manager-read ordering, VM receiver lifetimes,
vector scratch-store scheduling and cold selection registers. Earlier neutral
or regressive controls require a new premise to repeat. Keep the large parent.

Current producers: `.analysis/gpt-6.1-sol/player-selection-storage-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and bound-parent-final.py.
The initial1370 manifest stays immutable. Keep current maintained-17 and selected
draw-typed-chain-owner carriers expanded. Older required Player caches and
witnesses remain; paths and restoration commands are in [RE_ARTIFACTS.md](RE_ARTIFACTS.md).

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-selection-storage-20261023/bound-parent-final.py maintained-17
```

Expected mismatch1380/1435,49 fields. Restore archived ignored paths before
historical consumers, never old source into src/ and never weaken input pins.

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

The [current cleanup](evidence/player-initialization-draw-owner/cleanup-summary.json)
archives14 closed images and14 private source proposals:378 files/28.14MB become
0.41MB, reclaiming27.73MB. All25068 protected-file hashes are unchanged during
compaction. Current/selected carriers and required old caches remain expanded.
The preceding timer-commit batch separately reclaimed18.45MB.

Earlier [handoff compaction](evidence/handoff-artifact-compact-20261023/README.md)
reclaimed62.57MB; vector ordering separately reclaimed31.64MB. Use the
[artifact and restoration index](RE_ARTIFACTS.md) for every archive path and
restore command. All nine unrelated D3D files remain untracked, unstaged and
hash-unchanged. Target, tools, Wine, database and game data are retained.

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

Current source verifies the complete1435-byte Player parent at1380/55 after49
independent bindings and reproduces519 private/maintained public owners raw.
Eight complete shared guards pass1753/all99. Maintained reproduction compiles
Player and reuses16 pinned inputs;16 complete images/new Player compilations
are recorded in the current packet. No independent full-graph pair, new exact
acceptance, canonical cold, full CI, product closure or runtime validation is
claimed. The older07b1be2 checkpoint separately cold-replayed the original14-input
Chain recipe4/838/all58. Different Stage/caller graphs are not interchangeable.
Original classes/TUs, flags and ownership remain independently unknown.
