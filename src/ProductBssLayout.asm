; Zero-initialized relocatable layout for g_FrontEndInput and .bss globals.
; Contains NO strings, NO initialized tables, and NO hardcoded addresses;
; exists solely so VC7.1 preserves exact sub-4-byte struct-interior offsets
; (+0x06/+0x32 in g_FrontEndInput and +0x136/+0x6F6/+0x6F7 in g_MainSupervisorView).

.686P
.MODEL FLAT
OPTION CASEMAP:NONE

PUBLIC ?g_FrontEndInput@@3UFrontEndInputView@@A
PUBLIC ?g_GameInputBits@@3IA
PUBLIC ?g_MainInputEdges@@3GA
PUBLIC ?g_ResultMenuHeldInputWord@@3IA
PUBLIC ?g_GuiInputCurrent@@3IA
PUBLIC ?g_PlayerInputBits@@3IA
PUBLIC ?g_GuiInputEdges@@3IA
PUBLIC ?g_AsciiManagerView@@3PAUAsciiManagerView@@A
PUBLIC ?g_GameEffectUpdate@@3PAUGameEffectUpdateView@@A
PUBLIC ?g_GameMenu@@3PAUGameMenuView@@A
PUBLIC ?g_StageSecondary@@3PAUStageScriptView@@A
PUBLIC ?g_GameAnimationSlots@@3PAUGameChainOwnerView@@A
PUBLIC ?g_StagePrimary@@3PAUStageScriptView@@A
PUBLIC ?g_EnemyAnimationMode@@3PAUEnemyAnimationModeView@@A
PUBLIC ?g_GameOwner4776EC@@3PAUGameChainOwnerView@@A
PUBLIC ?g_ItemAutoCollectGate@@3PAUItemAutoCollectGateView@@A
PUBLIC ?g_PlayerUpdateGate28@@3PAUPlayerUpdateGate28@@A
PUBLIC ?g_ScriptViewerBomb@@3PAXA
PUBLIC ?g_EnemyBulletManager@@3PAXA
PUBLIC ?g_EnemyPrimaryResourceOwner@@3PAUEnemyPrimaryResourceOwnerView@@A
PUBLIC ?g_EnemyPrimaryResourceOwner@@3PAUItemPrimaryResourceOwnerView@@A
PUBLIC ?g_GameOwner4776F0@@3PAUGameChainOwnerView@@A
PUBLIC ?g_ScriptViewerBullets@@3PAUBulletManagerView@@A
PUBLIC ?g_EnemyGameState@@3PAUEnemyGameStateView@@A
PUBLIC ?g_GameOwner4776F4@@3PAUGameAdditionalChainView@@A
PUBLIC ?g_GuiGameOwner@@3PAUGuiGameOwnerView@@A
PUBLIC ?g_ScriptViewer@@3PAUScriptViewerView@@A
PUBLIC ?g_GameOwner4776FC@@3PAUGameChainOwnerView@@A
PUBLIC ?g_EndingView@@3PAUEndingView@@A
PUBLIC ?g_EnemyManager@@3PAUEnemyManagerView@@A
PUBLIC ?g_PlayerDrawGate10@@3PAUPlayerDrawGate10@@A
PUBLIC ?g_ReplayFpsSource@@3PAUReplayFpsSource@@A
PUBLIC ?g_ReplayTimingSource@@3PAUReplayTimingSource@@A
PUBLIC ?g_GuiView@@3PAUGuiView@@A
PUBLIC ?g_ItemPowerDisplayOwner@@3PAUItemPowerDisplayOwnerView@@A
PUBLIC ?g_PlayerDrawGate9EB8@@3PAUPlayerDrawGate9EB8@@A
PUBLIC ?g_FrontEndDemoPath@@3PADA
PUBLIC ?g_GameDefaultReplayPath@@3QBDB
PUBLIC ?g_BulletUpdateGate@@3PAUBulletUpdateGateView@@A
PUBLIC ?g_FpsSampleGateView@@3PAUFpsSampleGateView@@A
PUBLIC ?g_GameManager@@3PAUGameManager@@A
PUBLIC ?g_GuiResetSession@@3PAUGuiResetSessionView@@A
PUBLIC ?g_PlayerDrawGate54@@3PAUPlayerDrawGate54@@A
PUBLIC ?g_ReplayRuntime@@3PAUReplayRuntimeState@@A
PUBLIC ?g_ResultGameManager@@3PAUResultGameManagerView@@A
PUBLIC ?g_GameOwner477814@@3PAUHintOwnerView@@A
PUBLIC ?g_GameOwner477818@@3PAUGameChainOwnerView@@A
PUBLIC ?g_ItemManager@@3PAUItemManagerView@@A
PUBLIC ?g_EnemyLaserManager@@3PAUEnemyLaserManagerView@@A
PUBLIC ?g_StartupWorker@@3PAUStartupWorkerView@@A
PUBLIC ?g_GameUiTransitionVmIds@@3PAUAnmVmIdView@@A
PUBLIC ?g_GameOwner477830@@3PAUGameChainOwnerView@@A
PUBLIC ?g_PlayerDrawGate4@@3PAUPlayerDrawGate4@@A
PUBLIC ?g_Player@@3PAUPlayer@@A
PUBLIC ?g_PlayerUpdateGate10@@3PAUPlayerUpdateGate10@@A
PUBLIC ?g_ReplayManager@@3PAVReplayManager@@A
PUBLIC ?g_EnemySpellStatistics@@3PAEA
PUBLIC ?g_FrontEndProfileData@@3PAEA
PUBLIC ?g_GameStatisticsRows@@3PAUGameStatisticsOwner@@A
PUBLIC ?g_GuiPersistentData@@3PAEA
PUBLIC ?g_GameOwner477840@@3PAUGameChainOwnerView@@A
PUBLIC ?g_ScriptViewerPopups@@3PAXA
PUBLIC ?g_SptTestView@@3PAUSptTestView@@A
PUBLIC ?g_FrontEndScoreFormatTable@@3PAXA
PUBLIC ?g_FrontEndSelectedStageRecord@@3PAEA
PUBLIC ?g_GameStageUpdate@@3PAUGameStageUpdateView@@A
PUBLIC ?g_ScriptViewerStageRoute@@3PAXA
PUBLIC ?g_FrontEndControllerView@@3PAUFrontEndControllerView@@A
PUBLIC ?g_PbgArchiveCount@@3HA
PUBLIC ?g_TextFont31@@3PAUHFONT__@@A
PUBLIC ?g_TextFont30@@3PAUHFONT__@@A
PUBLIC ?g_TextFont29@@3PAUHFONT__@@A
PUBLIC ?g_TextFont28@@3PAUHFONT__@@A
PUBLIC ?g_TextFont27@@3PAUHFONT__@@A
PUBLIC ?g_TextFont26@@3PAUHFONT__@@A
PUBLIC ?g_TextFont25@@3PAUHFONT__@@A
PUBLIC ?g_TextFont24@@3PAUHFONT__@@A
PUBLIC ?g_TextFont23@@3PAUHFONT__@@A
PUBLIC ?g_TextFont22@@3PAUHFONT__@@A
PUBLIC ?g_TextFont21@@3PAUHFONT__@@A
PUBLIC ?g_TextFont20@@3PAUHFONT__@@A
PUBLIC ?g_TextFont19@@3PAUHFONT__@@A
PUBLIC ?g_TextFont18@@3PAUHFONT__@@A
PUBLIC ?g_TextFont17@@3PAUHFONT__@@A
PUBLIC ?g_GameStartupCounter@@3HA
PUBLIC ?g_AlternateRngView@@3URngView@@A
PUBLIC ?g_RngView@@3URngView@@A
PUBLIC ?g_InputJoystickCaps@@3PAUInputJoystickCapsView@@A
PUBLIC ?g_MainAllocationRegistry@@3PAUMainAllocationRegistryView@@A
PUBLIC ?g_Chain@th10@@3PAUChain@1@A
PUBLIC ?g_GuiRetainedMessageFile491BE8@@3PAXA
PUBLIC ?g_GameUiTransitionResetIndicator@@3HA
PUBLIC ?g_PlayerSharedOptionData@@3PAUPlayerOptionDataView@@A
PUBLIC ?g_PlayerShotUnknown2CCallbacks@@3PAPAXA
PUBLIC ?g_PlayerShotCollisionCallbacks@@3PAP6IHPAUPlayer@@PAUPlayerShotRuntimeView@@PBUPlayerFloat3@@@ZA
PUBLIC ?g_MainModeTransitionCounter@@3HA
PUBLIC ?g_FrontEndReplayCursor@@3HA
PUBLIC ?g_FrontEndStageCursor@@3HA
PUBLIC ?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A
PUBLIC ?g_EnemyAnimationOwner@@3PAUEnemyAnimationOwnerView@@A
PUBLIC _g_AnmManagerPtr
PUBLIC ?g_AnmZeroVector491C14@@3UAnmFloat3View@@A
PUBLIC ?g_EnemyInterpolationOrigin@@3UPlayerFloat3@@A
PUBLIC ?g_EnemyInterpolationBasis@@3UEnemyFloat2@@A
PUBLIC ?g_MainSupervisorView@@3UMainSupervisorView@@A
PUBLIC ?g_Direct3DDevice@@3PAUD3d9DeviceView@@A
PUBLIC ?g_InputDirectInput8@@3PAUInputDirectInput8View@@A
PUBLIC ?g_InputKeyboardDevice@@3PAUInputDeviceView@@A
PUBLIC ?g_InputPadDevices@@3PAPAUInputDeviceView@@A
PUBLIC ?g_TransitionViewport@@3UAnmViewportView@@A
PUBLIC ?g_AnmBackbufferFormat@@3IA
PUBLIC ?g_InputJoystickXThreshold@@3FA
PUBLIC ?g_InputJoystickYThreshold@@3FA
PUBLIC ?g_AnmHardwareFlags@@3EA
PUBLIC ?g_FrontEndSupervisorFlags@@3IA
PUBLIC ?g_GameConfigurationFlags491D78@@3IA
PUBLIC ?g_ReplayInputBehaviorFlags@@3IA
PUBLIC ?g_AnmBackgroundCameraPosition@@3UAnmFloat3View@@A
PUBLIC ?g_EnemyGlobalPositionOffset@@3UPlayerFloat3@@A
PUBLIC ?g_StagePublishedCamera@@3UStageCameraStateView@@A
PUBLIC ?g_AnmPosition491DA0@@3UAnmFloat3View@@A
PUBLIC ?g_EnemyViewMatrix@@3EA
PUBLIC ?g_EnemyProjectionMatrix@@3EA
PUBLIC ?g_EnemyViewport@@3EA
PUBLIC ?g_TransitionShakeX@@3MA
PUBLIC ?g_TransitionShakeY@@3MA
PUBLIC ?g_AnmPositionOffsetDelta@@3UAnmFloat3View@@A
PUBLIC ?g_EnemyPositionOffsetDelta@@3UPlayerFloat3@@A
PUBLIC ?g_AnmPhotoBlend@@3UAnmPhotoBlendView@@A
PUBLIC ?g_StageSecondaryCamera@@3UStageCameraStateView@@A
PUBLIC ?g_AnmViewportOwner@@3PAUAnmViewportOwnerView@@A
PUBLIC ?g_FpsDisplayMode@@3HA
PUBLIC ?g_GameTransitionState@@3HA
PUBLIC ?g_GuiGlobalMode@@3HA
PUBLIC ?g_GameNewSession@@3HA
PUBLIC ?g_ReplayStageFlags@@3IA
PUBLIC ?g_FrontEndMusicCommentAnm@@3PAUAnmLoadedView@@A
PUBLIC ?g_AnmPreloadStopRequested@@3CA
PUBLIC ?g_GameConfigurationFlags@@3IA
PUBLIC ?g_GuiSupervisorFlags@@3IA
PUBLIC ?g_InputSupervisorFlags@@3IA
PUBLIC ?g_GameStartupTimeTick@@3IA
PUBLIC ?g_MainScreenshotThreadHandle@@3KA
PUBLIC ?g_GameWorkerState@@3PAEA
PUBLIC ?g_CriticalSections@th10@@3PAU_RTL_CRITICAL_SECTION@@A
PUBLIC ?g_EnemyCallbackCriticalSection@@3UEnemyCriticalSectionView@@A
PUBLIC ?g_ItemCallbackCriticalSection@@3UItemCriticalSectionView@@A
PUBLIC ?g_PlayerCallbackCriticalSection@@3UPlayerCriticalSectionView@@A
PUBLIC ?gReplayFileCriticalSection@@3U_RTL_CRITICAL_SECTION@@A
PUBLIC ?g_ChainNestCounter@th10@@3EA
PUBLIC ?g_EnemyCallbackLockDepth@@3EA
PUBLIC ?g_ItemCallbackLockDepth@@3EA
PUBLIC ?g_PlayerCallbackLockDepth@@3EA
PUBLIC ?gReplayFileOpenCount@@3EA
PUBLIC ?g_LogLockDepth@th10@@3EA
PUBLIC ?g_MainExecutableChecksum@@3IA
PUBLIC ?g_MainExecutableSize@@3IA
PUBLIC ?g_GameStartupVersionSize@@3HA
PUBLIC ?g_GameStartupVersionData@@3PAEA
PUBLIC ?g_GameStartupTextColor@@3IA
PUBLIC ?g_StageScriptGlobal4923A8@@3HA
PUBLIC ?g_PbgArchives@@3PAVPbgArchive@@A
PUBLIC ?g_GameWindowView@@3UGameWindowView@@A
PUBLIC ?g_InputEnabled@@3HA
PUBLIC ?g_MainSoundOwner@@3PAEA
PUBLIC ?g_GameLoaderPending@@3HA
PUBLIC ?g_SoundBgmFileBaseOffset@@3HA
PUBLIC ?g_SoundDataLoadThreadHandle@@3PAXA
PUBLIC ?g_FrontEndSoundBgmVolume@@3HA
PUBLIC ?g_FrontEndSoundSfxVolume@@3HA
PUBLIC ?g_FrontEndSoundBgmAttenuation@@3HA
PUBLIC ?g_AnmQuadVerticesNoDiffuse@@3PAUAnmRenderVertexNoDiffuseView@@A
PUBLIC ?g_AnmQuadVertices@@3PAUAnmRenderVertexView@@A
PUBLIC ?g_AnmBackgroundQuadVertices@@3PAUAnmRenderVertexNoDiffuseView@@A
PUBLIC ?g_MainPbgArchive@@3VPbgArchive@@A
PUBLIC ?g_FrontEndPracticeSecretIdleFrames@@3HA
PUBLIC ?g_FrontEndPracticeSecretProgress@@3HA
PUBLIC ?g_FrontEndKeyboardTransitions@@3PAEA
PUBLIC ?g_FrontEndStageKeyboardState@@3PAEA
PUBLIC ?g_MainExclusiveMutex@@3PAXA
PUBLIC ?g_InputJoystickButtons@@3PACA
PUBLIC ?g_EnemyEclFilenameBuffer@@3PADA
PUBLIC ?g_StageFilePath@@3PADA
PUBLIC ?g_EclWorldAnchor@@3UAnmFloat3View@@A
PUBLIC ?g_EclWorldAnchorFlags@@3IA
PUBLIC ?g_FrontEndPreviousKeyboardState@@3PAEA
PUBLIC ?g_FrontEndKeyboardState@@3PAEA

