# GUI drawing arithmetic order — 2026-10-31

The complete `GuiView::DrawStageElements @415800..415AD3` remains **NONEXACT**
at724/724 bytes. Two observed arithmetic schedules are restored, but its first
rectangle still materializes the output address too late. No exact owner,
credit, match unit, boundary, source mapping or origin classification is added.

## Source and target observations

The [previous checkpoint](../gui-draw-context-20261030/README.md) has LEA before
FMUL and the bottom store before FADD. A fresh three-input build reproduces
both differences against current headers. Two header pins changed since that
checkpoint, so all three source inputs are rebuilt at entry; the old image is
not treated as current source evidence.

The retained source captures the real score width before reading the color.
A compiler memory barrier separates these reads from bounds stores. A second
barrier separates the right-edge addition from bottom/right publication.
Width and right remain in x87 registers: no extra spill, call, temporary
rectangle or stack space is emitted. The target FILD/color/FMUL sequence and
FADD-before-bottom order now agree. The multiplier field is correctly at D2;
its old position is D6. Original barriers, qualifiers and source helpers remain
unknown. No source/header ABI declarations or compiler/linker flags change.

Target LEA is at D6, native LEA at EC. The four-byte instruction crosses the
left/top stores and addition, shifting that addition's constant field from
target EC to native E8. This26-byte interval D6..EF is an unresolved region,
not a count of wrong bytes. The complete owner still rejects. Normalized
prefix0..D5 and suffixF0..2D3 compare equal after explicit field binding;
neither is a match unit or separate exact credit.

## Complete evidence and scoped validation

Warm and zero-reuse three-input cold images reproduce the same complete724
bytes,173 instructions,33 fields,23 fixed local destinations and RET4 at2D1.
Every target field is freshly enumerated. All33 roles, actual public data/callee
identities and float payloads are checked independently; the one position
difference remains mandatory and unwaived. Both actual direct callees have
PDB-owned bodies. All23 target branch destinations agree. The acceptance
comparator is unchanged.

```sh
scripts/repo-python docs/evidence/gui-draw-read-order-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-draw-close-20261031/reviewed-candidate
scripts/repo-python docs/evidence/gui-draw-read-order-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-draw-close-20261031/candidate-cold
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit anm-render-draw --unit anm-draw-colored-rectangle --json
```

Both candidate images preserve actual Draw216/all21 and rectangle483/all14,
plus the provisional draw callback7/all1:706/all36. The independent canonical
cold selects only the two real callees and passes699/all35 in two existing
producers. The callback remains context with no ledger admission. Interpreter
and other historical units are not replayed merely because they share headers.
No host failure/lifecycle tests or whole-product checks are required.

[results.json](results.json) retains15 source/header pins, the unchanged genuine
callback-rooted three-input graph, full native geometry/identities, all33 bound
roles, region hashes, compact control receipts and canonical replay metadata.
Native work is serial on CPU0 at nice10, bounded and exit-logged. Original
class/TU/data ownership, product closure, runtime and portability remain open.

## Controls parked

| Control | Bytes | Observation |
| --- | ---: | --- |
| Two arithmetic-stage barriers |724|Correct arithmetic; initial color/FILD and LEA order differ |
| Remove first barrier |724|Retains an x87 value across draw and loses required right reload; restored |
| Width read before color |724|Restores FILD/color/FMUL; retains only LEA placement difference |
| Output-pointer helper |724|Complete native inventory neutral against preceding control; restored |
| Width constructor |724|Complete native inventory neutral; restored |
| Narrow read-barrier primitive |740|VC7.1 emits an actual external call and spill; restored |
| Observed named offset constant |724|Hoists FADD and retains an extra x87 value; restored |

The unsupported-primitive/private-constant controls are diagnostic receipts;
their unresolved anchor or non-public data identity is never accepted as a
target binding. All seven models are complete owner comparisons. Reopen the
neutral helper/constructor models only with new distinguishing evidence.

Before selecting this owner, three complete interpreter controls are tested
and restored: explicit shared counter4464, clear-before-resource4472 and typed
statistics records4472. Each retains337 native fields/121 locals/150 calls,
without target exactness. The explicit jump fails to restore target counter
placement. The barrier orders the clear/read but leaves the target schedule
wrong. Existing statistics records, including their owner+8 bias, are byte-
neutral against that barrier candidate. Gui.cpp is restored byte-for-byte to
12e8780; no interpreter source change or cold replay follows. Their compact
private receipts prevent repeating these failed predictions.
