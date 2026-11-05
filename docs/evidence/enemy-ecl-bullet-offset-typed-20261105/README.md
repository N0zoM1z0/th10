# TH10 14KB ECL boss: prove bullet-pattern position-offset vector

Target: original Japanese TH10 v1.00a `0x0040E770`, complete **14,416-byte**
`EnemyRuntimeView::DispatchEclInstruction`; target SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
**The whole owner remains NONEXACT.**

The FIRE_BULLET_PATTERN branch reads three anonymous floats through
`runtimeAddress + ((patternIndex * 3 + 0x4D1) * 4) + {0, 4, 8}`.
`0x4D1 * 4 == 0x1344`, independently the asserted starting offset of
`EnemyRuntimeView::bulletPatternOffsets[8]`; each element is the actual
12-byte `PlayerFloat3`. The rewritten complete boss reads `x/y/z` through
this vector view and retains the native address/index evaluation sequence,
with the destination pattern record accessed between Y and Z as before.

The **real /GL 11-source VC7.1 linked** test included two rejected natural
source variants: direct `bulletPatternOffsets[index]` indexing and one
cached pointer. Both increased the complete candidate owner from **14,352**
to **14,368** bytes and shifted linked-field locations; even though their
addresses are equivalent, they are not byte-neutral and their target exactness
was not established. A declaration inside an unbraced `switch` case was also
rejected by VC7.1 (C2360), then corrected within the experiment.

The retained vector-member form preserves the original index-register and
operand-lifetime shape: the complete candidate is **14,352/14,352** against
the previous candidate, with **716/716** linked fields in identical positions
and **zero ordinary candidate-to-candidate byte differences** after masking
only those fields. This is *not* a 14,352-byte original-target match; the
original is still 14,416 bytes. The native result is in `native-result.json`;
ignored local build is `build/gpt-web/20261105-ecl/boss-typed-bullet-vector-words/`.

Selected independently rebuilt exact neighbors `enemy-read-int-argument`
(16 bytes) and `enemy-read-raw-float-argument` (26 bytes) remain exact.
No full cold replay, whole-product closure, or runtime validation was run.
