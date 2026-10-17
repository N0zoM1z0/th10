#ifndef INPUT_CANDIDATE_KEY_MASKS_HPP
#define INPUT_CANDIDATE_KEY_MASKS_HPP

#include <climits>

namespace input_candidate {
typedef char RequireEightBitByte[(CHAR_BIT == 8 && UCHAR_MAX == 255) ? 1 : -1];
typedef char RequireSixteenBitShort[(USHRT_MAX == 65535 && sizeof(unsigned short) == 2) ? 1 : -1];

// Precondition: keys points to 256 readable, initialized bytes.
// Only bit 7 of each byte contributes. No platform API is called.
unsigned short Win32KeyMask(const unsigned char keys[256]);
unsigned short DirectInputKeyMask(const unsigned char keys[256]);
}

#endif
