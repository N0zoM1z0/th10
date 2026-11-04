# TH10 FrontEnd constant-interrupt adapters: two independent 27-byte exact owners

Original Japanese TH10 v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The disjoint physical owner extents **0x0042C730..0x0042C74A** and
**0x0042C750..0x0042C76A** are **27 bytes each**, no cropped tails,
synthetic filler or shared match credit. They load the real controller
`vmIds[index]` from offset +0x2C4, save EDI, pass the existing
`AnmVmIdView` to `AnmRenderManagerView::SetVmPendingInterruptAndExecute`
with hardcoded interrupt 2 or 3, restore EDI and return. The original
private input registers are **EAX=controller**, **ECX=index**. Authored
source versus compiler-generated adapter and original production TU remain
**indeterminate** despite exact generated code.

## Original callers and natural source reconstruction

The original target's hash-attested, read-only Ghidra call-xref graph
independently finds **2** direct calls to the interrupt-2 helper (both from
`UpdateOptions`) and **7** to the interrupt-3 helper, from
`FrontEndUpdateMainMenuReturn` (3), `UpdateOptions` (1), and
`UpdateDifficulty`, `UpdateCharacter`, `UpdateShotType` (one each).
Only these original direct caller paths use the separate natural C++
`FrontEndInterruptVmNowTwo` and `FrontEndInterruptVmNowThree` functions in
`src/FrontEnd.cpp`; other generic interrupt paths are still generic.

The codegen closure is an ordinary, retained source correction:
`void FrontEndInterruptVmNowTwo/Three(int index,
FrontEndControllerView *controller)` puts the natural index first, while
the existing `FrontEndVmIdStorageView::operator const AnmVmIdView&()`
lets the argument pass as a **real VM-ID value**, instead of a separately
loaded bare `int`. In the real VC7.1 `/GL /GS` frontend UpdateOptions graph,
this selects the original private EAX/ECX register schedule and true
`PUSH EDI; PUSH EDX; MOV EDX,[manager]; MOV EDI,2/3; CALL ...; POP EDI; RET`
instructions. No assembly, forced register variables, synthetic VM owner,
manual byte patch or fake caller is present. The old natural parameter
order yielded 32 bytes with displaced fields, **nonexact**; that result
is preserved as the independent, falsified earlier source control.

## Complete canonical target-bound proof

Both resulting helpers have their **own full 27-byte linked-PE PDB
contributions** in a genuine compilation rooted at
`FrontEndControllerView::UpdateOptions`, with seven unchanged real support
sources. All **19/19 ordinary code bytes** and **two/two** original linked
address fields match *for each function* under the strict comparator:

| Function | Original field | Kind and original target |
| --- | --- | --- |
| `FrontEndInterruptVmNowTwo`, 0x42C730 | `+0x0B` | `DIR32 g_AnmRenderManagerView @ 0x00491C10` |
| `FrontEndInterruptVmNowTwo`, 0x42C730 | `+0x15` | `REL32 SetVmPendingInterruptAndExecute @ 0x00449250` |
| `FrontEndInterruptVmNowThree`, 0x42C750 | `+0x0B` | `DIR32 g_AnmRenderManagerView @ 0x00491C10` |
| `FrontEndInterruptVmNowThree`, 0x42C750 | `+0x15` | `REL32 SetVmPendingInterruptAndExecute @ 0x00449250` |

Both exact owners retain all instructions including the final plain `RET`,
correct field identities, correct field offsets and independent 27-byte PDB
sizes. No artificial normalization masks non-linkage bytes. The actual
ANM callee symbol with `AnmVmIdView` parameter is also the **accepted 76-byte
original callee**, not an invented integer wrapper.

Two **separate selected canonical cold builds** each passed **27/27 exact
for both functions**. The distinct PE hashes and PDB GUIDs exclude reuse
of the first linked image:

| Selected cold | PE SHA-256 | PDB GUID |
| --- | --- | --- |
| 1 | `99647dd4ca23514cec3a25cefc7f243996e5580511638a6a4ea66d021b3e1c6e` | `2e7a4fff912fb4469a903519b8b62865` |
| 2 | `edbe50b644168a10bc235679a2f42314e4453a0ff6b4ec0d28b30dbe5be95a9a` | `a10e9dfdcdecb649a2e1aa3534127e8a` |

Same modified-source canonical replay also retained the **full 3,999/3,999
byte** `frontend-options-refresh` with all its **112 target fields**.
Separately selected cold guards retained `frontend-set-screen-state`
**80/80**, `frontend-set-screen` **83/83**, `frontend-delete-vm`
**83/83**. Thus **4,299/4,299 original bytes across the six distinct
selected owners**, including exactly **54 new** (two new functions), passed
without all-unit cold replay. `src/FrontEnd.cpp` was confirmed unchanged
before and after cold proof at SHA-256
`93cdc15b2a4050e9859db27df6e11096ebf4e8d827af50879dea1f5d0a24a9a5`.

Reproduce only affected owners:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-interrupt-vm-now-two-0042c730 \
  --unit frontend-interrupt-vm-now-three-0042c750 \
  --unit frontend-options-refresh --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-set-screen-state --unit frontend-set-screen \
  --unit frontend-delete-vm --json
scripts/repo-python scripts/ci.py
```

Regenerable original-bound receipts are ignored in
`.analysis/gpt-web/20261104-next/frontend-both-strict-existing.json`,
`frontend-adapters-cold1-guards.json`,
`frontend-adapters-cold2.json`, and
`frontend-adapters-other-guards.json`. This is exact compiled owner proof,
not original authored C++ identity, native executable completeness, in-game
runtime or portability acceptance.
