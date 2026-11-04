# TH10 ItemManager callback registration: 84/84 exact in genuine Chain graph

The attested Japanese TH10 v1.00a target (`resources/th10.exe`, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`)
contains the **complete 84-byte** `ItemManagerRegisterCallbacks` owner at
`0x0041AD90..0x0041ADE3`. The original enters with the manager in private EBX,
and registers real update and draw callbacks at priorities 21 and 25. It returns
zero with plain `RET`. This is a codegen exactness proof, not original symbol
spelling, source-TU provenance, a full-game image, or runtime equivalence.

## Actual source/dependency correction

The maintained `src/ItemManager.cpp` previously used three unrelated, unresolved
extern placeholders (`ItemCreateCallbackNode`, `ItemRegisterUpdateNode` and
`ItemRegisterDrawNode`) that did *not* reproduce the actual original call
identities. Direct target disassembly gives two calls to `Chain::AllocElem`
`0x00449ED0`, `Chain::AddToCalcChain` `0x00449AE0`, and
`Chain::AddToDrawChain` `0x00449B70`. These are existing, independently mapped
TH10 `Chain` methods; no new fake ABI declaration or target bytes are used.

The source now includes `Chain.hpp`, calls those real methods, preserves the
original `ItemManagerView::OnUpdate` `0x0041BA00` and `OnDraw` `0x0041BA30`
callbacks, clears each chain element's bit 1, publishes the manager at the
existing element +0x20, and stores the two chain nodes at manager +8/+0xC.
Genuine `Chain.cpp` and `AnmManager.cpp` support with the real
`GameCreateItemManager` caller naturally recover the original EBX receiver,
EDI priority and ESI element lifetime. A narrower isolated build had 93
bytes and a two-TU Chain build had 95 bytes; only the genuine selected
three-TU caller graph produces the 84-byte owner.

## Complete strict linked-PE acceptance

The canonical unit is `item-manager-register-callbacks-0041ad90`. The
specified pinned VC7.1 SP1 `/GL` compiler and exact matched source graph
produced **two independent cold PE/MAP/PDB objects**. Both have a complete
84-byte PDB owner and the strict complete binary comparator reports **84/84
exact**, with all **eight target-address fields independently bound**:

| Offset | Type | Bound original destination |
| --- | --- | --- |
| `+0x03` | DIR32 | update callback `0x0041BA00` |
| `+0x08` | REL32 | `Chain::AllocElem` `0x00449ED0` |
| `+0x13` | DIR32 | `th10::g_Chain` `0x00491BE4` |
| `+0x21` | REL32 | `Chain::AddToCalcChain` `0x00449AE0` |
| `+0x26` | DIR32 | draw callback `0x0041BA30` |
| `+0x2E` | REL32 | `Chain::AllocElem` `0x00449ED0` |
| `+0x3A` | DIR32 | `th10::g_Chain` `0x00491BE4` |
| `+0x48` | REL32 | `Chain::AddToDrawChain` `0x00449B70` |

Independent cold image SHA-256 digests:

- Cold 1: `515e41a4070688c9acc1ae64efa1e1a512a4c6892e034b1b48df0b410a4f3b0e`
- Cold 2: `aac285881dd2de4773596630c5584f6158e157a2d702a18b10f22141cf7478a3`

The exact comparison is `scripts/compare-linked-function.py`, not a raw
size-based score. The source bytes were installed before both canonical cold
builds. Four selected affected existing exact units also passed **260/260**
bytes: ItemManager OnUpdate 48, OnDraw 29, power display 79, and item record
constructor 104. No full historical cold replay was run.

Reproduce only the affected units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-manager-register-callbacks-0041ad90 \
  --unit item-manager-update-callback --unit item-manager-draw-callback \
  --unit item-power-display --unit item-record-constructor --json
scripts/repo-python scripts/ci.py
```

The record is admitted to `config/matches.csv` only after that full strict
verification. Archive/WPO graph selection and target-bound evidence are
explicit; separate game product, original source ownership, semantics under
execution and portability remain unproven.
