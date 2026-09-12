#include "Decompress.hpp"

#include <stdlib.h>

// Maintained reconstruction source. The original TH10 identifier/TU and the
// per-function normal-COFF/LTCG owner remain unknown.
unsigned char g_DecompressionRing[0x2000];
typedef char DecompressionRingSizeIs2000[
    (sizeof(g_DecompressionRing) == 0x2000) ? 1 : -1];

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

#define DECODE_ADVANCE_READ_HEAD \
    inBitMask >>= 1; \
    if (inBitMask == 0) \
        inBitMask = 0x80;

#define DECODE_HANDLE_FETCH \
    if (inBitMask == 0x80) \
    { \
        currByte = *inputCursor; \
        if (inputCursor - input >= inputSize) \
            currByte = 0; \
        else \
            inputCursor++; \
    }

#define DECODE_UNPACK_BIT \
    DECODE_HANDLE_FETCH; \
    inputBits = currByte & inBitMask; \
    DECODE_ADVANCE_READ_HEAD;

#define DECODE_UNPACK_BITS(bitCount) \
    outputBitMask = 1 << ((bitCount) - 1); \
    inputBits = 0; \
    while (outputBitMask != 0) \
    { \
        DECODE_HANDLE_FETCH; \
        if ((currByte & inBitMask) != 0) \
            inputBits |= outputBitMask; \
        outputBitMask >>= 1; \
        DECODE_ADVANCE_READ_HEAD; \
    }

#define DECODE_WRITE_BYTE(data) \
    *outputCursor++ = (unsigned char)(data); \
    g_DecompressionRing[dictionaryHead] = (unsigned char)(data); \
    dictionaryHead = LZSS_DICTPOS_MOD(dictionaryHead, 1);

unsigned char *DecompressData(unsigned char *input, int inputSize,
                              unsigned char *output, int outputSize)
{
    unsigned char inBitMask = 0x80;
    unsigned int currByte = 0;
    unsigned char *inputCursor = input;
    unsigned char *outputCursor;
    unsigned int dictionaryHead = 1;
    unsigned int inputBits;
    unsigned int outputBitMask;
    int matchOffset;
    int matchLength;
    int i;

    if (output == NULL)
    {
        output = (unsigned char *)malloc(outputSize);
        if (output == NULL)
            return NULL;
    }

    outputCursor = output;

    for (;;)
    {
        DECODE_UNPACK_BIT;
        if (inputBits != 0)
        {
            DECODE_UNPACK_BITS(8);
            DECODE_WRITE_BYTE(inputBits);
        }
        else
        {
            DECODE_UNPACK_BITS(13);
            matchOffset = (int)inputBits;
            if (matchOffset == 0)
                break;

            DECODE_UNPACK_BITS(4);
            matchLength = (int)inputBits + 2;
            for (i = 0; i <= matchLength; i++)
            {
                unsigned int dictionaryValue =
                    g_DecompressionRing[LZSS_DICTPOS_MOD(matchOffset, i)];
                DECODE_WRITE_BYTE(dictionaryValue);
            }
        }
    }

    while (inBitMask != 0x80)
        DECODE_UNPACK_BIT;

    return output;
}

#undef DECODE_WRITE_BYTE
#undef DECODE_UNPACK_BITS
#undef DECODE_UNPACK_BIT
#undef DECODE_HANDLE_FETCH
#undef DECODE_ADVANCE_READ_HEAD
#undef LZSS_DICTPOS_MOD
#undef LZSS_DICTSIZE_MASK
#undef LZSS_DICTSIZE
#undef LZSS_LENGTH_BITS
#undef LZSS_OFFSET_BITS
