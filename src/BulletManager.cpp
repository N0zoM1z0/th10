#include "BulletManager.hpp"

#include <math.h>
#include <string.h>

// These seams are named only for target-observed behavior. Their original
// owners/private register ABIs are still open reconstruction work.
extern float EnemyAngleFromPlayer(Player *player, const PlayerFloat3 *position);
extern int g_BulletSpriteScriptBase[];
extern int g_BulletEffectMode[];
extern int g_BulletEffectScriptByColor[];
extern int g_BulletDrawBucket[];
extern float g_BulletCollisionSize[];
extern unsigned int EnemyCancelBulletRecord(unsigned char *bullet);
extern void *g_EnemyBulletManager;
extern void BulletQueueSoundAtPosition(int soundId, float positionX);


struct EnemySoundQueueView
{
    unsigned char unknown000[0x408];
    int cueValues[134];
    int activeSoundIds[12];
    int sampleCounts[12];
    int samples[12][128];

    void QueueSoundCue(int soundId, float positionX);
    void QueueSoundSample(int soundId, int sample);
};
extern unsigned char g_MainSoundOwner[];


struct BulletPositionView
{
    float x;
    float y;
    float z;

    __declspec(noinline) int IsOutsidePlayfield(float width, float height);
};


struct BulletWrapPositionView
{
    float x;
    float y;
    float z;

    __declspec(noinline) int IsOutsidePlayfield(float width, float height);
};

struct BulletUpdateGateView
{
    unsigned char unknown000[0x58];
    unsigned int flags58;
};

extern float g_BulletCullLeft;
extern float g_BulletCullRight;
extern float g_BulletCullTop;
extern float g_BulletCullBottom;
extern BulletUpdateGateView *g_BulletUpdateGate;

extern void BulletUpdateBoundaryBounce(BulletRuntimeView *bullet);
extern void BulletUpdateHorizontalWrap(BulletRuntimeView *bullet);
extern void BulletUpdateVerticalWrap(BulletRuntimeView *bullet);
extern void BulletUpdateState8(BulletRuntimeView *bullet);

struct AnmOpcodeVectorView
{
    float x;
    float y;
    float z;
    void FromAngleMagnitude(float angle, float magnitude);
};

__declspec(noinline) int BulletPositionView::IsOutsidePlayfield(
    float width, float height)
{
    float halfWidth = width * 0.5f;
    if (!(x + halfWidth <= g_BulletCullLeft) &&
        !(x - halfWidth >= g_BulletCullRight)) {
        float halfHeight = height * 0.5f;
        if (!(y + halfHeight <= g_BulletCullTop) &&
            !(y - halfHeight >= g_BulletCullBottom)) {
            return 0;
        }
    }
    return 1;
}

// Target 0x004061D0. Wrap transforms use the visible playfield top (Y=0)
// rather than the wider -64 culling margin used by 0x00406160.
__declspec(noinline) int BulletWrapPositionView::IsOutsidePlayfield(
    float width, float height)
{
    float halfWidth = width * 0.5f;
    if (!(x + halfWidth <= g_BulletCullLeft) &&
        !(x - halfWidth >= g_BulletCullRight)) {
        float halfHeight = height * 0.5f;
        if (!(y + halfHeight <= 0.0f) &&
            !(y - halfHeight >= g_BulletCullBottom)) {
            return 0;
        }
    }
    return 1;
}


// Target 0x00405BE0. The retail helper receives the bullet in private EAX.
void BulletRuntimeView::Deactivate()
{
    state = 0;
    stateTimer.SetCurrent(0);
    activeTimer.SetCurrent(0);
}

void BulletRuntimeView::UpdateDeceleration()
{
    BulletExStateView &state = exStates[0];
    if (state.timer.current <= 16) {
        reinterpret_cast<AnmOpcodeVectorView *>(&velocity)->
            FromAngleMagnitude(
                angle,
                (5.0f - state.timer.subframe * 0.3125f) + speed);
    }
    else {
        activeTransformFlags ^= BULLET_TRANSFORM_DECELERATE;
    }
    state.timer.previous = state.timer.current;
    if (*state.timer.scale > 0.99f && *state.timer.scale < 1.01f) {
        ++state.timer.current;
        state.timer.subframe += 1.0f;
    }
    else {
        state.timer.subframe += *state.timer.scale;
        state.timer.current = static_cast<int>(state.timer.subframe);
    }
}

