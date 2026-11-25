#include "Main.hpp"
#include "Ending.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GameScoreState.hpp"
#include "InputState.hpp"
#include "EnemySoundQueue.hpp"
#include "TransitionEffect.hpp"
#include <stdlib.h>
#include <string.h>

// Target observations determine this recovery. Reference hypotheses from
// levzzz5154/th10-decomp (MIT), copyright(c)2026 levzzz, c6791d82dc9f02e55eca45a5314f900a85481efd;
// Further stored-ID expression lead: d1f31212514781c928fba145d8207397c8fb5650,
// matching2/msvc/src/game/ending/recovery_script_executor.cpp.
// license: docs/licenses/th10-decomp-MIT.txt. No reference volatile casts,
// forced helper controls, assembly or explicit return-output stack slots.
extern char g_StageFilePath[];                  //497C38, capacity unknown
extern const char *g_EndingScriptPaths[12];      //474440, external data owner
extern char g_EndingDataErrorMessage[];          //46CB68, external message
extern unsigned char *g_GuiPersistentData;      //47783C
extern int g_GuiDifficulty;                     //474C74
extern int g_GameStartupCounter;                //4918A4
extern void PlayerSetShotVmInterrupt2(unsigned int *id);
extern void ResultMenuPendingThree(AnmVmIdView *id);
extern void EnemyMarkPendingInterrupt(int *id);
extern int __stdcall FrontEndLoadMusic(int slot, const char *path);
extern int __stdcall FrontEndPlayMusic(int slot, int trackIndex);
extern int __stdcall GameQueueScaledSoundCommand(float magnitude);
namespace th10 { namespace leaf {
unsigned int Fn0040BA80();
unsigned char *Fn00417010(unsigned char *payload);
} }

namespace
{
// Ordinary shared lookup used by the constructor's three direct consumers.
inline AnmVmView *ResolveEndingTextVm(AnmRenderManagerView *manager, int &id)
{
    AnmVmView *vm = manager->FindVm(AnmVmIdView(id));
    if (vm == NULL)
        id = 0;
    return vm;
}
inline void ReleaseEndingTextVm(AnmRenderManagerView *manager, int &id)
{
    AnmVmView *vm = manager->FindVm(AnmVmIdView(id));
    if (vm)
    {
        vm->flags35C |= 0x04000000u;
        if (!vm->layerNode.previous)
            for (AnmVmLayerNodeView *node = vm->layerNode.next; node; node = node->next)
                static_cast<AnmVmView *>(node->owner)->flags35C |= 0x04000000u;
    }
    id = 0;
}
// Text slots already contain IDs; lookup consumes that stored ID value.
inline AnmVmView *ResolveEndingTextSlot(AnmVmIdView *id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*id);
    if (vm == NULL)
        id->value = 0;
    return vm;
}
// File header+4 supplies the first instruction offset.
inline unsigned char *EndingScriptStart(unsigned char *file)
{
    // The script header carries an unchecked signed offset. Retail forms
    // the entry point through modulo-32-bit Win32 address arithmetic.
    return reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(file) +
        static_cast<unsigned int>(EndingRecordInt(file)));
}
// Opcodes 8 and 15..17 carry destination, resource and script dwords.
struct EndingAnimationCommand
{
    unsigned short time;
    unsigned char opcode;
    unsigned char payloadBytes;
    int destination;
    int resource;
    int script;
};
typedef char EndingAnimationCommandSize[(sizeof(EndingAnimationCommand) == 16) ? 1 : -1];
// Creation completes before this helper reads the current destination cursor.
inline void PublishEndingAnimation(EndingTextVmView *text, const AnmVmIdView &created)
{
    text->animationIds[EndingRecordInt(text->instruction)] = created.value;
}
inline void ReleaseEndingBank(int slot)
{
    AnmLoadedView *&bank = g_AnmRenderManagerView->loadedAnms[slot];
    if (bank)
    {
        bank->Release();
        delete bank;
        bank = NULL;
    }
}
}

