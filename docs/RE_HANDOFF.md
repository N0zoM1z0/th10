# TH10 reconstruction handoff

Updated 2026-10-23 after the typed ECL trace checkpoint. Reconstruction remains
**active-incomplete**. Current ECL Run improves6993/27 to6995/25 with unchanged
complete7020-byte extent. No header, ABI, layout, profile, recipe or exact-ledger
change is made. Operator commit prefix: `gpt-6.1-sol: ...`. Local commits; no push.

## Current state

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target7020 | 7020 bytes;6995 matching/25 ordinary differences;189 fields/62 local destinations;171-byte float callee exact |
| ANM ExecuteScript / target9964 | 9964 bytes;nonexact;case7541/7995,90/92 target-sized groups,gap2 |
| Enemy dispatcher / target14416 | 14336 bytes;frame2C4;case3245/11128,45 target-sized groups,gap328 |
| StageExecuteScript / target1868 | 1868 bytes;frame8C;nonexact;case1443/1619,13/15 target-sized groups,gap2 |
| PlayerInitialize / target1435 | 1435 bytes;1380 matching/55 differences;49 independently bound fields;nonexact |
| Authored / source / authored exact | 734 functions/273579 bytes;source614/248174;exact409/74716 |
| Authored source-present nonexact / absent | 205 /120 functions |
| Overall exact / source mappings | 1252 functions/141647 bytes;1098 mappings |
| Windows i386 product / runtime | Open;semantic reconstruction and portability not started |

Case scores and size proximity grant no exactness credit. [Live progress](PROGRESS.md)
covers1776 reviewed candidates,518 excluded and524 indeterminate origins; the
authored denominator remains provisional. Prioritize large-function exact work.

## Next large-owner route: ECL Run

The current carrier is
`build/gpt-6.1-sol/ecl-case-order-20261023/maintained-current`; selected private
witness typed-disabled-trace remains beside it. The canonical EnemyItemDrops
carrier is retained as a **before-state6993 witness**, not current-source ECL
evidence. Use the new explicit context/helper below. Do not rebuild over the old
canonical cache casually: historical experiments pin its exact artifacts.

Read the [typed trace packet](evidence/ecl-typed-trace-lifetimes/README.md) and
[all25 residual bytes/189 bindings](evidence/ecl-typed-trace-lifetimes/current-parent-proof.json).
Typed disabled consumers preserve FORMAT service calls and live instruction
rereads. Six ordinary bytes change: four repairs at146E/14D7/168A/1710 and two
losses atAF/1FB. Original logger signatures/classes/TUs remain unknown. Three
emitted static consumers gain no target identity or small-function exact credit.

Remaining differences: thread registers4 bytes, commuted addresses3, stack
homes16 and common latch2. Keep all7020 bytes,189 fields/62 locals,60 table words,
88 selectors and the171-byte callee; never crop or move a local destination.
Current full replay SHA is
aeccdec2b6e53998a8e12793cf5a3860e4522847412afd607bba526ebc7d75e6.
All16 ECL contracts pass1617/all21. Selected/actual maintained builds reproduce
352 complete public owners/95263 bytes raw; only Run has ordinary changes versus
before, with the other351 raw-identical or independently bound address moves.
Static/unowned fragments and native runtime remain outside that peer proof.

The32-image/32-object batch has18 full parent comparisons,11 strict size
rejections and3 binding refusals. Plain math helpers are neutral; shared floats
repair math bytes but break six raw-negation bytes. Reversed mapping gives6992
before/6994 with typed trace; neither improves6995. Scalar pairs grow7028/7032.
Case-order plans are now executed: FORMAT-before-math fails malloc binding;
before-frame is7012 bytes. Actual opcode helpers are6480/6972/6884/6320.

The /Op harness now links with separate C/CI anchors: actual Run6964, ANM9980,
Enemy14336. Run and two ECL guard sizes fail; seven selected guards pass861/all11.
It is no longer an unexecuted profile comparison. No math ABI is substituted.
Further helper/value/aggregate permutations need a new target or optimizer premise.
Prior [misrouted labels](evidence/ecl-scratch-route-audit/README.md) remain corrected;
older large-owner routes are in [historical handoffs](RE_HANDOFF_HISTORY_20261023.md).

