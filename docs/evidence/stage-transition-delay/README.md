# Complete primary-stage transition delay (2026-10-19)

GameResetAnimationSlots at0x4045B0 now closes **92 authored exact bytes**.
Two independent cold CL13.10.6030/LINK7.10.6030 links reproduce the complete
PDB-owned function and its sole independently target-bound DIR32 field.
The same-image update callback7 and static chain marker28 retain35 existing
bytes and their one call field in both images. No duplicate credit, boundary,
owned-byte denominator or origin-confidence change is made.

## Target and actual caller binding

Read-only local Ghidra attests the disk target and full mapped `.text` before
and after the rolled-back review. Dense target decoding consumes92 bytes,
ending at0x40460B; four CC bytes precede the independent evaluator at0x404610.
The helper lazily initializes the timer at owner+0x2A1C, sets current60,
subframe60.0f and previous59, then ORs4 into flags at+0x2A18. Its sole address
field at+0x2C binds the existing game-speed scalar0x476F78.

The complete GameManagerUpdate call window distinguishes two objects:

| Caller | Loaded pointer | Target | Maintained interface |
| --- | --- | --- | --- |
| 0x4181F2 | menu global0x4776E4 | 0x404530 | GameEnterMenuState, still nonexact |
| 0x4181FC | primary-stage global0x4776E8 | 0x4045B0 | GameResetAnimationSlots, exact92 |

This corrects the preliminary routing label in the preceding handoff; no
incorrect menu binding entered the exact ledger. The119-byte menu helper also
spawns an effect and starts a30-frame timer, so the two owners are not merged.
`target-facts.json` records the independently decoded operands and whole-owner
hash. Original class/TU names and complete storage/data ownership stay unknown.

## Maintained source and compiler context

The real GameManager.cpp definition reuses the established StageScriptView
layout and AnmVmTimerView::SetCurrent. It casts the caller's existing partial
GameChainOwnerView pointer to the maintained Stage view, sets the transition
timer to60 and ORs4. No new global, shadow layout, ABI annotation, forced
outlining, padding, fake return or synthetic caller is introduced.

The existing single-TU GameManager callback graph uses real OnUpdate as entry
and `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, with no support sources.
The profile is reproduced from that existing canonical graph; it does not
identify original production flags. Twenty CPP/quoted-header hashes agree
across the cold links, whose PE hashes and PDB identities differ.
The emitted helper takes its owner in EAX and returns with plain RET, matching
the target's private interface. Its source declaration remains ordinary C++.
The native update core retains a real direct call to the helper; its own
complete contribution remains nonexact and receives no additional credit.

The two existing guard recipes move to the same canonical output directory,
without changing their entry/profile or adding credit. One native build covers
all three complete functions:

```sh
scripts/repo-python scripts/build-match-unit.py --unit game-stage-transition-delay
for unit in game-stage-transition-delay game-manager-update-callback game-manager-mark-chains; do
    scripts/repo-python scripts/compare-linked-function.py --unit "$unit" --json || exit 1
done
```

`native-summary.json` pins both strict results, inputs, tools and the declared
graph. The complete GameManager update core remains nonexact; its other
dependencies and caller topology are separate claims. Diagnostic unresolved
symbols are data anchors, so the image is not executed. This does not close
whole-product/runtime, semantic or portability gates.

The next119-byte menu-helper dependency is the82-byte transition factory.
`next-effect-target.json` pins its one-size allocator call452493 and RET20.
The maintained factory's nothrow allocation and cdecl model need separate
review against that target and the historical compiler; the allocator's
exception/runtime behavior is not established by this call-site evidence.

Canonical default replay, toolchain declarations, tracking, generated progress,
full public CI and whitespace checks pass. Only the new helper and its two
same-graph guards are replayed; no historical full native cohort is rebuilt.
