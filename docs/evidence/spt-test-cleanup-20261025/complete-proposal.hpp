#ifndef TH10_RECONSTRUCTION_SPT_TEST_HPP
#define TH10_RECONSTRUCTION_SPT_TEST_HPP
#include "ScriptViewer.hpp"
#include "EnemyEclResourceBase.hpp"
struct EnemyEclHostBaseView;
struct EclVmScriptDatabase;
// Provisional view of the separate "Spt Test" display owner. Only consumed
// offsets are established; original class, allocation size and TU are unknown.
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
    unsigned char unknown2D0[8];
    EnemyEclResourceBaseView *scriptDatabase;
    EnemyEclHostBaseView *activeVm;
    unsigned char *scriptData;
    SptTestView();
    ~SptTestView();
    int RegisterTasks();
    void ClearScriptNames();
    int Draw();
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
// Actual registered callback42C580: body926 remains an explicit dependency.
int __fastcall SptTestUpdateCallback(void *context);
int __fastcall SptTestDrawCallback(void *context);
#endif
