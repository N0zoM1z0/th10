#include "Main.hpp"
#include "FrontEnd.hpp"
#include "Startup.hpp"
#include "GameErrorContext.hpp"

extern int g_GameStartupCounter;

// Target42CAA0..42CB5F. Both tasks exist before either resource is loaded.
int FrontEndControllerView::RegisterTasks()
{
    th10::ChainElem *element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(FrontEndUpdateCallback));
    element->flags &= ~2u;
    element->arg = this;
    th10::Chain::AddToCalcChain(element, 6, th10::g_Chain);
    updateTask = element;
    element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(FrontEndDrawCallback));
    element->arg = this;
    element->flags &= ~2u;
    th10::Chain::AddToDrawChain(element, 3, th10::g_Chain);
    drawTask = element;
    frontAnm = g_AnmRenderManagerView->PreloadAnm(25, "title.anm");
    if (frontAnm == NULL)
        goto fail;
    titleAnm = g_AnmRenderManagerView->PreloadAnm(26, "title_v.anm");
    if (titleAnm == NULL)
        goto fail;
    cursor.wraps = 1;
    g_GameStartupCounter = 0;
    return 0;

fail:
    th10::g_GameErrorContext.Log(g_StartupFrontError);
    return -1;
}

// Target42C9F0..42CA97 ignores the thread argument and returns with plain RET.
int FrontEndLoadThread()
{
    if (g_FrontEndControllerView->RegisterTasks() != 0)
    {
        g_MainSupervisorView.nextGameMode =
            ((~(g_MainSupervisorView.flags >> 12)) & 1) | 2;
        return 0;
    }
    if (g_StartupWorker != NULL)
    {
        while (g_StartupWorker->frames < 300)
        {
            if (g_MainSupervisorView.flags & 0x80)
                break;
            Sleep(16);
        }
        g_AnmRenderManagerView->ReleaseAnm(1);
    }
    g_FrontEndControllerView->updateTask->flags |= 2;
    return 0;
}
