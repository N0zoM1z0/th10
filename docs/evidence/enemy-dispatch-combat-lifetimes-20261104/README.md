# Enemy ECL 14,416-byte dispatcher: life, drops, collision and owner fields

The target is attested original Japanese TH10 v1.00a (SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).
This is real C++ source recovery in the complete `EnemyRuntimeView::DispatchEclInstruction`
at `0x0040E770`, **not** a claim that the full 14,416-byte target is exact.

## Recovered owner members

The existing `EnemyRuntimeView` in `Enemy.hpp` defines native-mapped
`damageHitbox` and `playerCollisionHitbox` float2s (+0xB0/+0xB8), the
current life (+0x13C0), gauge maximum (+0x13C4), previous/mirrored life
(+0x13C8), item drop type/counts (+0x13CC/+0x13D0), area floats
(+0x1400/+0x1404), animation script mode (+0x1448/+0x144C), and the
full-enemy owner pointer (+0x14D8). The source previously read or wrote
these by integer-cast runtime-offset expressions. **23 direct owner field
accesses** were replaced by the already established corresponding types.
The item-type-to-drop-index calculation still uses its original relative
index, now from the real itemDropType member rather than a new shadow array.
The host pointer is still explicitly converted to an integer only where
the existing code's raw arithmetic requires it.

The `ENEMY_ECL_SET_BULLET_PROTECTION_RANGE` and corresponding firing tests
actually store/load a *float* at +0x1454 even though the retained layout
names its backing dword `unknown1454`. The new source expresses that
observable float view through `&unknown1454`, without misrepresenting its
underlying storage layout, changing the structure, or casting the entire
runtime owner to a made-up address. These three native float reads/writes
remain semantically identical.

## Whole-owner native result

Current pinned VC7.1 SP1 real Enemy update graph uses 11 source inputs,
with the modified `EnemyEclDispatcher.cpp` freshly compiled. One changed
Player input was also freshly compiled rather than borrowing a stale
source-object pin, and other unchanged support objects were SHA-verified.
The original target is not used as a source of code bytes.

The 14,352-byte real PDB dispatcher contribution and **716 decoded native
address fields** are the same as the preceding typed bounds/offsets
candidate. All 716 candidate field locations and types are unchanged;
all **14,352 preliminary ordinary positions compare identical** to the
previous candidate after normalization. This result does *not* assert that
the candidate equals the 14,416-byte target: that large function remains
NONEXACT, with no canonical match or partial-byte credit.

[Native result](native-result.json) records current source SHA and geometry.
Reproduction uses the scoped ignored
`.analysis/gpt-web/20261104-live/large-enemy-typed-link.py` driver,
output `build/gpt-web/20261104-live-enemy/combat-lifetime-typed/`,
with the preceding bounds source epoch selected as baseline. Existing exact
helpers should be cold-replayed only with explicit affected `--unit` selectors;
full-product, runtime and portability gates are still open.
