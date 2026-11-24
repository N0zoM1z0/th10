#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ItemManager.hpp"
#include "ItemResources.hpp"
#include "Chain.hpp"
#include "ItemPowerDisplay.hpp"
#include "GameScoreState.hpp"
#include "Player.hpp"

void ItemVectorView::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp dword ptr [eax]
        fmul magnitude
        fstp dword ptr [eax + 4]
    }
#else
    // The natural C++ path implements the same polar XY projection as the
    // target's x87 FSINCOS path; preserve the existing Z component.
    x = static_cast<float>(cos(angle)) * magnitude;
    y = static_cast<float>(sin(angle)) * magnitude;
#endif
}

void GameScoreStateView::AddFaith(int amount)
{
    // The target adds two 32-bit words before clamping the signed result.
    // Unsigned addition preserves native wrap at the signed bounds.
    // Publish the wrapped native ADD through the signed faith field
    // without implementation-defined unsigned-to-signed narrowing.
    const unsigned int nextFaithBits =
        static_cast<unsigned int>(faith) +
        static_cast<unsigned int>(amount / 10);
    memcpy(&faith, &nextFaithBits, sizeof faith);
    if (faith > 99999)
        faith = 99999;
}

void GameScoreStateView::DecayFaith(int amount)
{
    // Reconstruct the original 32-bit word subtraction before the signed cap.
    // Publish the wrapped native SUB through the signed faith field
    // before the original signed 5000-point floor test.
    const unsigned int nextFaithBits =
        static_cast<unsigned int>(faith) -
        static_cast<unsigned int>(amount / 10);
    memcpy(&faith, &nextFaithBits, sizeof faith);
    if (faith < 5000)
        faith = 5000;
}

void GameScoreStateView::SetFaith(int amount)
{
    faith = amount / 10;
    if ((timer.flags & 1u) == 0)
    {
        timer.current = 0;
        timer.previous = -999999;
        timer.subframe = 0.0f;
        timer.scale = &g_PlayerTimerScale;
        timer.flags |= 1u;
    }
    timer.current = 0;
    timer.subframe = 0.0f;
    timer.previous = -1;
}

void GameScoreStateView::SetTimerCurrent(int value)
{
    if ((timer.flags & 1u) == 0)
    {
        timer.current = 0;
        timer.previous = -999999;
        timer.subframe = 0.0f;
        timer.scale = &g_PlayerTimerScale;
        timer.flags |= 1u;
    }
    timer.current = value;
    // Native timer publication stores the low 32 bits even at INT_MIN.
    const unsigned int previousBits = static_cast<unsigned int>(value) - 1u;
    memcpy(&timer.previous, &previousBits, sizeof timer.previous);
    timer.subframe = (float)value;
}

void GameScoreStateView::AddRank(int amount)
{
    // The native ADD wraps before the signed [-0x400, 0x400] bounds test.
    // i386 ADD publishes the exact wrapped 32-bit rank representation.
    const unsigned int nextRankBits =
        static_cast<unsigned int>(rank) + static_cast<unsigned int>(amount);
    memcpy(&rank, &nextRankBits, sizeof rank);
    if (rank > 0x400)
        rank = 0x400;
    else if (rank < -0x400)
        rank = -0x400;
}

// TH10 0x0041B8E0-0x0041B9F8 draws the fixed 0x896-row item pool.  The
// record layout below comes from that body and the neighboring update/spawn
// owners: each 0x3F0-byte row starts with a complete AnmVmView, then keeps
// its world-space position at +0x3AC, active state at +0x3DC and the
// sprite-selection value at +0x3E4.
struct FpsSampleGateView
{
    unsigned char unknown000[0x58];
    unsigned int flags58;
};

extern FpsSampleGateView *g_FpsSampleGateView;
extern short g_PlayerPower;