_BSS SEGMENT PARA PUBLIC 'BSS'
    ALIGN 16
?g_FrontEndInput@@3UFrontEndInputView@@A LABEL BYTE
?g_GameInputBits@@3IA LABEL BYTE
    DB 6 DUP(?)
?g_MainInputEdges@@3GA LABEL BYTE
?g_ResultMenuHeldInputWord@@3IA LABEL BYTE
    DB 38 DUP(?)
?g_GuiInputCurrent@@3IA LABEL BYTE
?g_PlayerInputBits@@3IA LABEL BYTE
    DB 6 DUP(?)
?g_GuiInputEdges@@3IA LABEL BYTE
    DB 38 DUP(?)
    ALIGN 16
?g_AsciiManagerView@@3PAUAsciiManagerView@@A LABEL BYTE
?g_GameEffectUpdate@@3PAUGameEffectUpdateView@@A LABEL BYTE
    DB 4 DUP(?)
?g_GameMenu@@3PAUGameMenuView@@A LABEL BYTE
?g_StageSecondary@@3PAUStageScriptView@@A LABEL BYTE
    DB 4 DUP(?)
?g_GameAnimationSlots@@3PAUGameChainOwnerView@@A LABEL BYTE
?g_StagePrimary@@3PAUStageScriptView@@A LABEL BYTE
    DB 4 DUP(?)
