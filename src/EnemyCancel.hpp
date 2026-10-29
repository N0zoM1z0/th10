#pragma once

#include "AnmManager.hpp"

#include <stddef.h>

struct PlayerFloat3;

struct EnemyCancelManagerView
{
    unsigned char unknown000000[0x44];
    AnmFloat3View cancelCenter;
    AnmFloat3View cancelSize;
    unsigned int unknown05C;
    unsigned char bulletStorage[0x3e0af0];
    void *primaryEnemyResource;

    __declspec(noinline) unsigned int CancelAllBullets(int mode);
    __declspec(noinline) unsigned int CancelBulletPattern(
        const PlayerFloat3 *position, float radius, int mode, int flags);
};
typedef char EnemyCancelManagerCenterAt44[
    (offsetof(EnemyCancelManagerView, cancelCenter) == 0x44) ? 1 : -1];
typedef char EnemyCancelManagerSizeAt50[
    (offsetof(EnemyCancelManagerView, cancelSize) == 0x50) ? 1 : -1];
typedef char EnemyCancelManagerBulletsAt60[
    (offsetof(EnemyCancelManagerView, bulletStorage) == 0x60) ? 1 : -1];
typedef char EnemyCancelManagerResourceAt3E0B50[
    (offsetof(EnemyCancelManagerView, primaryEnemyResource) == 0x3e0b50) ? 1 : -1];


__declspec(noinline) unsigned int EnemyCancelBulletRecord(
    unsigned char *bullet);

struct EnemyEffectWaitManager;
__declspec(noinline) unsigned int EnemyApplyBulletClear(
    EnemyEffectWaitManager *manager, int mode);
