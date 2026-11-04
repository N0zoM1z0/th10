# TH10 Startup worker Draw — complete 220-byte WPO owner frontier

**Superseded exactness frontier (2026-11-04):** A later, real 19-TU
WPO source correction to the **second** VM's `AnmLoadedView *const loaded`
local now reproduces this complete **220/220** owner and every bound field
in two selected cold replays. This document retains the previous, genuinely
nonexact 216/220 stage as historical negative control. See
[new canonical evidence](../startup-draw-exact-20261104/README.md).


Original target is the pinned Japanese TH10 v1.00a executable, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The physical **220-byte** `0x0041FDD0..0x0041FEAB` member body draws the
startup signature and text, allocates the ANM VM, marks its flags/layer,
initializes the loaded script, registers the VM, stores its ID into
`StartupWorkerView::signatureVmId` at +0x3DC, advances both ready states,
and ticks the worker frame counter. The original member has a stack owner
and `RET 4`; original source spelling/TU remains unknown.

## A real return-by-value source improvement

The former maintained `src/Startup.cpp` expressed the first ANM VM ID call as
`signatureVmId = g_AnmRenderManagerView->AddVmVariant0(created).value;`.
A direct, independently repeated VC7.1 SP1 `/GL /GS /EHs` WPO experiment
under the **real `Main.cpp` entry `WinMain@16`** and **18 support TUs**
found that an ordinary scoped `AnmVmIdView resultId =
AddVmVariant0(created)` followed by `signatureVmId = resultId.value`
preserves the original observable call and assignment but restores the full
**220-byte native PDB contribution**. The historical direct-expression body
was only **218/220** in this selected producer. The 220-byte native candidate
has the genuine target `AddVmVariant0` call and hidden ID return storage,
with the original branch placement and no invented ABI, byte padding,
synthetic inputs, assembly, or dropped instructions.

A strict original-target comparator — **all 220 bytes, seven independently
bound external fields, no crops** — reports **216/220 byte-exact after
normalization** and four ordinary byte differences at offsets
`+0xA1`, `+0xAD`, `+0xB8`, `+0xBA`: target reuses EAX through the
second CreateVmAtScreenVariant0 path where the source uses EDX. It is still
**NONEXACT** and has **zero new match credit**. The target's exact
callee/calling-convention identities in the seven fields are:

| Offset | Binding | Original target |
| --- | --- | --- |
| `+0x15`, `+0x4C` | ANM render manager global | `0x00491C10` |
| `+0x21` | `AllocateVm` | `0x00449950` |
| `+0x46` | `AnmLoadedView::InitializeVm` | `0x00449870` |
| `+0x55` | `AddVmVariant0` | `0x004489D0` |
| `+0x74` | ASCII manager global | `0x004776E0` |
| `+0xB3` | `CreateVmAtScreenVariant0` | `0x00448D50` |

Two independently linked alternate natural shapes (direct constructor
initialization and const-reference return) both emitted a complete 220-byte
contribution at 188/192 preliminary ordinary equal bytes; the retained
first version was additionally checked with the strict seven-field
comparison. An experimental deferred *assignment* to `resultId` compiled
as only 218 bytes and 70/192 ordinary equal, and was explicitly **not**
retained. That unsuccessful attempt left a dirty source file after a
compiler timeout, which was inspected rather than discarded; the retained
source file was checked byte-for-byte against the successful artifact.

## Focused guards in the changed source graph

The retained source SHA-256
`91c4dab69877b4e3f7e6d17c170fcaf2783e9b63dd4073baab5d08f9463b15eb`
was unchanged **before and after** selected canonical cold replay of the
real Startup graph. All previously accepted affected units pass:
`startup-worker-constructor` **128/128**, `startup-worker-create` **69/69**,
`startup-update-callback` **63/63**, **260/260 exact** in total. These
are regression guards, *not new credits* for the still-open Draw owner.

Private experimental receipts (ignored, never committed as original game
artifacts): `.analysis/gpt-web/20261104-next/startup-draw-strict-named-result.json`,
`startup-dirty-verified.json`, `startup-draw-source-selected-guards.json`;
`build/gpt-web/20261104-next-startup-draw-return/named-id-value/` retains the
actual PE/MAP/PDB source graph. A complete native product build and runtime
behavior/portability proof remain outside this comparison.

Reproduce only affected accepted owners, never all-unit cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit startup-worker-create --unit startup-worker-constructor \
  --unit startup-update-callback --json
scripts/repo-python scripts/ci.py
```
