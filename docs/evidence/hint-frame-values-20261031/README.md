# Hint frame values and exact update callback — 2026-10-31

The complete `HintOwnerView::OnUpdate @4198A0..4198A9` is newly exact:10 bytes,
including its one target-bound call to the actual `Update` body. The callback's
origin remains indeterminate. Its registered factory context derives the real
ECX-to-EBX adapter without a calling-convention or header change.

The actual571-byte `Update @419650..41988A` remains **NONEXACT**, despite equal
complete size. Its arithmetic and lookup phases now reproduce; the loop still
uses the ID array as its base instead of the target's width array. There is no
authored exact-byte credit or partial owner admission.

## Source and complete evidence

The prior source emits541/571 and a7/10 callback in the genuine eight-input
factory graph. Fresh target decoding contradicts its widened arithmetic model:
target constants and operands are DWORD floats, and height is recomputed along
the selected branches. Natural float coordinates/distances restore retained
x87 operands, memory comparisons, FIMUL, the two conversion paths and target
EBX owner/RET ABI. No universal floating-point or control-word claim follows.

A small lookup helper clears a failed ID before the frame's interpolation gate.
Its natural expansion restores the zero-ID path, shared failed-lookup return,
ID clear and separate null test. Sharing the integer alpha/quotient locals
across the two fade branches restores both target stack slots. All arithmetic
and tail instructions from offsetE2 onward now match after real field binding.

Warm and zero-reuse eight-input cold reproduce the full571 bytes,170 native
instructions,17 resolved fields,23 local branches and RET at23A. Target geometry
is571/169/17/23 with the same RET. Every field identity and DWORD literal payload
binds independently. The manager field is misplaced: target72 versus native6E.
The two cursor LEAs, ID-relative displacements, width load and an internal
alignment instruction remain different. Local destinations are retained in
full; none of these differences are waived by the acceptance comparator.

Three diagnostic regions0..3C,4E..62 andE2..23B compare equal at fixed offsets,
including actual branch operands. They are not independent units or credit.
The unchanged complete1317-byte consumption owner still reproduces1308/1317
and passes its [previous full evidence check](../hint-consume-context-20261031/README.md).
Its small-ID/register/script-push frontier remains open.

```sh
scripts/repo-python docs/evidence/hint-frame-values-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/hint-consume-lifetimes-20261031/reviewed-final
scripts/repo-python docs/evidence/hint-frame-values-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/hint-consume-lifetimes-20261031/final-cold
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit hint-update-callback --unit hint-create-owner --unit hint-draw-callback --json
```

Both final images preserve factory67/all5, draw callback6/all0, clear79/all2,
scale136/all1 and the new update callback10/all1:298 bytes/all9 fields through
the unchanged complete comparator. The required independent canonical subset is
new update callback, factory and draw callback:83 bytes/all6 in one existing
producer. The completed command also selected scale136/all1 and Caution104/all2
from the previous campaign's set, totaling323/all9 in three producers. These two
extra producers were unnecessary after consumption source/body restoration and
are not future gates. Caution calls AddAutomaticEntry at41A120, not this frame
owner or ConsumeEntries. Clear remains a same-image guard only.

[results.json](results.json) retains complete target/native inventories, field
bindings, the unchanged source graph and profiles, source/header pins, bounded
control fingerprints and scoped cold receipts. Native jobs are serial on CPU0
at nice10 with bounded runtimes and exit logs. No host failure/lifecycle tests,
historical full-cohort replay or product/runtime/portability claim is added.

## Complete controls and restored attempts

| Model | Bytes | Finding |
| --- | ---: | --- |
| Large-branch volatile owner |1296 consume|Merges resource reads; failed push/read prediction; restored |
| Named created-ID wrapper |1308 consume|Moves hidden output to an incoming-argument slot; restored |
| Explicit lookup value |1308 consume|Complete inventory neutral; restored and parked |
| Owner capture before barrier |1303 consume|Moves owner read before branch; restored |
| Branch-local float literals/height/alpha |570 update|Restores EBX owner and callback shape; retained via next controls |
| Float coordinates and horizontal distance |564 update|Restores x87 loads, comparisons and DWORD fade literal |
| Lookup phase helper |571 update|Restores complete zero/lookup/clear/null flow |
| Separate width induction pointer |598 update|Extra pointer spill, larger frame and wrong owner register; restored |
| Shared alpha/quotient locals |571 update|Restores the third branch's alpha stack slot; retained |
| Width-derived slot index |609 update|Emits pointer difference and extra indexed operations; restored |
| Local width address |571 update|Complete inventory neutral; restored and parked |
| Owner/slot lookup helper parameters |571 update|Complete inventory neutral; restored and parked |
| Typed cached position reference |571 update|Uses position+104 instead of the target's y-based+108 cursor; restored |

The consumption experiments are complete comparisons before restoration; their
smaller sizes are not an improvement metric. Neutral models reopen only with
new evidence predicting a distinguishing result. Source/header/ABI/graph
ownership and the target's original declarations remain separate claims.
