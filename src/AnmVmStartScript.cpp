#include "Main.hpp"
#include "AnmManager.hpp"
#include "AnmLoadedStartScript.hpp"

// Target 0x00404F30: reset an entity VM, zero its three position vectors,
// set the 16-pixel glyph box and start the script. Retail passes the VM in
// ESI, the script index in EAX and the resource on the stack (RET 4).
__declspec(noinline) void __stdcall StartEntityScript_00404f30(
    AnmVmView *entity, int scriptIndex, AnmLoadedView *resource)
{
    entity->Initialize();
    entity->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    entity->position = AnmFloat3View(0.0f, 0.0f, 0.0f);
    entity->alternatePosition = AnmFloat3View(0.0f, 0.0f, 0.0f);
    entity->glyphWidth = entity->glyphHeight = 0x10;
    entity->scriptIndex = static_cast<short>(scriptIndex);
    StartAnmLoadedScript(resource, entity, scriptIndex);
}
