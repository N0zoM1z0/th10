#include "Main.hpp"
#include "FrontEnd.hpp"
#include "ReplayManager.hpp"

#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// TH10 0x0040AD20 is the shared cursor setter used by multiple FrontEnd
// controllers. The physical helper consumes value in ECX and cursor in EDX;
// the member API in FrontEnd.hpp delegates here without claiming that this
// private helper ABI was the original source declaration.
int __fastcall FrontEndCursorSetCurrent(
    int value, FrontEndCursorView *cursor)
{
    int count = cursor->count;
    if (count != 0)
    {
        if (value >= count)
        {
            --count;
            cursor->current = count;
            return count;
        }

        int chosen = value < 0 ? 0 : value;
        cursor->current = chosen;
        return chosen;
    }

    cursor->current = value;
    return value;
}

void FrontEndCursorView::Push()
{
    savedCurrent[saveDepth] = current;
    savedCount[saveDepth] = count;
    ++saveDepth;
    if (saveDepth >= 16)
        saveDepth = 15;
    disabledEntryCount = 0;
}

void FrontEndCursorView::Pop()
{
    --saveDepth;
    if (saveDepth < 0)
        saveDepth = 0;
    current = savedCurrent[saveDepth];
    count = savedCount[saveDepth];
    disabledEntryCount = 0;
}

int FrontEndCursorView::Move(int amount)
{
    if (count <= 0)
        return current;

tryAgain:
    current += amount;
    while (current >= count)
    {
        if (wraps != 0)
            current -= count;
        else
            current = count - 1;
    }
    while (current < 0)
    {
        if (wraps != 0)
            current += count;
        else
            current = 0;
    }
    for (int i = 0; i < disabledEntryCount; ++i)
    {
        if (disabledEntries[i] == current)
            goto tryAgain;
    }
    return current;
}


struct FrontEndInputView
{
    unsigned int current;
    unsigned short repeated;
    unsigned short pressed;

    int IsRepeated(unsigned short mask);
};

// TH10 0x0040ACE0. FrontEnd callers keep the shared input object in ECX while
// /GL promotes the 16-bit mask to EAX. The source method keeps the natural
// object API and lets the selected target caller graph recover that private ABI.
int FrontEndInputView::IsRepeated(unsigned short mask)
{
    if ((pressed & mask) == 0 && (repeated & mask) == 0)
        return 0;
    return 1;
}

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
extern int g_FrontEndStageCursor;
extern int g_FrontEndStageShortcut;
extern int g_FrontEndSavedDifficulty;
extern unsigned int g_FrontEndSupervisorFlags;
extern int g_FrontEndNextGameMode;
extern unsigned char *g_FrontEndProfileData;
extern const char *g_FrontEndStageNames[];
extern unsigned char g_FrontEndKeyboardState[256];
extern unsigned char g_FrontEndPreviousKeyboardState[256];
extern unsigned char g_FrontEndKeyboardTransitions[256];
extern int g_FrontEndPracticeSecretIdleFrames;
extern int g_FrontEndPracticeSecretProgress;
extern const unsigned int g_FrontEndPracticeSecretKeys[22];
extern const signed char g_FrontEndPracticeDifficulties[110];
extern const char g_FrontEndPracticeUnavailableFormat[];
extern const char *g_FrontEndScoreEntryAlphabet;
extern void *g_FrontEndScoreFormatTable;
extern unsigned char g_FrontEndScoreEntryFormatTable[];
extern unsigned char g_FrontEndDefaultScoreFormatTable[];
extern int g_FrontEndReplayCursor;
extern const char *g_ReplayCharacterNames[];
extern const char *g_ReplayDifficultyNames[];
extern const char *g_ReplayCompletionNames[];

extern int g_ReplayCurrentStage;
extern int g_ReplayCharacter;
extern int g_ReplayShotType;
extern int g_ReplayDifficulty;

extern int g_FrontEndSoundBgmVolume;
extern int g_FrontEndSoundSfxVolume;
extern int g_FrontEndSoundBgmAttenuation;
extern FrontEndControllerMappingView g_FrontEndControllerMapping;
extern FrontEndControllerMappingView g_FrontEndSavedControllerMapping;

extern void FrontEndQueueSoundCommand(
    const char *command, int parameter0, int parameter1);
extern void FrontEndPlaySound(int soundId);
extern void FrontEndLoadMusic(int slot, const char *path);
extern void FrontEndPlayMusic(int slot, int loopStart);
extern void FrontEndResetAsciiAuxiliary(int *state);
extern signed char *__fastcall FrontEndGetControllerState(int controllerIndex);
extern int FrontEndReadKeyboardState(unsigned char *keyboardState);

extern int FrontEndUpdateMainMenu(FrontEndControllerView *controller);
extern int FrontEndUpdateMainMenuReturn(FrontEndControllerView *controller);
extern int FrontEndBeginGame();
extern int FrontEndUpdateMusicRoom(FrontEndControllerView *controller);
extern int FrontEndUpdateResult(FrontEndControllerView *controller);
extern int FrontEndDrawResult(FrontEndControllerView *controller);
extern void FrontEndUnlockPracticeRecords();
extern void * __stdcall FrontEndBeginSelectionTransition(
    int type, int duration, int parameter2, int parameter3, int parameter4,
    int chainPriority);
extern int __stdcall FrontEndFinalizeGameSelection(float value);


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
    FRONT_END_SOUND_MOVE = 12,
    FRONT_END_SOUND_UNLOCK = 44,
    FRONT_END_PRACTICE_RECORD_COUNT = 110,
    FRONT_END_PRACTICE_ROWS_PER_PAGE = 10,
    FRONT_END_SCORE_ENTRY_ROWS = 10,
    FRONT_END_SCORE_ENTRY_NAME_LENGTH = 8,
    FRONT_END_SCORE_ENTRY_COLUMNS = 13
};

struct FrontEndStageScoreRecordView
{
    int score;
    unsigned char unknown004;
    unsigned char available;
    unsigned char unknown006[2];
};

typedef char FrontEndStageScoreRecordSizeIs8[
    (sizeof(FrontEndStageScoreRecordView) == 8) ? 1 : -1];

struct FrontEndScoreRecordView
{
    int score;
    signed char stageNameIndex;
    signed char scoreSuffix;
    char name[10];
    time_t timestamp;
    float slowdownRate;
};

typedef char FrontEndScoreRecordSizeIs18[
    (sizeof(FrontEndScoreRecordView) == 0x18) ? 1 : -1];

struct FrontEndScoreTableView
{
    unsigned char unknown000[0x10];
};

extern int FrontEndInsertScore(FrontEndScoreTableView *table);

enum FrontEndKeyConfigBindingView
{
    FRONT_END_KEY_CONFIG_SHOT = 0,
    FRONT_END_KEY_CONFIG_BOMB = 1,
    FRONT_END_KEY_CONFIG_FOCUS = 2,
    FRONT_END_KEY_CONFIG_MENU = 3,
    FRONT_END_KEY_CONFIG_SKIP = 4,
    FRONT_END_KEY_CONFIG_BINDING_COUNT = 5
};

enum
{
    FRONT_END_KEY_CONFIG_ROOT_VM = 2,
    FRONT_END_CONTROLLER_BUTTON_COUNT = 31
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
    controller->vmIds[index].SetInterrupt(interrupt);
}

static void InterruptVmId(int id, short interrupt)
{
    g_AnmRenderManagerView->SetVmPendingInterrupt(id, interrupt);
}

static void DeleteVm(
    FrontEndControllerView *controller, int index)
{
    AnmVmIdView *id = &controller->vmIds[index];
    if (g_AnmRenderManagerView->FindVm(id->value) != NULL)
        InterruptVmId(id->value, 1);
    id->value = 0;
}

static int InputRepeated(unsigned short mask)
{
    return g_FrontEndInput.IsRepeated(mask);
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
    if (parent == NULL)
        return 0;
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

static void InterruptChildVm(
    FrontEndControllerView *controller, int rootIndex,
    short scriptIndex, short interrupt)
{
    int id = FindChildVmId(&controller->vmIds[rootIndex], scriptIndex);
    InterruptVmId(id, interrupt);
}

static void DisableVmTree(int id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(id);
    if (vm == NULL)
        return;

    vm->drawEnabled = 0;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->drawEnabled = 0;
        node = node->next;
    }
}

static void DisableChildVmTree(
    FrontEndControllerView *controller, int rootIndex, short scriptIndex)
{
    DisableVmTree(
        FindChildVmId(&controller->vmIds[rootIndex], scriptIndex));
}

static void EnsureAsciiSelectionVm(float x, float y)
{
    if (g_AsciiManagerView->auxiliaryVm89A4 == 0)
    {
        AnmFloat3View position(x, y, 0.0f);
        g_AsciiManagerView->auxiliaryVm89A4 =
            g_AsciiManagerView->asciiAnm->CreateVmAtScreenVariant0(
                6, &position).value;
    }
}

