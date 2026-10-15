#pragma once
#include <stddef.h>

// Narrow shared flag view; other subsystem views and global storage remain open.
struct EnemyGameStateView
{
    unsigned char unknown000[0x378c];
    unsigned int flags378C;
};
typedef char EnemyGameStateFlagsAt378C[
    (offsetof(EnemyGameStateView, flags378C) == 0x378c) ? 1 : -1];
extern EnemyGameStateView *g_EnemyGameState;
