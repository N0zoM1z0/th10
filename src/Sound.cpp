#include <windows.h>
#include <dsound.h>
#include <string.h>
#include <stdlib.h>

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
    unsigned char unknown000[0x90];
    ThBgmFormat *format;

    __declspec(noinline) HRESULT Reopen(ThBgmFormat *format);
};

class CSound
{
protected:
    LPDIRECTSOUNDBUFFER *soundBuffers;
    DWORD bufferSize;
    CWaveFile *waveFile;
    DWORD bufferCount;

public:
    int fadeProgress;
    int fadeTotal;
    int fadeType;
    unsigned char unknown020[0x30 - 0x20];
    BOOL m_bIsPlaying;
    unsigned char unknown034[0x5c - 0x34];

    virtual ~CSound();
    HRESULT Stop();
    HRESULT Pause();
    HRESULT Unpause();
    HRESULT Play(DWORD priority, DWORD flags);
    HRESULT SetVolume(int volume);
    HRESULT FillBufferWithSound(
        LPDIRECTSOUNDBUFFER buffer, BOOL repeatIfBufferLarger);

    CWaveFile *GetWaveFile()
    {
        return waveFile;
    }

    LPDIRECTSOUNDBUFFER GetBuffer(DWORD index)
    {
        if (soundBuffers == NULL || index >= bufferCount)
            return NULL;
        return soundBuffers[index];
    }

    void FadeOut(float seconds)
    {
        fadeType = 1;
        const int frames = static_cast<int>(seconds * 60.0f);
        fadeProgress = frames;
        fadeTotal = frames;
    }
};

class CStreamingSound : public CSound
{
public:
    unsigned char unknown05C[0x74 - 0x5c];
    BOOL isLocked;

    HRESULT InitSoundBuffers();
    HRESULT Reset();
    HRESULT HandleWaveStreamNotification(BOOL loopedPlay);
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

// Values are read from the target queue switch; the names are descriptive.
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

struct SoundPlayerCommand
{
    int opcode;
    int argument;
    int step;
    char path[256];
};

struct SoundCueMetadata
{
    short volume;
    short unknown002;
    int unknown004;
};
extern SoundCueMetadata g_SoundCueMetadata[];

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
    unsigned char unknown5210[0x52c4 - 0x5210];
    int bgmVolume;
    int sfxVolume;
    int unknown52CC;

    int GetFmtIndexByName(const char *path);
    int ReopenBgm(const char *path);
    int LoadBgm(int index);
    int PreloadBgm(int index, const char *path);
    int ProcessQueues();
    void StopBgm();
    static DWORD WINAPI BgmPlayerThread(LPVOID parameter);
};

typedef char SoundPlayerViewSizeIs52D0[
    (sizeof(SoundPlayerView) == 0x52d0) ? 1 : -1];
typedef char SoundPlayerBgmOffsets[
    (sizeof(CSound) == 0x5c &&
     sizeof(CStreamingSound) == 0x78 &&
     offsetof(CSound, m_bIsPlaying) == 0x30 &&
     offsetof(CStreamingSound, isLocked) == 0x74 &&
     sizeof(SoundPlayerCommand) == 0x10c &&
     sizeof(SoundCueMetadata) == 8 &&
     offsetof(SoundPlayerView, duplicateSoundBuffers) == 0x208 &&
     offsetof(SoundPlayerView, manager) == 0x610 &&
     offsetof(SoundPlayerView, soundQueue) == 0x620 &&
     offsetof(SoundPlayerView, soundQueueRequestCounts) == 0x650 &&
     offsetof(SoundPlayerView, soundQueuePanData) == 0x680 &&
     offsetof(SoundPlayerView, bgmPreloadFormats) == 0x1e80 &&
     offsetof(SoundPlayerView, bgmPreloadAllocations) == 0x1ec0 &&
     offsetof(SoundPlayerView, bgmPreloadData) == 0x1f00 &&
     offsetof(SoundPlayerView, bgmPreloadAllocSizes) == 0x1f40 &&
     offsetof(SoundPlayerView, bgmFormats) == 0x1f84 &&
     offsetof(SoundPlayerView, unknownNameSlot) == 0x5108 &&
     offsetof(SoundPlayerView, loadedBgmSlot) == 0x1f80 &&
     offsetof(SoundPlayerView, commandQueue) == 0x1f88 &&
     offsetof(SoundPlayerView, bgmFileNames) == 0x4108 &&
     offsetof(SoundPlayerView, bgm) == 0x5208 &&
     offsetof(SoundPlayerView, bgmUpdateEvent) == 0x520c &&
     offsetof(SoundPlayerView, bgmVolume) == 0x52c4) ? 1 : -1];

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

