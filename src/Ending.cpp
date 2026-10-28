#include "Main.hpp"
#include "Ending.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "InputState.hpp"
#include "EnemySoundQueue.hpp"
#include "TransitionEffect.hpp"
#include <stdlib.h>
#include <string.h>

// Target observations determine this recovery. Reference hypotheses from
// levzzz5154/th10-decomp (MIT), copyright(c)2026 levzzz, c6791d82dc9f02e55eca45a5314f900a85481efd;
// license: docs/licenses/th10-decomp-MIT.txt. No reference volatile casts,
// forced helper controls, assembly or explicit return-output stack slots.
extern char g_StageFilePath[];                  //497C38, capacity unknown
extern const char *g_EndingScriptPaths[12];      //474440, external data owner
extern char g_EndingDataErrorMessage[];          //46CB68, external message
extern unsigned char *g_GuiPersistentData;      //47783C
extern int g_GuiCharacter;                     //474C68
extern int g_GuiShotType;                      //474C6C
extern int g_GuiDifficulty;                    //474C74
extern int g_GameStartupValue474C90;
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
    AnmVmView *vm = ResolveEndingTextVm(manager, id);
    if (vm)
    {
        vm->flags35C |= 0x04000000u;
        if (!vm->layerNode.previous)
            for (AnmVmLayerNodeView *node = vm->layerNode.next; node; node = node->next)
                static_cast<AnmVmView *>(node->owner)->flags35C |= 0x04000000u;
    }
    id = 0;
}
// File header+4 supplies the first instruction offset.
inline unsigned char *EndingScriptStart(unsigned char *file)
{
    return file + EndingRecordInt(file);
}
// Creation completes before this helper reads the current destination cursor.
inline void PublishEndingAnimation(EndingTextVmView *text, const AnmVmIdView &created)
{
    text->animationIds[EndingRecordInt(text->instruction)] = created.value;
}
// Observed common node construction; no extra fields or virtual interface.
struct EndingTaskView : th10::ChainElem
{
    EndingTaskView()
    {
        callback = NULL;
        addedCallback = NULL;
        deletedCallback = NULL;
        prio = 0;
        flags &= ~1u;
        self = this;
        prev = next = NULL;
    }
};
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

