#include "GameStatistics.hpp"
#include "FileSystem.hpp"
#include <stdlib.h>
#include <string.h>
#include "Decompress.hpp"
#include "Rng.hpp"
// The record initializer is reconstructed in GameStatisticsRecordDefaults.cpp.
// The retail callee has a stack argument and RET 4; its real source is
// included in the statistics constructor LTCG graph instead of a stand-in.
void InitializeStatisticsTail42ADD0(GameStatisticsTail *tail);
void __stdcall InitializeGameStatisticsRecord(GameStatisticsRecord *record);

GameStatisticsOwner::GameStatisticsOwner()
{
    memset(this, 0, sizeof(*this));
    int fileSize;
    header = reinterpret_cast<GameStatisticsFileHeader *>(FileSystem::OpenFile("scoreth10.dat", &fileSize, 1));
    InitializeStatisticsTail42ADD0(&tail);
    for (GameStatisticsRecord *record = records; static_cast<unsigned int>(record - records) < 7; ++record)
        InitializeGameStatisticsRecord(record);
    Load42B030();
}

GameStatisticsOwner *CreateStatistics42AF20()
{
    g_GameStatisticsRows = new GameStatisticsOwner;
    return g_GameStatisticsRows;
}

int GameStatisticsOwner::Load42B030()
{
    if (header != NULL && header->magic == 0x30314854 &&
        header->formatVersion == 3)
    {
        FileSystem::Decrypt(reinterpret_cast<unsigned char *>(header) + 0x18,
            header->compressedSize, 0xac, 0x35, 0x10, header->compressedSize);
        unsigned char *input = reinterpret_cast<unsigned char *>(header) + 0x18;
        // Original 0x0042B071 performs SHL EAX,2 on the stored 32-bit size.
        // Preserve the low word without signed-left-shift undefined behavior.
        loadedPayload = static_cast<unsigned char *>(malloc(
            static_cast<unsigned int>(header->uncompressedSize) << 2));
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
                    // Serialized payload bytes are addressed with native
                    // DWORD offsets. The retail loader does not clamp a
                    // corrupt input recordIndex to the seven-entry array.
                    unsigned int checksum = 0;
                    for (unsigned int i = 0; i < 0x4374; ++i)
                        checksum += *reinterpret_cast<const unsigned char *>(
                            reinterpret_cast<unsigned int>(cursor) +
                            static_cast<unsigned int>(i) + 8u);
                    if (checksum == record->checksum && record->unknown08 == 0x437c)
                        memcpy(reinterpret_cast<GameStatisticsRecord *>(
                            reinterpret_cast<unsigned int>(&records[0]) +
                            static_cast<unsigned int>(record->recordIndex) *
                            static_cast<unsigned int>(sizeof(GameStatisticsRecord))),
                        cursor, 0x437c);
                }
            }
            else if (record->magic == 0x5453)
            {
                if (record->unknown02 == 0)
                {
                    unsigned int checksum = 0;
                    for (unsigned int i = 0; i < 0x440; ++i)
                        checksum += *reinterpret_cast<const unsigned char *>(
                            reinterpret_cast<unsigned int>(cursor) +
                            static_cast<unsigned int>(i) + 8u);
                    if (checksum == record->checksum && record->unknown08 == 0x448)
                        memcpy(&tail, cursor, 0x448);
                }
            }
            else
                goto fallback;
            // Stream records use native low-32-bit size words. An unsigned
            // view preserves the original wrapped subtraction and its signed
            // negative check, without an out-of-range integer conversion.
            *reinterpret_cast<unsigned int *>(&remaining) -= record->unknown08;
            if (remaining < 0)
                goto fallback;
            // A malformed recorded length may advance beyond the backing
            // array; the retail cursor is a flat Win32 address word.
            cursor = reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(cursor) + record->unknown08);
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
    header->magic = 0x30314854;
    header->formatVersion = 3;
    header->unknown0C = 0x100;
    return 0;
}

void InitializeStatisticsTail42ADD0(GameStatisticsTail *tail)
{
    memset(tail, 0, sizeof(*tail));
    unsigned char *bytes = reinterpret_cast<unsigned char *>(tail);
    tail->magic = 0x5453;
    tail->unknown002 = 0;
    *reinterpret_cast<unsigned int *>(bytes + 8) = sizeof(*tail);
    strcpy(tail->parsedPayload.initialText0C, "        ");
    for (int i = 0; i < 512; ++i)
        tail->parsedPayload.randomWords046[i] = g_RngView.GetRandomU16();
}
