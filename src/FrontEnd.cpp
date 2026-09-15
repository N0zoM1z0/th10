#include "Main.hpp"
#include "FrontEnd.hpp"
#include "ReplayManager.hpp"

#include <string.h>


struct FrontEndInputView
{
    unsigned int current;
    unsigned short repeated;
    unsigned short pressed;
};

extern FrontEndInputView g_FrontEndInput;
extern unsigned int g_FrontEndFlags;
extern int g_FrontEndMode;
extern int g_FrontEndDemoFrames;
extern int g_FrontEndDemoIndex;
extern const char *g_FrontEndDemoPaths[4];
extern char g_FrontEndDemoPath[];
extern unsigned char g_FrontEndStageRecords[][0x30];
extern unsigned char *g_FrontEndSelectedStageRecord;
extern int g_FrontEndSelectedStage;
extern int g_FrontEndSelectedStageMirror;
extern int g_FrontEndSavedDifficulty;
extern unsigned int g_FrontEndSupervisorFlags;
extern int g_FrontEndNextGameMode;

extern int g_ReplayCurrentStage;
extern int g_ReplayCharacter;
extern int g_ReplayShotType;
extern int g_ReplayDifficulty;

extern int g_FrontEndSoundBgmVolume;
extern int g_FrontEndSoundSfxVolume;
extern int g_FrontEndSoundBgmAttenuation;

extern void FrontEndQueueSoundCommand(
    const char *command, int parameter0, int parameter1);
extern void FrontEndPlaySound(int soundId);
extern void FrontEndLoadMusic(int slot, const char *path);
extern void FrontEndPlayMusic(int slot, int loopStart);
extern void FrontEndResetAsciiAuxiliary(int *state);

extern int FrontEndUpdateMainMenu(FrontEndControllerView *controller);
extern int FrontEndUpdateMainMenuReturn(FrontEndControllerView *controller);
extern int FrontEndBeginGame();
extern int FrontEndUpdateKeyConfig(FrontEndControllerView *controller);
extern int FrontEndUpdateDifficulty(FrontEndControllerView *controller);
extern int FrontEndUpdateCharacter(FrontEndControllerView *controller);
extern int FrontEndUpdateShotType(FrontEndControllerView *controller);
extern int FrontEndUpdateStage(FrontEndControllerView *controller);
extern int FrontEndUpdateReplay(FrontEndControllerView *controller);
extern int FrontEndUpdatePractice(FrontEndControllerView *controller);
extern int FrontEndUpdateMusicRoom(FrontEndControllerView *controller);
extern int FrontEndUpdateSpecial(FrontEndControllerView *controller);
extern int FrontEndUpdateResult(FrontEndControllerView *controller);


namespace
{

enum
{
    FRONT_END_FLAG_DEMO_TRANSITION = 0x20,
    FRONT_END_TRANSITION_ACTIVE = 0x02,
    FRONT_END_ROOT_VM = 1,
    FRONT_END_RENDER_LAYER = 15,
    FRONT_END_INPUT_UP = 0x10,
    FRONT_END_INPUT_DOWN = 0x20,
    FRONT_END_INPUT_LEFT = 0x40,
    FRONT_END_INPUT_RIGHT = 0x80,
    FRONT_END_INPUT_CANCEL = 0x0a,
    FRONT_END_INPUT_CONFIRM = 0x1001,
    FRONT_END_SOUND_PREVIEW = 4,
    FRONT_END_SOUND_SELECT = 10,
    FRONT_END_SOUND_CANCEL = 11,
    FRONT_END_SOUND_MOVE = 12
};

static void ResetStateTimer(FrontEndControllerView *controller)
{
    controller->stateTimer.SetCurrent(0);
}

static void SetScreen(FrontEndControllerView *controller, int screen)
{
    controller->screen = screen;
    controller->screenState = 0;
    ResetStateTimer(controller);
}

static void SetScreenState(FrontEndControllerView *controller, int state)
{
    controller->screenState = state;
    ResetStateTimer(controller);
}

static void CreateVm(FrontEndControllerView *controller, int scriptIndex)
{
    controller->vmIds[scriptIndex] =
        controller->frontAnm->CreateVmVariant0(
            scriptIndex, FRONT_END_RENDER_LAYER);
}

static void InterruptVmNow(
    FrontEndControllerView *controller, int index, short interrupt)
{
    g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
        controller->vmIds[index].value, interrupt);
}

