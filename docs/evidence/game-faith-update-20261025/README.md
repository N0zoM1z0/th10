# Game faith update: canonical replay, 2026-10-25

## Complete owner and ordering

GameUpdateFaithDecay at **0x418A00..0x418A40** is the existing reviewed authored
65-byte owner. The applied natural source uses the established GameScoreState
view and same-layout timer boundary, without Stage/score diagnostic integration.

- If timer.current is positive, invoke the real timer Add(-1.0f), then return.
  Even if this reaches zero, faith decay does not occur in that call.
- Otherwise, only when faith exceeds 5000, subtract the **old unknown10** value
  first, then raise unknown10 to a minimum of 18, then floor faith at 5000.
  Raising the rate before subtraction would change the observed operation order.

This establishes bounded target behavior, not the original class spelling,
object lifetime, canonical shared-storage layout or runtime closure.

## Fresh canonical evidence

The real OnUpdate graph is **GameManager.cpp, ScriptViewerWorker.cpp,
AnmManager.cpp**, in that order, all **/GL without /GS**, with the standard
non-runnable diagnostic anchor harness. Fresh canonical cold build exits 0;
[canonical comparison](canonical-comparison.json) matches **65 bytes / all 1
field**, unit game-update-faith-decay. The prior ignored diagnostic cold build
independently used identical candidate source and also passed.

Independent canonical review replays Faith plus six existing guards, **453
bytes / all 14 fields**: delay 92, callback 7, mark-chains 28, Tick 72, Stop
101 and real timer Add 88. All 68 preserved original artifacts remain unchanged.
The original GameStageTransition recipe separately cold-passes its three guards
127/all 2, and original OnDraw cold-passes 31/all 1: four existing units,
158 bytes / all 3 fields. Regression inputs are preserved; these are not
additional new owners.

- Applied GameManager.cpp SHA-256:
  `2edb5631c3c6bdb3b960cb2110213cc55aa167e0c51f7a59570ed237c377c543`
- Fresh canonical image:
  `fc2e7a967aa0b0bd95e5a4d6ba9f2819ebd533b8388dbfb67a0eb82dd0471746`
- Canonical independent receipt:
  `17833de8443509fcb17e461cad5979cabcfb9ae9ad3462084beb3a3cb27160b2`

[Summary JSON](summary.json) records the diagnostic image, exact graph and small
receipt references. The copied canonical comparison contains no disassembly or
binary payload. Original diagnostic freezes remain historical; their old source
pins are not overwritten or falsely asserted current after source application.

## Admission and CI scope

**Ledger admission is complete after canonical verification:** +65 authored
exact bytes / one owner, and +65 source-present bytes / one owner. Existing
boundary/origin and the 273658-byte authored denominator are unchanged. Totals
are authored exact **413 / 75103 bytes**, authored source **618 / 248561 bytes**,
overall exact **1256 / 142034 bytes**, and 1102 source mappings. No other owner
is credited: GameManagerUpdateBody remains **1572/1544, nonexact** in this graph.
**All 112 target-independent CI stages pass for this maintained-source
checkpoint**, verified 2026-10-25 04:03:35 UTC. In
`.analysis/gpt-dots/game-faith-checkpoint-20261025`, root-ci.exit and
post-ci-inputs.exit are both 0; all **1498 frozen source, documentation and
protected-file inputs** were preserved. The root-ci.log SHA-256 is
`f38c070b31b5c73dafc7cd987badeb0fee180c5a4f933904351b36cd9d324255`.
The summary records log/pin hashes. This documentation-only result update
follows the successful freeze; the canonical proof, +65-byte admission and
nonexact parent are unchanged. Prior CI results remain separately scoped.
