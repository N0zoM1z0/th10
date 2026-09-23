#include "AnmManager.hpp"

// Target 0x00449450 resolves this id through the manager and clears it if the
// VM is no longer present in either list.
AnmVmView *AnmVmIdView::GetVm()
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*this);
    if (vm == NULL)
        value = 0;
    return vm;
}

// Target 0x00449470 sets an interrupt on a live VM and its root child chain.
void AnmVmIdView::SetInterrupt(short interrupt)
{
    AnmVmIdView vmId(value);
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(vmId);
    if (vm == NULL)
        return;

    vm->pendingInterrupt = interrupt;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        AnmVmView &child = *static_cast<AnmVmView *>(node->owner);
        child.pendingInterrupt = interrupt;
        node = node->next;
    }
}

// Target 0x00449590 sets flag bit 2 on the VM and, for a root VM, its children.
void AnmVmIdView::SetFlag2()
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*this);
    if (vm == NULL)
        return;

    const unsigned int mask = 2u;
    vm->flags35C |= mask;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->flags35C |= mask;
        node = node->next;
    }
}

// Target 0x004495E0 clears flag bit 2 on the VM and, for a root VM, its children.
void AnmVmIdView::ClearFlag2()
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*this);
    if (vm == NULL)
        return;

    const unsigned int mask = 0xfffffffdu;
    vm->flags35C &= mask;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->flags35C &= mask;
        node = node->next;
    }
}

// Target 0x00449630 requests deletion and clears the id even before cleanup runs.
void AnmVmIdView::Release()
{
    g_AnmRenderManagerView->MarkVmForDeletion(*this);
    value = 0;
}