static void InterruptVm(
    FrontEndControllerView *controller, int index, short interrupt)
{
    g_AnmRenderManagerView->SetVmPendingInterrupt(
        controller->vmIds[index].value, interrupt);
}

static int InputRepeated(unsigned short mask)
{
    return ((g_FrontEndInput.pressed | g_FrontEndInput.repeated) & mask) != 0;
}

static int TimerReachedMultiple(
    const AnmVmTimerView *timer, int divisor)
{
    return timer->current != timer->previous &&
        timer->current % divisor == 0;
}

static AnmVmView *ResolveVm(AnmVmIdView *id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(id->value);
    if (vm == NULL)
        id->value = 0;
    return vm;
}

static int FindChildVmId(
    AnmVmIdView *parentId, short scriptIndex)
{
    AnmVmView *parent = ResolveVm(parentId);
    AnmVmLayerNodeView *node = &parent->layerNode;
    while (node != NULL)
    {
        AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
        if (vm->scriptIndex == scriptIndex)
            return vm->id;
        node = node->next;
    }
    return 0;
}

static AnmVmView *FindChildVm(
    FrontEndControllerView *controller, short scriptIndex)
{
    return g_AnmRenderManagerView->FindVm(
        FindChildVmId(
            &controller->vmIds[FRONT_END_ROOT_VM], scriptIndex));
}

static void SetChildSprite(
    FrontEndControllerView *controller, short scriptIndex, int spriteIndex)
{
    AnmVmView *vm = FindChildVm(controller, scriptIndex);
    if (vm != NULL)
        vm->anmFile->SetSprite(vm, spriteIndex);
}

static void SetChildDrawEnabled(
    FrontEndControllerView *controller, short scriptIndex, int enabled)
{
    FindChildVm(controller, scriptIndex)->drawEnabled = enabled != 0;
}

static void SetColorModeSprites(FrontEndControllerView *controller)
{
    static const short sprites[3][2] = {
        {0x2e, 0x2f},
        {0x30, 0x31},
        {0x2c, 0x2d}
    };
    int colorMode = g_MainSupervisorView.colorMode;
    SetChildSprite(controller, 0x1f, sprites[colorMode][0]);
    SetChildSprite(controller, 0x20, sprites[colorMode][1]);
}

static void SetVolumeDigitSprites(
    FrontEndControllerView *controller, int value,
    short normalFirst, short selectedFirst)
{
    SetChildSprite(controller, normalFirst + 0, value / 100 + 0x33);
    SetChildSprite(controller, normalFirst + 1, value / 10 % 10 + 0x33);
    SetChildSprite(controller, normalFirst + 2, value % 10 + 0x33);
    SetChildSprite(controller, selectedFirst + 0, value / 100 + 0x3d);
    SetChildSprite(controller, selectedFirst + 1, value / 10 % 10 + 0x3d);
    SetChildSprite(controller, selectedFirst + 2, value % 10 + 0x3d);
}

static void SetVolumeLeadingDigitVisibility(
    FrontEndControllerView *controller, int value,
    short normalFirst, short selectedFirst)
{
    int showHundreds = value >= 100;
    int showTens = value >= 10;
    SetChildDrawEnabled(controller, normalFirst + 0, showHundreds);
    SetChildDrawEnabled(controller, normalFirst + 1, showTens);
    SetChildDrawEnabled(controller, selectedFirst + 0, showHundreds);
    SetChildDrawEnabled(controller, selectedFirst + 1, showTens);
}

static void BeginOptionsClose(FrontEndControllerView *controller)
{
    InterruptVm(controller, FRONT_END_ROOT_VM, 6);
    FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
    SetScreenState(controller, FRONT_END_OPTIONS_CLOSING);
}

