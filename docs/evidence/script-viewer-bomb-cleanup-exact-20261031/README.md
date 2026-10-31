# ScriptViewer bomb-owner destructor — complete 137-byte exact owner (2026-10-31)

`ScriptViewerBombOwnerView::~ScriptViewerBombOwnerView @ 0x00405620..0x004056A8` is now **canonical linked-PE exact: all 137 original bytes, 15/15 independently target-bound fields**. The verified target is Japanese TH10 1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. This admits one already reviewed authored function, with unchanged extent, source/owner mapping and original game binary.

## Genuine parent and callee graph

The maintained ordinary destructor in `src/ScriptViewerLifecycle.cpp` removes the existing update and draw task nodes independently, enters/leaves the actual supervisor critical section on each path, balances the same lock counter, and clears `g_ScriptViewerBomb` last. The original target decodes to 40 complete instructions, including conditional branches for both task pointers and a `RET 4` at relative offset `+0x86`; neither the final global-clear store nor the return is dropped.

Earlier 14-source lifecycle links were a discovery clue, not the canonical proof. A new two-TU native graph starts at the actual `ScriptViewerReleaseBomb` entry in `ScriptViewerLifecycle.cpp`, including its real `ScriptViewerDeleteOwned` call and actual `src/Chain.cpp` callee. The unchanged pinned VC7.1 SP1 `/GL /GS /EHs` compilation produces the complete 137-byte PDB contribution in its source module; the 114-byte real `th10::Chain::UnregisterElem` callee is present in the support module. No C++ source edits, ABI forcing, dummy calls, inert padding or synthesized returns were made.

## Exhaustive address-binding contract

| Original offset(s) | Type and independently checked original target |
| --- | --- |
| `+0x07`, `+0x0E` | `DIR32` `EnterCriticalSection @0x004660B0`, `LeaveCriticalSection @0x004660B4` IAT |
| `+0x1B`, `+0x4F` | `DIR32` actual `th10::g_Chain @0x00491BE4` |
| `+0x22`, `+0x38`, `+0x56`, `+0x6C` | `DIR32` `g_MainSupervisorView @0x00491C28` plus `0x64C`, original `0x00492274` |
| `+0x2A`, `+0x40`, `+0x5E`, `+0x74` | `DIR32` same real Supervisor base plus `0x6F4`, original `0x0049231C` |
| `+0x33`, `+0x67` | `REL32` actual `th10::Chain::UnregisterElem @0x00449F60` |
| `+0x7D` | `DIR32` actual `g_ScriptViewerBomb @0x004776EC` |

These are all 15 fields of the unique 137-byte complete PDB contribution. The manifest binds the true named Supervisor base with explicit addends instead of accepting coincident addresses of synthetic standalone globals. The unchanged comparator checks all direct local branches, both real Chain calls, every original typed field, complete prologue/epilogue bytes and fixed function extent.

## Two independent bounded cold replays

Two independently rebuilt **two-input** selected canonical cold links of `script-viewer-bomb-destructor` both strictly return **137/137 exact and 15 bound fields**, PDB GUIDs `dbb8a7321af00040919ed3708bfb2a69` and `4ee80400db3a434eb5a880eab30869e3`. Full ignored receipts are under `.analysis/gpt-web/current-turn/bomb-cold-{1,2}.json`; generated PE/MAP/PDB remain under the canonical `build/match-linked/ScriptViewerBombCleanup/` output. The related 236-byte BulletManager destructor stays outside this exact admission; it requires an independent EH-handler contract and cannot be credited from a normalized 236-byte image alone.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit script-viewer-bomb-destructor --json
scripts/repo-python scripts/ci.py
git diff --check
```

This proves source-to-target compiler codegen and the observed callee identities, not original source spelling, original production TU, complete data lifecycle, exception behavior, whole EXE, runtime equivalence or portability.
