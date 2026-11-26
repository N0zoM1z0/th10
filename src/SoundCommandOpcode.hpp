#ifndef TH10_SOUND_COMMAND_OPCODE_HPP
#define TH10_SOUND_COMMAND_OPCODE_HPP

// Directly evidenced by SoundPlayerView::ProcessQueues() (src/Sound.cpp)
// and all QueueCommand / FrontEndQueueSoundCommand callers across
// MainLifecycle.cpp, GameManager.cpp, and FrontEnd.cpp.
enum SoundPlayerCommandOpcode
{
    SOUND_COMMAND_NONE = 0,
    SOUND_COMMAND_PRELOAD_BGM = 1,
    SOUND_COMMAND_LOAD_BGM = 2,
    SOUND_COMMAND_STOP_BGM = 3,
    SOUND_COMMAND_RELEASE_BGM = 4,
    SOUND_COMMAND_FADE_BGM = 5,
    SOUND_COMMAND_PAUSE_BGM = 6,
    SOUND_COMMAND_UNPAUSE_BGM = 7,
    SOUND_COMMAND_SET_BGM_VOLUME = 8
};

#endif
