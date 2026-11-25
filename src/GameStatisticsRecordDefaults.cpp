#include "GameStatistics.hpp"

// Original fixed-width name at 0x0046E858. The nine consumed bytes and their
// initialization order are backed by direct original PE reads. The actual
// physical string owner and adjacent storage have not been established.
extern const char g_GameStatisticsDefaultEntryName[9];
// Original signed 110-entry rank-kind view, read beginning at 0x004743C0.
// Another caller shares this target data. This only declares a view.
extern signed char g_GameStatisticsRankKind[110];

struct GameStatisticsRankScoreSlot
{
    int score;
    unsigned char active;
    unsigned char status;
    char playerName[9];
    unsigned char reserved0F;
    unsigned int extra;
    unsigned int padding;
};
typedef char GameStatisticsRankScoreSlotSize18[
    sizeof(GameStatisticsRankScoreSlot)==0x18 ? 1 : -1];

struct GameStatisticsRankKindSlot
{
    int index;
    int kind;
    unsigned char unused[0x88];
};
typedef char GameStatisticsRankKindSlotSize90[
    sizeof(GameStatisticsRankKindSlot)==0x90 ? 1 : -1];

// Target 0x0042ACB0..0x0042ADC1: the full 274-byte score defaults owner.
// Neither the class/TU name nor the physical table allocation is established.
void __stdcall InitializeGameStatisticsRecord(GameStatisticsRecord *record)
{
    memset(record, 0, sizeof(*record));
    record->magic = 0x5243;
    record->unknown02 = 0;
    record->unknown08 = 0x437c;

    GameStatisticsRankScoreSlot (*ranks)[10] =
        reinterpret_cast<GameStatisticsRankScoreSlot (*)[10]>(record->unknown010);
    for (int group = 0; group < 5; ++group)
    {
        for (int rank = 0; rank < 10; ++rank)
        {
            ranks[group][rank].score = 1000000 - rank * 100000;
            unsigned char *activeByte =
                reinterpret_cast<unsigned char *>(&ranks[group][rank]) +
                offsetof(GameStatisticsRankScoreSlot, active);
            activeByte[0] = 1;
            strcpy(reinterpret_cast<char *>(activeByte + 2), "--------");
            ranks[group][rank].extra = 0;
            ranks[group][rank].status = 0;
        }
    }

    for (int index = 0; index < 110; ++index)
    {
        record->spellHistory[index].spellIndex = index;
        record->spellHistory[index].difficulty = g_GameStatisticsRankKind[index];
    }
}
