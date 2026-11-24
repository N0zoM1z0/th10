#include "Main.hpp"
#include "EnemySoundQueue.hpp"
#include "FrontEnd.hpp"
#include "Player.hpp"
#include "InputState.hpp"
#include "InputPolling.hpp"
#include "ReplayManager.hpp"
#include "FileSystem.hpp"
#include "VmIdInterrupt.hpp"
#include "GameScoreState.hpp"
#include <stdlib.h>
#include <math.h>

#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// TH10 0x0040AD20 is the shared cursor setter used by multiple FrontEnd
// controllers. It takes the value in ECX and the cursor in EDX; the member API
// in FrontEnd.hpp delegates here.
__declspec(noinline) int __fastcall FrontEndCursorSetCurrent(
    int value, FrontEndCursorView *cursor)
{
    int count = cursor->count;
    if (count != 0)
    {
        if (value >= count)
        {
            // The original LEA/DEC publishes count-1 as a signed native
            // DWORD, even when the source count is INT_MIN. Copy the low
            // four bytes without overflowing a signed C++ subtraction.
            const unsigned int nextCountBits =
                static_cast<unsigned int>(count) - 1u;
            memcpy(&count, &nextCountBits, sizeof count);
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
extern int g_MainModeTransitionCounter;
extern int g_FrontEndDemoFrames;
extern int g_FrontEndDemoIndex;
extern const char *g_FrontEndDemoPaths[4];
extern char g_FrontEndDemoPath[];
extern unsigned char g_FrontEndStageRecords[][0x30];
extern unsigned char g_ScriptViewerRouteDescriptor[];
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
// Retail inlines every front-end sound cue as a direct queue call.
static inline void FrontEndPlaySound(int soundId)
{
    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
        soundId, 0);
}
int __stdcall FrontEndLoadMusic(int slot, const char *path);
int __stdcall FrontEndPlayMusic(int slot, int trackIndex);
extern signed char *__fastcall FrontEndGetControllerState(int controllerIndex);
extern int FrontEndReadKeyboardState(unsigned char *keyboardState);

int __stdcall FrontEndUpdateMainMenu(FrontEndControllerView *controller);
int __stdcall FrontEndUpdateMainMenuReturn(FrontEndControllerView *controller);
int GamePrepareDummyMusic();
int __stdcall FrontEndUpdateMusicRoom(FrontEndControllerView *controller);
// TH10 practice unlock helper: 0x0042C8C0, reviewed 93-byte extent.
// The source signature preserves the observed explicit profile receiver;
// The target passes it in EAX; source-to-native ABI correspondence and
// original authorship remain unproved.
void FrontEndUnlockPracticeRecords(unsigned char *profile)
{
    for (int entry = 0; entry < 16; ++entry)
        profile[0x1d882 + entry] = 1;
    for (int group = 0; group < 6; ++group)
    {
        for (int difficulty = 0; difficulty < 4; ++difficulty)
        {
            for (int stage = 0; stage < 6; ++stage)
                profile[0x4e9 + group * 0x437c +
                    (difficulty * 6 + stage) * 8] = 1;
        }
    }
}
int __stdcall GameQueueScaledSoundCommand(float magnitude);
int __fastcall CountGameStatisticsRankKind(int unused, int kind);
void *__stdcall StageCreateTransitionEffect(int drawPriority, int kind, int frames,
    unsigned int value0, unsigned int value1, unsigned int value2);

// Target 0x0042C6D0 is a repeated front-end VM retirement seam.  It is kept
// as a TU-visible helper so LTCG can retain the target's private EDI/ESI
// controller/index inputs while selecting the established manager FindVm
// receiver ABI.
void DeleteVm(FrontEndControllerView *controller, int index)
{
    FrontEndVmIdStorageView *id = &controller->vmIds[index];
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*id);
    if (vm != NULL)
    {
        vm->pendingInterrupt = 1;
        if (vm->layerNode.previous == NULL)
        {
            AnmVmLayerNodeView *node = vm->layerNode.next;
            while (node != NULL)
            {
                static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
                node = node->next;
            }
        }
    }
    id->value = 0;
}


namespace
{

enum
{
    FRONT_END_FLAG_DEMO_TRANSITION = 0x20,
    FRONT_END_DRAW_CALLBACK_UNLOCKED = 0x02,
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
    FrontEndScoreRecordView records[5][10];
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

static int InputRepeated(unsigned short mask)
{
    return g_FrontEndInput.IsRepeated(mask);
}

static AnmVmView *ResolveVm(FrontEndVmIdStorageView *id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(id->value);
    if (vm == NULL)
        id->value = 0;
    return vm;
}

static int FindChildVmId(
    FrontEndVmIdStorageView *parentId, short scriptIndex)
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

static FrontEndStageScoreRecordView *GetFrontEndStageScoreRecord(int stage)
{
    const unsigned int shotWord =
        static_cast<unsigned int>(g_ReplayShotType) +
        static_cast<unsigned int>(g_ReplayCharacter) * 3u;
    const unsigned int stageWord =
        static_cast<unsigned int>(stage) +
        static_cast<unsigned int>(g_ReplayDifficulty) * 6u;
    return reinterpret_cast<FrontEndStageScoreRecordView *>(
        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
        shotWord * 0x437cu + 0x4dcu +
        stageWord * static_cast<unsigned int>(sizeof(FrontEndStageScoreRecordView)));
}

static __forceinline int CountPracticeRecordsForDifficulty(int difficulty)
{
    return CountGameStatisticsRankKind(0, difficulty);
}

static unsigned char *GetPracticeCatalogRecord(int record)
{
    return reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(g_FrontEndProfileData) + 0x19a8cu +
        static_cast<unsigned int>(record) * 0x90u);
}

static int *GetPracticeResultRecord(int shot, int record)
{
    return reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
        static_cast<unsigned int>(shot) * 0x437cu + 0x624u +
        static_cast<unsigned int>(record) * 0x90u);
}

static FrontEndScoreRecordView *GetPracticeScoreRecord(
    int shot, int difficulty, int row)
{
    return reinterpret_cast<FrontEndScoreRecordView *>(
        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
        static_cast<unsigned int>(shot) * 0x437cu + 0x18u +
        (static_cast<unsigned int>(difficulty) * 10u +
         static_cast<unsigned int>(row)) * 0x18u);
}

static FrontEndScoreTableView *GetFrontEndScoreTable(int shot)
{
    return reinterpret_cast<FrontEndScoreTableView *>(
        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
        static_cast<unsigned int>(shot) * 0x437cu + 8u);
}

static __forceinline void InterruptPracticeRows(FrontEndControllerView *controller)
{
    for (int row = 0; row < FRONT_END_PRACTICE_ROWS_PER_PAGE; ++row)
        EnemyMarkPendingInterrupt(&controller->practiceRowVmIds[row].value);
}

static __forceinline void UpdatePracticeSecretSequence()
{
    memcpy(
        g_FrontEndPreviousKeyboardState, g_FrontEndKeyboardState,
        sizeof(g_FrontEndPreviousKeyboardState));
    InputKeyboardSnapshotView keyboard;
#pragma inline_depth(0)
    const int read = keyboard.ReadKeyboardState(g_FrontEndKeyboardState);
#pragma inline_depth(16)
    if (read == 0)
        goto tickTimeout;

    for (int key = 0; key < 256; ++key)
    {
        g_FrontEndKeyboardTransitions[key] =
            (g_FrontEndPreviousKeyboardState[key] ^
             g_FrontEndKeyboardState[key]) &
            g_FrontEndKeyboardState[key];
    }

    if (static_cast<unsigned int>(g_FrontEndPracticeSecretProgress) >= 22u)
    {
        FrontEndUnlockPracticeRecords(g_FrontEndProfileData);
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
        unsigned char pressed = 0;
        for (int key = 0; key < 57; ++key)
            pressed |= g_FrontEndKeyboardTransitions[key];
        if ((pressed & 0x80) != 0)
            g_FrontEndPracticeSecretProgress = 0;
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

// The options screen reaches the configuration and sound queue from the
// supervisor at 0x00491C28. The queue at 0x00492590 is 0x968 bytes later.
struct FrontEndOptionsApplicationView
{
    MainSupervisorView supervisor;
    unsigned char unknown784[0x968 - sizeof(MainSupervisorView)];
    EnemySoundQueueView sound;
};

typedef char FrontEndOptionsSoundOffset[
    (offsetof(FrontEndOptionsApplicationView, sound) == 0x968) ? 1 : -1];

static __forceinline FrontEndOptionsApplicationView &GetOptionsApplication()
{
    return reinterpret_cast<FrontEndOptionsApplicationView &>(g_MainSupervisorView);
}

static __forceinline GameConfigurationView &GetOptionsConfiguration()
{
    return GetOptionsApplication().supervisor.configuration;
}

static __forceinline void QueueOptionsSound(int soundId)
{
    GetOptionsApplication().sound.QueueSoundSample(soundId, 0);
}

// Options owners keep the parent-lookup manager across label operations.
// Failed parent lookup clears its id but retains the target's unchecked list
// contract. Return the typed child id so each caller selects its resolution seam.
static __forceinline AnmVmIdView FindOptionsChildId(
    FrontEndControllerView *controller, AnmRenderManagerView *manager,
    short scriptIndex)
{
    FrontEndVmIdStorageView *parentId = &controller->vmIds[FRONT_END_ROOT_VM];
    AnmVmView *parent = manager->FindVm(*parentId);
    if (parent == NULL)
        parentId->value = 0;

    AnmVmLayerNodeView *node = &parent->layerNode;
    while (node != NULL)
    {
        AnmVmView *child = static_cast<AnmVmView *>(node->owner);
        if (child->scriptIndex == scriptIndex)
            return AnmVmIdView(static_cast<AnmVmView *>(node->owner)->id);
        node = node->next;
    }
    return AnmVmIdView();
}

static __forceinline AnmVmView *FindOptionsChildVm(
    FrontEndControllerView *controller, AnmRenderManagerView *manager,
    short scriptIndex)
{
    return manager->FindVm(FindOptionsChildId(controller, manager, scriptIndex));
}

static __forceinline void RunOptionsCursor(FrontEndControllerView *controller)
{
    g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
        static_cast<const AnmVmIdView &>(controller->vmIds[FRONT_END_ROOT_VM]), 3);
}

static __forceinline void SetOptionsLabelSprite(
    AnmRenderManagerView *manager, FrontEndControllerView *controller,
    int scriptIndex, int spriteIndex)
{
    AnmVmIdView child = FindOptionsChildId(controller, manager, scriptIndex);
    child.SetSprite(spriteIndex);
}

static __forceinline void SetHintModeSprites(
    AnmRenderManagerView *manager, FrontEndControllerView *controller)
{
    if (GetOptionsConfiguration().hintMode == 0)
    {
        SetOptionsLabelSprite(manager, controller, 0x1f, 0x2e);
        SetOptionsLabelSprite(manager, controller, 0x20, 0x2f);
    }
    else if (GetOptionsConfiguration().hintMode == 1)
    {
        SetOptionsLabelSprite(manager, controller, 0x1f, 0x30);
        SetOptionsLabelSprite(manager, controller, 0x20, 0x31);
    }
    else
    {
        SetOptionsLabelSprite(manager, controller, 0x1f, 0x2c);
        SetOptionsLabelSprite(manager, controller, 0x20, 0x2d);
    }
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

// Key-config display retains its manager, but samples root slot 2 per label.
// __forceinline is a scoped expansion hypothesis, not a recovered source keyword.
// Failed parent lookup preserves the target's unchecked parent-list precondition.
static __forceinline AnmVmIdView FindKeyConfigChildId(
    FrontEndControllerView *controller, AnmRenderManagerView *manager,
    short scriptIndex)
{
    FrontEndVmIdStorageView *parentId = &controller->vmIds[FRONT_END_KEY_CONFIG_ROOT_VM];
    AnmVmView *parent = manager->FindVm(*parentId);
    if (parent == NULL)
        parentId->value = 0;

    AnmVmLayerNodeView *node = &parent->layerNode;
    while (node != NULL)
    {
        AnmVmView *child = static_cast<AnmVmView *>(node->owner);
        if (child->scriptIndex == scriptIndex)
            return AnmVmIdView(static_cast<AnmVmView *>(node->owner)->id);
        node = node->next;
    }
    return AnmVmIdView();
}

static __forceinline AnmVmView *FindKeyConfigChildVm(
    FrontEndControllerView *controller, AnmRenderManagerView *manager,
    short scriptIndex)
{
    return manager->FindVm(FindKeyConfigChildId(controller, manager, scriptIndex));
}


} // namespace

// Target 0x0042C730 and 0x0042C750 retain separate constant-interrupt handlers.
__declspec(noinline) void FrontEndControllerView::InterruptVmNowTwo(
    int index)
{
    g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
        static_cast<const AnmVmIdView &>(vmIds[index]), 2);
}

__declspec(noinline) void FrontEndControllerView::InterruptVmNowThree(
    int index)
{
    g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
        static_cast<const AnmVmIdView &>(vmIds[index]), 3);
}

// Target42C670..42C6CE retains the receiver on the stack and script in EDI.
__declspec(noinline) void FrontEndControllerView::CreateVm(int scriptIndex)
{
    AnmLoadedView *resource = frontAnm;
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->renderLayer = FRONT_END_RENDER_LAYER;
    vm->flags35C |= 0x40000000u;
    resource->InitializeVm(vm, scriptIndex);
    AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
    vmIds[scriptIndex] = id;
}


__declspec(noinline) inline void FrontEndControllerView::SetScreenStateTarget(
    int state)
{
    screenState = state;
    stateTimer.SetCurrent(0);
}

__declspec(noinline) void FrontEndControllerView::SetScreenTarget(int screen)
{
    this->screen = screen;
    screenState = 0;
    stateTimer.SetCurrent(0);
}

int AsciiManagerView::EnsureSelectionVm(float x, float y)
{
    AnmFloat3View position(x, y, 0.0f);
    int vmId = auxiliaryVm89A4;
    if (vmId == 0)
    {
        vmId = asciiAnm->CreateVmAtScreenVariant0(6, &position).value;
        auxiliaryVm89A4 = vmId;
    }
    return vmId;
}


// TH10_FRONTEND_FUNCTION: 0x0042C850 FrontEndHasAnyClear
// Six per-shot progress bytes select whether Extra is available. The broader
// profile owner and the original type/name remain unresolved.
int FrontEndHasAnyClear(const unsigned char *profile)
{
    if (profile[0x1d888] != 0 || profile[0x1d889] != 0 ||
        profile[0x1d88a] != 0 || profile[0x1d88b] != 0 ||
        profile[0x1d88c] != 0 || profile[0x1d88d] != 0)
        return 1;
    return 0;
}


// TH10_FRONTEND_FUNCTION: 0x0042D300 FrontEndUpdateMainMenu
int __stdcall FrontEndUpdateMainMenu(FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case 0:
        if (g_AnmRenderManagerView->FindVm(controller->vmIds[0x58]) == NULL)
        {
            controller->CreateVm(0x58);
            controller->CreateVm(0x5a);
            controller->titleAuxVmId =
                controller->titleAnm->CreateVmVariant0(0, 15);
        }
        controller->SetScreenStateTarget(1);
        controller->cursor.SetCurrent(0);
        // The opening threshold is sampled after the phase/timer reset.
    case 1:
        if (controller->stateTimer.current > 10)
            controller->SetScreenStateTarget(2);
        break;
    case 2:
        if ((g_FrontEndInput.pressed & 0x160b) != 0)
        {
            controller->MarkVmInterrupt(0x5a, 6);
            controller->SetScreenStateTarget(4);
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                0x20, 0);
            EnemyMarkPendingInterrupt(&controller->titleAuxVmId.value);
        }
        break;
    case 4:
        if (controller->stateTimer.current >= 30)
            controller->SetScreenTarget(FRONT_END_SCREEN_MAIN_MENU_RETURN);
        break;
    }
    return 1;
}


