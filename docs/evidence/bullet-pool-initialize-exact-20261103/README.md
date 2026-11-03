# TH10 bullet-pool initialization: exact 163-byte owner

This checkpoint recovers and closes `0x00405E20..0x00405EC2` as
`BulletManagerView::Initialize` in the new maintained
`src/BulletPoolCreation.cpp`. The original Japanese 1.00a target is pinned
by `config/target.toml`. The surrounding `CreateBulletManager` at
`0x00406060..0x00406115` (182 bytes) is also now mapped to complete
semantic C++ **but remains nonexact**. It is not credited in
`config/matches.csv`, and its native constructor/destructor/EH ABI source
provenance remains open.

## Complete original entry and source recovery

Direct native disassembly and hash-attested Ghidra source analysis independently
confirm the following complete 163-byte `RET` function:

- The receiver is in private **EBX**. It asks the global renderer at
  `0x00491C10` to `PreloadAnm(7, "bullet.anm")` through the real original
  callee `0x00447280`, and stores the result to owner `+0x3E0B50`.
- On failure the same real `th10::GameErrorContext` at `0x00474F70`
  logs the original error format at `0x0046CD54` via the native
  `0x0044B810` Log owner, returning -1. This path does not register tasks.
- On success it sets the bullet cursor +0x10 to the actual manager's
  2001-entry pool +0x60, and sentinel `bullets[2000].state` at
  `+0x3E07A6` to 5 (short write).
- It allocates an update task with callback `0x00406770`, masks task flag
  bit 1 off, saves this as ChainElem +0x20, and registers it in the
  real calc chain at **priority 20**. The resulting task is saved at owner
  +8. It similarly registers draw callback `0x004067A0` in the draw
  chain at **priority 29**, stores its task at +0xC, and returns zero.

The containing type, callbacks, array dimensions and original
`0x3E0B54`-byte allocation are already supported by maintained
`BulletManagerView`, `BulletRuntimeView` and `ScriptViewerLifecycle.cpp`.
The new source publishes that existing native global as
`g_ScriptViewerBullets`; it does not introduce a second bullet manager
instance or change the existing destructor body. The class name/physical
TU, exact original C++ spelling and runtime behavior remain provisional.

## How the exact source graph was found

An initial `src/BulletPoolCreation.cpp`-only VC7.1 SP1 `/GL /GS /EHs`
link produced **152/163** for the initializer. Adding real `src/Chain.cpp`
yielded 160/163. Adding real `src/AnmManager.cpp` yielded **163/163** and
105/107 ordinary byte equality, with the remaining two bytes encoding
`GameErrorContext::Log`'s wrong stack-based vs native ECX receiver and
caller cleanup length. Including the unchanged real
`src/GameErrorContext.cpp` source in the same LTCG graph resolves both
and produces the full 163/163 exactly, ordinary **107/107**.

The canonical `bullet-pool-initialize` unit uses the actual Bullet factory
as entry and these three real supporting source files, no synthetic caller,
manual register annotations, padding, binary patch or toolchain change.
The strict comparator binds **14/14** original address/control fields:
PreloadAnm+real path, resource-error string, Log/global error context,
update/draw callback addresses, both Chain::AllocElem and both Chain insert
calls, two g_Chain references, and the global ANM manager reference.
All calls agree with original targets and every byte is compared.

Two independent zero-reuse cold links with distinct PE SHA-256 and PDB
GUIDs reproduce all 163/163 bytes/all14 fields. Full comparison reports:
`.analysis/gpt-web/20261103/bullet-pool-cold{1,2}.json`
(private build evidence). The target is **codegen exact**, not a product,
original-TU, global-ownership or runtime execution proof.

## Larger factory follow-up (historical nonexact controls)

`CreateBulletManager @ 0x00406060` allocates the full **0x3E0B54** bytes,
constructs 2001 original BulletRuntime records using compiler vector
construction, zeros the whole object and publishes the global pointer
`0x004776F0`, then initializes the new owner. On registration or resource
load failure, it invokes the separate existing full BulletManager destructor
and scalar delete before returning null. The first anchored candidate
emitted **179/182**, while the full real-support graph emitted **183/182**
and did not preserve the complete factory byte schedule. The factory has **subsequently become exact** with the additional, real
`ScriptViewerLifecycle.cpp` support source (182/182, all13 fields, strict
21-byte allocation-EH handler): see
[factory closure](../bullet-pool-create-exact-20261103/README.md).
The generated BulletRuntime constructor later closed independently at
215/215 raw-exact; see [constructor evidence](../bullet-runtime-generated-constructor-exact-20261103/README.md).

Only explicitly selected related exact units should be cold-replayed;
never use an entire-product or full-historical cold run for this checkpoint.
