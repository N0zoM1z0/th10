#include "Main.hpp"
#include "ScriptViewer.hpp"
#include "ScriptViewerOwners.hpp"
#include "ItemManager.hpp"
#include "BulletManager.hpp"
#include "InputState.hpp"
#include "Enemy.hpp"
#include "EclVm.hpp"
#include "Gui.hpp"
#include <stdlib.h>
#include <string.h>

// Adapted in part from levzzz5154/th10-decomp (MIT), copyright (c) 2026 levzzz,
// commit c6791d82dc9f02e55eca45a5314f900a85481efd. See docs/licenses/th10-decomp-MIT.txt.
// The executable, fresh local Ghidra listing and maintained TH10 interfaces
// determine these bodies. In particular task registration sets bit 2, whereas
// that reference's registration candidate sets bit 1.
extern void *g_ScriptViewerStageRoute;                  // 0x477848
extern unsigned char g_ScriptViewerRouteDescriptor[];   // 0x474788
extern int g_ScriptViewerStage;                        // 0x474C7C
extern int g_ScriptViewerShownStage;                   // 0x474C80

ScriptViewerWorkerView::ScriptViewerWorkerView()
{
    thread = NULL;
    threadId = 0;
    stopRequested = 0;
    running = 0;
}

ScriptViewerView::ScriptViewerView()
{
    memset(this, 0, sizeof(*this));
    unknown000 |= 2;
    g_ScriptViewer = this;
}

int ScriptViewerView::RegisterTasks()
{
    g_ScriptViewerStageRoute = g_ScriptViewerRouteDescriptor;
    g_ScriptViewerStage = 0;
    g_ScriptViewerShownStage = 0;
    th10::ChainElem *task = th10::Chain::AllocElem(ScriptViewerUpdateCallback);
    task->flags |= 2;
    task->arg = this;
    th10::Chain::AddToCalcChain(task, 5, th10::g_Chain);
    updateTask = task;
    task = th10::Chain::AllocElem(ScriptViewerDrawCallback);
    task->flags |= 2;
    task->arg = this;
    th10::Chain::AddToDrawChain(task, 0x27, th10::g_Chain);
    drawTask = task;
    return 0;
}

ScriptViewerView *ScriptViewerCreate()
{
    ScriptViewerView *view = new ScriptViewerView;
    if (view->RegisterTasks() != 0)
    {
        delete view;
        return NULL;
    }
    return view;
}

// These symbols describe target storage aliases and unreconstructed callees;
// they do not establish storage ownership or a runnable product link.
struct ScriptViewerChainOwnerView
{
    unsigned char unknown00[8];
    th10::ChainElem *update;
    th10::ChainElem *draw;
};
extern BulletManagerView *g_ScriptViewerBullets;         // 0x4776F0
extern EnemyManagerView *g_EnemyManager;                 // 0x477704
extern GuiView *g_GuiView;                               // 0x47770C
extern void *g_ScriptViewerBomb;            // 0x4776EC
extern void *g_ScriptViewerPopups;          // 0x477840
extern void ScriptViewerReleaseGui(GuiView *owner);                         // 0x4148E0
extern void ScriptViewerReleasePlayer(Player *owner);                      // 0x425090
extern void ScriptViewerReleaseBullets(BulletManagerView *owner);          // 0x406140
extern void ScriptViewerReleaseBomb(void *owner);                          // 0x405730
extern void ScriptViewerReleaseItems(ItemManagerView *owner);              // 0x41AFB0
extern void ScriptViewerReleasePopups(void *owner);                        // 0x42B6D0
extern void ScriptViewerReleaseEnemies(EnemyManagerView *owner);           // 0x40D730
extern int ScriptViewerLoadThread(); // 0x40A340
extern void *ScriptViewerCreateBullets(); // 0x406060
extern void *ScriptViewerCreateBomb();    // 0x4056B0
extern void *GameCreateItemManager();     // 0x41AED0
extern void *ScriptViewerCreatePopups();  // 0x42B660
extern void *ScriptViewerCreateGui();     // 0x414830