?g_EnemyAnimationMode@@3PAUEnemyAnimationModeView@@A LABEL BYTE
?g_GameOwner4776EC@@3PAUGameChainOwnerView@@A LABEL BYTE
?g_ItemAutoCollectGate@@3PAUItemAutoCollectGateView@@A LABEL BYTE
?g_PlayerUpdateGate28@@3PAUPlayerUpdateGate28@@A LABEL BYTE
?g_ScriptViewerBomb@@3PAXA LABEL BYTE
    DB 4 DUP(?)
?g_EnemyBulletManager@@3PAXA LABEL BYTE
?g_EnemyPrimaryResourceOwner@@3PAUEnemyPrimaryResourceOwnerView@@A LABEL BYTE
?g_EnemyPrimaryResourceOwner@@3PAUItemPrimaryResourceOwnerView@@A LABEL BYTE
?g_GameOwner4776F0@@3PAUGameChainOwnerView@@A LABEL BYTE
?g_ScriptViewerBullets@@3PAUBulletManagerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_EnemyGameState@@3PAUEnemyGameStateView@@A LABEL BYTE
?g_GameOwner4776F4@@3PAUGameAdditionalChainView@@A LABEL BYTE
?g_GuiGameOwner@@3PAUGuiGameOwnerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_ScriptViewer@@3PAUScriptViewerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_GameOwner4776FC@@3PAUGameChainOwnerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_EndingView@@3PAUEndingView@@A LABEL BYTE
    DB 4 DUP(?)
