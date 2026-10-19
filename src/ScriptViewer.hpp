#ifndef TH10_RECONSTRUCTION_SCRIPT_VIEWER_HPP
#define TH10_RECONSTRUCTION_SCRIPT_VIEWER_HPP

#include "FrontEnd.hpp"
#include "Chain.hpp"

// Descriptive views of the debug script browser. Original class names and
// production translation-unit ownership remain unknown. Layout is established
// by the TH10 constructor, update body and loader, not by the reference project.
struct ScriptViewerWorkerView
{
    void *thread;
    unsigned int threadId;
    int stopRequested;
    int running;
    unsigned int unknown014;
    void *entry018;
    unsigned int unknown01C;
    ScriptViewerWorkerView();
    virtual ~ScriptViewerWorkerView();
    __declspec(noinline) void Stop();
    __declspec(noinline) void Start(void *context, int (*entry)());
};

struct ScriptViewerCursorView : FrontEndCursorView
{
    ScriptViewerCursorView()
    {
        saveDepth = 0;
        current = 0;
        disabledEntryCount = 0;
        wraps = 1;
        count = 999;
    }
};

struct ScriptViewerView
{
    unsigned int unknown000;
    unsigned int unknown004;
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    ScriptViewerWorkerView worker;
    int state;
    char **scriptNames;
    int scriptCount;
    ScriptViewerCursorView menu;
    ScriptViewerCursorView files;
    ScriptViewerCursorView entries;
    unsigned int unknown2C4;
    AnmVmView vm;
    AnmFloat3View position;
    int value680;
    unsigned int flags;

    __declspec(noinline) ScriptViewerView();
    ~ScriptViewerView();
    __declspec(noinline) int RegisterTasks();
    __declspec(noinline) int InitializeManagers();
    __declspec(noinline) void ClearScriptNames();
    __declspec(noinline) int Update();
    int Draw();
};

typedef char ScriptViewerLayout[
    (sizeof(ScriptViewerView) == 0x688 &&
     offsetof(ScriptViewerView, worker) == 0x10 &&
     offsetof(ScriptViewerView, state) == 0x30 &&
     offsetof(ScriptViewerView, menu) == 0x3c &&
     offsetof(ScriptViewerView, files) == 0x114 &&
     offsetof(ScriptViewerView, entries) == 0x1ec &&
     offsetof(ScriptViewerView, vm) == 0x2c8 &&
     offsetof(ScriptViewerView, flags) == 0x684) ? 1 : -1];

extern ScriptViewerView *g_ScriptViewer;
ScriptViewerView *ScriptViewerCreate();
int __fastcall ScriptViewerUpdateCallback(void *context);
int __fastcall ScriptViewerDrawCallback(void *context);

#endif
