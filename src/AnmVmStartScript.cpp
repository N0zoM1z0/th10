#include "Main.hpp"
#include "AnmManager.hpp"

#pragma code_seg(".text$00404F30_fn")
__declspec(noinline) void __stdcall EntityStartScript_00404f30(
    AnmLoadedView *resource, AnmVmView *entity, int scriptIndex)
{
    entity->Initialize();
    entity->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    entity->position = AnmFloat3View(0.0f, 0.0f, 0.0f);
    entity->alternatePosition = AnmFloat3View(0.0f, 0.0f, 0.0f);
    entity->glyphHeight = 0x10;
    entity->glyphWidth = 0x10;
    entity->scriptIndex = static_cast<short>(scriptIndex);
    resource->InitializeAndExecuteScriptIndex(entity, scriptIndex);
}
#pragma code_seg()
