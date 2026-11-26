// Initialized tables, game state, and unified .bss globals for TH10 v1.00a.
// Pure C++ definitions replacing the _DATA and _BSS sections of ProductData.asm.
//
// Compiled WITHOUT /Gy and WITHOUT /GL so VC7.1 preserves declaration order
// for the three contiguous enclosing structs (g_GameScoreState, g_FrontEndInput,
// and g_MainSupervisorView / g_MainSoundOwner) while keeping all storage
// 100% relocatable and Position Independent.

#include <stddef.h>
#include "GameErrorContext.hpp"
#include "ScriptViewerWorker.hpp"

// Forward declarations for code pointers referenced in initialized tables
struct AnmVmView;
struct Player;
struct PlayerShotRuntimeView;
extern int __fastcall InstallAnmVmVertexBuffer(AnmVmView *vm, const unsigned int *data);
extern int __fastcall PlayerShotAcquireTarget(Player *player, PlayerShotRuntimeView *shot, int index);
extern int __fastcall PlayerShotTrackEnemy(void *player, PlayerShotRuntimeView *shot);
extern int __fastcall PlayerShotSetOptionOrigin(Player *player, PlayerShotRuntimeView *shot);

// ===========================================================================
// 1. Initialized data tables (.data)
// ===========================================================================

