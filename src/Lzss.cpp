#include "Lzss.hpp"
#include "Decompress.hpp"

#include <stdlib.h>

// Maintained reconstruction source. TH10 retains source-path strings for
// src/pack/LzssUtil.cpp, but no reviewed code xref binds these bodies to an
// original translation unit or original C++ identifiers. Physical normal-COFF
// versus LTCG ownership also remains unknown.
LzssTreeNode g_LzssTree[0x2001];
typedef char LzssTreeSizeIs1800C[
    (sizeof(g_LzssTree) == 0x1800c) ? 1 : -1];

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)
#define LZSS_BREAKEVEN 3
#define LZSS_LOOKAHEAD_MAX ((1 << LZSS_LENGTH_BITS) + LZSS_BREAKEVEN - 1)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

#define ENCODE_ADVANCE_WRITE_HEAD \
    outputBitMask >>= 1; \
    if (outputBitMask == 0) \
    { \
        *outputCursor++ = (unsigned char)outputBits; \
        outputBits = 0; \
        outputBitMask = 0x80; \
    }

#define ENCODE_PACK_BIT(bit) \
    if (bit) \
        outputBits |= outputBitMask; \
    ENCODE_ADVANCE_WRITE_HEAD;

#define ENCODE_PACK_BITS(bitCount, writeOneIf) \
    bitfieldMask = 1 << ((bitCount) - 1); \
    while (bitfieldMask != 0) \
    { \
        if (writeOneIf) \
            outputBits |= outputBitMask; \
        ENCODE_ADVANCE_WRITE_HEAD; \
        bitfieldMask >>= 1; \
    }

unsigned char *CompressData(unsigned char *input, int inputSize, int *outputSize)
{
    unsigned char outputBitMask = 0x80;
    unsigned int outputBits = 0;
    unsigned char *output = (unsigned char *)malloc(inputSize * 2);
    if (output == NULL)
        return NULL;

    unsigned char *inputCursor = input;
    unsigned char *outputCursor = output;
    *outputSize = 0;
    Lzss::InitEncoderState();

    unsigned int dictionaryHead = 1;
    int i;
    int dictionaryValue;
    for (i = 0; i < LZSS_LOOKAHEAD_MAX; i++)
    {
        if (inputCursor - input >= inputSize)
            dictionaryValue = -1;
        else
            dictionaryValue = *inputCursor++;

        if (dictionaryValue == -1)
            break;
        g_DecompressionRing[dictionaryHead + i] = (unsigned char)dictionaryValue;
    }

    int maxMatchLength = i;
    Lzss::InitTree(dictionaryHead);
    int matchLength = 0;
    int matchOffset = 0;
    unsigned int bitfieldMask;

    while (maxMatchLength > 0)
    {
        if (matchLength > maxMatchLength)
            matchLength = maxMatchLength;

        int bytesToCopyToDictionary;
        if (matchLength < LZSS_BREAKEVEN)
        {
            bytesToCopyToDictionary = 1;
            ENCODE_PACK_BIT(1);
            ENCODE_PACK_BITS(8,
                             (bitfieldMask & g_DecompressionRing[dictionaryHead]) != 0);
        }
        else
        {
            ENCODE_PACK_BIT(0);
            ENCODE_PACK_BITS(LZSS_OFFSET_BITS,
                             (bitfieldMask & matchOffset) != 0);
            ENCODE_PACK_BITS(LZSS_LENGTH_BITS,
                             (bitfieldMask & (matchLength - LZSS_BREAKEVEN)) != 0);
            bytesToCopyToDictionary = matchLength;
        }

        for (i = 0; i < bytesToCopyToDictionary; i++)
        {
            Lzss::DeleteString(LZSS_DICTPOS_MOD(dictionaryHead, LZSS_LOOKAHEAD_MAX));

            if (inputCursor - input >= inputSize)
                dictionaryValue = -1;
            else
                dictionaryValue = *inputCursor++;

            if (dictionaryValue == -1)
                maxMatchLength--;
            else
                g_DecompressionRing[
                    LZSS_DICTPOS_MOD(dictionaryHead, LZSS_LOOKAHEAD_MAX)] =
                    (unsigned char)dictionaryValue;

            dictionaryHead = LZSS_DICTPOS_MOD(dictionaryHead, 1);
            if (maxMatchLength != 0)
                matchLength = Lzss::AddString(dictionaryHead, &matchOffset);
        }
    }

    ENCODE_PACK_BIT(0);
    ENCODE_PACK_BITS(LZSS_OFFSET_BITS, false);

    *outputSize = (int)(outputCursor - output);
    return output;
}

