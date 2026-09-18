#pragma once

namespace FileSystem
{
unsigned char *OpenFile(const char *path, int *sizeOut, int mode);
int CheckIfFileAlreadyExists(const char *path);
int CloseWriteFile();

unsigned char *Decrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes);
unsigned char *Encrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes);
}
