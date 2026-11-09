#include "Main.hpp"
#include "AnmLoadedStartScript.hpp"

#pragma code_seg(".text$03context")
int __stdcall LoadScriptCompilerContext(
    AnimationObject_0043E710 **objects,
    int count,
    AnimationResource_0043E710 *resource)
{
    for (int slot = 0; slot < count; ++slot)
        LoadScript_0043e710(resource, objects[slot], slot);
    return count;
}
#pragma code_seg()
