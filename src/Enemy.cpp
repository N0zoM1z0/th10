#include "Enemy.hpp"

#include <math.h>

struct EnemyManagedVmView
{
    unsigned char unknown000[0x300];
    unsigned int color;
    unsigned char unknown304[0x58];
    unsigned int flags;
};
typedef char EnemyManagedVmColorAt300[
    (offsetof(EnemyManagedVmView, color) == 0x300) ? 1 : -1];
typedef char EnemyManagedVmFlagsAt35C[
    (offsetof(EnemyManagedVmView, flags) == 0x35c) ? 1 : -1];

extern PlayerFloat3 g_EnemyGlobalPositionOffset;
extern float g_EnemyPlayfieldMinX;
extern float g_EnemyPlayfieldMaxX;
extern float g_EnemyPlayfieldMinY;
extern float g_EnemyPlayfieldMaxY;
extern float g_EnemyDirectionNegativeThreshold;
extern float g_EnemyDirectionPositiveThreshold;
extern int g_EnemyDamageProcessedFlag;

struct EnemyAnimationModeView
{
    unsigned char unknown000[0x28];
    int value28;
};
struct EnemyGameStateView
{
    unsigned char unknown000[0x378c];
    unsigned int flags378C;
};
struct EnemyVisualStateView
{
    unsigned char unknown000[0x9da4];
    unsigned int flags9DA4;
};
extern EnemyAnimationModeView *g_EnemyAnimationMode;
extern EnemyGameStateView *g_EnemyGameState;
extern EnemyVisualStateView *g_EnemyVisualState;

// Descriptive interfaces for target-observed callees. Several target machine
// boundaries use EAX/ESI/EDI live-ins; these declarations deliberately express
// logical source values instead of claiming those private conventions.
void EnemyEvaluatePositionInterpolation(
    EnemyPositionInterpolationView *interpolation, PlayerFloat3 *out);
void EnemyEvaluateScalarInterpolation(
    EnemyScalarInterpolationView *interpolation, EnemyFloat2 *out);
float EnemyWrapAngle(float angle);
void EnemySetVectorFromPolar(PlayerFloat3 *out, float angle, float magnitude);
void EnemyAdvanceMotion(EnemyMotionView *motion);
void EnemySetAnimationScript(EnemyRuntimeView *enemy, int script);
int EnemyRunEcl(EnemyFullObjectView *owner, float scale);
const unsigned char *EnemyResolveRuntimeCallback(EnemyFullObjectView *owner);
void EnemyResetEclState(EnemyFullObjectView *owner);
void EnemyRunCallbackEcl(
    EnemyFullObjectView *owner, const unsigned char *callbackName);
void EnemyInstallCallbackEcl(
    EnemyFullObjectView *owner, const unsigned char *callbackName);
void EnemyAddScoreReward(int value);
int EnemyFinalizeDeath(EnemyFullObjectView *owner);
void EnemyCheckPlayerCollision(
    const PlayerFloat3 *position, const EnemyFloat2 *size, Player *player);
void EnemySetManagedVmPositionWithOffset(
    unsigned int vmId, const PlayerFloat3 *position);
void EnemySetManagedVmPositionExact(
    unsigned int vmId, const PlayerFloat3 *position);
EnemyManagedVmView *EnemyResolveManagedVm(unsigned int vmId);
void EnemySpawnDamageEffect(int kind, float positionX);
void EnemyAdvanceTimer(PlayerTimerView *timer, float amount);

