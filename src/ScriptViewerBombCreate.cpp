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

// Target 0x405750: stateful per-frame Bomb owner update. The input is the
// 0x48-byte controller, whose active VM belongs to the ANM render manager.
int ScriptViewerBombOwnerView::Update()
{
    int currentMode = mode28;
    if (currentMode == 0)
        return 1;
    // Native i386 DEC wraps the mode word before the zero test, including
    // INT_MIN; publish it without signed C++ overflow or pointer aliasing.
    const unsigned int nextModeBits =
        static_cast<unsigned int>(currentMode) - 1u;
    memcpy(&currentMode, &nextModeBits, sizeof currentMode);
    if (currentMode == 0)
    {
        int vmId = static_cast<int>(*reinterpret_cast<volatile unsigned int *>(&vmId2C));
        AnmRenderManagerView *manager = g_AnmRenderManagerView;
        AnmVmView *vm = manager->FindVm(AnmVmIdView(vmId));
        if (vm == NULL)
        {
            *reinterpret_cast<volatile unsigned int *>(&vmId2C) = 0;
            *reinterpret_cast<volatile int *>(&mode28) = 0;
            return 1;
        }
        EnemyGameStateView *state = g_EnemyGameState;
        if ((state->flags378C & 1u) != 0 &&
            ((state->spellId >= 93 && state->spellId <= 96) ||
             state->spellId == 109))
            position30.y -= 0.5f;
        else if (g_GameStatisticsIndex0 == 0)
            position30.y -= 1.0f;
        else
            position30.y -= 1.3f;

        const unsigned int nextVmId = *reinterpret_cast<volatile unsigned int *>(&vmId2C);
        AnmFloat3View screenPosition;
        screenPosition.x = position30.x + 224.0f;
        screenPosition.y = position30.y + 16.0f;
        screenPosition.z = position30.z;
        manager->SetVmPosition(AnmVmIdView(static_cast<int>(nextVmId)),
            &screenPosition);
        // VM +0x40 is scaleY: retain its exact IEEE-754 word rather than
        // converting the floating-point value to an integer.
        memcpy(&unknown03C, &vm->scaleY, sizeof(unknown03C));
        EmitAuxiliary();
    }
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
    timer.Tick();
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
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
