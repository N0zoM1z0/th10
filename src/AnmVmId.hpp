#pragma once

#include <string.h>

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
        // Native VM IDs increment as one modulo-32-bit word. The caller
        // still receives the pre-increment value (postfix semantics).
        const int previous = value;
        const unsigned int nextBits = static_cast<unsigned int>(value) + 1u;
        memcpy(&value, &nextBits, sizeof value);
        return previous;
    }

    TH10_ANM_NOINLINE AnmVmView *GetVm();
    TH10_ANM_NOINLINE void SetInterrupt(short interrupt);
    TH10_ANM_NOINLINE void SetFlag2();
    TH10_ANM_NOINLINE void ClearFlag2();
    TH10_ANM_NOINLINE void Release();
    // Marks the VM for deletion and clears this id; defined after the manager.
    inline void MarkForDeletionAndClear();
    TH10_ANM_NOINLINE void SetSprite(int spriteIndex);
    TH10_ANM_NOINLINE void SetSpriteWithAnm(
        AnmLoadedView *loaded, int spriteIndex);

    int value;
};

typedef char AnmVmIdViewSizeIs04[
    (sizeof(AnmVmIdView) == 0x04) ? 1 : -1];
