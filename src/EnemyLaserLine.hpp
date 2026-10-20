#pragma once
#include "EnemyLaser.hpp"

// Known-slot partial interface of the existing type-0 object, vtable46DA60.
// This does not replace the external table or close C++ construction ownership.
struct EnemyLaserLineView
{
    virtual void RunCommands();
    virtual int Initialize(const EnemyLaserType0RequestView *initial);
    virtual int Update();
    virtual int Draw();
    virtual void UnknownSlot10();
    virtual void UnknownSlot14();
    virtual int CheckCollisionBox(EnemyLaserVectorView *, EnemyLaserVectorView *, int);
    virtual void UnknownSlot1C();
    virtual void UnknownSlot20();
    virtual void Motion0001();
    virtual void Motion0010();
    virtual void Motion0020();
    virtual void Motion0040();
    virtual void Motion0100();
    virtual void Motion0080();
    virtual void Bounce();
    virtual void Motion100000();
    virtual void Motion200000();
    virtual void Motion4000000();

    void *previous;
    void *next;
    unsigned int unknown00C;
    AnmVmTimerView age;
    EnemyLaserVectorView position;
    EnemyLaserVectorView velocity;
    float angle;
    float length;
    float width;
    float growSpeed;
    float traveled;
    unsigned char deletionCounter;
    unsigned char unknown051[3];
    unsigned int id;
    EnemyLaserPatternRecordView patterns[18];
    int commandIndex;
    unsigned int flags;
    unsigned int unknown408;
    int soundId;
    AnmVmTimerView holdTimer;
    EnemyLaserType0RequestView request;
    AnmVmView primaryVm;
    AnmVmView secondaryVm;
};
typedef char EnemyLaserLineLayout[
    (sizeof(EnemyLaserLineView) == 0xd58 &&
     offsetof(EnemyLaserLineView, age) == 0x10 &&
     offsetof(EnemyLaserLineView, position) == 0x24 &&
     offsetof(EnemyLaserLineView, velocity) == 0x30 &&
     offsetof(EnemyLaserLineView, flags) == 0x404 &&
     offsetof(EnemyLaserLineView, holdTimer) == 0x410 &&
     offsetof(EnemyLaserLineView, request) == 0x424 &&
     offsetof(EnemyLaserLineView, primaryVm) == 0x600 &&
     offsetof(EnemyLaserLineView, secondaryVm) == 0x9ac) ? 1 : -1];

struct EnemyLaserAccelerationRecordView
{
    AnmVmTimerView timer;
    float growAcceleration;
    unsigned int unknown018;
    EnemyLaserVectorView velocityAcceleration;
    int duration;
    unsigned char unknown02C[8];
};
typedef char EnemyLaserAccelerationRecordLayout[
    (sizeof(EnemyLaserAccelerationRecordView) == 0x34 &&
     offsetof(EnemyLaserAccelerationRecordView, growAcceleration) == 0x14 &&
     offsetof(EnemyLaserAccelerationRecordView, velocityAcceleration) == 0x1c &&
     offsetof(EnemyLaserAccelerationRecordView, duration) == 0x28) ? 1 : -1];
