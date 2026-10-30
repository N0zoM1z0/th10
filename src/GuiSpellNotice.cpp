#include "Main.hpp"
#include "Gui.hpp"
#include "GuiScore.hpp"
#include "EnemyGameState.hpp"
#include "EnemySoundQueue.hpp"
#include "GameScoreState.hpp"
#include "GameStatistics.hpp"
#include "ReplayManager.hpp"
#include "VmIdInterrupt.hpp"

struct SpellCaptureRecordView
{
    char name[0x80];
    int captures;
    int attempts;
    unsigned char unknown88[8];
};
typedef char SpellCaptureRecordLayout[
    (sizeof(SpellCaptureRecordView) == 0x90 &&
     offsetof(SpellCaptureRecordView, captures) == 0x80 &&
     offsetof(SpellCaptureRecordView, attempts) == 0x84) ? 1 : -1];

struct GameChainOwnerView;
extern GameChainOwnerView *g_GameAnimationSlots;

// Physical owner includes640 code bytes and the following28-byte switch table.
// Complete maintained source remains a nonexact660-byte native candidate.
// The target passes kind in EAX and the GUI/value on the stack. All three
// logical inputs remain ordinary C++ parameters; LTCG selects the private ABI.
void __stdcall GuiShowScoreNotice(int kind, GuiView *gui, int value)
{
    switch (kind)
    {
    case 0:
    {
        g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14);
        memset(&gui->specialVmId9E14, 0, sizeof(gui->specialVmId9E14));
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        gui->specialVmId9E14.value = gui->frontAnm->CreateVmVariant0(0x47, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
        int remaining = value;
        int visible = 0;
        int divisor = 10000000;
        for (int i = 0; i < 8; ++i)
        {
            AnmVmIdView *id = &gui->valueVmIds[i];
            g_AnmRenderManagerView->MarkVmForDeletion(*id);
            memset(id, 0, sizeof(*id));
            AnmLoadedView *loaded = gui->frontAnm;
            AnmVmView *createdVm = g_AnmRenderManagerView->AllocateVm();
            createdVm->renderLayer = 15;
            createdVm->useAlternateRng = 1;
            loaded->InitializeVm(createdVm, i + 0x27);
            AnmVmIdView created = g_AnmRenderManagerView->AddVmVariant0(createdVm);
            const int digit = remaining / divisor;
            const int remainder = remaining % divisor;
            id->value = created.value;
            if (digit != 0)
                visible = 1;
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
            AnmVmView *vm = g_AnmRenderManagerView->FindVm(*id);
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
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
        g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14);
        gui->specialVmId9E14 = AnmVmIdView();
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        gui->specialVmId9E14.value = gui->frontAnm->CreateVmVariant0(0x48, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
        break;
    case 2:
        g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E18);
        gui->specialVmId9E18 = AnmVmIdView();
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        gui->specialVmId9E18.value = gui->frontAnm->CreateVmVariant0(0x49, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
        break;
    case 3:
        g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E18);
        gui->specialVmId9E18 = AnmVmIdView();
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        gui->specialVmId9E18.value = gui->frontAnm->CreateVmVariant0(0x4a, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
        break;
    case 4:
        g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E18);
        gui->specialVmId9E18 = AnmVmIdView();
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        gui->specialVmId9E18.value = gui->frontAnm->CreateVmVariant0(0x4b, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
        break;
    case 5:
        break;
    case 6:
        g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14);
        gui->specialVmId9E14 = AnmVmIdView();
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        gui->specialVmId9E14.value = gui->frontAnm->CreateVmVariant0(0x4c, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
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
            const int groupA = *reinterpret_cast<int *>(
                reinterpret_cast<unsigned char *>(&g_GameScoreState) + 0x28);
            const int groupB = *reinterpret_cast<int *>(
                reinterpret_cast<unsigned char *>(&g_GameScoreState) + 0x2c);
            const int group = groupA * 3 + groupB;
            GameStatisticsRecord *row = &g_GameStatisticsRows->records[group];
            SpellCaptureRecordView *record = reinterpret_cast<SpellCaptureRecordView *>(
                reinterpret_cast<unsigned char *>(row) + state->spellId * 0x90 + 0x59c);
            if (record->captures < 99999)
                ++record->captures;
            row = &g_GameStatisticsRows->records[6];
            record = reinterpret_cast<SpellCaptureRecordView *>(
                reinterpret_cast<unsigned char *>(row) + state->spellId * 0x90 + 0x59c);
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
