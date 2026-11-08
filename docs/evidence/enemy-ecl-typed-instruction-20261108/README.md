# TH10 Enemy ECL dispatcher: signed opcode and typed current-instruction view

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## Source recovery

`EnemyEclInstructionView::opcode` is now a signed `short`, matching the
corresponding `EclVmInstruction::opcode` field in the generic VM instruction
view. The target dispatcher loads the word and sign-extends it before the
`0x100..0x1B4` switch (`mov ax,[instruction+4]; movsx ecx,ax`). The former
unsigned declaration was an unnecessarily weak description of that shared
instruction stream.

`EnemyRuntimeView::DispatchEclInstruction` now keeps its current instruction
as `const EnemyEclInstructionView *instruction`, rather than carrying that
pointer as an integer `iVar27` and repeatedly casting it at field access
sites. `EnemySpawnScalarWordAt` likewise accepts the typed instruction view.
`iVar27` remains an integer for the other, unrelated index/count lifetimes;
there is no blanket rename or pointer-to-integer semantic change.

## Fresh selected 12-TU graph

The complete selected Enemy/ECL dependency graph was cold-linked with the
current source, using the `enemy-apply-bullet-clear` entry recipe. This is a
focused 12-TU boss graph, **not** an all-unit cold replay. The full PDB
contribution for `EnemyRuntimeView::DispatchEclInstruction` is **14,416
bytes**, matching the original owner extent.

| Measure | Before typed view | Current |
| --- | ---: | ---: |
| Candidate PDB owner | 14,416 | 14,416 |
| Candidate relocation fields | 720 | 720 |
| Comparable ordinary bytes matching original | 1,252 / 11,536 | 1,255 / 11,536 |
| Ordinary byte differences | 10,284 | 10,281 |

The three-byte gain is a real candidate-to-original comparison improvement,
but the body remains **NONEXACT**. The 720 fields are candidate relocation
fields, **not** a claim that 720 original target fields have been independently
bound. No exact ledger entry or new exact credit was added.

## Selected original-bound regression guards

Twenty-one already-exact owners from the changed dispatcher translation unit
were replayed in focused groups. All passed: **2,363 / 2,363 bytes**.

| Group | Owners | Exact bytes |
| --- | ---: | ---: |
| EnemyDispatcherHelpers | 5 | 413 |
| AnmReleaseSlot | 6 | 142 |
| EnemyBulletBounds | 1 | 91 |
| EnemyItemDrops | 8 | 1,653 |
| EclWorldAnchorFromEnemyDispatcher | 1 | 64 |
| **Total** | **21** | **2,363** |

The guard replays are scoped to these owners; no whole-repository historical
replay was run. `native-result.json` records source/object pins, the complete
large-owner comparison and the individual guard receipts.
The guard receipts above belong to the signed-opcode/typed-instruction
source epoch before the subsequent `EnemyBeginSpell` receiver-type correction.
The later focused relink below rechecks both large owners, but does not claim
a new 21-owner guard replay for that later source hash.

## Second boss baseline: EnemyBeginSpell

A focused relink using the same 11 support objects and recompiling the current
dispatcher TU emits `EnemyBeginSpell @0x00409280` as a complete 2,432-byte PDB
contribution at candidate `0x00416F90`, matching the target physical extent.
The candidate has 108 relocation fields (67 DIR32, 41 REL32); after masking
those candidate fields, **1,403/2,000 ordinary bytes match** and 597 differ.
These fields are not an independently original-bound linkage proof, and the
function remains NONEXACT.

The target call site at `0x00410F3A` pushes `value`, inline name, `spellId`, and
state in reverse order. At the callee, the name is loaded from entry
`ESP+0x0C`, state from `ESP+0x04`, and `spellId` from `ESP+0x08`; the store at
`0x004092E3` writes that second argument to `state+0x3788`. The maintained
signature is therefore `EnemyBeginSpell(state, spellId, name, value)`. An
intermediate source revision had the second and fourth parameter names
reversed; that was a source-level bug, not a target ambiguity. The corrected
signature preserves the 2,432-byte extent and 1,403/2,000 ordinary-byte
comparison. See [`receiver-type-relink.json`](receiver-type-relink.json) and
[`enemy-begin-spell-baseline.json`](enemy-begin-spell-baseline.json).
The seven-entry stage-switch table at function offset `+0x964` also has
identical target-relative destinations in both images: `0x6E4`, `0x74B`,
`0x796`, `0x7E1`, `0x82C`, `0x877`, and `0x8BF`. This validates the switch
layout and branch destinations, not the entire function body. See
[`stage-switch-table.json`](stage-switch-table.json).
## Reproduction

Run from the repository root:

```sh
scripts/repo-python docs/evidence/enemy-ecl-typed-instruction-20261108/replay-selected-12tu.py
```

This rebuilds only the selected Enemy/ECL 12-TU graph, then compares the
complete dispatcher PDB contribution against the pinned original. The
machine-readable result in this directory was produced from that graph.
