#include "FileSystem.hpp"
#include "PbgArchive.hpp"

#include <windows.h>
#include <stdlib.h>
#include <string.h>

extern HANDLE gReplayFileHandle;
extern CRITICAL_SECTION gReplayFileCriticalSection;
extern unsigned char gReplayFileOpenCount;

extern PbgArchive g_PbgArchives[20];
extern int g_PbgArchiveCount;
// Distinct target storage at 0x00497990; original global name is unknown.
extern PbgArchive g_MainPbgArchive;

// Maintained reconstruction source. The namespace/function names are retained
// after TH10 target behavior recovery; original TU and physical compiler
// ownership remain unknown.
namespace ReplayFile
{
// TH10_FILESYSTEM_FUNCTION: 0x0044B6B0 ReplayFile::Open
int Open(const char *path)
{
    EnterCriticalSection(&gReplayFileCriticalSection);
    ++gReplayFileOpenCount;
    gReplayFileHandle = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (gReplayFileHandle == INVALID_HANDLE_VALUE)
    {
        LPSTR errorMessage;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), 0x400,
            reinterpret_cast<LPSTR>(&errorMessage), 0, NULL);
        LocalFree(errorMessage);
        LeaveCriticalSection(&gReplayFileCriticalSection);
        --gReplayFileOpenCount;
        return -1;
    }
    return 0;
}

// TH10_FILESYSTEM_FUNCTION: 0x0044B790 ReplayFile::Read
// The target receives size through private EDI after LTCG; the maintained
// source keeps the ordinary source-level parameter.
void *Read(unsigned int size)
{
    DWORD bytesRead;

    if (gReplayFileHandle == INVALID_HANDLE_VALUE)
        return NULL;

    void *data = malloc(size);
    if (data == NULL)
    {
        CloseHandle(gReplayFileHandle);
        return NULL;
    }

    ReadFile(gReplayFileHandle, data, size, &bytesRead, NULL);
    return data;
}
}

namespace FileSystem
{
// Target 0x0044B360. Mode zero reads only the main archive; other modes
// read only disk. The archive basename fallback deliberately keeps the path.
unsigned char *__stdcall OpenFile(const char *path, int *sizeOut, int mode)
{
    EnterCriticalSection(&gReplayFileCriticalSection);
    ++gReplayFileOpenCount;

    DWORD size;
    unsigned char *data;
    if (mode == 0)
    {
        const char *backslash = strrchr(path, '\\');
        const char *name = strrchr(backslash == NULL ? path : backslash + 1, '/');
        name = name == NULL ? path : name + 1;
        size = g_MainPbgArchive.GetEntryDecompressedSize(name);
        if (sizeOut != NULL)
            *sizeOut = size;
        if (size == 0)
            goto openError;
        data = static_cast<unsigned char *>(malloc(size));
        if (data == NULL)
            goto openError;
        g_MainPbgArchive.ReadDecompressEntry(name, data);
    }
    else
    {
        HANDLE file = CreateFileA(
            path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (file == INVALID_HANDLE_VALUE)
            goto openError;
        size = GetFileSize(file, NULL);
        data = static_cast<unsigned char *>(malloc(size));
        if (data == NULL)
        {
            CloseHandle(file);
            goto openError;
        }
        ReadFile(file, data, size, &size, NULL);
        if (sizeOut != NULL)
            *sizeOut = size;
        CloseHandle(file);
    }

    LeaveCriticalSection(&gReplayFileCriticalSection);
    --gReplayFileOpenCount;
    return data;

openError:
    LeaveCriticalSection(&gReplayFileCriticalSection);
    --gReplayFileOpenCount;
    return NULL;
}

// TH10_FILESYSTEM_FUNCTION: 0x0044B4D0 FileSystem::CheckIfFileAlreadyExists
int __stdcall CheckIfFileAlreadyExists(const char *path)
{
    EnterCriticalSection(&gReplayFileCriticalSection);
    ++gReplayFileOpenCount;
    HANDLE handle = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        LeaveCriticalSection(&gReplayFileCriticalSection);
        --gReplayFileOpenCount;
        return 1;
    }
    LeaveCriticalSection(&gReplayFileCriticalSection);
    --gReplayFileOpenCount;
    return 0;
}

// TH10_FILESYSTEM_FUNCTION: 0x004357D0 FileSystem::LoadArchive
// The retained target body receives its filename through a private LTCG EAX
// seam. This source signature records the behavior without claiming that ABI.
bool LoadArchive(const char *filename)
{
    if (!g_PbgArchives[g_PbgArchiveCount].Load(filename))
        return false;
    ++g_PbgArchiveCount;
    return true;
}

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
