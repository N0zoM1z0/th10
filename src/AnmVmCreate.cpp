#include "AnmManager.hpp"
// Target 0x00449870 prepares a newly allocated VM for a loaded script. The
// allocator owns the full VM reset; this phase initializes spawn-local state
// before handing the script index to the loaded-resource executor.
void AnmLoadedView::InitializeVm(AnmVmView *vm, int scriptIndex)
{
    vm->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->position = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->alternatePosition = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->flags35C |= 0x40000000u;
    vm->scriptIndex = static_cast<short>(scriptIndex);
    vm->glyphWidth = 0x10;
    vm->glyphHeight = 0x10;
    SetAndExecuteScriptIndex(vm, scriptIndex);
}
