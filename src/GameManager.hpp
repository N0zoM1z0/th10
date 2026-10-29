#pragma once
#include "AnmManager.hpp"
#include "GameConfiguration.hpp"

struct GameChainElementView;
struct GameManager
{
    unsigned int unknown000;
    int stageResourceIndex;
    GameChainElementView *updateChain;
    GameChainElementView *drawChain;
    AnmVmTimerView stageTimer;
    GameConfigurationView configuration;
    unsigned int m_Flags;
    int initializationMode;

    GameManager();
    unsigned int __fastcall OnDraw();
    static unsigned int __fastcall OnUpdate(GameManager *manager);
};

typedef char GameManagerModeAt5C[
    (offsetof(GameManager, initializationMode) == 0x5c &&
     sizeof(GameManager) == 0x60) ? 1 : -1];

extern GameManager *g_GameManager;
GameManager *__stdcall GameManagerCreate(int mode);
void GameManagerDelete(GameManager *manager);
void __stdcall GameManagerCleanup(GameManager *manager);
