#include "Enemy.hpp"

extern float __stdcall EnemyWrapAngle(float angle);

// Target 0x00413170-0x00413183 normalizes one motion angle and stores it in
// the +0x1C scalar used by the 0x11C/0x11E/0x120/0x122 ECL motion cases.
void __stdcall EnemySetMotionAngle(EnemyMotionView *motion, float angle)
{
    motion->value1C = EnemyWrapAngle(angle);
}
