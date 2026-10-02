#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "Player.hpp"
#include "ReplayManager.hpp"
#include "Enemy.hpp"
#include "GameScoreState.hpp"

// Natural source candidate adapted from th10-decomphelp-forN0.  The bounded
// OnDraw body is retained here only after direct TH10 IDA boundary review;
// the other GameManager rows in that repository are not part of this source.

typedef unsigned int ZunBool;
typedef unsigned char u8;
typedef unsigned int u32;

extern "C" void *g_AnmManagerPtr;

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

ZunBool __fastcall GameManager::OnDraw()
{
    void *pAnm;

    if ((m_Flags & 4) == 0)
    {
        pAnm = g_AnmManagerPtr;
        *(u32 *)((u8 *)pAnm + 0x50) = 0;
        *(u32 *)((u8 *)pAnm + 0x54) = 0;
        *(u32 *)((u8 *)pAnm + 0x4c) = 0;
        *(u32 *)((u8 *)pAnm + 0x58) = 0;
    }
    return 1;
}

// Partial views observed only in the 0x418190 update core. Their global aliases
// below are target-address obligations, not proven original data/TU owners.
struct GameChainElementView
{
    u32 unknown00;
    u32 flags;
    u8 unknown008[0x18];
    GameManager *owner;
};
struct GameChainOwnerView
{
    u8 unknown00[8];
    GameChainElementView *updateChain;
    GameChainElementView *drawChain;
};
struct GameMenuView
{
    u8 unknown000[0x2a18];
    u8 flags2A18;
};
struct GameGuiUpdateView
{
    u8 unknown000[0x9e14];
    int transitionVmId;
    u8 unknown9E18[0xa0];
    int dialogueState;
};
struct GameStageUpdateView
{
    int resourceIndex;
    const char *menuPath;
    u32 unknown008;
    const char *enemyPath;
    const char *musicPaths[2];
    u8 unknown018[0x0c];
    int musicIndex;
};
struct GameEffectUpdateView
{
    u8 unknown0000[0x89a0];
    int firstVmId;
    int secondVmId;
};
struct GameAdditionalChainView
{
    GameChainOwnerView chains;
    u8 unknown0010[0x379c];
    GameChainElementView *extraChain;
};
struct GameStatisticsRowView
{
    u8 unknown000[0x4c8];
    int sessionStarts;
    int activeFrames;
    u8 unknown4D0[0x3eac];
};
typedef char GameManagerTimerAt10[(offsetof(GameManager, stageTimer) == 0x10) ? 1 : -1];
typedef char GameManagerFlagsAt58[(offsetof(GameManager, m_Flags) == 0x58) ? 1 : -1];
typedef char GameStatisticsStride437C[(sizeof(GameStatisticsRowView) == 0x437c) ? 1 : -1];
typedef char GameAdditionalChainAt37AC[(offsetof(GameAdditionalChainView, extraChain) == 0x37ac) ? 1 : -1];

extern GameMenuView *g_GameMenu;                       // 0x4776E4
extern GameChainOwnerView *g_GameAnimationSlots;       // 0x4776E8
extern GameChainOwnerView *g_GameOwner4776F0;
extern GameChainOwnerView *g_GameOwner477830;
extern GameChainOwnerView *g_GameOwner477818;
extern GameChainOwnerView *g_GameOwner47781C;
extern GameChainOwnerView *g_GameOwner4776FC;
extern GameChainOwnerView *g_GameOwner4776EC;
extern GameChainOwnerView *g_GameOwner477840;
extern GameAdditionalChainView *g_GameOwner4776F4;
extern GameChainOwnerView *g_GameOwner477814;
extern EnemyManagerView *g_EnemyManager;
extern GameGuiUpdateView *g_GameGuiUpdate;              // 0x47770C
extern GameStageUpdateView *g_GameStageUpdate;          // 0x477848
extern GameEffectUpdateView *g_GameEffectUpdate;        // 0x4776E0
extern GameStatisticsRowView *g_GameStatisticsRows;     // 0x47783C
extern unsigned char g_GameWorkerState[];              // 0x492254
extern unsigned char g_GameMusicState[];               // 0x492590
extern unsigned char g_GameSoundState[];               // 0x491C28
extern unsigned int g_GameConfigurationFlags;          // 0x491FF4
extern unsigned int g_GameConfigurationFlags491D78;
extern int g_GameTransitionState;                      // 0x491FB8
extern unsigned int g_GameRuntimeFlags;                // 0x474CA0
extern unsigned int g_GameInputBits;                   // 0x474E30
extern int g_GameStatisticsIndex0;                     // 0x474C68
extern int g_GameStatisticsIndex1;                     // 0x474C6C
extern int g_GameFrameCounter0;                        // 0x474C88
extern int g_GameFrameCounter1;                        // 0x474C8C
extern unsigned char g_GameStageState[];               // 0x474C40
extern const char g_GameInitialEclName[];               // 0x46D278