?g_EnemyManager@@3PAUEnemyManagerView@@A LABEL BYTE
?g_PlayerDrawGate10@@3PAUPlayerDrawGate10@@A LABEL BYTE
    DB 4 DUP(?)
?g_ReplayFpsSource@@3PAUReplayFpsSource@@A LABEL BYTE
?g_ReplayTimingSource@@3PAUReplayTimingSource@@A LABEL BYTE
    DB 4 DUP(?)
?g_GuiView@@3PAUGuiView@@A LABEL BYTE
?g_ItemPowerDisplayOwner@@3PAUItemPowerDisplayOwnerView@@A LABEL BYTE
?g_PlayerDrawGate9EB8@@3PAUPlayerDrawGate9EB8@@A LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndDemoPath@@3PADA LABEL BYTE
?g_GameDefaultReplayPath@@3QBDB LABEL BYTE
    DB 256 DUP(?)
?g_BulletUpdateGate@@3PAUBulletUpdateGateView@@A LABEL BYTE
?g_FpsSampleGateView@@3PAUFpsSampleGateView@@A LABEL BYTE
?g_GameManager@@3PAUGameManager@@A LABEL BYTE
?g_GuiResetSession@@3PAUGuiResetSessionView@@A LABEL BYTE
?g_PlayerDrawGate54@@3PAUPlayerDrawGate54@@A LABEL BYTE
?g_ReplayRuntime@@3PAUReplayRuntimeState@@A LABEL BYTE
?g_ResultGameManager@@3PAUResultGameManagerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_GameOwner477814@@3PAUHintOwnerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_GameOwner477818@@3PAUGameChainOwnerView@@A LABEL BYTE
?g_ItemManager@@3PAUItemManagerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_EnemyLaserManager@@3PAUEnemyLaserManagerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_StartupWorker@@3PAUStartupWorkerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_GameUiTransitionVmIds@@3PAUAnmVmIdView@@A LABEL BYTE
    DB 12 DUP(?)