// Target40B3A0/192; worker destruction performs the final second Stop.
EndingTextVmView::~EndingTextVmView()
{
    worker.Stop();
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    for (int i = 0; i < 5; ++i)
        ReleaseEndingTextVm(manager, textIds[i]);
}

// Target40B480/106: load first, then release the previous buffer even on failure.
unsigned char *EndingView::LoadScript(const char *path)
{
    g_StageFilePath[0] = '\0';
    strcat(g_StageFilePath, path);
    unsigned char *loaded = FileSystem::OpenFile(g_StageFilePath, NULL, 0);
    if (scriptData)
    {
        free(scriptData);
        scriptData = NULL;
    }
    scriptData = loaded;
    return loaded;
}

struct EndingGameCountersView
{
    unsigned char unknown000[0x28];
    int character;
    int shotType;
    int unknown030;
    int difficulty;
    unsigned char unknown038[0x18];
    int continueState;
};

struct EndingUnlocksView
{
    unsigned char unknown000[0x1d882];
    unsigned char seen[12];
    unsigned char alternateSeen;
};

typedef char EndingGameCountersOffsets[
    (offsetof(EndingGameCountersView, character) == 0x28 &&
     offsetof(EndingGameCountersView, shotType) == 0x2c &&
     offsetof(EndingGameCountersView, difficulty) == 0x34 &&
     offsetof(EndingGameCountersView, continueState) == 0x50) ? 1 : -1];

typedef char EndingUnlocksOffsets[
    (offsetof(EndingUnlocksView, seen) == 0x1d882 &&
     offsetof(EndingUnlocksView, alternateSeen) == 0x1d88e) ? 1 : -1];

static __forceinline EndingUnlocksView *GetEndingUnlocks()
{
    return reinterpret_cast<EndingUnlocksView *>(g_GuiPersistentData);
}

static __forceinline AnmLoadedView *GetEndingAsciiResource(AsciiManagerView *ascii)
{
    return ascii->asciiAnm;
}

static __forceinline void SetEndingFlag(unsigned int &flags, unsigned int bit)
{
    flags |= bit;
}

int EndingView::Initialize()
{
    th10::ChainElem *update = new th10::ChainElem;
    unsigned int updateFlags;
    memcpy(&updateFlags, &update->flags, sizeof(updateFlags));
    update->flags = updateFlags | 3;
    update->callback = reinterpret_cast<th10::ChainCallback>(EndingUpdateCallback);
    update->addedCallback = NULL;
    update->deletedCallback = NULL;
    update->arg = this;
    th10::g_Chain->AddToCalcChain(update, 28);
    updateTask = update;

    th10::ChainElem *draw = new th10::ChainElem;
    draw->flagBits.heapAllocated = 1;
    draw->flags |= 2;
    draw->callback = reinterpret_cast<th10::ChainCallback>(th10::leaf::Fn0040BA80);
    draw->addedCallback = NULL;
    draw->deletedCallback = NULL;
    draw->arg = this;
    th10::g_Chain->AddToDrawChain(draw, 6);
    drawTask = draw;

    int &auxiliaryId = g_AsciiManagerView->auxiliaryVm89A4;
    AnmFloat3View position(480.0f, 392.0f, 0.0f);
    if (auxiliaryId == 0)
        auxiliaryId = GetEndingAsciiResource(g_AsciiManagerView)->
            CreateVmAtScreenVariant0(6, &position).value;

    EndingGameCountersView *counters =
        reinterpret_cast<EndingGameCountersView *>(&g_GameScoreState);
    endingIndex = counters->character * 3 + counters->shotType;
    if (counters->continueState == 0 && counters->difficulty >= 1)
        endingIndex += 6;
    if (GetEndingUnlocks()->seen[endingIndex] == 0)
        SetEndingFlag(flags, 1);
    if (GetEndingUnlocks()->alternateSeen == 0)
        SetEndingFlag(flags, 2);
    GetEndingUnlocks()->seen[endingIndex] = 1;
    if (endingIndex >= 6)
        GetEndingUnlocks()->alternateSeen = 1;

    const char *scriptPath = g_EndingScriptPaths[endingIndex];
    g_StageFilePath[0] = '\0';
    strcat(g_StageFilePath, scriptPath);
    scriptData = FileSystem::OpenFile(g_StageFilePath, NULL, 0);
    if (!scriptData)
    {
        th10::g_GameErrorContext.Log(g_EndingDataErrorMessage);
        return -1;
    }
    text = new EndingTextVmView(EndingScriptStart(scriptData));
    return 0;
}

