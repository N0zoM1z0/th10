# TH10 14,416-byte enemy ECL dispatcher: typed motion/laser/pattern addresses

Target: attested Japanese TH10 1.00a original SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `EnemyRuntimeView::DispatchEclInstruction` physical target at
`0x0040E770` is **14,416 bytes** and **remains NONEXACT**. This checkpoint
restores real source-level ownership, rather than claiming target matching.

## What was recovered in the maintained boss source

`src/EnemyEclDispatcher.cpp` previously built pointers to native motion
records and the initial bullet-pattern payload from untyped
`runtimeAddress + offset` arithmetic. The original `EnemyRuntimeView`
layout already establishes `offsetMotion` at +0x58 and `baseMotion` at
+0x84, with position x/y/z as adjacent 32-bit words. Nine instances were
replaced with naturally typed positions, typed motion object addresses and
`bulletPatterns[0].words[8]` (the native +0x2e4 source for two laser-copy
paths). The final copy-to-motion path preserves **bitwise word stores**, not
float conversions, to retain the original data representation.

Four other raw pointer/index computations are equivalent to the existing
12-byte `bulletPatternOffsets[]` records at +0x1344. The reset path now
zeros the original X/Y **words**, while the setter publishes X/Y as the
original floats. Both continue to use the original decoded script indices;
no new buffer, layout, out-of-range clamp, or fake ABI was introduced.

## Complete native codegen preservation

Two actual, sequential, selected 11-source pinned VC7.1 SP1 LTCG builds
recompiled `EnemyEclDispatcher.cpp` and the newer Player support source
rather than reusing a stale compiler object. All other reused supports were
checked against source/object SHA-256. Each build reported the full
**14,352-byte PDB candidate**, the same **716 decoded candidate address
fields with identical offsets and types**, and **zero ordinary-byte
changes** from its immediately preceding selected source epoch after
normalizing those candidate address fields. The final source SHA is stored
in [native-results.json](native-results.json) along with both comparisons.

These are **candidate-to-candidate** preservation checks, not original
14,416-byte target equality. Neither full-target geometry nor static
callee identity is newly accepted by this check. The original owner and
whole executable remain NONEXACT; no `config/matches.csv` entry or fake
partial-byte credit is created.

The reproducible ignored driver is
`.analysis/gpt-web/20261104-live/large-enemy-typed-link.py`, with actual
PE/MAP/PDB outputs in
`build/gpt-web/20261104-live-enemy/motion-bullet-address-typed/`
and `.../indexed-bullet-offsets-typed/`. Only affected already-exact units
are cold replayed in scope; all-unit cold replay, original source ancestry,
full-game product, runtime correctness and portability remain separate gates.
