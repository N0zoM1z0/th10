#pragma once

namespace FileSystem
{
unsigned char *Decrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes);
unsigned char *Encrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes);
}
