# ASCII manager factory — complete 69-byte exact owner (2026-10-31)

`AsciiManagerCreate @ 0x00401440..0x00401484` is now **canonical linked-PE exact: 69/69 physical bytes, five independently bound REL32 fields**. The sole valid target is original Japanese TH10 v1.00a SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. This adds one already reviewed authored owner and 69 exact bytes; neither function inventory nor source count changes.

## Genuine context corrects the old lifecycle hypothesis

Previous single-TU `AsciiManagerCreate`-rooted `/GL` probes emitted **70 bytes**, with a different destructor call convention (an extra ECX receiver move instead of target `push esi`) and a one-byte size gap. Earlier text called the factory `nonexact` and suggested an explicit destructor or `__stdcall` declaration would be needed. Those source/ABI alterations were correctly rejected. They are now superseded by real compiler context rather than revived.

`src/AnmManager.cpp` already contains the natural C++ `new AsciiManagerView`, `Initialize()` test, and `delete manager; return NULL` failure cleanup. Its actual `AsciiManagerCreate`/`AsciiManagerView::Initialize` producer has two real `/GL` support TUs, `src/GameErrorContext.cpp` and `src/Chain.cpp`, under the repository's existing VC7.1 SP1 profile. Selecting that three-input graph naturally generates the target's 69-byte body *without editing this factory, faking a receiver, adding a test-only helper or forcing a return*. It pushes ESI, calls `operator new`, performs the full constructor/null-branch sequence, calls Initialize, conditionally destroys/frees the object, and returns either NULL or its ESI-owned object.

The full address field inventory, checked against original instructions and the native PDB contributor, is:

| Owner offset | Role | TH10 target |
| --- | --- | --- |
| `+0x07` | C++ operator new | `0x00452493` |
| `+0x15` | AsciiManagerView constructor | `0x00401000` |
| `+0x22` | AsciiManagerView::Initialize | `0x00401110` |
| `+0x30` | AsciiManagerView destructor | `0x00401260` |
| `+0x36` | C++ operator delete | `0x004524A1` |

No target bytes or call targets are inferred from coincident anchor addresses, and the destructor's separate incomplete reconstruction does not prevent verifying the factory's typed call-site identity. The entire 69-byte PDB contribution, all branch destinations, both returns and all five target fields match. No partial function or padding bytes are waived.

## Independent scoped cold verification

The factory is added as a separate canonical `ascii-manager-create` unit in the existing shared `AsciiManagerInitialize` producer, with identical graph to the previously accepted 330-byte initializer and newly accepted 10-byte high-priority callback. Two independent selected cold replays, with distinct PDB GUIDs `4f3a91ee5c501746beaf5e847d00c81f` and `dd00b4caa3ff8c46a975081ca1a88d0e`, each accept **all three complete units, 409/409 bytes and 34 linked fields**. This guards the real initialization and draw-callback registration while admitting only the newly closed 69-byte factory. Reports are retained in ignored `.analysis/gpt-web/current-turn/ascii-create-cold-{1,2}.json`.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit ascii-manager-create --unit ascii-manager-initialize --unit ascii-manager-on-draw-high --json
scripts/repo-python scripts/ci.py
git diff --check
```

This is bounded source-codegen and call-site closure, not a claim of original physical translation-unit provenance, exact standalone destructor, full resource ownership, whole-game product build, runtime semantic equivalence or portability. No all-unit cold replay was performed.
