# TH10 Enemy ECL 14KB boss: typed laser control through the real base class

Original target: Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
Original EnemyRuntimeView::DispatchEclInstruction starts at 0x0040E770
and owns 14,416 target bytes. This is an actual maintained C++ source
improvement to that function, which remains **NONEXACT**.

The original ECL SET_LASER_OFFSET, SET_LASER_ANGLE, SET_LASER_COUNT and
SET_LASER_SPEED commands each look up the same live laser object by its
native ID and write to that object's storage at +0x24, +0x3C, +0x44
and +0x48. The independent type-0 collision owner confirms the same
position/angle region. The existing shared EnemyLaserConstructionBaseView
previously represented +0x24..+0x4F as an entirely opaque byte array.
It now expresses:

- +0x24 EnemyLaserVectorView eclPosition: full three-float position copy
- +0x3C float eclAngle
- +0x44 float eclValue44
- +0x48 float eclValue48

The unnamed gaps stay opaque; +0x50 removalPhase and +0x54 id retain their
physical offsets. Each newly named field has compile-time offsetof checks.
These names represent observed opcode roles only, not original C++ names
or an invented duplicate laser object. The four instructions now use the
real shared base view rather than byte-offset lvalue casts, while preserving
the existing virtual table, timer and linked-list semantics.

## Complete owner VC7.1 compilation and selected guards

Because this changes the shared EnemyLaser.hpp, the first verification
physically **recompiled all 11 actual source inputs** of the selected Enemy
runtime update /GL graph under VC7.1 SP1; no previously built object was
reused across that header change. The entire candidate ECL dispatcher
remained **14,348 PDB-owned bytes**, with **716** original-shaped decoded
address fields, their identical offsets and types and **zero ordinary
byte differences** from the preceding candidate after its candidate
fields were normalized. The exact source and header SHA-256 pins and the
retained actual PE/MAP/PDB are in full-source-native.json.

A second source change typed the 12-byte position-vector assignment.
With no additional shared-header edit, the bounded follow-up image used
SHA-checked unchanged support objects and recompiled the changed
dispatcher. Its complete owner remained 14,348 bytes, 716 fields,
identical field geometry and **zero ordinary candidate byte differences**
(position-native.json). The installed source SHA-256 is
ea75f9779b978ad3a26eacfd427efc7d7c0d6cf2087cd141ac4bff0abf915cca.

The selected affected exact function guards were separately cold-replayed
from current source: type-0 laser constructor 202, type-1 laser constructor
219, laser draw-list 43, ECL ReadIntArgument 16: **480/480 bytes exact**.
All standard seven repository consistency checks passed. No all-unit
cold replay was run.

The 14,348-byte compiled candidate is not the original 14,416-byte
dispatcher. This change adds **zero canonical exact credit** and does not
claim the original TU/class spelling, complete Windows product, runtime
behavior or portability.
