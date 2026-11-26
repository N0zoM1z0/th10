# TH10 Position Independence (PI)

This document defines and verifies Position Independence (PI) for the
reconstructed TH10 v1.00a (`build/windows-i386/th10.exe`) product build,
following ReC98's PI criteria and the verification model used in TH08.

## Definition

A reconstructed build is position-independent when moving code, read-only
data, initialized data, and uninitialized globals to different virtual
addresses preserves program behavior without relying on the retail binary's
fixed layout:

1. **Symbolic references only**: code and initialized data reference functions,
   strings, vtables, and globals through symbolic relocations (never hardcoded
   retail `0x004xxxxx` addresses).
2. **Single storage per shared global**: cross-TU aliases of the same retail
   global bind to the exact same storage slot. No duplicate `.bss`/`.data`
   copies may exist across translation units.
3. **Struct-interior coherence**: fields accessed both through an enclosing
   struct (e.g., `g_GameScoreState`, `g_FrontEndInput`, `g_MainSupervisorView`)
   and through individual global aliases (`g_GameScore`, `g_PlayerCharacter`,
   `g_GuiDifficulty`, `g_MainInputEdges`, `th10::g_CriticalSections`) move
   together at their exact relative offsets.
4. **Reordered and relocated link equivalence**: linking all objects in reverse
   order at a non-default image base (`/base:0x00500000`) produces a valid PE32
   executable whose verified initialized pointers carry `IMAGE_REL_BASED_HIGHLOW`
   `.reloc` entries and whose startup behavior matches retail.

## Implementation

- **Pure C++ read-only strings & constants**: [src/ProductStrings.cpp](../src/ProductStrings.cpp)
  defines all built-in Shift-JIS/ASCII strings (with UTF-8 comments), float
  constants, and lookup tables as standard C++ literals and arrays. Note:
  symbols whose existing exact-unit `extern` declarations are non-`const`
  (`char[]` / `float`) are placed in `.data` by VC7.1; `extern const` symbols
  are placed in `.rdata`, and both sections are verified in `scripts/verify-pi.py`.
- **Pure C++ initialized data tables**: [src/ProductGlobals.cpp](../src/ProductGlobals.cpp)
  defines all initialized `.data` tables (bullet tables, stage records,
  ending/demo/shot paths, callback tables with direct C++ function pointers,
  `g_GameScoreState`, `g_MainScanWorker`, and `th10::g_GameErrorContext` using
  the canonical headers `ScriptViewerWorker.hpp` and `GameErrorContext.hpp`)
  and binds same-address aliases via `#pragma comment(linker, "/alternatename:...")`.
- **Authoritative zero-initialized `.bss` layout**: [src/ProductBssLayout.asm](../src/ProductBssLayout.asm)
  defines the unified zero-initialized `.bss` storage layout (187 symbol labels
  and 127 `DB ... DUP(?)` reservations, with zero strings, zero data bytes, and
  zero hardcoded addresses). Retaining `.asm` for `.bss` keeps contiguous
  single-storage layout and preserves VC7.1's 5 sub-4-byte struct-interior
  offsets (`+0x06`/`+0x32` in `g_FrontEndInput` and `+0x136`/`+0x6F6`/`+0x6F7`
  in `g_MainSupervisorView`).
- **DirectInput SDK constants**: `g_InputKeyboardDataFormat`,
  `g_InputJoystickDataFormat`, `g_InputKeyboardGuid`, and
  `g_InputDirectInputIID` bind directly to `dinput8.lib` / `dxguid.lib`
  (`_c_dfDIKeyboard`, `_c_dfDIJoystick2`, `_GUID_SysKeyboard`,
  `_IID_IDirectInput8A`) via linker `/alternatename`.
- **Cross-TU bridge**: [src/ProductBridge.cpp](../src/ProductBridge.cpp)
  wires placeholder-named declarations, the 4 runtime callbacks
  (`0x00420000`, `0x004200D0`, `0x0042BC30`, `0x0042BE10`), and `EnemyLaser`
  virtual dispatch to their reconstructed exact owners.

## Real PI bugs caught and fixed

