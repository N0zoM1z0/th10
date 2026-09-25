#pragma once

#include "AnmManager.hpp"
#include "Player.hpp"

#include <stddef.h>

struct BulletTransformPayloadView
{
    float float0;
    float float1;
    int int0;
    int int1;
};

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

struct BulletExStateView
{
    AnmVmTimerView timer;
    float value0;
    float value1;
    AnmFloat3View vector;
    int int0;
    int int1;
    int int2;
};

typedef char BulletExStateViewSizeIs34[
    (sizeof(BulletExStateView) == 0x34) ? 1 : -1];
typedef char BulletExStateValue0At14[
    (offsetof(BulletExStateView, value0) == 0x14) ? 1 : -1];
typedef char BulletExStateVectorAt1C[
    (offsetof(BulletExStateView, vector) == 0x1c) ? 1 : -1];
typedef char BulletExStateInt0At28[
    (offsetof(BulletExStateView, int0) == 0x28) ? 1 : -1];

struct BulletSpawnDescriptorView
{
    short bulletType;
    short color;
    PlayerFloat3 position;
    float angle;
    float angleStep;
    float speed1;
    float speed2;
    BulletTransformRecordView transforms[18];
    float laserStartOffset;
    float laserEndOffset;
    float laserStartLength;
    float laserWidth;
    int laserStartTime;
    int laserDuration;
    int laserDespawnDuration;
    int laserHitboxStartTime;
    int laserHitboxEndDelay;
    short count1;
    short count2;
    unsigned short aimMode;
    unsigned short unknown1FA;
    unsigned int transformFlags;
    int spawnSound;
    int transformSound;
    int transformStartIndex;
    void *templateSprites;
};

typedef char BulletSpawnDescriptorViewSizeIs210[
    (sizeof(BulletSpawnDescriptorView) == 0x210) ? 1 : -1];
typedef char BulletSpawnDescriptorTransformsAt20[
    (offsetof(BulletSpawnDescriptorView, transforms) == 0x20) ? 1 : -1];
typedef char BulletSpawnDescriptorCount1At1F4[
    (offsetof(BulletSpawnDescriptorView, count1) == 0x1f4) ? 1 : -1];
typedef char BulletSpawnDescriptorTransformFlagsAt1FC[
    (offsetof(BulletSpawnDescriptorView, transformFlags) == 0x1fc) ? 1 : -1];
typedef char BulletSpawnDescriptorTransformIndexAt208[
    (offsetof(BulletSpawnDescriptorView, transformStartIndex) == 0x208) ? 1 : -1];

struct BulletRuntimeView
{
    unsigned int flags;                         // +0x000
    int cancelBehavior;                         // +0x004
    AnmVmView vm;                               // +0x008
    PlayerFloat3 position;                      // +0x3B4
    PlayerFloat3 velocity;                      // +0x3C0
    unsigned char unknown3CC[0x0c];             // +0x3CC
    float speed;                                // +0x3D8
    unsigned char unknown3DC[0x08];             // +0x3DC
    float angle;                                // +0x3E4
    unsigned char unknown3E8[0x08];             // +0x3E8
    float collisionWidth;                       // +0x3F0
    float collisionHeight;                      // +0x3F4
    AnmVmTimerView stateTimer;                   // +0x3F8
    AnmVmTimerView activeTimer;                  // +0x40C
    int ownerTag;                               // +0x420
    unsigned char unknown424[0x10];             // +0x424
    int offscreenCullDelayFrames;               // +0x434
    int effectScript;                           // +0x438
    unsigned int activeTransformFlags;          // +0x43C
    unsigned int transformFlags;                // +0x440
    unsigned short unknown444;                  // +0x444
    unsigned short state;                       // +0x446
    unsigned char unknown448[0x0c];             // +0x448
    int value454;                               // +0x454
    int transformSound;                         // +0x458
    int transformIndex;                         // +0x45C
    int drawBucketIndex;                        // +0x460
    BulletTransformRecordView transforms[18];   // +0x464
    BulletExStateView exStates[9];               // +0x614
    unsigned char unknown7E8[0x02];              // +0x7E8
    short bulletType;                           // +0x7EA
    short color;                                // +0x7EC
    unsigned char unknown7EE[0x02];              // +0x7EE

