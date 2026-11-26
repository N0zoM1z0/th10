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
- **Authoritative zero-initialized `.bss` layout (`1` file)**:
  - [src/ProductBssLayout.asm](../src/ProductBssLayout.asm): unified
    zero-initialized `.bss` storage layout (187 symbol labels, 127 `DUP(?)`
    reservations; zero strings, zero data bytes, zero pointers). Retaining
    `.asm` preserves contiguous single-storage layout and VC7.1's 5 sub-4-byte
    struct-interior offsets (`+0x06`/`+0x32` in `g_FrontEndInput`;
    `+0x136`/`+0x6F6`/`+0x6F7` in `g_MainSupervisorView`).
- **Win32 resources (`2` files)**:
  - [src/resource.h](../src/resource.h) and [src/th10.rc](../src/th10.rc):
    shared Dialog 203 resource IDs (`IDD_MAIN_STARTUP_MODE`,
    `IDC_MAIN_FULLSCREEN`, `IDC_MAIN_ASK_ALWAYS`, `IDC_MAIN_WINDOWED`) used by
    both `rc.exe` and `src/Main.cpp`.

## Validation entry points by subsystem

| Subsystem | Production owner | Supporting headers | Validation entry |
| --- | --- | --- | --- |
| Game Configuration | `src/MainConfiguration.cpp` | `src/GameConfiguration.hpp` | `game-configuration-reset-defaults` + `scripts/build.py --fresh` |
| Supervisor & Entry | `src/Main.cpp`, `src/SupervisorModes.cpp` | `src/Main.hpp`, `src/resource.h` | `scripts/build.py --fresh` + `scripts/verify-pi.py` |
| Chain Scheduler | `src/Chain.cpp` | `src/Chain.hpp` | `chain-run-calc`, `chain-run-draw` + `scripts/check-semantic-protocols.py` |
| ANM Manager | `src/AnmManager.cpp` | `src/AnmManager.hpp`, `docs/ANM_RESOURCE_INDEX.md` | affected ANM exact units + `scripts/build.py --fresh` |
| Sound | `src/Sound.cpp`, `src/ZWave.cpp` | `src/Sound.hpp`, `docs/SOUND_RESOURCE_INDEX.md` | `scripts/check-semantic-protocols.py` + `scripts/build.py --fresh` |
| Input | `src/InputPolling.cpp` | `src/InputPolling.hpp` | `merge-joystick-input` + `scripts/build.py --fresh` |
| Product Data & PI | `src/ProductStrings.cpp`, `src/ProductGlobals.cpp`, `src/ProductBssLayout.asm` | `src/GameErrorContext.hpp`, `src/ScriptViewerWorker.hpp` | `scripts/build.py --fresh` + `--reverse-link` + `scripts/verify-pi.py` |

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
