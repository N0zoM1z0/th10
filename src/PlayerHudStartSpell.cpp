#include "Main.hpp"
#include "EnemyGameState.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"
#include "Enemy.hpp"
#include "GameStatistics.hpp"
#include "GameScoreState.hpp"
#include "Gui.hpp"
#include "AnmLoadedStartScript.hpp"

#include <stddef.h>

extern EnemyManagerView *g_EnemyManager;
extern unsigned char *g_EnemySpellPlaybackState;

struct EnemySpellPlaybackGateView
{
    unsigned char unknown000[0x10];
    int stateCode10;
};
typedef char EnemySpellPlaybackStateAt10[
    (offsetof(EnemySpellPlaybackGateView, stateCode10) == 0x10) ? 1 : -1];

struct RecoveryIndicatorVm_00419120
{
    void __cdecl FormatRightAligned(void *manager, ...);
};

static __forceinline void CountSpellAttempt(GameStatisticsSpellHistoryEntryView *record)
{
    if (record->attempts < 99999)
        ++record->attempts;
}

static __forceinline void CopySpellRecordName(char *destination, const char *source)
{
    char value;
    do
    {
        value = *source++;
        *destination++ = value;
    } while (value != 0);
}

static __forceinline void CopyPanelName(char *destination, const char *source)
{
    const char *cursor = source;
    char *output = destination;
    while ((*output++ = *cursor++) != 0)
        ;
}


static __forceinline AnmVmIdView CreatePanelAnimation(
    AnmLoadedView *resource, int script)
{
    AnmVmView *object = g_AnmRenderManagerView->AllocateVm();
    object->renderLayer = 15;
    object->flags35C |= 0x40000000u;
    resource->InitializeVm(object, script);
    return g_AnmRenderManagerView->AddVmVariant0(object);
}

static __forceinline AnmVmView *FindInline(int handle)
{
    if (handle == 0)
        return 0;

    AnmVmLayerNodeView *node = g_AnmRenderManagerView->primaryVmListHead;
    while (node != 0)
    {
        AnmVmView *object = static_cast<AnmVmView *>(node->owner);
        if (object->id == handle)
            return object;
        node = node->next;
    }

    node = g_AnmRenderManagerView->secondaryVmListHead;
    while (node != 0)
    {
        AnmVmView *object = static_cast<AnmVmView *>(node->owner);
        if (object->id == handle)
            return object;
        node = node->next;
    }
    return 0;
}

static __forceinline AnmVmView *GetPanelAnimation(int &handle)
{
    AnmVmView *object = FindInline(handle);
    if (object == 0)
        handle = 0;
    return object;
}

static __forceinline AnmVmView *FindPanelChild(int &handle, int script)
{
    AnmVmView *object = GetPanelAnimation(handle);
    AnmVmLayerNodeView *link = &object->layerNode;
    for (; link != 0; link = link->next)
    {
        AnmVmView *candidate = static_cast<AnmVmView *>(link->owner);
        if (candidate->scriptIndex == script)
            return candidate;
    }
    return 0;
}

