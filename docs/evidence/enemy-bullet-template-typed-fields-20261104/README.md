# TH10 14KB enemy ECL boss: reconstruct the full 0x210-byte bullet template fields

Original TH10 Japanese v1.00a (target SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).
The complete original `EnemyRuntimeView::DispatchEclInstruction` owner begins
at `0x0040E770`, contains **14,416** target bytes and **remains NONEXACT**.
This checkpoint modifies maintained C++ in that large owner and its
canonical `EnemyBulletPatternView` storage, **not just documentation**.

The target opcode-specific stores establish these offsets *relative to the
start of each 0x210-byte bullet-pattern record*:

| Native record offset | TH10-observed purpose |
| --- | --- |
| +0x00/+0x02 | sprite ID / sprite variant (`SET_BULLET_SPRITES`) |
| +0x10/+0x14 | angles A/B (`SET_BULLET_ANGLE`) |
| +0x18/+0x1C | speeds A/B (direct, difficulty, rank ops) |
| +0x1F4/+0x1F6 | two short bullet counts (direct and rank ops) |
| +0x1F8 | aiming mode |
| +0x1FC | template flags initialized by the original opcode |
| +0x200/+0x204 | two sound configuration words |

`src/Enemy.hpp` now models these in the original 0x210-byte
`EnemyBulletPatternFieldsView` union, using layout compile-time assertions
for every named offset and the full class size. It does not introduce a
second shadow structure or assert any unknown source-file provenance.

`src/EnemyEclDispatcher.cpp` now uses those real members for sprite,
aim-mode, angle, speed, count and sound writes instead of numeric `+0x2c4`,
`+0x2dc`, `+0x4b8` and similar offsets. These include **28 writes** in
rank/difficulty/direct branches in the later batch and the three sprite/aim
writes in the initial batch. Existing caller-scheduled operands and
`iVar26` pointer lifetimes are preserved; no target byte patches, inline
assembly or fake native register ABI were introduced.

## Genuine whole-owner native evidence

Two separate pinned VC7.1 SP1 /GL compilations within the existing
11-source Enemy update graph were performed on these two source epochs.
The changed dispatcher and more recent Player source were freshly compiled;
other unchanged support objects reused a verified SHA-256 pin.
Both produce **14,352 PDB-owned bytes** and exactly **716 linked address
fields**, all with unchanged offsets and field kinds. All remaining
ordinary bytes are **identical to the immediately previous candidate**
after candidate-field normalization. See [initial result](early-result.json)
and [final native result](native-result.json) for the actual produced
image paths, field checks and source SHA. This verifies no binary regression
from the previous candidate, **not** exactness against the 14,416-byte target.
No canonical match or partial exact credit is added for this boss.

Scoped replay of existing affected exact helper units and the regular seven
repository consistency checks should be used; **no full cold replay**.
Original historical TU/class names, product-link closure and runtime
correctness remain unresolved.
