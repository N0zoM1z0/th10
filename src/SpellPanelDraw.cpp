#include "EnemyGameState.hpp"

// TH10 0x004091C0: draw the eight score-digit VMs and the five auxiliary
// VMs only when the spell-panel's draw-enabled flag is set. Each Draw is
// performed even if an individual VM is invisible; that check is its callee's
// responsibility. Original private receiver register: EBX.
int EnemyGameStateView::DrawPanelVms()
{
    if ((flags378C & 1u) != 0)
    {
        for (int i = 0; i < 8; ++i)
            g_AnmRenderManagerView->Draw(&frontVms[i]);
        for (int i = 0; i < 5; ++i)
            g_AnmRenderManagerView->Draw(&secondaryFrontVms[i]);
    }
    return 1;
}

// TH10 0x00409270, ten-byte registered fastcall adapter to DrawPanelVms.
// Its emitted register allocation is independently subject to exact proof.
int __fastcall EnemySpellPanelDrawCallback(EnemyGameStateView *state)
{
    return state->DrawPanelVms();
}