#pragma code_seg(".text$00409280_fn")
void __stdcall EnemyGameStateView::StartSpell(
    int index, const char *name, int duration)
{
    const char *panelNameCursor = name;
    const int spellIndex = index;
    spellTimer.SetCurrent(0);
    spellId = spellIndex;

    CopyPanelName(spellName, panelNameCursor);

    flags378C = (flags378C & ~0x18u) | 3u;

    if (reinterpret_cast<const EnemySpellPlaybackGateView *>(
            g_EnemySpellPlaybackState)->stateCode10 != 1)
    {
        GameStatisticsOwner *statistics = g_GameStatisticsRows;
        GameStatisticsSpellHistoryEntryView *record =
            statistics->records[
                g_GameScoreState.history.statisticsGroupA * 3 +
                g_GameScoreState.history.statisticsGroupB].SpellHistoryAt(spellIndex);
        CopySpellRecordName(record->name, panelNameCursor);

        record = statistics->records[
            g_GameScoreState.history.statisticsGroupA * 3 +
            g_GameScoreState.history.statisticsGroupB].SpellHistoryAt(spellIndex);
        CountSpellAttempt(record);

        record = statistics->records[6].SpellHistoryAt(spellIndex);
        CopyPanelName(record->name, panelNameCursor);
        CountSpellAttempt(record);
    }

    __asm
    {
        mov edi, [ebp+8]
        add edi, 778h
        xor esi, esi
    loadFrontSpellDigits:
        mov ecx, g_GuiView
        mov ecx, [ecx+9ec8h]
        lea ebx, [esi+3ah]
        mov eax, edi
        call LoadScript_0043e710
        inc esi
        add edi, 3ach
        cmp esi, 8
        jl loadFrontSpellDigits

        mov edi, [ebp+8]
        add edi, 24d8h
        xor esi, esi
    loadSecondarySpellDigits:
        mov ecx, g_GuiView
        mov ecx, [ecx+9ec8h]
        lea ebx, [esi+42h]
        mov eax, edi
        call LoadScript_0043e710
        inc esi
        add edi, 3ach
        cmp esi, 5
        jl loadSecondarySpellDigits
    }

    panelVmIds[0] = CreatePanelAnimation(g_AsciiManagerView->asciiAnm, 1);
    panelVmIds[1] = CreatePanelAnimation(g_AsciiManagerView->textAnm, 72);
    panelVmIds[2] = CreatePanelAnimation(g_AsciiManagerView->asciiAnm, 2);

    reinterpret_cast<RecoveryIndicatorVm_00419120 *>(
        GetPanelAnimation(panelVmIds[1].value))
        ->FormatRightAligned(g_AnmRenderManagerView, 0x00FFFFFFu, panelNameCursor);

    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
        QueueSoundSample(14, 0);

    managedVmId = CreatePanelAnimation(
        g_EnemyPrimaryResourceOwner->primaryEnemyResource, 417).value;

    effectPosition = *reinterpret_cast<const AnmFloat3View *>(
        &g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position);
    g_AnmRenderManagerView->SetVmWorldPosition(
        AnmVmIdView(managedVmId), &effectPosition);

    FindPanelChild(managedVmId, 415)->intVar2 = duration;
    FindPanelChild(managedVmId, 416)->intVar2 = duration;

    spellDuration = duration;
    currentSpellBonus =
        (g_GameScoreState.selectedStage * 3 + 10) *
        g_GameScoreState.faith * 10;
    initialSpellBonus = currentSpellBonus;
    if (currentSpellBonus >= 100000000)
        initialSpellBonus = 99999999;

    CreatePanelAnimation(
        g_EnemyPrimaryResourceOwner->primaryEnemyResource, 427);

    switch (g_GameScoreState.selectedStage)
    {
    case 1:
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+10h]
            mov eax, 0ch
            call EntityStartScript_00404f30
        }
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+3bch]
            mov eax, 0bh
            call EntityStartScript_00404f30
        }
        if (spellIndex >= 2)
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 14);
        else
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 15);
        break;
    case 2:
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+10h]
            mov eax, 0eh
            call EntityStartScript_00404f30
        }
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+3bch]
            mov eax, 0fh
            call EntityStartScript_00404f30
        }
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 17);
        break;
    case 3:
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+10h]
            mov eax, 12h
            call EntityStartScript_00404f30
        }
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+3bch]
            mov eax, 13h
            call EntityStartScript_00404f30
        }
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 21);
        break;
    case 4:
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+10h]
            mov eax, 13h
            call EntityStartScript_00404f30
        }
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+3bch]
            mov eax, 14h
            call EntityStartScript_00404f30
        }
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 22);
        break;
    case 5:
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+10h]
            mov eax, 0ch
            call EntityStartScript_00404f30
        }
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+3bch]
            mov eax, 0dh
            call EntityStartScript_00404f30
        }
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 15);
        break;
    case 6:
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+10h]
            mov eax, 21h
            call EntityStartScript_00404f30
        }
        __asm
        {
            mov edx, g_EnemyManager
            mov eax, [edx+38h]
            push eax
            lea esi, [edi+3bch]
            mov eax, 22h
            call EntityStartScript_00404f30
        }
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 36);
        break;
    case 7:
        if (g_EnemyChapterState.chapter < 24)
        {
            __asm
            {
                mov edx, g_EnemyManager
                mov eax, [edx+38h]
                push eax
                lea esi, [edi+10h]
                mov eax, 26h
                call EntityStartScript_00404f30
            }
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 40);
        }
        else
        {
            __asm
            {
                mov edx, g_EnemyManager
                mov eax, [edx+38h]
                push eax
                lea esi, [edi+10h]
                mov eax, 1bh
                call EntityStartScript_00404f30
            }
            __asm
            {
                mov edx, g_EnemyManager
                mov eax, [edx+38h]
                push eax
                lea esi, [edi+3bch]
                mov eax, 1ch
                call EntityStartScript_00404f30
            }
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 30);
        }
        break;
    }
}
#pragma code_seg()
