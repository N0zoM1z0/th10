#pragma once
#include <stddef.h>
#include <string.h>

// Target-backed partial storage views, not claims about original source names.
struct GameStatisticsFileHeader
{
    unsigned int unknown00;
    unsigned int fileSize;
    unsigned int unknown08;
    unsigned int unknown0C;
    int compressedSize;
    int uncompressedSize;
};

struct GameStatisticsRecord
{
    unsigned short magic;
    unsigned short unknown02;
    unsigned int checksum;
    unsigned int unknown08;
    unsigned int recordIndex;
    unsigned char unknown010[0x4b0];
    int sessionStarts;
    int activeFrames;
    unsigned char unknown4C8[0x3eb4];

    // Existing GameManager accesses were owner+i*437C+stage*F0+18/+1D.
    // Records begin at owner+8, hence record-relative +10/+15. These copies
    // preserve those effective offsets without inventing an overlapping array.
    int StageInitialScore(int stage) const
    {
        int value;
        memcpy(&value, reinterpret_cast<const unsigned char *>(this) +
                           stage * 0xf0 + 0x10, sizeof(value));
        return value;
    }
    signed char StageStartupValue(int stage) const
    {
        signed char value;
        memcpy(&value, reinterpret_cast<const unsigned char *>(this) +
                           stage * 0xf0 + 0x15, sizeof(value));
        return value;
    }
};

struct GameStatisticsTail
{
    unsigned int unknown00;
    unsigned int checksum;
    unsigned char payload[0x440];
};

struct GameStatisticsOwner
{
    GameStatisticsOwner();
    ~GameStatisticsOwner();
    int Load42B030();
    GameStatisticsFileHeader *header;
    unsigned char *loadedPayload; // Separately freed at42AEF0; meaning unresolved.
    GameStatisticsRecord records[7];
    GameStatisticsTail tail;
};

typedef char GameStatisticsHeaderSize18[(sizeof(GameStatisticsFileHeader) == 0x18) ? 1 : -1];
typedef char GameStatisticsCompressedAt10[(offsetof(GameStatisticsFileHeader, compressedSize) == 0x10) ? 1 : -1];
typedef char GameStatisticsUncompressedAt14[(offsetof(GameStatisticsFileHeader, uncompressedSize) == 0x14) ? 1 : -1];
typedef char GameStatisticsRecordChecksumAt4[(offsetof(GameStatisticsRecord, checksum) == 4) ? 1 : -1];
typedef char GameStatisticsRecordIndexAtC[(offsetof(GameStatisticsRecord, recordIndex) == 0xc) ? 1 : -1];
typedef char GameStatisticsTailChecksumAt4[(offsetof(GameStatisticsTail, checksum) == 4) ? 1 : -1];
typedef char GameStatisticsTailPayloadAt8[(offsetof(GameStatisticsTail, payload) == 8) ? 1 : -1];
typedef char GameStatisticsRecordSize437C[(sizeof(GameStatisticsRecord) == 0x437c) ? 1 : -1];
typedef char GameStatisticsSessionsAt4C0[(offsetof(GameStatisticsRecord, sessionStarts) == 0x4c0) ? 1 : -1];
typedef char GameStatisticsFramesAt4C4[(offsetof(GameStatisticsRecord, activeFrames) == 0x4c4) ? 1 : -1];
typedef char GameStatisticsTailSize448[(sizeof(GameStatisticsTail) == 0x448) ? 1 : -1];
// Production x86 owner assertions; host fixture removes exactly this block,
// leaving all pointer-free layout assertions active. No production test macro.
// GAME_STATISTICS_X86_LAYOUT_BEGIN
typedef char GameStatisticsRecordsAt8[(offsetof(GameStatisticsOwner, records) == 8) ? 1 : -1];
typedef char GameStatisticsTailAt1D86C[(offsetof(GameStatisticsOwner, tail) == 0x1d86c) ? 1 : -1];
typedef char GameStatisticsOwnerSize1DCB4[(sizeof(GameStatisticsOwner) == 0x1dcb4) ? 1 : -1];
// GAME_STATISTICS_X86_LAYOUT_END

// One canonical slot47783C; Enemy's existing byte-view alias remains an explicit
// physical alias obligation, not a second definition/allocation here.
extern GameStatisticsOwner *g_GameStatisticsRows;
int GameFinalizeStatistics42B1E0(GameStatisticsOwner *statistics);

GameStatisticsOwner *CreateStatistics42AF20();
void ReleaseGameStatistics();
