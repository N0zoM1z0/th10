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

These are target machine-code reads/calls, not positional guesses or permission to waive relocated bytes. The original function's private ESI input is a product of its actual call context, not a new artificial source-level calling-convention annotation. The most compact faithful producer has `src/FrontEnd.cpp` as the main `/GL` source, `src/VmIdInterrupt.cpp` as one **real** `/GL` support source, and `ResultMenuUpdateResult` as the entry. That source really calls `ResultMenuClose` in state 13; its interrupt helper is a separately present 65-byte PDB-owned function (`?EnemyMarkPendingInterrupt@@YAXPAH@Z`), so the two target `+0x3E/+0x49` calls are bound to actual executable code rather than an ABI-less synthetic symbol. A unique `src/FrontEnd.cpp` PDB section contribution emits exactly 89 bytes; no source changes or target/source ABI annotations were needed. All four independently typed fields bind in the unchanged strict comparator. The older 15-input `ResultTransitions` cold comparison additionally corroborates the same ESI/89-byte codegen, but is not required as the canonical acceptance graph.

## Scoped independent complete acceptance

Two *independent canonical selected cold* links rebuild **only the two-input `ResultMenuUpdateResult`/`VmIdInterrupt` producer** and verify the entire `ResultMenuClose` PDB body and all four original call/global fields. Both return **89/89 exact**, with distinct PDB GUIDs `df1adf5bed65a54b990a0fdb6b212711` and `3f92d94add43084ca0d9151aa9ac976f` (ignored complete receipts: `.analysis/gpt-web/current-turn/result-close-small-cold-{1,2}.json`). The source image also contains the real 65-byte interrupt implementation in the `VmIdInterrupt` support module, but no separate new exact credit is claimed for that callee.

A previous 15-input `ResultTransitions` canonical cold had also passed this owner together with two pre-existing 411-byte result-menu callers: **911/911 bytes and 60 bound fields**, GUID `76271c4656e51d4285efd2dcbb1ab2aa`. The second attempted rebuild of that larger graph encountered a Wine path-conversion page fault, rather than an assembly or byte comparison failure; toolchain COFF/LTCG/resource/PE32 smoke passed. A separate standalone image previously reproduced the same 89 bytes. These historical controls are additional corroboration, not the new canonical recipe and not a reason to rerun all 15 sources.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit result-menu-close --json
scripts/repo-python scripts/ci.py
git diff --check
```

This is whole-owner machine-codegen exactness, not proof of the source TU's original spelling, complete graphics data ownership, final runnable EXE, runtime behavior or portability. No full TH10 cold replay was attempted.
