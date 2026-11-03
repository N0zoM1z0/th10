#pragma once
#include "AnmManager.hpp"
#include <stddef.h>
namespace th10 { struct ChainElem; }

// Shared partial view of the spell-card/panel owner, target global4776F4.
// Original class/data ownership remains open; all fields below have TH10 evidence.
// The native panel constructor does NOT run AnmVmIdView's default
// constructor on these three slots. They are bitwise storage until the
// subsequent full-object clear, then are used by VM-ID operations.
struct SpellPanelIdSlotsView
{
    unsigned int slots[3];
    AnmVmIdView &operator[](unsigned int index)
    {
        return *reinterpret_cast<AnmVmIdView *>(&slots[index]);
    }
};
typedef char SpellPanelIdSlotsSizeIs0C[
    sizeof(SpellPanelIdSlotsView) == 0x0c ? 1 : -1];

struct EnemyGameStateView
{
    unsigned int flags;
    unsigned int unknown004;
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    AnmVmView stageVms[2];
    SpellPanelIdSlotsView panelVmIds;
    int managedVmId;
    AnmVmView frontVms[8];
    AnmVmView secondaryFrontVms[5];
    AnmVmTimerView spellTimer;
    char spellName[0x40];
    int spellId;
    unsigned int flags378C;
    int currentSpellBonus;
    int initialSpellBonus;
    int spellDuration;
    AnmFloat3View effectPosition;
    unsigned int unknown37A8;
    th10::ChainElem *auxiliaryDrawTask;

    EnemyGameStateView();
    ~EnemyGameStateView();
    int RegisterTasks();

    int UpdateSpellPanel();
    int DrawPanelVms();
    void InvalidateSpellBonus();
};
typedef char EnemySpellEmbeddedVmsAt10[
    (offsetof(EnemyGameStateView, stageVms) == 0x10) ? 1 : -1];
typedef char EnemySpellFrontVmsAt778[
    (offsetof(EnemyGameStateView, frontVms) == 0x778 &&
     offsetof(EnemyGameStateView, secondaryFrontVms) == 0x24d8) ? 1 : -1];
typedef char EnemySpellTimerAt3734[
    (offsetof(EnemyGameStateView, spellTimer) == 0x3734) ? 1 : -1];
typedef char EnemySpellNameAt3748[
    (offsetof(EnemyGameStateView, spellName) == 0x3748 &&
     offsetof(EnemyGameStateView, spellId) == 0x3788) ? 1 : -1];
typedef char EnemySpellValuesAt3790[
    (offsetof(EnemyGameStateView, currentSpellBonus) == 0x3790 &&
     offsetof(EnemyGameStateView, initialSpellBonus) == 0x3794 &&
     offsetof(EnemyGameStateView, spellDuration) == 0x3798 &&
     offsetof(EnemyGameStateView, effectPosition) == 0x379c) ? 1 : -1];
typedef char EnemyGameStateVmIdAt774[
    (offsetof(EnemyGameStateView, managedVmId) == 0x774) ? 1 : -1];
typedef char EnemyGameStateFlagsAt378C[
    (offsetof(EnemyGameStateView, flags378C) == 0x378c) ? 1 : -1];

typedef char EnemyGameStateTasksAndSize[
    (offsetof(EnemyGameStateView, updateTask) == 8 &&
     offsetof(EnemyGameStateView, drawTask) == 0x0c &&
     offsetof(EnemyGameStateView, auxiliaryDrawTask) == 0x37ac &&
     sizeof(EnemyGameStateView) == 0x37b0) ? 1 : -1];

extern EnemyGameStateView *g_EnemyGameState;
EnemyGameStateView *CreateEnemyGameState();
int __fastcall EnemySpellPanelUpdateCallback(EnemyGameStateView *state);
int __fastcall EnemySpellPanelDrawCallback(EnemyGameStateView *state);
