# TH10 typed enemy world-position accessor leaf

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The six-byte leaf at `0x00408820` is a pointer adjustment:

```asm
add eax, 0x1068
ret
```

The maintained source now expresses the address as a typed field access:

```cpp
float *EnemyWorldPositionXPointer(EnemyFullObjectView *enemy)
{
    return &enemy->runtime.worldMotion.position.x;
}
```

The layout is supported by existing assertions in `Enemy.hpp`, not inferred
from the magic offset alone:

- `EnemyFullObjectView::runtime` is at `+0x103c`;
- `EnemyRuntimeView::worldMotion` is at `+0x02c`;
- `EnemyMotionView::position.x` is at offset zero within the position vector.

Together these establish the returned address at full-object `+0x1068`. Other
maintained TH10 paths independently read that same world-position X location.
The target leaf itself has no callers or xrefs, so this is a compatible typed
view and descriptive function name, not proof of the original class owner or
authorship.

## Focused exact replay

Command:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-decomphelp-fulltu-00408820 --json
```

Result: `exact`, 6/6 raw bytes, complete six-byte PDB contribution, zero
differences, and no linkage fields. No full cold replay, whole-image claim, or
runtime claim is made.
