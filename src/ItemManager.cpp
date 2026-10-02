#include <stddef.h>
#include <math.h>

#include "AnmManager.hpp"
#include "GameScoreState.hpp"
#include "Player.hpp"

struct ItemVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);

    AnmFloat3View operator*(float scale) const
    {
        return AnmFloat3View(x * scale, y * scale, z * scale);
    }
};

typedef char ItemVectorViewSizeIs0C[
    (sizeof(ItemVectorView) == 0x0c) ? 1 : -1];

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
    (void)angle;
    (void)magnitude;
    x = 0.0f;
    y = 0.0f;
#endif
}

void GameScoreStateView::AddFaith(int amount)
{
    faith += amount / 10;
    if (faith > 99999)
        faith = 99999;
}

void GameScoreStateView::DecayFaith(int amount)
{
    faith -= amount / 10;
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
    timer.previous = value - 1;
    timer.subframe = (float)value;
}

void GameScoreStateView::AddRank(int amount)
{
    rank += amount;
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
struct ItemPrimaryResourceOwnerView
{
    unsigned char unknown000000[0x3e0b50];
    AnmLoadedView *primaryEnemyResource;
};

struct FpsSampleGateView
{
    unsigned char unknown000[0x58];
    unsigned int flags58;
};

struct ItemRecordView
{
    AnmVmView vm;
    AnmFloat3View worldPosition;
    ItemVectorView velocity;
    unsigned char unknown3C4[4];
    GameScoreTimerView timer;
    int active;
    int kind;
    int spriteKind;
    float attractionSpeed;
    int delayFrames;
};

struct ItemChainElementView;
struct ItemManagerView
{
    unsigned char unknown000[0x08];
    ItemChainElementView *updateCallbackNode;
    ItemChainElementView *drawCallbackNode;
    unsigned char unknown010[4];
    ItemRecordView items[0x896];
    int animatedItemCount;
    int unknown21CEB8;
    int value21CEBC;

    int Draw();
    int ConvertPowerItems();
    static int __fastcall OnUpdate(ItemManagerView *manager);
    static int __fastcall OnDraw(ItemManagerView *manager);
};

typedef char ItemRecordViewSizeIs3F0[
    (sizeof(ItemRecordView) == 0x3f0) ? 1 : -1];
typedef char ItemRecordWorldPositionAt3AC[
    (offsetof(ItemRecordView, worldPosition) == 0x3ac) ? 1 : -1];
typedef char ItemRecordActiveAt3DC[
    (offsetof(ItemRecordView, active) == 0x3dc) ? 1 : -1];
typedef char ItemRecordSpriteKindAt3E4[
    (offsetof(ItemRecordView, spriteKind) == 0x3e4) ? 1 : -1];
typedef char ItemManagerItemsAt14[
    (offsetof(ItemManagerView, items) == 0x14) ? 1 : -1];
typedef char ItemManagerCallbacksAt08[
    (offsetof(ItemManagerView, updateCallbackNode) == 0x08 &&
     offsetof(ItemManagerView, drawCallbackNode) == 0x0c) ? 1 : -1];
typedef char ItemRecordVelocityAt3B8[
    (offsetof(ItemRecordView, velocity) == 0x3b8) ? 1 : -1];
typedef char ItemRecordTimerAt3C8[
    (offsetof(ItemRecordView, timer) == 0x3c8) ? 1 : -1];
typedef char ItemRecordKindAt3E0[
    (offsetof(ItemRecordView, kind) == 0x3e0) ? 1 : -1];
typedef char ItemRecordAttractionSpeedAt3E8[
    (offsetof(ItemRecordView, attractionSpeed) == 0x3e8) ? 1 : -1];
typedef char ItemRecordDelayAt3EC[
    (offsetof(ItemRecordView, delayFrames) == 0x3ec) ? 1 : -1];
typedef char ItemManagerAnimatedCountAt21CEB4[
    (offsetof(ItemManagerView, animatedItemCount) == 0x21ceb4) ? 1 : -1];
typedef char ItemManagerViewSizeIs21CEC0[
    (sizeof(ItemManagerView) == 0x21cec0) ? 1 : -1];

extern ItemPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;
extern FpsSampleGateView *g_FpsSampleGateView;
extern float g_ItemScreenOffsetX;
extern float g_ItemScreenOffsetY;
extern float g_ItemTopThreshold;
extern float g_ItemAlphaRange;
extern float g_ItemAlphaScale;
extern float g_ItemAlphaFull;

// Shared maintained definitions used by the independently reconstructed Gui
// and Enemy owners. These views do not establish original class/TU identities.
struct GuiScoreView
{
    int unknown00;
    int score;

    void Add(int amount)
    {
        score += amount / 10;
        if (score >= 1000000000)
            score = 999999999;
    }
};

struct EnemySoundQueueView
{
    unsigned char unknown000[0x408];
    int cueValues[134];
    int activeSoundIds[12];
    int sampleCounts[12];
    int samples[12][128];

    __declspec(noinline) void QueueSoundCue(int soundId, float positionX);
    __declspec(noinline) void QueueSoundSample(int soundId, int sample);
};

struct ItemAutoCollectGateView
{
    unsigned char unknown000[0x28];
    int value28;
};
struct ItemPopupOwnerView;
struct ItemPowerDisplayOwnerView;
extern ItemAutoCollectGateView *g_ItemAutoCollectGate;
extern ItemPopupOwnerView *g_ItemPopupOwner;
extern ItemPowerDisplayOwnerView *g_ItemPowerDisplayOwner; // 0x47770C
extern unsigned char *g_MainSoundOwner;
extern unsigned int g_PlayerInputBits;
extern short g_PlayerPower;
extern int g_EnemyDifficulty;

// TH10-local call contracts for still source-absent callees. Their target
// private register assignments are evidence, not imposed source conventions.
extern int ItemAddPower(GameScoreStateView *state, short amount); // 0x418930
extern void ItemAddLives(GameScoreStateView *state, int amount); // 0x4188A0
extern void ItemExtendFaithTimer(GameScoreStateView *state, int amount); // 0x412FF0
extern void ItemEmitValuePopup(ItemPopupOwnerView *owner,
    const AnmFloat3View *position, int value, unsigned int color); // 0x42B9C0
extern void ItemUpdatePowerDisplay(ItemPowerDisplayOwnerView *owner,
    int value, int percent); // 0x4054B0: ESI owner, EDX value, stack percent
extern float EnemyAngleFromPlayer(Player *player, const PlayerFloat3 *position);

// Complete update body 0x41AFD0-0x41B89C; its two selector tables extend
// physical ownership through 0x41B8DF. Names remain descriptive views.
int __stdcall ItemManagerUpdateBody(ItemManagerView *manager)
{
    int convertPowerItems = 0;
    int pickupScore;
    int rankDelta;
    manager->value21CEBC = 0;
    manager->animatedItemCount = 0;
    ItemRecordView *item = &manager->items[0];
    for (int remaining = 0x896; remaining != 0; --remaining, ++item)
    {
        if (item->active == 0)
            continue;

        if (item->active == 5)
        {
            if (--item->delayFrames < 0)
            {
                item->active = 2;
                item->vm.InitializeForLoadedScript(
                    g_EnemyPrimaryResourceOwner->primaryEnemyResource,
                    item->kind + 0x176);
            }
            continue;
        }

        if (item->active == 1)
        {
            if ((g_Player->runtimeState != 2 && g_Player->runtimeState != 4 &&
                 g_Player->drawPosition.y < 128.0f) ||
                g_ItemAutoCollectGate->value28 != 0)
            {
                item->attractionSpeed = g_Player->optionData->value08;
                item->active = 3;
                goto attract;
            }
            item->worldPosition += item->velocity * g_PlayerTimerScale;
            item->velocity.y += g_PlayerTimerScale * 0.03f;
            if (item->velocity.y >= 0.0f)
                item->velocity.x = 0.0f;
            if (item->velocity.y > 2.0f)
                item->velocity.y = 2.0f;
            if (item->worldPosition.y > 472.0f)
            {
                item->active = 0;
                continue;
            }
        }
        else if (item->active == 2)
        {
            item->worldPosition += item->velocity * g_PlayerTimerScale;
            item->velocity.y += g_PlayerTimerScale * 0.03f;
            if (item->velocity.y >= 0.0f)
            {
                item->attractionSpeed = g_Player->optionData->value08;
                item->active = 3;
                goto attract;
            }
            if (item->worldPosition.y > 472.0f)
            {
                item->active = 0;
                g_GameScoreState.AddRank(-4);
                continue;
            }
        }
        else if (item->active == 3)
        {
attract:
            float dx = g_Player->drawPosition.x - item->worldPosition.x;
            float dy = g_Player->drawPosition.y - item->worldPosition.y;
            float angle;
            if (dy == 0.0f && dx == 0.0f)
                angle = 1.5707963705062866f;
            else
                angle = static_cast<float>(atan2(dy, dx));
            item->velocity.FromAngleMagnitude(angle, item->attractionSpeed);
            item->worldPosition += item->velocity * g_PlayerTimerScale;
            if (item->attractionSpeed < 12.0f)
                item->attractionSpeed += 0.2f;
            if (g_Player->runtimeState == 4)
            {
                item->active = 1;
                item->velocity.x = 0.0f;
                item->velocity.y = 0.0f;
            }
        }
        else if (item->active == 4)
        {
            float speed = item->attractionSpeed;
            float angle = EnemyAngleFromPlayer(g_Player,
                reinterpret_cast<const PlayerFloat3 *>(&item->worldPosition));
            item->velocity.FromAngleMagnitude(angle, speed);
            item->worldPosition += item->velocity * g_PlayerTimerScale;
            if (item->attractionSpeed < 12.0f)
                item->attractionSpeed += 0.2f;
            if (g_Player->runtimeState == 4)
            {
                item->active = 1;
                item->velocity.x = 0.0f;
                item->velocity.y = 0.0f;
            }
        }

        if (g_Player->runtimeState == 2)
            goto animate;

        if (item->worldPosition.x < g_Player->derivedVectors[0].x ||
            item->worldPosition.y < g_Player->derivedVectors[0].y ||
            g_Player->derivedVectors[1].x < item->worldPosition.x ||
            g_Player->derivedVectors[1].y < item->worldPosition.y)
        {
            if (item->active != 4 && item->active != 3)
            {
                unsigned int focused = g_PlayerInputBits & 4;
                if ((focused != 0 &&
                     g_Player->derivedVectors[2].x <= item->worldPosition.x &&
                     g_Player->derivedVectors[2].y <= item->worldPosition.y &&
                     item->worldPosition.x <= g_Player->derivedVectors[3].x &&
                     item->worldPosition.y <= g_Player->derivedVectors[3].y) ||
                    (static_cast<short>(focused) == 0 &&
                     g_Player->derivedVectors[4].x <= item->worldPosition.x &&
                     g_Player->derivedVectors[4].y <= item->worldPosition.y &&
                     item->worldPosition.x <= g_Player->derivedVectors[5].x &&
                     item->worldPosition.y <= g_Player->derivedVectors[5].y))
                {
                    item->attractionSpeed = g_Player->optionData->value08 *
                        0.3333333432674408f;
                    item->active = 4;
                }
            }
            goto animate;
        }

        switch (item->kind)
        {
        case 1:
        case 10:
        {
            int changed = ItemAddPower(&g_GameScoreState, 1);
            int power = g_PlayerPower;
            ItemUpdatePowerDisplay(g_ItemPowerDisplayOwner, power / 20,
                ((power % 20) * 100) / 20);
            if (changed != 0)
            {
                RebuildPlayerOptions(g_Player);
                ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                    -1, 0xffffff40);
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                    QueueSoundCue(29, item->worldPosition.x);
                if (g_PlayerPower >= 100)
                    convertPowerItems = 1;
                g_GameScoreState.AddRank(12);
            }
            else
            {
                ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                    g_PlayerPower / 2, 0xffff4040);
            }
            ItemExtendFaithTimer(&g_GameScoreState, 60);
            break;
        }
        case 2:
            if (g_Player->drawPosition.y < 144.0f)
            {
                pickupScore = g_GameScoreState.faith * 10;
                pickupScore -= pickupScore % 10;
                goto maximumScore;
            }
            else
            {
                int reduction = static_cast<int>(
                    (g_Player->drawPosition.y - 144.0f) *
                    0.003289473708719015f *
                    (g_GameScoreState.faith * 0.5f - 5000.0f));
                pickupScore = g_GameScoreState.faith * 10 / 2 - reduction;
                pickupScore -= pickupScore % 10;
                ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                    pickupScore, 0xffffffff);
                rankDelta = 1;
                goto addScore;
            }
        case 5:
            pickupScore = g_GameScoreState.faith * 10;
maximumScore:
            ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                pickupScore, 0xffffff00);
            rankDelta = 8;
