#include "Main.hpp"
#include "VmIdInterrupt.hpp"
#include "EnemySoundQueue.hpp"
#include "ItemManager.hpp"
#include "EnemyGameState.hpp"
#include "GameStatistics.hpp"
#include "EnemyPrimaryResources.hpp"
#include "Enemy.hpp"
#include "EnemyCancel.hpp"
#include "EnemyLaser.hpp"
#include "EclVm.hpp"
#include "Rng.hpp"
#include "EclWorldAnchor.hpp"
#include "Gui.hpp"
#include "BulletManager.hpp"
#include "StageScript.hpp"

#include <math.h>
#include <new>
#include <string.h>


// The target dispatcher at 0x0040E770 owns opcodes 0x100 through 0x1B4.
// This translation retains neutral helper names where the original source name
// is not recoverable yet.  Each call and field access comes from the attested
// TH10 v1.00a body; adjacent ECL maps were used only to group opcode families.
enum EnemyEclOpcode
{
    ENEMY_ECL_CREATE_ENEMY = 0x100,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE = 0x101,
    ENEMY_ECL_SELECT_ANM_RESOURCE = 0x102,
    ENEMY_ECL_SET_ANM_SCRIPT = 0x103,
    ENEMY_ECL_CREATE_ENEMY_MIRRORED = 0x104,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED = 0x105,
    ENEMY_ECL_SET_MAIN_ANM_SCRIPT = 0x106,
    ENEMY_ECL_PLAY_ANM = 0x107,
    ENEMY_ECL_PLAY_ANM_ABSOLUTE = 0x108,
    ENEMY_ECL_CREATE_ENEMY_IF_NO_BOSS = 0x109,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_IF_NO_BOSS = 0x10A,
    ENEMY_ECL_CREATE_ENEMY_MIRRORED_IF_NO_BOSS = 0x10B,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED_IF_NO_BOSS = 0x10C,
    ENEMY_ECL_PLAY_SELECTED_ANM = 0x10D,
    ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET = 0x10E,
    ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET_IF_NO_BOSS = 0x10F,
    ENEMY_ECL_PLAY_ANM_HIGH = 0x110,
    ENEMY_ECL_PLAY_ANM_ROTATED = 0x111,
    ENEMY_ECL_SET_OFFSET_POSITION = 0x118,
    ENEMY_ECL_INTERPOLATE_OFFSET_POSITION = 0x119,
    ENEMY_ECL_SET_BASE_POSITION = 0x11A,
    ENEMY_ECL_INTERPOLATE_BASE_POSITION = 0x11B,
    ENEMY_ECL_SET_OFFSET_POLAR = 0x11C,
    ENEMY_ECL_INTERPOLATE_OFFSET_POLAR = 0x11D,
    ENEMY_ECL_SET_BASE_POLAR = 0x11E,
    ENEMY_ECL_INTERPOLATE_BASE_POLAR = 0x11F,
    ENEMY_ECL_SET_OFFSET_CIRCLE = 0x120,
    ENEMY_ECL_INTERPOLATE_OFFSET_CIRCLE = 0x121,
    ENEMY_ECL_SET_BASE_CIRCLE = 0x122,
    ENEMY_ECL_INTERPOLATE_BASE_CIRCLE = 0x123,
    ENEMY_ECL_MOVE_RANDOM = 0x124,
    ENEMY_ECL_MOVE_RANDOM_BASE = 0x125,
    ENEMY_ECL_COPY_PLAYER_TO_OFFSET = 0x126,
    ENEMY_ECL_COPY_PLAYER_TO_BASE = 0x127,
    ENEMY_ECL_ADD_OFFSET_POSITION = 0x128,
    ENEMY_ECL_ADD_BASE_POSITION = 0x129,
    ENEMY_ECL_ADD_OFFSET_VELOCITY = 0x12A,
    ENEMY_ECL_ADD_BASE_VELOCITY = 0x12B,
    ENEMY_ECL_SET_DAMAGE_HITBOX = 0x140,
    ENEMY_ECL_SET_PLAYER_HITBOX = 0x141,
    ENEMY_ECL_SET_FLAGS = 0x142,
    ENEMY_ECL_CLEAR_FLAGS = 0x143,
    ENEMY_ECL_SET_MOVEMENT_BOUNDS = 0x144,
    ENEMY_ECL_CLEAR_MOVEMENT_BOUNDS = 0x145,
    ENEMY_ECL_CLEAR_ITEM_DROPS = 0x146,
    ENEMY_ECL_ADD_ITEM_DROP = 0x147,
    ENEMY_ECL_SET_DROP_AREA = 0x148,
    ENEMY_ECL_DROP_ITEMS = 0x149,
    ENEMY_ECL_SET_MAIN_DROP = 0x14A,
    ENEMY_ECL_SET_LIFE = 0x14B,
    ENEMY_ECL_SET_BOSS_SLOT = 0x14C,
    ENEMY_ECL_RESET_UPDATE_TIMER = 0x14D,
    ENEMY_ECL_SET_INTERRUPT = 0x14E,
    ENEMY_ECL_SET_INVULNERABILITY = 0x14F,
    ENEMY_ECL_PLAY_SOUND = 0x150,
    ENEMY_ECL_SET_SCREEN_SHAKE = 0x151,
    ENEMY_ECL_READ_DIALOG = 0x152,
    ENEMY_ECL_WAIT_DIALOG = 0x153,
    ENEMY_ECL_WAIT_BOSS_CLEAR = 0x154,
    ENEMY_ECL_SET_TIMEOUT = 0x155,
    ENEMY_ECL_START_SPELL = 0x156,
    ENEMY_ECL_END_SPELL = 0x157,
    ENEMY_ECL_SET_CHAPTER = 0x158,
    ENEMY_ECL_KILL_ALL_ENEMIES = 0x159,
    ENEMY_ECL_SET_BULLET_PROTECTION_RANGE = 0x15A,
    ENEMY_ECL_SET_LIFE_MARKER = 0x15B,
    ENEMY_ECL_START_SPELL_2 = 0x15C,
    ENEMY_ECL_SELECT_FLOAT_BY_RANK_3 = 0x15D,
    ENEMY_ECL_SELECT_FLOAT_BY_RANK_5 = 0x15E,
    ENEMY_ECL_INTERPOLATE_FLOAT_BY_RANK = 0x15F,
    ENEMY_ECL_SELECT_INT_BY_RANK_3 = 0x160,
    ENEMY_ECL_SELECT_INT_BY_RANK_5 = 0x161,
    ENEMY_ECL_INTERPOLATE_INT_BY_RANK = 0x162,
    ENEMY_ECL_SELECT_INT_BY_DIFFICULTY = 0x163,
    ENEMY_ECL_SELECT_FLOAT_BY_DIFFICULTY = 0x164,
    ENEMY_ECL_START_SPELL_DIFFICULTY = 0x165,
    ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1 = 0x166,
    ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2 = 0x167,
    ENEMY_ECL_SET_SPELL_TIMEOUT = 0x168,
    ENEMY_ECL_SET_PLAYER_COLLISION_TIMER = 0x169,
    ENEMY_ECL_CANCEL_LASERS = 0x16A,
    ENEMY_ECL_ENABLE_BOMB_SHIELD = 0x16B,
    ENEMY_ECL_SET_GAME_SPEED_FLAG = 0x16C,
    ENEMY_ECL_WAIT_FOR_EFFECT = 0x16D,
    ENEMY_ECL_SET_ANM_MODE = 0x16E,
    ENEMY_ECL_SET_TIMER_SCALE = 0x16F,
    ENEMY_ECL_SUBTRACT_DIFFICULTY_VALUE = 0x170,
    ENEMY_ECL_INITIALIZE_BULLET_PATTERN = 0x190,
    ENEMY_ECL_FIRE_BULLET_PATTERN = 0x191,
    ENEMY_ECL_SET_BULLET_SPRITES = 0x192,
    ENEMY_ECL_SET_BULLET_OFFSET = 0x193,
    ENEMY_ECL_SET_BULLET_ANGLE = 0x194,
    ENEMY_ECL_SET_BULLET_SPEED = 0x195,
    ENEMY_ECL_SET_BULLET_COUNT = 0x196,
    ENEMY_ECL_SET_BULLET_AIM_MODE = 0x197,
    ENEMY_ECL_SET_BULLET_SOUND = 0x198,
    ENEMY_ECL_SET_BULLET_EXTRA = 0x199,
    ENEMY_ECL_CANCEL_ALL_BULLETS = 0x19A,
    ENEMY_ECL_COPY_BULLET_PATTERN = 0x19B,
    ENEMY_ECL_FIRE_LASER_A = 0x19C,
    ENEMY_ECL_FIRE_STRAIGHT_LASER = 0x19D,
    ENEMY_ECL_SET_LASER_OFFSET = 0x19E,
    ENEMY_ECL_SET_LASER_ANGLE = 0x19F,
    ENEMY_ECL_SET_LASER_SPEED = 0x1A0,
    ENEMY_ECL_SET_LASER_COUNT = 0x1A1,
    ENEMY_ECL_SET_LASER_AIM = 0x1A2,
    ENEMY_ECL_SET_LASER_SOUND = 0x1A3,
    ENEMY_ECL_CANCEL_BULLET_PATTERN = 0x1A4,
    ENEMY_ECL_CLEAR_BULLET_PATTERN = 0x1A5,
    ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_3 = 0x1A6,
    ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_5 = 0x1A7,
    ENEMY_ECL_INTERPOLATE_BULLET_SPEED_BY_RANK = 0x1A8,
    ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_3 = 0x1A9,
    ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_5 = 0x1AA,
    ENEMY_ECL_INTERPOLATE_BULLET_COUNT_BY_RANK = 0x1AB,
    ENEMY_ECL_FIRE_LASER = 0x1AC,
    ENEMY_ECL_FIRE_STRAIGHT_LASER_2 = 0x1AD,
    ENEMY_ECL_AIM_BULLET_AT_PLAYER = 0x1AE,
    ENEMY_ECL_FIRE_LASER_A_WITH_PATTERN = 0x1AF,
    ENEMY_ECL_FIRE_STRAIGHT_LASER_WITH_PATTERN = 0x1B0,
    ENEMY_ECL_FIRE_LASER_WITH_PATTERN = 0x1B1,
    ENEMY_ECL_FIRE_STRAIGHT_LASER_2_WITH_PATTERN = 0x1B2,
    ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY = 0x1B3,
    ENEMY_ECL_SET_BULLET_COUNT_BY_DIFFICULTY = 0x1B4,
};


// ENEMY_ECL_SET_BULLET_EXTRA indexes six consecutive 32-bit fields using
// a 0x18-byte stride. Their semantic names are intentionally provisional.
struct EnemyBulletExtraView
{
    float floatA;
    float floatB;
    int value08;
    int value0C;
    int value10;
    int value14;
};
typedef char EnemyBulletExtraSizeIs18[
    (sizeof(EnemyBulletExtraView) == 0x18) ? 1 : -1];
typedef char EnemyBulletExtraFieldsNativeOffsets[
    (offsetof(EnemyBulletExtraView, floatA) == 0x00 &&
     offsetof(EnemyBulletExtraView, floatB) == 0x04 &&
     offsetof(EnemyBulletExtraView, value08) == 0x08 &&
     offsetof(EnemyBulletExtraView, value0C) == 0x0c &&
     offsetof(EnemyBulletExtraView, value10) == 0x10 &&
     offsetof(EnemyBulletExtraView, value14) == 0x14) ? 1 : -1];
// ECL opcode 0x199 addresses these 0x18-byte slots from the first
// pattern's +0x20. There are 0x16 slots per 0x210-byte pattern stride.
// This is an *overlapping* view of pattern storage, not a second array:
// entries 20/21 alias the normal pattern tail and the next pattern's head.
typedef char EnemyBulletExtraPatternOverlayLayout[
    (offsetof(EnemyRuntimeView, bulletPatterns) +
         offsetof(EnemyBulletPatternFieldsView, unknown020) == 0x2e4 &&
     sizeof(EnemyBulletPatternView) == 0x16 * sizeof(EnemyBulletExtraView))
        ? 1 : -1];

// Multiple ECL opcode families use a signed, byte-scaled pattern cursor.
// Keep that native integer cursor lifetime at each expression site and bind
// its final field access to the independently checked 0x210-byte record.
// Its address addition is an i386 dword ADD, so use unsigned 32-bit arithmetic
// rather than invoking C++ signed overflow for out-of-range ECL indices.
static __forceinline EnemyBulletPatternView *EnemyPatternAtRuntimeByte(int byteAddress)
{
    return reinterpret_cast<EnemyBulletPatternView *>(
        static_cast<unsigned int>(byteAddress) +
        static_cast<unsigned int>(offsetof(EnemyRuntimeView, bulletPatterns)));
}

// This complete 0x210-byte Enemy pattern has the *same wire fields* as the
// bullet manager's spawn descriptor at this call boundary. The native
// renderer consumes it directly; no intermediate descriptor is allocated.
typedef char EnemyPatternBulletSpawnDescriptorOverlap[
    (sizeof(EnemyBulletPatternView) == sizeof(BulletSpawnDescriptorView) &&
     offsetof(EnemyBulletPatternFieldsView, spriteId) ==
         offsetof(BulletSpawnDescriptorView, bulletType) &&
     offsetof(EnemyBulletPatternFieldsView, spawnPositionX) ==
         offsetof(BulletSpawnDescriptorView, position) &&
     offsetof(EnemyBulletPatternFieldsView, countA) ==
         offsetof(BulletSpawnDescriptorView, count1) &&
     offsetof(EnemyBulletPatternFieldsView, flags1FC) ==
         offsetof(BulletSpawnDescriptorView, transformFlags)) ? 1 : -1];

// Native ECL index arithmetic uses a flat 32-bit base plus a typed stride.
// The script operands are not clamped by the original dispatcher.
template <class T> static __forceinline T *EnemyNativeWordIndex(T *first, int index)
{
    return reinterpret_cast<T *>(
        reinterpret_cast<unsigned int>(first) +
        static_cast<unsigned int>(index) * static_cast<unsigned int>(sizeof(T)));
}

// Native 0x210-byte pattern index/address word for unchecked ECL operands.
static __forceinline EnemyBulletPatternView *EnemyPatternAtRuntimeIndex(
    int runtimeAddress, int patternIndex)
{
    return EnemyPatternAtRuntimeByte(static_cast<int>(
        static_cast<unsigned int>(patternIndex) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress)));
}

