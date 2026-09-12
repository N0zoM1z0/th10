#include "PbgArchive.hpp"
#include "ResFile.hpp"

#include <stdlib.h>
#include <string.h>

// This file is maintained reconstruction source.  TH10's original source file,
// translation-unit partition, and per-function normal-COFF/LTCG ownership are
// still unknown; source presence here is not an exactness or object-owner claim.
namespace FileSystem
{
unsigned char *Decrypt(unsigned char *data, i32 size, u8 xorValue,
                       u8 xorValueIncrement, i32 chunkSize, i32 maxBytes);
}

unsigned char *DecompressData(unsigned char *input, i32 inputSize,
                              unsigned char *output, i32 outputSize);

struct PbgDecryptProfile
{
    u8 xorValue;
    u8 xorValueIncrement;
    u8 unknown02[2];
    i32 chunkSize;
    i32 maxBytes;
};
typedef char PbgDecryptProfileSizeIs0C[
    (sizeof(PbgDecryptProfile) == 0x0c) ? 1 : -1];
extern PbgDecryptProfile g_PbgDecryptProfiles[8];

PbgArchiveEntry::PbgArchiveEntry()
{
    filename = NULL;
}

PbgArchiveEntry::~PbgArchiveEntry()
{
    if (filename != NULL)
    {
        free(filename);
        filename = NULL;
    }
}

PbgArchive::PbgArchive()
{
    m_Entries = NULL;
    m_NumOfEntries = 0;
    m_Filename = NULL;
    m_FileAbstraction = NULL;
}

PbgArchive::~PbgArchive()
{
    Release();
}

bool PbgArchive::Load(const char *filename)
{
    Release();

    m_FileAbstraction = new CPbgFile;
    if (m_FileAbstraction == NULL)
        return false;

    if (ParseHeader(filename))
    {
        m_Filename = CopyFileName(filename);
        if (m_Filename != NULL)
        {
            m_FileAbstraction->Open(m_Filename, g_PbgFileOpenModes[0]);
            return true;
        }
    }

    Release();
    return false;
}

bool PbgArchive::LoadFromResource(const char *resourceName)
{
    Release();

    m_FileAbstraction = new CWin32ResourcePbgFile;
    if (m_FileAbstraction == NULL)
        return false;

    if (ParseHeader(resourceName))
    {
        m_Filename = CopyFileName(resourceName);
        if (m_Filename != NULL)
            return true;
    }

    Release();
    return false;
}

void PbgArchive::Release()
{
    if (m_Filename != NULL)
    {
        free(m_Filename);
        m_Filename = NULL;
    }

    if (m_Entries != NULL)
    {
        delete[] m_Entries;
        m_Entries = NULL;
    }

    if (m_FileAbstraction != NULL)
    {
        delete m_FileAbstraction;
        m_FileAbstraction = NULL;
    }

    m_NumOfEntries = 0;
}

unsigned char *PbgArchive::ReadDecompressEntry(const char *filename,
                                                unsigned char *outBuffer)
{
    unsigned char *compressedData = NULL;

    if (m_FileAbstraction == NULL)
        return NULL;

    PbgArchiveEntry *entry = FindEntry(filename);
    if (entry == NULL)
        goto read_error;

    i32 compressedSize = (i32)(entry[1].dataOffset - entry->dataOffset);
    u32 decompressedSize = entry->decompressedSize;

    if (compressedSize != (i32)decompressedSize || outBuffer == NULL)
    {
        compressedData = (unsigned char *)malloc(compressedSize);
        if (compressedData == NULL)
            goto read_error;
    }
    else
    {
        compressedData = outBuffer;
    }

    if (!m_FileAbstraction->Seek(entry->dataOffset, g_PbgFileSeekModes[0]))
        goto read_error;
    if (m_FileAbstraction->Read(compressedData, compressedSize) == 0)
        goto read_error;

    u8 filenameChecksum = 0;
    for (const u8 *cursor = (const u8 *)entry->filename; *cursor != 0; cursor++)
        filenameChecksum = (u8)(filenameChecksum + *cursor);
    PbgDecryptProfile &profile = g_PbgDecryptProfiles[filenameChecksum % 8];
    FileSystem::Decrypt(compressedData, compressedSize, profile.xorValue,
                        profile.xorValueIncrement, profile.chunkSize,
                        profile.maxBytes);

    unsigned char *decompressedData;
    if (compressedSize != (i32)decompressedSize)
        decompressedData = DecompressData(compressedData, compressedSize,
                                          outBuffer, decompressedSize);
    else
        decompressedData = compressedData;

    if (compressedData != outBuffer && compressedData != NULL)
        free(compressedData);
    return decompressedData;

read_error:
    if (compressedData != NULL)
        free(compressedData);
    return NULL;
}

