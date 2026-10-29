#include "Main.hpp"
#include "ScriptViewerOwners.hpp"
#include "BulletManager.hpp"
#include "Enemy.hpp"
#include "Gui.hpp"
#include "ItemManager.hpp"

// Descriptive lifecycle views reconstructed from TH10 entries and their real
// browser callers. They do not identify original source names or ownership.
extern BulletManagerView *g_ScriptViewerBullets;
extern void *g_ScriptViewerBomb;
extern void *g_ScriptViewerPopups;

ScriptViewerBombOwnerView::~ScriptViewerBombOwnerView()
{
    th10::ChainElem *task = updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    g_ScriptViewerBomb = NULL;
}

ScriptViewerPopupOwnerView::~ScriptViewerPopupOwnerView()
{
    th10::ChainElem *task = updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    g_ScriptViewerPopups = NULL;
}

BulletManagerView::~BulletManagerView()
{
    th10::ChainElem *task = updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(bulletAnm);
    g_ScriptViewerBullets = NULL;
}

// Separate observed nullable delete entries. LTCG chooses their private
// receiver registers from the real browser Update calls.
__declspec(noinline) void ScriptViewerReleaseGui(GuiView *owner) { ScriptViewerDeleteOwned(owner); }
__declspec(noinline) void ScriptViewerReleasePlayer(Player *owner) { ScriptViewerDeleteOwned(owner); }
__declspec(noinline) void ScriptViewerReleaseBullets(BulletManagerView *owner) { ScriptViewerDeleteOwned(owner); }
__declspec(noinline) void ScriptViewerReleaseBomb(void *owner)
{
    ScriptViewerDeleteOwned(static_cast<ScriptViewerBombOwnerView *>(owner));
}
__declspec(noinline) void ScriptViewerReleaseItems(ItemManagerView *owner)
{
    ScriptViewerDeleteOwned(owner);
}
__declspec(noinline) void ScriptViewerReleasePopups(void *owner)
{
    ScriptViewerDeleteOwned(static_cast<ScriptViewerPopupOwnerView *>(owner));
}
__declspec(noinline) void ScriptViewerReleaseEnemies(EnemyManagerView *owner) { ScriptViewerDeleteOwned(owner); }
