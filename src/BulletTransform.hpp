#ifndef TH10_BULLET_TRANSFORM_HPP
#define TH10_BULLET_TRANSFORM_HPP

#include <stddef.h>

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
    unsigned int kind;
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