// TH10_FRONTEND_FUNCTION: 0x0042D420 FrontEndUpdateMainMenuReturn
int __stdcall FrontEndUpdateMainMenuReturn(FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case 0:
        controller->cursor.count = 8;
        if (FrontEndHasAnyClear(g_FrontEndProfileData) == 0)
            controller->cursor.DisableEntry(1);
        if ((g_FrontEndFlags & 0x10) != 0)
        {
            controller->cursor.Select(2);
            g_FrontEndFlags &= ~0x10u;
        }
        if (g_AnmRenderManagerView->FindVm(controller->vmIds[0x58]) == NULL)
            controller->CreateVm(0x58);
        controller->CreateVm(0);
        controller->SetScreenStateTarget(1);
    case 1:
        if (controller->stateTimer.current > 10)
        {
            controller->SetScreenStateTarget(2);
            g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
                controller->vmIds[0], 3);
            controller->MarkVmInterrupt(0, static_cast<short>(controller->cursor.current + 0x11));
        }
        break;
    case 2:
    {
        FrontEndCursorView *cursor = &controller->cursor;
        cursor->previous = cursor->current;
        if (g_FrontEndInput.IsRepeated(FRONT_END_INPUT_UP))
            cursor->Move(-1);
        if (g_FrontEndInput.IsRepeated(FRONT_END_INPUT_DOWN))
            cursor->Move(1);
        if (cursor->previous != cursor->current)
        {
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                FRONT_END_SOUND_MOVE, 0);
            g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
                controller->vmIds[0], 3);
            controller->MarkVmInterrupt(0, static_cast<short>(cursor->current + 7));
        }
        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            if (cursor->current == 7)
                goto quit;
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                FRONT_END_SOUND_CANCEL, 0);
            cursor->Select(7);
            controller->InterruptVmNowThree(0);
            controller->MarkVmInterrupt(0, static_cast<short>(cursor->current + 7));
        }
        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            controller->MarkVmInterrupt(0, 6);
            switch (cursor->current)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                    FRONT_END_SOUND_SELECT, 0);
                controller->MarkVmInterrupt(0x5a, 7);
                controller->MarkVmInterrupt(0x5b, 7);
                controller->SetScreenStateTarget(4);
                break;
            case 5:
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                    FRONT_END_SOUND_SELECT, 0);
                controller->MarkVmInterrupt(0x5a, 7);
                controller->MarkVmInterrupt(0x5b, 7);
                controller->SetScreenStateTarget(4);
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                    FRONT_END_SOUND_SELECT, 0);
                break;
            case 6:
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                    FRONT_END_SOUND_SELECT, 0);
                controller->SetScreenStateTarget(4);
                controller->InterruptVmNowThree(0x5a);
                controller->InterruptVmNowThree(0x5b);
                break;
            case 7:
            quit:
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                    FRONT_END_SOUND_CANCEL, 0);
                controller->SetScreenStateTarget(4);
                break;
            }
        }
        break;
    }
    case 4:
        if (controller->stateTimer.current >= 20)
        {
            FrontEndCursorView *cursor = &controller->cursor;
            switch (cursor->current)
            {
            case 0:
                g_FrontEndFlags &= ~0x10u;
                DeleteVm(controller, 0x58);
                controller->SetScreenTarget(FRONT_END_SCREEN_DIFFICULTY);
                cursor->Push();
                if (g_ReplayDifficulty >= 4)
                    g_ReplayDifficulty = 1;
                FrontEndCursorSetCurrent(g_ReplayDifficulty, cursor);
                break;
            case 1:
                g_FrontEndFlags &= ~0x10u;
                DeleteVm(controller, 0x58);
                controller->SetScreenTarget(FRONT_END_SCREEN_DIFFICULTY);
                cursor->Push();
                controller->savedDifficulty = g_ReplayDifficulty;
                g_ReplayDifficulty = 4;
                cursor->Select(0);
                break;
            case 2:
                g_FrontEndFlags |= 0x10;
                DeleteVm(controller, 0x58);
                controller->SetScreenTarget(FRONT_END_SCREEN_DIFFICULTY);
                cursor->Push();
                if (g_ReplayDifficulty >= 4)
                    g_ReplayDifficulty = 1;
                FrontEndCursorSetCurrent(g_ReplayDifficulty, cursor);
                break;
            case 3:
                DeleteVm(controller, 0x58);
                controller->SetScreenTarget(FRONT_END_SCREEN_REPLAY);
                cursor->Push();
                break;
            case 4:
                DeleteVm(controller, 0x58);
                controller->SetScreenTarget(FRONT_END_SCREEN_PRACTICE);
                cursor->Push();
                break;
            case 5:
                DeleteVm(controller, 0x58);
                controller->SetScreenTarget(FRONT_END_SCREEN_MUSIC_ROOM);
                cursor->Push();
                break;
            case 6:
                controller->SetScreenTarget(FRONT_END_SCREEN_OPTIONS);
                cursor->Push();
                break;
            case 7:
                controller->SetScreenTarget(FRONT_END_SCREEN_START_GAME);
                break;
            }
        }
        break;
    }
    return 1;
}


struct FrontEndDemoStateView
{
    unsigned char unknown000[0x28];
    int character;
    int shotType;
    unsigned char unknown030[4];
    int difficulty;
    int savedDifficulty;
    int selectedStage;
    int initialStage;
    unsigned char unknown044[0x1c];
    unsigned int flags;
    int idleFrames;
    int nextDemo;
};

typedef char FrontEndDemoStateOffsets[
    (offsetof(FrontEndDemoStateView, selectedStage) == 0x3c &&
     offsetof(FrontEndDemoStateView, flags) == 0x60 &&
     offsetof(FrontEndDemoStateView, idleFrames) == 0x64 &&
     offsetof(FrontEndDemoStateView, nextDemo) == 0x68) ? 1 : -1];

static __forceinline int FrontEndUpdateIdleDemo(FrontEndControllerView *controller)
{
    FrontEndDemoStateView &demo =
        reinterpret_cast<FrontEndDemoStateView &>(g_GameStartupState);
    if (controller->screen == FRONT_END_SCREEN_MAIN_MENU ||
        controller->screen == FRONT_END_SCREEN_MAIN_MENU_RETURN)
    {
        ++demo.idleFrames;
        if ((g_FrontEndInput.current & 0x160b) != 0)
        {
            demo.idleFrames = 0;
        }
        else if (demo.idleFrames >= 900)
        {
            demo.flags = (demo.flags & ~0x40u) |
                FRONT_END_FLAG_DEMO_TRANSITION;
            strcpy(g_FrontEndDemoPath, g_FrontEndDemoPaths[demo.nextDemo]);
            ReplayManager *replay = ReplayManager::Load(g_FrontEndDemoPath);
            demo.nextDemo = (demo.nextDemo + 1) % 4;

            int stage = 0;
            while (stage < 8 && replay->stageStates[stage].header == NULL)
                ++stage;

            g_FrontEndSelectedStageRecord = g_FrontEndStageRecords[stage];
            demo.selectedStage = stage;
            demo.initialStage = stage;
            g_MainSupervisorView.nextGameMode = 12;
            ReplayDataHeader *header = replay->replayData;
            demo.character = header->character;
            demo.shotType = header->shotType;
            demo.savedDifficulty = demo.difficulty;
            demo.difficulty = header->difficulty;
            ReplayManager::Destroy(replay);

            int screen = controller->screen;
            demo.idleFrames = 0;
            return g_MainModeTransitionCounter = screen != FRONT_END_SCREEN_MAIN_MENU;
        }
    }
    return g_MainModeTransitionCounter;
}

// TH10_FRONTEND_FUNCTION: 0x0042CDF0 FrontEndControllerView::Update
int FrontEndControllerView::Update()
{
    int mode = FrontEndUpdateIdleDemo(this);
    FrontEndDemoStateView &demo =
        reinterpret_cast<FrontEndDemoStateView &>(g_GameStartupState);
    switch (screen)
    {
    case FRONT_END_SCREEN_INITIALIZE:
    {
        FrontEndControllerView *controller = this;
        AnmRenderManagerView *manager = g_AnmRenderManagerView;

        manager->MarkLoadedVmsForDeletion(manager->loadedAnms[6]);
        manager->MarkLoadedVmsForDeletion(manager->loadedAnms[7]);
        manager->MarkLoadedVmsForDeletion(manager->loadedAnms[2]);
        manager->MarkLoadedVmsForDeletion(manager->loadedAnms[3]);
        manager->MarkLoadedVmsForDeletion(manager->loadedAnms[0]);

        int *asciiAuxiliary = &g_AsciiManagerView->auxiliaryVm89A4;
        EnemyMarkPendingInterrupt(asciiAuxiliary);
        *asciiAuxiliary = 0;

        if (mode == 3)
        {
            FrontEndCursorView *cursor = &controller->cursor;
            controller->cursor.count = 10;
            cursor->Select(0);
            cursor->Push();
            controller->SetScreenTarget(FRONT_END_SCREEN_SCORE_ENTRY);
            controller->CreateVm(0x5b);
            controller->MarkVmInterrupt(0x5b, 9);
            g_MainModeTransitionCounter = 1;
            controller->drawTask->flags |= FRONT_END_DRAW_CALLBACK_UNLOCKED;
            goto score_entry;
        }

        if ((demo.flags & FRONT_END_FLAG_DEMO_TRANSITION) == 0)
        {
            FrontEndLoadMusic(0, "bgm/th10_02.wav");
            FrontEndPlayMusic(0, 0);
            mode = g_MainModeTransitionCounter;
        }
        else
        {
            demo.difficulty = demo.savedDifficulty;
        }
        demo.flags &= ~FRONT_END_FLAG_DEMO_TRANSITION;

        if (mode == 0)
        {
            controller->SetScreenTarget(mode + 1);
            g_MainModeTransitionCounter = 1;
        }
        else if (mode == 1)
        {
            if (demo.difficulty == 4)
                controller->cursor.Select(1);
            controller->SetScreenTarget(FRONT_END_SCREEN_MAIN_MENU_RETURN);
            controller->CreateVm(0x5b);
            goto main_menu_return;
        }
        else if (mode == 2)
        {
            FrontEndCursorView *cursor = &controller->cursor;
            controller->cursor.count = 10;
            cursor->Select(3);
            cursor->Push();
            controller->SetScreenTarget(FRONT_END_SCREEN_REPLAY);
            controller->CreateVm(0x5b);
            controller->MarkVmInterrupt(0x5b, 9);
            g_MainModeTransitionCounter = 1;
            controller->drawTask->flags |= FRONT_END_DRAW_CALLBACK_UNLOCKED;
            goto replay_menu;
        }
        FrontEndUpdateMainMenu(controller);
        break;
    }
    case FRONT_END_SCREEN_MAIN_MENU:
        FrontEndUpdateMainMenu(this);
        break;
    case FRONT_END_SCREEN_MAIN_MENU_RETURN:
main_menu_return:
        drawTask->flags |= FRONT_END_DRAW_CALLBACK_UNLOCKED;
        FrontEndUpdateMainMenuReturn(this);
        break;
    case FRONT_END_SCREEN_OPTIONS:
        UpdateOptions(this);
        break;
    case FRONT_END_SCREEN_KEY_CONFIG:
        UpdateKeyConfig(this);
        break;
    case FRONT_END_SCREEN_START_GAME:
        g_MainSupervisorView.nextGameMode =
            ((~(g_MainSupervisorView.flags >> 12)) & 1) | 2;
        // Fall through to the music setup shared by the launch paths.
    case FRONT_END_SCREEN_START_GAME_10:
    case FRONT_END_SCREEN_START_GAME_13:
        GamePrepareDummyMusic();
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
    case FRONT_END_SCREEN_MUSIC_ROOM:
        FrontEndUpdateMusicRoom(this);
        break;
    case FRONT_END_SCREEN_PRACTICE:
        UpdatePractice(this);
        break;
    case FRONT_END_SCREEN_REPLAY:
replay_menu:
        UpdateReplay(this);
        break;
    case FRONT_END_SCREEN_SCORE_ENTRY:
score_entry:
        UpdateScoreEntry(this);
        break;
    case FRONT_END_SCREEN_REPLAY_SAVE:
        UpdateReplaySave(this);
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
    case FRONT_END_SCREEN_REPLAY_SAVE:
        DrawReplaySave(this);
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
        controller->cursor.Select(0);
        controller->CreateVm(FRONT_END_ROOT_VM);
        controller->RefreshOptionsDisplay();
        controller->SetScreenStateTarget(FRONT_END_OPTIONS_OPENING);
        // Initialization falls through to the opening timer check.
    case FRONT_END_OPTIONS_OPENING:
        if (controller->stateTimer.current > 6)
        {
            controller->SetScreenStateTarget(FRONT_END_OPTIONS_ACTIVE);
            RunOptionsCursor(controller);
            SetHintModeSprites(g_AnmRenderManagerView, controller);
            controller->MarkVmInterrupt(
                FRONT_END_ROOT_VM, controller->cursor.current + 0x11);
        }
        break;
    case FRONT_END_OPTIONS_ACTIVE:
    {
        FrontEndCursorView *cursor = &controller->cursor;
        cursor->previous = cursor->current;
        if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_UP)))
            cursor->Move(-1);
        if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_DOWN)))
            cursor->Move(1);
        if (cursor->previous != cursor->current)
        {
            QueueOptionsSound(FRONT_END_SOUND_MOVE);
            RunOptionsCursor(controller);
            controller->MarkVmInterrupt(
                FRONT_END_ROOT_VM, controller->cursor.current + 7);
        }
        if (g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL)
        {
            if (controller->cursor.current == 5)
            {
                controller->MarkVmInterrupt(FRONT_END_ROOT_VM, 6);
                QueueOptionsSound(FRONT_END_SOUND_CANCEL);
                controller->SetScreenStateTarget(FRONT_END_OPTIONS_CLOSING);
            }
            else
            {
                QueueOptionsSound(FRONT_END_SOUND_CANCEL);
                controller->cursor.Select(5);
                controller->InterruptVmNowThree(FRONT_END_ROOT_VM);
                controller->MarkVmInterrupt(
                    FRONT_END_ROOT_VM, controller->cursor.current + 7);
            }
        }
        else
        {
            if (controller->cursor.current == 2
                    && reinterpret_cast<const PlayerTimerView &>(controller->stateTimer)
                        .HasIntervalTick(60))
                QueueOptionsSound(FRONT_END_SOUND_PREVIEW);
            AnmRenderManagerView *manager;
            if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_LEFT)))
            {
                manager = g_AnmRenderManagerView;
                switch (controller->cursor.current)
                {
                case 0:
                    if (static_cast<unsigned char>(GetOptionsConfiguration().hintMode) > 0)
                        --GetOptionsConfiguration().hintMode;
                    else
                        GetOptionsConfiguration().hintMode = 2;
                    SetHintModeSprites(manager, controller);
                    QueueOptionsSound(FRONT_END_SOUND_MOVE);
                    break;
                case 1:
                    if (GetOptionsConfiguration().bgmVolume < 5)
                    {
                        GetOptionsConfiguration().bgmVolume = 0;
                        goto refresh_music;
                    }
                    GetOptionsConfiguration().bgmVolume += 0xFB;
                refresh_music:
                    controller->RefreshOptionsDisplay();
                    break;
                case 2:
                    if (GetOptionsConfiguration().sfxVolume < 5)
                    {
                        GetOptionsConfiguration().sfxVolume = 0;
                        goto refresh_sound;
                    }
                    GetOptionsConfiguration().sfxVolume += 0xFB;
                refresh_sound:
                    controller->RefreshOptionsDisplay();
                    break;
                }
            }
            else
            {
                manager = g_AnmRenderManagerView;
            }

            if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_RIGHT)))
            {
                switch (controller->cursor.current)
                {
                case 0:
                    if (static_cast<unsigned char>(GetOptionsConfiguration().hintMode) < 2)
                        ++GetOptionsConfiguration().hintMode;
                    else
                        GetOptionsConfiguration().hintMode = 0;
                    QueueOptionsSound(FRONT_END_SOUND_MOVE);
                    SetHintModeSprites(manager, controller);
                    break;
                case 1:
                    GetOptionsConfiguration().bgmVolume += 5;
                    if (GetOptionsConfiguration().bgmVolume > 100)
                        GetOptionsConfiguration().bgmVolume = 100;
                    controller->RefreshOptionsDisplay();
                    break;
                case 2:
                    GetOptionsConfiguration().sfxVolume += 5;
                    if (GetOptionsConfiguration().sfxVolume > 100)
                        GetOptionsConfiguration().sfxVolume = 100;
                    controller->RefreshOptionsDisplay();
                    break;
                }
            }
            if (g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM)
            {
                switch (controller->cursor.current)
                {
                case 5:
                    controller->MarkVmInterrupt(FRONT_END_ROOT_VM, 6);
                    QueueOptionsSound(FRONT_END_SOUND_CANCEL);
                    controller->SetScreenStateTarget(FRONT_END_OPTIONS_CLOSING);
                    break;
                case 4:
                    GetOptionsConfiguration().bgmVolume = 100;
                    GetOptionsConfiguration().sfxVolume = 80;
                    GetOptionsConfiguration().hintMode = 0;
                    controller->RefreshOptionsDisplay();
                    SetHintModeSprites(manager, controller);
                    QueueOptionsSound(FRONT_END_SOUND_SELECT);
                    break;
                case 3:
                    controller->MarkVmInterrupt(FRONT_END_ROOT_VM, 6);
                    QueueOptionsSound(FRONT_END_SOUND_SELECT);
                    controller->SetScreenStateTarget(FRONT_END_OPTIONS_CLOSING);
                    break;
                }
            }
        }
        break;
    }
    case FRONT_END_OPTIONS_CLOSING:
        if (controller->stateTimer.current >= 10)
        {
            FrontEndCursorView *cursor = &controller->cursor;
            switch (cursor->current)
            {
            case 3:
                controller->SetScreenTarget(FRONT_END_SCREEN_KEY_CONFIG);
                cursor->Push();
                break;
            case 5:
                controller->InterruptVmNowTwo(0x5a);
                controller->InterruptVmNowTwo(0x5b);
                controller->SetScreenTarget(FRONT_END_SCREEN_MAIN_MENU_RETURN);
                cursor->Pop();
                break;
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

    g_FrontEndSoundBgmVolume = GetOptionsConfiguration().bgmVolume;
    FrontEndQueueSoundCommand("SetVol", 8, 0);
    g_FrontEndSoundSfxVolume = GetOptionsConfiguration().sfxVolume;

    if (g_FrontEndSoundSfxVolume != 0)
    {
        volume = static_cast<float>(g_FrontEndSoundBgmVolume) / 100.0f;
        volume = 1.0f - volume;
        volume *= volume;
        volume *= volume;
        volume = 1.0f - volume;
        g_FrontEndSoundBgmAttenuation =
            -5000 - static_cast<int>(-5000.0f * volume);
    }
    else
    {
        g_FrontEndSoundBgmAttenuation = -10000;
    }

    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm;

    vm = FindOptionsChildVm(this, manager, 0x25);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().bgmVolume / 100 + 0x33);
    vm = FindOptionsChildVm(this, manager, 0x26);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().bgmVolume / 10 % 10 + 0x33);
    vm = FindOptionsChildVm(this, manager, 0x27);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().bgmVolume % 10 + 0x33);
    vm = FindOptionsChildVm(this, manager, 0x29);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().bgmVolume / 100 + 0x3d);
    vm = FindOptionsChildVm(this, manager, 0x2a);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().bgmVolume / 10 % 10 + 0x3d);
    vm = FindOptionsChildVm(this, manager, 0x2b);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().bgmVolume % 10 + 0x3d);

    vm = FindOptionsChildVm(this, manager, 0x2d);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().sfxVolume / 100 + 0x33);
    vm = FindOptionsChildVm(this, manager, 0x2e);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().sfxVolume / 10 % 10 + 0x33);
    vm = FindOptionsChildVm(this, manager, 0x2f);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().sfxVolume % 10 + 0x33);
    vm = FindOptionsChildVm(this, manager, 0x31);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().sfxVolume / 100 + 0x3d);
    vm = FindOptionsChildVm(this, manager, 0x32);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().sfxVolume / 10 % 10 + 0x3d);
    vm = FindOptionsChildVm(this, manager, 0x33);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, GetOptionsConfiguration().sfxVolume % 10 + 0x3d);

    if (GetOptionsConfiguration().bgmVolume < 10)
    {
        FindOptionsChildVm(this, manager, 0x25)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x26)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x29)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x2a)->drawEnabled = 0;
    }
    else if (GetOptionsConfiguration().bgmVolume < 100)
    {
        FindOptionsChildVm(this, manager, 0x25)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x26)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x29)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x2a)->drawEnabled = 1;
    }
    else
    {
        FindOptionsChildVm(this, manager, 0x25)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x26)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x29)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x2a)->drawEnabled = 1;
    }

    if (GetOptionsConfiguration().sfxVolume < 10)
    {
        FindOptionsChildVm(this, manager, 0x2d)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x2e)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x31)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x32)->drawEnabled = 0;
    }
    else if (GetOptionsConfiguration().sfxVolume < 100)
    {
        FindOptionsChildVm(this, manager, 0x2d)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x2e)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x31)->drawEnabled = 0;
        FindOptionsChildVm(this, manager, 0x32)->drawEnabled = 1;
    }
    else
    {
        FindOptionsChildVm(this, manager, 0x2d)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x2e)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x31)->drawEnabled = 1;
        FindOptionsChildVm(this, manager, 0x32)->drawEnabled = 1;
    }
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
        controller->CreateVm(FRONT_END_KEY_CONFIG_ROOT_VM);
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
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm;

    vm = FindKeyConfigChildVm(this, manager, 0x43);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x44);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x4d);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x4e);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SHOT] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x45);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x46);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x4f);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x50);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_BOMB] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x47);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x48);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x51);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x52);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_FOCUS] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x49);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_MENU] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x4a);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_MENU] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x53);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_MENU] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x54);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_MENU] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x4b);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x4c);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP] % 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x55);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP] / 10 + 0x33);

    vm = FindKeyConfigChildVm(this, manager, 0x56);
    if (vm != NULL)
        vm->anmFile->SetSprite(
            vm, keyConfigBindings[FRONT_END_KEY_CONFIG_SKIP] % 10 + 0x33);
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
            controller->CreateVm(0x5e);
            controller->difficultyAuxVmId =
                g_AsciiManagerView->asciiAnm->CreateVmVariant0(
                    8, FRONT_END_RENDER_LAYER);
        }

        controller->cursor.count = g_ReplayDifficulty < 4 ? 4 : 1;
        DeleteVm(controller, 0x62);
        controller->CreateVm(rootIndex);
        InterruptVmNow(controller, rootIndex, 3);
        InterruptVm(
            controller, rootIndex,
            static_cast<short>(controller->cursor.current + 0x11));
        controller->CreateVm(0x62);
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
                controller->InterruptVmNowThree(rootIndex);
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

        controller->CreateVm(0x63);
        DeleteVm(controller, 0x7d);
        controller->CreateVm(0x7d);
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
            controller->InterruptVmNowThree(0x7d);
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


