# TH10 spell-panel lifecycle: native 176 + 211 + 402 + 136 bytes

**Current status:** registration owner 0x004089C0 is now canonical exact
(**211/211, 13/13 fields**) using the real Chain LTCG producer. The other
three recovered lifecycle owners remain nonexact. Earlier isolated probes
retained below are historical controls, not the selected final context.

Target: original JP 1.00a at hash in `config/target.toml`. This source
checkpoint recovers four previously unmapped, reviewed target function
bodies into the EXISTING `EnemyGameStateView` owner; no second synthetic
`AnmPairDrawOwner` object is introduced. The global pointer at
`0x004776F4` and the exact `0x37B0` allocation bind the same owner as
`SpellPanel.cpp`, `EnemyGameState.hpp` and `EnemyEclDispatcher.cpp`.

## Independently recovered original calls and structure

- Constructor **0x00408910..0x004089BF (176 bytes)** is called with one
  object stack pointer. It constructs 2 `AnmVmView` entries at +0x10, 8 at
  +0x778 and 5 at +0x24D8 using the real `AnmVmView` constructor. It also
  clears the timer's active flag at +0x3744, zeroes `0x37B0` object bytes
  with REP STOS, ORs bit 2 into owner flags, and publishes global 0x4776F4.
- Task registration **0x004089C0..0x00408A92 (211 bytes)** allocates three
  true `ChainElem` nodes with the target callbacks at `0x00409220`,
  `0x00409230`, `0x00409270`, clears each node flag bit2, and registers a
  calculation task at priority **22** and two drawing tasks at **14/37**.
  Their owner pointer is stored in ChainElem +0x20 and their node pointers
  reside at owner offsets +8, +0xC and +0x37AC. Timer +0x3734 is initialized
  using the same observed `SetCurrent(0)` branch, with original speed scalar
  `0x00476F78`.
- Full destructor **0x00408AF0..0x00408C81 (402 bytes)** first marks all
  three managed VM IDs at +0x768/+0x76C/+0x770 for deletion using the actual
  ANM manager callee `0x004492A0`, then resets each. It unregisters the three
  nonnull tasks under the real critical section at `0x00492274`, maintaining
  the nesting byte at `0x0049231C`, then clears global 0x004776F4. The
  compiler-generated tail destroys VM arrays in reverse **5, 8, 2** order;
  the source retains actual C++ array members and their destructors.
- Factory **0x00408C90..0x00408D17 (136 bytes)** allocates **0x37B0** bytes,
  constructs the object, registers its three tasks, and on registration
  failure destroys/frees the actual owner before returning null. Native SEH
  and delete guards are part of the real complete code, not host-simulated
  failure tests.

The existing real `EnemyGameStateView` header is extended with the three
chain-node pointers, typed `flags` and its trailing +0x37AC node. Compile-time
layout checks enforce all three offsets and `sizeof == 0x37B0`. This is
in-place ownership recovery, not an invented parallel object definition.

## Scoped native compiler diagnostic — all open

The four recovered functions reside in `src/EnemyGameStateLifecycle.cpp`.
Pinned VC7.1 SP1 `/GL /GS /EHsc` source+PDB linked diagnostic in a selected
factory-entry graph yields these preliminary source contributions:

| Owner | Native PDB bytes | Target bytes | Status |
| --- | ---: | ---: | --- |
| `EnemyGameStateView::RegisterTasks` | 206 | 211 | NONEXACT |
| `EnemyGameStateView::~EnemyGameStateView` | 392 | 402 | NONEXACT |
| `CreateEnemyGameState` | 136 | 136 | NONEXACT; 87/96 preliminary comparable ordinary bytes, 10 candidate fields |
| `EnemyGameStateView::EnemyGameStateView` | generated, no authored-origin backlog line in this probe | 176 | NONEXACT; origin indeterminate |

The candidate constructor is indeed generated with real VM-array EH
construction and has further compiler differences from the original,
including extra default initialization of typed VM-ID records. We deliberately
did not delete those constructors, force private register argument placement,
copy original code bytes, add padding or claim an exact factory based solely
on equal lengths. The prior annotated `AnmPairOwnerDrawCallback` exactly uses
this owner's first two VMs; the original anonymous class view remains a
compatible *consumer view*, not a separate allocated owner.

The private build log is
`.analysis/gpt-web/20261103/spell-panel-lifecycle-first.log` and the
PDB-linked diagnostic is retained below `build/probe-ltcg/` (regenerable).
The original physical translation unit and runtime/game-product equivalence
remain open. No `config/matches.csv` row or codegen credit for these four.

## Registration complete canonical recovery (2026-11-03)

A new scoped compiler experiment passed the **actual, unchanged**
`src/Chain.cpp` as one `/GL` support source to the lifecycle file and
selected `CreateEnemyGameState` as the link entry. Unlike the initial
206-byte isolated function, the compiler now sees real `Chain::AllocElem`,
`Chain::AddToCalcChain` and `Chain::AddToDrawChain` implementations, giving
the target's private ESI element and EDI priority register transports.
The complete 0x004089C0 owner becomes **211/211** with **159/159** ordinary
bytes and exactly **13** real address/control fields at the original
instruction locations. The bound fields cover all three callback pointers
`0x00409220/0x00409230/0x00409270`, three AllocElem call sites,
three `g_Chain` references, one calc and two draw insertion calls and the
`0x00476F78` timer speed pointer. No old target byte is copied or patched.

Two independent zero-reuse cold builds of the canonical unit
`enemy-spell-panel-register-tasks`, using pinned VC7.1 SP1, replayed
**211/211 exact** with all 13 fields and no byte differences. Independent
PE SHA-256 and PDB GUIDs confirm two separate outputs. Full reports:
`.analysis/gpt-web/20261103/spell-register-cold{1,2}.json`.

In the same genuine Chain graph the destructor emits 341/402 and factory
70/136; this context is NOT accepted for them. They remain source-present,
NONEXACT and do not receive ledger or partial-function credit. The
constructor still needs its own exact extent and compiler context review.
The no-Chain context's 392/402 destructor, 136/136 factory (87/96 ordinary)
and 206/211 registration results remain useful for comparison but are not
canonical evidence. The recorded 211-byte proof closes only its own owner.
