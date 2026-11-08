# EnemyRuntimeUpdate aggregate vector temporary (2026-11-08)

Target `EnemyRuntimeUpdate @ 0x0040DC80` is 2,412 bytes. This experiment
reconstructs the world-velocity sum with an ordinary aggregate temporary:

```cpp
struct EnemyFloat3SumView { float x; float y; float z; };

const volatile float &baseX = baseMotion->position.x;
const volatile float &baseY = baseMotion->position.y;
const volatile float &baseZ = baseMotion->position.z;
const AnmFloat3View &offsetPosition = /* offset-motion position view */;
const AnmFloat3View &worldPosition = /* world-motion position view */;
const EnemyFloat3SumView sum = {
    baseX + offsetPosition.x,
    baseY + offsetPosition.y,
    baseZ + offsetPosition.z};
*reinterpret_cast<AnmFloat3View *>(&enemy->worldMotion.velocity) =
    sum - worldPosition;
```

The aggregate initializer is materially better than the previous constructor
hypothesis. It preserves the target's X/Y/Z x87 `fld`/`fadd` order and moves
the Z-sum spill from candidate `ESP+0x2C` to the target's `ESP+0x24`, without
changing the 2,412-byte function extent or the 0x18-byte local frame. The
scoped structural comparison rises from 1,580/2,136 to **1,582/2,136**
comparable bytes, with 69 linkage fields. The vector block at `+0x246..+0x28E`
now matches the target instruction sequence and stack slot; the call relocation
is graph-specific.

This remains NONEXACT. The first remaining comparable-byte differences are
at `+0x49D` and continue through the 32-byte comparison tail. Equal function
size and the local vector-block match are not full-function proof. No runtime,
portable-runtime, or whole-image claim follows.

## Reproduction

The targeted verification recipe is
`.analysis/gpt-web/20261108-current/enemy-runtime-aggregate-sum-verify.py`.
It compiles only `src/Enemy.cpp` with the configured `enemy-apply-bullet-clear`
profile and links it with the retained 11 support LTCG objects; it does not run
a full cold replay. Candidate PE SHA-256 is recorded in `results.json`.