addScore:
            g_GameScoreState.AddRank(rankDelta);
            reinterpret_cast<GuiScoreView *>(&g_GameScoreState)->Add(pickupScore);
            ItemExtendFaithTimer(&g_GameScoreState, 100);
            break;
        case 3:
        {
            int amount = 5000;
            switch (g_EnemyDifficulty)
            {
            case 0: case 1: amount = 5000; break;
            case 2: amount = 8000; break;
            case 3: case 4: amount = 10000; break;
            }
            ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                amount, 0xff00ff00);
            g_GameScoreState.AddFaith(amount);
            ItemExtendFaithTimer(&g_GameScoreState, 120);
            break;
        }
        case 4:
        case 11:
        {
            int changed = ItemAddPower(&g_GameScoreState, 20);
            int power = g_PlayerPower;
            ItemUpdatePowerDisplay(g_ItemPowerDisplayOwner, power / 20,
                ((power % 20) * 100) / 20);
            if (changed != 0)
            {
                RebuildPlayerOptions(g_Player);
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                    QueueSoundCue(29, item->worldPosition.x);
                ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                    -1, 0xffffff40);
                g_GameScoreState.AddRank(24);
                if (g_PlayerPower >= 100)
                    convertPowerItems = 1;
            }
            else
            {
                ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                    g_PlayerPower / 2, 0xffff4040);
            }
            ItemExtendFaithTimer(&g_GameScoreState, 20);
            break;
        }
        case 7:
            ItemAddLives(&g_GameScoreState, 1);
            g_GameScoreState.AddRank(256);
            break;
        case 8:
            g_GameScoreState.AddFaith(10);
            ItemExtendFaithTimer(&g_GameScoreState, 3);
            reinterpret_cast<GuiScoreView *>(&g_GameScoreState)->Add(10);
            break;
        case 9:
            ItemEmitValuePopup(g_ItemPopupOwner, &item->worldPosition,
                100, 0xff00ff00);
            g_GameScoreState.AddFaith(100);
            ItemExtendFaithTimer(&g_GameScoreState, 60);
            break;
        }
        item->active = 0;
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
            QueueSoundCue(20, item->worldPosition.x);
        continue;

