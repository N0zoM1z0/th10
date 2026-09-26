#include "BulletManager.hpp"

#include <string.h>

extern float EnemyAngleFromPlayer(Player *player, const PlayerFloat3 *position);
extern int g_BulletSpriteScriptBase[];
extern unsigned int EnemyCancelBulletRecord(unsigned char *bullet);
extern void *g_EnemyBulletManager;
extern void BulletQueueSoundAtPosition(int soundId, float positionX);

static float BulletResolveTransformAngle(
    BulletRuntimeView *bullet, float requestedAngle)
{
    // TH10 uses two sentinels: <= -990 means keep the current bullet angle;
    // >= 990 means aim at the player; values in between are literal angles.
    if (requestedAngle <= -990.0f)
        return bullet->angle;
    if (requestedAngle < 990.0f)
        return requestedAngle;
    return EnemyAngleFromPlayer(g_Player, &bullet->position);
}

// Target 0x00406D90-0x0040733D. TH08/TH095 were consulted only after the
// TH10-local record stride, 18-entry bound, field offsets and branch values
// were recovered from the target. Numeric names are retained for the two
// TH10-only actions whose gameplay semantics remain unproven.
void BulletRuntimeView::AdvanceTransformProgram()
{
    BulletTransformRecordView *record;
    unsigned int kind;

nextRecord:
    if (this->transformIndex >= 18)
        return;

    record = &this->transforms[this->transformIndex];
    kind = record->kind;

    if (kind == BULLET_TRANSFORM_NONE)
        return;
    if (record->allowWhileActive == 0 && this->activeTransformFlags != 0)
        return;

    if (kind == BULLET_TRANSFORM_SKIP)
    {
        ++this->transformIndex;
        goto nextRecord;
    }

    switch (kind)
    {
    case BULLET_TRANSFORM_DECELERATE:
        this->activeTransformFlags |= BULLET_TRANSFORM_DECELERATE;
        this->exStates[0].timer.SetCurrent(0);
        *reinterpret_cast<int *>(&this->exStates[0].vector.z) = 0;
        break;

    case BULLET_TRANSFORM_ACCELERATE_VECTOR:
        this->activeTransformFlags |= BULLET_TRANSFORM_ACCELERATE_VECTOR;
        this->exStates[1].value0 = record->payload.float0;
        this->exStates[1].value1 =
            BulletResolveTransformAngle(this, record->payload.float1);
        this->exStates[1].timer.SetCurrent(0);
        this->exStates[1].int0 = record->payload.int0;
        this->exStates[1].vector.FromAngleMagnitude(
            this->exStates[1].value1, this->exStates[1].value0);
        if (this->transformIndex != 0 && this->transformSound >= 0)
            BulletQueueSoundAtPosition(this->transformSound, 0.0f);
        break;

    case BULLET_TRANSFORM_ACCELERATE_POLAR:
        this->activeTransformFlags |= BULLET_TRANSFORM_ACCELERATE_POLAR;
        this->exStates[2].value0 = record->payload.float0;
        this->exStates[2].value1 = record->payload.float1;
        this->exStates[2].timer.SetCurrent(0);
        this->exStates[2].int0 = record->payload.int0;
        if (this->transformIndex != 0 && this->transformSound >= 0)
            BulletQueueSoundAtPosition(this->transformSound, 0.0f);
        break;

    case BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE:
    case BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED:
    case BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE:
        this->activeTransformFlags |= kind;
        this->exStates[3].value1 =
            BulletResolveTransformAngle(this, record->payload.float0);
        this->exStates[3].value0 =
            record->payload.float1 > -999.0f
                ? record->payload.float1
                : this->speed;
        // This timer reset is inlined in the target for the direction-change
        // state rather than emitted through 0x00405410.
        this->exStates[3].timer.SetCurrent(0);
        this->exStates[3].int0 = record->payload.int0;
        this->exStates[3].int1 = record->payload.int1;
        this->exStates[3].int2 = 0;
        break;

    case BULLET_TRANSFORM_BOUNCE_ALL_EDGES:
    case BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM:
    case BULLET_TRANSFORM_BOUNCE_VARIANT:
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
        ++this->transformIndex;
        goto nextRecord;

    case BULLET_TRANSFORM_SET_SPRITE:
        {
            int scriptIndex =
                g_BulletSpriteScriptBase[record->payload.int0] +
                record->payload.int1;
            vm.InitializeForLoadedScript(
                reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->bulletAnm,
                scriptIndex);
        }
        break;

    case BULLET_TRANSFORM_WAIT:
        this->activeTransformFlags |= kind;
        this->exStates[5].timer.SetCurrent(record->payload.int0);
        break;

    case BULLET_TRANSFORM_DESPAWN:
        EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(this));
        break;

    case BULLET_TRANSFORM_PLAY_SOUND:
        BulletQueueSoundAtPosition(
            record->payload.int0, this->position.x);
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
            BulletSpawnDescriptorView pattern;
            unsigned int packedPattern;
            int fadeParent;

            memset(&pattern, 0, sizeof(pattern));
            pattern.position = this->position;

            packedPattern =
                static_cast<unsigned int>(record->payload.int0);
            fadeParent = packedPattern & 0x80000000u;
            pattern.bulletType = 0;
            pattern.color =
                static_cast<signed char>((packedPattern >> 8) & 0xff);
            pattern.transformStartIndex = packedPattern & 0xff;
            pattern.count1 =
                static_cast<short>(record->payload.int1);
            pattern.speed1 = record->payload.float0;
            pattern.speed2 = record->payload.float1;

            ++record;
            ++this->transformIndex;
            pattern.count2 =
                *reinterpret_cast<short *>(&record->payload.int0);
            pattern.transformFlags =
                static_cast<unsigned int>(record->payload.int1);
            pattern.angle = record->payload.float0;
            pattern.angleStep = record->payload.float1;
            pattern.transformSound = -1;
            memcpy(
                pattern.transforms,
                this->transforms,
                sizeof(pattern.transforms));

            reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->
                SpawnBulletPattern(&pattern);
            ++this->transformIndex;
            if (fadeParent != 0)
                EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(this));
            goto nextRecord;
        }

    case BULLET_TRANSFORM_SET_OWNER_TAG:
        this->ownerTag = record->payload.int0;
        ++this->transformIndex;
        goto nextRecord;

    case BULLET_TRANSFORM_JUMP:
        this->transformIndex = record->payload.int0;
        goto nextRecord;

    case BULLET_TRANSFORM_STATE_8:
        this->activeTransformFlags |= BULLET_TRANSFORM_STATE_8;
        this->exStates[8].value0 = record->payload.float0;
        this->exStates[8].value1 = record->payload.float1;
        this->exStates[8].timer.SetCurrent(0);
        this->exStates[8].int0 = record->payload.int0;
        break;

    default:
        break;
    }

    ++this->transformIndex;
    goto nextRecord;
}
