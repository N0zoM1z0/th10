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
    th10::g_Chain->AddToCalcChain(task, 5);
    updateTask = task;
    task = th10::Chain::AllocElem(SptTestDrawCallback);
    task->flags |= 2;
    task->arg = this;
    th10::g_Chain->AddToDrawChain(task, 0x27);
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
// Destructor42BC30 remains an explicit unresolved dependency; its complete
// first proposal is nonexact and is not admitted with these helpers.

#include "Enemy.hpp"
#include "InputState.hpp"
// The real loader callback42BE10 ignores the context; declared dependency only.
extern int SptTestLoadCallback();
static inline void SelectScriptViewerEntry(FrontEndCursorView &cursor, int value)
{
    if (cursor.count != 0)
        cursor.current = value >= cursor.count ? cursor.count - 1 : value;
    else
        cursor.current = 0;
}

static inline void RememberScriptViewerCursor(FrontEndCursorView *cursor)
{
    cursor->previous = cursor->current;
}


int SptTestView::Update()
{
    WIN32_FIND_DATAA find;
    switch (state)
    {
    case 4:
        reinterpret_cast<EclVmHost *>(activeVm)->Run(1.0f);
        if (!activeVm || !activeVm->activeEclContext->currentInstruction)
        {
            if (activeVm) delete activeVm;
            activeVm = NULL;
            state = 1;
        }
        break;
    case 1:
        RememberScriptViewerCursor(&menu);
        if (g_FrontEndInput.IsActiveInline(0x20)) menu.Move(1);
        if (g_FrontEndInput.IsActiveInline(0x10)) menu.Move(-1);
        switch (menu.current)
        {
        case 2:
            if (g_FrontEndInput.pressed & 0x1001)
                g_MainSupervisorView.SetNextGameMode(3);
            break;
        case 1:
            if (scriptDatabase)
            {
                RememberScriptViewerCursor(&entries);
                if (static_cast<unsigned short>(g_FrontEndInput.IsRepeated(0x80))) entries.Move(1);
                if (static_cast<unsigned short>(g_FrontEndInput.IsRepeated(0x40))) entries.Move(-1);
                if (g_FrontEndInput.mask008 & 0x1001)
                {
                    if (activeVm)
                    {
                        delete activeVm;
                        activeVm = NULL;
                    }
                    activeVm = scriptDatabase->CreateHost(scriptDatabase->subroutines[entries.current].name);
                    state = 4;
                }
            }
            break;
        case 0:
            RememberScriptViewerCursor(&files);
            if (g_FrontEndInput.IsActiveInline(0x80)) files.Move(1);
            if (g_FrontEndInput.IsActiveInline(0x40)) files.Move(-1);
            if (g_FrontEndInput.pressed & 0x1001)
            {
                flags2D4 &= ~2u;
                worker.Start(this, SptTestLoadCallback);
                state = 2;
            }
            break;
        }
        break;
    case 0:
        ClearScriptNames();
        int count = 0;
        HANDLE search = FindFirstFileA("../../data/*.ecl", &find);
        if (search != INVALID_HANDLE_VALUE)
        {
            do { ++count; } while (FindNextFileA(search, &find));
        }
        FindClose(search);
        scriptCount = count;
        if (count)
        {
            scriptNames = static_cast<char **>(malloc(count * sizeof(char *)));
            search = FindFirstFileA("../../data/*.ecl", &find);
            for (int i = 0; i < count; ++i)
            {
                scriptNames[i] = static_cast<char *>(malloc(strlen(find.cFileName) + 1));
                strcpy(scriptNames[i], find.cFileName);
                if (!FindNextFileA(search, &find)) break;
            }
            FindClose(search);
        }
        menu.count = 3;
        menu.wraps = 1;
        SelectScriptViewerEntry(menu, 0);
        files.count = count;
        files.wraps = 1;
        SelectScriptViewerEntry(files, 0);
        entries.count = 1;
        entries.wraps = 1;
        SelectScriptViewerEntry(entries, 0);
        state = 1;
        position.x = 0.0f;
        position.y = 32.0f;
        position.z = 0.0f;
        value2D0 = 1000;
        break;
    }
    return 1;
}

int __fastcall SptTestUpdateCallback(void *context)
{
    return static_cast<SptTestView *>(context)->Update();
}
