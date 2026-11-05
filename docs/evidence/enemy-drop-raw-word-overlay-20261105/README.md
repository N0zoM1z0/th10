# Enemy ECL raw item-drop indexed words and typed 0x3C-byte packet

Target is original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The runtime `+0x13CC` drop packet is 0x3C bytes: an integer main type,
11 integer per-kind counts, one unexplained integer cleared by the ECL
clear opcode, and two binary32 radius words. `EnemyItemDropCountRadiusView`
already models these fields, but ECL `ADD_ITEM_DROP` still reconstructed its
indexed DWORD write through a cast of `&itemDrops.mainItemType` to `int*`.
The original opcode uses a computed index without clamping to a specific
member. The maintained `EnemyRuntimeView` now overlays the typed 0x3C-byte
packet with `int itemDropRawWords[15]` at the **same +0x13CC**, and the
14,416-byte target dispatcher uses `itemDropRawWords[iVar26]` for this one
indexed write. Type-preserving opcodes and lifecycle use `itemDrops`.
Compile-time checks bind both aliases to +0x13CC and their common 0x3C
size. This is an encoding/storage recovery, not a new array allocation or
an invented original source name. The target's out-of-range behavior is not
changed by adding an artificial bounds guard.

## Bounded native proof

A fresh pinned VC7.1 SP1 11-input `/GL` Enemy graph with the modified
`Enemy.hpp` and `EnemyEclDispatcher.cpp` was compiled and compared against
the previous original-source candidate graph. The complete **14,352-byte**
ECL dispatcher candidate retains **716 same-offset/type linkage fields**
and **zero normalized ordinary-byte differences**. BeginSpell (2,432 bytes,
108 fields), EnemyFinalizeDeath (162, 9), EnemyConfigureViewport (330, 10),
and complete EnemyFullObjectView constructor (672, 5) likewise retain their
full candidates and link geometries. This is a *candidate regression* proof,
not full original-byte equivalence; the original dispatcher is **14,416**
bytes and remains **NONEXACT**.

Separately, explicitly selected original-target exact replays passed
`enemy-drop-item-counts` (48), `enemy-drop-item-counts-core` (503) and
`enemy-runtime-constructor` (145): **696/696 exact**. No historical all-unit
cold replay. The seven-stage lightweight CI and whitespace gate pass.

Ignored reproducibility inputs:
`.analysis/gpt-web/20261105-enemy-drop-word/verify.py`,
`build/gpt-web/20261105-enemy-drop-word/current/native-result.json`, and
`.analysis/gpt-web/20261105-enemy-drop-word/selected-exact.json`.
No original code bytes were copied into source or patched, and no product,
semantic-runtime or portability claim follows.
