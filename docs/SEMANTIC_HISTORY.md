# TH10 Semantic Reconstruction History

Dated evidence records for semantic/readability batches on
`semantic/readability-protocols`. For workflow and acceptance rules see
[SEMANTIC_RECONSTRUCTION.md](SEMANTIC_RECONSTRUCTION.md).

### Configuration options & Supervisor mode protocol — 2026-11-26
- **Scope**: `src/GameConfiguration.hpp`, `src/MainConfiguration.cpp`, `src/SupervisorModes.cpp`, `src/Main.cpp`; docs `docs/SEMANTIC_RECONSTRUCTION.md`, `docs/SEMANTIC_INDEX.md`.
- **Observed**: All 8 `GameConfigurationView::options` bits (`0x001`..`0x100`) and format version `0x100003` are directly evidenced by `MainLoadConfiguration` (`0x00420870`) log strings and `WinMain` Dialog 203 check. All 12 `SupervisorGameMode` IDs (`0, 1, 2, 3, 4, 7, 10, 11, 12, 13, 14, 15`) and lock slot `5` are directly evidenced by `MainSupervisorView::UpdateGameMode` (`0x004218D0`) and `OnUpdate` (`0x0041FF80`).
- **VC7.1 exact oracle**: `game-configuration-reset-defaults` 125/125 exact (0 diffs).
- **Product + PI oracle**: `scripts/build.py`, `scripts/build.py --reverse-link`, `scripts/verify-pi.py`, `scripts/ci.py`, `git diff --check` all passed.
- **Result**: Accepted (`ecc09c5f`).

### ANM resource bank slots, SE cue IDs, Chain callback results, Input masks & Dialog IDs — 2026-11-26
- **Scope**: `src/AnmManager.hpp`, `src/AnmManager.cpp`, `src/Startup.cpp`, `src/BulletPoolCreation.cpp`, `src/EnemyLaserManager.cpp`, `src/FrontEndLaunch.cpp`, `src/GameManager.cpp`, `src/Gui.cpp`, `src/Player.cpp`, `src/StageLifecycle.cpp`, `src/Sound.hpp`, `src/Chain.hpp`, `src/InputPolling.hpp`, `src/SupervisorModes.cpp`, `src/Main.cpp`; docs `docs/SOURCE_MAP.md`, `docs/SEMANTIC_HISTORY.md`.
- **Observed**:
  - `AnmResourceSlot` (`0, 1, 2, 3, 4, 5, 6, 7, 8, 25, 26, 28`) directly evidenced by `PreloadAnm(slot, "*.anm")` calls across 9 TUs.
  - `SoundEffectId` (`0..36`, `SE_COUNT = 37`) directly evidenced 1:1 by `g_SoundFileNames[37]` in `src/Sound.cpp`.
  - `th10::ChainCallbackResult` (`0..6`) directly evidenced by `Chain::RunCalcChain` / `RunDrawChain` and `SupervisorModes.cpp`.
  - `GameInputButtonMask` (`0x0001..0x0800`) directly evidenced by `InputPolling.cpp` and `Main.cpp`.
  - `MainStartupDialogControlId` (`201, 202, 203`) directly evidenced by `src/th10.rc` and `MainConfigurationDialogProc`.
- **Verification**: Affected exact units, both `th10.exe` and `th10-reordered.exe` builds, `scripts/verify-pi.py` (all 13 offset contracts + `.reloc`), headless Wine/Xvfb parity check, `scripts/ci.py`, and `git diff --check`.
