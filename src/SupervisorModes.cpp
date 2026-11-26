#include "Main.hpp"
#include "Chain.hpp"
#include "GameManager.hpp"
#include "GameStageDescriptor.hpp"
#include "GameScoreState.hpp"
#include "FrontEnd.hpp"
#include "Ending.hpp"
#include "InputPolling.hpp"
#include "Sound.hpp"
#include <new>

extern void ReleaseModeOwners();
extern int g_MainModeTransitionCounter; //491C00, original role unknown.
extern unsigned char g_ScriptViewerRouteDescriptor[];
struct StartupWorkerView;
extern StartupWorkerView *CreateStartupWorker();

// Actual nullable lifecycle seams418150/42CDB0. The callees consume one stack
// owner; their private optimizer register choices are not source declarations.
void GameManagerDelete(GameManager *manager)
{
    if (manager != NULL)
    {
        GameManagerCleanup(manager);
        ::operator delete(manager);
    }
}

void FrontEndDelete(FrontEndControllerView *controller)
{
    delete controller;
}

// Target421C50..421C68.
void MainSupervisorView::LeaveLock(int index)
{
    ::LeaveCriticalSection(&criticalSections[index]);
    --criticalSectionLockCounts[index];
}

// Evidenced by MainSupervisorView::UpdateGameMode transitions (0x004218D0)
// and MainSupervisorView::OnUpdate (0x0041FF80).
enum SupervisorGameMode
{
    SUPERVISOR_MODE_STARTUP_INIT                = 0,
    SUPERVISOR_MODE_STARTUP_LOADING             = 1,
    SUPERVISOR_MODE_BOOT_READY                  = 2,
    SUPERVISOR_MODE_EXIT                        = 3,
    SUPERVISOR_MODE_TITLE                       = 4,
    SUPERVISOR_MODE_GAMEPLAY                    = 7,
    SUPERVISOR_MODE_NEXT_STAGE                  = 10,
    SUPERVISOR_MODE_RETRY_STAGE                 = 11,
    SUPERVISOR_MODE_REPLAY_PLAYBACK             = 12,
    SUPERVISOR_MODE_RESTART_GAME                = 13,
    SUPERVISOR_MODE_ENDING                      = 14,
    SUPERVISOR_MODE_RETURN_TO_TITLE_AFTER_CLEAR = 15
};

enum SupervisorLockSlot
{
    SUPERVISOR_LOCK_MODE = 5
};

// Target4218D0: code through421B0B plus two jump tables and a byte index map.
int MainSupervisorView::UpdateGameMode()
{
    if (currentGameMode != nextGameMode)
    {
        ::EnterCriticalSection(&criticalSections[SUPERVISOR_LOCK_MODE]);
        int previous = currentGameMode;
        ++criticalSectionLockCounts[SUPERVISOR_LOCK_MODE];
        previousGameMode = previous;
        clearColor = 0xff000000u;
        switch (nextGameMode)
        {
        case SUPERVISOR_MODE_STARTUP_INIT:
            nextGameMode = SUPERVISOR_MODE_STARTUP_LOADING;
            startupWorker = CreateStartupWorker();
            if (startupWorker != NULL)
                break;
            nextGameMode = SUPERVISOR_MODE_EXIT;
            goto quit;
        case SUPERVISOR_MODE_TITLE:
            switch (previous)
            {
            case SUPERVISOR_MODE_GAMEPLAY:
                GameManagerDelete(g_GameManager);
                // Fall through to recreate the title screen.
            case SUPERVISOR_MODE_STARTUP_LOADING:
            case SUPERVISOR_MODE_BOOT_READY:
                FrontEndCreate();
                break;
            case SUPERVISOR_MODE_ENDING:
                EndingDelete(g_EndingView);
                FrontEndCreate();
                break;
            }
            break;
        case SUPERVISOR_MODE_RETURN_TO_TITLE_AFTER_CLEAR:
            switch (previous)
            {
            case SUPERVISOR_MODE_BOOT_READY:
                break;
            case SUPERVISOR_MODE_ENDING:
                EndingDelete(g_EndingView);
                break;
            case SUPERVISOR_MODE_GAMEPLAY:
                GameManagerDelete(g_GameManager);
                break;
            default:
                goto finish;
            }
            nextGameMode = SUPERVISOR_MODE_TITLE;
            g_MainModeTransitionCounter = 3;
            FrontEndCreate();
            break;
        case SUPERVISOR_MODE_GAMEPLAY:
            if (previous == SUPERVISOR_MODE_TITLE)
                FrontEndDelete(g_FrontEndControllerView);
            newGameSession = 1;
            GameManagerCreate(0);
            break;
        case SUPERVISOR_MODE_REPLAY_PLAYBACK:
            if (previous == SUPERVISOR_MODE_TITLE)
                FrontEndDelete(g_FrontEndControllerView);
            nextGameMode = SUPERVISOR_MODE_GAMEPLAY;
            newGameSession = 1;
            GameManagerCreate(1);
            break;
        case SUPERVISOR_MODE_RETRY_STAGE:
        {
            int mode = g_GameManager->initializationMode;
            newGameSession = 0;
            if (previous == SUPERVISOR_MODE_GAMEPLAY)
                GameManagerDelete(g_GameManager);
            nextGameMode = SUPERVISOR_MODE_GAMEPLAY;
            GameManagerCreate(mode);
            break;
        }
        case SUPERVISOR_MODE_NEXT_STAGE:
            GameManagerDelete(g_GameManager);
            newGameSession = 1;
            nextGameMode = SUPERVISOR_MODE_GAMEPLAY;
            g_GameScoreState.SelectNextStage();
            GameManagerCreate(0);
            break;
        case SUPERVISOR_MODE_RESTART_GAME:
            GameManagerDelete(g_GameManager);
            newGameSession = 1;
            nextGameMode = SUPERVISOR_MODE_GAMEPLAY;
            GameManagerCreate(0);
            break;
        case SUPERVISOR_MODE_ENDING:
            if (previous == SUPERVISOR_MODE_GAMEPLAY)
                GameManagerDelete(g_GameManager);
            EndingCreate();
            break;
        case SUPERVISOR_MODE_EXIT:
    quit:
            ReleaseModeOwners();
            LeaveLock(SUPERVISOR_LOCK_MODE);
            return 4;
        }
    finish:
        currentGameMode = nextGameMode;
        ::LeaveCriticalSection(&criticalSections[SUPERVISOR_LOCK_MODE]);
        --criticalSectionLockCounts[SUPERVISOR_LOCK_MODE];
    }
    return 1;
}

