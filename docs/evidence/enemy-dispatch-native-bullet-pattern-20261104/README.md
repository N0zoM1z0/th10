# TH10 Enemy ECL 14KB boss: native bullet-pattern object copy

Original target TH10 Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This is an actual source improvement to the complete
`EnemyRuntimeView::DispatchEclInstruction` at target `0x0040E770`
(**14,416 target bytes**, still **NONEXACT**).

After establishing `bulletPatterns[index]` as the correct native array at
runtime +0x2C4, there was no reason to retain the separate
`EnemyBulletPatternCopyView` shadow struct (132 dwords, 0x210 bytes)
for the `ENEMY_ECL_COPY_BULLET_PATTERN` opcode. Both sides now use the real
`EnemyBulletPatternView` class directly:

```cpp
bulletPatterns[uVar23] = bulletPatterns[uVar22];
```

The original class already holds precisely 0x210 bytes, with the real
source-backed 132-word union and typed speed fields. Native value-copy
semantics remain a whole 0x210-byte copy. The formerly separate raw-word
copy wrapper and its redundant static size assertion were **removed**.
No padding, duplicate allocator, fake receiver ABI or target instruction
byte mutation was used.

## Whole-owner compiler check

The real 11-source VC7.1 SP1 `/GL /GS` Enemy update graph was linked,
compiling the changed `EnemyEclDispatcher.cpp` and newer Player source
from scratch; unchanged supports are reused only after original source
hash verification. The modified complete PDB dispatcher contribution is
**14,352 bytes**, exactly the same as its predecessor. All **716 decoded
linked address fields** have identical positions and types; candidate
ordinary bytes are **entirely identical** after normalizing those fields.
The deterministic full-owner source hash and comparison are recorded in
[native-result.json](native-result.json). The retained PE/MAP/PDB is
`build/gpt-web/20261104-live-enemy/pattern-native-struct-copy/`.

The comparator in this experiment is against the previous *candidate*,
not the original target bytes. No whole 14,416-byte exactness, original
C++ TU attribution, product closure, or runtime correctness is claimed.
This is independently useful typed-source recovery within the giant ECL
opcode interpreter, with no new canonical match or partial-byte credit.
