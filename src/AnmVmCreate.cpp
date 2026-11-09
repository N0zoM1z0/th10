#include "AnmManager.hpp"
// The value wrapper preserves the original flag word as a distinct value
// while the three animation-space vectors are reset. Besides matching the
// target's write order, this keeps the flag update from collapsing into the
// allocator's surrounding state setup.
struct AnmVmFlagValue
{
    unsigned int value;

    AnmVmFlagValue(unsigned int bits) : value(bits) {}
    AnmVmFlagValue operator|(unsigned int bits) const
    {
        return AnmVmFlagValue(value | bits);
    }
};

// Target 0x00449870 initializes the existing object's position, base position
// and render offset, then binds the selected script without replacing its
// render-layer or list identity.
__declspec(noinline) void AnmLoadedView::InitializeVm(AnmVmView *vm, int scriptIndex)
{
    AnmVmFlagValue flags =
        *reinterpret_cast<AnmVmFlagValue *>(&vm->flags35C);
    vm->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->position = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->flags35C = (flags | 0x40000000u).value;
    vm->scriptIndex = static_cast<short>(scriptIndex);
    vm->alternatePosition = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->glyphHeight = 0x10;
    vm->glyphWidth = 0x10;
    SetAndExecuteScriptIndex(vm, scriptIndex);
}

// Keep the loaded-resource initializer address-taken in the LTCG graph.
// Otherwise LTCG may replace its externally visible thiscall with a private
// register ABI at internal call sites.
typedef void (AnmLoadedView::*AnmLoadedVmInitializer)(AnmVmView *, int);
extern "C" {
AnmLoadedVmInitializer volatile g_AnmLoadedVmInitializer =
    &AnmLoadedView::InitializeVm;
}
#pragma comment(linker, "/include:_g_AnmLoadedVmInitializer")
