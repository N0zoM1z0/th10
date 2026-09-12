#pragma once

#include <windows.h>

extern unsigned char g_DecompressionRing[0x2000];

unsigned char *DecompressData(unsigned char *input, int inputSize,
                              unsigned char *output, int outputSize);
