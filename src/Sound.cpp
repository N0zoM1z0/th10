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
