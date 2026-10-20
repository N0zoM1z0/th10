#pragma once
#include "EnemyLaser.hpp"

// Known-slot view of vtable46DA10. This neither installs the original table
// nor replaces the separate D74-byte allocation/construction storage view.
struct EnemyLaserType1View
{
    virtual void RunCommands();
    virtual int Initialize(const EnemyLaserType1RequestView *initial);
    virtual int Update();
    virtual int Draw();
    virtual void UnknownSlot10();
    virtual int Clear(int spawnItems);
    virtual int CheckCollisionBox(EnemyLaserVectorView *, EnemyLaserVectorView *, int);
    virtual void UnknownSlot1C();
    virtual void UnknownSlot20();

    void *previous;
    void *next;
    unsigned int state;
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
    EnemyLaserType1RequestView request;
    AnmVmView primaryVm;
    AnmVmView secondaryVm;
};
typedef char EnemyLaserType1RuntimeLayout[
    (sizeof(EnemyLaserType1View) == 0xd74 &&
     offsetof(EnemyLaserType1View, state) == 0xc &&
     offsetof(EnemyLaserType1View, age) == 0x10 &&
     offsetof(EnemyLaserType1View, position) == 0x24 &&
     offsetof(EnemyLaserType1View, soundId) == 0x40c &&
     offsetof(EnemyLaserType1View, request) == 0x424 &&
     offsetof(EnemyLaserType1View, primaryVm) == 0x61c &&
     offsetof(EnemyLaserType1View, secondaryVm) == 0x9c8) ? 1 : -1];
