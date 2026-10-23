# TH10 reconstruction handoff

Updated 2026-10-23 after Player timer-commit scheduling. Reconstruction remains
**active-incomplete**. Maintained PlayerInitialize keeps1435 bytes and now matches
1370/leaves65 differences after49 independent address bindings. Headers, layouts,
ABI declarations, canonical recipes and exact ledgers are unchanged. Checkpoint
prefix: `gpt-6.1-sol: ...`.

## Current state

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target 7020 | 7020 bytes; 6993 matching,27 ordinary differences;189 fields/62 local destinations;171-byte float callee exact |
| ANM ExecuteScript / target 9964 | 9964 bytes; nonexact; case7541/7995,90/92 target-sized groups,gap2 |
| Enemy dispatcher / target 14416 | 14336 bytes; frame2C4;181 selectors/108 groups;case3245/11128,45 target-sized groups,gap328 |
| StageExecuteScript / target 1868 | 1868 bytes; frame8C;nonexact;case1443/1619,13/15 target-sized groups,gap2 |
| PlayerInitialize / target 1435 | 1435 bytes in current17-input graph;1370 matching/65 differences after all49 independent bindings;nonexact |
| Authored / source / exact | 734 functions/273579 bytes;source614/248174;exact409/74716 |
| Source-present nonexact / absent | 205 /120 authored functions |
| Overall exact | 1252 functions/141647 bytes;1098 source mappings |
| Windows product / runtime | Open;semantic reconstruction and portability not started |

Case scores, masked structural scores and size proximity grant no exactness
credit. Live counts are in [PROGRESS.md](PROGRESS.md). The1776 reviewed candidates
include518 excluded and524 indeterminate origins; the denominator remains open.
Large-function exact work remains the priority, without small-function count gains.

## Resume Player's complete initializer

Read the [timer-commit packet](evidence/player-initialization-timer-commit/README.md)
and preserve the [complete65-byte residual/all49 fields](evidence/player-initialization-timer-commit/parent-differences.json).
The [vector-order packet](evidence/player-initialization-vector-order/README.md)
retains the preceding1359/76 baseline and the
[owner-capture packet](evidence/player-initialization-owner-capture/README.md)
retains1355/80. Older1195/1170/1123 source pins are historical, not current.

A compiler-only read/write barrier between the actual first-timer previous=-2
and current=-1 stores delays the pure OR ESI,-1 until the target position. All21
former timer-region differences disappear. Across34 changed replayed bytes,
21 target bytes are repaired and10 target-equal bytes are lost in later vector
scratch-store scheduling: net11,1370/65. Five initializer barriers now remain.
The earlier callback-owner capture, contiguous position history and four-byte
vector-operand improvement are retained. Original barriers, qualifiers,
threading, classes and TU ownership remain unknown. No CPU fence is emitted.

Actual Chain/resource/log services, .sht loader426520/229, shared selection
storage474C40+28/+2C and Float3 values retain the1435-byte owner/12-byte scratch.
The loader is source-present, not exact. All49 fields are independently bound;
no ordinary/local bytes are masked. Do not bind shared-storage members to
coincident neighboring diagnostic anchors. The real internal vector helper
remains unmapped and receives no small-function credit.

Private and freshly compiled maintained Player objects reproduce519 complete
public PDB owners raw/140370 bytes. Only PlayerInitialize changes versus1359;
the other518 public owners are raw-identical. This is bounded public-owner
coverage, not static/unowned or runtime closure. The older vector-order packet
separately records the retained /OPT:NOREF helper copy and its earlier40h layout
movement; do not transfer its old field-only scope to the new raw-equality result.
Eight complete shared guards pass1753/all99. Maintained reproduction compiles
Player once and reuses16 pinned unchanged inputs, not a whole-graph cold pair.

The current batch has11 complete1435-byte images, all49 fields/eight guards,
and12 compiler invocations:11 accepted objects plus one rejected intrinsic
attempt. Draw-owner capture with ordinary/ordered reads regresses to1203; a
node-publication barrier gives1358 and an ordered cold character read1351.
A volatile previous-field store gives1358. A supported write-only timer barrier,
two real vector-value helpers, a write barrier after the calc-flags snapshot,
and actual Player /G7 all reproduce1370 byte-for-byte and are not adopted.
Pinned VC7.1 rejects _ReadBarrier with C4163; the driver refuses its object
before linking. Never turn that unsupported intrinsic into an opaque helper.

The prior missing-Stage-caller, GameManager and27-input wrapper graphs remain
neutral at their own1195 baseline. Actual WinMain entry is also neutral there.
These graphs do not close all target call-site identities or transfer their
result to different source. Earlier controls require a new premise to repeat.

The target-attested binder audit finds Ghidra's21 incoming references incomplete:
six extra raw CALLs in complete95/118-byte creators give a verified lower bound27.
All eight position creators call43E710 correctly. The expanded native image has
21 direct child calls with a different owner distribution; counts alone do not
close callers. The VM wrapper remains200/nativeEDX versus202/targetESI; its273-byte
core guard is exact. Inspect operands and missing/differently folded owners.

Of65 remaining differences,37 precede150,none lie in150..1FF,10 lie in200..3FF
and18 in400..59A. Prioritize callback/VM receiver lifetimes, vector scratch-store
scheduling and selector registers. Keep the whole large parent as the objective.

Current producers: `.analysis/gpt-6.1-sol/player-flag-lifetimes-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and bound-parent-final.py.
The initial1359 manifest stays immutable. Keep current maintained-17 and selected
first-timer-commit-barrier carriers expanded. All required older Player caches
and selected witnesses remain; actual paths and restore commands are indexed in
[RE_ARTIFACTS.md](RE_ARTIFACTS.md).

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-flag-lifetimes-20261023/bound-parent-final.py maintained-17
```

Expected mismatch1370/1435,49 fields. Restore archived ignored paths before
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

The [current cleanup](evidence/player-initialization-timer-commit/cleanup-summary.json)
archives nine closed images, one rejected intrinsic object tree and ten private
source proposals:249 files/18.85MB become0.40MB, reclaiming18.45MB. All24811
protected-file hashes are unchanged during compaction. Current/selected carriers
and required old caches remain expanded.

The [prior handoff cleanup](evidence/handoff-artifact-compact-20261023/README.md)
archives85 complete historical LTCG probe caches:1229 files/64.85MB become2.28MB,
reclaiming62.57MB. The preceding vector-order batch separately reclaimed31.64MB.
Use the [artifact and restoration index](RE_ARTIFACTS.md) for every archive path
and restore command. All nine unrelated untracked D3D files remain unstaged and
hash-unchanged. Original target, tools, Wine, database and game data are retained.

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

Current source verifies the complete1435-byte Player parent at1370/65 after49
independent bindings and reproduces519 private/maintained public owners raw.
Eight complete shared guards pass1753/all99. Maintained reproduction compiles
Player and reuses16 pinned inputs;11 complete images and12 compiler invocations
including one refused intrinsic object are recorded in the current packet.
No independent full-graph pair, new exact acceptance, canonical cold, full CI,
product closure or runtime validation is claimed. The older07b1be2 checkpoint
separately cold-replayed the original14-input Chain recipe4/838/all58. Different
Stage/caller graphs are not interchangeable. Original classes/TUs, flags and
ownership remain independently unknown.