EndingView::~EndingView()
{
    th10::ChainElem *task = updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    g_AnmRenderManagerView->ReleaseSurface(0);
    delete text;
    text = NULL;
    ReleaseEndingBank(29);
    ReleaseEndingBank(30);
    ReleaseEndingBank(31);
    ReleaseEndingBank(32);
    if (scriptData)
    {
        free(scriptData);
        scriptData = NULL;
    }
    scriptData = NULL;
    g_EndingView = NULL;
    g_GameStartupCounter = 0;
}

EndingView *EndingCreate()
{
    EndingView *ending = new EndingView;
    if (ending->Initialize() != 0)
    {
        delete ending;
        return NULL;
    }
    return ending;
}
void EndingDelete(EndingView *ending) { delete ending; }

int EndingView::Update()
{
    if (text->Tick() != 0)
    {
        g_MainSupervisorView.nextGameMode =
            (g_MainSupervisorView.flags & 0x1000) ? 2 : 15;
        return 1;
    }
    // The native ending frame gate uses a wrapping 32-bit INC before the
    // signed modulo-twelve skip check.
    const unsigned int nextSkipBits =
        static_cast<unsigned int>(skipCounter) + 1u;
    memcpy(&skipCounter, &nextSkipBits, sizeof skipCounter);
    if ((text->flags & 4) == 0 && (flags & 2) == 0 &&
        (text->flags & 2) != 0 && (g_FrontEndInput.current & 0x100) != 0)
        if (skipCounter % 12 != 0)
            return 6;
    return 1;
}
int __fastcall EndingUpdateCallback(EndingView *ending) { return ending->Update(); }

EndingTextVmView::EndingTextVmView(unsigned char *command)
{
    memset(this, 0, sizeof(*this));
    for (unsigned int i = 0; i < 5; ++i)
    {
        AnmLoadedView *bank = g_AsciiManagerView->textAnm;
        AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
        vm->renderLayer = 15;
        vm->flags35C |= 0x40000000u;
        bank->InitializeVm(vm, i + 0x42);
        AnmVmIdView created = g_AnmRenderManagerView->AddVmVariant0(vm);
        textIds[i] = created.value;
        AnmRenderManagerView *manager = g_AnmRenderManagerView;
        ResolveEndingTextVm(manager, textIds[i])->glyphWidth = 16;
        ResolveEndingTextVm(manager, textIds[i])->glyphHeight = 16;
        ResolveEndingTextVm(manager, textIds[i])->textFlags360 |= 2;
    }
    instruction = command;
    frameTimer.SetCurrent(0);
    instructionTimer.SetCurrent(0);
    waitTimer.SetCurrent(0);
    flags |= 1;
    color = 0xffffff;
}

int EndingTextVmView::Tick()
{
    if (Run() != 0)
        return 1;
    frameTimer.Tick();
    return 0;
}

