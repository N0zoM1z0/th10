void fixture::ClearPublishedOwner(Id id) {
    switch (id) {
    case Replay: g_ReplayManager = NULL; break;
    case Primary:
        g_GameAnimationSlots = NULL;
        if (effect == PrimaryClearsSecondary) g_GameMenu = NULL;
        break;
    case Secondary: g_GameMenu = NULL; break;
    case Owner830: g_GameOwner477830 = NULL; break;
    case Gui: g_GuiView = NULL; break;
    case PlayerId: g_Player = NULL; break;
    case Owner6F0: g_GameOwner4776F0 = NULL; break;
    case Item: g_GameOwner477818 = NULL; break;
    case Owner81C: g_GameOwner47781C = NULL; break;
    case Hint: g_GameOwner477814 = NULL; break;
    case Owner840: g_GameOwner477840 = NULL; break;
    case Enemy: g_EnemyManager = NULL; break;
    case Owner6FC: g_GameOwner4776FC = NULL; break;
    case Owner6EC: g_GameOwner4776EC = NULL; break;
    case Owner6F4:
        g_GameOwner4776F4 = NULL;
        if (effect == FinalCleanupChangesFlags) g_GameRuntimeFlags |= 0x21;
        break;
    default: CHECK(false && "unrecognized cleanup owner");
    }
}
int GameFinalizeStatistics42B1E0(GameStatisticsOwner *statistics) {
    using namespace fixture;
    CHECK(statistics == g_GameStatisticsRows);
    CHECK(g_GameRuntimeFlags == expectedInitialFlags);
    CHECK(g_PlayerTimerScale == 7.0f);
    CHECK(events.empty());
    Record(Stats);
    if (effect == StatsChangesFlags) g_GameRuntimeFlags = 0x4000000bu;
    return -1; // Ignored result, not a native implementation or closure claim.
}
AnmVmIdView AnmLoadedView::CreateVmAtScreenVariant0(int scriptIndex, const AnmFloat3View *position) {
    using namespace fixture;
    CHECK(this == &loaded);
    CHECK(scriptIndex == 6);
    CHECK(position->x == 480.0f && position->y == 392.0f && position->z == 0.0f);
    CHECK(ascii.auxiliaryVm89A4 == 0);
    CHECK(g_PlayerTimerScale == 1.0f);
    Record(CreateVm);
    ++createCount;
    if (effect == VmChangesTransition) g_GameTransitionState = 14;
    if (effect == VmChangesFlags) g_GameRuntimeFlags = 0x40000022u;
    if (effect == VmChangesAscii) g_AsciiManagerView = &alternateAscii;
    return AnmVmIdView(1907);
}
int AsciiManagerView::EnsureSelectionVm(float x, float y) {
    using namespace fixture;
    CHECK(this == &ascii);
    CHECK(x == expectedX && y == expectedY);
    CHECK(g_PlayerTimerScale == 1.0f);
    CHECK((g_GameRuntimeFlags & 3u) == 0);
    Record(Selection);
    ++selectionCount;
    if (effect == SelectionChangesTransition) g_GameTransitionState = (g_GameTransitionState == 15 ? 0 : 14);
    if (effect == SelectionChangesFlags) g_GameRuntimeFlags = 0x40000020u;
    if (effect == SelectionChangesCompare) g_GameComparison474C80 = 123;
    return -1; // Caller ignores the actual boundary's result.
}
unsigned char *th10::leaf::Fn00418A90(unsigned char *owner) {
    using namespace fixture;
    CHECK(owner == g_GameStageState);
    Record(Advance);
    return owner;
}
void __stdcall GameCleanupController422220(GameChainOwnerView *owner) { fixture::Clean(owner); }
// Audited host-only observation of the actual GuiView destructor expression.
// This intentionally does not execute the GUI destructor or construct a fake GuiView.
void fixture::ObserveGuiDestructor(GuiView *owner) { fixture::Clean(owner); }
void __stdcall GameCleanupAnimation405F70(GameChainOwnerView *owner) { fixture::Clean(owner); }
void __stdcall GameCleanupLinked41C1C0(GameChainOwnerView *owner) { fixture::Clean(owner); }
void __stdcall CleanupHintOwner(HintOwnerView *owner) {
    using namespace fixture;
    CHECK(owner == savedHint);
    Clean(owner); // Only boundary observation: this is NOT the real Hint body.
}
void __stdcall GameCleanupOwner42B570(GameChainOwnerView *owner) { fixture::Clean(owner); }
void GamePrepareGuiReuse414570(GuiView *owner) {
    using namespace fixture;
    CHECK(owner == g_GuiView && owner != NULL);
    CHECK((g_GameRuntimeFlags & 2u) != 0);
    CHECK(!criticalHeld);
    Record(ResetGui);
    if (effect == GuiChangesFlags) g_GameRuntimeFlags |= 9u;
}
void __stdcall GameClearEnemyVms(EnemyManagerView *manager) {
    using namespace fixture;
    CHECK(manager == g_EnemyManager && manager != NULL);
    CHECK(!Find(manager).cleaned);
    CHECK(!criticalHeld);
    Record(ClearEnemy);
}
void __stdcall GameCleanupOwner40AF00(GameChainOwnerView *owner) { fixture::Clean(owner); }
void __stdcall GameCleanupLoader405620(GameChainOwnerView *owner) { fixture::Clean(owner); }
void __stdcall GameCleanupOwner408AF0(GameAdditionalChainView *owner) { fixture::Clean(owner); }
void FrontEndQueueSoundCommand(const char *path, int opcode, int argument) {
    using namespace fixture;
    CHECK(std::strcmp(path, "dummy") == 0);
    CHECK(opcode == expectedOpcode && argument == 0);
    CHECK(g_GameManager == NULL);
    CHECK(!criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == initialDepth);
    CHECK(g_GameStartupCounter == 37 && g_StageScriptGlobal4923A8 == 39);
    Record(Sound);
    ++soundCount;
    if (effect == SoundChangesFlags) g_GameRuntimeFlags |= 1u;
}
