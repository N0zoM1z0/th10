# TH10 full ECL dispatcher — type-1 straight laser request fields

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The `FIRE_STRAIGHT_LASER` and `FIRE_STRAIGHT_LASER_2` opcode groups in the
complete **14,416-byte original ECL dispatcher** previously wrote 23
halfword, float and integer fields of the local laser request through
anonymous casts and numeric word indices. Their 0x1F8-byte scratch record
is already independently represented by `EnemyLaserType1RequestView` in
`EnemyLaser.hpp`. The maintained source now uses typed `angle`,
`maximumLength`, `initialLength`, `width`, `fixed2C`, `waitFrames`,
`growFrames`, `holdFrames`, `shrinkFrames`, `type`, `color` and `flags44`
fields in those two complete opcode groups. A native compile-time assertion
covers the eleven independently known type-1 offset positions. The
original 0x7E-word zeroing loop, optional 0x6C-word pattern transfer,
script-argument read order, flag-mask operations and `EnemyFireLaser`
call are unchanged. Unreviewed fields remain opaque.

The **complete eleven-source VC7.1 /GL graph** compiled the modified
`EnemyEclDispatcher.cpp` with ten source SHA-pinned unchanged support
objects. The entire ECL dispatcher remains **14,352 candidate bytes**
versus **14,416 original bytes**, all **716 candidate linked fields**
retain identical positions/types, and there are **zero ordinary candidate
to candidate byte differences** against the preceding laser type-0 source
epoch. Other full owners in that graph (`EnemyBeginSpell` 2,432 bytes/108
fields, `EnemyFinalizeDeath` 162/9, camera 330/10 and context-specific
drop core 505/27) likewise have unchanged complete candidate code.
These are candidate preservation observations, not proof that the 14KB
owner is original-target exact.

Selected canonical original-target replays of `enemy-set-laser-position`
**23/23**, `enemy-find-laser` **22/22** and `enemy-finalize-death`
**162/162** pass with the new source: **207/207 already credited bytes**.
There is no newly admitted exact function. Seven-stage lightweight CI
and whitespace checks pass. No complete historical cold replay, whole
product build or runtime claim. Native full link:
`build/gpt-web/20261105-laser-type1/full-ecl-boss/` (ignored).
Source pins and exact guards are in `native-result.json`.