void BulletRuntimeView::UpdateVectorAcceleration()
{
    BulletExStateView &state = exStates[1];
    if (state.timer.current < state.int0) {
        speed += g_AnmGameSpeed * state.value0;
        velocity.x += g_AnmGameSpeed * state.vector.x;
        velocity.y += g_AnmGameSpeed * state.vector.y;
        velocity.z += g_AnmGameSpeed * state.vector.z;
        if (fabsf(velocity.x) > 0.0001f ||
            fabsf(velocity.y) > 0.0001f) {
            angle = static_cast<float>(atan2(velocity.y, velocity.x));
        }
    }
    else {
        activeTransformFlags &= ~BULLET_TRANSFORM_ACCELERATE_VECTOR;
    }
    state.timer.previous = state.timer.current;
    if (*state.timer.scale > 0.99f && *state.timer.scale < 1.01f) {
        ++state.timer.current;
        state.timer.subframe += 1.0f;
    }
    else {
        state.timer.subframe += *state.timer.scale;
        state.timer.current = static_cast<int>(state.timer.subframe);
    }
}

void BulletRuntimeView::UpdatePolarAcceleration()
{
    BulletExStateView &state = exStates[2];
    if (state.timer.current < state.int0) {
        angle = AddNormalizeAngle(
            angle, g_AnmGameSpeed * state.value1);
        speed += g_AnmGameSpeed * state.value0;
        reinterpret_cast<AnmOpcodeVectorView *>(&velocity)->
            FromAngleMagnitude(angle, speed);
    }
    else {
        activeTransformFlags &= ~BULLET_TRANSFORM_ACCELERATE_POLAR;
    }
    state.timer.previous = state.timer.current;
    if (*state.timer.scale > 0.99f && *state.timer.scale < 1.01f) {
        ++state.timer.current;
        state.timer.subframe += 1.0f;
    }
    else {
        state.timer.subframe += *state.timer.scale;
        state.timer.current = static_cast<int>(state.timer.subframe);
    }
}

// Targets 0x00407780 and 0x004078E0. Both operate on transform state 3.
// Their retail calls use a private ESI receiver recovered from BulletUpdateRuntime.
void BulletRuntimeView::UpdateRelativeDirectionChange()
{
    BulletExStateView &state = exStates[3];
    float nextSpeed;

    if (state.timer.current >= state.int0) {
        if (transformSound >= 0) {
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundSample(transformSound, 0);
        }
        ++state.int2;
        if (state.int2 >= state.int1)
            activeTransformFlags &= ~BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE;

        angle += state.value1;
        speed = state.value0;
        nextSpeed = speed;
        state.timer.SetCurrent(0);
    }
    else {
        nextSpeed = speed;
        nextSpeed -=
            (nextSpeed * state.timer.subframe) /
                static_cast<float>(state.int0);
    }

    reinterpret_cast<AnmOpcodeVectorView *>(&velocity)->
        FromAngleMagnitude(angle, nextSpeed);
    state.timer.previous = state.timer.current;
    if (*state.timer.scale > 0.99f && *state.timer.scale < 1.01f) {
        ++state.timer.current;
        state.timer.subframe += 1.0f;
    }
    else {
        state.timer.subframe =
            *state.timer.scale + state.timer.subframe;
        state.timer.current = static_cast<int>(state.timer.subframe);
    }
}

