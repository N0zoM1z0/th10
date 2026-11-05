# TH10 complete spell-start and ECL owners: typed playback gate

The hash-pinned original Japanese TH10 v1.00a body `EnemyBeginSpell`
at `0x00409280` is **2,432 bytes**. Its original instructions at
`0x00409300..0x00409316` load the pointer at `0x00477838` and compare
its `+0x10` dword against one before updating spell statistics.
This independently establishes the actual offset, but *not* a descriptive
meaning for every playback state or the original class spelling.

`EnemySpellPlaybackGateView` now exposes that one `stateCode10` field
with a native `offsetof` assertion, preserving the external
`g_EnemySpellPlaybackState` pointer binding and `1` branch behavior.
The previous byte-pointer-plus-raw-int-cast in the real 2.4KB function
is replaced with a typed member read; all other playback fields remain
opaque. The retained record statistics order, script entry and 0x90-row
stride are unchanged.

A real 11-source VC7.1 `/GL` graph compiled the **complete** changed
`EnemyEclDispatcher.cpp` source, compared against the previously built
source epoch. `EnemyBeginSpell` remains 2,424 candidate bytes against
2,432 target, all **108** linked fields at the same locations/types,
and **zero** ordinary candidate differences. The companion complete
14,416-byte target `EnemyRuntimeView::DispatchEclInstruction` still
emits a 14,352-byte candidate, **716** fields in identical positions,
and **zero** changed ordinary candidate bytes. Neither large owner
is target-exact; no new exact credit is asserted.

Selected independent replays of `enemy-read-int-argument` (16 bytes)
and `enemy-configure-interrupt` (48 bytes) remain exact (64/64).
Seven-stage lightweight repository CI and diff verification passed;
no historical full cold replay, product closure or runtime proof.
Compiled evidence: ignored
`build/gpt-web/20261105-followup-ecl/spell-typed-playback-state/`
with detailed comparisons in `native-result.json`.
