# Exact TH10 ItemManager callback registration (84-byte physical owner)

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The reviewed source-written Item manager registration owner at
`0x0041AD90..0x0041ADE3` has a complete 84-byte extent, with one
ESI/EBX/EAX/EDI optimizer-private caller context. It returns with plain
`RET`, not a stack-argument return. Its original source filename and
physical production translation unit are unknown.

## Fix the actual source dependency, not the register outcome

The old `src/ItemManager.cpp` version used three fictional external
`ItemCreateCallbackNode`, `ItemRegisterUpdateNode`, and
`ItemRegisterDrawNode` declarations, plus an additional callback-chain
global. That isolated candidate produced **91 bytes**, never matched the
complete owner and did not bind its four original calls. The real target
instruction stream, already independently reviewed in the Chain ledger,
instead calls:

| Full target call | Bound original destination |
| --- | --- |
| allocate update callback | `th10::Chain::AllocElem @ 0x00449ED0` |
| insert update at priority 21 | `th10::Chain::AddToCalcChain @ 0x00449AE0` |
| allocate draw callback | `th10::Chain::AllocElem @ 0x00449ED0` |
| insert draw at priority 25 | `th10::Chain::AddToDrawChain @ 0x00449B70` |

The maintained C++ now invokes the real, existing Chain class, uses its
canonical global `th10::g_Chain @ 0x00491BE4`, clears each allocated
element's flag bit 1, publishes the original ItemManager argument
at node +0x20, and stores the update/draw nodes at manager +0x08/+0x0C.
This does not copy game instructions, modify source ABIs to force machine
registers, create a second Chain class, or fabricate register-pressure
work. It preserves the independent already exact callbacks at
`0x0041BA00` and `0x0041BA30`.

## Canonical 84/84 complete-owner proof

The genuine `GameCreateItemManager` source entry, with `ItemManager.cpp`,
`Chain.cpp` and `AnmManager.cpp`, was cold-built using the pinned VC7.1
SP1 `/GL` profile. Optimizer context naturally restores the target
private EBX manager receiver, ESI element, and EDI priority. The complete
PDB-owned **84 bytes** agree with the original without masking any
ordinary bytes. The **eight decoded address fields** are bound separately
to both real callback entrypoints, the two allocation calls, the two
Chain-insertion calls, and the twice-loaded original Chain global.

The same source and canonical recipe passed **two independently built PE/PDB
images**, with different image SHA-256 and PDB GUIDs. Both pass **84/84,
8/8 fields**. [native-proof.json](native-proof.json) records both concrete
cold identities and complete field manifests; the private executables and
PDBs remain ignored under `build/match-linked/ItemManagerRegisterCallbacks/`.

The preexisting four affected exact units were **selected and cold-built**
after this source modification, independently of the new match:
`item-manager-update-callback` 48,
`item-manager-draw-callback` 29,
`item-power-display` 79 and
`item-record-constructor` 104 bytes — **260/260**.
[Selected guards](selected-guards.json) record their results.
No whole-cohort replay was executed.

To reproduce only this new unit:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-manager-register-callbacks-0041ad90 --json
scripts/repo-python scripts/ci.py
```

This is exact *authored* codegen at the reviewed target extent, not proof of
the historical source spelling, a complete linked Windows game, runtime
equivalence or portability. The nonexact Chain allocation body is a
separately tracked owner; this result binds its **call target**, not its
codegen or runtime.
