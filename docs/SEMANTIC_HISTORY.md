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
- **Commit**: `780e35a0`
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

### Exact replay closure, lock-slot audit, fail-closed PI & CI guard — 2026-11-26
- **Commit**: `d3fd54d9`
- **Scope**:
  - **Lock-slot evidence & consumer closure**: extracted `SupervisorLockSlot` to `src/SupervisorLockSlot.hpp` (included by `src/Main.hpp`), keeping only the 3 directly evidenced slots (`SUPERVISOR_LOCK_CHAIN = 0`, `SUPERVISOR_LOCK_LOG = 3`, `SUPERVISOR_LOCK_MODE = 5`) and removing unverified `SUPERVISOR_LOCK_FILE_IO = 2` (`gReplayFileCriticalSection` is an independent BSS symbol, not `criticalSections[2]`). Applied `SUPERVISOR_LOCK_LOG` in `src/GameErrorContext.cpp` and `SUPERVISOR_LOCK_CHAIN` in `src/ScriptViewer.cpp` and `src/ReplayManager.cpp`.
  - **Data layout clarity**: documented `g_SoundCueMetadata_storage` in `src/ProductGlobals.cpp` (46 complete 8-byte `SoundCueMetadata` records + 1 trailing observed 4-byte word `0x32FCE0u`, semantic role unresolved) with a compile-time size assertion.
  - **PI verifier hardening**: `scripts/verify-pi.py` now cross-checks symbol section ownership against both `.map` and the PE section table on both `th10.exe` and `th10-reordered.exe`, fails closed on all 65 `/alternatename` pragmas in `src/ProductGlobals.cpp`, restricts `.reloc` checks strictly to `IMAGE_REL_BASED_HIGHLOW` (`type == 3`), and validates all 37 `g_SoundFileNames[0..36]` string pointers (42 total verified `HIGHLOW` pointer contracts).
  - **Semantic guard & CI**: wired `scripts/check-semantic-protocols.py` into `.github/workflows/ci.yml` and expanded it to check hex/decimal `case` labels, enum value domains (including forbidding `SUPERVISOR_LOCK_FILE_IO`), `resource.h` IDs, `Main.cpp` `INPUT_BUTTON_SCREENSHOT`, and all lock consumers (`Chain.cpp`, `ScriptViewer.cpp`, `ReplayManager.cpp`, `GameErrorContext.cpp`).
  - **Match recipe fix**: corrected `supervisor-update-game-mode` relocation base targets in `config/match-units.toml` (`g_GameScoreState` base `0x00474C40` + addends `0x40`/`0x3C`).
  - **Docs**: tightened `docs/POSITION_INDEPENDENCE.md` and `docs/SEMANTIC_RECONSTRUCTION.md` (exact verified `.reloc` pointer counts, startup parity scope, unified `--fresh` header-rebuild rule).
- **VC7.1 exact oracle (13/13 affected units, all 0 diffs)**:
  - `chain-run-calc`: 308/308
  - `chain-run-draw`: 268/268
  - `supervisor-update-callback`: 113/113
  - `main-register-chain`: 204/204
  - `supervisor-update-game-mode`: 666/666
  - `main-winmain`: 1389/1389
  - `main-configuration-dialog-proc`: 233/233
  - `main-present`: 236/236
  - `sound-player-initialize`: 786/786
  - `sound-data-loader-thread`: 107/107
  - `sound-release-owner`: 301/301
  - `decomphelp-game-error-log`: 201/201
  - `decomphelp-game-error-fatal`: 201/201
- **Product + PI oracle**: `scripts/build.py --fresh`, `scripts/build.py --reverse-link`, hardened `scripts/verify-pi.py`, `scripts/check-semantic-protocols.py`, `scripts/ci.py`, `git diff --check`.

