#ifndef TH10_RECONSTRUCTION_ENEMY_HPP
#define TH10_RECONSTRUCTION_ENEMY_HPP

#include "Player.hpp"

#include <stddef.h>

struct EnemyFloat2
{
    float x;
    float y;
};
typedef char EnemyFloat2SizeIs08[(sizeof(EnemyFloat2) == 0x08) ? 1 : -1];

// Maintained view of the 0x2C movement records consumed by the reviewed TH10
// hostile runtime owner. Position, velocity and the flag word are target-proven;
// the four scalar motion parameters retain offset-based names because their
// original semantics remain unresolved.
struct EnemyMotionView
{
    PlayerFloat3 position;
    PlayerFloat3 velocity;
    float value18;
    float value1C;
    float value20;
    float value24;
    unsigned int flags;
};
typedef char EnemyMotionViewSizeIs2C[(sizeof(EnemyMotionView) == 0x2c) ? 1 : -1];
typedef char EnemyMotionVelocityAt0C[(offsetof(EnemyMotionView, velocity) == 0x0c) ? 1 : -1];
typedef char EnemyMotionFlagsAt28[(offsetof(EnemyMotionView, flags) == 0x28) ? 1 : -1];

// Opaque interpolation records with only the target-observed duration/mode
// controls exposed. Their helper ABIs are private register conventions.
struct EnemyPositionInterpolationView
{
    unsigned char unknown000[0x44];
    int duration;
    int mode;
};
typedef char EnemyPositionInterpolationSizeIs4C[
    (sizeof(EnemyPositionInterpolationView) == 0x4c) ? 1 : -1];
typedef char EnemyPositionInterpolationDurationAt44[
    (offsetof(EnemyPositionInterpolationView, duration) == 0x44) ? 1 : -1];

struct EnemyScalarInterpolationView
{
    unsigned char unknown000[0x34];
    int duration;
    int mode;
};
typedef char EnemyScalarInterpolationSizeIs3C[
    (sizeof(EnemyScalarInterpolationView) == 0x3c) ? 1 : -1];
typedef char EnemyScalarInterpolationDurationAt34[
    (offsetof(EnemyScalarInterpolationView, duration) == 0x34) ? 1 : -1];

struct EnemyFullObjectView;

// Maintained exact-size view of the embedded runtime tail beginning at full
// enemy object +0x103C. Only fields exercised by the reviewed update owner are
// named. This does not claim the original source type or member identifiers.
struct EnemyRuntimeView
{
    EnemyMotionView previousMotion;
    EnemyMotionView worldMotion;
    EnemyMotionView offsetMotion;
    EnemyMotionView baseMotion;
    EnemyFloat2 damageHitbox;
    EnemyFloat2 playerCollisionHitbox;
    unsigned int managedVmIds[8];
    unsigned char unknown0E0[0x14];
    int animationBaseScript;
    int animationDirection;
    unsigned char unknown0FC[0x20];
    PlayerTimerView updateTimer;
    unsigned char unknown130[0x0c];
    EnemyPositionInterpolationView positionInterpolations[2];
    EnemyScalarInterpolationView scalarInterpolations[4];
    unsigned char unknown2C4[0x10e0];
    float visibilityExtentX;
    float visibilityExtentY;
    float movementBoundsCenterX;
    float movementBoundsCenterY;
    float movementBoundsSizeX;
    float movementBoundsSizeY;
    int scoreReward;
    int life;
    unsigned char unknown13C4[0x04];
    int ageCounter;
    unsigned char unknown13CC[0x48];
    int damageFlashFrames;
    unsigned char unknown1418[0x04];
    PlayerTimerView damageReductionTimer;
    PlayerTimerView playerCollisionTimer;
    unsigned int flags;
    int animationScriptPrimary;
    int animationScriptAlternate;
    unsigned char unknown1450[0x88];
    EnemyFullObjectView *owner;
};