?g_GameOwner477830@@3PAUGameChainOwnerView@@A LABEL BYTE
?g_PlayerDrawGate4@@3PAUPlayerDrawGate4@@A LABEL BYTE
    DB 4 DUP(?)
?g_Player@@3PAUPlayer@@A LABEL BYTE
    DB 4 DUP(?)
?g_PlayerUpdateGate10@@3PAUPlayerUpdateGate10@@A LABEL BYTE
?g_ReplayManager@@3PAVReplayManager@@A LABEL BYTE
    DB 4 DUP(?)
?g_EnemySpellStatistics@@3PAEA LABEL BYTE
?g_FrontEndProfileData@@3PAEA LABEL BYTE
?g_GameStatisticsRows@@3PAUGameStatisticsOwner@@A LABEL BYTE
?g_GuiPersistentData@@3PAEA LABEL BYTE
    DB 4 DUP(?)
?g_GameOwner477840@@3PAUGameChainOwnerView@@A LABEL BYTE
?g_ScriptViewerPopups@@3PAXA LABEL BYTE
    DB 4 DUP(?)
?g_SptTestView@@3PAUSptTestView@@A LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndScoreFormatTable@@3PAXA LABEL BYTE
?g_FrontEndSelectedStageRecord@@3PAEA LABEL BYTE
?g_GameStageUpdate@@3PAUGameStageUpdateView@@A LABEL BYTE
?g_ScriptViewerStageRoute@@3PAXA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndControllerView@@3PAUFrontEndControllerView@@A LABEL BYTE
    DB 4 DUP(?)
