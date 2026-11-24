// Panel copy and appearance source informed by th10-decomp (MIT),
// commit db6797d245afd2e58b2cd31b60d2857499d3aa7c. See docs/licenses/th10-decomp-MIT.txt.

#include "Main.hpp"
#include "EnemyGameState.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"
#include "Enemy.hpp"
#include "GameStatistics.hpp"
#include "GameScoreState.hpp"
#include "Gui.hpp"
#include "AnmLoadedStartScript.hpp"
#include "ReplayManager.hpp"

extern EnemyManagerView *g_EnemyManager;

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

static inline void CopyPanelName(char *const destination, const char *const source)
{
    const char *cursor = source;
    char *output = destination;
    while ((*output++ = *cursor++) != 0)
        ;
}


struct SpellPanelAppearance
{
    int renderLayer;
    unsigned int flags;
};

static inline AnmVmIdView CreatePanelAnimation(
    AnmLoadedView *resource, int script, SpellPanelAppearance appearance)
{
    AnmVmView *object = g_AnmRenderManagerView->AllocateVm();
    object->renderLayer = appearance.renderLayer;
    object->flags35C |= appearance.flags;
    resource->InitializeVm(object, script);
    return g_AnmRenderManagerView->AddVmVariant0(object);
}

static inline AnmVmIdView CreatePanelAnimation(
    AnmLoadedView *resource, int script)
{
    SpellPanelAppearance appearance = {15, 0x40000000u};
    return CreatePanelAnimation(resource, script, appearance);
}

static __forceinline AnmVmView *FindInline(int handle)
{
    if (handle == 0)
        return 0;

    AnmVmLayerNodeView *node = g_AnmRenderManagerView->primaryVmListHead;
    while (node != 0)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == handle)
            return static_cast<AnmVmView *>(node->owner);
        node = node->next;
    }

    node = g_AnmRenderManagerView->secondaryVmListHead;
    while (node != 0)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == handle)
            return static_cast<AnmVmView *>(node->owner);
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
        if (static_cast<AnmVmView *>(link->owner)->scriptIndex == script)
            return static_cast<AnmVmView *>(link->owner);
    }
    return 0;
}

void __stdcall EnemyGameStateView::StartSpell(
    int index, const char *name, int duration, int)
{
    const int spellIndex = index;
    spellTimer.SetCurrent(0);
    spellId = spellIndex;

    const char *panelName = name;
    char *panelNameDestination = spellName;
    while ((*panelNameDestination++ = *panelName++) != 0)
        ;

    flags378C = (flags378C & ~0x18u) | 3u;

    if (g_ReplayManager->mode != REPLAY_MANAGER_PLAYBACK)
    {
        GameStatisticsOwner *statistics = g_GameStatisticsRows;
        GameStatisticsSpellHistoryEntryView *record =
            statistics->records[
                g_GameScoreState.history.statisticsGroupA * 3 +
                g_GameScoreState.history.statisticsGroupB].SpellHistoryAt(spellIndex);
        CopySpellRecordName(record->name, name);

        record = statistics->records[
            g_GameScoreState.history.statisticsGroupA * 3 +
            g_GameScoreState.history.statisticsGroupB].SpellHistoryAt(spellIndex);
        CountSpellAttempt(record);

        CopyPanelName(statistics->records[6].SpellHistoryAt(spellIndex)->name, name);
        CountSpellAttempt(statistics->records[6].SpellHistoryAt(spellIndex));
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

    // Retail 0x00409681 calls the canonical right-aligned ANM text renderer.
    // The loaded panel name is its format argument, not a separate %s value.
    g_AnmRenderManagerView->DrawTextRight(
        GetPanelAnimation(panelVmIds[1].value), 0x00FFFFFFu,
        name);

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
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[0], 12);
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[1], 11);
        if (spellIndex >= 2)
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 14);
        else
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 15);
        break;
    case 2:
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[0], 14);
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[1], 15);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 17);
        break;
    case 3:
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[0], 18);
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[1], 19);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 21);
        break;
    case 4:
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[0], 19);
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[1], 20);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 22);
        break;
    case 5:
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[0], 12);
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[1], 13);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 15);
        break;
    case 6:
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[0], 33);
        AnmVmStartScript(
            g_EnemyManager->typedEffectResources.stageAnimationAnm,
            &stageVms[1], 34);
        CreatePanelAnimation(
            g_EnemyManager->typedEffectResources.stageAnimationAnm, 36);
        break;
    case 7:
        if (g_EnemyChapterState.chapter < 24)
        {
            AnmVmStartScript(
                g_EnemyManager->typedEffectResources.stageAnimationAnm,
                &stageVms[0], 38);
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 40);
        }
        else
        {
            AnmVmStartScript(
                g_EnemyManager->typedEffectResources.stageAnimationAnm,
                &stageVms[0], 27);
            AnmVmStartScript(
                g_EnemyManager->typedEffectResources.stageAnimationAnm,
                &stageVms[1], 28);
            CreatePanelAnimation(
                g_EnemyManager->typedEffectResources.stageAnimationAnm, 30);
        }
        break;
    }
}
