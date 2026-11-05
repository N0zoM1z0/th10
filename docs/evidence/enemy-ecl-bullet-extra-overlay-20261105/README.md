# TH10 ECL dispatcher: recover the bullet-extra overlapping record view

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

In the **complete** 14,416-byte original `EnemyRuntimeView::DispatchEclInstruction`
owner, opcode `0x199` writes six fields of a 24-byte bullet extra record.
The maintained implementation previously addressed the first five stores at
`runtime + 0x2e4 + 0x18*(22*pattern+slot)`, but the final float was an
unrelated-looking `runtime + 8*(3*(22*pattern+slot)+0x5d)` expression.
Both addresses refer to the **same extra record**, with the final float at
`+0x04`. The base is now derived from the real `EnemyRuntimeView::bulletPatterns`
and `EnemyBulletPatternFieldsView::unknown020`, and compile-time checks bind
`sizeof(record)==0x18`, the six offsets, the runtime `+0x2e4` base and
**22 records per 0x210-byte pattern stride**.

This is an **overlapping view**, not a new `EnemyRuntimeView` member or an
independent 22-element array within each bullet pattern. The last two records
straddle the normal pattern tail and next pattern's header. We deliberately
retain the native unsigned/signless ECL operand interpretation and unguarded
index behavior; no invented bounds or original source names are claimed.

A fresh pinned VC7.1 SP1 eleven-TU native graph recompiled the modified full
ECL dispatcher, checked all ten reused support translation-unit source hashes,
and linked a complete owner. Keeping the sixth write's native 8-byte-unit
index form produces **14,352/14,352 candidate bytes identical to the previous
candidate**, with the complete **716/716 linked fields at identical
positions/types**. `EnemyBeginSpell` also remains 2,432 candidate bytes and
108 unchanged fields. A direct pointer-cursor rewrite of that sixth store was
actually compiled and **rejected**: the function remained the same length but
moved 223 downstream linked-field offsets. The selected form is the source
actually installed, not merely a hypothetical rewrite.

Selected *original-bound* cold replays of `enemy-ecl-resource-load-package`,
`enemy-read-int-argument` and `enemy-read-raw-float-argument` pass without
changes. Seven-stage CI and `git diff --check` passed. This is **semantic and
layout progress**, not new exactness: the target dispatcher remains 14,416
original bytes versus 14,352 candidate; its independently bound original
fields remain unresolved. No full historical cohort replay was run.

Machine-readable narrow results: [native-result.json](native-result.json).
Private native witnesses and compiler logs live only in ignored
`build/gpt-web/20261105-typed-extra/` and `.analysis/gpt-web/20261105/`.
