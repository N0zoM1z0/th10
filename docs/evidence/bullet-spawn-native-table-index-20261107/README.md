# Bullet SpawnSingleBullet: native 32-bit indexed table reads

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The **complete** original `BulletManagerView::SpawnSingleBullet @0x004067D0`
is **1,413 bytes**. The maintained seven-TU `/GL /GS` native PDB
candidate is **1,504 bytes and 44 candidate address fields**, still
**NONEXACT**, not an accepted owner.

The bullet descriptor's signed `short` type and color select global
sprite ANM script bases, effect modes, effect scripts by color, draw
bucket indices and collision widths. Native i386 computes each indexed
address as a 32-bit base plus an index multiplied by the table's typed
element width; a negative/out-of-range script-controlled short still
forms the corresponding low address word, without a source bounds check.
Previous `table[descriptor->bulletType]` and `table[descriptor->color]`
C++ expressions performed out-of-array pointer arithmetic. The installed
natural typed `BulletNativeTableRead<T>` inline helper forms the native
unsigned address word first and then reads the corresponding `int` or
`float` element. All five existing read sites retain evaluation order,
original table storage, signed-short extension, result types, and actual
consumer calls. There are **no** added bounds checks, fake ABI declarations,
register-forcing assembly, or binary patches. The original code does not
prove out-of-range reads safe or portable on non-Win32 targets.

## Current-source full-owner native validation

A **fresh** selected 7-TU canonical `BulletHorizontalWrap` compiler graph
was rebuilt from current sources to avoid stale prior Anm/Enemy/Transform
support objects. An `all-table-helper` source control then recompiled
only the modified real BulletManager TU and linked with exactly the other
six unchanged native current-source objects. The final installed source
was independently cold-built under the same canonical seven-TU unit.
Complete PDB owner geometry and non-field ordinary code bytes compared
identically:

| Candidate whole PDB owner | Bytes | Candidate fields | Changed non-field bytes |
| --- | ---: | ---: | ---: |
| `SpawnSingleBullet` | 1,504 | 44 | 0 |
| `SpawnBulletPattern` | 194 | 6 | 0 |
| `UpdateAbsoluteDirectionChange` | 335 | 8 | 0 |
| `UpdateHorizontalWrap` | 148 | 8 | 0 |
| `UpdateVerticalWrap` | 172 | 8 | 0 |

The above field counts are source candidate geometry. The 1,504-byte
spawner is still longer than the original by 91 bytes and remains
NONEXACT. The 335-byte AbsoluteDirectionChange remains nonexact at
**333/335** when independently original-bound; this experiment does not
promote it.

A separate **selected original-target** replay cold-built only the five
related accepted Bullet owners, in their two real canonical build graphs:

| Previously exact owner | Original bytes |
| --- | ---: |
| `bullet-spawn-pattern-exact` | 194/194 |
| `bullet-update-horizontal-wrap` | 148/148 |
| `bullet-update-vertical-wrap` | 172/172 |
| `bullet-update-polar-acceleration` | 209/209 |
| `bullet-update-vector-acceleration` | 305/305 |
| **Total** | **1,028/1,028** |

All declared original call fields for the selected units pass. This is
codegen-preserving semantic reconstruction, **zero new canonical exact
credit**. No full historical cold replay or runnable game-product claim.

Private original/current source and native proof:
`.analysis/gpt-web/20261107-turn3/bullet-table-native.py`,
`bullet-table-installed-compare.json`, `bullet-table-installed-guards.json`,
`build/gpt-web/20261107-turn3-bullet-table/` PE/MAP/PDB, and
`build/match-linked/BulletHorizontalWrap/` installed-source canonical graph.
