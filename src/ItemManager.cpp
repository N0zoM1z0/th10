#include <stddef.h>

struct ItemVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
};

typedef char ItemVectorViewSizeIs0C[
    (sizeof(ItemVectorView) == 0x0c) ? 1 : -1];

void ItemVectorView::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp dword ptr [eax]
        fmul magnitude
        fstp dword ptr [eax + 4]
    }
#else
    (void)angle;
    (void)magnitude;
    x = 0.0f;
    y = 0.0f;
#endif
}

struct GameScoreStateView
{
    int unknown00;
    int score;
    int highScore;
    int faith;

    void AddFaith(int amount);
};

typedef char GameScoreStateFaithAt0C[
    (offsetof(GameScoreStateView, faith) == 0x0c) ? 1 : -1];

void GameScoreStateView::AddFaith(int amount)
{
    faith += amount / 10;
    if (faith > 99999)
        faith = 99999;
}
