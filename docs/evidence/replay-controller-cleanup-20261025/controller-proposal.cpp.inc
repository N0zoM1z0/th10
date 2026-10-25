
// Partial controller view from the complete 0x422220 cleanup accesses.
// This does not claim the original class or the allocation's full extent.
struct GameReplayControllerView
{
    GameChainOwnerView chains;
    unsigned char unknown010[0x1cc];
    AnmVmIdView vmId1DC;
    unsigned char unknown1E0[0x0c];
    ReplayManager *replays[25];
};
typedef char GameReplayVmAt1DC[(offsetof(GameReplayControllerView, vmId1DC) == 0x1dc) ? 1 : -1];
typedef char GameReplaySlotsAt1EC[(offsetof(GameReplayControllerView, replays) == 0x1ec) ? 1 : -1];

void __stdcall GameCleanupController422220(GameChainOwnerView *owner)
{
    GameChainElementView *element = owner->updateChain;
    th10::Chain *chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(reinterpret_cast<th10::ChainElem *>(element), chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    element = owner->drawChain;
    chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(reinterpret_cast<th10::ChainElem *>(element), chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    GameReplayControllerView *controller = reinterpret_cast<GameReplayControllerView *>(owner);
    for (int i = 0; i < 25; ++i)
    {
        ReplayManager *replay = controller->replays[i];
        if (replay != NULL)
            delete replay;
    }
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(controller->vmId1DC);
    if (vm != NULL)
    {
        vm->flags35C |= 0x04000000u;
        if (vm->layerNode.previous == NULL)
        {
            AnmVmLayerNodeView *node = vm->layerNode.next;
            while (node != NULL)
            {
                static_cast<AnmVmView *>(node->owner)->flags35C |= 0x04000000u;
                node = node->next;
            }
        }
    }
    controller->vmId1DC.value = 0;
    g_GameOwner477830 = NULL;
}
