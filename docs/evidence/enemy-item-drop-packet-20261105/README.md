# TH10 enemy ECL item-drop packet as a typed runtime member

The original Japanese TH10 1.00a enemy runtime contains a contiguous
0x3c-byte item-drop packet at runtime offset 0x13cc. The previously maintained
`EnemyItemDropCountRadiusView` already describes its main item type, 11
per-kind counts, one unknown cleared word, and the two ellipse radii at
+0x34/+0x38. This checkpoint makes that *same complete view* the canonical
`EnemyRuntimeView::itemDrops` member, instead of keeping five parallel raw
fields and repeatedly casting `&itemDropType` back into a packet.

This changes real maintained C++ in `Enemy.hpp`, `Enemy.cpp` and the complete
14,416-byte target ECL dispatcher represented by `EnemyEclDispatcher.cpp`.
Construction, spawn, death finalization, count clearing, count assignment,
radius-setting, item scattering and main-drop selection use one typed member.
Static assertions bind the full packet size and its nested count/radius offsets.
The packet word at relative +0x30 is still unknown; it is not assigned an
invented meaning.

## Bounded native codegen check

The **fresh** pinned VC7.1 SP1 11-translation-unit `/GL` source graph was
compiled with the existing enemy ECL graph recipe, not an all-unit replay.
Under the full PDB function extents, comparing against the saved immediately
preceding current-source graph after masking each *candidate's* linkage fields
produces **zero changed ordinary code bytes** and unchanged field geometry:

- Enemy ECL dispatcher: 14,352 candidate bytes, 716 fields, no changes.
- EnemyBeginSpell: 2,432 bytes, 108 fields, no changes.
- EnemyFinalizeDeath: 162 bytes, 9 fields, no changes.
- EnemyConfigureViewport: 330 bytes, 10 fields, no changes.
- EnemyFullObjectView constructor: 672 bytes, 5 fields, no changes.

Raw private proof: `build/gpt-web/20261105-next-boss/embedded-item-drop-packet/`
(`native-result.json`), with replay recipe
`.analysis/gpt-web/20261105-next-boss/verify-item-drop-packet.py`.
The original SHA-256 was checked by `scripts/verify-target.py`:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

**No new exact credit:** both 14,352-byte ECL dispatcher candidate and
672-byte constructor remain NONEXACT against the original 14,416-byte and
673-byte target owners. This comparison checks byte-neutral source recovery
in a real compiler/link graph, not original-byte acceptance or a runnable
whole-game claim.
