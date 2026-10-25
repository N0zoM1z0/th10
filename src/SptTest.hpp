#ifndef TH10_RECONSTRUCTION_SPT_TEST_HPP
#define TH10_RECONSTRUCTION_SPT_TEST_HPP
#include "ScriptViewer.hpp"
struct EnemyEclHostBaseView;
struct EclVmScriptDatabase;
// Target-backed view of the separate Spt Test owner (allocation 0x2e4).
// Original class name and production TU remain unknown.
struct SptTestView
{
    unsigned int flags;
    unsigned int unknown004;
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    ScriptViewerWorkerView worker;
    unsigned int unknown02C;
    int state;
    char **scriptNames;
    int scriptCount;
    ScriptViewerCursorView menu;
    ScriptViewerCursorView files;
    ScriptViewerCursorView entries;
    AnmFloat3View position;
    int value2D0;
    unsigned int flags2D4;
    EclVmScriptDatabase *scriptDatabase;
    EnemyEclHostBaseView *activeVm;
    unsigned char *scriptData;
    SptTestView();
    ~SptTestView();
    int RegisterTasks();
    void ClearScriptNames();
    int Draw();
    int Update();
};
typedef char SptTestDrawLayout[(offsetof(SptTestView, state) == 0x30 &&
    offsetof(SptTestView, menu) == 0x3c &&
    offsetof(SptTestView, files) == 0x114 &&
    offsetof(SptTestView, entries) == 0x1ec &&
    offsetof(SptTestView, position) == 0x2c4 &&
    offsetof(SptTestView, scriptDatabase) == 0x2d8) ? 1 : -1];
typedef char SptTestFullLayout[(sizeof(SptTestView)==0x2e4 && offsetof(SptTestView,worker)==0x10 && offsetof(SptTestView,activeVm)==0x2dc && offsetof(SptTestView,scriptData)==0x2e0)?1:-1];
extern SptTestView *g_SptTestView;
SptTestView *SptTestCreate();
// Actual callback, original authored/outlined origin indeterminate.
int __fastcall SptTestUpdateCallback(void *context);
int __fastcall SptTestDrawCallback(void *context);
#endif