AnmVmIdView ResultMenuFindScript(AnmVmIdView *id, int scriptIndex);
static __forceinline unsigned int ReadReplayMenuPressedInput();

static __forceinline void HideUnclearedShotMark(
    FrontEndControllerView *controller, int shot)
{
    const int character = g_GameScoreState.history.statisticsGroupA;
    const unsigned int record = static_cast<unsigned int>(character) * 0x329du +
        static_cast<unsigned int>(g_ReplayDifficulty);
    const int *clears = reinterpret_cast<const int *>(g_FrontEndProfileData +
        0x4d0u + static_cast<unsigned int>(shot) * 0x437cu + record * 4u);
    if (*clears == 0)
    {
        AnmVmIdView &root = controller->vmIds[character + 0x96];
        AnmVmIdView child = ResultMenuFindScript(&root, character * 3 + 0x8a + shot);
        child.ClearFlag2();
    }
}

// TH10_FRONTEND_FUNCTION: 0x00430A60 FrontEndControllerView::UpdateShotType
int __stdcall FrontEndControllerView::UpdateShotType(
    FrontEndControllerView *controller)
{
    const int &character = g_GameScoreState.history.statisticsGroupA;
    switch (controller->screenState)
    {
    case FRONT_END_SELECTION_INITIALIZE:
        controller->cursor.count = 3;
        // Sample the difficulty after publishing the number of shot choices.
        if (*static_cast<const volatile int *>(&g_ReplayDifficulty) == 4)
        {
            const unsigned char *profile = g_FrontEndProfileData;
            if (profile[character * 3 + 0x1d888] == 0)
            {
                if (controller->cursor.current == 0)
                {
                    if (profile[character * 3 + 0x1d889] != 0)
                        controller->cursor.Select(1);
                    else
                        controller->cursor.Select(2);
                }
                controller->cursor.DisableEntry(0);
            }
            if (profile[character * 3 + 0x1d889] == 0)
            {
                if (controller->cursor.current == 1)
                {
                    if (profile[character * 3 + 0x1d888] != 0)
                        controller->cursor.Select(0);
                    else
                        controller->cursor.Select(2);
                }
                controller->cursor.DisableEntry(1);
            }
            const int selectedCharacter = character;
            if (profile[selectedCharacter * 3 + 0x1d88a] == 0)
            {
                if (controller->cursor.current == 2)
                {
                    if (profile[selectedCharacter * 3 + 0x1d888] != 0)
                        controller->cursor.Select(0);
                    else
                        controller->cursor.Select(1);
                }
                controller->cursor.DisableEntry(2);
            }
        }

        controller->CreateVm(0x64);
        DeleteVm(controller, character + 0x96);
        controller->CreateVm(character + 0x96);
        g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
            static_cast<const AnmVmIdView &>(controller->vmIds[character + 0x96]), 3);
        controller->MarkVmInterrupt(character + 0x96,
            static_cast<short>(controller->cursor.current + 0x11));
        controller->SetScreenStateTarget(FRONT_END_SELECTION_OPENING);
        HideUnclearedShotMark(controller, 0);
        HideUnclearedShotMark(controller, 1);
        HideUnclearedShotMark(controller, 2);
        // The opening timer is checked on the initialization frame.

    case FRONT_END_SELECTION_OPENING:
        if (controller->stateTimer.current > 6)
            controller->SetScreenStateTarget(FRONT_END_SELECTION_ACTIVE);
        break;

    case FRONT_END_SELECTION_ACTIVE:
    {
        FrontEndCursorView *cursor = &controller->cursor;
        cursor->previous = cursor->current;
        const volatile unsigned char *pressedByte =
            reinterpret_cast<const volatile unsigned char *>(
                &g_FrontEndInput.pressed);
        if ((*pressedByte & FRONT_END_INPUT_UP) != 0 ||
            (g_FrontEndInput.repeated & FRONT_END_INPUT_UP) != 0)
            cursor->Move(-1);
        if ((*pressedByte & FRONT_END_INPUT_DOWN) != 0 ||
            (g_FrontEndInput.repeated & FRONT_END_INPUT_DOWN) != 0)
            cursor->Move(1);

        if (cursor->previous != cursor->current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            controller->InterruptVmNowThree(character + 0x96);
            controller->MarkVmInterrupt(character + 0x96,
                static_cast<short>(cursor->current + 7));
        }

        const unsigned int pressed = ReadReplayMenuPressedInput();
        if ((pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            controller->SetScreenStateTarget(FRONT_END_SELECTION_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
        }
        else if ((pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            controller->SetScreenStateTarget(FRONT_END_SELECTION_CONFIRMED);
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);
            AnmVmIdView child = ResultMenuFindScript(
                &static_cast<AnmVmIdView &>(controller->vmIds[character + 0x96]),
                cursor->current + (character * 3 + 0x3f) * 2);
            child.SetInterrupt(6);
        }
        break;
    }

    case FRONT_END_SELECTION_CONFIRMED:
        if (controller->stateTimer.current == 10)
        {
            if ((g_GameStartupState.runtimeFlags & 0x10) == 0)
            {
                g_AsciiManagerView->EnsureSelectionVm(480.0f, 392.0f);
                StageCreateTransitionEffect(0x2b, 5, 0x20, 0, 0, 0);
            }
            else
            {
                g_GameScoreState.history.statisticsGroupB = controller->cursor.current;
                controller->cursor.Push();
                DeleteVm(controller, 0x64);
                controller->SetScreenTarget(FRONT_END_SCREEN_STAGE);
            }
        }

        if (controller->stateTimer.current >= 40)
        {
            g_GameScoreState.history.statisticsGroupB = controller->cursor.current;
            controller->cursor.Push();
            if ((g_GameStartupState.runtimeFlags & 0x10) == 0)
            {
                controller->SetScreenTarget(FRONT_END_SCREEN_START_GAME);
                if (g_ReplayDifficulty < 4)
                {
                    g_FrontEndSelectedStageRecord = g_ScriptViewerRouteDescriptor +
                        sizeof(g_FrontEndStageRecords[0]);
                    g_GameScoreState.selectedStage = 1;
                    g_GameScoreState.nextStage = 1;
                }
                else
                {
                    g_FrontEndSelectedStageRecord = g_ScriptViewerRouteDescriptor +
                        7 * sizeof(g_FrontEndStageRecords[0]);
                    g_GameScoreState.selectedStage = 7;
                    g_GameScoreState.nextStage = 7;
                }
                g_MainSupervisorView.nextGameMode = 7;
                GameQueueScaledSoundCommand(6.0f);
            }
            else
            {
                DeleteVm(controller, 0x64);
                controller->SetScreenTarget(FRONT_END_SCREEN_STAGE);
            }
        }
        break;

    case FRONT_END_SELECTION_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, character + 0x96);
            DeleteVm(controller, 0x64);
            controller->SetScreenTarget(FRONT_END_SCREEN_CHARACTER);
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
        controller->CreateVm(0x6a);
        controller->CreateVm(0x6b + g_ReplayCharacter);
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
            g_AsciiManagerView->EnsureSelectionVm(480.0f, 392.0f);
            StageCreateTransitionEffect(0x2b, 5, 0x20, 0, 0, 0);
        }

        if (controller->stateTimer.current >= 40)
        {
            controller->cursor.Push();
            SetScreen(controller, FRONT_END_SCREEN_START_GAME);
            g_FrontEndSelectedStage = controller->cursor.current + 1;
            g_FrontEndSelectedStageRecord =
                g_FrontEndStageRecords[controller->cursor.current];
            g_FrontEndSelectedStageMirror = g_FrontEndSelectedStage;
            GameQueueScaledSoundCommand(6.0f);
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


static __forceinline void ReplayResetOpeningTimer(AnmVmTimerView *timer)
{
    timer->SetCurrent(0);
}

static __forceinline unsigned int ReadReplayMenuPressedInput()
{
    // Confirm and cancel share the four-byte sample beginning at pressed.
    unsigned int input;
    const unsigned char *bytes =
        reinterpret_cast<const unsigned char *>(&g_FrontEndInput);
    memcpy(&input, bytes + offsetof(FrontEndInputView, pressed), sizeof(input));
    return input;
}

// TH10_FRONTEND_FUNCTION: 0x004315C0 FrontEndControllerView::UpdateReplay
int __stdcall FrontEndControllerView::UpdateReplay(
    FrontEndControllerView *controller)
{
    char replayPath[64];
    WIN32_FIND_DATAA findData;
    switch (controller->screenState)
    {
    case FRONT_END_REPLAY_INITIALIZE:
    {
        controller->cursor.count = 25;
        FrontEndCursorSetCurrent(g_FrontEndReplayCursor, &controller->cursor);
        g_FrontEndReplayCursor = 0;

        if (g_AnmRenderManagerView->FindVm(
                controller->vmIds[0x5e]) == NULL)
        {
            controller->CreateVm(0x5e);
            AnmLoadedView &resource = *g_AsciiManagerView->asciiAnm;
            controller->difficultyAuxVmId.value = resource.CreateVmVariant0(
                8, FRONT_END_RENDER_LAYER).value;
        }
        controller->CreateVm(0x65);
        controller->SetScreenStateTarget(FRONT_END_REPLAY_OPENING);

        memset(controller->replayFiles, 0, sizeof(controller->replayFiles));
        for (int replayNumber = 1; replayNumber <= 25; ++replayNumber)
        {
            sprintf(replayPath, "th10_%.2d.rpy", replayNumber);
            controller->replayFiles[replayNumber - 1] =
                ReplayManager::Load(replayPath);
        }

        _mkdir("replay");
        _chdir("replay");

        HANDLE search = FindFirstFileA("th10_ud????.rpy", &findData);
        if (search != INVALID_HANDLE_VALUE)
        {
            for (int replayIndex = 25; replayIndex < 50; ++replayIndex)
            {
                _chdir("../");
                controller->replayFiles[replayIndex] =
                    ReplayManager::Load(findData.cFileName);
                _chdir("replay");
                if (!FindNextFileA(search, &findData))
                    break;
            }
        }

        // The original calls FindClose even when FindFirstFileA returned the
        // invalid sentinel. Preserve that observable error-path behavior.
        FindClose(search);
        _chdir("../");
        controller->replayListOffset = 0;
    }
        // The opening timer is checked on the initialization frame.

    case FRONT_END_REPLAY_OPENING:
        if (controller->stateTimer.current > 6)
        {
            controller->screenState = FRONT_END_REPLAY_SELECT_FILE;
            ReplayResetOpeningTimer(&controller->stateTimer);
        }
        break;

    case FRONT_END_REPLAY_SELECT_FILE:
    {
        FrontEndCursorView *cursor = &controller->cursor;
        cursor->previous = cursor->current;
        const volatile unsigned char *pressedByte =
            reinterpret_cast<const volatile unsigned char *>(
                &g_FrontEndInput.pressed);
        if ((*pressedByte & FRONT_END_INPUT_UP) != 0 ||
            (g_FrontEndInput.repeated & FRONT_END_INPUT_UP) != 0)
            cursor->Move(-1);
        if ((*pressedByte & FRONT_END_INPUT_DOWN) != 0 ||
            (g_FrontEndInput.repeated & FRONT_END_INPUT_DOWN) != 0)
            cursor->Move(1);

        if (cursor->previous != cursor->current)
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);

        unsigned int pressed = ReadReplayMenuPressedInput();
        if ((pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            controller->SetScreenStateTarget(FRONT_END_REPLAY_CLOSING);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
        }
        else if ((pressed & FRONT_END_INPUT_CONFIRM) != 0 &&
                 controller->replayFiles[cursor->current] != NULL)
        {
            controller->SetScreenStateTarget(FRONT_END_REPLAY_SELECT_STAGE);
            controller->selectedReplay = cursor->current;
            cursor->Push();
            FrontEndPlaySound(FRONT_END_SOUND_SELECT);

            controller->cursor.count = 7;
            cursor->Select(0);
            int stage = 0;
            do
            {
                if (controller->replayFiles[controller->selectedReplay]->
                        stageStates[stage + 1].header == NULL)
                    cursor->DisableEntry(stage);
                ++stage;
            } while (stage < 7);
            cursor->Move(-1);
            cursor->Move(1);
        }
        break;
    }

    case FRONT_END_REPLAY_SELECT_STAGE:
    {
        if (controller->stateTimer.current < 15)
            break;

        FrontEndCursorView *cursor = &controller->cursor;
        cursor->previous = cursor->current;
        if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_UP)))
            cursor->Move(-1);
        if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_DOWN)))
            cursor->Move(1);

        if (cursor->previous != cursor->current)
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);

        unsigned int pressed = ReadReplayMenuPressedInput();
        if ((pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            cursor->Pop();
            controller->cursor.count = 25;
            controller->cursor.disabledEntryCount = 0;
            controller->SetScreenStateTarget(FRONT_END_REPLAY_SELECT_FILE);
            FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
        }
        else if ((pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            controller->selectedReplayStage = cursor->current;
            controller->SetScreenStateTarget(FRONT_END_REPLAY_STARTING);
        }
        break;
    }

    case FRONT_END_REPLAY_STARTING:
        if (controller->stateTimer.current == 2)
        {
            StageCreateTransitionEffect(0x2b, 5, 0x20, 0, 0, 0);
            g_AsciiManagerView->EnsureSelectionVm(480.0f, 392.0f);
        }

        if (controller->stateTimer.current >= 32)
        {
            controller->SetScreenTarget(FRONT_END_SCREEN_START_GAME);
            int stage = controller->selectedReplayStage + 1;
            g_FrontEndSelectedStageRecord = g_ScriptViewerRouteDescriptor +
                stage * sizeof(g_FrontEndStageRecords[0]);
            g_GameScoreState.selectedStage = stage;
            g_GameScoreState.nextStage = stage;
            GameQueueScaledSoundCommand(6.0f);
            g_MainSupervisorView.nextGameMode = 12;

            strcpy(g_FrontEndDemoPath,
                controller->replayFiles[controller->selectedReplay]->replayPath);
            ReplayDataHeader *header =
                controller->replayFiles[controller->selectedReplay]->replayData;
            g_GameScoreState.history.statisticsGroupA = header->character;
            g_GameScoreState.history.statisticsGroupB = header->shotType;
            memcpy(g_GameScoreState.history.unknown30 + 4,
                &header->difficulty, sizeof(header->difficulty));
            g_MainModeTransitionCounter = 2;
            g_FrontEndReplayCursor = controller->selectedReplay;
        }
        break;

    case FRONT_END_REPLAY_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            for (int replayIndex = 0; replayIndex < 50; ++replayIndex)
                delete controller->replayFiles[replayIndex];
            memset(controller->replayFiles, 0, sizeof(controller->replayFiles));

            DeleteVm(controller, 0x65);
            controller->MarkVmInterrupt(0x5a, 8);
            controller->MarkVmInterrupt(0x5b, 8);
            DeleteVm(controller, 0x5e);
            EnemyMarkPendingInterrupt(&controller->difficultyAuxVmId.value);
            controller->SetScreenTarget(FRONT_END_SCREEN_MAIN_MENU_RETURN);
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
static __forceinline AnmVmView *FindPracticeRecordVm(
    AnmRenderManagerView *manager, int id)
{
    if (id == 0)
        return NULL;
    AnmVmLayerNodeView *node;
    for (node = manager->primaryVmListHead; node != NULL; node = node->next)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == id)
            return static_cast<AnmVmView *>(node->owner);
    }
    for (node = manager->secondaryVmListHead; node != NULL; node = node->next)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == id)
            return static_cast<AnmVmView *>(node->owner);
    }
    return NULL;
}

