# Exact TH10 ItemManager destructor: 216 bytes, full vector EH binding

The original Japanese TH10 1.00a target at
`0x0041ADF0..0x0041AEC7` has a complete **216-byte**
`ItemManagerView::~ItemManagerView` body, returned with `RET 4`.
Original SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The source is independently classified authored, but the historical TU and
source spelling are not known.

## Repair the actual dependent callee

The old `src/ItemManager.cpp` body removed both update and draw Chain
registrations under the actual Item callback critical section, but it used
the fictional `ItemUnlinkCallbackNode` external and a separate
`g_ItemCallbackChainOwner` shadow name. The original 216-byte TH10 body
directly calls the previously verified real
`th10::Chain::UnregisterElem @ 0x00449F60` twice and reloads the true
`th10::g_Chain @ 0x00491BE4` separately for each removal.

The maintained C++ now calls that real method through the shared Chain
instance; it retains null-node guards, actual
`EnterCriticalSection @ IAT 0x004660B0` /
`LeaveCriticalSection @ IAT 0x004660B4`, increment/decrement of the real
byte lock depth `0x0049231C`, and the original array and global cleanup.
No artificial register variables, inline assembly, byte patches, duplicate
objects, allocation-failure simulator or invented null paths were added.

## Complete native PDB / address / exception proof

A real selected `GameCreateItemManager` producer compiling
`ItemManager.cpp + Chain.cpp + AnmManager.cpp` with pinned VC7.1 SP1
`/GL /GS /EHs` reproduces **216/216 complete ordinary and target-bound
bytes** after all **21 original address fields** are independently verified.
These include the two actual Chain unlink calls, the Chain global, the
critical-section globals and IAT functions, the 34-byte element destructor
`ItemRecordView::~ItemRecordView @ 0x0041AD60`, the global Item manager
`0x00477818`, and the genuine vector-destruction iterator
`0x004525FF`.

The new focused `scripts/linked_item_array_eh.py` checker additionally
resolves the parent-specific static
`__ehhandler$??1ItemManagerView@@QAE@XZ` at the original
`0x0046541C` and validates its unique 38-byte generated PDB
contribution in the *same source module*, comprising:
- 28-byte vector destructor unwind action at `0x00465400` with original
  element dtor, **0x896 = 2,198 elements**, **0x3F0 = 1,008-byte stride**,
  saved receiver `[EBP+4]`, and array base `+0x14`;
- 10-byte handler dispatch to original `___CxxFrameHandler @ 0x0046276D`;
- original immutable one-state FuncInfo `0x004714FC` and unwind map
  `0x004714F4`, with correct predecessor and unwind-action target.

This validation prevents incorrectly crediting the destructor from a
function-size or handler-address coincidence. The new isolated CI stage
`Validate Item array EH address binding` passes **one real target-positive,
one unrelated-byte mutation control and 28 intentional rejection cases**.
The existing Bullet array-EH stage passes independently.

**Two separate selected cold linked PE/PDB builds** match all 216 bytes
and 21 original address fields and have distinct image SHA-256 and PDB GUIDs.
See [native-proof.json](native-proof.json) for both receipts and the full
field/metadata evidence. In addition, six affected older canonical
selectors were cold replayed: prior Item registration 84, callback update 48,
callback draw 29, power display 79, record constructor 104 and destructor 34,
all **378/378 exact**. See [selected guards](selected-guards.json).

To reproduce only the newly accepted function and its matching EH checks:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-manager-destructor-0041adf0 --json
scripts/repo-python scripts/ci.py --only 'Validate Item array EH address binding'
scripts/repo-python scripts/ci.py
```

**Acceptance scope:** full exact authored codegen and generated EH
address/metadata bindings only; not a complete rebuilt TH10 executable,
original compiler TU provenance, semantic runtime equivalence, or
portability. No all-unit cold replay was used.
