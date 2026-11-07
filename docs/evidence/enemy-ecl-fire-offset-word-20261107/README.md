# Enemy ECL FIRE_BULLET_PATTERN offset cursor: native 32-bit words

Original Japanese TH10 v1.00a: SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The **complete** `EnemyRuntimeView::DispatchEclInstruction` original target
extent is **14,416 bytes**; the actual pinned `/GL /GS` current-source
11-TU compiler graph produces **14,352 bytes**, still **NONEXACT**.

The dispatcher's `FIRE_BULLET_PATTERN` reads an ECL-supplied pattern index
and forms the three-word vector cursor from the independently reviewed
`bulletPatternOffsets` origin at +0x1344. The original i386 arithmetic uses
the low 32 bits of multiplication and address addition, not the undefined
C++ overflow of a signed 32-bit `int`. The maintained C++ keeps the existing
signed `iVar27` cursor *after* the arithmetic, but makes its three-word
multiplication, base offset addition, and all three subsequent word-to-byte
address computations unsigned native 32-bit operations. The three x/y/z
reads retain their separate positions in the original argument schedule.
No range check, new storage or fake ABI is introduced.

## Fresh, scoped comparison

A new pinned-compiler **current-source** baseline was independently rebuilt
from the real eleven inputs in
`.analysis/gpt-web/20261107-session-th10/enemy-native.py` (private, ignored;
`build/gpt-web/20261107-session-th10-enemy/base/`). An actual newly
compiled changed dispatcher TU was linked against the *same* other ten
current source objects. Complete PDB code extents and code-field geometry:

| Owner | Complete candidate bytes | Candidate fields | Ordinary differences vs current baseline |
| --- | ---: | ---: | ---: |
| DispatchEclInstruction | 14,352 | 716 | **0** |
| EnemyBeginSpell | 2,432 | 108 | **0** |
| EnemyFinalizeDeath | 162 | 9 | **0** |
| EnemyConfigureViewportOwner | 330 | 10 | **0** |
| EnemyFullObjectView constructor | 672 | 5 | **0** |

This is a **candidate-vs-candidate source preservation** result, not a
14,416-byte original-target exact result. The field counts above are
candidate geometry only and must not be called target-bound identities.
The current source update removes actual C++ signed-arithmetic UB without
claiming a new original-byte match.

A separate **selected original-target** replay, not any full historical
replay, cold-built the five related exact ECL argument units and passed
**87/87** bytes: `enemy-read-int-argument`, `enemy-read-float-argument`,
`enemy-resolve-float-argument`, `enemy-read-raw-int-argument`, and
`enemy-read-raw-float-argument`. Their five declared original call fields
were checked. Private receipts: `build/gpt-web/20261107-session-th10-enemy/
fire-offset-word/comparison.json` and `.analysis/gpt-web/
20261107-session-th10/enemy-offset-guards.json`.

No canonical match ledger changes; product build/runtime and exactness of the
large dispatcher remain unproved.
