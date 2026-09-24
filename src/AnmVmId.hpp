#pragma once

#if defined(_MSC_VER)
#define TH10_ANM_NOINLINE __declspec(noinline)
#else
#define TH10_ANM_NOINLINE
#endif

struct AnmVmView;
struct AnmLoadedView;

struct AnmVmIdView
{
    AnmVmIdView() { value = 0; }
    AnmVmIdView(int initialValue) { value = initialValue; }

    int operator==(AnmVmIdView other)
    {
        return value == other.value;
    }

    int operator++(int)
    {
        return value++;
    }

    TH10_ANM_NOINLINE AnmVmView *GetVm();
    TH10_ANM_NOINLINE void SetInterrupt(short interrupt);
    TH10_ANM_NOINLINE void SetFlag2();
    TH10_ANM_NOINLINE void ClearFlag2();
    TH10_ANM_NOINLINE void Release();
    TH10_ANM_NOINLINE void SetSprite(int spriteIndex);
    TH10_ANM_NOINLINE void SetSpriteWithAnm(
        AnmLoadedView *loaded, int spriteIndex);

    int value;
};

typedef char AnmVmIdViewSizeIs04[
    (sizeof(AnmVmIdView) == 0x04) ? 1 : -1];
