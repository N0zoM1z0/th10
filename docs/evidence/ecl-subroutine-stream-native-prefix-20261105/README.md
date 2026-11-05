# TH10 ECL serialized subroutine prefix: recover the first-instruction offset

Verified original: Japanese TH10 v1.00a executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original **136-byte** `EclVmScriptDatabase::FindSubroutine @ 0x00450470`
returns a pointer into a named subroutine. Its complete target disassembly,
including the successful return at owner `+0x80`, establishes an `ADD EAX,
0x10` after loading the selected subroutine header pointer. The native SCPT
loader separately stores each header address from the serialized subroutine
offset array. These facts establish the first instruction at **header+0x10**,
not a direct pointer to an ECL instruction or a variable-length arbitrary
C++ array start.

`EclVmSubroutineStreamView` now models this target-observed on-disk region:
a **16-byte opaque prefix** plus a one-element first-instruction view
(`EclVmInstruction firstInstruction[1]`). The actual instruction sequence
remains variable-length; this view is not a heap allocation, a fixed-length
subroutine, a guessed header schema or evidence for the original source
symbol. A compile-time `offsetof` assertion binds `firstInstruction` at
+0x10. The sorted binary-search leaf now derives the original final pointer
addition from this C++ layout instead of a duplicated magic constant.

## Original-target verification (complete bodies)

A new pinned VC7.1 SP1 `/GL` **two-source** compile of the installed
`EclVm.cpp` and real `Enemy.cpp` support reconstructs these complete PDB
contributions:

- `EclVmContext::Run @0x0044E1A0`: **7,020 candidate bytes**, **189**
  independently original-target-bound fields and all local/table destinations.
  The original-target full comparison is **6,997/7,020**, with the same
  **23** previously documented ordinary-byte mismatches. Still NONEXACT;
  no inferred code/data bytes were masked beyond their real field roles.
- `EclVmScriptDatabase::FindSubroutine`: **136-byte candidate**, zero
  external fields; canonical selected original-target cold replay **136/136
  exact**.
- `EclVmScriptDatabase::AddScriptData`: **570-byte candidate**, two linked
  fields; the original target is **589 bytes**, still NONEXACT.

Explicitly selected original-bound cold replays on the changed source:
`ecl-vm-find-subroutine` 136, `ecl-vm-read-int` 144 and
`ecl-vm-host-run` 146 — **426/426 previously accepted target bytes**.
There is **zero new exact credit**. Target size, field bindings, ordinary
byte discrepancies and source pins are machine-readable in
`native-result.json`. Ignored selected native PE/MAP/PDB witnesses are at
`build/gpt-web/20261105-ecl-subroutine-stream/full-run/`.

The whole historical exact cohort was not rebuilt. Exact native codegen
of these selected owners does not establish a runnable full game, runtime
semantic equivalence or original source spelling.
