# Bullet SpawnSingleBullet: original i386 odd-pattern index +1 word

Japanese TH10 1.00a original SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The real `BulletManagerView::SpawnSingleBullet @0x004067D0` is a large
**1,413-byte** nonexact original owner. Its odd-spacing spawn-mode branch
executes **`LEA EAX,[ECX+1]` at target-relative `+0x152`** immediately
before the original signed rounded `/2` lowering (`CDQ`, `SUB`, `SAR`).
The old maintained expression `(index1 + 1) / 2` can trigger signed C++
overflow when given a large directly supplied input, though native i386
LEA preserves the low four bytes. The maintained C++ now computes the
same unsigned 32-bit word and converts back to signed `int` *before* the
existing signed division by two:

```cpp
static_cast<int>(static_cast<unsigned int>(index1) + 1u) / 2
```

The source retains all aim-mode cases, angle/speed operations, the actual
`index1`, `index2`, state flags, cursor, timer, ANM and sound consumers.
It does not add safety constraints, synthesize missing code, assert
original source spelling, or require a different compiler/ABI.

## Actual seven-TU boss and original-bound leaf evidence

A freshly rebuilt pinned VC7.1 SP1 `/GL /GS` seven-source Bullet manager
graph included the real BulletManager.cpp producer and six current-source
support TUs, with every header/source hash checked. Nine separate
natural C/C++ compiler trials independently and jointly tested this odd
index, the true 0x11 script color formula, the ANM script index addition,
and two native 0x7F0-byte bullet-pointer advances. **All nine** retained
the complete 1,504-byte `SpawnSingleBullet` PDB candidate with **44/44
unchanged candidate field offset/type/widths** and **zero ordinary
candidate byte differences** against the same-graph pinned baseline.
The original target is 1,413 bytes, so `SpawnSingleBullet` stays
**NONEXACT**; these 44 are candidate fields, not independently accepted
original link identities.

The same complete graph retained `SpawnBulletPattern 194/6 fields`,
`UpdateAbsoluteDirectionChange 335/8`, `UpdateHorizontalWrap 148/8` and
`UpdateVerticalWrap 172/8` as byte- and field-geometry-identical
candidates. The seven-TU SpawnBulletPattern field positions are **not**
the canonical source graph's original target field positions, so a
separate canonical graph is mandatory before claiming exactness.

After installing **only** the odd-index change, genuine selected original
cold replay in the correct two canonical graphs preserved:

| Previously exact owner | Complete original bytes | Real target fields |
| --- | ---: | ---: |
| `bullet-spawn-pattern-exact` | 194/194 | 6 |
| `bullet-update-horizontal-wrap` | 148/148 | 8 |
| `bullet-update-vertical-wrap` | 172/172 | 8 |
| `bullet-update-polar-acceleration` | 209/209 | 8 |
| `bullet-update-vector-acceleration` | 305/305 | 10 |

**1,028/1,028 original codegen bytes and all 40 declared target-bound
fields**. The final 7-TU image was compared over the **entire**
`SpawnSingleBullet` candidate, preserving 1,504 bytes, 44 candidate
field positions and zero ordinary differences. Separately, complete
original-bound `UpdateAbsoluteDirectionChange` stays **333/335** with
all eight true target fields; its EAX/EDX two-byte mismatch remains open.
No new canonical exact function/codegen bytes, complete production game
link or historical all-unit cold replay is claimed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-spawn-pattern-exact \
  --unit bullet-update-horizontal-wrap \
  --unit bullet-update-vertical-wrap \
  --unit bullet-update-polar-acceleration \
  --unit bullet-update-vector-acceleration --json
```

Installed `src/BulletManager.cpp` SHA-256:
`5b0cbe9673b702caecc687cbd2f4277e2f07b7db6da6330fde989ec0785fb689`.
The native source-epoch control variants, whole PDB owners, selected exact
cold and true target comparisons are retained in ignored
`.analysis/gpt-web/20261107-bullet-spawn-native/`,
`build/gpt-web/20261107-bullet-spawn-native/`,
`build/match-linked/{BulletHorizontalWrap,BulletRuntimeDeactivate}/`.
