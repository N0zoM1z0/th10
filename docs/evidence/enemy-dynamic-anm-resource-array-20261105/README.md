# TH10 complete ECL boss: typed dynamic ANM resource slots and death effects

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The TH10 enemy manager is a 0x68-byte object with four resource-pointer slots
beginning at +0x30. Slot 2 at +0x38 is independently reviewed as the stage
ANM resource. Actual ECL opcodes use a dynamically chosen resource index for
ANM VM creation, and the death finalizer uses another dynamic index to
create an ANM effect. Before this batch both consumers cast each `void *`
resource pointer to `AnmLoadedView *`, even though those storage slots have
known four-byte pointer layout.

The existing raw `effectResources[4]` array remains in the manager, with an
overlapping typed `AnmLoadedView *loadedAnmResources[4]` view. Native MSVC7.1
compile-time assertions verify its start at +0x30, slot 2 via `2 * sizeof`
(the older compiler cannot parse `offsetof` subscript expressions) at +0x38,
and the timer following at +0x40. Seven complete ECL ANM opcodes now use the
typed dynamic resource array, as does the independently exact death-effect
creator in `Enemy.cpp`. Package loader writers retain the original raw array;
no new bounds checks, owner class identity or changed pointer lifetime is
asserted.

The **entire eleven-source VC7.1 /GL graph was freshly recompiled**, rather
than reusing stale objects after the public ANM header and Enemy header
changed. Complete candidate comparisons against the preceding source epoch
show zero ordinary byte differences and identical linked-field geometry for:
`EnemyBeginSpell` 2,432 bytes / 108 fields; full
`EnemyRuntimeView::DispatchEclInstruction` 14,352 bytes / 716 fields;
`EnemyFinalizeDeath` 162 bytes / 9 fields; enemy constructor 672 bytes /
5 fields; camera configurator 330 bytes / 10 fields; and the context-specific
505-byte drop core / 27 fields. The *original* ECL dispatcher is 14,416 bytes;
these are candidate-preservation results, not new target exactness.

Affected independently registered original-target exact replays pass:
`enemy-manager-constructor` **29/29**, `enemy-ecl-resource-load-package`
**239/239**, and `enemy-finalize-death` **162/162** — **430/430 already exact
bytes**. Lightweight seven-stage CI and whitespace checks passed. This did
not run a full historical cold replay, product linker or runtime acceptance.
Full linked artifact: ignored local
`build/gpt-web/20261105-dynamic-anm/complete-ecl-resource-array/`;
current source hashes and results in `native-result.json`.
