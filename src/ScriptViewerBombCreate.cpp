#include "Main.hpp"
#include "ScriptViewerOwners.hpp"
#include <new>
#include <string.h>

extern void *g_ScriptViewerBomb;
extern int __fastcall ScriptViewerBombUpdateCallback405840(void *owner);
namespace th10 { namespace leaf { int Fn00405850(); } }

// Original 0x4056B0 constructs a 0x48-byte bomb controller in-place;
// the AnmVmTimerView member resets ownership bit at +0x24 before the
// enclosing constructor clears all eighteen DWORDs.
ScriptViewerBombOwnerView::ScriptViewerBombOwnerView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2u;
    g_ScriptViewerBomb = this;
}

// Target 0x4055C0 registers the two retained frame chain nodes.
int ScriptViewerBombOwnerView::RegisterTasks()
{
    th10::ChainElem *task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&ScriptViewerBombUpdateCallback405840));
    task->flags |= 2u;
    task->arg = this;
    th10::g_Chain->AddToCalcChain(task, 17);
    updateTask = task;

    task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&th10::leaf::Fn00405850));
    task->flags |= 2u;
    task->arg = this;
    th10::g_Chain->AddToDrawChain(task, 34);
    drawTask = task;
    return 0;
}

// AnmVM tree position propagation and distinct bomb-effect emission
// are external source-bound functions; never substitute a fabricated ABI.
#include "EnemyGameState.hpp"
extern int g_GameStatisticsIndex0;
typedef char ScriptViewerBombVmScaleYAt40[
    (offsetof(AnmVmView, scaleY) == 0x40) ? 1 : -1];

// A missing VM clears its id through the pointer. That store may alias the
// owner's mode, which orders the target's two zero stores.
static inline AnmVmView *ScriptViewerBombFindVmOrClear(
    AnmRenderManagerView *manager, AnmVmIdView *id)
{
    AnmVmView *vm = manager->FindVm(*id);
    if (vm == NULL)
        id->value = 0;
    return vm;
}

// Mode 1 follows its VM and drifts upwards; every active mode ticks the timer.
// A missing VM returns into Update's shared `return 1`.
static __forceinline void ScriptViewerBombStep(ScriptViewerBombOwnerView *bomb)
{
    switch (bomb->mode28)
    {
    case 1:
    {
        AnmVmView *vm = ScriptViewerBombFindVmOrClear(
            g_AnmRenderManagerView, reinterpret_cast<AnmVmIdView *>(&bomb->vmId2C));
        if (vm == NULL)
        {
            bomb->mode28 = 0;
            return;
        }
        EnemyGameStateView *state = g_EnemyGameState;
        if ((state->flags378C & 1u) != 0 &&
            ((state->spellId >= 93 && state->spellId <= 96) ||
             state->spellId == 109))
            bomb->position30.y -= 0.5f;
        else if (g_GameStatisticsIndex0 == 0)
            bomb->position30.y -= 1.0f;
        else
            bomb->position30.y -= 1.3f;
        AnmFloat3View screenPosition(bomb->position30.x + 224.0f,
            bomb->position30.y + 16.0f, bomb->position30.z);
        g_AnmRenderManagerView->SetVmPosition(
            *reinterpret_cast<AnmVmIdView *>(&bomb->vmId2C), &screenPosition);
        bomb->collisionRadius = vm->scaleY;
        bomb->EmitAuxiliary();
        break;
    }
    default:
        break;
    }
    // The target calls the shared timer Tick here rather than expanding it.
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
    bomb->timer.Tick();
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
}

// Target 0x405750: stateful per-frame Bomb owner update. The input is the
// 0x48-byte controller, whose active VM belongs to the ANM render manager.
int ScriptViewerBombOwnerView::Update()
{
    if (mode28 != 0)
        ScriptViewerBombStep(this);
    return 1;
}

// Target 0x405840 is an independent ECX update chain callback.
int __fastcall ScriptViewerBombUpdateCallback405840(void *owner)
{
    return static_cast<ScriptViewerBombOwnerView *>(owner)->Update();
}

// Original 0x4056B0 creates the owner and tears it down on registration
// failure. The nullable branch is an original codegen constraint.
void *ScriptViewerCreateBomb()
{
    ScriptViewerBombOwnerView *owner = new ScriptViewerBombOwnerView;
    if (owner->RegisterTasks() != 0)
    {
        if (owner != NULL)
        {
            owner->~ScriptViewerBombOwnerView();
            ::operator delete(owner);
        }
        return NULL;
    }
    return owner;
}
