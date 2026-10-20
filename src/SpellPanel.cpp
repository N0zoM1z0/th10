#include "Main.hpp"
#include "EnemyGameState.hpp"
#include "Enemy.hpp"
#include "Gui.hpp"
#include "StageScript.hpp"

struct GameChainOwnerView;
extern EnemyManagerView *g_EnemyManager;        // target477704
extern GameChainOwnerView *g_GameAnimationSlots; // target4776E8
extern unsigned char *g_EnemySpellStatistics;   // target47783C
extern int g_GameStatisticsIndex0;              // target474C68
extern int g_GameStatisticsIndex1;              // target474C6C

// Target408D60. Names describe the recovered panel role, not original symbols.
int EnemyGameStateView::UpdateSpellPanel()
{
    if ((flags378C & 1u) == 0)
        return 1;
    if (spellTimer.current >= 60)
        reinterpret_cast<StageScriptView *>(g_GameAnimationSlots)->flags &= ~1u;
    if (spellTimer.current >= 300 && (flags378C & 8u) == 0) {
        currentSpellBonus -= (initialSpellBonus - initialSpellBonus / 10) /
            (spellDuration - 300);
        currentSpellBonus -= currentSpellBonus % 10;
    }

    AnmRenderManagerView::ExecuteScript(&stageVms[0]);
    AnmRenderManagerView::ExecuteScript(&stageVms[1]);
    spellTimer.Tick();
    if (spellTimer.current >= 120) {
        if ((flags378C & 4u) == 0) {
            if (g_Player->drawPosition.y < 96.0f) {
                for (int i = 0; i < 3; ++i)
                    panelVmIds[i].SetInterrupt(3);
                for (int i = 0; i < 8; ++i)
                    frontVms[i].pendingInterrupt = 3;
                for (int i = 0; i < 5; ++i)
                    secondaryFrontVms[i].pendingInterrupt = 3;
                flags378C |= 4u;
            }
        } else if (g_Player->drawPosition.y > 128.0f) {
            for (int i = 0; i < 3; ++i)
                panelVmIds[i].SetInterrupt(2);
            for (int i = 0; i < 8; ++i)
                frontVms[i].pendingInterrupt = 2;
            for (int i = 0; i < 5; ++i)
                secondaryFrontVms[i].pendingInterrupt = 2;
            flags378C &= ~4u;
        }
    }

    if ((flags378C & 2u) != 0) {
        int value = currentSpellBonus;
        int divisor = 10000000;
        for (int i = 0; i < 8; ++i) {
            g_GuiView->frontAnm->SetSprite(
                &frontVms[i], value / divisor + 30);
            AnmRenderManagerView::ExecuteScript(&frontVms[i]);
            value %= divisor;
            divisor /= 10;
        }
    } else {
        g_GuiView->frontAnm->SetSprite(&frontVms[2], 41);
        AnmRenderManagerView::ExecuteScript(&frontVms[2]);
    }

    int record = g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3;
    int count = *reinterpret_cast<int *>(g_EnemySpellStatistics +
        record * 0x437c + spellId * 0x90 + 0x624);
    if (count >= 100)
        count = 99;
    // The target captures one GUI owner for all four counter writes, while
    // resampling its ANM resource and the second statistics record afterward.
    GuiView *gui = g_GuiView;
    gui->frontAnm->SetSprite(&secondaryFrontVms[0], count / 10 + 30);
    gui->frontAnm->SetSprite(&secondaryFrontVms[1], count % 10 + 30);
    record = g_GameStatisticsIndex1 + g_GameStatisticsIndex0 * 3;
    count = *reinterpret_cast<int *>(g_EnemySpellStatistics +
        record * 0x437c + spellId * 0x90 + 0x628);
    if (count >= 100)
        count = 99;
    gui->frontAnm->SetSprite(&secondaryFrontVms[3], count / 10 + 30);
    gui->frontAnm->SetSprite(&secondaryFrontVms[4], count % 10 + 30);
    for (int i = 0; i < 5; ++i)
        AnmRenderManagerView::ExecuteScript(&secondaryFrontVms[i]);

    const PlayerFloat3 &position =
        g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position;
    effectPosition += (AnmFloat3View(position.x, position.y, position.z) -
                       effectPosition) * 0.05f;
    g_AnmRenderManagerView->SetVmWorldPosition(AnmVmIdView(managedVmId), &effectPosition);
    return 1;
}

// Target409220 is the registered fastcall adapter. The target uses EDI for
// the member receiver; the native compiler context is still unresolved.
// Original adapter origin is unknown.
int __fastcall EnemySpellPanelUpdateCallback(EnemyGameStateView *state)
{
    return state->UpdateSpellPanel();
}
