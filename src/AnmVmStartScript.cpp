#include "Main.hpp"
#include "AnmManager.hpp"
#include "AnmLoadedStartScript.hpp"

// Target 0x00404F30 is the VM's script-start operation. In the retail
// internal call graph the VM receiver is kept in ESI, the script index is
// forwarded in EAX, and the loaded resource is the single stack argument.
__declspec(noinline) void AnmVmView::StartScript(
    int scriptIndex, AnmLoadedView *resource, void *unused)
{
    this->Initialize(unused);
    this->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    this->position = AnmFloat3View(0.0f, 0.0f, 0.0f);
    this->alternatePosition = AnmFloat3View(0.0f, 0.0f, 0.0f);
    this->glyphWidth = this->glyphHeight = 0x10;
    this->scriptIndex = static_cast<short>(scriptIndex);
    StartAnmLoadedScript(resource, this, scriptIndex);
}

// Keep the descriptive adapter for older call sites while the native VM
// method owns the internal ESI/EAX/stack ABI used by the bullet interpreter.
__declspec(noinline) void __stdcall StartEntityScript_00404f30(
    AnmVmView *entity, int scriptIndex, AnmLoadedView *resource)
{
    entity->StartScript(scriptIndex, resource, 0);
}
