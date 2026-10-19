# Complete script browser Draw owner

Natural `ScriptViewerView::Draw` closes **664 authored exact bytes** at
0x40A940. The existing callback at 0x40ABF0 closes ten more exact bytes whose
origin remains indeterminate. Two independent frozen-source VC7.1 SP1 cold
links compare both complete owners and all **43 new fields** with zero
differences. The 42 Draw fields include its switch-table reference and all
four local destinations. No cropped 647-byte comparison earns credit.

See [target ownership facts](target-facts.json),
[the two native proofs](native-summary.json) and
[the complete new field bindings](field-bindings.json).

## Physical extent correction

The earlier ledger ended at the RET at 0x40ABC6, covering 647 instruction bytes.
The bounded selector `unsigned(state - 1) <= 3` enters a four-slot absolute
jump table at 0x40ABC8. One NOP at 0x40ABC7 aligns that table. All four slots
land on instruction starts inside Draw, the table base has only the switch
reference at 0x40A981, and neither the ledger nor fresh Ghidra body ranges have
a competing owner. Eight CC bytes follow the table before the separate
callback at 0x40ABE0. The complete owned extent ends at 0x40ABD7: **647 code +
1 alignment + 16 table = 664 bytes**.

This target-local review adds 17 bytes to the provisional authored denominator.
The existing authored/medium origin classification is preserved. Ghidra's
reachable body remains 647 bytes; a fresh readOnly query checks the table and
canonical mapped-text hash separately, with temporary metadata rolled back.
The original fixed PE has no base-relocation entries. Target table ownership
comes from the selector, indexed JMP and actual absolute pointers; the linked
candidate supplies the four authoritative DIR32 fixups used by the comparator.
The old global boundary check still differs in historical evidence text at
0x401100; no broad regeneration or checker relaxation is adopted.

## Reproduced behavior and compiler context

The existing ScriptViewerCreate graph, support order and mixed `/GL` profiles
are unchanged. Source uses the maintained ASCII and ANM methods directly.
The real bodies let LTCG select the observed EDI browser, ESI/EBX marked-text
arguments, and ESI VM plus stack renderer at DrawNoRotation. No assembly,
register bridge, source padding or code-segment pragma is introduced. The MIT
reference at c6791d82dc9f02e55eca45a5314f900a85481efd supplied a source-shape
hypothesis; target bytes and local native comparisons establish these results.
Original source names, classes and production TU/profile ownership remain unknown.

Every state first writes `SprtView\n`. State 1 emits File, Ecl, Quit and the
menu marker. State 2 emits the selected filename in 0xFFFF4040 and returns.
State 3 emits integer-truncated X/Y in 0xFFA0A0A0. State 4 emits active enemy
count and returns. Other states return after the title. States 1 and 3 reset
the currently published ASCII manager to white, then draw the embedded VM only
when `flags & 2` is nonzero. Its position becomes (320, 240, 0), and the backend's return
value is ignored. The maintained `vm.position` offset 0x334 explains the
browser's target stores at 0x5FC..0x604.

The host fixture extracts the actual Draw and callback bodies. It checks all
state exits, missing files and enemy manager, selected names, integer formatting,
menu coordinates, colors, VM identity/position and ignored backend return.
Synchronous text sinks replace published ASCII/enemy/render managers and change
the draw flag to test the observed reloads and event order. Numeric witnesses
stay finite and within integer range; host execution makes no x87 edge or
Windows renderer/runtime claim.

Four existing complete-owner guards pass both native images: RegisterTasks 106,
Update 1251, AddSmallFormatText 100 and DrawNoRotation 515. The two renderer
units use temporary same-image selectors; their canonical recipes remain
unchanged. The new owners share the existing canonical browser output graph.
No full historical native cohort is replayed. Standard repository CI passes.

```sh
scripts/repo-python scripts/build-match-unit.py --unit script-viewer-draw
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-draw --json
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-draw-callback --json
scripts/repo-python scripts/test-script-viewer-draw.py
```

All nine reviewed authored ScriptViewer owners now have source. The browser
412-byte destructor remains nonexact, as do the previously documented larger
manager destructors and RebuildPlayerOptions. Product storage owners, complete
Windows i386 build/runtime, semantic reconstruction and portability remain open.
