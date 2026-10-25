#include "GameStatistics.hpp"
#include "FileSystem.hpp"
#include <stdlib.h>
#include <string.h>
#include "Decompress.hpp"
#include "Rng.hpp"
// The record initializer remains a declared, unreconstructed dependency.
void InitializeStatisticsTail42ADD0(GameStatisticsTail *tail);
void InitializeStatisticsRecord42ACB0(GameStatisticsRecord *record);

GameStatisticsOwner::GameStatisticsOwner()
{
    memset(this, 0, sizeof(*this));
    int fileSize;
    header = reinterpret_cast<GameStatisticsFileHeader *>(FileSystem::OpenFile("scoreth10.dat", &fileSize, 1));
    InitializeStatisticsTail42ADD0(&tail);
    for (GameStatisticsRecord *record = records; static_cast<unsigned int>(record - records) < 7; ++record)
        InitializeStatisticsRecord42ACB0(record);
    Load42B030();
}

GameStatisticsOwner *CreateStatistics42AF20()
{
    g_GameStatisticsRows = new GameStatisticsOwner;
    return g_GameStatisticsRows;
}

int GameStatisticsOwner::Load42B030()
{
    if (header != NULL && header->unknown00 == 0x30314854 &&
        *reinterpret_cast<unsigned short *>(&header->unknown08) == 3)
    {
        FileSystem::Decrypt(reinterpret_cast<unsigned char *>(header) + 0x18,
            header->compressedSize, 0xac, 0x35, 0x10, header->compressedSize);
        unsigned char *input = reinterpret_cast<unsigned char *>(header) + 0x18;
        loadedPayload = static_cast<unsigned char *>(malloc(header->uncompressedSize << 2));
        DecompressData(input, header->compressedSize, loadedPayload, header->uncompressedSize);
        int remaining = header->uncompressedSize;
        unsigned char *cursor = loadedPayload;
        while (remaining > 0)
        {
            GameStatisticsRecord *record = reinterpret_cast<GameStatisticsRecord *>(cursor);
            if (record->magic == 0x5243)
            {
                if (record->unknown02 == 0)
                {
                    unsigned int checksum = 0;
                    for (unsigned int i = 0; i < 0x4374; ++i)
                        checksum += cursor[i + 8];
                    if (checksum == record->checksum && record->unknown08 == 0x437c)
                        memcpy(&records[record->recordIndex], cursor, 0x437c);
                }
            }
            else if (record->magic == 0x5453)
            {
                if (record->unknown02 == 0)
                {
                    unsigned int checksum = 0;
                    for (unsigned int i = 0; i < 0x440; ++i)
                        checksum += cursor[i + 8];
                    if (checksum == record->checksum && record->unknown08 == 0x448)
                        memcpy(&tail, cursor, 0x448);
                }
            }
            else
                goto fallback;
            remaining -= record->unknown08;
            if (remaining < 0)
                goto fallback;
            cursor += record->unknown08;
        }
        return 0;
    }
fallback:
    if (header != NULL)
    {
        free(header);
        header = NULL;
    }
    header = static_cast<GameStatisticsFileHeader *>(malloc(0x18));
    memset(header, 0, 0x18);
    header->unknown00 = 0x30314854;
    *reinterpret_cast<unsigned short *>(&header->unknown08) = 3;
    header->unknown0C = 0x100;
    return 0;
}

void InitializeStatisticsTail42ADD0(GameStatisticsTail *tail)
{
    memset(tail, 0, sizeof(*tail));
    unsigned char *bytes = reinterpret_cast<unsigned char *>(tail);
    *reinterpret_cast<unsigned short *>(bytes) = 0x5453;
    *reinterpret_cast<unsigned short *>(bytes + 2) = 0;
    *reinterpret_cast<unsigned int *>(bytes + 8) = sizeof(*tail);
    strcpy(reinterpret_cast<char *>(bytes + 0xc), "        ");
    for (int i = 0; i < 512; ++i)
        *reinterpret_cast<unsigned short *>(bytes + 0x46 + i * 2) = g_RngView.GetRandomU16();
}
