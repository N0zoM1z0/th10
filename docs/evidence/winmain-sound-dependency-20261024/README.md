# WinMain Sound dependency diagnostic, 2026-10-24

## Scope and result

This experiment follows committed **45fce0f** and changes only the ignored
compile graph by appending existing `src/Sound.cpp`. No maintained source,
canonical configuration, comparator or metrics changed. Main.cpp remains
`e49f06a2c64f0eb6e2577237253a0b3bf0773d14fc51e991fcb2ced19d7be4f0`.

| Context | WinMain bytes / fields / calls / locals |
| --- | --- |
| Maintained ten-TU context | 1423 / 153 / 72 / 48 |
| Expanded eleven-TU diagnostic | 1453 / 154 / 71 / 46 |

Both are **nonexact against 1389 bytes**. These are complete owner extents and
inventory counts, not mismatch-byte counts. There is **no canonical graph
migration, new exact credit, source-owner credit or runtime closure**.
Expanded image SHA-256:
`165f28a711ec1133a87d035f2c6003260535a2b9f78e080d5b7a844794cae5ac`.
[Summary and reproducible graph](summary.json) pin all eleven source hashes,
profiles, ordered inputs and receipt identities.

## Actual graph and profiles

Primary source is `src/Main.cpp`. Support order is unchanged through item 9;
only item 10 is appended:

1. `src/GameErrorContext.cpp`
2. `src/FileSystem.cpp`
3. `src/Midi.cpp`
4. `src/PbgArchive.cpp`
5. `src/AnmManager.cpp`
6. `src/RandomMath.cpp`
7. `src/AnmVmCreate.cpp`
8. `src/AnmVmId.cpp`
9. `src/AnmInt3Scale.cpp`
10. `src/Sound.cpp`

Default compiler profile: `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`.
Only AnmInt3Scale adds `/Ob1`; Sound inherits the default LTCG profile, not a
normal-COFF or historical Sound profile. Entry is `WinMain@16`; harness is
`unresolved-symbol-data-anchors-v1`. Link profile:
`/ltcg /nodefaultlib /subsystem:console /incremental:no /fixed:no /base:0x00400000`
`/debug /debugtype:cv /opt:noref /opt:noicf /safeseh:no`.
No ZWave.cpp, /EHsc, source wrappers, new aliases or register hints were added.

## Original failure and supplemental ownership proof

Native compilation produced the retained image, but the original runner exited
**1**: the comparator expected Sound bodies in primary `source.ltcg.obj` rather
than their actual `support-09-Sound.ltcg.obj` contributions. Its incomplete
`trial-receipt.json`, log and exit remain unchanged.

The separate existing-image supplemental audit passes after adding the already
supported `pdb_source = "src/Sound.cpp"` selector to **exactly four** diagnostic
units: JoinWorkerThreads, ReleaseOwner, SetupThread and FindWaveChunk. No byte,
extent, field, profile or comparison gate was relaxed. All **107 original
hashes** are preserved; **no rebuild** occurred.

Independent read-only replay also **passes**, preserving **138 current original
artifacts** and verifying those **107 historical original hashes**. It confirms
the append-only graph, unchanged source hashes and sole four selector additions;
no native run was made by that review.

## Observations and acceptance limits

All thirteen intact guards pass: the prior nine total 1758/all 94; four Sound
owners add **604 bytes / all 17 fields** (Join 175/3, Release 301/6, SetupThread
57/7, FindWaveChunk 71/1). The combined total is **2362 bytes / all 111 fields**.
Log/Fatal/WriteFile remain 620/all 43, and UnprepareHeader remains 79/all 3.
StopPlayback remains 129/all 5 with the same **13 ordinary differences**, nonexact.
All preceding lifecycle/music properties replay successfully.

Actual Start/CreateThread work and Request/state-2 work inline into WinMain;
Join receives ESI and Release receives EAX as in the target. Full owner clearing,
window/handle publication and all six CreateThread arguments are checked. Five
negative controls reject wrong clear extent, thread parameter, cancellation
state and Join/Release receivers. `MainProcessSoundQueues` remains an unresolved
non-runnable anchor with no real PDB contribution; ProcessQueues is not silently
aliased. Diagnostic anchor storage is not shared runtime-storage evidence.

## Retained evidence and validation scope

All ignored receipts are under
`.analysis/gpt-dots/winmain-sound-dependency-20261024/`:

- `trial-freeze.json`, `trial-receipt.json`, `trial-root.log`, `trial-root.exit`
  and original `trial-units.toml` retain the native graph and original failure.
- `supplemental-ownership/receipt.json`, `contract-delta.json` and
  `trial-units.toml` retain the narrow existing-image ownership correction.
- `independent-review/receipt.json` binds the independent full replay, SHA-256
  `21279660633e296f6251f6492d163a6288066ebc395788d28446354fa1f77203`.

These ignored artifacts are not supplied by a Git clone. The existing 112-stage
CI pass verified at 21:39:42 UTC remains scoped to **45fce0f**. This documentation-
only checkpoint receives focused documentation checks, **not a new full CI pass**.
