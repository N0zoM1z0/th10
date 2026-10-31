# TransitionEffectView::Cleanup — complete 127-byte exact owner (2026-10-31)

`TransitionEffectView::Cleanup @ 0x0043CB00..0x0043CB7E` owns a dense **127-byte** reviewed authored function; both the complete physical source.ltcg.obj PDB contribution and **all fourteen** independently bound address fields match Japanese TH10 v1.00a SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. This adds **one exact owner and 127 authored code bytes**, no changes to boundary, origin, mapping, compiler flags or original game resources.

The maintained natural C++ source checks its existing `updateNode` and `drawNode` separately, with two real `th10::Chain::UnregisterElem` calls. It enters and leaves Windows `CriticalSection[0]` around each operation and independently increments/decrements the real chain nesting counter. Both non-null branches and early-skip tests appear in the complete target bytes. This is original linked machine-code alignment, not a host simulation of release behavior or a claim that the original implementation was exception-safe.

## Fourteen fixed target field roles

| Offset(s) | Original target identity |
| --- | --- |
| `+0x07`, `+0x0E` | IAT `EnterCriticalSection @0x004660B0`; `LeaveCriticalSection @0x004660B4` |
| `+0x1B`, `+0x4F` | `th10::g_Chain @0x00491BE4` |
| `+0x22`, `+0x38`, `+0x56`, `+0x6C` | existing `th10::g_CriticalSections @0x00492274` |
| `+0x2A`, `+0x40`, `+0x5E`, `+0x74` | existing `th10::g_ChainNestCounter @0x0049231C` |
| `+0x33`, `+0x67` | real `th10::Chain::UnregisterElem @0x00449F60` REL32 |

The strict comparator checks typed DIR32/REL32 identities, exact offsets, the original target addresses, all 127 code bytes, local branch destinations, and both true calls. It does not merely mask and forgive mismatched fields. The reviewed source body is `src/TransitionEffect.cpp`, whose unique actual PDB owner appears as a support TU under the real `StageCreate` graph; `pdb_source` explicitly selects it. This graph uses the existing StageScript + 12 real support sources and pinned per-source profiles, including ordinary `/GL` for TransitionEffect.cpp, and `/GL /GS` for the StageCreate entry. No source surgery was necessary.

## Two independent selected cold proofs

First scoped cold replay selected only `stage-transition-effect-cleanup`: **127/127, 14/14 fields, PDB GUID `4a4bda99a6193a439e4eec5177f3cf8a`**. Second independently rebuilt the same StageCreate graph selecting the new cleanup and pre-existing stage lifecycle initializer: **649/649 bytes and 35/35 fields**, including the original **522-byte** initializer, PDB GUID `f161607b19f69e4e8f66fcec7ad3d283`. Neither command ran the complete TH10 cold cohort. Full reports are ignored under `.analysis/gpt-web/current-turn/transition-cold-independent-{1,2}.json`.

The first replay attempt rejected the manifest while a transient duplicate diagnostic unit named `transition-effect-cleanup` overlapped the same target owner. This was *not* a compiler or machine-code failure; the checked manifest retains only canonical `stage-transition-effect-cleanup`. Independent cold retries above and the full strict unit comparator pass after that overlap was resolved. No duplicate target owner remains in the canonical manifest.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit stage-transition-effect-cleanup --unit stage-lifecycle-initialize --json
scripts/repo-python scripts/ci.py
git diff --check
```

All exactness claims are bounded to codegen, original target linkage roles, caller graph and PDB-owned extent. Original C++ spelling, production TU attribution, complete lifecycle state/data provenance, full EXE construction, runtime correctness and portability remain open.
