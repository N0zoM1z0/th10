# TH10 reconstruction handoff

Updated 2026-10-23 for operator-requested cleanup and handoff. Reconstruction is
**active-incomplete**. Latest reconstruction checkpoint: `67fa36a`; this cleanup
does not change game source, headers, ABI, profiles, recipes or exact ledgers.
Operator commit format: `gpt-6-astra-medium: xxx ~`. Local commits only; no push.

## Current state

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target7020 | 7020 bytes;6993 matching/27 ordinary differences;189 fields/62 local destinations;171-byte float callee exact |
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

Keep `src/EclVm.cpp` and `build/match-linked/EnemyItemDrops` as the maintained
7020/6993 reference. Preserve the complete owned table/selector suffix, all189
bindings/62 local destinations, and the171-byte callee. Never crop, move a local
destination, mask ordinary bytes or drop /GL to improve a score.

The27 ordinary differences fall into four groups: thread registers4 bytes,
commuted address encodings5, stack-home allocation16, common-latch registers2.
Review TH10 bytes and actual source patches before trusting older hypotheses.
The [source-route audit](evidence/ecl-scratch-route-audit/README.md) supersedes
seven incorrectly routed historical labels. Earlier routes and other large
owners are indexed in [historical handoffs](RE_HANDOFF_HISTORY_20261023.md).

The [latest ECL controls](evidence/handoff-ecl-cleanup-20261023/README.md) are now
recorded durably: eight images/nine total new ECL object compilations, counting
one later /Op attempt. Both LENGTH records produce7036 bytes; shared case records
produce6940/7056. All four fail the strict size check. A real stop-thread helper,
const current-instruction view and commuted Pop pointer expressions reproduce
the complete6993 replay. Spawn argument locals keep7020 but fail the independent
external-call binding at+F5; there is no valid full-parent score. All eight images
pass nine whole guards1137/all18. None improves maintained source.

The /Op object compiles, but its first link fails for _atan2/_sin/_cos. Reusing
that same object with distinct regenerated anchors still fails for __CIatan2.
Neither attempt produces an image or a parent/guard comparison. Do not label
/Op neutral or substitute a different math ABI. `format-before-math` and
`format-before-frame` were prepared but **never executed**; the sequential
driver stopped at the failed relink. Their sources/plans remain expanded under
`.analysis/gpt-6.1-sol/ecl-typed-values-20261023`.

Read that campaign's manifest, plans and patches before reusing a proposal.
Closed images, private source proposals and bulky structural reports are now
archived; restore required paths through [RE_ARTIFACTS.md](RE_ARTIFACTS.md).
Historical source/input pins remain immutable. New experiments need a fresh
current-input manifest, not a weakened old pin. Unexecuted case ordering or a
new target-supported ABI/source premise remains available; no result is assumed.

## Other large owners and retained witnesses

Player's [draw-owner packet](evidence/player-initialization-draw-owner/README.md)
retains the complete55-byte residual/all49 fields. Draw registration reads the
existing th10::g_Chain symbol while calc uses the retained owner view. Both bind
TH10 address491BE4 independently; distinct diagnostic anchors do not prove
runtime aliasing. Five compiler barriers, vector observations and twelve-byte
Float3 scratch remain. Original qualifiers/classes/TUs and data ownership are open.

Current Player inputs live in
`.analysis/gpt-6.1-sol/player-selection-storage-20261023`: manifest-final.json,
common-final.py, probe-maintained.py and bound-parent-final.py. Preserve its
maintained-17 and draw-typed-chain-owner carriers. The maintained/private pair
agrees across519 complete public owners/140370 bytes, with PlayerInitialize the
sole changed owner versus1370. Eight guards1753/all99 pass. This compiles Player
once/reuses16 pinned inputs; it is not an independent whole-graph cold pair.

The [value/caller controls](evidence/player-initialization-value-controls/README.md)
add16 images/15 objects without improving1380. Current-source27-input real-caller
and actual WinMain entry controls are neutral; both carriers remain expanded.
They do not close target caller identities. The shared VM wrapper is still
native200/EDX versus target202/ESI. More getter/width/copy permutations need a
new premise. Older timer/vector/caller packets remain historical evidence.

Preserve the [Stage color checkpoint](evidence/stage-color-storage/README.md),
`build/match-linked/StageRealCallback`, and the unadopted Enemy candidate
`build/gpt-6-astra-medium/enemy-selection-graph-20261022/scoped-rank-shared-aim`
(14396 bytes/gap298). The [artifact index](RE_ARTIFACTS.md) lists current and
historical support caches; graph names alone do not establish compatible inputs.

## Cleanup and safe resume

The [cleanup receipt](evidence/handoff-ecl-cleanup-20261023/cleanup-summary.json)
archives2785 files:109.04MB become1.57MB,reclaiming107.47MB. All22842 protected
hashes agree during compaction. It covers closed ECL outputs,13 older Player
control images and repeated source/build trees from19 terminal historical workers.
Outer worker receipts/producers and resource inputs remain expanded. Historical
workers include failed diagnostics; archiving grants no truth or exactness credit.
All nine unrelated D3D files remain untracked, unstaged and hash-unchanged.
Tools, Wine, private database, original target and current carriers are retained.

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
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit enemy-drop-item-counts
scripts/repo-python .analysis/gpt-6.1-sol/player-selection-storage-20261023/bound-parent-final.py maintained-17
```

Expected diagnostic outcomes: ECL6993/7020,189 fields/62 locals,171-byte callee
exact (command exits1 for the mismatch); Player1380/1435/all49 fields. These use
retained carriers and are not fresh cold builds. Re-attest Ghidra before new
target analysis. Prior operator execution preferences are direct local tools,
serial nice10 native builds, no subagents, readOnly/noanalysis/rollback Ghidra
maxcpu1/heap768m. Full mapped text401000..465A80 inclusive is412289 bytes, SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.

[Validation](evidence/handoff-ecl-cleanup-20261023/validation-summary.json) records
the actual focused checks. No game-source change or game compilation occurs in
this cleanup; toolchain smoke runs separately. Full CI, broad cold replay,
independent full-graph pairs, product/runtime closure and new exact acceptance
are not claimed. Progress counts and phase gates remain unchanged.
