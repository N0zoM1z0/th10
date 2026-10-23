# TH10 reconstruction handoff

Updated 2026-10-24 for operator-requested cleanup/handoff, starting at 7dd1c6d.
Reconstruction remains **active-incomplete**. Maintained code/headers/profiles/
recipes/ledgers are unchanged. Commit format: `gpt-6-astra-medium: ... ~`.
Local commits only; never push. Continue large-function exact work, not small helpers.

## Maintained state

| Owner / metric | Current maintained result |
| --- | --- |
| ECL Run / target 7020 | 7020 bytes; 6995 matching / 25 ordinary differences; 189 fields / 62 locals; 171-byte float callee exact |
| ANM ExecuteScript / target 9964 | 9964 bytes; nonexact; case 7541/7995, 90/92 sized groups, gap 2 |
| Enemy dispatcher / target 14416 | 14336 bytes; frame 2C4; case 3245/11128, 45 sized groups, gap 328 |
| StageExecuteScript / target 1868 | 1868 bytes; frame 8C; nonexact; case 1443/1619, 13/15 sized groups, gap 2 |
| PlayerInitialize / target 1435 | 1435 bytes; 1380 matching / 55 differences; 49 fields; nonexact |
| Authored / source / authored exact | 734 functions / 273579 bytes; source 614 / 248174; exact 409 / 74716 |
| Authored source-present nonexact / absent | 205 / 120 functions |
| Overall exact / mappings | 1252 functions / 141647 bytes; 1098 mappings |
| Windows i386 product / runtime | Open; semantic reconstruction and portability not started |

[Live progress](PROGRESS.md): 1776 reviewed, 518 excluded, 524 indeterminate origins.
Case scores, equal sizes and structural normalization grant no exact credit.

## Next route: retained private ANM two-bit candidate

The [recovered batch and continuation packet](evidence/handoff-cleanup-20261024/README.md)
documents 21 completed ANM and 6 ECL controls missing from the previous handoff.
The most promising **unadopted** source is
`.analysis/gpt-6.1-sol/anm-flag-return-context-20261023/flip-two-bit-local/AnmManager.cpp`;
its complete image/map/PDB/objects remain expanded at
`build/gpt-6.1-sol/anm-flag-return-context-20261023/flip-two-bit-local`.
Its four-byte local flag view changes no header/layout/ABI. TH09 supplied a
source-shape hypothesis; TH10 masks/behavior require TH10 evidence.

Fresh retained replay: 9964-byte contribution, 92/92 target-sized groups, gap 0,
case 7982/7994; 36 full guards covering 4618 bytes / all 121 fields and strict
ECL 6995/7020 / all 189 fields plus the 171-byte callee preserved. Structural
parent comparison matches 8552/8564 ordinary positions after enumerating 350
fields. Twelve structural differences remain: child91 preparation at +113,
+115..+11D and slow Tick at +249A/+24A0. Offsets are hexadecimal.

The complete semantic address replay is still unfinished. Next independently bind
all 350 fields / 95 local destinations, audit complete public peers, then test real
caller visibility and timer context. The packet lists five unresolved binding
identities and retained Ghidra/CRT evidence. No new cold pair or exact acceptance
exists; do not adopt the candidate merely from its case score. Eight later
controls are neutral; file-reference/by-value forms lose two positions and the
file snapshot regresses. Five ECL inline controls reproduce all 352 public owners
raw; `/Ob1` fails Run's complete extent. Do not repeat them without a new premise.

## Current replay inputs

ECL current carrier:
`build/gpt-6.1-sol/ecl-case-order-20261023/maintained-current`.
Its selected typed-disabled-trace witness also remains expanded. The canonical
`build/match-linked/EnemyItemDrops` cache is **before-state 6993**, not current ECL;
historical experiments pin it, so do not rebuild over it casually.
Read the [typed trace proof](evidence/ecl-typed-trace-lifetimes/README.md) for
all 25 residual bytes / 189 bindings, 16 guards and complete 352-owner preservation.

Current Player 1380 carrier and selected draw-owner witness remain under
`build/gpt-6.1-sol/player-selection-storage-20261023`.
The [draw-owner packet](evidence/player-initialization-draw-owner/README.md)
retains all 55 residual bytes / 49 fields. Its old global attestor freezes an older
ECL source; do not relax that manifest. The current helper separately verifies
the unchanged actual 17-input / 52-file Player graph, which excludes src/EclVm.cpp.

The [artifact index](RE_ARTIFACTS.md) lists StageRealCallback, the unadopted Enemy
14396-byte candidate, earlier Player callers/support caches and all archives.
Carrier names alone do not prove compatible inputs. Older route history remains
in [historical handoffs](RE_HANDOFF_HISTORY_20261023.md) and the linked packets.

## Recovery and checks

The [cleanup receipt](evidence/handoff-cleanup-20261024/cleanup-summary.json)
archives 26 closed controls / 598 files: 59.13 MB → 0.61 MB, reclaiming 58.52 MB.
Every member was read back and hash-checked; all 23412 protected hashes agreed
during compaction. The later [CI cache cleanup](evidence/handoff-cleanup-20261024/ci-cache-cleanup.json)
removes only reproducible Python bytecode and four successful host-CI outputs.
Current/selected carriers, manifests/plans/producers, resources/tools/Wine/database
and all nine unrelated untracked D3D files remain. Archive restoration is in the
artifact index. These private archives are not in Git and must accompany a
workspace transfer. Unknown legacy content remains untouched.

Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md), relevant source/
ledger rows and status/diffs before editing. Only original Japanese 1.00a
resources/th10.exe is valid, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

```sh
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
scripts/repo-python .analysis/gpt-6.1-sol/ecl-case-order-20261023/compare-current.py
scripts/repo-python .analysis/handoff-cleanup-20261024/validate-candidate.py
```

The helpers replay retained carriers; they do not compile an independent cold
pair. Expected: maintained ECL 6995/7020 / all 189 fields, 16 guards 1617 / all 21;
Player 1380/1435 / all 49 fields, 8 guards 1753 / all 99; private ANM structural
12 differences, 36 guards 4618 / all 121, 8 bounded child paths. Fresh Ghidra attestation is required
before new target analysis. Existing preference: direct local tools, serial
nice 10 native jobs, no subagents; Ghidra readOnly/noanalysis/rollback, maxcpu 1,
heap 768m. Full mapped text 401000..465A80 inclusive: 412289 bytes, SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.

[Validation scope](evidence/handoff-cleanup-20261024/validation-summary.json)
records the retained-carrier Oracles, repository checks and all 111 public CI
stages passing. Reconstructed game compilation and fresh Ghidra analysis were
outside this cleanup; the pinned toolchain smoke ran separately.
No new exact or phase-gate credit changes.