int EndingView::Initialize()
{
    th10::ChainElem *task = new EndingTaskView;
    task->flags |= 3;
    task->callback = reinterpret_cast<th10::ChainCallback>(EndingUpdateCallback);
    task->deletedCallback = task->addedCallback = NULL;
    task->arg = this;
    th10::Chain::AddToCalcChain(task, 28, th10::g_Chain);
    updateTask = task;
    task = new EndingTaskView;
    task->flags |= 3;
    task->callback = reinterpret_cast<th10::ChainCallback>(th10::leaf::Fn0040BA80);
    task->deletedCallback = task->addedCallback = NULL;
    task->arg = this;
    th10::Chain::AddToDrawChain(task, 6, th10::g_Chain);
    drawTask = task;

    AnmFloat3View position(480.0f, 392.0f, 0.0f);
    int &auxiliaryId = g_AsciiManagerView->auxiliaryVm89A4;
    if (auxiliaryId == 0)
        auxiliaryId =
            g_AsciiManagerView->asciiAnm->CreateVmAtScreenVariant0(6, &position).value;
    endingIndex = g_GuiShotType + g_GuiCharacter * 3;
    if (g_GameStartupValue474C90 == 0 && g_GuiDifficulty >= 1)
        endingIndex += 6;
    if (g_GuiPersistentData[0x1d882 + endingIndex] == 0)
        flags |= 1;
    if (g_GuiPersistentData[0x1d88e] == 0)
        flags |= 2;
    g_GuiPersistentData[0x1d882 + endingIndex] = 1;
    if (endingIndex >= 6)
        g_GuiPersistentData[0x1d88e] = 1;
    g_StageFilePath[0] = '\0';
    strcat(g_StageFilePath, g_EndingScriptPaths[endingIndex]);
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
        th10::Chain::UnregisterElem(task, chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(task, chain);
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
    ++skipCounter;
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
        unsigned char *command = instruction;
        switch (command[2])
        {
        case 0: return -1;
        case 3:
            if (textLine == 0)
            {
                for (int i = 0; i < 5; ++i)
                {
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    AnmVmView *vm = manager->FindVm(AnmVmIdView(textIds[i]));
                    if (!vm) textIds[i] = 0;
                    manager->DrawTextLeft(vm, 0xffffff, " ");
                    ResultMenuPendingThree(reinterpret_cast<AnmVmIdView *>(&textIds[i]));
                }
                AnmRenderManagerView *manager = g_AnmRenderManagerView;
                AnmVmView *vm = manager->FindVm(AnmVmIdView(textIds[0]));
                if (!vm) textIds[0] = 0;
                const char *text = reinterpret_cast<const char *>(
                    th10::leaf::Fn00417010(instruction + 4));
                manager->DrawTextLeft(vm, color, text);
                PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&textIds[0]));
                ++textLine;
            }
            else
            {
                const char *text = reinterpret_cast<const char *>(
                    th10::leaf::Fn00417010(command + 4));
                unsigned int currentColor = color;
                g_AnmRenderManagerView->DrawTextLeft(reinterpret_cast<AnmVmIdView *>(&textIds[textLine])->GetVm(), currentColor, text);
                PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&textIds[textLine]));
                if (++textLine >= 5) textLine = 0;
            }
            break;
        case 4:
            for (int i = 0; i < 5; ++i) ResultMenuPendingThree(reinterpret_cast<AnmVmIdView *>(&textIds[i]));
            break;
        case 5:
            if (waitTimer.current <= 0) waitTimer.SetCurrent(EndingRecordInt(command));
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
            if (waitTimer.current <= 0) waitTimer.SetCurrent(EndingRecordInt(command));
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
            g_AnmRenderManagerView->ReleaseAnm(EndingRecordInt(instruction) + 29);
            backgroundPath = reinterpret_cast<const char *>(instruction + 8);
            flags |= 4;
            worker.Start(this, EndingLoadAnimationCallback);
            instruction = EndingRecordNext(instruction);
            return 0;
        // Creation may change instruction; resolve its destination afterwards.
        case 8:
            {
                int &oldId = animationIds[EndingRecordInt(command)];
                g_AnmRenderManagerView->MarkVmForDeletion(AnmVmIdView(oldId));
                oldId = 0;
                PublishEndingAnimation(this,
                    resources[EndingRecordInt(instruction, 1)]->
                        CreateVmVariant0(EndingRecordInt(instruction, 2), 15));
            }
            break;
        case 15:
            if (g_GuiDifficulty == 1)
            {
                reinterpret_cast<AnmVmIdView *>(
                    &animationIds[EndingRecordInt(instruction)])->Release();
                PublishEndingAnimation(this,
                    resources[EndingRecordInt(instruction, 1)]->
                        CreateVmVariant0(EndingRecordInt(instruction, 2), 15));
            }
            break;
        case 16:
            if (g_GuiDifficulty == 2)
            {
                reinterpret_cast<AnmVmIdView *>(
                    &animationIds[EndingRecordInt(instruction)])->Release();
                PublishEndingAnimation(this,
                    resources[EndingRecordInt(instruction, 1)]->
                        CreateVmVariant0(EndingRecordInt(instruction, 2), 15));
            }
            break;
        case 17:
            if (g_GuiDifficulty == 3)
            {
                reinterpret_cast<AnmVmIdView *>(
                    &animationIds[EndingRecordInt(instruction)])->Release();
                PublishEndingAnimation(this,
                    resources[EndingRecordInt(instruction, 1)]->
                        CreateVmVariant0(EndingRecordInt(instruction, 2), 15));
            }
            break;
        case 9: color = EndingRecordInt(command); break;
        case 10:
            FrontEndLoadMusic(0, reinterpret_cast<const char *>(command + 4));
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
        case 13: StageCreateTransitionEffect(0x31, 0, EndingRecordInt(command), 0, 0, 0); break;
        case 14: StageCreateTransitionEffect(0x31, 5, EndingRecordInt(command), 0, 0, 0); break;
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
    g_StageFilePath[0] = '\0';
    strcat(g_StageFilePath, backgroundPath);
    AnmLoadedView *bank = g_AnmRenderManagerView->PreloadAnm(
        EndingRecordInt(instruction) + 29, g_StageFilePath);
    AsciiManagerView *ascii = g_AsciiManagerView;
    resources[EndingRecordInt(instruction)] = bank;
    flags &= ~4u;
    EnemyMarkPendingInterrupt(&ascii->auxiliaryVm89A4);
    ascii->auxiliaryVm89A4 = 0;
    return 0;
}
