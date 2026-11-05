#include "Main.hpp"
#include "ScriptViewerOwners.hpp"
#include "Player.hpp"
#include <new>
#include <string.h>

extern void *g_ScriptViewerPopups;
int __fastcall ScriptViewerPopupUpdate42B9A0(void *owner);
int __fastcall ScriptViewerPopupDraw42B9B0(void *owner);

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

// Complete 0x42B6F0 behavior: active record fading and timer expiry.
int ScriptViewerPopupOwnerView::Update()
{
    ScriptViewerPopupRecord *record = PopupRecords();
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

// Target 0x42B780: full 723-record digit popup renderer, preserving the
// target's fog transition and loop-internal player refresh.
int ScriptViewerPopupOwnerView::Draw()
{
    ScriptViewerPopupRecord *record = PopupRecords();
    if ((g_AnmHardwareFlags & 4) == 0 && g_MainSupervisorView.cachedFogEnabled != 0)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        g_MainSupervisorView.cachedFogEnabled = 0;
        g_Direct3DDevice->vtable->SetRenderState(g_Direct3DDevice, 28, 0);
    }
    Player *player = g_Player;
    for (int i = 0; i < 723; ++i, ++record)
    {
        if (!record->active)
            continue;
        float spacing = record->timer.current < 8 ? 8.0f / record->timer.subframe : 8.0f;
        vm.positionOffset.x = record->position.x - static_cast<int>(record->digitCount) * spacing * 0.5f + 224.0f;
        vm.positionOffset.y = record->position.y + 16.0f;
        vm.primaryColor.value = record->color;
        const float dx = player->drawPosition.x - record->position.x;
        const float dy = player->drawPosition.y - record->position.y;
        int distance = static_cast<int>(dy * dy + dx * dx);
        int alpha;
        if (distance > 4096)
            alpha = 208;
        else if (distance > 1024)
            alpha = 80 + ((distance - 1024) << 7) / 3072;
        else
            alpha = 80;
        int count = record->digitCount;
        unsigned char *digit = record->digits + count - 1;
        if (count > 0)
        {
            AnmVmView *const drawVm = &vm;
            do
            {
                if (record->timer.current >= 52 && *digit != 10)
                {
                    if (record->timer.current < 56)
                        drawVm->loadedSprite = &loadedDigitAnm->sprites[*digit + 207];
                    else
                        drawVm->loadedSprite = &loadedDigitAnm->sprites[*digit + 217];
                }
                else
                    drawVm->loadedSprite = &loadedDigitAnm->sprites[*digit + 196];
                drawVm->primaryColor.alpha = static_cast<unsigned char>(alpha);
                drawVm->spriteWidth = drawVm->loadedSprite->width;
                drawVm->flags35C |= 8;
                g_AnmRenderManagerView->DrawNoRotation(drawVm);
                drawVm->positionOffset.x += spacing;
                --digit;
            } while (--count);
            player = g_Player;
        }
    }
    return 1;
}

// Target 0x42B9B0: the draw scheduler's independent callback adapter.
int __fastcall ScriptViewerPopupDraw42B9B0(void *owner)
{
    return static_cast<ScriptViewerPopupOwnerView *>(owner)->Draw();
}

// Target 0x42B9C0 is called from ItemManager with this same 0x477840 owner.
// Popup row insertions wrap at 720, while Update/Draw iterate all 723 rows.
// Preserve that observed distinction and the original timer initialization.
void ScriptViewerPopupOwnerView::EmitValuePopup(
    const AnmFloat3View *position, int value, unsigned int color)
{
    if (nextPopupIndex >= 0x2d0)
        nextPopupIndex = 0;
    ScriptViewerPopupRecord *row = PopupRecords() + nextPopupIndex;
    int count = 0;
    row->active = 1;
    if (value < 0)
        goto negativeValue;
    while (value != 0)
    {
        row->digits[count] = static_cast<unsigned char>(value % 10);
        value /= 10;
        ++count;
    }
    if (count != 0)
        goto storePopup;
    row->digits[0] = 0;
singleDigit:
    count = 1;
storePopup:
    row->digitCount = static_cast<unsigned char>(count);
    row->color = color;
    if ((row->timer.flags & 1u) == 0)
    {
        row->timer.current = 0;
        row->timer.previous = -999999;
        row->timer.subframe = 0.0f;
        row->timer.scale = &g_AnmGameSpeed;
        row->timer.flags |= 1u;
    }
    row->timer.current = 0;
    row->timer.subframe = 0.0f;
    row->timer.previous = -1;
    row->position = *position;
    ++nextPopupIndex;
    return;

negativeValue:
    row->digits[0] = 10;
    goto singleDigit;
}


// Original 0x42B4D0: connect update/draw tasks and initialize the shared VM.
int ScriptViewerPopupOwnerView::Initialize()
{
    AnmLoadedView *loaded = g_AsciiManagerView->asciiAnm;
    loadedDigitAnm = loaded;
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
