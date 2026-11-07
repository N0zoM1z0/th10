# TH10 enemy ECL: native initialization and extra-record index words

Original target: Japanese TH10 v1.00a (SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).
The complete original `EnemyRuntimeView::DispatchEclInstruction @0x0040E770`
contains **14,416 bytes**. The current 11-TU VC7.1 `/GL /GS` build produces
**14,352 bytes / 716 candidate code-address fields**, still **NONEXACT**.

The indexed bullet pattern in `ENEMY_ECL_INITIALIZE_BULLET_PATTERN`
is selected by a 0x210-byte low-word multiplication and addition to the
runtime base. The older C++ computed the same expression as signed `int`,
which can invoke undefined signed overflow for an ECL-supplied index.
The new source uses unsigned 32-bit intermediate words and retains the
native final signed cursor passed to the already-typed record overlay.

`ENEMY_ECL_SET_BULLET_EXTRA` selects overlapping 0x18-byte records using
`patternIndex * 22 + slotIndex`, then computes a byte-scaled record cursor
and the floatB store using an eight-byte-scaled index expression. Both
operations use the original i386 low 32-bit word behavior, so the new C++
explicitly makes all multiplications and additions unsigned. The recovered
24-byte record, real member offsets, exact read/write evaluation order,
field types and underlying target ABI are unchanged. No bounds checks,
new allocations, fake register forcing, target patching or new storage.

## Native source and selection evidence

This work continues the maintained source and its real eleven-TU producer
context rather than resetting earlier changes. `src/EclVm.cpp` had itself
changed since the previous baseline; a new current-source producer graph
therefore recompiled both that translation unit and the dispatcher, while
reusing support object files only with matching source and header pins.
Separate experiments compiled the pattern initialization, extra-record
indices, extra floatB store and combined variants. The installed combined
source was **compiled again**, not inferred from earlier candidates.

Compared to the fresh current-source baseline, the complete PDB owners are:

| Owner | Current bytes | Candidate address fields | Ordinary byte differences |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

The field counts are **candidate geometry**, not original bound identities.
The dispatcher is still 64 bytes shorter than the **original** and remains
nonexact. This is C++ semantics recovery, not new whole-owner acceptance.

The *installed-source* selected cold replay of five canonical exact ECL
argument leaves (`enemy-read-int-argument`, `enemy-read-float-argument`,
`enemy-resolve-float-argument`, `enemy-read-raw-int-argument`,
`enemy-read-raw-float-argument`) passed **87/87** original bytes and all
five target-bound fields. The one complete eleven-TU native producer and
scoped cold replay were checked; no full historical replay was run.

Private reproduction and pinned native receipts are preserved at
`.analysis/gpt-web/20261107-session-th10/enemy-pattern-word-probe.py`,
`enemy-native-v2.py`, `enemy-pattern-trials.json`,
`enemy-pattern-final-exact-guards.json` and
`build/gpt-web/20261107-session-th10-enemy-v2/installed-pattern-words/`.
Whole-game runtime behavior and target exactness remain open.
