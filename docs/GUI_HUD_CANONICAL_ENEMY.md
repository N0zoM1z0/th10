# GUI HUD canonical enemy ownership

## Scope and status

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

## Validation and regression boundary

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

They include 41 mutation controls with predeclared intended CHECK expressions,
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

## Fresh canonical native observation

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
