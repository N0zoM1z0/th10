# GUI HUD canonical ownership

## Enemy migration: scope and status

`GuiView::UpdateStageElements` must read the enemy manager shared with the
canonical enemy producers. This recovery replaces its local shadow views and
`g_GuiEnemyManager` with the existing `Enemy.hpp` types and `g_EnemyManager`.
Production scope is `Gui.cpp`; no header layout or field-name broadening is needed.
The pre-change whole HUD is 3,772 bytes against the 3,836-byte target and remains
**non-exact**. Fresh native observation confirms the shared manager identity; the countdown reload and nonfinite residuals remain.
No match status or completion credit follows from this ownership correction.

## Identity and field derivation

Target executable SHA-256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Reviewed HUD owner: `0x414900..0x4157FC` (half-open), including its trailing table.

The target HUD reads manager pointer address `0x477704`. Two existing canonical
recipes independently bind `?g_EnemyManager@@3PAUEnemyManagerView@@A` there:
`enemy-manager-constructor` (DIR32 offset 21) and
`enemy-ecl-resource-load-package` (DIR32 offset 67), in `config/match-units.toml`.
The pre-change candidate instead anchors its shadow manager separately.
All offsets below are hexadecimal and relative to the indicated object:

| HUD access | Canonical access | Offset derivation |
| --- | --- | --- |
| Active boss | `specialEnemySlots[0]` | manager `+0x10` |
| Boss X | `runtime.worldMotion.position.x` | enemy `0x103C + 0x2C = 0x1068` |
| Life | `runtime.life` | enemy `0x103C + 0x13C0 = 0x23FC` |
| Maximum-life input | `runtime.unknown13C4` | enemy `0x103C + 0x13C4 = 0x2400` |
| Gauge threshold | `runtime.value13C8` | enemy `0x103C + 0x13C8 = 0x2404` |
| Flags | `runtime.flags` | enemy `0x103C + 0x1444 = 0x2480` |

Keep the neutral canonical field names. The manager constructor publishes
`g_EnemyManager = this`; its destructor clears it. ECL `SET_BOSS_SLOT` publishes
or clears special slots, and enemy death cleanup clears the selected slot.
`SET_LIFE` writes life and `unknown13C4` and initializes `value13C8`;
existing operand readers already consume slot-zero world position.

## Callback lifetime contracts

- Gauge phase captures manager at target `0x414ACF`, then boss at `0x414ADC`.
- Countdown captures manager at `0x41549A`, reloads it after sound/sprite
  callbacks at `0x4155AE`, then selects slot zero at `0x4155BA`.
- Retain that selected boss through marker `ExecuteScript` at `0x4156D5`;
  read its position afterward at `0x4156DA`. Do not reselect or cache X early.
- Preserve null gates and independent ANM resource/manager capture lifetimes.

The pre-change candidate reuses its shadow manager despite the source reload.
Canonical ownership neither proves callback mutation nor guarantees a native
reload. Do not add volatile, aliasing casts, ABI controls, or forced reloads.

## Enemy migration: validation and regression boundary

[`test-gui-boss-gauge-phase.py`](../scripts/test-gui-boss-gauge-phase.py) and [`test-gui-hud-lifetimes.py`](../scripts/test-gui-hud-lifetimes.py)
use canonical nested fields and retain callback-replacement and retained-boss
negative mutations. Obsolete declarations are removed from
`scripts/fixtures/gui-message-run.cpp` and `scripts/fixtures/gui-message-lifecycle.cpp`.
The source-extracted host fixtures use the actual complete enemy/player member
declarations with documented pointer-width assertion, timer-construction and
external-callee seams. They check bounded callback contracts, not complete game
execution. Pinned 32-bit compilation is still required for layout checks.
Canonical symbol identity alone does not establish physical target storage or
runtime closure.
Reuse the established 14-TU canonical graph. Recheck source closure at the actual
base: the pinned canonical configuration identifies seven producer groups / eleven units:
`AnmVmIdSetInterrupt`, `GuiLivesDisplayCount`, `PlayerResetRuntimeState`,
`GuiScoreAdd`, `AnmVmIdSetSprite` (two units), `MainNextGameMode`, and
`GuiTransitionEffectCallbacks` (four units). Preserve their existing exactness
and retain fresh whole HUD/Run evidence.

From the repository root, run both maintained host suites:

```sh
scripts/repo-python scripts/test-gui-boss-gauge-phase.py --source src/Gui.cpp
scripts/repo-python scripts/test-gui-hud-lifetimes.py --source src/Gui.cpp
```

The enemy-migration checkpoint used 41 mutation controls with predeclared intended CHECK expressions,
unique generated-source line anchors, exact diagnostics, empty stdout and exit
status 1. Wrong assertions, compile failures, signals, sanitizer diagnostics and
timeouts cannot count as successful mutation detection. Compiler/execution
timeouts are 120/15 seconds. Source guards reject retired shadows and missing
canonical member paths. The existing CI already invokes both suites.

## Separate floating-point finding; no bundled fix

