#ifndef TH10_GAME_SCORE_STATE_HPP
#define TH10_GAME_SCORE_STATE_HPP

#include <stddef.h>

struct GameScoreTimerView
{
    int previous;
    int current;
    float subframe;
    float *scale;
    unsigned int flags;
};

typedef char GameScoreTimerViewSizeIs14[
    (sizeof(GameScoreTimerView) == 0x14) ? 1 : -1];

struct GameScoreStateView
{
    int unknown00;
    int score;
    int highScore;
    int faith;
    int unknown10;
    GameScoreTimerView timer;
    unsigned char unknown28[0x30];
    int rank;

    void AddFaith(int amount);
    void DecayFaith(int amount);
    void SetFaith(int amount);
    void SetTimerCurrent(int value);
    void AddRank(int amount);
};

typedef char GameScoreStateFaithAt0C[
    (offsetof(GameScoreStateView, faith) == 0x0c) ? 1 : -1];
typedef char GameScoreStateTimerAt14[
    (offsetof(GameScoreStateView, timer) == 0x14) ? 1 : -1];
typedef char GameScoreStateRankAt58[
    (offsetof(GameScoreStateView, rank) == 0x58) ? 1 : -1];
typedef char GameScoreStateSizeIs5C[
    (sizeof(GameScoreStateView) == 0x5c) ? 1 : -1];

extern GameScoreStateView g_GameScoreState;
extern float g_PlayerTimerScale;

#endif
