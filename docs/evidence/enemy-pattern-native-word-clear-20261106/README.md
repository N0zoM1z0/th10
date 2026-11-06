# TH10 Enemy ECL bullet pattern: exactly 0x84 native words per 0x210-byte record

Original: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete `EnemyRuntimeView::DispatchEclInstruction` handles
`INITIALIZE_BULLET_PATTERN` by selecting one pattern index with the
original signed byte-scale cursor, then clearing **0x84 consecutive 32-bit
words** (0x210 bytes) at runtime offset +0x2C4 plus pattern stride 0x210.
It then publishes the native default counts, sprite/angle/speed fields,
sound and flags, and clears the first two words of the pattern's separate
position offset. The maintained `EnemyBulletPatternView` already proves
both its 0x210 size and its union of raw `words[0x84]` with the typed
field overlay, but the clear loop still used an independently recomputed
`unsigned int*` from a raw runtime pointer and offset.

The source now takes the exact same address via
`EnemyPatternAtRuntimeByte(iVar11)->words`, preserving the original signed
integer cursor, the decrementing 0x84 loop, write order and subsequent
field stores. This is the native typed-word record, not a new allocation,
`memset`, synthetic padding, private ABI, or original C++ symbol claim.

## Bounded native full-owner and original exact guards

A fresh **11-translation-unit pinned VC7.1 SP1 `/GL`** native compile
produces complete PDB owners with *identical* candidate field geometry and
**zero ordinary candidate code differences** versus the immediate preceding
source-pinned graph: Enemy ECL dispatcher **14,352 bytes / 716 fields**;
BeginSpell 2,432/108; death finalizer 162/9; viewport 330/10; complete
Enemy constructor 672/5. The original ECL dispatcher is **14,416 bytes**,
so this is a source-layout restoration and regression proof, **NOT** an
accepted exact large function.

Three directly affected, previously exact owners were explicitly selected
for original-target cold replay across only their relevant source graphs:
`enemy-read-int-argument` **16/16**,
`enemy-read-raw-int-argument` **17/17**, and the complete
`bullet-spawn-pattern-exact` **194/194**: **227/227 original-bound bytes**.
The seven standard lightweight CI checks and whitespace passed; there was
no all-unit cold replay or whole-game runtime/build test.

Private evidence:
`.analysis/gpt-web/20261106-enemy-pattern-zero-word-array/verify.py`,
`build/gpt-web/20261106-enemy-pattern-zero-word-array/current/native-result.json`,
`.analysis/gpt-web/20261106-enemy-pattern-zero-word-array/selected.json`.

Current `src/EnemyEclDispatcher.cpp` SHA-256: `51a588fe0e0092644aceaf2a7965b1f421e7d55ecb680b79ebe20b71a50ee662`.