ScriptViewerView::~ScriptViewerView()
{
    ClearScriptNames();
    th10::ChainElem *task = updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    ScriptViewerDeleteOwned(g_GuiView);
    ScriptViewerDeleteOwned(g_Player);
    ScriptViewerDeleteOwned(g_ScriptViewerBullets);
    ScriptViewerDeleteOwned(g_EnemyManager);
    ScriptViewerDeleteOwned(static_cast<ScriptViewerBombOwnerView *>(g_ScriptViewerBomb));
    ScriptViewerDeleteOwned(g_ItemManager);
    ScriptViewerDeleteOwned(static_cast<ScriptViewerPopupOwnerView *>(g_ScriptViewerPopups));
    g_ScriptViewer = NULL;
}

int ScriptViewerView::InitializeManagers()
{
    PlayerCreate();
    ScriptViewerCreateBullets();
    ScriptViewerCreateBomb();
    GameCreateItemManager();
    ScriptViewerCreatePopups();
    EnemyManagerCreate(scriptNames[files.current]);
    ScriptViewerCreateGui();
    entries.count = reinterpret_cast<EclVmScriptDatabase *>(
        g_EnemyManager->scriptDatabase)->subroutineCount;
    entries.Move(1);
    entries.Move(-1);
    state = 1;
    return 0;
}

int ScriptViewerLoadThread()
{
    g_ScriptViewer->InitializeManagers();
    return 0;
}

static inline void ResumeScriptViewerOwner(void *owner)
{
    ScriptViewerChainOwnerView *chains = static_cast<ScriptViewerChainOwnerView *>(owner);
    if (chains->update) chains->update->flags &= ~2u;
    if (chains->draw) chains->draw->flags &= ~2u;
}

static inline void SuspendScriptViewerOwner(void *owner)
{
    ScriptViewerChainOwnerView *chains = static_cast<ScriptViewerChainOwnerView *>(owner);
    if (chains->update) chains->update->flags |= 2u;
    if (chains->draw) chains->draw->flags |= 2u;
}

// 0x40A010 dereferences both tasks unconditionally, then rebuilds options.
// The null-tolerant retirement helper at 0x409E20 is a different owner.
__declspec(noinline) void ScriptViewerSuspendPlayer(Player *player)
{
    ScriptViewerChainOwnerView *chains = reinterpret_cast<ScriptViewerChainOwnerView *>(player);
    chains->update->flags |= 2u;
    chains->draw->flags |= 2u;
    RebuildPlayerOptions(player);
}

void ScriptViewerView::ClearScriptNames()
{
    if (scriptNames)
    {
        for (int i = 0; i < scriptCount; ++i)
        {
            if (scriptNames[i])
            {
                free(scriptNames[i]);
                scriptNames[i] = NULL;
            }
        }
        if (scriptNames)
        {
            free(scriptNames);
            scriptNames = NULL;
        }
        scriptNames = NULL;
    }
}

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

