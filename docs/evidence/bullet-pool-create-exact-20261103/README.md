# TH10 BulletManager factory: all 182 bytes exact, real destructor + allocation EH

This checkpoint closes the **complete** original Japanese 1.00a factory
`0x00406060..0x00406115` (182 bytes, plain `RET`) with the maintained
`CreateBulletManager()` in `src/BulletPoolCreation.cpp`. Both original PE
identity and function extent were directly verified. The same source already
provides a **separately exact** 163-byte `BulletManagerView::Initialize` at
0x00405E20. Their combined 345 target bytes are exact in separate canonical
VC7.1 compilation graphs. Neither proves the retail executable runs as a
whole, nor the true source's original filename or spelling.

## Complete native behavior, not a small-leaf substitute

The factory allocates the full **0x3E0B54-byte** projectile manager using
`operator new` (0x00452493), invokes compiler vector construction of all
**2,001** embedded `BulletRuntimeView` elements (`0x7F0` bytes each) through
real constructor **0x00405D00**, real destructor **0x00405DE0** and CRT
vector-constructor helper **0x0045252D**, then clears the entire allocation
with 0xF82D5 dwords. It publishes the real global pointer **0x004776F0**,
calls the 163-byte `Initialize`, and returns its owner on success. On
failure the original calls the real **236-byte** `BulletManagerView`
destructor (0x00405F70) with a stack argument, then scalar
`operator delete` (0x004524A1), returning NULL. No guards/loops/padding or
synthetic calling convention are added in source.

## The missing compiler context was the original destructor

The four-source `/GL /GS /EHs` graph using `BulletPoolCreation.cpp`,
`Chain.cpp`, `AnmManager.cpp`, and `GameErrorContext.cpp` reproduced
**183** bytes vs native **182**; at the error path it generated
`MOV ECX, ESI` followed by a thiscall destructor. That form is not the
original, which uses `PUSH ESI` followed by the native 0x00405F70 callee
that consumes one stack argument (`RET 4`).

Adding the **actual unchanged** `src/ScriptViewerLifecycle.cpp` as another
`/GL` input exposed the real `BulletManagerView` destructor to VC7.1. The
compiler then chose the target's private caller/callee transport naturally,
without annotations, assembly, binary edits, or fake stubs. In that genuine
five-source graph the entire factory becomes **182/182** bytes with
**130/130** ordinary comparable bytes, **13** target-address fields and
no instructions or branch lengths left unmatched. Target-bound fields:

- `+0x03`: generated allocation-EH handler at **0x004658AB**;
- `+0x09,+0x11,+0x9C,+0xAE`: the native SEH-list pointer words;
- `+0x1D`: true scalar allocation at 0x00452493;
- `+0x38,+0x3D`: the two actual BulletRuntime destructor/constructor
  descriptors at 0x00405DE0/0x00405D00;
- `+0x50`: the compiler vector-constructor helper 0x0045252D;
- `+0x61`: the global bullet manager at 0x004776F0;
- `+0x76,+0x85`: real Initialize/destructor calls 0x00405E20/0x00405F70;
- `+0x8B`: scalar delete 0x004524A1.

Two independent zero-reuse cold builds using `CreateBulletManager` as the
real LTCG entry produced different PE SHA-256s and PDB GUIDs. Both passed the
strict **full 182/182, 13/13** canonical byte/field comparator, with receipts
`.analysis/gpt-web/20261103/bullet-pool-create-cold{1,2}.json` (local,
regenerable). No normalized or unbound bytes are omitted from the result.

## Exception-handler protocol is independently verified

The factory's PDB-owned generated EH handler shares its parent source module.
The existing opt-in allocation-EH verifier checks the **entire 21-byte**
region: a real 11-byte cleanup action at original 0x004658A0, loading the
saved allocation pointer from `[EBP-0x10]` and calling the real scalar
delete, followed by a 10-byte dispatch at **0x004658AB** to
`___CxxFrameHandler` at 0x0046276D. The retail handler loads immutable
`FuncInfo` at **0x0047189C**, with one unwind action named by the map at
**0x00471894**. The compiler output's PDB contribution, code and read-only
metadata must obey this same fully parsed protocol. This proves the handler
*address binding* for factory codegen, not runtime exception behavior or
an independently exact generated-helper function.

The 2001-entry `BulletRuntimeView` **constructor** at 0x00405D00 remains a
separate, currently source-absent/nonexact owner even though its target
address is bound by the factory. The already matched 236-byte manager
destructor remains an independently accepted canonical function. Only
selected affected exact units were replayed; no complete cold product replay.
