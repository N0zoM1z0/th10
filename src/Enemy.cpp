#include "Enemy.hpp"

#include <math.h>
#include <new>
#include <stdlib.h>
#include <string.h>

struct EnemyManagedVmView;
struct EnemyManagedVmListNodeView
{
    EnemyManagedVmView *vm;
    EnemyManagedVmListNodeView *next;
};

struct EnemyManagedVmView
{
    unsigned int id;
    unsigned char unknown004[0x10];
    EnemyManagedVmListNodeView *children;
    int childState;
    unsigned char unknown01C[0x2e4];
    unsigned int color;
    unsigned char unknown304[0x58];
    unsigned int flags;
};
typedef char EnemyManagedVmChildrenAt014[
    (offsetof(EnemyManagedVmView, children) == 0x14) ? 1 : -1];
typedef char EnemyManagedVmColorAt300[
    (offsetof(EnemyManagedVmView, color) == 0x300) ? 1 : -1];
typedef char EnemyManagedVmFlagsAt35C[
    (offsetof(EnemyManagedVmView, flags) == 0x35c) ? 1 : -1];

struct EnemyManagedVmRegistryView
{
    unsigned char unknown000[0x72dad4];
    EnemyManagedVmListNodeView *primaryList;
    unsigned char unknown72DAD8[0x04];
    EnemyManagedVmListNodeView *secondaryList;
};
typedef char EnemyManagedVmRegistryPrimaryAt72DAD4[
    (offsetof(EnemyManagedVmRegistryView, primaryList) == 0x72dad4) ? 1 : -1];
typedef char EnemyManagedVmRegistrySecondaryAt72DADC[
    (offsetof(EnemyManagedVmRegistryView, secondaryList) == 0x72dadc) ? 1 : -1];

extern PlayerFloat3 g_EnemyGlobalPositionOffset;
extern float g_EnemyPlayfieldMinX;
extern float g_EnemyPlayfieldMaxX;
extern float g_EnemyPlayfieldMinY;
extern float g_EnemyPlayfieldMaxY;
extern float g_EnemyDirectionNegativeThreshold;
extern float g_EnemyDirectionPositiveThreshold;
extern int g_EnemyDamageProcessedFlag;
extern float g_PlayerTimerScale;
extern int g_EnemySpawnLayerIndex;
extern EnemyManagerView *g_EnemyManager;
extern EnemyManagedVmRegistryView *g_EnemyManagedVmRegistry;

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
void EnemyCheckPlayerCollision(
    const PlayerFloat3 *position, const EnemyFloat2 *size, Player *player);
void EnemySetManagedVmPositionWithOffset(
    unsigned int vmId, const PlayerFloat3 *position);
void EnemySetManagedVmPositionExact(
    unsigned int vmId, const PlayerFloat3 *position);
EnemyManagedVmView *EnemyResolveManagedVm(unsigned int vmId);
void EnemySpawnDamageEffect(int kind, float positionX);
void EnemyAdvanceTimer(PlayerTimerView *timer, float amount);
void EnemyPrepareRuntimeStorage(EnemyRuntimeView *runtime);
int EnemyLookupEclSubroutine(void *scriptDatabase, const char *name);
void EnemyInvokeScalarDeletingDestructor(EnemyFullObjectView *enemy, int freeObject);
void EnemyPlayDeathSound(int soundId, float positionX);
void EnemySpawnDeathEffect(
    const PlayerFloat3 *position, void *resource, int scriptId);
void EnemySpawnItem(
    const PlayerFloat3 *position, int itemType, int owner, float angle, float speed);
void EnemyDropItemCounts(
    const PlayerFloat3 *position, int *itemDropBlock);
void EnemyPlaySound(int soundId);

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


