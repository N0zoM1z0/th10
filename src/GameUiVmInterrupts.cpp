#include "GameUiVmInterrupts.hpp"
#include "AnmManager.hpp"

// Three adjacent ANM VM IDs at 0x00477824..0x0047782F and the observed
// independent reset indicator at 0x00491BEC. Physical storage/owner unknown.
extern AnmVmIdView g_GameUiTransitionVmIds[3];
extern int g_GameUiTransitionResetIndicator;


// Target 0x00420F30 is a member initializer: the owner is the implicit
// object and the caller supplies the screen-space position to propagate.
void GameUiVmTransitionView::Initialize(const AnmFloat3View *position)
{
    if (transitionState != 0)
        return;

    AnmLoadedView *resource = transitionAnm;
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 0);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[0] = manager->AddVmVariant0(vm);

    resource = transitionAnm;
    manager = g_AnmRenderManagerView;
    vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 1);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[1] = manager->AddVmVariant0(vm);

    resource = transitionAnm;
    manager = g_AnmRenderManagerView;
    vm = manager->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 15;
    resource->InitializeVm(vm, 2);
    manager = g_AnmRenderManagerView;
    g_GameUiTransitionVmIds[2] = manager->AddVmVariant0(vm);
    transitionState = 1;

    manager = g_AnmRenderManagerView;
    manager->SetVmPosition(g_GameUiTransitionVmIds[0], position);
    manager->SetVmPosition(g_GameUiTransitionVmIds[1], position);
    manager->SetVmPosition(g_GameUiTransitionVmIds[2], position);
}

// Target 0x00421180 takes both the owner and a position. If the transition
// is already initialized it skips VM creation/position propagation but
// still restores the manager's default transition viewport when necessary.
void __stdcall GameUiTransitionInitializeSecondary(
    GameUiVmTransitionView *owner, const AnmFloat3View *position)
{
    if (owner->transitionState == 0)
    {
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

        manager = g_AnmRenderManagerView;
        manager->SetVmPosition(g_GameUiTransitionVmIds[0], position);
        manager->SetVmPosition(g_GameUiTransitionVmIds[1], position);
        manager->SetVmPosition(g_GameUiTransitionVmIds[2], position);
    }

    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    if (manager->captureAnmIndex < 0)
    {
        manager->captureAnmIndex = 8;
        manager->transitionViewportUnknown2C = 0;
        manager->transitionViewportUnknown30 = 0;
        manager->transitionViewportWidth = 0x280;
        manager->transitionViewportHeight = 0x1e0;
        manager->transitionViewportUnknown3C = 0;
        manager->transitionViewportUnknown40 = 0;
        manager->transitionViewportWidth2 = 0x280;
        manager->transitionViewportHeight2 = 0x1e0;
    }
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