void BulletRuntimeView::UpdateAbsoluteDirectionChange()
{
    BulletExStateView &state = exStates[3];
    float nextSpeed;

    if (state.timer.current >= state.int0) {
        if (transformSound >= 0) {
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundSample(transformSound, 0);
        }
        ++state.int2;
        if (state.int2 >= state.int1)
            activeTransformFlags &= ~BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE;

        int nextAngleBits =
            *reinterpret_cast<int *>(&state.value1);
        float newSpeed = state.value0;
        speed = newSpeed;
        *reinterpret_cast<int *>(&angle) = nextAngleBits;
        nextSpeed = newSpeed;
        state.timer.SetCurrent(0);
    }
    else {
        nextSpeed = speed;
        nextSpeed -=
            (nextSpeed * state.timer.subframe) /
                static_cast<float>(state.int0);
    }

    reinterpret_cast<AnmOpcodeVectorView *>(&velocity)->
        FromAngleMagnitude(angle, nextSpeed);
    state.timer.previous = state.timer.current;
    if (*state.timer.scale > 0.99f && *state.timer.scale < 1.01f) {
        ++state.timer.current;
        state.timer.subframe += 1.0f;
    }
    else {
        state.timer.subframe =
            *state.timer.scale + state.timer.subframe;
        state.timer.current = static_cast<int>(state.timer.subframe);
    }
}

// Target 0x00408660. Returns the shortest signed angular difference
// between two already-normalized angles. The sole target caller is the
// TH10-only transform-state helper at 0x00407EF0.
__declspec(noinline) float __stdcall BulletAngleDifference(
    float angle, float reference)
{
    float difference = angle - reference;
    if (difference > 3.1415927f)
        return angle - (reference + 6.2831855f);
    if (reference - angle > 3.1415927f)
        difference = angle - (reference - 6.2831855f);
    return difference;
}


void BulletRuntimeView::UpdateAimedDirectionChange()
{
    BulletExStateView &state = exStates[3];
    float nextSpeed;

    if (state.timer.current >= state.int0) {
        if (transformSound >= 0) {
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundSample(transformSound, 0);
        }
        ++state.int2;
        if (state.int2 >= state.int1)
            activeTransformFlags &= ~BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED;

        float dx = g_Player->drawPosition.x - position.x;
        float dy = g_Player->drawPosition.y - position.y;
        float aimedAngle;
        if (dy == 0.0f && dx == 0.0f)
            aimedAngle = 1.5707964f;
        else
            aimedAngle = static_cast<float>(atan2(dy, dx));

        angle = AddNormalizeAngle(aimedAngle, state.value1);
        speed = state.value0;
        nextSpeed = speed;
        state.timer.SetCurrent(0);
    }
    else {
        nextSpeed = speed;
        nextSpeed -=
            (nextSpeed * state.timer.subframe) /
                static_cast<float>(state.int0);
    }

    reinterpret_cast<AnmOpcodeVectorView *>(&velocity)->
        FromAngleMagnitude(angle, nextSpeed);
    state.timer.Tick();
}

// Target 0x00407BE0. Retail LTCG keeps the BulletRuntimeView owner in ESI.
void BulletRuntimeView::UpdateBoundaryBounce()
{
    BulletExStateView &state = exStates[4];

    if (position.x <= g_BulletCullLeft ||
        position.x >= g_BulletCullRight ||
        position.y <= 0.0f ||
        position.y >= g_BulletCullBottom) {
        int soundId = transformSound;
        int bounced = 0;

        if (soundId >= 0) {
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundSample(soundId, 0);
        }

        if (position.x < g_BulletCullLeft ||
            position.x >= g_BulletCullRight) {
            angle = AddNormalizeAngle(-angle - 3.1415927f, 0.0f);
            bounced = 1;
            if (position.x < g_BulletCullLeft)
                position.x = -384.0f - position.x;
            else
                position.x = 384.0f - position.x;
        }

        if ((activeTransformFlags & BULLET_TRANSFORM_BOUNCE_VARIANT) == 0 &&
            (position.y < 0.0f ||
             (position.y >= g_BulletCullBottom &&
              (activeTransformFlags &
               BULLET_TRANSFORM_BOUNCE_ALL_EDGES) != 0))) {
            bounced = 1;
            angle = -angle;
            if (position.y < 0.0f)
                position.y = -position.y;
            else
                position.y =
                    (g_BulletCullBottom - position.y) +
                    g_BulletCullBottom;
        }

        if (state.value0 > -990.0f)
            speed = state.value0;

        reinterpret_cast<AnmOpcodeVectorView *>(&velocity)->
            FromAngleMagnitude(angle, speed);

        if (bounced != 0)
            ++state.int0;
        if (state.int0 >= state.int1) {
            activeTransformFlags &=
                ~(BULLET_TRANSFORM_BOUNCE_ALL_EDGES |
                  BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM |
                  BULLET_TRANSFORM_BOUNCE_VARIANT);
        }
    }
}


