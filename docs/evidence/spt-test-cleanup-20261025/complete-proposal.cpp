#include "Main.hpp"
#include "SptTest.hpp"
#include "EclVm.hpp"

// 0x42C360: this is not the ScriptViewer owner at 0x40A940.
int SptTestView::Draw()
{
    AnmFloat3View textPosition(0.0f, 0.0f, 0.0f);
    g_AsciiManagerView->AddSmallFormatText(&textPosition, "Spt Test\n");
    switch (state)
    {
    case 2:
        g_AsciiManagerView->color = 0xffff4040;
        textPosition.x = 32.0f;
        textPosition.y = 16.0f;
        textPosition.z = 0.0f;
        g_AsciiManagerView->AddSmallFormatText(
            &textPosition, "Loading %s", scriptNames[files.current]);
        g_AsciiManagerView->color = 0xffffffff;
        return 1;
    case 1:
        textPosition.x = 42.0f;
        textPosition.y = 16.0f;
        textPosition.z = 0.0f;
        if (scriptCount)
            g_AsciiManagerView->AddSmallFormatText(
                &textPosition, "File %s", scriptNames[files.current]);
        else
            g_AsciiManagerView->AddSmallFormatText(&textPosition, "File not found.");
        textPosition.y = 26.0f;
        if (scriptDatabase)
            g_AsciiManagerView->AddSmallFormatText(
                &textPosition, "Ecl %s", scriptDatabase->subroutines[entries.current].name);
        else
            g_AsciiManagerView->AddSmallFormatText(&textPosition, "Ecl not load");
        textPosition.y = 36.0f;
        g_AsciiManagerView->AddSmallFormatText(&textPosition, "Quit");
        textPosition.x = 32.0f;
        textPosition.y = menu.current * 10.0f + 16.0f;
        g_AsciiManagerView->AddSmallFormatText(&textPosition, ">");
        g_AsciiManagerView->color = 0xffffffff;
        break;
    case 3:
        {
            g_AsciiManagerView->color = 0xffa0a0a0;
            AnmFloat3View statusPosition(32.0f, 16.0f, 0.0f);
            g_AsciiManagerView->AddSmallFormatText(
                &statusPosition, "Pos %.3d %.3d",
                static_cast<int>(position.x), static_cast<int>(position.y));
            g_AsciiManagerView->color = 0xffffffff;
            return 1;
        }
    }
    return 1;
}

int __fastcall SptTestDrawCallback(void *context)
{
    return static_cast<SptTestView *>(context)->Draw();
}

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
