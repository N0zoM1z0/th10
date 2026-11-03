#include "Main.hpp"
#include "AnmManager.hpp"
#include "ScriptViewerWorker.hpp"
#include "TextRenderer.hpp"
#include "StageScript.hpp"
#include "Rng.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <process.h>

// Startup sources are independently maintained; this 186-byte owner only
// invokes their real entrypoints. The original TU identity is still unknown.
int GameLoadStartupVersion();
struct FpsCounterView;
FpsCounterView *CreateFpsCounter();
void SoundDataLoaderThread();
int InputDeviceInitThread();
extern unsigned char g_MainSoundOwner[0x52d0];
extern HANDLE g_SoundDataLoadThreadHandle;
extern ScriptViewerWorkerView g_MainScanWorker;

// Original target data references: 0x491FF8 and 0x4923A8. The physical
// storage/meaning of these globals has not been independently allocated.
extern unsigned int g_GameStartupTimeTick;
extern unsigned int g_GameStartupTextColor;

// Target 0x004201B0..0x00420269, CC-delimited 186-byte runtime bootstrap.
int GameInitializeRuntimeSystems()
{
    GameLoadStartupVersion();
    g_AnmGameSpeed = 1.0f;
    g_GameStartupTextColor = 0xff000000u;
    StageResetPublishedCameras();
    const unsigned int tick = timeGetTime();
    g_GameStartupTimeTick = tick;
    g_RngView.seed = static_cast<unsigned short>(tick);
    g_AlternateRngView.seed = static_cast<unsigned short>(tick);

    DWORD threadId;
    g_SoundDataLoadThreadHandle = CreateThread(
        NULL, 0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(&SoundDataLoaderThread),
        g_MainSoundOwner, 0, &threadId);

    CreateFpsCounter();
    // The original call site invokes Stop, then prepares the worker fields
    // and passes all six arguments to the CRT thread-launcher in this owner.
    g_MainScanWorker.Stop();
    g_MainScanWorker.entry018 = reinterpret_cast<void *>(&InputDeviceInitThread);
    g_MainScanWorker.running = 1;
    g_MainScanWorker.stopRequested = 0;
    g_MainScanWorker.thread = reinterpret_cast<void *>(_beginthreadex(
        NULL, 0,
        reinterpret_cast<unsigned (__stdcall *)(void *)>(&InputDeviceInitThread),
        &g_MainSupervisorView, 0, &g_MainScanWorker.threadId));
    g_AnmRenderManagerView->SetupVertexBuffer();
    TextHelperView::CreateTextBuffer();
    return 0;
}
