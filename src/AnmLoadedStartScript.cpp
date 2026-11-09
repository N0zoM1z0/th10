#include <string.h>
#include "AnmLoadedStartScript.hpp"

struct AnimationManager_0043E710
{
    unsigned char reserved00[0x4c];
    int script_tick_counter;
};

extern "C" AnimationManager_0043E710 *data_00491c10;
extern "C" float data_00476f78;
extern "C" void __stdcall asm_FUN_0043ee30_0043ee30(
    AnimationObject_0043E710 *object);

#define Entity_Init AnmVmInitialize

#pragma code_seg(".text$01accessor")
static __declspec(noinline) void __fastcall LoadScriptBody_0043e710(
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

    Entity_Init(script, object);

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
        object->scriptTimer.scale = &data_00476f78;
        object->scriptTimer.flags |= 1;
    }

    object->scriptTimer.current = 0;
    object->scriptTimer.subframe = 0.0f;
    object->scriptTimer.previous = -1;

    unsigned int *flag_word = &object->AnimationFlags();
    *flag_word &= ~1u;

    asm_FUN_0043ee30_0043ee30(object);
    ++data_00491c10->script_tick_counter;
}

extern "C" __declspec(naked) void __cdecl LoadScript_0043e710(void)
{
    __asm
    {
        mov edx, eax
        push ebx
        jmp LoadScriptBody_0043e710
    }
}

__declspec(noinline) void __stdcall LoadScriptAdapter_0043e710(
    AnimationResource_0043E710 *resource,
    AnimationObject_0043E710 *object,
    int index)
{
    __asm
    {
        mov ecx, resource
        mov eax, object
        mov ebx, index
        call LoadScript_0043e710
    }
}
#pragma code_seg()
