#include <string.h>
#include "AnmLoadedStartScript.hpp"

#pragma code_seg(".text$01accessor")
extern "C" __declspec(noinline) void __stdcall LoadScript_0043e710(
    AnimationResource_0043E710 *resource,
    AnimationObject_0043E710 *object,
    int index)
{
    void *script = resource->scripts[index];
    if (script == 0 || resource->loading != 0)
    {
        memset(object, 0, sizeof(AnmVmView));
        return;
    }

    object->Initialize();

    unsigned int mask = object->AnimationFlags() & 0xFFFFF9FFu;
    object->ScriptIndex() = static_cast<short>(index);
    object->ScriptResourceId() = resource->id;
    object->ScriptResource() = resource;
    object->AnimationFlags() = mask;

    void *fresh = resource->scripts[index];
    object->ScriptBase() = fresh;
    object->ScriptCursor() = fresh;

    if ((object->scriptTimer.flags & 1) == 0)
    {
        object->scriptTimer.current = 0;
        object->scriptTimer.previous = static_cast<int>(0xFFF0BDC1u);
        object->scriptTimer.subframe = 0.0f;
        object->scriptTimer.scale = &g_AnmGameSpeed;
        object->scriptTimer.flags |= 1;
    }

    object->scriptTimer.current = 0;
    object->scriptTimer.subframe = 0.0f;
    object->scriptTimer.previous = -1;

    unsigned int *flag_word = &object->AnimationFlags();
    *flag_word &= ~1u;

    AnmRenderManagerView::ExecuteScript(object);
    ++g_AnmRenderManagerView->scriptsStartedThisFrame;
}
#pragma code_seg()
