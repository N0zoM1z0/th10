# TH10 reconstruction handoff

Updated 2026-10-23. Reconstruction remains **active-incomplete**. Player startup
now binds its VM call to the real shared ANM initializer. Headers, layouts,
compiler profiles, canonical recipes and exact ledgers are unchanged. Checkpoints
use `gpt-6.1-sol: ...`.

## Current state

Latest maintained change: [Player's real VM binding](evidence/player-initializer-binding/README.md).
The previous byte-improving source checkpoint is2b03d2b, Stage packed-color
scheduling. Cleanup fd366f2 and six neutral Stage controls0902294 remain recorded.

| Owner / metric | Maintained state |
| --- | --- |
| ECL Run / target 7020 | 7020 bytes; 6993 matching, 27 ordinary differences; 189 fields/62 local destinations; 171-byte float callee exact |
| ANM ExecuteScript / target 9964 | 9964 bytes; nonexact; case 7541/7995, 90/92 target-sized groups, gap 2 |
| Enemy dispatcher / target 14416 | 14336 bytes; frame 2C4; 181 selectors/108 groups; case 3245/11128, 45 target-sized groups, gap 328 |
| StageExecuteScript / target 1868 | 1868 bytes; frame 8C; nonexact; case 1443/1619, 13/15 target-sized groups, gap 2 |
| PlayerInitialize / target 1435 |1116 bytes/nonexact in the 17-TU graph; now calls real shared VM initializer; structural98/1267 is diagnostic only|
| Authored / source / exact | 734 functions/273579 bytes; source 613/247945; exact 409/74716 |
| Source-present nonexact / absent | 204 / 121 authored functions |
| Overall exact | 1252 functions/141647 bytes; 1097 source mappings |
| Windows product / runtime | Open; semantic reconstruction and portability not started |

Case scores and structural diagnostics grant no exactness credit. Live ledger
counts are in [PROGRESS.md](PROGRESS.md). There are 1776 reviewed candidates,
518 excluded origins and 524 indeterminate origins; the denominator remains open.

## Latest Player/Stage work and next route

Target 0x4248B1 in the 1435-byte Player initializer directly calls 0x404F30 with VM at
Player+0x14, resource at +0x10 and script 0. Player.cpp now uses the existing full ANM
boundary view and removes its declaration-only helper. In the measured graph,
one unresolved placeholder disappears; private and maintained compilations have
identical raw bytes across all 366 complete public PDB owners. Player remains
1116 bytes; masked positions 126->98/1267 do not establish target improvement.
This is a call-identity/source-closure repair, with no new exact credit.

Next work may attack the remaining real dependency bindings in this large Player
initializer, or rotate to another large owner. Do not replace that priority with
small-function closure. Keep the before/after 17-TU carriers under
`build/gpt-6.1-sol/stage-initializer-formals-20261023` as identified in the evidence.

The adopted [color view](evidence/stage-color-storage/README.md) changes exactly
nine raw bytes: five target repairs, zero target-equal losses, four still
different. All other 1859 parent bytes, including linked fields/table words,
remain unchanged. The canonical nine-TU StageCreate graph is refreshed at
`build/match-linked/StageRealCallback`; keep it and both before-state witnesses
under `build/gpt-6.1-sol/stage-script-values-20261023`.

The [integer-lifetime controls](evidence/stage-integer-lifetimes/README.md)
are now fully recovered. Timer receiver precomputation and four-byte memcpy
produce the complete baseline raw body. An ordinary global-store helper is
baseline-equal after 20 independently bound address fields are replayed. Sharing
one operand local between 12/13 adds four ordinary differences. All four retain
1868 bytes and 11 complete guards covering 987 bytes/all 36 fields. The declared
`global-forceinline` control was **not executed**.

Continue the large-owner exact work, with Stage 403C80/1868 as the current route:

- Opcode13 target 404202 uses EAX and its five-byte accumulator store; maintained
  native 401792 uses **EDX**, adding one byte. The prior ECX note was wrong.
- Opcode12 target 4041CE uses DL; maintained native 40175E uses CL. SetCurrent(0)
  is fully inlined in both spans. Receiver precomputation did not repair it.
- The [six vector/visibility controls](evidence/stage-vector-lifetimes/README.md)
  are complete-parent neutral: five raw-equal; moving the existing initializer
  definition into Stage's TU is equal after 3 independently bound fields. All
  retain 11 guards 987/all 36 and all 15 table entries. Shared vectors, explicit
  component fills and named VM operands did not repair register allocation.
- Member argument reversal is now tested across consistent snapshots/all nine
  compiled TUs: complete Stage and initializer bytes are neutral. Ordinary free/
  cdecl emits Stage 1872 and initializer198/plain RET and is rejected. Adding all
  then-maintained 26 named calls/16 fresh TUs and the corrected Player call/17 TUs
  leaves Stage baseline-equal after 13 independently bound address fields.
- Native VM initializer remains 200 versus target 202, with EDX versus targetESI
  receiver. Current audit confirms 28 direct calls in11 reviewed owners, replacing
  the old 23/eight-owner inventory. Source now has 27 explicit member calls; the
  unmapped 0x404450 owner and two unowned raw E8 hints keep caller closure open.
  These counts prove no original signature, class, translation unit or runtime.
- Do not repeat those six forms, the four integer controls, low-byte color
  snapshot, six spline copies or signed channel snapshots. Preserve the complete
  parent, physical group order and strict local offsets. Establish exhaustive
  independent target field bindings before any whole-parent exact claim. Keep
  ANM/ECL/Enemy open; no small-function closure substitutes for this priority.

The three larger owners' latest exclusions and evidence are indexed in the
[historical handoffs](RE_HANDOFF_HISTORY_20261023.md). In particular, seven old
ECL trial labels were wrong: consult the
[source-route audit](evidence/ecl-scratch-route-audit/README.md) and actual patches.

## Preserved state and artifact restoration

- Keep `build/match-linked/EnemyItemDrops` and useful unresolved candidates,
  especially `build/gpt-6-astra-medium/enemy-selection-graph-20261022/scoped-rank-shared-aim`.
  That 14396-byte Enemy candidate has gap 298 and is not adopted or exact.
- Preserve all nine unrelated untracked `docs/evidence/d3d-adapter-format/` files,
  including its `.gitattributes`; they remain unstaged and hash-unchanged.
- [Today's cleanup receipt](evidence/handoff-cleanup-20261023/README.md) records
  the verified lossless archive of 329 historical compiler reports and removal
  of 60 redundant Stage build files, saving 180720282 logical bytes in total.
- [The preceding cleanup](evidence/handoff-cleanup-20261022/README.md) holds 28
  historical GUI reports. Both archives remain ignored private working state.
- Small manifests, patches, source snapshots, producers and current Stage/ANM
  replay inputs remain expanded. Some historical scripts expect archived JSON:
  restore the required member before using them. Do not restore historical
  source or loosen old manifest pins to make an old producer run.

From the repository root, restore today's archive with:

```sh
tar -xzf .analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz -C .
```

Append one exact original path from the inventory to restore only that member.
The old GUI archive has its separate restoration command in the preceding receipt.
Cleanup recovery manifest/producers are in `.analysis/handoff-cleanup-20261023`;
Stage controls are in `.analysis/gpt-6.1-sol/stage-dispatch-integer-lifetimes-20261023`.

## Resume safely

Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md),
relevant source/ledger rows and this handoff. Recover status/diffs before editing.
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

Operator preferences: use direct local tools without Factory MCP, serial nice10
native builds, readOnly/noanalysis/rollback Ghidra with maxcpu1/heap768m, and no
subagents without authorization. Attest the database before new target analysis.
Full mapped text 401000..465A80 inclusive is 412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Use focused affected complete-owner replays; full CI is reserved for meaningful
source milestones. Authorized checkpoints are committed and pushed, with remote
HEAD verified. Original class/TU/flags, data ownership and product/runtime remain
independent unknowns.

The 17-TU before/after graphs each pass 11 Stage guards 987/all 36 and the existing
273-byte VM initializer:12 whole checks/1260/all 36. Four Chain contracts reject
there at unequal complete extents in both images; their canonical 14-TU current-
source cold separately passes all 4/838/all 58 in the original recipe. No partial
extent is accepted. Preserve its archived previous carrier. The unchanged
Stage/Enemy canonical witnesses, nine D3D files and prior useful candidates remain.
Cleanup removes 81 superseded native files/3964133 bytes plus 160 unchanged,
uncompiled snapshot copies/2470106 bytes. See the latest evidence packet for
checks and retained carriers. No full CI or runtime validation is claimed;
exact counts stay unchanged.
