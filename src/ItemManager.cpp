#include <stddef.h>

#include "AnmManager.hpp"

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
    unsigned char unknown10[0x48];
    int rank;

    void AddFaith(int amount);
    void DecayFaith(int amount);
    void AddRank(int amount);
};

typedef char GameScoreStateFaithAt0C[
    (offsetof(GameScoreStateView, faith) == 0x0c) ? 1 : -1];
typedef char GameScoreStateRankAt58[
    (offsetof(GameScoreStateView, rank) == 0x58) ? 1 : -1];

void GameScoreStateView::AddFaith(int amount)
{
    faith += amount / 10;
    if (faith > 99999)
        faith = 99999;
}

void GameScoreStateView::DecayFaith(int amount)
{
    faith -= amount / 10;
    if (faith < 5000)
        faith = 5000;
}

void GameScoreStateView::AddRank(int amount)
{
    rank += amount;
    if (rank > 0x400)
        rank = 0x400;
    else if (rank < -0x400)
        rank = -0x400;
}

// TH10 0x0041B8E0-0x0041B9F8 draws the fixed 0x896-row item pool.  The
// record layout below comes from that body and the neighboring update/spawn
// owners: each 0x3F0-byte row starts with a complete AnmVmView, then keeps
// its world-space position at +0x3AC, active state at +0x3DC and the
// sprite-selection value at +0x3E4.
struct ItemPrimaryResourceOwnerView
{
    unsigned char unknown000000[0x3e0b50];
    AnmLoadedView *primaryEnemyResource;
};

struct FpsSampleGateView
{
    unsigned char unknown000[0x58];
    unsigned int flags58;
};

struct ItemRecordView
{
    AnmVmView vm;
    AnmFloat3View worldPosition;
    unsigned char unknown3B8[0x24];
    int active;
    unsigned char unknown3E0[4];
    int spriteKind;
    unsigned char unknown3E8[8];
};

struct ItemManagerView
{
    unsigned char unknown000[0x14];
    ItemRecordView items[0x896];

    int Draw();
    static int __fastcall OnDraw(ItemManagerView *manager);
};

typedef char ItemRecordViewSizeIs3F0[
    (sizeof(ItemRecordView) == 0x3f0) ? 1 : -1];
typedef char ItemRecordWorldPositionAt3AC[
    (offsetof(ItemRecordView, worldPosition) == 0x3ac) ? 1 : -1];
typedef char ItemRecordActiveAt3DC[
    (offsetof(ItemRecordView, active) == 0x3dc) ? 1 : -1];
typedef char ItemRecordSpriteKindAt3E4[
    (offsetof(ItemRecordView, spriteKind) == 0x3e4) ? 1 : -1];
typedef char ItemManagerItemsAt14[
    (offsetof(ItemManagerView, items) == 0x14) ? 1 : -1];

extern ItemPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;
extern FpsSampleGateView *g_FpsSampleGateView;
extern float g_ItemScreenOffsetX;
extern float g_ItemScreenOffsetY;
extern float g_ItemTopThreshold;
extern float g_ItemAlphaRange;
extern float g_ItemAlphaScale;
extern float g_ItemAlphaFull;

int ItemManagerView::Draw()
{
    ItemRecordView *item = &items[0];
    for (int remaining = 0x896; remaining != 0; --remaining, ++item)
    {
        if (item->active == 0)
            continue;

        item->vm.position.x = item->worldPosition.x + g_ItemScreenOffsetX;
        item->vm.position.y = item->worldPosition.y + g_ItemScreenOffsetY;
        item->vm.position.z = item->worldPosition.z;

        if (item->vm.position.y < g_ItemTopThreshold)
        {
            const float y = item->vm.position.y;
            item->vm.position.y = 24.0f;
            const float delta = y - g_ItemTopThreshold;
            if (delta >= g_ItemAlphaRange)
            {
                item->vm.primaryColor.alpha = 0xff;
            }
            else
            {
                item->vm.primaryColor.alpha = static_cast<unsigned char>(
                    static_cast<int>(
                        delta * g_ItemAlphaScale * g_ItemAlphaFull));
            }

            if (item->vm.activeSpriteIndex != item->spriteKind + 0x161)
            {
                g_EnemyPrimaryResourceOwner->primaryEnemyResource->SetSprite(
                    &item->vm, item->spriteKind + 0x160);
            }
        }
        else if (item->vm.activeSpriteIndex != item->spriteKind + 0x158)
        {
            g_EnemyPrimaryResourceOwner->primaryEnemyResource->SetSprite(
                &item->vm, item->spriteKind + 0x157);
            item->vm.primaryColor.alpha = 0xff;
        }

        g_AnmRenderManagerView->Draw(&item->vm);
    }
    return 1;
}

// The registered draw adapter at 0x0041BA30 has the normal callback receiver
// in ECX.  With Draw present in the same LTCG graph VC7.1 naturally lowers
// the final call to the target's private EAX receiver and tail jump.
int __fastcall ItemManagerView::OnDraw(ItemManagerView *manager)
{
    FpsSampleGateView *gate = g_FpsSampleGateView;
    if (gate != NULL && (gate->flags58 & 4) != 0)
        return 1;
    return manager->Draw();
}