// Maintained logical constructor for target 0x0040D830-0x0040DAD0. The target
// machine boundary carries the full object in ESI and one stack subroutine-name
// argument with RET 4; this ordinary C++ spelling does not claim that private
// ABI. Primary/base vptr writes are compiler/type ownership still under review.
EnemyFullObjectView *EnemyConstruct(
    EnemyFullObjectView *enemy, const char *eclSubroutineName)
{
    enemy->value1010 = 0;
    enemy->value1014 = 0;

    EnemyPrepareRuntimeStorage(&enemy->runtime);
    memset(&enemy->runtime, 0, sizeof(enemy->runtime));

    enemy->embeddedScriptState.value00 = 0;
    enemy->embeddedScriptState.subroutineOffset = 0;
    enemy->activeScriptState = &enemy->embeddedScriptState;
    enemy->self101C = enemy;
    enemy->flags1028 &= ~1u;
    enemy->value1020 = 0;
    enemy->value1018 = -1;
    enemy->scriptStateMirror = &enemy->embeddedScriptState;
    enemy->ownedAllocations = NULL;
    enemy->value1038 = 0;

    enemy->runtime.owner = enemy;
    enemy->runtime.listNode.enemy = enemy;
    enemy->runtime.listNode.next = NULL;
    enemy->runtime.listNode.previous = NULL;

    enemy->runtime.damageHitbox.x = 24.0f;
    enemy->runtime.damageHitbox.y = 24.0f;
    enemy->runtime.playerCollisionHitbox.x = 24.0f;
    enemy->runtime.playerCollisionHitbox.y = 24.0f;
    enemy->runtime.value1400 = 32.0f;
    enemy->runtime.value1404 = 32.0f;
    enemy->runtime.managerSlot = -1;

    if ((enemy->runtime.updateTimer.flags & 1u) == 0)
    {
        enemy->runtime.updateTimer.previous = -999999;
        enemy->runtime.updateTimer.current = 0;
        enemy->runtime.updateTimer.subframe = 0.0f;
        enemy->runtime.updateTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.updateTimer.flags |= 1u;
    }
    enemy->runtime.updateTimer.current = 0;
    enemy->runtime.updateTimer.subframe = 0.0f;
    enemy->runtime.updateTimer.previous = -1;

    if ((enemy->runtime.damageReductionTimer.flags & 1u) == 0)
    {
        enemy->runtime.damageReductionTimer.previous = -999999;
        enemy->runtime.damageReductionTimer.current = 0;
        enemy->runtime.damageReductionTimer.subframe = 0.0f;
        enemy->runtime.damageReductionTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.damageReductionTimer.flags |= 1u;
    }
    enemy->runtime.damageReductionTimer.current = 0;
    enemy->runtime.damageReductionTimer.subframe = 0.0f;
    enemy->runtime.damageReductionTimer.previous = -1;

    if ((enemy->runtime.playerCollisionTimer.flags & 1u) == 0)
    {
        enemy->runtime.playerCollisionTimer.previous = -999999;
        enemy->runtime.playerCollisionTimer.current = 0;
        enemy->runtime.playerCollisionTimer.subframe = 0.0f;
        enemy->runtime.playerCollisionTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.playerCollisionTimer.flags |= 1u;
    }
    enemy->runtime.playerCollisionTimer.current = 0;
    enemy->runtime.playerCollisionTimer.subframe = 0.0f;
    enemy->runtime.playerCollisionTimer.previous = -1;

    enemy->scriptDatabase = g_EnemyManager->scriptDatabase;
    enemy->activeScriptState->subroutineOffset =
        EnemyLookupEclSubroutine(enemy->scriptDatabase, eclSubroutineName);
    enemy->activeScriptState->value00 = 0;

    enemy->runtime.life = 0;
    enemy->runtime.unknown13C4 = 0;
    enemy->runtime.ageCounter = 0;

    for (int i = 0; i < 8; ++i)
    {
        enemy->runtime.callbackThresholds[i].threshold = -1;
        enemy->runtime.callbackThresholds[i].callbackId = -1;
        enemy->runtime.callbackThresholds[i].state = 0;
    }

    return enemy;
}

