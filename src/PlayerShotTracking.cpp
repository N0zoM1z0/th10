#include "Player.hpp"
#include "Enemy.hpp"

#include <math.h>

// Native target 0x00428CE0 is raw-byte-identical for 80 bytes to the
// independently exact BulletAngleDifference owner at 0x00408660. Preserve a
// separate source callable owner; the physical TU and C++ spelling are open.
__declspec(noinline) float __stdcall PlayerShotAngleDifference(
    float angle, float reference)
{
    float difference = angle - reference;
    if (difference > 3.1415927f)
        return angle - (reference + 6.2831855f);
    if (reference - angle > 3.1415927f)
        difference = angle - (reference - 6.2831855f);
    return difference;
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
    if (enemy != NULL)
    {
        if ((enemy->runtime.flags & 0x11u) != 0 ||
            (enemy->runtime.flags & 0xC0000u) != 0)
        {
            shot->trackedEnemy = NULL;
            enemy = NULL;
        }
    }

    if (enemy == NULL)
    {
        float magnitude = shot->motion.polarMagnitude + 0.1f;
        if (magnitude > 16.0f)
            magnitude = 16.0f;
        shot->motion.polarMagnitude = magnitude;
        return 0;
    }

    const float dx = enemy->runtime.worldMotion.position.x -
                     shot->motion.position.x;
    const float dy = enemy->runtime.worldMotion.position.y -
                     shot->motion.position.y;
    const float targetAngle = static_cast<float>(atan2(dy, dx));
    const float difference = PlayerShotAngleDifference(
        targetAngle, shot->motion.angle);
    float magnitude = shot->motion.polarMagnitude;
    if (shot->timer.current >= 120)
    {
        shot->motion.polarMagnitude = magnitude + 0.2f;
        return 0;
    }

    float deviation = static_cast<float>(fabs(difference));
    if (deviation >= 0.7853982f)
    {
        magnitude -= 0.3f;
        if (magnitude < 4.0f)
            magnitude = 4.0f;
    }
    else if (deviation < 0.2617994f)
    {
        magnitude += 0.1f;
        if (magnitude > 16.0f)
            magnitude = 16.0f;
    }
    shot->motion.angle = EnemyWrapAngle(
        shot->motion.angle + difference * 0.2f);
    shot->motion.polarMagnitude = magnitude;
    return 0;
}
