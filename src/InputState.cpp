#include "InputState.hpp"

// Complete target 0x40AC20..0x40AC8C. The target clears +4, but accumulates
// only bit zero in +0x30 for every repeating key. Do not replace this with a
// conventional per-key repeat mask or reset +0x30 as an inferred correction.
void FrontEndInputView::UpdateDebugRepeat()
{
    unsigned short bits = previousRaw;
    repeated = 0;
    for (int i = 0; i < 16; ++i)
    {
        if (bits & 1)
        {
            ++repeatCounters[i];
            if (repeatCounters[i] >= 26)
            {
                repeatFlags |= 1;
                repeatCounters[i] -= 8;
            }
        }
        else
            repeatCounters[i] = 0;
        bits >>= 1;
    }
    frameMask032 = (previousRaw ^ previousHeld) & previousRaw;
    frameMask034 = (previousRaw ^ previousHeld) & ~previousRaw;
}
