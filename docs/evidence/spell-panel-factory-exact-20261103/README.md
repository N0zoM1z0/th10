# TH10 136-byte spell-panel factory: canonical linked-PE exact

Target hash: original Japanese TH10 v1.00a, verified with
`config/target.toml`. Complete original owner
`0x00408C90..0x00408D17` is **136 bytes**, and ends with plain `RET`.
It allocates a `0x37B0`-byte `EnemyGameStateView`, calls the existing
full VM-array constructor at 0x00408910, registers three tasks through the
211-byte method at 0x004089C0, and either returns the live object or
invokes the real 402-byte destructor and `operator delete` on failure.
It does not add an artificial allocation-failure fallback, skip error
paths, or replace C++ EH with a host simulation. Source resides in
`src/EnemyGameStateLifecycle.cpp`, already mapped by a previous checkpoint.

## Fresh compiler-context recovery

The pinned VC7.1 SP1 `/GL /GS /EHsc` *isolated* lifecycle compiler graph
emitted 136-byte PDB extent with **87/96 comparable ordinary bytes**.
Nine residual non-address instruction bytes were EBX/ESI register encodings
inside the owner-lifetime and teardown branch. Matching the size was not
accepted as exact. An independent graph with the real, unchanged
`src/Chain.cpp` as the sole `/GL` support input made the compiler see
native `Chain::AllocElem`, `AddToCalcChain` and `AddToDrawChain` private
callers, naturally keeping the panel owner in **EBX**, as in the original.
No artificial register moves, inline assembly, padding, symbol aliases or
source branches were introduced into the maintained function.

Two fresh zero-reuse cold builds of the separate canonical
`enemy-spell-panel-create` unit, both with actual `CreateEnemyGameState`
as link entry, reproduce the **full 136/136 PDB-owned bytes** exactly after
binding **10 of 10** independently reviewed source/target address fields:
the EH handler, four FS exception-list operands, `operator new` at
0x00452493, real constructor at 0x00408910, registration at 0x004089C0,
real destructor at 0x00408AF0 and `operator delete` at 0x004524A1.
The complete ordinary 96/96 bytes and branch destinations match.
Both builds have different PE SHA-256 and PDB GUID identifiers. Strict
source/candidate/target JSON comparison receipts are retained under
`.analysis/gpt-web/20261103/spell-factory-cold{1,2}.json`.

## Independent EH handler identity

The existing opt-in `allocation_eh_handler` verifier, without code changes,
validates the candidate generated **21-byte** code region sharing the
factory's PDB module. Both candidate and retail handlers have a single
11-byte cleanup action `MOV EAX,[EBP-0x10]; PUSH EAX; CALL scalar-delete;
POP ECX; RET`, and 10-byte `FuncInfo`/`___CxxFrameHandler` dispatch.
Target handler **0x0046586B** loads the immutable 28-byte `FuncInfo` at
0x00471854, whose one-state unwind map at 0x0047184C points to the
actual action at 0x00465860. The real scalar delete target is 0x004524A1.
The proof also checks PDB-generated contribution/provenance and the
saved allocation slot; it grants an EH handler *address binding*, not
runtime correctness or generated-helper exact-function credit.

The source declaration and original product TU/callback ABI origin remain
unknown. The constructed object's destructor 0x00408AF0 still emits a
nonexact 386-402 byte candidate depending on compilation context; this
factory's exact REL32 call-site is not a claim that destructor itself
matches. This owner adds **136 authored exact codegen bytes** in the
reviewed-source classification without claiming finished game reconstruction.