// Target 0x0043D7D0. Comparison uses this owner, but the filename copy uses
// the global sound owner. Seek/read/close results are intentionally unchecked.
int SoundPlayerView::PreloadBgm(int index, const char *path)
{
    if (bgmPreloadAllocations[index] != NULL &&
        strcmp(path, bgmFileNames[index]) == 0)
        return 0;

    strcpy(reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)
               ->bgmFileNames[index], path);
    if ((g_MainSupervisorView.options & 0x10) == 0 || manager == NULL)
        return 0;

    if (bgmPreloadAllocations[index] != NULL)
    {
        free(bgmPreloadAllocations[index]);
        bgmPreloadAllocations[index] = NULL;
    }
    HANDLE file = CreateFileA(
        unknownNameSlot, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);
    if (file == INVALID_HANDLE_VALUE)
        return -1;

    const int formatIndex = GetFmtIndexByName(path);
    SetFilePointer(file, bgmFormats[formatIndex].startOffset,
                   NULL, FILE_BEGIN);
    BYTE *allocation = static_cast<BYTE *>(
        malloc(bgmFormats[formatIndex].preloadAllocSize));
    if (allocation == NULL)
    {
        CloseHandle(file);
        return -1;
    }

    DWORD bytesRead;
    ReadFile(file, allocation, bgmFormats[formatIndex].preloadAllocSize,
             &bytesRead, NULL);
    CloseHandle(file);

    bgmPreloadFormats[index] = &bgmFormats[formatIndex];
    bgmPreloadAllocations[index] = allocation;
    bgmPreloadData[index] = allocation;
    bgmPreloadAllocSizes[index] = bgmPreloadFormats[index]->preloadAllocSize;
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

