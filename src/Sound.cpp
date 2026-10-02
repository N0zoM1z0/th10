#include <windows.h>
#include <dsound.h>
#include <string.h>

#include "Main.hpp"
#include "SoundFormat.hpp"

// TH10 retains ".\src\core\sound.cpp" source-path strings.  These names
// are maintained/descriptive; the original symbols are not recovered.
extern unsigned char g_MainSoundOwner[0x52d0];
extern HANDLE g_SoundDataLoadThreadHandle;

// The target thread body at 0x0043D080 returns with plain RET and ignores the
// API argument, so keep its source-level ABI unresolved.  The launcher only
// needs its address and explicitly adapts that address to CreateThread's type.
void SoundDataLoaderThread();

HANDLE StartSoundLoadThread()
{
    DWORD threadId;
    return g_SoundDataLoadThreadHandle =
        CreateThread(
            NULL,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(SoundDataLoaderThread),
            g_MainSoundOwner,
            0,
            &threadId);
}


class CWaveFile
{
public:
    __declspec(noinline) HRESULT Reopen(ThBgmFormat *format);
};

class CSound
{
public:
    virtual ~CSound();
    HRESULT Stop();

    CWaveFile *GetWaveFile()
    {
        return *reinterpret_cast<CWaveFile **>(
            reinterpret_cast<unsigned char *>(this) + 0x0c);
    }
};

class CSoundManager
{
public:
    LPDIRECTSOUND directSound;

    HRESULT CreateStreamingFromMemory(
        CSound **sound, BYTE *data, ULONG dataSize, ThBgmFormat *format,
        DWORD creationFlags, GUID algorithm, DWORD bufferCount,
        DWORD notifySize, HANDLE notifyEvent);
};

struct SoundPlayerView
{
    void *directSound;
    unsigned char unknown004[0x610 - 4];
    CSoundManager *manager;
    DWORD bgmThreadId;
    HANDLE bgmThreadHandle;
    unsigned char unknown61C[0x1e80 - 0x61c];
    ThBgmFormat *bgmPreloadFormats[16];
    BYTE *bgmPreloadAllocations[16];
    BYTE *bgmPreloadData[16];
    ULONG bgmPreloadAllocSizes[16];
    unsigned int loadedBgmSlot;
    ThBgmFormat *bgmFormats;
    unsigned char unknown1F88[0x4108 - 0x1f88];
    char bgmFileNames[16][256];
    char unknownNameSlot[256];
    CSound *bgm;
    HANDLE bgmUpdateEvent;
    unsigned char unknown5210[0x52d0 - 0x5210];

    int GetFmtIndexByName(const char *path);
    int ReopenBgm(const char *path);
    int LoadBgm(int index);
    void StopBgm();
    static DWORD WINAPI BgmPlayerThread(LPVOID parameter);
};

typedef char SoundPlayerViewSizeIs52D0[
    (sizeof(SoundPlayerView) == 0x52d0) ? 1 : -1];
typedef char SoundPlayerBgmOffsets[
    (offsetof(SoundPlayerView, manager) == 0x610 &&
     offsetof(SoundPlayerView, bgmPreloadFormats) == 0x1e80 &&
     offsetof(SoundPlayerView, bgmPreloadAllocations) == 0x1ec0 &&
     offsetof(SoundPlayerView, bgmPreloadData) == 0x1f00 &&
     offsetof(SoundPlayerView, bgmPreloadAllocSizes) == 0x1f40 &&
     offsetof(SoundPlayerView, loadedBgmSlot) == 0x1f80 &&
     offsetof(SoundPlayerView, bgmFileNames) == 0x4108) ? 1 : -1];

void SoundPlayerView::StopBgm()
{
    if (bgm == NULL)
        return;

    bgm->Stop();
    if (bgmThreadHandle != NULL)
    {
        PostThreadMessageA(bgmThreadId, WM_QUIT, 0, 0);
        while (WaitForSingleObject(bgmThreadHandle, 256) != WAIT_OBJECT_0)
            PostThreadMessageA(bgmThreadId, WM_QUIT, 0, 0);

        CloseHandle(bgmThreadHandle);
        CloseHandle(bgmUpdateEvent);
        bgmThreadHandle = NULL;
    }

    if (bgm != NULL)
    {
        delete bgm;
        bgm = NULL;
    }
}


int SoundPlayerView::GetFmtIndexByName(const char *path)
{
    char basename[128];
    const char *separator = strrchr(path, '/');
    if (separator == NULL)
        separator = strrchr(path, '\\');

    if (separator == NULL)
        strcpy(basename, path);
    else
        strcpy(basename, separator + 1);

    int index = 0;
    while (bgmFormats[index].name[0] != '\0')
    {
        if (strcmp(bgmFormats[index].name, basename) == 0)
            break;
        ++index;
    }

    if (bgmFormats[index].name[0] == '\0')
        index = 0;
    return index;
}

int SoundPlayerView::ReopenBgm(const char *path)
{
    if (bgm == NULL)
        return -1;

    const int index = GetFmtIndexByName(path);
    ThBgmFormat *format = &bgmFormats[index];
    CWaveFile *waveFile = bgm->GetWaveFile();
    waveFile->Reopen(format);
    return 0;
}

int SoundPlayerView::LoadBgm(int index)
{
    if (manager == NULL)
        return -1;
    if (g_MainSupervisorView.configUnknown13B[0] == 0)
        return -1;
    if (directSound == NULL)
        return -1;

    if ((g_MainSupervisorView.options & 0x10) == 0)
        return ReopenBgm(bgmFileNames[index]);

    if (bgmPreloadAllocations[index] == NULL)
        return -1;

    ThBgmFormat *format = bgmPreloadFormats[index];
    DWORD blockAlign = format->format.nBlockAlign;
    DWORD notifySize = format->format.nSamplesPerSec * 4 * blockAlign / 16;
    notifySize -= notifySize % blockAlign;

    bgmUpdateEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    bgmThreadHandle = CreateThread(
        NULL, 0, BgmPlayerThread, g_MainSupervisorView.gameWindow,
        0, &bgmThreadId);

    HRESULT result = manager->CreateStreamingFromMemory(
        &bgm, bgmPreloadData[index], bgmPreloadAllocSizes[index], format,
        DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY,
        GUID_NULL, 16, notifySize, bgmUpdateEvent);
    if (result < 0)
        return -1;

    loadedBgmSlot = index;
    return 0;
}
