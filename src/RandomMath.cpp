#include "Rng.hpp"

#include <limits.h>

// Target 0x0044B9B0-0x0044B9D6 advances the 16-bit state and generation
// counter. The same operation is inlined twice by each wider RNG method.
unsigned short RngView::GetRandomU16()
{
    unsigned short temp = (seed ^ 0x9630) - 0x6553;
    seed = (((temp & 0xc000) >> 14) + temp * 4) & 0xffff;
    ++generationCount;
    return seed;
}

// Target 0x0044B9E0-0x0044BA2D concatenates two successive 16-bit results.
unsigned int RngView::GetRandomU32()
{
    return GetRandomU16() << 16 | GetRandomU16();
}

// Target 0x0044BB20-0x0044BB8C maps the unsigned result to [0, 1].
float RngView::GetRandomF32()
{
    return static_cast<float>(GetRandomU32()) /
        static_cast<float>(UINT_MAX);
}

// Target 0x0044BB90-0x0044BC02 maps the unsigned result to [-1, 1].
float RngView::GetRandomF32Signed()
{
    return static_cast<float>(GetRandomU32()) /
        static_cast<float>(INT_MAX) - 1.0f;
}
