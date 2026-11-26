#ifndef TH10_SOUND_HPP
#define TH10_SOUND_HPP

#include <stddef.h>
#include <windows.h>
#include <dsound.h>
#include "SoundFormat.hpp"

class CSound;
class CSoundManager;

// Directly evidenced 1:1 by g_SoundFileNames[37] in src/Sound.cpp.
enum SoundEffectId
{
    SE_PLST00   = 0,  // se_plst00.wav
    SE_ENEP00   = 1,  // se_enep00.wav
    SE_PLDEAD00 = 2,  // se_pldead00.wav
    SE_POWER0   = 3,  // se_power0.wav
    SE_POWER1   = 4,  // se_power1.wav
    SE_TAN00    = 5,  // se_tan00.wav
    SE_TAN01    = 6,  // se_tan01.wav
    SE_TAN02    = 7,  // se_tan02.wav
    SE_OK00     = 8,  // se_ok00.wav
    SE_CANCEL00 = 9,  // se_cancel00.wav
    SE_SELECT00 = 10, // se_select00.wav
    SE_GUN00    = 11, // se_gun00.wav
    SE_CAT00    = 12, // se_cat00.wav
    SE_LAZER00  = 13, // se_lazer00.wav
    SE_LAZER01  = 14, // se_lazer01.wav
    SE_ENEP01   = 15, // se_enep01.wav
    SE_DAMAGE00 = 16, // se_damage00.wav
    SE_ITEM00   = 17, // se_item00.wav
    SE_KIRA00   = 18, // se_kira00.wav
    SE_KIRA01   = 19, // se_kira01.wav
    SE_KIRA02   = 20, // se_kira02.wav
    SE_TIMEOUT  = 21, // se_timeout.wav
    SE_GRAZE    = 22, // se_graze.wav
    SE_POWERUP  = 23, // se_powerup.wav
    SE_PAUSE    = 24, // se_pause.wav
    SE_CARDGET  = 25, // se_cardget.wav
    SE_OPTION   = 26, // se_option.wav
    SE_DAMAGE01 = 27, // se_damage01.wav
    SE_TIMEOUT2 = 28, // se_timeout2.wav
    SE_INVALID  = 29, // se_invalid.wav
    SE_SLASH    = 30, // se_slash.wav
    SE_CH00     = 31, // se_ch00.wav
    SE_CH01     = 32, // se_ch01.wav
    SE_HINT00   = 33, // se_hint00.wav
    SE_EXTEND   = 34, // se_extend.wav
    SE_BONUS3   = 35, // se_bonus3.wav
    SE_WATER    = 36, // se_water.wav
    SE_COUNT    = 37
};

struct SoundPlayerCommand
{
    int opcode;
    int argument;
    int step;
    char path[256];
};


struct SoundPlayerView
{
    // Partial target-offset view. Unnamed fields retain unresolved ownership.
    void *directSound;
    unsigned char unknown004[4];
    LPDIRECTSOUNDBUFFER soundBuffers[128];
    LPDIRECTSOUNDBUFFER duplicateSoundBuffers[128];
    int unconsumedSoundMetadata[128];
    LPDIRECTSOUNDBUFFER initialSoundBuffer;
    HWND gameWindow;
    CSoundManager *manager;
    DWORD bgmThreadId;
    HANDLE bgmThreadHandle;
    int unknown61C;
    int soundQueue[12];
    int soundQueueRequestCounts[12];
    int soundQueuePanData[12][128];
    ThBgmFormat *bgmPreloadFormats[16];
    BYTE *bgmPreloadAllocations[16];
    BYTE *bgmPreloadData[16];
    ULONG bgmPreloadAllocSizes[16];
    unsigned int loadedBgmSlot;
    ThBgmFormat *bgmFormats;
    SoundPlayerCommand commandQueue[32];
    char bgmFileNames[16][256];
    char unknownNameSlot[256];
    CSound *bgm;
    HANDLE bgmUpdateEvent;
    unsigned char unknown5210[8];
    HANDLE soundSetupThreadHandle;
    HANDLE soundDataThreadHandle;
    DWORD soundSetupThreadId;
    int soundDataLoadState;
    HWND soundSetupWindow;
    int soundSetupFinished;
    unsigned char *soundFileData[SE_COUNT];
    int bgmVolume;
    int sfxVolume;
    int unknown52CC;

    int Initialize(HWND window);
    int LoadSoundSample(int index, const char *path);
    int GetFmtIndexByName(const char *path);
    int ReopenBgm(const char *path);
    int OpenDiskBgm(const char *path);
    int PreparePlayback();
    int LoadBgm(int index);
    int PreloadBgm(int index, const char *path);
    int __stdcall ProcessQueues();
    void QueueCommand(const char *path, int opcode, int argument);
    void StopBgm();
    void ServiceBgmFades();
    static DWORD WINAPI BgmPlayerThread(LPVOID parameter);
};


#endif
