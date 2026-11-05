# Embedded EnemyRuntime item-drop packet: shared ECL and lifecycle storage

Original target: Japanese TH10 v1.00a; SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The physical runtime range +0x13CC..+0x1407 is one contiguous 0x3C-byte
item-drop packet. The maintained `EnemyItemDropCountRadiusView` already
models its observed fields: primary item kind at +0, eleven 32-bit item
counts at +4, reserved word +0x30, and binary32 X/Y radii at +0x34/+0x38.
Previously `EnemyRuntimeView` maintained five parallel members while the
constructor, scatter helper and ECL opcode cases cast among them. This
change embeds `EnemyItemDropCountRadiusView itemDrops` as a real member of
the runtime with compile-time checks for the complete 0x3C-byte physical
extent and all established subfield positions.

The original constructor's initialization, EnemySpawn, death finalizer,
ECL `CLEAR_ITEM_DROPS`, `ADD_ITEM_DROP`, `SET_DROP_AREA`, `DROP_ITEMS` and
`SET_MAIN_DROP` now access the shared member. The indexed ECL item-count
write retains its original word indexing and the original non-clamping
semantics. `CLEAR_ITEM_DROPS` continues to clear 48 bytes of counts plus
reserved word, leaving the ellipse radii intact.

A **fresh 11-TU** pinned VC7.1 /GL native link compiled the changed
original source and retained the complete 14,352-byte candidate ECL
dispatcher with 716 candidate linkage fields at identical locations/types
and zero ordinary differences relative to the immediately preceding source
context. Related BeginSpell (2432/108 fields), death-finalizer (162/9),
viewport (330/10) and full constructor (672/5) likewise have zero
candidate-to-candidate ordinary differences. Reproducible native graph:
`build/gpt-web/20261105-next-boss/embedded-item-drop-packet/`.

Selected independently **original-target-bound** cold exact replays passed
`enemy-drop-item-counts` 48/48, `enemy-drop-item-counts-core` 503/503 and
`enemy-runtime-constructor` 145/145: **696/696 total**, in two native
artifacts. The seven-stage lightweight CI and whitespace gate also passed.

The original ECL dispatcher is 14,416 bytes, so the 64-byte gap remains;
full original byte agreement and exact new credit are explicitly **not**
claimed. Neither a whole rebuilt game nor runtime/portability proof follows.
