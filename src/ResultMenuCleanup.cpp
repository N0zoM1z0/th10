// TH10 0x422220: independent ResultMenu lifecycle teardown.
// The original implementation's physical translation unit is unknown.
#include "Main.hpp"
#include "ResultMenu.hpp"
#include "Chain.hpp"
#include "ReplayManager.hpp"
#include <new>

struct GameChainOwnerView;
extern GameChainOwnerView *g_GameOwner477830;

// Original receiver is supplied on the stack; never change the RET 4 ABI.
void __stdcall GameCleanupController422220(GameChainOwnerView *opaque)
{
    ResultMenuView *owner = reinterpret_cast<ResultMenuView *>(opaque);
    th10::ChainElem *task = owner->updateTask;
    th10::Chain *chain = th10::g_Chain;
    if (task != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }

    task = owner->drawTask;
    chain = th10::g_Chain;
    if (task != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(task);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }

    for (int i = 0; i < 25; ++i)
    {
        ReplayManager *replay = owner->replayFiles[i];
        if (replay != NULL)
            delete replay;
    }

    AnmVmView *vm = g_AnmRenderManagerView->FindVm(
        AnmVmIdView(owner->auxiliaryVm.value));
    if (vm != NULL)
    {
        const unsigned int deletionFlag = 0x04000000u;
        vm->flags35C |= deletionFlag;
        if (vm->layerNode.previous == NULL)
        {
            AnmVmLayerNodeView *node = vm->layerNode.next;
            while (node != NULL)
            {
                static_cast<AnmVmView *>(node->owner)->flags35C |= deletionFlag;
                node = node->next;
            }
        }
    }
    owner->auxiliaryVm.value = 0;
    g_GameOwner477830 = NULL;
}
