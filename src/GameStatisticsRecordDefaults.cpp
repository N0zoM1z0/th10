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
            memcpy(activeByte - 4, &score, sizeof score);
            activeByte[0] = 1;
            // The original reads the fixed name as two DWORDs and a final
            // byte. Keep the ninth-byte store after the independent status
            // and score updates, preserving its true destination word.
            unsigned char *const nameDest = activeByte + 2;
            // Two independent four-byte copies preserve the target DWORD
            // read/write order without requiring aligned uint32 pointers.
            memcpy(nameDest, &g_GameStatisticsDefaultEntryName[0], 4);
            memcpy(nameDest + 4, &g_GameStatisticsDefaultEntryName[4], 4);
            const unsigned char ninth = g_GameStatisticsDefaultEntryName[8];
            const int zero = 0;
            memcpy(activeByte + 12, &zero, sizeof zero);
            activeByte[1] = 0;
            score -= 100000;
            activeByte += 24;
            nameDest[8] = ninth;
        } while (score > 0);
    } while (--groupsRemaining != 0);

    int *kindData = reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(record) + 0x628);
    for (int index = 0; index < 110; ++index)
    {
        kindData[-1] = index;
        kindData[0] = g_GameStatisticsRankKind[index];
        kindData += 0x90 / sizeof(int);
    }
}
