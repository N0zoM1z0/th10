#include "Main.hpp"
#include "ReplayManager.hpp"
#include "GameScoreState.hpp"
#include "Rng.hpp"
#include <string.h>

extern unsigned int g_ReplayStageValue1B8;

// Target 42A6A0 constructs a zeroed 0x1C4-byte stage header before publishing
// its pointer; the allocation-null branch precedes the unchecked header use.
ReplayStageDataHeader::ReplayStageDataHeader()
{
    memset(this, 0, sizeof(*this));
}

// Complete stage restoration used by GameManagerInitialize at 417AFB.
// The meaningful manager parameter naturally derives the private EBX / RET.
void GameRestoreReplayState(ReplayManager *replay)
{
    if (replay->mode == REPLAY_MANAGER_RECORD)
    {
        replay->stageHeaders[g_GameScoreState.selectedStage] =
            new ReplayStageDataHeader;
        ReplayStageDataHeader *header =
            replay->stageHeaders[g_GameScoreState.selectedStage];
        header->unknown002 = g_RngView.seed;
        g_RngView.generationCount = 0;
        header->stageIndex = static_cast<short>(g_GameScoreState.selectedStage);
        header->unknownFlag1C0 = g_MainSupervisorView.newGameSession;
    }
    else if (replay->mode == REPLAY_MANAGER_PLAYBACK)
    {
        ReplayStageState &state = replay->stageStates[g_GameScoreState.selectedStage];
        state.recordCursor = state.recordStart;
        state.fpsCursor = state.fpsStart;
        state.recordIndex = 0;
        ReplayStageDataHeader *header = state.header;
        g_RngView.seed = header->unknown002;
        g_RngView.generationCount = 0;
        g_GameScoreState.score = header->unknown00C;
        g_GameScoreState.power = header->unknown010;
        g_GameScoreState.SetFaith(header->unknown014 * 10);
        g_GameScoreState.SetTimerCurrent(header->unknown018);
        *reinterpret_cast<int *>(g_GameScoreState.unknown28 + 8) = header->unknown01C;
        g_GameScoreState.rank = header->unknown020;
        *reinterpret_cast<int *>(g_GameScoreState.unknown44 + 12) = header->unknown1B4;
        g_ReplayStageValue1B8 =
            replay->stageStates[g_GameScoreState.selectedStage].header->unknown1B8;
    }
}