// A script index selects a 12-byte XYZ offset in the same runtime record.
// Original i386 IMUL/LEA form low 32-bit words, without an index clamp.
static __forceinline PlayerFloat3 *EnemyPatternOffsetAtRuntimeIndex(
    int runtimeAddress, int index)
{
    return reinterpret_cast<PlayerFloat3 *>(
        static_cast<unsigned int>(runtimeAddress) +
        static_cast<unsigned int>(offsetof(EnemyRuntimeView, bulletPatternOffsets)) +
        static_cast<unsigned int>(index) *
        static_cast<unsigned int>(sizeof(PlayerFloat3)));
}

// Scalar values after a counted inline spawn name are a variable-length
// four-byte stream. Unchecked ECL offsets use native low 32-bit addresses.
static __forceinline const EclVmScalar *EnemySpawnScalarWordAt(
    const EnemyEclInstructionView *instruction, unsigned int index)
{
    return reinterpret_cast<const EclVmScalar *>(
        reinterpret_cast<unsigned int>(instruction) +
        static_cast<unsigned int>(offsetof(EnemyEclInstructionView, scalarOperands)) +
        index * static_cast<unsigned int>(sizeof(EclVmScalar)));
}

// The five direct opcode stores use one checked overlapping-record address.
// Keep each access expression local to its original argument read. Hoisting
// this pointer across those reads changes the full dispatcher's LTCG liveness.
static __forceinline EnemyBulletExtraView *EnemyExtraRecordAt(int baseAddress)
{
    return reinterpret_cast<EnemyBulletExtraView *>(
        static_cast<unsigned int>(baseAddress) +
        static_cast<unsigned int>(
            offsetof(EnemyRuntimeView, bulletPatterns) +
            offsetof(EnemyBulletPatternFieldsView, unknown020)));
}


struct EnemyManagedVmView;
extern int g_EnemyDifficulty;
extern EnemyManagerView *g_EnemyManager;
extern EnemyGameStateView *g_EnemyGameState;
extern void *g_EnemyBulletManager;
extern PlayerFloat3 g_EnemyGlobalPositionOffset;
extern PlayerFloat3 g_EnemyInterpolationOrigin;
extern EnemyFloat2 g_EnemyInterpolationBasis;
extern unsigned char g_EnemyViewMatrix;
extern unsigned char g_EnemyProjectionMatrix;
extern unsigned char g_EnemyViewport;
extern unsigned char g_EnemyWorldMatrix;

extern PlayerFloat3 *__stdcall D3DXVec3Project(
    PlayerFloat3 *output, const PlayerFloat3 *input, const void *viewport,
    const void *projection, const void *view, const void *world);
extern "C" AnmFloat3View *__stdcall D3DXVec3Normalize(
    AnmFloat3View *output, const AnmFloat3View *input);
extern "C" AnmMatrixView *__stdcall D3DXMatrixLookAtLH(
    AnmMatrixView *output, const AnmFloat3View *eye,
    const AnmFloat3View *at, const AnmFloat3View *up);
extern "C" AnmMatrixView *__stdcall D3DXMatrixPerspectiveFovLH(
    AnmMatrixView *output, float fov, float aspect,
    float nearPlane, float farPlane);

__declspec(noinline) void EnemyConfigureViewportOwner(
    AnmViewportOwnerView *viewport)
{
    if (g_AnmRenderManagerView != 0) {
        g_AnmRenderManagerView->FlushVertexBuffer();
    }

    // The target's viewport prefix aliases the independently checked
    // 0x118-byte StageCameraStateView in the real stage/camera owner.
    StageCameraStateView *camera =
        reinterpret_cast<StageCameraStateView *>(viewport);
    AnmFloat3View at;
    AnmFloat3View eye;
    at.x = camera->direction.x + camera->position.x;
    at.y = camera->direction.y + camera->position.y;
    at.z = camera->direction.z + camera->position.z;
    eye.x = camera->position.x + camera->rockingOffset.x;
    eye.y = camera->position.y + camera->rockingOffset.y;
    eye.z = camera->position.z + camera->rockingOffset.z;

    D3DXMatrixLookAtLH(
        &viewport->viewMatrix, &eye, &at, &camera->up);

    unsigned int width = viewport->viewport.width;
    unsigned int height = viewport->viewport.height;
    D3DXMatrixPerspectiveFovLH(
        &viewport->projectionMatrix, camera->value048,
        static_cast<float>(width) / static_cast<float>(height),
        30.0f, 1800.0f);

    g_MainSupervisorView.d3dDevice->vtable->SetTransform(
        g_MainSupervisorView.d3dDevice, 2, &viewport->viewMatrix);
    g_MainSupervisorView.d3dDevice->vtable->SetTransform(
        g_MainSupervisorView.d3dDevice, 3, &viewport->projectionMatrix);

    const AnmFloat3View *up = &camera->up;
    AnmFloat3View cross;
    cross.x = up->z * camera->direction.y - up->y * camera->direction.z;
    cross.y = up->x * camera->direction.z - up->z * camera->direction.x;
    cross.z = up->y * camera->direction.x - up->x * camera->direction.y;
    viewport->cameraRight = cross;
    D3DXVec3Normalize(&viewport->cameraRight, &viewport->cameraRight);

    if (g_AnmRenderManagerView != 0) {
        g_AnmRenderManagerView->screenShakeX = camera->value0E8;
        g_AnmRenderManagerView->screenShakeY = camera->value0EC;
    }
}

__declspec(noinline) void EnemyPrepareProjection(
    MainSupervisorView *supervisor, int viewportIndex)
{
    MainCameraRecordView *record =
        reinterpret_cast<MainCameraRecordView *>(supervisor->unknown154) +
        viewportIndex;
    AnmViewportOwnerView *viewport = &record->prefix;
    supervisor->activeViewport = viewport;
    EnemyConfigureViewportOwner(viewport);
    supervisor->d3dDevice->vtable->SetViewport(
        supervisor->d3dDevice, &supervisor->activeViewport->viewport);
    supervisor->viewportConfigured = viewportIndex;
}
extern float g_AnmGameSpeed;

static __declspec(noinline) void EnemyInitializePositionInterpolation(
    EnemyPositionInterpolationView *interpolation)
{
    EnemyInterpolationTimerView &timer = interpolation->timer;
    unsigned int flags = timer.flags;
    if ((flags & 1) == 0) {
        flags |= 1;
        timer.current = 0;
        timer.previous = -999999;
        timer.subframe = 0.0f;
        timer.scale = &g_AnmGameSpeed;
        timer.flags = flags;
    }
    timer.current = 0;
    timer.subframe = 0.0f;
    timer.previous = -1;
}
extern void EnemySpawnCancelEffect(
    float *position, int kind, int color, float angle, float scale);
extern int *EnemyCreateCancelVm(void *resource);
extern int EnemyResolveCancelVm(int id);
extern float g_EnemyCancelHalfScale;
extern float g_EnemyPlayfieldMinX;
extern float g_EnemyPlayfieldMaxX;
extern float g_EnemyPlayfieldMinY;
extern float g_EnemyPlayfieldMaxY;
extern float g_EnemyCancelHighThreshold;
extern float g_EnemyCancelUpperThreshold;
extern unsigned int g_EnemyCancelColorHigh[];
extern unsigned int g_EnemyCancelColorMid[];
extern unsigned int g_EnemyCancelColorLow[];

// The bullet stores its sprite in the embedded ANM VM at full record +0x39C.
// The sprite's width at +0x34 selects the cancel color palette, and the
// newly created effect VM stores its primary color at +0x2FC.
typedef char EnemyCancelSpriteWidthAndColorLayout[
    (offsetof(BulletRuntimeView, vm) +
     offsetof(AnmVmView, loadedSprite) == 0x39c &&
     offsetof(AnmSpriteView, width) == 0x34 &&
     offsetof(AnmVmView, primaryColor) == 0x2fc) ? 1 : -1];

__declspec(noinline) unsigned int EnemyCancelManagerView::CancelAllBullets(int mode)
{
    AnmFloat3View halfSize = cancelSize * g_EnemyCancelHalfScale;
    AnmFloat3View minimum = cancelCenter - halfSize;
    AnmFloat3View maximum = cancelCenter + halfSize;
    unsigned char *bullet = bulletStorage;
    for (int remaining = 2000; remaining != 0;
         --remaining, bullet += 0x7f0) {
        BulletRuntimeView *record = reinterpret_cast<BulletRuntimeView *>(bullet);
        short state = record->state;
        if (state == 0 || state == 3) {
            continue;
        }

        float *position = &record->position.x;
        float radiusX =
            record->collisionWidth *
            g_EnemyCancelHalfScale;
        float radiusY =
            record->collisionHeight *
            g_EnemyCancelHalfScale;
        if (minimum.x <= position[0] + radiusX &&
            position[0] - radiusX <= maximum.x &&
            minimum.y <= position[1] + radiusY &&
            position[1] - radiusY <= maximum.y) {
            record->flags |= 8;
            if (mode != 0) {
                EnemySpawnCancelEffect(
                    position, 8, -1, -1.5707964f, 0.6f);
            }

            int *vmId = EnemyCreateCancelVm(primaryEnemyResource);
            int vm = EnemyResolveCancelVm(*vmId);
            AnmSpriteView *colorOwner = record->vm.loadedSprite;
            if (colorOwner != 0) {
                float value = colorOwner->width;
                short colorIndex =
                    record->color;
                unsigned int color;
                if (value > g_EnemyCancelHighThreshold) {
                    color = g_EnemyCancelColorHigh[colorIndex];
                }
                else if (value > g_EnemyCancelUpperThreshold) {
                    color = g_EnemyCancelColorMid[colorIndex];
                }
                else {
                    color = g_EnemyCancelColorLow[colorIndex];
                }
                reinterpret_cast<AnmVmView *>(vm)->primaryColor.value = color;
            }
            record->unknown450 = 0;
        }
    }
    return 0;
}

struct EnemyBulletPositionView
{
    float x;
    float y;
    float z;

    __declspec(noinline) int IsOutsidePlayfield(float marginX, float marginY);
};


__declspec(noinline) int EnemyBulletPositionView::IsOutsidePlayfield(
    float marginX, float marginY)
{
    if (!(x + marginX <= g_EnemyPlayfieldMaxX) &&
        !(x - marginX >= g_EnemyPlayfieldMinX) &&
        !(y + marginY <= g_EnemyPlayfieldMaxY) &&
        !(y - marginY >= g_EnemyPlayfieldMinY)) {
        return 0;
    }
    return 1;
}

__declspec(noinline) unsigned int EnemyCancelBulletRecord(
    unsigned char *bullet)
{
    BulletRuntimeView *record = reinterpret_cast<BulletRuntimeView *>(bullet);
    short state = record->state;
    if (state != 2 && state != 1) {
        return 0;
    }

    EnemyBulletPositionView *position =
        reinterpret_cast<EnemyBulletPositionView *>(&record->position);
    if (position->IsOutsidePlayfield(8.0f, 8.0f) == 0) {
        record->state = 3;
        record->vm.pendingInterrupt = 1;
        int zero = 0;
        int effectScript = record->effectScript;
        if (effectScript >= 0) {
            g_EnemyPrimaryResourceOwner->primaryEnemyResource->
                CreateVmAtWorldVariant0(
                    effectScript,
                    reinterpret_cast<const AnmFloat3View *>(&record->position));
        }

        unsigned int *timerFlags =
            &record->stateTimer.flags;
        unsigned int timerFlagValue = *timerFlags;
        if ((timerFlagValue & 1u) == 0) {
            timerFlagValue |= 1u;
            record->stateTimer.current = zero;
            record->stateTimer.previous = -999999;
            record->stateTimer.subframe = 0.0f;
            record->stateTimer.scale = &g_AnmGameSpeed;
            *timerFlags = timerFlagValue;
        }
        record->stateTimer.current = zero;
        record->stateTimer.subframe = 0.0f;
        record->stateTimer.previous = -1;
        return 1;
    }
    record->state = 3;
    record->vm.pendingInterrupt = 1;
    record->flags |= 8;
    return 1;
}

struct EnemyCircleDisplacementView : PlayerFloat3
{
    EnemyCircleDisplacementView(float xValue, float yValue, float zValue)
    {
        x = xValue;
        y = yValue;
        z = zValue;
    }
};

__declspec(noinline) unsigned int EnemyCancelManagerView::CancelBulletPattern(
    const PlayerFloat3 *position, float radius, int mode, int flags)
{
    BulletRuntimeView *bullet =
        reinterpret_cast<BulletRuntimeView *>(bulletStorage);
    for (int remaining = 2000; remaining != 0;
         --remaining, ++bullet) {
        short state = bullet->state;
        if (state == 0 || state == 3) {
            continue;
        }
        if (flags != 0 && bullet->cancelBehavior != 0) {
            continue;
        }

        PlayerFloat3 displacement = EnemyCircleDisplacementView(
            bullet->position.x - position->x,
            bullet->position.y - position->y,
            bullet->position.z - position->z);
        float effectiveRadius =
            bullet->collisionWidth *
                0.5f +
            radius;
        if (!(displacement.x * displacement.x +
                displacement.y * displacement.y > effectiveRadius * effectiveRadius)) {
            EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(bullet));
            EnemyBulletPositionView *bulletPosition =
                reinterpret_cast<EnemyBulletPositionView *>(&bullet->position);
            if (bulletPosition->IsOutsidePlayfield(2.0f, 2.0f) == 0 &&
                mode != 0) {
                g_ItemManager->Spawn(
                    reinterpret_cast<const AnmFloat3View *>(bulletPosition),
                    8, -1, -1.5707964f, 0.6f);
            }
        }
    }
    return 0;
}



extern void EnemyEndSpell(EnemyGameStateView *gameState);
__declspec(noinline) void EnemyEnableBombShield(
    EnemyGameStateView *gameState)
{
    unsigned int *flags = &gameState->flags378C;
    *flags |= 0x10;
    g_AnmRenderManagerView->MarkVmForDeletion(
        *reinterpret_cast<AnmVmIdView *>(&gameState->managedVmId));
    gameState->managedVmId = 0;
}
// Narrow storage view: only the target-confirmed 12-byte destination at
// laser object +0x430 is interpreted; surrounding laser subtype fields are
// deliberately opaque and this does not assert an original class identity.
struct EnemyLaserPositionAt430View
{
    unsigned char unknown000[0x430];
    PlayerFloat3 position430;
};
typedef char EnemyLaserPositionAt430Layout[
    (offsetof(EnemyLaserPositionAt430View, position430) == 0x430 &&
     sizeof(EnemyLaserPositionAt430View) == 0x43c) ? 1 : -1];

