# TH10 typed bullet cancel sprite and effect VM color storage

Original target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained **complete 418-byte candidate** `EnemyCancelManagerView::CancelAllBullets`
scans 2,000 0x7F0-byte bullet records. Its color-selection path formerly used
raw `bullet + 0x39C`, `sprite + 0x34`, and `vm + 0x2FC` expressions. The real
`BulletRuntimeView::vm` begins at +0x08; `AnmVmView::loadedSprite` at VM +0x394
is therefore bullet +0x39C. `AnmSpriteView::width` is at +0x34, and
`AnmVmView::primaryColor` is at VM +0x2FC. All three locations now use the
existing typed members, with a native `offsetof` assertion proving the
combined offsets. The sprite pointer null check, width-threshold comparisons,
color lookup order, and effect VM publication remain unchanged.

A real pinned **eleven-source VC7.1 `/GL` graph** compiled the modified source
and ten hash-verified unchanged support objects. The entire cancel-loop
candidate is still **418 bytes**, with 12 unchanged linked fields and **zero
ordinary candidate-to-candidate byte changes**. The same graph preserves the
complete `EnemyBeginSpell` **2,432-byte candidate** (2,432-byte original), its
108 fields, and the **14,352-byte ECL dispatch candidate** (14,416-byte
original), its 716 fields; both have zero ordinary byte changes against their
own previous candidate. The full original-bound proof is **not** established
for these three owners, so none is credited exact.

Affected already-accepted canonical local replays independently pass
`enemy-cancel-bullet-record` **199/199** and
`enemy-cancel-bullet-pattern` **260/260**. These 459 previously exact bytes
remain accepted. Seven-stage lightweight repository CI and diff checks passed;
no full cold replay, product closure, or runtime correctness is claimed.

Reproduction: ignored local build
`build/gpt-web/20261105-resume/typed-cancel-sprite-owner/` and
`native-result.json` source pins.
