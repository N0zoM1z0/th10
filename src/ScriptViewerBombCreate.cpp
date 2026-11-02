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
