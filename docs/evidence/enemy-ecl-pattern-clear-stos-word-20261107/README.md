# Enemy ECL pattern initialization: native 528-byte STOSD cursor

The original Japanese TH10 v1.00a target (SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`)
contains a full **14,416-byte** `EnemyRuntimeView::DispatchEclInstruction`
owner at `0x0040E770`. The opcode's 0x210-byte bullet-pattern clearing
uses a native `REP STOSD` to write **0x84 dwords**, specifically at original
relative offset `+0x19DE` following `LEA EDI,[EDX+0x2C4]`. The pattern
cursor is computed from a script index without clamping. The original
compiler therefore forms low 32-bit, modulo-address cursor increments
rather than C++ in-array pointer arithmetic.

The live worktree contained an uncommitted attempt at the equivalent
`puVar5` cursor advance, replacing `puVar5 + 1` with a low-32-bit word
addition by `sizeof(unsigned int)`. This source expresses the actual
unchecked Win32 address arithmetic; it introduces **no** bounds check or
portable pointer guarantee. The existing 132-dword clear, pattern index,
follow-up count defaults, flags, and call order remain untouched. The
native target's `REP STOSD` was independently inspected rather than
inferred solely from old notes.

## Complete producer and selected original-bound guards

A new **eleven-input current-source** VC7.1 SP1 `/GL` graph was established
under `build/gpt-web/20261107-active-enemy-graph/current` by recompiling
three sources whose recorded input hashes changed. Header pins and all other
sources matched. The edited dispatcher TU alone was then recompiled,
linked and compared as complete functions in
`build/gpt-web/20261107-active-enemy-graph/pattern-clear-stos-word`.

| Complete PDB candidate owner | Candidate extent | Fields (candidate) | Changed ordinary bytes |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

All five preserve their field location/type/width geometry. The giant
original **14,416-byte** function remains **NONEXACT** (64-byte extent
shortfall). The 716 fields are still *candidate-bound*, **not** independently
original-bound exactness. The five related exact ECL argument leaves were
cold recompiled against the actual original and passed **87/87** complete
bytes with their target-bound operands. Seven lightweight CI stages passed;
no complete historical replay or whole product build was run.

Reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
scripts/repo-python scripts/ci.py
git diff --check
```

Private receipts are under `.analysis/gpt-web/20261107-active/`, including
`enemy-stos-exact-replay.json` and `enemy-stos-ci.log`.