// Target41FF80..41FFF0 is the real caller of the mode transition owner.
// Sound fade maintenance uses the already established global Sound owner.
int __fastcall MainSupervisorView::OnUpdate(MainSupervisorView *supervisor)
{
    if ((supervisor->flags & 0x80) != 0 && supervisor->modeWorker.running == 0)
        g_MainSupervisorView.nextGameMode = SUPERVISOR_MODE_EXIT;
    reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->ServiceBgmFades();
    PollInputFrame(0);
    if (g_AnmRenderManagerView->ServicePreloadedAnms() != 0)
        return 4;
    if (supervisor->updateGate != 0)
        return supervisor->updateGate == 2 ? 4 : 1;
    int result = supervisor->UpdateGameMode();
    if (result != 1)
        return result;
    return 1;
}

// Target420470..42053B is the real address escape of the calculation callback.
// Unreconstructed callback addresses remain declarations rather than stubs.
extern int __fastcall MainAddedCallback(MainSupervisorView *supervisor);
extern int __fastcall MainDrawCallback(MainSupervisorView *supervisor);
extern int MainDrawLateCallback();
extern int MainDrawFinalCallback();
int MainRegisterChain()
{
    g_MainSupervisorView.currentGameMode = -2;
    g_MainSupervisorView.nextGameMode = 0;
    g_MainSupervisorView.unknown398 = 0;
    th10::ChainElem *element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(MainSupervisorView::OnUpdate));
    element->arg = &g_MainSupervisorView;
    element->flags |= 2;
    element->addedCallback = reinterpret_cast<th10::ChainCallback>(MainAddedCallback);
    int result = th10::g_Chain->AddToCalcChain(element, 1);
    if (result != 0)
        return result;
    element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(MainDrawCallback));
    element->arg = &g_MainSupervisorView;
    element->flags |= 2;
    th10::g_Chain->AddToDrawChain(element, 1);
    element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(MainDrawLateCallback));
    element->arg = &g_MainSupervisorView;
    element->flags |= 2;
    th10::g_Chain->AddToDrawChain(element, 40);
    element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(MainDrawFinalCallback));
    element->arg = &g_MainSupervisorView;
    element->flags |= 2;
    th10::g_Chain->AddToDrawChain(element, 50);
    return 0;
}

// Target421B70..421B86: the retained member form of mode10's stage publication.
void GameScoreStateView::SelectNextStage()
{
    selectedStage = nextStage;
    g_GameStageUpdate = reinterpret_cast<GameStageUpdateView *>(
        g_ScriptViewerRouteDescriptor) + nextStage;
}
