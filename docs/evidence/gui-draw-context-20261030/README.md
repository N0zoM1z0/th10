# Complete GUI drawing candidate — 2026-10-30

`GuiView::DrawStageElements` at415800..415AD3 now emits724/724 bytes,
**NONEXACT**, versus708 at entry. The target's two inverted DWORD flag values
restore its marker/final-pair register allocation, branches and loop alignment.
There is no new exact owner, byte, source mapping, boundary or origin credit.

## Complete scope and remaining differences

Independent target decode covers173 instructions,33 address fields,23 direct
local branches and RET4. Four CC bytes precede and12 follow. The sole encoded
E8/E9 entry/interior reference is the real callback's CALL415AF1 to the entry.
The seven-byte registered415AF0 callback remains context only; this batch adds
no ledger boundary, source mapping, origin classification or match for it.

The target reads boss flags as a DWORD, shifts/inverts bit4, then inverts/tests
bit0 separately. The old mask predicates became a byte TEST11. Explicit real
inverted-bit values recover the target operations and the ECX manager lifetime.
No volatile storage, fake ABI/register declaration, forced inline, numeric data
alias, padding or profile change is introduced. Shared headers are unchanged.

The complete candidate contains the same173 instructions and all23 local
destination offsets agree with target. Every one of the33 field roles has an
independently checked callee/global identity or literal payload. This does not
validate their physical positions: the first score rectangle still schedules
LEA before FMUL and the bottom store before FADD. Native factor/address fields
are at D6/F4 versus target D2/EC. All ordinary bytes and field positions remain
mandatory;724 physical bytes and33 roles are not an exact-match claim.

Source still draws40 unconditional VMs, optional score/background/segments,
the gated boss marker and spell pair, with the observed rereads and stored
right edges. Actual body ABI is one stack GUI pointer/RET4. Draw uses ECX
manager/EAX VM/plain RET; rectangle uses EDI bounds/EBX color/plain RET.
Original type/class/TU/helper ancestry and GPU/runtime/portability remain open.

## Reproduction and parked controls

The diagnostic retains the original genuine three-input responsibility order:
GuiDraw.cpp, AnmRectangle.cpp, AnmManager.cpp. Its profile is copied unchanged
from the current canonical ANM renderer, with the real callback as root.
A fresh serial CPU0/nice10/bounded cold rebuilds all three objects without reuse
and reproduces every owned724 native byte,33 resolved field identities,
173 instructions,23 fixed branches and RET4. Source/graph pins and the native
receipt are in `results.json`; this is native candidate reproducibility only.

```sh
scripts/repo-python docs/evidence/gui-draw-context-20261030/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-draw-context-20261030/candidate-cold
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit anm-render-draw --unit anm-draw-colored-rectangle --json
```

Only those two actual canonical callees are replayed:699 bytes/all35 fields.
They also pass strict full comparisons in the GUI image. Its callback passes
7/all1 as provisional context, without ledger admission. No existing canonical
recipe includes GuiDraw.cpp, and no unrelated parent source/header input changes.
Main, hit, movement, state2 and initializer remain at their prior nonexact states.

Output helper, scalar right value, direct constructor, left-plus-width, inline
color, array storage and width-then-offset controls are byte-neutral to the
724-byte candidate. Aggregate value return adds a real16-byte temporary and
emits756/frame20; it is rejected. All are restored. Reopen these specific models
only with new evidence and a distinguishing prediction. Private control artifacts
are archived after hash verification; no speculative flag/TU sweep is adopted.

The canonical match graph and authored totals stay101532/275817 across495
owners, total codegen169636/1372. Source mappings1207/authored689/backlog194.
Product/runtime closure, original data/TU ownership and portability stay separate.
