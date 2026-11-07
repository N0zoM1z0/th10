# ECL FindSubroutine: indexed native eight-byte record pointer

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EclVmScriptDatabase::FindSubroutine @0x00450470` is an **already exact**
136-byte original-linked function (not a newly accepted function).
The complete target `EclVmContext::Run @0x0044E1A0` has 7,020 bytes and
189 independently original-bound fields; it remains **NONEXACT**.

Binary search indexes records in a serialized, sorted subroutine table.
`EclVmSubroutineEntry` is eight bytes (name and stream header pointers),
and the native i386 `LEA` uses the low-32-bit indexed byte address. The
previous `subroutines[middle]` C++ expression formed a typed array pointer
based on `middle`, which can be out-of-range if a corrupted script table
count drives binary search outside the actual allocation. The installed
source uses `EclSubroutineAtWord(base, middle)` and a single immutable
`mid` pointer in the body of each binary-search iteration, preserving both
record reads and original name/stream header offsets. The helper computes
`(unsigned)base + (unsigned)middle * sizeof(EclVmSubroutineEntry)` before
forming the typed pointer. No added bounds check, clamping, manual register
assignment, new storage or original-byte patch. Out-of-range reads remain
unsafe; this restores the target's Win32 address arithmetic, not portability.

## Complete native codegen evidence

Three independent natural C++ variants were compiled in the actual current
EclVm/Enemy two-TU `/GL` graph: a shared typed pointer helper, direct
unsigned byte-address expressions, and one shared `mid` pointer. **All
three** generated the complete original `FindSubroutine` **136/136** raw
bytes, with no missing/shifted address fields, and preserved the parent
`Run` **6,997/7,020** original ordinary bytes with all **189/189**
original-bound code/data fields and exact `ReadFloatValue` callee.

Separately, five natural `AddScriptData` subroutine-count lifetime probes
were compiled and compared to the full **589-byte** original. They changed
the candidate owner to **605 bytes**, even while preserving `Run`, and
were **rejected**. The installed `mid` helper was then recompiled afresh:
its entire 589-byte `AddScriptData` candidate is unchanged in ordinary
bytes and the two candidate CRT field positions; it remains **nonexact**
(502/581 ordinary bytes vs original, 79 differences). This explicitly
refutes interpreting a shorter source as sufficient for exact acceptance.

The installed-source focused cold replay, *not* the all-unit historical
replay, verifies four previously exact canonical original-bound units:
`ecl-vm-find-subroutine` 136, `ecl-vm-host-run` 146,
`ecl-vm-read-float-value` 171 and `ecl-vm-stack-pop` 122:
**575/575** bytes passed in the affected 1-TU and 2-TU graphs.
No new exact ledger credit, game product build, or runtime claim.

Private native evidence: `.analysis/gpt-web/20261107-turn4/ecl-lookup-word.py`,
`ecl-lookup-results.json`, `ecl-lookup-installed-run.json`,
`ecl-lookup-guards.json`, `ecl-count-lifetime.py`, `ecl-count-results.json`,
`build/gpt-web/20261107-turn4-ecl-lookup/` linked PE/MAP/PDB and
`build/match-linked/EclVmHostRunEnemy/` installed source graph.
