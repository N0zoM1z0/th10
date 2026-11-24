#include "Main.hpp"
#include "Gui.hpp"
#include "GuiScore.hpp"
#include "EnemyGameState.hpp"
#include "EnemySoundQueue.hpp"
#include "GameScoreState.hpp"
#include "GameStatistics.hpp"
#include "ReplayManager.hpp"
#include "VmIdInterrupt.hpp"

struct GameChainOwnerView;
extern GameChainOwnerView *g_GameAnimationSlots;

static __forceinline AnmVmIdView CreateScoreNoticeDigit(
    AnmLoadedView *loaded, int scriptIndex)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    unsigned int flags = vm->flags35C;
    vm->renderLayer = 15;
    vm->flags35C = flags | 0x40000000u;
    loaded->InitializeVm(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant0(vm);
}

// Physical owner: 640 code bytes plus a 28-byte switch table. The target
// passes kind in EAX and the GUI/value on the stack.
void __stdcall GuiShowScoreNotice(int kind, GuiView *gui, int value)
{
    switch (kind)
    {
    case 0:
    {
        gui->specialVmId9E14.MarkForDeletionAndClear();
        gui->specialVmId9E14.value = gui->frontAnm->CreateVmVariant0(0x47, 15).value;
        int visible = 0;
        int remaining = value;
        int divisor = 10000000;
        for (value = 0; value < 8; ++value)
        {
            AnmVmIdView *id = &gui->valueVmIds[value];
            id->MarkForDeletionAndClear();
            *id = CreateScoreNoticeDigit(gui->frontAnm, 0x27 + value);
            int digit = remaining / divisor;
            int remainder = remaining % divisor;
            if (digit != 0)
                visible = 1;
            AnmVmView *vm = g_AnmRenderManagerView->FindVm(*id);
            if (vm != NULL)
                vm->anmFile->SetSprite(vm, digit + 8);
            if (visible == 0)
                id->ClearFlag2();
            else
                id->SetFlag2();
            remaining = remainder;
            divisor /= 10;
        }
        break;
    }
    case 1:
        gui->specialVmId9E14.MarkForDeletionAndClear();
        gui->specialVmId9E14.value = gui->frontAnm->CreateVmVariant0(0x48, 15).value;
        break;
    case 2:
        gui->specialVmId9E18.MarkForDeletionAndClear();
        gui->specialVmId9E18.value = gui->frontAnm->CreateVmVariant0(0x49, 15).value;
        break;
    case 3:
        gui->specialVmId9E18.MarkForDeletionAndClear();
        gui->specialVmId9E18.value = gui->frontAnm->CreateVmVariant0(0x4a, 15).value;
        break;
    case 4:
        gui->specialVmId9E18.MarkForDeletionAndClear();
        gui->specialVmId9E18.value = gui->frontAnm->CreateVmVariant0(0x4b, 15).value;
        break;
    case 6:
        gui->specialVmId9E14.MarkForDeletionAndClear();
        gui->specialVmId9E14.value = gui->frontAnm->CreateVmVariant0(0x4c, 15).value;
        break;
    }
}

// Target 0x00409C00. ECL's actual spell-end case supplies this receiver.
void EnemyEndSpell(EnemyGameStateView *state)
{
    if ((state->flags378C & 1) == 0)
        return;
    *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(g_GameAnimationSlots) + 0x2a18) |= 1;
    EnemyMarkPendingInterrupt(&state->panelVmIds[0].value);
    EnemyMarkPendingInterrupt(&state->panelVmIds[1].value);
    EnemyMarkPendingInterrupt(&state->panelVmIds[2].value);
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    state->flags378C &= ~1u;
    AnmVmIdView *managedId = reinterpret_cast<AnmVmIdView *>(&state->managedVmId);
    manager->MarkVmForDeletion(*managedId);
    *managedId = AnmVmIdView();
    if ((state->flags378C & 2) != 0)
    {
        g_GuiScore.Add(state->currentSpellBonus);
        GuiShowScoreNotice(0, g_GuiView, state->currentSpellBonus);
        if (g_ReplayManager->mode != REPLAY_MANAGER_PLAYBACK)
        {
            const int groupA = g_GameScoreState.history.statisticsGroupA;
            const int groupB = g_GameScoreState.history.statisticsGroupB;
            const int group = groupA * 3 + groupB;
            GameStatisticsRecord *row = &g_GameStatisticsRows->records[group];
            GameStatisticsSpellHistoryEntryView *record =
                row->SpellHistoryAt(state->spellId);
            if (record->captures < 99999)
                ++record->captures;
            row = &g_GameStatisticsRows->records[6];
            record = row->SpellHistoryAt(state->spellId);
            if (record->captures < 99999)
                ++record->captures;
        }
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(45, 0);
    }
    else
    {
        GuiView *gui = g_GuiView;
        AnmVmIdView *id = &gui->specialVmId9E14;
        g_AnmRenderManagerView->MarkVmForDeletion(*id);
        memset(id, 0, sizeof(*id));
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        id->value = gui->frontAnm->CreateVmVariant0(0x48, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
    }
}