static void InitializeFrontEndScreen(FrontEndControllerView *controller)
{
    int mode = g_FrontEndMode;

    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(
        g_AnmRenderManagerView->loadedAnms[6]);
    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(
        g_AnmRenderManagerView->loadedAnms[7]);
    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(
        g_AnmRenderManagerView->loadedAnms[2]);
    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(
        g_AnmRenderManagerView->loadedAnms[3]);
    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(
        g_AnmRenderManagerView->loadedAnms[0]);

    int *asciiAuxiliary = reinterpret_cast<int *>(
        g_AsciiManagerView->unknown89A0 + 4);
    FrontEndResetAsciiAuxiliary(asciiAuxiliary);
    *asciiAuxiliary = 0;

    if (mode == 3)
    {
        controller->cursor.count = 10;
        controller->cursor.current = 0;
        controller->cursor.Push();
        SetScreen(controller, FRONT_END_SCREEN_SPECIAL);
        CreateVm(controller, 0x5b);
        InterruptVm(controller, 0x5b, 9);
        controller->transitionOwner->flags004 |= FRONT_END_TRANSITION_ACTIVE;
        g_FrontEndMode = 1;
        FrontEndUpdateSpecial(controller);
        return;
    }

    if ((g_FrontEndFlags & FRONT_END_FLAG_DEMO_TRANSITION) == 0)
    {
        FrontEndLoadMusic(0, "bgm/th10_02.wav");
        FrontEndPlayMusic(0, 0);
        mode = g_FrontEndMode;
    }
    else
    {
        g_ReplayDifficulty = g_FrontEndSavedDifficulty;
    }
    g_FrontEndFlags &= ~FRONT_END_FLAG_DEMO_TRANSITION;

    if (mode == 0)
    {
        SetScreen(controller, FRONT_END_SCREEN_MAIN_MENU);
        g_FrontEndMode = 1;
        FrontEndUpdateMainMenu(controller);
    }
    else if (mode == 1)
    {
        if (g_ReplayDifficulty == 4)
        {
            if (controller->cursor.count == 0)
                controller->cursor.current = 1;
            else if (controller->cursor.count <= 1)
                controller->cursor.current = controller->cursor.count - 1;
            else
                controller->cursor.current = 1;
        }
        SetScreen(controller, FRONT_END_SCREEN_MAIN_MENU_RETURN);
        CreateVm(controller, 0x5b);
        controller->transitionOwner->flags004 |= FRONT_END_TRANSITION_ACTIVE;
        FrontEndUpdateMainMenuReturn(controller);
    }
    else if (mode == 2)
    {
        controller->cursor.count = 10;
        controller->cursor.current = 3;
        controller->cursor.Push();
        SetScreen(controller, FRONT_END_SCREEN_PRACTICE);
        CreateVm(controller, 0x5b);
        InterruptVm(controller, 0x5b, 9);
        controller->transitionOwner->flags004 |= FRONT_END_TRANSITION_ACTIVE;
        g_FrontEndMode = 1;
        FrontEndUpdatePractice(controller);
    }
    else
    {
        FrontEndUpdateMainMenu(controller);
    }
}

static void RotateDemoReplay(FrontEndControllerView *controller)
{
    int stage;
    ReplayManager *replay;

    ++g_FrontEndDemoFrames;
    if ((g_FrontEndInput.current & 0x160b) != 0)
    {
        g_FrontEndDemoFrames = 0;
        return;
    }
    if (g_FrontEndDemoFrames < 900)
        return;

    g_FrontEndFlags &= ~0x40u;
    g_FrontEndFlags |= FRONT_END_FLAG_DEMO_TRANSITION;
    strcpy(g_FrontEndDemoPath, g_FrontEndDemoPaths[g_FrontEndDemoIndex]);
    replay = ReplayManager::Load(g_FrontEndDemoPath);
    g_FrontEndDemoIndex = (g_FrontEndDemoIndex + 1) % 4;

    for (stage = 0; stage < 8; ++stage)
    {
        if (replay->stageStates[stage].header != NULL)
            break;
    }
    g_FrontEndSelectedStage = stage;
    g_FrontEndSelectedStageMirror = stage;
    g_FrontEndNextGameMode = 12;
    g_ReplayCharacter = replay->replayData->character;
    g_ReplayShotType = replay->replayData->shotType;
    g_FrontEndSavedDifficulty = g_ReplayDifficulty;
    g_FrontEndSelectedStageRecord = g_FrontEndStageRecords[stage];
    g_ReplayDifficulty = replay->replayData->difficulty;
    ReplayManager::Destroy(replay);

    g_FrontEndDemoFrames = 0;
    g_FrontEndMode = controller->screen != FRONT_END_SCREEN_MAIN_MENU;
}

} // namespace