// ECL SET_LASER_SOUND writes a binary32 value at laser receiver +0x440.
// The concrete subtype is not known for every returned laser, so this
// deliberately does not attach the word to a speculative derived class.
struct EnemyLaserControlFloatAt440View
{
    unsigned char unknown000[0x440];
    float value440;
};
typedef char EnemyLaserControlFloatAt440Layout[
    (offsetof(EnemyLaserControlFloatAt440View, value440) == 0x440 &&
     sizeof(EnemyLaserControlFloatAt440View) == 0x444) ? 1 : -1];

static __declspec(noinline) void EnemySetLaserPosition(
    int laser, const PlayerFloat3 *position)
{
    reinterpret_cast<EnemyLaserPositionAt430View *>(laser)->position430 =
        *position;
}
static __declspec(noinline) int EnemyFindLaser(
    EnemyLaserManagerView *manager, int id)
{
    EnemyLaserNodeView *laser = manager->head;
    while (laser != 0) {
        if (laser->id == id) {
            return reinterpret_cast<int>(laser);
        }
        laser = laser->next;
    }
    return 0;
}
extern void __stdcall EnemyKillAll(EnemyManagerView *manager);
// Full Enemy +0x2494 holds eight native 0x10-byte callback records.
// The timer-name store uses the original record-scaled +0x24A origin.
enum EnemyCallbackRecordAddress
{
    ENEMY_CALLBACK_FULL_BASE =
        offsetof(EnemyFullObjectView, runtime) +
        offsetof(EnemyRuntimeView, callbackThresholds),
    ENEMY_CALLBACK_TIMER_SCALED_BASE =
        (ENEMY_CALLBACK_FULL_BASE +
         offsetof(EnemyCallbackThresholdView, timerCallbackName)) /
        sizeof(EnemyCallbackThresholdView)
};
typedef char EnemyCallbackRecordNativeAddresses[
    (ENEMY_CALLBACK_FULL_BASE == 0x2494 &&
     ENEMY_CALLBACK_TIMER_SCALED_BASE == 0x24a &&
     sizeof(EnemyCallbackThresholdView) == 0x10) ? 1 : -1];

static __declspec(noinline) void __stdcall EnemyConfigureInterrupt(
    EnemyFullObjectView *owner, int slot, int lifeThreshold,
    int timerThreshold, const unsigned char *callbackName)
{
    unsigned char *record = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(owner) +
        static_cast<unsigned int>(slot) *
            static_cast<unsigned int>(sizeof(EnemyCallbackThresholdView)));
    *reinterpret_cast<int *>(record + ENEMY_CALLBACK_FULL_BASE) = lifeThreshold;
    if (lifeThreshold >= 0) {
        EnemyCallbackThresholdView *threshold =
            reinterpret_cast<EnemyCallbackThresholdView *>(
                record + ENEMY_CALLBACK_FULL_BASE);
        threshold->timerThreshold = timerThreshold;
        threshold->lifeCallbackName = callbackName;
        *reinterpret_cast<const unsigned char **>(
            reinterpret_cast<unsigned int>(owner) +
            (static_cast<unsigned int>(slot) +
             static_cast<unsigned int>(ENEMY_CALLBACK_TIMER_SCALED_BASE)) *
            static_cast<unsigned int>(sizeof(EnemyCallbackThresholdView))) = callbackName;
    }
}

static __declspec(noinline) void EnemyInitializeScalarInterpolation(
    EnemyScalarInterpolationView *interpolation)
{
    EnemyInterpolationTimerView &timer = interpolation->timer;
    unsigned int flags = timer.flags;
    if ((flags & 1) == 0) {
        flags |= 1;
        timer.current = 0;
        timer.previous = -999999;
        timer.subframe = 0.0f;
        timer.scale = &g_AnmGameSpeed;
        timer.flags = flags;
    }
    timer.current = 0;
    timer.subframe = 0.0f;
    timer.previous = -1;
}
static __declspec(noinline) void EnemySetChapter(
    EnemyChapterStateView *state, int chapter)
{
    if (state->chapter == chapter) {
        state->chapter = chapter;
        return;
    }
    state->chapter = chapter;
    state->chapterTimer = 0;
}
extern void __stdcall EnemySetMotionAngle(EnemyMotionView *motion, float angle);
extern int g_EnemyDialogIndex;
extern int g_EnemyDialogState;

// In the observed GUI message file's indexed table, each entry is eight
// bytes. Original EnemyReadDialog loads the signed content offset from
// entry +4 and adds it to the file base after message allocation.
struct EnemyDialogFileIndexEntryView
{
    unsigned int unknown000;
    int contentOffset;
};
typedef char EnemyDialogFileIndexEntryLayout[
    (sizeof(EnemyDialogFileIndexEntryView) == 8 &&
     offsetof(EnemyDialogFileIndexEntryView, contentOffset) == 4) ? 1 : -1];

static void EnemyReadDialog(GuiView *gui, int index)
{
    // The new-expression allocates the message before either file dereference.
    // Keep both loads at their original source evaluation point.
    GuiMessageVmView *dialog = new GuiMessageVmView(
        reinterpret_cast<unsigned char *>(
            reinterpret_cast<unsigned int>(gui->messageFile) +
            static_cast<unsigned int>(
                EnemyNativeWordIndex(
                    reinterpret_cast<const EnemyDialogFileIndexEntryView *>(gui->messageFile),
                    index)->contentOffset)));
    gui->message = dialog;
    dialog->messageIndex = index;
    int nextIndex = static_cast<int>(static_cast<unsigned int>(index) + 1u);
    int previousIndex = g_EnemyDialogIndex;
    g_EnemyDialogIndex = nextIndex;
    if (previousIndex != nextIndex) {
        g_EnemyDialogState = 0;
    }
}
static __declspec(noinline) unsigned int EnemyWaitForEffect(
    EnemyEffectWaitManager *manager)
{
    EnemyEffectWaitNode *node = manager->head;
    while (node != 0) {
        EnemyEffectWaitNode *next = node->next;
        unsigned char flag = node->flag50;
        if (node->state != 1 && flag == 0) {
            node->flag50 = 1;
        }
        node = next;
    }
    return 0;
}
__declspec(noinline) unsigned int EnemyEffectWaitManager::ApplyPatternCancel(
    const PlayerFloat3 *position, float radius, int mode)
{
    cancelPosition = *position;
    unsigned int total = 0;
    EnemyEffectWaitNode *node = head;
    while (node != 0) {
        EnemyEffectWaitNode *next = node->next;
        if (node->state != 1) {
            total += node->ApplyPatternCancel(position, radius, mode);
        }
        node = next;
    }
    return total;
}

__declspec(noinline) unsigned int EnemyApplyBulletClear(
    EnemyEffectWaitManager *manager, int mode)
{
    EnemyEffectWaitNode *node = manager->head;
    while (node != 0) {
        EnemyEffectWaitNode *next = node->next;
        if (node->state != 1) {
            node->ApplyClear(mode);
        }
        node = next;
    }
    return 1;
}
extern unsigned int * __stdcall EnemySetScreenShake(
    int enabled, int duration, unsigned int horizontal,
    unsigned int vertical, unsigned int flags);
struct EnemySoundCueMetadataView
{
    int unknown000;
    unsigned short unknown004;
    short cueValue;
};

// Sound IDs come from ECL, not a checked host array index. The original
// i386 lookup multiplies the signed ID as one 32-bit word and reads the
// 0x06 cue halfword of an 8-byte metadata record.
typedef char EnemySoundCueNativeRecordLayout[
    (sizeof(EnemySoundCueMetadataView) == 8 &&
     offsetof(EnemySoundCueMetadataView, cueValue) == 6) ? 1 : -1];



extern EnemySoundCueMetadataView g_EnemySoundCueMetadata[];

void EnemySoundQueueView::QueueSoundCue(int soundId, float positionX)
{
    float scaledPosition = positionX * 5.208333492279053f;
    int cueValue = EnemyNativeWordIndex(g_EnemySoundCueMetadata, soundId)->cueValue;
    int sample = static_cast<int>(scaledPosition);
    int slot = 0;
    while (slot < 12) {
        int activeId = activeSoundIds[slot];
        if (activeId < 0) {
            break;
        }
        if (activeId == soundId) {
            int count = sampleCounts[slot];
            if (count >= 128) {
                return;
            }
            // The queue rejects counts >= 128 but never rejects a negative
            // stored count. Retail indexes this sample slot in flat i386
            // address words, without creating a bounded C++ array pointer.
            *EnemyNativeWordIndex(&samples[slot][0], count) = sample;
            sampleCounts[slot] = sampleCounts[slot] + 1;
            return;
        }
        ++slot;
    }
    if (slot < 12) {
        activeSoundIds[slot] = soundId;
        *EnemyNativeWordIndex(&cueValues[0], soundId) = cueValue;
        samples[slot][0] = sample;
        ++sampleCounts[slot];
    }
}
void EnemySoundQueueView::QueueSoundSample(int soundId, int sample)
{
    int cueValue = EnemyNativeWordIndex(g_EnemySoundCueMetadata, soundId)->cueValue;
    int slot = 0;
    while (slot < 12) {
        int activeId = activeSoundIds[slot];
        if (activeId < 0) {
            break;
        }
        if (activeId == soundId) {
            int count = sampleCounts[slot];
            if (count >= 128) {
                return;
            }
            // The queue rejects counts >= 128 but never rejects a negative
            // stored count. Retail indexes this sample slot in flat i386
            // address words, without creating a bounded C++ array pointer.
            *EnemyNativeWordIndex(&samples[slot][0], count) = sample;
            sampleCounts[slot] = sampleCounts[slot] + 1;
            return;
        }
        ++slot;
    }
    if (slot < 12) {
        activeSoundIds[slot] = soundId;
        *EnemyNativeWordIndex(&cueValues[0], soundId) = cueValue;
        samples[slot][0] = sample;
        ++sampleCounts[slot];
    }
}

extern __declspec(noinline) float __stdcall EnemyWrapAngle(float angle);

struct EnemyDropVectorView
{
    float x;
    float y;
    float z;

    __declspec(noinline) void FromAngleRadii(
        float angle, float radiusX, float radiusY)
    {
#if defined(_MSC_VER) && defined(_M_IX86)
        __asm
        {
            mov eax, this
            fld angle
            fsincos
            fmul radiusX
            fstp [eax]
            fmul radiusY
            fstp [eax + 4]
        }
#else
        x = static_cast<float>(cos(angle)) * radiusX;
        y = static_cast<float>(sin(angle)) * radiusY;
#endif
    }
};

// Scatter storage/order informed by th10-decomp (MIT), commit
// c6791d82dc9f02e55eca45a5314f900a85481efd; TH10 target/native evidence is authoritative.
// See docs/licenses/th10-decomp-MIT.txt.
__declspec(noinline) void __stdcall EnemyDropItemCountsCore(
    const PlayerFloat3 *position, int *itemDropBlock)
{
    EnemyItemDropCountRadiusView *scatter =
        reinterpret_cast<EnemyItemDropCountRadiusView *>(itemDropBlock);
    float angle = g_RngView.GetRandomF32Signed() * 3.1415927f;

    for (int itemIndex = 0; itemIndex < 11; ++itemIndex) {
        for (int i = 0; i < scatter->countsByKind[itemIndex]; ++i) {
            EnemyDropVectorView offset;
            offset.FromAngleRadii(
                angle,
                scatter->radiusX, scatter->radiusY);

            unsigned int radiusBits = g_RngView.GetRandomU32();
            // VC7.1 supplies the target unsigned high-bit correction once.
            float radiusScale = static_cast<float>(radiusBits);
            const float height = position->z;
            radiusScale =
                radiusScale * 1.1641532182693481e-10f + 0.5f;

            offset.x *= radiusScale;
            offset.y *= radiusScale;
            offset.x += position->x;
            offset.y += position->y;
            offset.z = height;

            g_ItemManager->Spawn(
                reinterpret_cast<const AnmFloat3View *>(&offset),
                itemIndex + 1, 0xffffffffu, -1.5707964f, 2.2f);

            unsigned int angleBits = g_RngView.GetRandomU32();
            float angleRandom = static_cast<float>(angleBits);
            angleRandom =
                angleRandom * 4.656612873077393e-10f - 1.0f;
            angle = EnemyWrapAngle(
                angleRandom * 0.7853982f + angle + 1.5707964f);
        }
    }

    // Clear the eleven counts and the following opaque word, *not* the
    // separately persisted X/Y radii at packet +0x34/+0x38.
    memset(scatter->countsByKind, 0,
        sizeof(scatter->countsByKind) + sizeof(scatter->unknown030));
}

static __declspec(noinline) void EnemyDropItemCounts(
    const PlayerFloat3 *position, int *itemDropBlock)
{
    EnemyItemDropCountRadiusView *scatter =
        reinterpret_cast<EnemyItemDropCountRadiusView *>(itemDropBlock);
    if (scatter->mainItemType > 0) {
        g_ItemManager->Spawn(
            reinterpret_cast<const AnmFloat3View *>(position),
            scatter->mainItemType, -1, -1.5707964f, 2.2f);
    }
    EnemyDropItemCountsCore(position, itemDropBlock);
    scatter->mainItemType = 0;
}

extern float EnemyRandomAngle();

static float EnemyAbsoluteFloat(float value)
{
    return value < 0.0f ? -value : value;
}

// Target 0x0044BC70-0x0044BCCD wraps one angle into the signed-pi interval.
// The shared counter and 32-iteration guard are both visible in the target.
__declspec(noinline) float __stdcall EnemyWrapAngle(float angle)
{
    int i = 0;
    while (angle > 3.1415927f)
    {
        angle -= 6.2831855f;
        if (i++ > 32)
            break;
    }
    while (angle < -3.1415927f)
    {
        angle += 6.2831855f;
        if (i++ > 32)
            break;
    }
    return angle;
}

int EnemyRuntimeView::ReadIntArgument(int index)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ReadInt(index);
}

int *EnemyRuntimeView::ResolveIntArgument(int index)
{
    return reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ResolveInt(index);
}

float EnemyRuntimeView::ReadFloatArgument(int index)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ReadFloat(index);
}

float *EnemyRuntimeView::ResolveFloatArgument(int index)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ResolveFloat(index);
}

int EnemyRuntimeView::ReadRawIntArgument(int index, int rawValue)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ReadIntValue(index, rawValue);
}

float EnemyRuntimeView::ReadRawFloatArgument(int index, float rawValue)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return EclVmContext::ReadFloatValue(index, host->activeContext, rawValue);
}

#define ENEMY_READ_INT_DIRECT(index) \
    (reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ReadInt(index))

int EnemyRuntimeView::DispatchEclInstruction()