typedef char EnemyRuntimeViewSizeIs14DC[
    (sizeof(EnemyRuntimeView) == 0x14dc) ? 1 : -1];
typedef char EnemyRuntimeWorldMotionAt02C[
    (offsetof(EnemyRuntimeView, worldMotion) == 0x02c) ? 1 : -1];
typedef char EnemyRuntimeOffsetMotionAt058[
    (offsetof(EnemyRuntimeView, offsetMotion) == 0x058) ? 1 : -1];
typedef char EnemyRuntimeBaseMotionAt084[
    (offsetof(EnemyRuntimeView, baseMotion) == 0x084) ? 1 : -1];
typedef char EnemyRuntimeDamageHitboxAt0B0[
    (offsetof(EnemyRuntimeView, damageHitbox) == 0x0b0) ? 1 : -1];
typedef char EnemyRuntimePlayerHitboxAt0B8[
    (offsetof(EnemyRuntimeView, playerCollisionHitbox) == 0x0b8) ? 1 : -1];
typedef char EnemyRuntimeManagedVmsAt0C0[
    (offsetof(EnemyRuntimeView, managedVmIds) == 0x0c0) ? 1 : -1];
typedef char EnemyRuntimeUpdateTimerAt11C[
    (offsetof(EnemyRuntimeView, updateTimer) == 0x11c) ? 1 : -1];
typedef char EnemyRuntimePositionInterpolationAt13C[
    (offsetof(EnemyRuntimeView, positionInterpolations) == 0x13c) ? 1 : -1];
typedef char EnemyRuntimeScalarInterpolationAt1D4[
    (offsetof(EnemyRuntimeView, scalarInterpolations) == 0x1d4) ? 1 : -1];
typedef char EnemyRuntimeVisibilityExtentAt13A4[
    (offsetof(EnemyRuntimeView, visibilityExtentX) == 0x13a4) ? 1 : -1];
typedef char EnemyRuntimeScoreAt13BC[
    (offsetof(EnemyRuntimeView, scoreReward) == 0x13bc) ? 1 : -1];
typedef char EnemyRuntimeLifeAt13C0[
    (offsetof(EnemyRuntimeView, life) == 0x13c0) ? 1 : -1];
typedef char EnemyRuntimeDamageFlashAt1414[
    (offsetof(EnemyRuntimeView, damageFlashFrames) == 0x1414) ? 1 : -1];
typedef char EnemyRuntimeDamageTimerAt141C[
    (offsetof(EnemyRuntimeView, damageReductionTimer) == 0x141c) ? 1 : -1];
typedef char EnemyRuntimeCollisionTimerAt1430[
    (offsetof(EnemyRuntimeView, playerCollisionTimer) == 0x1430) ? 1 : -1];
typedef char EnemyRuntimeFlagsAt1444[
    (offsetof(EnemyRuntimeView, flags) == 0x1444) ? 1 : -1];
typedef char EnemyRuntimeOwnerAt14D8[
    (offsetof(EnemyRuntimeView, owner) == 0x14d8) ? 1 : -1];

// TH10 allocation and constructor clearing jointly establish the full object
// size and the runtime-tail offset. All pre-tail storage remains opaque here.
struct EnemyFullObjectView
{
    unsigned char unknown000[0x103c];
    EnemyRuntimeView runtime;
};
typedef char EnemyFullObjectViewSizeIs2518[
    (sizeof(EnemyFullObjectView) == 0x2518) ? 1 : -1];
typedef char EnemyFullObjectRuntimeAt103C[
    (offsetof(EnemyFullObjectView, runtime) == 0x103c) ? 1 : -1];

// Descriptive maintained name for the reviewed 0x0040DC80-0x0040E5EB owner.
// The target boundary is one stack EnemyRuntimeView* argument with RET 4. The
// original source identifier, member/free-function spelling and TU are unknown.
int __stdcall EnemyRuntimeUpdate(EnemyRuntimeView *enemy);

#endif
