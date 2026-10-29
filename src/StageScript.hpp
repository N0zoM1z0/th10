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
    union { unsigned int color; AnmColorView packedColor; };
    StageFogView() {}
    // Initialize the six channels and their derived packed color in place.
    void Set(float start, float end, float blue, float green,
        float red, float alpha)
    {
        this->start = start;
        this->end = end;
        this->blue = blue;
        this->green = green;
        this->red = red;
        this->alpha = alpha;
        unsigned char *bytes = reinterpret_cast<unsigned char *>(&color);
        bytes[0] = static_cast<unsigned char>(static_cast<int>(blue));
        bytes[1] = static_cast<unsigned char>(static_cast<int>(green));
        bytes[2] = static_cast<unsigned char>(static_cast<int>(red));
        bytes[3] = static_cast<unsigned char>(static_cast<int>(alpha));
    }
    StageFogView(float start, float end, float blue, float green, float red, float alpha);
    StageFogView *SubtractTo(StageFogView &output, const StageFogView &other) const;
    StageFogView *AddTo(StageFogView &output, const StageFogView &other) const;
    StageFogView *ScaleTo(StageFogView &output, float scale) const;
};
typedef char StageFogSizeIs1C[
    (sizeof(StageFogView) == 0x1c) ? 1 : -1];
typedef char StageFogColorAt18[
    (offsetof(StageFogView, color) == 0x18 &&
     offsetof(StageFogView, packedColor) == 0x18) ? 1 : -1];
struct StageFogInterpolationView
{
    StageFogView initial, final, initialTangent, finalTangent;
    AnmVmTimerView timer;
    int duration, mode;
    // Target 0x004049A0: EBX receiver, stack output and RET4. Each result
    // remains a full six-channel value plus its derived packed color.
    StageFogView *Evaluate(StageFogView *output);
};
struct StageCameraStateView
{
    void ConfigureCamera();
    void InstallViewport();
    AnmFloat3View position;
    AnmFloat3View direction;
    AnmFloat3View up;
    AnmFloat3View normalizedDirection;
    AnmFloat3View cameraRight;
    AnmFloat3View rockingOffset;
    float value048;
    AnmMatrixView viewMatrix, projectionMatrix;
    AnmViewportView viewport;
    unsigned char unknown0E4[4];
    float value0E8, value0EC;
    AnmFloat3View positionDelta;
    StageFogView fog;
};
struct StageObjectPrimitiveView
{
    short type, byteSize, unknown004, vmIndex;
};
struct StageObjectView
{
    int BoundsCulled(const AnmFloat3View *position,
        const StageCameraStateView *camera, float distanceSquared) const;
    short id;
    unsigned char layer, flags;
    AnmFloat3View position, extent;
    StageObjectPrimitiveView firstPrimitive;
};
struct StageDataHeaderView
{
    short objectCount, objectVmCount;
    unsigned int instancesOffset, scriptOffset, unknown00C;
    char anmPath[0x80];
};
namespace th10 { struct ChainElem; }
struct StageInstanceView
{
    short objectIndex, unknown002;
    AnmFloat3View position;
};
struct StageQuadPrimitiveView
{
    StageObjectPrimitiveView header;
    AnmFloat3View position;
    float width, height;
};
struct StageScriptView
{
    unsigned int lifecycleFlags, unknown004;
    th10::ChainElem *updateChain, *earlyDrawChain;
    StageDataHeaderView *data;
    StageObjectView **objects;
    StageInstanceView *instances;
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
    unsigned int unknown2A38, unknown2A3C;
    th10::ChainElem *lateDrawChain;
    unsigned char *originalData;
    int originalDataSize;
    StageCameraStateView camera;

    StageScriptView();
    ~StageScriptView();
    int Initialize(const char *path, int priorityOffset);
    int LoadData(const char *path);
    int UpdateObjectVms();
    int Update();
};
StageScriptView *__stdcall StageCreate(const char *path, int priorityOffset);
int __fastcall StageUpdateCallback(StageScriptView *stage);
int __stdcall StageExecuteScript(StageScriptView *stage);
int __stdcall StageDrawInstances(StageScriptView *stage, int layer);
int __stdcall StageDrawEarly(StageScriptView *stage);
int __stdcall StageDrawLate(StageScriptView *stage);
int __fastcall StageDrawEarlyCallback(StageScriptView *stage);
int __fastcall StageDrawLateCallback(StageScriptView *stage);

typedef char StageScriptLayoutChecks[
    (offsetof(StageInstructionView, args) == 8 &&
     sizeof(StageFogView) == 0x1c && sizeof(StageFogInterpolationView) == 0x8c &&
     offsetof(StageFogView, color) == 0x18 &&
     offsetof(StageFogInterpolationView, timer) == 0x70 &&
     offsetof(StageFogInterpolationView, duration) == 0x84 &&
     sizeof(StageCameraStateView) == 0x118 &&
     offsetof(StageCameraStateView, cameraRight) == 0x30 &&
     offsetof(StageCameraStateView, rockingOffset) == 0x3c &&
     offsetof(StageCameraStateView, value0E8) == 0xe8 &&
     offsetof(StageCameraStateView, value0EC) == 0xec &&
     offsetof(StageCameraStateView, viewMatrix) == 0x4c &&
     offsetof(StageCameraStateView, projectionMatrix) == 0x8c &&
     offsetof(StageCameraStateView, viewport) == 0xcc &&
     sizeof(StageInstanceView) == 0x10 &&
     offsetof(StageInstanceView, position) == 4 &&
     offsetof(StageQuadPrimitiveView, position) == 8 &&
     offsetof(StageQuadPrimitiveView, width) == 0x14 &&
     sizeof(StageQuadPrimitiveView) == 0x1c &&
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
     sizeof(StageDataHeaderView) == 0x90 &&
     offsetof(StageScriptView, updateChain) == 8 &&
     offsetof(StageScriptView, earlyDrawChain) == 0xc &&
     offsetof(StageScriptView, lateDrawChain) == 0x2a40 &&
     offsetof(StageScriptView, originalData) == 0x2a44 &&
     offsetof(StageScriptView, originalDataSize) == 0x2a48 &&
     sizeof(StageScriptView) == 0x2b64) ? 1 : -1];
