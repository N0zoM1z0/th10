#ifndef TH10_RECONSTRUCTION_INPUT_STATE_HPP
#define TH10_RECONSTRUCTION_INPUT_STATE_HPP

#include <stddef.h>

// Minimum shared view at 0x474E30. The first eight bytes are consumed by the
// FrontEnd; 0x40AC20 establishes the later debug-repeat fields. This view does
// not establish the allocation size, indexed record stride or array count.
struct FrontEndInputView
{
    union
    {
        unsigned int current;
        struct { unsigned short raw; unsigned short previous; };
    };
    unsigned short repeated;
    unsigned short pressed;
    unsigned int mask008;
    unsigned char unknown00C[0x1e];
    unsigned short replayHoldCounter;
    unsigned short previousRaw;
    unsigned short previousHeld;
    unsigned short repeatFlags;
    unsigned short frameMask032;
    unsigned short frameMask034;
    unsigned short unknown036;
    unsigned short repeatCounters[16];

    int IsRepeated(unsigned short mask);
    __declspec(noinline) void UpdateDebugRepeat();
    bool IsActiveInline(unsigned short mask) const
    {
        return (pressed & mask) || (repeated & mask);
    }
};

typedef char InputStateObservedOffsets[
    (sizeof(unsigned short) == 2 && sizeof(unsigned int) == 4 &&
     offsetof(FrontEndInputView, repeated) == 4 &&
     offsetof(FrontEndInputView, pressed) == 6 &&
     offsetof(FrontEndInputView, mask008) == 8 &&
     offsetof(FrontEndInputView, replayHoldCounter) == 0x2a &&
     offsetof(FrontEndInputView, previousRaw) == 0x2c &&
     offsetof(FrontEndInputView, repeatFlags) == 0x30 &&
     offsetof(FrontEndInputView, frameMask032) == 0x32 &&
     offsetof(FrontEndInputView, repeatCounters) == 0x38) ? 1 : -1];

extern FrontEndInputView g_FrontEndInput;

#endif
