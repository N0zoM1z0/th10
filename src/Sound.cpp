#include <windows.h>
#include <dsound.h>
#include <string.h>
#include <stdlib.h>

#include "Main.hpp"
#include "SoundFormat.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"

// Target pointer table 0x00474B40, in load order. Pointer slots are mutable.
const char *g_SoundFileNames[37] =
{
    "se_plst00.wav",
    "se_enep00.wav",
    "se_pldead00.wav",
    "se_power0.wav",
    "se_power1.wav",
    "se_tan00.wav",
    "se_tan01.wav",
    "se_tan02.wav",
    "se_ok00.wav",
    "se_cancel00.wav",
    "se_select00.wav",
    "se_gun00.wav",
    "se_cat00.wav",
    "se_lazer00.wav",
    "se_lazer01.wav",
    "se_enep01.wav",
    "se_damage00.wav",
    "se_item00.wav",
    "se_kira00.wav",
    "se_kira01.wav",
    "se_kira02.wav",
    "se_timeout.wav",
    "se_graze.wav",
    "se_powerup.wav",
    "se_pause.wav",
    "se_cardget.wav",
    "se_option.wav",
    "se_damage01.wav",
    "se_timeout2.wav",
    "se_invalid.wav",
    "se_slash.wav",
    "se_ch00.wav",
    "se_ch01.wav",
    "se_hint00.wav",
    "se_extend.wav",
    "se_bonus3.wav",
    "se_water.wav",
};
// Target 0x0046FDD4: CP932 sound-file read failure diagnostic.
char g_SoundDataLoadErrorFormat[] =
    "error : Sound \x83\x74\x83\x40\x83\x43\x83\x8b\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf\x82\xc8\x82\xa2\x20\x83\x66\x81\x5b\x83\x5e\x82\xf0\x8a\x6d\x94\x46 %s\r\n";

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

    ~CSoundManager()
    {
        if (directSound != NULL)
        {
            directSound->Release();
            directSound = NULL;
        }
    }

    CSoundManager() : directSound(NULL) {}

    HRESULT Initialize(HWND window, DWORD cooperativeLevel)
    {
        if (directSound != NULL)
        {
            directSound->Release();
            directSound = NULL;
        }
        HRESULT result = DirectSoundCreate(NULL, &directSound, NULL);
        if (FAILED(result))
            return result;
        result = directSound->SetCooperativeLevel(window, cooperativeLevel);
        return result;
    }

    HRESULT SetPrimaryBufferFormat(DWORD channels, DWORD samplesPerSecond, DWORD bitsPerSample);

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
     offsetof(SoundPlayerView, soundSetupThreadHandle) == 0x5218 &&
     offsetof(SoundPlayerView, soundDataThreadHandle) == 0x521c &&
     offsetof(SoundPlayerView, soundSetupThreadId) == 0x5220 &&
     offsetof(SoundPlayerView, soundSetupWindow) == 0x5228 &&
     offsetof(SoundPlayerView, soundSetupFinished) == 0x522c &&
     offsetof(SoundPlayerView, soundDataLoadState) == 0x5224 &&
     offsetof(SoundPlayerView, soundFileData) == 0x5230 &&
     offsetof(SoundPlayerView, bgmVolume) == 0x52c4) ? 1 : -1];

char g_SoundInitSuccessFormat[] = "\x44\x69\x72\x65\x63\x74\x53\x6f\x75\x6e\x64\x20\x82\xcd\x90\xb3\x8f\xed\x82\xc9\x8f\x89\x8a\xfa\x89\xbb\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\x0d\x0a";
char g_SoundInitFailureFormat[] = "\x44\x69\x72\x65\x63\x74\x53\x6f\x75\x6e\x64\x20\x83\x49\x83\x75\x83\x57\x83\x46\x83\x4e\x83\x67\x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x82\xaa\x8e\xb8\x94\x73\x82\xb5\x82\xbd\x82\xe6\x0d\x0a";

