#include "Main.hpp"
#include "AnmManager.hpp"
#include "AnmLoadedStartScript.hpp"

// Target 0x00404F30: reset a VM, clear its three position vectors and start
// one script of a loaded ANM resource. Under LTCG the VM arrives in ESI and
// the script index in EAX; the resource is the only stack argument (RET 4).
__declspec(noinline) void __stdcall AnmVmStartScript(
    AnmLoadedView *resource, AnmVmView *vm, int scriptIndex)
{
    AnmVmInitialize(0, vm);
    vm->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->position = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->alternatePosition = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->glyphWidth = vm->glyphHeight = 0x10;
    vm->scriptIndex = static_cast<short>(scriptIndex);
    StartAnmLoadedScript(resource, vm, scriptIndex);
}
