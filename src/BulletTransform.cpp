#include "BulletManager.hpp"

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


// Velocity and direction opcodes take an angle operand: values at or below
// -990 keep the bullet's angle, values at or above 990 aim at the player.
static __forceinline float SelectTransformAngle(
    BulletRuntimeView *bullet, const float &value)
{
    if (value <= -990.0f)
        return bullet->angle;
    if (value >= 990.0f)
        return g_Player->AngleFromPosition(&bullet->position);
    return value;
}

// Target 0x00406D90-0x0040733D. Up to 18 transform records are executed per
// call. Numeric names are retained for the two TH10-only actions whose
// gameplay semantics remain unproven.
void BulletRuntimeView::AdvanceTransformProgram()
{
    while (this->transformIndex < 18)
    {
        const int index = this->transformIndex;
        BulletTransformRecordView *record = &this->transforms[index];
        const unsigned int kind = static_cast<unsigned int>(record->kind);

        if (kind == BULLET_TRANSFORM_NONE)
            return;
        if (record->allowWhileActive == 0 && this->activeTransformFlags != 0)
            return;
        if (kind == BULLET_TRANSFORM_SKIP)
        {
            this->transformIndex = index + 1;
            continue;
        }

        switch (static_cast<unsigned int>(record->kind))
        {
        case BULLET_TRANSFORM_DECELERATE:
            this->activeTransformFlags |= BULLET_TRANSFORM_DECELERATE;
            this->exStates[0].timer.SetCurrent(0);
            this->exStates[0].vector.z = 0.0f;
            break;

        case BULLET_TRANSFORM_ACCELERATE_VECTOR:
            this->activeTransformFlags |= BULLET_TRANSFORM_ACCELERATE_VECTOR;
            this->exStates[1].value0 = record->payload.float0;
            this->exStates[1].value1 =
                SelectTransformAngle(this, record->payload.float1);
            this->exStates[1].timer.SetCurrent(0);
            this->exStates[1].int0 = record->payload.int0;
            reinterpret_cast<AnmOpcodeVectorView *>(
                &this->exStates[1].vector)->FromAngleMagnitude(
                    this->exStates[1].value1, this->exStates[1].value0);
            if (this->transformIndex != 0 && this->transformSound >= 0)
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                    QueueSoundSample(this->transformSound, 0);
            break;

        case BULLET_TRANSFORM_ACCELERATE_POLAR:
            this->activeTransformFlags |= BULLET_TRANSFORM_ACCELERATE_POLAR;
            this->exStates[2].value0 = record->payload.float0;
            this->exStates[2].value1 = record->payload.float1;
            this->exStates[2].timer.SetCurrent(0);
            this->exStates[2].int0 = record->payload.int0;
            if (this->transformIndex != 0 && this->transformSound >= 0)
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                    QueueSoundSample(this->transformSound, 0);
            break;

        case BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE:
        case BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED:
        case BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE:
        {
            BulletExStateView &state = this->exStates[3];
            this->activeTransformFlags |= kind;
            state.value1 = SelectTransformAngle(this, record->payload.float0);
            {
                float *speedSlot = &state.value0;
                *speedSlot = record->payload.float1 > -999.0f
                    ? record->payload.float1 : this->speed;
            }
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
            state.int0 = record->payload.int0;
            state.int1 = record->payload.int1;
            state.int2 = 0;
            break;
        }

        case BULLET_TRANSFORM_BOUNCE_ALL_EDGES:
        case BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM:
        case BULLET_TRANSFORM_BOUNCE_HORIZONTAL_ONLY:
            this->activeTransformFlags |= kind;
            this->exStates[4].value0 = record->payload.float0;
            this->exStates[4].int1 = record->payload.int0;
            this->exStates[4].int0 = 0;
            break;

        case BULLET_TRANSFORM_SET_CANCEL_BEHAVIOR:
            this->cancelBehavior = record->payload.int0;
            break;

        case BULLET_TRANSFORM_SET_CULL_DELAY:
            this->offscreenCullDelayFrames = record->payload.int0;
            this->transformIndex = index + 1;
            continue;

        case BULLET_TRANSFORM_SET_SPRITE:
            AnmVmStartScript(
                reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->bulletAnm,
                &vm,
                g_BulletSpriteScriptBase[record->payload.int0] + record->payload.int1);
            break;

        case BULLET_TRANSFORM_WAIT:
            this->activeTransformFlags |= kind;
            this->exStates[5].timer.SetCurrent(record->payload.int0);
            break;

        case BULLET_TRANSFORM_DESPAWN:
            EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(this));
            break;

        case BULLET_TRANSFORM_PLAY_SOUND:
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundCue(record->payload.int0, this->position.x);
            break;

        case BULLET_TRANSFORM_WRAP_X:
            this->activeTransformFlags |= kind;
            this->exStates[6].timer.SetCurrent(record->payload.int0);
            break;

        case BULLET_TRANSFORM_WRAP_Y:
            this->activeTransformFlags |= kind;
            this->exStates[7].timer.SetCurrent(record->payload.int0);
            break;

        case BULLET_TRANSFORM_SPAWN_CHILD_PATTERN:
        {
            // A child pattern spans this record and the next one.
            BulletSpawnDescriptorView pattern;
            memset(&pattern, 0, sizeof(pattern));
            pattern.position = this->position;
            const unsigned int despawnAfter =
                static_cast<unsigned int>(record->payload.int0) & 0x80000000u;
            const int packed = record->payload.int0;
            pattern.aimMode = 0;
            pattern.bulletType = 0;
            pattern.color = reinterpret_cast<const signed char *>(
                &record->payload.int0)[1];
            pattern.speed1 = record->payload.float0;
            pattern.transformStartIndex = packed & 0xff;
            pattern.count1 = static_cast<short>(record->payload.int1);
            const unsigned int speed2Bits =
                *reinterpret_cast<const unsigned int *>(&record->payload.float1);
            ++record;
            this->transformIndex = index + 1;
            const short count2 = static_cast<short>(record->payload.int0);
            const unsigned int flags =
                static_cast<unsigned int>(record->payload.int1);
            pattern.count2 = count2;
            *reinterpret_cast<unsigned int *>(&pattern.speed2) = speed2Bits;
            pattern.transformFlags = flags;
            pattern.angle = record->payload.float0;
            pattern.angleStep = record->payload.float1;
            pattern.transformSound = -1;
            memcpy(pattern.transforms, this->transforms, sizeof(pattern.transforms));
            pattern.SpawnBulletPattern(
                reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager));
            ++this->transformIndex;
            if (despawnAfter == 0)
                continue;
            EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(this));
            break;
        }

        case BULLET_TRANSFORM_SET_OWNER_TAG:
            this->ownerTag = record->payload.int0;
            this->transformIndex = index + 1;
            continue;

        case BULLET_TRANSFORM_JUMP:
            this->transformIndex = record->payload.int0;
            continue;

        case BULLET_TRANSFORM_HOMING:
        {
            // The homing opcode value is also its active-transform flag bit.
            BulletHomingStateView &state =
                *reinterpret_cast<BulletHomingStateView *>(&this->exStates[8]);
            this->activeTransformFlags |= BULLET_TRANSFORM_HOMING;
            state.turnFactor = record->payload.float0;
            state.targetAngleOffset = record->payload.float1;
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
            break;
        }
        }
        ++this->transformIndex;
    }
}