1. **Duplicate `.bss` storage (caught during Wine runtime testing)**:
   `MainLifecycle.cpp` defined `g_MainSupervisorView` separately from
   `ProductData.asm`'s `.bss`, causing `WinMain` to initialize
   `g_MainSupervisorView.criticalSections` in one copy while
   `GameErrorContext::Log` entered `th10::g_CriticalSections[3]` in the other
   and deadlocked. Auditing `th10.map` caught 4 more duplicate definitions
   (`g_AsciiManagerView`, `g_ScriptViewerBullets`, `g_EnemyGameState`,
   `g_ItemManager`). Converting all 5 to `extern` unified every TU onto one
   storage slot per retail global.
2. **Cross-TU symbol-name collisions (caught during link audit)**:
   6 symbol names were reused for distinct retail addresses across separate
   TUs (`g_EnemyGlobalPositionOffset` / `g_EnemyPositionOffsetDelta`,
   `g_GameOwnerBulletAnmErrorMessage` / `g_GameOwnerUiAnmErrorMessage`,
   `g_FrontEndSupervisorFlags` / `g_InputSupervisorFlags`,
   `g_Player` / `g_GameOwner4776F4`,
   `g_ReplayDifficultyNames` / `g_ResultDifficultyShortNames`,
   `g_FrontEndKeyboardState` / `g_FrontEndStageKeyboardState`). Each distinct
   retail global now has a distinct C++ symbol.
3. **Self-pointer relocation**: `th10::g_GameErrorContext.m_BufferEnd`
   (`+0x2000`) is initialized to `&th10::g_GameErrorContext` via a COFF
   relocation rather than a fixed retail address.

## Verification

Reproduce both builds and run the PI verifier:

```bash
scripts/repo-python scripts/build.py --fresh
scripts/repo-python scripts/build.py --reverse-link
scripts/repo-python scripts/verify-pi.py
```
(Use `--fresh` after editing shared `.hpp` headers so VC7.1 recompiles all
dependent translation units; incremental `scripts/build.py` only tracks `.cpp`
mtimes.)

Observed results (`th10.exe` @ `0x00400000` vs `th10-reordered.exe` @ `0x00500000`):

- **Symbol displacement**: every probed `.text`, `.rdata`, `.data`, and `.bss`
  symbol moved by `+868 KB` to `+1.23 MB` (e.g. `_WinMain@16` `0x0042F960 ->
  0x00512AC0`, `g_GameErrorContext` `0x00458E40 -> 0x00558E40`,
  `g_AnmRenderManagerView` `0x00476830 -> 0x00576810`), with section ownership
  independently cross-checked against both the `.map` file and the PE section
  table.
- **Shared-storage identity**: all alias groups (`AnmRenderManager`,
  `Difficulty`, `CriticalSections`, `PlayfieldBottom`) and all 65
  `/alternatename` pragmas in `src/ProductGlobals.cpp` resolve fail-closed to
  identical addresses within each build.
- **Struct-interior offsets**: all checked offsets (`g_GameScoreState` fields
  `+0x04/+0x28/+0x34/+0x60`, `g_FrontEndInput` fields `+0x06/+0x2C`) remain
  exact across both links.
- **Verified PE `.reloc` pointer contracts**: 42 initialized pointers are
  explicitly checked for `IMAGE_REL_BASED_HIGHLOW` (`type == 3`) `.reloc`
  entries and target values in both PEs — the 5 code/self pointers
  (`g_GameErrorContext.m_BufferEnd`, `g_AnmSpecialVmDescriptors[0].cb`,
  `g_PlayerShotSpawnCallbacks[1]`, and `g_PlayerShotUpdateCallbacks[1..2]`)
  and all 37 `g_SoundFileNames[0..36]` string pointers (targeting `.rdata`
  `"se_*.wav"` literals). Additional pointer-bearing data tables remain
  candidates for future verifier expansion.
- **Headless Wine/Xvfb startup parity**: with a seeded windowed `th10.cfg`
  (skipping Dialog 203) and no `th10.dat` archive present, `resources/th10.exe`,
  `build/windows-i386/th10.exe`, and `build/windows-i386/th10-reordered.exe`
  all load `DINPUT8.dll`, `DSOUND.dll`, `d3d9.dll`, and `d3dx9_31.dll`, create
  the game window (`東方風神録 〜 Mountain of Faith. ver 1.00a`), initialize
  Direct3D9, and reach the exact same retail missing-archive teardown point
  (`GameFinalizeStatistics42B1E0 + 0x0`: `0x0042B1E0` retail, `0x00426610`
  standard build, `0x0051D330` reordered build). This verifies startup parity
  rather than full gameplay parity; full in-game testing requires a private
  `th10.dat` archive.
