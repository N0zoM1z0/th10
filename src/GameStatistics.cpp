#include "GameStatistics.hpp"
#include "FileSystem.hpp"
#include "Lzss.hpp"
#include <stdlib.h>
#include <string.h>

// Target42B1E0..42B413, complete564-byte owner. Physical private EBX receiver
// does not establish a source calling convention. Preserve the target's failure
// behavior; this routine saves but never destroys the statistics owner.
int GameFinalizeStatistics42B1E0(GameStatisticsOwner *statistics)
{
    if (statistics->header == NULL)
        return -1;

    unsigned char *scratch = static_cast<unsigned char *>(malloc(0x200000));
    memcpy(scratch, statistics->header, 0x18);
    unsigned int appendOffset = 0x18;
    // The original loop carries a cursor to each record's index field,
    // at +0x0C. The magic, checksum, payload and full-copy base are
    // accessed relative to that one real field, without duplicate layouts.
    unsigned char *indexCursor =
        reinterpret_cast<unsigned char *>(&statistics->records[0].recordIndex);
    for (unsigned int i = 0; i < 7; ++i, indexCursor += sizeof(GameStatisticsRecord))
    {
        if (*reinterpret_cast<unsigned short *>(indexCursor - offsetof(GameStatisticsRecord, recordIndex)) == 0x5243)
        {
            *reinterpret_cast<unsigned int *>(indexCursor) = i;
            unsigned int checksum = 0;
            const unsigned char *payload = indexCursor - (offsetof(GameStatisticsRecord, recordIndex) - offsetof(GameStatisticsRecord, unknown08));
            for (unsigned int j = 0; j < 0x4374; ++j)
                checksum += payload[j];
            *reinterpret_cast<unsigned int *>(indexCursor - (offsetof(GameStatisticsRecord, recordIndex) - offsetof(GameStatisticsRecord, checksum))) = checksum;
            memcpy(scratch + appendOffset, indexCursor - offsetof(GameStatisticsRecord, recordIndex), sizeof(GameStatisticsRecord));
            appendOffset += 0x437c;
        }
    }

    unsigned int checksum = 0;
    for (unsigned int j = 0; j < 0x440; ++j)
        checksum += statistics->tail.payload[j];
    statistics->tail.checksum = checksum;
    memcpy(scratch + appendOffset, &statistics->tail, 0x448);
    statistics->header->uncompressedSize = appendOffset + 0x430;
    unsigned char *compressed = CompressData(
        scratch + 0x18, statistics->header->uncompressedSize,
        &statistics->header->compressedSize);
    statistics->header->fileSize = statistics->header->compressedSize + 0x18;
    FileSystem::Encrypt(compressed, statistics->header->compressedSize,
                        0xac, 0x35, 0x10, statistics->header->compressedSize);

    if (FileSystem::OpenWriteFile("scoreth10.dat") != 0)
        return -1;

    FileSystem::WriteToOpenFile(statistics->header, 0x18);
    FileSystem::WriteToOpenFile(compressed, statistics->header->compressedSize);
    FileSystem::CloseWriteFile();
    if (compressed != NULL)
        free(compressed);
    free(scratch);
    return 0;
}

// Target42AEF0..42AF1C owns both loaded buffers.
GameStatisticsOwner::~GameStatisticsOwner()
{
    if (header != NULL)
    {
        free(header);
        header = NULL;
    }
    if (loadedPayload != NULL)
    {
        free(loadedPayload);
        loadedPayload = NULL;
    }
}

// Target42AF90..42AFDB clears the global after deleting the nullable owner.
void ReleaseGameStatistics()
{
    delete g_GameStatisticsRows;
    g_GameStatisticsRows = NULL;
}