    void AdvanceTransformProgram();
};

typedef char BulletRuntimeViewSizeIs7F0[
    (sizeof(BulletRuntimeView) == 0x7f0) ? 1 : -1];
typedef char BulletRuntimePositionAt3B4[
    (offsetof(BulletRuntimeView, position) == 0x3b4) ? 1 : -1];
typedef char BulletRuntimeSpeedAt3D8[
    (offsetof(BulletRuntimeView, speed) == 0x3d8) ? 1 : -1];
typedef char BulletRuntimeAngleAt3E4[
    (offsetof(BulletRuntimeView, angle) == 0x3e4) ? 1 : -1];
typedef char BulletRuntimeCollisionAt3F0[
    (offsetof(BulletRuntimeView, collisionWidth) == 0x3f0 &&
     offsetof(BulletRuntimeView, collisionHeight) == 0x3f4) ? 1 : -1];
typedef char BulletRuntimeTimersAt3F8[
    (offsetof(BulletRuntimeView, stateTimer) == 0x3f8 &&
     offsetof(BulletRuntimeView, activeTimer) == 0x40c) ? 1 : -1];
typedef char BulletRuntimeOwnerTagAt420[
    (offsetof(BulletRuntimeView, ownerTag) == 0x420) ? 1 : -1];
typedef char BulletRuntimeCullDelayAt434[
    (offsetof(BulletRuntimeView, offscreenCullDelayFrames) == 0x434) ? 1 : -1];
typedef char BulletRuntimeActiveTransformsAt43C[
    (offsetof(BulletRuntimeView, activeTransformFlags) == 0x43c) ? 1 : -1];
typedef char BulletRuntimeTransformFlagsAt440[
    (offsetof(BulletRuntimeView, transformFlags) == 0x440) ? 1 : -1];
typedef char BulletRuntimeEffectScriptAt438[
    (offsetof(BulletRuntimeView, effectScript) == 0x438) ? 1 : -1];
typedef char BulletRuntimeStateAt446[
    (offsetof(BulletRuntimeView, state) == 0x446) ? 1 : -1];
typedef char BulletRuntimeTransformSoundAt458[
    (offsetof(BulletRuntimeView, transformSound) == 0x458) ? 1 : -1];
typedef char BulletRuntimeTransformIndexAt45C[
    (offsetof(BulletRuntimeView, transformIndex) == 0x45c) ? 1 : -1];
typedef char BulletRuntimeTransformsAt464[
    (offsetof(BulletRuntimeView, transforms) == 0x464) ? 1 : -1];
typedef char BulletRuntimeExStatesAt614[
    (offsetof(BulletRuntimeView, exStates) == 0x614) ? 1 : -1];
typedef char BulletRuntimeTypeColorAt7EA[
    (offsetof(BulletRuntimeView, bulletType) == 0x7ea &&
     offsetof(BulletRuntimeView, color) == 0x7ec) ? 1 : -1];


typedef char BulletRuntimeVmAt008[
    (offsetof(BulletRuntimeView, vm) == 0x008) ? 1 : -1];

struct BulletManagerView
{
    unsigned char unknown000[0x10];
    BulletRuntimeView *bulletCursor;             // +0x000010
    unsigned char unknown014[0x4c];
    BulletRuntimeView bullets[2001];             // +0x000060
    AnmLoadedView *bulletAnm;                    // +0x3E0B50

    int SpawnSingleBullet(
        BulletSpawnDescriptorView *descriptor,
        int index1, int index2, float angleToPlayer);
    int SpawnBulletPattern(BulletSpawnDescriptorView *descriptor);
};

typedef char BulletManagerCursorAt10[
    (offsetof(BulletManagerView, bulletCursor) == 0x10) ? 1 : -1];
typedef char BulletManagerBulletsAt60[
    (offsetof(BulletManagerView, bullets) == 0x60) ? 1 : -1];
typedef char BulletManagerAnmAt3E0B50[
    (offsetof(BulletManagerView, bulletAnm) == 0x3e0b50) ? 1 : -1];
