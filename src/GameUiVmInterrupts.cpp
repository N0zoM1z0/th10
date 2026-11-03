#include "GameUiVmInterrupts.hpp"
#include "AnmManager.hpp"

// Three adjacent ANM VM IDs at 0x00477824..0x0047782F and the observed
// independent reset indicator at 0x00491BEC. Physical storage/owner unknown.
extern AnmVmIdView g_GameUiTransitionVmIds[3];
extern int g_GameUiTransitionResetIndicator;

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
