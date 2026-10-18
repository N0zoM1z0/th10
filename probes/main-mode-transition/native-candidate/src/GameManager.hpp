#ifndef TH10_RECONSTRUCTION_GAME_MANAGER_HPP
#define TH10_RECONSTRUCTION_GAME_MANAGER_HPP

#include "Player.hpp"

typedef unsigned int ZunBool;
typedef unsigned char u8;
typedef unsigned int u32;

struct GameChainElementView;

struct GameManager
{
    u32 unknown000;
    int stageResourceIndex;
    GameChainElementView *updateChain;
    GameChainElementView *drawChain;
    PlayerTimerView stageTimer;
    u32 configuration[13];
    u32 m_Flags;
    int initializationMode;

    ZunBool __fastcall OnDraw();
    static ZunBool __fastcall OnUpdate(GameManager *manager);
};

extern GameManager *g_GameManager; // Existing maintained target-storage identity: 0x477810.

typedef char GameManagerSharedInitializationModeAt5C[
    (offsetof(GameManager, initializationMode) == 0x5c &&
     sizeof(((GameManager *)0)->initializationMode) == 4) ? 1 : -1];

#endif
