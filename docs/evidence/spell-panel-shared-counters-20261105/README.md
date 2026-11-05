# TH10 SpellPanel: read shared captures and attempts without raw dword offsets

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The retail `EnemyGameStateView::UpdateSpellPanel @ 0x00408D60` owns
**1,057 bytes**; the registered callback at `0x00409220` owns **10**.

The target reads two dwords from the same 0x90-byte spell-history record:
owner statistics `+0x624` (record+0x80, `captures`) and `+0x628`
(record+0x84, `attempts`). The newly shared
`GameStatisticsSpellHistoryEntryView` has exactly those fields, checked
by native `sizeof`/`offsetof` assertions. The maintained complete panel
now reads `history->captures` and `history->attempts` instead of
anonymous casts from absolute offsets. Crucially it still **resamples**
the stage/group selector between these two reads, matching the target's
real data-dependency/reload sequence, rather than caching one record.

After this actual `src/SpellPanel.cpp` edit, separately selected canonical
pinned VC7.1 /GL /GS replays show `enemy-spell-panel-update` **1057/1057**
exact with all target-bound original fields and local destinations, and
`enemy-spell-panel-update-callback` **10/10** exact. Both were exact
before this refactor, so **no new codegen credit** is added. Seven-stage
lightweight CI and whitespace verification passed. No complete historical
cold replay, source identity, product closure, or runtime claim.
See `native-result.json` for the installed source hashes and replay result.