animate:
        AnmRenderManagerView::ExecuteScript(&item->vm);
        item->timer.previous = item->timer.current;
        if (*item->timer.scale > 0.99f && *item->timer.scale < 1.01f)
        {
            ++item->timer.current;
            item->timer.subframe += 1.0f;
        }
        else
        {
            item->timer.subframe += *item->timer.scale;
            item->timer.current = static_cast<int>(item->timer.subframe);
        }
        ++manager->animatedItemCount;
    }
    if (convertPowerItems != 0)
        manager->ConvertPowerItems();
    return 1;
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
struct ItemChainElementView
{
    unsigned char unknown000[4];
    unsigned int flags;
    unsigned char unknown008[0x18];
    ItemManagerView *argument;
};
typedef char ItemChainFlagsAt04[
    (offsetof(ItemChainElementView, flags) == 0x04) ? 1 : -1];
typedef char ItemChainArgumentAt20[
    (offsetof(ItemChainElementView, argument) == 0x20) ? 1 : -1];
typedef int (__fastcall *ItemCallback)(ItemManagerView *manager);
extern void *g_ItemCallbackChainOwner;
extern ItemChainElementView *ItemCreateCallbackNode(ItemCallback callback);
extern void ItemRegisterUpdateNode(void *owner, int priority,
    ItemChainElementView *node);