static __forceinline void StopPracticeRowVm(AnmVmView *vm, unsigned short interrupt)
{
    if (vm != NULL)
    {
        vm->pendingInterrupt = interrupt;
        if (vm->layerNode.previous == NULL)
        {
            for (AnmVmLayerNodeView *child = vm->layerNode.next;
                 child != NULL; child = child->next)
                static_cast<AnmVmView *>(child->owner)->pendingInterrupt = interrupt;
        }
    }
}

static __forceinline void AnimatePracticeInput(
    FrontEndControllerView *controller, int index)
{
    g_AnmRenderManagerView->SetVmPendingInterruptAndExecute(
        static_cast<const AnmVmIdView &>(controller->vmIds[index]), 2);
}

static __forceinline void SelectPracticeCursor(FrontEndCursorView &cursor, int index)
{
    if (cursor.count != 0)
        cursor.current = index >= cursor.count ? cursor.count - 1 : index;
    else
        cursor.current = index;
}

int __stdcall FrontEndControllerView::UpdatePractice(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_PRACTICE_INITIALIZE:
    {
        controller->cursor.count = 6;
        SelectPracticeCursor(controller->cursor, 0);
        controller->practiceDifficultyCursor.count = 5;
        SelectPracticeCursor(controller->practiceDifficultyCursor, 1);
        controller->practiceDifficultyCursor.wraps = 1;

        controller->practicePageCursor.count =
            (CountPracticeRecordsForDifficulty(
                controller->practiceDifficultyCursor.current) + 9) / 10 + 1;
        SelectPracticeCursor(controller->practicePageCursor, 0);
        FrontEndCursorView *page = &controller->practicePageCursor;
        page->wraps = 1;

        if (g_AnmRenderManagerView->FindVm(
                controller->vmIds[0x5e]) == NULL)
        {
            controller->CreateVm(0x5e);
            AnmLoadedView &resource = *g_AsciiManagerView->asciiAnm;
            controller->difficultyAuxVmId = resource.CreateVmVariant0(
                8, FRONT_END_RENDER_LAYER);
        }

        controller->CreateVm(0x66);
        controller->SetScreenStateTarget(FRONT_END_PRACTICE_OPENING);
        controller->CreateVm(0x98 + controller->cursor.current / 3);
        controller->CreateVm(0x9a + controller->cursor.current);
        controller->CreateVm(0xa0 + controller->practiceDifficultyCursor.current);
        controller->CreateVm(0xa8);
        controller->CreateVm(0xa9);
        controller->CreateVm(0xaa);
        controller->CreateVm(0xab);
        controller->CreateVm(0xa5);
        controller->CreateVm(0xa6);
        controller->CreateVm(0xa7);
        controller->CreateVm(0xac);
    }
        // Fall through: creation callbacks can advance the opening timer.

    case FRONT_END_PRACTICE_OPENING:
        if (controller->stateTimer.current > 6)
            controller->SetScreenStateTarget(FRONT_END_PRACTICE_ACTIVE);
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
            AnimatePracticeInput(controller, 0xaa);
        }
        if (InputRepeated(FRONT_END_INPUT_DOWN))
        {
            difficulty->Move(1);
            AnimatePracticeInput(controller, 0xab);
        }

        if (difficulty->previous != difficulty->current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            DeleteVm(controller, 0xa0 + controller->practiceDifficultyCursor.previous);
            controller->CreateVm(0xa0 + difficulty->current);
            if (page->current > 0)
            {
                SelectPracticeCursor(*page, 1);
                controller->RefreshPracticeRecords();
            }
            page->count =
                (CountPracticeRecordsForDifficulty(difficulty->current) + 9) /
                    10 + 1;
        }

        if (InputRepeated(FRONT_END_INPUT_LEFT))
        {
            controller->cursor.Move(-1);
            AnimatePracticeInput(controller, 0xa8);
        }
        if (InputRepeated(FRONT_END_INPUT_RIGHT))
        {
            controller->cursor.Move(1);
            AnimatePracticeInput(controller, 0xa9);
        }

        if (controller->cursor.previous != controller->cursor.current)
        {
            FrontEndPlaySound(FRONT_END_SOUND_MOVE);
            if (controller->cursor.current / 3 !=
                controller->cursor.previous / 3)
            {
                DeleteVm(
                    controller, 0x98 + controller->cursor.previous / 3);
                controller->CreateVm(0x98 + controller->cursor.current / 3);
            }
            DeleteVm(controller, 0x9a + controller->cursor.previous);
            controller->CreateVm(0x9a + controller->cursor.current);
            if (page->current > 0)
                controller->RefreshPracticeRecords();
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            if (page->current == 0)
            {
                int row = 0;
                do
                {
                    AnmLoadedView *resource = g_AsciiManagerView->textAnm;
                    controller->practiceRowVmIds[row] = resource->CreateVmVariant0(
                        row + 0x17, FRONT_END_RENDER_LAYER);
                    ++row;
                } while (row < FRONT_END_PRACTICE_ROWS_PER_PAGE);
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
            controller->SetScreenStateTarget(FRONT_END_PRACTICE_CLOSING);
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
            FrontEndVmIdStorageView *row = controller->practiceRowVmIds;
            for (int remaining = FRONT_END_PRACTICE_ROWS_PER_PAGE; remaining != 0; --remaining, ++row)
                StopPracticeRowVm(FindPracticeRecordVm(g_AnmRenderManagerView, row->value), 1);
        }
        break;
    }

    case FRONT_END_PRACTICE_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x66);
            controller->MarkVmInterrupt(0x5a, 8);
            controller->MarkVmInterrupt(0x5b, 8);
            DeleteVm(controller, 0x5e);
            EnemyMarkPendingInterrupt(&controller->difficultyAuxVmId.value);
            controller->SetScreenTarget(FRONT_END_SCREEN_MAIN_MENU_RETURN);
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

    unsigned char *shotProfile = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
        static_cast<unsigned int>(cursor.current) * 0x437cu);
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


static __forceinline void SelectScoreEntryRow(FrontEndCursorView &cursor, int index)
{
    if (cursor.count != 0)
        cursor.current = index >= cursor.count ? cursor.count - 1 :
            (index < 0 ? 0 : index);
    else
        cursor.current = index;
}

static __forceinline void WriteScoreEntryCharacter(
    FrontEndControllerView &controller, const char &character, int alphabetLength)
{
    if (controller.scoreEntryNameCursor < FRONT_END_SCORE_ENTRY_NAME_LENGTH)
    {
        controller.scoreEntryName[controller.scoreEntryNameCursor] = character;
        ++controller.scoreEntryNameCursor;
        if (controller.scoreEntryNameCursor >= FRONT_END_SCORE_ENTRY_NAME_LENGTH)
            FrontEndCursorSetCurrent(alphabetLength - 1,
                &controller.scoreEntryKeyboardCursor);
    }
    else
    {
        controller.scoreEntryName[controller.scoreEntryNameCursor - 1] = character;
    }
}

static __forceinline void EraseScoreEntryCharacter(FrontEndControllerView &controller)
{
    int cursor = controller.scoreEntryNameCursor - 1;
    controller.scoreEntryNameCursor = cursor;
    controller.scoreEntryName[cursor] = ' ';
}

struct ScoreEntryShotView
{
    unsigned char prefix[8];
    FrontEndScoreTableView scores;
    unsigned char remaining[0x437c - 8 - sizeof(FrontEndScoreTableView)];
};

struct ScoreEntryProfileView
{
    ScoreEntryShotView shots[2][3];
    unsigned char remaining[0x1d878 - 6 * sizeof(ScoreEntryShotView)];
    char lastName[9];
};

typedef char ScoreEntryProfileOffsets[
    (sizeof(ScoreEntryShotView) == 0x437c &&
     offsetof(ScoreEntryShotView, scores.records) == 0x18 &&
     offsetof(ScoreEntryProfileView, lastName) == 0x1d878) ? 1 : -1];

static __forceinline FrontEndScoreTableView &GetCurrentScoreEntryTable()
{
    ScoreEntryProfileView *profile =
        reinterpret_cast<ScoreEntryProfileView *>(g_FrontEndProfileData);
    return profile->shots[g_ReplayCharacter][g_ReplayShotType].scores;
}

static __forceinline int InsertCompletedRunScore()
{
    const int stageCount = 8;
    g_FrontEndSelectedStage = stageCount;
    g_FrontEndSelectedStageMirror = stageCount;
    g_FrontEndScoreFormatTable = g_ScriptViewerRouteDescriptor +
        stageCount * sizeof(g_FrontEndStageRecords[0]);
    int rank = FrontEndInsertScore(&GetCurrentScoreEntryTable());
    g_FrontEndScoreFormatTable = g_ScriptViewerRouteDescriptor;
    g_FrontEndSelectedStage = 0;
    g_FrontEndSelectedStageMirror = 0;
    return rank;
}

struct ScoreEntryScreen
{
    FrontEndControllerView *const &controller;
};

static __forceinline int UpdateScoreEntryScreen(
    const ScoreEntryScreen &screen, AnmVmIdView &backdrop)
{
    FrontEndControllerView *const &controller = screen.controller;
    switch (controller->screenState)
    {
    case FRONT_END_SCORE_ENTRY_INITIALIZE:
    {
        controller->cursor.count = 30;
        FrontEndLoadMusic(0, "bgm/th10_17.wav");
        FrontEndPlayMusic(0, 17);

        if (g_AnmRenderManagerView->FindVm(
                controller->vmIds[0x5e]) == NULL)
        {
            controller->CreateVm(0x5e);
            AnmLoadedView &resource = *g_AsciiManagerView->asciiAnm;
            backdrop = resource.CreateVmVariant0(8, FRONT_END_RENDER_LAYER);
            controller->difficultyAuxVmId = backdrop;
        }

        controller->CreateVm(0x68);
        controller->SetScreenStateTarget(FRONT_END_SCORE_ENTRY_OPENING);
        controller->CreateVm(0x98 + g_ReplayCharacter);
        controller->CreateVm(0x9a + g_ReplayCharacter * 3 + g_ReplayShotType);
        controller->CreateVm(0xa0 + g_ReplayDifficulty);

        int insertedRow = InsertCompletedRunScore();

        if (insertedRow >= 0)
        {
            controller->stateTimer.SetCurrent(0);
            controller->cursor.wraps = 1;
            FrontEndCursorSetCurrent(insertedRow, &controller->cursor);

            FrontEndCursorView *keyboard = &controller->scoreEntryKeyboardCursor;
            SelectScoreEntryRow(*keyboard, 0);
            controller->scoreEntryKeyboardCursor.count =
                strlen(g_FrontEndScoreEntryAlphabet);
            controller->scoreEntryKeyboardCursor.wraps = 1;

            const ScoreEntryProfileView *profile =
                reinterpret_cast<ScoreEntryProfileView *>(g_FrontEndProfileData);
            char *editingName = controller->scoreEntryName;
            const char *lastName = profile->lastName;
            const char *character = lastName;
            char letter;
            do
            {
                letter = *character;
                editingName[character - lastName] = letter;
                ++character;
            } while (letter != '\0');
            if (strcmp(editingName, "        ") != 0)
                keyboard->Move(-1);

            int cursor;
            for (cursor = FRONT_END_SCORE_ENTRY_NAME_LENGTH; cursor > 0; --cursor)
            {
                if (controller->scoreEntryName[cursor - 1] != ' ')
                    break;
            }
            controller->scoreEntryNameCursor = cursor;
            controller->scoreEntryUnavailable = 0;
        }
        else
        {
            SelectScoreEntryRow(controller->cursor, -1);
            controller->scoreEntryUnavailable = 1;
        }
    }
    // The opening timer is checked on the initialization frame.
    case FRONT_END_SCORE_ENTRY_OPENING:
        if (controller->stateTimer.current > 6)
            controller->SetScreenStateTarget(FRONT_END_SCORE_ENTRY_ACTIVE);
        break;

    case FRONT_END_SCORE_ENTRY_ACTIVE:
    {
        if (controller->scoreEntryUnavailable == 0)
        {
            FrontEndInputView *input = &g_FrontEndInput;
            FrontEndCursorView *keyboard = &controller->scoreEntryKeyboardCursor;
            keyboard->previous = keyboard->current;
            if (static_cast<short>(input->IsRepeated(FRONT_END_INPUT_UP)))
                keyboard->Move(-FRONT_END_SCORE_ENTRY_COLUMNS);
            if (static_cast<short>(input->IsRepeated(FRONT_END_INPUT_DOWN)))
                keyboard->Move(FRONT_END_SCORE_ENTRY_COLUMNS);
            if (static_cast<short>(input->IsRepeated(FRONT_END_INPUT_LEFT)))
            {
                if (keyboard->current % FRONT_END_SCORE_ENTRY_COLUMNS != 0)
                    keyboard->Move(-1);
                else
                    keyboard->Move(FRONT_END_SCORE_ENTRY_COLUMNS - 1);
            }
            if (static_cast<short>(input->IsRepeated(FRONT_END_INPUT_RIGHT)))
            {
                if (keyboard->current % FRONT_END_SCORE_ENTRY_COLUMNS !=
                    FRONT_END_SCORE_ENTRY_COLUMNS - 1)
                    keyboard->Move(1);
                else
                    keyboard->Move(1 - FRONT_END_SCORE_ENTRY_COLUMNS);
            }
            if (keyboard->previous != keyboard->current)
                FrontEndPlaySound(FRONT_END_SOUND_MOVE);
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            if (controller->scoreEntryUnavailable == 0)
            {
                const char *alphabet = g_FrontEndScoreEntryAlphabet;
                int alphabetLength = strlen(alphabet);
                int key = controller->scoreEntryKeyboardCursor.current;
                FrontEndControllerView &editor = *controller;
                if (key < alphabetLength - 3)
                {
                    WriteScoreEntryCharacter(
                        editor, alphabet[key], alphabetLength);
                    FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                }
                else if (key == alphabetLength - 3)
                {
                    WriteScoreEntryCharacter(editor, ' ', alphabetLength);
                    FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                }
                else if (key == alphabetLength - 2)
                {
                    if (controller->scoreEntryNameCursor == 0)
                        break;
                    EraseScoreEntryCharacter(*controller);
                    FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                }
                else if (key == alphabetLength - 1)
                {
                    const char *name = controller->scoreEntryName;
                    FrontEndScoreRecordView &record =
                        GetCurrentScoreEntryTable().records[g_ReplayDifficulty]
                            [controller->cursor.current];
                    strcpy(record.name, name);
                    ScoreEntryProfileView *profile =
                        reinterpret_cast<ScoreEntryProfileView *>(g_FrontEndProfileData);
                    char *lastName = profile->lastName;
                    const char *character = name;
                    char letter;
                    do
                    {
                        letter = *character;
                        lastName[character - name] = letter;
                        ++character;
                    } while (letter != '\0');
                    controller->SetScreenStateTarget(FRONT_END_SCORE_ENTRY_CLOSING);
                    FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                }
                else
                {
                    FrontEndPlaySound(FRONT_END_SOUND_SELECT);
                }
            }
            else
            {
                controller->SetScreenStateTarget(FRONT_END_SCORE_ENTRY_CLOSING);
                FrontEndPlaySound(FRONT_END_SOUND_SELECT);
            }
        }

        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            if (controller->scoreEntryUnavailable == 0)
            {
                if (controller->scoreEntryNameCursor == 0)
                    break;
                FrontEndPlaySound(FRONT_END_SOUND_CANCEL);
                EraseScoreEntryCharacter(*controller);
            }
            else
            {
                controller->SetScreenStateTarget(FRONT_END_SCORE_ENTRY_CLOSING);
                FrontEndPlaySound(FRONT_END_SOUND_SELECT);
            }
        }
        break;
    }

    case FRONT_END_SCORE_ENTRY_CLOSING:
        if (controller->stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x68);
            DeleteVm(controller, 0x98 + g_ReplayCharacter);
            const volatile int &character = g_ReplayCharacter;
            DeleteVm(controller, 0x9a + character * 3 + g_ReplayShotType);
            DeleteVm(controller, 0xa0 + g_ReplayDifficulty);
            controller->SetScreenTarget(FRONT_END_SCREEN_REPLAY_SAVE);
        }
        break;
    }
    return 1;
}