{
  // The ECL bullet-pattern pointer arithmetic retains its original signed
  // integer cursor lifetime, while record stride and starting address are
  // now tied to the reviewed layout instead of numeric duplicates.
  // Indexed bullet launch offsets use the genuine 12-byte float3 records
  // at runtime +0x1344; zeroing keeps the original word-wise bit stores.
  // Native motion base/offset pointers and copied position words now name
  // the existing 0x2C-byte motion records; laser copies start at the
  // existing first bullet pattern's word 8, not an invented raw base.
  // Remaining life/drops, collision sizes, mode state and host back-pointer
  // use the native EnemyRuntimeView members rather than the old absolute
  // offsets. Keep squared protection range at +0x1454 as a float view of
  // the still-indeterminate backing dword, not a new layout field.
  // The offscreen/movement bounding rectangle and eight bullet pattern
  // offsets are the independently checked native runtime members, not
  // duplicate pointer-arithmetic field projections.
  // Native motion coordinates at +0x2c/+0x30/+0x34 are the typed world
  // position; view their bits as words only at the target's bit-copy sites.
  // The animation state +0xe8..+0xf8 is the existing named runtime storage.
  // Runtime flags at +0x1444 are the native EnemyRuntimeView::flags
  // member, independently layout-checked in Enemy.hpp. Preserve the
  // target's byte-wide tests by explicitly reading its low byte.
  int runtimeAddress = reinterpret_cast<int>(this);
  short opcode;
  float fVar2;
  unsigned char bVar3;
  unsigned int uVar4;
  unsigned int *puVar5;
  unsigned int uVar6;
  int *piVar7;
  unsigned int uVar8;
  float fVar9;
  float fVar10;
  int iVar11;
  unsigned int *puVar13;
  unsigned int *puVar14;
  float *pfVar15;
  unsigned char *pbVar16;
  int iVar17;
  float fVar19;
  float fVar20;
  float fVar21;
  int uVar22;
  int uVar23;
  int uVar24;
  int uVar25;
  int iVar26;
  int iVar27;
  const EnemyEclInstructionView *instruction;
  int secondLowBulletCount;
  unsigned int uVar28;
  EnemyMotionView *selectedMotion;
  EnemyPositionInterpolationView *positionInterpolation;
  float *positionInterpolationValues;
  EnemyScalarInterpolationView *firstScalarInterpolation;
  EnemyScalarInterpolationView *secondScalarInterpolation;
  float *firstScalarValues;
  float *secondScalarValues;
  EnemyFloat2 initialPolar;
  EnemyFloat2 finalPolar;
  float local_2c8;
  float local_2c4;
  float local_2bc;
  PlayerFloat3 vectorScratch;
  float local_2a8;
  float local_2a4;
  char local_29d;
  float local_29c;
  float local_298;
  EnemyLaserRequestScratch local_288;
  typedef char EclLaserRequestPatternPositions[
      (offsetof(EnemyLaserType0RequestView, position) == 0x00 &&
       offsetof(EnemyLaserType0RequestView, patternWords) == 0x2c &&
       offsetof(EnemyLaserType1RequestView, position) == 0x00 &&
       offsetof(EnemyLaserType1RequestView, patternWords) == 0x48) ? 1 : -1];
  // The two straight-laser layouts share this native 0x1F8 request overlay.
  // These offsets are established by the independent type-1 laser consumer.
  typedef char EclStraightLaserRequestOffsets[
      (offsetof(EnemyLaserType1RequestView, angle) == 0x18 &&
       offsetof(EnemyLaserType1RequestView, initialLength) == 0x24 &&
       offsetof(EnemyLaserType1RequestView, maximumLength) == 0x20 &&
       offsetof(EnemyLaserType1RequestView, width) == 0x28 &&
       offsetof(EnemyLaserType1RequestView, waitFrames) == 0x30 &&
       offsetof(EnemyLaserType1RequestView, growFrames) == 0x34 &&
       offsetof(EnemyLaserType1RequestView, holdFrames) == 0x38 &&
       offsetof(EnemyLaserType1RequestView, shrinkFrames) == 0x3c &&
       offsetof(EnemyLaserType1RequestView, type) == 0x40 &&
       offsetof(EnemyLaserType1RequestView, color) == 0x42 &&
       offsetof(EnemyLaserType1RequestView, flags44) == 0x44) ? 1 : -1];
  union {
    EnemySpawnRequestView spawnRequest;
    float floatWords[16];
    unsigned char bytes[0x84];
  } local_90;

  iVar26 = reinterpret_cast<int>(owner);
  instruction = owner->activeEclContext->currentInstruction;
  opcode = instruction->opcode;
  switch(opcode) {
  case ENEMY_ECL_CREATE_ENEMY_IF_NO_BOSS:
    if (reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]) != 0) {
      return 0;
    }
  // Inline SCPT name byte counts are advanced by one native dword
  // before division into scalar-operand slots.
  case ENEMY_ECL_CREATE_ENEMY:
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(
            instruction
                ->spawnOperands.inlineNameByteCount) + 4u);
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    if ((flags & 0x40000) == 0) {
      fVar9 = worldMotion.position.x;
      fVar19 = (ReadRawFloatArgument((1), (EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26))->real)));
      fVar10 = worldMotion.position.y;
      local_90.spawnRequest.position.x = (float)(fVar19 + fVar9);
      fVar19 = (ReadRawFloatArgument((2), (EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 1u)->real)));
      fVar19 = fVar19 + fVar10;
    }
    else {
      fVar9 = worldMotion.position.x;
      fVar19 = (ReadRawFloatArgument((1), (EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26))->real)));
      fVar10 = worldMotion.position.y;
      vectorScratch.x = (float)(fVar19 + fVar9);
      vectorScratch.y = ReadRawFloatArgument(
          2, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 1u)->real) + fVar10;
      vectorScratch.z = worldMotion.position.z;
      EnemyPrepareProjection(&g_MainSupervisorView, 0);
      D3DXVec3Project(&local_90.spawnRequest.position, &vectorScratch,
                      &g_EnemyViewport, &g_EnemyProjectionMatrix,
                      &g_EnemyViewMatrix, &g_EnemyWorldMatrix);
      local_90.spawnRequest.position.x = local_90.spawnRequest.position.x - 224.0f;
      local_90.spawnRequest.position.z = 0.0;
      fVar19 = local_90.spawnRequest.position.y - 16.0f;
    }
    local_90.spawnRequest.position.y = (float)fVar19;
    uVar22 = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 2u)->integer);
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 3u)->integer);
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 4u)->integer);
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
dispatch_spawn_enemy:
    EnemySpawn(
        g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_IF_NO_BOSS:
    if (reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]) != 0) {
      return 0;
    }
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE:
dispatch_create_enemy_absolute:
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(
            instruction
                ->spawnOperands.inlineNameByteCount) + 4u);
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    fVar19 = (ReadRawFloatArgument((1), (EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26))->real)));
    local_90.spawnRequest.position.x = (float)fVar19;
    fVar19 = (ReadRawFloatArgument((2), (EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 1u)->real)));
    local_90.spawnRequest.position.y = (float)fVar19;
    uVar22 = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 2u)->integer);
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 3u)->integer);
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 4u)->integer);
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
    goto dispatch_spawn_enemy;
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET:
dispatch_create_enemy_global:
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(
            instruction
                ->spawnOperands.inlineNameByteCount) + 4u);
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    local_90.spawnRequest.position.x =
        ReadRawFloatArgument(
            1, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26))->real) +
        g_EnemyGlobalPositionOffset.x;
    local_90.spawnRequest.position.y =
        ReadRawFloatArgument(
            2, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 1u)->real) +
        g_EnemyGlobalPositionOffset.y;
    local_90.spawnRequest.position.z =
        ReadRawFloatArgument(
            3, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 2u)->real);
    uVar22 = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 3u)->integer);
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 4u)->integer);
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(6, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 5u)->integer);
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
    local_90.spawnRequest.setFlag40000 = local_90.spawnRequest.setFlag40000 | 1;
    EnemySpawn(
        g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET_IF_NO_BOSS:
    if (reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]) == 0) goto dispatch_create_enemy_global;
    break;
  case ENEMY_ECL_CREATE_ENEMY_MIRRORED:
dispatch_create_enemy_mirrored:
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(
            instruction
                ->spawnOperands.inlineNameByteCount) + 4u);
    fVar9 = worldMotion.position.x;
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    local_90.spawnRequest.position.x =
        ReadRawFloatArgument(
            1, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26))->real) + fVar9;
    fVar10 = worldMotion.position.y;
    local_90.spawnRequest.position.y =
        ReadRawFloatArgument(
            2, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 1u)->real) + fVar10;
    uVar22 = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 2u)->integer);
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 3u)->integer);
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 4u)->integer);
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
    local_90.spawnRequest.setFlag0800 = 1;
    EnemySpawn(
        g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED:
    goto dispatch_create_enemy_absolute_mirrored;
  case ENEMY_ECL_CREATE_ENEMY_MIRRORED_IF_NO_BOSS:
    if (reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]) == 0) goto dispatch_create_enemy_mirrored;
    break;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED_IF_NO_BOSS:
    if (reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]) != 0) {
      return 0;
    }
    goto dispatch_create_enemy_absolute_mirrored;
  // ANM resources have four declared slots, but script selectors are not
  // clamped by the target. Preserve the flat i386 dword index arithmetic.
  case ENEMY_ECL_SELECT_ANM_RESOURCE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    value0E8 = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_ANM_SCRIPT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->Release();
    if ((int)uVar22 < 0) {
      return 0;
    }
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          (*EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], value0E8))->
              CreateVmVariant2((int)uVar22, 5);
      *EnemyNativeWordIndex(&managedVmIds[0], iVar26) = createdVm.value;
    }
    uVar6 = *EnemyNativeWordIndex(&managedVmIds[0], iVar26);
    if (iVar26 == 0) {
      uVar22 = ENEMY_READ_INT_DIRECT(1);
      value0F0 = (int)uVar22;
      value0EC = value0E8;
      uVar6 = *EnemyNativeWordIndex(&managedVmIds[0], iVar26);
    }
    iVar27 = reinterpret_cast<int>(
        reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->GetVm());
    goto dispatch_update_primary_anm_bounds;
  case ENEMY_ECL_PLAY_ANM:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          (*EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], static_cast<int>(uVar22)))->
              CreateVmVariant2((int)uVar23, 6);
      iVar26 = reinterpret_cast<int>(createdVm.GetVm());
    }
    if ((flags & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          PublishEclWorldAnchor(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position)));
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[0] = puVar5[0];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[1] = puVar5[1];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[2] = puVar5[2];
      return 0;
    }
    reinterpret_cast<EnemyPositionDwordView *>(
        &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[0] =
        reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[0];
    reinterpret_cast<EnemyPositionDwordView *>(
        &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[1] =
        reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[1];
    reinterpret_cast<EnemyPositionDwordView *>(
        &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[2] =
        reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[2];
    return 0;
  case ENEMY_ECL_PLAY_ANM_HIGH:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          (*EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], static_cast<int>(uVar22)))->
              CreateVmVariant0((int)uVar23, 6);
      iVar26 = reinterpret_cast<int>(createdVm.GetVm());
    }
    if ((flags & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          PublishEclWorldAnchor(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position)));
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[0] = puVar5[0];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[1] = puVar5[1];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[2] = puVar5[2];
      return 0;
    }
    reinterpret_cast<EnemyPositionDwordView *>(
        &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[0] =
        reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[0];
    reinterpret_cast<EnemyPositionDwordView *>(
        &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[1] =
        reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[1];
    reinterpret_cast<EnemyPositionDwordView *>(
        &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[2] =
        reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[2];
    return 0;
  case ENEMY_ECL_PLAY_ANM_ABSOLUTE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    (*EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], static_cast<int>(uVar22)))->
        CreateVmVariant2((int)uVar23, 6);
    return 0;
  case ENEMY_ECL_PLAY_ANM_ROTATED:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          (*EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], static_cast<int>(uVar22)))->
              CreateVmVariant2((int)uVar23, 6);
      iVar26 = reinterpret_cast<int>(createdVm.GetVm());
    }
    if ((flags & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          PublishEclWorldAnchor(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position)));
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[0] = puVar5[0];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[1] = puVar5[1];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[2] = puVar5[2];
    }
    else {
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[0] =
          reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[0];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[1] =
          reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[1];
      reinterpret_cast<EnemyPositionDwordView *>(
          &reinterpret_cast<AnmVmView *>(iVar26)->positionOffset)->word[2] =
          reinterpret_cast<const EnemyPositionDwordView *>(&worldMotion.position)->word[2];
    }
    fVar19 = (ReadFloatArgument(2));
    reinterpret_cast<AnmVmView *>(iVar26)->rotation.z = (float)fVar19;
    reinterpret_cast<AnmVmView *>(iVar26)->flags35C |= 4u;
    return 0;
  case ENEMY_ECL_SET_MAIN_ANM_SCRIPT:
    // Script-selected VM IDs are unchecked in the target. Address all slots
    // with its native modulo-32 word stride, as in SET_ANM_SCRIPT above.
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    iVar27 = (int)uVar22;
    reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->Release();
    {
      AnmVmIdView createdVm =
          (*EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], value0E8))->
              CreateVmVariant2(iVar27, 5);
      *EnemyNativeWordIndex(&managedVmIds[0], iVar26) = createdVm.value;
    }
    uVar6 = *EnemyNativeWordIndex(&managedVmIds[0], iVar26);
    iVar11 = reinterpret_cast<int>(
        reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->GetVm());
    if (iVar26 == 0) {
      visibilityExtentX = reinterpret_cast<const AnmVmView *>(iVar11)->spriteHeight * reinterpret_cast<const AnmVmView *>(iVar11)->scaleY;
      visibilityExtentY = reinterpret_cast<const AnmVmView *>(iVar11)->spriteWidth * reinterpret_cast<const AnmVmView *>(iVar11)->scaleX;
    }
    if ((*reinterpret_cast<const unsigned char *>(&flags) & 0x10) != 0) {
      reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->ClearFlag2();
    }
    if (iVar26 == 0) {
      flags = flags | 0x1000;
      animationBaseScript = iVar27;
      value0F0 = iVar27;
      animationDirection = 0;
      value0EC = value0E8;
      return 0;
    }
    break;
  case ENEMY_ECL_PLAY_SELECTED_ANM:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->Release();
    {
      AnmVmIdView createdVm =
          (*EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], value0E8))->
              CreateVmVariant2(animationBaseScript + 5, 5);
      *EnemyNativeWordIndex(&managedVmIds[0], iVar26) = createdVm.value;
    }
    iVar27 = reinterpret_cast<int>(
        reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->GetVm());
