#include "GameStatistics.hpp"

// Original signed 110-entry rank-kind view, read beginning at 0x004743C0.
// Another caller shares this target data. This only declares a view.
extern signed char g_GameStatisticsRankKind[110];

// Target 0x0042ACB0..0x0042ADC1: the full 274-byte score defaults owner.
// Neither the class/TU name nor the physical table allocation is established.
void __stdcall InitializeGameStatisticsRecord(GameStatisticsRecord *record)
{
    memset(record, 0, sizeof(*record));
    record->magic = 0x5243;
    record->unknown02 = 0;
    record->unknown08 = 0x437c;

    for (int group = 0; group < 5; ++group)
    {
        for (int rank = 0; rank < 10; ++rank)
        {
            record->rankScores[group][rank].score = 1000000 - rank * 100000;
            record->rankScores[group][rank].active = 1;
            // The pooled nine-byte name equals original 0x0046E858.
            strcpy(record->rankScores[group][rank].playerName, "--------");
            record->rankScores[group][rank].extra = 0;
            record->rankScores[group][rank].status = 0;
        }
    }

    for (int index = 0; index < 110; ++index)
    {
        record->spellHistory[index].spellIndex = index;
        record->spellHistory[index].difficulty = g_GameStatisticsRankKind[index];
    }
}
