#ifndef TH10_SUPERVISOR_LOCK_SLOT_HPP
#define TH10_SUPERVISOR_LOCK_SLOT_HPP

// Directly evidenced indices into MainSupervisorView::criticalSections[7]:
// - slot 0 (CHAIN): Chain.cpp (dispatchers & RemoveAllFromList), Main.cpp
//   (DeleteFpsCounter), TransitionEffect.cpp, ScriptViewer.cpp, ReplayManager.cpp,
//   and all task-unregistration lifecycle TUs (AnmManager, Ending, EnemyGameState,
//   EnemyLaserManager, FrontEnd, GameManager, Gui, Hint, ResultMenu, ScriptViewer,
//   Stage, Startup)
// - slot 3 (LOG): GameErrorContext.cpp (th10::g_CriticalSections[3])
// - slot 5 (MODE): SupervisorModes.cpp (UpdateGameMode / LeaveLock) and
//   Main.cpp (GameWindowView::GetTimestamp timing state)
// Note: gReplayFileCriticalSection (FileSystem.cpp) is a separate BSS symbol,
// NOT MainSupervisorView::criticalSections[2]; slots 1, 2, 4, 6 remain unknown.
enum SupervisorLockSlot
{
    SUPERVISOR_LOCK_CHAIN = 0,
    SUPERVISOR_LOCK_LOG   = 3,
    SUPERVISOR_LOCK_MODE  = 5
};

#endif
