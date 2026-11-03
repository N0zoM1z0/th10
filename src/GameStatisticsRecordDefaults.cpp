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

    unsigned char *activeByte = reinterpret_cast<unsigned char *>(record) + 0x14;
    int groupsRemaining = 5;
    do
    {
        int score = 1000000;
        do
        {
            *reinterpret_cast<int *>(activeByte - 4) = score;
            activeByte[0] = 1;
            memcpy(activeByte + 2, g_GameStatisticsDefaultEntryName, 9);
            *reinterpret_cast<int *>(activeByte + 12) = 0;
            activeByte[1] = 0;
            score -= 100000;
            activeByte += 24;
        } while (score > 0);
    } while (--groupsRemaining != 0);

    int *kindData = reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(record) + 0x628);
    int first = 0;
    int second = 1;
    int fourth = 3;
    do
    {
        kindData[-1] = first;
        kindData[0] = g_GameStatisticsRankKind[first];
        kindData[35] = second;
        kindData[36] = g_GameStatisticsRankKind[second];
        kindData[71] = fourth-1;
        kindData[72] = g_GameStatisticsRankKind[fourth-1];
        kindData[107] = fourth;
        kindData[108] = g_GameStatisticsRankKind[fourth];
        kindData[143] = first+4;
        kindData[144] = g_GameStatisticsRankKind[first+4];
        first += 5;
        second += 5;
        fourth += 5;
        kindData += 180;
    } while (second < 111);

}
