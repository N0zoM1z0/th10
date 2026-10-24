# HUD conditional clears and delete identity, 2026-10-24

## Measured scope

Starting from capture checkpoint **726d795**, two individually measured natural
conditional-clear models and a separate deallocator-call model retain the captured-life sequence and all **13 exact
guards, 1468 bytes / all 79 fields**. All three native exits and independent reviews pass. Full HUD remains **3824 versus 3836 bytes, nonexact**. This batch adds
**zero new exact bytes, source owners or authored denominator bytes**.

| Model | Complete HUD extent | Syntactic fields / local branches / calls |
| --- | --- | --- |
| Captured-life baseline | 3772 = 3741 code + 3 alignment + 28 table | 71 / 248 / 21 |
| Excess-segment clear inside its nonzero gate | 3824 = 3796 code + 0 alignment + 28 table | 72 / 249 / 21 |
| Hidden-gauge clear inside its nonzero gate | 3824 = 3796 code + 0 alignment + 28 table | 72 / 249 / 21 |
| Explicit global operator delete | 3824 = 3796 code + 0 alignment + 28 table | 72 / 249 / 21 |
| Target | 3836 = 3805 code + 3 alignment + 28 table | 72 / 250 / 21 |

All seven table words remain owned. Inventories are syntactic, not accepted
semantic bindings; the 12-byte extent gap is not a mismatch-byte count.

The excess model makes zero-ID paths skip lookup and clear, while every
terminating nonzero path clears once. Its +52-byte extent change also reschedules
other phases; it is not a one-branch byte change. The extra physical absolute
field is a real EnemyManager reload after countdown calls, recovered naturally
without changing manager source or adding volatile/alias constraints.

The separate hidden model changes **exactly one branch displacement byte** in
the complete HUD. Every other HUD byte, all table bytes and the full 4480-byte
Gui Run owner are unchanged against the excess candidate. Zero ID now skips the
ID clear but still reaches the five unconditional gauge-reset stores; all
terminating nonzero paths clear once. The manager load still precedes the gate,
unlike target, so that ordering is not claimed repaired. Independent life,
excess and hidden proofs pass; deliberate invalid-graph controls reject.

## Separately measured HUD deallocator call

The third model changes only `free(finishedMessage)` to explicit global
`operator delete(finishedMessage)`, on the accepted source-current Gui image
from the repair below. Native exit 0, all 13 guards and inherited life/excess/
hidden proofs pass. HUD remains 3824 bytes. The actual call now names scalar
operator delete, not free; sharing a CRT backend does not make these identities
interchangeable.

Manual cleanup remains intact: six ID-zero stores, 18 flag-access effects,
one captured-owner argument, four-byte caller cleanup, null-owner bypass and
GUI null publication after release. No destructor edge is introduced; typed
delete would duplicate the already manual cleanup. This is a bounded call and
effect inventory, not full semantic or runtime proof. The diagnostic callee is
still anchor-backed. Independent review passes with **4340 frozen inputs** and
the repaired baseline preserved. Only **two HUD bytes**, offsets 0xB89/0xB8A
inside the actual call displacement, change. All other HUD bytes, seven table
words, six manual cleanup walks and the complete 4480-byte Run owner are
raw-identical to that baseline. This is before/after accounting, not target
exactness or a mismatch score.

## Canonical deallocator identity repair

A separate repair changes Enemy.cpp, Chain.cpp and five symbol contracts to
actual scalar `operator delete`, rather than mapping `_free` onto delete.
The pinned archive's complete five-byte delete body identifies target 0x4524A1;
its tail call reaches the distinct free routine at 0x452422. Explicit global
operator delete models proven deallocation only. Enemy payload type and original
allocation/deletion spelling remain unknown.

Three **existing** authored exact owners are revalidated: release allocations
45 bytes, host destructor 47 bytes, and chain unregister 114 bytes, **206 bytes
already credited**. Exactly one cold build per Enemy / ScriptViewer / Gui
producer supports all **55 distinct comparisons** (11 / 31 / 13), each replayed
canonically and independently. No exact numerator or denominator changes.

Preserve the original root exit **1**: its GUI observer requested an obsolete
nonstatic Run symbol. A separate same-image supplemental audit uses the real
static selector, passes all 13 guards and preserves 205 original artifacts;
there is **no repeat native build**. The earlier Chain line-ending mismatch
stopped in preflight before any compiler started; exact reviewed candidate bytes
were restored without changing the frozen contract. The final acceptance review
passes and verifies the actual delete call fields and separate free anchor.

After the separately measured HUD deallocator edit, Gui.cpp changed again and
is a ScriptViewer support input. One additional source-current ScriptViewer cold
build and all 31 comparisons/independent replays pass for Gui SHA `6ad41833...`.
This is a changed-input regression, not an arbitrary repeat of the repair.
The earlier Enemy result and fresh 13-guard GUI trial remain applicable. Receipt:
`.analysis/gpt-dots/scriptviewer-hud-delete-current-20261024/receipt.json`.

## Host observer correction

The first full CI exited 1 because the extracted message-phase fixture intercepted
only free, not explicit operator delete, and attempted to release synthetic stack
fixtures. The original log/exit and frozen-input check are preserved. A narrow
phase-only class-scoped observer now intercepts that explicit call without
replacing process-global deallocation or changing production source. All 16
existing mutation checks remain; a wrong-deallocator mutation is added. Focused
checks pass with UBSan and controlled exit-1/line diagnostics required. A new full
CI v2 passed all 112 stages with frozen inputs preserved; the first failed run
is not relabeled successful.

A bounded scan found one analogous Chain host adapter. Its exact token now
observes operator delete, with only the already reviewed Chain LF hash updated
in the host-input manifest. All other pins and exact-count rejection rules are
unchanged. Staged normal and ASan/UBSan runs each passed 1375 assertions; strict
EOL tests passed 19/19. No production code was altered by either observer repair.

## Receipts and pending boundary

[Machine-readable summary](summary.json) records source/image identities and
receipt hashes. The actual fourteen-TU Gui graph and order remain those in the
[capture evidence](../hud-life-capture-20261024/README.md): `/GL` **without /GS**,
no support overrides. Ignored evidence is retained in:

- `.analysis/gpt-dots/hud-conditional-clear-trial-20261024/`
- `.analysis/gpt-dots/hud-hidden-clear-trial-20261024/`
- `.analysis/gpt-dots/hud-deallocator-trial-20261024/`, including the passed
  `independent-review/receipt.json`.
- `.analysis/gpt-dots/free-delete-binding-audit-20261024/repair-validation/`,
  especially `final-acceptance-review.json` and `supplemental-gui/receipt.json`.

These packets are not supplied by a Git clone; no disassembly or target payload
is copied here. Full CI v2 passed all 112 stages for this batch, exit 0, with
frozen inputs preserved, verified at 23:33:14 UTC. Only CI-result documentation
changed afterward. The first CI failure and its unchanged inputs remain recorded.
WinMain stays parked; runtime closure, original allocation spelling and full
HUD field/local-target acceptance remain unproved.

## Next hypothesis boundary

- Completed separately: excess clear, hidden clear and the actual HUD
  deallocator-call model, each with full-owner and inherited-guard evidence.
- Being prepared, **not tested**: default-first boss-marker comparison.
- Separate and **untried**: a float-local model for the observed target float4
  versus candidate double8 multiply referent. Original expression spelling and
  the remaining ordinary/control-flow differences are not thereby resolved.