// TH10_FRONTEND_FUNCTION: 0x0042CDF0 FrontEndControllerView::Update
int FrontEndControllerView::Update()
{
    if (screen == FRONT_END_SCREEN_MAIN_MENU ||
        screen == FRONT_END_SCREEN_MAIN_MENU_RETURN)
    {
        RotateDemoReplay(this);
    }

    switch (screen)
    {
    case FRONT_END_SCREEN_INITIALIZE:
        InitializeFrontEndScreen(this);
        break;
    case FRONT_END_SCREEN_MAIN_MENU:
        FrontEndUpdateMainMenu(this);
        break;
    case FRONT_END_SCREEN_MAIN_MENU_RETURN:
        FrontEndUpdateMainMenuReturn(this);
        break;
    case FRONT_END_SCREEN_START_GAME:
        g_FrontEndNextGameMode =
            ((g_FrontEndSupervisorFlags & 0x1000) == 0 ? 1 : 0) | 2;
        FrontEndBeginGame();
        break;
    case FRONT_END_SCREEN_OPTIONS:
        UpdateOptions(this);
        break;
    case FRONT_END_SCREEN_KEY_CONFIG:
        FrontEndUpdateKeyConfig(this);
        break;
    case FRONT_END_SCREEN_DIFFICULTY:
        FrontEndUpdateDifficulty(this);
        break;
    case FRONT_END_SCREEN_CHARACTER:
        FrontEndUpdateCharacter(this);
        break;
    case FRONT_END_SCREEN_SHOT_TYPE:
        FrontEndUpdateShotType(this);
        break;
    case FRONT_END_SCREEN_STAGE:
        FrontEndUpdateStage(this);
        break;
    case FRONT_END_SCREEN_START_GAME_10:
    case FRONT_END_SCREEN_START_GAME_13:
        FrontEndBeginGame();
        break;
    case FRONT_END_SCREEN_REPLAY:
        FrontEndUpdateReplay(this);
        break;
    case FRONT_END_SCREEN_PRACTICE:
        FrontEndUpdatePractice(this);
        break;
    case FRONT_END_SCREEN_MUSIC_ROOM:
        FrontEndUpdateMusicRoom(this);
        break;
    case FRONT_END_SCREEN_SPECIAL:
        FrontEndUpdateSpecial(this);
        break;
    case FRONT_END_SCREEN_RESULT:
        FrontEndUpdateResult(this);
        break;
    }

    stateTimer.Tick();
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x0042D920 FrontEndControllerView::UpdateOptions
int __stdcall FrontEndControllerView::UpdateOptions(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_OPTIONS_INITIALIZE:
        controller->cursor.count = 6;
        controller->cursor.current = 0;
        CreateVm(controller, FRONT_END_ROOT_VM);
        controller->RefreshOptionsDisplay();
        SetScreenState(controller, FRONT_END_OPTIONS_OPENING);
        // The target intentionally falls through into the opening wait.

    case FRONT_END_OPTIONS_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_OPTIONS_ACTIVE);
            InterruptVmNow(controller, FRONT_END_ROOT_VM, 3);
            SetColorModeSprites(controller);
            InterruptVm(
                controller, FRONT_END_ROOT_VM,
                static_cast<short>(controller->cursor.current + 0x11));
            return 1;
        }
        break;

    case FRONT_END_OPTIONS_ACTIVE:
        controller->cursor.previous = controller->cursor.current;
        if (InputRepeated(FRONT_END_INPUT_UP))
            controller->cursor.Move(-1);
        if (InputRepeated(FRONT_END_INPUT_DOWN))
            controller->cursor.Move(1);

        if (controller->cursor.previous != controller->cursor.current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            InterruptVmNow(controller, FRONT_END_ROOT_VM, 3);
            InterruptVm(
                controller, FRONT_END_ROOT_VM,
                static_cast<short>(controller->cursor.current + 7));
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            if (controller->cursor.current != 5)
            {
                FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
                controller->cursor.current = 5;
                InterruptVmNow(controller, FRONT_END_ROOT_VM, 3);
                InterruptVm(controller, FRONT_END_ROOT_VM, 12);
                return 1;
            }
            BeginOptionsClose(controller);
            return 1;
        }

        if (controller->cursor.current == 2 &&
            TimerReachedMultiple(&controller->stateTimer, 60))
        {
            FrontEndPlaySound(FRONT_END_SOUND_PREVIEW);
        }

        if (InputRepeated(FRONT_END_INPUT_LEFT))
        {
            switch (controller->cursor.current)
            {
            case 0:
                if (g_MainSupervisorView.colorMode == 0)
                    g_MainSupervisorView.colorMode = 2;
                else
                    --g_MainSupervisorView.colorMode;
                SetColorModeSprites(controller);
                FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                break;
            case 1:
                g_MainSupervisorView.bgmVolume -= 5;
                if (g_MainSupervisorView.bgmVolume < 0)
                    g_MainSupervisorView.bgmVolume = 0;
                controller->RefreshOptionsDisplay();
                break;
            case 2:
                g_MainSupervisorView.sfxVolume -= 5;
                if (g_MainSupervisorView.sfxVolume < 0)
                    g_MainSupervisorView.sfxVolume = 0;
                controller->RefreshOptionsDisplay();
                break;
            }
        }

        if (InputRepeated(FRONT_END_INPUT_RIGHT))
        {
            switch (controller->cursor.current)
            {
            case 0:
                if (g_MainSupervisorView.colorMode < 2)
                    ++g_MainSupervisorView.colorMode;
                else
                    g_MainSupervisorView.colorMode = 0;
                SetColorModeSprites(controller);
                FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                break;
            case 1:
                g_MainSupervisorView.bgmVolume += 5;
                if (g_MainSupervisorView.bgmVolume > 100)
                    g_MainSupervisorView.bgmVolume = 100;
                controller->RefreshOptionsDisplay();
                break;
            case 2:
                g_MainSupervisorView.sfxVolume += 5;
                if (g_MainSupervisorView.sfxVolume > 100)
                    g_MainSupervisorView.sfxVolume = 100;
                controller->RefreshOptionsDisplay();
                break;
            }
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            switch (controller->cursor.current)
            {
            case 3:
                BeginOptionsClose(controller);
                return 1;
            case 4:
                g_MainSupervisorView.bgmVolume = 100;
                g_MainSupervisorView.sfxVolume = 80;
                g_MainSupervisorView.colorMode = 0;
                controller->RefreshOptionsDisplay();
                SetColorModeSprites(controller);
                FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                return 1;
            case 5:
                BeginOptionsClose(controller);
                return 1;
            }
        }
        break;

    case FRONT_END_OPTIONS_UNUSED:
        break;

    case FRONT_END_OPTIONS_CLOSING:
        if (controller->stateTimer.current >= 10)
        {
            if (controller->cursor.current == 3)
            {
                SetScreen(controller, FRONT_END_SCREEN_KEY_CONFIG);
                controller->cursor.Push();
            }
            else if (controller->cursor.current == 5)
            {
                InterruptVmNow(controller, 0x5a, 2);
                InterruptVmNow(controller, 0x5b, 2);
                SetScreen(controller, FRONT_END_SCREEN_MAIN_MENU_RETURN);
                controller->cursor.Pop();
                return 1;
            }
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x0042E5A0 FrontEndControllerView::RefreshOptionsDisplay
void FrontEndControllerView::RefreshOptionsDisplay()
{
    float volume;

    g_FrontEndSoundBgmVolume = g_MainSupervisorView.bgmVolume;
    FrontEndQueueSoundCommand("SetVol", 8, 0);
    g_FrontEndSoundSfxVolume = g_MainSupervisorView.sfxVolume;

    if (g_FrontEndSoundSfxVolume == 0)
    {
        g_FrontEndSoundBgmAttenuation = -10000;
    }
    else
    {
        volume = static_cast<float>(g_FrontEndSoundBgmVolume) / 100.0f;
        volume = 1.0f - volume;
        volume *= volume;
        volume *= volume;
        volume = 1.0f - volume;
        g_FrontEndSoundBgmAttenuation =
            -5000 - static_cast<int>(-5000.0f * volume);
    }

    SetVolumeDigitSprites(this, g_MainSupervisorView.bgmVolume, 0x25, 0x29);
    SetVolumeDigitSprites(this, g_MainSupervisorView.sfxVolume, 0x2d, 0x31);
    SetVolumeLeadingDigitVisibility(
        this, g_MainSupervisorView.bgmVolume, 0x25, 0x29);
    SetVolumeLeadingDigitVisibility(
        this, g_MainSupervisorView.sfxVolume, 0x2d, 0x31);
}
