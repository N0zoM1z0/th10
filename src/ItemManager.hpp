#ifndef TH10_ITEM_MANAGER_HPP
#define TH10_ITEM_MANAGER_HPP

#include "AnmManager.hpp"
#include <stddef.h>

// Existing ItemManager.cpp declarations shared intact for genuine lifecycle callers.
struct ItemVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);

    AnmFloat3View operator*(float scale) const
    {
        return AnmFloat3View(x * scale, y * scale, z * scale);
    }
};

typedef char ItemVectorViewSizeIs0C[
    (sizeof(ItemVectorView) == 0x0c) ? 1 : -1];

struct ItemRecordView
{
    ItemRecordView() {}
    ~ItemRecordView() {}

    AnmVmView vm;
    AnmFloat3View worldPosition;
    ItemVectorView velocity;
    int unknown3C4;
    AnmVmTimerView timer;
    int active;
    int kind;
    int spriteKind;
    float attractionSpeed;
    int delayFrames;
};

struct ItemChainElementView;
struct ItemManagerView
{
    unsigned int flags;
    unsigned char unknown004[4];
    ItemChainElementView *updateCallbackNode;
    ItemChainElementView *drawCallbackNode;
    unsigned char unknown010[4];
    ItemRecordView items[0x896];
    int animatedItemCount;
    int delayedItemCursor;
    int delayedSpawnRequests;

    ItemManagerView();
    ~ItemManagerView();
    int Draw();
    int ConvertPowerItems();
    int Spawn(const AnmFloat3View *position, int kind, unsigned int color,
        float angle, float magnitude);
    static int __fastcall OnUpdate(ItemManagerView *manager);
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
typedef char ItemManagerCallbacksAt08[
    (offsetof(ItemManagerView, updateCallbackNode) == 0x08 &&
     offsetof(ItemManagerView, drawCallbackNode) == 0x0c) ? 1 : -1];
typedef char ItemRecordVelocityAt3B8[
    (offsetof(ItemRecordView, velocity) == 0x3b8) ? 1 : -1];
typedef char ItemRecordTimerAt3C8[
    (offsetof(ItemRecordView, timer) == 0x3c8) ? 1 : -1];
typedef char ItemRecordKindAt3E0[
    (offsetof(ItemRecordView, kind) == 0x3e0) ? 1 : -1];
typedef char ItemRecordAttractionSpeedAt3E8[
    (offsetof(ItemRecordView, attractionSpeed) == 0x3e8) ? 1 : -1];
typedef char ItemRecordDelayAt3EC[
    (offsetof(ItemRecordView, delayFrames) == 0x3ec) ? 1 : -1];
typedef char ItemManagerAnimatedCountAt21CEB4[
    (offsetof(ItemManagerView, animatedItemCount) == 0x21ceb4) ? 1 : -1];
typedef char ItemManagerDelayedCursorAt21CEB8[
    (offsetof(ItemManagerView, delayedItemCursor) == 0x21ceb8 &&
     offsetof(ItemManagerView, delayedSpawnRequests) == 0x21cebc) ? 1 : -1];
typedef char ItemManagerViewSizeIs21CEC0[
    (sizeof(ItemManagerView) == 0x21cec0) ? 1 : -1];

extern ItemManagerView *g_ItemManager;

#endif