// Ordinary source interfaces for still-unreconstructed dependencies. Several
// target bodies use private LTCG register receivers: their observed machine
// live-ins remain in the ledger/analysis, rather than invented source ABIs.
extern void GameFinishWorker(unsigned char *worker);
extern void __stdcall GamePrepareAnimationSlots(GameChainOwnerView *owner);
extern void GameEnterMenuState(GameMenuView *menu);
extern void GameResetAnimationSlots(GameChainOwnerView *owner);
extern void GameResetAnimationOwner(GameChainOwnerView *owner);
extern void __stdcall GameClearEnemyVms(EnemyManagerView *manager);
extern void GameClearLinkedObjects(GameChainOwnerView *owner);
extern void GameResetGui(GameGuiUpdateView *gui);
extern void __stdcall GamePlayStageMusic(int channel, int musicIndex);
extern void GameAssignMusic(unsigned char *music, const char *name, int mode, int value);
extern void __stdcall GameDestroyMenu(GameMenuView *menu);
extern void GameEmitDemoEffect(int script, int type, int duration, int x, int y, int z);
extern void GameQueueSound(unsigned char *sound, int soundId);
extern void GameUpdateGui(GameGuiUpdateView *gui);
extern void GameUpdateFaithDecay(unsigned char *state);
extern void GameAdvanceTimer(PlayerTimerView *timer);
extern void EnemyMarkPendingInterrupt(int *vmId);

static inline void MarkGameChains(GameChainOwnerView *owner)
{
    if (owner->updateChain != NULL)
        owner->updateChain->flags |= 2;
    if (owner->drawChain != NULL)
        owner->drawChain->flags |= 2;
}

static __forceinline void ResetGameStage()
{
    GameResetAnimationOwner(g_GameOwner4776F0);
    PlayerResetRuntimeState(g_Player);
    memset(reinterpret_cast<u8 *>(g_GameOwner477818) + 0x14, 0, 0x21cea0);
    GameClearEnemyVms(g_EnemyManager);
    GameClearLinkedObjects(g_GameOwner47781C);
    g_GameFrameCounter0 = 0;
    g_GameFrameCounter1 = 0;
    g_ReplayManager->BeginStage();
    EnemySpawnRequestView request;
    memset(&request, 0, sizeof(request));
    EnemySpawn(g_EnemyManager, g_GameInitialEclName, &request);
    GameResetGui(g_GameGuiUpdate);
    MarkGameChains(g_GameOwner477830);
    GameChainOwnerView *playerChains = reinterpret_cast<GameChainOwnerView *>(g_Player);
    playerChains->updateChain->flags |= 2;
    playerChains->drawChain->flags |= 2;
    RebuildPlayerOptions(g_Player);
    MarkGameChains(g_GameOwner4776F0);
    MarkGameChains(reinterpret_cast<GameChainOwnerView *>(g_EnemyManager));
    MarkGameChains(g_GameOwner477818);
    MarkGameChains(g_GameOwner47781C);
    g_GameOwner4776FC->updateChain->flags |= 2;
    g_GameOwner4776FC->drawChain->flags |= 2;
    MarkGameChains(g_GameOwner4776EC);
    g_GameOwner477840->updateChain->flags |= 2;
    g_GameOwner477840->drawChain->flags |= 2;
    MarkGameChains(&g_GameOwner4776F4->chains);
    g_GameOwner4776F4->extraChain->flags |= 2;
    MarkGameChains(g_GameOwner477814);
}

// The registered ECX adapter pushes its owner and the target core returns RET4.
// This noinline source split models that verified machine boundary; original
// source-written versus LTCG-outline provenance remains unknown.
__declspec(noinline) ZunBool __stdcall GameManagerUpdateBody(GameManager *manager);

ZunBool __fastcall GameManager::OnUpdate(GameManager *manager)
{
    return GameManagerUpdateBody(manager);
}

