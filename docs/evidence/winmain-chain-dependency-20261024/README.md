# WinMain Chain dependency diagnostic, 2026-10-24

## Bounded result

After **97a206f**, only existing `src/Chain.cpp` was appended to the ignored
Sound dependency graph. Source remains at **45fce0f**; Main.cpp SHA-256 is
`e49f06a2c64f0eb6e2577237253a0b3bf0773d14fc51e991fcb2ced19d7be4f0`.
No source, canonical configuration or metrics changed.

| Context | WinMain bytes / fields / calls / locals |
| --- | --- |
| Maintained ten-TU context | 1423 / 153 / 72 / 48 |
| Sound eleven-TU diagnostic | 1453 / 154 / 71 / 46 |
| Sound + Chain twelve-TU diagnostic | 1533 / 155 / 71 / 48 |

All remain **nonexact against 1389 bytes**. These are extents and inventory
counts, not mismatch-byte counts. New exact bytes and source owners are **zero**.
No canonical graph migration or runtime closure is claimed.
Twelve-TU image SHA-256:
`187250991fd10383de3d37afb34355511ce48428910c5d6e4ee1f8045de3c7ba`.

Native runner exit is **0**. Independent full replay and detailed creation/
routing review **pass**, with all original files unchanged and no review-time
native run. The complete parent remains nonexact.

## Reproducible graph

[Summary](summary.json) records all twelve source hashes and exact graph/profile
arrays. Primary is Main.cpp; support order is GameErrorContext.cpp, FileSystem.cpp,
Midi.cpp, PbgArchive.cpp, AnmManager.cpp, RandomMath.cpp, AnmVmCreate.cpp,
AnmVmId.cpp, AnmInt3Scale.cpp, Sound.cpp, then Chain.cpp, all under `src/`.
The [eleven-TU graph](../winmain-sound-dependency-20261024/README.md) is unchanged
except for that final append.

Common profile is `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`; only
AnmInt3Scale adds `/Ob1`. Entry remains `WinMain@16`, with the same unresolved-
symbol data-anchor harness and linked LTCG profile recorded in the summary.
No flag matrix, noinline forcing, source alias or canonical manifest edit occurred.

## Precise boundary and storage limits

The real creation wrapper **and constructor both inline into WinMain**. The
original retains an explicit constructor call; resolving a real definition
therefore did not recover that target call boundary. A standalone constructor
also remains: **59 complete bytes with the same 16 ordinary differences**, not
an exact body; it has zero address fields. No source attribute or ABI fabrication
is justified by this result.

Detailed review proves actual `new 0x48`, null-path bypass of all owner accesses,
and all **16 DWORD sentinel stores** on the nonnull path. Only flag bit 0 is
cleared; argument words at **+0x20 and +0x44 remain untouched**. Separate paths
publish either zero or the actual allocation result to the Main callback slot.
Whole-owner growth from 1453 to 1533 partitions as prefix **0**, creation/SDK
setup **+81**, suffix **-1**, total **+80 bytes**. This explains emitted extents,
not a byte-mismatch score or 81-byte improvement.

Six seams now resolve to actual Chain definitions: MainCreateCallbackOwner,
MainRunCalcChain, MainRunDrawChain, AllocElem, AddToCalcChain and AddToDrawChain.
Definition visibility is not a claim that every call remains outlined or that
these bodies are exact. Whole-image direct-edge counts are wrapper 0,
constructor 0, RunCalc 1, RunDraw 1, AllocElem 5, AddCalc 2 and AddDraw 3.
`g_MainCallbackOwner` at `0x414304` and `g_Chain` at `0x4142D8` retain **distinct
candidate storage** despite their target-role identity; shared runtime storage
has not been proved. Register/release-chain, destroy-callback and configuration
facades remain unresolved; MainProcessSoundQueues is still unresolved as well.

All inherited thirteen guards pass **2362 bytes / all 111 fields**, three peers
retain **620 / all 43**, and UnprepareHeader retains **79 / all 3**. StopPlayback
remains **129 / all 5**, with its 13 ordinary differences. Prior lifecycle,
Sound, music and publication properties pass without granting parent exactness.

## Retained evidence and next representative

Ignored packet `.analysis/gpt-dots/winmain-chain-dependency-20261024/` retains
`trial-freeze.json`, `trial-receipt.json`, `trial-root.exit`, full inventories,
`trial-chain-properties.json` and `independent-review/receipt.json`. Detailed
review adds `independent-review/creation-proof.json`, `six-seam-routing.json`
and `README.md`. These artifacts are not supplied by a Git clone; summary hashes
pin receipt identities without copying disassembly or target bytes.

**Park WinMain against flag/noinline matrices.** Reopen only with new target,
def-use, lifetime, callee or real-context evidence and a falsifiable prediction.
The next larger representative is **HUD UpdateStageElements, 3836 bytes**, with
a queued capture/lifetime model. Only read-only review is underway; it is **not
tested** and has no result or exact credit.

The 112-stage CI pass verified at 21:39:42 UTC remains scoped to **45fce0f**.
This documentation-only checkpoint receives focused documentation checks, not
a new full-CI claim.
