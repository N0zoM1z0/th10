#pragma once

// TH10 keeps one 16-bit seed, a saved seed, and a 32-bit generation counter
// in each gameplay RNG object. The arithmetic methods are reconstructed in
// RandomMath.cpp from their complete target bodies.
struct RngView
{
    unsigned short seed;
    unsigned short savedSeed;
    unsigned int generationCount;

    unsigned short GetRandomU16();
    unsigned int GetRandomU32();
    float GetRandomF32();
    float GetRandomF32Signed();

    unsigned int GetRandomU32InRange(unsigned int range)
    {
        return range != 0 ? GetRandomU32() % range : 0;
    }

    float GetRandomF32InRange(float range)
    {
        return GetRandomF32() * range;
    }
};

typedef char RngViewSizeIs08[
    (sizeof(RngView) == 0x08) ? 1 : -1];

extern RngView g_RngView;
extern RngView g_AlternateRngView;
