# TH10 1,057-byte SpellPanel: reuse real statistics-record history accessor

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained `EnemyGameStateView::UpdateSpellPanel` retrieves both
counters via `GameStatisticsOwner::records[group].SpellHistoryAt(spellId)`.
This replaces two separately handwritten global pointer + 0x437C group
stride + 0x90 spell stride + 0x5A4 offsets. Each read still resamples the
stage/group selector **and** the global statistics pointer separately,
matching the target-observed reloads on either side of the intervening GUI
calls. The shared `GameStatisticsSpellHistoryEntryView` distinguishes
successful captures at +0x80 from attempts at +0x84. The original game
source identity and unchecked script index validity remain open.

The actual pinned VC7.1 /GL /GS canonical selected cold replay of the
**complete 1,057-byte** original UpdateSpellPanel and its **10-byte**
registered fastcall adapter passes both `enemy-spell-panel-update`
**1057/1057** and `enemy-spell-panel-update-callback` **10/10**, including
all declared original linkages and internal destinations. These are
already accepted exact functions, so no new exact credit is added.

Seven-stage lightweight CI and git diff validation pass; no full historical
cohort, product build or runtime claim. Source pins and replay summary are
in `native-result.json`.
