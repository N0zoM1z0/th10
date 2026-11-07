# Bullet SpawnSingleBullet: original 0x7F0-byte pool cursor address words

Original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `BulletManagerView::SpawnSingleBullet @0x004067D0` is a
complete **1,413-byte** large function. It scans the real 2001-record
`BulletRuntimeView bullets[]` pool (including its sentinel slot),
checking each record's `state` and advancing the current cursor with
native **`ADD EBP,0x7F0`** in the actual original at offsets `+0x2D`,
`+0x4D`, `+0x68`, `+0x83`, `+0x9E`, and again at `+0x54E` after an
already spawned bullet. The true record type is asserted to be
`sizeof(BulletRuntimeView)==0x7F0` in the existing header.

Previously the maintained C++ used `++bullet` at its scan and final
publication sites. That expresses C++ element-pointer arithmetic, which
has undefined behavior once an unchecked cursor exceeds the underlying
pool object, whereas retail i386 always computes a low 32-bit address
word even across the sentinel. The two natural source operations now
explicitly form the same **unsigned 32-bit native address addition**:

```cpp
bullet = reinterpret_cast<BulletRuntimeView *>(
    reinterpret_cast<unsigned int>(bullet) +
    static_cast<unsigned int>(sizeof(BulletRuntimeView)));
```

The original pool size, sentinel test, input iteration cap, ring reset to
`&bullets[0]`, active cursor publication, bullet allocation, ANM setup,
transform programming and all other arithmetic are unchanged. The prior
native low-word odd `index1+1` before signed quotient remains installed.
This reconstructs target i386 arithmetic without asserting original C++
spelling, portable pointer validity, or safe malformed-index runtime.

## Actual current-context whole-owner compiler evidence

A fresh VC7.1 SP1 `/GL /GS` seven-TU Bullet graph was built using the
real BulletManager.cpp and six source-hash-pinned current supports.
Nine separate natural source variants were compiled, including odd index,
color effect script index, ANM script addition, two separate cursor
advances, and multiple combinations. All nine emitted the **same complete
1,504-byte PDB-owned** `SpawnSingleBullet` candidate as the true
current-source baseline, with **44 identical candidate linkage
field offset/type/width positions** and **zero ordinary candidate
code differences**. Four adjacent source functions also retained
unchanged candidate code and field geometry, including the 194-byte
SpawnBulletPattern. This candidate match is **not** original-target
exactness: the original SpawnSingleBullet is 1,413 bytes and the 91-byte
gap remains, so the large owner stays **NONEXACT**.

The final two-pointer source was installed on top of the separately
committed odd-index change. An independently selected **current-source
canonical cold replay** in the two real supported build graphs preserved:

| Preaccepted exact unit | Complete original bytes | Target fields |
| --- | ---: | ---: |
| `bullet-spawn-pattern-exact` | 194/194 | 6 |
| `bullet-update-horizontal-wrap` | 148/148 | 8 |
| `bullet-update-vertical-wrap` | 172/172 | 8 |
| `bullet-update-polar-acceleration` | 209/209 | 8 |
| `bullet-update-vector-acceleration` | 305/305 | 10 |

**1,028/1,028 original bytes**, all **40 true target-bound fields**.
The full final-source SpawnSingle PDB contribution was independently
checked against the pinned 7-TU baseline and remains **1,504 candidate
bytes, 44 candidate fields, zero ordinary differences**. The adjacent
absolute-direction update remains **333/335 original bytes** and all
8 bound fields, explicitly NONEXACT. Field geometries from the 7-TU
SpawnBulletPattern must not be substituted for canonical four-TU field
positions; the exact 194-byte result above is from its real unit graph.
No new canonical codegen owner/bytes, all-history cold or whole-game
executable result is claimed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-spawn-pattern-exact \
  --unit bullet-update-horizontal-wrap \
  --unit bullet-update-vertical-wrap \
  --unit bullet-update-polar-acceleration \
  --unit bullet-update-vector-acceleration --json
```

Final `src/BulletManager.cpp` SHA-256:
`9d4aaf9c5b590bd0e9f5be597a936d49050621d294a67f0a4c7fb3406f8345c6`.
Actual native source trials and final-source selected target receipts:
`.analysis/gpt-web/20261107-bullet-spawn-native/`,
`build/gpt-web/20261107-bullet-spawn-native/`, and
`build/match-linked/{BulletHorizontalWrap,BulletRuntimeDeactivate}/`.