Target alpha comparison admits less **or unordered**; the candidate admits only
less. Verified target constants are double `64.0` at `0x470CF0` and float
`-2.984375` at `0x470CE8`; the candidate double coefficient has the same exact
value. Neither body changes x87 precision, and neither spills the distance.
Normal NaN branching assumes masked invalid exceptions. Reachability, conversion
outcome, and a runtime bug are unproved; C++ NaN-to-integer conversion is outside
its defined domain. A predicate change alone is not a complete nonfinite fix.

## Enemy-migration canonical native observation

Source Gui.cpp SHA-256: `7a7942a2b8027fa4779232768c0e57a23c5ed853c9b76075dc3ebc461bd8cd93`.
PE SHA-256: `85ac5e8b2464096dd28f57bbc2414957932696a49999306cfc9acf75520db2f5`.
Retained evidence SHA-256: `243aa3c5636dde0e82f91a227da89760b6612cbdd3c67fcb1b3d9c0494666976`.
Canonical replay SHA-256: `8d47e2d2c3320757de8f4d34b681e9a9961f96bffb3f7942ee91d54a4d05cc8d`.
Input freeze SHA-256: `8f97f5c5fd3beeb64555b4c9549b75af083ef0433cc26f2cb0e78d918fb65458`.
The replay retains all eleven existing exact units across seven producers, 912
matched bytes. This adds no whole-HUD/Run exactness or completion credit.

Whole HUD remains 3,772 bytes (3,741 executable, three alignment bytes, seven
four-byte table entries), with 1,081 instructions and 21 unchanged call identities.
All seven table destinations and all intrafunction direct jump destinations remain unchanged.
The gauge load at `0x40231F` and countdown load at `0x402CD6` now address canonical
`g_EnemyManager` at `0x42B470`, replacing shadow `0x42B58C`. The shadow MAP symbol is
absent; the canonical variable remains anchor-backed. Slot `+0x10` and nested
field offsets `+0x1068`, `+0x23FC`, `+0x2400`, `+0x2404`, `+0x2480` are retained.

Complete local byte accounting finds twenty changed instructions / 22 changed
bytes: these two manager referents, fourteen same-symbol HIGHLOW address shifts
and four same-symbol call displacements. Every other HUD byte, including padding
and tables, is unchanged. Address-field validation and independent GNU decoding
cover the whole executable span. This is a bounded HUD finding, not an assertion
that every changed producer contribution is equivalent.

The native countdown reload was **not recovered**: `0x402DD6` still copies captured
ESI to ECX before selecting boss slot zero at `0x402DDE`; target `0x4155AE` reloads
its manager global. The selected boss is retained in ESI across marker execution
at `0x402EE5`, with X read afterward at `0x402EEA`. Call arguments, field accesses,
null gates and RET4 remain unchanged. No volatile or forced-reload workaround is
introduced, and synthetic host callback replacements do not prove actual runtime
callback behavior.

The alpha branch remains `TEST AH,5 / JP` at `0x402F22/0x402F25`, unlike target
`TEST AH,1 / JNE` at `0x415712/0x415715`. The earlier less-versus-unordered finding
and all x87 precision, invalid-exception, conversion and reachability qualifications
therefore remain. Canonical identity does not establish target physical storage,
alias behavior, private-ABI/runtime closure, a recovered reload or target execution.
Logs and binary evidence remain external to the repository documentation.

## Player ownership follow-up: native observation recorded

The separate player follow-up replaces HUD's `GuiPlayerView` / `g_GuiPlayer`
with the existing `Player` / `g_Player`. Production scope remains `Gui.cpp`;
Run and the floating-point expressions are unchanged. Bounded player native
results are recorded below. The enemy-migration native results above are historical evidence
for that change only and are not player-follow-up native results.

Target HUD loads player pointer `0x477834`. Four existing DIR32 linkage recipes
in `config/match-units.toml` bind `?g_Player@@3PAUPlayer@@A` to that address:
`player-option-special-body` (+0x01), `bullet-update-state8` (+0x1F),
`bullet-update-aimed-direction` (+0x53), and `bullet-runtime-update` (+0x250).
The enemy-migration baseline still anchors `g_GuiPlayer` at `0x42B5B0` separately
from canonical `g_Player` at `0x42B644`; these are candidate addresses, not proof
of physical target storage. Player construction/destruction publishes/clears
`g_Player`, and movement/reset paths produce the float draw position.

The shadow float X/Y at `+0x3C0/+0x3C4` map to `Player::drawPosition.x/y`.
Do not substitute `Player::positionX/Y`: those are distinct integer fields at
`+0x3CC/+0x3D0`. The old portrait resource member at `+0x10` maps to `resource`
(`void*`), already used canonically by Run; HUD has no resource read to migrate.
`Player.hpp` supplies the complete member declarations and target layout checks.

Preserve three callback-separated player-read phases:

- Panel visibility: target load `0x4149A6`, after `displayVms2`, `displayVms3`
  and `displayVms4` callbacks; preserve the existing null gate.
