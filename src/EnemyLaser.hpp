#ifndef TH10_RECONSTRUCTION_ENEMY_LASER_HPP
#define TH10_RECONSTRUCTION_ENEMY_LASER_HPP
#include "AnmManager.hpp"
#include "Player.hpp"
#include <stddef.h>

// These are explicit views of the single target47781C allocation. The narrow
// lookup and known-slot virtual views preserve existing call boundaries; they
// are not separate owners or replacement concrete vtables.
struct EnemyLaserNodeView
{
    unsigned char unknown000[0x08];
    EnemyLaserNodeView *next;
    unsigned char unknown00C[0x54 - 0x0c];
    int id;
};
struct EnemyLaserManagerView
{
    unsigned char unknown000[0x18];
    EnemyLaserNodeView *head;
};
typedef char EnemyLaserNodeNextAt08[
    (offsetof(EnemyLaserNodeView, next) == 0x08) ? 1 : -1];
typedef char EnemyLaserNodeIdAt54[
    (offsetof(EnemyLaserNodeView, id) == 0x54) ? 1 : -1];
typedef char EnemyLaserManagerHeadAt18[
    (offsetof(EnemyLaserManagerView, head) == 0x18) ? 1 : -1];

extern EnemyLaserManagerView *g_EnemyLaserManager;

struct EnemyEffectWaitNode
{
    virtual void UnknownVirtual0();
    virtual void UnknownVirtual1();
    virtual void UnknownVirtual2();
    virtual void UnknownVirtual3();
    virtual void UnknownVirtual4();
    virtual unsigned int ApplyClear(int mode);
    virtual void UnknownVirtual6();
    virtual unsigned int ApplyPatternCancel(
        const PlayerFloat3 *position, float radius, int mode);

    unsigned int unknown004;
    EnemyEffectWaitNode *next;
    int state;
    unsigned char unknown010[0x40];
    unsigned char flag50;
};

struct EnemyEffectWaitManager
{
    unsigned char unknown000[0x18];
    EnemyEffectWaitNode *head;
    unsigned char unknown01C[0x440 - 0x1c];
    PlayerFloat3 cancelPosition;

    __declspec(noinline) unsigned int ApplyPatternCancel(
        const PlayerFloat3 *position, float radius, int mode);
};

// Scalar words retain the target's integer/float reinterpretation per opcode.
union EnemyLaserWordView
{
    float scalar;
    int integer;
    unsigned int bits;
};
struct EnemyLaserCommandView
{
    EnemyLaserWordView arguments[4];
    unsigned int opcode;
    int allowWhileActive;
};
typedef char EnemyLaserCommandLayout[
    (sizeof(EnemyLaserCommandView) == 0x18 &&
     offsetof(EnemyLaserCommandView, opcode) == 0x10 &&
     offsetof(EnemyLaserCommandView, allowWhileActive) == 0x14) ? 1 : -1];

struct EnemyLaserPatternRecordView
{
    AnmVmTimerView timer;
    union
    {
        unsigned char unknown014[0x34 - 0x14];
        EnemyLaserWordView values[8];
    };
};
struct EnemyLaserCommonStorageView
{
    const void **vtable;
    EnemyLaserCommonStorageView *previous;
    EnemyLaserCommonStorageView *next;
    unsigned int unknown00C;
    AnmVmTimerView timer;
    unsigned char unknown024[0x54 - 0x24];
    unsigned int id;
    EnemyLaserPatternRecordView patterns[18];
    unsigned char unknown400[0x410 - 0x400];
    AnmVmTimerView finalTimer;
};
typedef char EnemyLaserCommonSizeIs424[
    (sizeof(EnemyLaserCommonStorageView) == 0x424) ? 1 : -1];
typedef char EnemyLaserCommonIdAt054[
    (offsetof(EnemyLaserCommonStorageView, id) == 0x54) ? 1 : -1];
typedef char EnemyLaserPatternSizeIs034[
    (sizeof(EnemyLaserPatternRecordView) == 0x34) ? 1 : -1];
// Complete storage view: +18 is sentinel.next, while +434 is the append tail.
// The callback-chain prefix remains opaque here; GameManager keeps its existing
// separately typed chain-prefix view at that boundary.
struct EnemyLaserManagerStorageView
{
    unsigned char unknown000[0x10];
    EnemyLaserCommonStorageView sentinel;
    EnemyLaserCommonStorageView *tail;
    int count;
    unsigned int nextId;
    PlayerFloat3 cancelPosition;
    unsigned char unknown44C[0x458 - 0x44c];
    AnmLoadedView *effectResource;
};
typedef char EnemyLaserManagerStorageLayout[
    (sizeof(EnemyLaserManagerStorageView) == 0x45c &&
     offsetof(EnemyLaserManagerStorageView, sentinel) == 0x10 &&
     offsetof(EnemyLaserManagerStorageView, sentinel) +
         offsetof(EnemyLaserCommonStorageView, next) == 0x18 &&
     offsetof(EnemyLaserManagerStorageView, tail) == 0x434 &&
     offsetof(EnemyLaserManagerStorageView, count) == 0x438 &&
     offsetof(EnemyLaserManagerStorageView, nextId) == 0x43c &&
     offsetof(EnemyLaserManagerStorageView, cancelPosition) == 0x440 &&
     offsetof(EnemyLaserManagerStorageView, effectResource) == 0x458) ? 1 : -1];
typedef char EnemyEffectWaitViewLayout[
    (offsetof(EnemyEffectWaitNode, next) == 0x08 &&
     offsetof(EnemyEffectWaitNode, state) == 0x0c &&
     offsetof(EnemyEffectWaitNode, flag50) == 0x50 &&
     offsetof(EnemyEffectWaitManager, head) == 0x18 &&
     offsetof(EnemyEffectWaitManager, cancelPosition) == 0x440) ? 1 : -1];

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
    union
    {
        unsigned int patternWords[0x6c];
        EnemyLaserCommandView commands[18];
    };
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