// Maintained source for target 0x0040CFB0 plus its compiler-owned alignment and
// switch tables through 0x0040D1F0. The target machine boundary uses EAX for the
// 0x40-byte request and two stack arguments (manager, ECL subroutine name), then
// RET 8. The ordinary signature below preserves logical values without claiming
// that private register assignment.
EnemyFullObjectView *EnemySpawn(
    EnemyManagerView *manager,
    const char *eclSubroutineName,
    const EnemySpawnRequestView *request)
{
    EnemyFullObjectView *enemy =
        static_cast<EnemyFullObjectView *>(::operator new(sizeof(EnemyFullObjectView)));
    if (enemy != NULL)
        enemy = EnemyConstruct(enemy, eclSubroutineName);

    enemy->runtime.offsetMotion.position = request->position;
    enemy->runtime.scoreReward = request->scoreReward;
    enemy->runtime.life = request->life;
    enemy->runtime.itemDropType = request->itemDropType;

    enemy->runtime.flags =
        (enemy->runtime.flags & ~0x800u) |
        ((static_cast<unsigned int>(request->setFlag0800) & 1u) << 11);
    enemy->spawnLayerMask =
        static_cast<unsigned char>(1u << g_EnemySpawnLayerIndex);

    for (int i = 0; i < 8; ++i)
        enemy->runtime.spawnParameters[i] = request->parameters[i];

    enemy->runtime.flags =
        (enemy->runtime.flags & ~0x40000u) |
        ((static_cast<unsigned int>(request->setFlag40000) & 1u) << 18);

    if ((enemy->runtime.damageReductionTimer.flags & 1u) == 0)
    {
        enemy->runtime.damageReductionTimer.previous = -999999;
        enemy->runtime.damageReductionTimer.current = 0;
        enemy->runtime.damageReductionTimer.subframe = 0.0f;
        enemy->runtime.damageReductionTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.damageReductionTimer.flags |= 1u;
    }
    enemy->runtime.damageReductionTimer.current = 2;
    enemy->runtime.damageReductionTimer.subframe = 2.0f;
    enemy->runtime.damageReductionTimer.previous = 1;

    EnemyRuntimeUpdate(&enemy->runtime);

    if ((enemy->runtime.flags & 0x8000u) != 0)
    {
        if (enemy->runtime.itemDropType == 1)
            enemy->runtime.itemDropType = 10;
        else if (enemy->runtime.itemDropType == 4)
            enemy->runtime.itemDropType = 11;
    }

    enemy->runtime.deathSoundId = (manager->spawnCounter & 1) + 2;
    enemy->runtime.deathEffectScript = 0x167;
    if (enemy->runtime.value0EC == 1)
    {
        switch (enemy->runtime.value0F0)
        {
        case 5:
        case 25:
        case 50:
            enemy->runtime.deathEffectScript = 0x164;
            break;
        case 10:
        case 30:
        case 51:
            enemy->runtime.deathEffectScript = 0x16a;
            break;
        case 15:
        case 35:
        case 52:
            enemy->runtime.deathEffectScript = 0x16d;
            break;
        default:
            break;
        }
    }
    enemy->runtime.deathEffectResourceIndex = 0;

    EnemyListNodeView *node = &enemy->runtime.listNode;
    if (manager->enemyListHead == NULL)
    {
        manager->enemyListHead = node;
    }
    else
    {
        EnemyListNodeView *tail = manager->enemyListTail;
        if (tail->next != NULL)
        {
            node->next = tail->next;
            tail->next->previous = node;
        }
        tail->next = node;
        node->previous = tail;
    }
    manager->enemyListTail = node;
    ++manager->activeEnemyCount;
    ++manager->spawnCounter;
    return enemy;
}

// Target 0x0040D750-0x0040D7F5 carries EnemyManagerView* in EDI and returns
// with plain RET. The target callback wrapper at 0x0040D810 supplies EDI from
// its conventional ECX callback argument.
int EnemyManagerUpdate(EnemyManagerView *manager)
{
    EnemyListNodeView *node = manager->enemyListHead;
    while (node != NULL)
    {
        EnemyFullObjectView *enemy = node->enemy;
        EnemyListNodeView *next = node->next;

        if ((enemy->runtime.flags & 0x20000u) == 0)
        {
            if (EnemyRuntimeUpdate(&enemy->runtime) == 0)
                enemy->runtime.flags &= ~0x400u;
            else
                EnemyInvokeScalarDeletingDestructor(enemy, 1);
        }
        else
        {
            EnemyInvokeScalarDeletingDestructor(enemy, 1);
        }
        node = next;
    }

    manager->timer.previous = manager->timer.current;
    const float scale = *manager->timer.scale;
    if (scale > 0.9900000095367432f && scale < 1.0099999904632568f)
    {
        ++manager->timer.current;
        manager->timer.subframe += 1.0f;
    }
    else
    {
        manager->timer.subframe += scale;
        manager->timer.current = static_cast<int>(manager->timer.subframe);
    }
    return 1;
}

// Registered at target update priority 0x12. The machine body is a thin
// ECX-to-EDI wrapper around the private EnemyManagerUpdate owner.
int __fastcall EnemyManagerUpdateCallback(EnemyManagerView *manager)
{
    return EnemyManagerUpdate(manager);
}

