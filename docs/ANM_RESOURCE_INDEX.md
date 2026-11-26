# TH10 ANM Resource Index

In TH10 v1.00a, integer IDs used across the ANM subsystem belong to **separate
namespaces** and must not be conflated:

1. **ANM Manager bank slot (`AnmResourceSlot` in `src/AnmManager.hpp`)**:
   passed to `AnmRenderManagerView::PreloadAnm(slot, path)`.
2. **File-local script ID**: index of a script inside one loaded `.anm` file.
3. **Sprite ID**: index of a sprite entry inside one loaded `.anm` file.
4. **Runtime VM ID (`AnmVmIdView`)**: handle returned by VM creation and
   resolved via `AnmRenderManagerView::FindVm(id)`.

## Evidenced `AnmResourceSlot` assignments

| Slot | Enum | Loaded `.anm` resource | Confirmed callers |
| --- | --- | --- | --- |
| `0` | `ANM_SLOT_TEXT` | `text.anm` | `AnmManager.cpp`, `Startup.cpp` |
| `1` | `ANM_SLOT_SIG` | `sig.anm` | `Startup.cpp` |
| `2` | `ANM_SLOT_ASCII` | `ascii.anm` | `AnmManager.cpp` |
| `3` | `ANM_SLOT_CAPTURE` | `capture.anm` | `AnmManager.cpp` |
| `4` | `ANM_SLOT_STAGE_PRIMARY` | `stage*.anm` (bank 0) | `StageLifecycle.cpp` |
| `5` | `ANM_SLOT_STAGE_SECONDARY` | `stage*.anm` (bank 1) | `StageLifecycle.cpp` |
| `6` | `ANM_SLOT_FRONT` | `front.anm` | `Startup.cpp`, `Gui.cpp` |
| `7` | `ANM_SLOT_BULLET` | `bullet.anm` | `Startup.cpp`, `BulletPoolCreation.cpp`, `EnemyLaserManager.cpp`, `GameManager.cpp` |
| `8` | `ANM_SLOT_PLAYER` | `pl00.anm` / `pl01.anm` | `Player.cpp` |
| `25` | `ANM_SLOT_TITLE` | `title.anm` | `FrontEndLaunch.cpp` |
| `26` | `ANM_SLOT_TITLE_VERTICAL` | `title_v.anm` | `FrontEndLaunch.cpp` |
| `28` | `ANM_SLOT_STAGE_UI` | `st0*logo.anm` / stage UI | `Gui.cpp` |
