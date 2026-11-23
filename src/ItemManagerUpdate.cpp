#include "EnemySoundQueue.hpp"
#include "GuiScore.hpp"
#include <math.h>

#include "ItemManager.hpp"
#include "ItemResources.hpp"
#include "ScriptViewerOwners.hpp"
#include "ItemPowerDisplay.hpp"
#include "GameScoreState.hpp"
#include "Player.hpp"

struct ItemAutoCollectGateView
{
    unsigned char unknown000[0x28];
    int value28;
};
extern ItemAutoCollectGateView *g_ItemAutoCollectGate;
extern void *g_ScriptViewerPopups;
extern unsigned char g_MainSoundOwner[0x52d0];
extern unsigned int g_PlayerInputBits;
extern int g_EnemyDifficulty;
extern float EnemyAngleFromPlayer(Player *player, const PlayerFloat3 *position);

static __forceinline bool ItemOutsideBox(float minX, float minY, float maxX, float maxY,
    const PlayerFloat3 &min, const PlayerFloat3 &max)
{
    return min.x > minX || min.y > minY || max.x < maxX || max.y < maxY;
}

// Complete update body 0x41AFD0-0x41B89C; its two selector tables extend
// physical ownership through 0x41B8DF. Names remain descriptive views.
int __stdcall ItemManagerUpdateBody(ItemManagerView *manager)
{
    ItemRecordView *item = manager->items;
    int convertPowerItems = 0;
    int pickupScore;
    int rankDelta;
    manager->delayedSpawnRequests = 0;
    manager->animatedItemCount = 0;
    for (int remaining = 0x896; remaining != 0; --remaining, ++item)
    {
        if (item->active == 0)
            continue;

        if (item->active == 5)
        {
            if (--item->delayFrames < 0)
            {
                item->active = 2;
                AnmVmStartScript(g_EnemyPrimaryResourceOwner->primaryEnemyResource, &item->vm, item->kind + 0x176);
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

        AnmFloat3View minPoint, maxPoint;
        Player *player = g_Player;
        if (player->runtimeState == 2)
            goto animate;

        maxPoint.x = item->worldPosition.x;
        maxPoint.y = item->worldPosition.y;
        minPoint.x = item->worldPosition.x;
        minPoint.y = item->worldPosition.y;
        if (!ItemOutsideBox(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y,
                player->derivedVectors[0], player->derivedVectors[1]))
        {
            switch (item->kind)
            {
            case 1:
            case 10:
            {
                int changed = g_GameScoreState.AddPower(1);
                int power = g_GameScoreState.power;
                g_ItemPowerDisplayOwner->UpdatePowerDisplay(power / 20,
                    ((power % 20) * 100) / 20);
                if (changed != 0)
                {
                    RebuildPlayerOptions(g_Player);
                    static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                        -1, 0xffffff40);
                    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                        QueueSoundCue(29, item->worldPosition.x);
                    if (g_GameScoreState.power >= 100)
                        convertPowerItems = 1;
                    g_GameScoreState.AddRank(12);
                    g_GameStartupState.score.ExtendFaithTimer(60);
                }
                else
                {
                    static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                        g_GameScoreState.power / 2, 0xffff4040);
                    g_GameStartupState.score.ExtendFaithTimer(60);
                }
                break;
            }
            case 2:
                if (player->drawPosition.y < 144.0f)
                {
                    int scaledFaith = g_GameScoreState.faith * 10;
                    pickupScore = scaledFaith - scaledFaith % 10;
                    static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                        pickupScore, 0xffffff00);
                    rankDelta = 8;
                    goto addScore;
                }
                else
                {
                    // Scale the distance below the collection line over the
                    // remaining height of the playfield.
                    float distanceScale = (player->drawPosition.y - 144.0f) /
                        (448.0f - 144.0f);
                    int reduction = static_cast<int>(distanceScale *
                        (g_GameScoreState.faith * 0.5f - 5000.0f));
                    pickupScore = g_GameScoreState.faith * 10 / 2 - reduction;
                    pickupScore -= pickupScore % 10;
                    static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                        pickupScore, 0xffffffff);
                    rankDelta = 1;
                    goto addScore;
                }
            case 5:
                pickupScore = g_GameScoreState.faith;
                pickupScore *= 10;
                static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                    pickupScore, 0xffffff00);
                rankDelta = 8;
    addScore:
                g_GameScoreState.AddRank(rankDelta);
                g_GuiScore.Add(pickupScore);
                g_GameStartupState.score.ExtendFaithTimer(100);
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
                static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                    amount, 0xff00ff00);
                g_GameScoreState.AddFaith(amount);
                g_GameStartupState.score.ExtendFaithTimer(120);
                break;
            }
            case 8:
                g_GameScoreState.AddFaith(10);
                g_GameStartupState.score.ExtendFaithTimer(3);
                reinterpret_cast<GuiScoreView *>(&g_GameStartupState.score)->Add(10);
                break;
            case 9:
                static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                    100, 0xff00ff00);
                g_GameScoreState.AddFaith(100);
                g_GameStartupState.score.ExtendFaithTimer(60);
                break;
            case 4:
            case 11:
            {
                int changed = g_GameScoreState.AddPower(20);
                int power = g_GameScoreState.power;
                g_ItemPowerDisplayOwner->UpdatePowerDisplay(power / 20,
                    ((power % 20) * 100) / 20);
                if (changed != 0)
                {
                    RebuildPlayerOptions(g_Player);
                    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                        QueueSoundCue(29, item->worldPosition.x);
                    static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                        -1, 0xffffff40);
                    g_GameScoreState.AddRank(24);
                    if (g_GameScoreState.power < 100)
                        goto largePowerTimer;
                    convertPowerItems = 1;
                    g_GameStartupState.score.ExtendFaithTimer(20);
                    break;
                }
                else
                {
                    static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups)->EmitValuePopup(&item->worldPosition,
                        g_GameScoreState.power / 2, 0xffff4040);
                }
    largePowerTimer:
                g_GameStartupState.score.ExtendFaithTimer(20);
                break;
            }
            case 7:
                g_GameScoreState.AddLives(1);
                g_GameScoreState.AddRank(256);
                break;
            }
            item->active = 0;
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundCue(20, item->worldPosition.x);
            continue;
        }

        if (item->active != 4 && item->active != 3)
        {
            unsigned int focused = g_PlayerInputBits & 4;
            if ((focused != 0 &&
                 !ItemOutsideBox(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y,
                     player->derivedVectors[2], player->derivedVectors[3])) ||
                (static_cast<short>(focused) == 0 &&
                 !ItemOutsideBox(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y,
                     player->derivedVectors[4], player->derivedVectors[5])))
            {
                float speed = player->optionData->value08;
                item->active = 4;
                item->attractionSpeed = speed * 0.3333333432674408f;
            }
        }

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
