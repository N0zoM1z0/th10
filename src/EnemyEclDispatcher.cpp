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


// ECL opcode 0x199 addresses these 0x18-byte slots from the first
// pattern's +0x20. There are 0x16 slots per 0x210-byte pattern stride.
// This is an *overlapping* view of pattern storage, not a second array:
// entries 20/21 alias the normal pattern tail and the next pattern's head.
typedef char EnemyBulletExtraPatternOverlayLayout[
    (offsetof(EnemyRuntimeView, bulletPatterns) +
         offsetof(EnemyBulletPatternFieldsView, transforms) == 0x2e4 &&
     sizeof(EnemyBulletPatternView) == 0x16 * sizeof(BulletTransformRecordView))
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
     offsetof(EnemyBulletPatternFieldsView, spriteVariant) ==
         offsetof(BulletSpawnDescriptorView, color) &&
     offsetof(EnemyBulletPatternFieldsView, spawnPositionX) ==
         offsetof(BulletSpawnDescriptorView, position) &&
     offsetof(EnemyBulletPatternFieldsView, angleA) ==
         offsetof(BulletSpawnDescriptorView, angle) &&
     offsetof(EnemyBulletPatternFieldsView, angleB) ==
         offsetof(BulletSpawnDescriptorView, angleStep) &&
     offsetof(EnemyBulletPatternFieldsView, speedA) ==
         offsetof(BulletSpawnDescriptorView, speed1) &&
     offsetof(EnemyBulletPatternFieldsView, speedB) ==
         offsetof(BulletSpawnDescriptorView, speed2) &&
     offsetof(EnemyBulletPatternFieldsView, transforms) ==
         offsetof(BulletSpawnDescriptorView, transforms) &&
     offsetof(EnemyBulletPatternFieldsView, laserStartOffset) ==
         offsetof(BulletSpawnDescriptorView, laserStartOffset) &&
     offsetof(EnemyBulletPatternFieldsView, laserEndOffset) ==
         offsetof(BulletSpawnDescriptorView, laserEndOffset) &&
     offsetof(EnemyBulletPatternFieldsView, laserStartLength) ==
         offsetof(BulletSpawnDescriptorView, laserStartLength) &&
     offsetof(EnemyBulletPatternFieldsView, laserWidth) ==
         offsetof(BulletSpawnDescriptorView, laserWidth) &&
     offsetof(EnemyBulletPatternFieldsView, laserStartTime) ==
         offsetof(BulletSpawnDescriptorView, laserStartTime) &&
     offsetof(EnemyBulletPatternFieldsView, laserDuration) ==
         offsetof(BulletSpawnDescriptorView, laserDuration) &&
     offsetof(EnemyBulletPatternFieldsView, laserDespawnDuration) ==
         offsetof(BulletSpawnDescriptorView, laserDespawnDuration) &&
     offsetof(EnemyBulletPatternFieldsView, laserHitboxStartTime) ==
         offsetof(BulletSpawnDescriptorView, laserHitboxStartTime) &&
     offsetof(EnemyBulletPatternFieldsView, laserHitboxEndDelay) ==
         offsetof(BulletSpawnDescriptorView, laserHitboxEndDelay) &&
     offsetof(EnemyBulletPatternFieldsView, countA) ==
         offsetof(BulletSpawnDescriptorView, count1) &&
     offsetof(EnemyBulletPatternFieldsView, countB) ==
         offsetof(BulletSpawnDescriptorView, count2) &&
     offsetof(EnemyBulletPatternFieldsView, aimMode) ==
         offsetof(BulletSpawnDescriptorView, aimMode) &&
     offsetof(EnemyBulletPatternFieldsView, unknown1FA) ==
         offsetof(BulletSpawnDescriptorView, unknown1FA) &&
     offsetof(EnemyBulletPatternFieldsView, transformFlags) ==
         offsetof(BulletSpawnDescriptorView, transformFlags) &&
     offsetof(EnemyBulletPatternFieldsView, spawnSound) ==
         offsetof(BulletSpawnDescriptorView, spawnSound) &&
     offsetof(EnemyBulletPatternFieldsView, transformSound) ==
         offsetof(BulletSpawnDescriptorView, transformSound) &&
     offsetof(EnemyBulletPatternFieldsView, transformStartIndex) ==
         offsetof(BulletSpawnDescriptorView, transformStartIndex) &&
     offsetof(EnemyBulletPatternFieldsView, templateSprites) ==
         offsetof(BulletSpawnDescriptorView, templateSprites)) ? 1 : -1];

// Native ECL index arithmetic uses a flat 32-bit base plus a typed stride.
// The script operands are not clamped by the original dispatcher.
template <class T> static __forceinline T *EnemyNativeWordIndex(T *first, int index)
{
    return reinterpret_cast<T *>(
        reinterpret_cast<unsigned int>(first) +
        static_cast<unsigned int>(index) * static_cast<unsigned int>(sizeof(T)));
}

// Managed ANM IDs occupy native four-byte slots. Script-indexed VM ops use
// the target's unchecked modulo-DWORD addressing, while SET_FLAGS and
// CLEAR_FLAGS visit all ten valid slots. Centralize the VM handle overlay.
static __forceinline AnmVmIdView *EnemyManagedVmIdAt(
    EnemyManagedVmIdSlotsView &slots, int index)
{
    return reinterpret_cast<AnmVmIdView *>(
        EnemyNativeWordIndex(&slots.views[0], index));
}

// Native 0x210-byte pattern index/address word for unchecked ECL operands.
static __forceinline EnemyBulletPatternView *EnemyPatternAtRuntimeIndex(
    EnemyRuntimeView *runtime, int patternIndex)
{
    return reinterpret_cast<EnemyBulletPatternView *>(
        reinterpret_cast<unsigned int>(&runtime->bulletPatterns[0]) +
        static_cast<unsigned int>(patternIndex) *
        static_cast<unsigned int>(sizeof(EnemyBulletPatternView)));
}

// A script index selects a 12-byte XYZ offset in the same runtime record.
// Original i386 IMUL/LEA form low 32-bit words, without an index clamp.
static __forceinline PlayerFloat3 *EnemyPatternOffsetAtRuntimeIndex(
    EnemyRuntimeView *runtime, int index)
{
    return reinterpret_cast<PlayerFloat3 *>(
        reinterpret_cast<unsigned int>(&runtime->bulletPatternOffsets[0]) +
        static_cast<unsigned int>(index) *
        static_cast<unsigned int>(sizeof(PlayerFloat3)));
}

