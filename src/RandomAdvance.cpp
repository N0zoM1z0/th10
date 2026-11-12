#include "Rng.hpp"
#include <string.h>

// Original 0x0044BA80..0x0044BB1A (155 bytes): two returned 16-bit draws,
// followed by two more seed advances. All four advances are observable via
// seed and generationCount; the upper word is the SECOND draw, not the first.
// The original uses a /GS-protected local result buffer; source attribution,
// caller and original private ABI remain unresolved.
unsigned int RngView::GetRandomU32SkipNextPair()
{
    unsigned short result[4];
    result[0] = GetRandomU16();
    result[1] = GetRandomU16();
    result[2] = GetRandomU16();
    result[3] = GetRandomU16();
    // Retail reads the low two 16-bit draws as one native DWORD. Copy the
    // representation rather than dereferencing an unaligned integer alias.
    unsigned int resultWord;
    memcpy(&resultWord, result, sizeof resultWord);
    return resultWord;
}
