# Full TH10 game owner creator 86/86 exact — 2026-11-02

The authored `GameCreateOwner40AF90` entry at original Japanese 1.00a
`0x0040AF90..0x0040AFE5` is **86/86 codegen exact**. The complete
VC7.1 SP1 `/GL /GS /EHs` linked PE in the four-real-source
GameManager/Chain/AnmManager/GameErrorContext graph yields an **86-byte PDB
section contribution**, all 86 bytes equal after strictly replaying **five
independently bound address fields**. Capstone decodes all 86 bytes and fixed
local control-flow branches match the original. No cropped prefix, patched
bytes or fabricated return path participates.

The five real fields are: operator new `??2@YAPAXI@Z` at +0x04 (retail
`0x00452493`), `g_GameOwner4776FC` at +0x28 (`0x004776FC`), actual
`GameOwner40AF90View::Initialize` at +0x33 (`0x0040AE70`), actual
`GameCleanupOwner40AF00` at +0x41 (`0x0040AF00`), and operator delete
`??3@YAXPAX@Z` at +0x47 (`0x004524A1`). The PDB symbol is
`?GameCreateOwner40AF90@@YAPAXXZ`; the recipe makes no assumption about
original physical TU name.

## Source and codegen recovery

The first control (default `new` and a manual `memset`) generated 83 bytes,
with the owner copied to EBX before target's EAX-based constructor stores.
Switching to value-initializing `new GameOwner40AF90View()` recovered the six
consecutive DWORD stores but initially generated 90 bytes, because VC7.1
synthesized a load/OR/store for the owner flags. A scoped compiler
`_ReadWriteBarrier` between value initialization and flag update induces the
retail direct `OR DWORD PTR [EAX],2` and restores an 86-byte *nonexact*
intermediate. Keeping the initialized pointer as the explicit merge result
of success/null allocation branches then generates the retail exact EAX-to-EBX
copy, immediate short jump, null-path EBX zero and the complete original
failure cleanup. The barrier models observed compiler scheduling and does
not prove the original C++ contained this intrinsic or a threading contract.

## Selected actual cold replay

The local canonical selected cold replay combines the creator and its exact
initializer in the same four-source image; the independently selected
GameManager/Chain cleanup graph is also cold-built. Their complete receipts:

| Owner | Original size | Replayed exact | Fields |
|---|---:|---:|---:|
| Creator `0x0040AF90` | 86 | 86/86 | 5 |
| Initialize `0x0040AE70` | 134 | 134/134 | 14 |
| Cleanup `0x0040AF00` | 137 | 137/137 | 15 |

All three return `exact`; 357/357 bytes total across two selected artifacts.
The historical whole-game cold replay is not run. Other game loop/ANM/ECL
parent functions remain nonexact, and this exact codegen does not prove
original-source ancestry, product/image closure or runtime behavior.

Reproduce:

    scripts/repo-python scripts/verify-target.py
    nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit game-owner-40af90-create --unit game-owner-40ae70-initialize --unit game-cleanup-owner-40af00 --json
    scripts/repo-python scripts/ci.py
    git diff --check
