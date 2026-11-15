#include "AnmManager.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

#include <math.h>

// Target 0x00428AD0: install Player's currently tracked Enemy as this
// shot's target only if its world X remains in the 224-unit playfield window.
// The observed entry uses ECX Player, EDX shot and one unused stack argument.
typedef char PlayerTrackedEnemyNativeOffset3504[
    offsetof(Player, trackedEnemy) == 0x3504 ? 1 : -1];
typedef char EnemyWorldXNativeOffset1068[
    offsetof(EnemyFullObjectView, runtime) +
    offsetof(EnemyRuntimeView, worldMotion) == 0x1068 ? 1 : -1];
int __fastcall PlayerShotAcquireTarget(
    Player *player, PlayerShotRuntimeView *shot, int unusedFrame)
{
    shot->trackedEnemy = NULL;
    EnemyFullObjectView *enemy = player->trackedEnemy;
    if (enemy != NULL)
    {
        shot->trackedEnemy = enemy;
        float x = enemy->runtime.worldMotion.position.x;
        if (fabs(x) > 224.0f)
            shot->trackedEnemy = NULL;
    }
    return 0;
}

// Native target 0x00428CE0 is raw-byte-identical for 80 bytes to the
// independently exact BulletAngleDifference owner at 0x00408660. Preserve a
// separate source callable owner; the physical TU and C++ spelling are open.
__declspec(noinline) double __stdcall PlayerShotAngleDifference(
    float angle, float reference)
{
    float difference = angle - reference;
    if (difference > 3.1415927f)
        return angle - (reference + 6.2831855f);
    if (reference - angle > 3.1415927f)
        difference = angle - (reference - 6.2831855f);
    return difference;
}

// Target 0x00428C20..0x00428C9C (125 bytes): a shot's initial
// position comes from the indicated player's option's fixed-point replay
// coordinates, then receives the descriptor offset minus shot velocity.
// Real native receiver register inputs are ECX=Player and EDX=shot.
int __fastcall PlayerShotSetOptionOrigin(
    Player *player, PlayerShotRuntimeView *shot)
{
    const PlayerShotDescriptorView *descriptor = shot->descriptor;
    const int optionIndex = static_cast<int>(descriptor->sourceIndex) - 1;
    const PlayerOptionRuntime *const option = reinterpret_cast<const PlayerOptionRuntime *>(
        reinterpret_cast<unsigned int>(&player->options[0]) +
        static_cast<unsigned int>(optionIndex) * sizeof(PlayerOptionRuntime));
    const PlayerPositionPair *optionPosition = &option->replayPair1;
    PlayerFloat3Value origin(
        static_cast<float>(optionPosition->x) * 0.01f,
        static_cast<float>(optionPosition->y) * 0.01f, 0.0f);
    shot->motion.position = origin;
    shot->motion.position.x +=
        descriptor->spawnOffsetX - shot->motion.velocity.x;
    shot->motion.position.y +=
        descriptor->spawnOffsetY - shot->motion.velocity.y;
    return 0;
}

// The raw target 0x00428B10 has one private live input, EDX=shot; the
// registered callback's ECX/owner identity has not been established.
extern float __stdcall EnemyWrapAngle(float angle);
int __fastcall PlayerShotTrackEnemy(void *unidentifiedOwner,
                                    PlayerShotRuntimeView *shot)
{
    if (shot->state == 2)
        return 0;
    EnemyFullObjectView *enemy = shot->trackedEnemy;
    if (enemy != NULL && ((enemy->runtime.flags & 0x11u) != 0 ||
                          (enemy->runtime.flags & 0xC0000u) != 0))
    {
        shot->trackedEnemy = NULL;
        enemy = NULL;
    }
    if (enemy != NULL)
    {
        float turn = PlayerShotAngleDifference(
            static_cast<float>(atan2(
                enemy->runtime.worldMotion.position.y - shot->motion.position.y,
                enemy->runtime.worldMotion.position.x - shot->motion.position.x)),
            shot->motion.angle);
        float magnitude = shot->motion.speedOrAngleStep;
        if (shot->timer.current < 120)
        {
            if (fabs(turn) >= 0.7853982f)
            {
                magnitude -= 0.3f;
                if (magnitude < 4.0f)
                    magnitude = 4.0f;
            }
            else if (fabs(turn) < 0.2617994f)
            {
                magnitude += 0.1f;
                if (magnitude > 16.0f)
                    magnitude = 16.0f;
            }
            shot->motion.angle = EnemyWrapAngle(turn * 0.2f + shot->motion.angle);
            shot->motion.speedOrAngleStep = magnitude;
            return 0;
        }
        shot->motion.speedOrAngleStep = magnitude + 0.2f;
        return 0;
    }
    float magnitude = shot->motion.speedOrAngleStep + 0.1f;
    if (magnitude > 16.0f)
        magnitude = 16.0f;
    shot->motion.speedOrAngleStep = magnitude;
    return 0;
}