dispatch_update_primary_anm_bounds:
    if (iVar26 == 0) {
      visibilityExtentX = reinterpret_cast<const AnmVmView *>(iVar27)->spriteHeight * reinterpret_cast<const AnmVmView *>(iVar27)->scaleY;
      visibilityExtentY = reinterpret_cast<const AnmVmView *>(iVar27)->spriteWidth * reinterpret_cast<const AnmVmView *>(iVar27)->scaleX;
    }
    if ((*reinterpret_cast<const unsigned char *>(&flags) & 0x10) != 0) {
      reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->ClearFlag2();
      return 0;
    }
    break;
  // Movement and interpolation instructions.
  case ENEMY_ECL_SET_OFFSET_POSITION:
  case ENEMY_ECL_SET_BASE_POSITION:
  {
    selectedMotion = &offsetMotion;
    if (opcode != 0x118) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    if (-999999.0 < (float)fVar19) {
      selectedMotion->position.x = (float)fVar19;
    }
    if (fVar21 > -999999.0) {
      selectedMotion->position.y = (float)fVar21;
    }
    selectedMotion->flags &= 0xfffffffe;
    *reinterpret_cast<AnmFloat3View *>(&worldMotion.position) =
        AnmFloat3View(baseMotion.position.x + offsetMotion.position.x,
                      baseMotion.position.y + offsetMotion.position.y,
                      baseMotion.position.z + offsetMotion.position.z);
    return 0;
  }
  case ENEMY_ECL_ADD_OFFSET_POSITION:
  case ENEMY_ECL_ADD_BASE_POSITION:
    pfVar15 = &offsetMotion.position.x;
    if (opcode != 0x128) {
      pfVar15 = &baseMotion.position.x;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    local_2a8 = (float)fVar21;
    fVar21 = (ReadFloatArgument(2));
    *pfVar15 = (float)fVar19 + *pfVar15;
    pfVar15[1] = local_2a8 + pfVar15[1];
    pfVar15[2] = (float)(fVar21 + pfVar15[2]);
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_POSITION:
  case ENEMY_ECL_INTERPOLATE_BASE_POSITION:
    selectedMotion = &offsetMotion;
    if (opcode != ENEMY_ECL_INTERPOLATE_OFFSET_POSITION) {
      selectedMotion = &baseMotion;
      positionInterpolation = &positionInterpolation1;
    }
    else {
      positionInterpolation = &positionInterpolation0;
    }
    positionInterpolationValues = reinterpret_cast<float *>(positionInterpolation);
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    positionInterpolation->duration = uVar22;
    positionInterpolation->initialTangent = g_EnemyInterpolationOrigin;
    positionInterpolation->finalTangent = g_EnemyInterpolationOrigin;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    positionInterpolation->mode = uVar22;
    positionInterpolation->initial = selectedMotion->position;
    if (-999999.0 < local_2c4) {
      vectorScratch.y = local_2c4;
    }
    else {
      vectorScratch.y = selectedMotion->position.y;
    }
    if (-999999.0 < local_2c8) {
      vectorScratch.x = local_2c8;
    }
    else {
      vectorScratch.x = selectedMotion->position.x;
    }
    vectorScratch.z = 0.0;
    positionInterpolation->final = vectorScratch;
    EnemyInitializePositionInterpolation(positionInterpolation);
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_SET_OFFSET_POLAR:
  case ENEMY_ECL_SET_BASE_POLAR:
    selectedMotion = &offsetMotion;
    if (opcode != 0x11c) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    local_2bc = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    if (-999999.0 < local_2bc) {
      if ((flags & 0x800) != 0) {
        fVar21 = EnemyWrapAngle(local_2bc - 1.5707964f);
        fVar21 = EnemyWrapAngle((float)(1.5707964f - fVar21));
        local_2bc = (float)fVar21;
      }
      EnemySetMotionAngle(selectedMotion, local_2bc);
    }
    if (-999999.0 < (float)fVar19) {
      selectedMotion->value18 = (float)fVar19;
    }
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_POLAR:
  case ENEMY_ECL_INTERPOLATE_BASE_POLAR:
  {
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolation0;
    if (opcode == 0x11d) {
      firstScalarInterpolation = &scalarInterpolation0;
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolation1;
    }
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    firstScalarInterpolation->mode = uVar22;

    if (uVar22 != 7) {
      if (!(-999999.0 < local_2c8)) {
        finalPolar.x = selectedMotion->value1C;
      }
      else {
        finalPolar.x = local_2c8;
        if ((flags & 0x800) != 0) {
          fVar19 = EnemyWrapAngle((float)(finalPolar.x - 1.5707964f));
          finalPolar.x = (float)EnemyWrapAngle((float)(1.5707964f - fVar19));
        }
      }
      if (!(-999999.0 < local_2c4)) {
        finalPolar.y = selectedMotion->value18;
      }
      else {
        finalPolar.y = local_2c4;
      }
    }
    else {
      if (-999999.0 < local_2c8) {
        finalPolar.x = local_2c8;
      }
      else {
        finalPolar.x = 0.0;
      }
      if (-999999.0 < local_2c4) {
        finalPolar.y = local_2c4;
      }
      else {
        finalPolar.y = 0.0;
      }
    }

    initialPolar.x = selectedMotion->value1C;
    initialPolar.y = selectedMotion->value18;
    if (3.1415927f <= static_cast<float>(fabs(initialPolar.x - finalPolar.x))) {
      if (initialPolar.x < finalPolar.x) {
        initialPolar.x = initialPolar.x + 6.2831855f;
      }
      else {
        finalPolar.x = finalPolar.x + 6.2831855f;
      }
    }
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent.x = g_EnemyInterpolationBasis.x;
    goto dispatch_initialize_polar_interpolation;
  }
  case ENEMY_ECL_SET_OFFSET_CIRCLE:
  case ENEMY_ECL_SET_BASE_CIRCLE:
    selectedMotion = &offsetMotion;
    if (opcode != 0x120) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    fVar20 = (ReadFloatArgument(2));
    local_2a8 = (float)fVar20;
    fVar20 = (ReadFloatArgument(3));
    if ((selectedMotion->flags & 1) == 0) {
      selectedMotion->velocity = selectedMotion->position;
    }
    if (-999999.0 < (float)fVar19) {
      EnemySetMotionAngle(selectedMotion, (float)fVar19);
    }
    if (-999999.0 < (float)fVar21) {
      selectedMotion->value18 = (float)fVar21;
    }
    if (-999999.0 < local_2a8) {
      selectedMotion->value20 = local_2a8;
    }
    if (-999999.0 < (float)fVar20) {
      selectedMotion->value24 = (float)fVar20;
    }
    selectedMotion->flags |= 1;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_CIRCLE:
  case ENEMY_ECL_INTERPOLATE_BASE_CIRCLE:
    selectedMotion = &offsetMotion;
    if (opcode != 0x121) {
      selectedMotion = &baseMotion;
    }
    firstScalarInterpolation = &scalarInterpolation0;
    if (opcode == 0x121) {
      secondScalarInterpolation = &scalarInterpolation2;
    }
    else {
      firstScalarInterpolation = &scalarInterpolation1;
      secondScalarInterpolation = &scalarInterpolation3;
    }
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_2bc = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));

    if (-999999.0 < local_2c4) {
      local_298 = local_2c4;
    }
    else {
      local_298 = selectedMotion->value18;
    }
    if (-999999.0 < local_2c8) {
      local_29c = local_2c8;
    }
    else {
      local_29c = selectedMotion->value1C;
    }

    local_2a4 = selectedMotion->value18;
    local_2a8 = selectedMotion->value1C;

    if (!(fVar19 > -999999.0)) {
      fVar19 = selectedMotion->value24;
    }
    if (-999999.0 < local_2bc) {
      vectorScratch.x = local_2bc;
    }
    else {
      vectorScratch.x = selectedMotion->value20;
    }
    vectorScratch.y = (float)fVar19;
    uVar6 = selectedMotion->value24Bits;
    uVar4 = selectedMotion->value20Bits;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    firstScalarValues = reinterpret_cast<float *>(firstScalarInterpolation);
    firstScalarInterpolation->duration = (int)uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->initial.x = local_2a8;
    firstScalarInterpolation->final.y = local_298;
    firstScalarInterpolation->mode = (int)uVar23;
    firstScalarInterpolation->initial.y = local_2a4;
    firstScalarInterpolation->final.x = local_29c;
    EnemyInitializeScalarInterpolation(firstScalarInterpolation);
    secondScalarValues = reinterpret_cast<float *>(secondScalarInterpolation);
    secondScalarInterpolation->duration = (int)uVar22;
    secondScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    secondScalarInterpolation->finalTangent = g_EnemyInterpolationBasis;
    reinterpret_cast<EnemyFloat2NativeBitView *>(&secondScalarInterpolation->initial)->words[0] = uVar4;
    secondScalarInterpolation->final.y = vectorScratch.y;
    secondScalarInterpolation->mode = (int)uVar23;
    reinterpret_cast<EnemyFloat2NativeBitView *>(&secondScalarInterpolation->initial)->words[1] = uVar6;
    secondScalarInterpolation->final.x = vectorScratch.x;
    EnemyInitializeScalarInterpolation(secondScalarInterpolation);
    selectedMotion->velocity = selectedMotion->position;
    selectedMotion->flags |= 1;
    return 0;
  // Movement and game-state instructions in target case order.
  case ENEMY_ECL_ADD_OFFSET_VELOCITY:
  case ENEMY_ECL_ADD_BASE_VELOCITY:
    iVar26 = reinterpret_cast<int>(&offsetMotion);
    if (opcode != 0x12a) {
      iVar26 = reinterpret_cast<int>(&baseMotion);
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    if (-999999.0 < (float)fVar19) {
      reinterpret_cast<EnemyMotionView *>(iVar26)->velocity.x = (float)fVar19;
    }
    if (fVar21 > -999999.0) {
      reinterpret_cast<EnemyMotionView *>(iVar26)->velocity.y = (float)fVar21;
    }
    goto dispatch_copy_player_to_motion;
  case ENEMY_ECL_COPY_PLAYER_TO_OFFSET:
    goto dispatch_copy_player_to_motion;
  case ENEMY_ECL_COPY_PLAYER_TO_BASE:
    iVar26 = reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]);
    baseMotion.position =
        reinterpret_cast<EnemyFullObjectView *>(iVar26)->runtime.worldMotion.position;
    return 0;
  case ENEMY_ECL_SET_MOVEMENT_BOUNDS:
    flags = flags | 0x200;
    fVar19 = (ReadFloatArgument(0));
    movementBoundsCenterX = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    movementBoundsCenterY = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    movementBoundsSizeX = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    movementBoundsSizeY = (float)fVar19;
    return 0;
  case ENEMY_ECL_CLEAR_MOVEMENT_BOUNDS:
    flags = flags & 0xfffffdff;
    return 0;
  case ENEMY_ECL_SET_BULLET_PROTECTION_RANGE:
    fVar19 = (ReadFloatArgument(0));
    bulletProtectionRadiusSquared = (float)(fVar19 * fVar19);
    break;
  case ENEMY_ECL_MOVE_RANDOM:
  case ENEMY_ECL_MOVE_RANDOM_BASE:
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolation0;
    if (opcode == 0x124) {
      firstScalarInterpolation = &scalarInterpolation0;
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolation1;
    }
    firstScalarValues = reinterpret_cast<float *>(firstScalarInterpolation);
    fVar9 = movementBoundsSizeX * 0.25f;
    if (movementBoundsCenterX - fVar9 <= worldMotion.position.x) {
      if (worldMotion.position.x <= fVar9 + movementBoundsCenterX) {
        if (worldMotion.position.x < g_Player->drawPosition.x) {
          fVar19 = EnemyRandomAngle();
          fVar19 = fVar19 * 1.5707964f;
          goto dispatch_random_angle_ready;
        }
        fVar19 = EnemyRandomAngle();
        fVar19 = fVar19 * 3.1415927f * 0.5f;
      }
      else {
        fVar19 = EnemyRandomAngle();
        fVar19 = fVar19 * 3.1415927f * 0.33333334f;
      }
      fVar19 = EnemyWrapAngle((float)(fVar19 + 3.1415927f));
    }
    else {
      fVar19 = EnemyRandomAngle();
      fVar19 = fVar19 * 1.0471976f;
    }
dispatch_random_angle_ready:
    local_2bc = (float)fVar19;
    fVar9 = movementBoundsSizeY * 0.25f;
    if (movementBoundsCenterY - fVar9 <= worldMotion.position.y) {
      if (fVar9 + movementBoundsCenterY < worldMotion.position.y) {
        local_2bc = -EnemyAbsoluteFloat(local_2bc);
      }
    }
    else {
      local_2bc = EnemyAbsoluteFloat(local_2bc);
    }
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    firstScalarInterpolation->mode = uVar22;
    finalPolar.y = 0.0;
    finalPolar.x = local_2bc;
    initialPolar.x = local_2bc;
    fVar19 = (ReadFloatArgument(2));
    initialPolar.y = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent.x = g_EnemyInterpolationBasis.x;
