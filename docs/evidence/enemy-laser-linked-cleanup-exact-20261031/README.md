# Linked enemy-laser manager cleanup — complete 195-byte exact (2026-10-31)

The reviewed authored `GameCleanupLinked41C1C0 @ 0x0041C1C0..0x0041C282` is **canonical linked-PE exact: all 195/195 target bytes, all 16 typed address fields**, in two independent local VC7.1 SP1 cold links. The target is original Japanese TH10 1.00a SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. This adds one complete authored owner and 195 exact code bytes without changing the established reviewed extent, source mapping, ABI or original game resources.

## Actual source and compact compiler graph

`src/EnemyLaserManager.cpp` already supplies the ordinary `void __stdcall GameCleanupLinked41C1C0(GameChainOwnerView*)` body. It unlinks the update and draw tasks from the real Chain under supervisor critical-section ownership and traverses/frees the owned laser common-storage linked list before clearing `g_EnemyLaserManager`. The full original target decodes as 62 instructions, including the null tests, both conditional scheduler removals, the linked-list loop, explicit `operator delete`, global reset and terminating `RET 4` at offset `+0xC0`. No code tail, branch or data field is cropped.

Historical thirteen/33-source probes were provisional; a fresh smaller real graph suffices. The canonical producer starts at the *actual source caller* `GameCreateOwner41C290` in `src/EnemyLaserManager.cpp`, with the actual `src/Chain.cpp` implementation as its only support source. It retains the established VC7.1 `/GL /GS` profile, original linker flags and unresolved-address anchor harness. The unique PDB section contributing the complete cleanup is `source.ltcg.obj`; the **114-byte actual `th10::Chain::UnregisterElem` body** appears in `support-00-Chain.ltcg.obj`. No inline fake, synthetic return, explicit register preservation, forced noinline or original machine-byte injection was added.

## Exhaustive target field ownership

The 16 field positions, with original target identities validated in the strict complete-owner comparator, are:

| Offset(s) | Type and original target binding |
| --- | --- |
| `+0x07`, `+0x0E` | `DIR32` Windows `EnterCriticalSection` IAT `0x004660B0` and `LeaveCriticalSection` IAT `0x004660B4` |
| `+0x1B`, `+0x4F` | `DIR32` actual `th10::g_Chain @0x00491BE4` |
| `+0x22`, `+0x38`, `+0x56`, `+0x6C` | `DIR32` `g_MainSupervisorView @0x00491C28` **plus `0x64C`**, resolving to `0x00492274` (critical sections) |
| `+0x2A`, `+0x40`, `+0x5E`, `+0x74` | `DIR32` same actual supervisor base **plus `0x6F4`**, resolving to `0x0049231C` (lock counters) |
| `+0x33`, `+0x67` | `REL32` `th10::Chain::UnregisterElem @0x00449F60` |
| `+0xA5` | `REL32` real `operator delete @0x004524A1` |
| `+0xB7` | `DIR32` `g_EnemyLaserManager @0x0047781C` |

The structured supervisor-base-plus-addend identities matter: these positions were **not** treated as arbitrary independent scalar globals solely because their absolute addresses agree. All linker-anchored fields are pinned to an actual named source symbol and correct original addend; branch displacement and complete physical bytes remain exact under the unmodified strict comparator.

## Independent scoped native acceptance

Two independently rebuilt **two-input** `/GL /GS` images, selected with only `enemy-laser-linked-cleanup`, each pass **195/195 complete owned bytes and all 16 fields**, with distinct PDB GUIDs `502bf7715a94f840b28aad8faf1bc208` and `441003a30c03ab4fac09812d9e15f733`. Full reports remain under ignored `.analysis/gpt-web/current-turn/game-cleanup-cold-{1,2}.json`. The larger historical source-linked images were discovery controls only: exact ledger admission follows the two new strict cold builds, not a normalized similarity score or any complete-product replay.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit enemy-laser-linked-cleanup --json
scripts/repo-python scripts/ci.py
git diff --check
```

Exact codegen and callee/caller identity do not establish original C++ spelling, complete physical production TU ancestry, global data ownership, final whole-game EXE, semantic runtime behavior or portability.
