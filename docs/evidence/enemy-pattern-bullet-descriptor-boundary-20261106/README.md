# Enemy ECL FIRE_BULLET_PATTERN: exact 0x210-byte source-to-spawn descriptor boundary

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The Enemy ECL large dispatcher chooses a bullet pattern, computes its
world-position offset and tests the original player-protection squared
radius before invoking `BulletManagerView::SpawnBulletPattern` with the
*same in-place* bullet pattern record. Maintained code previously
reassembled the receiver's runtime byte offset as a raw `int`, then cast
the result directly to `BulletSpawnDescriptorView *`. The source already
has `EnemyPatternAtRuntimeByte`, a target-verified typed expression for
the same signed byte cursor and `EnemyBulletPatternView` record.

This checkpoint uses that typed record locator at the actual spawn call
while preserving the identical `iVar26` lifetime, original protection
branch and C++ callee. A new compile-time field overlap contract verifies
both views have size **0x210**, with matching native `spriteId/bulletType`
at +0, `spawnPositionX/position` at +4, first count at +0x1F4, and
transform flags at +0x1FC. These checks prove the in-place descriptor
contract without allocating, copying, changing floating-point arithmetic,
hoisting a pointer across ECL reads, or fabricating original class names.

## Complete native caller plus exact callee proof

The pinned VC7.1 SP1 **11-translation-unit `/GL`** rebuild includes
edited `EnemyEclDispatcher.cpp`. Against the immediately preceding
source-pinned PDB graph, **all five entire owner extents, candidate
link-field geometry and ordinary code bytes are identical**:

- Enemy ECL extended dispatcher **14,352 candidate bytes / 716 fields**;
- BeginSpell 2,432/108, death finalizer 162/9,
  viewport 330/10 and full Enemy constructor 672/5.

The original Enemy ECL dispatcher is **14,416 bytes**; this is a concrete
source-model improvement and candidate regression proof, **not** an exact
large-function claim. The separately registered original-bound
`bullet-spawn-pattern-exact` callee is itself **194/194 bytes with six
independently bound target fields**, verified by a fresh explicitly
selected four-TU cold replay. The changed Enemy source's directly related
`enemy-read-int-argument` leaf also selected-replayed **16/16** original
bytes. These are **210/210 existing exact bytes** from two *scoped*
native graphs; neither creates new canonical exact credit. Seven-stage
lightweight CI and whitespace checks passed; no historical full cohort,
product/runtime, ABI or portability proof was performed.

Private witnesses:
`.analysis/gpt-web/20261106-enemy-bullet-spawn-alias/verify.py`,
`build/gpt-web/20261106-enemy-bullet-spawn-alias/current/native-result.json`,
`.analysis/gpt-web/20261106-enemy-bullet-spawn-alias/selected-operand.json`,
`.analysis/gpt-web/20261106-enemy-bullet-spawn-alias/selected-spawn.json`.

Installed `src/EnemyEclDispatcher.cpp` SHA-256: `0f496115327b594e053e3aa5e92f37addf5c968a23a613649b0b4376d5e1b034`.
