#pragma once
#include "AnmManager.hpp"
#include <stddef.h>

// Partial view of target4776F0; complete storage/lifetime ownership stays open.
struct EnemyPrimaryResourceOwnerView
{
    unsigned char unknown000000[0x3e0b50];
    AnmLoadedView *primaryEnemyResource;
};
typedef char EnemyPrimaryResourceAt3E0B50[
    (offsetof(EnemyPrimaryResourceOwnerView, primaryEnemyResource) == 0x3e0b50) ? 1 : -1];
extern EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;
