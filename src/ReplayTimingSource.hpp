#ifndef TH10_REPLAY_TIMING_SOURCE_HPP
#define TH10_REPLAY_TIMING_SOURCE_HPP
#include <stddef.h>

// Shared existing partial timing view at pointer global 0x477708.
// The declaration does not allocate or establish the global storage owner.
#pragma pack(push, 4)
struct ReplayTimingSource
{
    unsigned char unknown000[0x24];
    double lagNumerator;
    double lagDenominator;
};
#pragma pack(pop)
typedef char ReplayTimingSourceNumeratorAt24[
    (offsetof(ReplayTimingSource, lagNumerator) == 0x24) ? 1 : -1];
typedef char ReplayTimingSourceDenominatorAt2C[
    (offsetof(ReplayTimingSource, lagDenominator) == 0x2c) ? 1 : -1];

extern ReplayTimingSource *g_ReplayTimingSource;
#endif
