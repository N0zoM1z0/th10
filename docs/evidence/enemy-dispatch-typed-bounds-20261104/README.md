# Enemy ECL 14,416-byte boss: typed movement bounds and bullet offsets

Original target: Japanese TH10 v1.00a, verified SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The maintained `EnemyRuntimeView::DispatchEclInstruction` at original
`0x0040E770` has complete 14,416-byte target extent and is still **NONEXACT**.

The prior source repeatedly cast `runtimeAddress + 0x1344/0x1348/0x134c`
to float pointers across the bullet-pattern and laser-launch paths, and
used similar offset casts for visibility extent and movement-center/size
fields at `+0x13a4` through `+0x13b8`. The existing
`EnemyRuntimeView` type in `Enemy.hpp` already establishes these fields,
with native `offsetof` assertions for the bullet-pattern offsets and
movement geometry. This change replaces all **26** of those raw float
accesses with the corresponding typed runtime members. No secondary buffer,
new shadow field or reinterpretation of the float bits is introduced.

This builds on the separately committed typed flags and world-motion/
animation state passes. Its source improvement restores durable C++ data
ownership in the large ECL switch without inventing original C++ naming.

## Scoped native validation

The genuine 11-source Enemy update /GL /GS graph was linked with a fresh
compile of the modified dispatcher; current Player support was compiled
afresh because its source epoch differs from the historic baseline.
Other reused objects were guarded by current SHA-256 checks.

- Complete retained PDB contribution: **14,352 bytes** (unchanged).
- Candidate decoded address-field positions/types: **716**, unchanged.
- Candidate ordinary byte differences from the previous motion/animation
  source epoch, after masking candidate fields: **0**.
- Target 14,416-byte function exactness is **not** established; no
  `config/matches.csv` row or new exact-byte credit.

Reproducible ignored native link:
`build/gpt-web/20261104-live-enemy/bounds-bullet-typed/` and driver
`.analysis/gpt-web/20261104-live/large-enemy-typed-link.py`.
The [native-result.json](native-result.json) holds the current source hash
and comparison details. A whole executable, runtime proof and all-unit cold
replay were neither generated nor inferred.
