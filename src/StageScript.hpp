#pragma once
#include "AnmManager.hpp"

// Partial STD-stage views recovered from the script/update owners. Unnamed
// storage and the global camera aggregate retain unresolved ownership.
struct StageInstructionView
{
    int time;
    short opcode;
    short byteSize;
    union { int integers[11]; float reals[11]; } args;
};
struct StageFogView
{
    float start, end, blue, green, red, alpha;
    unsigned int color;
    StageFogView() {}
    StageFogView(float start, float end, float blue, float green, float red, float alpha);
    StageFogView operator-(const StageFogView &other) const;
    StageFogView operator+(const StageFogView &other) const;
    StageFogView operator*(float scale) const;
};
struct StageFogInterpolationView
{
    StageFogView initial, final, initialTangent, finalTangent;
    AnmVmTimerView timer;
    int duration, mode;
    // Target 0x004049A0: EBX receiver, stack output, RET4. Emitted ABI and
    // extended-precision conversion behavior require independent comparison.
    StageFogView *Evaluate(StageFogView *output);
};
struct StageCameraStateView
{
    AnmFloat3View position;
    AnmFloat3View direction;
    AnmFloat3View up;
    AnmFloat3View normalizedDirection;
    unsigned char unknown030[0x0c];
    float rockingOffset;
    unsigned char unknown040[8];
    float value048;
    unsigned char unknown04C[0x9c];
    int value0E8, value0EC;
    AnmFloat3View positionDelta;
    StageFogView fog;
};
struct StageObjectPrimitiveView
{
    short type, byteSize, unknown004, vmIndex;
};
struct StageObjectView
{
    short id;
    unsigned char layer, flags;
    AnmFloat3View position, extent;
    StageObjectPrimitiveView firstPrimitive;
};
struct StageDataHeaderView { short objectCount; };
struct StageScriptView
{
    unsigned char unknown000[0x10];
    StageDataHeaderView *data;
    StageObjectView **objects;
    void *instances;
    unsigned char *scriptBase;
    unsigned char rockingMode;
    unsigned char unknown021[3];
    AnmVmTimerView rockingTimer;
    AnmVmTimerView scriptTimer;
    StageInstructionView *instruction;
    AnmVmFloat3InterpolationView directionInterpolation;
    AnmVmFloat3InterpolationView positionInterpolation;
    StageFogInterpolationView fogInterpolation;
    unsigned char unknown174[4];
    AnmLoadedView *anm;
    AnmVmView *objectVms;
    AnmVmView scriptVms[8];
    float cullDistanceSquared;
    int unknown1EE4;
    unsigned int tint;
    int auxiliaryActive;
    AnmFloat3View auxiliaryPosition, auxiliaryDirection;
    AnmVmView auxiliaryVms[3];
    int visibleObjectCount, culledObjectCount;
    int drawCount;
    unsigned int flags;
    AnmVmTimerView transitionTimer;
    int unknown2A30;
    int frame;
    unsigned char unknown2A38[0x14];
    StageCameraStateView camera;

    int UpdateObjectVms();
    int Update();
};
int __stdcall StageExecuteScript(StageScriptView *stage);

typedef char StageScriptLayoutChecks[
    (offsetof(StageInstructionView, args) == 8 &&
     sizeof(StageFogView) == 0x1c && sizeof(StageFogInterpolationView) == 0x8c &&
     offsetof(StageFogView, color) == 0x18 &&
     offsetof(StageFogInterpolationView, timer) == 0x70 &&
     offsetof(StageFogInterpolationView, duration) == 0x84 &&
     sizeof(StageCameraStateView) == 0x118 &&
     offsetof(StageCameraStateView, fog) == 0xfc &&
     offsetof(StageObjectView, firstPrimitive) == 0x1c &&
     offsetof(StageScriptView, instruction) == 0x4c &&
     offsetof(StageScriptView, directionInterpolation) == 0x50 &&
     offsetof(StageScriptView, positionInterpolation) == 0x9c &&
     offsetof(StageScriptView, fogInterpolation) == 0xe8 &&
     offsetof(StageScriptView, anm) == 0x178 &&
     offsetof(StageScriptView, scriptVms) == 0x180 &&
     offsetof(StageScriptView, tint) == 0x1ee8 &&
     offsetof(StageScriptView, auxiliaryVms) == 0x1f08 &&
     offsetof(StageScriptView, flags) == 0x2a18 &&
     offsetof(StageScriptView, frame) == 0x2a34 &&
     offsetof(StageScriptView, camera) == 0x2a4c &&
     sizeof(StageScriptView) == 0x2b64) ? 1 : -1];
