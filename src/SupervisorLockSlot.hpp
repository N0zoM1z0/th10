#ifndef TH10_SUPERVISOR_LOCK_SLOT_HPP
#define TH10_SUPERVISOR_LOCK_SLOT_HPP

// Directly evidenced indices into MainSupervisorView::criticalSections[7]:
// - slot 0: Chain.cpp, ScriptViewer.cpp, ReplayManager.cpp
// - slot 3: GameErrorContext.cpp (th10::g_CriticalSections[3])
// - slot 5: SupervisorModes.cpp (UpdateGameMode / LeaveLock)
// Note: gReplayFileCriticalSection (FileSystem.cpp) is a separate BSS symbol,
// NOT MainSupervisorView::criticalSections[2]; slots 1, 2, 4, 6 remain unknown.
enum SupervisorLockSlot
{
    SUPERVISOR_LOCK_CHAIN = 0,
    SUPERVISOR_LOCK_LOG   = 3,
    SUPERVISOR_LOCK_MODE  = 5
};

#endif
