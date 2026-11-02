# ResultMenu controller lifecycle: full constructor, initializer and factory

Reviewed Japanese TH10 v1.00a original and actual VC7.1 SP1 `/GL /GS /EHs`
`src/ResultMenuLifecycle.cpp` + `src/Chain.cpp` linked-PE PDB contributions.
This source was **implemented**, not inferred from a matching prefix:

| Target entry | Full original bytes | Complete strict native comparison | Target-bound fields | Original origin |
| --- | ---: | ---: | ---: | --- |
| `ResultMenuView::ResultMenuView` @ `0x004220E0` | 101 | **101/101 exact** | 1 | indeterminate (source-authored vs compiler-generated constructor) |
| `ResultMenuView::Initialize` @ `0x00422150` | 142 | **142/142 exact** | 9 | authored |
| `GameCreateController` @ `0x00422360` | 67 | **67/67 exact** | 5 | authored |

All 15 target address-field positions and identities are bound, including
`g_GameOwner477830`, the actual update/draw callbacks, `g_Chain`, real
`Chain::AllocElem`, `AddToCalcChain`, `AddToDrawChain`, `g_AnmGameSpeed`,
operator new/delete, real constructor, initializer, and cleanup
`GameCleanupController422220` (the cleanup itself remains nonexact and
source-absent). The PDB-owning constructor/initializer/factory contributions
are independently taken from the real linked image; **310/310 complete
bytes**, without masking unbound locations, changing target bytes, forcing
jump encodings, or ignoring any failure/null path.

## Source reconstruction

The actual controller allocation is 0x2C8 bytes. Its two nested cursor
subobjects initialize their initial state and 999-count controls, followed
by the target whole-object 0xB2-dword clear and flag/global publication.
Three `AnmVmIdView` default constructors originally emitted 18 unwanted
bytes: the retail constructor has no individual VM-ID writes before that
full clear. `ResultMenuVmIdStorage` is a four-byte, interface-compatible
storage view with a no-op default constructor and delegating ID operations.
The explicit `FrontEnd.cpp` overloads of `ResultMenuFindScript` and
`ResultMenuPendingThree` keep the real VM-ID implementation for both
existing standalone ID temporaries and the new controller member storage.
Original type names and constructor spelling are not established.

The initializer registers the genuine `0x422A90` update and already exact
`0x422AA0` draw callback nodes in priorities 9 and 46, publishes the
callback nodes at owner +8/+C, and restores timer state. The second node's
argument write occurs before its flags write; changing that source order
recovered the final four normal instruction bytes. The creator calls
operator new, the actual outlined constructor and initializer, and on
failure the actual controller cleanup and operator delete, preserving the
observed complete null-allocation/return path. The emitted wrapper at
`0x422A90` is **not separately exact** in this isolated producer; the
initializer's bound pointer to that real target address is not a claim
about callback body or origin.

## Focused proof and protections

The selected **three-owner cold** `result-menu-constructor`,
`result-menu-initialize`, `result-menu-creator` passes the complete 310 bytes,
15 fields in one real two-source image. Two separately cold-built controller
owners were also inspected independently. `result-menu-update` 206/206
and `result-menu-draw-rows-callback` 7/7 are protected in a fresh
selected affected-`FrontEnd.cpp` cold replay after the storage change.

For `ResultMenuUpdate` the entire 206-byte PDB section contains a
14-byte opcode selector array after three absolute pointer slots. Its
last byte (+0xCD) is **data** (retail 0x02), not an x86 instruction;
Capstone decodes 205/206 when processing the raw combined extent. The
canonical comparison now documents the one-byte terminal data span,
while comparing the complete unchanged owner and all 16 existing address
fields. It reports 206/206 exact, not an artificially cropped 205-byte
function or new codegen credit. The previously verified 17-source
`result-menu-resume` owner was not cold-replayed here because the
attempt exceeded the available scoped execution resource; no regression
pass is asserted for it. The full game, runtime, origin, and product closure
remain separate.

Reproduce only affected graphs:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit result-menu-constructor --unit result-menu-initialize \
  --unit result-menu-creator --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit result-menu-update --unit result-menu-draw-rows-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```
