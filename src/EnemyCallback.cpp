#include "AnmManager.hpp"
#include "Enemy.hpp"
#include "Gui.hpp"

extern float g_PlayerTimerScale;
extern int g_EnemyTimeoutValue;

// Target 0x004127A0-0x004129F0 is called from both callback checkpoints in
// EnemyRuntimeUpdate. This overlay exposes only the Player words that the
// timer-expiry branch changes; their broader Player semantics remain unknown.
struct EnemyCallbackPlayerStateView
{
    unsigned char unknown0000[0x0ad4];
    unsigned int flags0AD4;
    unsigned char unknown0AD8[0x03a8];
    unsigned int flags0E80;
    unsigned char unknown0E84[0x0754];
    unsigned int flags15D8;
    unsigned char unknown15DC[0x03a8];
    unsigned int flags1984;
    unsigned char unknown1988[0x03a8];
    unsigned int flags1D30;
    unsigned char unknown1D34[0x03a8];
    unsigned int flags20DC;
    unsigned char unknown20E0[0x03a8];
    unsigned int flags2488;
    unsigned char unknown248C[0x12ac];
    int state3738;
    unsigned char unknown373C[0x50];
    unsigned int flags378C;
    int state3790;
};
typedef char EnemyCallbackPlayerFlags0AD4At0AD4[
    (offsetof(EnemyCallbackPlayerStateView, flags0AD4) == 0x0ad4) ? 1 : -1];
typedef char EnemyCallbackPlayerFlags2488At2488[
    (offsetof(EnemyCallbackPlayerStateView, flags2488) == 0x2488) ? 1 : -1];
typedef char EnemyCallbackPlayerState3738At3738[
    (offsetof(EnemyCallbackPlayerStateView, state3738) == 0x3738) ? 1 : -1];
typedef char EnemyCallbackPlayerFlags378CAt378C[
    (offsetof(EnemyCallbackPlayerStateView, flags378C) == 0x378c) ? 1 : -1];
typedef char EnemyCallbackPlayerState3790At3790[
    (offsetof(EnemyCallbackPlayerStateView, state3790) == 0x3790) ? 1 : -1];


static __forceinline void EnemyResetCallbackTimer(EnemyRuntimeTimerView *timer)
{
    if ((timer->flags & 1u) == 0)
    {
        timer->current = 0;
        timer->previous = -999999;
        timer->subframe = 0.0f;
        timer->scale = &g_PlayerTimerScale;
        timer->flags |= 1u;
    }
    timer->current = 0;
    timer->subframe = 0.0f;
    timer->previous = -1;
}

const unsigned char *__fastcall EnemyResolveRuntimeCallback(
    EnemyFullObjectView *owner)
{
    EnemyRuntimeView *enemy = &owner->runtime;
    const int zero = 0;
    unsigned int lifeIndex = 0;
    EnemyCallbackThresholdView *lifeRecord = enemy->callbackThresholds;

    for (; lifeIndex < 8; ++lifeIndex, ++lifeRecord)
    {
        volatile int &lifeThreshold = *reinterpret_cast<volatile int *>(
            &lifeRecord->lifeThreshold);
        if (lifeThreshold >= zero)
        {
            enemy->lifeToNextCallback = static_cast<int>(
                static_cast<unsigned int>(enemy->life) -
                static_cast<unsigned int>(enemy->callbackThresholds[lifeIndex].lifeThreshold));
            if (enemy->life <= enemy->callbackThresholds[lifeIndex].lifeThreshold)
            {
                enemy->life = enemy->callbackThresholds[lifeIndex].lifeThreshold;
                enemy->callbackThresholds[lifeIndex].lifeThreshold = -1;
                EnemyResetCallbackTimer(&enemy->updateTimer);
                enemy->flags &= ~0x10000u;
                return enemy->callbackThresholds[lifeIndex].lifeCallbackName;
            }
            break;
        }
    }

    unsigned int timerIndex = 0;
    EnemyCallbackThresholdView *timerRecord = enemy->callbackThresholds;
    for (; timerIndex < 8; ++timerIndex, ++timerRecord)
    {
        volatile int &timerLifeThreshold = *reinterpret_cast<volatile int *>(
            &timerRecord->lifeThreshold);
        if (timerLifeThreshold >= zero &&
            *reinterpret_cast<volatile int *>(&timerRecord->timerThreshold) > zero)
        {
            int seconds = static_cast<int>(
                static_cast<unsigned int>(enemy->callbackThresholds[timerIndex].timerThreshold) -
                static_cast<unsigned int>(enemy->updateTimer.current) + 59u) / 60;
            if (seconds > 99)
                seconds = 99;
            g_GuiView->spellSeconds = seconds;

            if (enemy->updateTimer.current <
                enemy->callbackThresholds[timerIndex].timerThreshold)
                return NULL;

            enemy->life = enemy->callbackThresholds[timerIndex].lifeThreshold;
            enemy->callbackThresholds[timerIndex].lifeThreshold = -1;
            EnemyResetCallbackTimer(&enemy->updateTimer);
            enemy->flags |= 0x10000u;

            g_EnemyTimeoutValue = static_cast<int>(
                static_cast<unsigned int>(g_EnemyTimeoutValue) - 3000u);
            if (g_EnemyTimeoutValue < 5000)
                g_EnemyTimeoutValue = 5000;

            EnemyCallbackPlayerStateView *player =
                reinterpret_cast<EnemyCallbackPlayerStateView *>(g_Player);
            if ((player->flags378C & 8u) == 0 && player->state3738 >= 60)
            {
                player->state3790 = 0;
                player->flags378C &= ~2u;
                player->flags0AD4 &= ~2u;
                player->flags0E80 &= ~2u;
                player->flags15D8 &= ~2u;
                player->flags1984 &= ~2u;
                player->flags1D30 &= ~2u;
                player->flags20DC &= ~2u;
                player->flags2488 &= ~2u;
            }
            return enemy->callbackThresholds[timerIndex].timerCallbackName;
        }
    }
    return NULL;
}
