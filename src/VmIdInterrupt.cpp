#include "AnmManager.hpp"
#include "VmIdInterrupt.hpp"

// Target409E50..409E90 is shared by Enemy and FrontEnd VM-id retirement.
__declspec(noinline) void EnemyMarkPendingInterrupt(int *id)
{
    AnmVmView *vm =
        g_AnmRenderManagerView->FindVm(AnmVmIdView(*id));
    if (vm == 0) {
        return;
    }

    vm->pendingInterrupt = 1;
    if (vm->layerNode.previous != 0) {
        return;
    }

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != 0) {
        static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
        node = node->next;
    }
}