// Some ECL opcodes carry the offset index as a native DWORD index (rather
// than a 12-byte record index). Retain that intermediate cursor across the
// X/Y reads and the pattern selection before the final Z read.
static __forceinline const PlayerFloat3 *EnemyPatternOffsetAtWordCursor(
    unsigned int runtimeAddress, unsigned int offsetDwordCursor)
{
    return reinterpret_cast<const PlayerFloat3 *>(
        runtimeAddress + offsetDwordCursor * sizeof(unsigned int));
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
static __forceinline BulletTransformRecordView *EnemyExtraRecordAt(int baseAddress)
{
    return reinterpret_cast<BulletTransformRecordView *>(
        static_cast<unsigned int>(baseAddress) +
        static_cast<unsigned int>(
            offsetof(EnemyRuntimeView, bulletPatterns) +
            offsetof(EnemyBulletPatternFieldsView, transforms)));
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
    MainCameraRecordView *record = &supervisor->cameraRecords[viewportIndex];
    AnmViewportOwnerView *viewport = &record->prefix;
    supervisor->activeViewport = viewport;
    EnemyConfigureViewportOwner(viewport);
    supervisor->d3dDevice->vtable->SetViewport(
        supervisor->d3dDevice, &supervisor->activeViewport->viewport);
    supervisor->viewportConfigured = viewportIndex;
}
extern float g_AnmGameSpeed;

__declspec(noinline) void EnemyInitializePositionInterpolation(
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
    EnemyLaserNodeView *laser, const PlayerFloat3 *position)
{
    reinterpret_cast<EnemyLaserPositionAt430View *>(laser)->position430 =
        *position;
}
static __declspec(noinline) EnemyLaserNodeView *EnemyFindLaser(
    EnemyLaserManagerView *manager, int id)
{
    EnemyLaserNodeView *laser = manager->head;
    while (laser != 0) {
        if (laser->id == id) {
            return laser;
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

__declspec(noinline) void EnemyInitializeScalarInterpolation(
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
    // The retail read occurs after publishing the message pointer and
    // message index. Keep this one observed global load ordered.
    const volatile int &previousIndexRef = g_EnemyDialogIndex;
    int previousIndex = previousIndexRef;
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
        // The unchecked count uses native 32-bit INC wrap, even when an
        // empty slot retains the most-positive signed count word.
        const unsigned int nextCountBits =
            static_cast<unsigned int>(sampleCounts[slot]) + 1u;
        memcpy(&sampleCounts[slot], &nextCountBits, sizeof nextCountBits);
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
        // The unchecked count uses native 32-bit INC wrap, even when an
        // empty slot retains the most-positive signed count word.
        const unsigned int nextCountBits =
            static_cast<unsigned int>(sampleCounts[slot]) + 1u;
        memcpy(&sampleCounts[slot], &nextCountBits, sizeof nextCountBits);
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
    int *itemDropBlock, const PlayerFloat3 *position)
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
    return EclVmContext::ReadFloatValue(host->activeContext, index, rawValue);
}

#include "TransitionEffect.hpp"
#include "EclWorldAnchor.hpp"
#include "EnemyGameState.hpp"
// Dispatch control-flow shape adapted from levzzz5154/th10-decomp (MIT).
struct Float2 {
    float x, y;
    Float2() {}
    Float2(float x_, float y_) { x = x_; y = y_; }
};
struct Float3 {
    float x, y, z;
    Float3() {}
    struct LinePosition {};
    Float3(const Float3 &first, const Float3 &second, LinePosition)
        : x(second.x + first.x), y(first.y + second.y), z(first.z + second.z) {}
    Float3(const Float3 &first, const Float3 &second)
        : x(first.x + second.x), y(first.y + second.y), z(first.z + second.z) {}
    Float3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
};
static __forceinline Float3 operator+(const Float3& a, const Float3& b) {
    return Float3(a.x + b.x, a.y + b.y, a.z + b.z);
}
struct FrameTimer {
    int previous;
    int current;
    float elapsed;
    const float* speed;
    unsigned int flags;
};

struct Float3Interp {
    Float3 initial, goal, control_1, control_2;
    FrameTimer timer;
    int end_time;
    int mode;
};
struct Float2Interp {
    Float2 initial, goal, control_1, control_2;
    FrameTimer timer;
    int end_time;
    int mode;
};

struct EnemyDispatchMotionView {
    Float3 position;        // +0x00
    Float3 origin;          // +0x0C
    float speed;            // +0x18
    float angle;            // +0x1C
    float radial_speed;     // +0x20
    float angular_speed;    // +0x24
    unsigned int flags;     // +0x28
};
struct EnemyDispatchPatternCommandView {
    float a, b;
    int c, d, e, f;
};
struct EnemyDispatchPatternView {
    short type;             // +0x000
    short color;            // +0x002
    Float3 position;        // +0x004
    float angle_1;          // +0x010
    float angle_2;          // +0x014
    float speed_1;          // +0x018
    float speed_2;          // +0x01C
    EnemyDispatchPatternCommandView ex[18];       // +0x020
    unsigned char unknown_1d0[0x24];
    short count_1;          // +0x1F4
    short count_2;          // +0x1F6
    short aim_mode;         // +0x1F8
    unsigned int flags;     // +0x1FC
    int sound;              // +0x200
    int sound_2;            // +0x204
    unsigned char unknown_208[8];
};
struct Laser {
    unsigned char unknown_000[0x24];
    Float3 f24;                 // +0x024
    unsigned char unknown_030[0xC];
    float f3c;                  // +0x03C
    unsigned char unknown_040[4];
    float f44;                  // +0x044
    float f48;                  // +0x048
    unsigned char unknown_04c[0x430 - 0x4C];
    Float3 f430;                // +0x430
    unsigned char unknown_43c[4];
    float f440;                 // +0x440
};
struct LaserParams {
    Float3 position;
};
struct LineLaserParams : LaserParams {
    float f0c, f10, f14, f18, f1c, f20;
    short type, color;      // +0x24
    unsigned int flags;     // +0x28
    EnemyDispatchPatternCommandView ex[18];       // +0x2C
};
struct BeamLaserParams : LaserParams {
    float f0c, f10, f14, f18, f1c, f20, f24, f28, f2c;
    int f30, f34, f38, f3c;
    short type, color;      // +0x40
    unsigned int flags;     // +0x44
    EnemyDispatchPatternCommandView ex[18];       // +0x48
    __forceinline BeamLaserParams() { memset(this, 0, sizeof(*this)); f2c = 8.0f; }
    struct Raw {};
    __forceinline BeamLaserParams(Raw) {}
};

// Keep the double literal: motion operands use a strict greater-than test.
#define UNSET_MOTION_COMPONENT (-999999.0)

static __forceinline Float3 operator-(const Float3& a, const Float3& b) {
    return Float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static __forceinline void SubtractWait(EnemyFullObjectView *enemy, float wait)
{
    enemy->activeEclContext->currentTime -= wait;
}
static __forceinline float OperandRange(float low, float high) { return high - low; }
static __forceinline float SquaredLength(float x, float y) { return x * x + y * y; }
static __forceinline float DistanceSquared(const Float3& a, const Float3& b) { return SquaredLength(a.x - b.x, a.y - b.y); }

// Enemy-specific ECL opcodes (0x100..0x1B4) dispatched from EclVm::Run.
enum EnemyDispatchOpcode {
    // Spawning (boss-guarded variants skip spawn when boss[0] != 0)
    ECL_ENEMY_CREATE = 0x100,
    ECL_ENEMY_CREATE_ABS = 0x101,
    ECL_ENEMY_CREATE_MIRRORED = 0x104,
    ECL_ENEMY_CREATE_ABS_MIRRORED = 0x105,
    ECL_ENEMY_CREATE_NO_BOSS = 0x109,
    ECL_ENEMY_CREATE_ABS_NO_BOSS = 0x10a,
    ECL_ENEMY_CREATE_MIRRORED_NO_BOSS = 0x10b,
    ECL_ENEMY_CREATE_ABS_MIRRORED_NO_BOSS = 0x10c,
    ECL_ENEMY_CREATE_3D = 0x10e,
    ECL_ENEMY_CREATE_3D_NO_BOSS = 0x10f,

    // Animations & effects
    ECL_ANM_SELECT = 0x102,
    ECL_ANM_SET_SLOT = 0x103,
    ECL_ANM_SET_MAIN = 0x106,
    ECL_ANM_SPAWN_FRONT_AT = 0x107,
    ECL_ANM_SPAWN_FRONT = 0x108,
    ECL_ANM_SET_SLOT_ALT = 0x10d,
    ECL_ANM_SPAWN_BACK_AT = 0x110,
    ECL_ANM_SPAWN_FRONT_ROTATED = 0x111,

    // Motion (even = absolute, odd = relative where paired)
    ECL_MOVE_POS_ABS = 0x118,
    ECL_MOVE_POS_TIME_ABS = 0x119,
    ECL_MOVE_POS_REL = 0x11a,
    ECL_MOVE_POS_TIME_REL = 0x11b,
    ECL_MOVE_VEL_ABS = 0x11c,
    ECL_MOVE_VEL_TIME_ABS = 0x11d,
    ECL_MOVE_VEL_REL = 0x11e,
    ECL_MOVE_VEL_TIME_REL = 0x11f,
    ECL_MOVE_ORBIT_ABS = 0x120,
    ECL_MOVE_ORBIT_TIME_ABS = 0x121,
    ECL_MOVE_ORBIT_REL = 0x122,
    ECL_MOVE_ORBIT_TIME_REL = 0x123,
    ECL_MOVE_RAND_ABS = 0x124,
    ECL_MOVE_RAND_REL = 0x125,
    ECL_MOVE_TO_BOSS_ABS = 0x126,
    ECL_MOVE_TO_BOSS_REL = 0x127,
    ECL_MOVE_ADD_ABS = 0x128,
    ECL_MOVE_ADD_REL = 0x129,
    ECL_MOVE_ORIGIN_ABS = 0x12a,
    ECL_MOVE_ORIGIN_REL = 0x12b,

    // Hitboxes, flags, bounds, drops, life & boss state
    ECL_SET_HITBOX = 0x140,
    ECL_SET_CONTACT_BOX = 0x141,
    ECL_FLAGS_SET = 0x142,
    ECL_FLAGS_CLEAR = 0x143,
    ECL_SET_BOUNDS = 0x144,
    ECL_CLEAR_BOUNDS = 0x145,
    ECL_DROP_CLEAR_EXTRA = 0x146,
    ECL_DROP_ADD_EXTRA = 0x147,
    ECL_DROP_SET_SPREAD = 0x148,
    ECL_DROP_FLUSH = 0x149,
    ECL_DROP_SET_MAIN = 0x14a,
    ECL_SET_LIFE = 0x14b,
    ECL_SET_BOSS = 0x14c,
    ECL_RESET_TIMER = 0x14d,
    ECL_BIND_INTERRUPT = 0x14e,
    ECL_SET_INVULN_TIMER = 0x14f,
    ECL_PLAY_SOUND = 0x150,
    ECL_SCREEN_EFFECT = 0x151,
    ECL_STAGE_CLEAR_BONUS = 0x152,
    ECL_WAIT_DIALOGUE = 0x153,
    ECL_WAIT_BOSS_CLEAR = 0x154,
    ECL_SET_INTERRUPT_CURSOR = 0x155,
    ECL_SPELL_START = 0x156,
    ECL_SPELL_END = 0x157,
    ECL_SCORE_SET_MODE = 0x158,
    ECL_KILL_ALL_ENEMIES = 0x159,
    ECL_SET_SHOOT_RANGE = 0x15a,
    ECL_SET_LIFE_MARKER = 0x15b,
    ECL_SPELL_START_ALT = 0x15c,
    ECL_RANK_SELECT_F3 = 0x15d,
    ECL_RANK_SELECT_F5 = 0x15e,
    ECL_RANK_LERP_F = 0x15f,
    ECL_RANK_SELECT_I3 = 0x160,
    ECL_RANK_SELECT_I5 = 0x161,
    ECL_RANK_LERP_I = 0x162,
    ECL_DIFF_SELECT_I = 0x163,
    ECL_DIFF_SELECT_F = 0x164,
    ECL_SPELL_START_DIFF = 0x165,
    ECL_SPELL_START_DIFF_M1 = 0x166,
    ECL_SPELL_START_DIFF_M2 = 0x167,
    ECL_FRONT_SET_STATE = 0x168,
    ECL_SET_CONTACT_TIMER = 0x169,
    ECL_SPELL_MARK_FLAG = 0x16a,
    ECL_SPELL_STOP_ANIM = 0x16b,
    ECL_SET_FLAG_19 = 0x16c,
    ECL_LASER_MARK_ELIGIBLE = 0x16d,
    ECL_SET_COVERED_SCRIPT = 0x16e,
    ECL_SET_GAME_SPEED = 0x16f,
    ECL_WAIT_BY_DIFF = 0x170,

    // Shooters & lasers
    ECL_SHOOTER_RESET = 0x190,
    ECL_SHOOTER_FIRE = 0x191,
    ECL_SHOOTER_TYPE = 0x192,
    ECL_SHOOTER_OFFSET = 0x193,
    ECL_SHOOTER_ANGLE = 0x194,
    ECL_SHOOTER_SPEED = 0x195,
    ECL_SHOOTER_COUNT = 0x196,
    ECL_SHOOTER_AIM = 0x197,
    ECL_SHOOTER_SOUND = 0x198,
    ECL_SHOOTER_EX = 0x199,
    ECL_CLEAR_BULLETS_TO_ITEMS = 0x19a,
    ECL_SHOOTER_COPY = 0x19b,
    ECL_LASER_LINE = 0x19c,
    ECL_LASER_BEAM = 0x19d,
    ECL_LASER_SET_POS = 0x19e,
    ECL_LASER_SET_VEC = 0x19f,
    ECL_LASER_SET_F48 = 0x1a0,
    ECL_LASER_SET_F44 = 0x1a1,
    ECL_LASER_SET_F3C = 0x1a2,
    ECL_LASER_SET_F440 = 0x1a3,
    ECL_CLEAR_RADIUS_ITEMS = 0x1a4,
    ECL_CLEAR_RADIUS = 0x1a5,
    ECL_SHOOTER_SPEED_RANK3 = 0x1a6,
    ECL_SHOOTER_SPEED_RANK5 = 0x1a7,
    ECL_SHOOTER_SPEED_RANK_LERP = 0x1a8,
    ECL_SHOOTER_COUNT_RANK3 = 0x1a9,
    ECL_SHOOTER_COUNT_RANK5 = 0x1aa,
    ECL_SHOOTER_COUNT_RANK_LERP = 0x1ab,
    ECL_LASER_LINE_UNFLAGGED = 0x1ac,
    ECL_LASER_BEAM_UNFLAGGED = 0x1ad,
    ECL_ANGLE_TO_PLAYER = 0x1ae,
    ECL_LASER_LINE_EX = 0x1af,
    ECL_LASER_BEAM_EX = 0x1b0,
    ECL_LASER_LINE_EX_UNFLAGGED = 0x1b1,
    ECL_LASER_BEAM_EX_UNFLAGGED = 0x1b2,
    ECL_SHOOTER_SPEED_DIFF = 0x1b3,
    ECL_SHOOTER_COUNT_DIFF = 0x1b4
};

// EnemyRuntimeView flags read or changed by these ECL commands.
enum EnemyExecFlags {
    ENEMY_FLAG_INTANGIBLE = 0x10,
    ENEMY_FLAG_LIMITED = 0x200,
    ENEMY_FLAG_MIRRORED = 0x800,
    ENEMY_FLAG_FACES = 0x1000,
    ENEMY_FLAG_BOSS = 0x8000,
    ENEMY_FLAG_UNSCROLLED = 0x40000
};

#include "EnemyLaser.hpp"
extern int g_EnemyDifficulty;
extern EnemyChapterStateView g_EnemyChapterState;
static __forceinline void ApplyDispatcherLaserVector(Laser *laser, const Float3 &value)
{
    EnemySetLaserPosition(reinterpret_cast<EnemyLaserNodeView *>(laser),
        reinterpret_cast<const PlayerFloat3 *>(&value));
}

static __forceinline AnmLoadedView *GetDispatcherAnimation(int file)
{
    return *EnemyNativeWordIndex(&g_EnemyManager->loadedAnmResources[0], file);
}

static __forceinline void SetDispatcherDropSpread(EnemyItemDropCountRadiusView &drops,
    float width, float height)
{
    drops.radiusX = width;
    drops.radiusY = height;
}

static __forceinline int ReadDispatchHostInt(EclVmHost *host, int index)
{
    return host->activeContext->ReadInt(index);
}

static __forceinline int ReadDispatcherInt(EnemyRuntimeView *runtime, int index)
{
    return ReadDispatchHostInt(reinterpret_cast<EclVmHost *>(runtime->owner), index);
}

extern const float g_EnemyRankMidpoint;

static __forceinline void SetDispatcherGaugeMarker(GuiView *gui, int index,
    int full, float amount, int color)
{
    GuiBossGaugeValueView *marker = EnemyNativeWordIndex(&gui->bossGaugeValues[0], index);
    marker->color = color;
    marker->value = amount / static_cast<float>(full);
}

struct EnemyDispatchFlags
{
    unsigned int precedingBits : 19;
    unsigned int bit19 : 1;
};

int EnemyRuntimeView::DispatchEclInstruction() {
    EnemyEclInstructionView *instruction = owner->activeEclContext->currentInstruction;
    switch (instruction->opcode) {
    // Spawning. Each boss-guarded variant falls through to the corresponding
    // spawn when boss slot zero is empty. Numeric operands follow the packed name.
    case ECL_ENEMY_CREATE_NO_BOSS:
        if (g_EnemyManager->specialEnemySlots[0] != 0) break;
    // -----------------------------------------------------------------------
    // Enemy spawning (0x100..0x10f)
    // -----------------------------------------------------------------------
    case ECL_ENEMY_CREATE: {
        int first_numeric_word = static_cast<int>(static_cast<unsigned int>(instruction->spawnOperands.inlineNameByteCount) + 4u) / 4;
        EnemySpawnRequestView spawn;
        memset(&spawn, 0, sizeof(spawn));
        if (!(flags & ENEMY_FLAG_UNSCROLLED)) {
            float x = reinterpret_cast<Float3 &>(worldMotion.position).x;
            spawn.position.x = x + ReadRawFloatArgument(1, EnemySpawnScalarWordAt(instruction, first_numeric_word)->real);
            float y = reinterpret_cast<Float3 &>(worldMotion.position).y;
            spawn.position.y = y + ReadRawFloatArgument(2, EnemySpawnScalarWordAt(instruction, first_numeric_word + 1)->real);
        } else {
            Float3 world;
            float x = reinterpret_cast<Float3 &>(worldMotion.position).x;
            world.x = x + ReadRawFloatArgument(1, EnemySpawnScalarWordAt(instruction, first_numeric_word)->real);
            float y = reinterpret_cast<Float3 &>(worldMotion.position).y;
            world.y = y + ReadRawFloatArgument(2, EnemySpawnScalarWordAt(instruction, first_numeric_word + 1)->real);
            world.z = reinterpret_cast<Float3 &>(worldMotion.position).z;
            EnemyPrepareProjection(&g_MainSupervisorView, 0);
            D3DXVec3Project(reinterpret_cast<PlayerFloat3 *>(&spawn.position),
                reinterpret_cast<const PlayerFloat3 *>(&world), &g_EnemyViewport,
                &g_EnemyProjectionMatrix, &g_EnemyViewMatrix, &g_EnemyWorldMatrix);
            spawn.position.x -= 224.0f;
            spawn.position.z = 0.0f;
            spawn.position.y -= 16.0f;
        }
        spawn.life = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, first_numeric_word + 2)->integer);
        spawn.scoreReward = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, first_numeric_word + 3)->integer);
        spawn.itemDropType = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, first_numeric_word + 4)->integer);
        memcpy(&spawn.eclVariables, &eclVariables, sizeof(eclVariables));
        EnemySpawn(g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName), &spawn);
        return 0;
    }
    case ECL_ENEMY_CREATE_ABS_NO_BOSS:
        if (g_EnemyManager->specialEnemySlots[0] != 0) break;
    case ECL_ENEMY_CREATE_ABS: {
        int first_numeric_word = static_cast<int>(static_cast<unsigned int>(instruction->spawnOperands.inlineNameByteCount) + 4u) / 4;
        EnemySpawnRequestView spawn;
        memset(&spawn, 0, sizeof(spawn));
        spawn.position.x = ReadRawFloatArgument(1, EnemySpawnScalarWordAt(instruction, first_numeric_word)->real);
        spawn.position.y = ReadRawFloatArgument(2, EnemySpawnScalarWordAt(instruction, first_numeric_word + 1)->real);
        spawn.life = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, first_numeric_word + 2)->integer);
        spawn.scoreReward = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, first_numeric_word + 3)->integer);
        spawn.itemDropType = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, first_numeric_word + 4)->integer);
        memcpy(&spawn.eclVariables, &eclVariables, sizeof(eclVariables));
        EnemySpawn(g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName), &spawn);
        return 0;
    }
    case ECL_ENEMY_CREATE_3D_NO_BOSS:
        if (g_EnemyManager->specialEnemySlots[0] != 0) break;
    case ECL_ENEMY_CREATE_3D: {
        int first_numeric_word = static_cast<int>(static_cast<unsigned int>(instruction->spawnOperands.inlineNameByteCount) + 4u) / 4;
        EnemySpawnRequestView spawn;
        memset(&spawn, 0, sizeof(spawn));
        spawn.position.x = ReadRawFloatArgument(1, EnemySpawnScalarWordAt(instruction, first_numeric_word)->real) + g_EnemyGlobalPositionOffset.x;
        spawn.position.y = ReadRawFloatArgument(2, EnemySpawnScalarWordAt(instruction, first_numeric_word + 1)->real) + g_EnemyGlobalPositionOffset.y;
        spawn.position.z = ReadRawFloatArgument(3, EnemySpawnScalarWordAt(instruction, first_numeric_word + 2)->real);
        spawn.life = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, first_numeric_word + 3)->integer);
        spawn.scoreReward = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, first_numeric_word + 4)->integer);
        spawn.itemDropType = ReadRawIntArgument(6, EnemySpawnScalarWordAt(instruction, first_numeric_word + 5)->integer);
        memcpy(&spawn.eclVariables, &eclVariables, sizeof(eclVariables));
        spawn.setFlag40000 |= 1;
        EnemySpawn(g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName), &spawn);
        return 0;
    }
    case ECL_ENEMY_CREATE_MIRRORED_NO_BOSS:
        if (g_EnemyManager->specialEnemySlots[0] != 0) break;
    case ECL_ENEMY_CREATE_MIRRORED: {
        int first_numeric_word = static_cast<int>(static_cast<unsigned int>(instruction->spawnOperands.inlineNameByteCount) + 4u) / 4;
        EnemySpawnRequestView spawn;
        memset(&spawn, 0, sizeof(spawn));
        float x = reinterpret_cast<Float3 &>(worldMotion.position).x;
        spawn.position.x = x + ReadRawFloatArgument(1, EnemySpawnScalarWordAt(instruction, first_numeric_word)->real);
        float y = reinterpret_cast<Float3 &>(worldMotion.position).y;
        spawn.position.y = y + ReadRawFloatArgument(2, EnemySpawnScalarWordAt(instruction, first_numeric_word + 1)->real);
        spawn.life = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, first_numeric_word + 2)->integer);
        spawn.scoreReward = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, first_numeric_word + 3)->integer);
        spawn.itemDropType = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, first_numeric_word + 4)->integer);
        memcpy(&spawn.eclVariables, &eclVariables, sizeof(eclVariables));
        spawn.setFlag0800 = 1;
        EnemySpawn(g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName), &spawn);
        return 0;
    }
    case ECL_ENEMY_CREATE_ABS_MIRRORED_NO_BOSS:
        if (g_EnemyManager->specialEnemySlots[0] != 0) break;
    case ECL_ENEMY_CREATE_ABS_MIRRORED: {
        int first_numeric_word = static_cast<int>(static_cast<unsigned int>(instruction->spawnOperands.inlineNameByteCount) + 4u) / 4;
        EnemySpawnRequestView spawn;
        memset(&spawn, 0, sizeof(spawn));
        spawn.position.x = ReadRawFloatArgument(1, EnemySpawnScalarWordAt(instruction, first_numeric_word)->real);
        spawn.position.y = ReadRawFloatArgument(2, EnemySpawnScalarWordAt(instruction, first_numeric_word + 1)->real);
        spawn.life = ReadRawIntArgument(3, EnemySpawnScalarWordAt(instruction, first_numeric_word + 2)->integer);
        spawn.scoreReward = ReadRawIntArgument(4, EnemySpawnScalarWordAt(instruction, first_numeric_word + 3)->integer);
        spawn.itemDropType = ReadRawIntArgument(5, EnemySpawnScalarWordAt(instruction, first_numeric_word + 4)->integer);
        memcpy(&spawn.eclVariables, &eclVariables, sizeof(eclVariables));
        spawn.setFlag0800 = 1;
        EnemySpawn(g_EnemyManager, reinterpret_cast<const char *>(instruction->spawnOperands.subroutineName), &spawn);
        return 0;
    }

    // Animation files, attached slots and independent effects.
    // -----------------------------------------------------------------------
    // Animations & visual effects (0x102..0x111)
    // -----------------------------------------------------------------------
    case ECL_ANM_SELECT:
        selectedAnmResourceIndex = ReadDispatcherInt(this, 0);
        return 0;
    case ECL_ANM_SET_SLOT: {
        int slot = ReadDispatcherInt(this, 0);
        int script = ReadDispatcherInt(this, 1);
        EnemyManagedVmIdAt(managedVmIds, slot)->Release();
        if (script < 0) break;
        *EnemyManagedVmIdAt(managedVmIds, slot) = GetDispatcherAnimation(selectedAnmResourceIndex)->CreateVmVariant2(ReadDispatcherInt(this, 1), 5);
        if (slot == 0) {
            primaryAnmScriptIndex = ReadDispatcherInt(this, 1);
            primaryAnmResourceIndex = selectedAnmResourceIndex;
        }
        AnmVmView *vm = EnemyManagedVmIdAt(managedVmIds, slot)->GetVm();
        if (slot == 0) {
            // Retail swaps the axes here: visibilityExtentX (+0x13A4) takes sprite_height * scale.y
            // and visibilityExtentY (+0x13A8) takes sprite_width * scale.x.
            visibilityExtentX = vm->spriteHeight * vm->scaleY;
            visibilityExtentY = vm->spriteWidth * vm->scaleX;
        }
        if (flags & ENEMY_FLAG_INTANGIBLE) EnemyManagedVmIdAt(managedVmIds, slot)->ClearFlag2();
        return 0;
    }
    case ECL_ANM_SPAWN_FRONT_AT: {
        int file = ReadDispatcherInt(this, 0);
        AnmVmIdView handle = GetDispatcherAnimation(file)->CreateVmVariant2(ReadDispatcherInt(this, 1), 6);
        AnmVmView *vm = handle.GetVm();
        if (!(flags & ENEMY_FLAG_UNSCROLLED)) vm->positionOffset = *PublishEclWorldAnchor(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position));
        else vm->positionOffset = reinterpret_cast<const AnmFloat3View &>(worldMotion.position);
        goto finish_instruction;
    }
    case ECL_ANM_SPAWN_BACK_AT: {
        int file = ReadDispatcherInt(this, 0);
        AnmVmIdView handle = GetDispatcherAnimation(file)->CreateVmVariant0(ReadDispatcherInt(this, 1), 6);
        AnmVmView *vm = handle.GetVm();
        if (!(flags & ENEMY_FLAG_UNSCROLLED)) vm->positionOffset = *PublishEclWorldAnchor(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position));
        else vm->positionOffset = reinterpret_cast<const AnmFloat3View &>(worldMotion.position);
        goto leave_dispatch;
    }
    case ECL_ANM_SPAWN_FRONT: {
        int file = ReadDispatcherInt(this, 0);
        GetDispatcherAnimation(file)->CreateVmVariant2(ReadDispatcherInt(this, 1), 6);
        return 0;
    }
    case ECL_ANM_SPAWN_FRONT_ROTATED: {
        int file = ReadDispatcherInt(this, 0);
        AnmVmIdView handle = GetDispatcherAnimation(file)->CreateVmVariant2(ReadDispatcherInt(this, 1), 6);
        AnmVmView *vm = handle.GetVm();
        if (!(flags & ENEMY_FLAG_UNSCROLLED)) vm->positionOffset = *PublishEclWorldAnchor(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position));
        else vm->positionOffset = reinterpret_cast<const AnmFloat3View &>(worldMotion.position);
        vm->rotation.z = ReadFloatArgument(2);
        vm->flags35C |= 4u;
        return 0;
    }
    case ECL_ANM_SET_MAIN: {
        int slot = ReadDispatcherInt(this, 0);
        int script = ReadDispatcherInt(this, 1);
        EnemyManagedVmIdAt(managedVmIds, slot)->Release();
        *EnemyManagedVmIdAt(managedVmIds, slot) = GetDispatcherAnimation(selectedAnmResourceIndex)->CreateVmVariant2(script, 5);
        AnmVmView *vm = EnemyManagedVmIdAt(managedVmIds, slot)->GetVm();
        if (slot == 0) {
            visibilityExtentX = vm->spriteHeight * vm->scaleY;
            visibilityExtentY = vm->spriteWidth * vm->scaleX;
        }
        if (flags & ENEMY_FLAG_INTANGIBLE) EnemyManagedVmIdAt(managedVmIds, slot)->ClearFlag2();
        if (slot == 0) {
            flags |= ENEMY_FLAG_FACES;
            animationBaseScript = script;
            primaryAnmScriptIndex = script;
            animationDirection = 0;
            primaryAnmResourceIndex = selectedAnmResourceIndex;
        }
        return 0;
    }
    case ECL_ANM_SET_SLOT_ALT: {
        int slot = ReadDispatcherInt(this, 0);
        EnemyManagedVmIdAt(managedVmIds, slot)->Release();
        AnmLoadedView *resource = GetDispatcherAnimation(selectedAnmResourceIndex);
        *EnemyManagedVmIdAt(managedVmIds, slot) = resource->CreateVmVariant2(animationBaseScript + 5, 5);
        AnmVmView *vm = EnemyManagedVmIdAt(managedVmIds, slot)->GetVm();
        if (slot == 0) {
            visibilityExtentX = vm->spriteHeight * vm->scaleY;
            visibilityExtentY = vm->spriteWidth * vm->scaleX;
        }
        if (flags & ENEMY_FLAG_INTANGIBLE) EnemyManagedVmIdAt(managedVmIds, slot)->ClearFlag2();
        return 0;
    }

    // Paired opcodes select the base or offset motion record before applying
    // the same operation. An unset component leaves the current value in place
    // or supplies the mode-specific interpolation default.
    // -----------------------------------------------------------------------
    // Motion & trajectories (0x118..0x12b)
    // -----------------------------------------------------------------------
    case ECL_MOVE_POS_ABS:
    case ECL_MOVE_POS_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_POS_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        float x = ReadFloatArgument(0);
        float y = ReadFloatArgument(1);
        if (x > UNSET_MOTION_COMPONENT) motion->position.x = x;
        if (y > UNSET_MOTION_COMPONENT) motion->position.y = y;
        motion->flags &= ~1u;
        reinterpret_cast<Float3 &>(worldMotion.position) = reinterpret_cast<EnemyDispatchMotionView &>(baseMotion).position + reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion).position;
        return 0;
    }
    case ECL_MOVE_ADD_ABS:
    case ECL_MOVE_ADD_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_ADD_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        float x = ReadFloatArgument(0);
        float y = ReadFloatArgument(1);
        float z = ReadFloatArgument(2);
        motion->position.x += x;
        motion->position.y += y;
        motion->position.z += z;
        return 0;
    }
    case ECL_MOVE_POS_TIME_ABS:
    case ECL_MOVE_POS_TIME_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_POS_TIME_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        Float3Interp* interp = instruction->opcode == ECL_MOVE_POS_TIME_ABS ? &reinterpret_cast<Float3Interp &>(positionInterpolation0) : &reinterpret_cast<Float3Interp &>(positionInterpolation1);
        float x = ReadFloatArgument(2);
        float y = ReadFloatArgument(3);
        interp->end_time = ReadDispatcherInt(this, 0);
        interp->control_1 = reinterpret_cast<const Float3 &>(g_EnemyInterpolationOrigin);
        interp->control_2 = reinterpret_cast<const Float3 &>(g_EnemyInterpolationOrigin);
        interp->mode = ReadDispatcherInt(this, 1);
        interp->initial = motion->position;
        interp->goal = Float3(x > UNSET_MOTION_COMPONENT ? x : motion->position.x,
                              y > UNSET_MOTION_COMPONENT ? y : motion->position.y, 0.0f);
        EnemyInitializePositionInterpolation(reinterpret_cast<EnemyPositionInterpolationView *>(interp));
        motion->flags &= ~1u;
        return 0;
    }
    case ECL_MOVE_VEL_ABS:
    case ECL_MOVE_VEL_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_VEL_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        float angle;
        float speed;
        angle = ReadFloatArgument(0);
        speed = ReadFloatArgument(1);
        if (angle > UNSET_MOTION_COMPONENT) {
            if (flags & ENEMY_FLAG_MIRRORED) angle = EnemyWrapAngle(1.5707964f - EnemyWrapAngle(angle - 1.5707964f));
            EnemySetMotionAngle(reinterpret_cast<EnemyMotionView *>(motion), angle);
        }
        if (speed > UNSET_MOTION_COMPONENT) motion->speed = speed;
        motion->flags &= ~1u;
        return 0;
    }
    case ECL_MOVE_VEL_TIME_ABS:
    case ECL_MOVE_VEL_TIME_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_VEL_TIME_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        Float2Interp* interp = instruction->opcode == ECL_MOVE_VEL_TIME_ABS ? &reinterpret_cast<Float2Interp &>(scalarInterpolation0) : &reinterpret_cast<Float2Interp &>(scalarInterpolation1);
        float angle = ReadFloatArgument(2);
        float speed = ReadFloatArgument(3);
        interp->mode = ReadDispatcherInt(this, 1);
        Float2 goal;
        if (interp->mode != 7) {
            {
                float goal_angle;
                goal_angle = angle > UNSET_MOTION_COMPONENT
                    ? ((flags & ENEMY_FLAG_MIRRORED) ? EnemyWrapAngle(1.5707964f - EnemyWrapAngle(angle - 1.5707964f)) : angle)
                    : motion->angle;
                goal.x = goal_angle;
            }
            {
                float goal_speed;
                goal_speed = speed > UNSET_MOTION_COMPONENT ? speed : motion->speed;
                goal.y = goal_speed;
            }
        } else {
            if (angle > UNSET_MOTION_COMPONENT) goal.x = angle;
            else goal.x = 0.0f;
            if (speed > UNSET_MOTION_COMPONENT) goal.y = speed;
            else goal.y = 0.0f;
        }
        Float2 initial(motion->angle, motion->speed);
        if (fabs(initial.x - goal.x) >= 3.1415927f) {
            if (initial.x < goal.x) initial.x += 6.2831855f;
            else goal.x += 6.2831855f;
        }
        interp->end_time = ReadDispatcherInt(this, 0);
        interp->control_1 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        interp->control_2 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        interp->initial = initial;
        interp->goal = goal;
        EnemyInitializeScalarInterpolation(reinterpret_cast<EnemyScalarInterpolationView *>(interp));
        motion->flags &= ~1u;
        return 0;
    }
    case ECL_MOVE_ORBIT_ABS:
    case ECL_MOVE_ORBIT_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_ORBIT_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        float angle = ReadFloatArgument(0);
        float speed = ReadFloatArgument(1);
        float radial = ReadFloatArgument(2);
        float angular = ReadFloatArgument(3);
        if (!(motion->flags & 1)) motion->origin = motion->position;
        if (angle > UNSET_MOTION_COMPONENT) EnemySetMotionAngle(reinterpret_cast<EnemyMotionView *>(motion), angle);
        if (speed > UNSET_MOTION_COMPONENT) motion->speed = speed;
        if (radial > UNSET_MOTION_COMPONENT) motion->radial_speed = radial;
        if (angular > UNSET_MOTION_COMPONENT) motion->angular_speed = angular;
        motion->flags |= 1;
        return 0;
    }
    case ECL_MOVE_ORBIT_TIME_ABS:
    case ECL_MOVE_ORBIT_TIME_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_ORBIT_TIME_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        Float2Interp* direction = instruction->opcode == ECL_MOVE_ORBIT_TIME_ABS ? &reinterpret_cast<Float2Interp &>(scalarInterpolation0) : &reinterpret_cast<Float2Interp &>(scalarInterpolation1);
        Float2Interp* orbit = instruction->opcode == ECL_MOVE_ORBIT_TIME_ABS ? &reinterpret_cast<Float2Interp &>(scalarInterpolation2) : &reinterpret_cast<Float2Interp &>(scalarInterpolation3);
        float angle = ReadFloatArgument(2);
        float speed = ReadFloatArgument(3);
        float radial = ReadFloatArgument(4);
        float angular = ReadFloatArgument(5);
        Float2 goal(angle > UNSET_MOTION_COMPONENT ? angle : motion->angle,
                    speed > UNSET_MOTION_COMPONENT ? speed : motion->speed);
        Float2 initial(motion->angle, motion->speed);
        Float2 orbit_goal(radial > UNSET_MOTION_COMPONENT ? radial : motion->radial_speed,
                          angular > UNSET_MOTION_COMPONENT ? angular : motion->angular_speed);
        Float2 orbit_initial(motion->radial_speed, motion->angular_speed);
        int end_time = ReadDispatcherInt(this, 0);
        int mode = ReadDispatcherInt(this, 1);
        direction->end_time = end_time;
        direction->control_1 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        direction->control_2 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        direction->mode = mode;
        direction->initial = initial;
        direction->goal = goal;
        EnemyInitializeScalarInterpolation(reinterpret_cast<EnemyScalarInterpolationView *>(direction));
        orbit->end_time = end_time;
        orbit->control_1 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        orbit->control_2 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        orbit->mode = mode;
        orbit->initial = orbit_initial;
        orbit->goal = orbit_goal;
        EnemyInitializeScalarInterpolation(reinterpret_cast<EnemyScalarInterpolationView *>(orbit));
        motion->origin = motion->position;
        motion->flags |= 1;
        return 0;
    }
    case ECL_MOVE_ORIGIN_ABS:
    case ECL_MOVE_ORIGIN_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_ORIGIN_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        float x = ReadFloatArgument(0);
        float y = ReadFloatArgument(1);
        if (x > UNSET_MOTION_COMPONENT) motion->origin.x = x;
        if (y > UNSET_MOTION_COMPONENT) motion->origin.y = y;
    }
    // Fall through after changing the orbit origin. The selected position
    // is then copied from boss slot zero in both cases.
    case ECL_MOVE_TO_BOSS_ABS:
        reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion).position = reinterpret_cast<const Float3 &>(g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position);
        return 0;
    case ECL_MOVE_TO_BOSS_REL:
        reinterpret_cast<EnemyDispatchMotionView &>(baseMotion).position = reinterpret_cast<const Float3 &>(g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position);
        return 0;

    // Movement bounds, firing range and bounded random direction.
    case ECL_SET_BOUNDS:
        flags |= ENEMY_FLAG_LIMITED;
        movementBoundsCenterX = ReadFloatArgument(0);
        movementBoundsCenterY = ReadFloatArgument(1);
        movementBoundsSizeX = ReadFloatArgument(2);
        movementBoundsSizeY = ReadFloatArgument(3);
        return 0;
    case ECL_CLEAR_BOUNDS:
        flags &= ~(unsigned int)ENEMY_FLAG_LIMITED;
        return 0;
    case ECL_SET_SHOOT_RANGE: {
        float range = ReadFloatArgument(0);
        bulletProtectionRadiusSquared = range * range;
        break;
    }
    case ECL_MOVE_RAND_ABS:
    case ECL_MOVE_RAND_REL: {
        EnemyDispatchMotionView* motion = instruction->opcode == ECL_MOVE_RAND_ABS ? &reinterpret_cast<EnemyDispatchMotionView &>(offsetMotion) : &reinterpret_cast<EnemyDispatchMotionView &>(baseMotion);
        Float2Interp* interp = instruction->opcode == ECL_MOVE_RAND_ABS ? &reinterpret_cast<Float2Interp &>(scalarInterpolation0) : &reinterpret_cast<Float2Interp &>(scalarInterpolation1);
        float angle;
        if (movementBoundsCenterX - movementBoundsSizeX * 0.25f > reinterpret_cast<Float3 &>(worldMotion.position).x) {
            angle = g_RngView.GetRandomF32Signed() * (3.1415927f / 3.0f);
        } else if (movementBoundsSizeX * 0.25f + movementBoundsCenterX < reinterpret_cast<Float3 &>(worldMotion.position).x) {
            angle = EnemyWrapAngle((g_RngView.GetRandomF32Signed() * 3.1415927f) * 0.33333334f + 3.1415927f);
        } else if (reinterpret_cast<Float3 &>(worldMotion.position).x < reinterpret_cast<const Float3 &>(g_Player->drawPosition).x) {
            angle = g_RngView.GetRandomF32Signed() * (3.1415927f / 2.0f);
        } else {
            angle = EnemyWrapAngle((g_RngView.GetRandomF32Signed() * 3.1415927f) * 0.5f + 3.1415927f);
        }
        if (movementBoundsCenterY - movementBoundsSizeY * 0.25f > reinterpret_cast<Float3 &>(worldMotion.position).y) angle = fabs(angle);
        else if (movementBoundsSizeY * 0.25f + movementBoundsCenterY < reinterpret_cast<Float3 &>(worldMotion.position).y) angle = -fabs(angle);
        interp->mode = ReadDispatcherInt(this, 1);
        Float2 goal;
        if (interp->mode != 7) {
            goal.x = angle;
            goal.y = 0.0f;
        } else {
            goal.x = angle;
            goal.y = 0.0f;
        }
        Float2 initial;
        initial.x = angle;
        initial.y = ReadFloatArgument(2);
        interp->end_time = ReadDispatcherInt(this, 0);
        interp->control_1 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        interp->control_2 = reinterpret_cast<const Float2 &>(g_EnemyInterpolationBasis);
        interp->initial = initial;
        interp->goal = goal;
        EnemyInitializeScalarInterpolation(reinterpret_cast<EnemyScalarInterpolationView *>(interp));
        motion->flags &= ~1u;
        return 0;
    }

    // Life markers, enemy state and animation visibility.
    case ECL_SET_LIFE_MARKER:
    {
        const int color = ReadDispatcherInt(this, 2);
        const float value = ReadFloatArgument(1);
        const int full = lifeGaugeReference;
        const int marker = ReadDispatcherInt(this, 0);
        SetDispatcherGaugeMarker(g_GuiView, marker, full, value, color);
        return 0;
    }
    // -----------------------------------------------------------------------
    // Hitboxes, bounds, drops, boss/spell state & difficulty (0x140..0x170)
    // -----------------------------------------------------------------------
    case ECL_SET_HITBOX:
        damageHitbox.x = ReadFloatArgument(0);
        damageHitbox.y = ReadFloatArgument(1);
        return 0;
    case ECL_SET_CONTACT_BOX:
        playerCollisionHitbox.x = ReadFloatArgument(0);
        playerCollisionHitbox.y = ReadFloatArgument(1);
        return 0;
    case ECL_FLAGS_SET:
        flags |= ReadDispatcherInt(this, 0);
        if (flags & ENEMY_FLAG_INTANGIBLE) {
            for (int i = 0; i < 10; ++i) EnemyManagedVmIdAt(managedVmIds, i)->ClearFlag2();
        }
        return 0;
    case ECL_FLAGS_CLEAR:
        flags &= ~ReadDispatcherInt(this, 0);
        if (!(flags & ENEMY_FLAG_INTANGIBLE)) {
            for (int i = 0; i < 10; ++i) EnemyManagedVmIdAt(managedVmIds, i)->SetFlag2();
        }
        return 0;

    // Item drops: extra counts, spread, flush and main item.
    case ECL_DROP_CLEAR_EXTRA:
        memset(&itemDropRawWords[1], 0, 12 * sizeof(int));
        return 0;
    case ECL_DROP_ADD_EXTRA: {
        int kind = ReadDispatcherInt(this, 0);
        if (flags & ENEMY_FLAG_BOSS) {
            if (kind == 1) kind = 10;
            else if (kind == 4) kind = 11;
        }
        itemDropRawWords[kind] = ReadDispatcherInt(this, 1);
        return 0;
    }
    case ECL_DROP_SET_SPREAD:
        SetDispatcherDropSpread(itemDrops, ReadFloatArgument(0), ReadFloatArgument(1));
        return 0;
    case ECL_DROP_FLUSH:
        EnemyDropItemCounts(&itemDrops.mainItemType, &worldMotion.position);
        return 0;
    case ECL_DROP_SET_MAIN: {
        int kind = ReadDispatcherInt(this, 0);
        if (flags & ENEMY_FLAG_BOSS) {
            if (kind == 1) kind = 10;
            else if (kind == 4) kind = 11;
        }
        itemDropRawWords[0] = kind;
        return 0;
    }

    // Life, boss registration and timers.
    case ECL_SET_LIFE: {
        life = ReadDispatcherInt(this, 0);
        lifeGaugeReference = life;
        for (int i = 0; i < 4; ++i) {
            g_GuiView->bossGaugeValues[i].value = 0.0f;
            g_GuiView->bossGaugeValues[i].color = 0;
        }
        lifeToNextCallback = life;
        return 0;
    }
    case ECL_SET_BOSS: {
        int index = ReadDispatcherInt(this, 0);
        if (index < 0) {
            if (flags & ENEMY_FLAG_BOSS) g_EnemyManager->specialEnemySlots[managerSlot] = 0;
            flags &= ~(unsigned int)ENEMY_FLAG_BOSS;
        } else {
            flags |= ENEMY_FLAG_BOSS;
            g_EnemyManager->specialEnemySlots[index] = owner;
            managerSlot = index;
        }
        return 0;
    }
    case ECL_SET_INVULN_TIMER:
        damageReductionTimer.SetCurrent(ReadDispatcherInt(this, 0));
        return 0;
    case ECL_SET_CONTACT_TIMER:
        playerCollisionTimer.SetCurrent(ReadDispatcherInt(this, 0));
        return 0;
    case ECL_KILL_ALL_ENEMIES:
        EnemyKillAll(g_EnemyManager);
        return 0;

    // Select an integer or float operand by difficulty.
    case ECL_DIFF_SELECT_I:
        switch (g_EnemyDifficulty) {
        case 0:
            *ResolveIntArgument(0) = ReadDispatcherInt(this, 1);
            goto finish_instruction;
        case 1:
            *ResolveIntArgument(0) = ReadDispatcherInt(this, 2);
            goto finish_instruction;
        case 2:
            *ResolveIntArgument(0) = ReadDispatcherInt(this, 3);
            goto finish_instruction;
        case 3:
        case 4:
            *ResolveIntArgument(0) = ReadDispatcherInt(this, 4);
            goto finish_instruction;
        }
        break;
    case ECL_DIFF_SELECT_F:
        switch (g_EnemyDifficulty) {
        case 0:
            *ResolveFloatArgument(0) = ReadFloatArgument(1);
            goto finish_instruction;
        case 1:
            *ResolveFloatArgument(0) = ReadFloatArgument(2);
            goto finish_instruction;
        case 2:
            *ResolveFloatArgument(0) = ReadFloatArgument(3);
            goto finish_instruction;
        case 3:
            *ResolveFloatArgument(0) = ReadFloatArgument(4);
            goto finish_instruction;
        case 4:
            *ResolveFloatArgument(0) = ReadFloatArgument(4);
            goto finish_instruction;
        }
        break;

    // EnemyDispatchPatternView slots: reset, copy, fire and configure the shot.
    // -----------------------------------------------------------------------
    // Bullet patterns and lasers (0x190..0x1B4)
    // -----------------------------------------------------------------------
    case ECL_SHOOTER_RESET: {
        int index = ReadDispatcherInt(this, 0);
        memset(&reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index], 0, sizeof(EnemyDispatchPatternView));
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].angle_1 = 0.0f;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = 1;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = 1;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = 2.0f;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].sound = 7;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].sound_2 = 0x18;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].flags = 0x203;
        reinterpret_cast<Float3 *>(bulletPatternOffsets)[index].x = 0.0f;
        reinterpret_cast<Float3 *>(bulletPatternOffsets)[index].y = 0.0f;
        return 0;
    }
    case ECL_SHOOTER_COPY:
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[ReadDispatcherInt(this, 0)] = reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[ReadDispatcherInt(this, 1)];
        return 0;
    case ECL_SHOOTER_FIRE: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].position = Float3(reinterpret_cast<const Float3 *>(bulletPatternOffsets)[index], reinterpret_cast<const Float3 &>(worldMotion.position));
        if (bulletProtectionRadiusSquared != 0.0f) {
            if (!(bulletProtectionRadiusSquared <= DistanceSquared(reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].position, reinterpret_cast<const Float3 &>(g_Player->drawPosition)))) break;
        }
        SpawnBulletPattern(reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager),
            reinterpret_cast<BulletSpawnDescriptorView *>(&bulletPatterns[index]));
        return 0;
    }
    case ECL_SHOOTER_TYPE: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].type = ReadDispatcherInt(this, 1);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].color = ReadDispatcherInt(this, 2);
        return 0;
    }
    case ECL_SHOOTER_OFFSET: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<Float3 *>(bulletPatternOffsets)[index].x = ReadFloatArgument(1);
        reinterpret_cast<Float3 *>(bulletPatternOffsets)[index].y = ReadFloatArgument(2);
        return 0;
    }
    case ECL_SHOOTER_ANGLE: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].angle_1 = ReadFloatArgument(1);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].angle_2 = ReadFloatArgument(2);
        return 0;
    }
    case ECL_SHOOTER_SPEED: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(1);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(2);
        return 0;
    }
    case ECL_SHOOTER_COUNT: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 1);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 2);
        return 0;
    }

    // EnemyDispatchPatternView speed/count variants select operand pairs by difficulty or rank.
    case ECL_SHOOTER_SPEED_DIFF: {
        int index = ReadDispatcherInt(this, 0);
        float value;
        if (g_EnemyDifficulty == 0) value = ReadFloatArgument(1);
        else if (g_EnemyDifficulty == 1) value = ReadFloatArgument(2);
        else if (g_EnemyDifficulty == 2) value = ReadFloatArgument(3);
        else value = ReadFloatArgument(4);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = value;
        if (g_EnemyDifficulty == 0) value = ReadFloatArgument(5);
        else if (g_EnemyDifficulty == 1) value = ReadFloatArgument(6);
        else if (g_EnemyDifficulty == 2) value = ReadFloatArgument(7);
        else value = ReadFloatArgument(8);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = value;
        return 0;
    }
    case ECL_SHOOTER_COUNT_DIFF: {
        int index = ReadDispatcherInt(this, 0);
        int value;
        if (g_EnemyDifficulty == 0) value = ReadDispatcherInt(this, 1);
        else if (g_EnemyDifficulty == 1) value = ReadDispatcherInt(this, 2);
        else if (g_EnemyDifficulty == 2) value = ReadDispatcherInt(this, 3);
        else value = ReadDispatcherInt(this, 4);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = value;
        if (g_EnemyDifficulty == 0) value = ReadDispatcherInt(this, 5);
        else if (g_EnemyDifficulty == 1) value = ReadDispatcherInt(this, 6);
        else if (g_EnemyDifficulty == 2) value = ReadDispatcherInt(this, 7);
        else value = ReadDispatcherInt(this, 8);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = value;
        goto leave_dispatch;
    }
    case ECL_SHOOTER_SPEED_RANK3: {
        int index = ReadDispatcherInt(this, 0);
        if (g_EnemyChapterState.rank >= 0x200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(5);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(6);
            goto finish_instruction;
        }
        if (g_EnemyChapterState.rank >= -0x200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(3);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(4);
            goto leave_dispatch;
        } else {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(1);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(2);
            goto finish_instruction;
        }
    }
    case ECL_SHOOTER_SPEED_RANK5: {
        int index = ReadDispatcherInt(this, 0);
        if (g_EnemyChapterState.rank >= 600) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(9);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(10);
            return 0;
        }
        if (g_EnemyChapterState.rank >= 200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(7);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(8);
            return 0;
        }
        if (g_EnemyChapterState.rank >= -200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(5);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(6);
            return 0;
        }
        if (g_EnemyChapterState.rank >= -600) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(3);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(4);
            goto leave_dispatch;
        } else {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ReadFloatArgument(1);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ReadFloatArgument(2);
            goto finish_instruction;
        }
    }
    case ECL_SHOOTER_SPEED_RANK_LERP: {
        int index = ReadDispatcherInt(this, 0);
        float low_1 = ReadFloatArgument(1);
        float low_2 = ReadFloatArgument(2);
        float high_1 = ReadFloatArgument(3);
        float high_2 = ReadFloatArgument(4);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_1 = ((float)g_EnemyChapterState.rank + g_EnemyRankMidpoint) * (high_1 - low_1) / 2048.0f + low_1;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].speed_2 = ((float)g_EnemyChapterState.rank + g_EnemyRankMidpoint) * OperandRange(low_2, high_2) / 2048.0f + low_2;
        return 0;
    }
    case ECL_SHOOTER_COUNT_RANK3: {
        int index = ReadDispatcherInt(this, 0);
        if (g_EnemyChapterState.rank >= 0x200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 5);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 6);
            goto finish_instruction;
        }
        if (g_EnemyChapterState.rank >= -0x200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 3);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 4);
        } else {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 1);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 2);
        }
        goto finish_instruction;
    }
    case ECL_SHOOTER_COUNT_RANK5: {
        int index = ReadDispatcherInt(this, 0);
        if (g_EnemyChapterState.rank >= 600) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 9);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 10);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= 200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 7);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 8);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= -200) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 5);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 6);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= -600) {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 3);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 4);
        } else {
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = ReadDispatcherInt(this, 1);
            reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = ReadDispatcherInt(this, 2);
        }
        goto leave_dispatch;
    }
    case ECL_SHOOTER_COUNT_RANK_LERP: {
        int index = ReadDispatcherInt(this, 0);
        int low_1 = ReadDispatcherInt(this, 1);
        int low_2 = ReadDispatcherInt(this, 2);
        int high_1 = ReadDispatcherInt(this, 3);
        int high_2 = ReadDispatcherInt(this, 4);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_1 = (high_1 - low_1) * (g_EnemyChapterState.rank + 1024) / 2048 + low_1;
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].count_2 = (high_2 - low_2) * (g_EnemyChapterState.rank + 1024) / 2048 + low_2;
        goto leave_dispatch;
    }

    // Pattern aim, sound, and extended parameters.
    case ECL_SHOOTER_AIM: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].aim_mode = ReadDispatcherInt(this, 1);
        return 0;
    }
    case ECL_SHOOTER_SOUND: {
        int index = ReadDispatcherInt(this, 0);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].sound = ReadDispatcherInt(this, 1);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].sound_2 = ReadDispatcherInt(this, 2);
        return 0;
    }
    case ECL_SHOOTER_EX: {
        // This native index spans 22 records. The final slots overlap the
        // current pattern tail and the next pattern.
        int index = ReadDispatcherInt(this, 0);
        int entry = ReadDispatcherInt(this, 1);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].ex[entry].f = ReadDispatcherInt(this, 2);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].ex[entry].e = ReadDispatcherInt(this, 3);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].ex[entry].c = ReadDispatcherInt(this, 4);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].ex[entry].d = ReadDispatcherInt(this, 5);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].ex[entry].a = ReadFloatArgument(6);
        reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[index].ex[entry].b = ReadFloatArgument(7);
        return 0;
    }

    // Enemy script slots, manager commands and wait conditions.
    case ECL_RESET_TIMER:
        updateTimer.SetCurrent(0);
        return 0;
    case ECL_BIND_INTERRUPT:
        EnemyConfigureInterrupt(owner, ReadDispatcherInt(this, 0), ReadDispatcherInt(this, 1), ReadDispatcherInt(this, 2), instruction->interruptOperands.callbackName);
        return 0;
    case ECL_SET_INTERRUPT_CURSOR:
        EnemyNativeWordIndex(&owner->runtime.callbackThresholds[0], ReadDispatcherInt(this, 0))->timerCallbackName =
            instruction->timeoutOperands.callbackName;
        return 0;
    case ECL_CLEAR_BULLETS_TO_ITEMS:
        reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->CancelBulletsInArea(1);
        EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager), 1);
        return 0;
    case ECL_PLAY_SOUND:
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundCue(ReadDispatcherInt(this, 0), worldMotion.position.x);
        return 0;
    case ECL_SCREEN_EFFECT:
        StageCreateTransitionEffect(0x31, 1, ReadDispatcherInt(this, 0), ReadDispatcherInt(this, 1), ReadDispatcherInt(this, 2), 0);
        return 0;
    case ECL_STAGE_CLEAR_BONUS:
        EnemyReadDialog(g_GuiView, ReadDispatcherInt(this, 0));
        reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->CancelBulletsInArea(0);
        EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager), 0);
        EnemyKillAll(g_EnemyManager);
        return 0;
    case ECL_WAIT_DIALOGUE:
        if (g_GuiView->message != 0 && g_GuiView->message->inputCooldown == 0) return -1;
        break;
    case ECL_WAIT_BOSS_CLEAR:
        if (g_EnemyManager->specialEnemySlots[0] != 0) return -1;
        break;

    // Card commands. The start variants decode a packed name and may adjust
    // the card ID for the current difficulty.
    case ECL_SPELL_START:
    case ECL_SPELL_START_ALT:
    case ECL_SPELL_START_DIFF:
    case ECL_SPELL_START_DIFF_M1:
    case ECL_SPELL_START_DIFF_M2: {
        char name[128];
        const unsigned char *text = instruction->spellOperands.encryptedName;
        unsigned char key = 0x77;
        unsigned char step = 7;
        int length = instruction->spellOperands.encryptedNameLength;
        for (int i = 0; i < length; ++i) {
            name[i] = text[i] ^ key;
            key += step;
            step += 0x10;
        }
        int id = ReadDispatcherInt(this, 0);
        switch (instruction->opcode) {
        case ECL_SPELL_START_DIFF:
            id += g_EnemyDifficulty;
            break;
        case ECL_SPELL_START_DIFF_M1:
            id += g_EnemyDifficulty - 1;
            break;
        case ECL_SPELL_START_DIFF_M2:
            id += g_EnemyDifficulty - 2;
            break;
        }
        g_EnemyGameState->StartSpell(id, name, ReadDispatcherInt(this, 1), ReadDispatcherInt(this, 2));
        return 0;
    }
    case ECL_SPELL_END:
        EnemyEndSpell(g_EnemyGameState);
        return 0;
    case ECL_SPELL_MARK_FLAG:
        g_EnemyGameState->flags378C |= 8u;
        return 0;
    case ECL_SPELL_STOP_ANIM:
        EnemyEnableBombShield(g_EnemyGameState);
        return 0;
    case ECL_SET_FLAG_19: {
        reinterpret_cast<EnemyDispatchFlags *>(&flags)->bit19 = ReadDispatcherInt(this, 0);
        return 0;

    // Laser creation. Paired opcodes optionally copy shooter zero's extended
    // parameters; line and beam lasers have different parameter layouts.
    }
    case ECL_LASER_LINE:
    case ECL_LASER_LINE_EX: {
        LineLaserParams params;
        memset(&params, 0, sizeof(params));
        if (instruction->opcode == ECL_LASER_LINE_EX) memcpy(params.ex, reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[0].ex, sizeof(params.ex));
        params.position = Float3(reinterpret_cast<const Float3 *>(bulletPatternOffsets)[0], reinterpret_cast<const Float3 &>(worldMotion.position), Float3::LinePosition());
        params.type = ReadDispatcherInt(this, 0);
        params.color = ReadDispatcherInt(this, 1);
        params.f0c = ReadFloatArgument(2);
        params.f20 = ReadFloatArgument(3);
        params.f14 = ReadFloatArgument(4);
        params.f10 = ReadFloatArgument(5);
        params.f18 = ReadFloatArgument(6);
        params.f1c = ReadFloatArgument(7);
        params.flags |= 1;
        reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->Spawn(reinterpret_cast<EnemyLaserRequestScratch *>(&params), 0);
        return 0;
    }
    case ECL_LASER_BEAM:
    case ECL_LASER_BEAM_EX: {
        BeamLaserParams params;
        int* result = ResolveIntArgument(0);
        if (instruction->opcode == ECL_LASER_BEAM_EX) memcpy(params.ex, reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[0].ex, sizeof(params.ex));
        params.position = Float3(reinterpret_cast<const Float3 *>(bulletPatternOffsets)[0], reinterpret_cast<const Float3 &>(worldMotion.position));
        params.type = ReadDispatcherInt(this, 1);
        params.color = ReadDispatcherInt(this, 2);
        params.f18 = ReadFloatArgument(3);
        params.f24 = ReadFloatArgument(4);
        params.f20 = ReadFloatArgument(5);
        params.f30 = ReadDispatcherInt(this, 6);
        params.f34 = ReadDispatcherInt(this, 7);
        params.f38 = ReadDispatcherInt(this, 8);
        params.f3c = ReadDispatcherInt(this, 9);
        params.f28 = ReadFloatArgument(10);
        params.flags = ReadDispatcherInt(this, 11) | 2;
        int id = reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->Spawn(reinterpret_cast<EnemyLaserRequestScratch *>(&params), 1);
        if (result) *result = id;
        return 0;
    }
    case ECL_LASER_LINE_UNFLAGGED:
    case ECL_LASER_LINE_EX_UNFLAGGED: {
        LineLaserParams params;
        memset(&params, 0, sizeof(params));
        if (instruction->opcode == ECL_LASER_LINE_EX_UNFLAGGED) memcpy(params.ex, reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[0].ex, sizeof(params.ex));
        params.position = Float3(reinterpret_cast<const Float3 *>(bulletPatternOffsets)[0], reinterpret_cast<const Float3 &>(worldMotion.position));
        params.type = ReadIntArgument(0);
        params.color = ReadIntArgument(1);
        params.f0c = ReadFloatArgument(2);
        params.f20 = ReadFloatArgument(3);
        params.f14 = ReadFloatArgument(4);
        params.f10 = ReadFloatArgument(5);
        params.f18 = ReadFloatArgument(6);
        params.f1c = ReadFloatArgument(7);
        params.flags &= ~1u;
        reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->Spawn(reinterpret_cast<EnemyLaserRequestScratch *>(&params), 0);
        return 0;
    }
    case ECL_LASER_BEAM_UNFLAGGED:
    case ECL_LASER_BEAM_EX_UNFLAGGED: {
        BeamLaserParams params((BeamLaserParams::Raw()));
        EnemyInitializeLaserConfig(reinterpret_cast<EnemyLaserRequestScratch *>(&params));
        int* result = ResolveIntArgument(0);
        if (instruction->opcode == ECL_LASER_BEAM_EX_UNFLAGGED) memcpy(params.ex, reinterpret_cast<EnemyDispatchPatternView *>(bulletPatterns)[0].ex, sizeof(params.ex));
        params.position = Float3(reinterpret_cast<const Float3 *>(bulletPatternOffsets)[0], reinterpret_cast<const Float3 &>(worldMotion.position));
        params.type = ReadIntArgument(1);
        params.color = ReadIntArgument(2);
        params.f18 = ReadFloatArgument(3);
        params.f24 = ReadFloatArgument(4);
        params.f20 = ReadFloatArgument(5);
        params.f30 = ReadIntArgument(6);
        params.f34 = ReadIntArgument(7);
        params.f38 = ReadIntArgument(8);
        params.f3c = ReadIntArgument(9);
        params.f28 = ReadFloatArgument(10);
        params.flags = ReadIntArgument(11) & ~2u;
        int id = reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->Spawn(reinterpret_cast<EnemyLaserRequestScratch *>(&params), 1);
        if (result) *result = id;
        return 0;
    }

    // Updates to an existing laser and radius checks against bullets/lasers.
    case ECL_LASER_SET_POS: {
        Laser* laser = reinterpret_cast<Laser *>(EnemyFindLaser(reinterpret_cast<EnemyLaserManagerView *>(g_EnemyLaserManager), ReadIntArgument(0)));
        if (laser) laser->f24 = Float3(ReadFloatArgument(1), ReadFloatArgument(2), 0.0f);
        return 0;
    }
    case ECL_LASER_SET_VEC: {
        Laser* laser = reinterpret_cast<Laser *>(EnemyFindLaser(reinterpret_cast<EnemyLaserManagerView *>(g_EnemyLaserManager), ReadIntArgument(0)));
        if (laser) ApplyDispatcherLaserVector(laser, Float3(ReadFloatArgument(1), ReadFloatArgument(2), 0.0f));
        return 0;
    }
    case ECL_LASER_SET_F48: {
        Laser* laser = reinterpret_cast<Laser *>(EnemyFindLaser(reinterpret_cast<EnemyLaserManagerView *>(g_EnemyLaserManager), ReadIntArgument(0)));
        if (laser) laser->f48 = ReadFloatArgument(1);
        return 0;
    }
    case ECL_LASER_SET_F44: {
        Laser* laser = reinterpret_cast<Laser *>(EnemyFindLaser(reinterpret_cast<EnemyLaserManagerView *>(g_EnemyLaserManager), ReadIntArgument(0)));
        if (laser) laser->f44 = ReadFloatArgument(1);
        return 0;
    }
    case ECL_LASER_SET_F3C: {
        Laser* laser = reinterpret_cast<Laser *>(EnemyFindLaser(reinterpret_cast<EnemyLaserManagerView *>(g_EnemyLaserManager), ReadIntArgument(0)));
        if (laser) laser->f3c = ReadFloatArgument(1);
        return 0;
    }
    case ECL_LASER_SET_F440: {
        Laser* laser = reinterpret_cast<Laser *>(EnemyFindLaser(reinterpret_cast<EnemyLaserManagerView *>(g_EnemyLaserManager), ReadIntArgument(0)));
        if (laser) laser->f440 = ReadFloatArgument(1);
        return 0;
    }
    case ECL_CLEAR_RADIUS_ITEMS: {
        float radius = ReadFloatArgument(0);
        reinterpret_cast<EnemyCancelManagerView *>(g_EnemyBulletManager)->CancelBulletPattern(&worldMotion.position, radius, 1, 0);
        reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager)->ApplyPatternCancel(&worldMotion.position, radius, 1);
        return 0;
    }
    case ECL_CLEAR_RADIUS: {
        float radius = ReadFloatArgument(0);
        reinterpret_cast<EnemyCancelManagerView *>(g_EnemyBulletManager)->CancelBulletPattern(&worldMotion.position, radius, 0, 0);
        reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager)->ApplyPatternCancel(&worldMotion.position, radius, 0);
        return 0;
    }
    case ECL_SCORE_SET_MODE:
        EnemySetChapter(&g_EnemyChapterState, ReadIntArgument(0));
        return 0;

    // Select or interpolate a value by rank. The integer and float forms
    // deliberately keep their own thresholds and arithmetic.
    case ECL_RANK_SELECT_F3:
        if (g_EnemyChapterState.rank >= 0x200) *ResolveFloatArgument(0) = ReadFloatArgument(2);
        else if (g_EnemyChapterState.rank <= -0x200) *ResolveFloatArgument(0) = ReadFloatArgument(0);
        else *ResolveFloatArgument(0) = ReadFloatArgument(0);
        goto finish_instruction;
    case ECL_RANK_SELECT_F5:
        if (g_EnemyChapterState.rank >= 600) {
            *ResolveFloatArgument(0) = ReadFloatArgument(4);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= 200) {
            *ResolveFloatArgument(0) = ReadFloatArgument(3);
            break;
        }
        if (g_EnemyChapterState.rank >= -200) {
            *ResolveFloatArgument(0) = ReadFloatArgument(2);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= -400) {
            *ResolveFloatArgument(0) = ReadFloatArgument(1);
        } else {
            *ResolveFloatArgument(0) = ReadFloatArgument(0);
        }
        goto finish_instruction;
    case ECL_RANK_LERP_F: {
        float low = ReadFloatArgument(1);
        float high = ReadFloatArgument(2);
        *ResolveFloatArgument(0) = ((float)g_EnemyChapterState.rank + g_EnemyRankMidpoint) * OperandRange(low, high) / 2048.0f + low;
        return 0;
    }
    case ECL_RANK_SELECT_I3:
        if (g_EnemyChapterState.rank >= 0x200) *ResolveIntArgument(0) = ReadIntArgument(2);
        else if (g_EnemyChapterState.rank > -0x200) *ResolveIntArgument(0) = ReadIntArgument(0);
        else *ResolveIntArgument(0) = ReadIntArgument(0);
        goto leave_dispatch;
    case ECL_RANK_SELECT_I5:
        if (g_EnemyChapterState.rank >= 600) {
            *ResolveIntArgument(0) = ReadIntArgument(4);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= 200) {
            *ResolveIntArgument(0) = ReadIntArgument(3);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= -200) {
            *ResolveIntArgument(0) = ReadIntArgument(2);
            goto leave_dispatch;
        }
        if (g_EnemyChapterState.rank >= -400) {
            *ResolveIntArgument(0) = ReadIntArgument(1);
        } else {
            *ResolveIntArgument(0) = ReadIntArgument(0);
        }
        goto leave_dispatch;
    case ECL_RANK_LERP_I: {
        int low = ReadIntArgument(1);
        int high = ReadIntArgument(2);
        *ResolveIntArgument(0) = (high - low) * (g_EnemyChapterState.rank + 1024) / 2048 + low;
        return 0;
    }

    // Additional game state, player direction and VM timing.
    case ECL_FRONT_SET_STATE:
        g_GuiView->bossGaugeSegmentCount = ReadIntArgument(0);
        return 0;
    case ECL_LASER_MARK_ELIGIBLE:
        EnemyWaitForEffect(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager));
        return 0;
    case ECL_SET_COVERED_SCRIPT: {
        unsigned int value = static_cast<unsigned int>(ReadIntArgument(0));
        flags ^= ((value << 20) ^ flags) & 0x100000u;
        animationScriptPrimary = ReadIntArgument(1);
        flags &= ~0x200000u;
        animationScriptAlternate = animationBaseScript;
        return 0;
    }
    case ECL_SET_GAME_SPEED:
        g_AnmGameSpeed = ReadFloatArgument(0);
        return 0;
    case ECL_ANGLE_TO_PLAYER: {
        // Operand evaluation separates the two reads of the global player pointer.
        const Float3* player_for_x = &reinterpret_cast<const Float3 &>(g_Player->drawPosition);
        float origin_x = ReadFloatArgument(1);
        const Float3* player_for_y = &reinterpret_cast<const Float3 &>(g_Player->drawPosition);
        *ResolveFloatArgument(0) = (float)atan2(player_for_y->y - ReadFloatArgument(2), player_for_x->x - origin_x);
        goto finish_instruction;
    }
    case ECL_WAIT_BY_DIFF: {
        float wait;
        if (g_EnemyDifficulty == 0) wait = (float)ReadIntArgument(0);
        else if (g_EnemyDifficulty == 1) wait = (float)ReadIntArgument(1);
        else if (g_EnemyDifficulty == 2) wait = (float)ReadIntArgument(2);
        else wait = (float)ReadIntArgument(3);
        SubtractWait(owner, wait);
        break;
    }
    // Certain opcode tails share the switch exit rather than the final return.
    leave_dispatch:
        break;
    }
finish_instruction:
    return 0;
}