int g_BulletSpriteScriptBase[28] = {
    0, 64, 80, 96, 112, 128, 144,
    160, 176, 192, 208, 224, 275, 283,
    291, 299, 307, 347, 373, 315, 323,
    331, 339, 240, 48, 374, 243, 0,
};
float g_BulletCollisionSize[28] = {
    4.0f, 6.0f, 6.0f, 4.0f, 4.0f, 4.0f, 4.0f,
    4.0f, 4.0f, 0.0f, 4.0f, 6.0f, 10.0f, 8.0f,
    8.0f, 8.0f, 8.0f, 28.0f, 6.0f, 10.0f, 8.0f,
    8.0f, 8.0f, 6.0f, 4.0f, 6.0f, 4.0f, 0.0f,
};
int g_BulletDrawBucket[28] = {
    5, 3, 3, 4, 4, 4, 3,
    4, 4, 4, 4, 3, 1, 2,
    2, 2, 2, 0, 2, 2, 2,
    1, 2, 3, 5, 2, 4, 0,
};
int g_BulletEffectMode[27] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 2,
    3, 1, 1, 1, 1, 0, 0, 4, 0,
};
int g_BulletEffectScriptByColor[9] = { 17, 21, 25, 29, 33, 37, 41, 47, 0 };
unsigned int g_BulletCancelPaletteSmall[16] = { 0xFF808080u, 0xFFFF1010u, 0xFFFF1010u, 0xFF801080u, 0xFF801080u, 0xFF1010FFu, 0xFF1010FFu, 0xFF108080u, 0xFF108080u, 0xFF10FF10u, 0xFF10FF10u, 0xFF10FF10u, 0xFF808010u, 0xFF808010u, 0xFF808010u, 0xFF808080u };
unsigned int g_BulletCancelPaletteMedium[8] = { 0xFF808080u, 0xFFFF1010u, 0xFF801080u, 0xFF1010FFu, 0xFF108080u, 0xFF10FF10u, 0xFF808010u, 0xFF808080u };
unsigned int g_BulletCancelPaletteLarge[4] = { 0xFFFF1010u, 0xFF1010FFu, 0xFF10FF10u, 0xFF808010u };
extern const signed char g_FrontEndPracticeDifficulties[112] = {
    2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1,
    2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1,
    2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1,
    2, 3, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2,
    3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2,
    3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2,
    3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0,
};
struct AnmSpecialVmDescriptorView { int id; void *callback; };
extern const AnmSpecialVmDescriptorView g_AnmSpecialVmDescriptors[2] = {
    { 0x160, reinterpret_cast<void *>(InstallAnmVmVertexBuffer) },
    { 0, 0 },
};
const char *g_EndingScriptPaths[13] = {
    "e00.msg",
    "e01.msg",
    "e02.msg",
    "e03.msg",
    "e04.msg",
    "e05.msg",
    "e06.msg",
    "e07.msg",
    "e08.msg",
    "e09.msg",
    "e10.msg",
    "e11.msg",
    "staff.msg",
};
int g_GuiNormalExtendScores[5] = { 2000000, 4000000, 8000000, 15000000, 1000000000 };
int g_GuiExtraExtendScores[3] = { 3000000, 10000000, 1000000000 };
const char *g_FrontEndScoreEntryAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-=.,!?@:;[]()_/{}|~^#$%&*   ";
const char *g_ReplayCharacterNames[6] = {
    "ReimuA ",
    "ReimuB ",
    "ReimuC ",
    "MarisaA",
    "MarisaB",
    "MarisaC",
};
const char *g_ReplayDifficultyNames[5] = {
    "Easy   ",
    "Normal ",
    "Hard   ",
    "Lunatic",
    "Extra  ",
};
const char *g_ResultDifficultyShortNames[5] = {
    "E",
    "N",
    "H",
    "L",
    "X",
};
const char *g_FrontEndStageNames[10] = {
    "test   ",
    "Stage 1",
    "Stage 2",
    "Stage 3",
    "Stage 4",
    "Stage 5",
    "Stage 6",
    "Extra  ",
    "Clear  ",
    "ExClear",
};
const char *g_ReplayCompletionNames[10] = {
    "tst",
    "St1",
    "St2",
    "St3",
    "St4",
    "St5",
    "St6",
    "Ex ",
    "All",
    "ExA",
};
void *g_PlayerShotSpawnCallbacks[3] = {
    0,
    reinterpret_cast<void *>(PlayerShotAcquireTarget),
    0,
};
typedef void (__fastcall *PlayerShotUpdateCb)(Player *, PlayerShotRuntimeView *);
PlayerShotUpdateCb g_PlayerShotUpdateCallbacks[4] = {
    0,
    reinterpret_cast<PlayerShotUpdateCb>(PlayerShotTrackEnemy),
    reinterpret_cast<PlayerShotUpdateCb>(PlayerShotSetOptionOrigin),
    0,
};
struct FrontEndStageRecordEntry {
    int stageIndex;
    const char *stdPath;
    const char *anmPath;
    const char *eclPath;
    const char *stageBgmPath;
    const char *bossBgmPath;
    const char *midMsgPath;
    const char *bossMsgPath;
    const char *logoAnmPath;
    unsigned int tail[3];
};
FrontEndStageRecordEntry g_FrontEndStageRecords_storage[8] = {
    { 0, "stage00.std", "stgenm00.anm", "stage00.ecl", "bgm/th10_00.wav", "bgm/th10_01.wav", "st01_00.msg", "st01_00.msg", "st01logo.anm", { 0x0u, 0x0u, 0x0u } },
    { 1, "stage01.std", "stgenm01.anm", "stage01.ecl", "bgm/th10_00.wav", "bgm/th10_01.wav", "st01_00.msg", "st01_01.msg", "st01logo.anm", { 0x1u, 0x2u, 0x0u } },
    { 2, "stage02.std", "stgenm02.anm", "stage02.ecl", "bgm/th10_03.wav", "bgm/th10_04.wav", "st02_00.msg", "st02_01.msg", "st02logo.anm", { 0x3u, 0x4u, 0x0u } },
    { 3, "stage03.std", "stgenm03.anm", "stage03.ecl", "bgm/th10_05.wav", "bgm/th10_06.wav", "st03_00.msg", "st03_01.msg", "st03logo.anm", { 0x5u, 0x6u, 0x0u } },
    { 4, "stage04.std", "stgenm04.anm", "stage04.ecl", "bgm/th10_07.wav", "bgm/th10_08.wav", "st04_00.msg", "st04_01.msg", "st04logo.anm", { 0x7u, 0x8u, 0x0u } },
    { 5, "stage05.std", "stgenm05.anm", "stage05.ecl", "bgm/th10_09.wav", "bgm/th10_10.wav", "st05_00.msg", "st05_01.msg", "st05logo.anm", { 0x9u, 0xAu, 0x0u } },
    { 6, "stage06.std", "stgenm06.anm", "stage06.ecl", "bgm/th10_11.wav", "bgm/th10_12.wav", "st06_00.msg", "st06_01.msg", "st06logo.anm", { 0xBu, 0xCu, 0x0u } },
};
FrontEndStageRecordEntry g_FrontEndDefaultScoreFormatTable_storage = { 7, "stage07.std", "stgenm07.anm", "stage07.ecl", "bgm/th10_15.wav", "bgm/th10_16.wav", "st07_00.msg", "st07_01.msg", "st07logo.anm", { 0xDu, 0xEu, 0x0u } };
char *g_PbgFileOpenModes[4] = { const_cast<char *>("r"), const_cast<char *>("w"), const_cast<char *>("a"), 0 };
struct TextRenderFormatInfo { unsigned int f0, f1, f2, f3; };
TextRenderFormatInfo g_TextRenderFormats[11] = {
    { 0x16u, 0x20u, 0x0u, 0xFF0000u },
    { 0xFF00u, 0xFFu, 0x15u, 0x20u },
    { 0xFF000000u, 0xFF0000u, 0xFF00u, 0xFFu },
    { 0x18u, 0x10u, 0x0u, 0x7C00u },
    { 0x3E0u, 0x1Fu, 0x17u, 0x10u },
    { 0x0u, 0xF800u, 0x7E0u, 0x1Fu },
    { 0x19u, 0x10u, 0x8000u, 0x7C00u },
    { 0x3E0u, 0x1Fu, 0x1Au, 0x10u },
    { 0xF000u, 0xF00u, 0xF0u, 0xFu },
    { 0xFFFFFFFFu, 0x0u, 0x0u, 0x0u },
    { 0x0u, 0x0u, 0x1u, 0x0u },
};
// Contiguous 47-entry Sound metadata table (0x004749C8..0x00474B40, 376 bytes):
// Three overlapping views share this single 376-byte storage block:
// - g_SoundPlaybackMetadata (Sound.cpp, @ +0): 47 x 8-byte SoundPlaybackMetadata
//   { int sampleIndex; short volume; short unknown006; }
// - g_EnemySoundCueMetadata (EnemyEclDispatcher.cpp, @ +0): 47 x 8-byte
//   EnemySoundCueMetadataView { int unknown000; unsigned short unknown004; short cueValue; }
// - g_SoundCueMetadata (Sound.cpp, @ +4): overlapping view starting at +4
//   (46 complete 8-byte strides + 4-byte tail 0x32FCE0u = 47th entry's
//   volume=0xFCE0 and cueValue/unknown006=0x0032, read via cueValue at +6).
#pragma data_seg(".data$th10_soundmeta")
// Align g_SoundCueMetadata_storage to a 16-byte boundary while keeping
// g_EnemySoundCueMetadata_head immediately preceding it at -4 bytes (no
// compiler-inserted alignment gap between head and storage).
unsigned int _soundmeta_align_pad[3] = { 0u, 0u, 0u };
unsigned int g_EnemySoundCueMetadata_head = 0x00000000u;
unsigned int g_SoundCueMetadata_storage[93] = {
    0xF894u, 0x0u, 0xF7CCu, 0x1u, 0x5FB50u, 0x1u, 0x5FA24u, 0x2u,
    0x64FBB4u, 0x3u, 0x64FD44u, 0x4u, 0x64FD44u, 0x5u, 0x32F894u, 0x6u,
    0x32F768u, 0x7u, 0x32F6A0u, 0x8u, 0x64FE0Cu, 0x9u, 0x64FE70u, 0xAu,
    0xAFCE0u, 0xBu, 0xAFA24u, 0xCu, 0x64FC18u, 0x5u, 0x32FBB4u, 0xDu,
    0x32FAECu, 0xEu, 0x32FA88u, 0xFu, 0x64FC7Cu, 0x10u, 0xFC90u, 0x11u,
    0xFA24u, 0x5u, 0x14FED4u, 0x6u, 0x14F8F8u, 0x7u, 0x14F8F8u, 0x12u,
    0x32FBB4u, 0x13u, 0x32FAECu, 0x14u, 0x32FA24u, 0x15u, 0x64FE0Cu, 0x16u,
    0x14FBB4u, 0x17u, 0x5AFCE0u, 0x16u, 0x14FB50u, 0x12u, 0x32FE0Cu, 0x18u,
    0x64FCE0u, 0x19u, 0x64FCE0u, 0x1Au, 0x64FCE0u, 0x1Bu, 0xFE0Cu, 0x1Cu,
    0x64FED4u, 0x1Du, 0x640000u, 0x1Eu, 0x640000u, 0x1Eu, 0x64FDA8u, 0x8u,
    0x64FED4u, 0x1Fu, 0x64FED4u, 0x20u, 0x64FED4u, 0x21u, 0x64FED4u, 0x22u,
    0x64FF9Cu, 0x23u, 0x640000u, 0x24u, 0x32FCE0u,
};
#pragma data_seg()
typedef char SoundCueMetadataStorageLayout[
    (sizeof(g_EnemySoundCueMetadata_head) + sizeof(g_SoundCueMetadata_storage) == 47 * 8 &&
     sizeof(g_SoundCueMetadata_storage) == 46 * 8 + 4) ? 1 : -1];