// Target 0x4054B0 binds sprites for the integer and two fractional power
// digits. The intervening VM is not changed by this helper.
void ItemPowerDisplayOwnerView::UpdatePowerDisplay(int value, int percent)
{
    frontAnm->SetSprite(&powerVms[0], value + 8);
    frontAnm->SetSprite(&powerVms[2], percent / 10 + 8);
    frontAnm->SetSprite(&powerVms[3], percent % 10 + 8);
}

// The target calls the same 0x44BF40 timer implementation as the ANM
// executor. Both independently reviewed storage views have the same layout.
// The observed death/collection callers retain this complete shared helper.
__declspec(noinline) void GameScoreStateView::ExtendFaithTimer(int amount)
{
    if (timer.current < 130)
    {
        AnmVmTimerView *sharedTimer =
            reinterpret_cast<AnmVmTimerView *>(&timer);
        sharedTimer->Add(static_cast<float>(amount));
        if (timer.current > 130)
            sharedTimer->SetCurrent(130);
    }
}

// Target 0x418930 consumes signed short power. Only an overshoot, rather
// than an exact arrival at100, replaces the managed power-notice VM.
int GameScoreStateView::AddPower(short amount)
{
    if (power >= 100)
        return 0;
    power += amount;
    if (power > 100)
    {
        power = 100;
        ItemPowerDisplayOwnerView *gui = g_ItemPowerDisplayOwner;
        g_AnmRenderManagerView->MarkVmForDeletion(gui->powerNoticeVmId);
        memset(&gui->powerNoticeVmId, 0, sizeof(gui->powerNoticeVmId));
        // Target retains the real creator call and its hidden ID return.
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        gui->powerNoticeVmId.value = gui->frontAnm->CreateVmVariant0(0x49, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
    }
    return (static_cast<int>(power) - static_cast<int>(amount)) / 20 !=
        static_cast<int>(power) / 20;
}

int __fastcall ItemManagerView::OnUpdate(ItemManagerView *manager)
{
    FpsSampleGateView *gate = g_FpsSampleGateView;
    if (gate != NULL &&
        (((gate->flags58 | (gate->flags58 >> 2)) & 1) != 0 ||
         (gate->flags58 & 0x400) != 0))
        return 1;
    return ItemManagerUpdateBody(manager);
}

// The actual callback address escapes through this recovered registration
// owner; it supplies the ECX callback contract without a synthetic caller.
// Both registered callbacks are the same native 0x24-byte ChainElem, not
// a parallel private Item node. Verify the original published offsets.
typedef char ItemChainFlagsAt04[
    (offsetof(th10::ChainElem, flags) == 0x04) ? 1 : -1];
typedef char ItemChainArgumentAt20[
    (offsetof(th10::ChainElem, arg) == 0x20) ? 1 : -1];

__declspec(noinline) int ItemManagerRegisterCallbacks(ItemManagerView *manager)
{
    th10::ChainElem *node =
        th10::Chain::AllocElem(reinterpret_cast<th10::ChainCallback>(
            &ItemManagerView::OnUpdate));
    node->flags &= ~2u;
    node->arg = manager;
    th10::g_Chain->AddToCalcChain(node, 21);
    manager->updateCallbackNode = node;

    node = th10::Chain::AllocElem(reinterpret_cast<th10::ChainCallback>(
        &ItemManagerView::OnDraw));
    node->flags &= ~2u;
    node->arg = manager;
    th10::g_Chain->AddToDrawChain(node, 25);
    manager->drawCallbackNode = node;
    return 0;
}

// The playfield origin sits at (224, 16) on screen.
static inline void ItemPlayfieldToScreen(
    const AnmFloat3View *playfield, AnmFloat3View *screen)
{
    screen->x = playfield->x + 224.0f;
    screen->y = playfield->y + 16.0f;
    screen->z = playfield->z;
}

int ItemManagerView::Draw()
{
    ItemRecordView *item = &items[0];
    for (int remaining = 0x896; remaining != 0; --remaining, ++item)
    {
        if (item->active == 0)
            continue;

        ItemPlayfieldToScreen(&item->worldPosition, &item->vm.position);
        if (item->vm.position.y < 8.0f)
        {
            const float distance = item->vm.position.y - 8.0f;
            item->vm.position.y = 24.0f;
            if (distance >= 32.0f)
                item->vm.primaryColor.alpha = 0xff;
            else
                item->vm.primaryColor.alpha =
                    static_cast<unsigned char>(255.0f * (distance / 32.0f));
            if (item->vm.activeSpriteIndex != item->spriteKind + 0x161)
            {
                g_EnemyPrimaryResourceOwner->primaryEnemyResource->SetSprite(
                    &item->vm, item->spriteKind + 0x160);
            }
        }
        else if (item->vm.activeSpriteIndex != item->spriteKind + 0x158)
        {
            g_EnemyPrimaryResourceOwner->primaryEnemyResource->SetSprite(
                &item->vm, item->spriteKind + 0x157);
            item->vm.primaryColor.alpha = 0xff;
        }

        g_AnmRenderManagerView->Draw(&item->vm);
    }
    return 1;
}

// The registered draw adapter at 0x0041BA30 has the normal callback receiver
// in ECX.  With Draw present in the same LTCG graph VC7.1 naturally lowers
// the final call to the target's private EAX receiver and tail jump.
int __fastcall ItemManagerView::OnDraw(ItemManagerView *manager)
{
    FpsSampleGateView *gate = g_FpsSampleGateView;
    if (gate != NULL && (gate->flags58 & 4) != 0)
        return 1;
    return manager->Draw();
}

// The allocation at0x41AED0 constructs every record before clearing the
// complete manager. This intentionally also clears the timer flags just
// initialized by the member constructors.
ItemManagerView *g_ItemManager;

ItemManagerView::ItemManagerView()
{
    memset(this, 0, sizeof(ItemManagerView));
    flags |= 2u;
    g_ItemManager = this;
}

struct ItemCriticalSectionView { unsigned char storage[0x18]; };
extern ItemCriticalSectionView g_ItemCallbackCriticalSection; // 0x492274
extern unsigned char g_ItemCallbackLockDepth; // 0x49231C
// Direct original destination: th10::Chain::UnregisterElem at 0x00449F60.
static void RemoveItemCallbackNode(th10::ChainElem *node)
{
    th10::Chain *const chain = th10::g_Chain;
    if (node == NULL)
        return;
    EnterCriticalSection(&g_ItemCallbackCriticalSection);
    ++g_ItemCallbackLockDepth;
    chain->UnregisterElem(node);
    LeaveCriticalSection(&g_ItemCallbackCriticalSection);
    --g_ItemCallbackLockDepth;
}

// Target0x41ADF0 removes both callbacks, clears the published manager and
// destroys all896 records. The caller owns the allocation release.
ItemManagerView::~ItemManagerView()
{
    RemoveItemCallbackNode(updateCallbackNode);
    RemoveItemCallbackNode(drawCallbackNode);
    g_ItemManager = NULL;
}

void *GameCreateItemManager()
{
    ItemManagerView *manager = new ItemManagerView;
    // The target calls registration even if operator new returns NULL.
    if (ItemManagerRegisterCallbacks(manager) != 0)
    {
        if (manager != NULL)
        {
            manager->~ItemManagerView();
            free(manager);
        }
        return NULL;
    }
    return manager;
}

int ItemManagerView::ConvertPowerItems()
{
    for (int index = 0; index < 150; ++index)
    {
        ItemRecordView *item = &items[index];
        if (item->active == 0)
            continue;
        int kind = item->kind;
        if (kind != 1 && kind != 4)
            continue;
        item->active = 0;
        // The last conversion arm is retained in the target despite the
        // preceding predicate excluding those values.
        if (kind == 1)
            Spawn(&item->worldPosition, 9, 0xffffffffu, -1.5707964f, 2.2f);
        else if (kind == 4)
            Spawn(&item->worldPosition, 9, 0xffffffffu, -1.5707964f, 2.2f);
        else if (kind == 10 || kind == 11)
            Spawn(&item->worldPosition, 5, 0xffffffffu, -1.5707964f, 2.2f);
        else
            continue;
        g_EnemyPrimaryResourceOwner->primaryEnemyResource->
            CreateVmAtWorldVariant0(0x189, &item->worldPosition);
    }
    return 0;
}

// Complete0x41BB00 owner:793 body bytes plus26 bytes of table alignment,
// three absolute jump slots and the eleven-byte kind selector. The normal
// pool is150 records; kind8 uses the following2048-record ring.
static __forceinline ItemRecordView *FindFreeNormalItem(ItemRecordView *item)
{
    for (int slot = 0; slot < 150; ++slot, ++item)
    {
        if (item->active == 0)
            return item;
    }
    return NULL;
}

static __forceinline void ResetSpawnMotion(ItemRecordView &item)
{
    item.velocity.z = 0.0f;
    item.timer.SetCurrent(0);
    item.unknown3C4 = 0;
}

int ItemManagerView::Spawn(const AnmFloat3View *position, int kind,
    unsigned int color, float angle, float magnitude)
{
    if (kind != 8)
    {
        ItemRecordView *item = FindFreeNormalItem(items);
        if (item == NULL)
            return 0;

        item->active = 1;
        AnmFloat3View *location = &item->worldPosition;
        *location = *position;
        if (location->x <= -192.0f)
            location->x = -192.0f;
        else if (location->x >= 192.0f)
            location->x = 192.0f;
        item->velocity.FromAngleMagnitude(angle, magnitude);
        ResetSpawnMotion(*item);

        short power;
        memcpy(&power, &g_PlayerPower, sizeof power);
        if (power >= 100)
        {
            switch (kind)
            {
            case 10:
            case 11:
                kind = 5;
                break;
            case 1:
            case 4:
                kind = 9;
                break;
            }
        }

        // The previous slot contents determine whether to spawn the effect.
        if (item->kind == 3)
        {
            g_EnemyPrimaryResourceOwner->primaryEnemyResource->
                CreateVmAtWorldVariant0(0x189, location);
        }
        item->kind = kind;
        item->spriteKind = kind;
        if (kind == 10)
        {
            item->spriteKind = 1;
            AnmVmStartScript(g_EnemyPrimaryResourceOwner->primaryEnemyResource,
                &item->vm, 0x177);
        }
        else if (kind == 11)
        {
            item->spriteKind = 4;
            AnmVmStartScript(g_EnemyPrimaryResourceOwner->primaryEnemyResource,
                &item->vm, 0x17a);
        }
        else
        {
            AnmVmStartScript(g_EnemyPrimaryResourceOwner->primaryEnemyResource,
                &item->vm, kind + 0x176);
        }
        item->vm.primaryColor.value = color;
        return 0;
    }

    int cursor = delayedItemCursor;
    ItemRecordView *item = &items[150 + cursor];
    int requests = ++delayedSpawnRequests;
    if (item->active == 0)
    {
        int delayFrames;
        if (requests >= 1024)
            delayFrames = cursor % 32 + 16;
        else if (requests >= 512)
            delayFrames = cursor % 16 + 8;
        else if (requests >= 256)
            delayFrames = cursor % 8 + 4;
        else
            delayFrames = cursor % 4;
        item->delayFrames = delayFrames;
        item->kind = 8;
        item->spriteKind = 8;
        item->active = 5;
        item->worldPosition = *position;
        item->velocity.FromAngleMagnitude(angle, magnitude);
        ResetSpawnMotion(*item);
    }
    delayedItemCursor = (delayedItemCursor + 1) % 2048;
    return 0;
}
