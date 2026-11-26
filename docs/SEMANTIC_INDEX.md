# TH10 Semantic Index

Navigate TH10 v1.00a subsystems from their canonical source owners. For exact
function counts see [PROGRESS.md](PROGRESS.md); for build and Position
Independence contracts see [POSITION_INDEPENDENCE.md](POSITION_INDEPENDENCE.md);
for semantic batch rules see [SEMANTIC_RECONSTRUCTION.md](SEMANTIC_RECONSTRUCTION.md).

## Subsystems

| Subsystem | Primary headers & sources | Role |
| --- | --- | --- |
| Entry, window, D3D9 & config | [Main.hpp](../src/Main.hpp), [Main.cpp](../src/Main.cpp), [MainLifecycle.cpp](../src/MainLifecycle.cpp), [MainConfiguration.cpp](../src/MainConfiguration.cpp), [GameConfiguration.hpp](../src/GameConfiguration.hpp), [SupervisorModes.cpp](../src/SupervisorModes.cpp) | `WinMain`, Direct3D9 init/reset, `th10.cfg` load/save, mode switching |
| Callback scheduler (Chain) | [Chain.hpp](../src/Chain.hpp), [Chain.cpp](../src/Chain.cpp) | Priority-ordered calc/draw task lists |
| Archive & file I/O | [PbgArchive.hpp](../src/PbgArchive.hpp), [PbgArchive.cpp](../src/PbgArchive.cpp), [FileSystem.hpp](../src/FileSystem.hpp), [FileSystem.cpp](../src/FileSystem.cpp), [Lzss.cpp](../src/Lzss.cpp), [Decompress.cpp](../src/Decompress.cpp) | `th10.dat` archive decryption, LZSS decompression, file read/write |
| ANM interpreter & renderer | [AnmManager.hpp](../src/AnmManager.hpp), [AnmManager.cpp](../src/AnmManager.cpp), [AnmVmCreate.cpp](../src/AnmVmCreate.cpp), [AnmVmVertexBuffer.cpp](../src/AnmVmVertexBuffer.cpp), [TextRenderer.cpp](../src/TextRenderer.cpp) | Sprite/VM script interpreter, D3D9 vertex batching, texture banks, ASCII/text rendering |
| ECL virtual machine | [EclVm.hpp](../src/EclVm.hpp), [EclVm.cpp](../src/EclVm.cpp), [EnemyEclDispatcher.cpp](../src/EnemyEclDispatcher.cpp) | Enemy bytecode VM, subroutine calls, instruction dispatch |
| Enemies & spell cards | [Enemy.hpp](../src/Enemy.hpp), [Enemy.cpp](../src/Enemy.cpp), [EnemyCallback.cpp](../src/EnemyCallback.cpp), [EnemyGameStateLifecycle.cpp](../src/EnemyGameStateLifecycle.cpp), [SpellPanel.cpp](../src/SpellPanel.cpp) | Enemy lifecycle, motion/interpolation, spell card timers & bonuses |
| Bullets & lasers | [BulletManager.hpp](../src/BulletManager.hpp), [BulletManager.cpp](../src/BulletManager.cpp), [BulletPoolCreation.cpp](../src/BulletPoolCreation.cpp), [EnemyLaser.hpp](../src/EnemyLaser.hpp), [EnemyLaser.cpp](../src/EnemyLaser.cpp), [LaserLine.cpp](../src/LaserLine.cpp), [LaserType1.cpp](../src/LaserType1.cpp) | 2000-slot bullet pool, pattern emitters, line & infinite lasers |
| Player, shots & bombs | [Player.hpp](../src/Player.hpp), [Player.cpp](../src/Player.cpp), [PlayerShotTracking.cpp](../src/PlayerShotTracking.cpp), [ScriptViewerBombCreate.cpp](../src/ScriptViewerBombCreate.cpp) | Player movement, hitbox, options, shot/bomb callbacks |
| Items & popups | [ItemManager.hpp](../src/ItemManager.hpp), [ItemManager.cpp](../src/ItemManager.cpp), [ScriptViewerPopupCreate.cpp](../src/ScriptViewerPopupCreate.cpp) | Power/point/faith items, auto-collect, score popups |
| Stage, camera & fog | [StageScript.hpp](../src/StageScript.hpp), [StageLifecycle.cpp](../src/StageLifecycle.cpp), [StageDraw.cpp](../src/StageDraw.cpp), [StageCameraDefaults.cpp](../src/StageCameraDefaults.cpp) | `.std` stage backgrounds, camera matrices, fog interpolation |
| HUD, dialogue & ending | [Gui.hpp](../src/Gui.hpp), [Gui.cpp](../src/Gui.cpp), [GuiScoreDisplay.cpp](../src/GuiScoreDisplay.cpp), [Ending.hpp](../src/Ending.hpp), [Ending.cpp](../src/Ending.cpp) | Side HUD, boss gauge, `.msg` dialogue, endings & staff roll |
| Front-end, menus & replay | [FrontEnd.hpp](../src/FrontEnd.hpp), [FrontEnd.cpp](../src/FrontEnd.cpp), [FrontEndLaunch.cpp](../src/FrontEndLaunch.cpp), [ReplayManager.hpp](../src/ReplayManager.hpp), [ReplayManager.cpp](../src/ReplayManager.cpp), [GameStatistics.cpp](../src/GameStatistics.cpp) | Title/practice/music room/result menus, replay save/load, score records |
| Input & audio | [InputPolling.hpp](../src/InputPolling.hpp), [InputPolling.cpp](../src/InputPolling.cpp), [InputDirectInputInit.cpp](../src/InputDirectInputInit.cpp), [Sound.cpp](../src/Sound.cpp), [ZWave.cpp](../src/ZWave.cpp) | DirectInput8 + WinMM joystick polling, DirectSound BGM/SE streaming |
| Product build & globals | [ProductStrings.cpp](../src/ProductStrings.cpp), [ProductGlobals.cpp](../src/ProductGlobals.cpp), [ProductBridge.cpp](../src/ProductBridge.cpp), [ProductBssLayout.asm](../src/ProductBssLayout.asm), [th10.rc](../src/th10.rc) | Built-in Shift-JIS strings, initialized data tables, cross-TU dispatch, `.bss` layout, Win32 dialog |
| Leaf accessors | [DecomphelpLeafAccessors.cpp](../src/DecomphelpLeafAccessors.cpp) | Small compiler/accessor leaf functions (`th10::leaf::Fn004xxxxx`) |