ZunBool __stdcall GameManagerUpdateBody(GameManager *manager)
{
    if (manager->stageTimer.current == 0)
    {
        if ((manager->m_Flags & 8) != 0)
        {
            GameFinishWorker(g_GameWorkerState);
            g_GameTransitionState = (~(g_GameConfigurationFlags >> 12) & 1) | 2;
            return 1;
        }
        GamePrepareAnimationSlots(g_GameAnimationSlots);
        if (g_GameMenu != NULL)
        {
            GameEnterMenuState(g_GameMenu);
            GameResetAnimationSlots(g_GameAnimationSlots);
            manager->m_Flags |= 0x800;
            EnemyMarkPendingInterrupt(&g_GameGuiUpdate->transitionVmId);
        }
        else
        {
            manager->m_Flags &= ~0x800;
            ResetGameStage();
            if ((g_GameRuntimeFlags & 0x20) == 0)
                GamePlayStageMusic(0, g_GameStageUpdate->musicIndex);
            GameEffectUpdateView *effect = g_GameEffectUpdate;
            EnemyMarkPendingInterrupt(&effect->firstVmId);
            EnemyMarkPendingInterrupt(&effect->secondVmId);
            effect->secondVmId = 0;
        }
    }
    else if (manager->stageTimer.current == 30 && (manager->m_Flags & 0x800) != 0)
    {
        manager->m_Flags &= ~0x800;
        ResetGameStage();
        GameAssignMusic(g_GameMusicState, "dummy",
            (g_GameConfigurationFlags491D78 & 0x10) != 0 ? 4 : 3, 0);
        GamePlayStageMusic(0, g_GameStageUpdate->musicIndex);
        GameEffectUpdateView *effect = g_GameEffectUpdate;
        EnemyMarkPendingInterrupt(&effect->secondVmId);
        effect->secondVmId = 0;
        manager->stageTimer.SetCurrent(0);
    }

    GameMenuView *menu = g_GameMenu;
    if (menu != NULL && (menu->flags2A18 & 8) != 0)
    {
        GameDestroyMenu(menu);
        free(menu);
    }
    if ((manager->m_Flags & 4) != 0)
    {
        manager->m_Flags |= 0x80;
        return 1;
    }

    GameFinishWorker(g_GameWorkerState);
    if ((g_GameRuntimeFlags & 0x20) != 0)
    {
        if ((g_GameInputBits & 0x160b) != 0 || (manager->m_Flags & 0x70) != 0)
            g_GameTransitionState = (g_GameConfigurationFlags & 0x1000) != 0 ? 2 : 4;
        if (manager->stageTimer.current == 2940)
            GameEmitDemoEffect(43, 5, 60, 0, 0, 0);
        else if (manager->stageTimer.current == 3000)
            GameQueueSound(g_GameSoundState, 4);
    }
    GameUpdateGui(g_GameGuiUpdate);
    if ((manager->m_Flags & 0x10) != 0 || (manager->m_Flags & 0x20) != 0 ||
        (manager->m_Flags & 0x40) != 0)
        return 3;

    if (g_ReplayManager->mode != REPLAY_MANAGER_PLAYBACK)
    {
        GameStatisticsRowView &row =
            g_GameStatisticsRows[g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3];
        if (row.activeFrames < 215999999)
            ++row.activeFrames;
    }
    if (*reinterpret_cast<int *>(reinterpret_cast<u8 *>(g_EnemyManager) + 0x10) == 0 &&
        g_GameGuiUpdate->dialogueState == 0 &&
        manager->stageTimer.current >= 90)
        GameUpdateFaithDecay(g_GameStageState);
    ++g_GameFrameCounter0;
    ++g_GameFrameCounter1;
    GameAdvanceTimer(&manager->stageTimer);
    return 1;
}

struct GameStageStatisticView
{
    u8 unknown000[0x18];
    int initialScore;
    u8 unknown01C;
    signed char startupValue1D;
    u8 unknown01E[0xd2];
};
// The target clears these two eight-byte spans through floating zero stores;
// their original member types are not established by this partial storage view.
struct GameFpsResetView
{
    u8 unknown000[0x24];
    double pair24;
    double pair2C;
};
typedef char GameStageStatisticStrideF0[(sizeof(GameStageStatisticView) == 0xf0) ? 1 : -1];
typedef char GameManagerModeAt5C[(offsetof(GameManager, initializationMode) == 0x5c) ? 1 : -1];
typedef char GameChainOwnerAt20[(offsetof(GameChainElementView, owner) == 0x20) ? 1 : -1];

