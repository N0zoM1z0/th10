# Complete GUI update candidate — 2026-10-30

`GuiView::UpdateStageElements` at414900..4157FB now emits3836/3836 bytes,
**NONEXACT**, versus3824 at this session's current-context entry. Strict replay
binds all72 resolved fields, all250 fixed local destinations, both RET4 exits
and the complete seven-way selector. Four ordinary bytes at891..894 still differ;
there is no new exact byte, owner, source mapping, boundary or origin credit.

## Complete owner and source recovery

The target owns3805 executable bytes/1088 instructions, three alignment bytes
and28 table bytes at offsetEE0. The table destinations are514,524,538,54C,563,
56A,571. Fourteen CC bytes precede and four follow. Both exits consume one stack
GUI pointer and return RET4. The target PE relocation/direct-call inventory has
70 fields; two additional fixed ECX sound-object immediates atBFD/C30 are absent
from its relocation table. They are still mandatory operands, giving72 explicit
bindings. All literal payloads and21 direct call identities are checked.

Real control-flow corrections put the GUI overlap flag before each independently
null-checked Player branch, preserve both inverted boss DWORD bits, select boss
phase inside each game-mode branch, and branch stage2/3 directly around gauge
allocation. The ten-segment loop uses the target unsigned bound. Existing VM-ID
postincrement/equality operations reproduce the two publication sequences.
Capturing divisor10000 before the first panel call restores its lifetime. Six
complete four-byte ID clears use the existing memset intrinsic and recover the
clear-before-next-slot schedules. Shared headers and compiler profiles stay fixed.

The previously declared and registered `GuiUpdateCallback` now has a real source
definition. Its complete target415AE0..415AE6 adapter pushes ECX, calls the owner
and returns. A same-image strict7/all1 comparison passes, without assigning a new
boundary, mapping, origin or exact credit. Original source-written versus emitted
adapter ancestry remains unknown.

## Remaining difference

At owner offset890 the target loads `[ECX]`, then adds `[ESI+0C]`; the maintained
message lifetime Tick instead loads `[ESI+0C]`, then adds `[ECX]`. This produces
four differing ordinary bytes at891..894. The later frame timer already has the
required operand order. Equal arithmetic, complete size and matching branches
cannot waive this instruction difference. No volatile, fake ABI, copied code,
forced inline, numeric alias, padding or profile/TU sweep is introduced.

Message-reference, lexical-timer and step-value controls are neutral. A real
result-returning addition helper closes the message pair but reverses the later
frame pair; sharing it, taking the step by value and compound assignment leave
that new residual. An output helper adds stores/loads and emits3848. Whole ID
assignment is neutral, a release helper outlines and emits3168, and double
marker arithmetic changes an actual operand width. All are restored and parked.
Changing the root to the real callback changes InitializeVm's native ABI and
emits3840, so it is rejected. The retained graph uses the existing genuine
InitializeVm root and the canonical17-input order/profiles in results.json.
Reopen these models only with new evidence and a distinguishing prediction.

## Reproduction and scoped validation

One independent zero-reuse17-input cold reproduces all3836 native bytes,
72 field identities,250 branches, both exits and seven selector entries. The
native prefix decodes1089 instructions including its one alignment instruction;
the full table is accounted for by authoritative DIR32 fields. Source/graph pins,
normalized native receipt and strict target differences are retained. This proves
candidate reproducibility only.

```sh
scripts/repo-python docs/evidence/gui-update-context-20261030/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-update-context-20261030/final-cold
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --json
```

Those two explicit canonical selectors pass1372 bytes/all34 fields. Both plus
the provisional callback pass in the final GUI image:1379/all35. Compiler/linker
jobs are serial, CPU0/nice10, bounded with retained exits. The source change makes
the previous Player17-input Gui.cpp pin historical; current context is
`.analysis/gpt-6.1-sol/gui-update-context-20261030/final-cold-context.json`.
No full cohort or unrelated parent replay is required. No canonical GUI update
unit or matches.csv row is added. Original types/helper/TU/data ownership,
product/runtime closure and portability remain open. Totals stay101532/275817,
495 authored exact owners; codegen169636/1372; source1207/authored689/backlog194.
