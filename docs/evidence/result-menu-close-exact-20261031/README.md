# Complete 89-byte result-menu shutdown (2026-10-31)

Original Japanese TH10 v1.00a target SHA-256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. `ResultMenuClose @0x00423510..0x00423568` is now **complete linked-PE exact: 89/89 owned bytes and 4/4 independently typed original address fields**. One already reviewed authored function, 89 authored bytes, is added; the fixed function extent, original target bytes and source/provenance inventories are unchanged.

## Actual source and target ABI

The maintained ordinary C++ `ResultMenuClose(ResultMenuView*)` in `src/FrontEnd.cpp` sets `state=13`, resets the embedded cursor timer with its actual `SetCurrent(0)` path, forwards the two existing VM IDs to the real `EnemyMarkPendingInterrupt` callee, and restores the saved ANM game speed. The original 89-byte target uses an ESI result-menu receiver, tests timer initialized bit 0, preserves timer fields, issues **two distinct calls** to the same real `EnemyMarkPendingInterrupt @0x00409E50`, and completes with plain `RET`. It neither deletes the controller nor deallocates the replay slots.

The full original bound-field inventory is:

| Function offset | Field | Target identity |
| --- | --- | --- |
| `+0x23` | `DIR32` | `g_AnmGameSpeed @0x00476F78` |
| `+0x3E` | `REL32` | `EnemyMarkPendingInterrupt @0x00409E50` |
| `+0x49` | `REL32` | second call to same actual callee |
| `+0x54` | `DIR32` | `g_AnmGameSpeed @0x00476F78` |

These are target machine-code reads/calls, not positional guesses or permission to waive relocated bytes. The original function's private ESI input is a product of its true caller context, not a new artificial source-level calling-convention annotation. Under the already existing `ResultTransitions` compiler graph rooted in `AnmLoadedView::InitializeVm` with thirteen real support sources, the genuine `FrontEnd.cpp` support object is the unique PE/MAP/PDB owner, and the full 89 native bytes and all four typed fields compare zero difference. Its `src/FrontEnd.cpp` effective override retains the existing `/GL /GS /EHs` VC7.1 SP1 compile profile. No source change or compiler flag modification is required.

## Scoped independent complete acceptance

The first canonical selected cold replay includes this new owner and its actual shared result-menu caller guards (`result-menu-open-game-over` and `game-complete-stage-transition`). All three complete units pass: **89 + 411 + 411 = 911/911 bytes and 60 linkage fields**, one shared producer, PDB GUID `76271c4656e51d4285efd2dcbb1ab2aa`. This establishes that the closing function's caller dependencies have not regressed.

A second canonical attempt encountered a Wine VC7.1 link helper page fault while converting a support-object path; it was **not** a byte comparison. The official `verify-toolchain.py --execute` native COFF, LTCG, resource and PE32 smoke passed. The independent second **15-input cold link** then used the exact deterministic Wine `Z:` path for this repository, retaining the identical compiler flags, linker switches, support sources and effective TU profiles, and ran the unchanged complete strict comparator against the new PE/MAP/PDB image. It also returned **89/89 and all 4 target fields exact**, PDB from `support-10-FrontEnd.ltcg.obj`, independent image SHA-256 `af789fdedff3a6678ff3a237432296fc231d4ce8514cabe248db095e1de9c111`. No target file or machine bytes were modified; deterministic Wine path mapping is not a linker/ABI workaround. The two reports are under `.analysis/gpt-web/current-turn/result-close-cold-1.json` and `.analysis/gpt-web/current-turn/result-close-independent-proof.json`. The additional failing Wine outputs remain diagnostic only.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit result-menu-close --unit result-menu-open-game-over --unit game-complete-stage-transition --json
scripts/repo-python scripts/ci.py
git diff --check
```

This is whole-owner machine-codegen exactness, not proof of the source TU's original spelling, complete graphics data ownership, final runnable EXE, runtime behavior or portability. No full TH10 cold replay was attempted.
