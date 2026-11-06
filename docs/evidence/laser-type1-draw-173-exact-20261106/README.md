# Type-1 laser drawing — complete 173-byte exact owner

Original Japanese TH10 v1.00a, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EnemyLaserType1View::Draw` owns `0x0041EA40..0x0041EAEC`, **173 bytes**,
with the actual ECX receiver and plain RET. The original type-1 vtable at
`0x0046DA10 + 0x0C` independently points to this entry.

The fresh two-input baseline already emitted173 bytes, but its ten field
positions disagreed with the original. Direct scalar coordinate assignments
allowed angle/Z reads and coordinate/rotation/flag stores to move. The exact
type-0 sibling supplies a concrete successful source model: an ordinary
Float3 destination helper writes X+224, Y+16 and Z. Applying that same model
locally to type-1 primary and secondary positions restores the complete
target schedule. No forced inlining, register/ABI annotation, padding,
header/layout change or compiler-profile change is used. Original helper,
class spelling and production TU remain unknown.

The primary VM at+61C remains cached for rotation, flag4 and rendering.
After its render call, the original rereads traveled and tests equality to
zero, excluding unordered values with JP at+6E to+A8. The secondary VM at+9C8
uses fresh position and renderer reads. Position stores are VM+334; the
allocating creator's offset340 is a separate field.

The first final-source link freshly compiles both actual inputs and passes
**173/173 bytes,10/10 independently original-bound fields**. These cover
224 twice,16 twice, zero, the angle helper, two renderer-owner references
and two real Draw calls. The immediate pi/2, every ordinary instruction,
the local JP destination and the return are included. Actual same-image
callee comparisons also pass AddNormalizeAngle94/all6 and renderer216/all21,
including the renderer's complete owned dispatch table.

A separate canonical cold recompiles both inputs under the established
`/GL /GS` recipe and repeats the complete parent comparison:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit laser-type1-draw --json
```

Canonical output is `build/match-linked/LaserType1Draw/`. It passes173/all10;
the same-image required callees again pass310/all27. Thus each image covers
483 complete bytes/all37 fields, with **new authored exact credit limited
to one owner /173 bytes**. The Type1 Update body and shared headers are
unchanged; no historical cohort or unrelated source consumers are replayed.

[Compact results](native-result.json) pin both images, final sources,
original field bindings, baseline rejection and separate callee checks.
Full receipts and read-only target/native dumps remain privately under
`.analysis/gpt-web/20261106-laser-type1-draw/`. No host failure scenarios,
runtime, product-closure or portability tests were run or claimed.
