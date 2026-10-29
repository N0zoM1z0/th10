#pragma once

#include "AnmManager.hpp"
#include "Chain.hpp"
#include "ScriptViewerWorker.hpp"
#include <stddef.h>

// Partial view of the owner published at477820. Names are descriptive; the
// startup callback ignores the argument supplied by its thread launcher.
struct StartupWorkerView
{
    unsigned int flags;
    unsigned int unknown004;
    th10::ChainElem *calcElement;
    th10::ChainElem *drawElement;
    ScriptViewerWorkerView worker;
    unsigned int unknown02C;
    AnmVmView vm;
    int signatureVmId;
    AnmLoadedView *signatureAnm;
    int signatureReady;
    int textReady;
    int frames;

    StartupWorkerView();
    ~StartupWorkerView();
    int RegisterTasks();
    int __stdcall Draw();
    static int StartupThread();
    static int __fastcall OnUpdate(StartupWorkerView *owner);
    static int __fastcall OnDraw(StartupWorkerView *owner);
};

typedef char StartupWorkerSignatureAt3E0[
    (offsetof(StartupWorkerView, worker) == 0x10 &&
     offsetof(StartupWorkerView, vm) == 0x30 &&
     offsetof(StartupWorkerView, signatureVmId) == 0x3dc &&
     offsetof(StartupWorkerView, signatureAnm) == 0x3e0 &&
     offsetof(StartupWorkerView, frames) == 0x3ec &&
     sizeof(StartupWorkerView) == 0x3f0) ? 1 : -1];

extern StartupWorkerView *g_StartupWorker;
extern char g_StartupFrontError[];
