#ifndef TH10_BULLET_TRANSFORM_HPP
#define TH10_BULLET_TRANSFORM_HPP

#include <stddef.h>

enum BulletTransformKindView
{
    BULLET_TRANSFORM_NONE = 0x00000000,
    BULLET_TRANSFORM_DECELERATE = 0x00000001,
    BULLET_TRANSFORM_ACCELERATE_VECTOR = 0x00000010,
    BULLET_TRANSFORM_ACCELERATE_POLAR = 0x00000020,
    BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE = 0x00000040,
    BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED = 0x00000080,
    BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE = 0x00000100,
    BULLET_TRANSFORM_BOUNCE_ALL_EDGES = 0x00000400,
    BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM = 0x00000800,
    BULLET_TRANSFORM_SET_CANCEL_BEHAVIOR = 0x00001000,
    BULLET_TRANSFORM_SET_CULL_DELAY = 0x00002000,
    BULLET_TRANSFORM_SET_SPRITE = 0x00004000,
    BULLET_TRANSFORM_WAIT = 0x00008000,
    BULLET_TRANSFORM_DESPAWN = 0x00010000,
    BULLET_TRANSFORM_PLAY_SOUND = 0x00020000,
    BULLET_TRANSFORM_WRAP_X = 0x00100000,
    BULLET_TRANSFORM_WRAP_Y = 0x00200000,
    BULLET_TRANSFORM_SPAWN_CHILD_PATTERN = 0x00400000,
    BULLET_TRANSFORM_SET_OWNER_TAG = 0x01000000,
    BULLET_TRANSFORM_JUMP = 0x02000000,

    // TH10-only transform-state kinds. Their exact gameplay names are not
    // established yet; the state slots and payload fields below are target
    // proven, so keep the numeric names until callers/scripts identify them.
    BULLET_TRANSFORM_STATE_8 = 0x04000000,
    BULLET_TRANSFORM_BOUNCE_VARIANT = 0x08000000,

    // Program control marker: advance to the next record without executing
    // an action.
    BULLET_TRANSFORM_SKIP = 0x80000000
};

typedef char BulletTransformKindViewSizeIs4[
    (sizeof(BulletTransformKindView) == sizeof(unsigned int)) ? 1 : -1];

struct BulletTransformPayloadView
{
    float float0;
    float float1;
    // The second transform record of a child-pattern spawn reuses the low
    // signed halfword of payload int0 for the second bullet count.
    union
    {
        int int0;
        short int0LowSigned16;
    };
    int int1;
};

typedef char BulletTransformPayloadLowHalfAt08[
    (offsetof(BulletTransformPayloadView, int0) == 0x08 &&
     offsetof(BulletTransformPayloadView, int0LowSigned16) == 0x08) ? 1 : -1];

struct BulletTransformRecordView
{
    BulletTransformPayloadView payload;
    BulletTransformKindView kind;
    int allowWhileActive;
};

typedef char BulletTransformPayloadViewSizeIs10[
    (sizeof(BulletTransformPayloadView) == 0x10) ? 1 : -1];
typedef char BulletTransformRecordViewSizeIs18[
    (sizeof(BulletTransformRecordView) == 0x18) ? 1 : -1];
typedef char BulletTransformRecordViewKindAt10[
    (offsetof(BulletTransformRecordView, kind) == 0x10) ? 1 : -1];
typedef char BulletTransformRecordViewAllowAt14[
    (offsetof(BulletTransformRecordView, allowWhileActive) == 0x14) ? 1 : -1];

#endif // TH10_BULLET_TRANSFORM_HPP