### Repo-wide Supervisor lock closure, Sound metadata +4 PI contract & guard precision — 2026-11-26
- **Commit**: `3dada38b`
- **Scope**:
  - **Repo-wide Supervisor lock closure**: moved `#include "SupervisorLockSlot.hpp"` to the top of `src/Main.hpp` (outside `#pragma pack(push, 4)`), updated evidence comments in `src/SupervisorLockSlot.hpp`, and replaced all 124 remaining bare `[0]` / `[5]` lock array accesses across all 15 consumer TUs (`AnmManager.cpp`, `Chain.cpp`, `Ending.cpp`, `EnemyGameStateLifecycle.cpp`, `EnemyLaserManager.cpp`, `FrontEndLifecycle.cpp`, `GameManager.cpp`, `Gui.cpp`, `Hint.cpp`, `Main.cpp`, `ResultMenuCleanup.cpp`, `ScriptViewerLifecycle.cpp`, `StageLifecycle.cpp`, `Startup.cpp`, `TransitionEffect.cpp`). Zero bare `0`/`3`/`5` lock indices remain in `src/*.cpp`.
  - **Sound metadata contiguous 376-byte layout contract & real bug fix**: documented all three overlapping views (`SoundPlaybackMetadata[47]`, `EnemySoundCueMetadataView[47]`, `SoundCueMetadata` @ `+4`, trailing `0x32FCE0u` = 47th entry's `volume=0xFCE0` / `cueValue=0x0032`) in `src/ProductGlobals.cpp` and added a post-link `+4` offset + `.data` section + 376-byte content contract to `scripts/verify-pi.py`. The new PI contract immediately caught a real VC7.1 4-byte alignment gap (`delta=+8`) between `g_EnemySoundCueMetadata_head` and `g_SoundCueMetadata_storage[93]`; fixed via `#pragma data_seg(".data$th10_soundmeta")` with a 12-byte leading alignment pad so both standard and reverse-linked PEs satisfy `delta=+4`.
  - **Semantic guard precision**: upgraded `scripts/check-semantic-protocols.py` to enforce exact enum name/value mappings (`SupervisorLockSlot`, `ChainCallbackResult`, `SoundEffectId`, `GameInputButtonMask`), repo-wide `src/*.cpp` lock-index scanning, `Chain.cpp` calc/draw case sets, the 37-entry `g_SoundFileNames` table, and `InputPolling.cpp` `MergeJoystickInput` button masks.
- **VC7.1 exact oracle (21/21 affected units, all 0 diffs — including 2 closed from `d3fd54d9`)**:
  - `script-viewer-destructor`: 412/412
  - `replay-manager-destructor`: 367/367
  - `decomphelp-chain-remove-all`: 82/82
  - `stage-transition-effect-cleanup`: 127/127
  - `main-get-timestamp-fps`: 282/282
  - `main-delete-fps-counter`: 84/84
  - `anm-render-manager-destructor`: 206/206
  - `ending-destructor`: 362/362
  - `enemy-spell-panel-destructor`: 402/402
  - `enemy-laser-linked-cleanup`: 195/195
  - `frontend-delete`: 18/18
  - `game-cleanup-owner-40af00`: 137/137
  - `game-manager-cleanup`: 1105/1105
  - `gui-destructor`: 574/574
  - `hint-cleanup-owner`: 346/346
  - `result-menu-cleanup`: 311/311
  - `script-viewer-bullet-destructor`: 236/236
  - `script-viewer-bomb-destructor`: 137/137
  - `script-viewer-popup-owner-destructor`: 232/232
  - `stage-lifecycle-destructor`: 499/499
  - `startup-worker-destructor`: 417/417
- **Product + PI oracle**: `scripts/build.py --fresh`, `scripts/build.py --reverse-link`, hardened `scripts/verify-pi.py` (including `+4` 376-byte Sound metadata contract on both PEs), upgraded `scripts/check-semantic-protocols.py`, `scripts/ci.py`, `git diff --check`.

### SoundPlayerCommandOpcode protocol closure — 2026-11-26
- **Scope**:
  - Extracted `SoundPlayerCommandOpcode` (`SOUND_COMMAND_NONE = 0` .. `SOUND_COMMAND_SET_BGM_VOLUME = 8`) to `src/SoundCommandOpcode.hpp` (included by `src/Sound.hpp` and `src/FrontEnd.cpp`).
  - Symbolized all 22 bare numeric opcode sites across `src/Sound.cpp`, `src/MainLifecycle.cpp`, `src/GameManager.cpp`, and `src/FrontEnd.cpp`.
  - Added exact-enum and repo-wide zero-bare-opcode enforcement to `scripts/check-semantic-protocols.py`.
- **VC7.1 exact oracle (13/13 clean units 0 diffs; 2 pre-existing `ResultTransitions` LTCG shift)**:
  - `sound-process-queues`: 1452/1452
  - `sound-player-queue-command`: 104/104
  - `main-release-game`: 293/293
  - `game-prepare-dummy-music`: 41/41
  - `game-manager-cleanup`: 1105/1105
  - `game-manager-update-body`: 1544/1544
  - `frontend-options-refresh`: 3999/3999
  - `front-end-load-music`: 120/120
  - `front-end-play-music`: 73/73
  - `front-end-queue-scaled-sound`: 84/84
  - `front-end-update-music-room`: 2796/2796
  - `result-menu-open-pause`: 382/382
  - `result-menu-resume`: 74/74
  - Note: `result-menu-open-game-over` and `game-complete-stage-transition` (both in the `ResultTransitions` `/GL` image) exhibit a pre-existing 2-byte LTCG register-allocation shift at `RequestCaptureRectangle` (`offset 210 -> 212`) introduced on `main` (`69f5d411`) when `g_AsciiManagerView` in `src/AnmManager.cpp` was converted from a tentative definition to `extern` for PI single-storage; the `QueueCommand` call sites themselves are byte-identical.
- **Product + PI oracle**: `scripts/build.py --fresh`, `scripts/build.py --reverse-link`, `scripts/verify-pi.py`, `scripts/check-semantic-protocols.py`, `scripts/ci.py`, `git diff --check`.
