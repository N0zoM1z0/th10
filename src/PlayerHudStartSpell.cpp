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

    // The retained raw-register experiment treated EBP as the HUD receiver,
    // but the observed VC7.1 candidate gives EBP to the stack frame.
    // Use the real typed VM pointers and source-level resource API instead.
    for (int digit = 0; digit < 8; ++digit)
        StartAnmLoadedScript(g_GuiView->frontAnm, &frontVms[digit], digit + 58);

    for (int digit = 0; digit < 5; ++digit)
        StartAnmLoadedScript(g_GuiView->frontAnm, &secondaryFrontVms[digit], digit + 66);

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
        StartEntityScript_00404f30(
            &stageVms[0], 12,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        StartEntityScript_00404f30(
            &stageVms[1], 11,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        if (spellIndex >= 2)
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 14);
        else
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 15);
        break;
    case 2:
        StartEntityScript_00404f30(
            &stageVms[0], 14,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        StartEntityScript_00404f30(
            &stageVms[1], 15,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 17);
        break;
    case 3:
        StartEntityScript_00404f30(
            &stageVms[0], 18,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        StartEntityScript_00404f30(
            &stageVms[1], 19,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 21);
        break;
    case 4:
        StartEntityScript_00404f30(
            &stageVms[0], 19,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        StartEntityScript_00404f30(
            &stageVms[1], 20,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 22);
        break;
    case 5:
        StartEntityScript_00404f30(
            &stageVms[0], 12,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        StartEntityScript_00404f30(
            &stageVms[1], 13,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 15);
        break;
    case 6:
        StartEntityScript_00404f30(
            &stageVms[0], 33,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        StartEntityScript_00404f30(
            &stageVms[1], 34,
            g_EnemyManager->typedEffectResources.stageAnimationAnm);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 36);
        break;
    case 7:
        if (g_EnemyChapterState.chapter < 24)
        {
            StartEntityScript_00404f30(
                &stageVms[0], 38,
                g_EnemyManager->typedEffectResources.stageAnimationAnm);
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 40);
        }
        else
        {
            StartEntityScript_00404f30(
                &stageVms[0], 27,
                g_EnemyManager->typedEffectResources.stageAnimationAnm);
            StartEntityScript_00404f30(
                &stageVms[1], 28,
                g_EnemyManager->typedEffectResources.stageAnimationAnm);
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 30);
        }
        break;
    }
}
#pragma code_seg()
