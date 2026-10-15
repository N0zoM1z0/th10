#ifndef TH10_RECONSTRUCTION_ENEMY_LASER_HPP
#define TH10_RECONSTRUCTION_ENEMY_LASER_HPP
#include <stddef.h>

// Shared target-backed request views. These definitions are identical in the
// dispatcher and all six real laser-factory caller owners.
struct EnemyLaserVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
    int IsOutsidePlayfield(float extentX, float extentY);
};

typedef char EnemyLaserVectorViewSizeIs0C[
    (sizeof(EnemyLaserVectorView) == 0x0c) ? 1 : -1];

struct EnemyLaserType0RequestView
{
    EnemyLaserVectorView position;
    float angle;
    float maximumLength;
    float initialLength;
    float terminalDistance;
    float width;
    float speed;
    short type;
    short color;
    unsigned int flags;
    unsigned int patternWords[0x6c];
};

typedef char EnemyLaserType0RequestViewSizeIs1DC[
    (sizeof(EnemyLaserType0RequestView) == 0x1dc) ? 1 : -1];
typedef char EnemyLaserType0RequestAngleAt00C[
    (offsetof(EnemyLaserType0RequestView, angle) == 0x0c) ? 1 : -1];
typedef char EnemyLaserType0RequestMaximumAt010[
    (offsetof(EnemyLaserType0RequestView, maximumLength) == 0x10) ? 1 : -1];
typedef char EnemyLaserType0RequestInitialAt014[
    (offsetof(EnemyLaserType0RequestView, initialLength) == 0x14) ? 1 : -1];
typedef char EnemyLaserType0RequestTypeAt024[
    (offsetof(EnemyLaserType0RequestView, type) == 0x24) ? 1 : -1];
typedef char EnemyLaserType0RequestColorAt026[
    (offsetof(EnemyLaserType0RequestView, color) == 0x26) ? 1 : -1];
typedef char EnemyLaserType0RequestPatternAt02C[
    (offsetof(EnemyLaserType0RequestView, patternWords) == 0x2c) ? 1 : -1];

struct EnemyLaserType1RequestView
{
    EnemyLaserVectorView position;
    unsigned char unknown00C[0x18 - 0x0c];
    float value18;
    float value1C;
    float value20;
    float value24;
    float value28;
    float fixed2C;
    unsigned int value30;
    unsigned int value34;
    unsigned int value38;
    unsigned int value3C;
    short value40;
    short value42;
    unsigned int flags44;
    unsigned int patternWords[0x6c];
};
typedef char EnemyLaserType1RequestViewSizeIs1F8[
    (sizeof(EnemyLaserType1RequestView) == 0x1f8) ? 1 : -1];
typedef char EnemyLaserType1FixedAt02C[
    (offsetof(EnemyLaserType1RequestView, fixed2C) == 0x2c) ? 1 : -1];
typedef char EnemyLaserType1Value40At040[
    (offsetof(EnemyLaserType1RequestView, value40) == 0x40) ? 1 : -1];
typedef char EnemyLaserType1FlagsAt044[
    (offsetof(EnemyLaserType1RequestView, flags44) == 0x44) ? 1 : -1];
typedef char EnemyLaserType1PatternAt048[
    (offsetof(EnemyLaserType1RequestView, patternWords) == 0x48) ? 1 : -1];

struct EnemyLaserRequestScratch
{
    union
    {
        unsigned char bytes[0x1f8];
        unsigned int words[0x7e];
        EnemyLaserType0RequestView type0;
        EnemyLaserType1RequestView type1;
    };
};
typedef char EnemyLaserRequestScratchSizeIs1F8[
    (sizeof(EnemyLaserRequestScratch) == 0x1f8) ? 1 : -1];

EnemyLaserRequestScratch *EnemyInitializeLaserConfig(
    EnemyLaserRequestScratch *request);
unsigned int EnemyFireLaser(
    void *manager, EnemyLaserRequestScratch *request, int type);
#endif
