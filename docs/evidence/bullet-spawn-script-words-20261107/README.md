# Bullet SpawnSingleBullet: native ANM and effect-script index arithmetic

Target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `BulletManagerView::SpawnSingleBullet @0x004067D0` (1,413
bytes) computes the ANM VM script from a 32-bit table lookup plus the
signed color at original `+0x3CB..+0x3D9`, and uses
`LEA ECX,[EAX+EAX+0x11]` at `+0x406` for effect script mode zero.
Original x86 retains the low 32 bits; C++ signed arithmetic on these
unchecked inputs can overflow. The installed source explicitly performs
those two additions/products as `unsigned int` words, then publishes the
same signed `int` result without a clamp, new script, altered array layout,
or changed call order. It retains previously committed native odd-index and
0x7F0-byte pool cursor arithmetic.

The original *whole boss* is NONEXACT: pinned current seven-input VC7.1
`/GL /GS` PDB candidate remains **1,504 bytes with 44 candidate fields**,
compared with original **1,413**. Nine independent C++ variants (the two
script arithmetic sites, odd spawn index, both pool advances, and their
combinations) compiled and linked with exactly zero changes in full
ordinary candidate bytes and identical offset/type/width geometry.
This is same-graph candidate preservation, **not** original-bound exactness
for the large owner.

After installing both script expressions on the current source, a fresh
selected original-bound cold replay of the **actual canonical** affected
Bullet graphs passed five preaccepted units:

| Unit | Original bytes | Original fields |
| --- | ---: | ---: |
| `bullet-spawn-pattern-exact` | 194/194 | 6 |
| `bullet-update-horizontal-wrap` | 148/148 | 8 |
| `bullet-update-vertical-wrap` | 172/172 | 8 |
| `bullet-update-polar-acceleration` | 209/209 | 8 |
| `bullet-update-vector-acceleration` | 305/305 | 10 |

Total **1,028/1,028**, all **40 target-bound fields** in two canonical
compiler graphs. The complete current seven-input `SpawnSingleBullet`
remains byte/field-geometry-identical to its current-source pinned
baseline (1,504/44 candidate). Independently target-bound
`UpdateAbsoluteDirectionChange` still matches **333/335** with eight real
fields and is NONEXACT. No new canonical exact codegen credit, production
runtime/game-EXE guarantee, or full historical cold replay.

Reproduce scoped guards:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-spawn-pattern-exact \
  --unit bullet-update-horizontal-wrap \
  --unit bullet-update-vertical-wrap \
  --unit bullet-update-polar-acceleration \
  --unit bullet-update-vector-acceleration --json
```

Installed `src/BulletManager.cpp` SHA-256:
`5edcaf7151ebcacce077d8116d7e2adf1e33ef2dc1389ab4950be2253b81ec0b`.
Actual trial scripts and compact source-pinned comparison receipts:
`.analysis/gpt-web/20261107-bullet-spawn-native/`.
Transient PE/MAP/PDB control artifacts are reproducible from that driver;
check whether they have been pruned before using any scratch path.