dispatch_initialize_polar_interpolation:
    firstScalarInterpolation->finalTangent.y = g_EnemyInterpolationBasis.y;
    firstScalarInterpolation->initial = initialPolar;
    firstScalarInterpolation->final = finalPolar;
    EnemyInitializeScalarInterpolation(firstScalarInterpolation);
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_SET_LIFE_MARKER:
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    fVar19 = (ReadFloatArgument(1));
    local_2a8 = (float)fVar19;
    iVar26 = lifeGaugeReference;
    uVar23 = ENEMY_READ_INT_DIRECT(0);
    EnemyNativeWordIndex(&g_GuiView->bossGaugeValues[0], static_cast<int>(uVar23))->color = (unsigned int)uVar22;
    EnemyNativeWordIndex(&g_GuiView->bossGaugeValues[0], static_cast<int>(uVar23))->value = local_2a8 / (float)iVar26;
    return 0;
  case ENEMY_ECL_SET_DAMAGE_HITBOX:
    fVar19 = (ReadFloatArgument(0));
    damageHitbox.x = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    damageHitbox.y = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_PLAYER_HITBOX:
    fVar19 = (ReadFloatArgument(0));
    playerCollisionHitbox.x = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    playerCollisionHitbox.y = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_FLAGS:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar8 = flags | (unsigned int)uVar22;
    flags = uVar8;
    if ((uVar8 & 0x10) != 0) {
      for (iVar26 = 0; iVar26 < 10; ++iVar26)
        reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->ClearFlag2();
      return 0;
    }
    break;
  case ENEMY_ECL_CLEAR_FLAGS:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar8 = flags & ~(unsigned int)uVar22;
    flags = uVar8;
    if ((uVar8 & 0x10) == 0) {
      for (iVar26 = 0; iVar26 < 10; ++iVar26)
        reinterpret_cast<AnmVmIdView *>(EnemyNativeWordIndex(&managedVmIds[0], iVar26))->SetFlag2();
      return 0;
    }
    break;
  case ENEMY_ECL_CLEAR_ITEM_DROPS:
    memset(itemDrops.countsByKind, 0,
        sizeof(itemDrops.countsByKind) + sizeof(itemDrops.unknown030));
    return 0;
  case ENEMY_ECL_ADD_ITEM_DROP:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if ((char)((unsigned int)flags >> 8) < '\0') {
      if (iVar26 == 1) {
        iVar26 = 10;
      }
      else if (iVar26 == 4) {
        iVar26 = 0xb;
      }
    }
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    *EnemyNativeWordIndex(&itemDropRawWords[0], iVar26) = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_DROP_AREA:
    fVar19 = (ReadFloatArgument(1));
    fVar21 = (ReadFloatArgument(0));
    itemDrops.radiusX = (float)fVar21;
    itemDrops.radiusY = (float)fVar19;
    return 0;
  case ENEMY_ECL_DROP_ITEMS:
    EnemyDropItemCounts(&worldMotion.position, &itemDrops.mainItemType);
    return 0;
  case ENEMY_ECL_SET_MAIN_DROP:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if ((char)((unsigned int)flags >> 8) < '\0') {
      if (iVar26 == 1) {
        itemDrops.mainItemType = 10;
        return 0;
      }
      if (iVar26 == 4) {
        iVar26 = 0xb;
      }
    }
    itemDrops.mainItemType = iVar26;
    return 0;
  case ENEMY_ECL_SET_LIFE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    life = (int)uVar22;
    lifeGaugeReference = (int)uVar22;
    g_GuiView->bossGaugeValues[0].value = 0.0f;
    g_GuiView->bossGaugeValues[0].color = 0;
    g_GuiView->bossGaugeValues[1].value = 0.0f;
    g_GuiView->bossGaugeValues[1].color = 0;
    g_GuiView->bossGaugeValues[2].value = 0.0f;
    g_GuiView->bossGaugeValues[2].color = 0;
    g_GuiView->bossGaugeValues[3].value = 0.0f;
    g_GuiView->bossGaugeValues[3].color = 0;
    lifeToNextCallback = static_cast<unsigned int>(life);
    return 0;
  case ENEMY_ECL_SET_BOSS_SLOT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar27 = (int)uVar22;
    if (iVar27 < 0) {
      if ((char)(flags >> 8) < 0) {
        *EnemyNativeWordIndex(&g_EnemyManager->specialEnemySlots[0], managerSlot) = 0;
      }
      flags &= 0xffff7fff;
      return 0;
    }
    uVar4 = flags;
    iVar26 = reinterpret_cast<int>(g_EnemyManager);
    uVar4 |= 0x8000;
    flags = uVar4;
    *EnemyNativeWordIndex(&reinterpret_cast<EnemyManagerView *>(iVar26)->specialEnemySlots[0], iVar27) = owner;
    managerSlot = iVar27;
    return 0;
  case ENEMY_ECL_SET_INVULNERABILITY:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    damageReductionTimer.SetCurrent((int)uVar22);
    return 0;
  case ENEMY_ECL_SET_PLAYER_COLLISION_TIMER:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    playerCollisionTimer.SetCurrent((int)uVar22);
    return 0;
  case ENEMY_ECL_KILL_ALL_ENEMIES:
    EnemyKillAll(g_EnemyManager);
    return 0;
  case ENEMY_ECL_SELECT_INT_BY_DIFFICULTY:
    switch(g_EnemyDifficulty) {
    case 0:
      uVar22 = ENEMY_READ_INT_DIRECT(1);
      break;
    case 1:
      uVar22 = ENEMY_READ_INT_DIRECT(2);
      break;
    case 2:
      uVar22 = ENEMY_READ_INT_DIRECT(3);
      break;
    case 3:
    case 4:
      uVar22 = ENEMY_READ_INT_DIRECT(4);
      break;
    default:
      goto dispatch_complete;
    }
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    *puVar5 = (int)uVar22;
    return 0;
  case ENEMY_ECL_SELECT_FLOAT_BY_DIFFICULTY:
    switch(g_EnemyDifficulty) {
    case 0:
      fVar19 = ReadFloatArgument(1);
      goto dispatch_store_float_result;
    case 1:
      fVar21 = ReadFloatArgument(2);
      goto dispatch_store_float_result_a;
    case 2:
dispatch_difficulty_float_index_3:
      fVar19 = ReadFloatArgument(3);
      goto dispatch_store_float_result;
    case 3:
dispatch_difficulty_float_index_4_a:
      fVar21 = ReadFloatArgument(4);
      goto dispatch_store_float_result_a;
    case 4:
      fVar19 = ReadFloatArgument(4);
      goto dispatch_store_float_result;
    default:
      goto dispatch_complete;
    }
  // Original i386 IMUL/ADD compute the low 32-bit pattern address even
  // when script-provided indices overflow a signed C++ expression.
  // The indexed 0x210-byte pattern cursor is also a native 32-bit word.
  case ENEMY_ECL_INITIALIZE_BULLET_PATTERN:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    iVar11 = static_cast<int>(
        static_cast<unsigned int>(iVar26) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    puVar5 = EnemyPatternAtRuntimeByte(iVar11)->words;
    for (iVar27 = 0x84; iVar27 != 0; iVar27 = iVar27 + -1) {
      *puVar5 = 0;
      // The original STOSD-style loop advances a native 32-bit address
      // word, including when an unchecked ECL pattern index wraps.
      puVar5 = reinterpret_cast<unsigned int *>(
          reinterpret_cast<unsigned int>(puVar5) + sizeof(unsigned int));
    }
    EnemyPatternAtRuntimeIndex(runtimeAddress, iVar26)->fields.countA = 1;
    EnemyPatternAtRuntimeIndex(runtimeAddress, iVar26)->fields.countB = 1;
    EnemyPatternAtRuntimeIndex(runtimeAddress, iVar26)->fields.angleABits = 0;
    EnemyPatternAtRuntimeIndex(runtimeAddress, iVar26)->fields.speedABits = 0x40000000u;
    EnemyPatternAtRuntimeIndex(runtimeAddress, iVar26)->fields.soundId = 7;
    EnemyPatternAtRuntimeIndex(runtimeAddress, iVar26)->fields.soundVariant = 0x18;
    EnemyPatternAtRuntimeIndex(runtimeAddress, iVar26)->fields.flags1FC = 0x203;
    reinterpret_cast<EnemyPositionDwordView *>(EnemyPatternOffsetAtRuntimeIndex(runtimeAddress, iVar26))->word[0] = 0;
    reinterpret_cast<EnemyPositionDwordView *>(EnemyPatternOffsetAtRuntimeIndex(runtimeAddress, iVar26))->word[1] = 0;
    return 0;
  case ENEMY_ECL_COPY_BULLET_PATTERN:
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    uVar23 = ENEMY_READ_INT_DIRECT(0);
    *EnemyPatternAtRuntimeIndex(runtimeAddress, static_cast<int>(uVar23)) =
        *EnemyPatternAtRuntimeIndex(runtimeAddress, static_cast<int>(uVar22));
    return 0;
  case ENEMY_ECL_FIRE_BULLET_PATTERN:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    // The original IMUL/LEA form treats both the three-word offset index
    // and its final byte-address as modulo-2^32 words. A negative or very
    // large ECL index must not overflow a signed C++ expression.
    iVar27 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(PlayerFloat3) / sizeof(unsigned int)) +
        static_cast<unsigned int>(offsetof(EnemyRuntimeView, bulletPatternOffsets) /
                                  sizeof(unsigned int)));
    vectorScratch.x = reinterpret_cast<const PlayerFloat3 *>(
        static_cast<unsigned int>(runtimeAddress) +
        static_cast<unsigned int>(iVar27) * sizeof(unsigned int))->x + worldMotion.position.x;
    vectorScratch.y = reinterpret_cast<const PlayerFloat3 *>(
        static_cast<unsigned int>(runtimeAddress) +
        static_cast<unsigned int>(iVar27) * sizeof(unsigned int))->y + worldMotion.position.y;
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    vectorScratch.z = reinterpret_cast<const PlayerFloat3 *>(
        static_cast<unsigned int>(runtimeAddress) +
        static_cast<unsigned int>(iVar27) * sizeof(unsigned int))->z + worldMotion.position.z;
    EnemyPatternAtRuntimeByte(iVar26)->fields.spawnPositionX = vectorScratch.x;
    EnemyPatternAtRuntimeByte(iVar26)->fields.spawnPositionY = vectorScratch.y;
    fVar9 = 0.0f;
    EnemyPatternAtRuntimeByte(iVar26)->fields.spawnPositionZ = vectorScratch.z;
    if ((bulletProtectionRadiusSquared == fVar9) ||
       (fVar9 = EnemyPatternAtRuntimeByte(iVar26)->fields.spawnPositionY - g_Player->drawPosition.y,
       fVar10 = EnemyPatternAtRuntimeByte(iVar26)->fields.spawnPositionX - g_Player->drawPosition.x,
       fVar9 = fVar9 * fVar9 + fVar10 * fVar10,
       bulletProtectionRadiusSquared <= fVar9)) {
      reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->
          SpawnBulletPattern(reinterpret_cast<BulletSpawnDescriptorView *>(
              EnemyPatternAtRuntimeByte(iVar26)));
      return 0;
    }
    break;
  case ENEMY_ECL_SET_BULLET_SPRITES:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    EnemyPatternAtRuntimeByte(iVar26)->fields.spriteId = static_cast<unsigned short>(uVar22);
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    EnemyPatternAtRuntimeByte(iVar26)->fields.spriteVariant = static_cast<unsigned short>(uVar22);
    return 0;
  case ENEMY_ECL_SET_BULLET_OFFSET:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    fVar19 = (ReadFloatArgument(1));
    EnemyPatternOffsetAtRuntimeIndex(runtimeAddress, static_cast<int>(uVar22))->x = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    EnemyPatternOffsetAtRuntimeIndex(runtimeAddress, static_cast<int>(uVar22))->y = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_BULLET_ANGLE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    fVar19 = (ReadFloatArgument(1));
    EnemyPatternAtRuntimeByte(iVar26)->fields.angleA = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    EnemyPatternAtRuntimeByte(iVar26)->fields.angleB = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_BULLET_SPEED:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    goto dispatch_bullet_speed_lowest;
  case ENEMY_ECL_SET_BULLET_COUNT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    EnemyPatternAtRuntimeByte(iVar26)->fields.countA = (short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    EnemyPatternAtRuntimeByte(iVar26)->fields.countB = (short)uVar22;
    return 0;
  // The target carries the scaled pattern cursor in one i386 dword;
  // publishing each speed member also adds the runtime base modulo 2^32.
  case ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY:
  {
    int speedIndex;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    if (g_EnemyDifficulty == 0) {
      speedIndex = 1;
    }
    else if (g_EnemyDifficulty == 1) {
      speedIndex = 2;
    }
    else {
      speedIndex = 3;
      if (g_EnemyDifficulty != 2) {
        speedIndex = 4;
      }
    }
    fVar19 = ReadFloatArgument(speedIndex);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)));
    EnemyPatternAtRuntimeByte(static_cast<int>(
        static_cast<unsigned int>(iVar26) +
        static_cast<unsigned int>(runtimeAddress)))->fields.speedA = (float)fVar19;
    if (g_EnemyDifficulty == 0) {
      speedIndex = 5;
    }
    else if (g_EnemyDifficulty == 1) {
      speedIndex = 6;
    }
    else {
      speedIndex = 7;
      if (g_EnemyDifficulty != 2) {
        speedIndex = 8;
      }
    }
    fVar19 = ReadFloatArgument(speedIndex);
    EnemyPatternAtRuntimeByte(static_cast<int>(
        static_cast<unsigned int>(iVar26) +
        static_cast<unsigned int>(runtimeAddress)))->fields.speedB = (float)fVar19;
    return 0;
  }
  case ENEMY_ECL_SET_BULLET_COUNT_BY_DIFFICULTY:
  {
    int countIndex;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    if (g_EnemyDifficulty == 0) {
      countIndex = 1;
    }
    else if (g_EnemyDifficulty == 1) {
      countIndex = 2;
    }
    else {
      countIndex = 3;
      if (g_EnemyDifficulty != 2) {
        countIndex = 4;
      }
    }
    uVar23 = ENEMY_READ_INT_DIRECT(countIndex);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    EnemyPatternAtRuntimeByte(iVar26)->fields.countA = (short)uVar23;
    if (g_EnemyDifficulty == 0) {
      countIndex = 5;
    }
    else if (g_EnemyDifficulty == 1) {
      countIndex = 6;
    }
    else {
      countIndex = 7;
      if (g_EnemyDifficulty != 2) {
        countIndex = 8;
      }
    }
    uVar22 = ENEMY_READ_INT_DIRECT(countIndex);
    EnemyPatternAtRuntimeByte(iVar26)->fields.countB = (short)uVar22;
    return 0;
  }
  case ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_3:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 0x200) {
dispatch_bullet_speed_high:
      iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
      fVar19 = (ReadFloatArgument(5));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedA = (float)fVar19;
      fVar19 = (ReadFloatArgument(6));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedB = (float)fVar19;
      return 0;
    }
    iVar27 = static_cast<int>(
        static_cast<unsigned int>(g_EnemyChapterState.rank) + 0x200u);
    goto dispatch_select_bullet_speed_low;
  case ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_5:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 600) {
      iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
      fVar19 = (ReadFloatArgument(9));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedA = (float)fVar19;
      fVar19 = (ReadFloatArgument(10));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedB = (float)fVar19;
      return 0;
    }
    if (g_EnemyChapterState.rank >= 200) {
      iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
      fVar19 = (ReadFloatArgument(7));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedA = (float)fVar19;
      fVar19 = (ReadFloatArgument(8));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedB = (float)fVar19;
      return 0;
    }
    if (g_EnemyChapterState.rank >= -200) goto dispatch_bullet_speed_high;
    iVar27 = static_cast<int>(
        static_cast<unsigned int>(g_EnemyChapterState.rank) + 600u);
dispatch_select_bullet_speed_low:
    iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
    if (0 <= iVar27) {
      fVar19 = (ReadFloatArgument(3));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedA = (float)fVar19;
      fVar19 = (ReadFloatArgument(4));
      EnemyPatternAtRuntimeByte(iVar26)->fields.speedB = (float)fVar19;
      return 0;
    }