// Maintained source for the reviewed 0x0040DC80-0x0040E5EB hostile runtime
// update owner. Allocation/constructor evidence proves that the sole target
// argument points to the 0x14DC-byte tail at full object +0x103C. The target
// body independently loads that pointer from the stack and returns with RET 4.
int __stdcall EnemyRuntimeUpdate(EnemyRuntimeView *enemy)
{
    if ((enemy->flags & 0x400u) != 0)
        return 0;

    enemy->flags |= 0x400u;
    enemy->previousMotion = enemy->worldMotion;

    EnemyFloat2 interpolation;
    if (enemy->scalarInterpolations[0].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[0], &interpolation);
        enemy->offsetMotion.value1C = EnemyWrapAngle(interpolation.x);
        enemy->offsetMotion.value18 = interpolation.y;
    }
    if (enemy->scalarInterpolations[2].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[2], &interpolation);
        enemy->offsetMotion.value20 = interpolation.x;
        enemy->offsetMotion.value24 = interpolation.y;
    }
    if (enemy->scalarInterpolations[1].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[1], &interpolation);
        enemy->baseMotion.value1C = EnemyWrapAngle(interpolation.x);
        enemy->baseMotion.value18 = interpolation.y;
    }
    if (enemy->scalarInterpolations[3].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[3], &interpolation);
        enemy->baseMotion.value20 = interpolation.x;
        enemy->baseMotion.value24 = interpolation.y;
    }

    if (enemy->positionInterpolations[0].duration == 0)
    {
        if ((enemy->offsetMotion.flags & 1u) == 0)
        {
            EnemySetVectorFromPolar(
                &enemy->offsetMotion.velocity,
                enemy->offsetMotion.value1C,
                enemy->offsetMotion.value18);
            enemy->offsetMotion.velocity.z = 0.0f;
        }
        else
        {
            enemy->offsetMotion.value20 += enemy->offsetMotion.value24;
            enemy->offsetMotion.value1C = EnemyWrapAngle(
                enemy->offsetMotion.value1C + enemy->offsetMotion.value18);
        }
    }
    else
    {
        PlayerFloat3 target;
        EnemyEvaluatePositionInterpolation(
            &enemy->positionInterpolations[0], &target);
        enemy->offsetMotion.velocity.x = target.x - enemy->offsetMotion.position.x;
        enemy->offsetMotion.velocity.y = target.y - enemy->offsetMotion.position.y;
        enemy->offsetMotion.velocity.z = target.z - enemy->offsetMotion.position.z;
    }

    if (enemy->positionInterpolations[1].duration == 0)
    {
        if ((enemy->baseMotion.flags & 1u) == 0)
        {
            EnemySetVectorFromPolar(
                &enemy->baseMotion.velocity,
                enemy->baseMotion.value1C,
                enemy->baseMotion.value18);
            enemy->baseMotion.velocity.z = 0.0f;
        }
        else
        {
            enemy->baseMotion.value20 += enemy->baseMotion.value24;
            enemy->baseMotion.value1C = EnemyWrapAngle(
                enemy->baseMotion.value1C + enemy->baseMotion.value18);
        }
    }
    else
    {
        PlayerFloat3 target;
        EnemyEvaluatePositionInterpolation(
            &enemy->positionInterpolations[1], &target);
        enemy->baseMotion.velocity.x = target.x - enemy->baseMotion.position.x;
        enemy->baseMotion.velocity.y = target.y - enemy->baseMotion.position.y;
        enemy->baseMotion.velocity.z = target.z - enemy->baseMotion.position.z;
    }

    EnemyAdvanceMotion(&enemy->offsetMotion);
    if ((enemy->flags & 0x40000u) != 0)
    {
        enemy->baseMotion.position.x += g_EnemyGlobalPositionOffset.x;
        enemy->baseMotion.position.y += g_EnemyGlobalPositionOffset.y;
        enemy->baseMotion.position.z += g_EnemyGlobalPositionOffset.z;
    }
    EnemyAdvanceMotion(&enemy->baseMotion);

    enemy->worldMotion.velocity.x =
        enemy->baseMotion.position.x + enemy->offsetMotion.position.x -
        enemy->worldMotion.position.x;
    enemy->worldMotion.velocity.y =
        enemy->baseMotion.position.y + enemy->offsetMotion.position.y -
        enemy->worldMotion.position.y;
    enemy->worldMotion.velocity.z =
        enemy->baseMotion.position.z + enemy->offsetMotion.position.z -
        enemy->worldMotion.position.z;
    EnemyAdvanceMotion(&enemy->worldMotion);

    if ((enemy->flags & 0x200u) != 0)
    {
        const float halfWidth = enemy->movementBoundsSizeX * 0.5f;
        const float minimumX = enemy->movementBoundsCenterX - halfWidth;
        const float maximumX = enemy->movementBoundsCenterX + halfWidth;
        if (enemy->worldMotion.position.x < minimumX)
            enemy->worldMotion.position.x = minimumX;
        else if (maximumX < enemy->worldMotion.position.x)
            enemy->worldMotion.position.x = maximumX;

        const float halfHeight = enemy->movementBoundsSizeY * 0.5f;
        const float minimumY = enemy->movementBoundsCenterY - halfHeight;
        const float maximumY = enemy->movementBoundsCenterY + halfHeight;
        if (enemy->worldMotion.position.y < minimumY)
            enemy->worldMotion.position.y = minimumY;
        else if (maximumY < enemy->worldMotion.position.y)
            enemy->worldMotion.position.y = maximumY;

        enemy->offsetMotion.position.x =
            enemy->worldMotion.position.x - enemy->baseMotion.position.x;
        enemy->offsetMotion.position.y =
            enemy->worldMotion.position.y - enemy->baseMotion.position.y;
        enemy->offsetMotion.position.z =
            enemy->worldMotion.position.z - enemy->baseMotion.position.z;
    }

    const float visibilityHalfWidth = enemy->visibilityExtentX * 0.5f;
    const float visibilityHalfHeight = enemy->visibilityExtentY * 0.5f;
    if (enemy->worldMotion.position.x + visibilityHalfWidth < g_EnemyPlayfieldMinX ||
        g_EnemyPlayfieldMaxX < enemy->worldMotion.position.x - visibilityHalfWidth ||
        enemy->worldMotion.position.y + visibilityHalfHeight < g_EnemyPlayfieldMinY ||
        g_EnemyPlayfieldMaxY < enemy->worldMotion.position.y - visibilityHalfHeight)
    {
        if ((enemy->flags & 0x100u) != 0 && (enemy->flags & 4u) == 0)
            return -1;
    }
    else
    {
        enemy->flags |= 0x100u;
    }

    if ((enemy->flags & 0x100000u) != 0)
    {
        if (g_EnemyAnimationMode->value28 != 0)
        {
            if ((enemy->flags & 0x200000u) == 0)
            {
                enemy->animationBaseScript = enemy->animationScriptPrimary;
                EnemySetAnimationScript(enemy, enemy->animationScriptPrimary);
                enemy->flags |= 0x200001u;
            }
        }
        else if ((enemy->flags & 0x200000u) != 0)
        {
            enemy->animationBaseScript = enemy->animationScriptAlternate;
            EnemySetAnimationScript(enemy, enemy->animationScriptAlternate);
            enemy->flags &= 0xffdffffeu;
        }
    }

    const float timerScale = *enemy->updateTimer.scale;
    if (EnemyRunEcl(enemy->owner, timerScale) != 0)
        return -1;

    enemy->flags &= ~0x2000u;
    if ((enemy->flags & 0x11u) == 0)
    {
        int damage = g_Player->CalculateDamageToTarget(
            &enemy->worldMotion.position,
            reinterpret_cast<const PlayerFloat3 *>(&enemy->damageHitbox),
            NULL);
        if (g_Player->runtimeState == 2 || g_Player->runtimeState == 0)
            damage /= 5;

        if (damage != 0)
        {
            if ((g_EnemyGameState->flags378C & 1u) != 0 &&
                (enemy->flags & 0x8000u) != 0)
            {
                damage /= 5;
                if (damage < 1)
                    damage = 1;
            }

            if ((enemy->flags & 8u) == 0 &&
                enemy->damageReductionTimer.current < 1)
            {
                enemy->life -= damage;
            }

            const unsigned char *callback =
                EnemyResolveRuntimeCallback(enemy->owner);
            if (callback != NULL)
            {
                EnemyResetEclState(enemy->owner);
                EnemyRunCallbackEcl(enemy->owner, callback);
                if (EnemyRunEcl(enemy->owner, timerScale) != 0)
                    return -1;
            }

            if (enemy->life < 1 && (enemy->flags & 0x40u) == 0)
            {
                EnemyAddScoreReward(enemy->scoreReward);
                if (EnemyFinalizeDeath(enemy->owner) != 0)
                    return 1;
            }
            enemy->flags |= 0x2000u;
        }
        g_EnemyDamageProcessedFlag = 1;
    }

    const unsigned char *callback = EnemyResolveRuntimeCallback(enemy->owner);
    if (callback != NULL)
    {
        EnemyResetEclState(enemy->owner);
        EnemyInstallCallbackEcl(enemy->owner, callback);
    }

    if ((enemy->flags & 0x12u) == 0 &&
        enemy->playerCollisionTimer.current < 1)
    {
        EnemyCheckPlayerCollision(
            &enemy->worldMotion.position, &enemy->playerCollisionHitbox, g_Player);
    }

    if ((enemy->flags & 0x1000u) != 0)
    {
        int direction;
        if (g_EnemyDirectionNegativeThreshold <= enemy->worldMotion.velocity.x)
        {
            if (enemy->worldMotion.velocity.x <= g_EnemyDirectionPositiveThreshold)
                direction = 0;
            else
                direction = 1;
        }
        else
        {
            direction = -1;
        }

        const int previousDirection = enemy->animationDirection;
        if (previousDirection != direction)
        {
            int scriptOffset = 0;
            if (previousDirection == -1)
                scriptOffset = 3 - (direction != 0);
            else if (previousDirection == 0)
                scriptOffset = (direction != -1) + 1;
            else if (previousDirection == 1)
                scriptOffset = direction == 0 ? 4 : 1;

            enemy->animationDirection = direction;
            EnemySetAnimationScript(
                enemy, enemy->animationBaseScript + scriptOffset);
        }
    }

    int i;
    if ((enemy->flags & 0x40000u) == 0)
    {
        for (i = 0; i < 8; ++i)
            EnemySetManagedVmPositionWithOffset(
                enemy->managedVmIds[i], &enemy->worldMotion.position);
    }
    else
    {
        for (i = 0; i < 8; ++i)
            EnemySetManagedVmPositionExact(
                enemy->managedVmIds[i], &enemy->worldMotion.position);
    }

    if ((enemy->flags & 0x11u) == 0 &&
        (enemy->flags & 0xc0000u) == 0)
    {
        EnemyFullObjectView *tracked = g_Player->trackedEnemy;
        const float playerX = g_Player->drawPosition.x;
        if (tracked == NULL ||
            (float)fabs(enemy->worldMotion.position.x - playerX) >
                (float)fabs(tracked->runtime.worldMotion.position.x - playerX))
        {
            if (g_Player->trackedEnemyValid == 0)
                g_Player->trackedEnemy = enemy->owner;
            g_Player->trackedEnemyValid = 1;
        }
    }

    EnemyManagedVmView *primaryVm = EnemyResolveManagedVm(enemy->managedVmIds[0]);
    if (primaryVm == NULL)
        enemy->managedVmIds[0] = 0;

    if (enemy->damageFlashFrames != 0)
    {
        primaryVm->flags &= ~0x8000u;
        if ((enemy->flags & 0x8000u) != 0)
            g_EnemyVisualState->flags9DA4 &= ~0x8000u;
        enemy->damageFlashFrames--;
    }
    else if ((enemy->flags & 0x2000u) != 0)
    {
        primaryVm->color = 0xff0000ffu;
        primaryVm->flags |= 0x8000u;
        enemy->damageFlashFrames = 4;

        int effectKind = 0x13;
        if ((enemy->flags & 0x8000u) != 0)
        {
            const unsigned int gameFlags = g_EnemyGameState->flags378C;
            if ((gameFlags & 1u) == 0 || (gameFlags & 8u) == 0)
            {
                if ((gameFlags & 1u) == 0)
                {
                    if (enemy->ageCounter < 900)
                        effectKind = 0x23;
                }
                else if (enemy->ageCounter < 300)
                {
                    effectKind = 0x23;
                }
            }
        }
        EnemySpawnDamageEffect(effectKind, enemy->worldMotion.position.x);
    }

    if (enemy->damageReductionTimer.current > 0)
        EnemyAdvanceTimer(&enemy->damageReductionTimer, -1.0f);
    if (enemy->playerCollisionTimer.current > 0)
        EnemyAdvanceTimer(&enemy->playerCollisionTimer, -1.0f);

    enemy->updateTimer.previous = enemy->updateTimer.current;
    const float updateScale = *enemy->updateTimer.scale;
    if (updateScale > 0.9900000095367432f &&
        updateScale < 1.0099999904632568f)
    {
        ++enemy->updateTimer.current;
        enemy->updateTimer.subframe += 1.0f;
    }
    else
    {
        enemy->updateTimer.subframe += updateScale;
        enemy->updateTimer.current = (int)enemy->updateTimer.subframe;
    }

    return 0;
}
