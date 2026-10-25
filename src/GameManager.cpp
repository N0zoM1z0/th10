#include "Main.hpp" // Shield ANM imports before the Windows SDK.
#include "ItemManager.hpp"
#include "ReplayChain.hpp"
#include "Hint.hpp"
#include "GameEffectUpdate.hpp"
#include <stddef.h>
#include <new>
#include <stdlib.h>
#include <string.h>
#include "Player.hpp"
#include "ReplayManager.hpp"
#include "Enemy.hpp"
#include "EnemyLaser.hpp"
#include "GameScoreState.hpp"
#include "GameStatistics.hpp"
#include "Gui.hpp"
#include "StageScript.hpp"
#include "GameStageDescriptor.hpp"
#include "ScriptViewer.hpp"

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
    AnmVmTimerView stageTimer;
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
struct GameAdditionalChainView
{
    GameChainOwnerView chains;
    u8 unknown0010[0x379c];
    GameChainElementView *extraChain;
};
typedef char GameGuiTransitionIdAt9E14[(offsetof(GuiView, specialVmId9E14) == 0x9e14) ? 1 : -1];
typedef char GameManagerTimerAt10[(offsetof(GameManager, stageTimer) == 0x10) ? 1 : -1];
typedef char GameManagerFlagsAt58[(offsetof(GameManager, m_Flags) == 0x58) ? 1 : -1];
typedef char GameAdditionalChainAt37AC[(offsetof(GameAdditionalChainView, extraChain) == 0x37ac) ? 1 : -1];

extern GameMenuView *g_GameMenu;                       // 0x4776E4
extern GameChainOwnerView *g_GameAnimationSlots;       // 0x4776E8
extern GameChainOwnerView *g_GameOwner4776F0;
extern GameChainOwnerView *g_GameOwner477830;
extern GameChainOwnerView *g_GameOwner477818;
extern GameChainOwnerView *g_GameOwner4776FC;
extern GameChainOwnerView *g_GameOwner4776EC;
extern GameChainOwnerView *g_GameOwner477840;
extern GameAdditionalChainView *g_GameOwner4776F4;
// Target477814 uses the canonical g_GameOwner477814 declaration in Hint.hpp.
extern EnemyManagerView *g_EnemyManager;
// Target47770C uses the canonical g_GuiView declaration in Gui.hpp.
extern GameStageUpdateView *g_GameStageUpdate;          // 0x477848
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
extern void __stdcall GamePrepareAnimationSlots(GameChainOwnerView *owner);
extern void GameEnterMenuState(GameMenuView *menu);
extern void GameResetAnimationSlots(GameChainOwnerView *owner);
extern void GameResetAnimationOwner(GameChainOwnerView *owner);
extern void __stdcall GameClearEnemyVms(EnemyManagerView *manager);
extern void GameClearLinkedObjects(GameChainOwnerView *owner);
extern void __stdcall GamePlayStageMusic(int channel, int musicIndex);
extern void GameAssignMusic(unsigned char *music, const char *name, int mode, int value);
extern void __stdcall GameDestroyMenu(GameMenuView *menu);
extern void GameEmitDemoEffect(int script, int type, int duration, int x, int y, int z);
extern void GameQueueSound(unsigned char *sound, int soundId);
extern void GameUpdateGui(GuiView *gui);
extern void GameUpdateFaithDecay(unsigned char *state);
extern void EnemyMarkPendingInterrupt(int *vmId);

