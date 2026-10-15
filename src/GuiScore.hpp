#ifndef TH10_RECONSTRUCTION_GUI_SCORE_HPP
#define TH10_RECONSTRUCTION_GUI_SCORE_HPP
#include <stddef.h>

// Shared view of target score owner 0x00474C40; storage remains external.
struct GuiScoreView
{
    int unknown00;
    int score;

    void Add(int amount)
    {
        score += amount / 10;
        if (score >= 1000000000)
            score = 999999999;
    }
};

typedef char GuiScoreSizeIs08[(sizeof(GuiScoreView) == 8) ? 1 : -1];
typedef char GuiScoreValueAt04[(offsetof(GuiScoreView, score) == 4) ? 1 : -1];
extern GuiScoreView g_GuiScore;
#endif
