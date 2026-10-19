#ifndef TH10_RECONSTRUCTION_ENDING_HPP
#define TH10_RECONSTRUCTION_ENDING_HPP
#include "AnmManager.hpp"
#include "ScriptViewer.hpp"
#include "Chain.hpp"
#include <string.h>
#include <limits.h>

struct EndingView;
extern EndingView *g_EndingView; // target477700; storage and original type open.

// Serialized variable-length records: header4, payload length at+3. Only
// integer-bearing operations require four bytes for each consumed operand.
inline unsigned short EndingRecordTime(const unsigned char *p)
{
    unsigned short value;
    memcpy(&value, p, sizeof(value));
    return value;
}
inline int EndingRecordInt(const unsigned char *p, int operand = 0)
{
    int value;
    memcpy(&value, p + 4 + operand * 4, sizeof(value));
    return value;
}
inline unsigned char *EndingRecordNext(unsigned char *p)
{
    return p + 4 + p[3];
}

typedef char EndingRecordScalarWidths[
    (CHAR_BIT == 8 && sizeof(unsigned short) == 2 && sizeof(int) == 4) ? 1 : -1];

struct EndingTextVmView
{
    int unknown000;
    AnmVmTimerView frameTimer;
    AnmVmTimerView instructionTimer;
    AnmVmTimerView waitTimer;
    AnmVmIdView textIds[5];
    unsigned char *instruction;
    unsigned char unknown058[0x18];
    const char *backgroundPath;
    unsigned int flags;
    int textLine;
    unsigned int color;
    AnmLoadedView *resources[4];
    // Descriptive interval before the worker, not proof of original capacity.
    AnmVmIdView animationIds[16];
    ScriptViewerWorkerView worker;

    explicit EndingTextVmView(unsigned char *command);
    ~EndingTextVmView();
    int Tick();
    int __stdcall Run(); // observed one stack receiver and RET4; original spelling unknown.
    int LoadAnimation();
};
typedef char EndingTextLayout[
    (sizeof(EndingTextVmView) == 0xec &&
     offsetof(EndingTextVmView, instructionTimer) == 0x18 &&
     offsetof(EndingTextVmView, textIds) == 0x40 &&
     offsetof(EndingTextVmView, instruction) == 0x54 &&
     offsetof(EndingTextVmView, backgroundPath) == 0x70 &&
     offsetof(EndingTextVmView, flags) == 0x74 &&
     offsetof(EndingTextVmView, resources) == 0x80 &&
     offsetof(EndingTextVmView, animationIds) == 0x90 &&
     offsetof(EndingTextVmView, worker) == 0xd0) ? 1 : -1];

struct EndingView
{
    unsigned int flags000;
    unsigned int unknown004;
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    unsigned int unknown010;
    unsigned char *scriptData;
    EndingTextVmView *text;
    int endingIndex;
    unsigned int flags;
    int skipCounter;

    EndingView()
    {
        memset(this, 0, sizeof(*this));
        flags000 |= 2;
        g_EndingView = this;
    }
    ~EndingView();
    int Initialize();
    unsigned char *LoadScript(const char *path);
    int Update();
};
typedef char EndingLayout[
    (sizeof(EndingView) == 0x28 && offsetof(EndingView, scriptData) == 0x14 &&
     offsetof(EndingView, text) == 0x18 && offsetof(EndingView, flags) == 0x20) ? 1 : -1];

EndingView *EndingCreate();
void EndingDelete(EndingView *ending);
int __fastcall EndingUpdateCallback(EndingView *ending);
int EndingLoadAnimationCallback();
#endif
