# TH10 BulletManagerView::SpawnBulletPattern — complete 194-byte native exact

Target: original Japanese TH10 v1.00a at original address
`0x004073E0..0x004074A1`, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The game-owned `SpawnBulletPattern` source in `src/BulletManager.cpp`
calculates the player's angle from a pattern's position, iterates the
nested `count2 × count1` spawn indices through the real
`BulletManagerView::SpawnSingleBullet` member, and queues the pattern's
spawn sound when the encoded transform flags include 0x200. The complete
native function uses its original private descriptor/manager register
schedule; no fake calling convention, target-code patch, synthetic padding
or speculative null/overflow guard was introduced.

## Independent original-bound complete-owner verification

The earlier large-context diagnostic produced **195 candidate bytes** and
was unable to claim original codegen credit. A distinct coherent four-TU
pinned VC7.1 SP1 `/GL /GS` graph, rooted in the *actual*
`BulletManagerView::UpdateBullets` and including the current maintained
`BulletManager.cpp`, instead naturally emits the **complete 194-byte**
PDB-owned `SpawnBulletPattern`. This is not a 194-byte prefix or a partial
inline slice. Manual reads of the original executable establish all six
native binding obligations, independently verified against candidate MAP/PDB:

- `+0x05` DIR32: `g_Player` at `0x00477834`
- `+0x1E` and `+0x2F` DIR32: binary32 zero constant at `0x00470B04`
- `+0x79` REL32: `SpawnSingleBullet` at `0x004067D0`
- `+0xB1` DIR32: `g_MainSoundOwner` at `0x00492590`
- `+0xB6` REL32: actual `EnemySoundQueueView::QueueSoundCue` at `0x0043DD10`

The first strict original-bound whole-owner comparator passed **194/194
original bytes and all 6/6 target symbol/target identities**, with no
normalization gaps or local code elisions. Initial native image SHA-256:
`cbcb3cab293c7e7d1a64abd1f4aeb84a78b61edcbf7d79b423452e24d06bd04a`.
The first image/MAP/PDB were archived under private
`.analysis/gpt-web/20261106-spawn-pattern-exact/first-artifact/`.

The canonical source-preserving registration
`[units.bullet-spawn-pattern-exact]` then performed a **separate
current-source cold rebuild of only the required four-TU group**, selected
with `scripts/repo-python scripts/replay-exact-units.py
--unit bullet-spawn-pattern-exact --json` (not the historical complete
cohort). The second complete original-bound comparison again passed
**194/194 and 6/6 fields**, now with newly generated PE SHA-256
`e8865ae637e9056ff924a1e5e8c0a88bb7d50b2e3bde3c1d54bd4c7d914452b2`.
Reproducible canonical output is in
`build/match-linked/BulletRuntimeDeactivate/`; private selected receipt is
`.analysis/gpt-web/20261106-spawn-pattern-exact/canonical-replay.json`.

**New canonical authored exact credit: one whole 194-byte owner.** The
legacy isolated 195-byte result describes a different source/compiler
context, not the current canonical result. This does not assert original
source spelling, physical TU, complete game product, runtime equivalence,
private ABI closure or portability.

Current `src/BulletManager.cpp` SHA-256: `2bda79b9659785c784bb33d7c9b453c4cfefb249d37631d63cc20956d18f25aa`.
