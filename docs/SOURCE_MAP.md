# TH10 Source Map

This map explains how `src/` translation units are organized and how
[scripts/build.py](../scripts/build.py) builds `build/windows-i386/th10.exe`
with pinned VC7.1 SP1 6030 (`/GL` + `/LTCG`).

## Build graph overview

- **Active C++ translation units (`125` files)**: all `src/*.cpp` except
  `src/EnemyLaserPatternCancel.cpp` (a standalone exact-comparison TU whose
  `EnemyEffectWaitManager::ApplyPatternCancel` body is already compiled in
  `src/EnemyEclDispatcher.cpp`).
- **LTCG `/GL` TUs (`123` files)**: all reconstructed game owners, compiled
  with their per-TU VC7.1 SP1 profile from [config/match-units.toml](../config/match-units.toml)
  (`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, plus `/GS` / `/EHs` / `/EHsc`
  where required by the target).
- **Non-LTCG C++ product data TUs (`2` files)**:
  - [src/ProductStrings.cpp](../src/ProductStrings.cpp): all built-in `.rdata`
    Shift-JIS/ASCII strings (with UTF-8 comments), float constants, and lookup
    tables.
  - [src/ProductGlobals.cpp](../src/ProductGlobals.cpp): all initialized `.data`
    tables, callback tables, stage records, `g_GameScoreState`,
    `g_MainScanWorker`, `th10::g_GameErrorContext`, and `#pragma comment(linker,
    "/alternatename:...")` shared-storage aliases.
- **Zero-initialized `.bss` layout (`1` file)**:
  - [src/ProductBssLayout.asm](../src/ProductBssLayout.asm): zero-initialized
    `DUP(?)` `.bss` storage preserving VC7.1's 5 sub-4-byte struct-interior
    offsets (`+0x06`/`+0x32` in `g_FrontEndInput`; `+0x136`/`+0x6F6`/`+0x6F7`
    in `g_MainSupervisorView`). Contains no strings, data bytes, or pointers.
- **Win32 resources (`1` file)**:
  - [src/th10.rc](../src/th10.rc): startup display-mode selection Dialog 203
    (`MAIN_DIALOG_BTN_FULLSCREEN = 201`, `MAIN_DIALOG_CHK_ASK_ALWAYS = 202`,
    `MAIN_DIALOG_BTN_WINDOWED = 203`).

## Source families

| Family | Files | Notes |
| --- | --- | --- |
| Entry & Supervisor | `Main.cpp`, `MainLifecycle.cpp`, `MainConfiguration.cpp`, `SupervisorModes.cpp`, `Startup.cpp`, `GameStartupRuntime.cpp`, `GameStartupVersion.cpp` | `WinMain`, D3D9 device lifecycle, `th10.cfg`, mode transitions |
| Core infrastructure | `Chain.cpp`, `FileSystem.cpp`, `PbgArchive.cpp`, `Lzss.cpp`, `Decompress.cpp`, `GameErrorContext.cpp`, `Rng.cpp` | Task scheduler, `th10.dat` archive, logging, RNG |
| Rendering & text | `AnmManager.cpp`, `AnmVmCreate.cpp`, `AnmVmVertexBuffer.cpp`, `TextRenderer.cpp`, `TextFontRelease.cpp`, `TransitionEffect.cpp` | ANM VM, D3D9 batching, GDI font/text rendering |
| Gameplay core | `GameManager.cpp`, `Player.cpp`, `PlayerShotTracking.cpp`, `BulletManager.cpp`, `BulletPoolCreation.cpp`, `ItemManager.cpp`, `SpellPanel.cpp` | Main gameplay loop, player/bullets/items |
| Enemies, ECL & lasers | `EclVm.cpp`, `Enemy.cpp`, `EnemyCallback.cpp`, `EnemyEclDispatcher.cpp`, `EnemyGameStateLifecycle.cpp`, `EnemyLaser.cpp`, `EnemyLaserManager.cpp`, `Laser*.cpp` | ECL VM, enemy scripts, line & infinite lasers |
| Stage, HUD & menus | `StageLifecycle.cpp`, `StageDraw.cpp`, `StageCameraDefaults.cpp`, `Gui.cpp`, `GuiScoreDisplay.cpp`, `FrontEnd.cpp`, `FrontEndLaunch.cpp`, `Ending.cpp`, `ReplayManager.cpp`, `GameStatistics.cpp` | Backgrounds, HUD, menus, replays, score records |
| Input & audio | `InputDirectInputInit.cpp`, `InputKeyboardSnapshot.cpp`, `InputPolling.cpp`, `InputWinmmJoystickCapabilities.cpp`, `Sound.cpp`, `ZWave.cpp` | DirectInput8, WinMM joystick, DirectSound |
| Product glue & leaves | `ProductBridge.cpp`, `ProductStrings.cpp`, `ProductGlobals.cpp`, `ProductBssLayout.asm`, `DecomphelpLeafAccessors.cpp` | Whole-program linkage, pure-C++ data, leaf accessors |
