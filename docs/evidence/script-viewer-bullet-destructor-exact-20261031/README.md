# Bullet manager destructor — complete 236-byte native exact, including EH binding (2026-10-31)

The complete authored `BulletManagerView::~BulletManagerView @ 0x00405F70..0x0040605B` now passes **236/236 linked-PE bytes and 23/23 bound fields** in the authentic Japanese TH10 1.00a target, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Its reviewed extent, source mapping, natural C++ destructor and original game executable remain unchanged. **Exactly one new authored owner/236 bytes** is admitted; *no separately generated EH code or callee code is credited*.

## The source graph is real; the EH helper is separately reviewed

The normal destructor in `src/ScriptViewerLifecycle.cpp` unregisters its update and draw tasks under the actual `th10::Chain` lock and nested counter, invokes the genuine ANM manager `MarkLoadedVmsForDeletion` on the bullet ANM resource, clears `g_ScriptViewerBullets`, and has the source's real exception-protected array member cleanup. A controlled, faithful 3-source graph starts from the actual `ScriptViewerReleaseBullets` caller and includes both `src/Chain.cpp` and `src/AnmManager.cpp` as independently compiled support sources. Omitting ANM from this graph changes the destructor's length to 238, so the preexisting 14-input source-context probe and standalone probes cannot be used as the canonical proof. The correct VC7.1 SP1 `/GL /GS /EHs` graph produces exactly **236 PDB-owned bytes**, 23 relocation/control fields, the genuine Chain callee, and an ANM deletion call; no C++ ABI, compiler flags, target bytes or dummy source calls were changed.

Among the 23 original fields are three separate `__except_list` zeros (`+0x02/+0x11/+0xE1`), the generated EH dispatch address at `+0x09`, two `g_Chain` fields, both Windows critical-section imports, eight `g_MainSupervisorView` fields (real base `0x00491C28` plus `0x64C` or `0x6F4`), two actual `Chain::UnregisterElem @0x00449F60` REL32 calls, the ANM owner `g_AnmRenderManagerView @0x00491C10`, its `MarkLoadedVmsForDeletion @0x004493E0` REL32, the array-element `BulletRuntimeView` destructor target `0x00405DE0`, the player-script bullet global `g_ScriptViewerBullets @0x004776F0`, and vector destructor iterator `0x004525FF`. All typed identities and exact offsets are in `script-viewer-bullet-destructor` in the canonical TOML. A typed original target callee address is not the same as a separate complete callee reconstruction.

The EH target `__ehhandler$??1BulletManagerView@@QAE@XZ` is **not** an arbitrary relocated address to waive. A narrowly scoped `scripts/linked_bullet_array_eh.py` verifies, against the original executable and *the new native linked image*, all of the following:

* The handler is one unaliased named MAP static and lives in a **38-byte unique PDB contribution of its very same parent module**: 28-byte cleanup action, 10-byte handler dispatch.
* Handler dispatch is `mov eax, FuncInfo` / `jmp ___CxxFrameHandler`; the target uses original `0x0046276D`. FuncInfo occupies immutable readable `.rdata`, is exactly one-state (0x19930520, state count 1, other fields 0), and its unwind row is `predecessor=-1`, `action=handler-28`.
* The complete action pushes original target `BulletRuntimeView` element destructor address `0x00405DE0`, array count **2001 (`0x7D1`)**, element stride **2032 (`0x7F0`)**, loads saved `this` from `[ebp+4]`, adds **`0x60`**, invokes original vector destructor iterator `0x004525FF`, and returns.
* The identical structure is checked independently in the generated candidate image, including actual PDB module ownership, immutable data sections, one-state metadata, action/dispatch instruction boundaries and real compiler-generated callee identity. Such binding **does not** assert the `BulletRuntimeView` destructor itself is exact, or grant product/runtime EH semantics; the latter source body is independently unresolved.

The handler selector is permitted **only** for this reviewed 236-byte parent, fixed offset `+0x09` and original handler `0x0046553C`, and cannot be mixed with existing EH/static selectors. The negative-control test `scripts/test-linked-bullet-array-eh.py` uses the actual TH10 and local PE/PDB, passes one native/target positive plus a non-EH neighbor-byte control, then rejects **28** incompatible mutations: wrong owner/field, MAP/PDB ownership, handler entry, unwind state or action, count, stride, element destructor, saved-this/member offset, iterator/frame target, aliases, metadata mutability, etc. An incomplete or guessed EH linkage cannot pass merely by masking four bytes.

## Two independent *selected* native cold passes

Round one selects only the Bullet destructor: full **236/236 + 23 fields**, PDB GUID `2f114250190b334a98d1d3bc85dec1d3`. Round two independently recompiles Bullet and also the previously accepted 137-byte Bomb destructor in its own 2-source graph: Bullet **236/236** and Bomb **137/137**, a total **373/373** across the two *separate* scoped producers, Bullet PDB GUID `40a58cb900d3964e945167209c1defd8`. Full receipts are in ignored `.analysis/gpt-web/current-turn/bullet-cold-{1,2}.json`. This is not a historical or whole-game cold replay; no partial padding or generated-helper exact credit is admitted.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/ci.py --only 'Validate Bullet array EH address binding'
scripts/repo-python scripts/replay-exact-units.py --unit script-viewer-bullet-destructor --json
scripts/repo-python scripts/ci.py
git diff --check
```

The exactness claim is bounded to the whole parent codegen, fixed original fields and reviewed generated-handler address protocol. Original physical TU attribution, independent element destructor source, data ownership, C++ exception runtime safety, final product EXE and portability remain separate.
