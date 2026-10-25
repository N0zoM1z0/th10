#include "Main.hpp"
#include "EnemyLaser.hpp"
#include "GameErrorContext.hpp"
#include <new>

extern int __fastcall EnemyLaserUpdateCallback(void *);
extern int __fastcall EnemyLaserDrawCallback(void *);
struct GameChainOwnerView;

int EnemyLaserManagerStorageView::Initialize()
{
    effectResource = g_AnmRenderManagerView->PreloadAnm(7, "bullet.anm");
    if (!effectResource)
    {
        th10::g_GameErrorContext.Log("\x93\x47\x92\x65\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x0d\x0a");
        return -1;
    }
    th10::ChainElem *task = th10::Chain::AllocElem(EnemyLaserUpdateCallback);
    task->flags &= ~2u;
    th10::Chain *chain = th10::g_Chain;
    task->arg = this;
    th10::Chain::AddToCalcChain(task, 0x13, chain);
    updateTask = task;
    task = th10::Chain::AllocElem(EnemyLaserDrawCallback);
    task->flags &= ~2u;
    chain = th10::g_Chain;
    task->arg = this;
    th10::Chain::AddToDrawChain(task, 0x1b, chain);
    drawTask = task;
    tail = reinterpret_cast<EnemyLaserCommonStorageView *>(&sentinel);
    return 0;
}

void __stdcall GameCleanupLinked41C1C0(GameChainOwnerView *owner)
{
    EnemyLaserManagerStorageView *manager = reinterpret_cast<EnemyLaserManagerStorageView *>(owner);
    th10::ChainElem *task = manager->updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(task, chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    task = manager->drawTask;
    chain = th10::g_Chain;
    if (task)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(task, chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    EnemyLaserCommonStorageView *node = manager->sentinel.next;
    while (node)
    {
        EnemyLaserCommonStorageView *savedNext = node->next;
        reinterpret_cast<EnemyLaserConstructionBaseView *>(node)->Unlink();
        node->previous->next = node->next;
        if (node->next) node->next->previous = node->previous;
        ::operator delete(node);
        node = savedNext;
    }
    g_EnemyLaserManager = NULL;
}

void *GameCreateOwner41C290()
{
    EnemyLaserManagerStorageView *manager = new EnemyLaserManagerStorageView;
    if (manager->Initialize())
    {
        if (manager)
        {
            GameCleanupLinked41C1C0(reinterpret_cast<GameChainOwnerView *>(manager));
            ::operator delete(manager);
        }
        return NULL;
    }
    return manager;
}