int SoundPlayerView::ProcessQueues()
{
    if (manager == NULL)
        return 0;

    SoundPlayerCommand *command = commandQueue;

process_command:
    bool processNext = false;
    switch (command->opcode)
    {
    case SOUND_COMMAND_PRELOAD_BGM:
        if ((g_MainSupervisorView.options & 0x10) != 0)
        {
            if (command->step != 0)
            {
                ++command->step;
                break;
            }
            StopBgm();
        }
        PreloadBgm(command->argument, command->path);
        processNext = true;
        goto remove_command;

    case SOUND_COMMAND_LOAD_BGM:
        if ((g_MainSupervisorView.options & 0x10) != 0 &&
            command->argument >= 0)
        {
            if (command->step == 0)
            {
                if (LoadBgm(command->argument) != 0)
                    goto remove_command;
            }
            else if (command->step == 2)
            {
                if (bgm != NULL &&
                    FAILED(static_cast<CStreamingSound *>(bgm)->Reset()))
                    goto remove_command;
            }
            else if (command->step == 5)
            {
                LPDIRECTSOUNDBUFFER buffer = bgm->GetBuffer(0);
                command->argument =
                    bgm->GetWaveFile()->format->totalLength != 0;
                if (FAILED(bgm->FillBufferWithSound(buffer, command->argument)))
                    goto remove_command;
            }
            else if (command->step == 7)
            {
                bgm->Play(0, DSBPLAY_LOOPING);
            }
            else if (command->step >= 20)
            {
                goto remove_command;
            }
        }
        else
        {
            if (bgm == NULL)
                goto remove_command;
            if (command->step == 0)
                bgm->Stop();
            else if (command->step == 1)
            {
                if (static_cast<CStreamingSound *>(bgm)->isLocked)
                    break;
                static_cast<CStreamingSound *>(bgm)->InitSoundBuffers();
            }
            else if (command->step == 2)
            {
                const char *path = command->argument >= 0
                    ? bgmFileNames[command->argument] : command->path;
                int index = GetFmtIndexByName(path);
                bgm->GetWaveFile()->Reopen(&bgmFormats[index]);
            }
            else if (command->step == 3)
            {
                LPDIRECTSOUNDBUFFER buffer = bgm->GetBuffer(0);
                static_cast<CStreamingSound *>(bgm)->Reset();
                command->argument =
                    bgm->GetWaveFile()->format->totalLength != 0;
                if (FAILED(bgm->FillBufferWithSound(buffer, command->argument)))
                    goto remove_command;
            }
            else if (command->step == 4)
                bgm->Play(0, DSBPLAY_LOOPING);
            else if (command->step >= 7)
                goto remove_command;
        }
        ++command->step;
        break;

    case SOUND_COMMAND_STOP_BGM:
        if (bgm == NULL)
            goto remove_command;
        if (command->step == 0)
            bgm->Stop();
        else if (command->step == 1)
            goto remove_command;
        ++command->step;
        break;

    case SOUND_COMMAND_RELEASE_BGM:
        if (bgm == NULL)
            goto remove_command;
        if (command->step == 0)
            bgm->Stop();
        else if (command->step == 1)
        {
            if (bgmThreadHandle == NULL)
                goto remove_command;
            PostThreadMessageA(bgmThreadId, WM_QUIT, 0, 0);
        }
        else if (command->step == 2)
        {
            if (WaitForSingleObject(bgmThreadHandle, 256) != WAIT_OBJECT_0)
            {
                PostThreadMessageA(bgmThreadId, WM_QUIT, 0, 0);
                --command->step;
            }
            else
                bgmThreadHandle = NULL;
        }
        else if (command->step == 3)
        {
            CloseHandle(bgmThreadHandle);
            CloseHandle(bgmUpdateEvent);
            bgmThreadHandle = NULL;
            if (bgm != NULL)
            {
                delete bgm;
                bgm = NULL;
            }
        }
        else if (command->step == 10)
            goto remove_command;
        ++command->step;
        break;

    case SOUND_COMMAND_FADE_BGM:
        if (reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->bgm != NULL)
            reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->bgm->FadeOut(
                static_cast<float>(command->argument));
        goto remove_command;

    case SOUND_COMMAND_PAUSE_BGM:
        if (g_MainSupervisorView.configUnknown13B[0] == 1)
        {
            if (static_cast<CStreamingSound *>(bgm)->isLocked)
                break;
            if (bgm != NULL)
                bgm->Pause();
        }
        goto remove_command;

    case SOUND_COMMAND_UNPAUSE_BGM:
        if (g_MainSupervisorView.configUnknown13B[0] == 1)
        {
            if (static_cast<CStreamingSound *>(bgm)->isLocked)
                break;
            if (bgm != NULL)
                bgm->Unpause();
        }
        goto remove_command;

    case SOUND_COMMAND_SET_BGM_VOLUME:
        if (bgm != NULL)
            bgm->SetVolume(bgmVolume);
        goto remove_command;

    default:
        break;

    remove_command:
        for (int i = 0; i < 31; ++i, ++command)
        {
            if (command->opcode == SOUND_COMMAND_NONE)
                break;
            memcpy(command, command + 1, sizeof(*command));
        }
        if (processNext)
            goto process_command;
    }

    if (g_MainSupervisorView.configUnknown13B[1] == 0)
        return commandQueue[0].opcode;

    for (int i = 0; i < 12; ++i)
    {
        const int soundIndex = soundQueue[i];
        if (soundIndex < 0)
            break;

        int count = soundQueueRequestCounts[i];
        soundQueue[i] = -1;
        if (count < 0)
        {
            if (duplicateSoundBuffers[soundIndex] != NULL)
                duplicateSoundBuffers[soundIndex]->Stop();
            soundQueueRequestCounts[i] = 0;
            continue;
        }

        int pan = 0;
        for (int j = 0; j < count; ++j)
            pan += soundQueuePanData[i][j];
        pan /= count;
        soundQueueRequestCounts[i] = 0;

        LPDIRECTSOUNDBUFFER buffer = duplicateSoundBuffers[soundIndex];
        if (buffer == NULL)
            continue;

        buffer->Stop();
        buffer->SetCurrentPosition(0);
        buffer->SetPan(pan);

        const int volume = reinterpret_cast<SoundPlayerView *>(
            g_MainSoundOwner)->sfxVolume;
        if (volume == 0)
            buffer->SetVolume(-10000);
        else
        {
            float remaining = 1.0f - static_cast<float>(volume) * 0.01f;
            float scale = 1.0f - remaining * remaining * remaining;
            buffer->SetVolume(static_cast<int>(
                (g_SoundCueMetadata[soundIndex].volume + 5000) * scale) - 5000);
        }
        buffer->Play(0, 0, 0);
    }
    return commandQueue[0].opcode;
}

// Target 0x0043E3A0. The thread argument is ignored. Each notification reloads
// the global BGM pointer, including after the refill call.
DWORD WINAPI SoundPlayerView::BgmPlayerThread(LPVOID)
{
    SoundPlayerView *owner = reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner);
    BOOL done = FALSE;
    MSG message;
    do
    {
        const DWORD waitResult = MsgWaitForMultipleObjects(
            1, &owner->bgmUpdateEvent, FALSE, INFINITE,
            QS_KEY | QS_MOUSE | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_HOTKEY);
        if (owner->bgm == NULL)
            done = TRUE;
        switch (waitResult)
        {
        case WAIT_OBJECT_0:
            if (owner->bgm != NULL && owner->bgm->m_bIsPlaying)
            {
                static_cast<CStreamingSound *>(owner->bgm)->isLocked = TRUE;
                static_cast<CStreamingSound *>(owner->bgm)
                    ->HandleWaveStreamNotification(TRUE);
                static_cast<CStreamingSound *>(owner->bgm)->isLocked = FALSE;
            }
            break;
        case WAIT_OBJECT_0 + 1:
            while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
            {
                if (message.message == WM_QUIT)
                    done = TRUE;
            }
            break;
        }
    } while (!done);
    return 0;
}
