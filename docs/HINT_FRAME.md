# Hint entry consumption and frame update

## Current state

| Complete owner | Current result | Evidence |
| --- | --- | --- |
| Update419650..41988A |571 bytes/all17 fields exact|[Accepted frame proof](evidence/hint-frame-exact-20261031/README.md) |
| OnUpdate4198A0..4198A9 |10 bytes/all1 field exact; origin indeterminate|[Callback proof](evidence/hint-frame-values-20261031/README.md) |
| ConsumeEntries419120..419644 |1308/1317 NONEXACT|[Complete consumption proof](evidence/hint-consume-context-20261031/README.md) |

The canonical frame and callback use the existing eight-input factory graph,
with Hint.cpp as primary source and the pinned VC7.1 LTCG profiles. The frame
has169 instructions,23 fixed local branches, private EBX owner and a plain
return. Exactness covers complete owned bytes and every resolved field.
Original names, declarations, class/TU ownership, product closure, runtime
semantics and portability remain independent.

## Maintained layouts and behavior

The existing owner view is1A8 bytes: IDs start atDC, cached XYZ positions at104,
widths at17C and the next-slot index at1A4. Entry position uses AnmFloat3View;
the loaded resource is the existing effect owner's field at899C. These views
and canonical globals introduce no new storage.

ConsumeEntries caches the next link before mutation, decrements a countdown
only for zero/matching base and still processes an already due nonmatching
entry. It clears the old slot ID after primary/secondary lookup and propagates
pending deletion along a root VM's flat child list. Scale selects three creation
branches before post-call reads; alignment2/1/0 chooses the text operation and
cache update. Slot advance and unlink precede optional entry deletion. The text
is the existing format argument. Invalid-index/failed-resolution recovery and
new clamps are not added.

Update consumes the selected stage list, resolves each ID with failed-ID
clearing, and skips proximity alpha changes during active alpha interpolation.
It uses cached VM positions/widths and the VM sprite-height/scale product.
Inner, horizontal-band and vertical-band alpha branches keep the target's
strict comparisons and signed quotient(3*alpha)/4. Both frame counters advance;
the comparison base is not overwritten.

## Floating-point and code generation

Natural float coordinates/distances, DWORD constants and branch-local height
evaluation reproduce the frame's x87 operations. Player coordinates and
horizontal distance remain in x87; dy, half-width and outer-width materialize
at the observed boundaries. The native control word and general host/native
floating-point equivalence are not established.

The width-array expression is written at its three genuine uses. Ordinary CSE
keeps one half-width calculation, while loop reduction selects the target width
cursor. A failed-ID lookup helper and shared alpha/quotient locals reproduce
the observed lookup flow and stack slots. No explicit extra induction pointer,
forced registers, padding, shared ABI declaration or profile change is used.

Consumption still differs in small-ID result/register ordering and common
script-PUSH placement. Its three resource observations and large-branch compiler
barrier are source models of scheduling; original qualifiers remain unknown.
That callee's nonexactness does not transfer to the exact caller or conversely.

## Validation and historical reports

```sh
scripts/repo-python scripts/replay-exact-units.py --unit hint-frame-update --unit hint-update-callback --json
scripts/repo-python scripts/ci.py
```

Only new/affected exact selectors are replayed. Caution calls AddAutomaticEntry,
not either frame owner; its older extra replay is not a current gate.

The former widened-double source model and host fixture/mutation/sanitizer
reports describe older checkpoints, not the maintained source or current
requirements. They remain recoverable with
`git show da009fb:docs/HINT_FRAME.md`. Current native facts are in the linked
complete proofs; later product/runtime/portability work stays open.
