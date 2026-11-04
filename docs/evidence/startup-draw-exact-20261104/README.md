# TH10 StartupWorker::Draw — complete 220-byte canonical exact owner

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete physical **220-byte** `StartupWorkerView::Draw` is
`0x0041FDD0..0x0041FEAB`, with a stack receiver, ordinary `RET 4`, seven
real external address/callee fields and no owner padding omitted.
It allocates the startup signature ANM VM, sets render layer 15 and bit-30,
initializes the loaded script, registers that VM and records its ID at owner
`+0x3DC`. A separate text-ready arm creates screen VM script 6 from the
existing ASCII `AnmLoadedView` at (480.0f, 392.0f, 0.0f), stores its ID at
ASCII `+0x89A4`, updates the ready states, advances frames and returns one.
The program's original source spelling, producer TU, whole-product runtime
and portability remain unproven; exact codegen is a separate bounded claim.

## Natural C++ change and causal evidence

The first source correction introduced a real scoped `AnmVmIdView resultId`
for `AddVmVariant0(created)` before reading `.value`. That preserved the
original C++ behavior while selecting the complete 220-byte PDB contribution
(the old expression emitted 218 bytes) and matching 216/220 original bytes.
The **new** correction in `src/Startup.cpp` also obtains
`AnmLoadedView *const loaded = g_AsciiManagerView->asciiAnm` inside the
second VM's *existing zero-ID branch* and calls
`loaded->CreateVmAtScreenVariant0(6, &position).value`. This is an ordinary
alias of the real already-existing object, not a fake ESI/EDX receiver or
handwritten instruction. It changes VC7.1's second ANM VM call scheduling
from EDX to the original EAX at four previously differing ordinary byte
positions, without moving any external symbol, branch, or local address.
The branch continues to test exactly the existing ID and has no new
observable side effects.

A completed private native panel actually **rejected** an alternative
scoped `AnmVmIdView nextId` after its PDB expanded to 222 bytes. The
chosen `loaded` pointer candidate was independently compared against the
*entire* original PDB owner, not a matching prefix, and yielded zero
ordinary-byte differences after binding the correct seven original fields.
All input TUs were unchanged except the two real source-expression lifetimes.

## Strict complete owner and all seven original fields

The genuine source producer is **`Main.cpp` with 18 actual support TUs**
(including `Startup.cpp`), entry `WinMain@16`, pinned VC7.1 SP1
`/GL /GS` WPO with the recorded `/EHs`, `/EHsc` and `/Ob1` support
profiles. The canonical unit is `startup-worker-draw-0041fdd0`; its PDB
contribution comes from the real Startup support module. Both the original
fields and their candidate C++ symbol identities are independent obligations:

| Owner offset | Field | Original target |
| --- | --- | --- |
| `+0x15`, `+0x4C` | `DIR32` ANM render manager pointer | `0x00491C10` |
| `+0x21` | `REL32 AllocateVm` | `0x00449950` |
| `+0x46` | `REL32 InitializeVm` | `0x00449870` |
| `+0x55` | `REL32 AddVmVariant0` | `0x004489D0` |
| `+0x74` | `DIR32` ASCII manager pointer | `0x004776E0` |
| `+0xB3` | `REL32 CreateVmAtScreenVariant0` | `0x00448D50` |

**220/220 complete PDB bytes, all seven actual field identities/positions,
and all ordinary code bytes** pass the strict canonical target comparator.
No linked address, internal branch, trailing instruction or original
physical extent is waived or cropped.

## Independent selected cold proofs and sibling regressions

Two separate complete-source, selected canonical cold builds reproduce exact
codegen and have **different PE SHA-256 and PDB GUIDs**, excluding reuse of
the first compiled image:

| Cold | PE SHA-256 | PDB GUID |
| --- | --- | --- |
| 1 | `18f4bcdc996293ea918ec6cf95b76dafb9991ca31e1a9a0bd8a4861bee3adf1e` | `54220ee32e49cf4e91749a59b02b0826` |
| 2 | `1734ea8739647ccd8c5571e17e73da9fe967f9ea56bbaaab98c94a96f87478ba` | `914eeaf95c8985458ddda2ef3212bd3c` |

The first new cold also selected the previously exact Startup worker
constructor **128/128**, creator **69/69**, and update callback **63/63**,
for **480/480 original bytes** across four distinct complete owners.
`Startup.cpp` SHA-256 was the same before and after the build:
`588f3d6f97c1d941b0ab9b5a54f3d915ec1151d523bc919cab7139cf1a4c33a3`.
Previously accepted 260 bytes are regression checks, **not** counted again.
Only Startup Draw adds 220 newly accepted exact bytes and one function.

Run **only the four affected owners**, never the full cold cohort:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit startup-worker-draw-0041fdd0 \
  --unit startup-worker-constructor --unit startup-worker-create \
  --unit startup-update-callback --json
scripts/repo-python scripts/ci.py
```

Ignored full native PE/MAP/PDB and receipts are under
`.analysis/gpt-web/20261104-next/startup-draw-exact-cold1-guards.json`,
`startup-draw-exact-cold2.json`, and
`startup-draw-second-id-results.json`. No original executable or proprietary
compiler binaries are added to this commit.
