# ANM world-creator and shared-start context (2026-11-08)

Scope: four complete 118-byte world-coordinate VM creators and the shared
203-byte `InitializeAndExecuteScriptIndex`. The four world creators are now
**canonical exact linked-PE units**. The shared start helper remains open.

| Owner | Target | Complete size | Current result |
| --- | --- | ---: | --- |
| CreateVmAtWorldVariant0 | 0x448DB0 | 118 | exact |
| CreateVmAtWorldVariant1 | 0x449010 | 118 | exact |
| CreateVmAtWorldVariant2 | 0x448EE0 | 118 | exact |
| CreateVmAtWorldVariant3 | 0x449140 | 118 | exact |
| InitializeAndExecuteScriptIndex | 0x43E710 | 203 | unresolved (202/203 prior candidate) |

## Target observations

The target-attested `target-facts.json` pins all four full creator extents,
code hashes, seven address fields per function, the translation constants and
the shared helper. Every creator receives its position in EDI, uses three
stack words for resource, hidden `AnmVmIdView` return storage and script index,
and ends in RET12. The allocated VM is captured in ESI. The target's critical
schedule is: load `flags35C` at +0x14, store zero to `renderLayer` at +0x22,
OR bit 30 at +0x29, then store flags at +0x2E. It adds binary32 constants
224.0f and 16.0f to position X/Y, copies Z, starts the script, reloads the
manager and inserts the VM into the variant-specific list.

## Natural C++ reconstruction that closes all four leaves

The previous direct source sequence emitted the flag OR/store before clearing
the layer, leaving 13 ordinary bytes different in every complete function.
The following ordinary C++ expression shape makes the compiler preserve the
observed alias-sensitive schedule without inline assembly, volatile accesses,
byte padding or target-specific intrinsics:

```cpp
unsigned int flags = vm->flags35C;
unsigned int *renderLayer = &vm->renderLayer;
*renderLayer = 0;
vm->flags35C = flags | 0x40000000u;
```

This is applied identically in all four `CreateVmAtWorldVariant0..3`
implementations in `src/AnmManager.cpp`. The important property is not a
forced statement-order trick: taking the address of the adjacent member gives
MSVC's alias analysis a real pointer alias to account for while the earlier
flags value remains live. The emitted sequence is byte-identical to the
reviewed target sequence.

Bounded source experiments were run against the existing single-TU VC7.1
SP1 `/GL` screen-family recipe before promoting the change:

- The original `flags |= bit; layer = 0;` shape: 13 ordinary differences per
  118-byte creator.
- Merely moving the layer assignment before the flag expression, or using
  several scalar flag-local forms: still 13 differences.
- Taking a pointer to the layer before loading flags, or using a byte-offset
  pointer: 18 differences due to the compiler scheduling the layer store too
  early.
- Loading flags first, then taking and writing the member pointer, then
  storing the updated flags: zero ordinary differences for all four leaves.
- A byte-offset pointer alias and `memset` variant were also checked; the
  member pointer form above is the simplest natural expression with the same
  exact output.

## Canonical replay

Four canonical units are now declared in `config/match-units.toml`:
`anm-create-vm-world-variant0..3`. Each unit is a complete 118-byte
PDB-section contribution in a cold linked PE built with the pinned VC7.1 SP1
CL13.10.6030 / LINK7.10.6030 `/GL` profile and the existing
`AnmVmView::Initialize` entry context. Every unit exhaustively accounts for
seven independently reviewed DIR32/REL32 fields:

| Field offset | Kind | Target |
| ---: | --- | ---: |
| +0x09 | DIR32 | g_AnmRenderManagerView @ 0x491C10 |
| +0x0E | REL32 | AllocateVm @ 0x449950 |
| +0x38 | DIR32 | 224.0f @ 0x470B4C |
| +0x47 | DIR32 | 16.0f @ 0x470B48 |
| +0x5D | REL32 | InitializeAndExecuteScriptIndex @ 0x43E710 |
| +0x63 | DIR32 | g_AnmRenderManagerView @ 0x491C10 |
| +0x6C | REL32 | AddVmVariantN (variant-specific) |

Two independent focused cold replays of the four canonical units returned
`exact` for all four functions: 118/118 compared bytes, complete decoding,
complete relocation normalization, and no ordinary-byte differences. The
first replay image SHA-256 was
`a3b2c449995259f10da825d3a1eb36a25f145f2fd795d4d5a27f201cf19e421c`; the
second was
`88ef117ba5b5ed3b194afcbe3f6d41a7df0100508f05932da66aa01dcc07c692`.
The image identity varies between cold link invocations, but all four complete
function comparisons and all seven target-bound fields passed on both runs.

The four existing 95-byte screen creators also replayed exact in the same
focused source graph (380/380 bytes, five fields each). This does not imply
that the entire executable or every incoming caller is closed. The linker
harness intentionally contains unresolved anchors and is diagnostic, not a
runnable product image. The original production translation-unit ownership
and whole-image closure remain separate questions.

## Shared helper remains open

The shared helper is still 203 bytes. Its prior single-TU candidate had one
ordinary receiver-register difference at +0x27: target `MOV EDX,EDI`, candidate
`MOV ECX,EDI`, before calling `Initialize`. Matching the callee's address does
not waive the receiver instruction. Do not mark this helper exact or combine
its near-match with the four exact creators.

## Reproduction and next work

Run only the touched leaves when iterating:

```sh
scripts/repo-python scripts/replay-exact-units.py --json \
  --unit anm-create-vm-world-variant0 \
  --unit anm-create-vm-world-variant1 \
  --unit anm-create-vm-world-variant2 \
  --unit anm-create-vm-world-variant3
```

The four source implementations and canonical recipes are retained. Next
ANM work should attack the shared 203-byte helper with real caller/receiver
and lifetime evidence, while the boss-side `EnemyBeginSpell` work should
continue independently. Do not repeat the already-negative store-order,
bitfield, outlining or free-helper experiments without new evidence.
