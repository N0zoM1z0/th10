#include "BulletManager.hpp"

extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)
#include "EnemySoundQueue.hpp"

#include <string.h>

extern int g_BulletSpriteScriptBase[];
extern unsigned int EnemyCancelBulletRecord(unsigned char *bullet);
extern void *g_EnemyBulletManager;
extern unsigned char g_MainSoundOwner[];

// The transform program uses the native opcode-vector primitive, distinct
// from the general ANM float-vector helper despite identical storage.
struct AnmOpcodeVectorView
{
    float x;
    float y;
    float z;
    void FromAngleMagnitude(float angle, float magnitude);
};

// Target 0x00406D90-0x0040733D. TH08/TH095 were consulted only after the
// TH10-local record stride, 18-entry bound, field offsets and branch values
// were recovered from the target. Numeric names are retained for the two
// TH10-only actions whose gameplay semantics remain unproven.
void BulletRuntimeView::AdvanceTransformProgram()
{
    unsigned int kind;
    BulletTransformRecordView *record;
    int index;
    register int scaledIndex;
    volatile int *cursor;
    unsigned int packedPattern;
    float selectedAngle;
    BulletSpawnDescriptorView pattern;

    // The retail loop observes the cursor for the outer bound, then reads it
    // again before forming the record address. Keep those memory operations
    // distinct; several opcode paths also update the cursor before the tail.
    cursor = &this->transformIndex;
    if (*cursor >= 18)
        return;

    do
    {
        index = *cursor;
        scaledIndex = index * 3;
        kind = this->transforms[index].kind;
        record = this->transforms + index;

        if (kind == BULLET_TRANSFORM_NONE)
            return;
        if (record->allowWhileActive == 0 && this->activeTransformFlags != 0)
            return;

        if (kind == BULLET_TRANSFORM_SKIP)
        {
            *cursor = index + 1;
            goto loopTail;
        }

        if (kind <= 0x4000u)
        {
            if (kind != BULLET_TRANSFORM_SET_SPRITE)
            {
                if (kind <= 0x100u)
                {
                    if (kind == BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE)
                        goto directionChange;

                    switch (kind)
                    {
                    case BULLET_TRANSFORM_DECELERATE:
                        this->activeTransformFlags |= BULLET_TRANSFORM_DECELERATE;
                        this->exStates[0].timer.SetCurrent(0);
                        this->exStates[0].vector.z = 0.0f;
                        break;

                    case BULLET_TRANSFORM_ACCELERATE_VECTOR:
                        this->activeTransformFlags |= BULLET_TRANSFORM_ACCELERATE_VECTOR;
                        this->exStates[1].value0 = record->payload.float0;
                        if (record->payload.float1 <= -990.0f)
                            selectedAngle = this->angle;
                        else if (record->payload.float1 >= 990.0f)
                            selectedAngle =
                                g_Player->AngleFromPosition(&this->position);
                        else
                            selectedAngle = record->payload.float1;
                        this->exStates[1].value1 = static_cast<float>(selectedAngle);
                        this->exStates[1].timer.SetCurrent(0);
                        this->exStates[1].int0 = record->payload.int0;
                        reinterpret_cast<AnmOpcodeVectorView *>(
                            &this->exStates[1].vector)->FromAngleMagnitude(
                                this->exStates[1].value1,
                                this->exStates[1].value0);
                        if (*cursor != 0 && this->transformSound >= 0)
                            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                                QueueSoundSample(this->transformSound, 0);
                        break;

                    case BULLET_TRANSFORM_ACCELERATE_POLAR:
                        this->activeTransformFlags |= BULLET_TRANSFORM_ACCELERATE_POLAR;
                        this->exStates[2].value0 = record->payload.float0;
                        this->exStates[2].value1 = record->payload.float1;
                        this->exStates[2].timer.SetCurrent(0);
                        this->exStates[2].int0 = record->payload.int0;
                        if (*cursor != 0 && this->transformSound >= 0)
                            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                                QueueSoundSample(this->transformSound, 0);
                        break;

                    case BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE:
                    case BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED:
                        goto directionChange;
                    }
                }
                else if (kind <= 0x1000u)
                {
                    if (kind != BULLET_TRANSFORM_SET_CANCEL_BEHAVIOR)
                    {
                        if (kind == BULLET_TRANSFORM_BOUNCE_ALL_EDGES ||
                            kind == BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM)
                            goto bounce;
                    }
                    else
                    {
                        this->cancelBehavior = record->payload.int0;
                    }
                }
                else if (kind == BULLET_TRANSFORM_SET_CULL_DELAY)
                {
                    this->offscreenCullDelayFrames = record->payload.int0;
                    *cursor = index + 1;
                    goto loopTail;
                }
            }
            else
            {
                const int scriptIndex =
                    g_BulletSpriteScriptBase[record->payload.int0] +
                    record->payload.int1;
                _ReadWriteBarrier();
                BulletManagerView *manager =
                    reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager);
                AnmLoadedView *resource = manager->bulletAnm;
                vm.StartScript(scriptIndex, resource, manager);
            }
        }
        else if (kind <= 0x400000u)
        {
            if (kind != BULLET_TRANSFORM_SPAWN_CHILD_PATTERN)
            {
                if (kind <= 0x20000u)
                {
                    if (kind != BULLET_TRANSFORM_PLAY_SOUND)
                    {
                        if (kind != BULLET_TRANSFORM_WAIT)
                        {
                            if (kind == BULLET_TRANSFORM_DESPAWN)
                                EnemyCancelBulletRecord(
                                    reinterpret_cast<unsigned char *>(this));
                        }
                        else
                        {
                            this->activeTransformFlags |= kind;
                            this->exStates[5].timer.SetCurrent(record->payload.int0);
                        }
                    }
                    else
                    {
                        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                            QueueSoundCue(record->payload.int0, this->position.x);
                    }
                }
                else if (kind != BULLET_TRANSFORM_WRAP_X)
                {
                    if (kind == BULLET_TRANSFORM_WRAP_Y)
                    {
                        this->activeTransformFlags |= kind;
                        this->exStates[7].timer.SetCurrent(record->payload.int0);
                    }
                }
                else
                {
                    this->activeTransformFlags |= kind;
                    this->exStates[6].timer.SetCurrent(record->payload.int0);
                }
            }
            else
            {
                memset(&pattern, 0, sizeof(pattern));
                pattern.position = this->position;
                packedPattern = static_cast<unsigned int>(record->payload.int0);
                pattern.aimMode = 0;
                pattern.bulletType = 0;
                pattern.color = reinterpret_cast<const signed char *>(
                    &record->payload.int0)[1];
                pattern.transformStartIndex = packedPattern & 0xff;
                pattern.count1 = static_cast<short>(record->payload.int1);
                pattern.speed1 = record->payload.float0;
                pattern.speed2 = record->payload.float1;
                packedPattern &= 0x80000000u;

                record = reinterpret_cast<BulletTransformRecordView *>(
                    reinterpret_cast<unsigned int>(record) +
                    sizeof(BulletTransformRecordView));
                *cursor = index + 1;
                pattern.count2 = record->payload.int0LowSigned16;
                pattern.transformFlags = static_cast<unsigned int>(record->payload.int1);
                pattern.angleStep = record->payload.float1;
                pattern.angle = record->payload.float0;
                pattern.transformSound = -1;
                BulletManagerView *manager =
                    reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager);
                _ReadWriteBarrier();
                memcpy(pattern.transforms, this->transforms, sizeof(pattern.transforms));

                pattern.SpawnBulletPattern(manager);
                ++*cursor;
                if (packedPattern == 0)
                    goto loopTail;
                EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(this));
                goto advanceRecord;
            }
        }
        else if (kind <= 0x4000000u)
        {
            if (kind != BULLET_TRANSFORM_HOMING)
            {
                if (kind != BULLET_TRANSFORM_SET_OWNER_TAG)
                {
                    if (kind != BULLET_TRANSFORM_JUMP)
                        goto advanceRecord;
                    index = record->payload.int0;
                    goto storeIndex;
                }

                this->ownerTag = record->payload.int0;
                *cursor = index + 1;
                goto loopTail;
            }

        homing:
            // The homing opcode value is also its active-transform flag bit.
            this->activeTransformFlags |= kind;
            BulletHomingStateView &state =
                *reinterpret_cast<BulletHomingStateView *>(&this->exStates[8]);
            state.turnFactor = record->payload.float0;
            state.targetAngleOffset = record->payload.float1;
            _ReadWriteBarrier();
            if ((state.timer.flags & 1u) == 0)
            {
                state.timer.current = 0;
                state.timer.previous = -999999;
                state.timer.subframe = 0.0f;
                state.timer.scale = &g_AnmGameSpeed;
                state.timer.flags |= 1u;
            }
            state.timer.current = 0;
            state.timer.subframe = 0.0f;
            state.timer.previous = -1;
            state.durationFrames = record->payload.int0;
        }
        else if (kind == BULLET_TRANSFORM_BOUNCE_HORIZONTAL_ONLY)
        {
            goto bounce;
        }
        goto advanceRecord;

directionChange:
        this->activeTransformFlags |= kind;
        if (record->payload.float0 <= -990.0f)
            selectedAngle = this->angle;
        else if (record->payload.float0 >= 990.0f)
            selectedAngle =
                g_Player->AngleFromPosition(&this->position);
        else
            selectedAngle = record->payload.float0;
        this->exStates[3].value1 = static_cast<float>(selectedAngle);
        this->exStates[3].value0 = record->payload.float1 > -999.0f ? record->payload.float1 : this->speed;
        this->exStates[3].timer.SetCurrent(0);
        this->exStates[3].int0 = record->payload.int0;
        this->exStates[3].int1 = record->payload.int1;
        this->exStates[3].int2 = 0;
        goto advanceRecord;

bounce:
        this->activeTransformFlags |= kind;
        this->exStates[4].value0 = record->payload.float0;
        this->exStates[4].int1 = record->payload.int0;
        this->exStates[4].int0 = 0;

advanceRecord:
        index = *cursor;
        ++index;
storeIndex:
        *cursor = index;
loopTail:
        ;
    } while (*cursor < 18);
}