// TH10_FRONTEND_FUNCTION: 0x00432CB0 FrontEndControllerView::UpdateScoreEntry
int __stdcall FrontEndControllerView::UpdateScoreEntry(
    FrontEndControllerView *controller)
{
    ScoreEntryScreen screen = { controller };
    AnmVmIdView backdrop;
    return UpdateScoreEntryScreen(screen, backdrop);
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

// TH10_FRONTEND_FUNCTION: 0x00433570 FrontEndControllerView::UpdateReplaySave
int __stdcall FrontEndControllerView::UpdateReplaySave(
    FrontEndControllerView *controller)
{
    FrontEndControllerView &screen = *controller;
    char replayPath[64];
    switch (screen.screenState)
    {
    case FRONT_END_REPLAY_SAVE_INITIALIZE:
        screen.cursor.count = 25;
        screen.cursor.wraps = 1;
        screen.cursor.Select(0);
        g_FrontEndSelectedStageRecord = g_ScriptViewerRouteDescriptor +
            8 * sizeof(g_FrontEndStageRecords[0]);
        g_GameScoreState.selectedStage = 8;
        g_GameScoreState.nextStage = 8;
        for (int replayNumber = 1; replayNumber <= 25; ++replayNumber)
        {
            sprintf(replayPath, "th10_%.2d.rpy", replayNumber);
            screen.replayFiles[replayNumber - 1] =
                ReplayManager::Load(replayPath);
        }
        screen.CreateVm(0x69);
        screen.SetScreenStateTarget(FRONT_END_REPLAY_SAVE_OPENING);
        // The opening timer is tested on the initialization frame too.
    case FRONT_END_REPLAY_SAVE_OPENING:
    {
        const bool openingDelayDone = screen.stateTimer.current > 6;
        if (openingDelayDone)
            screen.SetScreenStateTarget(FRONT_END_REPLAY_SAVE_SELECT_SLOT);
        break;
    }

    case FRONT_END_REPLAY_SAVE_SELECT_SLOT:
    {
        FrontEndCursorView *cursor = &screen.cursor;
        cursor->previous = cursor->current;
        const volatile unsigned char *pressedByte =
            reinterpret_cast<const volatile unsigned char *>(
                &g_FrontEndInput.pressed);
        const bool slotUp = (*pressedByte & FRONT_END_INPUT_UP) != 0 ||
            (g_FrontEndInput.repeated & FRONT_END_INPUT_UP) != 0;
        if (slotUp)
            cursor->Move(-1);
        const bool slotDown = (*pressedByte & FRONT_END_INPUT_DOWN) != 0 ||
            (g_FrontEndInput.repeated & FRONT_END_INPUT_DOWN) != 0;
        if (slotDown)
            cursor->Move(1);
        const bool slotChanged = cursor->previous != cursor->current;
        if (slotChanged)
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                FRONT_END_SOUND_MOVE, 0);

        const unsigned int pressed = ReadReplayMenuPressedInput();
        const bool cancelSlot = (pressed & FRONT_END_INPUT_CANCEL) != 0;
        if (cancelSlot)
        {
            screen.SetScreenStateTarget(FRONT_END_REPLAY_SAVE_CLOSING);
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                FRONT_END_SOUND_CANCEL, 0);
        }
        else if ((pressed & FRONT_END_INPUT_CONFIRM) != 0)
        {
            FrontEndCursorView *keyboard = &screen.scoreEntryKeyboardCursor;
            screen.selectedReplay = cursor->current;
            keyboard->Select(0);
            screen.scoreEntryKeyboardCursor.count = strlen(g_FrontEndScoreEntryAlphabet);
            screen.scoreEntryKeyboardCursor.wraps = 1;
            ReplayDataHeader *&header = g_ReplayManager->replayData;
            time(reinterpret_cast<time_t *>(&header->timestamp));
            header->completionState = 8;
            strcpy(screen.scoreEntryName,
                reinterpret_cast<char *>(g_FrontEndProfileData + 0x1d878));
            screen.scoreEntryNameCursor = 0;
            const bool hasPreviousName = strcmp(screen.scoreEntryName, "        ") != 0;
            if (hasPreviousName)
                keyboard->Move(-1);
            int length;
            for (length = FRONT_END_SCORE_ENTRY_NAME_LENGTH; length > 0; --length)
            {
                const bool hasNameCharacter = screen.scoreEntryName[length - 1] != ' ';
                if (hasNameCharacter)
                    break;
            }
            screen.scoreEntryNameCursor = length;
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                FRONT_END_SOUND_SELECT, 0);
            screen.SetScreenStateTarget(FRONT_END_REPLAY_SAVE_ENTER_NAME);
        }
        break;
    }

    case FRONT_END_REPLAY_SAVE_ENTER_NAME:
    {
        FrontEndCursorView *keyboard = &screen.scoreEntryKeyboardCursor;
        keyboard->previous = keyboard->current;
        const bool gridUp = g_FrontEndInput.IsRepeated(FRONT_END_INPUT_UP) != 0;
        if (gridUp)
            keyboard->Move(-FRONT_END_SCORE_ENTRY_COLUMNS);
        const bool gridDown = g_FrontEndInput.IsRepeated(FRONT_END_INPUT_DOWN) != 0;
        if (gridDown)
            keyboard->Move(FRONT_END_SCORE_ENTRY_COLUMNS);
        const bool gridLeft = g_FrontEndInput.IsRepeated(FRONT_END_INPUT_LEFT) != 0;
        if (gridLeft)
        {
            const bool canMoveLeft = keyboard->current % FRONT_END_SCORE_ENTRY_COLUMNS != 0;
            if (canMoveLeft)
                keyboard->Move(-1);
            else
                keyboard->Move(FRONT_END_SCORE_ENTRY_COLUMNS - 1);
        }
        const bool gridRight = g_FrontEndInput.IsRepeated(FRONT_END_INPUT_RIGHT) != 0;
        if (gridRight)
        {
            const bool canMoveRight = keyboard->current % FRONT_END_SCORE_ENTRY_COLUMNS !=
                FRONT_END_SCORE_ENTRY_COLUMNS - 1;
            if (canMoveRight)
                keyboard->Move(1);
            else
                keyboard->Move(1 - FRONT_END_SCORE_ENTRY_COLUMNS);
        }
        const bool gridChanged = keyboard->previous != keyboard->current;
        if (gridChanged)
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                FRONT_END_SOUND_MOVE, 0);

        const bool confirmCharacter = (g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM) != 0;
        if (confirmCharacter)
        {
            const char *alphabet = g_FrontEndScoreEntryAlphabet;
            int alphabetLength = strlen(alphabet);
            int key = keyboard->current;
            const bool isCharacter = key < alphabetLength - 3;
            if (isCharacter)
            {
                int cursor = screen.scoreEntryNameCursor;
                if (cursor < FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                {
                    screen.scoreEntryName[cursor] = alphabet[key];
                    ++screen.scoreEntryNameCursor;
                    if (screen.scoreEntryNameCursor >= FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                        FrontEndCursorSetCurrent(alphabetLength - 1, keyboard);
                }
                else
                    screen.scoreEntryName[cursor - 1] = alphabet[key];
            }
            else if (key == alphabetLength - 3)
            {
                int cursor = screen.scoreEntryNameCursor;
                if (cursor < FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                {
                    screen.scoreEntryName[cursor] = ' ';
                    ++screen.scoreEntryNameCursor;
                    if (screen.scoreEntryNameCursor >= FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                        FrontEndCursorSetCurrent(alphabetLength - 1, keyboard);
                }
                else
                    screen.scoreEntryName[cursor - 1] = ' ';
            }
            else if (key == alphabetLength - 2)
            {
                if (screen.scoreEntryNameCursor == 0)
                    break;
                --screen.scoreEntryNameCursor;
                screen.scoreEntryName[screen.scoreEntryNameCursor] = ' ';
            }
            else if (key == alphabetLength - 1)
            {
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                    FRONT_END_SOUND_UNLOCK, 0);
                sprintf(replayPath, "th10_%.2d.rpy", screen.cursor.current + 1);
                ReplayManager::Destroy(screen.replayFiles[screen.cursor.current]);
                g_ReplayManager->SaveReplay(replayPath, screen.scoreEntryName);
                // Saving may change the slot; loading keeps this sampled slot.
                int slot = screen.cursor.current;
                screen.replayFiles[slot] = ReplayManager::Load(replayPath);
                strcpy(reinterpret_cast<char *>(g_FrontEndProfileData + 0x1d878),
                    screen.scoreEntryName);
                screen.SetScreenStateTarget(FRONT_END_REPLAY_SAVE_SELECT_SLOT);
            }
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                FRONT_END_SOUND_SELECT, 0);
        }
        if ((g_FrontEndInput.pressed & FRONT_END_INPUT_CANCEL) != 0)
        {
            if (screen.scoreEntryNameCursor == 0)
                screen.SetScreenStateTarget(FRONT_END_REPLAY_SAVE_SELECT_SLOT);
            else
            {
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(
                    FRONT_END_SOUND_CANCEL, 0);
                --screen.scoreEntryNameCursor;
                screen.scoreEntryName[screen.scoreEntryNameCursor] = ' ';
            }
        }
        break;
    }

    case FRONT_END_REPLAY_SAVE_CLOSING:
        if (screen.stateTimer.current >= 6)
        {
            DeleteVm(controller, 0x69);
            screen.MarkVmInterrupt(0x5a, 8);
            screen.MarkVmInterrupt(0x5b, 8);
            DeleteVm(controller, 0x5e);
            EnemyMarkPendingInterrupt(&screen.difficultyAuxVmId.value);
            screen.SetScreenTarget(FRONT_END_SCREEN_MAIN_MENU_RETURN);
            screen.cursor.Pop();
            ReplayManager *recorded = g_ReplayManager;
            if (recorded != NULL)
                delete recorded;
            FrontEndLoadMusic(0, "bgm/th10_02.wav");
            FrontEndPlayMusic(0, 0);
            ReplayManager **files = screen.replayFiles;
            ReplayManager **slot = files;
            for (int remaining = 25; remaining != 0; --remaining, ++slot)
            {
                ReplayManager *replay = *slot;
                if (replay != NULL)
                    delete replay;
            }
            memset(files, 0, sizeof(screen.replayFiles));
        }
        break;
    }
    return 1;
}



// TH10_FRONTEND_FUNCTION: 0x00433B30 FrontEndControllerView::DrawReplaySave
int __stdcall FrontEndControllerView::DrawReplaySave(
    FrontEndControllerView *controller)
{
    switch (controller->screenState)
    {
    case FRONT_END_REPLAY_SAVE_ENTER_NAME:
    {
        AnmFloat3View position(58.0f, 240.0f, 0.0f);
        int slot = controller->selectedReplay;
        if (controller->stateTimer.current < 10)
            position.y = (10.0f - controller->stateTimer.subframe) *
                ((slot * 15 + 80) - 240.0f) * 0.1f + 240.0f;
        ReplayDataHeader *data = g_ReplayManager->replayData;
        tm *date = localtime(reinterpret_cast<time_t *>(&data->timestamp));
        g_AsciiManagerView->AddFormatText(
            &position, "No.%.2d %s %.2d/%.2d/%.2d %.2d:%.2d %s %s %s %2.1f%%",
            slot + 1, "        ", date->tm_year % 100, date->tm_mon + 1,
            date->tm_mday, date->tm_hour, date->tm_min,
            g_ReplayCharacterNames[data->character * 3 + data->shotType],
            g_ReplayDifficultyNames[data->difficulty],
            g_ReplayCompletionNames[8], data->slowRate);
        if (controller->stateTimer.current >= 10)
        {
            int alphabetLength = strlen(g_FrontEndScoreEntryAlphabet);
            position.x = 112.0f;
            position.y = 240.0f;
            position.z = 0.0f;
            g_AsciiManagerView->color = 0xffffffffu;
            g_AsciiManagerView->AddFormatText(&position, "%s", controller->scoreEntryName);
            position.x = controller->scoreEntryNameCursor * 9 + 112.0f;
            if (controller->scoreEntryNameCursor == FRONT_END_SCORE_ENTRY_NAME_LENGTH)
                position.x -= 9.0f;
            g_AsciiManagerView->color = 0xffffff00u;
            g_AsciiManagerView->AddFormatText(&position, "_");
            g_AsciiManagerView->color = 0xffffffffu;
            position.x = 212.0f;
            position.y = 360.0f;
            position.z = 0.0f;
            for (int key = 0; key < alphabetLength; ++key)
            {
                g_AsciiManagerView->color = controller->scoreEntryKeyboardCursor.current == key
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
                g_AsciiManagerView->AddFormatText(&position, "%c", glyph);
                if (static_cast<unsigned int>(key) % FRONT_END_SCORE_ENTRY_COLUMNS ==
                    FRONT_END_SCORE_ENTRY_COLUMNS - 1)
                {
                    position.x = 212.0f;
                    position.y += 16.0f;
                }
                else
                    position.x += 18.0f;
            }
            g_AsciiManagerView->color = 0xffffffffu;
        }
        break;
    }

    case FRONT_END_REPLAY_SAVE_SELECT_SLOT:
    {
        AnmFloat3View position(58.0f, 80.0f, 0.0f);
        g_AsciiManagerView->drawShadow = 1;
        for (int slot = 0; slot < 25; ++slot)
        {
            g_AsciiManagerView->color = controller->cursor.current == slot
                ? 0xffffff00u : 0xff808080u;
            ReplayManager *replay = controller->replayFiles[slot];
            if (replay != NULL)
            {
                ReplayDataHeader *data = replay->replayData;
                tm *date = localtime(reinterpret_cast<time_t *>(&data->timestamp));
                g_AsciiManagerView->AddFormatText(
                    &position, "No.%.2d %s %.2d/%.2d/%.2d %.2d:%.2d %s %s %s %2.1f%%",
                    slot + 1, data->replayName, date->tm_year % 100, date->tm_mon + 1,
                    date->tm_mday, date->tm_hour, date->tm_min,
                    g_ReplayCharacterNames[data->character * 3 + data->shotType],
                    g_ReplayDifficultyNames[data->difficulty],
                    g_ReplayCompletionNames[data->completionState], data->slowRate);
            }
            else
                g_AsciiManagerView->AddFormatText(
                    &position, "No.%.2d -------- --/--/-- --:-- ------- ------- --- ---%%", slot + 1);
            position.y += 15.0f;
        }
        break;
    }
    default:
        return 1;
    }
    g_AsciiManagerView->color = 0xffffffffu;
    g_AsciiManagerView->drawShadow = 0;
    return 1;
}


// Original Japanese CP932 text, preserved as byte-valued C++ string literals.
extern const char g_MusicLockedTitleFormat[] = "No.%d  \201H\201H\201H\201H\201H\201H\201H\201H\201H\201H\201H";
const char *g_MusicWarningLines[8] = {
    "\201@",
    "\201@\201@\201\226\201\226\221I\221\360\202\265\202\275\213\310\202\315\202\334\202\276\203Q\201[\203\200\222\206\202\305\215\304\220\266\202\263\202\352\202\304\202\242\202\334\202\271\202\361\201\226\201\226",
    " ",
    "\201@\201@\201@\201@\213\310\202\314\203R\203\201\203\223\203g\202\252\203l\203^\203o\203\214\202\311\202\310\202\351\213\260\202\352\202\252\202\240\202\350\202\334\202\267\201B",
    "\201@\201@\201@\201@\201@\201@\201@\201@\201@\202\273\202\352\202\305\202\340\215\304\220\266\202\265\202\334\202\267\202\251\201H",
    "\201@",
    "\201@\201@\201@\215\304\220\266\202\265\202\275\202\242\225\373\202\315\202\340\202\244\210\352\223x\214\210\222\350\203{\203^\203\223\202\360\211\237\202\265\202\304\202\255\202\276\202\263\202\242",
    "\201@\201@\201@\215\304\220\266\202\265\202\275\202\255\202\310\202\242\225\373\202\315\201A\203J\201[\203\134\203\213\202\360\210\332\223\256\202\265\202\304\202\255\202\276\202\263\202\242",
};

extern AnmLoadedView *g_FrontEndMusicCommentAnm; // target 0x00491FEC

// These parsers deliberately retain the target's read-before-count check,
// destructive delimiter replacement and unbounded copy. They are not safe
// general-purpose text parsers. Only synthetic tests supply sentinel padding;
// production OpenFile mode 0 allocates exactly size bytes, so padding is unproved.
static char *FrontEndMusicSkipLine(char *cursor, int *remaining)
{
    while (*cursor != '\n' && *cursor != '\r')
    {
        if (*remaining == 0)
            return cursor;
        ++cursor;
        --*remaining;
    }
    if (*remaining != 0)
    {
        while ((*cursor == '\n' || *cursor == '\r') && *remaining != 0)
        {
            ++cursor;
            --*remaining;
        }
    }
    return cursor;
}

static char *__stdcall FrontEndMusicCopyLine(
    char *destination, char *cursor, int *remaining)
{
    char *end = cursor;
    while (*end != '\n' && *end != '\r')
    {
        if (*remaining == 0)
            return end;
        ++end;
        --*remaining;
    }
    if (*remaining != 0)
    {
        *end = '\0';
        strcpy(destination, cursor);
        ++end;
        --*remaining;
        while ((*end == '\n' || *end == '\r') && *remaining != 0)
        {
            ++end;
            --*remaining;
        }
    }
    return end;
}

// Target 0x00434A80, used by both music-room row animation sites.
__declspec(noinline) void __stdcall StartMusicRowInterpolation(
    AnmVmView *vm, int duration, const AnmFloat3View *initial,
    const AnmFloat3View *final, unsigned char mode)
{
    vm->positionInterpolation.duration = duration;
    vm->positionInterpolation.initialTangent = g_AnmZeroVector491C14;
    vm->positionInterpolation.finalTangent = g_AnmZeroVector491C14;
    vm->positionInterpolation.mode = mode;
    vm->positionInterpolation.initial = *initial;
    vm->positionInterpolation.final = *final;
    vm->positionInterpolation.timer.SetCurrent(0);
}

// Target 0x00420A90. The original assumes a dot and a bounded path; it
// overwrites three extension bytes without truncating a longer extension.
int __stdcall FrontEndLoadMusic(int slot, const char *path)
{
    char name[256];
    strcpy(name, path);
    char *extension = strrchr(name, '.');
    extension[1] = 'w';
    extension[2] = 'a';
    extension[3] = 'v';
    FrontEndQueueSoundCommand(name, 1, slot);
    return 1;
}

// Target 0x00420B10. Playback is queued; profile publication is immediate
// even if the command queue has no free slot. The second argument is a track.
int __stdcall FrontEndPlayMusic(int slot, int trackIndex)
{
    if ((g_FrontEndSupervisorFlags & 0x10) != 0)
        FrontEndQueueSoundCommand("dummy", 4, 0);
    FrontEndQueueSoundCommand("dummy", 2, slot);
    *reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
        0x1d892u + static_cast<unsigned int>(trackIndex)) = 1;
    return 0;
}

// Target 0x00420C30..0x00420C83. Zero and ordered speed > 1 bypass
// scaling; unordered speed reaches division. Compute the selected double value
// in its branch, retaining the quotient until the integer conversion.
int __stdcall GameQueueScaledSoundCommand(float magnitude)
{
    double value;
    if (g_AnmGameSpeed == 0.0f)
        value = magnitude;
    else if (g_AnmGameSpeed > 1.0f)
        value = magnitude;
    else
        value = static_cast<double>(magnitude) / g_AnmGameSpeed;
    FrontEndQueueSoundCommand("", 5, static_cast<int>(value));
    return 0;
}

// Target 0x0042C770 marks pending interrupts only; ANM execution is deferred.
// Other historical InterruptVmNow callers have not been audited in this batch.
void FrontEndControllerView::MarkVmInterrupt(int index, short interrupt)
{
    AnmVmIdView id(vmIds[index].value);
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(id);
    if (vm != NULL)
    {
        vm->pendingInterrupt = interrupt;
        if (vm->layerNode.previous == NULL)
        {
            AnmVmLayerNodeView *node = vm->layerNode.next;
            while (node != NULL)
            {
                static_cast<AnmVmView *>(node->owner)->pendingInterrupt = interrupt;
                node = node->next;
            }
        }
    }
}