typedef ZunBool (__fastcall *GameUpdateCallback)(GameManager *);
typedef ZunBool (__fastcall GameManager::*GameDrawCallback)();
typedef char GameDrawCallbackIsFourBytes[(sizeof(GameDrawCallback) == 4) ? 1 : -1];
extern GameChainElementView *__stdcall GameCreateUpdateChain(GameUpdateCallback callback);
extern GameChainElementView *__stdcall GameCreateDrawChain(GameDrawCallback callback);
extern void GameRegisterUpdateChain(void *chain, int priority, GameChainElementView *element);
extern void GameRegisterDrawChain(void *chain, int priority, GameChainElementView *element);
extern void *g_GameMainChain;                              // 0x491BE4
extern u32 g_GameConfigurationSnapshot[13];               // 0x491D48
extern int g_GameNewSession;                              // 0x491FC4
extern int g_GameStartingStage;                           // 0x474C74
extern int g_GameStartupMode;                             // 0x474C7C
extern int g_GameStartupValue474C94;                            // 0x474C94
extern int g_GameStartupValue474C90;                           // 0x474C90
extern volatile int g_PlayerLivesDisplayCount;                            // 0x474C70
extern int g_GameLivesOverride;                            // 0x474CAC
extern short g_PlayerPower;
extern int g_GameRankResetCounter;                        // 0x474C9C
extern int g_EnemyRank;                                   // 0x474C98
extern int g_GameShotStartupValue;                        // 0x474C50
extern const char g_GameDefaultReplayPath[];              // 0x477710
extern GameFpsResetView *g_GameFpsReset;                   // 0x477708
extern int g_GameLoaderPending;                          // 0x494518
extern int g_GameFrameRestorePending;                    // 0x474C84
extern int g_GameWorkerReady;                            // 0x492260
extern int g_GameWorkerCancelled;                        // 0x492264
extern int g_GameStartupCounter;                         // 0x4918A4
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);
extern void *__stdcall GameCreateMenu(const char *path, int mode);
extern void *GameCreateGui();
extern void *GameCreateProjectilePool();
extern void *GameCreateItemManager();
extern void *GameCreateOwner41C290();
extern void *GameCreateController();
extern void *GameCreateStageOwner();
extern void *GameCreateOwner42B660();
extern void GameRestoreReplayState(ReplayManager *replay);
extern void __stdcall GameReloadGui(GameGuiUpdateView *gui);
extern void *GameCreateOwner40AF90();
extern void *GameCreateLoaderOwner();
extern void *GameCreateOwner408C90();
extern void GamePrepareDummyMusic();
extern void __stdcall GamePrepareMusicPath(int slot, const char *path);
extern void __stdcall GameStartSoundState(unsigned char *sound);
extern void __stdcall GameStopSoundState(unsigned char *sound);
extern void GamePublishStartup(GameManager *manager);
namespace th10 { namespace leaf {
    unsigned int Fn00417800(unsigned char *owner);
} }

