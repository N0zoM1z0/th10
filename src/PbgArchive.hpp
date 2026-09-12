#pragma once

#include "PbgFile.hpp"

#include <stddef.h>
#include <windows.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

struct PbgArchiveHeader
{
    u32 magic;
    i32 encodedFileTableDecompressedSize;
    i32 encodedFileTableCompressedSize;
    i32 encodedEntryCount;
};
typedef char PbgArchiveHeaderSizeIs10[
    (sizeof(PbgArchiveHeader) == 0x10) ? 1 : -1];

struct PbgArchiveEntry
{
    PbgArchiveEntry();
    ~PbgArchiveEntry();

    char *filename;
    u32 dataOffset;
    u32 decompressedSize;
    // Copied from each file-table record.  No retail reader has been observed.
    u32 unconsumedMetadata;
};
typedef char PbgArchiveEntrySizeIs10[
    (sizeof(PbgArchiveEntry) == 0x10) ? 1 : -1];
typedef char PbgArchiveEntryDataOffsetAt04[
    (offsetof(PbgArchiveEntry, dataOffset) == 0x04) ? 1 : -1];
typedef char PbgArchiveEntryDecompressedSizeAt08[
    (offsetof(PbgArchiveEntry, decompressedSize) == 0x08) ? 1 : -1];
typedef char PbgArchiveEntryMetadataAt0C[
    (offsetof(PbgArchiveEntry, unconsumedMetadata) == 0x0c) ? 1 : -1];

class PbgArchive
{
  public:
    PbgArchive();
    ~PbgArchive();

    bool Load(const char *filename);
    void Release();
    unsigned char *ReadDecompressEntry(const char *filename,
                                       unsigned char *outBuffer);
    u32 GetEntryDecompressedSize(const char *filename);
    PbgArchiveEntry *FindEntry(const char *filename);
    bool ParseHeader(const char *filename);
    PbgArchiveEntry *AllocEntries(void *entryBuffer, i32 count, u32 dataOffset);
    char *CopyFileName(const char *filename);

    static i32 SeekPastInt(void **ptr);
    static void *SeekPastString(void **ptr);

  private:
    PbgArchiveEntry *m_Entries;
    i32 m_NumOfEntries;
    char *m_Filename;
    CPbgFile *m_FileAbstraction;
};
typedef char PbgArchiveSizeIs10[(sizeof(PbgArchive) == 0x10) ? 1 : -1];
