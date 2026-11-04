// Original 0x004088C0..0x0040890A: count occurrences of one signed rank
// kind across the 22 five-byte records at original table 0x004743C0.
// The first register (ECX) is unused; EDX holds the signed rank kind. Both
// the original source identity and the physical owner remain indeterminate.
// This is the SAME table consumed by GameStatisticsRecordDefaults.cpp, not a
// replacement array or private copy of its bytes.
extern signed char g_GameStatisticsRankKind[110];

int __fastcall CountGameStatisticsRankKind(int unused, int kind)
{
    int count = 0;
    int index = 0;
    do
    {
        if (g_GameStatisticsRankKind[index] == kind)
            ++count;
        if (g_GameStatisticsRankKind[index + 1] == kind)
            ++count;
        if (g_GameStatisticsRankKind[index + 2] == kind)
            ++count;
        if (g_GameStatisticsRankKind[index + 3] == kind)
            ++count;
        if (g_GameStatisticsRankKind[index + 4] == kind)
            ++count;
        index += 5;
    } while (index < 110);
    return count;
}