// Target 0x00407DA0. Retail LTCG keeps the BulletRuntimeView owner in EBX.
void BulletRuntimeView::UpdateHorizontalWrap()
{
    AnmSpriteView *sprite = vm.loadedSprite;
    if (reinterpret_cast<BulletWrapPositionView *>(&position)->
            IsOutsidePlayfield(sprite->width, sprite->height) == 0)
        return;

    if (position.x < g_BulletCullLeft) {
        position.x += sprite->width + 384.0f;
    }
    else if (position.x > g_BulletCullRight) {
        position.x -= sprite->width + 384.0f;
    }
    else {
        goto checkTimer;
    }

    exStates[6].timer.Add(-1.0f);
    if (transformSound >= 0) {
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
            QueueSoundSample(transformSound, 0);
    }

checkTimer:
    if (exStates[6].timer.current <= 0)
        activeTransformFlags ^= BULLET_TRANSFORM_WRAP_X;
}


// Target 0x00407E40. Retail LTCG keeps the BulletRuntimeView owner in EBX.
void BulletRuntimeView::UpdateVerticalWrap()
{
    AnmSpriteView *sprite = vm.loadedSprite;
    if (reinterpret_cast<BulletWrapPositionView *>(&position)->
            IsOutsidePlayfield(sprite->width, sprite->height) == 0)
        return;

    if (position.y < 0.0) {
        position.y += sprite->height + g_BulletCullBottom;
    }
    else if (position.y > g_BulletCullBottom) {
        position.y -= sprite->height + g_BulletCullBottom;
    }
    else {
        goto checkTimer;
    }

    exStates[7].timer.Add(-1.0f);
    if (transformSound >= 0) {
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
            QueueSoundSample(transformSound, 0);
    }

checkTimer:
    if (exStates[7].timer.current <= 0)
        activeTransformFlags ^= BULLET_TRANSFORM_WRAP_Y;
}


// Target 0x00407EF0. Retail LTCG keeps the BulletRuntimeView owner in ESI.
void BulletRuntimeView::UpdateState8()
{
    BulletExStateView &state = exStates[8];

    if (state.timer.current >= state.int0) {
        activeTransformFlags &= ~BULLET_TRANSFORM_STATE_8;
        goto tickTimer;
    }

    {
        float dx = g_Player->drawPosition.x - position.x;
        float dy = g_Player->drawPosition.y - position.y;
        float aimedAngle;
        if (dy == 0.0f && dx == 0.0f)
            aimedAngle = 1.5707964f;
        else
            aimedAngle = static_cast<float>(atan2(dy, dx));

        angle = AddNormalizeAngle(
            angle,
            BulletAngleDifference(
                AddNormalizeAngle(state.value1, aimedAngle), angle) *
                state.value0 * g_AnmGameSpeed);
        reinterpret_cast<AnmOpcodeVectorView *>(&velocity)->
            FromAngleMagnitude(angle, speed);
    }

tickTimer:
    state.timer.previous = state.timer.current;
    if (*state.timer.scale > 0.99f && *state.timer.scale < 1.01f) {
        ++state.timer.current;
        state.timer.subframe += 1.0f;
    }
    else {
        state.timer.subframe =
            *state.timer.scale + state.timer.subframe;
        state.timer.current = static_cast<int>(state.timer.subframe);
    }
}