Current producers: .analysis/gpt-6.1-sol/ecl-case-order-20261023. Use
manifest-current.json and compare-current.py; manifest-final.json/probe-maintained.py
bind the completed actual-source compile. The initial before manifest stays
immutable. Each build compiles ECL/reuses nine pinned inputs; no independent
whole-graph cold pair or exact acceptance is claimed.

## Other large owners and retained witnesses

Player's [draw-owner packet](evidence/player-initialization-draw-owner/README.md)
retains the complete55-byte residual/all49 fields. Draw registration reads the
existing th10::g_Chain symbol while calc uses the retained owner view. Both bind
TH10 address491BE4 independently; distinct diagnostic anchors do not prove
runtime aliasing. Five compiler barriers, vector observations and twelve-byte
Float3 scratch remain. Original qualifiers/classes/TUs and data ownership are open.

Player's maintained/selected carriers remain under
.analysis/gpt-6.1-sol/player-selection-storage-20261023. Its earlier519-owner
raw proof and eight guards1753/all99 are retained. Current ECL source is outside
the actual Player17-input/52-project-file include closure, all independently
verified unchanged. The new compare-current.py replays1435/1380/all49 and all
eight guards without weakening the old producer's broader frozen ECL hash.
Old Player manifests remain immutable; their global attestor is now historical.

The [value/caller controls](evidence/player-initialization-value-controls/README.md)
add16 images/15 objects without improving1380. The earlier27-input real-caller
and actual WinMain entry controls are neutral; both historical carriers remain expanded.
They do not close target caller identities. The shared VM wrapper is still
native200/EDX versus target202/ESI. More getter/width/copy permutations need a
new premise. Older timer/vector/caller packets remain historical evidence.

Preserve the [Stage color checkpoint](evidence/stage-color-storage/README.md),
`build/match-linked/StageRealCallback`, and the unadopted Enemy candidate
`build/gpt-6-astra-medium/enemy-selection-graph-20261022/scoped-rank-shared-aim`
(14396 bytes/gap298). The [artifact index](RE_ARTIFACTS.md) lists current and
historical support caches; graph names alone do not establish compatible inputs.

## Cleanup and safe resume

The [latest cleanup](evidence/ecl-typed-trace-lifetimes/cleanup-summary.json)
archives542 files from30 closed controls:52.81MB become0.47MB,reclaiming52.34MB.
All23212 protected hashes agree during compaction. Current/selected and before
carriers remain expanded; historical private proposals/reports need restoration
through [RE_ARTIFACTS.md](RE_ARTIFACTS.md). The prior cleanup separately reclaimed
107.47MB; do not add it to this batch's saving. All nine unrelated D3D files
remain untracked/unstaged/hash-unchanged; tools, Wine, database and target remain.

Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md), relevant
source/ledger rows and Git status/diffs before changing state. The only valid
target is ignored original Japanese1.00a resources/th10.exe, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
scripts/repo-python .analysis/gpt-6.1-sol/ecl-case-order-20261023/compare-current.py
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit context --context-manifest .analysis/gpt-6.1-sol/ecl-case-order-20261023/maintained-current-context.toml
```

Expected diagnostic outcomes: ECL6995/7020,189 fields/62 locals,171-byte callee
exact (command exits1 for the mismatch); Player1380/1435/all49 fields. These use
retained carriers and are not fresh cold builds. Re-attest Ghidra before new
target analysis. Prior operator execution preferences are direct local tools,
serial nice10 native builds, no subagents, readOnly/noanalysis/rollback Ghidra
maxcpu1/heap768m. Full mapped text401000..465A80 inclusive is412289 bytes, SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.

[Validation](evidence/ecl-typed-trace-lifetimes/validation-summary.json) records
the actual focused scope:32 completed images/32 ECL compilations, current parent,
all16 ECL guards, source-projected FORMAT controls and unchanged Player graph.
No independent full-graph pair, canonical cold, full CI, new exact acceptance,
product/runtime closure, semantic or port completion is claimed. Counts and
phase gates remain unchanged. Continue on the complete large parent.
