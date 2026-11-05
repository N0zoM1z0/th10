# TH10 bullet absolute-direction: preserve bitwise angle semantics with memcpy

Original is the pinned Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete 335-byte `BulletRuntimeView::UpdateAbsoluteDirectionChange`
owner is at `0x004078E0`, and it remains **NONEXACT**.

The absolute-direction transform stores the next angle as an IEEE-754
32-bit payload in `BulletExStateView::value1`, with `angle` also holding
those exact bits. The maintained source now copies the 32-bit payload
through `memcpy` into the integer temporary and thence into the angle
field instead of dereferencing an incompatible `int *` pointer to floats.
It preserves the original bit representation without a floating-point
conversion and does not replace the target's 32-bit stored value with a
rounded arithmetic result. The relevant native layout is independently
checked in `BulletManager.hpp`.

A real VC7.1 `/GL /GS` **seven-source** build of `BulletManager.cpp`
plus BulletRuntime, BulletTransform, PlayerCollision, AnmManager,
EnemyEclDispatcher and Enemy produces the complete **335-byte** owner.
The target comparison has exactly **eight** linked fields and two
ordinary mismatches at `+0xD7` and `+0xDD`; these are the compiler's
register choice for the forwarded angle, **not** an accepted exact match.
The source candidate before and after the bit-copy edit has identical
335-byte size, eight identical field positions/types and **zero ordinary
candidate-to-candidate differences**, masking only the decoded fields.

A focused canonical replay of `bullet-update-horizontal-wrap` (148 bytes)
and `bullet-update-deceleration` (164 bytes) passes **312/312** bytes
exact from installed source. Lightweight seven-stage CI and diff checks
pass; no full cold replay, product closure or runtime validation is claimed.
Full ignored artifacts are in `build/gpt-web/20261105-continuation-bullet/`.