// 0x417870: actual registration and full startup/error paths. The two callback
// pointer source types are hypotheses; VC7.1 represents this nonvirtual member
// pointer in four bytes, matching the observed draw registration argument.
int __stdcall GameManagerInitialize(GameManager *manager)
{
    GameChainElementView *element;
    manager->m_Flags |= 4;
    while (reinterpret_cast<int *>(g_AnmManagerPtr)[0] >= 0 ||
        reinterpret_cast<int *>(g_AnmManagerPtr)[1] >= 0)
    {
        if ((g_GameConfigurationFlags & 0x80) != 0)
            goto failed;
        Sleep(1);
    }
    g_PlayerTimerScale = 1.0f;
    g_GameFrameCounter0 = 0;
    g_GameFrameCounter1 = 0;
    if (g_GameNewSession != 0)
    {
        if (g_GameStartupMode == 7)
            g_GameStartingStage = 4;
        GameStatisticsRowView *statistics = g_GameStatisticsRows;
        GameStatisticsRowView &row =
            statistics[g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3];
        GameStageStatisticView &stage =
            reinterpret_cast<GameStageStatisticView *>(&row)[g_GameStartingStage];
        g_GameScoreState.unknown00 = stage.initialScore;
        g_GameStartupValue474C94 = stage.startupValue1D;
        if ((g_GameRuntimeFlags & 8) == 0)
            g_GameStartupValue474C90 = 0;
        g_GameScoreState.score = 0;
        g_GameScoreState.SetFaith(50000);
        if ((g_GameRuntimeFlags & 0x10) == 0)
            g_PlayerLivesDisplayCount = 2;
        else if (g_GameLivesOverride == 0)
            g_PlayerLivesDisplayCount = 9;
        else
            g_PlayerLivesDisplayCount = g_GameLivesOverride - 1;
        g_PlayerPower = g_GameStartupMode != 1 ? 80 : 0;
        g_GameRuntimeFlags &= ~4;
        if (manager->initializationMode == 0)
        {
            GameStatisticsRowView &startRow =
                statistics[g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3];
            if (startRow.sessionStarts < 99999)
                ++startRow.sessionStarts;
        }
        g_GameRankResetCounter = 0;
        g_EnemyRank = (g_GameRuntimeFlags & 8) != 0 ? -512 : 0;
    }
    else if (g_GameScoreState.unknown00 < g_GameScoreState.score)
        g_GameScoreState.unknown00 = g_GameScoreState.score;

    g_GameShotStartupValue = 9;
    element = GameCreateUpdateChain(&GameManager::OnUpdate);
    element->flags &= ~2;
    element->owner = manager;
    GameRegisterUpdateChain(g_GameMainChain, 10, element);
    manager->updateChain = element;
    element = GameCreateDrawChain(&GameManager::OnDraw);
    element->flags &= ~2;
    element->owner = manager;
    GameRegisterDrawChain(g_GameMainChain, 4, element);
    manager->drawChain = element;
    memcpy(manager->configuration, g_GameConfigurationSnapshot, 0x34);
    manager->stageResourceIndex = g_GameStageUpdate->resourceIndex;

    if ((g_GameRuntimeFlags & 2) == 0)
    {
        if (ReplayManager::Create(manager->initializationMode, g_GameDefaultReplayPath) == NULL ||
            GameCreateMenu(g_GameStageUpdate->menuPath, 0) == NULL ||
            GameCreateGui() == NULL || PlayerCreate() == NULL ||
            GameCreateProjectilePool() == NULL || GameCreateItemManager() == NULL ||
            GameCreateOwner41C290() == NULL || GameCreateController() == NULL ||
            GameCreateStageOwner() == NULL || GameCreateOwner42B660() == NULL)
            goto failed;
    }
    else
    {
        GameRestoreReplayState(g_ReplayManager);
        GameReloadGui(g_GameGuiUpdate);
        if (GameCreateMenu(g_GameStageUpdate->menuPath, 0) == NULL)
            goto failed;
    }
    if ((g_GameRuntimeFlags & 9) == 0)
    {
        if (EnemyManagerCreate(g_GameStageUpdate->enemyPath) == NULL)
            goto failed;
    }
    else
        th10::leaf::Fn00417800(reinterpret_cast<u8 *>(g_EnemyManager));
    if (GameCreateOwner40AF90() == NULL || GameCreateLoaderOwner() == NULL ||
        GameCreateOwner408C90() == NULL)
        goto failed;
    if ((g_GameRuntimeFlags & 0x20) == 0)
    {
        GamePrepareDummyMusic();
        GamePrepareMusicPath(0, g_GameStageUpdate->musicPaths[0]);
        GamePrepareMusicPath(1, g_GameStageUpdate->musicPaths[1]);
    }
    g_GameFpsReset->pair2C = 0.0;
    g_GameFpsReset->pair24 = 0.0;
    manager->stageTimer.SetCurrent(0);
    while (g_GameLoaderPending != 0)
        Sleep(16);
    if (g_GameFrameRestorePending != 0)
        g_GameFrameCounter1 = 0;
    g_GameFrameRestorePending = 0;
    GameStartSoundState(g_GameSoundState);
    manager->m_Flags &= ~4;
    g_GameRuntimeFlags &= ~0xb;
    g_GameWorkerCancelled = 0;
    g_GameWorkerReady = 1;
    g_GameStartupCounter = 0;
    GamePublishStartup(manager);
    return 0;

failed:
    manager->m_Flags |= 8;
    GameStopSoundState(g_GameSoundState);
    g_GameWorkerCancelled = 0;
    g_GameWorkerReady = 1;
    MarkGameChains(reinterpret_cast<GameChainOwnerView *>(manager));
    return -1;
}
