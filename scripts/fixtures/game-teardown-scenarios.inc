void Reset(const Scenario &s) {
    CHECK(!criticalHeld);
    events.clear(); expected.clear();
    allocationCount = 0;
    std::memset(released, 0, sizeof(released));
    std::memset(cleaned, 0, sizeof(cleaned));
    std::memset(managerElements, 0, sizeof(managerElements));
    std::memset(ownerElements, 0, sizeof(ownerElements));
    std::memset(&g_MainSupervisorView, 0, sizeof(g_MainSupervisorView));
    enterCount = unregisterCount = soundCount = selectionCount = createCount = 0;
    effect = s.effect;
    initialDepth = s.depth;
    expectedInitialFlags = s.flags;
    expectedOpcode = (s.options & 0x10) ? 4 : 3;
    g_GameRuntimeFlags = s.flags;
    g_GameTransitionState = s.transition;
    g_GameStartupMode = 17;
    g_GameComparison474C80 = s.equal ? 17 : 18;
    g_GameStartupCounter = 37;
    g_StageScriptGlobal4923A8 = 39;
    g_GameConfigurationFlags491D78 = s.options;
    g_PlayerTimerScale = 7.0f;
    g_MainSupervisorView.hintMode = static_cast<signed char>(s.hintMode);
    g_MainSupervisorView.options = s.options;
    g_MainSupervisorView.criticalSectionLockCounts[0] = initialDepth;
    for (int i = 0; i < 7; ++i) g_MainSupervisorView.criticalSections[i].tag = i;
    static GameStatisticsOwner statistics;
    g_GameStatisticsRows = &statistics;
    ascii.asciiAnm = &loaded;
    ascii.auxiliaryVm89A4 = s.existingVm ? 712 : 0;
    alternateAscii.asciiAnm = &loaded;
    alternateAscii.auxiliaryVm89A4 = 713;
    g_AsciiManagerView = &ascii;
    savedReplay = g_ReplayManager = s.missing == Replay ? NULL : Allocate<ReplayManager>(Replay);
    savedPrimary = s.missing == Primary ? NULL : Allocate<StageScriptView>(Primary);
    savedSecondary = s.missing == Secondary ? NULL : Allocate<StageScriptView>(Secondary);
    g_GameAnimationSlots = reinterpret_cast<GameChainOwnerView *>(savedPrimary);
    g_GameMenu = reinterpret_cast<GameMenuView *>(savedSecondary);
    saved830 = g_GameOwner477830 = s.missing == Owner830 ? NULL : Allocate<GameChainOwnerView>(Owner830);
    g_GuiView = s.missing == Gui ? NULL : AllocateGui();
    g_Player = s.missing == PlayerId ? NULL : Allocate<Player>(PlayerId);
    saved6F0 = g_GameOwner4776F0 = s.missing == Owner6F0 ? NULL : Allocate<GameChainOwnerView>(Owner6F0);
    savedItem = s.missing == Item ? NULL : Allocate<ItemManagerView>(Item);
    g_GameOwner477818 = reinterpret_cast<GameChainOwnerView *>(savedItem);
    g_EnemyLaserManager = s.missing == Owner81C ? NULL : reinterpret_cast<EnemyLaserManagerView *>(Allocate<GameChainOwnerView>(Owner81C));
    savedHint = g_GameOwner477814 = s.missing == Hint ? NULL : Allocate<HintOwnerView>(Hint);
    g_GameOwner477840 = s.missing == Owner840 ? NULL : Allocate<GameChainOwnerView>(Owner840);
    g_EnemyManager = s.missing == Enemy ? NULL : Allocate<EnemyManagerView>(Enemy);
    g_GameOwner4776FC = s.missing == Owner6FC ? NULL : Allocate<GameChainOwnerView>(Owner6FC);
    g_GameOwner4776EC = s.missing == Owner6EC ? NULL : Allocate<GameChainOwnerView>(Owner6EC);
    g_GameOwner4776F4 = s.missing == Owner6F4 ? NULL : Allocate<GameAdditionalChainView>(Owner6F4);
    activeManager = g_GameManager = Allocate<GameManager>(Game);
    activeManager->updateChain = (s.chainMask & 1) ? &managerElements[0] : NULL;
    activeManager->drawChain = (s.chainMask & 2) ? &managerElements[1] : NULL;
    elementBeforeEnter = activeManager->updateChain ? activeManager->updateChain : activeManager->drawChain;
    th10::g_Chain = &chains[0];
    switchChains = s.mutateChains;
    for (int i = 0; i < 4; ++i) ownerElements[i].flags = 0xabcdefefu;
    for (int i = 0; i < 3; ++i) {
        replayElements[i].isHeapAllocated = 1;
        replayElements[i].unknownFlag1 = 1;
        replayElements[i].unknownFlags = 0x1234567;
    }
    if (saved830) {
        saved830->updateChain = (s.callbackMask & 1) ? &ownerElements[0] : NULL;
        saved830->drawChain = (s.callbackMask & 2) ? &ownerElements[1] : NULL;
    }
    if (saved6F0) {
        saved6F0->updateChain = (s.callbackMask & 4) ? &ownerElements[2] : NULL;
        saved6F0->drawChain = (s.callbackMask & 8) ? &ownerElements[3] : NULL;
    }
    if (savedReplay) {
        savedReplay->updateChain = &replayElements[0];
        savedReplay->playbackChain = (s.callbackMask & 16) ? &replayElements[1] : NULL;
        savedReplay->drawChain = (s.callbackMask & 32) ? &replayElements[2] : NULL;
    }
    if (savedItem) std::memset(static_cast<void *>(savedItem), 0xa5, sizeof(*savedItem));
    expectedX = s.transition == 11 ? 224.0f : 480.0f;
    expectedY = s.transition == 11 ? 416.0f : 392.0f;
}

