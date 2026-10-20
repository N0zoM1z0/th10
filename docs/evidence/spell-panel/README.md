# Spell-card panel frame update

`EnemyGameStateView::UpdateSpellPanel @ 408D60` now has maintained source for
its complete **1,057-byte authored owner**. The registered callback at `409220`
adds ten source-present bytes with origin left indeterminate. Neither owner is
exact; this checkpoint adds **zero exact bytes**. Original production class,
translation-unit and data ownership remain open.

## Target review

The original Japanese TH10 1.00a target has SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local headless Ghidra ran read-only/noanalysis with a 768 MiB heap and one
active processor. The executable and complete mapped `.text` were attested;
temporary metadata was rolled back and mapped bytes remained unchanged.
Decompiler output supplied hypotheses, independently checked against raw
instructions and existing TH10 helpers.

The update returns at `409180`, before CC padding. Its Ghidra body contains
1,045 bytes and omits six-byte alignment instructions at `408EDA` and `408F5A`;
both belong inside the existing 1,057-byte physical extent. No authored
denominator correction is needed. Callback `409220..409229` saves EDI, transfers
ECX to EDI, calls the update, restores EDI and returns. The callback address is
installed at `4089C2`; CC padding separates both neighbors. Its source-written
versus compiler-generated origin cannot be determined from that protocol.

`scripts/report-spell-panel.py` fails closed on target/decoder identity,
complete decoding, local branch destinations, all fifteen static callsites,
padding, registration, selected sampling operands and current boundary ledgers.
It also checks the actual start-of-spell bonus stores. The target has no PE
base relocations; raw control flow and neighbors establish these boundaries.

## Recovered state and behavior

The shared partial owner at global `4776F4` now has one header definition,
replacing inconsistent same-name definitions in the ECL TU and the old narrow
header. Native assertions preserve the two stage VMs, three IDs at `768`,
managed ID `774`, eight bonus VMs `778`, five counter VMs `24D8`, timer `3734`,
name `3748`, spell ID `3788`, flags `378C` and marker `379C`.

Bonus field names were reversed in the old recovery: **current bonus is3790,
initial bonus is3794**, and duration is3798. The existing spell-start function
still writes both from the same calculated value and caps **only3794** to
99,999,999. Renaming preserves those writes. No gameplay correction is made.

Inactive updates return one before accessing other globals. Pre-timer frame60
clears background flag1; frame300 begins the signed bonus decay unless flag8
is set. Two stage VMs execute before the timer advances. Post-timer frame120
enables strict Y<96 / Y>128 hysteresis: three managed IDs receive interrupts,
all eight bonus VMs and five counter VMs receive pending interrupts, then the
owner's visibility bit changes while preserving its other live bits.

Digit updates call the genuine **SetSprite43E5A0**, followed by explicit VM
execution. SetAndExecuteScriptIndex43E7E0 would reset/execute script state and
is the wrong helper. SetSprite itself has no callback or VM execution. Bonus
digits retain one value snapshot across execution while resampling the GUI
resource per digit. The counter phase captures the then-current GUI once for
four sprite writes, reloads its frontAnm each time, and independently resamples
statistics for attempts and captures. Counts are capped above99 only; signed
division/remainder are retained. All five counter VMs then execute.

The marker reads special enemy slot0 at manager477704, with target position at
enemy+1068, interpolates by .05 and resolves the current renderer/managed ID
through the actual SetVmWorldPosition helper. Background4776E8, GUI47770C,
statistics47783C and selection474C68/474C6C keep their existing TH10 identities.

## Native and host scope

One final complete cold link compiles SpellPanel.cpp, AnmManager.cpp and
AnmVmId.cpp with the pinned VC7.1 SP1 `/GL /GS` profile and the genuine callback
entry, freezing16 source/header inputs. Update emits **1060/1057**, callback
**7/10**, both nonexact. The candidate saves/transfers EDI in the member and
tail-forwards from the adapter; the target saves/transfers it in the callback.
An ordinary call-then-return-one callback control produces13/10 without changing
the1060-byte owner and is rejected/restored. No forced attributes or invented
register context are adopted. Timer operand order and x87 marker spill/rounding
remain outside an exact or general numeric-equivalence claim.

The same carrier structurally preserves the existing complete SetInterrupt65,
SetSprite356 and SetVmWorldPosition111 bodies, modulo2/2/5 link fields. This is
a diagnostic observation, not a new canonical replay or new exact credit.
The changed ECL TU and header consumers Enemy.cpp/Player.cpp separately compile
with native layout assertions under VC7.1 `/GL /GS /EHs`. Their complete parent
bytes were not replayed. The older FrontEnd cache has stale EnemyGameState.hpp
and EnemyEclDispatcher.cpp inputs; its hash guard must reject whole-cache reuse.
Unchanged objects may be reused only after checking their individual dependency
closure. The separate MIDI cache remains current.

`scripts/test-spell-panel.py` extracts the actual update/callback and actual
SetSprite, SetInterrupt, FindVm and world-position helper bodies. Canonical
panel/ANM headers use explicit host layout/assertion/prefill adaptations.
VM execution and the outer Player/GUI/Enemy/background globals are host roles.
Sprite entry observation only records calls and never invokes callbacks.
ASan/UBSan/leak checks pass, with thirteen strict mutants required to fail at
their named functional assertion. Sanitizer failures do not qualify. The
updated auxiliary-player test consumes the same canonical owner header and
passes. All86 public CI checks and final target/tracking/toolchain/progress/
whitespace checks pass. Host cases cover defined arithmetic/valid sprite and statistics
storage; they do not prove invalid pointers, integer overflow, native x87/NaN
payloads, runtime reachability, production construction, product or port gates.

The MIT reference from [issue1](https://github.com/N0zoM1z0/th10/issues/1),
[`levzzz5154/th10-decomp`](https://github.com/levzzz5154/th10-decomp), is pinned
at `c6791d82dc9f02e55eca45a5314f900a85481efd` in ignored
`_reference/th10-decomp`. Its `matching2/msvc/src/game/card/card_update.cpp`
supplied source-shape hypotheses; its generic Configure name did not identify
the actual TH10 helper. The [MIT attribution](../../licenses/th10-decomp-MIT.txt)
is retained. No placement pragmas, compiler-control attributes, assembly
fallbacks or upstream completion metrics are adopted. Pre-existing D3D
evidence remains untouched.
