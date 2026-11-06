# ECL name table: counted offset cursor in the parameter home — 2026-11-06

The complete `EclVmScriptDatabase::AddScriptData @450220` now emits **589
bytes**, matching the reviewed original extent. Both CRT call fields are at
their original positions, `+6C` and `+101`. Strict comparison gives **510/589
bytes**, with **two of two original-bound fields**. The owner remains
**NONEXACT**, with zero new exact credit and no boundary or source-count change.

## Source model and original observations

The original function reuses its input parameter's stack slot as the cursor
over the SCPT file's counted four-byte instruction offsets. After the saved
registers are pushed, that parameter home is `[esp+24]`. Its stack frame is
`0x10`; the old table stays in EBP across allocation and its two tests. Its
complete body, including validation/merge/cleanup/package paths, is 589 bytes.
These facts were independently rechecked in the hash-verified Japanese
v1.00a original. The existing virtual slot and header layout are unchanged.

The prior maintained implementation introduced a separate volatile offset
cursor and kept the previous table pointer volatile. Its fresh two-input
baseline emits 585 bytes, with allocation/free fields at `+70/+108` and a
`0xC` frame.

The installed model instead:

- updates the database's subroutine count through a small ordinary `int&`
  helper;
- reuses `scriptData` for offset progression and gives its **loop reads** a
  const volatile reference view of the actual parameter home;
- keeps writes and the initial name-stream calculation ordinary, avoiding an
  extra read immediately after storing the initial offset cursor;
- retains ordinary saved old-table pointers for the copy/initial test and
  conditional free, removing volatility from the old-table pointer;
- expresses the initial nonempty serialized-count guard as an unsigned
  comparison, recovering target `JBE` rather than `JE`.

This recovers the target frame, parameter-slot progression, old-pointer EBP
lifetime, `REP MOVSD` at `+FB`, and both direct call positions. The first
count load/sum/shift scheduling, first-fill loop register homes, selected
merge-tail stores and final file-count update still differ. Pointer views and
volatility are provisional compiler-visible lifetime models of observed
loads, not claims about original declarations or concurrent mutation.

The MIT source at upstream
[`d1f3121`](https://github.com/levzzz5154/th10-decomp/blob/d1f31212514781c928fba145d8207397c8fb5650/matching2/msvc/src/script/sptlib/ecl_name_table.cpp)
suggested the count-reference helper and parameter reuse. Copyright (c) 2026
levzzz; [retained MIT license](../../licenses/th10-decomp-MIT.txt). Its
identity pointer selector, source flags and separate compiler context are not
adopted. Ordinary `_malloc`/`_free` retain their actual CRT cdecl interfaces.

## Bounded native evidence

The existing two-input `/GL` graph is preserved: EclVm.cpp plus Enemy.cpp,
with real `EnemyRuntimeUpdate` entry and pinned VC7.1/linker profiles. A fresh
baseline is followed by changed-TU/nonincremental-link probes with verified
unchanged inputs; the installed source is finally cold-built from both inputs.
Compiler/link jobs are serial, nice10, one CPU and bounded by retained logs.

| Source model | Complete bytes | Allocation/free field offsets |
| --- | ---: | --- |
| Fresh prior source | 585 | `70 / 108` |
| Count reference alone | 585 | `6F / 108` |
| Ordinary parameter cursor and ordinary old-table views | 547 | `6A / F1` |
| Entire cursor volatile, including its initial read/write | 605 | `70 / 10C` |
| Read-only loop view, installed source | 589 | **`6C / 101`** |

Early count snapshots, a named count reference and direct scalar accumulation
regress to 605 bytes. They are restored and parked. A force-inline annotation
does not improve the useful count/cursor model and is also restored. No fake
ABI, padding, copied instructions, test matrix or runtime failure scenario is
used. No incomplete owner or field-position-only credit is accepted.

The final same-image table consumer `FindSubroutine` passes **136/136 bytes**;
its independent canonical cold also passes with explicit
`--unit ecl-vm-find-subroutine`. The generic ECL Run remains **6997/7020**,
with all **189 original-bound fields**, sixty jump words and 88 selectors
compared. Its full 171-byte ReadFloatValue callee/four fields passes as well.
The maintained Run diagnostic handles the shifted image's linear decoder
stopping in jump-table data; complete PE fields plus the reviewed selector
suffix cover every remaining byte. No comparator or canonical recipe changes.

[native-result.json](native-result.json) pins sources, complete owner extents,
original-bound call fields, image/PDB identities, Run and selected-cold results.
Private native sources, disassembly and logs remain under
`.analysis/gpt-web/20261106-ecl-count-reference/`; the final diagnostic image is
`build/gpt-web/20261106-ecl-count-reference/final/`. This establishes current
native measurements, not runtime, product closure or portability.