int ScriptViewerView::Update()
{
    WIN32_FIND_DATAA find;
    switch (state)
    {
    case 4:
        g_FrontEndInput.previousHeld = g_FrontEndInput.previousRaw;
        g_FrontEndInput.previousRaw = g_FrontEndInput.raw;
        g_FrontEndInput.UpdateDebugRepeat();
        g_FrontEndInput.frameMask032 = g_FrontEndInput.pressed;
        if (g_FrontEndInput.frameMask032 & 8)
        {
            ResumeScriptViewerOwner(g_Player);
            PlayerResetRuntimeState(g_Player);
            ResumeScriptViewerOwner(g_ScriptViewerBullets);
            g_ScriptViewerBullets->ResetPool();
            ResumeScriptViewerOwner(g_EnemyManager);
            EnemyManagerClear(g_EnemyManager);
            ResumeScriptViewerOwner(g_ItemManager);
            memset(g_ItemManager->items, 0, sizeof(g_ItemManager->items));
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
            if (g_EnemyManager)
            {
                RememberScriptViewerCursor(&entries);
                if (static_cast<unsigned short>(g_FrontEndInput.IsRepeated(0x80))) entries.Move(1);
                if (static_cast<unsigned short>(g_FrontEndInput.IsRepeated(0x40))) entries.Move(-1);
                if (g_FrontEndInput.mask008 & 0x1001)
                {
                    ScriptViewerSuspendPlayer(g_Player);
                    PlayerResetRuntimeState(g_Player);
                    SuspendScriptViewerOwner(g_ScriptViewerBullets);
                    g_ScriptViewerBullets->ResetPool();
                    g_EnemyManager->SuspendCallbacks();
                    EnemyManagerClear(g_EnemyManager);
                    GameResetGui(g_GuiView);
                    SuspendScriptViewerOwner(g_ItemManager);
                    memset(g_ItemManager->items, 0, sizeof(g_ItemManager->items));
                    EnemySpawnRequestView request;
                    memset(&request, 0, sizeof(request));
                    request.position.x = 0.0f;
                    request.position.y = 0.0f;
                    request.position.z = 0.0f;
                    request.life = 10000;
                    EnemySpawn(g_EnemyManager,
                        reinterpret_cast<EclVmScriptDatabase *>(g_EnemyManager->scriptDatabase)->
                            subroutines[entries.current].name,
                        &request);
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
                ScriptViewerReleaseGui(g_GuiView);
                ScriptViewerReleasePlayer(g_Player);
                ScriptViewerReleaseBullets(g_ScriptViewerBullets);
                ScriptViewerReleaseBomb(g_ScriptViewerBomb);
                ScriptViewerReleaseItems(g_ItemManager);
                ScriptViewerReleasePopups(g_ScriptViewerPopups);
                ScriptViewerReleaseEnemies(g_EnemyManager);
                flags &= ~2u;
                worker.Start(this, ScriptViewerLoadThread);
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
        value680 = 1000;
        break;
    }
    return 1;
}

int ScriptViewerView::Draw()
{
    AnmFloat3View textPosition(0.0f, 0.0f, 0.0f);
    g_AsciiManagerView->AddSmallFormatText(&textPosition, "SprtView\n");
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
        if (g_EnemyManager)
            g_AsciiManagerView->AddSmallFormatText(
                &textPosition, "Ecl %s",
                reinterpret_cast<EclVmScriptDatabase *>(g_EnemyManager->scriptDatabase)->
                    subroutines[entries.current].name);
        else
            g_AsciiManagerView->AddSmallFormatText(
                &textPosition, "Ecl %d", entries.current);
        textPosition.y = 36.0f;
        g_AsciiManagerView->AddSmallFormatText(&textPosition, "Quit");
        textPosition.x = 32.0f;
        textPosition.y = menu.current * 10.0f + 16.0f;
        g_AsciiManagerView->AddSmallFormatText(&textPosition, ">");
        g_AsciiManagerView->color = 0xffffffff;
        break;
    case 4:
        textPosition.x = 500.0f;
        textPosition.y = 240.0f;
        textPosition.z = 0.0f;
        g_AsciiManagerView->AddSmallFormatText(
            &textPosition, "Enemy %d", g_EnemyManager->activeEnemyCount);
        return 1;
    case 3:
        {
            g_AsciiManagerView->color = 0xffa0a0a0;
            AnmFloat3View statusPosition(32.0f, 16.0f, 0.0f);
            g_AsciiManagerView->AddSmallFormatText(
                &statusPosition, "Pos %.3d %.3d",
                static_cast<int>(position.x), static_cast<int>(position.y));
            g_AsciiManagerView->color = 0xffffffff;
            break;
        }
    default:
        return 1;
    }
    if (flags & 2)
    {
        vm.position = AnmFloat3View(320.0f, 240.0f, 0.0f);
        g_AnmRenderManagerView->DrawNoRotation(&vm);
    }
    return 1;
}

int __fastcall ScriptViewerUpdateCallback(void *context)
{
    return static_cast<ScriptViewerView *>(context)->Update();
}

int __fastcall ScriptViewerDrawCallback(void *context)
{
    return static_cast<ScriptViewerView *>(context)->Draw();
}
