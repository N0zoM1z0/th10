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

// Target4218D0: code through421B0B plus two jump tables and a byte index map.
int MainSupervisorView::UpdateGameMode()
{
    if (currentGameMode != nextGameMode)
    {
        ::EnterCriticalSection(&criticalSections[5]);
        ++criticalSectionLockCounts[5];
        previousGameMode = currentGameMode;
        clearColor = 0xff000000u;
        switch (nextGameMode)
        {
        case 0:
            nextGameMode = 1;
            startupWorker = CreateStartupWorker();
            if (startupWorker == NULL)
            {
                nextGameMode = 3;
                goto quit;
            }
            break;
        case 3:
    quit:
            ReleaseModeOwners();
            LeaveLock(5);
            return 4;
        case 4:
            switch (previousGameMode)
            {
            case 1:
            case 2:
                break;
            case 7:
                GameManagerDelete(g_GameManager);
                break;
            case 14:
                EndingDelete(g_EndingView);
                break;
            default:
                goto finish;
            }
    createMenu:
            FrontEndCreate();
            break;
        case 7:
            if (previousGameMode == 4)
                FrontEndDelete(g_FrontEndControllerView);
            newGameSession = 1;
            GameManagerCreate(0);
            break;
        case 10:
            GameManagerDelete(g_GameManager);
            newGameSession = 1;
            nextGameMode = 7;
            g_GameScoreState.SelectNextStage();
            GameManagerCreate(0);
            break;
        case 11:
            {
                int mode = g_GameManager->initializationMode;
                newGameSession = 0;
                if (previousGameMode == 7)
                    GameManagerDelete(g_GameManager);
                nextGameMode = 7;
                GameManagerCreate(mode);
            }
            break;
        case 12:
            if (previousGameMode == 4)
                FrontEndDelete(g_FrontEndControllerView);
            nextGameMode = 7;
            newGameSession = 1;
            GameManagerCreate(1);
            break;
        case 13:
            GameManagerDelete(g_GameManager);
            newGameSession = 1;
            nextGameMode = 7;
            GameManagerCreate(0);
            break;
        case 14:
            if (previousGameMode == 7)
                GameManagerDelete(g_GameManager);
            EndingCreate();
            break;
        case 15:
            switch (previousGameMode)
            {
            case 2:
                break;
            case 7:
                GameManagerDelete(g_GameManager);
                break;
            case 14:
                EndingDelete(g_EndingView);
                break;
            default:
                goto finish;
            }
            nextGameMode = 4;
            g_MainModeTransitionCounter = 3;
            goto createMenu;
        }
    finish:
        currentGameMode = nextGameMode;
        ::LeaveCriticalSection(&criticalSections[5]);
        --criticalSectionLockCounts[5];
    }
    return 1;
}

// Target41FF80..41FFF0 is the real caller of the mode transition owner.
// Sound fade maintenance uses the already established global Sound owner.
int __fastcall MainSupervisorView::OnUpdate(MainSupervisorView *supervisor)
{
    if ((supervisor->flags & 0x80) != 0 && supervisor->closeRequestBlocked == 0)
        g_MainSupervisorView.nextGameMode = 3;
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
    element->flags |= 2;
    element->arg = &g_MainSupervisorView;
    element->addedCallback = reinterpret_cast<th10::ChainCallback>(MainAddedCallback);
    int result = th10::Chain::AddToCalcChain(element, 1, th10::g_Chain);
    if (result != 0)
        return result;
    element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(MainDrawCallback));
    element->flags |= 2;
    element->arg = &g_MainSupervisorView;
    th10::Chain::AddToDrawChain(element, 1, th10::g_Chain);
    element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(MainDrawLateCallback));
    element->flags |= 2;
    element->arg = &g_MainSupervisorView;
    th10::Chain::AddToDrawChain(element, 40, th10::g_Chain);
    element = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(MainDrawFinalCallback));
    element->flags |= 2;
    element->arg = &g_MainSupervisorView;
    th10::Chain::AddToDrawChain(element, 50, th10::g_Chain);
    return 0;
}

// Target421B70..421B86: the retained member form of mode10's stage publication.
void GameScoreStateView::SelectNextStage()
{
    selectedStage = nextStage;
    g_GameStageUpdate = reinterpret_cast<GameStageUpdateView *>(
        g_ScriptViewerRouteDescriptor) + nextStage;
}
