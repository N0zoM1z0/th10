#include "Main.hpp"
#include "Gui.hpp"
#include "GameScoreState.hpp"
#include "EnemySoundQueue.hpp"
#include <string.h>

// Target aliases retain their established logical interfaces. Their shared
// storage and original production translation unit remain open.
extern int g_GuiValue4C90;
extern int g_GuiValue4C94;
extern int g_GuiValue4C9C;
extern int g_GuiDifficulty;
extern unsigned int g_GuiGameFlags;
extern int g_GuiNormalExtendScores[5];
extern int g_GuiExtraExtendScores[3];
extern void GuiSetLivesDisplayCount(GuiView *gui, int count);

// Target 0x004188A0: cap lives at nine, publish a successful extend notice,
// then refresh the life display from the shared global state.
void GameAddLives(int amount, GameScoreStateView *state)
{
    int &lives = *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(state) + 0x30);
    lives += amount;
    if (lives > 9)
    {
        lives = 9;
    }
    else
    {
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(0x2c, 0);
        GuiView *gui = g_GuiView;
        AnmVmIdView *id = &gui->specialVmId9E18;
        g_AnmRenderManagerView->MarkVmForDeletion(*id);
        memset(id, 0, sizeof(*id));
// The target retains the real 0x00448D00 creator call at this site.
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
        id->value = gui->frontAnm->CreateVmVariant0(0x4b, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
    }
    GuiSetLivesDisplayCount(g_GuiView,
        *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(
            &g_GameScoreState) + 0x30));
}

// Complete score-display update at 0x00417040. GameManager's actual caller
// supplies the GuiView; the target's private receiver is ESI.
void GameUpdateGui(GuiView *gui)
{
    int digitBoundary = 100000000;
    if (g_GameScoreState.score != gui->value9E78)
    {
        const int previousScore = gui->value9E78;
        int step = (g_GameScoreState.score - gui->value9E78) / 32;
        if (step >= 0x8d55e)
            step = 0x8d55e;
        else if (step == 0)
            step = 1;
        if (gui->value9E7C < step)
            gui->value9E7C = step;
        if (gui->value9E7C > g_GameScoreState.score - gui->value9E78)
            gui->value9E7C = g_GameScoreState.score - gui->value9E78;
        gui->value9E78 += gui->value9E7C;
        if (gui->value9E78 >= g_GameScoreState.score)
            gui->value9E7C = 0;
        if (gui->value9E78 >= digitBoundary && previousScore < digitBoundary)
            gui->displayVms1[9].pendingInterrupt = 4;
    }
    if (g_GameScoreState.unknown00 < gui->value9E78)
    {
        g_GameScoreState.unknown00 = gui->value9E78;
        g_GuiValue4C94 = g_GuiValue4C90;
        g_GuiGameFlags |= 4;
        if ((g_GuiGameFlags & 4) == 0)
        {
            GuiView *owner = g_GuiView;
            AnmVmIdView *id = &owner->specialVmId9E18;
            g_AnmRenderManagerView->MarkVmForDeletion(*id);
            memset(id, 0, sizeof(*id));
// The target retains the real 0x00448D00 creator call at this site.
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
            id->value = owner->frontAnm->CreateVmVariant0(0x4a, 15).value;
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
        }
    }
    if (gui->frameTimer.current >= 20)
    {
        if (g_GameScoreState.unknown00 >= digitBoundary && gui->value9E74 < digitBoundary)
            gui->displayVms0[9].pendingInterrupt = 4;
        gui->value9E74 = g_GameScoreState.unknown00;
    }

    int highScore = g_GameScoreState.unknown00;
    int score = gui->value9E78;
    for (int i = 1; i < 10; ++i)
    {
        if (highScore != 0)
        {
            gui->frontAnm->SetSprite(&gui->displayVms0[i], highScore % 10 + 8);
            highScore /= 10;
        }
        else
            gui->frontAnm->SetSprite(&gui->displayVms0[i], 8);
        if (score != 0)
        {
            gui->frontAnm->SetSprite(&gui->displayVms1[i], score % 10 + 8);
            score /= 10;
        }
        else
            gui->frontAnm->SetSprite(&gui->displayVms1[i], 8);
        AnmRenderManagerView::ExecuteScript(&gui->displayVms0[i]);
        AnmRenderManagerView::ExecuteScript(&gui->displayVms1[i]);
    }
    gui->frontAnm->SetSprite(&gui->displayVms0[0], g_GuiValue4C94 + 8);
    gui->frontAnm->SetSprite(&gui->displayVms1[0], g_GuiValue4C90 + 8);
    AnmRenderManagerView::ExecuteScript(&gui->displayVms0[0]);
    AnmRenderManagerView::ExecuteScript(&gui->displayVms1[0]);
    if ((gui->guiFlags & 0x20) == 0)
    {
        if (g_GuiDifficulty != 4)
        {
            if (gui->value9E78 < g_GuiNormalExtendScores[g_GuiValue4C9C])
                return;
        }
        else if (gui->value9E78 < g_GuiExtraExtendScores[g_GuiValue4C9C])
            return;
        GameAddLives(1, &g_GameScoreState);
        ++g_GuiValue4C9C;
    }
}
