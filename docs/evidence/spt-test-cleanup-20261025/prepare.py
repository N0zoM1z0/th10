from pathlib import Path
import shutil,json,hashlib
root=Path.cwd();p=root/'.analysis/spt-test-cleanup-20261025';d=p/'candidate';assert not d.exists();d.mkdir()
for f in (root/'src').iterdir():
 if f.is_file():shutil.copy2(f,d/f.name)
f=d/'Enemy.cpp';b=f.read_bytes();a=b.index(b'// TH10\'s 0x1098-byte Enemy ECL resource');z=b.index(b'struct EnemyEclResourceView :',a);part=b[a:z]
(d/'EnemyEclResourceBase.hpp').write_bytes(b'#pragma once\r\n#include "EclVm.hpp"\r\n#include <stdlib.h>\r\n#include <string.h>\r\n'+part);f.write_bytes(b[:a]+b'#include "EnemyEclResourceBase.hpp"\r\n\r\n'+b[z:])
f=d/'ScriptViewer.cpp';b=f.read_bytes();a=b.index(b'ScriptViewerWorkerView::ScriptViewerWorkerView()');z=b.index(b'ScriptViewerView::ScriptViewerView()',a);ctor=b[a:z];f.write_bytes(b[:a]+b[z:]);f=d/'ScriptViewerWorker.cpp';f.write_bytes(f.read_bytes()+b'\n'+ctor)
f=d/'SptTest.hpp';s=f.read_text().replace('#include "FrontEnd.hpp"','#include "ScriptViewer.hpp"\n#include "EnemyEclResourceBase.hpp"\nstruct EnemyEclHostBaseView;').replace('    unsigned char unknown000[0x30];','    unsigned int flags;\n    unsigned int unknown004;\n    th10::ChainElem *updateTask;\n    th10::ChainElem *drawTask;\n    ScriptViewerWorkerView worker;\n    unsigned int unknown02C;').replace('FrontEndCursorView ','ScriptViewerCursorView ').replace('    EclVmScriptDatabase *scriptDatabase;','    EnemyEclResourceBaseView *scriptDatabase;\n    EnemyEclHostBaseView *activeVm;\n    unsigned char *scriptData;\n    SptTestView();\n    ~SptTestView();\n    int RegisterTasks();\n    void ClearScriptNames();').replace('int __fastcall SptTestDrawCallback','typedef char SptTestFullLayout[(sizeof(SptTestView)==0x2e4 && offsetof(SptTestView,worker)==0x10 && offsetof(SptTestView,activeVm)==0x2dc && offsetof(SptTestView,scriptData)==0x2e0)?1:-1];\nextern SptTestView *g_SptTestView;\nSptTestView *SptTestCreate();\n// Actual registered callback42C580: body926 remains an explicit dependency.\nint __fastcall SptTestUpdateCallback(void *context);\nint __fastcall SptTestDrawCallback');f.write_text(s)
f=d/'SptTest.cpp';s=f.read_text();s+='''
#include "Main.hpp"
#include "Enemy.hpp"
#include <stdlib.h>
#include <string.h>
SptTestView::SptTestView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2;
    g_SptTestView = this;
}
int SptTestView::RegisterTasks()
{
    th10::ChainElem *task = th10::Chain::AllocElem(SptTestUpdateCallback);
    task->flags |= 2;
    task->arg = this;
    th10::Chain::AddToCalcChain(task, 5, th10::g_Chain);
    updateTask = task;
    task = th10::Chain::AllocElem(SptTestDrawCallback);
    task->flags |= 2;
    task->arg = this;
    th10::Chain::AddToDrawChain(task, 0x27, th10::g_Chain);
    drawTask = task;
    return 0;
}
SptTestView *SptTestCreate()
{
    SptTestView *view = new SptTestView;
    if (view->RegisterTasks() != 0)
    {
        delete view;
        return NULL;
    }
    return view;
}
void SptTestView::ClearScriptNames()
{
    if (scriptNames)
    {
        for (int i = 0; i < scriptCount; ++i)
            if (scriptNames[i])
            {
                free(scriptNames[i]);
                scriptNames[i] = NULL;
            }
        if (scriptNames)
        {
            free(scriptNames);
            scriptNames = NULL;
        }
        scriptNames = NULL;
    }
}
SptTestView::~SptTestView()
{
    ClearScriptNames();
    th10::ChainElem *task = updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(task, chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(task, chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    if (scriptData)
    {
        free(scriptData);
        scriptData = NULL;
    }
    if (scriptDatabase)
    {
        delete scriptDatabase;
        scriptDatabase = NULL;
    }
    if (activeVm)
    {
        delete activeVm;
        activeVm = NULL;
    }
    g_AnmRenderManagerView->ReleaseAnm(7);
    g_AnmRenderManagerView->ReleaseAnm(6);
    g_SptTestView = NULL;
}
''';f.write_text(s)
print('prepared complete isolated natural family; shared declarations',len(part),'worker ctor',len(ctor))
