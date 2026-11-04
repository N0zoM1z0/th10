#include "ReplayManager.hpp"
#include <time.h>

// Physical target 0x00429B30..0x00429B53 uses EDI for the ReplayManager
// and ESI for a completion selector. No original direct caller was proven.
// This descriptive C++ surface preserves the entire original behavior;
// its ordinary compiler calling convention is not an asserted private ABI.
extern int g_GameStartupMode; // Existing 0x00474C7C storage, not a new copy.

typedef char ReplayDataTimestampAt0C[
    offsetof(ReplayDataHeader, timestamp) == 0x0C ? 1 : -1];
typedef char ReplayDataCompletionStateAt5C[
    offsetof(ReplayDataHeader, completionState) == 0x5C ? 1 : -1];

int StampReplayHeaderTimeAndCompletion(
    ReplayManager *manager, unsigned int completionSelector)
{
    time(reinterpret_cast<time_t *>(&manager->replayData->timestamp));
    unsigned int state = completionSelector + 7u;
    if (completionSelector == 0)
        state = static_cast<unsigned int>(g_GameStartupMode);
    manager->replayData->completionState = static_cast<int>(state);
    return 0;
}
