#include "Main.hpp"
#include "ScriptViewer.hpp"
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
extern const unsigned char g_ScriptViewerWorkerVtable[]; // 0x4703E4
extern void *g_ScriptViewerStageRoute;                  // 0x477848
extern unsigned char g_ScriptViewerRouteDescriptor[];   // 0x474788
extern int g_ScriptViewerStage;                        // 0x474C7C
extern int g_ScriptViewerShownStage;                   // 0x474C80

ScriptViewerWorkerView::ScriptViewerWorkerView()
{
    vtable = g_ScriptViewerWorkerVtable;
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
struct ScriptViewerItemsView
{
    ScriptViewerChainOwnerView chains;
    int unknown10;
    unsigned char slots[0x21cea0];
};
typedef char ScriptViewerInputOffsets[
    (offsetof(ScriptViewerItemsView, slots) == 0x14) ? 1 : -1];

extern BulletManagerView *g_ScriptViewerBullets;         // 0x4776F0
extern ScriptViewerItemsView *g_ScriptViewerItems;        // 0x477818
extern EnemyManagerView *g_EnemyManager;                 // 0x477704
extern GuiView *g_GuiView;                               // 0x47770C
extern void *g_ScriptViewerBomb;            // 0x4776EC
extern void *g_ScriptViewerPopups;          // 0x477840
extern void ScriptViewerReleaseGui(GuiView *owner);                         // 0x4148E0
extern void ScriptViewerReleasePlayer(Player *owner);                      // 0x425090
extern void ScriptViewerReleaseBullets(BulletManagerView *owner);          // 0x406140
extern void ScriptViewerReleaseBomb(void *owner);                          // 0x405730
extern void ScriptViewerReleaseItems(ScriptViewerItemsView *owner);        // 0x41AFB0
extern void ScriptViewerReleasePopups(void *owner);                        // 0x42B6D0
extern void ScriptViewerReleaseEnemies(EnemyManagerView *owner);           // 0x40D730
extern void ScriptViewerStartWorker(
    ScriptViewerWorkerView *worker, ScriptViewerView *view, int (*entry)());
extern int ScriptViewerLoadThread(); // 0x40A340
extern void *ScriptViewerCreateBullets(); // 0x406060
extern void *ScriptViewerCreateBomb();    // 0x4056B0
extern void *GameCreateItemManager();     // 0x41AED0
extern void *ScriptViewerCreatePopups();  // 0x42B660
extern void *ScriptViewerCreateGui();     // 0x414830

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
            ResumeScriptViewerOwner(g_ScriptViewerItems);
            memset(g_ScriptViewerItems->slots, 0, sizeof(g_ScriptViewerItems->slots));
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
                    SuspendScriptViewerOwner(g_ScriptViewerItems);
                    memset(g_ScriptViewerItems->slots, 0, sizeof(g_ScriptViewerItems->slots));
                    EnemySpawnRequestView request;
                    memset(&request, 0, sizeof(request));
                    request.position.x = request.position.y = request.position.z = 0.0f;
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
                ScriptViewerReleaseItems(g_ScriptViewerItems);
                ScriptViewerReleasePopups(g_ScriptViewerPopups);
                ScriptViewerReleaseEnemies(g_EnemyManager);
                flags &= ~2u;
                ScriptViewerStartWorker(&worker, this, ScriptViewerLoadThread);
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

int __fastcall ScriptViewerUpdateCallback(void *context)
{
    return static_cast<ScriptViewerView *>(context)->Update();
}

int __fastcall ScriptViewerDrawCallback(void *context)
{
    return static_cast<ScriptViewerView *>(context)->Draw();
}
