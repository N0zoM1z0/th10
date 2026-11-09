#include "GameUiVmInterrupts.hpp"
#include "AnmManager.hpp"

// Three adjacent ANM VM IDs at 0x00477824..0x0047782F and the observed
// independent reset indicator at 0x00491BEC. Physical storage/owner unknown.
extern AnmVmIdView g_GameUiTransitionVmIds[3];
extern int g_GameUiTransitionResetIndicator;


// The two transition owners each allocate the same three root VMs. The
// original owner type names remain unknown; +0x3C8 is the ANM resource used
// by both constructors and +0x6FC gates the one-time setup.
// Target 0x00420F30: create the three transition VMs for the first owner.
void __stdcall GameUiTransitionInitialize(GameUiVmTransitionView *owner)
{
    if (owner->transitionState != 0)
        return;
    AnmLoadedView *resource = owner->transitionAnm;
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 0);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[0] = manager->AddVmVariant0(vm);

    resource = owner->transitionAnm;
    manager = g_AnmRenderManagerView;
    vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 1);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[1] = manager->AddVmVariant0(vm);

    resource = owner->transitionAnm;
    manager = g_AnmRenderManagerView;
    vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 2);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[2] = manager->AddVmVariant0(vm);
    owner->transitionState = 1;
}

// Target 0x00421180: second owner uses the same three-script transition set.
void __stdcall GameUiTransitionInitializeSecondary(GameUiVmTransitionView *owner)
{
    if (owner->transitionState != 0)
        return;
    AnmLoadedView *resource = owner->transitionAnm;
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 0);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[0] = manager->AddVmVariant0(vm);

    resource = owner->transitionAnm;
    manager = g_AnmRenderManagerView;
    vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 1);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[1] = manager->AddVmVariant0(vm);

    resource = owner->transitionAnm;
    manager = g_AnmRenderManagerView;
    vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 2);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[2] = manager->AddVmVariant0(vm);
    owner->transitionState = 1;
}

// Target 0x00421070: acknowledge a pending transition
// and broadcast the 1 interrupt to three root VMs and their children.
void __stdcall GameUiSendVmInterrupt1(GameUiVmTransitionView *owner)
{
    if (owner->transitionState == 1)
    {
        AnmRenderManagerView *renderer = g_AnmRenderManagerView;
        AnmVmView *vm0 = renderer->FindVm(g_GameUiTransitionVmIds[0]);
        if (vm0 != NULL)
        {
            vm0->pendingInterrupt = 1;
            if (vm0->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm0->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
                    node = node->next;
                }
            }
        }
        AnmVmView *vm1 = renderer->FindVm(g_GameUiTransitionVmIds[1]);
        if (vm1 != NULL)
        {
            vm1->pendingInterrupt = 1;
            if (vm1->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm1->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
                    node = node->next;
                }
            }
        }
        AnmVmView *vm2 = renderer->FindVm(g_GameUiTransitionVmIds[2]);
        if (vm2 != NULL)
        {
            vm2->pendingInterrupt = 1;
            if (vm2->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm2->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
                    node = node->next;
                }
            }
        }
        g_GameUiTransitionVmIds[0].value = 0;
        g_GameUiTransitionVmIds[1].value = 0;
        g_GameUiTransitionVmIds[2].value = 0;
        owner->transitionState = 0;
    }
    if (g_GameUiTransitionResetIndicator != 0)
        g_GameUiTransitionResetIndicator = 0;
}

// Target 0x00421300: acknowledge a pending transition
// and broadcast the 2 interrupt to three root VMs and their children.
void __stdcall GameUiSendVmInterrupt2(GameUiVmTransitionView *owner)
{
    if (owner->transitionState == 1)
    {
        AnmRenderManagerView *renderer = g_AnmRenderManagerView;
        AnmVmView *vm0 = renderer->FindVm(g_GameUiTransitionVmIds[0]);
        if (vm0 != NULL)
        {
            vm0->pendingInterrupt = 2;
            if (vm0->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm0->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 2;
                    node = node->next;
                }
            }
        }
        AnmVmView *vm1 = renderer->FindVm(g_GameUiTransitionVmIds[1]);
        if (vm1 != NULL)
        {
            vm1->pendingInterrupt = 2;
            if (vm1->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm1->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 2;
                    node = node->next;
                }
            }
        }
        AnmVmView *vm2 = renderer->FindVm(g_GameUiTransitionVmIds[2]);
        if (vm2 != NULL)
        {
            vm2->pendingInterrupt = 2;
            if (vm2->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm2->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 2;
                    node = node->next;
                }
            }
        }
        g_GameUiTransitionVmIds[0].value = 0;
        g_GameUiTransitionVmIds[1].value = 0;
        g_GameUiTransitionVmIds[2].value = 0;
        owner->transitionState = 2;
    }
    if (g_GameUiTransitionResetIndicator != 0)
        g_GameUiTransitionResetIndicator = 0;
}