static FrontEndStageScoreRecordView *GetFrontEndStageScoreRecord(int stage)
{
    int shotGroup = g_ReplayShotType + g_ReplayCharacter * 3;
    FrontEndStageScoreRecordView *records =
        reinterpret_cast<FrontEndStageScoreRecordView *>(
            g_FrontEndProfileData + shotGroup * 0x437c + 0x4dc);
    return &records[stage + g_ReplayDifficulty * 6];
}

static int CountPracticeRecordsForDifficulty(int difficulty)
{
    int count = 0;
    for (int record = 0; record < FRONT_END_PRACTICE_RECORD_COUNT; ++record)
    {
        if (g_FrontEndPracticeDifficulties[record] == difficulty)
            ++count;
    }
    return count;
}

static unsigned char *GetPracticeCatalogRecord(int record)
{
    return g_FrontEndProfileData + 0x19a8c + record * 0x90;
}

static int *GetPracticeResultRecord(int shot, int record)
{
    return reinterpret_cast<int *>(
        g_FrontEndProfileData + shot * 0x437c + 0x624 + record * 0x90);
}

static FrontEndScoreRecordView *GetPracticeScoreRecord(
    int shot, int difficulty, int row)
{
    return reinterpret_cast<FrontEndScoreRecordView *>(
        g_FrontEndProfileData + shot * 0x437c + 0x18 +
        (difficulty * 10 + row) * 0x18);
}

static FrontEndScoreTableView *GetFrontEndScoreTable(int shot)
{
    return reinterpret_cast<FrontEndScoreTableView *>(
        g_FrontEndProfileData + shot * 0x437c + 8);
}

static void InterruptPracticeRows(FrontEndControllerView *controller)
{
    for (int row = 0; row < FRONT_END_PRACTICE_ROWS_PER_PAGE; ++row)
        InterruptVmId(controller->practiceRowVmIds[row].value, 1);
}

static void UpdatePracticeSecretSequence()
{
    memcpy(
        g_FrontEndPreviousKeyboardState, g_FrontEndKeyboardState,
        sizeof(g_FrontEndPreviousKeyboardState));
    if (FrontEndReadKeyboardState(g_FrontEndKeyboardState) == 0)
        goto tickTimeout;

    for (int key = 0; key < 256; ++key)
    {
        g_FrontEndKeyboardTransitions[key] =
            (g_FrontEndPreviousKeyboardState[key] ^
             g_FrontEndKeyboardState[key]) &
            g_FrontEndKeyboardState[key];
    }

    if (g_FrontEndPracticeSecretProgress >= 22)
    {
        FrontEndUnlockPracticeRecords();
        FrontEndPlaySound(FRONT_END_SOUND_UNLOCK);
        g_FrontEndPracticeSecretProgress = 0;
    }
    else if ((g_FrontEndKeyboardTransitions[
                  g_FrontEndPracticeSecretKeys[
                      g_FrontEndPracticeSecretProgress]] & 0x80) != 0)
    {
        ++g_FrontEndPracticeSecretProgress;
        g_FrontEndPracticeSecretIdleFrames = 0;
        goto tickTimeout;
    }
    else
    {
        for (int key = 0; key < 57; ++key)
        {
            if ((g_FrontEndKeyboardTransitions[key] & 0x80) != 0)
            {
                g_FrontEndPracticeSecretProgress = 0;
                break;
            }
        }
    }

tickTimeout:
    ++g_FrontEndPracticeSecretIdleFrames;
    if (g_FrontEndPracticeSecretIdleFrames > 300)
    {
        g_FrontEndPracticeSecretProgress = 0;
        g_FrontEndPracticeSecretIdleFrames = 0;
    }
}

static int ReadFrontEndStageShortcut()
{
    int directInput = FrontEndReadKeyboardState(g_FrontEndKeyboardState);
    int firstKey = directInput != 0 ? 2 : '1';
    int shortcut;

    for (shortcut = 1; shortcut <= 9; ++shortcut)
    {
        if ((g_FrontEndKeyboardState[firstKey + shortcut - 1] & 0x80) != 0)
            return shortcut;
    }
    return 0;
}

static void DrawReplaySummary(
    AsciiManagerView *ascii, AnmFloat3View *position,
    ReplayManager *replay, int replayNumber)
{
    ReplayDataHeader *data = replay->replayData;
    tm *date = localtime(reinterpret_cast<time_t *>(&data->timestamp));
    ascii->AddFormatText(
        position,
        "No.%.2d %s %.2d/%.2d/%.2d %.2d:%.2d %s %s %s %2.1f%%",
        replayNumber, data->replayName, date->tm_year % 100,
        date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min,
        g_ReplayCharacterNames[data->character * 3 + data->shotType],
        g_ReplayDifficultyNames[data->difficulty],
        g_ReplayCompletionNames[data->completionState], data->slowRate);
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

static void CopyKeyConfigBindingsFromGlobal(
    FrontEndControllerView *controller)
{
    controller->keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT] =
        g_FrontEndControllerMapping.shotButton;
    controller->keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB] =
        g_FrontEndControllerMapping.bombButton;
    controller->keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS] =
        g_FrontEndControllerMapping.focusButton;
    controller->keyConfigBindings[FRONT_END_KEY_CONFIG_MENU] =
        g_FrontEndControllerMapping.menuButton;
    controller->keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP] =
        g_FrontEndControllerMapping.skipButton;
}

static void CommitKeyConfigBindings(FrontEndControllerView *controller)
{
    g_FrontEndControllerMapping.shotButton =
        controller->keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT];
    g_FrontEndControllerMapping.bombButton =
        controller->keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB];
    g_FrontEndControllerMapping.focusButton =
        controller->keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS];
    g_FrontEndControllerMapping.menuButton =
        controller->keyConfigBindings[FRONT_END_KEY_CONFIG_MENU];
    g_FrontEndControllerMapping.skipButton =
        controller->keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP];

    g_FrontEndSavedControllerMapping = g_FrontEndControllerMapping;
}

static void SetKeyBindingDigitSprites(
    FrontEndControllerView *controller, short binding,
    short normalFirst, short selectedFirst)
{
    SetChildSprite(controller, normalFirst + 0, binding / 10 + 0x33);
    SetChildSprite(controller, normalFirst + 1, binding % 10 + 0x33);
    SetChildSprite(controller, selectedFirst + 0, binding / 10 + 0x33);
    SetChildSprite(controller, selectedFirst + 1, binding % 10 + 0x33);
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

    int *asciiAuxiliary = &g_AsciiManagerView->auxiliaryVm89A4;
    FrontEndResetAsciiAuxiliary(asciiAuxiliary);
    *asciiAuxiliary = 0;

    if (mode == 3)
    {
        controller->cursor.count = 10;
        controller->cursor.current = 0;
        controller->cursor.Push();
        SetScreen(controller, FRONT_END_SCREEN_SCORE_ENTRY);
        CreateVm(controller, 0x5b);
        InterruptVm(controller, 0x5b, 9);
        controller->transitionOwner->flags004 |= FRONT_END_TRANSITION_ACTIVE;
        g_FrontEndMode = 1;
        FrontEndControllerView::UpdateScoreEntry(controller);
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
        FrontEndControllerView::UpdatePractice(controller);
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
        UpdateKeyConfig(this);
        break;
    case FRONT_END_SCREEN_DIFFICULTY:
        UpdateDifficulty(this);
        break;
    case FRONT_END_SCREEN_CHARACTER:
        UpdateCharacter(this);
        break;
    case FRONT_END_SCREEN_SHOT_TYPE:
        UpdateShotType(this);
        break;
    case FRONT_END_SCREEN_STAGE:
        UpdateStage(this);
        break;
    case FRONT_END_SCREEN_START_GAME_10:
    case FRONT_END_SCREEN_START_GAME_13:
        FrontEndBeginGame();
        break;
    case FRONT_END_SCREEN_REPLAY:
        UpdateReplay(this);
        break;
    case FRONT_END_SCREEN_PRACTICE:
        UpdatePractice(this);
        break;
    case FRONT_END_SCREEN_MUSIC_ROOM:
        FrontEndUpdateMusicRoom(this);
        break;
    case FRONT_END_SCREEN_SCORE_ENTRY:
        UpdateScoreEntry(this);
        break;
    case FRONT_END_SCREEN_RESULT:
        FrontEndUpdateResult(this);
        break;
    }

    stateTimer.Tick();
    return 1;
}

// TH10_FRONTEND_FUNCTION: 0x0042D2E0 FrontEndUpdateCallback
int __fastcall FrontEndUpdateCallback(FrontEndControllerView *controller)
{
    return controller->Update();
}


