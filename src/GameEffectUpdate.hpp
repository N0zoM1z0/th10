#pragma once
#include <stddef.h>
struct AnmLoadedView;
// Shared view of the existing effect owner, not a second owner or global.
struct GameEffectUpdateView
{
    unsigned char unknown0000[0x899C];
    AnmLoadedView *loadedResource;
    int firstVmId;
    int secondVmId;
};
#if defined(_M_IX86)
typedef char GameEffectUpdateLayoutCheck[
    sizeof(GameEffectUpdateView) == 0x89A8 &&
    offsetof(GameEffectUpdateView, loadedResource) == 0x899C &&
    offsetof(GameEffectUpdateView, firstVmId) == 0x89A0 &&
    offsetof(GameEffectUpdateView, secondVmId) == 0x89A4 ? 1 : -1];
#endif
extern GameEffectUpdateView *g_GameEffectUpdate; // 0x4776E0