static __forceinline AnmVmIdView CreateMusicPreviewVm(
    AnmLoadedView *loaded, int script)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    loaded->InitializeVm(vm, script);
    return g_AnmRenderManagerView->AddVmVariant0(vm);
}

static __forceinline AnmVmIdView CreateMusicTrackRowVm(
    AnmLoadedView *loaded, int script)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    const unsigned int flags = vm->flags35C;
    vm->renderLayer = 15;
    vm->flags35C = flags | 0x40000000u;
    loaded->InitializeVm(vm, script);
    return g_AnmRenderManagerView->AddVmVariant0(vm);
}

// The music-room owner includes two alignment bytes followed by its
// four-entry phase table at 0x004349CC.
int __stdcall FrontEndUpdateMusicRoom(FrontEndControllerView *controller)
{
    AnmFloat3View position;
    int remaining;
    int i;

    switch (controller->screenState)
    {
    case 0:
        if (controller->stateTimer.current == 1)
        {
            controller->cursor.count = 6;
            controller->cursor.Select(0);
            if (g_AnmRenderManagerView->FindVm(controller->vmIds[0x5f]) == NULL)
            {
                controller->CreateVm(0x5f);
                AnmLoadedView *resource = g_AsciiManagerView->asciiAnm;
                controller->difficultyAuxVmId.value =
                    resource->CreateVmVariant0(8, 15).value;
            }
            controller->CreateVm(0x67);
            int count = 0;
            controller->musicText = reinterpret_cast<char *>(
                FileSystem::OpenFile("musiccmt.txt", &remaining, 0));
            char *cursor = controller->musicText;
            if (cursor == NULL)
                goto closeMusicRoom;
            while (remaining > 0)
            {
                if (*cursor == '#'
                    || *cursor != '@')
                    cursor = FrontEndMusicSkipLine(cursor, &remaining);
                else
                {
                    // Target advances past '@' without decrementing remaining.
                    cursor = FrontEndMusicCopyLine(
                        controller->musicPaths[count], cursor + 1, &remaining);
                    cursor = FrontEndMusicCopyLine(
                        controller->musicTitles[count], cursor, &remaining);
                    for (int line = 0; line < 8; ++line)
                        cursor = FrontEndMusicCopyLine(
                            controller->musicComments[count][line], cursor, &remaining);
                    ++count;
                }
            }
            controller->cursor.count = count;
            controller->cursor.Select(0);
            controller->musicListOffset = 0;
            controller->musicTrackCount = count;
            for (int i = 0; i < 8; ++i)
                controller->musicCommentVmIds[i] =
                    CreateMusicPreviewVm(g_FrontEndMusicCommentAnm, i + 0x27);
            controller->musicCommentLine = 0;
            controller->musicSelectedTrack = 0;
            controller->musicWarning = 0;
        }
        if (controller->stateTimer.current < 10)
        {
            position.x = 64.0f;
            position.z = 0.0f;
            position.y = 96.0f - static_cast<float>(controller->musicListOffset) * 20.0f +
                static_cast<float>(controller->stateTimer.current * 40 - 40);
            for (i = controller->stateTimer.current * 2 - 2;
                 i < controller->stateTimer.current * 2; ++i)
            {
                if (i >= controller->musicTrackCount)
                    break;
                const AnmVmIdView created =
                    CreateMusicTrackRowVm(controller->frontAnm, i + 0xad);
                controller->musicRowVmIds[i].value = created.value;
                FrontEndVmIdStorageView *rowId = &controller->musicRowVmIds[i];
                AnmRenderManagerView *manager = g_AnmRenderManagerView;
                AnmVmView *vm = manager->FindVm(*rowId);
                if (vm == NULL)
                    rowId->value = 0;
                if (*reinterpret_cast<unsigned char *>(
                        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
                        0x1d892u + static_cast<unsigned int>(i)) != 0)
                    manager->DrawTextLeft(
                        vm, 0xffffff, controller->musicTitles[i]);
                else
                    manager->DrawTextLeft(
                        vm, 0xffffff, g_MusicLockedTitleFormat, i + 1);
                if (i >= controller->musicListOffset &&
                    i < controller->musicListOffset + 10)
                    vm->flags35C |= 2;
                else
                    vm->flags35C &= ~2u;
                if (i == controller->cursor.current)
                    position.x = static_cast<float>(position.x - 4.0);
                StartMusicRowInterpolation(vm, 4, &vm->position, &position, 0);
                if (i == controller->cursor.current)
                    position.x = static_cast<float>(position.x + 4.0);
                position.y += 20.0f;
                vm->pendingInterrupt = i != controller->cursor.current ? 3 : 2;
            }
        }
        if (controller->stateTimer.current >= 10)
            SetScreenState(controller, 1);
        break;
    case 1:
        if (controller->stateTimer.current % 2 == 0 && controller->musicCommentLine < 8)
        {
            AnmVmView *vm = controller->musicCommentVmIds[controller->musicCommentLine].GetVm();
            if (*reinterpret_cast<unsigned char *>(
                    reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
                    0x1d892u + static_cast<unsigned int>(controller->musicSelectedTrack)) == 0 &&
                controller->musicWarning != 0)
                g_AnmRenderManagerView->DrawTextLeft(vm, 0x8080ff,
                    g_MusicWarningLines[controller->musicCommentLine]);
            else
                g_AnmRenderManagerView->DrawTextLeft(vm, 0xffffff,
                    controller->musicComments[controller->musicSelectedTrack][controller->musicCommentLine]);
            vm->pendingInterrupt = 2;
            ++controller->musicCommentLine;
        }
        if (controller->stateTimer.current > 4)
            controller->SetScreenStateTarget(2);
        break;
    case 2:
        if (controller->stateTimer.current % 2 == 0 && controller->musicCommentLine < 8)
        {
            AnmVmView *vm = controller->musicCommentVmIds[controller->musicCommentLine].GetVm();
            if (*reinterpret_cast<unsigned char *>(
                    reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
                    0x1d892u + static_cast<unsigned int>(controller->musicSelectedTrack)) == 0 &&
                controller->musicWarning != 0)
                g_AnmRenderManagerView->DrawTextLeft(vm, 0x8080ff,
                    g_MusicWarningLines[controller->musicCommentLine]);
            else
                g_AnmRenderManagerView->DrawTextLeft(vm, 0xffffff,
                    controller->musicComments[controller->musicSelectedTrack][controller->musicCommentLine]);
            vm->pendingInterrupt = 2;
            ++controller->musicCommentLine;
        }
        {
            FrontEndCursorView *menuCursor = &controller->cursor;
            menuCursor->previous = menuCursor->current;
            if (InputRepeated(0x10)) menuCursor->Move(-1);
            if (InputRepeated(0x20)) menuCursor->Move(1);
            if (menuCursor->previous != menuCursor->current)
            {
                FrontEndPlaySound(12);
                const int scroll = controller->musicListOffset;
                const int selection = menuCursor->current;
                if (selection < scroll)
                    controller->musicListOffset = selection;
                else if (selection >= scroll + 10)
                    controller->musicListOffset = selection - 9;
                position.x = 64.0f;
                position.z = 0.0f;
                position.y = 96.0f - static_cast<float>(controller->musicListOffset) * 20.0f;
                for (i = 0; i < controller->musicTrackCount; ++i)
                {
                    FrontEndVmIdStorageView *rowId = &controller->musicRowVmIds[i];
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    AnmVmView *vm = manager->FindVm(*rowId);
                    if (vm == NULL)
                        rowId->value = 0;
                    if (i >= controller->musicListOffset && i < controller->musicListOffset + 10)
                        vm->flags35C |= 2;
                    else
                        vm->flags35C &= ~2u;
                    if (i == menuCursor->current)
                        position.x = static_cast<float>(position.x - 4.0);
                    if (fabs(vm->position.y - position.y) < 40.0f)
                        StartMusicRowInterpolation(vm, 4, &vm->position, &position, 0);
                    else
                        vm->position = position;
                    if (i == menuCursor->current)
                        position.x = static_cast<float>(position.x + 4.0);
                    position.y += 20.0f;
                    vm->pendingInterrupt = i != menuCursor->current ? 3 : 2;
                }
                controller->musicWarning = 0;
            }
        }
        if ((g_FrontEndInput.pressed & 0x1001) != 0)
        {
            // Target confirm path resolves each ID then walks only root children.
            for (int i = 0; i < 8; ++i)
            {
                AnmVmView *vm = g_AnmRenderManagerView->FindVm(
                    controller->musicCommentVmIds[i]);
                if (vm != NULL)
                {
                    vm->pendingInterrupt = 3;
                    if (vm->layerNode.previous == NULL)
                    {
                        AnmVmLayerNodeView *node = vm->layerNode.next;
                        while (node != NULL)
                        {
                            static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 3;
                            node = node->next;
                        }
                    }
                }
            }
            controller->musicSelectedTrack = controller->cursor.current;
            controller->musicCommentLine = 0;
            ResetStateTimer(controller);
            if (*reinterpret_cast<unsigned char *>(
                    reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
                    0x1d892u + static_cast<unsigned int>(controller->musicSelectedTrack)) == 0 &&
                controller->musicWarning == 0)
            {
                if ((g_FrontEndSupervisorFlags & 0x10) != 0)
                    FrontEndQueueSoundCommand("dummy", 4, 0);
                else
                    FrontEndQueueSoundCommand("dummy", 3, 0);
                controller->musicWarning = 1;
                break;
            }
            FrontEndLoadMusic(0, controller->musicPaths[controller->cursor.current]);
            if ((g_FrontEndSupervisorFlags & 0x10) != 0)
                FrontEndQueueSoundCommand("dummy", 4, 0);
            FrontEndQueueSoundCommand("dummy", 2, 0);
            // Target marks byte zero, not musicSelectedTrack's byte.
            g_FrontEndProfileData[0x1d892] = 1;
            controller->musicWarning = 0;
            break;
        }
        if ((g_FrontEndInput.pressed & 0x0a) == 0)
            break;
        {
closeMusicRoom:
            if (controller->musicText != NULL)
            {
                free(controller->musicText);
                controller->musicText = NULL;
            }
            controller->musicText = NULL;
            controller->cursor.Pop();
            for (int i = 0; i < controller->musicTrackCount; ++i)
                EnemyMarkPendingInterrupt(&controller->musicRowVmIds[i].value);
            // Target close path expands the two-list lookup and root-child walk.
            for (int i = 0; i < 8; ++i)
            {
                AnmVmView *vm = g_AnmRenderManagerView->FindVm(
                    controller->musicCommentVmIds[i]);
                if (vm != NULL)
                {
                    vm->pendingInterrupt = 1;
                    if (vm->layerNode.previous == NULL)
                    {
                        AnmVmLayerNodeView *node = vm->layerNode.next;
                        while (node != NULL)
                        {
                            static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
                            node = node->next;
                        }
                    }
                }
            }
            FrontEndPlaySound(11);
            SetScreenState(controller, 3);
        }
        break;
    case 3:
        if (controller->stateTimer.current >= 10)
        {
            DeleteVm(controller, 0x67);
            controller->MarkVmInterrupt(0x5a, 8);
            controller->MarkVmInterrupt(0x5b, 8);
            DeleteVm(controller, 0x5f);
            EnemyMarkPendingInterrupt(&controller->difficultyAuxVmId.value);
            controller->SetScreenTarget(2);
            FrontEndLoadMusic(0, "bgm/th10_02.wav");
            FrontEndPlayMusic(0, 0);
            controller->cursor.Pop();
        }
        break;
    }
    return 0;
}

// Result/replay-name controller: maintained here with the existing cursor/input
// source, not a claim about original translation-unit ownership. All external
// storage aliases remain unresolved; pause and result bodies share this view.
#include "ResultMenu.hpp"
#include "Gui.hpp"
#include "EnemySoundQueue.hpp"
extern unsigned char g_MainSoundOwner[0x52d0];
extern unsigned int g_GameRuntimeFlags;
extern unsigned int g_GameConfigurationFlags;
extern int g_GameTransitionState;
struct ResultGameChainView { unsigned int unknown00; unsigned int flags; };
struct ResultGameManagerView
{
    unsigned char unknown000[8];
    ResultGameChainView *updateChain;
    void *drawChain;
    AnmVmTimerView timer;
    unsigned char unknown024[0x34];
    unsigned int flags58;
    int initializationMode;
};
typedef char ResultGameManagerPauseFields[(offsetof(ResultGameManagerView,flags58)==0x58 && offsetof(ResultGameManagerView,initializationMode)==0x5c)?1:-1];
extern ResultGameManagerView *g_ResultGameManager; // pointer alias at 0x477810
extern void __stdcall ResultMenuOpenPause(ResultMenuView *menu); // 0x422AB0
extern void __stdcall ResultMenuUpdatePause(ResultMenuView *menu); // 0x422C80

static void ResultMenuSound(int soundId)
{
    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(soundId, 0);
}

// Target 0x4243F0 and 0x40C4D0. Pending publication only, no script execution.
void ResultMenuPendingSix(const AnmVmIdView &id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(AnmVmIdView(id.value));
    if (vm != NULL)
    {
        vm->pendingInterrupt = 6;
        if (vm->layerNode.previous == NULL)
            for (AnmVmLayerNodeView *node = vm->layerNode.next; node != NULL; node = node->next)
                static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 6;
    }
}
void ResultMenuPendingThree(AnmVmIdView *id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(AnmVmIdView(id->value));
    if (vm != NULL)
    {
        vm->pendingInterrupt = 3;
        if (vm->layerNode.previous == NULL)
            for (AnmVmLayerNodeView *node = vm->layerNode.next; node != NULL; node = node->next)
                static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 3;
    }
}

// Target 0x4497D0 returns an ID aggregate, not a VM pointer. The target clears
// an unresolved input ID but still walks vm+0x10: a valid VM is a precondition.
AnmVmIdView ResultMenuFindScript(AnmVmIdView *id, int scriptIndex)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*id);
    if (vm == NULL)
        id->value = 0;
    AnmVmLayerNodeView *node;
    for (node = &vm->layerNode; node != NULL; node = node->next)
    {
        AnmVmView *entry = static_cast<AnmVmView *>(node->owner);
        if (entry->scriptIndex == scriptIndex)
            break;
    }
    if (node == NULL)
        return AnmVmIdView(0);
    return AnmVmIdView(static_cast<AnmVmView *>(node->owner)->id);
}

// ResultMenu slots have the same four-byte representation as VM IDs but
// deliberately omit the default constructor's early zero stores.
static void ResultMenuPendingThree(ResultMenuVmIdStorage *id)
{
    ResultMenuPendingThree(reinterpret_cast<AnmVmIdView *>(id));
}
static AnmVmIdView ResultMenuFindScript(ResultMenuVmIdStorage *id, int scriptIndex)
{
    return ResultMenuFindScript(reinterpret_cast<AnmVmIdView *>(id), scriptIndex);
}

// Target 0x421F60, GUI receiver in EAX; shared maintained score/storage views.
void ResultMenuSynchronizeScore(GuiView *gui)
{
    gui->value9E78 = g_GameScoreState.score;
    const int score = gui->value9E78;
    if (g_GameScoreState.unknown00 < score)
        g_GameScoreState.unknown00 = score;
}

// Target 0x423510 does not deallocate the controller or replay list.
void ResultMenuClose(ResultMenuView *menu)
{
    menu->state = 13;
    menu->timer.SetCurrent(0);
    EnemyMarkPendingInterrupt(&menu->secondaryVm.value);
    EnemyMarkPendingInterrupt(&menu->mainVm.value);
    g_AnmGameSpeed = menu->savedSpeed;
}

// Target 0x423570. Preserve the second stage read after the opaque insertion;
// it is not an unconditional restoration of the temporary stage/format values.
void __stdcall ResultMenuPrepareScore(ResultMenuView *menu)
{
    if (g_FrontEndSelectedStage == 7 && menu->completed != 0)
    {
        g_FrontEndScoreFormatTable = g_FrontEndScoreEntryFormatTable;
        g_FrontEndSelectedStage = 8;
        g_FrontEndSelectedStageMirror = 8;
    }
    int row = FrontEndInsertScore(GetFrontEndScoreTable(
        g_ReplayShotType + g_ReplayCharacter * 3));
    if (g_FrontEndSelectedStage == 7 && menu->completed != 0)
    {
        g_FrontEndScoreFormatTable = g_FrontEndDefaultScoreFormatTable;
        g_FrontEndSelectedStage = 7;
        g_FrontEndSelectedStageMirror = 7;
    }
    // Preserve the native success-first branch and late failure publication.
    if (row >= 0)
    {
        menu->cursor.count = 25;
        menu->cursor.wraps = 1;
        menu->cursor.Select(row);
        menu->keyboard.Select(0);
        menu->keyboard.count = strlen(g_FrontEndScoreEntryAlphabet);
        menu->keyboard.wraps = 1;
        strcpy(menu->name, reinterpret_cast<char *>(g_FrontEndProfileData + 0x1d878));
        menu->nameIndex = 0;
        if (strcmp(menu->name, "        ") != 0)
            menu->keyboard.Move(-1);
        int length;
        for (length = 8; length > 0; --length)
        {
            if (menu->name[length - 1] != ' ')
                break;
        }
        menu->nameIndex = length;
        menu->scoreUnavailable = 0;
    }
    else
    {
        menu->scoreUnavailable = 1;
    }
}

static __forceinline void ResultMenuEnterSaveChoice(ResultMenuView *menu, int selection)
{
    menu->state = 8;
    menu->cursor.count = 3;
    menu->cursor.wraps = 1;
    menu->cursor.Select(selection);
    ResultMenuPendingThree(&menu->mainVm);
    menu->mainVm.SetInterrupt(static_cast<short>(menu->cursor.current + 7));
}

