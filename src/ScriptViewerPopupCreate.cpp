#include "Main.hpp"
#include "ScriptViewerOwners.hpp"
#include <new>
#include <string.h>

extern void *g_ScriptViewerPopups;
int __fastcall ScriptViewerPopupUpdate42B9A0(void *owner);
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

// Target record: 0x40 bytes at owner+0x3C4, with its timer at +0x1C.
// The record array is intentionally not a constructed C++ member: original
// outer-object initialization clears it after initializing its VM member.
struct ScriptViewerPopupRecord
{
    unsigned char digits[12];
    AnmFloat3View position;
    unsigned int color;
    AnmVmTimerView timer;
    unsigned char reserved30[8];
    unsigned char active;
    unsigned char digitCount;
    unsigned char reserved3A[6];
};
typedef char PopupRecordSize40[(sizeof(ScriptViewerPopupRecord) == 0x40) ? 1 : -1];
typedef char PopupRecordTimerAt1C[(offsetof(ScriptViewerPopupRecord, timer) == 0x1C) ? 1 : -1];

// Complete 0x42B6F0 behavior: active record fading and timer expiry.
int ScriptViewerPopupOwnerView::Update()
{
    ScriptViewerPopupRecord *record = reinterpret_cast<ScriptViewerPopupRecord *>(
        reinterpret_cast<unsigned char *>(this) + 0x3C4);
    for (int index = 0; index < 723; ++index, ++record)
    {
        if (record->active)
        {
            record->position.y -= g_AnmGameSpeed * 0.5f;
            record->timer.Tick();
            if (record->timer.current > 60)
                record->active = 0;
        }
    }
    return 1;
}

// Original update callback 0x42B9A0 receives its record owner in ECX.
int __fastcall ScriptViewerPopupUpdate42B9A0(void *owner)
{
    return static_cast<ScriptViewerPopupOwnerView *>(owner)->Update();
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
