# Enemy ECL INITIALIZE_BULLET_PATTERN: preserve raw float/DWORD default words

Target is Japanese TH10 v1.00a, original SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

At target Enemy ECL `INITIALIZE_BULLET_PATTERN`, the native 0x210-byte pattern
record is cleared with 0x84 DWORD stores, then the count and four default
position/velocity bits are published. The initial `angleA` at packet +0x10
is the **raw DWORD zero**, `speedA` at +0x18 is literal
**0x40000000** (IEEE-754 binary32 2.0), while the first two words of the
12-byte bullet pattern offset are zeroed. These are bit-pattern stores, not
fresh floating arithmetic or implicit float conversions.

Maintained `EnemyBulletPatternFieldsView` now overlaps `float angleA` with
`unsigned int angleABits` and `float speedA` with `unsigned int speedABits`,
all at previously verified +0x10/+0x18; both native offsets are asserted.
`EnemyPositionDwordView` (three raw 32-bit words, twelve bytes) already
proves the pattern offset layout. The four real opcode stores now use
these typed bit aliases rather than independent `unsigned int*` casts.
The original clear-loop, counts, sound, flags, and field write order are
unchanged. No allocation, rounding, new bounds guard or historical original
source spelling is claimed.

## Bounded actual native proof

A fresh **11-translation-unit pinned VC7.1 SP1 `/GL`** compile with the
changed `Enemy.hpp` and `EnemyEclDispatcher.cpp`, compared with the actual
preceding source native graph, preserves the complete native candidate
PDB owners, linked field geometry, and **zero ordinary candidate byte
regressions**:

- `EnemyRuntimeView::DispatchEclInstruction`: **14,352 bytes** and 716
  same-offset/type candidate fields;
- `EnemyBeginSpell`: 2,432 bytes / 108 fields;
- Enemy death finalizer 162/9, viewport 330/10, full constructor 672/5.

The real original Enemy ECL dispatcher is **14,416** bytes. Candidate
regression equality is **not** a full original-bound match; the big owner
remains **NONEXACT**, without new canonical credit.

Selected original-target exact cold replay for the direct integer/raw
argument interfaces passed `enemy-read-int-argument` **16/16**,
`enemy-read-raw-int-argument` **17/17** and
`enemy-read-raw-float-argument` **26/26**, total **59/59** accepted bytes.
Seven lightweight CI checks and whitespace pass. No full historical
cold replay or whole-game runtime/product claim.

Private source-pinned reproduction:
`.analysis/gpt-web/20261106-enemy-pattern-default/verify.py`,
`build/gpt-web/20261106-enemy-pattern-default/current/native-result.json`,
`.analysis/gpt-web/20261106-enemy-pattern-default/selected.json`.

Source SHA-256: `Enemy.hpp` `60678bb9dff027560a6f8c023f4a43f02d69aa9b50e4fbd73463a338755c56d3`, `EnemyEclDispatcher.cpp` `dcc6c83256c9ecba1134e00a85261bb797256db35b4240abc7b9096578bd4834`.
