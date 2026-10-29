#include "Main.hpp"
#include "FrontEnd.hpp"
#include "ReplayManager.hpp"
#include <process.h>
#include <stdlib.h>
#include <string.h>

// Target42C920..42C9EB: construction precedes the full5ACC-byte owner clear.
FrontEndControllerView::FrontEndControllerView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2;
    g_FrontEndControllerView = this;
}

// Target42C5B0..42C5B5 is the sole virtual slot, not a deleting destructor.
unsigned int FrontEndControllerView::GetSize()
{
    return sizeof(*this);
}

// Target42CD50..42CDAF expands the common worker launch protocol.
FrontEndControllerView *FrontEndCreate()
{
    FrontEndControllerView *controller = new FrontEndControllerView;
    ScriptViewerWorkerView &threadWorker = controller->worker;
    threadWorker.Stop();
    threadWorker.entry018 = reinterpret_cast<void *>(FrontEndLoadThread);
    threadWorker.running = 1;
    threadWorker.stopRequested = 0;
    threadWorker.thread = reinterpret_cast<void *>(_beginthreadex(
        NULL, 0, reinterpret_cast<unsigned (__stdcall *)(void *)>(FrontEndLoadThread),
        controller, 0, &threadWorker.threadId));
    return controller;
}

static void RemoveFrontEndTask(th10::ChainElem *element)
{
    th10::Chain *chain = th10::g_Chain;
    if (element == NULL)
        return;
    EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
    ++g_MainSupervisorView.criticalSectionLockCounts[0];
    chain->UnregisterElem(element);
    LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
    --g_MainSupervisorView.criticalSectionLockCounts[0];
}

// Target42CB60..42CD4A includes automatic worker destruction and its unwind edge.
FrontEndControllerView::~FrontEndControllerView()
{
    worker.Stop();
    RemoveFrontEndTask(updateTask);
    RemoveFrontEndTask(drawTask);
    g_AnmRenderManagerView->ReleaseAnm(25);
    g_AnmRenderManagerView->ReleaseAnm(26);
    for (int i = 0; i < 50; ++i)
        delete replayFiles[i];
    AnmVmView *auxiliary = g_AnmRenderManagerView->FindVm(difficultyAuxVmId);
    if (auxiliary != NULL)
    {
        auxiliary->pendingInterrupt = 1;
        if (auxiliary->layerNode.previous == NULL)
        {
            for (AnmVmLayerNodeView *node = auxiliary->layerNode.next;
                 node != NULL; node = node->next)
                static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
        }
    }
    if (musicText != NULL)
    {
        free(musicText);
        musicText = NULL;
    }
    g_FrontEndControllerView = NULL;
}