// Target0x4045B0 starts the primary stage's60-frame transition. The actual
// GameManager caller supplies4776E8, separately from the menu entry4776E4.
void GameResetAnimationSlots(GameChainOwnerView *owner)
{
    StageScriptView *stage = reinterpret_cast<StageScriptView *>(owner);
    stage->transitionTimer.SetCurrent(60);
    stage->flags |= 4;
}

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
    GameClearLinkedObjects(reinterpret_cast<GameChainOwnerView *>(g_EnemyLaserManager));
    g_GameFrameCounter0 = 0;
    g_GameFrameCounter1 = 0;
    g_ReplayManager->BeginStage();
    EnemySpawnRequestView request;
    memset(&request, 0, sizeof(request));
    EnemySpawn(g_EnemyManager, g_GameInitialEclName, &request);
    GameResetGui(g_GuiView);
    MarkGameChains(g_GameOwner477830);
    GameChainOwnerView *playerChains = reinterpret_cast<GameChainOwnerView *>(g_Player);
    playerChains->updateChain->flags |= 2;
    playerChains->drawChain->flags |= 2;
    RebuildPlayerOptions(g_Player);
    MarkGameChains(g_GameOwner4776F0);
    MarkGameChains(reinterpret_cast<GameChainOwnerView *>(g_EnemyManager));
    MarkGameChains(g_GameOwner477818);
    MarkGameChains(reinterpret_cast<GameChainOwnerView *>(g_EnemyLaserManager));
    g_GameOwner4776FC->updateChain->flags |= 2;
    g_GameOwner4776FC->drawChain->flags |= 2;
    MarkGameChains(g_GameOwner4776EC);
    g_GameOwner477840->updateChain->flags |= 2;
    g_GameOwner477840->drawChain->flags |= 2;
    MarkGameChains(&g_GameOwner4776F4->chains);
    g_GameOwner4776F4->extraChain->flags |= 2;
    if (g_GameOwner477814->updateChain != NULL)
        g_GameOwner477814->updateChain->flags |= 2;
    if (g_GameOwner477814->drawChain != NULL)
        g_GameOwner477814->drawChain->flags |= 2;
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
            reinterpret_cast<ScriptViewerWorkerView *>(g_GameWorkerState)->Stop();
            g_GameTransitionState = (~(g_GameConfigurationFlags >> 12) & 1) | 2;
            return 1;
        }
        GamePrepareAnimationSlots(g_GameAnimationSlots);
        if (g_GameMenu != NULL)
        {
            GameEnterMenuState(g_GameMenu);
            GameResetAnimationSlots(g_GameAnimationSlots);
            manager->m_Flags |= 0x800;
            EnemyMarkPendingInterrupt(&g_GuiView->specialVmId9E14.value);
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
        ::operator delete(menu);
    }
    if ((manager->m_Flags & 4) != 0)
    {
        manager->m_Flags |= 0x80;
        return 1;
    }

    reinterpret_cast<ScriptViewerWorkerView *>(g_GameWorkerState)->Stop();
    if ((g_GameRuntimeFlags & 0x20) != 0)
    {
        if ((g_GameInputBits & 0x160b) != 0 || (manager->m_Flags & 0x70) != 0)
            g_GameTransitionState = (g_GameConfigurationFlags & 0x1000) != 0 ? 2 : 4;
        if (manager->stageTimer.current == 2940)
            GameEmitDemoEffect(43, 5, 60, 0, 0, 0);
        else if (manager->stageTimer.current == 3000)
            GameQueueSound(g_GameSoundState, 4);
    }
    GameUpdateGui(g_GuiView);
    if ((manager->m_Flags & 0x10) != 0 || (manager->m_Flags & 0x20) != 0 ||
        (manager->m_Flags & 0x40) != 0)
        return 3;

    if (g_ReplayManager->mode != REPLAY_MANAGER_PLAYBACK)
    {
        GameStatisticsRecord &row =
            g_GameStatisticsRows->records[g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3];
        if (row.activeFrames < 215999999)
            ++row.activeFrames;
    }
    if (*reinterpret_cast<int *>(reinterpret_cast<u8 *>(g_EnemyManager) + 0x10) == 0 &&
        g_GuiView->message == NULL &&
        manager->stageTimer.current >= 90)
        GameUpdateFaithDecay(g_GameStageState);
    ++g_GameFrameCounter0;
    ++g_GameFrameCounter1;
    manager->stageTimer.Tick();
    return 1;
}