void Lzss::InitTree(int root)
{
    g_LzssTree[LZSS_DICTSIZE].right = root;
    g_LzssTree[root].parent = LZSS_DICTSIZE;
    g_LzssTree[root].right = 0;
    g_LzssTree[root].left = 0;
}

void Lzss::InitEncoderState()
{
    int i;
    for (i = 0; i < LZSS_DICTSIZE; i++)
        g_DecompressionRing[i] = 0;

    for (i = 0; i < LZSS_DICTSIZE + 1; i++)
    {
        g_LzssTree[i].parent = 0;
        g_LzssTree[i].left = 0;
        g_LzssTree[i].right = 0;
    }
}

int Lzss::AddString(int newNode, int *matchPosition)
{
    if (newNode == 0)
        return 0;

    int testNode = g_LzssTree[LZSS_DICTSIZE].right;
    int matchLength = 0;
    for (;;)
    {
        int i;
        int delta = 0;
        for (i = 0; i < LZSS_LOOKAHEAD_MAX; i++)
        {
            delta = g_DecompressionRing[LZSS_DICTPOS_MOD(newNode, i)] -
                    g_DecompressionRing[LZSS_DICTPOS_MOD(testNode, i)];
            if (delta != 0)
                break;
        }

        if (i >= matchLength)
        {
            matchLength = i;
            *matchPosition = testNode;
            if (matchLength >= LZSS_LOOKAHEAD_MAX)
            {
                ReplaceNode(testNode, newNode);
                return matchLength;
            }
        }

        int *child;
        if (delta >= 0)
            child = &g_LzssTree[testNode].right;
        else
            child = &g_LzssTree[testNode].left;

        if (*child == 0)
        {
            *child = newNode;
            g_LzssTree[newNode].parent = testNode;
            g_LzssTree[newNode].right = 0;
            g_LzssTree[newNode].left = 0;
            return matchLength;
        }
        testNode = *child;
    }
}

void __fastcall Lzss::DeleteString(int node)
{
    if (g_LzssTree[node].parent == 0)
        return;

    if (g_LzssTree[node].right == 0)
        ContractNode(node, g_LzssTree[node].left);
    else if (g_LzssTree[node].left == 0)
        ContractNode(node, g_LzssTree[node].right);
    else
    {
        int replacement = FindNextNode(node);
        DeleteString(replacement);
        ReplaceNode(node, replacement);
    }
}

void Lzss::ContractNode(int oldNode, int newNode)
{
    g_LzssTree[newNode].parent = g_LzssTree[oldNode].parent;
    int parent = g_LzssTree[oldNode].parent;
    if (g_LzssTree[parent].right == oldNode)
        g_LzssTree[parent].right = newNode;
    else
        g_LzssTree[parent].left = newNode;
    g_LzssTree[oldNode].parent = 0;
}

void Lzss::ReplaceNode(int oldNode, int newNode)
{
    int parent = g_LzssTree[oldNode].parent;
    if (g_LzssTree[parent].left == oldNode)
        g_LzssTree[parent].left = newNode;
    else
        g_LzssTree[parent].right = newNode;

    g_LzssTree[newNode] = g_LzssTree[oldNode];
    g_LzssTree[g_LzssTree[newNode].left].parent = newNode;
    g_LzssTree[g_LzssTree[newNode].right].parent = newNode;
    g_LzssTree[oldNode].parent = 0;
}

int Lzss::FindNextNode(int node)
{
    int next = g_LzssTree[node].left;
    while (g_LzssTree[next].right != 0)
        next = g_LzssTree[next].right;
    return next;
}

#undef ENCODE_PACK_BITS
#undef ENCODE_PACK_BIT
#undef ENCODE_ADVANCE_WRITE_HEAD
#undef LZSS_DICTPOS_MOD
#undef LZSS_DICTSIZE_MASK
#undef LZSS_LOOKAHEAD_MAX
#undef LZSS_BREAKEVEN
#undef LZSS_DICTSIZE
#undef LZSS_LENGTH_BITS
#undef LZSS_OFFSET_BITS
