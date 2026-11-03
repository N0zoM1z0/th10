# TH10 FPS/replay timing counter creator — complete nonexact source

The reviewed authored 151-byte target at 0x00413350..0x004133E6 was absent
from maintained source. It is now `CreateFpsCounter()` in the *existing*
`src/Main.cpp`, alongside the true typed `FpsCounterView`, `CalculateFps`,
`DrawFpsCounter`, registered callback, and destructor. No parallel opaque
shadow class or duplicated global allocation was invented.

Direct original disassembly and Ghidra establish a 0x8C-byte allocation,
full 0x23-dword zero fill, setting owner flag bit 2, and publication at the
actual replay timing global 0x00477708. It constructs a 0x24-byte
`th10::ChainElem` by its real default constructor, sets a callback to
0x00413690 and its owner at ChainElem+0x20, ORs flags 3, registers the
node on draw chain priority **47** through the real 0x00449B70 method, and
stores the node at owner+0x0C. The function returns the owner, and retains
the original unchecked child allocation behavior; it does not call the
higher-level `Chain::AllocElem` in place of that actual constructor.

The initial pinned VC7.1 scoped `Main.cpp + Chain.cpp` `/GL` entry
diagnostic produced **145/151**, 39/127 comparable ordinary bytes. A new
independent, natural first-allocation/result pointer split emitted exactly
**151/151** PDB bytes, **109/127** ordinary bytes, and the same six candidate
fields. Neither is exact. This is **source-present NONEXACT**; no
`config/matches.csv` row or new byte credit. The calling site's separate
complete 186-byte startup function is exact in its own three-source graph,
which does not prove this 151-byte dependency. The original TU/name,
product/runtime behavior and source-level constructor origin remain open.
Private probe log: `.analysis/gpt-web/20261103/fps-constructor-first.log`.

## 2026-11-03: source-level allocation/result lifetime improvement

The first baseline used a single `counter` pointer across allocation, the
nullable zero fill, global publication and ChainElem registration, which
made VC7.1 immediately allocate EBX to the first `new` return and emitted
**145/151**. Direct original disassembly shows a *distinct* first allocation
pointer in EDX and a conditional transfer to the final return object in EBX.
The maintained C++ now expresses separate `allocated` and `counter`
variables, with a real null branch. This preserves behavior and naturally
restores the original **151-byte complete extent** without assembly or
register forcing. Strict diagnostic of the entire linked contribution yields
**109/127 ordinary bytes matching** with six candidate address fields.
The remaining 18 ordinary differences stem from the compiler swapping the
roles of its zero register and returned owner (`EBX` and `EBP` vs original
`EBP` and `EBX`), not from missing VM or task registration operations.

Three further genuine VC7.1 `Main.cpp + Chain.cpp`/GL experiments
(predeclaration of result, separately named Chain receiver, and copied
result lifetime) remained 151/151 and 109/127; a prior eager-null-result
variant regressed to 149/151. With real `AnmManager.cpp` support the same
151/151,109/127 result is retained. The rejected alternative source forms
were restored byte-for-byte. Private trial receipts under
`.analysis/gpt-web/20261103/fps-natural-variants-results.json` and
`fps-second-results.json` preserve all sizes/differences. Existing FPS
calculation, drawing, and teardown exact units are cold-replayed only in
their affected scope. This 151-byte owner is still **NONEXACT**, receives
no canonical match ledger credit, and has no full game runtime proof.
