#include "AnmManager.hpp"

AnmVmIdView AnmLoadedView::CreateVmVariant0(
    int scriptIndex, unsigned int renderLayer)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->renderLayer = renderLayer;
    vm->flags35C |= 0x40000000u;
    InitializeVm(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant0(vm);
}
