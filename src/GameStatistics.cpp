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
    // Traverse the enclosing owner by its actual recordIndex field. The
    // independent loop-index lifetime restores retail's two setup stores.
    unsigned int i = 0;
    unsigned char *indexCursor = reinterpret_cast<unsigned char *>(statistics)
        + offsetof(GameStatisticsOwner, records)
        + offsetof(GameStatisticsRecord, recordIndex);
    for (; i < 7; ++i, indexCursor += sizeof(GameStatisticsRecord))
    {
        if (*reinterpret_cast<unsigned short *>(
                indexCursor - offsetof(GameStatisticsRecord, recordIndex))
            == 0x5243)
        {
            *reinterpret_cast<unsigned int *>(indexCursor) = i;
            unsigned int checksum = 0;
            const unsigned char *payload = indexCursor -
                (offsetof(GameStatisticsRecord, recordIndex) -
                 offsetof(GameStatisticsRecord, unknown08));
            for (unsigned int j = 0; j < 0x4374; ++j)
                checksum += payload[j];
            *reinterpret_cast<unsigned int *>(indexCursor -
                (offsetof(GameStatisticsRecord, recordIndex) -
                 offsetof(GameStatisticsRecord, checksum))) = checksum;
            memcpy(scratch + appendOffset,
                   indexCursor - offsetof(GameStatisticsRecord, recordIndex),
                   sizeof(GameStatisticsRecord));
            appendOffset += sizeof(GameStatisticsRecord);
        }
    }

    unsigned int checksum = 0;
    // Each group owns four real bytes; start from byte +2 so the original
    // four load offsets and its decrementing group counter are preserved.
    const unsigned char *tailCursor =
        reinterpret_cast<const unsigned char *>(statistics)
        + offsetof(GameStatisticsOwner, tail)
        + offsetof(GameStatisticsTail, payload) + 2;
    unsigned int remaining = 0x110;
    do {
        checksum += tailCursor[-1] + tailCursor[-2] +
                    tailCursor[1] + tailCursor[0];
        tailCursor += 4;
    } while (--remaining != 0);
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
