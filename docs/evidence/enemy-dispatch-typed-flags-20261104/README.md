# TH10 enemy ECL dispatcher: recover native runtime flag storage

The original Japanese TH10 1.00a target remains the authority, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This changes the **actual** `src/EnemyEclDispatcher.cpp` body of the
large `EnemyRuntimeView::DispatchEclInstruction` at `0x0040E770`
(target extent **14,416 bytes**, still NONEXACT).

The prior decompiler-shaped implementation accessed the same bitfield through
26 repeated `*(unsigned int *)(runtimeAddress + 0x1444)` casts and two
byte-wide casts. These are the existing `EnemyRuntimeView::flags`, whose
offset `0x1444` is asserted in `src/Enemy.hpp`. All 26 integer accesses
now use the typed member; the two low-byte tests retain their actual byte-width
through `reinterpret_cast<const unsigned char *>(&flags)`. This covers
spawn placement, animation-VM state, polar movement, movement bounds, drops,
damage flags and ECL mode bits without adding a parallel shadow field.

## Actual native validation

The pinned MSVC 7.1 SP1 source graph contains the real Enemy owner and ten
support compilation units, with the existing `/GL /GS` split. The specific
dispatcher source was recompiled; the more recent Player.cpp was also
recompiled instead of reusing the stale Player object from the old receipt.
Other unchanged objects were reused **only** after SHA-256 source/object checks.
The target-independent baseline is the original 2026-11-03 complete native
graph, not a claim that an earlier analysis hypothesis was correct.

- Source contribution: **14,352/14,352 bytes** before and after.
- Decoded native address-field geometry: **716/716**, identical positions
  and field types.
- Location-normalized candidate ordinary differences **before vs after: 0**.
- The complete physical 14,416-byte target is **not** reproduced; no new
  canonical exact unit or partial-byte credit is claimed.
- The unchanged current Player source may differ from the earlier graph,
  so the native-result records the observed equality rather than inferring
  a new target-binding receipt.

Private reproducible link driver:
`.analysis/gpt-web/20261104-live/large-enemy-typed-link.py`;
retained actual PE/MAP/PDB:
`build/gpt-web/20261104-live-enemy/flags-typed/`.
`native-result.json` records the exact output, geometry and source pin.
This is substantive typed-source recovery and a code-generation-preserving
step toward the full boss, **not** a whole-game or runtime proof.