// Registered through the target draw-chain registrar at priority 0x14.
int __fastcall EnemyManagerDrawCallback(EnemyManagerView *manager)
{
    (void)manager;
    return 1;
}

// Maintained destructor body for target 0x0040DAE0-0x0040DC61. Its sole full
// object argument is stack-bound with RET 4. The scalar-deleting wrapper at
// 0x0040CC50 owns optional object deallocation; this body only tears down
// references and owned suballocations.
void __stdcall EnemyTeardown(EnemyFullObjectView *enemy)
{
    EnemyManagerView *manager = g_EnemyManager;
    EnemyListNodeView *listNode = &enemy->runtime.listNode;

    if (manager->enemyListHead == listNode)
        manager->enemyListHead = listNode->next;
    if (manager->enemyListTail == listNode)
        manager->enemyListTail = listNode->previous;
    if (listNode->next != NULL)
        listNode->next->previous = listNode->previous;
    if (listNode->previous != NULL)
        listNode->previous->next = listNode->next;
    listNode->next = NULL;
    listNode->previous = NULL;
    --manager->activeEnemyCount;

    if ((enemy->runtime.flags & 0x8000u) != 0)
        manager->specialEnemySlots[enemy->runtime.managerSlot] = NULL;

    for (int i = 0; i < 10; ++i)
    {
        const unsigned int vmId = enemy->runtime.managedVmIds[i];
        if (vmId != 0)
        {
            EnemyManagedVmView *vm = NULL;
            EnemyManagedVmListNodeView *vmNode =
                g_EnemyManagedVmRegistry->primaryList;
            while (vmNode != NULL)
            {
                if (vmNode->vm->id == vmId)
                {
                    vm = vmNode->vm;
                    break;
                }
                vmNode = vmNode->next;
            }
            if (vm == NULL)
            {
                vmNode = g_EnemyManagedVmRegistry->secondaryList;
                while (vmNode != NULL)
                {
                    if (vmNode->vm->id == vmId)
                    {
                        vm = vmNode->vm;
                        break;
                    }
                    vmNode = vmNode->next;
                }
            }

            if (vm != NULL)
            {
                vm->flags |= 0x4000000u;
                if (vm->childState == 0)
                {
                    EnemyManagedVmListNodeView *child = vm->children;
                    while (child != NULL)
                    {
                        child->vm->flags |= 0x4000000u;
                        child = child->next;
                    }
                }
            }
        }
        enemy->runtime.managedVmIds[i] = 0;
    }

    if (g_Player != NULL)
    {
        if (g_Player->trackedEnemy == enemy)
        {
            g_Player->trackedEnemy = NULL;
            g_Player->trackedEnemyValid = 0;
        }
        for (int i = 0; i < 128; ++i)
        {
            if (g_Player->shots[i].trackedEnemy == enemy)
                g_Player->shots[i].trackedEnemy = NULL;
        }
    }

    EnemyOwnedAllocationNodeView *owned = enemy->ownedAllocations;
    while (owned != NULL)
    {
        EnemyOwnedAllocationNodeView *next = owned->next;
        free(owned->allocation);
        free(owned);
        owned = next;
    }
}

// Reviewed target 0x0040E5F0-0x0040E691 finalization path. The direct callees
// use private register live-ins; these descriptive calls express only the
// target-observed logical values and do not claim their original declarations.
int __stdcall EnemyFinalizeDeath(EnemyFullObjectView *enemy)
{
    EnemyRuntimeView *runtime = &enemy->runtime;

    if (runtime->deathSoundId >= 0)
        EnemyPlayDeathSound(runtime->deathSoundId, runtime->worldMotion.position.x);

    if (runtime->deathEffectScript >= 0)
    {
        EnemySpawnDeathEffect(
            &runtime->worldMotion.position,
            g_EnemyManager->effectResources[runtime->deathEffectResourceIndex],
            runtime->deathEffectScript);
    }

    if (runtime->itemDropType > 0)
    {
        EnemySpawnItem(
            &runtime->worldMotion.position,
            runtime->itemDropType,
            -1,
            -1.5707964f,
            2.2f);
    }

    EnemyDropItemCounts(&runtime->worldMotion.position, &runtime->itemDropType);
    runtime->itemDropType = 0;
    EnemyPlaySound(10);
    return 1;
}
