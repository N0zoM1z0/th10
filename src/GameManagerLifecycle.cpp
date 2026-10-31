#include "Main.hpp"
#include "GameManager.hpp"
#include <process.h>
#include <string.h>

extern short g_DefaultGameKeyBindings[9];
int __stdcall GameManagerInitialize(GameManager *manager);

// Target418C40..418CBC. The same defaults are used by other configuration owners.
void GameConfigurationView::ResetDefaults()
{
    memset(this, 0, sizeof(*this));
    colorMode = 0;
    unknown016 = 600;
    unknown018 = 600;
    unknown01B = 1;
    unknown01C = 1;
    options |= 0x100;
    format = 0x100003;
    windowed = 0;
    frameskip = 0;
    memcpy(keyBindings, g_DefaultGameKeyBindings, sizeof(keyBindings));
    unknown01F = 2;
    hintMode = 0;
    bgmVolume = 100;
    sfxVolume = 80;
}

GameManager::GameManager()
{
    memset(this, 0, sizeof(*this));
}

// Target417C70..417C7B is the actual argument-ignoring callback.
int GameManagerLoadThread()
{
    return GameManagerInitialize(g_GameManager);
}

// Target420EA0..420F15 expands the worker launch under critical section6.
int MainSupervisorView::StartModeThread(int (*entry)(), void *context)
{
    EnterCriticalSection(&criticalSections[6]);
    ++criticalSectionLockCounts[6];
    ScriptViewerWorkerView &threadWorker = modeWorker;
    threadWorker.Stop();
    threadWorker.entry018 = reinterpret_cast<void *>(entry);
    threadWorker.running = 1;
    threadWorker.stopRequested = 0;
    threadWorker.thread = reinterpret_cast<void *>(_beginthreadex(
        NULL, 0, reinterpret_cast<unsigned (__stdcall *)(void *)>(entry),
        context, 0, &threadWorker.threadId));
    LeaveCriticalSection(&criticalSections[6]);
    --criticalSectionLockCounts[6];
    return 0;
}

// Target4180E0..41814A preserves construction even before the full60-byte clear.
GameManager *__stdcall GameManagerCreate(int mode)
{
    GameManager *manager = new GameManager;
    MainD3d9DeviceView *device = g_MainSupervisorView.d3dDevice;
    device->vtable->Call014(device);
    manager->initializationMode = mode;
    g_GameManager = manager;
    manager->m_Flags |= 4;
    g_MainSupervisorView.StartModeThread(GameManagerLoadThread, NULL);
    return manager;
}