?g_PbgArchiveCount@@3HA LABEL BYTE
    DB 106520 DUP(?)
?g_TextFont31@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont30@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont29@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont28@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont27@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont26@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont25@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont24@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont23@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont22@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont21@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont20@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont19@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont18@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_TextFont17@@3PAUHFONT__@@A LABEL BYTE
    DB 4 DUP(?)
?g_GameStartupCounter@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_AlternateRngView@@3URngView@@A LABEL BYTE
    DB 8 DUP(?)
?g_RngView@@3URngView@@A LABEL BYTE
    DB 8 DUP(?)
?g_InputJoystickCaps@@3PAUInputJoystickCapsView@@A LABEL BYTE
    DB 808 DUP(?)
?g_MainAllocationRegistry@@3PAUMainAllocationRegistryView@@A LABEL BYTE
    DB 4 DUP(?)
?g_Chain@th10@@3PAUChain@1@A LABEL BYTE
    DB 4 DUP(?)
?g_GuiRetainedMessageFile491BE8@@3PAXA LABEL BYTE
    DB 4 DUP(?)
?g_GameUiTransitionResetIndicator@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_PlayerSharedOptionData@@3PAUPlayerOptionDataView@@A LABEL BYTE
    DB 4 DUP(?)
?g_PlayerShotUnknown2CCallbacks@@3PAPAXA LABEL BYTE
    DB 4 DUP(?)
?g_PlayerShotCollisionCallbacks@@3PAP6IHPAUPlayer@@PAUPlayerShotRuntimeView@@PBUPlayerFloat3@@@ZA LABEL BYTE
    DB 8 DUP(?)
?g_MainModeTransitionCounter@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndReplayCursor@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndStageCursor@@3HA LABEL BYTE
    DB 8 DUP(?)
?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A LABEL BYTE
?g_EnemyAnimationOwner@@3PAUEnemyAnimationOwnerView@@A LABEL BYTE
_g_AnmManagerPtr LABEL BYTE
    DB 4 DUP(?)
?g_AnmZeroVector491C14@@3UAnmFloat3View@@A LABEL BYTE
?g_EnemyInterpolationOrigin@@3UPlayerFloat3@@A LABEL BYTE
    DB 12 DUP(?)
?g_EnemyInterpolationBasis@@3UEnemyFloat2@@A LABEL BYTE
    DB 8 DUP(?)
?g_MainSupervisorView@@3UMainSupervisorView@@A LABEL BYTE
    DB 8 DUP(?)