void Run(const Scenario &s) {
    Reset(s);
    unsigned int flags = s.effect == StatsChangesFlags ? 0x40000008u : (s.flags & ~3u);
    int finalTransition = s.transition;
    expected.push_back(Event{Stats, NoId});
    if (s.transition == 10) {
        if (!s.existingVm) expected.push_back(Event{CreateVm, NoId});
        if (s.effect == VmChangesFlags) flags = 0x40000022u;
        if (s.equal) flags |= 1u;
        if (s.effect == VmChangesTransition) finalTransition = 14;
    }
    if (s.transition == 11 || s.transition == 4 || s.transition == 15 || s.transition == 13)
        expected.push_back(Event{Selection, NoId});
    if (s.effect == SelectionChangesFlags) flags = 0x40000020u;
    if (s.transition == 11) flags |= 2u;
    if (s.effect == SelectionChangesTransition) finalTransition = (s.transition == 15 ? 0 : 14);
    if (s.transition == 13) {
        if (!s.equal || s.effect == SelectionChangesCompare) {
            expected.push_back(Event{Advance, NoId}); flags |= 8u;
        }
        flags |= 1u;
    }
    bool reuse = (flags & 2u) != 0;
    if (!reuse) {
        for (Id id : {Replay, Primary, Secondary, Owner830, Gui, PlayerId, Owner6F0, Item, Owner81C, Hint, Owner840}) {
            if (id == s.missing) continue;
            if (id == Replay && (finalTransition == 14 || finalTransition == 15)) continue;
            if (id == Secondary && s.effect == PrimaryClearsSecondary) continue;
            AddPair(id);
        }
    } else {
        expected.push_back(Event{ResetGui, NoId});
        if (s.missing != Secondary) AddPair(Secondary);
        if (s.effect == GuiChangesFlags) flags |= 9u;
    }
    if ((flags & 9u) == 0) {
        if (s.missing != Enemy) AddPair(Enemy);
    } else expected.push_back(Event{ClearEnemy, NoId});
    for (Id id : {Owner6FC, Owner6EC, Owner6F4}) if (id != s.missing) AddPair(id);
    if (s.effect == FinalCleanupChangesFlags) flags |= 0x21u;
    int chainsExpected = ((s.chainMask & 1) != 0) + ((s.chainMask & 2) != 0);
    for (int i = 0; i < chainsExpected; ++i) {
        expected.push_back(Event{Enter, NoId});
        expected.push_back(Event{Unregister, NoId});
        expected.push_back(Event{Leave, NoId});
    }
    bool soundExpected = (flags & 0x22u) == 0;
    if (soundExpected) expected.push_back(Event{Sound, NoId});
    if (s.effect == SoundChangesFlags && soundExpected) flags |= 1u;
    expectedFinalFlags = flags;

    GameManagerCleanup(activeManager);

    CHECK(released[Hint] == ((!reuse && s.missing != Hint) ? 1 : 0));
    CHECK(events == expected);
    CHECK(g_GameRuntimeFlags == expectedFinalFlags);
    CHECK(g_PlayerTimerScale == 1.0f);
    CHECK(g_GameManager == NULL);
    CHECK(g_GameStartupCounter == 1);
    CHECK(static_cast<unsigned int>(g_StageScriptGlobal4923A8) == ((flags & 1u) ? 0u : 0xff000000u));
    CHECK(!criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == initialDepth);
    CHECK(unregisterCount == chainsExpected && enterCount == chainsExpected);
    CHECK(soundCount == (soundExpected ? 1 : 0));
    CHECK(Find(activeManager).live && released[Game] == 0);
    CHECK(activeManager->updateChain == ((s.chainMask & 1) ? &managerElements[switchChains ? 2 : 0] : NULL));
    CHECK(activeManager->drawChain == ((s.chainMask & 2) ? &managerElements[switchChains ? 3 : 1] : NULL));
    if (s.transition == 10) {
        CHECK(createCount == (s.existingVm ? 0 : 1));
        CHECK(ascii.auxiliaryVm89A4 == (s.existingVm ? 712 : 1907));
        CHECK(alternateAscii.auxiliaryVm89A4 == 713);
    }
    if (reuse) {
        CHECK(g_GameMenu == reinterpret_cast<GameMenuView *>(savedPrimary));
        CHECK(g_GameAnimationSlots == reinterpret_cast<GameChainOwnerView *>(savedPrimary));
        CHECK(g_GameOwner477814 == savedHint && Find(savedHint).live && cleaned[Hint] == 0);
        CHECK(g_Player != NULL && Find(g_Player).live);
        CHECK(g_GameOwner477818 == reinterpret_cast<GameChainOwnerView *>(savedItem));
        for (int i = 0; i < 4; ++i) {
            unsigned int expectedFlags = (s.callbackMask & (1u << i)) ? (0xabcdefefu & ~2u) : 0xabcdefefu;
            CHECK(ownerElements[i].flags == expectedFlags);
        }
        CHECK(replayElements[0].unknownFlag1 == 1);
        CHECK(replayElements[1].unknownFlag1 == ((s.callbackMask & 16u) ? 0u : 1u));
        CHECK(replayElements[2].unknownFlag1 == ((s.callbackMask & 32u) ? 0u : 1u));
        for (int i = 0; i < 3; ++i) {
            CHECK(replayElements[i].isHeapAllocated == 1 && replayElements[i].unknownFlags == 0x1234567);
        }
        for (unsigned char byte : savedItem->prefix) CHECK(byte == 0xa5);
        const unsigned char *bytes = reinterpret_cast<const unsigned char *>(savedItem->items);
        for (std::size_t i = 0; i < 0x21cea0; ++i) CHECK(bytes[i] == 0);
        for (unsigned char byte : savedItem->suffix) CHECK(byte == 0xa5);
    }
    Finish();
    ++cases;
}
