#include "BulletManager.hpp"
#include "GameErrorContext.hpp"

#include <new>
#include <string.h>

// Native 0x004776F0; the ScriptViewer lifecycle already uses this exact
// BulletManagerView pointer, which was only externally declared before.
BulletManagerView *g_ScriptViewerBullets;

extern char g_GameOwnerBulletAnmErrorMessage[];
extern int __fastcall BulletManagerUpdateCallback(BulletManagerView *manager);
extern int __fastcall BulletManagerDrawCallback(BulletManagerView *manager);

// Target 0x00406060 performs implicit construction of 2001 BulletRuntimeView
// instances before clearing the entire allocation. That array's own
// constructor / destructor are separate original target owners.
BulletManagerView::BulletManagerView()
{
    memset(this, 0, sizeof(*this));
    g_ScriptViewerBullets = this;
}

// Target 0x00405E20: the task chain is installed only after the bullet ANM
// resource was actually preloaded. The two callbacks are registered with
// priorities 20/29 and independent handles at +8/+0xC.
int BulletManagerView::Initialize()
{
    bulletAnm = g_AnmRenderManagerView->PreloadAnm(7, "bullet.anm");
    if (bulletAnm == NULL)
    {
        th10::g_GameErrorContext.Log(g_GameOwnerBulletAnmErrorMessage);
        return -1;
    }
    bulletCursor = bullets;
    bullets[2000].state = 5;

    th10::ChainElem *task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&BulletManagerUpdateCallback));
    task->arg = this;
    task->flags &= ~2u;
    th10::g_Chain->AddToCalcChain(task, 20);
    updateTask = task;

    task = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&BulletManagerDrawCallback));
    task->arg = this;
    task->flags &= ~2u;
    th10::g_Chain->AddToDrawChain(task, 29);
    drawTask = task;
    return 0;
}

// Target 0x00406060: allocate the complete game projectile manager, perform
// its native array construction and global publication, register tasks and
// on a failure run its existing real destructor and scalar delete.
BulletManagerView *CreateBulletManager()
{
    BulletManagerView *manager = new BulletManagerView;
    if (manager->Initialize() != 0)
    {
        delete manager;
        return NULL;
    }
    return manager;
}
