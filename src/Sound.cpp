#include <windows.h>

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


class CSound
{
public:
    virtual ~CSound();
    HRESULT Stop();
};

struct SoundPlayerView
{
    unsigned char unknown000[0x614];
    DWORD bgmThreadId;
    HANDLE bgmThreadHandle;
    unsigned char unknown61C[0x5208 - 0x61c];
    CSound *bgm;
    HANDLE bgmUpdateEvent;
    unsigned char unknown5210[0x52d0 - 0x5210];

    void StopBgm();
};

typedef char SoundPlayerViewSizeIs52D0[
    (sizeof(SoundPlayerView) == 0x52d0) ? 1 : -1];

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

    delete bgm;
    bgm = NULL;
}
