# TH10 reconstruction handoff

Updated 2026-10-24 after the ANM child request checkpoint, starting at 3d3639d.
Reconstruction remains **active-incomplete**. Commit prefix: `gpt-6.1-sol: ...`.
Local commits only; never push. Continue complete large-function exact work.

## Maintained state

| Owner / metric | Current maintained result |
| --- | --- |
| ANM ExecuteScript / target 9964 | **9960/9964**, 4 ordinary differences; all 350 fields /95 fixed local destinations; case7990/7994,92/92 groups,gap0 |
| ECL Run / target 7020 | 6995/7020,25 differences;189 fields/62 locals;171-byte float callee exact |
| Enemy dispatcher / target 14416 | 14336 bytes;frame2C4;case3245/11128,45 sized groups,gap328 |
| StageExecuteScript / target 1868 | Last retained1868 bytes;frame8C;nonexact;case1443/1619,13/15 groups,gap2 |
| PlayerInitialize / target 1435 | 1380/1435,55 differences;49 fields;actual17-input graph recompiledANM |
| Authored / source / authored exact | 734 functions/273579 bytes;source614/248174;exact409/74716 |
| Authored source-present nonexact / absent | 205 /120 functions |
| Overall exact / mappings | 1252 functions/141647 bytes;1098 mappings |
| Windows i386 product / runtime | Open;semantic reconstruction and portability not started |

[Live progress](PROGRESS.md):1776 reviewed,518 excluded,524 indeterminate origins.
ANM is still nonexact. No exact percentage, header, ABI, profile, recipe,
boundary/origin, exact count or phase gate changes.

## Next route: ANM's four ordinary bytes

Read the [request-record packet](evidence/anm-child-request-record/README.md),
[full350-field contract](evidence/anm-child-request-record/binding-contract.json)
and [actual maintained proof](evidence/anm-child-request-record/current-parent-proof.json).
The source-local initialized flag word groups the two flip bits; child91 uses a
real consumed script/layer record. Actual maintained compilation reproduces the
selected private model across all 352 public owners raw. Against before, the
other 351 public owners remain raw-identical. No source owner/class is inferred.

| Offset, hexadecimal | Remaining source/codegen difference |
| --- | --- |
| +113, +115 | Child91 layer MOV/PUSH uses ECX instead of target EDX |
| +249A, +24A0 | Second radial vertex Z addition swaps position.z/positionOffset.z loads |

The previous handoff's slow-Tick attribution of the latter pair was wrong.
Fresh complete-offset decoding/Ghidra places them at 4412C8/4412CE in the radial
loop. Baseline slow Tick already matches. Do not keep optimizing that old premise.

Target-only decoding independently inventories 9588 code/alignment bytes and
94 owned table words, then joins all 350 slots/types/widths. All95 local offsets
are fixed. Fifteen vector-component fields use correct12-byte bases/addends,
not neighboring4-byte anchors. Five formerly unknown callee/data identities are
reviewed without child exactness or original-owner credit; native Decrement68
versus target20 remains nonexact. The strict comparator is unchanged.

The 23 private ANM controls and one eleven-object real-Player relink are executed.
Only the selected request-record model improves the full9952 replay to9960.
Const/member-store/layer-order variants reproduce9960; constructor/reference
forms, ID reconstitution, whole-vector reversal, explicit construction,
copy accumulations and XY/depth splits do not improve it. Uniform records in
all four child cases shrink the owner and are refused. The full control packet
keeps strict size/field failures separate from valid whole-parent scores.
A new target/context premise is needed before repeating those forms.

## Current replay and before-state carriers

Current producer root:
`.analysis/gpt-6.1-sol/anm-full-parent-bindings-20261024`.
Use `manifest-current.json` / `compare-current.py`; the actual compilation
manifest and initial before manifest remain immutable. The current manifest
separately records the later noncompiled function-ledger note and reviewed
effect-host input-pin updates.

| Carrier under corresponding build root | Purpose |
| --- | --- |
| `maintained-current` | Actual maintained ANM compile in ten-input graph;9960/all 350/95 and ECL6995 |
| `child91-argument-record` | Selected private model;352 complete public owners raw-equal |
| `maintained-player-17` | Actual ANM recompile in Player17-input/52-file graph;Player1380/all 49 |

All36 ANM-context guards pass4618/all 121. ECL retains all 189 fields/62 locals,
complete171-byte callee and16 guards1617/all 21. Player separately retains eight
guards1753/all 99; across519 public owners, the other518 remain raw-identical.
These peer proofs exclude static/unowned fragments and runtime behavior.

The earlier ecl-case-order and Player source freezes are now historical because
src/AnmManager.cpp changed. Do not weaken or overwrite them; use the new actual-source
replay. Their carriers and the original canonical EnemyItemDrops6993 cache
remain protected before/support inputs. The previous two-bit-local ANM carrier
retains9952/12. [Artifacts/restoration](RE_ARTIFACTS.md) lists StageRealCallback,
the unadopted Enemy14396 candidate, older caller graphs and all private archives.
A Git clone does not include ignored evidence; transfer the retained workspace
and archives/inventories when their receipts are needed.

## Recovery and validation

Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md), relevant
source/ledger rows and live status/diffs before editing. Preserve all nine
unrelated untracked D3D files. Only original Japanese1.00a resources/th10.exe is
valid, SHA-2562f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

```sh
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
scripts/repo-python .analysis/gpt-6.1-sol/anm-full-parent-bindings-20261024/compare-current.py
```

Expected retained-carrier replay: ANM9960/9964/all 350/95;36 guards4618/all 121;
ECL6995/7020/all 189/62 and171-byte callee/16 guards1617/all 21;
Player1380/1435/all 49 and8 guards1753/all 99. Actual game compilations this batch:
25 ANM objects across23 private controls and two maintained graphs; one extra
caller relink reuses11 objects. No independent whole-graph cold pair or exact
acceptance is claimed. Re-attest Ghidra before new target analysis.

Direct local tools, serial nice10 native jobs, no subagents; Ghidra
readOnly/noanalysis/rollback,maxcpu1,heap768m. Full mapped text401000..465A80
inclusive:412289 bytes, SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
[Validation](evidence/anm-child-request-record/validation-summary.json) and
[cleanup](evidence/anm-child-request-record/cleanup-summary.json) record the
actual completed checkpoint scope. Closed controls compact53.87MB→0.59MB;
all 23680 protected hashes agree during compaction. Native product/runtime, semantic and port
closure remain unachieved; continue the complete large parent.
