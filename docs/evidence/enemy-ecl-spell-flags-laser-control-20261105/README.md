# TH10 ECL cancel-lasers flag and laser subtype scalar control, native storage

Original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ECL `CANCEL_LASERS` opcode modifies bit 3 of the game-state dword
at +0x378C. This is the same target-proven `EnemyGameStateView::flags378C`
used by the already exact spell-end path and spell-start boss. The maintained
complete 14KB dispatcher now names this member (`flags378C |= 8`) instead of
reconstructing `g_EnemyGameState + 0x378C` through anonymous integer casts.
A native `offsetof == 0x378C` assertion exists in the owning game-state
header. The ECL bit transition is unchanged.

`SET_LASER_SOUND` obtains a returned laser instance and stores a float at
receiver +0x440. This exact offset is observed in the original opcode
behavior, but does not by itself prove that all laser subtypes share a
semantic `sound` field at that address. The new **narrow**
`EnemyLaserControlFloatAt440View` therefore asserts only the 0x440 binary32
slot and the occupied 0x444 bytes, leaving its prefix completely opaque;
it does not extend the common 0x424-byte laser base or infer a derived class
or historical field name. The original `ReadFloatArgument(1)` evaluation,
null lookup guard, and store ordering are preserved.

A real eleven-source VC7.1 `/GL` link recompiles the complete ECL dispatcher
with ten unchanged source-hash-checked support objects. The entire
**14,352-byte** candidate against the **14,416-byte** original retains **716**
linked fields at the same offsets/types, with **zero** ordinary candidate
differences. The 2,432-byte `EnemyBeginSpell`, 162-byte death finalizer,
330-byte viewport configurator and 505-byte context-dependent drop core
likewise retain all their candidate field geometry and ordinary code bytes.
The ECL dispatcher remains **NONEXACT**; size and candidate stability do
not establish original target equality.

Selected registered original-target replays independently pass
`enemy-set-laser-position` **23/23**, `enemy-find-laser` **22/22**, and
`enemy-end-spell` **399/399**, preserving **444/444** previously accepted
bytes, with zero newly exact credit. Seven-stage lightweight CI and
whitespace checks pass. No full cold replay, product link, or runtime
validation was run. Current source hashes and comparisons are included in
`native-result.json`; ignored compiled graph:
`build/gpt-web/20261105-ecl-state-flags/full-ecl-boss/`.
