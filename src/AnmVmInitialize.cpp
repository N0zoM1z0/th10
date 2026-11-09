#include "AnmManager.hpp"
#include <string.h>

__declspec(noinline) void __fastcall AnmVmInitialize(
    void *unused, AnmVmView *vm)
{
    AnmFloat3View savedPosition = vm->positionOffset;
    unsigned int savedLayer = vm->renderLayer;

    memset(vm, 0, sizeof(AnmVmView));

    vm->renderLayer = savedLayer;
    vm->positionOffset = savedPosition;

    vm->primaryColor.value = 0xffffffff;
    vm->scaleX = 1.0f;
    vm->scaleY = 1.0f;
    vm->matrix23C.SetIdentity();
    vm->flagsLow16 = 7;
    vm->scriptTimer.Initialize();

    vm->positionInterpolation.duration = 0;
    vm->primaryColorInterpolation.duration = 0;
    vm->primaryAlphaInterpolation.duration = 0;
    vm->rotationInterpolation.duration = 0;
    vm->scaleInterpolation.duration = 0;
    vm->secondaryColorInterpolation.duration = 0;
    vm->secondaryAlphaInterpolation.duration = 0;

    AnmVmLayerNodeView *managerNode = &vm->managerNode;
    managerNode->owner = vm;
    managerNode->next = NULL;
    managerNode->previous = NULL;
    AnmVmLayerNodeView *layerNode = &vm->layerNode;
    layerNode->owner = vm;
    layerNode->next = NULL;
    layerNode->previous = NULL;
}