dispatch_bullet_speed_lowest:
    fVar19 = (ReadFloatArgument(1));
    EnemyPatternAtRuntimeByte(iVar26)->fields.speedA = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    EnemyPatternAtRuntimeByte(iVar26)->fields.speedB = (float)fVar19;
    return 0;
  case ENEMY_ECL_INTERPOLATE_BULLET_SPEED_BY_RANK:
  {
    int patternIndex = ENEMY_READ_INT_DIRECT(0);
    float lowSpeedA = ReadFloatArgument(1);
    float lowSpeedB = ReadFloatArgument(2);
    float highSpeedA = ReadFloatArgument(3);
    float highSpeedB = ReadFloatArgument(4);
    EnemyPatternAtRuntimeIndex(runtimeAddress, patternIndex)->fields.speedA =
         (highSpeedA - lowSpeedA) * ((float)g_EnemyChapterState.rank + 1024.0f) *
         0.00048828125f + lowSpeedA;
    EnemyPatternAtRuntimeIndex(runtimeAddress, patternIndex)->fields.speedB =
         (highSpeedB - lowSpeedB) * ((float)g_EnemyChapterState.rank + 1024.0f) *
         0.00048828125f + lowSpeedB;
    return 0;
  }
  // Each pattern selection below uses the native 32-bit IMUL/ADD address
  // word; an ECL-provided index can overflow a signed C++ multiplication.
  // Retain the original byte-address cursor and its evaluation sites.
  case ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_3:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 0x200) {
dispatch_bullet_count_high:
      iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
      uVar22 = ENEMY_READ_INT_DIRECT(5);
      EnemyPatternAtRuntimeByte(iVar26)->fields.countA = (short)uVar22;
      uVar22 = ENEMY_READ_INT_DIRECT(6);
      EnemyPatternAtRuntimeByte(iVar26)->fields.countB = (short)uVar22;
      return 0;
    }
    iVar27 = static_cast<int>(
        static_cast<unsigned int>(g_EnemyChapterState.rank) + 0x200u);
    goto dispatch_select_bullet_count_low;
  case ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_5:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 600) {
      iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
      uVar22 = ENEMY_READ_INT_DIRECT(9);
      EnemyPatternAtRuntimeByte(iVar26)->fields.countA = (short)uVar22;
      {
        EclVmHost *const countHost = reinterpret_cast<EclVmHost *>(owner);
        uVar22 = countHost->activeContext->ReadInt(10);
      }
      EnemyPatternAtRuntimeByte(iVar26)->fields.countB = (short)uVar22;
      return 0;
    }
    if (g_EnemyChapterState.rank >= 200) {
      iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
      uVar22 = ENEMY_READ_INT_DIRECT(7);
      EnemyPatternAtRuntimeByte(iVar26)->fields.countA = (short)uVar22;
      {
        EclVmHost *const countHost = reinterpret_cast<EclVmHost *>(owner);
        uVar22 = countHost->activeContext->ReadInt(8);
      }
      EnemyPatternAtRuntimeByte(iVar26)->fields.countB = (short)uVar22;
      return 0;
    }
    if (g_EnemyChapterState.rank >= -200) goto dispatch_bullet_count_high;
    iVar27 = static_cast<int>(
        static_cast<unsigned int>(g_EnemyChapterState.rank) + 600u);
