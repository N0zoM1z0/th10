# TH10 complete 14,416-byte ECL dispatcher — boss slot and player-position storage

Original: Japanese v1.00a TH10, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Target owner: `EnemyRuntimeView::DispatchEclInstruction`, `0x0040E770` (14,416 bytes). **NONEXACT**.

The maintained 14KB opcode interpreter formerly treated the manager's first
special-enemy slot at `EnemyManagerView + 0x10` as an anonymous 32-bit word in
seven branch guards and two movement-copy sites. The target-reviewed manager
layout shows this is `specialEnemySlots[0]`, a pointer to the full enemy object;
its presence gates conditional enemy creation and the wait-for-boss-clear
instruction. The new source accesses the real member and retains the cached
object pointer where the native movement-copy path needs it.

The random movement chooser, bullet spawn protection distance and player-aim
instruction read `Player + 0x3C0/+0x3C4`. The reviewed `Player` layout establishes
these as `drawPosition.x/y`. The converted player-aim code deliberately reads
`drawPosition.y` through the already cached `iVar26` player receiver, **not**
through a fresh global `g_Player` dereference after an ECL argument call. The
latter natural-looking change was tested and caused a different 716-field
linkage layout and ordinary codegen; it was rejected, not promoted.

A full **11-source, original VC7.1 /GL linked** graph was compiled for the
new 14KB owner. For a controlled source A/B comparison, the pre-edit dispatcher
was compiled separately from `HEAD` with all other support objects reused only
after verifying source/header hashes from the same freshly generated graph.
Both whole owner contributions are **14,352 bytes** (not 14,352 matching target
bytes). Their **716** linked fields have identical offsets/types; after masking
only those fields, **zero ordinary bytes differ**. The original target is
**14,416 bytes**: no new exactness credit, no closure/runtime claim.

Scoped real native replays of `enemy-runtime-constructor`,
`enemy-read-int-argument`, and `enemy-read-raw-float-argument` are exact; the
ordinary seven-stage repository CI passed. This is structural source recovery
with verified candidate preservation, not a historical source-name claim.
See `native-result.json` and the ignored current-session graph in
`build/gpt-web/20261105-ecl/typed-slots-cached-player-view/`.