struct PbgDecryptProfile { unsigned int key, step, limit; };
PbgDecryptProfile g_PbgDecryptProfiles[8] = {
    { 0xAA371Bu, 0x40u, 0x2800u },
    { 0xBBE951u, 0x40u, 0x3000u },
    { 0xCC51C1u, 0x80u, 0x3200u },
    { 0xDD1903u, 0x400u, 0x7800u },
    { 0xEECDABu, 0x200u, 0x2800u },
    { 0xFF3412u, 0x80u, 0x3200u },
    { 0x119735u, 0x80u, 0x2800u },
    { 0x773799u, 0x400u, 0x2000u },
};
void *gReplayFileHandle = reinterpret_cast<void *>(-1);

// Contiguous GameScoreState block (0x00474C40 .. 0x00474CB0)
#pragma data_seg(".data$th10_score")
unsigned char _score_slot_00474C40[4] = { 0x00, 0x00, 0x00, 0x00 };
int g_GameScore = 0;
unsigned char _score_slot_00474C48[4] = { 0x00, 0x00, 0x00, 0x00 };
int g_EnemyTimeoutValue = 0;
unsigned char _score_slot_00474C50[8] = { 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };
unsigned char _score_slot_00474C58[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
int g_GameStatisticsIndex0 = 0;
int g_GameStatisticsIndex1 = 0;
int g_GuiPowerLevel = 0;
unsigned char _score_slot_00474C74[8] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
int g_FrontEndSelectedStage = 0;
int g_FrontEndSelectedStageMirror = 0;
int g_EnemyDialogIndex = 0;
int g_GameFrameCounter0 = 0;
int g_EnemyDialogState = 0;
int g_GameStartupValue474C90 = 0;
int g_GuiValue4C94 = 0;
int g_EnemyRank = 0;
int g_GuiValue4C9C = 0;
unsigned char _score_slot_00474CA0[12] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
int g_FrontEndStageShortcut = 0;
#pragma data_seg()

unsigned char g_TextRenderBuffer_storage[0x124] = { 0 };
ScriptViewerWorkerView g_MainScanWorker;
float g_EnemyWorldMatrix_storage[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };


short g_DefaultGameKeyBindings[9] = { 0, 1, 2, 3, -1, -1, -1, -1, 4 };
namespace th10 {
GameErrorContext g_GameErrorContext = { { 0 }, g_GameErrorContext.m_Buffer, 0 };
}

float g_AnmGameSpeed = 1.0f;
int g_PlayerOptionPositionBase[9] = { 0, 0, 1, 3, 6, 10, 11, 13, 16 };
float g_PlayerOptionValue08ByCharacter[2] = { 5.0f, 7.0f };
float g_PlayerExtent2ByCharacter[2] = { 100.0f, 118.0f };
float g_PlayerOptionValue0CByCharacter[2] = { 40.0f, 48.0f };
float g_PlayerOptionValue04ByCharacter[2] = { 2.0f, 3.5f };
const char *g_PlayerShotDataFilenames[6] = {
    "pl00a.sht",
    "pl00b.sht",
    "pl00c.sht",
    "pl01a.sht",
    "pl01b.sht",
    "pl01c.sht",
};
const char *g_FrontEndDemoPaths[4] = {
    "demo/demo1.rpy",
    "demo/demo2.rpy",
    "demo/demo3.rpy",
    "demo/demo0.rpy",
};

// Shared-storage alias bindings (single storage slot per retail global)
#pragma comment(linker, "/alternatename:?g_EclWorldOffsetX@@3MA=?g_BulletDrawWorldOffsetX@@3MA")
#pragma comment(linker, "/alternatename:?g_EclWorldOffsetY@@3MA=?g_BulletDrawWorldOffsetY@@3MA")
#pragma comment(linker, "/alternatename:?g_EndingDataErrorMessage@@3PADA=?TH_ERR_DATA_CORRUPTED@th10@@3PADA")
#pragma comment(linker, "/alternatename:?g_EnemyCancelColorHigh@@3PAIA=?g_BulletCancelPaletteLarge@@3PAIA")
#pragma comment(linker, "/alternatename:?g_EnemyCancelColorLow@@3PAIA=?g_BulletCancelPaletteSmall@@3PAIA")
#pragma comment(linker, "/alternatename:?g_EnemyCancelColorMid@@3PAIA=?g_BulletCancelPaletteMedium@@3PAIA")
#pragma comment(linker, "/alternatename:?g_EnemyChapterState@@3UEnemyChapterStateView@@A=?_score_slot_00474C40@@3PAEA")
#pragma comment(linker, "/alternatename:?g_EnemyDamageProcessedFlag@@3HA=?_score_slot_00474C50@@3PAEA")
#pragma comment(linker, "/alternatename:?g_EnemyDifficulty@@3HA=?_score_slot_00474C74@@3PAEA")
#pragma comment(linker, "/alternatename:?g_EnemyPlayfieldMaxX@@3MA=?g_BulletCullLeft@@3MA")
#pragma comment(linker, "/alternatename:?g_EnemyPlayfieldMinX@@3MA=?g_BulletCullRight@@3MA")
#pragma comment(linker, "/alternatename:?g_EnemyPlayfieldMinY@@3MA=?g_BulletCullBottom@@3MA")
#pragma comment(linker, "/alternatename:?g_EnemySoundCueMetadata@@3PAUEnemySoundCueMetadataView@@A=?g_EnemySoundCueMetadata_head@@3IA")
#pragma comment(linker, "/alternatename:?g_EnemySpawnLayerIndex@@3HA=?_score_slot_00474C74@@3PAEA")
#pragma comment(linker, "/alternatename:?g_EnemyWorldMatrix@@3EA=?g_EnemyWorldMatrix_storage@@3PAMA")
#pragma comment(linker, "/alternatename:?g_FrontEndDefaultScoreFormatTable@@3PAEA=?g_FrontEndDefaultScoreFormatTable_storage@@3UFrontEndStageRecordEntry@@A")
#pragma comment(linker, "/alternatename:?g_FrontEndFlags@@3IA=?_score_slot_00474CA0@@3PAEA")
#pragma comment(linker, "/alternatename:?g_FrontEndScoreEntryFormatTable@@3PAEA=?g_PbgFileOpenModes@@3PAPADA")
#pragma comment(linker, "/alternatename:?g_FrontEndStageRecords@@3PAY0DA@EA=?g_FrontEndStageRecords_storage@@3PAUFrontEndStageRecordEntry@@A")
#pragma comment(linker, "/alternatename:?g_GameFrameCounter1@@3HA=?g_EnemyDialogState@@3HA")
#pragma comment(linker, "/alternatename:?g_GameFrameRestorePending@@3HA=?g_EnemyDialogIndex@@3HA")
#pragma comment(linker, "/alternatename:?g_GameLivesOverride@@3HA=?g_FrontEndStageShortcut@@3HA")
#pragma comment(linker, "/alternatename:?g_GameRuntimeFlags@@3IA=?_score_slot_00474CA0@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GameScoreState@@3UGameScoreStateView@@A=?_score_slot_00474C40@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GameStageState@@3PAEA=?_score_slot_00474C40@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GameStartingStage@@3HA=?_score_slot_00474C74@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GameStartupMode@@3HA=?g_FrontEndSelectedStage@@3HA")
#pragma comment(linker, "/alternatename:?g_GameStartupState@@3UGameStartupStateView@@A=?_score_slot_00474C40@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GameStatisticsRankKind@@3PACA=?g_FrontEndPracticeDifficulties@@3QBCB")
#pragma comment(linker, "/alternatename:?g_GuiCounter4C4C@@3HA=?g_EnemyTimeoutValue@@3HA")
#pragma comment(linker, "/alternatename:?g_GuiDifficulty@@3HA=?_score_slot_00474C74@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GuiGameFlags@@3IA=?_score_slot_00474CA0@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GuiMessageState@@3HA=?g_EnemyDialogIndex@@3HA")
#pragma comment(linker, "/alternatename:?g_GuiMessageStateAux@@3HA=?g_EnemyDialogState@@3HA")
#pragma comment(linker, "/alternatename:?g_GuiPower@@3FA=?_score_slot_00474C48@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GuiScore@@3UGuiScoreView@@A=?_score_slot_00474C40@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GuiStage@@3HA=?g_FrontEndSelectedStage@@3HA")
#pragma comment(linker, "/alternatename:?g_GuiState4C58@@3HA=?_score_slot_00474C58@@3PAEA")
#pragma comment(linker, "/alternatename:?g_GuiValue4C90@@3HA=?g_GameStartupValue474C90@@3HA")
#pragma comment(linker, "/alternatename:?g_PlayerAuxiliaryDrawSpan@@3HA=?_score_slot_00474C58@@3PAEA")
#pragma comment(linker, "/alternatename:?g_PlayerCharacter@@3HA=?g_GameStatisticsIndex0@@3HA")
#pragma comment(linker, "/alternatename:?g_PlayerLifecycleFlags@@3IA=?_score_slot_00474CA0@@3PAEA")
#pragma comment(linker, "/alternatename:?g_PlayerLivesDisplayCount@@3HC=?g_GuiPowerLevel@@3HA")
#pragma comment(linker, "/alternatename:?g_PlayerPower@@3FA=?_score_slot_00474C48@@3PAEA")
#pragma comment(linker, "/alternatename:?g_PlayerRuntimeGateValue@@3HA=?g_GuiPowerLevel@@3HA")
#pragma comment(linker, "/alternatename:?g_PlayerShotType@@3HA=?g_GameStatisticsIndex1@@3HA")
#pragma comment(linker, "/alternatename:?g_PlayerTimerScale@@3MA=?g_AnmGameSpeed@@3MA")
#pragma comment(linker, "/alternatename:?g_ReplayCharacter@@3HA=?g_GameStatisticsIndex0@@3HA")
#pragma comment(linker, "/alternatename:?g_ReplayDifficulty@@3HA=?_score_slot_00474C74@@3PAEA")
#pragma comment(linker, "/alternatename:?g_ReplayShotType@@3HA=?g_GameStatisticsIndex1@@3HA")
#pragma comment(linker, "/alternatename:?g_ReplayStageValue1B8@@3IA=?g_GuiValue4C9C@@3HA")
#pragma comment(linker, "/alternatename:?g_ScriptViewerRouteDescriptor@@3PAEA=?g_FrontEndStageRecords_storage@@3PAUFrontEndStageRecordEntry@@A")
#pragma comment(linker, "/alternatename:?g_ScriptViewerShownStage@@3HA=?g_FrontEndSelectedStageMirror@@3HA")
#pragma comment(linker, "/alternatename:?g_ScriptViewerStage@@3HA=?g_FrontEndSelectedStage@@3HA")
#pragma comment(linker, "/alternatename:?g_SoundCueMetadata@@3PAUSoundCueMetadata@@A=?g_SoundCueMetadata_storage@@3PAIA")
#pragma comment(linker, "/alternatename:?g_SoundPlaybackMetadata@@3PAUSoundPlaybackMetadata@@A=?g_EnemySoundCueMetadata_head@@3IA")
#pragma comment(linker, "/alternatename:?g_TextRenderBuffer@@3UTextRenderBufferView@@A=?g_TextRenderBuffer_storage@@3PAEA")
#pragma comment(linker, "/alternatename:_g_EnemyLaserPlayfieldMaximumX=?g_BulletCullRight@@3MA")
#pragma comment(linker, "/alternatename:_g_EnemyLaserPlayfieldMaximumY=?g_BulletCullBottom@@3MA")
#pragma comment(linker, "/alternatename:_g_EnemyLaserPlayfieldMinimumX=?g_BulletCullLeft@@3MA")
#pragma comment(linker, "/alternatename:_g_EnemyLaserPlayfieldMinimumY=?g_EnemyPlayfieldMaxY@@3MA")
#pragma comment(linker, "/alternatename:_g_PlayerShotPlayfieldMaximumX=?g_BulletCullRight@@3MA")
#pragma comment(linker, "/alternatename:_g_PlayerShotPlayfieldMaximumY=?g_BulletCullBottom@@3MA")
#pragma comment(linker, "/alternatename:_g_PlayerShotPlayfieldMinimumX=?g_BulletCullLeft@@3MA")
#pragma comment(linker, "/alternatename:_g_PlayerShotPlayfieldMinimumY=?g_EnemyPlayfieldMaxY@@3MA")
