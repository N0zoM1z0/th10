#pragma once

namespace ReplayFile
{
int Open(const char *path);
void *Read(unsigned int size);
}

namespace FileSystem
{
unsigned char *__stdcall OpenFile(const char *path, int *sizeOut, int mode);
int __stdcall CheckIfFileAlreadyExists(const char *path);
bool LoadArchive(const char *filename);
int CloseWriteFile();

unsigned char *Decrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes);
unsigned char *Encrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes);
}
