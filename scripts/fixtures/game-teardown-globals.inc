// All storage below is local diagnostic host state, never native bindings.
namespace th10 { Chain *g_Chain; namespace leaf { unsigned char *Fn00418A90(unsigned char *owner); } }
MainSupervisorView g_MainSupervisorView;
AsciiManagerView *g_AsciiManagerView;
GameManager *g_GameManager;
ReplayManager *g_ReplayManager;
GameChainOwnerView *g_GameAnimationSlots, *g_GameOwner477830, *g_GameOwner4776F0,
    *g_GameOwner477818, *g_GameOwner47781C, *g_GameOwner477840,
    *g_GameOwner4776FC, *g_GameOwner4776EC;
GameAdditionalChainView *g_GameOwner4776F4;
GameMenuView *g_GameMenu;
GameGuiUpdateView *g_GameGuiUpdate;
GameStatisticsRowView *g_GameStatisticsRows;
Player *g_Player;
EnemyManagerView *g_EnemyManager;
HintOwnerView *g_GameOwner477814;
unsigned int g_GameRuntimeFlags, g_GameConfigurationFlags491D78;
int g_GameTransitionState, g_GameComparison474C80, g_GameStartupMode,
    g_GameStartupCounter, g_StageScriptGlobal4923A8;
float g_PlayerTimerScale;
unsigned char g_GameStageState[0x60];
void EnterCriticalSection(CRITICAL_SECTION *section);
void LeaveCriticalSection(CRITICAL_SECTION *section);