u32 PbgArchive::GetEntryDecompressedSize(const char *filename)
{
    PbgArchiveEntry *entry = FindEntry(filename);
    if (entry != NULL)
        return entry->decompressedSize;
    return 0;
}

PbgArchiveEntry *PbgArchive::FindEntry(const char *filename)
{
    if (m_Entries == NULL)
        return NULL;

    PbgArchiveEntry *entry = m_Entries;
    for (i32 i = m_NumOfEntries; i > 0; i--, entry++)
    {
        if (_stricmp(filename, entry->filename) == 0)
            return entry;
    }
    return NULL;
}

bool PbgArchive::ParseHeader(const char *filename)
{
    PbgArchiveHeader header;
    unsigned char *fileTableBuffer = NULL;
    unsigned char *decompressedFileTable = NULL;

    if (m_FileAbstraction == NULL)
        return false;
    if (!m_FileAbstraction->Open(filename, g_PbgFileOpenModes[0]))
        goto parse_error;
    if (m_FileAbstraction->Read(&header, sizeof(header)) == 0)
        goto parse_error;

    FileSystem::Decrypt((unsigned char *)&header, sizeof(header), 0x1b, 0x37,
                        sizeof(header), sizeof(header));
    if (header.magic != 0x31414854)
        goto parse_error;

    header.encodedFileTableDecompressedSize -= 123456789;
    header.encodedFileTableCompressedSize -= 987654321;
    m_NumOfEntries = header.encodedEntryCount - 135792468;

    u32 archiveSize = m_FileAbstraction->GetSize();
    u32 fileTableOffset =
        archiveSize - (u32)header.encodedFileTableCompressedSize;
    m_FileAbstraction->Seek(fileTableOffset, g_PbgFileSeekModes[0]);

    i32 fileTableSize = header.encodedFileTableCompressedSize;
    fileTableBuffer = (unsigned char *)malloc(fileTableSize);
    if (fileTableBuffer == NULL)
        goto parse_error;
    if (m_FileAbstraction->Read(fileTableBuffer, fileTableSize) == 0)
        goto parse_error;

    FileSystem::Decrypt(fileTableBuffer, fileTableSize, 0x3e, 0x9b, 0x80,
                        fileTableSize);
    decompressedFileTable = DecompressData(
        fileTableBuffer, fileTableSize, NULL,
        header.encodedFileTableDecompressedSize);
    if (decompressedFileTable == NULL)
        goto parse_error;

    m_Entries = AllocEntries(decompressedFileTable, m_NumOfEntries,
                             fileTableOffset);
    if (m_Entries == NULL)
        goto parse_error;

    free(fileTableBuffer);
    free(decompressedFileTable);
    return true;

parse_error:
    free(fileTableBuffer);
    free(decompressedFileTable);
    if (m_FileAbstraction != NULL)
    {
        delete m_FileAbstraction;
        m_FileAbstraction = NULL;
    }
    return false;
}

PbgArchiveEntry *PbgArchive::AllocEntries(void *entryBuffer, i32 count,
                                          u32 dataOffset)
{
    PbgArchiveEntry *buffer = new PbgArchiveEntry[count + 1];
    if (buffer == NULL)
        return NULL;

    void *entryData = entryBuffer;
    for (i32 i = 0; i < count; i++)
    {
        buffer[i].filename = CopyFileName((const char *)entryData);
        SeekPastString(&entryData);
        buffer[i].dataOffset = *(u32 *)entryData;
        SeekPastInt(&entryData);
        buffer[i].decompressedSize = *(u32 *)entryData;
        SeekPastInt(&entryData);
        buffer[i].unconsumedMetadata = *(u32 *)entryData;
        SeekPastInt(&entryData);
    }

    buffer[count].dataOffset = dataOffset;
    buffer[count].decompressedSize = 0;
    return buffer;
}

char *PbgArchive::CopyFileName(const char *filename)
{
    size_t size = strlen(filename) + 1;
    char *copy = (char *)malloc(size);
    if (copy != NULL)
        strcpy(copy, filename);
    return copy;
}

i32 PbgArchive::SeekPastInt(void **ptr)
{
    *ptr = (u8 *)*ptr + sizeof(i32);
    return *(i32 *)*ptr;
}

void *PbgArchive::SeekPastString(void **ptr)
{
    i32 size = (i32)strlen((const char *)*ptr) + 1;
    if (size % 4 != 0)
        size += 4 - size % 4;
    *ptr = (u8 *)*ptr + size;
    return *ptr;
}