// Target 0x004065C0. Retail LTCG keeps the manager in private ESI.
int BulletManagerView::UpdateBullets()
{
    activeBulletCount = 0;
    drawBucketHeads[5] = 0;
    drawBucketHeads[4] = 0;
    drawBucketHeads[3] = 0;
    drawBucketHeads[2] = 0;
    drawBucketHeads[1] = 0;
    drawBucketHeads[0] = 0;
    drawBucketTails[5] = 0;
    drawBucketTails[4] = 0;
    drawBucketTails[3] = 0;
    drawBucketTails[2] = 0;
    drawBucketTails[1] = 0;
    drawBucketTails[0] = 0;

    BulletRuntimeView *bullet = &bullets[0];
    for (int remaining = 2000; remaining != 0; --remaining, ++bullet) {
        if (bullet->state == 0)
            continue;

        BulletUpdateGateView *updateGate = g_BulletUpdateGate;
        if (updateGate == 0 ||
            (updateGate->flags58 & 0x00000002u) == 0 ||
            (updateGate->flags58 & 0x00000400u) == 0) {
            if (BulletUpdateRuntime(bullet) != 0)
                continue;
        }

        int bucket = bullet->drawBucketIndex;
        if (drawBucketHeads[bucket] != 0)
            drawBucketTails[bucket]->nextInDrawBucket = bullet;
        else
            drawBucketHeads[bucket] = bullet;
        drawBucketTails[bullet->drawBucketIndex] = bullet;
        bullet->nextInDrawBucket = 0;
        ++activeBulletCount;
        bullet->stateTimer.Tick();
    }
    return 1;
}

// Target 0x00406770. Registered as the BulletManager calc-chain callback.
// The chain passes the manager owner in ECX.
int __fastcall BulletManagerUpdateCallback(BulletManagerView *manager)
{
    BulletUpdateGateView *updateGate = g_BulletUpdateGate;
    if (updateGate != 0) {
        unsigned int flags = updateGate->flags58;
        if ((((flags >> 2) | flags) & 1u) != 0)
            return 1;
    }
    return manager->UpdateBullets();
}