// Target 0x4236F0: 3260 bytes including the eight-entry switch table.
void __stdcall ResultMenuUpdateResult(ResultMenuView *menu)
{
    char replayPath[64];
    const int entryState = menu->state;
    switch (entryState)
    {
    case 6:
        if (menu->timer.current >= 10 && (g_FrontEndInput.pressed & 0x1001) != 0)
        {
            ResultMenuSound(10);
            menu->state = 7;
            menu->mainVm.SetInterrupt(2);
            menu->timer.SetCurrent(0);
        }
        break;
    case 7:
        if (menu->timer.current < 10)
            break;
        ResultMenuSynchronizeScore(g_GuiView);
        if ((g_GameRuntimeFlags & 0x10) != 0)
        {
            const unsigned int shotWord =
                static_cast<unsigned int>(g_ReplayCharacter) * 3u +
                static_cast<unsigned int>(g_ReplayShotType);
            const unsigned int stageWord =
                static_cast<unsigned int>(g_FrontEndSelectedStage) +
                static_cast<unsigned int>(g_ReplayDifficulty) * 6u;
            int *record = reinterpret_cast<int *>(
                reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
                shotWord * 0x437cu + 0x4dcu + stageWord * 8u);
            if (g_GameScoreState.score > *record)
                *record = g_GameScoreState.score;
            ResultMenuEnterSaveChoice(menu, 2);
            return;
        }

        ResultMenuPrepareScore(menu);
        menu->timer.SetCurrent(0);
        if (menu->scoreUnavailable == 0)
        {
            menu->state = 12;
            menu->mainVm.ClearFlag2();
            return;
        }
        ResultMenuEnterSaveChoice(menu, 2);
        return;
    case 8:
        {
            FrontEndCursorView *rows = &menu->cursor;
            rows->previous = rows->current;
            if ((g_FrontEndInput.pressed & 0x10) != 0 || (g_FrontEndInput.repeated & 0x10) != 0)
                rows->Move(-1);
            if ((g_FrontEndInput.pressed & 0x20) != 0 || (g_FrontEndInput.repeated & 0x20) != 0)
                rows->Move(1);
            if (rows->previous != rows->current)
            {
                menu->mainVm.SetInterrupt(static_cast<short>(rows->current + 7));
                ResultMenuSound(12);
            }
        }
        if ((g_FrontEndInput.pressed & 0x1001) != 0)
        {
            int choice = menu->cursor.current;
            ResultMenuPendingSix(ResultMenuFindScript(
                reinterpret_cast<AnmVmIdView *>(&menu->mainVm),
                choice + 0x7c));
            ResultMenuSound(10);
            menu->state = 13;
            menu->timer.SetCurrent(0);
            ResultMenuSynchronizeScore(g_GuiView);
            choice = menu->cursor.current;
            switch (choice)
            {
            case 1: {
                menu->state = 10;
                menu->timer.SetCurrent(0);
                menu->mainVm.ClearFlag2();
                menu->cursor.Push();
                menu->cursor.count = 25;
                menu->cursor.wraps = 1;
                menu->cursor.Select(0);
                for (int slot = 1; slot <= 25; ++slot)
                {
                    sprintf(replayPath, "th10_%.2d.rpy", slot);
                    menu->replayFiles[slot - 1] = ReplayManager::Load(replayPath);
                }
                break;
            }

            case 2: {
                menu->state = 13;
                menu->timer.SetCurrent(0);
                ResultMenuSound(10);
                break;
            }
            }

        }
        // Independent test: simultaneous confirm/cancel also reaches this path.
        if ((g_FrontEndInput.pressed & 0x0a) != 0)
        {
            ResultMenuSound(11);
            if (menu->cursor.current != 2)
            {
                menu->cursor.Select(2);
                menu->mainVm.SetInterrupt(static_cast<short>(menu->cursor.current + 7));
            }
        }
        break;
    case 10:
        // Retail branches on AX, not the full EAX input-helper return.
        if (menu->timer.current < 10)
            break;
        {
            FrontEndCursorView *rows = &menu->cursor;
            rows->previous = rows->current;
            if (static_cast<short>(g_FrontEndInput.IsRepeated(0x10))) rows->Move(-1);
            if (static_cast<short>(g_FrontEndInput.IsRepeated(0x20))) rows->Move(1);
            if (rows->previous != rows->current) ResultMenuSound(12);
        }
        if ((g_FrontEndInput.pressed & 0x1001) != 0)
        {
            menu->state = 11;
            menu->timer.SetCurrent(0);
            menu->keyboard.Select(0);
            menu->keyboard.count = strlen(g_FrontEndScoreEntryAlphabet);
            menu->keyboard.wraps = 1;
            if (menu->completed != 0 && (g_GameRuntimeFlags & 0x10) == 0)
            {
                ReplayDataHeader **data = &g_ReplayManager->replayData;
                time(reinterpret_cast<time_t *>(&(*data)->timestamp));
                (*data)->completionState = 8;
            }
            else
            {
                ReplayDataHeader **data = &g_ReplayManager->replayData;
                time(reinterpret_cast<time_t *>(&(*data)->timestamp));
                (*data)->completionState = g_FrontEndSelectedStage;
            }
            strcpy(menu->name, reinterpret_cast<char *>(g_FrontEndProfileData + 0x1d878));
            menu->nameIndex = 0;
            if (strcmp(menu->name, "        ") != 0) menu->keyboard.Move(-1);
            int length = 8;
            do
            {
                if (menu->name[length - 1] != ' ') break;
                --length;
            } while (length > 0);
            menu->nameIndex = length;
            ResultMenuSound(10);
            return;
        }
        if ((g_FrontEndInput.pressed & 0x0a) != 0)
        {
            menu->state = 8;
            menu->timer.SetCurrent(0);
            menu->mainVm.SetFlag2();
            menu->cursor.Pop();
            menu->cursor.count = 3;
            menu->cursor.wraps = 1;
            menu->mainVm.SetInterrupt(static_cast<short>(menu->cursor.current + 7));
            ReplayManager **output = menu->replayFiles;
            for (int remaining = 25; remaining > 0; --remaining)
            {
                ReplayManager *file = *output;
                if (file != NULL) delete file;
                *output++ = NULL;
            }
            menu->state = 8;
            menu->timer.SetCurrent(0);
            ResultMenuSound(11);
        }
        break;
    case 11: // Replay-file naming: distinct from the score-entry state.
    {
        // Preserve the same native 16-bit input-result tests here.
        if (menu->timer.current < 10) break;
        FrontEndCursorView *keyboard = &menu->keyboard;
        keyboard->previous = keyboard->current;
        if (static_cast<short>(g_FrontEndInput.IsRepeated(0x10))) keyboard->Move(-13);
        if (static_cast<short>(g_FrontEndInput.IsRepeated(0x20))) keyboard->Move(13);
        if (static_cast<short>(g_FrontEndInput.IsRepeated(0x40)))
        {
            if (keyboard->current % 13 != 0) keyboard->Move(-1);
            else keyboard->Move(12);
        }
        if (static_cast<short>(g_FrontEndInput.IsRepeated(0x80)))
        {
            if (keyboard->current % 13 != 12) keyboard->Move(1);
            else keyboard->Move(-12);
        }
        if (keyboard->previous != keyboard->current) ResultMenuSound(12);
        if ((g_FrontEndInput.pressed & 0x1001) != 0)
        {
            const char *alphabet = g_FrontEndScoreEntryAlphabet;
            int length = strlen(alphabet);
            int key = keyboard->current;
            if (key < length - 3)
            {
                if (menu->nameIndex < 8)
                {
                    menu->name[menu->nameIndex] = alphabet[key];
                    ++menu->nameIndex;
                    if (menu->nameIndex >= 8)
                        FrontEndCursorSetCurrent(length - 1, keyboard);
                }
                else menu->name[menu->nameIndex - 1] = alphabet[key];
            }
            else if (key == length - 3)
            {
                if (menu->nameIndex < 8)
                {
                    menu->name[menu->nameIndex] = ' ';
                    ++menu->nameIndex;
                    if (menu->nameIndex >= 8)
                        FrontEndCursorSetCurrent(length - 1, keyboard);
                }
                else menu->name[menu->nameIndex - 1] = ' ';
            }
            else if (key == length - 2)
            {
                if (menu->nameIndex == 0) return;
                --menu->nameIndex;
                menu->name[menu->nameIndex] = ' ';
                ResultMenuSound(11);
                return;
            }
            else if (key == length - 1)
            {
                ResultMenuSound(44);
                sprintf(replayPath, "th10_%.2d.rpy", menu->cursor.current + 1);
                ReplayManager::Destroy(menu->replayFiles[menu->cursor.current]);
                g_ReplayManager->SaveReplay(replayPath, menu->name);
                // Original re-reads the selected slot after saving.
                int slot = menu->cursor.current;
                menu->replayFiles[slot] = ReplayManager::Load(replayPath);
                menu->state = 10;
                menu->timer.SetCurrent(0);
                strcpy(reinterpret_cast<char *>(g_FrontEndProfileData + 0x1d878), menu->name);
                return;
            }
            ResultMenuSound(10);
        }
        if ((g_FrontEndInput.pressed & 0x0a) != 0)
        {
            ResultMenuSound(11);
            if (menu->nameIndex == 0)
            {
                menu->state = 10;
                menu->timer.SetCurrent(0);
                return;
            }
            --menu->nameIndex;
            menu->name[menu->nameIndex] = ' ';
            return;
        }
        break;
    }
    case 12: // Score-entry naming: a separately dispatched state in retail.
        if (menu->timer.current < 10) break;
        if (menu->scoreUnavailable == 0)
        {
            FrontEndCursorView *keyboard = &menu->keyboard;
            keyboard->previous = keyboard->current;
            if (static_cast<short>(g_FrontEndInput.IsRepeated(0x10))) keyboard->Move(-13);
            if (static_cast<short>(g_FrontEndInput.IsRepeated(0x20))) keyboard->Move(13);
            if (static_cast<short>(g_FrontEndInput.IsRepeated(0x40)))
            {
                if (keyboard->current % 13 != 0) keyboard->Move(-1);
                else keyboard->Move(12);
            }
            if (static_cast<short>(g_FrontEndInput.IsRepeated(0x80)))
            {
                if (keyboard->current % 13 != 12) keyboard->Move(1);
                else keyboard->Move(-12);
            }
            if (keyboard->previous != keyboard->current) ResultMenuSound(12);
        }
        if ((g_FrontEndInput.pressed & 0x1001) != 0)
        {
            if (menu->scoreUnavailable == 0)
            {
                const char *alphabet = g_FrontEndScoreEntryAlphabet;
                int length = strlen(alphabet);
                int key = menu->keyboard.current;
                if (key < length - 3)
                {
                    if (menu->nameIndex < 8)
                    {
                        menu->name[menu->nameIndex] = alphabet[key];
                        ++menu->nameIndex;
                        if (menu->nameIndex >= 8)
                            FrontEndCursorSetCurrent(length - 1, &menu->keyboard);
                    }
                    else menu->name[menu->nameIndex - 1] = alphabet[key];
                }
                else if (key == length - 3)
                {
                    if (menu->nameIndex < 8)
                    {
                        menu->name[menu->nameIndex] = ' ';
                        ++menu->nameIndex;
                        if (menu->nameIndex >= 8)
                            FrontEndCursorSetCurrent(length - 1, &menu->keyboard);
                    }
                    else menu->name[menu->nameIndex - 1] = ' ';
                }
                else if (key == length - 2)
                {
                    if (menu->nameIndex == 0) return;
                    --menu->nameIndex;
                    menu->name[menu->nameIndex] = ' ';
                }
                else if (key == length - 1)
                {
                    const char *name = menu->name;
                    // Score-profile addresses are flat native DWORDs.
                    strcpy(reinterpret_cast<char *>(
                        reinterpret_cast<unsigned int>(g_FrontEndProfileData) +
                        (static_cast<unsigned int>(g_ReplayCharacter) * 3u +
                         static_cast<unsigned int>(g_ReplayShotType)) * 0x437cu +
                        0x1eu +
                        (static_cast<unsigned int>(menu->cursor.current) +
                         static_cast<unsigned int>(g_ReplayDifficulty) * 10u) * 0x18u),
                        name);
                    char *lastName = reinterpret_cast<char *>(g_FrontEndProfileData + 0x1d878);
                    const char *cursor = name;
                    char letter;
                    do
                    {
                        letter = *cursor;
                        lastName[cursor - name] = letter;
                        ++cursor;
                    } while (letter != '\0');
                    menu->mainVm.SetFlag2();
                    ResultMenuEnterSaveChoice(menu, 0);
                }
                ResultMenuSound(10);
            }
            else
            {
                menu->mainVm.SetFlag2();
                ResultMenuEnterSaveChoice(menu, 0);
            }
        }
        if ((g_FrontEndInput.pressed & 0x0a) != 0)
        {
            if (menu->cursor.current >= 0)
            {
                if (menu->nameIndex != 0)
                {
                    ResultMenuSound(11);
                    --menu->nameIndex;
                    menu->name[menu->nameIndex] = ' ';
                }
            }
            else
            {
                menu->state = 8;
                menu->cursor.count = 3;
                menu->cursor.Select(0);
                menu->timer.SetCurrent(0);
                ResultMenuSound(10);
                return;
            }
        }
        break;
    case 13:
        if (menu->timer.current < 12) break;
        ResultMenuClose(menu);
        menu->state = 21;
        switch (menu->cursor.current)
        {
        case 0:
            g_GameTransitionState = menu->completed != 0 ? 10 : 13;
            break;
        case 1:
            g_MainSupervisorView.SetNextGameMode(4);
            break;
        case 2:
            g_MainSupervisorView.SetNextGameMode(4);
            break;
        }
        break;
    }
}

// Target 0x4223F0, real result-controller caller. Timer scale must already be
// initialized here, unlike SetCurrent's lazy initialization contract.
int ResultMenuUpdate(ResultMenuView *menu)
{
    switch (menu->state)
    {
    case 0:
        if ((g_GameRuntimeFlags & 0x20) == 0 &&
            ((g_FrontEndInput.pressed & 8) != 0 || (g_GameConfigurationFlags & 0x10) != 0) &&
            g_ResultGameManager->updateChain != NULL &&
            (g_ResultGameManager->updateChain->flags & 2) != 0 &&
            g_ResultGameManager->timer.current >= 30)
            ResultMenuOpenPause(menu);
        break;
    case 1: case 2: case 3: case 4: case 5:
        ResultMenuUpdatePause(menu);
        break;
    case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13:
        ResultMenuUpdateResult(menu);
        break;
    }
    menu->timer.previous = menu->timer.current;
    if (*menu->timer.scale > 0.99f && *menu->timer.scale < 1.01f)
    {
        ++menu->timer.current;
        menu->timer.subframe += 1.0f;
    }
    else
        menu->timer.current = static_cast<int>(menu->timer.subframe += *menu->timer.scale);
    return 1;
}

// Target 0x4224C0. Position is a value argument: RET16 consumes owner + float3.
// The alphabet length is captured once; the final three cells are control glyphs.
void __stdcall ResultMenuDrawNameEntry(ResultMenuView *menu, AnmFloat3View position)
{
    int characterCount = strlen(g_FrontEndScoreEntryAlphabet);
    g_AsciiManagerView->AddFormatText(&position, "%s", menu->name);
    position.x += static_cast<float>(menu->nameIndex * 9);
    if (menu->nameIndex == 8)
        position.x -= 9.0f;
    g_AsciiManagerView->color = 0xffffff00u;
    g_AsciiManagerView->AddFormatText(&position, "_");
    g_AsciiManagerView->color = 0xffffffffu;

    position.x = 112.0f;
    position.y = 320.0f;
    position.z = 0.0f;
    for (int index = 0; index < characterCount; ++index)
    {
        g_AsciiManagerView->color = menu->keyboard.current == index ?
            0xffffff00u : 0xff808080u;
        if (index < characterCount - 3)
            g_AsciiManagerView->AddFormatText(
                &position, "%c", g_FrontEndScoreEntryAlphabet[index]);
        else
            g_AsciiManagerView->AddFormatText(&position, "%c",
                index == characterCount - 3 ? 0x81 :
                index == characterCount - 2 ? 0x7f : 0x80);
        if (static_cast<unsigned int>(index) % 13 == 12)
        {
            position.x = 112.0f;
            position.y += 16.0f;
        }
        else
            position.x += 18.0f;
    }
    g_AsciiManagerView->color = 0xffffffffu;
}

// Target 0x422660. ASCII+8988 selects the queued text shadow, not a game lock.
int __stdcall ResultMenuDrawRows(ResultMenuView *menu)
{
    g_AsciiManagerView->drawShadow = 1;
    switch (menu->state)
    {
    case 10:
    case 17:
    {
        AnmFloat3View position(48.0f, 64.0f, 0.0f);
        for (int row = 0; row < 25; ++row)
        {
            g_AsciiManagerView->color = menu->cursor.current == row ?
                0xffffff00u : 0xff808080u;
            ReplayManager *file = menu->replayFiles[row];
            if (file != NULL)
            {
                ReplayDataHeader *data = file->replayData;
                tm *date = localtime(reinterpret_cast<time_t *>(&data->timestamp));
                g_AsciiManagerView->AddFormatText(&position,
                    "No.%.2d %s %.2d/%.2d/%.2d %s %s %s", row + 1,
                    data->replayName, date->tm_year % 100, date->tm_mon + 1,
                    date->tm_mday,
                    g_ReplayCharacterNames[data->character * 3 + data->shotType],
                    g_ReplayDifficultyNames[data->difficulty],
                    g_ReplayCompletionNames[data->completionState]);
            }
            else
                g_AsciiManagerView->AddFormatText(&position,
                    "No.%.2d -------- --/--/-- ------- - St-", row + 1);
            position.y += 15.0f;
        }
        g_AsciiManagerView->color = 0xffffffffu;
        break;
    }
    case 11:
    case 18:
    {
        AnmFloat3View position;
        position.z = 0.0f;
        int selected = menu->cursor.current;
        if (menu->timer.current < 10)
        {
            float settled = selected * 15.0f + 64.0f;
            position.y = settled + (224.0f - settled) * menu->timer.current * 0.1f;
        }
        else
            position.y = 224.0f;
        position.x = 102.0f;
        ResultMenuDrawNameEntry(menu, position);
        position.x = 48.0f;
        ReplayDataHeader *data = g_ReplayManager->replayData;
        tm *date = localtime(reinterpret_cast<time_t *>(&data->timestamp));
        g_AsciiManagerView->AddFormatText(&position,
            "No.%.2d          %.2d/%.2d/%.2d %s %s %s", selected + 1,
            date->tm_year % 100, date->tm_mon + 1, date->tm_mday,
            g_ReplayCharacterNames[data->character * 3 + data->shotType],
            g_ReplayDifficultyNames[data->difficulty],
            g_ReplayCompletionNames[g_FrontEndSelectedStage]);
        break;
    }
    case 12:
    case 19:
    {
        AnmFloat3View position;
        position.x = 48.0f;
        position.z = 0.0f;
        int selected = menu->cursor.current;
        position.y = 64.0f;
        g_AsciiManagerView->AddFormatText(&position, "            Score Ranking!!");
        position.x = 75.0f;
        position.y = selected * 18.0f + 96.0f;
        if (menu->scoreUnavailable == 0)
            ResultMenuDrawNameEntry(menu, position);
        else
            selected = -1;
        position.x = 48.0f;
        position.y = 96.0f;
        for (int row = 0; row < 10; ++row)
        {
            g_AsciiManagerView->color = row == selected ?
                0xffffff00u : 0xff808080u;
            if (GetCurrentScoreEntryTable().records[g_ReplayDifficulty][row].timestamp != 0)
            {
                FrontEndScoreTableView *table = &GetCurrentScoreEntryTable();
                tm *date = localtime(&table->records[g_ReplayDifficulty][row].timestamp);
                // Resample the score selection after localtime.
                FrontEndScoreRecordView &record =
                    GetCurrentScoreEntryTable().records[g_ReplayDifficulty][row];
                g_AsciiManagerView->AddFormatText(&position,
                    "%2d %s %.9ld%d %.2d/%.2d/%.2d %s", row + 1,
                    record.name, static_cast<long>(record.score), record.scoreSuffix,
                    date->tm_year % 100, date->tm_mon + 1, date->tm_mday,
                    g_FrontEndStageNames[record.stageNameIndex]);
            }
            else
            {
                FrontEndScoreRecordView &record =
                    GetCurrentScoreEntryTable().records[g_ReplayDifficulty][row];
                g_AsciiManagerView->AddFormatText(&position,
                    "%2d %s %.9ld%d --/--/-- Stage -", row + 1,
                    record.name, static_cast<long>(record.score), record.scoreSuffix);
            }
            position.y += 18.0f;
        }
        g_AsciiManagerView->color = 0xffffffffu;
        break;
    }
    }
    g_AsciiManagerView->drawShadow = 0;
    return 1;
}

