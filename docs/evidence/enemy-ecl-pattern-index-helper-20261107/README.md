# Enemy ECL 0x210-byte pattern index: native checked-layout word formation

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original full `EnemyRuntimeView::DispatchEclInstruction @0x0040E770`
owns **14,416 bytes**, while the pinned native current-source 11-TU
`/GL /GS` graph emits **14,352 candidate bytes** with **716 candidate
address fields**; it remains **NONEXACT**, and these field counts are
not independently original-bound.

## Actual C++ recovery

The ECL bullet-pattern opcode family uses a 0x210-byte record stride and
script-controlled indices, with no native bounds check. The seven default
pattern fields in opcode `INITIALIZE_BULLET_PATTERN` and the `AIM_MODE`
member store were previously written with `bulletPatterns[index]`, which
can perform undefined C++ array pointer arithmetic for a script-provided
index outside the declared C++ array. A shared force-inline
`EnemyPatternAtRuntimeIndex(runtimeAddress, patternIndex)` now forms the
actual low-32-bit i386 word:

`(unsigned)patternIndex * sizeof(EnemyBulletPatternView) + (unsigned)runtimeAddress`

then passes the signed low word to the previously established
`EnemyPatternAtRuntimeByte` native member-offset overlay. It keeps the
existing field types and register-independent evaluation sites. The two
stores in `INTERPOLATE_BULLET_COUNT_BY_RANK` also add the runtime base to
their precomputed byte stride as explicit unsigned 32-bit address words,
instead of signed C++ overflow. No fake ABI, target-byte copy, extra
bounds branch, replacement storage, or runtime behavior tests were added.

## Full native trials and selected exact guards

After ECL source changes, the eleven **current** original source/header
inputs were pinned and a fresh native base recompiled the changed two
support objects rather than assuming an older cached PDB still applied.
Three isolated natural-source variants (interpolation final word, pattern
initialization index word, aim-mode index word) and a combined variant
were each built under VC7.1 SP1. A further variant reusing the earlier
`iVar11` cursor had changed code-field geometry, and was **rejected**.
The selected shared inline-helper variant was recompiled independently
from its **installed** source and compared to that freshly rebuilt base.

| Complete PDB owner | Candidate size | Candidate fields | Ordinary byte differences versus base |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

All five complete candidate owner extents and field offset/type/width
geometry match the new current-source base. This **does not** establish
original-target exactness for the still shorter dispatcher. Five directly
affected **previously exact** ECL argument leaves were selected for a
cold original-bound replay: `enemy-read-int-argument`,
`enemy-read-float-argument`, `enemy-resolve-float-argument`,
`enemy-read-raw-int-argument`, `enemy-read-raw-float-argument`.
All pass **87/87** original bytes and their declared target fields.

Private reproduction: `.analysis/gpt-web/20261107-turn2/
enemy-dispatcher-more-words.py`, `enemy-pattern-index-helper-trial.py`,
`enemy-index-installed-exact-guards.json`, `enemy-index-installed-proof.log`,
and `build/gpt-web/20261107-session-th10-enemy-v2/
installed-pattern-index-helper/` (native PE/MAP/PDB and full comparison).
No full historical cold replay or full game product/runtime claim.