#pragma var_order(speed, scanned, bullet, angle, transformFlags, this)
int BulletManagerView::SpawnSingleBullet(
    BulletSpawnDescriptorView *descriptor,
    int index1, int index2, float angleToPlayer)
{
    float speed;
    int scanned;
    BulletRuntimeView *bullet;
    float angle;
    unsigned int transformFlags;

    scanned = 0;
    bullet = bulletCursor;
    for (scanned = 0; scanned < 2000; ++scanned)
    {
        if (bullet->state == 0)
            break;
        ++bullet;
        if (bullet->state == 5)
            bullet = &bullets[0];
    }
    if (scanned >= 2000)
        return 1;

    angle = 0.0f;
    if (descriptor->count2 > 1)
    {
        speed = descriptor->speed1 -
            (descriptor->speed1 - descriptor->speed2) *
                static_cast<float>(index2) /
                static_cast<float>(descriptor->count2);
    }
    else
    {
        speed = descriptor->speed1;
    }

    switch (descriptor->aimMode)
    {
    case 0:
    case 1:
        if ((descriptor->count1 & 1) != 0)
            angle += static_cast<float>((index1 + 1) / 2) *
                descriptor->angleStep;
        else
            angle += static_cast<float>(index1 / 2) *
                descriptor->angleStep + descriptor->angleStep * 0.5f;
        if ((index1 & 1) != 0)
            angle *= -1.0f;
        if (descriptor->aimMode == 0)
            angle += angleToPlayer;
        angle += descriptor->angle;
        break;

    case 2:
        angle = angleToPlayer;
    case 3:
        angle += static_cast<float>(index1) * 6.2831855f /
            static_cast<float>(descriptor->count1);
        angle += static_cast<float>(index2) * descriptor->angleStep +
            descriptor->angle;
        break;

    case 4:
        angle = angleToPlayer;
    case 5:
        angle += 3.1415927f / static_cast<float>(descriptor->count1);
        angle += static_cast<float>(index1) * 6.2831855f /
            static_cast<float>(descriptor->count1);
        angle += descriptor->angle;
        break;

    case 6:
        angle = g_RngView.GetRandomF32InRange(
            descriptor->angle - descriptor->angleStep) +
            descriptor->angleStep;
        break;

    case 7:
        speed = g_RngView.GetRandomF32InRange(
            descriptor->speed1 - descriptor->speed2) +
            descriptor->speed2;
        angle += static_cast<float>(index1) * 6.2831855f /
            static_cast<float>(descriptor->count1);
        angle += static_cast<float>(index2) * descriptor->angleStep +
            descriptor->angle;
        break;

    case 8:
        angle = g_RngView.GetRandomF32InRange(
            descriptor->angle - descriptor->angleStep) +
            descriptor->angleStep;
        speed = g_RngView.GetRandomF32InRange(
            descriptor->speed1 - descriptor->speed2) +
            descriptor->speed2;
        break;

    default:
        break;
    }

    bullet->flags |= 1;
    bullet->state = 1;
    bullet->stateTimer.SetCurrent(0);
    bullet->activeTimer.SetCurrent(0);
    bullet->speed = speed;
    bullet->angle = AddNormalizeAngle(angle, 0.0f);
    bullet->position = descriptor->position;
    bullet->position.z = 0.1f;
    reinterpret_cast<AnmFloat3View *>(&bullet->velocity)->
        FromAngleMagnitude(angle, speed);

    bullet->activeTransformFlags = descriptor->transformFlags;
    bullet->color = descriptor->color;
    bullet->bulletType = descriptor->bulletType;
    bullet->flags = (bullet->flags & ~0x0cu) | 2u;
    bullet->value454 = 0;

    bullet->vm.InitializeForLoadedScript(
        bulletAnm,
        g_BulletSpriteScriptBase[descriptor->bulletType] + descriptor->color);

    switch (g_BulletEffectMode[descriptor->bulletType])
    {
    case 0:
        bullet->effectScript = descriptor->color * 2 + 0x11;
        break;
    case 1:
        bullet->effectScript = g_BulletEffectScriptByColor[descriptor->color];
        break;
    case 2:
        bullet->effectScript = -1;
        break;
    case 3:
        bullet->effectScript = 0x1d;
        break;
    case 4:
        bullet->effectScript = 0x13;
        break;
    default:
        break;
    }

    bullet->drawBucketIndex = g_BulletDrawBucket[descriptor->bulletType];
    bullet->transformSound = descriptor->transformSound;
    bullet->offscreenCullDelayFrames = 10;
    bullet->collisionWidth = g_BulletCollisionSize[descriptor->bulletType];
    bullet->collisionHeight = bullet->collisionWidth;

    transformFlags = descriptor->transformFlags;
    if ((transformFlags & 2u) != 0)
    {
        bullet->vm.pendingInterrupt = 7;
        bullet->state = 2;
    }
    else if ((transformFlags & 4u) != 0)
    {
        bullet->vm.pendingInterrupt = 8;
        bullet->state = 2;
    }
    else if ((transformFlags & 8u) != 0)
    {
        bullet->vm.pendingInterrupt = 9;
        bullet->state = 2;
    }
    else
    {
        bullet->vm.pendingInterrupt = 2;
    }

    if (bullet->state == 2)
    {
        bullet->position.x -= bullet->velocity.x * 4.0f;
        bullet->position.y -= bullet->velocity.y * 4.0f;
        bullet->position.z -= bullet->velocity.z * 4.0f;
    }

    memcpy(
        bullet->transforms, descriptor->transforms,
        sizeof(descriptor->transforms));
    bullet->transformFlags = descriptor->transformFlags;
    bullet->activeTransformFlags = 0;
    bullet->transformIndex = descriptor->transformStartIndex;
    bullet->AdvanceTransformProgram();
    AnmRenderManagerView::ExecuteScript(&bullet->vm);

    ++bullet;
    if (bullet->state == 5)
        bulletCursor = &bullets[0];
    else
        bulletCursor = bullet;

    return 0;
}


int BulletManagerView::SpawnBulletPattern(BulletSpawnDescriptorView *descriptor)
{
    int index2;
    int index1;
    float angleToPlayer;
    float dx = g_Player->drawPosition.x - descriptor->position.x;
    float dy = g_Player->drawPosition.y - descriptor->position.y;

    if (dy == 0.0f && dx == 0.0f)
        angleToPlayer = 1.5707964f;
    else
        angleToPlayer = (float)atan2(dy, dx);

    for (index2 = 0; index2 < descriptor->count2; ++index2)
    {
        for (index1 = 0; index1 < descriptor->count1; ++index1)
        {
            if (SpawnSingleBullet(
                    descriptor, index1, index2, angleToPlayer) != 0)
                goto doneSpawning;
        }
    }

doneSpawning:
    if ((descriptor->transformFlags & 0x200u) != 0)
    {
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
            QueueSoundCue(descriptor->spawnSound, descriptor->position.x);
    }
    return 0;
}