// TH10_FRONTEND_FUNCTION: 0x0042D260 FrontEndControllerView::Draw
int FrontEndControllerView::Draw()
{
    switch (screen)
    {
    case FRONT_END_SCREEN_REPLAY:
        DrawReplay(this);
        break;
    case FRONT_END_SCREEN_PRACTICE:
        DrawPractice();
        break;
    case FRONT_END_SCREEN_SCORE_ENTRY:
        DrawScoreEntry(this);
        break;
    case FRONT_END_SCREEN_RESULT:
        FrontEndDrawResult(this);
        break;
    case FRONT_END_SCREEN_STAGE:
        DrawStageScores();
        break;
    }
    return 1;
}

// TH10_FRONTEND_FUNCTION: 0x0042D2F0 FrontEndDrawCallback
int __fastcall FrontEndDrawCallback(FrontEndControllerView *controller)
{
    return controller->Draw();
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


// TH10_FRONTEND_FUNCTION: 0x0042F540 FrontEndControllerView::UpdateKeyConfig
int __stdcall FrontEndControllerView::UpdateKeyConfig(
    FrontEndControllerView *controller)
{
    int controllerButton;
    signed char *controllerState;

    switch (controller->screenState)
    {
    case FRONT_END_KEY_CONFIG_INITIALIZE:
        controller->cursor.count = 7;
        controller->cursor.SetCurrent(0);
        CreateVm(controller, FRONT_END_KEY_CONFIG_ROOT_VM);
        SetScreenState(controller, FRONT_END_KEY_CONFIG_OPENING);
        CopyKeyConfigBindingsFromGlobal(controller);
        controller->RefreshKeyConfigDisplay();
        // The target intentionally falls through into the opening wait.

    case FRONT_END_KEY_CONFIG_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_KEY_CONFIG_ACTIVE);
            InterruptVmNow(controller, FRONT_END_KEY_CONFIG_ROOT_VM, 3);
            InterruptVm(
                controller, FRONT_END_KEY_CONFIG_ROOT_VM,
                static_cast<short>(controller->cursor.current + 0x11));
            return 1;
        }
        break;

    case FRONT_END_KEY_CONFIG_ACTIVE:
        controller->cursor.previous = controller->cursor.current;
        if (InputRepeated(FRONT_END_INPUT_UP))
            controller->cursor.Move(-1);
        if (InputRepeated(FRONT_END_INPUT_DOWN))
            controller->cursor.Move(1);

        if (controller->cursor.previous != controller->cursor.current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            InterruptVmNow(controller, FRONT_END_KEY_CONFIG_ROOT_VM, 3);
            InterruptVm(
                controller, FRONT_END_KEY_CONFIG_ROOT_VM,
                static_cast<short>(controller->cursor.current + 7));
        }

        controllerState = FrontEndGetControllerState(0);
        for (controllerButton = 0;
             controllerButton < FRONT_END_CONTROLLER_BUTTON_COUNT;
             ++controllerButton)
        {
            if (controllerState[controllerButton] < 0)
            {
                if (controller->cursor.current <
                    FRONT_END_KEY_CONFIG_BINDING_COUNT)
                {
                    controller->AssignKeyConfigBinding(
                        controller->cursor.current, controllerButton);
                }
                break;
            }
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0 &&
            controller->cursor.current == 6)
        {
            CopyKeyConfigBindingsFromGlobal(controller);
            controller->RefreshKeyConfigDisplay();
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            InterruptVm(controller, FRONT_END_KEY_CONFIG_ROOT_VM, 6);
            SetScreenState(controller, FRONT_END_KEY_CONFIG_CLOSING);
            return 1;
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            if (controller->cursor.current == 5)
            {
                CopyKeyConfigBindingsFromGlobal(controller);
                controller->RefreshKeyConfigDisplay();
                FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                return 1;
            }
            if (controller->cursor.current == 6)
            {
                CommitKeyConfigBindings(controller);
                FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
                InterruptVm(controller, FRONT_END_KEY_CONFIG_ROOT_VM, 6);
                SetScreenState(controller, FRONT_END_KEY_CONFIG_CLOSING);
                return 1;
            }
        }
        break;

    case FRONT_END_KEY_CONFIG_UNUSED:
        break;

    case FRONT_END_KEY_CONFIG_CLOSING:
        if (controller->stateTimer.current >= 10)
        {
            SetScreen(controller, FRONT_END_SCREEN_OPTIONS);
            controller->cursor.Pop();
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x0042F8B0 FrontEndControllerView::RefreshKeyConfigDisplay
void FrontEndControllerView::RefreshKeyConfigDisplay()
{
    SetKeyBindingDigitSprites(
        this, keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT], 0x43, 0x4d);
    SetKeyBindingDigitSprites(
        this, keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB], 0x45, 0x4f);
    SetKeyBindingDigitSprites(
        this, keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS], 0x47, 0x51);
    SetKeyBindingDigitSprites(
        this, keyConfigBindings[FRONT_END_KEY_CONFIG_MENU], 0x49, 0x53);
    SetKeyBindingDigitSprites(
        this, keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP], 0x4b, 0x55);
}


// TH10_FRONTEND_FUNCTION: 0x00430250 FrontEndControllerView::AssignKeyConfigBinding
void FrontEndControllerView::AssignKeyConfigBinding(
    int bindingIndex, int controllerButton)
{
    short previousBinding = keyConfigBindings[bindingIndex];
    if (previousBinding == controllerButton)
        return;

    if (bindingIndex != FRONT_END_KEY_CONFIG_SHOT &&
        keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT] == controllerButton)
    {
        keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT] = previousBinding;
    }
    if (bindingIndex != FRONT_END_KEY_CONFIG_BOMB &&
        keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB] == controllerButton)
    {
        keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB] = previousBinding;
    }
    if (bindingIndex != FRONT_END_KEY_CONFIG_FOCUS &&
        keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS] == controllerButton)
    {
        keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS] = previousBinding;
    }
    if (bindingIndex != FRONT_END_KEY_CONFIG_MENU &&
        keyConfigBindings[FRONT_END_KEY_CONFIG_MENU] == controllerButton)
    {
        keyConfigBindings[FRONT_END_KEY_CONFIG_MENU] = previousBinding;
    }
    if (bindingIndex != FRONT_END_KEY_CONFIG_SKIP &&
        keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP] == controllerButton)
    {
        keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP] = previousBinding;
    }

    keyConfigBindings[bindingIndex] = static_cast<short>(controllerButton);
    RefreshKeyConfigDisplay();
    FrontEndPlaySound(FRONT_END_SOUND_SELECT);
}


