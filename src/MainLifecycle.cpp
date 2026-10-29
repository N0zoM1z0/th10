#include "Main.hpp"
#include "GameManager.hpp"
#include "FrontEnd.hpp"
#include "Startup.hpp"
#include "Ending.hpp"
#include "ReplayManager.hpp"
#include "ReplayTimingSource.hpp"
#include "Midi.hpp"
#include "Sound.hpp"
#include "PbgArchive.hpp"
#include <stdlib.h>
#include <string.h>

// Target421F00..421F46 constructs configuration and modeWorker in member order.
MainSupervisorView::MainSupervisorView()
{
    memset(this, 0, sizeof(*this));
    flags |= 0x140;
}

// Target4658F0 constructs491C28 and registers its automatic destruction.
MainSupervisorView g_MainSupervisorView;

extern ScriptViewerWorkerView g_MainScanWorker;
extern PbgArchive g_MainPbgArchive;
extern char g_MainDummyBgmPath[];
struct FpsCounterView;
void DeleteFpsCounter(FpsCounterView *counter);
void MainReleaseTextResources();
void GameManagerDelete(GameManager *manager);
void FrontEndDelete(FrontEndControllerView *controller);

// Target4203F0..42046F. Each actual owner supplies its own cleanup/global effects.
void ReleaseModeOwners()
{
    GameManagerDelete(g_GameManager);
    FrontEndDelete(g_FrontEndControllerView);
    delete g_StartupWorker;
    EndingDelete(g_EndingView);
    delete g_ReplayManager;
}

// Target420270..420394; preserve global and receiver-relative ownership.
int MainSupervisorView::ReleaseGame()
{
    g_MainScanWorker.Stop();
    reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->soundDataLoadState = 2;
    g_MainSupervisorView.modeWorker.Stop();
    if (g_MainSupervisorView.buffer764 != NULL)
    {
        free(g_MainSupervisorView.buffer764);
        g_MainSupervisorView.buffer764 = NULL;
    }
    ReleaseModeOwners();
    DeleteFpsCounter(reinterpret_cast<FpsCounterView *>(g_ReplayTimingSource));
    D3d9VertexBufferView *&vertexBuffer = g_AnmRenderManagerView->quadVertexBuffer;
    if (vertexBuffer != NULL)
    {
        MainInputObjectView *object =
            reinterpret_cast<MainInputObjectView *>(vertexBuffer);
        object->vtable->Release(object);
        vertexBuffer = NULL;
    }
    reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->QueueCommand(
        g_MainDummyBgmPath, 4, 0);
    MainReleaseTextResources();
    if (inputDevice0 != NULL)
    {
        inputDevice0->vtable->Call020(inputDevice0);
        if (inputDevice0 != NULL)
        {
            inputDevice0->vtable->Release(inputDevice0);
            inputDevice0 = NULL;
        }
    }
    if (inputDevice1 != NULL)
    {
        inputDevice1->vtable->Call020(inputDevice1);
        if (inputDevice1 != NULL)
        {
            inputDevice1->vtable->Release(inputDevice1);
            inputDevice1 = NULL;
        }
    }
    if (inputInterface != NULL)
    {
        inputInterface->vtable->Release(inputInterface);
        inputInterface = NULL;
    }
    if (g_MainSupervisorView.timerOwner != NULL)
    {
        g_MainSupervisorView.timerOwner->StopTimer();
        delete g_MainSupervisorView.timerOwner;
        g_MainSupervisorView.timerOwner = NULL;
    }
    g_MainPbgArchive.Release();
    return 0;
}
