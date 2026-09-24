#ifndef TH10_SOUND_FORMAT_HPP
#define TH10_SOUND_FORMAT_HPP

#include <windows.h>
#include <mmreg.h>

// TH10's BGM-format table has 0x34-byte rows.  The Reopen path independently
// reads startOffset at +0x10 and totalLength at +0x1C.
struct ThBgmFormat
{
    char name[16];
    int startOffset;
    DWORD preloadAllocSize;
    int introLength;
    int totalLength;
    WAVEFORMATEX format;
};

typedef char ThBgmFormatSizeIs34[
    (sizeof(ThBgmFormat) == 0x34) ? 1 : -1];

#endif
