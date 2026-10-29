#ifndef TH10_SOUND_HPP
#define TH10_SOUND_HPP

#include <stddef.h>
#include <windows.h>
#include <dsound.h>
#include "SoundFormat.hpp"

class CSound;
class CSoundManager;

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
    unsigned char *soundFileData[37];
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
    static DWORD WINAPI BgmPlayerThread(LPVOID parameter);
};


#endif
