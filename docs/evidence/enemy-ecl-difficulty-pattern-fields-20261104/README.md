# Enemy ECL full dispatcher: typed difficulty/rank bullet-pattern fields

**Real source repair:** In the complete EnemyRuntimeView::DispatchEclInstruction
at original Japanese TH10 1.00a address 0x0040E770, four remaining untyped
runtime-relative writes in ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY and
ENEMY_ECL_INTERPOLATE_BULLET_COUNT_BY_RANK now address the existing
EnemyBulletPatternView 0x210-byte record and use its typed speedA,
speedB, countA and countB fields.

The previous expressions combined index * 0x210 with runtimeAddress and
literals +0x2DC/+0x2E0 or +0x4B8/+0x4BA. The typed expressions retain the
same runtime origin and scaled index, use record base +0x2C4, and select
offsets +0x18/+0x1C and +0x1F4/+0x1F6. Independent struct checks in
src/Enemy.hpp attest those offsets. Input evaluation order, signed division,
short-width results, difficulty selection and rank interpolation remain
unchanged. No range checks, shadow storage, ABI changes or target patches.

## Whole-owner compiler comparison

The pinned MSVC 7.1 SP1 /GL real 11-source Enemy-update graph was built with
the modified dispatcher freshly compiled. Unchanged support objects are
reused only under SHA-256 source and object checks; changed sources recompile.

The retained native-result.json records:

- 14,348-byte complete candidate PDB contribution, identical prior size
- 716 decoded linked address fields with identical offsets and types
- Zero ordinary byte differences from the previous complete candidate,
  after both candidates' identical address fields were zeroed
- Current source hash and actual retained PE/MAP/PDB build path

This is source recovery inside the original **14,416-byte** target, still
**NONEXACT**. The comparison is against the preceding candidate image,
not a target-exactness claim; no new canonical match or partial-byte
credit. Original TU ownership, game runtime and product linking remain open.

Reproduce from the ignored local
.analysis/gpt-web/20261104-current/large-enemy-current-graph.py driver
with the immediate predecessor image selected as its baseline.
No full historical cold replay was executed.
