#pragma once
#include "AnmManager.hpp"

struct AnimationObject_0043E710 : AnmVmView
{
};

struct AnimationResource_0043E710
{
    unsigned short id;
    unsigned char reserved002[0x11c - 0x002];
    void **scripts;
    unsigned char reserved120[0x124 - 0x120];
    int loading;
};

typedef char AnimationObject_0043E710_Size[
    (sizeof(AnimationObject_0043E710) == 0x3ac) ? 1 : -1];
typedef char AnimationResource_0043E710_ScriptsAt11C[
    (offsetof(AnimationResource_0043E710, scripts) == 0x11c) ? 1 : -1];
typedef char AnimationResource_0043E710_LoadingAt124[
    (offsetof(AnimationResource_0043E710, loading) == 0x124) ? 1 : -1];

extern "C" __declspec(noinline) void __stdcall LoadScript_0043e710(
    AnimationResource_0043E710 *resource,
    AnimationObject_0043E710 *object,
    int index);

inline void StartAnmLoadedScript(
    AnmLoadedView *resource, AnmVmView *object, int index)
{
    LoadScript_0043e710(
        reinterpret_cast<AnimationResource_0043E710 *>(resource),
        reinterpret_cast<AnimationObject_0043E710 *>(object),
        index);
}
