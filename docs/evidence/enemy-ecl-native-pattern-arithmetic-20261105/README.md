# TH10 full ECL dispatcher: derive bullet-pattern stride and base from native types

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete 14,416-byte original `EnemyRuntimeView::DispatchEclInstruction`
contains many ECL pattern opcodes: initialize, fire, aim, sprites, angle,
count/speed, difficulty/rank interpolation, and sound. Source-level pointer
cursors in these paths repeatedly used literal `0x210` pattern stride and
`+0x2C4` base. The same two values are independently layout-checked in
`Enemy.hpp`: `sizeof(EnemyBulletPatternView) == 0x210` and
`offsetof(EnemyRuntimeView, bulletPatterns) == 0x2C4`.

All **41** pointer-base occurrences and **18** pattern-stride expressions
inside the *complete* dispatcher now use those native type declarations
(with an explicit `int` conversion to retain the original signed pointer
arithmetic and register-lifetime expression). The original dynamic pattern
index, script argument evaluation order, field aliases, and per-opcode
branch structure remain unchanged. No new private array bounds or original
C++ spelling is assumed; the actual target accesses remain unguarded.

A real pinned eleven-source VC7.1 `/GL` graph recompiles the entire modified
`EnemyEclDispatcher.cpp`, reusing ten unchanged object files only after
checking their exact source hashes. Its complete ECL dispatcher remains
**14,352 candidate bytes** versus **14,416 original**, with **716** linked
fields at unchanged positions/types and **zero** ordinary candidate-to-
candidate byte differences. Independently complete companion owners
`EnemyBeginSpell` (2,432 candidate bytes / 108 fields),
`EnemyFinalizeDeath` (162 / 9), camera (330 / 10), and the separate-context
drop core (505 / 27) are also unchanged in this graph. These are
candidate-preservation measurements, **not** full original-target matches.

Selected canonical original-bound replays remain exact:
`enemy-ecl-resource-load-package` **239/239**,
`enemy-read-int-argument` **16/16**, and
`enemy-read-raw-float-argument` **26/26** — **281/281 previously credited
bytes**, no new exact credit. Seven-stage lightweight CI and whitespace
checks pass. No historical full cold replay, product build, or runtime
claim. Pinned source hashes and comparisons: `native-result.json`;
ignored native artifact:
`build/gpt-web/20261105-bullet-pattern-address/full-ecl-boss/`.
