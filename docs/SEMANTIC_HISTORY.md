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
- **Commit**: `30ef38a7`
- **Scope**: `src/AnmManager.hpp`, `src/AnmManager.cpp`, `src/Startup.cpp`, `src/BulletPoolCreation.cpp`, `src/EnemyLaserManager.cpp`, `src/FrontEndLaunch.cpp`, `src/GameManager.cpp`, `src/Gui.cpp`, `src/Player.cpp`, `src/StageLifecycle.cpp`, `src/Sound.hpp`, `src/Chain.hpp`, `src/InputPolling.hpp`, `src/SupervisorModes.cpp`, `src/Main.cpp`; docs `docs/SOURCE_MAP.md`, `docs/SEMANTIC_HISTORY.md`.
- **Exact replay**:
  - `enemy-laser-manager-initialize`: 146/146 bytes, 0 diffs.
  - `enemy-laser-manager-create`: 91/91 bytes, 0 diffs.
- **Product + PI oracle**: `scripts/build.py`, `scripts/build.py --reverse-link`, `scripts/verify-pi.py`, headless Wine/Xvfb parity, `scripts/ci.py`, `git diff --check` all passed.
- **Known limitations addressed in follow-up batch**: `ChainCallbackResult` value 7 and `Chain.cpp` dispatchers, `Sound.cpp` `SE_COUNT` loops, `InputPolling.cpp` button masks, shared `resource.h`, and `ProductGlobals.cpp` ODR types were closed in the hardening batch below.

### ProductData ODR/PI hardening, protocol closure & semantic guard — 2026-11-26
- **Scope**:
  - **ProductData/PI**: `src/ProductGlobals.cpp` (removed duplicate local `GameErrorContext` and `ScriptViewerWorkerView` definitions in favor of canonical headers; trimmed `g_SoundCueMetadata_storage` to 93 dwords removing 37 dead un-relocated retail filename addresses), `scripts/verify-pi.py` (added real PE section verification, automatic check of all live `ProductGlobals.cpp` `/alternatename` pragmas, and all 5 initialized code/self pointer `.reloc` checks).
  - **Protocol closure**:
    - `th10::ChainCallbackResult`: added `CHAIN_CALLBACK_RESULT_INVOKE_DELETED_CALLBACK = 7`, symbolized all switch cases in `src/Chain.cpp` (`MainRunCalcChain` & `MainRunDrawChain`) and remaining `1` in `SupervisorModes.cpp::OnUpdate`.
    - `SupervisorGameMode` & `SupervisorLockSlot`: moved to `src/Main.hpp`, added `SUPERVISOR_MODE_UNINITIALIZED = -2` sentinel (applied in `MainRegisterChain`) and `SUPERVISOR_LOCK_CHAIN = 0` (applied in `src/Chain.cpp`).
    - `GameInputButtonMask`: applied in `src/InputPolling.cpp` (`MergeJoystickInput` WinMM & DirectInput paths) and `src/Main.cpp` (`INPUT_BUTTON_SCREENSHOT`); bit `0x04` (`controllerMappings[2]`) intentionally left unnamed pending TH10 evidence. Status: `Applied in InputPolling.cpp & Main.cpp; FrontEnd.cpp/Gui.cpp consumers remain open`.
    - `SoundEffectId`: applied `SE_COUNT` to `g_SoundFileNames[SE_COUNT]` and all three load/free loops in `src/Sound.cpp`.
    - Win32 Dialog 203 IDs: unified via `src/resource.h` included by both `src/th10.rc` and `src/Main.cpp`.
  - **Guard & Navigation**: added `scripts/check-semantic-protocols.py`, `docs/ANM_RESOURCE_INDEX.md`, `docs/SOUND_RESOURCE_INDEX.md`, updated `README.md`, `docs/SOURCE_MAP.md`, `docs/POSITION_INDEPENDENCE.md` (fixed local `file://` links), and `docs/RE_HANDOFF.md`.
- **Exact replay**: `input-controller-buttons` (264/264, 0 diffs), `merge-joystick-input` (833/833, 0 diffs), `poll-input-frame` (850/850, 0 diffs).
- **Product + PI oracle**: `scripts/build.py --fresh`, `scripts/build.py --reverse-link`, `scripts/verify-pi.py`, `scripts/check-semantic-protocols.py`, headless Wine/Xvfb parity on both PEs, `scripts/ci.py`, `git diff --check`.
