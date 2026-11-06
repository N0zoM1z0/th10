# Enemy ECL rank interpolation: native 32-bit word arithmetic

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The source-present **14,416-byte** `EnemyRuntimeView::DispatchEclInstruction`
contains ranked bullet-pattern counts and a ranked integer operand selection.
Direct original disassembly at `0x00410A91..0x00410AA3`,
`0x00410AC0..0x00410AD2`, and `0x00411B50..0x00411B6B` independently
shows `SUB`, `ADD 0x400`, low-word `IMUL`, then signed quotient formation
(`CDQ`, `AND 0x7FF`, arithmetic shift). The final integer selection also
performs a **32-bit ADD after** division. The original 32-bit arithmetic
wraps even if a script supplies extreme integers.

Previously, C++ expressions for three interpolated products and the final
integer publication used signed subtraction/addition/multiplication. In
standard C++, overflowing these `int` expressions is undefined behavior.
The maintained source now performs the subtractions, rank bias additions,
products and final integer publication explicitly in the **existing
32-bit unsigned word representation**. The same low 32 bits are then used
for the *original signed division* and the destination word. There are no
new bounds guards, widened intermediate types, ABI annotations, indirect
stand-ins, or target-byte patches. The conversion back to signed is pinned
to the actual VC7.1 i386 compiler semantics and is not a portability proof.

## Four tested source edits and complete native graph

Three independent per-site candidates and a combined candidate were compiled
with pinned VC7.1 SP1 from the true `src/EnemyEclDispatcher.cpp` under the
11-TU `EnemyRuntimeUpdate` graph; an additional independent final-word
publication candidate and an all-four combination were also built. Every
experiment used a fresh **changed-TU recompile and relink** with ten support
objects reused only after their actual source/header hashes were verified.
Unchanged input objects are from a separately **full 11-TU baseline cold
compile** at the current source epoch, not an unverified historical binary.

The final installed, commented source was compiled and relinked again:

| Actual complete PDB-owned function | Candidate bytes | Fields | Ordinary candidate-vs-baseline differences |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewport` | 330 | 10 | 0 |
| `EnemyFullObjectView` constructor | 672 | 5 | 0 |

All field **positions/types/widths** match in that current source graph.
These are **candidate-vs-candidate preservation comparisons, not independent
original target bindings**. The original dispatcher's size is **14,416**;
the 64-byte gap persists, so it is explicitly **NONEXACT**. These whole-owner
comparisons are not credited to the canonical exact ledger.

Selected original-bound canonical cold replay uses the real nine-TU
`DispatchEclInstruction` entry but selects **only** the five affected ECL
operand helper owners:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument \
  --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
```

All pass, **87/87 bytes** with all five independently bound callee fields.
No historical all-unit cold replay was run. No new exact credit is claimed.

Pinned trial and final receipts:
`.analysis/gpt-web/20261106-chat-th10-enemy-rank/trial-results.json`,
`maintained-result.json`, `selected.json`, plus actual
PE/MAP/PDB in `build/gpt-web/20261106-chat-th10-enemy-rank/maintained/`.
Installed `EnemyEclDispatcher.cpp` SHA-256:
`7d30a9b073172e0e1867972a0530d92400a9f6f432d81c17214e815b71e31471`.
Original compiler partitions, whole-game linkage/runtime behavior and
portability remain open.
