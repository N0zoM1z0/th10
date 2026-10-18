#ifndef TH10_RECONSTRUCTION_INPUT_STATE_HPP
#define TH10_RECONSTRUCTION_INPUT_STATE_HPP

#include <stddef.h>
#include <limits.h>

// Shared record at target base 0x474E30; observed indexed stride is 0x6A.
// Unknown spans are actual untouched data, not code-generation padding.
struct FrontEndInputView
{
    unsigned short current;
    unsigned short previous;
    unsigned short repeated;
    unsigned short pressed;
    unsigned short released;
    unsigned short counters[16];
    unsigned char unknown2A[46];
    short shotButton;
    short bombButton;
    short focusButton;
    short menuButton;
    unsigned char unknown60[8];
    short skipButton;

    int IsRepeated(unsigned short mask);
};

// Declaration only: allocation count, initialization and lifecycle unresolved.
extern FrontEndInputView g_InputRecords[];

#define TH10_INPUT_LAYOUT_ASSERT(name, condition) \
    typedef char name[(condition) ? 1 : -1]
TH10_INPUT_LAYOUT_ASSERT(InputByteIs8, CHAR_BIT == 8);
TH10_INPUT_LAYOUT_ASSERT(InputShortIs16, sizeof(short) == 2 && SHRT_MIN == -32768 && SHRT_MAX == 32767);
TH10_INPUT_LAYOUT_ASSERT(InputUnsignedShortIs16, sizeof(unsigned short) == 2 && USHRT_MAX == 65535);
TH10_INPUT_LAYOUT_ASSERT(InputCurrentAt00, offsetof(FrontEndInputView, current) == 0);
TH10_INPUT_LAYOUT_ASSERT(InputPreviousAt02, offsetof(FrontEndInputView, previous) == 2);
TH10_INPUT_LAYOUT_ASSERT(InputRepeatedAt04, offsetof(FrontEndInputView, repeated) == 4);
TH10_INPUT_LAYOUT_ASSERT(InputPressedAt06, offsetof(FrontEndInputView, pressed) == 6);
TH10_INPUT_LAYOUT_ASSERT(InputReleasedAt08, offsetof(FrontEndInputView, released) == 8);
TH10_INPUT_LAYOUT_ASSERT(InputCountersAt0A, offsetof(FrontEndInputView, counters) == 10);
TH10_INPUT_LAYOUT_ASSERT(InputCountersSize20, sizeof(((FrontEndInputView *)0)->counters) == 32);
TH10_INPUT_LAYOUT_ASSERT(InputUnknownAt2A, offsetof(FrontEndInputView, unknown2A) == 42);
TH10_INPUT_LAYOUT_ASSERT(InputShotAt58, offsetof(FrontEndInputView, shotButton) == 88);
TH10_INPUT_LAYOUT_ASSERT(InputBombAt5A, offsetof(FrontEndInputView, bombButton) == 90);
TH10_INPUT_LAYOUT_ASSERT(InputFocusAt5C, offsetof(FrontEndInputView, focusButton) == 92);
TH10_INPUT_LAYOUT_ASSERT(InputMenuAt5E, offsetof(FrontEndInputView, menuButton) == 94);
TH10_INPUT_LAYOUT_ASSERT(InputUnknownAt60, offsetof(FrontEndInputView, unknown60) == 96);
TH10_INPUT_LAYOUT_ASSERT(InputSkipAt68, offsetof(FrontEndInputView, skipButton) == 104);
TH10_INPUT_LAYOUT_ASSERT(InputStrideIs6A, sizeof(FrontEndInputView) == 106);
#undef TH10_INPUT_LAYOUT_ASSERT

#endif