// The target clears these two eight-byte spans through floating zero stores;
// their original member types are not established by this partial storage view.
struct GameFpsResetView
{
    u8 unknown000[0x24];
    double pair24;
    double pair2C;
};
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
extern void *GameCreateGui();
extern void *GameCreateProjectilePool();
extern void *GameCreateItemManager();
extern void *GameCreateOwner41C290();
extern void *GameCreateController();
extern void *GameCreateOwner42B660();
extern void GameRestoreReplayState(ReplayManager *replay);
extern void __stdcall GameReloadGui(GuiView *gui);
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
        GameStatisticsOwner *statistics = g_GameStatisticsRows;
        GameStatisticsRecord &row =
            statistics->records[g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3];
        int startingStage = g_GameStartingStage;
        g_GameScoreState.unknown00 = row.StageInitialScore(startingStage);
        g_GameStartupValue474C94 = row.StageStartupValue(startingStage);
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
            GameStatisticsRecord &startRow =
                statistics->records[g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3];
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
            StageCreate(g_GameStageUpdate->menuPath, 0) == NULL ||
            GameCreateGui() == NULL || PlayerCreate() == NULL ||
            GameCreateProjectilePool() == NULL || GameCreateItemManager() == NULL ||
            GameCreateOwner41C290() == NULL || GameCreateController() == NULL ||
            CreateHintOwner() == NULL || GameCreateOwner42B660() == NULL)
            goto failed;
    }
    else
    {
        GameRestoreReplayState(g_ReplayManager);
        GameReloadGui(g_GuiView);
        if (StageCreate(g_GameStageUpdate->menuPath, 0) == NULL)
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

// Genuine unresolved dependencies: no native substitute implementations.
// Private target EBX/EAX receiver assignments are not source ABI declarations.
extern void __stdcall GameCleanupController422220(GameChainOwnerView *owner);
extern void __stdcall GameCleanupAnimation405F70(GameChainOwnerView *owner);
extern void __stdcall GameCleanupLinked41C1C0(GameChainOwnerView *owner);
extern void __stdcall GameCleanupOwner42B570(GameChainOwnerView *owner);
extern void GamePrepareGuiReuse414570(GuiView *owner);
extern void __stdcall GameCleanupOwner40AF00(GameChainOwnerView *owner);
extern void __stdcall GameCleanupLoader405620(GameChainOwnerView *owner);
extern void __stdcall GameCleanupOwner408AF0(GameAdditionalChainView *owner);
extern void FrontEndQueueSoundCommand(const char *path, int opcode, int argument);
extern GameManager *g_GameManager;                         // 0x477810
extern int g_GameComparison474C80;
extern int g_StageScriptGlobal4923A8;
namespace th10 { namespace leaf {
    unsigned char *Fn00418A90(unsigned char *owner);
} }

// Target417C80. Complete terminal/reuse transition owner, not an allocation release.
// Existing GameManager pointer aliases remain target-storage obligations. The
// typed casts below select already-maintained real owners, not new storage.
void __stdcall GameManagerCleanup(GameManager *manager)
{
    GameFinalizeStatistics42B1E0(g_GameStatisticsRows);
    g_GameRuntimeFlags &= ~3u;
    int transition = g_GameTransitionState;
    g_PlayerTimerScale = 1.0f;
    if (transition == 10)
    {
        AsciiManagerView *ascii = g_AsciiManagerView;
        if (ascii->auxiliaryVm89A4 == 0)
        {
            AnmFloat3View position(480.0f, 392.0f, 0.0f);
            ascii->auxiliaryVm89A4 =
                ascii->asciiAnm->CreateVmAtScreenVariant0(6, &position).value;
            transition = g_GameTransitionState;
        }
        if (g_GameComparison474C80 == g_GameStartupMode)
            g_GameRuntimeFlags |= 1u;
    }
    else if (transition == 11)
    {
        g_AsciiManagerView->EnsureSelectionVm(224.0f, 416.0f);
        g_GameRuntimeFlags |= 2u;
        transition = g_GameTransitionState;
    }
    else if (transition == 4 || transition == 15)
    {
        g_AsciiManagerView->EnsureSelectionVm(480.0f, 392.0f);
        transition = g_GameTransitionState;
    }
    else if (transition == 13)
    {
        g_AsciiManagerView->EnsureSelectionVm(480.0f, 392.0f);
        if (g_GameComparison474C80 != g_GameStartupMode)
        {
            th10::leaf::Fn00418A90(g_GameStageState);
            g_GameRuntimeFlags |= 8u;
        }
        g_GameRuntimeFlags |= 1u;
        transition = g_GameTransitionState;
    }

    if ((g_GameRuntimeFlags & 2u) == 0)
    {
        if (transition != 14 && transition != 15)
        {
            ReplayManager *replay = g_ReplayManager;
            if (replay != NULL)
                delete replay;
        }
        StageScriptView *stage = reinterpret_cast<StageScriptView *>(g_GameAnimationSlots);
        if (stage != NULL)
            delete stage;
        stage = reinterpret_cast<StageScriptView *>(g_GameMenu);
        if (stage != NULL)
            delete stage;
        GameChainOwnerView *controller = g_GameOwner477830;
        if (controller != NULL)
        {
            GameCleanupController422220(controller);
            ::operator delete(controller);
        }
        GuiView *gui = g_GuiView;
        if (gui != NULL)
        {
            gui->~GuiView();
            ::operator delete(gui);
        }
        Player *player = g_Player;
        if (player != NULL)
            delete player;
        GameChainOwnerView *animation = g_GameOwner4776F0;
        if (animation != NULL)
        {
            GameCleanupAnimation405F70(animation);
            ::operator delete(animation);
        }
        ItemManagerView *items = reinterpret_cast<ItemManagerView *>(g_GameOwner477818);
        if (items != NULL)
            delete items;
        GameChainOwnerView *linked = reinterpret_cast<GameChainOwnerView *>(g_EnemyLaserManager);
        if (linked != NULL)
        {
            GameCleanupLinked41C1C0(linked);
            ::operator delete(linked);
        }
        HintOwnerView *hint = g_GameOwner477814;
        if (hint != NULL)
        {
            CleanupHintOwner(hint);
            delete hint;
        }
        GameChainOwnerView *owner42B570 = g_GameOwner477840;
        if (owner42B570 != NULL)
        {
            GameCleanupOwner42B570(owner42B570);
            ::operator delete(owner42B570);
        }
    }
    else
    {
        GamePrepareGuiReuse414570(g_GuiView);
        StageScriptView *stage = reinterpret_cast<StageScriptView *>(g_GameMenu);
        if (stage != NULL)
            delete stage;
        g_GameMenu = reinterpret_cast<GameMenuView *>(g_GameAnimationSlots);
        GameChainOwnerView *controller = g_GameOwner477830;
        if (controller->updateChain != NULL)
            controller->updateChain->flags &= ~2u;
        if (controller->drawChain != NULL)
            controller->drawChain->flags &= ~2u;
        GameChainOwnerView *animation = g_GameOwner4776F0;
        if (animation->updateChain != NULL)
            animation->updateChain->flags &= ~2u;
        if (animation->drawChain != NULL)
            animation->drawChain->flags &= ~2u;
        ReplayManager *replay = g_ReplayManager;
        if (replay->playbackChain != NULL)
            replay->playbackChain->unknownFlag1 = 0;
        if (replay->drawChain != NULL)
            replay->drawChain->unknownFlag1 = 0;
        ItemManagerView *items = reinterpret_cast<ItemManagerView *>(g_GameOwner477818);
        memset(items->items, 0, 0x21cea0);
    }

    if ((g_GameRuntimeFlags & 9u) == 0)
    {
        EnemyManagerView *enemy = g_EnemyManager;
        if (enemy != NULL)
            delete enemy;
    }
    else
        GameClearEnemyVms(g_EnemyManager);
    GameChainOwnerView *owner40AF00 = g_GameOwner4776FC;
    if (owner40AF00 != NULL)
    {
        GameCleanupOwner40AF00(owner40AF00);
        ::operator delete(owner40AF00);
    }
    GameChainOwnerView *loader = g_GameOwner4776EC;
    if (loader != NULL)
    {
        GameCleanupLoader405620(loader);
        ::operator delete(loader);
    }
    GameAdditionalChainView *owner408AF0 = g_GameOwner4776F4;
    if (owner408AF0 != NULL)
    {
        GameCleanupOwner408AF0(owner408AF0);
        ::operator delete(owner408AF0);
    }

    GameChainElementView *element = manager->updateChain;
    th10::Chain *chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(reinterpret_cast<th10::ChainElem *>(element), chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    element = manager->drawChain;
    chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(reinterpret_cast<th10::ChainElem *>(element), chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    g_GameManager = NULL;
    if ((g_GameRuntimeFlags & 0x22u) == 0)
        FrontEndQueueSoundCommand("dummy",
            (g_GameConfigurationFlags491D78 & 0x10u) != 0 ? 4 : 3, 0);
    unsigned int finalFlags = g_GameRuntimeFlags;
    g_GameStartupCounter = 1;
    g_StageScriptGlobal4923A8 = (finalFlags & 1u) != 0 ? 0 : static_cast<int>(0xff000000u);
}