int __stdcall EndingTextVmView::Run()
{
    if ((flags & 4) != 0)
        return 0;
    if ((g_EndingView->flags & 1) == 0 && (flags & 2) == 0 &&
        (g_FrontEndInput.current & 0x100) != 0 && (flags & 1) != 0)
        instructionTimer.SetCurrent(EndingRecordTime(instruction));
    while (instructionTimer.current >= EndingRecordTime(instruction))
    {
        switch (instruction[2])
        {
        case 0: return -1;
        case 3:
            if (textLine == 0)
            {
                for (int i = 0; i < 5; ++i)
                {
                    AnmVmView *vm = ResolveEndingTextSlot(
                        reinterpret_cast<AnmVmIdView *>(&textIds[i]));
                    g_AnmRenderManagerView->DrawTextLeft(vm, 0xffffff, " ");
                    ResultMenuPendingThree(reinterpret_cast<AnmVmIdView *>(&textIds[i]));
                }
                AnmVmView *vm = ResolveEndingTextSlot(
                    reinterpret_cast<AnmVmIdView *>(&textIds[0]));
                const char *text = reinterpret_cast<const char *>(
                    th10::leaf::Fn00417010(instruction + 4));
                g_AnmRenderManagerView->DrawTextLeft(vm, color, text);
                PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&textIds[0]));
                ++textLine;
            }
            else
            {
                g_AnmRenderManagerView->DrawTextLeft(
                    reinterpret_cast<AnmVmIdView *>(&textIds[textLine])->GetVm(),
                    color, reinterpret_cast<const char *>(
                        th10::leaf::Fn00417010(instruction + 4)));
                PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&textIds[textLine]));
                // The text-row counter is advanced by native i386 INC,
                // including low-DWORD wrap before its signed five-row guard.
                textLine = static_cast<int>(
                    static_cast<unsigned int>(textLine) + 1u);
                if (textLine >= 5) textLine = 0;
            }
            break;
        case 4:
            {
                AnmVmIdView *id = reinterpret_cast<AnmVmIdView *>(textIds);
                for (int remaining = 5; remaining != 0; --remaining, ++id)
                    ResultMenuPendingThree(id);
            }
            break;
        case 5:
            if (waitTimer.current <= 0) waitTimer.SetCurrent(EndingRecordInt(instruction));
            waitTimer.Add(-1.0f);
            if (EndingRecordInt(instruction) < 0) waitTimer.SetCurrent(999);
            if ((g_FrontEndInput.pressed & 0x1001) == 0 && waitTimer.current > 0)
            {
                if ((g_EndingView->flags & 1) != 0) return 0;
                if ((g_FrontEndInput.current & 0x100) == 0) return 0;
                if (waitTimer.current % 6 != 0) return 0;
                waitTimer.SetCurrent(0);
            }
            else
            {
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(0, 0);
                waitTimer.SetCurrent(0);
            }
            break;
        case 6:
            if (waitTimer.current <= 0) waitTimer.SetCurrent(EndingRecordInt(instruction));
            waitTimer.Add(-1.0f);
            if ((g_FrontEndInput.pressed & 0x1001) == 0 && waitTimer.current > 0)
            {
                if ((g_EndingView->flags & 1) != 0) return 0;
                if ((g_FrontEndInput.current & 0x100) == 0) return 0;
                if (waitTimer.current % 6 != 0) return 0;
            }
            else
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(0, 0);
            waitTimer.SetCurrent(0);
            textLine = 0;
            g_GameStartupCounter = 0;
            break;
        case 7:
            g_AsciiManagerView->EnsureSelectionVm(480.0f, 392.0f);
            // The resource bank index is computed by a wrapping i386 add;
            // keep the terminal script word's low 32 bits on conversion.
            g_AnmRenderManagerView->ReleaseAnm(static_cast<int>(
                static_cast<unsigned int>(EndingRecordInt(instruction)) + 29u));
            flags |= 4;
            backgroundPath = reinterpret_cast<const char *>(instruction + 8);
            worker.Start(this, EndingLoadAnimationCallback);
            instruction = EndingRecordNext(instruction);
            return 0;
        // Creation may change instruction; resolve its destination afterwards.
        case 8:
            {
                int &oldId = animationIds[EndingRecordInt(instruction)];
                g_AnmRenderManagerView->MarkVmForDeletion(AnmVmIdView(oldId));
                oldId = 0;
                const EndingAnimationCommand *command =
                    reinterpret_cast<const EndingAnimationCommand *>(instruction);
                PublishEndingAnimation(this,
                    resources[command->resource]->CreateVmVariant0(command->script, 15));
            }
            break;
        case 15:
            if (g_GuiDifficulty == 1)
            {
                reinterpret_cast<AnmVmIdView *>(
                    &animationIds[EndingRecordInt(instruction)])->Release();
                const EndingAnimationCommand *command =
                    reinterpret_cast<const EndingAnimationCommand *>(instruction);
                PublishEndingAnimation(this,
                    resources[command->resource]->CreateVmVariant0(command->script, 15));
            }
            break;
        case 16:
            if (g_GuiDifficulty == 2)
            {
                reinterpret_cast<AnmVmIdView *>(
                    &animationIds[EndingRecordInt(instruction)])->Release();
                const EndingAnimationCommand *command =
                    reinterpret_cast<const EndingAnimationCommand *>(instruction);
                PublishEndingAnimation(this,
                    resources[command->resource]->CreateVmVariant0(command->script, 15));
            }
            break;
        case 17:
            if (g_GuiDifficulty == 3)
            {
                reinterpret_cast<AnmVmIdView *>(
                    &animationIds[EndingRecordInt(instruction)])->Release();
                const EndingAnimationCommand *command =
                    reinterpret_cast<const EndingAnimationCommand *>(instruction);
                PublishEndingAnimation(this,
                    resources[command->resource]->CreateVmVariant0(command->script, 15));
            }
            break;
        case 9: color = EndingRecordInt(instruction); break;
        case 10:
            FrontEndLoadMusic(0, reinterpret_cast<const char *>(instruction + 4));
            if (strcmp(reinterpret_cast<const char *>(instruction + 4), "bgm/th10_13.wav") == 0)
                FrontEndPlayMusic(0, 15);
            else
                FrontEndPlayMusic(0, 16);
            break;
        case 11:
            GameQueueScaledSoundCommand(3.0f);
            flags &= ~1u;
            break;
        case 12:
            {
                AnmRenderManagerView *manager = g_AnmRenderManagerView;
                for (int i = 0; i < 5; ++i)
                {
                    manager->MarkVmForDeletion(AnmVmIdView(textIds[i]));
                    textIds[i] = 0;
                }
                unsigned char *loaded = g_EndingView->LoadScript(
                    reinterpret_cast<const char *>(instruction + 4));
                if (!loaded) return -1;
                memset(this, 0, sizeof(*this));
                instruction = EndingScriptStart(loaded);
                frameTimer.SetCurrent(0);
                instructionTimer.SetCurrent(0);
                waitTimer.SetCurrent(0);
                color = 0xffffff;
                flags |= 2;
                continue;
            }
        case 13: StageCreateTransitionEffect(0x31, 0, EndingRecordInt(instruction), 0, 0, 0); break;
        case 14: StageCreateTransitionEffect(0x31, 5, EndingRecordInt(instruction), 0, 0, 0); break;
        }
        instruction = EndingRecordNext(instruction);
    }
    instructionTimer.Tick();
    return 0;
}

int EndingLoadAnimationCallback()
{
    g_EndingView->text->LoadAnimation();
    return 0;
}
int EndingTextVmView::LoadAnimation()
{
    const char *path = backgroundPath;
    g_StageFilePath[0] = '\0';
    strcat(g_StageFilePath, path);
    AnmLoadedView *bank = g_AnmRenderManagerView->PreloadAnm(
        *reinterpret_cast<const int *>(instruction + 4) + 29, g_StageFilePath);
    resources[EndingRecordInt(instruction)] = bank;
    flags &= ~4u;
    int *const id = &g_AsciiManagerView->auxiliaryVm89A4;
    EnemyMarkPendingInterrupt(id);
    *id = 0;
    return 0;
}
