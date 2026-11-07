# TransitionShakeEnvelope: recover flag-select and float stack lifetime

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete original `TransitionShakeEnvelope @0x0043C710..0x0043C86C`
is **349 bytes**; this callback remains **NONEXACT**.

## Concrete target-observed source recovery

The original uses two separate tests on the single 32-bit game-manager flag
word. Its `0x0043C736..0x0043C74D` instruction sequence loads the word,
shifts a copy right two bits, ORs that copy with the original word, tests
bit zero and then checks `0x72` on the low byte. The prior maintained
expression `(game->flags58 & 0x77u)` is semantically correct but MSVC 7.1
optimizes it to a short memory `TEST`, removing the target's actual
SHR/OR/test sequence. The source now explicitly expresses the two equivalent
flag tests and retains the null short circuit. In the real 14-TU `/GL` graph,
this independent source edit increases the generated owner from **297 to
315 bytes**, recovering the original pause-gate structure without changing
flag behavior.

After calculating the decay envelope, the original loads `frames` as an x87
integer operand, multiplies by `envelope`, **stores a 32-bit float** in a
stack slot and clears the extra x87 stack element *before* making two calls
to `AnmRandomU32InRange`. Later branches reload this same amplitude, negate
it or copy its raw float dword to X/Y shake globals. The previous ordinary
`const float amplitude` let the compiler retain it in x87 across calls and
emit `FIMUL` instead, producing the wrong schedule. The maintained
`const volatile float amplitude` models an observable float local store;
this is a **compiler-lifetime hypothesis, not evidence of actual historical
volatile source or multithreaded synchronization**.

The two edits together emit a **352-byte** owner: only **three bytes longer**
than the complete 349-byte original. The remaining target divergence is
not solely its extent: the original uses ECX/EAX for the game pointer/flag
and ESI for the three-term phase limit, while this compiler context uses
EAX/ECX and saves EBX around the phase-limit calculation. These changes
shift subsequent addresses and some encodings. We **do not** call this owner
exact or register any partial-byte credit.

## Repeated actual compilations and original-field proof

From current source, a pinned MSVC 7.1 SP1 `/GL` primary/support diagnostic
compiled the changed *real* `src/TransitionEffect.cpp` while preserving
verified unaffected input objects for the real Gui updater graph. Natural
variants independently tried split/named/early-return flag tests, volatile
amplitude, explicitly converted/reversed multiply, three-order phase-limit
sums, guarded condition inversions and volatile read views. Results ranged
from wrong 40/297/302/315/320/334/339/348/350/351 bytes to the retained
352-byte source. Some variants were closer in size but regressed register
allocation, branch placement or field positions. All unselected source
variants were restored and retained as diagnostics, not exact claims.

A **fresh current-source canonical cold** rebuilt the actual 14-input image
and selected only the affected already accepted original-bound owners:

| Selected existing exact function | Original bytes | Bound fields |
| --- | ---: | ---: |
| `TransitionEffectView::Initialize` | 396/396 | 37 |
| `TransitionFadeIn` | 133/133 | 5 |
| `TransitionDelete` | 26/26 | 2 |
| `TransitionDim` | 175/175 | 8 |
| **Total** | **730/730** | **52** |

The initializer's own `TransitionShakeEnvelope` callback address still binds
the original `0x0043C710`, but the candidate's static PDB extent is now
**352** rather than the old **297**. Its static address-size metadata was
updated to describe the *actual* contribution, retaining the target size
**349** separately; this address binding never implied callee exactness.

An independent final read of the original target verifies **all 16 actual
external data/callee encoded identities**, then checks that the final
candidate's 16 emitted fields refer to the corresponding actual native
symbols in the same order. Their offsets are **not** all identical: the
second original field starts at `+0x17` vs native `+0x16`, and later
candidate fields are generally shifted three bytes. Such semantic field
correspondence is a diagnostic, **not canonical field-location acceptance**.

Private reproducible trial sources, results, disassembly, source pins and
original-field proof:
`.analysis/gpt-web/20261107-shake-envelope/trial.py`,
`results.json`, `verify-installed.py`, `installed-proof.json` and
`selected-exact.json`; actual final PE/MAP/PDB under
`build/match-linked/GuiTransitionEffectFactory/`.
Only the selected native cold and lightweight CI are current gates. No full
historical replay, exact-owner promotion, original TU identity, runnable game
or runtime/portability claim is made.