?g_Direct3DDevice@@3PAUD3d9DeviceView@@A LABEL BYTE
    DB 4 DUP(?)
?g_InputDirectInput8@@3PAUInputDirectInput8View@@A LABEL BYTE
    DB 4 DUP(?)
?g_InputKeyboardDevice@@3PAUInputDeviceView@@A LABEL BYTE
    DB 4 DUP(?)
?g_InputPadDevices@@3PAPAUInputDeviceView@@A LABEL BYTE
    DB 184 DUP(?)
?g_TransitionViewport@@3UAnmViewportView@@A LABEL BYTE
    DB 32 DUP(?)
?g_AnmBackbufferFormat@@3IA LABEL BYTE
    DB 74 DUP(?)
?g_InputJoystickXThreshold@@3FA LABEL BYTE
    DB 2 DUP(?)
?g_InputJoystickYThreshold@@3FA LABEL BYTE
    DB 24 DUP(?)
?g_AnmHardwareFlags@@3EA LABEL BYTE
?g_FrontEndSupervisorFlags@@3IA LABEL BYTE
?g_GameConfigurationFlags491D78@@3IA LABEL BYTE
?g_ReplayInputBehaviorFlags@@3IA LABEL BYTE
    DB 4 DUP(?)
?g_AnmBackgroundCameraPosition@@3UAnmFloat3View@@A LABEL BYTE
?g_EnemyGlobalPositionOffset@@3UPlayerFloat3@@A LABEL BYTE
?g_StagePublishedCamera@@3UStageCameraStateView@@A LABEL BYTE
    DB 36 DUP(?)
?g_AnmPosition491DA0@@3UAnmFloat3View@@A LABEL BYTE
    DB 40 DUP(?)
?g_EnemyViewMatrix@@3EA LABEL BYTE
    DB 64 DUP(?)
?g_EnemyProjectionMatrix@@3EA LABEL BYTE
    DB 64 DUP(?)
?g_EnemyViewport@@3EA LABEL BYTE
    DB 28 DUP(?)
?g_TransitionShakeX@@3MA LABEL BYTE
    DB 4 DUP(?)
?g_TransitionShakeY@@3MA LABEL BYTE
    DB 4 DUP(?)
?g_AnmPositionOffsetDelta@@3UAnmFloat3View@@A LABEL BYTE
?g_EnemyPositionOffsetDelta@@3UPlayerFloat3@@A LABEL BYTE
    DB 12 DUP(?)
?g_AnmPhotoBlend@@3UAnmPhotoBlendView@@A LABEL BYTE
    DB 28 DUP(?)
?g_StageSecondaryCamera@@3UStageCameraStateView@@A LABEL BYTE
    DB 280 DUP(?)
?g_AnmViewportOwner@@3PAUAnmViewportOwnerView@@A LABEL BYTE
    DB 12 DUP(?)
?g_FpsDisplayMode@@3HA LABEL BYTE
?g_GameTransitionState@@3HA LABEL BYTE
?g_GuiGlobalMode@@3HA LABEL BYTE
    DB 12 DUP(?)
?g_GameNewSession@@3HA LABEL BYTE
?g_ReplayStageFlags@@3IA LABEL BYTE
    DB 40 DUP(?)
?g_FrontEndMusicCommentAnm@@3PAUAnmLoadedView@@A LABEL BYTE
    DB 8 DUP(?)
?g_AnmPreloadStopRequested@@3CA LABEL BYTE
?g_GameConfigurationFlags@@3IA LABEL BYTE
?g_GuiSupervisorFlags@@3IA LABEL BYTE
?g_InputSupervisorFlags@@3IA LABEL BYTE
    DB 4 DUP(?)
?g_GameStartupTimeTick@@3IA LABEL BYTE
    DB 316 DUP(?)
?g_MainScreenshotThreadHandle@@3KA LABEL BYTE
    DB 288 DUP(?)
?g_GameWorkerState@@3PAEA LABEL BYTE
    DB 32 DUP(?)