- Gauge positioning: target load `0x414B5A`, after `displayVms5[0..6]` callbacks
  and gauge boss/life setup; preserve the active-path live-player assumption.
- Marker alpha: target reload `0x4156E6`, after marker `ExecuteScript` at
  `0x4156D5`. Retain the previously selected boss and read its X afterward at
  `0x4156DA`, while using the newly read player for subtraction at `0x415702`.

Do not capture one player at function entry, reselect the boss after the marker
callback, or cache boss X before it. Separate fixture replacements at
`displayVms4[1]`, `displayVms5[6]`, and `bossPositionVm` can distinguish these
contracts. Use valid actual Player objects with different float/integer position
sentinels. The author confirms two additional inventory-checked temporary-host
constructor seams: `PlayerOptionRuntime::flags80` and
`PlayerConstructorFlagView::flags` initialize to zero instead of reading an
uninitialized flag. Existing timer seams remain; production `Player.hpp` is
unchanged. Host Player constructor/destructor definitions are dependency seams,
not production lifecycle coverage.

The same two reproduction commands above now run six lifetime tests and five
gauge tests, including 50 strictly classified mutation controls: all 41 prior
controls plus nine player-specific controls. Distinct callback replacements
exercise the actual owner prefix through gauge work and the retained-boss /
current-player marker path; wrong integer-field and stale-player controls must
fail at their predeclared CHECK. Exact diagnostics and 120/15-second compiler /
execution limits remain. Independent ASan/UBSan positive runs supplement these
bounded host contracts; they do not establish target lifecycle behavior.
Reuse the 14-TU graph and recheck the seven-producer/eleven-unit source closure
for this separate change. Preserve the enemy lifetime controls and all existing
exact units. No whole-owner match or completion credit is claimed.

## Player follow-up: bounded native observation

The separate replay retains all eleven canonical units across seven producers
(912 exact bytes). The reviewed source and artifact identities are:

- `Gui.cpp`: `dde1c03602d80e3616fe3d42c0befe186d3078723128db1c6d89fc1a598a8241`
- PE: `a062c8295ef581332799b57f165fef4c8374cb2279a8e76972d34c8b8643b855`
- Evidence: `527b61f1af8a4e4f06b4cbe8b09777854aa13e06b3a21887b14052b2a05328ef`
- Replay: `bb5752721d8ed7b7a6de1a2d70a7ac47fa9b255f30440004809d4d88e21327d4`
- Input freeze: `973ff6bee86549638d54ab2f6cd14bf0dc67f283268fc44897492f1777a099b7`

Against the enemy-migration baseline evidence `243aa3c5...`, HUD remains
3,772 bytes: 3,741 executable bytes, three padding bytes and seven table words.
Its owner SHA-256 is
`1ac04b5802d9845c7ee1e99ea13d4fda2e8d21a13a3ca8eeacd9af31d054d593`.
The three loads at `0x4021EF`, `0x4023A8` and `0x402EF6` now reference the single
canonical `g_Player` MAP identity at `0x42B640`, replacing shadow address
`0x42B5B0`; the shadow MAP identity is absent. These are the only three semantic
operand-role changes within HUD. The complete raw delta is eleven instructions /
fourteen bytes: these three corrections, four same-role HIGHLOW address shifts,
and four same-identity external CALL address shifts. All other HUD bytes,
including padding and table words, remain unchanged.

The three callback-separated reads retain their panel, gauge and post-marker
positions. All float accesses remain at `+0x3C0/+0x3C4`; no integer
`+0x3CC/+0x3D0` substitution occurs. Marker `ExecuteScript` at `0x402EE5` still
precedes the retained-boss X read at `0x402EEA` and player reload at `0x402EF6`.
Canonical enemy identity remains `g_EnemyManager` at `0x42B470`. All 21 call
identities/order, seven table destinations, intrafunction direct jump destinations,
and ABI instruction forms including `RET 4` are preserved. Eight comparator
branch-window notices have identical branch bytes/target indices and merely
include a corrected player referent in their destination windows.

The countdown still reuses its captured manager (`MOV ECX,ESI` at `0x402DD6`):
the missing target-style post-countdown reload is not recovered. The unchanged
`TEST AH,5 / JP` alpha predicate retains the earlier less-versus-unordered
residual, with the same x87 precision, invalid-exception, conversion and
reachability limits. Canonical identities and host callback witnesses do not
establish target physical storage, alias behavior, private-ABI/runtime closure
or actual callback replacement behavior. This bounded HUD review grants no new
exactness/completion credit and makes no whole-producer equivalence claim.
The separate provenance review passes the bounded HUD/Run, 47 required owners
and eleven registered units. It also records an unrelated `DrawScoreEntry`
scheduling change: two independent stack-slot MOV reloads exchange order
within the same eight-byte span; the loop enters at the pair start, with no
decoded direct jump to its interior. This observation does not extend equivalence
to whole producers; 218 broader base/addend cases remain unresolved.
Logs and binary artifacts remain outside the repository documentation.