// Target 0x422AA0 is installed as the scheduler draw callback at0x42217A.
int __fastcall ResultMenuDrawRowsCallback(ResultMenuView *menu)
{
    return ResultMenuDrawRows(menu);
}

#include "ReplayTimingSource.hpp"
extern int g_GameStartupValue474C90;

namespace
{

// Target 0x421FA0. The table is a legacy view into profile-backed records;
// this pointer arithmetic does not claim complete table storage/lifetime closure.
// Equal signed scores insert ahead of the existing row; no qualifying row
// returns -1 without writing. Copy assignment preserves all24 record bytes.
int FrontEndInsertScore(FrontEndScoreTableView *table)
{
    int row = 0;
    while (row < 10)
    {
        if (table->records[g_ReplayDifficulty][row].score <= g_GameScoreState.score)
            break;
        ++row;
    }
    if (row >= 10)
        return -1;
    for (int index = 9; index > row; --index)
        table->records[g_ReplayDifficulty][index] =
            table->records[g_ReplayDifficulty][index - 1];
    table->records[g_ReplayDifficulty][row].score = g_GameScoreState.score;
    table->records[g_ReplayDifficulty][row].scoreSuffix =
        static_cast<signed char>(g_GameStartupValue474C90);
    table->records[g_ReplayDifficulty][row].stageNameIndex =
        static_cast<signed char>(g_FrontEndSelectedStage);
    time(&table->records[g_ReplayDifficulty][row].timestamp);
    // The target reloads difficulty after time(), then writes exactly9 bytes,
    // leaving the tenth name byte untouched. There is no denominator guard.
    strcpy(table->records[g_ReplayDifficulty][row].name, "        ");
    table->records[g_ReplayDifficulty][row].slowdownRate = static_cast<float>(
        100.0 - (g_ReplayTimingSource->lagNumerator /
                 g_ReplayTimingSource->lagDenominator) * 100.0);
    return row;
}

} // namespace

// Target 0x424480. The receiver is the cached capture manager, while lookup
// deliberately uses the current global manager. Sprite/ANM pointers must exist.
// All four conversions precede the availability test, as in the target.
int AnmRenderManagerView::RequestCaptureRectangle(
    AnmVmIdView id, int x, int y, int width, int height)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(id);
    AnmSpriteView *sprite = vm->loadedSprite;
    int sourceHeight = static_cast<int>(sprite->height);
    int sourceWidth = static_cast<int>(sprite->width);
    int sourceY = static_cast<int>(sprite->sourceY);
    int sourceX = static_cast<int>(sprite->sourceX);
    if (captureSurfaceIndex >= 0)
        return -1;
    // Publish the destination rectangle before the source crop; these are
    // independent fields but the order also matches the native store schedule.
    captureSurfaceIndex = vm->anmFile->anmFileIndex;
    captureDestinationX = x;
    captureDestinationY = y;
    captureDestinationWidth = width;
    captureDestinationHeight = height;
    captureSourceX = sourceX;
    captureSourceHeight = sourceHeight;
    captureSourceY = sourceY;
    captureSourceWidth = sourceWidth;
    captureRequestState = 0;
    return 0;
}

// Target 0x422AB0. Allocation failure is not checked by the original path.
void __stdcall ResultMenuOpenPause(ResultMenuView *menu)
{
    menu->state = 1;
    menu->timer.SetCurrent(0);
    g_ResultGameManager->flags58 |= 0x10;
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmLoadedView *resource = g_AsciiManagerView->captureAnm;
    AnmVmView *vm = manager->AllocateVm();
    unsigned int vmFlags = vm->flags35C;
    vm->renderLayer = 15;
    vm->flags35C = vmFlags | 0x40000000;
    resource->InitializeVm(vm, 0);
    manager = g_AnmRenderManagerView;
    AnmVmIdView id = manager->AddVmVariant1(vm);
    menu->secondaryVm = id;
    manager->RequestCaptureRectangle(id, 32, 16, 384, 448);
    resource = g_GuiView->frontAnm;
    menu->frontAnm = resource;
    vm = manager->AllocateVm();
    vmFlags = vm->flags35C;
    vm->renderLayer = 15;
    vm->flags35C = vmFlags | 0x40000000;
    resource->InitializeVm(vm, 121);
    AnmVmIdView mainId = g_AnmRenderManagerView->AddVmVariant1(vm);
    menu->mainVm = mainId;
    ResultMenuPendingThree(&menu->mainVm);
    ResultMenuSound(32);
    FrontEndQueueSoundCommand("Pause", 6, 0);
    if (g_ResultGameManager->initializationMode != 0)
    {
        ResultMenuFindScript(reinterpret_cast<AnmVmIdView *>(&menu->mainVm), 117)
            .MarkForDeletionAndClear();
    }
    menu->savedSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
}

// Target 0x4231D0..0x42336A: game-over result setup, script128/completed0.
// Allocation and capture inputs are unchecked, as in the original path.
void ResultMenuOpenGameOver(ResultMenuView *menu)
{
    ResultGameManagerView *game = g_ResultGameManager;
    if (game->initializationMode == 1)
    {
        g_GameTransitionState = (g_GameConfigurationFlags & 0x1000) != 0 ? 2 : 4;
        return;
    }
    menu->state = 6;
    menu->timer.SetCurrent(0);
    game->flags58 |= 0x10;
    AnmLoadedView *resource = g_AsciiManagerView->captureAnm;
    menu->secondaryVm = resource->CreateVmVariant1(0, 15);
    g_AnmRenderManagerView->RequestCaptureRectangle(
        menu->secondaryVm, 32, 16, 384, 448);
    resource = g_GuiView->frontAnm;
    menu->frontAnm = resource;
    menu->mainVm = resource->CreateVmVariant1(128, 15);
    FrontEndLoadMusic(0, "bgm/th10_17.wav");
    if ((g_FrontEndSupervisorFlags & 0x10) != 0)
        FrontEndQueueSoundCommand("dummy", 4, 0);
    FrontEndQueueSoundCommand("dummy", 2, 0);
    g_FrontEndProfileData[0x1d8a3] = 1;
    menu->completed = 0;
    menu->savedSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
}

// Target 0x423370..0x42350A: completion setup, script129/completed1.
void GameCompleteStageTransition(ResultMenuView *menu)
{
    ResultGameManagerView *game = g_ResultGameManager;
    if (game->initializationMode == 1)
    {
        g_GameTransitionState = (g_GameConfigurationFlags & 0x1000) != 0 ? 2 : 4;
        return;
    }
    menu->state = 6;
    menu->timer.SetCurrent(0);
    game->flags58 |= 0x10;
    AnmLoadedView *resource = g_AsciiManagerView->captureAnm;
    menu->secondaryVm = resource->CreateVmVariant1(0, 15);
    g_AnmRenderManagerView->RequestCaptureRectangle(
        menu->secondaryVm, 32, 16, 384, 448);
    resource = g_GuiView->frontAnm;
    menu->frontAnm = resource;
    menu->mainVm = resource->CreateVmVariant1(129, 15);
    FrontEndLoadMusic(0, "bgm/th10_17.wav");
    if ((g_FrontEndSupervisorFlags & 0x10) != 0)
        FrontEndQueueSoundCommand("dummy", 4, 0);
    FrontEndQueueSoundCommand("dummy", 2, 0);
    g_FrontEndProfileData[0x1d8a3] = 1;
    menu->completed = 1;
    menu->savedSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
}

// Target 0x422C30. This does not change state or clear the two primary IDs.
extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)

void ResultMenuResume(ResultMenuView *menu)
{
    g_ResultGameManager->flags58 &= ~0x10;
    FrontEndQueueSoundCommand("UnPause", 7, 0);
    g_AnmRenderManagerView->MarkVmForDeletion(*reinterpret_cast<AnmVmIdView *>(&menu->auxiliaryVm));
    menu->auxiliaryVm.value = 0;
    _ReadWriteBarrier();
    g_AnmGameSpeed = menu->savedSpeed;
}

enum ResultMenuPausePhase
{
    RESULT_MENU_PAUSE_CLOSED = 0,
    RESULT_MENU_PAUSE_OPENING = 1,
    RESULT_MENU_PAUSE_CHOOSING = 2,
    RESULT_MENU_PAUSE_CLOSING = 3,
    RESULT_MENU_PAUSE_CONFIRM_OPENING = 4,
    RESULT_MENU_PAUSE_CONFIRM_RESOLVING = 5
};

static __forceinline short PauseMenuInputActive(
    const FrontEndInputView *input, unsigned short mask)
{
    if ((input->pressed & mask) != 0)
        return 1;
    if ((input->repeated & mask) != 0)
        return 1;
    return 0;
}

// The pause input word starts at FrontEndInputView::pressed (0x00474E36).
extern unsigned int g_ResultMenuHeldInputWord;

// Target 0x00422C80. The two shortcuts and escape can override an
// earlier choice during the same input update.
void __stdcall ResultMenuUpdatePause(ResultMenuView *menu)
{
    switch (menu->state)
    {
    case RESULT_MENU_PAUSE_OPENING:
        if (menu->timer.current >= 10)
        {
            menu->state = RESULT_MENU_PAUSE_CHOOSING;
            menu->cursor.count = g_ResultGameManager->initializationMode != 0 ? 2 : 3;
            menu->cursor.wraps = 1;
            menu->cursor.Select(0);
            menu->mainVm.SetInterrupt(static_cast<short>(menu->cursor.current + 7));
            return;
        }
        break;

    case RESULT_MENU_PAUSE_CHOOSING:
    {
        FrontEndCursorView *cursor = &menu->cursor;
        cursor->previous = cursor->current;
        if (PauseMenuInputActive(&g_FrontEndInput, FRONT_END_INPUT_UP))
            cursor->Move(-1);
        if (PauseMenuInputActive(&g_FrontEndInput, FRONT_END_INPUT_DOWN))
            cursor->Move(1);
        if (cursor->previous != cursor->current)
        {
            menu->mainVm.SetInterrupt(static_cast<short>(cursor->current + 7));
            ResultMenuSound(FRONT_END_SOUND_MOVE);
        }
        if ((g_ResultMenuHeldInputWord & FRONT_END_INPUT_CONFIRM) != 0)
        {
            ResultMenuSound(FRONT_END_SOUND_SELECT);
            switch (cursor->current)
            {
            case 2:
                ResultMenuPendingSix(ResultMenuFindScript(
                    reinterpret_cast<AnmVmIdView *>(&menu->mainVm), 117));
                menu->state = RESULT_MENU_PAUSE_CONFIRM_OPENING;
                break;
            case 1:
                ResultMenuPendingSix(ResultMenuFindScript(
                    reinterpret_cast<AnmVmIdView *>(&menu->mainVm), 116));
                menu->state = g_ResultGameManager->initializationMode != 0 ?
                    RESULT_MENU_PAUSE_CLOSING : RESULT_MENU_PAUSE_CONFIRM_OPENING;
                break;
            case 0:
                EnemyMarkPendingInterrupt(&menu->secondaryVm.value);
                EnemyMarkPendingInterrupt(&menu->mainVm.value);
                EnemyMarkPendingInterrupt(&menu->auxiliaryVm.value);
                menu->state = RESULT_MENU_PAUSE_CLOSING;
                break;
            }
            menu->timer.SetCurrent(0);
        }
        if ((g_ResultMenuHeldInputWord & 0x4000) != 0)
        {
            ResultMenuSound(FRONT_END_SOUND_SELECT);
            ResultMenuPendingSix(ResultMenuFindScript(
                reinterpret_cast<AnmVmIdView *>(&menu->mainVm), 117));
            menu->state = RESULT_MENU_PAUSE_CLOSING;
            menu->timer.SetCurrent(0);
            menu->cursor.Select(2);
            EnemyMarkPendingInterrupt(&menu->secondaryVm.value);
        }
        if ((g_ResultMenuHeldInputWord & 0x200) != 0)
        {
            ResultMenuSound(FRONT_END_SOUND_SELECT);
            ResultMenuPendingSix(ResultMenuFindScript(
                reinterpret_cast<AnmVmIdView *>(&menu->mainVm), 116));
            menu->state = RESULT_MENU_PAUSE_CLOSING;
            menu->timer.SetCurrent(0);
            menu->cursor.Select(1);
        }
        goto escape;
    }

    case RESULT_MENU_PAUSE_CONFIRM_OPENING:
    {
        if (menu->timer.current < 20)
            return;
        if (menu->timer.current == 20)
        {
            menu->cursor.Push();
            menu->cursor.count = 2;
            menu->cursor.wraps = 1;
            menu->cursor.Select(1);
            menu->mainVm.SetInterrupt(14);
        }
        if (menu->timer.current < 30)
            return;
        if (menu->timer.current == 30)
            menu->mainVm.SetInterrupt(static_cast<short>(menu->cursor.current + 15));

        FrontEndCursorView *cursor = &menu->cursor;
        cursor->previous = cursor->current;
        if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_UP)))
            cursor->Move(-1);
        if (static_cast<short>(g_FrontEndInput.IsRepeated(FRONT_END_INPUT_DOWN)))
            cursor->Move(1);
        if (cursor->previous != cursor->current)
        {
            menu->mainVm.SetInterrupt(static_cast<short>(cursor->current + 15));
            ResultMenuSound(FRONT_END_SOUND_MOVE);
        }
        if ((g_ResultMenuHeldInputWord & FRONT_END_INPUT_CONFIRM) != 0)
        {
            ResultMenuSound(FRONT_END_SOUND_SELECT);
            switch (cursor->current)
            {
            case 1:
                ResultMenuPendingSix(ResultMenuFindScript(
                    reinterpret_cast<AnmVmIdView *>(&menu->mainVm), 120));
                menu->state = RESULT_MENU_PAUSE_CONFIRM_RESOLVING;
                break;
            case 0:
                ResultMenuPendingSix(ResultMenuFindScript(
                    reinterpret_cast<AnmVmIdView *>(&menu->mainVm), 119));
                menu->state = RESULT_MENU_PAUSE_CONFIRM_RESOLVING;
                break;
            }
            menu->timer.SetCurrent(0);
        }
        goto escape;
    }

    case RESULT_MENU_PAUSE_CONFIRM_RESOLVING:
        if (menu->timer.current < 20)
            break;
        switch (menu->cursor.current)
        {
        case 1:
        {
            menu->cursor.Pop();
            short frame = static_cast<short>(menu->cursor.current);
            frame += 7;
            menu->mainVm.SetInterrupt(frame);
            menu->state = RESULT_MENU_PAUSE_CHOOSING;
            menu->timer.SetCurrent(0);
            return;
        }
        case 0:
            EnemyMarkPendingInterrupt(&menu->secondaryVm.value);
            EnemyMarkPendingInterrupt(&menu->mainVm.value);
            menu->state = RESULT_MENU_PAUSE_CLOSING;
            menu->cursor.Pop();
            break;
        }
        menu->timer.SetCurrent(0);
        return;

    case RESULT_MENU_PAUSE_CLOSING:
        if (menu->timer.current < 12)
            break;
        menu->state = RESULT_MENU_PAUSE_CLOSED;
        switch (menu->cursor.current)
        {
        case 2:
            EnemyMarkPendingInterrupt(&menu->mainVm.value);
            g_GameTransitionState = 10;
            return;
        case 1:
            EnemyMarkPendingInterrupt(&menu->secondaryVm.value);
            EnemyMarkPendingInterrupt(&menu->mainVm.value);
            g_MainSupervisorView.SetNextGameMode(4);
            return;
        case 0:
            ResultMenuResume(menu);
            break;
        }
        break;
    }
    return;

escape:
    if ((g_ResultMenuHeldInputWord & 8) != 0)
    {
        menu->cursor.Select(0);
        EnemyMarkPendingInterrupt(&menu->secondaryVm.value);
        EnemyMarkPendingInterrupt(&menu->mainVm.value);
        EnemyMarkPendingInterrupt(&menu->auxiliaryVm.value);
        menu->state = RESULT_MENU_PAUSE_CLOSING;
        menu->timer.SetCurrent(0);
    }
}