?g_CriticalSections@th10@@3PAU_RTL_CRITICAL_SECTION@@A LABEL BYTE
?g_EnemyCallbackCriticalSection@@3UEnemyCriticalSectionView@@A LABEL BYTE
?g_ItemCallbackCriticalSection@@3UItemCriticalSectionView@@A LABEL BYTE
?g_PlayerCallbackCriticalSection@@3UPlayerCriticalSectionView@@A LABEL BYTE
    DB 48 DUP(?)
?gReplayFileCriticalSection@@3U_RTL_CRITICAL_SECTION@@A LABEL BYTE
    DB 120 DUP(?)
?g_ChainNestCounter@th10@@3EA LABEL BYTE
?g_EnemyCallbackLockDepth@@3EA LABEL BYTE
?g_ItemCallbackLockDepth@@3EA LABEL BYTE
?g_PlayerCallbackLockDepth@@3EA LABEL BYTE
    DB 2 DUP(?)
?gReplayFileOpenCount@@3EA LABEL BYTE
    DB 1 DUP(?)
?g_LogLockDepth@th10@@3EA LABEL BYTE
    DB 97 DUP(?)
?g_MainExecutableChecksum@@3IA LABEL BYTE
    DB 4 DUP(?)
?g_MainExecutableSize@@3IA LABEL BYTE
    DB 4 DUP(?)
?g_GameStartupVersionSize@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_GameStartupVersionData@@3PAEA LABEL BYTE
    DB 28 DUP(?)
?g_GameStartupTextColor@@3IA LABEL BYTE
?g_StageScriptGlobal4923A8@@3HA LABEL BYTE
    DB 8 DUP(?)
?g_PbgArchives@@3PAVPbgArchive@@A LABEL BYTE
    DB 320 DUP(?)
?g_GameWindowView@@3UGameWindowView@@A LABEL BYTE
    DB 12 DUP(?)
?g_InputEnabled@@3HA LABEL BYTE
    DB 148 DUP(?)
?g_MainSoundOwner@@3PAEA LABEL BYTE
    DB 8072 DUP(?)
?g_GameLoaderPending@@3HA LABEL BYTE
    DB 12940 DUP(?)
?g_SoundBgmFileBaseOffset@@3HA LABEL BYTE
    DB 8 DUP(?)
?g_SoundDataLoadThreadHandle@@3PAXA LABEL BYTE
    DB 168 DUP(?)
?g_FrontEndSoundBgmVolume@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndSoundSfxVolume@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndSoundBgmAttenuation@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_AnmQuadVerticesNoDiffuse@@3PAUAnmRenderVertexNoDiffuseView@@A LABEL BYTE
    DB 96 DUP(?)
?g_AnmQuadVertices@@3PAUAnmRenderVertexView@@A LABEL BYTE
    DB 112 DUP(?)
?g_AnmBackgroundQuadVertices@@3PAUAnmRenderVertexNoDiffuseView@@A LABEL BYTE
    DB 96 DUP(?)
?g_MainPbgArchive@@3VPbgArchive@@A LABEL BYTE
    DB 16 DUP(?)
?g_FrontEndPracticeSecretIdleFrames@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndPracticeSecretProgress@@3HA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndKeyboardTransitions@@3PAEA LABEL BYTE
    DB 256 DUP(?)
?g_FrontEndStageKeyboardState@@3PAEA LABEL BYTE
    DB 256 DUP(?)
?g_MainExclusiveMutex@@3PAXA LABEL BYTE
    DB 8 DUP(?)
?g_InputJoystickButtons@@3PACA LABEL BYTE
    DB 136 DUP(?)
?g_EnemyEclFilenameBuffer@@3PADA LABEL BYTE
?g_StageFilePath@@3PADA LABEL BYTE
    DB 328 DUP(?)
?g_EclWorldAnchor@@3UAnmFloat3View@@A LABEL BYTE
    DB 12 DUP(?)
?g_EclWorldAnchorFlags@@3IA LABEL BYTE
    DB 4 DUP(?)
?g_FrontEndPreviousKeyboardState@@3PAEA LABEL BYTE
    DB 256 DUP(?)
?g_FrontEndKeyboardState@@3PAEA LABEL BYTE
    DB 880 DUP(?)
_BSS ENDS

END