extern void ItemRegisterDrawNode(void *owner, int priority,
    ItemChainElementView *node);

int ItemManagerRegisterCallbacks(ItemManagerView *manager)
{
    ItemChainElementView *node =
        ItemCreateCallbackNode(&ItemManagerView::OnUpdate);
    node->flags &= ~2u;
    node->argument = manager;
    ItemRegisterUpdateNode(g_ItemCallbackChainOwner, 21, node);
    manager->updateCallbackNode = node;

    node = ItemCreateCallbackNode(&ItemManagerView::OnDraw);
    node->flags &= ~2u;
    node->argument = manager;
    ItemRegisterDrawNode(g_ItemCallbackChainOwner, 25, node);
    manager->drawCallbackNode = node;
    return 0;
}

int ItemManagerView::Draw()
{
    ItemRecordView *item = &items[0];
    for (int remaining = 0x896; remaining != 0; --remaining, ++item)
    {
        if (item->active == 0)
            continue;

        item->vm.position.x = item->worldPosition.x + g_ItemScreenOffsetX;
        item->vm.position.y = item->worldPosition.y + g_ItemScreenOffsetY;
        item->vm.position.z = item->worldPosition.z;

        if (item->vm.position.y < g_ItemTopThreshold)
        {
            const float y = item->vm.position.y;
            item->vm.position.y = 24.0f;
            const float delta = y - g_ItemTopThreshold;
            if (delta >= g_ItemAlphaRange)
            {
                item->vm.primaryColor.alpha = 0xff;
            }
            else
            {
                item->vm.primaryColor.alpha = static_cast<unsigned char>(
                    static_cast<int>(
                        delta * g_ItemAlphaScale * g_ItemAlphaFull));
            }

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
