#include "Main.hpp"
#include "ScriptViewerOwners.hpp"
#include <new>
#include <string.h>

extern void *g_ScriptViewerPopups;
extern unsigned int __fastcall ScriptViewerPopupUpdate42B9A0(void *owner);
extern unsigned int __fastcall ScriptViewerPopupDraw42B9B0(void *owner);

// Original 0x42B430: the allocation is 0xB884 bytes and the embedded
// ANM machine is constructed before the full object is zeroed.
ScriptViewerPopupOwnerView::ScriptViewerPopupOwnerView()
{
    unsigned char *slot = reinterpret_cast<unsigned char *>(this) + 0x3F0;
    for (int i = 0; i < 0x2D3; ++i)
    {
        *reinterpret_cast<unsigned int *>(slot) &= ~1u;
        slot += 0x40;
    }
    memset(this, 0, sizeof(*this));
    *reinterpret_cast<unsigned int *>(this) |= 2u;
    g_ScriptViewerPopups = this;
}

// Original 0x42B4D0: connect update/draw tasks and initialize the shared VM.
int ScriptViewerPopupOwnerView::Initialize()
{
    AnmLoadedView *loaded = g_AsciiManagerView->asciiAnm;
    unknown010 = reinterpret_cast<unsigned int>(loaded);
    th10::ChainElem *task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&ScriptViewerPopupUpdate42B9A0));
    task->arg = this;
    task->flags &= ~2u;
    th10::g_Chain->AddToCalcChain(task, 15);
    updateTask = task;

    task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&ScriptViewerPopupDraw42B9B0));
    task->arg = this;
    task->flags &= ~2u;
    th10::g_Chain->AddToDrawChain(task, 35);
    drawTask = task;

    vm.Initialize();
    vm.anmFile = loaded;
    loaded->SetSprite(&vm, 196);
    return 0;
}

// Original 0x42B660: full target-sized allocation, real outlined constructor,
// registration, destructor, and scalar free on initialization failure.
ScriptViewerPopupOwnerView *GameCreateOwner42B660()
{
    ScriptViewerPopupOwnerView *owner = new ScriptViewerPopupOwnerView;
    ScriptViewerPopupOwnerView *initializedOwner;
    if (owner != NULL)
        initializedOwner = owner;
    else
        initializedOwner = NULL;
    if (initializedOwner->Initialize() != 0)
    {
        if (initializedOwner != NULL)
        {
            initializedOwner->~ScriptViewerPopupOwnerView();
            ::operator delete(initializedOwner);
        }
        return NULL;
    }
    return initializedOwner;
}