// TH10_FRONTEND_FUNCTION: 0x00430320 FrontEndControllerView::UpdateDifficulty
int __stdcall FrontEndControllerView::UpdateDifficulty(
    FrontEndControllerView *controller)
{
    int rootIndex = 0x77 + (g_ReplayDifficulty >= 4);

    switch (controller->screenState)
    {
    case FRONT_END_SELECTION_INITIALIZE:
        if (ResolveVm(&controller->vmIds[0x5e]) == NULL)
        {
            CreateVm(controller, 0x5e);
            controller->difficultyAuxVmId =
                g_AsciiManagerView->asciiAnm->CreateVmVariant0(
                    8, FRONT_END_RENDER_LAYER);
        }

        controller->cursor.count = g_ReplayDifficulty < 4 ? 4 : 1;
        DeleteVm(controller, 0x62);
        CreateVm(controller, rootIndex);
        InterruptVmNow(controller, rootIndex, 3);
        InterruptVm(
            controller, rootIndex,
            static_cast<short>(controller->cursor.current + 0x11));
        CreateVm(controller, 0x62);
        SetScreenState(controller, FRONT_END_SELECTION_OPENING);
        break;

    case FRONT_END_SELECTION_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_SELECTION_ACTIVE);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_ACTIVE:
        if (g_ReplayDifficulty < 4)
        {
            controller->cursor.previous = controller->cursor.current;
            if (InputRepeated(FRONT_END_INPUT_UP))
                controller->cursor.Move(-1);
            if (InputRepeated(FRONT_END_INPUT_DOWN))
                controller->cursor.Move(1);

            if (controller->cursor.previous != controller->cursor.current)
            {
                FrontEndPlaySound(FRONT_END_SOUND_MOVE);
                InterruptVmNow(controller, rootIndex, 3);
                InterruptVm(
                    controller, rootIndex,
                    static_cast<short>(controller->cursor.current + 7));
            }
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            SetScreenState(controller, FRONT_END_SELECTION_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            DeleteVm(controller, rootIndex);
            return 1;
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            short childScript = g_ReplayDifficulty < 4
                ? static_cast<short>(controller->cursor.current + 0x6d)
                : static_cast<short>(0x71);
            InterruptVm(controller, rootIndex, 6);
            InterruptChildVm(controller, rootIndex, childScript, 2);
            SetScreenState(controller, FRONT_END_SELECTION_CONFIRMED);
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_CONFIRMED:
        if (controller->stateTimer.current >= 14)
        {
            DeleteVm(controller, 0x62);
            SetScreen(controller, FRONT_END_SCREEN_CHARACTER);
            if (g_ReplayDifficulty < 4)
                g_ReplayDifficulty = controller->cursor.current;
            controller->cursor.Push();
            controller->cursor.count = 2;
            controller->cursor.SetCurrent(g_ReplayCharacter);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x62);
            InterruptVm(controller, 0x5a, 8);
            InterruptVm(controller, 0x5b, 8);
            DeleteVm(controller, 0x5e);
            InterruptVmId(controller->difficultyAuxVmId.value, 1);
            SetScreen(controller, FRONT_END_SCREEN_MAIN_MENU_RETURN);
            if (g_ReplayDifficulty < 4)
                g_ReplayDifficulty = controller->cursor.current;
            else
                g_ReplayDifficulty = controller->savedDifficulty;
            controller->cursor.Pop();
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x004306A0 FrontEndControllerView::UpdateCharacter
int __stdcall FrontEndControllerView::UpdateCharacter(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_SELECTION_INITIALIZE:
        controller->cursor.count = 2;
        if (g_ReplayDifficulty == 4)
        {
            if (g_FrontEndProfileData[0x1d888] == 0 &&
                g_FrontEndProfileData[0x1d889] == 0 &&
                g_FrontEndProfileData[0x1d88a] == 0)
            {
                if (controller->cursor.current == 0)
                    controller->cursor.SetCurrent(1);
                controller->cursor.DisableEntry(0);
            }
            if (g_FrontEndProfileData[0x1d88b] == 0 &&
                g_FrontEndProfileData[0x1d88c] == 0 &&
                g_FrontEndProfileData[0x1d88d] == 0)
            {
                if (controller->cursor.current == 1)
                    controller->cursor.SetCurrent(0);
                controller->cursor.DisableEntry(1);
            }
        }

        CreateVm(controller, 0x63);
        DeleteVm(controller, 0x7d);
        CreateVm(controller, 0x7d);
        InterruptVmNow(controller, 0x7d, 3);
        InterruptVm(
            controller, 0x7d,
            static_cast<short>(controller->cursor.current + 0x11));
        SetScreenState(controller, FRONT_END_SELECTION_OPENING);
        break;

    case FRONT_END_SELECTION_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_SELECTION_ACTIVE);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_ACTIVE:
        controller->cursor.previous = controller->cursor.current;
        if (InputRepeated(FRONT_END_INPUT_LEFT))
            controller->cursor.Move(-1);
        if (InputRepeated(FRONT_END_INPUT_RIGHT))
            controller->cursor.Move(1);

        if (controller->cursor.previous != controller->cursor.current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            InterruptVmNow(controller, 0x7d, 3);
            InterruptVm(
                controller, 0x7d,
                static_cast<short>(controller->cursor.current + 7));
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            SetScreenState(controller, FRONT_END_SELECTION_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            return 1;
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            int current = controller->cursor.current;
            InterruptChildVm(
                controller, 0x7d,
                static_cast<short>(current + 0x79), 6);
            InterruptChildVm(
                controller, 0x7d,
                static_cast<short>(current + 0x7b), 6);
            InterruptChildVm(
                controller, 0x7d,
                static_cast<short>(0x7a - current), 1);
            InterruptChildVm(
                controller, 0x7d,
                static_cast<short>(0x7c - current), 1);
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);
            SetScreenState(controller, FRONT_END_SELECTION_CONFIRMED);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_CONFIRMED:
        if (controller->stateTimer.current >= 14)
        {
            int previousCharacter = g_ReplayCharacter;
            DeleteVm(controller, 0x63);
            SetScreen(controller, FRONT_END_SCREEN_SHOT_TYPE);
            g_ReplayCharacter = controller->cursor.current;
            controller->cursor.Push();
            controller->cursor.count = 3;
            if (previousCharacter == g_ReplayCharacter)
                controller->cursor.SetCurrent(g_ReplayShotType);
            else
                controller->cursor.SetCurrent(0);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x7d);
            DeleteVm(controller, 0x63);
            SetScreen(controller, FRONT_END_SCREEN_DIFFICULTY);
            g_ReplayCharacter = controller->cursor.current;
            controller->cursor.Pop();
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00430A60 FrontEndControllerView::UpdateShotType
int __stdcall FrontEndControllerView::UpdateShotType(
    FrontEndControllerView *controller)
{
    int rootIndex = g_ReplayCharacter + 0x96;

    switch (controller->screenState)
    {
    case FRONT_END_SELECTION_INITIALIZE:
        controller->cursor.count = 3;
        if (g_ReplayDifficulty == 4)
        {
            unsigned char *unlocks =
                g_FrontEndProfileData + 0x1d888 + g_ReplayCharacter * 3;
            if (unlocks[0] == 0)
            {
                if (controller->cursor.current == 0)
                    controller->cursor.SetCurrent(unlocks[1] == 0 ? 2 : 1);
                controller->cursor.DisableEntry(0);
            }
            if (unlocks[1] == 0)
            {
                if (controller->cursor.current == 1)
                    controller->cursor.SetCurrent(unlocks[0] == 0 ? 2 : 0);
                controller->cursor.DisableEntry(1);
            }
            if (unlocks[2] == 0)
            {
                if (controller->cursor.current == 2)
                    controller->cursor.SetCurrent(unlocks[0] == 0 ? 1 : 0);
                controller->cursor.DisableEntry(2);
            }
        }

        CreateVm(controller, 0x64);
        DeleteVm(controller, rootIndex);
        CreateVm(controller, rootIndex);
        InterruptVmNow(controller, rootIndex, 3);
        InterruptVm(
            controller, rootIndex,
            static_cast<short>(controller->cursor.current + 0x11));
        SetScreenState(controller, FRONT_END_SELECTION_OPENING);

        {
            int recordIndex = g_ReplayCharacter * 0x329d + g_ReplayDifficulty;
            int firstRecord = *reinterpret_cast<int *>(
                g_FrontEndProfileData + 0x4d0 + recordIndex * 4);
            int secondRecord = *reinterpret_cast<int *>(
                g_FrontEndProfileData + 0x484c + recordIndex * 4);
            int thirdRecord = *reinterpret_cast<int *>(
                g_FrontEndProfileData + 0x8bc8 + recordIndex * 4);
            short firstScript = static_cast<short>(
                g_ReplayCharacter * 3 + 0x8a);

            if (firstRecord == 0)
                DisableChildVmTree(controller, rootIndex, firstScript);
            if (secondRecord == 0)
                DisableChildVmTree(controller, rootIndex, firstScript + 1);
            if (thirdRecord == 0)
                DisableChildVmTree(controller, rootIndex, firstScript + 2);
        }
        break;

    case FRONT_END_SELECTION_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_SELECTION_ACTIVE);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_ACTIVE:
        controller->cursor.previous = controller->cursor.current;
        if (InputRepeated(FRONT_END_INPUT_UP))
            controller->cursor.Move(-1);
        if (InputRepeated(FRONT_END_INPUT_DOWN))
            controller->cursor.Move(1);

        if (controller->cursor.previous != controller->cursor.current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            InterruptVmNow(controller, rootIndex, 3);
            InterruptVm(
                controller, rootIndex,
                static_cast<short>(controller->cursor.current + 7));
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            SetScreenState(controller, FRONT_END_SELECTION_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            return 1;
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            short scriptIndex = static_cast<short>(
                g_ReplayCharacter * 3 + 0x3f +
                controller->cursor.current);
            SetScreenState(controller, FRONT_END_SELECTION_CONFIRMED);
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);
            InterruptChildVm(controller, rootIndex, scriptIndex, 6);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_CONFIRMED:
        if (controller->stateTimer.current == 10)
        {
            if ((g_FrontEndSupervisorFlags & 0x10) == 0)
            {
                EnsureAsciiSelectionVm(480.0f, 392.0f);
                FrontEndBeginSelectionTransition(5, 0x20, 0, 0, 0, 0x2b);
            }
            else
            {
                g_ReplayShotType = controller->cursor.current;
                controller->cursor.Push();
                DeleteVm(controller, 0x64);
                SetScreen(controller, FRONT_END_SCREEN_STAGE);
            }
        }

        if (controller->stateTimer.current >= 40)
        {
            g_ReplayShotType = controller->cursor.current;
            controller->cursor.Push();
            if ((g_FrontEndSupervisorFlags & 0x10) != 0)
            {
                DeleteVm(controller, 0x64);
                SetScreen(controller, FRONT_END_SCREEN_STAGE);
                return 1;
            }

            SetScreen(controller, FRONT_END_SCREEN_START_GAME);
            if (g_ReplayDifficulty < 4)
            {
                g_FrontEndSelectedStageRecord = g_FrontEndStageRecords[0];
                g_FrontEndSelectedStage = 1;
                g_FrontEndSelectedStageMirror = 1;
            }
            else
            {
                g_FrontEndSelectedStageRecord = g_FrontEndStageRecords[6];
                g_FrontEndSelectedStage = 7;
                g_FrontEndSelectedStageMirror = 7;
            }
            g_FrontEndNextGameMode = 7;
            FrontEndFinalizeGameSelection(6.0f);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, rootIndex);
            DeleteVm(controller, 0x64);
            SetScreen(controller, FRONT_END_SCREEN_CHARACTER);
            controller->cursor.Pop();
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00430FF0 FrontEndControllerView::UpdateStage
int __stdcall FrontEndControllerView::UpdateStage(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_SELECTION_INITIALIZE:
        controller->cursor.count = 6;
        controller->cursor.SetCurrent(g_FrontEndStageCursor);
        CreateVm(controller, 0x6a);
        CreateVm(controller, 0x6b + g_ReplayCharacter);
        SetScreenState(controller, FRONT_END_SELECTION_OPENING);
        break;

    case FRONT_END_SELECTION_OPENING:
        if (controller->stateTimer.current > 10)
        {
            SetScreenState(controller, FRONT_END_SELECTION_ACTIVE);
            return 1;
        }
        break;

    case FRONT_END_SELECTION_ACTIVE:
        controller->cursor.previous = controller->cursor.current;
        if (InputRepeated(FRONT_END_INPUT_UP))
            controller->cursor.Move(-1);
        if (InputRepeated(FRONT_END_INPUT_DOWN))
            controller->cursor.Move(1);

        if (controller->cursor.previous != controller->cursor.current)
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            SetScreenState(controller, FRONT_END_SELECTION_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            g_FrontEndStageCursor = controller->cursor.current;
            return 1;
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            FrontEndStageScoreRecordView *record =
                GetFrontEndStageScoreRecord(controller->cursor.current + 1);
            if (record->available != 0)
                SetScreenState(controller, FRONT_END_SELECTION_CONFIRMED);

            FrontEndPlaySound(
                record->available != 0 ? FRONT_END_SOUND_SELECT : 0x25);
            g_FrontEndStageCursor = controller->cursor.current;
            g_FrontEndStageShortcut = ReadFrontEndStageShortcut();
        }
        break;

    case FRONT_END_SELECTION_CONFIRMED:
        if (controller->stateTimer.current == 10)
        {
            EnsureAsciiSelectionVm(480.0f, 392.0f);
            FrontEndBeginSelectionTransition(5, 0x20, 0, 0, 0, 0x2b);
        }

        if (controller->stateTimer.current >= 40)
        {
            controller->cursor.Push();
            SetScreen(controller, FRONT_END_SCREEN_START_GAME);
            g_FrontEndSelectedStage = controller->cursor.current + 1;
            g_FrontEndSelectedStageRecord =
                g_FrontEndStageRecords[controller->cursor.current];
            g_FrontEndSelectedStageMirror = g_FrontEndSelectedStage;
            FrontEndFinalizeGameSelection(6.0f);
            g_FrontEndNextGameMode = 7;
            return 1;
        }
        break;

    case FRONT_END_SELECTION_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x6a);
            DeleteVm(controller, 0x6b + g_ReplayCharacter);
            SetScreen(controller, FRONT_END_SCREEN_SHOT_TYPE);
            controller->cursor.Pop();
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00431410 FrontEndControllerView::DrawStageScores
int FrontEndControllerView::DrawStageScores()
{
    AsciiManagerView *ascii = g_AsciiManagerView;

    if (screenState >= FRONT_END_SELECTION_ACTIVE &&
        screenState <= FRONT_END_SELECTION_CONFIRMED)
    {
        ascii->drawShadow = 1;
        AnmFloat3View position(80.0f, 80.0f, 0.0f);

        if (stateTimer.current >= 10 ||
            screenState == FRONT_END_SELECTION_CONFIRMED)
        {
            position.x = g_ReplayCharacter == 0 ? 168.0f : 296.0f;
            position.y = 216.0f;

            for (int stage = 1; stage <= 6; ++stage)
            {
                FrontEndStageScoreRecordView *record =
                    GetFrontEndStageScoreRecord(stage);
                if (cursor.current == stage - 1)
                {
                    if (record->available == 0)
                    {
                        ascii->color = 0xffdfdfdf;
                    }
                    else if (screenState == FRONT_END_SELECTION_CONFIRMED &&
                             stateTimer.current % 4 >= 2)
                    {
                        ascii->color = 0xff000000;
                    }
                    else
                    {
                        ascii->color = 0xffffff00;
                    }
                }
                else
                {
                    ascii->color = 0xff808080;
                }

                if (record->available == 0)
                {
                    ascii->AddFormatText(
                        &position, "%s  ---------",
                        g_FrontEndStageNames[stage]);
                }
                else
                {
                    ascii->AddFormatText(
                        &position, "%s  %.8d0",
                        g_FrontEndStageNames[stage], record->score);
                }
                position.y += 18.0f;
            }
        }

        ascii->color = 0xffffffff;
        ascii->drawShadow = 0;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x004315C0 FrontEndControllerView::UpdateReplay
int __stdcall FrontEndControllerView::UpdateReplay(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_REPLAY_INITIALIZE:
    {
        controller->cursor.count = 25;
        controller->cursor.SetCurrent(g_FrontEndReplayCursor);
        g_FrontEndReplayCursor = 0;

        if (ResolveVm(&controller->vmIds[0x5e]) == NULL)
        {
            CreateVm(controller, 0x5e);
            controller->difficultyAuxVmId =
                g_AsciiManagerView->asciiAnm->CreateVmVariant0(
                    8, FRONT_END_RENDER_LAYER);
        }
        CreateVm(controller, 0x65);
        SetScreenState(controller, FRONT_END_REPLAY_OPENING);

        memset(controller->replayFiles, 0, sizeof(controller->replayFiles));
        for (int replayNumber = 1; replayNumber <= 25; ++replayNumber)
        {
            char replayPath[64];
            sprintf(replayPath, "th10_%.2d.rpy", replayNumber);
            controller->replayFiles[replayNumber - 1] =
                ReplayManager::Load(replayPath);
        }

        _mkdir("replay");
        _chdir("replay");

        WIN32_FIND_DATAA findData;
        HANDLE search = FindFirstFileA("th10_ud????.rpy", &findData);
        if (search != INVALID_HANDLE_VALUE)
        {
            int replayIndex = 25;
            do
            {
                _chdir("../");
                controller->replayFiles[replayIndex] =
                    ReplayManager::Load(findData.cFileName);
                _chdir("replay");
                ++replayIndex;
            }
            while (replayIndex < 50 && FindNextFileA(search, &findData));
        }

        // The original calls FindClose even when FindFirstFileA returned the
        // invalid sentinel. Preserve that observable error-path behavior.
        FindClose(search);
        _chdir("../");
        controller->replayListOffset = 0;
        break;
    }

    case FRONT_END_REPLAY_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_REPLAY_SELECT_FILE);
            return 1;
        }
        break;

    case FRONT_END_REPLAY_SELECT_FILE:
        controller->cursor.previous = controller->cursor.current;
        if (InputRepeated(FRONT_END_INPUT_UP))
            controller->cursor.Move(-1);
        if (InputRepeated(FRONT_END_INPUT_DOWN))
            controller->cursor.Move(1);

        if (controller->cursor.previous != controller->cursor.current)
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            SetScreenState(controller, FRONT_END_REPLAY_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            return 1;
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0 &&
            controller->replayFiles[controller->cursor.current] != NULL)
        {
            SetScreenState(controller, FRONT_END_REPLAY_SELECT_STAGE);
            controller->selectedReplay = controller->cursor.current;
            controller->cursor.Push();
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);

            controller->cursor.count = 7;
            controller->cursor.SetCurrent(0);
            ReplayManager *replay =
                controller->replayFiles[controller->selectedReplay];
            for (int stage = 1; stage <= 7; ++stage)
            {
                if (replay->stageStates[stage].header == NULL)
                    controller->cursor.DisableEntry(stage - 1);
            }
            controller->cursor.Move(-1);
            controller->cursor.Move(1);
        }
        break;

    case FRONT_END_REPLAY_SELECT_STAGE:
        if (controller->stateTimer.current < 15)
            break;

        controller->cursor.previous = controller->cursor.current;
        if (InputRepeated(FRONT_END_INPUT_UP))
            controller->cursor.Move(-1);
        if (InputRepeated(FRONT_END_INPUT_DOWN))
            controller->cursor.Move(1);

        if (controller->cursor.previous != controller->cursor.current)
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            controller->cursor.Pop();
            controller->cursor.count = 25;
            controller->cursor.disabledEntryCount = 0;
            SetScreenState(controller, FRONT_END_REPLAY_SELECT_FILE);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            return 1;
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            controller->selectedReplayStage = controller->cursor.current;
            SetScreenState(controller, FRONT_END_REPLAY_STARTING);
        }
        break;

    case FRONT_END_REPLAY_STARTING:
        if (controller->stateTimer.current == 2)
        {
            FrontEndBeginSelectionTransition(5, 0x20, 0, 0, 0, 0x2b);
            EnsureAsciiSelectionVm(480.0f, 392.0f);
        }

        if (controller->stateTimer.current >= 32)
        {
            SetScreen(controller, FRONT_END_SCREEN_START_GAME);
            g_FrontEndSelectedStage = controller->selectedReplayStage + 1;
            g_FrontEndSelectedStageRecord =
                g_FrontEndStageRecords[controller->selectedReplayStage];
            g_FrontEndSelectedStageMirror = g_FrontEndSelectedStage;
            FrontEndFinalizeGameSelection(6.0f);
            g_FrontEndNextGameMode = 12;

            ReplayManager *replay =
                controller->replayFiles[controller->selectedReplay];
            strcpy(g_FrontEndDemoPath, replay->replayPath);
            g_ReplayCharacter = replay->replayData->character;
            g_ReplayShotType = replay->replayData->shotType;
            g_ReplayDifficulty = replay->replayData->difficulty;
            g_FrontEndMode = 2;
            g_FrontEndReplayCursor = controller->selectedReplay;
        }
        break;

    case FRONT_END_REPLAY_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            for (int replayIndex = 0; replayIndex < 50; ++replayIndex)
                ReplayManager::Destroy(controller->replayFiles[replayIndex]);
            memset(controller->replayFiles, 0, sizeof(controller->replayFiles));

            DeleteVm(controller, 0x65);
            InterruptVm(controller, 0x5a, 8);
            InterruptVm(controller, 0x5b, 8);
            DeleteVm(controller, 0x5e);
            InterruptVmId(controller->difficultyAuxVmId.value, 1);
            SetScreen(controller, FRONT_END_SCREEN_MAIN_MENU_RETURN);
            controller->cursor.Pop();
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00431BA0 FrontEndControllerView::DrawReplay
int __stdcall FrontEndControllerView::DrawReplay(
    FrontEndControllerView *controller)
{
    AsciiManagerView *ascii = g_AsciiManagerView;

    if (controller->screenState == FRONT_END_REPLAY_SELECT_FILE)
    {
        ascii->drawShadow = 1;
        AnmFloat3View position(58.0f, 80.0f, 0.0f);

        for (int replayIndex = 0; replayIndex < 25; ++replayIndex)
        {
            ascii->color = controller->cursor.current == replayIndex
                ? 0xffffff00 : 0xff808080;
            ReplayManager *replay = controller->replayFiles[replayIndex];
            if (replay == NULL)
            {
                ascii->AddFormatText(
                    &position,
                    "No.%.2d -------- --/--/-- --:-- ------- ------- --- ---%%",
                    replayIndex + 1);
            }
            else
            {
                DrawReplaySummary(ascii, &position, replay, replayIndex + 1);
            }
            position.y += 15.0f;
        }
    }
    else if (controller->screenState == FRONT_END_REPLAY_SELECT_STAGE)
    {
        ascii->drawShadow = 1;
        AnmFloat3View position(80.0f, 80.0f, 0.0f);
        if (controller->stateTimer.current < 10)
        {
            position.y += controller->selectedReplay * 15.0f *
                (10.0f - controller->stateTimer.subframe) * 0.1f;
        }

        ReplayManager *replay =
            controller->replayFiles[controller->selectedReplay];
        DrawReplaySummary(
            ascii, &position, replay, controller->selectedReplay + 1);

        if (controller->stateTimer.current >= 10)
        {
            position.x = 220.0f;
            position.y = 128.0f;
            for (int stage = 1; stage <= 7; ++stage)
            {
                ascii->color = controller->cursor.current == stage - 1
                    ? 0xffffff00 : 0xff808080;
                ReplayStageDataHeader *header =
                    replay->stageStates[stage].header;
                if (header == NULL)
                {
                    ascii->AddFormatText(
                        &position, "%s  ---------",
                        g_FrontEndStageNames[stage]);
                }
                else
                {
                    ReplayStageDataHeader *nextHeader = stage < 6
                        ? replay->stageStates[stage + 1].header : NULL;
                    if (nextHeader != NULL)
                    {
                        ascii->AddFormatText(
                            &position, "%s  %.8d%d",
                            g_FrontEndStageNames[stage], nextHeader->unknown00C,
                            nextHeader->unknown1B4);
                    }
                    else
                    {
                        ascii->AddFormatText(
                            &position, "%s  %.8d%d",
                            g_FrontEndStageNames[stage], replay->replayData->score,
                            replay->replayData->unknown060);
                    }
                }
                position.y += 18.0f;
            }
        }
    }
    else
    {
        return 1;
    }

    ascii->color = 0xffffffff;
    ascii->drawShadow = 0;
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00431EE0 FrontEndControllerView::UpdatePractice
int __stdcall FrontEndControllerView::UpdatePractice(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_PRACTICE_INITIALIZE:
        controller->cursor.count = 6;
        controller->cursor.SetCurrent(0);
        controller->practiceDifficultyCursor.count = 5;
        controller->practiceDifficultyCursor.SetCurrent(1);
        controller->practiceDifficultyCursor.wraps = 1;

        controller->practicePageCursor.count =
            (CountPracticeRecordsForDifficulty(
                controller->practiceDifficultyCursor.current) + 9) / 10 + 1;
        controller->practicePageCursor.SetCurrent(0);
        controller->practicePageCursor.wraps = 1;

        if (ResolveVm(&controller->vmIds[0x5e]) == NULL)
        {
            CreateVm(controller, 0x5e);
            controller->difficultyAuxVmId =
                g_AsciiManagerView->asciiAnm->CreateVmVariant0(
                    8, FRONT_END_RENDER_LAYER);
        }

        CreateVm(controller, 0x66);
        SetScreenState(controller, FRONT_END_PRACTICE_OPENING);
        CreateVm(controller, 0x98 + controller->cursor.current / 3);
        CreateVm(controller, 0x9a + controller->cursor.current);
        CreateVm(
            controller,
            0xa0 + controller->practiceDifficultyCursor.current);
        CreateVm(controller, 0xa8);
        CreateVm(controller, 0xa9);
        CreateVm(controller, 0xaa);
        CreateVm(controller, 0xab);
        CreateVm(controller, 0xa5);
        CreateVm(controller, 0xa6);
        CreateVm(controller, 0xa7);
        CreateVm(controller, 0xac);
        break;

    case FRONT_END_PRACTICE_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_PRACTICE_ACTIVE);
            return 1;
        }
        break;

    case FRONT_END_PRACTICE_ACTIVE:
    {
        FrontEndCursorView *difficulty =
            &controller->practiceDifficultyCursor;
        FrontEndCursorView *page = &controller->practicePageCursor;

        controller->cursor.previous = controller->cursor.current;
        difficulty->previous = difficulty->current;
        page->previous = page->current;

        if (InputRepeated(FRONT_END_INPUT_UP))
        {
            difficulty->Move(-1);
            InterruptVmNow(controller, 0xaa, 2);
        }
        if (InputRepeated(FRONT_END_INPUT_DOWN))
        {
            difficulty->Move(1);
            InterruptVmNow(controller, 0xab, 2);
        }

        if (difficulty->previous != difficulty->current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            DeleteVm(controller, 0xa0 + difficulty->previous);
            CreateVm(controller, 0xa0 + difficulty->current);
            if (page->current > 0)
            {
                page->SetCurrent(1);
                controller->RefreshPracticeRecords();
            }
            page->count =
                (CountPracticeRecordsForDifficulty(difficulty->current) + 9) /
                    10 + 1;
        }

        if (InputRepeated(FRONT_END_INPUT_LEFT))
        {
            controller->cursor.Move(-1);
            InterruptVmNow(controller, 0xa8, 2);
        }
        if (InputRepeated(FRONT_END_INPUT_RIGHT))
        {
            controller->cursor.Move(1);
            InterruptVmNow(controller, 0xa9, 2);
        }

        if (controller->cursor.previous != controller->cursor.current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            if (controller->cursor.previous / 3 !=
                controller->cursor.current / 3)
            {
                DeleteVm(
                    controller, 0x98 + controller->cursor.previous / 3);
                CreateVm(
                    controller, 0x98 + controller->cursor.current / 3);
            }
            DeleteVm(controller, 0x9a + controller->cursor.previous);
            CreateVm(controller, 0x9a + controller->cursor.current);
            if (page->current > 0)
                controller->RefreshPracticeRecords();
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            if (page->current == 0)
            {
                for (int row = 0; row < FRONT_END_PRACTICE_ROWS_PER_PAGE;
                     ++row)
                {
                    controller->practiceRowVmIds[row] =
                        g_AsciiManagerView->asciiAnm->CreateVmVariant0(
                            row + 0x17, FRONT_END_RENDER_LAYER);
                }
            }

            page->Move(1);
            if (page->current == 0)
                InterruptPracticeRows(controller);
            else
                controller->RefreshPracticeRecords();
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);
        }

        if (difficulty->current == 4 && controller->cursor.current == 2)
        {
            if ((g_FrontEndInput.pressed & 0x160b) != 0)
            {
                g_FrontEndPracticeSecretProgress = 0;
                g_FrontEndPracticeSecretIdleFrames = 0;
            }
            UpdatePracticeSecretSequence();
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            SetScreenState(controller, FRONT_END_PRACTICE_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
            DeleteVm(controller, 0xa0 + difficulty->current);
            DeleteVm(controller, 0x9a + controller->cursor.current);
            DeleteVm(controller, 0x98 + controller->cursor.current / 3);
            DeleteVm(controller, 0xa8);
            DeleteVm(controller, 0xa9);
            DeleteVm(controller, 0xaa);
            DeleteVm(controller, 0xab);
            DeleteVm(controller, 0xa5);
            DeleteVm(controller, 0xa6);
            DeleteVm(controller, 0xa7);
            DeleteVm(controller, 0xac);
            InterruptPracticeRows(controller);
            return 1;
        }
        break;
    }

    case FRONT_END_PRACTICE_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x66);
            InterruptVm(controller, 0x5a, 8);
            InterruptVm(controller, 0x5b, 8);
            DeleteVm(controller, 0x5e);
            InterruptVmId(controller->difficultyAuxVmId.value, 1);
            SetScreen(controller, FRONT_END_SCREEN_MAIN_MENU_RETURN);
            controller->cursor.Pop();
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00432690 FrontEndControllerView::RefreshPracticeRecords
int FrontEndControllerView::RefreshPracticeRecords()
{
    int record = 0;
    int matchingRecords = 0;
    int recordsToSkip =
        practicePageCursor.current * FRONT_END_PRACTICE_ROWS_PER_PAGE -
        FRONT_END_PRACTICE_ROWS_PER_PAGE;

    while (matchingRecords < recordsToSkip)
    {
        if (g_FrontEndPracticeDifficulties[record] ==
            practiceDifficultyCursor.current)
        {
            ++matchingRecords;
        }
        ++record;
    }

    practiceDisplayedEntries = 0;
    int row = 0;
    for (; row < FRONT_END_PRACTICE_ROWS_PER_PAGE; ++row)
    {
        while (record < FRONT_END_PRACTICE_RECORD_COUNT &&
               g_FrontEndPracticeDifficulties[record] !=
                   practiceDifficultyCursor.current)
        {
            ++record;
        }
        if (record >= FRONT_END_PRACTICE_RECORD_COUNT)
            break;

        unsigned char *catalog = GetPracticeCatalogRecord(record);
        int *result = GetPracticeResultRecord(cursor.current, record);
        AnmVmView *vm = ResolveVm(&practiceRowVmIds[row]);
        if (*reinterpret_cast<int *>(catalog + 0x84) != 0)
        {
            char name[168];
            strcpy(name, reinterpret_cast<char *>(catalog));
            int length = strlen(name);
            if (length < 42)
            {
                memset(name + length, ' ', 42 - length);
                length = 42;
            }
            name[length] = '\0';

            unsigned int color = result[0] != 0
                ? 0x00ffff80u : 0x00efefefu;
            g_AnmRenderManagerView->DrawTextCentered(
                vm, color, "No.%3d %s %4d/%4d",
                row + 1, name, result[0], result[1]);
        }
        else
        {
            g_AnmRenderManagerView->DrawTextCentered(
                vm, 0x00808080u, g_FrontEndPracticeUnavailableFormat,
                row + 1, result[0], result[1]);
        }

        ++record;
        ++practiceDisplayedEntries;
    }

    for (; row < FRONT_END_PRACTICE_ROWS_PER_PAGE; ++row)
    {
        AnmVmView *vm = ResolveVm(&practiceRowVmIds[row]);
        g_AnmRenderManagerView->DrawTextCentered(
            vm, 0xffffffffu, " ");
    }
    return 0;
}


// TH10_FRONTEND_FUNCTION: 0x004329F0 FrontEndControllerView::DrawPractice
int FrontEndControllerView::DrawPractice()
{
    if (screenState != FRONT_END_PRACTICE_ACTIVE)
        return 1;

    AsciiManagerView *ascii = g_AsciiManagerView;
    ascii->drawShadow = 1;
    AnmFloat3View position(48.0f, 160.0f, 0.0f);
    int difficulty = practiceDifficultyCursor.current;

    if (practicePageCursor.current == 0)
    {
        unsigned int shade = 0xff;
        for (int row = 0; row < FRONT_END_PRACTICE_ROWS_PER_PAGE; ++row)
        {
            ascii->color =
                ((shade << 8 | shade) << 8) | 0xff0000ffu;
            FrontEndScoreRecordView *record =
                GetPracticeScoreRecord(cursor.current, difficulty, row);
            if (record->timestamp == 0)
            {
                ascii->AddFormatText(
                    &position,
                    "%2d  %s  %9ld%d  ----/--/-- --:--  Stage -  ---%%",
                    row + 1, record->name, record->score,
                    record->scoreSuffix, (double)record->slowdownRate);
            }
            else
            {
                tm *localTime = localtime(&record->timestamp);
                ascii->AddFormatText(
                    &position,
                    "%2d  %s  %9ld%d  %.4d/%.2d/%.2d %.2d:%.2d  %s  %2.1f%%",
                    row + 1, record->name, record->score,
                    record->scoreSuffix, localTime->tm_year + 1900,
                    localTime->tm_mon + 1, localTime->tm_mday,
                    localTime->tm_hour, localTime->tm_min,
                    g_FrontEndStageNames[record->stageNameIndex],
                    (double)record->slowdownRate);
            }
            shade -= 0x10;
            position.y += 18.0f;
        }
    }

    unsigned char *shotProfile =
        g_FrontEndProfileData + cursor.current * 0x437c;
    ascii->color = 0xffffffffu;
    position.x = 328.0f;
    position.y = 378.0f;
    ascii->AddFormatText(
        &position, "    %5d",
        *reinterpret_cast<int *>(shotProfile + 0x4c8));

    int playTimeFrames =
        *reinterpret_cast<int *>(shotProfile + 0x4cc);
    position.y = 396.0f;
    ascii->AddFormatText(
        &position, "%3d:%.2d:%.2d", playTimeFrames / 216000,
        playTimeFrames / 3600 % 60, playTimeFrames / 60 % 60);

    position.y = 414.0f;
    ascii->AddFormatText(
        &position, "    %5d",
        *reinterpret_cast<int *>(shotProfile + 0x4d0 + difficulty * 4));
    ascii->color = 0xffffffffu;
    ascii->drawShadow = 0;
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00432CB0 FrontEndControllerView::UpdateScoreEntry
int __stdcall FrontEndControllerView::UpdateScoreEntry(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_SCORE_ENTRY_INITIALIZE:
    {
        controller->cursor.count = 30;
        FrontEndLoadMusic(0, "bgm/th10_17.wav");
        FrontEndPlayMusic(0, 17);

        if (ResolveVm(&controller->vmIds[0x5e]) == NULL)
        {
            CreateVm(controller, 0x5e);
            controller->difficultyAuxVmId =
                g_AsciiManagerView->asciiAnm->CreateVmVariant0(
                    8, FRONT_END_RENDER_LAYER);
        }

        CreateVm(controller, 0x68);
        SetScreenState(controller, FRONT_END_SCORE_ENTRY_OPENING);
        CreateVm(controller, 0x98 + g_ReplayCharacter);
        int shot = g_ReplayShotType + g_ReplayCharacter * 3;
        CreateVm(controller, 0x9a + shot);
        CreateVm(controller, 0xa0 + g_ReplayDifficulty);

        g_FrontEndSelectedStage = FRONT_END_SCORE_ENTRY_NAME_LENGTH;
        g_FrontEndSelectedStageMirror = FRONT_END_SCORE_ENTRY_NAME_LENGTH;
        g_FrontEndScoreFormatTable = g_FrontEndScoreEntryFormatTable;
        int insertedRow = FrontEndInsertScore(GetFrontEndScoreTable(shot));
        g_FrontEndScoreFormatTable = g_FrontEndDefaultScoreFormatTable;
        g_FrontEndSelectedStage = 0;
        g_FrontEndSelectedStageMirror = 0;

        if (insertedRow < 0)
        {
            controller->cursor.SetCurrent(insertedRow);
            controller->scoreEntryUnavailable = 1;
        }
        else
        {
            ResetStateTimer(controller);
            controller->cursor.wraps = 1;
            controller->cursor.SetCurrent(insertedRow);

            FrontEndCursorView *keyboard =
                &controller->scoreEntryKeyboardCursor;
            keyboard->SetCurrent(0);
            keyboard->count = strlen(g_FrontEndScoreEntryAlphabet);
            keyboard->wraps = 1;

            strcpy(
                controller->scoreEntryName,
                reinterpret_cast<char *>(
                    g_FrontEndProfileData + 0x1d878));
            if (strcmp(controller->scoreEntryName, "        ") != 0)
                keyboard->Move(-1);

            int cursor = FRONT_END_SCORE_ENTRY_NAME_LENGTH;
            while (cursor > 0 &&
                   controller->scoreEntryName[cursor - 1] == ' ')
            {
                --cursor;
            }
            controller->scoreEntryNameCursor = cursor;
            controller->scoreEntryUnavailable = 0;
        }
    }
    case FRONT_END_SCORE_ENTRY_OPENING:
        if (controller->stateTimer.current > 6)
        {
            SetScreenState(controller, FRONT_END_SCORE_ENTRY_ACTIVE);
            return 1;
        }
        break;

    case FRONT_END_SCORE_ENTRY_ACTIVE:
    {
        FrontEndCursorView *keyboard =
            &controller->scoreEntryKeyboardCursor;
        if (controller->scoreEntryUnavailable == 0)
        {
            keyboard->previous = keyboard->current;
            if (InputRepeated(FRONT_END_INPUT_UP))
                keyboard->Move(-FRONT_END_SCORE_ENTRY_COLUMNS);
            if (InputRepeated(FRONT_END_INPUT_DOWN))
                keyboard->Move(FRONT_END_SCORE_ENTRY_COLUMNS);
            if (InputRepeated(FRONT_END_INPUT_LEFT))
            {
                keyboard->Move(
                    keyboard->current % FRONT_END_SCORE_ENTRY_COLUMNS == 0
                        ? FRONT_END_SCORE_ENTRY_COLUMNS - 1 : -1);
            }
            if (InputRepeated(FRONT_END_INPUT_RIGHT))
            {
                keyboard->Move(
                    keyboard->current % FRONT_END_SCORE_ENTRY_COLUMNS ==
                            FRONT_END_SCORE_ENTRY_COLUMNS - 1
                        ? -(FRONT_END_SCORE_ENTRY_COLUMNS - 1) : 1);
            }
            if (keyboard->previous != keyboard->current)
                FrontEndPlaySound(FRONT_END_SOUND_MOVE);
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            if (controller->scoreEntryUnavailable == 0)
            {
                int alphabetLength = strlen(g_FrontEndScoreEntryAlphabet);
                int key = keyboard->current;
                if (key < alphabetLength - 3)
                {
                    int cursor = controller->scoreEntryNameCursor;
                    if (cursor < FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                    {
                        controller->scoreEntryName[cursor] =
                            g_FrontEndScoreEntryAlphabet[key];
                        ++controller->scoreEntryNameCursor;
                        if (controller->scoreEntryNameCursor >=
                            FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                        {
                            keyboard->SetCurrent(alphabetLength - 1);
                        }
                    }
                    else
                    {
                        controller->scoreEntryName[
                            FRONT_END_SCORE_ENTRY_NAME_LENGTH - 1] =
                                g_FrontEndScoreEntryAlphabet[key];
                    }
                }
                else if (key == alphabetLength - 3)
                {
                    int cursor = controller->scoreEntryNameCursor;
                    if (cursor < FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                    {
                        controller->scoreEntryName[cursor] = ' ';
                        ++controller->scoreEntryNameCursor;
                        if (controller->scoreEntryNameCursor >=
                            FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                        {
                            keyboard->SetCurrent(alphabetLength - 1);
                        }
                    }
                    else
                    {
                        controller->scoreEntryName[
                            FRONT_END_SCORE_ENTRY_NAME_LENGTH - 1] = ' ';
                    }
                }
                else if (key == alphabetLength - 2)
                {
                    if (controller->scoreEntryNameCursor == 0)
                        return 1;
                    --controller->scoreEntryNameCursor;
                    controller->scoreEntryName[
                        controller->scoreEntryNameCursor] = ' ';
                }
                else if (key == alphabetLength - 1)
                {
                    int shot = g_ReplayShotType + g_ReplayCharacter * 3;
                    FrontEndScoreRecordView *record =
                        GetPracticeScoreRecord(
                            shot, g_ReplayDifficulty,
                            controller->cursor.current);
                    strcpy(record->name, controller->scoreEntryName);
                    strcpy(
                        reinterpret_cast<char *>(
                            g_FrontEndProfileData + 0x1d878),
                        controller->scoreEntryName);
                    SetScreenState(
                        controller, FRONT_END_SCORE_ENTRY_CLOSING);
                }
            }
            else
            {
                SetScreenState(
                    controller, FRONT_END_SCORE_ENTRY_CLOSING);
            }
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            if (controller->scoreEntryUnavailable != 0)
            {
                SetScreenState(
                    controller, FRONT_END_SCORE_ENTRY_CLOSING);
                FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                return 1;
            }
            if (controller->scoreEntryNameCursor != 0)
            {
                FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
                --controller->scoreEntryNameCursor;
                controller->scoreEntryName[
                    controller->scoreEntryNameCursor] = ' ';
                return 1;
            }
        }
        break;
    }

    case FRONT_END_SCORE_ENTRY_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x68);
            DeleteVm(controller, 0x98 + g_ReplayCharacter);
            DeleteVm(
                controller,
                0x9a + g_ReplayShotType + g_ReplayCharacter * 3);
            DeleteVm(controller, 0xa0 + g_ReplayDifficulty);
            SetScreen(controller, FRONT_END_SCREEN_RESULT);
        }
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x00433230 FrontEndControllerView::DrawScoreEntry
int __stdcall FrontEndControllerView::DrawScoreEntry(
    FrontEndControllerView *controller)
{
    if (controller->screenState != FRONT_END_SCORE_ENTRY_ACTIVE)
        return 1;

    AsciiManagerView *ascii = g_AsciiManagerView;
    ascii->drawShadow = 1;
    AnmFloat3View position(48.0f, 160.0f, 0.0f);
    int shot = g_ReplayShotType + g_ReplayCharacter * 3;
    unsigned int shade = 0xff;

    for (int row = 0; row < FRONT_END_SCORE_ENTRY_ROWS; ++row)
    {
        if (controller->scoreEntryUnavailable != 0)
        {
            ascii->color =
                ((shade << 8 | shade) << 8) | 0xff0000ffu;
        }
        else
        {
            ascii->color = controller->cursor.current == row
                ? 0xffffffffu : 0xff404040u;
        }

        FrontEndScoreRecordView *record =
            GetPracticeScoreRecord(shot, g_ReplayDifficulty, row);
        if (record->timestamp == 0)
        {
            ascii->AddFormatText(
                &position,
                "%2d  %s  %9ld%d  ----/--/-- --:--  Stage -  ---%%",
                row + 1, record->name, record->score,
                record->scoreSuffix, (double)record->slowdownRate);
        }
        else
        {
            tm *localTime = localtime(&record->timestamp);
            ascii->AddFormatText(
                &position,
                "%2d  %s  %9ld%d  %.4d/%.2d/%.2d %.2d:%.2d  %s  %2.1f%%",
                row + 1, record->name, record->score,
                record->scoreSuffix, localTime->tm_year + 1900,
                localTime->tm_mon + 1, localTime->tm_mday,
                localTime->tm_hour, localTime->tm_min,
                g_FrontEndStageNames[record->stageNameIndex],
                (double)record->slowdownRate);
        }
        shade -= 0x10;
        position.y += 18.0f;
    }

    if (controller->scoreEntryUnavailable == 0)
    {
        int alphabetLength = strlen(g_FrontEndScoreEntryAlphabet);
        position.x = 84.0f;
        position.y =
            controller->cursor.current * 18.0f + 160.0f;
        ascii->color = 0xffffffffu;
        ascii->AddFormatText(&position, "%s", controller->scoreEntryName);

        position.x =
            controller->scoreEntryNameCursor * 9.0f + 84.0f;
        if (controller->scoreEntryNameCursor ==
            FRONT_END_SCORE_ENTRY_NAME_LENGTH)
        {
            position.x -= 9.0f;
        }
        ascii->color = 0xffffff00u;
        ascii->AddFormatText(&position, "_");

        position.x = 212.0f;
        position.y = 360.0f;
        position.z = 0.0f;
        for (int key = 0; key < alphabetLength; ++key)
        {
            ascii->color =
                controller->scoreEntryKeyboardCursor.current == key
                    ? 0xffffff00u : 0xff808080u;
            int glyph;
            if (key < alphabetLength - 3)
                glyph = g_FrontEndScoreEntryAlphabet[key];
            else if (key == alphabetLength - 3)
                glyph = 0x81;
            else if (key == alphabetLength - 2)
                glyph = 0x7f;
            else
                glyph = 0x80;
            ascii->AddFormatText(&position, "%c", glyph);

            if (key % FRONT_END_SCORE_ENTRY_COLUMNS ==
                FRONT_END_SCORE_ENTRY_COLUMNS - 1)
            {
                position.x = 212.0f;
                position.y += 16.0f;
            }
            else
            {
                position.x += 18.0f;
            }
        }
        ascii->color = 0xffffffffu;
    }
    return 1;
}
