#include "FileSystem.hpp"

#include <windows.h>
#include <stdlib.h>
#include <string.h>

extern HANDLE gReplayFileHandle;
extern CRITICAL_SECTION gReplayFileCriticalSection;
extern unsigned char gReplayFileOpenCount;

// Maintained reconstruction source. The namespace/function name is retained as
// an adjacent-supported descriptive name after TH10 target behavior recovery;
// original TU and physical compiler ownership remain unknown.
namespace FileSystem
{
unsigned char *Decrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes)
{
    int remainder = size % chunkSize;
    int unencryptedBytes = remainder < chunkSize / 4 ? remainder : 0;
    int copySize = maxBytes > size ? size : maxBytes;
    unsigned char *temporary = (unsigned char *)malloc(copySize);
    if (temporary == NULL)
        return data;

    unencryptedBytes += size & 1;
    size -= unencryptedBytes;
    memcpy(temporary, data, copySize);

    unsigned char *inputCursor = temporary;
    unsigned char *outputCursor = data;

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
            chunkSize = size;

        unsigned char *outputCursorBackup = outputCursor;
        outputCursor += chunkSize - 1;

        int i;
        for (i = (chunkSize + 1) / 2; i > 0; i--, inputCursor++)
        {
            *outputCursor = *inputCursor ^ xorValue;
            outputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        outputCursor = outputCursorBackup + chunkSize - 2;
        for (i = chunkSize / 2; i > 0; i--, inputCursor++)
        {
            *outputCursor = *inputCursor ^ xorValue;
            outputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        size -= chunkSize;
        outputCursor = outputCursorBackup + chunkSize;
        maxBytes -= chunkSize;
    }

    free(temporary);
    return data;
}

unsigned char *Encrypt(unsigned char *data, int size, unsigned char xorValue,
                       unsigned char xorValueIncrement, int chunkSize,
                       int maxBytes)
{
    int remainder = size % chunkSize;
    int unencryptedBytes = remainder < chunkSize / 4 ? remainder : 0;
    int copySize = maxBytes > size ? size : maxBytes;
    unsigned char *temporary = (unsigned char *)malloc(copySize);
    if (temporary == NULL)
        return data;

    unencryptedBytes += size & 1;
    size -= unencryptedBytes;
    memcpy(temporary, data, copySize);

    unsigned char *inputCursor = temporary;
    unsigned char *outputCursor = data;

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
            chunkSize = size;

        unsigned char *inputCursorBackup = inputCursor;
        inputCursor += chunkSize - 1;

        int i;
        for (i = (chunkSize + 1) / 2; i > 0; i--, outputCursor++)
        {
            *outputCursor = *inputCursor ^ xorValue;
            inputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        inputCursor = inputCursorBackup + chunkSize - 2;
        for (i = chunkSize / 2; i > 0; i--, outputCursor++)
        {
            *outputCursor = *inputCursor ^ xorValue;
            inputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        size -= chunkSize;
        inputCursor = inputCursorBackup + chunkSize;
        maxBytes -= chunkSize;
    }

    free(temporary);
    return data;
}

int CloseWriteFile()
{
    if (gReplayFileHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(gReplayFileHandle);
        LeaveCriticalSection(&gReplayFileCriticalSection);
        --gReplayFileOpenCount;
    }
    return 0;
}
}
