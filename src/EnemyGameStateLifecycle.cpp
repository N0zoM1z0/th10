#include "Main.hpp"
#include "EnemyGameState.hpp"
#include "Chain.hpp"

#include <new>
#include <string.h>

// This is the target 0x004776F4 game-owned spell panel, already used by
// SpellPanel/EnemyEclDispatcher. Avoid a second shadow/anonymous owner.
EnemyGameStateView *g_EnemyGameState;

// The 50-byte callback at 0x00409230 is already source-mapped and exact.
// Its descriptive receiver type was historically private to AnmManager.cpp.
struct AnmPairDrawOwnerView;
extern int __fastcall AnmPairOwnerDrawCallback(AnmPairDrawOwnerView *owner);

// Target 0x00408910, original allocation 0x37B0. The VM arrays and their
// genuine timer members are constructed by C++ before this body. The retail
// constructor clears the full object even after those constructions.
EnemyGameStateView::EnemyGameStateView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2u;
    g_EnemyGameState = this;
}

// Target 0x004089C0. Register three tasks in observed calc/draw order and
// prepare the spell timer. No success/error check is present on AllocElem.
int EnemyGameStateView::RegisterTasks()
{
    th10::ChainElem *task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&EnemySpellPanelUpdateCallback));
    task->arg = this;
    task->flags &= ~2u;
    th10::g_Chain->AddToCalcChain(task, 22);
    updateTask = task;

    task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&AnmPairOwnerDrawCallback));
    task->flags &= ~2u;
    task->arg = this;
    th10::g_Chain->AddToDrawChain(task, 14);
    drawTask = task;

    task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&EnemySpellPanelDrawCallback));
    task->arg = this;
    task->flags &= ~2u;
    th10::g_Chain->AddToDrawChain(task, 37);
    auxiliaryDrawTask = task;

    spellTimer.SetCurrent(0);
    return 0;
}

// Target 0x00408AF0. The concrete compiler-generated destructor tail
// reverses the 5, 8 and 2 VM arrays automatically; do not memcpy/skip them.
EnemyGameStateView::~EnemyGameStateView()
{
    g_AnmRenderManagerView->MarkVmForDeletion(panelVmIds[0]);
    panelVmIds[0].value = 0;
    g_AnmRenderManagerView->MarkVmForDeletion(panelVmIds[1]);
    panelVmIds[1].value = 0;
    g_AnmRenderManagerView->MarkVmForDeletion(panelVmIds[2]);
    panelVmIds[2].value = 0;

    if (updateTask != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::g_Chain->UnregisterElem(updateTask);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    if (drawTask != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::g_Chain->UnregisterElem(drawTask);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    if (auxiliaryDrawTask != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::g_Chain->UnregisterElem(auxiliaryDrawTask);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    g_EnemyGameState = NULL;
}

// Target 0x00408C90, including construction, task-registration failure
// cleanup through the genuine destructor, and operator delete.
EnemyGameStateView *CreateEnemyGameState()
{
    EnemyGameStateView *state = new EnemyGameStateView;
    if (state->RegisterTasks() != 0)
    {
        delete state;
        return NULL;
    }
    return state;
}