// Target 0x0043CD30-0x0043D041. Partial source recovery: the sample-loader
// source and private ABI are recovered below; byte mismatches remain, so no
// codegen credit is claimed for this initializer.
int SoundPlayerView::Initialize(HWND window)
{
    memset(unconsumedSoundMetadata, 0xff, sizeof(unconsumedSoundMetadata));
    for (int i = 0; i < 12; ++i)
        soundQueue[i] = -1;
    CSoundManager *createdManager = new CSoundManager;
    manager = createdManager;
    if (FAILED(createdManager->Initialize(window, DSSCL_PRIORITY)))
    {
        th10::g_GameErrorContext.Log(g_SoundInitFailureFormat);
        if (manager != NULL)
        {
            delete manager;
            manager = NULL;
        }
        return -1;
    }
    createdManager->SetPrimaryBufferFormat(2, 44100, 16);
    directSound = manager->directSound;
    WAVEFORMATEX format;
    DSBUFFERDESC description;
    ZeroMemory(&format, sizeof(format));
    ZeroMemory(&description, sizeof(description));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = 44100;
    format.nAvgBytesPerSec = 176400;
    format.nBlockAlign = 4;
    format.wBitsPerSample = 16;
    description.dwSize = sizeof(description);
    description.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_GLOBALFOCUS;
    description.dwBufferBytes = 0x8000;
    description.lpwfxFormat = &format;
    bgmThreadHandle = NULL;
    if (FAILED(static_cast<LPDIRECTSOUND>(directSound)->CreateSoundBuffer(
            &description, &initialSoundBuffer, NULL)))
        return -1;
    void *firstData;
    void *secondData;
    DWORD firstSize;
    DWORD secondSize;
    if (FAILED(initialSoundBuffer->Lock(0, 0x8000, &firstData, &firstSize,
                                       &secondData, &secondSize, 0)))
        return -1;
    // Target clears the requested fixed length, not the returned firstSize.
    memset(firstData, 0, 0x8000);
    initialSoundBuffer->Unlock(firstData, firstSize, secondData, secondSize);
    initialSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
    bgmVolume = 100;
    sfxVolume = 100;
    SetTimer(window, 0, 250, NULL);
    gameWindow = window;
    SoundPlayerView *globalOwner = reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner);
    for (int i = 0; i < 37; ++i)
    {
        if (globalOwner->soundDataLoadState == 2)
            return -1;
        if (globalOwner->LoadSoundSample(i, g_SoundFileNames[i]) != 0)
        {
            th10::g_GameErrorContext.Log(g_SoundDataLoadErrorFormat, g_SoundFileNames[i]);
            return -1;
        }
    }
    th10::g_GameErrorContext.Log(g_SoundInitSuccessFormat);
    return 0;
}


// Target 0x0043D250. The serialized chunk length is used literally: no RIFF
// padding adjustment or malformed-input bounds repair is present in the game.
unsigned char *__stdcall FindWaveChunk(unsigned char *data, unsigned int remaining,
                             const char *tag, unsigned int *chunkSize)
{
    while (remaining > 0)
    {
        *chunkSize = *reinterpret_cast<unsigned int *>(data + 4);
        if (strncmp(reinterpret_cast<const char *>(data), tag, 4) == 0)
            return data + 8;
        remaining -= *chunkSize + 8;
        data += *chunkSize + 8;
    }
    return NULL;
}

char g_SoundNotRiffFormat[] = "Wav \x83\x74\x83\x40\x83\x43\x83\x8b\x82\xb6\x82\xe1\x82\xc8\x82\xa2 %s\r\n";
char g_SoundInvalidWaveFormat[] = "Wav \x83\x74\x83\x40\x83\x43\x83\x8b\x82\xb6\x82\xe1\x82\xc8\x82\xa2? %s\r\n";

