#include "AnmRectangle.hpp"
#include "Gui.hpp"
#include "Enemy.hpp"

// Existing target storage identities; no parallel storage is introduced.
extern EnemyManagerView *g_EnemyManager; // 0x00477704
extern int g_GuiState4C58;               // 0x00474C58

typedef char GuiDrawEnemySlotAt10[
    (offsetof(EnemyManagerView, specialEnemySlots) == 0x10) ? 1 : -1];
typedef char GuiDrawEnemyFlagsAt2480[
    (offsetof(EnemyFullObjectView, runtime) +
     offsetof(EnemyRuntimeView, flags) == 0x2480) ? 1 : -1];
typedef char GuiDrawStateAlphaAt8393[
    (offsetof(GuiView, displayVms5) +
     offsetof(AnmVmView, primaryColor) +
     offsetof(AnmColorView, alpha) == 0x8393) ? 1 : -1];

// Complete stage GUI draw owner 0x00415800-0x00415AD3 (724 bytes).
// Resource callbacks may change global managers, GUI values, or rectangle
// contents. Preserve each target reread and the stored right-edge boundaries.
// Ordinary C++ float expressions here do not establish native x87 precision.
int __stdcall GuiView::DrawStageElements(GuiView *gui)
{
    int i;
    for (i = 0; i < 10; ++i)
    {
        g_AnmRenderManagerView->Draw(&gui->displayVms0[i]);
        g_AnmRenderManagerView->Draw(&gui->displayVms1[i]);
    }
    for (i = 0; i < 9; ++i)
        g_AnmRenderManagerView->Draw(&gui->displayVms2[i]);
    for (i = 0; i < 4; ++i)
        g_AnmRenderManagerView->Draw(&gui->displayVms3[i]);
    for (i = 0; i < 7; ++i)
        g_AnmRenderManagerView->Draw(&gui->displayVms5[i]);

    AnmFloatRectView rectangle;
    unsigned int color;
    if (g_GuiState4C58 != 0)
    {
        color = static_cast<unsigned int>(
            gui->displayVms5[0].primaryColor.alpha) << 24;
        rectangle.left = 51.0f;
        rectangle.top = 461.0f;
        rectangle.right = g_GuiState4C58 *
            0.29230770468711853f + 51.0f;
        rectangle.bottom = 463.0f;
        DrawColoredRectangle(&rectangle, color);

        color = (static_cast<unsigned int>(
            gui->displayVms5[0].primaryColor.alpha) << 24) | 0x00ffffffu;
        rectangle.left = 50.0f;
        rectangle.top = 460.0f;
        rectangle.right = rectangle.right - 1.0f;
        rectangle.bottom = 462.0f;
        DrawColoredRectangle(&rectangle, color);
    }

    if (gui->bossGaugeDisplayed > 0.0f)
    {
        rectangle.left = 41.0f;
        rectangle.top = 23.0f;
        rectangle.right = gui->bossGaugeDisplayed * 342.0f + 41.0f;
        rectangle.bottom = 25.0f;
        DrawColoredRectangle(&rectangle, 0xff000000u);

        rectangle.left = 40.0f;
        rectangle.top = 22.0f;
        rectangle.right = rectangle.right - 1.0f;
        rectangle.bottom = 24.0f;
        DrawColoredRectangle(&rectangle, 0xffffffffu);

        for (i = 0; i < 4; ++i)
        {
            if (gui->bossGaugeValues[i].value != 0.0f)
            {
                const float value = gui->bossGaugeValues[i].value <
                    gui->bossGaugeDisplayed
                    ? gui->bossGaugeValues[i].value : gui->bossGaugeDisplayed;
                color = gui->bossGaugeValues[i].color;
                rectangle.left = 40.0f;
                rectangle.top = 22.0f;
                rectangle.right = value * 342.0f + 40.0f;
                rectangle.bottom = 24.0f;
                DrawColoredRectangle(&rectangle, color);
            }
        }
    }

    EnemyManagerView *manager = g_EnemyManager;
    if (manager != NULL)
    {
        EnemyFullObjectView *boss = manager->specialEnemySlots[0];
        // The target tests inverted bits 4 and 0 from one DWORD flag value.
        if (boss != NULL && ((~(boss->runtime.flags >> 4)) & 1u) != 0 &&
            ((~boss->runtime.flags) & 1u) != 0)
        {
            g_AnmRenderManagerView->Draw(&gui->bossPositionVm);
            manager = g_EnemyManager;
        }
        if (manager != NULL && gui->spellSeconds >= 0 &&
            manager->specialEnemySlots[0] != NULL && gui->message == NULL)
        {
            for (i = 0; i < 2; ++i)
                g_AnmRenderManagerView->Draw(&gui->displayVms4[i]);
        }
    }
    return 1;
}

// Actual registered draw callback at target 0x00415AF0. Its seven code bytes
// are caller context only, not a separately claimed authored owner.
int __fastcall GuiDrawCallback(void *owner)
{
    return GuiView::DrawStageElements(static_cast<GuiView *>(owner));
}