dispatch_select_bullet_count_low:
    iVar11 = reinterpret_cast<int>(owner);
    iVar26 = static_cast<int>(
          static_cast<unsigned int>(iVar26) *
          static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
          static_cast<unsigned int>(runtimeAddress));
    if (0 <= iVar27) {
      uVar22 = ENEMY_READ_INT_DIRECT(3);
      EnemyPatternAtRuntimeByte(iVar26)->fields.countA = (short)uVar22;
      uVar22 = ENEMY_READ_INT_DIRECT(4);
      EnemyPatternAtRuntimeByte(iVar26)->fields.countB = (short)uVar22;
      return 0;
    }
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    EnemyPatternAtRuntimeByte(iVar26)->fields.countA = (short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    EnemyPatternAtRuntimeByte(iVar26)->fields.countB = (short)uVar22;
    return 0;
  case ENEMY_ECL_INTERPOLATE_BULLET_COUNT_BY_RANK:
    // Native SUB, ADD and IMUL keep the low 32-bit word even on overflow.
    // The subsequent signed division still rounds toward zero.
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    secondLowBulletCount = ENEMY_READ_INT_DIRECT(2);
    uVar24 = ENEMY_READ_INT_DIRECT(3);
    uVar25 = ENEMY_READ_INT_DIRECT(4);
    iVar27 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)));
    iVar26 = static_cast<int>((static_cast<unsigned int>(uVar24) -
        static_cast<unsigned int>(uVar23)) *
        (static_cast<unsigned int>(g_EnemyChapterState.rank) + 0x400u));
    EnemyPatternAtRuntimeByte(static_cast<int>(
        static_cast<unsigned int>(iVar27) +
        static_cast<unsigned int>(runtimeAddress)))->fields.countA =
         (short)(iVar26 / 2048) + (short)uVar23;
    iVar26 = static_cast<int>((static_cast<unsigned int>(uVar25) -
        static_cast<unsigned int>(secondLowBulletCount)) *
        (static_cast<unsigned int>(g_EnemyChapterState.rank) + 0x400u));
    EnemyPatternAtRuntimeByte(static_cast<int>(
        static_cast<unsigned int>(iVar27) +
        static_cast<unsigned int>(runtimeAddress)))->fields.countB =
         (short)(iVar26 / 2048) +
         (short)secondLowBulletCount;
    return 0;
  case ENEMY_ECL_SET_BULLET_AIM_MODE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    EnemyPatternAtRuntimeIndex(runtimeAddress, static_cast<int>(uVar22))->fields.aimMode = static_cast<unsigned short>(uVar23);
    return 0;
  case ENEMY_ECL_SET_BULLET_SOUND:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)) +
        static_cast<unsigned int>(runtimeAddress));
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    EnemyPatternAtRuntimeByte(iVar26)->fields.soundId = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    EnemyPatternAtRuntimeByte(iVar26)->fields.soundVariant = (int)uVar22;
    return 0;
  // Opcode 0x199 uses 22 overlapping 0x18-byte records per pattern.
  // Keep the two index words and final eight-byte-scaled float store as
  // modulo-32-bit arithmetic, even when script inputs exceed signed int.
  case ENEMY_ECL_SET_BULLET_EXTRA:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    iVar26 = static_cast<int>(
        static_cast<unsigned int>(uVar22) *
        static_cast<unsigned int>(
            sizeof(EnemyBulletPatternView) / sizeof(EnemyBulletExtraView)) +
        static_cast<unsigned int>(uVar23));
    iVar27 = static_cast<int>(
        static_cast<unsigned int>(runtimeAddress) +
        static_cast<unsigned int>(iVar26) *
        static_cast<unsigned int>(sizeof(EnemyBulletExtraView)));
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    EnemyExtraRecordAt(iVar27)->value14 = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(3);
    EnemyExtraRecordAt(iVar27)->value10 = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(4);
    EnemyExtraRecordAt(iVar27)->value08 = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(5);
    EnemyExtraRecordAt(iVar27)->value0C = (int)uVar22;
    fVar19 = (ReadFloatArgument(6));
    EnemyExtraRecordAt(iVar27)->floatA = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    // Keep the native index scaled in eight-byte units. This is the +4
    // floatB member of the same extra slot, not a separate float array.
    *reinterpret_cast<float *>(static_cast<unsigned int>(runtimeAddress) +
        (static_cast<unsigned int>(iVar26) * 3u +
         static_cast<unsigned int>(
             (offsetof(EnemyRuntimeView, bulletPatterns) +
              offsetof(EnemyBulletPatternFieldsView, unknown020) +
              offsetof(EnemyBulletExtraView, floatB)) / 8)) * 8u) = (float)fVar19;
    return 0;
  case ENEMY_ECL_RESET_UPDATE_TIMER:
    updateTimer.SetCurrent(0);
    return 0;
  case ENEMY_ECL_SET_INTERRUPT:
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    uVar24 = ENEMY_READ_INT_DIRECT(0);
    EnemyConfigureInterrupt(
        owner, (int)uVar24, (int)uVar23, (int)uVar22,
        instruction->interruptOperands.callbackName);
    return 0;
  case ENEMY_ECL_SET_TIMEOUT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    EnemyNativeWordIndex(&owner->runtime.callbackThresholds[0], static_cast<int>(uVar22))->timerCallbackName =
        instruction->timeoutOperands.callbackName;
    return 0;
  case ENEMY_ECL_CANCEL_ALL_BULLETS:
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelAllBullets(1);
    EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager), 1);
    return 0;
  case ENEMY_ECL_PLAY_SOUND:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
        QueueSoundCue((int)uVar22, worldMotion.position.x);
    return 0;
  case ENEMY_ECL_SET_SCREEN_SHAKE:
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    uVar6 = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    uVar4 = (unsigned int)uVar22;
    uVar28 = 0;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    EnemySetScreenShake(1,(int)uVar22,uVar4,uVar6,uVar28);
    return 0;
  case ENEMY_ECL_READ_DIALOG:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    EnemyReadDialog(g_GuiView, (int)uVar22);
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelAllBullets(0);
    EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager), 0);
    EnemyKillAll(g_EnemyManager);
    return 0;
  case ENEMY_ECL_WAIT_DIALOG:
    if (g_GuiView->message != 0 &&
       g_GuiView->message->inputCooldown == 0) {
      return 0xffffffff;
    }
    break;
  case ENEMY_ECL_WAIT_BOSS_CLEAR:
    if (reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]) != 0) {
      return 0xffffffff;
    }
    break;
  case ENEMY_ECL_START_SPELL:
  case ENEMY_ECL_START_SPELL_2:
  case ENEMY_ECL_START_SPELL_DIFFICULTY:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2:
  {
    const EnemyEclSpellInlineOperandsView &spellOperands =
        instruction->spellOperands;
    const unsigned char *encryptedName = spellOperands.encryptedName;
    const int nameLength = spellOperands.encryptedNameLength;
    iVar17 = 0;
    bVar3 = 0x77;
    local_29d = '\a';
    if (0 < nameLength) {
      do {
        *EnemyNativeWordIndex(&local_90.bytes[0], iVar17) =
            *EnemyNativeWordIndex(encryptedName, iVar17) ^ bVar3;
        bVar3 = bVar3 + local_29d;
        local_29d = local_29d + '\x10';
        iVar17 = iVar17 + 1;
      } while (iVar17 < nameLength);
    }
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    // The target uses ADD/LEA on native 32-bit words for difficulty-biased
    // spell IDs; the signed input must not introduce C++ overflow UB.
    switch (instruction->opcode) {
    case ENEMY_ECL_START_SPELL_DIFFICULTY:
      iVar26 = static_cast<int>(static_cast<unsigned int>(iVar26) +
          static_cast<unsigned int>(g_EnemyDifficulty));
      break;
    case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1:
      iVar26 = static_cast<int>(static_cast<unsigned int>(iVar26) +
          static_cast<unsigned int>(g_EnemyDifficulty) - 1u);
      break;
    case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2:
      iVar26 = static_cast<int>(static_cast<unsigned int>(iVar26) +
          static_cast<unsigned int>(g_EnemyDifficulty) - 2u);
      break;
    }
    ENEMY_READ_INT_DIRECT(2);
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    g_EnemyGameState->StartSpell(iVar26, reinterpret_cast<const char *>(local_90.bytes), (int)uVar22);
    return 0;
  }
  case ENEMY_ECL_END_SPELL:
    EnemyEndSpell(g_EnemyGameState);
    return 0;
  case ENEMY_ECL_CANCEL_LASERS:
    // The target changes bit 3 in the same game-state flag word already
    // shared by spell start/end and the bomb-shield helper.
    g_EnemyGameState->flags378C |= 8;
    return 0;
  case ENEMY_ECL_ENABLE_BOMB_SHIELD:
    EnemyEnableBombShield(g_EnemyGameState);
    return 0;
  case ENEMY_ECL_SET_GAME_SPEED_FLAG:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    flags =
         flags ^
         ((static_cast<unsigned int>(uVar22) << 0x13 ^ flags) & 0x80000u);
    return 0;
  case ENEMY_ECL_FIRE_LASER_A:
  case ENEMY_ECL_FIRE_LASER_A_WITH_PATTERN:
    pfVar15 = reinterpret_cast<float *>(&local_288);
    for (iVar27 = 0x77; iVar27 != 0; iVar27 = iVar27 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    if (opcode == 0x1af) {
      memcpy(local_288.type0.patternWords,
             &bulletPatterns[0].words[8],
             sizeof(local_288.type0.patternWords));
    }
    vectorScratch.x = bulletPatternOffsets[0].x + worldMotion.position.x;
    vectorScratch.y = bulletPatternOffsets[0].y + worldMotion.position.y;
    vectorScratch.z = bulletPatternOffsets[0].z + worldMotion.position.z;
    *reinterpret_cast<PlayerFloat3 *>(&local_288) = vectorScratch;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    local_288.type0.type = (unsigned short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    local_288.type0.color = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(2));
    local_288.type0.angle = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_288.type0.speed = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.type0.initialLength = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.type0.maximumLength = (float)fVar19;
    fVar19 = (ReadFloatArgument(6));
    local_288.type0.terminalDistance = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    local_288.type0.width = (float)fVar19;
    local_288.type0.flags |= 1;
    EnemyFireLaser(g_EnemyLaserManager, &local_288, 0);
    return 0;
  case ENEMY_ECL_FIRE_STRAIGHT_LASER:
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_WITH_PATTERN:
    pfVar15 = reinterpret_cast<float *>(&local_288);
    for (iVar26 = 0x7e; iVar26 != 0; iVar26 = iVar26 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    local_288.type1.fixed2C = 8.0f;
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    if (instruction->opcode == 0x1b0) {
      memcpy(local_288.type1.patternWords,
             &bulletPatterns[0].words[8],
             sizeof(local_288.type1.patternWords));
    }
    vectorScratch.x = bulletPatternOffsets[0].x + worldMotion.position.x;
    vectorScratch.y = bulletPatternOffsets[0].y + worldMotion.position.y;
    vectorScratch.z = bulletPatternOffsets[0].z + worldMotion.position.z;
    *reinterpret_cast<PlayerFloat3 *>(&local_288) = vectorScratch;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    local_288.type1.type = (unsigned short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    local_288.type1.color = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(3));
    local_288.type1.angle = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.type1.initialLength = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.type1.maximumLength = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(6);
    local_288.type1.waitFrames = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(7);
    local_288.type1.growFrames = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(8);
    local_288.type1.holdFrames = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(9);
    local_288.type1.shrinkFrames = (unsigned int)uVar22;
    fVar19 = (ReadFloatArgument(10));
    local_288.type1.width = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(0xb);
    local_288.type1.flags44 = (unsigned int)uVar22 | 2;
    uVar6 = EnemyFireLaser(g_EnemyLaserManager, &local_288, 1);
    if (puVar5 != (unsigned int *)0x0) {
      *puVar5 = uVar6;
      return 0;
    }
    break;
  case ENEMY_ECL_FIRE_LASER:
  case ENEMY_ECL_FIRE_LASER_WITH_PATTERN:
    pfVar15 = reinterpret_cast<float *>(&local_288);
    for (iVar26 = 0x77; iVar26 != 0; iVar26 = iVar26 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    if (opcode == 0x1b1) {
      memcpy(
          local_288.type0.patternWords,
          reinterpret_cast<const void *>(&bulletPatterns[0].words[8]),
          sizeof(local_288.type0.patternWords));
    }
    vectorScratch.x = bulletPatternOffsets[0].x + worldMotion.position.x;
    vectorScratch.y = bulletPatternOffsets[0].y + worldMotion.position.y;
    vectorScratch.z = bulletPatternOffsets[0].z + worldMotion.position.z;
    local_288.type0.position.x = vectorScratch.x;
    local_288.type0.position.y = vectorScratch.y;
    local_288.type0.position.z = vectorScratch.z;
    uVar22 = ReadIntArgument(0);
    local_288.type0.type = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(1);
    local_288.type0.color = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(2));
    local_288.type0.angle = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_288.type0.speed = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.type0.initialLength = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.type0.maximumLength = (float)fVar19;
    fVar19 = (ReadFloatArgument(6));
    local_288.type0.terminalDistance = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    local_288.type0.width = (float)fVar19;
    local_288.type0.flags &= 0xfffffffe;
    EnemyFireLaser(g_EnemyLaserManager, &local_288, 0);
    return 0;
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_2:
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_2_WITH_PATTERN:
    EnemyInitializeLaserConfig(&local_288);
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    if (instruction->opcode == 0x1b2) {
      puVar14 = &bulletPatterns[0].words[8];
      puVar13 = local_288.type1.patternWords;
      for (iVar26 = 0x6c; iVar26 != 0; iVar26 = iVar26 + -1) {
        *puVar13 = *puVar14;
        puVar14 = puVar14 + 1;
        puVar13 = puVar13 + 1;
      }
    }
    vectorScratch.x = bulletPatternOffsets[0].x + worldMotion.position.x;
    vectorScratch.y = bulletPatternOffsets[0].y + worldMotion.position.y;
    vectorScratch.z = bulletPatternOffsets[0].z + worldMotion.position.z;
    local_288.type1.position.x = vectorScratch.x;
    local_288.type1.position.y = vectorScratch.y;
    local_288.type1.position.z = vectorScratch.z;
    uVar22 = ReadIntArgument(1);
    local_288.type1.type = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(2);
    local_288.type1.color = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(3));
    local_288.type1.angle = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.type1.initialLength = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.type1.maximumLength = (float)fVar19;
    uVar22 = ReadIntArgument(6);
    local_288.type1.waitFrames = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(7);
    local_288.type1.growFrames = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(8);
    local_288.type1.holdFrames = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(9);
    local_288.type1.shrinkFrames = (unsigned int)uVar22;
    fVar19 = (ReadFloatArgument(10));
    local_288.type1.width = (float)fVar19;
    uVar22 = ReadIntArgument(0xb);
    local_288.type1.flags44 = (unsigned int)uVar22 & 0xfffffffd;
    uVar6 = EnemyFireLaser(g_EnemyLaserManager, &local_288, 1);
    if (puVar5 != (unsigned int *)0x0) {
      *puVar5 = uVar6;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_OFFSET:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(2));
      fVar21 = (ReadFloatArgument(1));
      vectorScratch.x = (float)fVar21;
      vectorScratch.y = (float)fVar19;
      vectorScratch.z = 0.0f;
      reinterpret_cast<EnemyLaserConstructionBaseView *>(iVar26)->eclPosition =
          *reinterpret_cast<const EnemyLaserVectorView *>(&vectorScratch);
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_ANGLE:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(2));
      fVar21 = (ReadFloatArgument(1));
      vectorScratch.x = (float)fVar21;
      vectorScratch.z = 0.0;
      vectorScratch.y = (float)fVar19;
      EnemySetLaserPosition(
          iVar26, reinterpret_cast<const PlayerFloat3 *>(&vectorScratch.x));
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_SPEED:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      reinterpret_cast<EnemyLaserConstructionBaseView *>(iVar26)->eclValue48 = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_COUNT:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      reinterpret_cast<EnemyLaserConstructionBaseView *>(iVar26)->eclValue44 = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_AIM:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      reinterpret_cast<EnemyLaserConstructionBaseView *>(iVar26)->eclAngle = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_SOUND:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      reinterpret_cast<EnemyLaserControlFloatAt440View *>(iVar26)->value440 =
          (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_CANCEL_BULLET_PATTERN:
    fVar19 = (ReadFloatArgument(0));
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelBulletPattern(
            &worldMotion.position, (float)fVar19, 1, 0);
    reinterpret_cast<EnemyEffectWaitManager *>(
        g_EnemyLaserManager)->ApplyPatternCancel(
            &worldMotion.position, (float)fVar19, 1);
    return 0;
  case ENEMY_ECL_CLEAR_BULLET_PATTERN:
    fVar19 = (ReadFloatArgument(0));
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelBulletPattern(
            &worldMotion.position, (float)fVar19, 0, 0);
    reinterpret_cast<EnemyEffectWaitManager *>(
        g_EnemyLaserManager)->ApplyPatternCancel(
            &worldMotion.position, (float)fVar19, 0);
    return 0;
  case ENEMY_ECL_SET_CHAPTER:
    uVar22 = ReadIntArgument(0);
    EnemySetChapter(&g_EnemyChapterState, (int)uVar22);
    return 0;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_3:
    if (g_EnemyChapterState.rank >= 0x200) {
      iVar27 = 2;
      goto dispatch_rank_float_tail_b;
    }
    iVar27 = 0;
    if (g_EnemyChapterState.rank > -0x200) goto dispatch_rank_float_tail_b;
    goto dispatch_rank_float_tail_a;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_5:
    if (g_EnemyChapterState.rank >= 600) {
      goto dispatch_difficulty_float_index_4_a;
    }
    if (g_EnemyChapterState.rank >= 200) {
      goto dispatch_difficulty_float_index_3;
    }
    if (g_EnemyChapterState.rank >= -200) {
      iVar27 = 2;
      goto dispatch_rank_float_tail_a;
    }
    if (g_EnemyChapterState.rank < -400) {
      iVar27 = 0;
      goto dispatch_rank_float_tail_a;
    }
    fVar19 = ReadFloatArgument(1);
    iVar26 = reinterpret_cast<int>(owner);
    pfVar15 = reinterpret_cast<EclVmContext *>(
        *reinterpret_cast<int *>(static_cast<unsigned int>(iVar26) + 4u))->ResolveFloat(0);
    *pfVar15 = (float)fVar19;
    return 0;
dispatch_rank_float_tail_b:
    fVar19 = ReadFloatArgument(iVar27);
dispatch_store_float_result:
    // Keep this owner lifetime explicit: the shipped shared tail reuses EBX
    // as the full owner immediately before ResolveFloat.
    iVar26 = reinterpret_cast<int>(owner);
    pfVar15 = reinterpret_cast<EclVmContext *>(
        *reinterpret_cast<int *>(static_cast<unsigned int>(iVar26) + 4u))->ResolveFloat(0);
    *pfVar15 = (float)fVar19;
    return 0;
dispatch_rank_float_tail_a:
    fVar21 = ReadFloatArgument(iVar27);
dispatch_store_float_result_a:
    positionInterpolationValues = ResolveFloatArgument(0);
    *positionInterpolationValues = (float)fVar21;
    return 0;
  case ENEMY_ECL_INTERPOLATE_FLOAT_BY_RANK:
    fVar19 = (ReadFloatArgument(1));
    fVar21 = (ReadFloatArgument(2));
    local_2a8 = (float)fVar21;
    pfVar15 = (float *)((int)ResolveFloatArgument(0));
    *pfVar15 = (local_2a8 - (float)fVar19) * ((float)g_EnemyChapterState.rank + 1024.0f) * 0.00048828125f +
               (float)fVar19;
    return 0;
  case ENEMY_ECL_SELECT_INT_BY_RANK_3:
    if (0x1ff < g_EnemyChapterState.rank) {
dispatch_rank_int_high:
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(2);
      *puVar5 = (int)uVar22;
      return 0;
    }
    goto dispatch_rank_int_low;
  case ENEMY_ECL_SELECT_INT_BY_RANK_5:
    if (599 < g_EnemyChapterState.rank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(4);
      *puVar5 = (int)uVar22;
      return 0;
    }
    if (199 < g_EnemyChapterState.rank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(3);
      *puVar5 = (int)uVar22;
      return 0;
    }
    if (-0xc9 < g_EnemyChapterState.rank) goto dispatch_rank_int_high;
    if (-0x191 < g_EnemyChapterState.rank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(1);
      *puVar5 = (int)uVar22;
      return 0;
    }
dispatch_rank_int_low:
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    uVar22 = ReadIntArgument(0);
    *puVar5 = (int)uVar22;
    return 0;
  case ENEMY_ECL_INTERPOLATE_INT_BY_RANK:
    // Restore native word-wrap interpolation before signed division and
    // final 32-bit value publication, without widening the product.
    uVar22 = ReadIntArgument(1);
    uVar23 = ReadIntArgument(2);
    piVar7 = (int *)((int)ResolveIntArgument(0));
    iVar26 = static_cast<int>((static_cast<unsigned int>(uVar23) -
        static_cast<unsigned int>(uVar22)) *
        (static_cast<unsigned int>(g_EnemyChapterState.rank) + 0x400u));
    *piVar7 = static_cast<int>(static_cast<unsigned int>(iVar26 / 2048) +
        static_cast<unsigned int>(uVar22));
    return 0;
  case ENEMY_ECL_SET_SPELL_TIMEOUT:
    uVar22 = ReadIntArgument(0);
    g_GuiView->bossGaugeSegmentCount = (int)uVar22;
    return 0;
  case ENEMY_ECL_WAIT_FOR_EFFECT:
    EnemyWaitForEffect(
        reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager));
    return 0;
  case ENEMY_ECL_SET_ANM_MODE:
    uVar22 = ReadIntArgument(0);
    flags =
         flags ^
         ((static_cast<unsigned int>(uVar22) << 0x14 ^ flags) & 0x100000u);
    uVar22 = ReadIntArgument(1);
    animationScriptPrimary = (int)uVar22;
    flags = flags & 0xffdfffff;
    animationScriptAlternate = animationBaseScript;
    return 0;
  case ENEMY_ECL_SET_TIMER_SCALE:
    fVar19 = (ReadFloatArgument(0));
    g_AnmGameSpeed = (float)fVar19;
    return 0;
  case ENEMY_ECL_AIM_BULLET_AT_PLAYER:
    pfVar15 = &g_Player->drawPosition.x;
    fVar19 = (ReadFloatArgument(1));
    iVar26 = reinterpret_cast<int>(g_Player);
    fVar21 = (ReadFloatArgument(2));
    fVar19 = atan2(reinterpret_cast<Player *>(iVar26)->drawPosition.y - fVar21,
                   *pfVar15 - (float)fVar19);
    goto dispatch_store_float_result;
  case ENEMY_ECL_SUBTRACT_DIFFICULTY_VALUE:
    if (g_EnemyDifficulty == 0) {
      uVar22 = ReadIntArgument(0);
      fVar9 = (float)(int)uVar22;
    }
    else if (g_EnemyDifficulty == 1) {
      uVar22 = ReadIntArgument(1);
      fVar9 = (float)(int)uVar22;
    }
    else if (g_EnemyDifficulty == 2) {
      uVar22 = ReadIntArgument(2);
      fVar9 = (float)(int)uVar22;
    }
    else {
      uVar22 = ReadIntArgument(3);
      fVar9 = (float)(int)uVar22;
    }
    // The active ECL context's first word is the frame-time scalar.
    // Preserve the pointer's lifetime across the read/modify/write.
    pfVar15 = &owner->activeEclContext->currentTime;
    *pfVar15 = *pfVar15 - fVar9;
    break;
  }
dispatch_complete:
  return 0;
dispatch_create_enemy_absolute_mirrored:
  iVar26 = static_cast<int>(
        static_cast<unsigned int>(
            instruction
                ->spawnOperands.inlineNameByteCount) + 4u);
  iVar26 /= 4;
  memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
  fVar19 = (ReadRawFloatArgument((1), (EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26))->real)));
  local_90.spawnRequest.position.x = (float)fVar19;
  fVar19 = (ReadRawFloatArgument((2), (EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 1u)->real)));
  local_90.spawnRequest.position.y = (float)fVar19;
  uVar22 = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 2u)->integer);
  local_90.spawnRequest.life = uVar22;
  uVar22 = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 3u)->integer);
  local_90.spawnRequest.scoreReward = uVar22;
  uVar22 = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, static_cast<unsigned int>(iVar26) + 4u)->integer);
  local_90.spawnRequest.itemDropType = uVar22;
  local_90.spawnRequest.eclVariables = eclVariables;
  local_90.spawnRequest.setFlag0800 = 1;
  goto dispatch_spawn_enemy;
dispatch_copy_player_to_motion:
  iVar26 = reinterpret_cast<int>(g_EnemyManager->specialEnemySlots[0]);
  reinterpret_cast<EnemyPositionDwordView *>(&offsetMotion.position)->word[0] = reinterpret_cast<const EnemyPositionDwordView *>(&reinterpret_cast<EnemyFullObjectView *>(iVar26)->runtime.worldMotion.position)->word[0];
  reinterpret_cast<EnemyPositionDwordView *>(&offsetMotion.position)->word[1] = reinterpret_cast<const EnemyPositionDwordView *>(&reinterpret_cast<EnemyFullObjectView *>(iVar26)->runtime.worldMotion.position)->word[1];
  reinterpret_cast<EnemyPositionDwordView *>(&offsetMotion.position)->word[2] = reinterpret_cast<const EnemyPositionDwordView *>(&reinterpret_cast<EnemyFullObjectView *>(iVar26)->runtime.worldMotion.position)->word[2];
  return 0;
}