// Target 0x0043D390-0x0043D63B. Consumes one loader-owned allocation.
// API failure retains the published DirectSound buffer; cancellation before
// acquisition leaves the archive slot untouched and returns zero.
int SoundPlayerView::LoadSoundSample(int index, const char *path)
{
    if (manager == NULL)
        return 0;
    LPDIRECTSOUNDBUFFER *buffer = &soundBuffers[index];
    if (*buffer != NULL)
    {
        (*buffer)->Release();
        *buffer = NULL;
    }
    while (soundFileData[index] == NULL)
    {
        Sleep(10);
        if (reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->soundDataLoadState == 2)
            return 0;
    }
    unsigned char *allocation = soundFileData[index];
    soundFileData[index] = NULL;
    if (allocation == NULL)
        return -1;
    if (strncmp(reinterpret_cast<const char *>(allocation), "RIFF", 4) != 0)
    {
        th10::g_GameErrorContext.Log(g_SoundNotRiffFormat, path);
        free(allocation);
        return -1;
    }
    unsigned int remaining = *reinterpret_cast<unsigned int *>(allocation + 4);
    unsigned char *data = allocation + 8;
    if (strncmp(reinterpret_cast<const char *>(data), "WAVE", 4) != 0)
    {
        th10::g_GameErrorContext.Log(g_SoundInvalidWaveFormat, path);
        free(allocation);
        return -1;
    }
    data += 4;
    remaining -= 12;
    unsigned int chunkSize;
    WAVEFORMATEX format;
    unsigned char *formatData = FindWaveChunk(data, remaining, "fmt ", &chunkSize);
    unsigned char *samples = NULL;
    if (formatData != NULL)
    {
        format = *reinterpret_cast<WAVEFORMATEX *>(formatData);
        samples = FindWaveChunk(data, remaining, "data", &chunkSize);
    }
    if (samples == NULL)
    {
        th10::g_GameErrorContext.Log(g_SoundInvalidWaveFormat, path);
        free(allocation);
        return -1;
    }
    LPDIRECTSOUND device = static_cast<LPDIRECTSOUND>(directSound);
    unsigned int sampleSize = chunkSize;
    DSBUFFERDESC description;
    ZeroMemory(&description, sizeof(description));
    description.dwSize = sizeof(description);
    description.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_GLOBALFOCUS |
                          DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
    description.dwBufferBytes = sampleSize;
    description.lpwfxFormat = &format;
    if (FAILED(device->CreateSoundBuffer(&description, buffer, NULL)))
    {
        free(allocation);
        return -1;
    }
    void *firstData;
    void *secondData;
    DWORD firstSize;
    DWORD secondSize;
    if (FAILED((*buffer)->Lock(0, sampleSize, &firstData, &firstSize,
                               &secondData, &secondSize, 0)))
    {
        free(allocation);
        return -1;
    }
    memcpy(firstData, samples, firstSize);
    if (secondSize != 0)
        memcpy(secondData, samples + firstSize, secondSize);
    (*buffer)->Unlock(firstData, firstSize, secondData, secondSize);
    free(allocation);
    return 0;
}

// Target 0x0043CCF0-0x0043CD28 ignores the CreateThread argument and returns
// with plain RET. It publishes completion even when Initialize fails.
void SoundSetupThread()
{
    SoundPlayerView *owner = reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner);
    owner->Initialize(owner->soundSetupWindow);
    while (owner->soundDataLoadState == 0)
        Sleep(1);
    owner->soundSetupFinished = 1;
}

// Existing WinMain seam. Its caller already clears the entire owner.
void MainStartSoundThread(unsigned char *soundOwner, HWND window)
{
    SoundPlayerView *owner = reinterpret_cast<SoundPlayerView *>(soundOwner);
    owner->soundSetupWindow = window;
    owner->soundSetupThreadHandle = CreateThread(NULL, 0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(SoundSetupThread), owner, 0,
        &owner->soundSetupThreadId);
}

