# TH10 Sound Resource Index

Sound-related integers in TH10 v1.00a belong to distinct namespaces and must
not be merged into a single enum:

| Namespace | Owner / Storage | Meaning |
| --- | --- | --- |
| `SoundEffectId` (`0..36`, `SE_COUNT = 37`) | `src/Sound.hpp`, `g_SoundFileNames[SE_COUNT]` in `src/Sound.cpp` | Index into the 37 static `se_*.wav` filenames and `SoundPlayerView::soundFileData[SE_COUNT]` |
| Sound queue cue ID | `g_SoundCueMetadata` (`src/ProductGlobals.cpp`), `EnemySoundQueueView` | Playback cue index passed to `QueueSoundCue` (maps to playback metadata and sample index; NOT always 1:1 with `SoundEffectId`) |
| DirectSound sample buffer slot | `SoundPlayerView::soundBuffers[128]` | Loaded DirectSound buffer slot |
| BGM preload slot | `SoundPlayerView::bgmPreloadFormats[16]` | Preloaded BGM track slot (`0..15`) |
| Command queue opcode | `SoundPlayerCommand::opcode` | Async command dispatched to `BgmPlayerThread` |

See `enum SoundEffectId` in [src/Sound.hpp](../src/Sound.hpp) for the full
1:1 mapping of `SE_PLST00` (`0`) through `SE_WATER` (`36`).