// Target 0x0043D080-0x0043D0EA. The launcher passes an unused argument;
// the observed body has no explicit arguments, no stable result, and plain RET.
// State 2 cancels before the next read. Each archive allocation (including NULL)
// is published before testing failure. Ownership remains with the sound owner.
void SoundDataLoaderThread()
{
    SoundPlayerView *owner = reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner);
    for (int i = 0; i < 37; ++i)
    {
        if (owner->soundDataLoadState == 2)
            return;
        owner->soundFileData[i] = FileSystem::OpenFile(g_SoundFileNames[i], NULL, 0);
        if (owner->soundFileData[i] == NULL)
        {
            th10::g_GameErrorContext.Log(g_SoundDataLoadErrorFormat, g_SoundFileNames[i]);
            return;
        }
    }
    while (owner->soundDataLoadState == 0)
        Sleep(1);
}

// Existing Main shutdown seam: target 0x00438DDA stores cancellation state2.
// This source helper has no separate target owner or exact-byte claim.
void MainRequestSoundThreadStop(unsigned char *soundOwner)
{
    reinterpret_cast<SoundPlayerView *>(soundOwner)->soundDataLoadState = 2;
}

// Target 0x0043CC40-0x0043CCEE: private ESI owner, zero result, plain RET.
// A missing first handle skips even the state transition and second handle.
int MainJoinSoundThread(unsigned char *soundOwner)
{
    SoundPlayerView *owner = reinterpret_cast<SoundPlayerView *>(soundOwner);
    if (owner->soundSetupThreadHandle != NULL)
    {
        if (owner->soundDataLoadState == 0)
            owner->soundDataLoadState = 1;
        while (WaitForSingleObject(owner->soundSetupThreadHandle, 100) == WAIT_TIMEOUT)
            Sleep(1);
        while (WaitForSingleObject(owner->soundDataThreadHandle, 100) == WAIT_TIMEOUT)
            Sleep(1);
        CloseHandle(owner->soundSetupThreadHandle);
        CloseHandle(owner->soundDataThreadHandle);
        owner->soundSetupThreadHandle = NULL;
        owner->soundDataThreadHandle = NULL;
    }
    return 0;
}

// Target 0x0043D120-0x0043D24C. Each null-store follows its releasing call;
// pointers used by later operations are deliberately reloaded from the owner.
int MainReleaseSound(unsigned char *soundOwner)
{
    SoundPlayerView *owner = reinterpret_cast<SoundPlayerView *>(soundOwner);
    if (owner->bgmFormats != NULL)
    {
        free(owner->bgmFormats);
        owner->bgmFormats = NULL;
    }
    for (int i = 0; i < 128; ++i)
    {
        if (owner->duplicateSoundBuffers[i] != NULL)
        {
            owner->duplicateSoundBuffers[i]->Release();
            owner->duplicateSoundBuffers[i] = NULL;
        }
        if (owner->soundBuffers[i] != NULL)
        {
            owner->soundBuffers[i]->Release();
            owner->soundBuffers[i] = NULL;
        }
    }
    for (int i = 0; i < 37; ++i)
    {
        if (owner->soundFileData[i] != NULL)
        {
            free(owner->soundFileData[i]);
            owner->soundFileData[i] = NULL;
        }
    }
    if (owner->manager != NULL)
    {
        KillTimer(owner->gameWindow, 1);
        owner->StopBgm();
        owner->directSound = NULL;
        owner->initialSoundBuffer->Stop();
        if (owner->initialSoundBuffer != NULL)
        {
            owner->initialSoundBuffer->Release();
            owner->initialSoundBuffer = NULL;
        }
        if (owner->bgm != NULL)
        {
            delete owner->bgm;
            owner->bgm = NULL;
        }
        if (owner->manager != NULL)
        {
            delete owner->manager;
            owner->manager = NULL;
        }
        for (int i = 0; i < 16; ++i)
        {
            if (owner->bgmPreloadAllocations[i] != NULL)
            {
                free(owner->bgmPreloadAllocations[i]);
                owner->bgmPreloadAllocations[i] = NULL;
            }
        }
    }
    return 0;
}

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
