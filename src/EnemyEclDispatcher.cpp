#include "Enemy.hpp"

#include <math.h>


// The target dispatcher at 0x0040E770 owns opcodes 0x100 through 0x1B4.
// This translation retains neutral helper names where the original source name
// is not recoverable yet.  Each call and field access comes from the attested
// TH10 v1.00a body; adjacent ECL maps were used only to group opcode families.
enum EnemyEclOpcode
{
    ENEMY_ECL_CREATE_ENEMY = 0x100,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE = 0x101,
    ENEMY_ECL_SELECT_ANM_RESOURCE = 0x102,
    ENEMY_ECL_SET_ANM_SCRIPT = 0x103,
    ENEMY_ECL_CREATE_ENEMY_MIRRORED = 0x104,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED = 0x105,
    ENEMY_ECL_SET_MAIN_ANM_SCRIPT = 0x106,
    ENEMY_ECL_PLAY_ANM = 0x107,
    ENEMY_ECL_PLAY_ANM_ABSOLUTE = 0x108,
    ENEMY_ECL_CREATE_ENEMY_IF_NO_BOSS = 0x109,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_IF_NO_BOSS = 0x10A,
    ENEMY_ECL_CREATE_ENEMY_MIRRORED_IF_NO_BOSS = 0x10B,
    ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED_IF_NO_BOSS = 0x10C,
    ENEMY_ECL_PLAY_SELECTED_ANM = 0x10D,
    ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET = 0x10E,
    ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET_IF_NO_BOSS = 0x10F,
    ENEMY_ECL_PLAY_ANM_HIGH = 0x110,
    ENEMY_ECL_PLAY_ANM_ROTATED = 0x111,
    ENEMY_ECL_SET_OFFSET_POSITION = 0x118,
    ENEMY_ECL_INTERPOLATE_OFFSET_POSITION = 0x119,
    ENEMY_ECL_SET_BASE_POSITION = 0x11A,
    ENEMY_ECL_INTERPOLATE_BASE_POSITION = 0x11B,
    ENEMY_ECL_SET_OFFSET_POLAR = 0x11C,
    ENEMY_ECL_INTERPOLATE_OFFSET_POLAR = 0x11D,
    ENEMY_ECL_SET_BASE_POLAR = 0x11E,
    ENEMY_ECL_INTERPOLATE_BASE_POLAR = 0x11F,
    ENEMY_ECL_SET_OFFSET_CIRCLE = 0x120,
    ENEMY_ECL_INTERPOLATE_OFFSET_CIRCLE = 0x121,
    ENEMY_ECL_SET_BASE_CIRCLE = 0x122,
    ENEMY_ECL_INTERPOLATE_BASE_CIRCLE = 0x123,
    ENEMY_ECL_MOVE_RANDOM = 0x124,
    ENEMY_ECL_MOVE_RANDOM_BASE = 0x125,
    ENEMY_ECL_COPY_PLAYER_TO_OFFSET = 0x126,
    ENEMY_ECL_COPY_PLAYER_TO_BASE = 0x127,
    ENEMY_ECL_ADD_OFFSET_POSITION = 0x128,
    ENEMY_ECL_ADD_BASE_POSITION = 0x129,
    ENEMY_ECL_ADD_OFFSET_VELOCITY = 0x12A,
    ENEMY_ECL_ADD_BASE_VELOCITY = 0x12B,
    ENEMY_ECL_SET_DAMAGE_HITBOX = 0x140,
    ENEMY_ECL_SET_PLAYER_HITBOX = 0x141,
    ENEMY_ECL_SET_FLAGS = 0x142,
    ENEMY_ECL_CLEAR_FLAGS = 0x143,
    ENEMY_ECL_SET_MOVEMENT_BOUNDS = 0x144,
    ENEMY_ECL_CLEAR_MOVEMENT_BOUNDS = 0x145,
    ENEMY_ECL_CLEAR_ITEM_DROPS = 0x146,
    ENEMY_ECL_ADD_ITEM_DROP = 0x147,
    ENEMY_ECL_SET_DROP_AREA = 0x148,
    ENEMY_ECL_DROP_ITEMS = 0x149,
    ENEMY_ECL_SET_MAIN_DROP = 0x14A,
    ENEMY_ECL_SET_LIFE = 0x14B,
    ENEMY_ECL_SET_BOSS_SLOT = 0x14C,
    ENEMY_ECL_RESET_UPDATE_TIMER = 0x14D,
    ENEMY_ECL_SET_INTERRUPT = 0x14E,
    ENEMY_ECL_SET_INVULNERABILITY = 0x14F,
    ENEMY_ECL_PLAY_SOUND = 0x150,
    ENEMY_ECL_SET_SCREEN_SHAKE = 0x151,
    ENEMY_ECL_READ_DIALOG = 0x152,
    ENEMY_ECL_WAIT_DIALOG = 0x153,
    ENEMY_ECL_WAIT_BOSS_CLEAR = 0x154,
    ENEMY_ECL_SET_TIMEOUT = 0x155,
    ENEMY_ECL_START_SPELL = 0x156,
    ENEMY_ECL_END_SPELL = 0x157,
    ENEMY_ECL_SET_CHAPTER = 0x158,
    ENEMY_ECL_KILL_ALL_ENEMIES = 0x159,
    ENEMY_ECL_SET_BULLET_PROTECTION_RANGE = 0x15A,
    ENEMY_ECL_SET_LIFE_MARKER = 0x15B,
    ENEMY_ECL_START_SPELL_2 = 0x15C,
    ENEMY_ECL_SELECT_FLOAT_BY_RANK_3 = 0x15D,
    ENEMY_ECL_SELECT_FLOAT_BY_RANK_5 = 0x15E,
    ENEMY_ECL_INTERPOLATE_FLOAT_BY_RANK = 0x15F,
    ENEMY_ECL_SELECT_INT_BY_RANK_3 = 0x160,
    ENEMY_ECL_SELECT_INT_BY_RANK_5 = 0x161,
    ENEMY_ECL_INTERPOLATE_INT_BY_RANK = 0x162,
    ENEMY_ECL_SELECT_INT_BY_DIFFICULTY = 0x163,
    ENEMY_ECL_SELECT_FLOAT_BY_DIFFICULTY = 0x164,
    ENEMY_ECL_START_SPELL_DIFFICULTY = 0x165,
    ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1 = 0x166,
    ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2 = 0x167,
    ENEMY_ECL_SET_SPELL_TIMEOUT = 0x168,
    ENEMY_ECL_SET_PLAYER_COLLISION_TIMER = 0x169,
    ENEMY_ECL_CANCEL_LASERS = 0x16A,
    ENEMY_ECL_ENABLE_BOMB_SHIELD = 0x16B,
    ENEMY_ECL_SET_GAME_SPEED_FLAG = 0x16C,
    ENEMY_ECL_WAIT_FOR_EFFECT = 0x16D,
    ENEMY_ECL_SET_ANM_MODE = 0x16E,
    ENEMY_ECL_SET_TIMER_SCALE = 0x16F,
    ENEMY_ECL_SUBTRACT_DIFFICULTY_VALUE = 0x170,
    ENEMY_ECL_INITIALIZE_BULLET_PATTERN = 0x190,
    ENEMY_ECL_FIRE_BULLET_PATTERN = 0x191,
    ENEMY_ECL_SET_BULLET_SPRITES = 0x192,
    ENEMY_ECL_SET_BULLET_OFFSET = 0x193,
    ENEMY_ECL_SET_BULLET_ANGLE = 0x194,
    ENEMY_ECL_SET_BULLET_SPEED = 0x195,
    ENEMY_ECL_SET_BULLET_COUNT = 0x196,
    ENEMY_ECL_SET_BULLET_AIM_MODE = 0x197,
    ENEMY_ECL_SET_BULLET_SOUND = 0x198,
    ENEMY_ECL_SET_BULLET_EXTRA = 0x199,
    ENEMY_ECL_CANCEL_ALL_BULLETS = 0x19A,
    ENEMY_ECL_COPY_BULLET_PATTERN = 0x19B,
    ENEMY_ECL_FIRE_LASER_A = 0x19C,
    ENEMY_ECL_FIRE_STRAIGHT_LASER = 0x19D,
    ENEMY_ECL_SET_LASER_OFFSET = 0x19E,
    ENEMY_ECL_SET_LASER_ANGLE = 0x19F,
    ENEMY_ECL_SET_LASER_SPEED = 0x1A0,
    ENEMY_ECL_SET_LASER_COUNT = 0x1A1,
    ENEMY_ECL_SET_LASER_AIM = 0x1A2,
    ENEMY_ECL_SET_LASER_SOUND = 0x1A3,
    ENEMY_ECL_CANCEL_BULLET_PATTERN = 0x1A4,
    ENEMY_ECL_CLEAR_BULLET_PATTERN = 0x1A5,
    ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_3 = 0x1A6,
    ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_5 = 0x1A7,
    ENEMY_ECL_INTERPOLATE_BULLET_SPEED_BY_RANK = 0x1A8,
    ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_3 = 0x1A9,
    ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_5 = 0x1AA,
    ENEMY_ECL_INTERPOLATE_BULLET_COUNT_BY_RANK = 0x1AB,
    ENEMY_ECL_FIRE_LASER = 0x1AC,
    ENEMY_ECL_FIRE_STRAIGHT_LASER_2 = 0x1AD,
    ENEMY_ECL_AIM_BULLET_AT_PLAYER = 0x1AE,
    ENEMY_ECL_FIRE_LASER_A_WITH_PATTERN = 0x1AF,
    ENEMY_ECL_FIRE_STRAIGHT_LASER_WITH_PATTERN = 0x1B0,
    ENEMY_ECL_FIRE_LASER_WITH_PATTERN = 0x1B1,
    ENEMY_ECL_FIRE_STRAIGHT_LASER_2_WITH_PATTERN = 0x1B2,
    ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY = 0x1B3,
    ENEMY_ECL_SET_BULLET_COUNT_BY_DIFFICULTY = 0x1B4,
};

struct EnemyGameStateView;
struct EnemyManagedVmView;
struct EnemyVisualStateView;
union EnemyLaserRequestScratch;

extern int g_EnemyDifficulty;
extern int g_EnemyRank;
extern float g_PlayerTimerScale;
extern EnemyManagerView *g_EnemyManager;
extern EnemyGameStateView *g_EnemyGameState;
extern EnemyVisualStateView *g_EnemyVisualState;
extern void *g_EnemyBulletManager;
extern PlayerFloat3 g_EnemyGlobalPositionOffset;
extern PlayerFloat3 g_EnemyInterpolationOrigin;
extern EnemyFloat2 g_EnemyInterpolationBasis;
extern unsigned char g_EnemyViewMatrix;
extern unsigned char g_EnemyProjectionMatrix;
extern unsigned char g_EnemyViewport;
extern unsigned char g_EnemyWorldMatrix;

extern PlayerFloat3 *D3DXVec3Project(
    PlayerFloat3 *output, const PlayerFloat3 *input, const void *viewport,
    const void *projection, const void *view, const void *world);
extern void EnemyPrepareProjection();
extern void EnemyInitializePositionInterpolation(
    EnemyPositionInterpolationView *interpolation);
extern void EnemySetTimerCurrent(PlayerTimerView *timer, int value);
extern unsigned int EnemyFireBulletPattern(int manager, int pattern, int owner);
extern unsigned int EnemyCancelBulletPattern(float value, int mode, int flags);
extern unsigned int EnemyCancelAllBullets(int mode);
extern void EnemyBeginSpell(int gameState, int spellId, char *name, int value);
extern void EnemyEndSpell(EnemyGameStateView *gameState);
extern void EnemyEnableBombShield(EnemyGameStateView *gameState);
extern void EnemyInitializeLaserConfig(EnemyLaserRequestScratch *config);
extern void EnemySetLaserPosition(int laser, const PlayerFloat3 *position);
extern int EnemyFindLaser(int id);
extern int EnemySpawnFromEclInstruction(
    EnemyManagerView *manager, const void *subroutineName,
    const EnemySpawnRequestView *request);
extern void EnemyKillAll(EnemyManagerView *manager);
extern void EnemyConfigureInterrupt(int first, int second, int third);
extern void EnemyInitializeScalarInterpolation(
    EnemyScalarInterpolationView *interpolation);
extern void EnemySetChapter(int chapter);
extern void __stdcall EnemySetMotionAngle(EnemyMotionView *motion, float angle);
extern void EnemyReadDialog();
extern PlayerFloat3 *EnemyGetAnmPosition(const PlayerFloat3 *position);
extern unsigned int EnemyFireLaser(
    void *manager, EnemyLaserRequestScratch *request, int type);
extern unsigned int EnemyWaitForEffect();
extern int EnemyApplyBulletCancel();
extern unsigned int EnemyApplyBulletClear();
extern unsigned int *EnemySetScreenShake(
    int enabled, int duration, unsigned int horizontal,
    unsigned int vertical, unsigned int flags);
extern void EnemyPlaySound(int soundId);
extern void EnemyDropItemCounts(const PlayerFloat3 *position, int *itemDropBlock);
extern unsigned int *EnemyCreateManagedVm(
    unsigned int resource, int script, int layer);
extern unsigned int *EnemyCreateManagedVmRotated(
    unsigned int resource, int script, int layer);
extern EnemyManagedVmView *EnemyResolveManagedVm(unsigned int id);
extern void EnemyShowManagedVm(unsigned int *id);
extern void EnemyHideManagedVm(unsigned int *id);
extern void EnemyReleaseManagedVm(unsigned int *id);
extern float EnemyRandomAngle();
extern float __stdcall EnemyWrapAngle(float angle);

static int EnemyDecodeOperandIndex(float encoded)
{
    union { float f; int i; } bits;
    bits.f = encoded;
    return bits.i;
}

static float EnemyEncodeOperandIndex(int index)
{
    union { float f; int i; } bits;
    bits.i = index;
    return bits.f;
}

union EnemyLaserRequestScratch
{
    float floats[0x7e];
    unsigned int words[0x7e];
    unsigned short halves[0xfc];
};
typedef char EnemyLaserRequestScratchSizeIs1F8[
    (sizeof(EnemyLaserRequestScratch) == 0x1f8) ? 1 : -1];

static float EnemyAbsoluteFloat(float value)
{
    return value < 0.0f ? -value : value;
}

// Target 0x0044BC70-0x0044BCCD wraps one angle into the signed-pi interval.
// The shared counter and 32-iteration guard are both visible in the target.
float __stdcall EnemyWrapAngle(float angle)
{
    int i = 0;
    while (angle > 3.1415927f)
    {
        angle -= 6.2831855f;
        if (i++ > 32)
            break;
    }
    while (angle < -3.1415927f)
    {
        angle += 6.2831855f;
        if (i++ > 32)
            break;
    }
    return angle;
}

// Target 0x00413170-0x00413183 normalizes one motion angle and stores it in
// the +0x1C scalar used by the 0x11C/0x11E/0x120/0x122 ECL motion cases.
void __stdcall EnemySetMotionAngle(EnemyMotionView *motion, float angle)
{
    motion->value1C = EnemyWrapAngle(angle);
}

int EnemyRuntimeView::DispatchEclInstruction()

{
  int runtimeAddress = reinterpret_cast<int>(this);
  short opcode;
  float fVar2;
  unsigned char bVar3;
  unsigned int uVar4;
  unsigned int *puVar5;
  unsigned int uVar6;
  int *piVar7;
  unsigned int uVar8;
  float fVar9;
  float fVar10;
  int iVar11;
  unsigned int *puVar13;
  unsigned int *puVar14;
  float *pfVar15;
  unsigned char *pbVar16;
  int iVar17;
  float fVar19;
  float fVar20;
  float fVar21;
  int uVar22;
  int uVar23;
  int uVar24;
  int uVar25;
  int iVar26;
  int iVar27;
  int secondLowBulletCount;
  unsigned int uVar28;
  EnemyMotionView *selectedMotion;
  EnemyPositionInterpolationView *positionInterpolation;
  float *positionInterpolationValues;
  EnemyScalarInterpolationView *firstScalarInterpolation;
  EnemyScalarInterpolationView *secondScalarInterpolation;
  float *firstScalarValues;
  float *secondScalarValues;
  float local_2c8;
  float local_2c4;
  float local_2bc;
  float local_2b4;
  float local_2b0;
  float local_2ac;
  float local_2a8;
  float local_2a4;
  PlayerFloat3 projectionInput;
  char local_29d;
  float local_29c;
  float local_298;
  EnemyLaserRequestScratch local_288;
  union {
    EnemySpawnRequestView spawnRequest;
    float floatWords[16];
    unsigned char bytes[0x84];
  } local_90;

  iVar26 = reinterpret_cast<int>(owner);
  iVar27 = reinterpret_cast<int>(owner->activeEclContext->currentInstruction);
  opcode = *(short *)(iVar27 + 4);
  switch(opcode) {
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE:
    goto dispatch_create_enemy_absolute;
  case ENEMY_ECL_SELECT_ANM_RESOURCE:
    uVar22 = ReadIntArgument(0);
    *(int *)((int)runtimeAddress + 0xe8) = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_ANM_SCRIPT:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    uVar22 = ReadIntArgument(1);
    EnemyReleaseManagedVm(&managedVmIds[iVar26]);
    if ((int)uVar22 < 0) {
      return 0;
    }
    uVar22 = ReadIntArgument(1);
    puVar5 = (unsigned int *)
             EnemyCreateManagedVm(*(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x30 + *(int *)((int)runtimeAddress + 0xe8) * 4),
                          (int)uVar22,5);
    *(unsigned int *)((int)runtimeAddress + 0xc0 + iVar26 * 4) = *puVar5;
    uVar6 = managedVmIds[iVar26];
    if (iVar26 == 0) {
      uVar22 = ReadIntArgument(1);
      *(int *)((int)runtimeAddress + 0xf0) = (int)uVar22;
      *(unsigned int *)((int)runtimeAddress + 0xec) = *(unsigned int *)((int)runtimeAddress + 0xe8);
      uVar6 = managedVmIds[iVar26];
    }
    iVar27 = reinterpret_cast<int>(EnemyResolveManagedVm(uVar6));
    goto dispatch_update_primary_anm_bounds;
  case ENEMY_ECL_CREATE_ENEMY_MIRRORED:
dispatch_create_enemy_mirrored:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    fVar9 = *(float *)((int)runtimeAddress + 0x2c);
    pfVar15 = local_90.floatWords;
    for (iVar11 = 0x10; iVar11 != 0; iVar11 = iVar11 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
    fVar19 = (ReadRawFloatArgument((1), (*(float *)(iVar27 + 0x10 + iVar26 * 4))));
    fVar10 = *(float *)((int)runtimeAddress + 0x30);
    local_90.spawnRequest.position.x = (float)(fVar19 + fVar9);
    fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
    local_90.spawnRequest.position.y = (float)(fVar19 + fVar10);
    uVar22 = ReadRawIntArgument(3, *(int *)(iVar27 + 0x18 + iVar26 * 4));
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
    local_90.spawnRequest.itemDropType = uVar22;
    puVar5 = (unsigned int *)((int)runtimeAddress + 0xfc);
    pbVar16 = reinterpret_cast<unsigned char *>(&local_90.spawnRequest.eclVariables);
    for (iVar26 = 8; iVar26 != 0; iVar26 = iVar26 + -1) {
      *(unsigned int *)pbVar16 = *puVar5;
      puVar5 = puVar5 + 1;
      pbVar16 = pbVar16 + 4;
    }
    local_90.spawnRequest.setFlag0800 = 1;
    EnemySpawnFromEclInstruction(
        g_EnemyManager, reinterpret_cast<const void *>(iVar27 + 0x14),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED:
    goto dispatch_create_enemy_absolute_mirrored;
  case ENEMY_ECL_SET_MAIN_ANM_SCRIPT:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    uVar22 = ReadIntArgument(1);
    iVar27 = (int)uVar22;
    EnemyReleaseManagedVm(&managedVmIds[iVar26]);
    puVar5 = (unsigned int *)
             EnemyCreateManagedVm(*(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x30 + *(int *)((int)runtimeAddress + 0xe8) * 4),
                          iVar27,5);
    uVar6 = *puVar5;
    *(unsigned int *)((int)runtimeAddress + 0xc0 + iVar26 * 4) = uVar6;
    iVar11 = reinterpret_cast<int>(EnemyResolveManagedVm(uVar6));
    if (iVar26 == 0) {
      *(float *)((int)runtimeAddress + 0x13a4) = *(float *)(iVar11 + 0x50) * *(float *)(iVar11 + 0x40);
      *(float *)((int)runtimeAddress + 0x13a8) = *(float *)(iVar11 + 0x4c) * *(float *)(iVar11 + 0x3c);
    }
    if ((*(unsigned char *)((int)runtimeAddress + 0x1444) & 0x10) != 0) {
      EnemyHideManagedVm(&managedVmIds[iVar26]);
    }
    if (iVar26 == 0) {
      *(unsigned int *)((int)runtimeAddress + 0x1444) = *(unsigned int *)((int)runtimeAddress + 0x1444) | 0x1000;
      *(int *)((int)runtimeAddress + 0xf4) = iVar27;
      *(int *)((int)runtimeAddress + 0xf0) = iVar27;
      *(unsigned int *)((int)runtimeAddress + 0xf8) = 0;
      *(unsigned int *)((int)runtimeAddress + 0xec) = *(unsigned int *)((int)runtimeAddress + 0xe8);
      return 0;
    }
    break;
  case ENEMY_ECL_PLAY_ANM:
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    puVar5 = (unsigned int *)EnemyCreateManagedVm(
        *(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x30 + (int)uVar22 * 4),
        (int)uVar23, 6);
    uVar6 = *puVar5;
    goto dispatch_place_temporary_anm;
  case ENEMY_ECL_PLAY_ANM_ABSOLUTE:
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    EnemyCreateManagedVm(*(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x30 + (int)uVar22 * 4),(int)uVar23,6);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0;
    }
  case ENEMY_ECL_CREATE_ENEMY:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    pfVar15 = local_90.floatWords;
    for (iVar11 = 0x10; iVar11 != 0; iVar11 = iVar11 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
    if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x40000) == 0) {
      fVar9 = *(float *)((int)runtimeAddress + 0x2c);
      fVar19 = (ReadRawFloatArgument((1), (*(float *)(iVar27 + 0x10 + iVar26 * 4))));
      fVar10 = *(float *)((int)runtimeAddress + 0x30);
      local_90.spawnRequest.position.x = (float)(fVar19 + fVar9);
      fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
      fVar19 = fVar19 + fVar10;
    }
    else {
      fVar9 = *(float *)((int)runtimeAddress + 0x2c);
      fVar19 = (ReadRawFloatArgument((1), (*(float *)(iVar27 + 0x10 + iVar26 * 4))));
      fVar10 = *(float *)((int)runtimeAddress + 0x30);
      local_2b4 = (float)(fVar19 + fVar9);
      fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
      local_2ac = *(float *)((int)runtimeAddress + 0x34);
      local_2b0 = (float)(fVar19 + fVar10);
      EnemyPrepareProjection();
      projectionInput.x = local_2b4;
      projectionInput.y = local_2b0;
      projectionInput.z = local_2ac;
      D3DXVec3Project(&local_90.spawnRequest.position, &projectionInput,
                      &g_EnemyViewport, &g_EnemyProjectionMatrix,
                      &g_EnemyViewMatrix, &g_EnemyWorldMatrix);
      local_90.spawnRequest.position.x = local_90.spawnRequest.position.x - 16.0f;
      local_90.spawnRequest.position.z = 0.0;
      fVar19 = local_90.spawnRequest.position.y - 100.0f;
    }
dispatch_finish_spawn_position:
    local_90.spawnRequest.position.y = (float)fVar19;
    uVar22 = ReadRawIntArgument(3, *(int *)(iVar27 + 0x18 + iVar26 * 4));
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
    local_90.spawnRequest.itemDropType = uVar22;
    puVar5 = (unsigned int *)((int)runtimeAddress + 0xfc);
    pbVar16 = reinterpret_cast<unsigned char *>(&local_90.spawnRequest.eclVariables);
    for (iVar26 = 8; iVar26 != 0; iVar26 = iVar26 + -1) {
      *(unsigned int *)pbVar16 = *puVar5;
      puVar5 = puVar5 + 1;
      pbVar16 = pbVar16 + 4;
    }
dispatch_spawn_enemy:
    EnemySpawnFromEclInstruction(
        g_EnemyManager, reinterpret_cast<const void *>(iVar27 + 0x14),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0;
    }
    goto dispatch_create_enemy_absolute;
  case ENEMY_ECL_CREATE_ENEMY_MIRRORED_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) == 0) goto dispatch_create_enemy_mirrored;
    break;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0;
    }
    goto dispatch_create_enemy_absolute_mirrored;
  case ENEMY_ECL_PLAY_SELECTED_ANM:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    EnemyReleaseManagedVm(&managedVmIds[iVar26]);
    puVar5 = (unsigned int *)
             EnemyCreateManagedVm(*(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x30 + *(int *)((int)runtimeAddress + 0xe8) * 4),
                          *(int *)((int)runtimeAddress + 0xf4) + 5,5);
    *(unsigned int *)((int)runtimeAddress + 0xc0 + iVar26 * 4) = *puVar5;
    iVar27 = reinterpret_cast<int>(EnemyResolveManagedVm(managedVmIds[iVar26]));
dispatch_update_primary_anm_bounds:
    if (iVar26 == 0) {
      *(float *)((int)runtimeAddress + 0x13a4) = *(float *)(iVar27 + 0x50) * *(float *)(iVar27 + 0x40);
      *(float *)((int)runtimeAddress + 0x13a8) = *(float *)(iVar27 + 0x4c) * *(float *)(iVar27 + 0x3c);
    }
    if ((*(unsigned char *)((int)runtimeAddress + 0x1444) & 0x10) != 0) {
      EnemyHideManagedVm(&managedVmIds[iVar26]);
      return 0;
    }
    break;
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET:
dispatch_create_enemy_global:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    pfVar15 = local_90.floatWords;
    for (iVar11 = 0x10; iVar11 != 0; iVar11 = iVar11 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
    fVar19 = (ReadRawFloatArgument((1), (*(float *)(iVar27 + 0x10 + iVar26 * 4))));
    local_90.spawnRequest.position.x = (float)(fVar19 + g_EnemyGlobalPositionOffset.x);
    fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
    local_90.spawnRequest.position.y = (float)(fVar19 + g_EnemyGlobalPositionOffset.y);
    fVar19 = (ReadRawFloatArgument((3), (*(float *)(iVar27 + 0x18 + iVar26 * 4))));
    local_90.spawnRequest.position.z = (float)fVar19;
    uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(6, *(int *)(iVar27 + 0x24 + iVar26 * 4));
    local_90.spawnRequest.itemDropType = uVar22;
    puVar5 = (unsigned int *)((int)runtimeAddress + 0xfc);
    pbVar16 = reinterpret_cast<unsigned char *>(&local_90.spawnRequest.eclVariables);
    for (iVar26 = 8; iVar26 != 0; iVar26 = iVar26 + -1) {
      *(unsigned int *)pbVar16 = *puVar5;
      puVar5 = puVar5 + 1;
      pbVar16 = pbVar16 + 4;
    }
    local_90.spawnRequest.setFlag40000 = local_90.spawnRequest.setFlag40000 | 1;
    EnemySpawnFromEclInstruction(
        g_EnemyManager, reinterpret_cast<const void *>(iVar27 + 0x14),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) == 0) goto dispatch_create_enemy_global;
    break;
  case ENEMY_ECL_PLAY_ANM_HIGH:
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    puVar5 = (unsigned int *)EnemyCreateManagedVmRotated(
        *(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x30 + (int)uVar22 * 4),
        (int)uVar23, 6);
    uVar6 = *puVar5;
dispatch_place_temporary_anm:
    iVar26 = reinterpret_cast<int>(EnemyResolveManagedVm(uVar6));
    if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          EnemyGetAnmPosition(&worldMotion.position));
      *(unsigned int *)(iVar26 + 0x340) = *puVar5;
      *(unsigned int *)(iVar26 + 0x344) = puVar5[1];
      *(unsigned int *)(iVar26 + 0x348) = puVar5[2];
      return 0;
    }
    *(unsigned int *)(iVar26 + 0x340) = *(unsigned int *)((int)runtimeAddress + 0x2c);
    *(unsigned int *)(iVar26 + 0x344) = *(unsigned int *)((int)runtimeAddress + 0x30);
    *(unsigned int *)(iVar26 + 0x348) = *(unsigned int *)((int)runtimeAddress + 0x34);
    return 0;
  case ENEMY_ECL_PLAY_ANM_ROTATED:
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    puVar5 = (unsigned int *)EnemyCreateManagedVm(
        *(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x30 + (int)uVar22 * 4),
        (int)uVar23, 6);
    iVar26 = reinterpret_cast<int>(EnemyResolveManagedVm(*puVar5));
    if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          EnemyGetAnmPosition(&worldMotion.position));
      *(unsigned int *)(iVar26 + 0x340) = *puVar5;
      *(unsigned int *)(iVar26 + 0x344) = puVar5[1];
      *(unsigned int *)(iVar26 + 0x348) = puVar5[2];
    }
    else {
      *(unsigned int *)(iVar26 + 0x340) = *(unsigned int *)((int)runtimeAddress + 0x2c);
      *(unsigned int *)(iVar26 + 0x344) = *(unsigned int *)((int)runtimeAddress + 0x30);
      *(unsigned int *)(iVar26 + 0x348) = *(unsigned int *)((int)runtimeAddress + 0x34);
    }
    fVar19 = (ReadFloatArgument(2));
    *(float *)(iVar26 + 0x2c) = (float)fVar19;
    *(unsigned int *)(iVar26 + 0x35c) = *(unsigned int *)(iVar26 + 0x35c) | 4;
    return 0;
  // Movement and interpolation instructions.
  case ENEMY_ECL_SET_OFFSET_POSITION:
  case ENEMY_ECL_SET_BASE_POSITION:
    selectedMotion = &offsetMotion;
    if (opcode != 0x118) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    if ((float)0.0f < (float)fVar19) {
      selectedMotion->position.x = (float)fVar19;
    }
    if (0.0f < fVar21) {
      selectedMotion->position.y = (float)fVar21;
    }
    selectedMotion->flags &= 0xfffffffe;
    worldMotion.position.x = baseMotion.position.x + offsetMotion.position.x;
    worldMotion.position.y = baseMotion.position.y + offsetMotion.position.y;
    worldMotion.position.z = baseMotion.position.z + offsetMotion.position.z;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_POSITION:
  case ENEMY_ECL_INTERPOLATE_BASE_POSITION:
    selectedMotion = &offsetMotion;
    positionInterpolation = &positionInterpolations[0];
    if (opcode == 0x119) {
      positionInterpolation = &positionInterpolations[0];
    }
    else {
      selectedMotion = &baseMotion;
      positionInterpolation = &positionInterpolations[1];
    }
    positionInterpolationValues = reinterpret_cast<float *>(positionInterpolation);
    fVar19 = (ReadFloatArgument(2));
    fVar10 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    fVar9 = (float)fVar19;
    uVar22 = ReadIntArgument(0);
    positionInterpolation->duration = uVar22;
    positionInterpolationValues[6] = g_EnemyInterpolationOrigin.x;
    positionInterpolationValues[7] = g_EnemyInterpolationOrigin.y;
    positionInterpolationValues[8] = g_EnemyInterpolationOrigin.z;
    positionInterpolationValues[9] = g_EnemyInterpolationOrigin.x;
    positionInterpolationValues[10] = g_EnemyInterpolationOrigin.y;
    positionInterpolationValues[0xb] = g_EnemyInterpolationOrigin.z;
    uVar22 = ReadIntArgument(1);
    fVar2 = (float)0.0f;
    positionInterpolation->mode = uVar22;
    positionInterpolationValues[0] = selectedMotion->position.x;
    positionInterpolationValues[1] = selectedMotion->position.y;
    positionInterpolationValues[2] = selectedMotion->position.z;
    if (fVar9 <= fVar2) {
      fVar9 = selectedMotion->position.y;
    }
    if (fVar10 <= (float)0.0f) {
      fVar10 = selectedMotion->position.x;
    }
    positionInterpolationValues[3] = fVar10;
    positionInterpolationValues[4] = fVar9;
    local_2ac = 0.0;
    positionInterpolationValues[5] = 0.0;
    local_2b4 = fVar10;
    local_2b0 = fVar9;
    EnemyInitializePositionInterpolation(positionInterpolation);
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_SET_OFFSET_POLAR:
  case ENEMY_ECL_SET_BASE_POLAR:
    selectedMotion = &offsetMotion;
    if (opcode != 0x11c) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    local_2bc = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    if ((float)0.0f < local_2bc) {
      if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x800) != 0) {
        fVar21 = EnemyWrapAngle(local_2bc - 1.5707964f);
        fVar21 = EnemyWrapAngle((float)(1.5707964f - fVar21));
        local_2bc = (float)fVar21;
      }
      EnemySetMotionAngle(selectedMotion, local_2bc);
    }
    if ((float)0.0f < (float)fVar19) {
      selectedMotion->value18 = (float)fVar19;
    }
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_POLAR:
  case ENEMY_ECL_INTERPOLATE_BASE_POLAR:
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolations[0];
    if (opcode == 0x11d) {
      firstScalarInterpolation = &scalarInterpolations[0];
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolations[1];
    }
    firstScalarValues = reinterpret_cast<float *>(firstScalarInterpolation);
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    uVar22 = ReadIntArgument(1);
    fVar9 = (float)0.0f;
    firstScalarInterpolation->mode = uVar22;
    if (uVar22 == 7) {
      if (local_2c8 <= fVar9) {
        local_2c8 = 0.0;
      }
      if (local_2c4 <= (float)0.0f) {
        local_2c4 = 0.0;
      }
    }
    else {
      if (local_2c8 <= fVar9) {
        fVar19 = selectedMotion->value1C;
      }
      else {
        fVar19 = local_2c8;
        if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x800) != 0) {
          fVar19 = EnemyWrapAngle((float)(fVar19 - 1.5707964f));
          fVar19 = EnemyWrapAngle((float)(1.5707964f - fVar19));
        }
      }
      local_2c8 = (float)fVar19;
      if (local_2c4 <= (float)0.0f) {
        local_2c4 = selectedMotion->value18;
      }
    }
    fVar9 = selectedMotion->value1C;
    local_2a4 = selectedMotion->value18;
    local_2a8 = fVar9;
    if (3.1415927f <= EnemyAbsoluteFloat(fVar9 - local_2c8)) {
      if (local_2c8 <= fVar9) {
        local_2c8 = local_2c8 + -3.1415927f;
      }
      else {
        local_2a8 = fVar9 + -3.1415927f;
      }
    }
    uVar22 = ReadIntArgument(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarValues[4] = g_EnemyInterpolationBasis.x;
    firstScalarValues[5] = g_EnemyInterpolationBasis.y;
    firstScalarValues[6] = g_EnemyInterpolationBasis.x;
    fVar9 = local_2a8;
    fVar10 = local_2a4;
    goto dispatch_initialize_polar_interpolation;
  case ENEMY_ECL_SET_OFFSET_CIRCLE:
  case ENEMY_ECL_SET_BASE_CIRCLE:
    selectedMotion = &offsetMotion;
    if (opcode != 0x120) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    fVar20 = (ReadFloatArgument(2));
    local_2a8 = (float)fVar20;
    fVar20 = (ReadFloatArgument(3));
    if ((selectedMotion->flags & 1) == 0) {
      selectedMotion->velocity = selectedMotion->position;
    }
    if ((float)0.0f < (float)fVar19) {
      EnemySetMotionAngle(selectedMotion, (float)fVar19);
    }
    if ((float)0.0f < (float)fVar21) {
      selectedMotion->value18 = (float)fVar21;
    }
    if ((float)0.0f < local_2a8) {
      selectedMotion->value20 = local_2a8;
    }
    if ((float)0.0f < (float)fVar20) {
      selectedMotion->value24 = (float)fVar20;
    }
    selectedMotion->flags |= 1;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_CIRCLE:
  case ENEMY_ECL_INTERPOLATE_BASE_CIRCLE:
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolations[0];
    secondScalarInterpolation = &scalarInterpolations[2];
    if (opcode == 0x121) {
      secondScalarInterpolation = &scalarInterpolations[2];
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolations[1];
      secondScalarInterpolation = &scalarInterpolations[3];
    }
    fVar19 = (ReadFloatArgument(2));
    local_2a8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    fVar9 = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    fVar10 = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    if (fVar9 <= (float)0.0f) {
      fVar9 = selectedMotion->value18;
    }
    if (local_2a8 <= (float)0.0f) {
      local_2a8 = selectedMotion->value1C;
    }
    local_2a4 = selectedMotion->value18;
    if (fVar19 <= 0.0f) {
      fVar19 = selectedMotion->value24;
    }
    if (fVar10 <= (float)0.0f) {
      fVar10 = selectedMotion->value20;
    }
    local_2b0 = (float)fVar19;
    local_2b4 = fVar10;
    local_29c = local_2a8;
    local_2a8 = selectedMotion->value1C;
    local_298 = fVar9;
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    firstScalarValues = reinterpret_cast<float *>(firstScalarInterpolation);
    firstScalarInterpolation->duration = (int)uVar22;
    firstScalarValues[4] = g_EnemyInterpolationBasis.x;
    firstScalarValues[5] = g_EnemyInterpolationBasis.y;
    firstScalarValues[6] = g_EnemyInterpolationBasis.x;
    fVar9 = g_EnemyInterpolationBasis.y;
    firstScalarValues[0] = local_2a8;
    firstScalarValues[7] = fVar9;
    firstScalarValues[3] = local_298;
    firstScalarInterpolation->mode = (int)uVar23;
    firstScalarValues[1] = local_2a4;
    firstScalarValues[2] = local_29c;
    EnemyInitializeScalarInterpolation(firstScalarInterpolation);
    secondScalarValues = reinterpret_cast<float *>(secondScalarInterpolation);
    secondScalarInterpolation->duration = (int)uVar22;
    secondScalarValues[4] = g_EnemyInterpolationBasis.x;
    secondScalarValues[5] = g_EnemyInterpolationBasis.y;
    secondScalarValues[6] = g_EnemyInterpolationBasis.x;
    fVar9 = g_EnemyInterpolationBasis.y;
    secondScalarValues[0] = selectedMotion->value20;
    secondScalarValues[7] = fVar9;
    secondScalarValues[3] = local_2b0;
    secondScalarInterpolation->mode = (int)uVar23;
    secondScalarValues[1] = selectedMotion->value24;
    secondScalarValues[2] = local_2b4;
    EnemyInitializeScalarInterpolation(secondScalarInterpolation);
    selectedMotion->velocity = selectedMotion->position;
    selectedMotion->flags |= 1;
    return 0;
  case ENEMY_ECL_MOVE_RANDOM:
  case ENEMY_ECL_MOVE_RANDOM_BASE:
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolations[0];
    if (opcode == 0x124) {
      firstScalarInterpolation = &scalarInterpolations[0];
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolations[1];
    }
    firstScalarValues = reinterpret_cast<float *>(firstScalarInterpolation);
    fVar9 = *(float *)((int)runtimeAddress + 0x13b4) * 0.25f;
    if (*(float *)((int)runtimeAddress + 0x13ac) - fVar9 <= *(float *)((int)runtimeAddress + 0x2c)) {
      if (*(float *)((int)runtimeAddress + 0x2c) <= fVar9 + *(float *)((int)runtimeAddress + 0x13ac)) {
        if (*(float *)((int)runtimeAddress + 0x2c) < *(float *)(reinterpret_cast<int>(g_Player) + 0x3c0)) {
          fVar19 = EnemyRandomAngle();
          fVar19 = fVar19 * 1.5707964f;
          goto dispatch_random_angle_ready;
        }
        fVar19 = EnemyRandomAngle();
        fVar19 = fVar19 * 3.1415927f * 2.0f;
      }
      else {
        fVar19 = EnemyRandomAngle();
        fVar19 = fVar19 * 3.1415927f * 0.33333334f;
      }
      fVar19 = EnemyWrapAngle((float)(fVar19 + 3.1415927f));
    }
    else {
      fVar19 = EnemyRandomAngle();
      fVar19 = fVar19 * 1.0471976f;
    }
dispatch_random_angle_ready:
    local_2bc = (float)fVar19;
    fVar9 = *(float *)((int)runtimeAddress + 0x13b8) * 0.25f;
    if (*(float *)((int)runtimeAddress + 0x13b0) - fVar9 <= *(float *)((int)runtimeAddress + 0x30)) {
      if (fVar9 + *(float *)((int)runtimeAddress + 0x13b0) < *(float *)((int)runtimeAddress + 0x30)) {
        local_2bc = -EnemyAbsoluteFloat(local_2bc);
      }
    }
    else {
      local_2bc = EnemyAbsoluteFloat(local_2bc);
    }
    uVar22 = ReadIntArgument(1);
    firstScalarInterpolation->mode = uVar22;
    local_2c4 = 0.0;
    local_2c8 = local_2bc;
    local_2b4 = local_2bc;
    fVar19 = (ReadFloatArgument(2));
    local_2b0 = (float)fVar19;
    uVar22 = ReadIntArgument(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarValues[4] = g_EnemyInterpolationBasis.x;
    firstScalarValues[5] = g_EnemyInterpolationBasis.y;
    firstScalarValues[6] = g_EnemyInterpolationBasis.x;
    fVar9 = local_2b4;
    fVar10 = local_2b0;
dispatch_initialize_polar_interpolation:
    firstScalarValues[7] = g_EnemyInterpolationBasis.y;
    firstScalarValues[0] = fVar9;
    firstScalarValues[1] = fVar10;
    firstScalarValues[2] = local_2c8;
    firstScalarValues[3] = local_2c4;
    EnemyInitializeScalarInterpolation(firstScalarInterpolation);
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_COPY_PLAYER_TO_OFFSET:
    goto dispatch_copy_player_to_motion;
  case ENEMY_ECL_COPY_PLAYER_TO_BASE:
    iVar26 = *(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10);
    *(unsigned int *)((int)runtimeAddress + 0x84) = *(unsigned int *)(iVar26 + 0x1068);
    *(unsigned int *)((int)runtimeAddress + 0x88) = *(unsigned int *)(iVar26 + 0x106c);
    *(unsigned int *)((int)runtimeAddress + 0x8c) = *(unsigned int *)(iVar26 + 0x1070);
    return 0;
  case ENEMY_ECL_ADD_OFFSET_POSITION:
  case ENEMY_ECL_ADD_BASE_POSITION:
    pfVar15 = (float *)((int)runtimeAddress + 0x58);
    if (opcode != 0x128) {
      pfVar15 = (float *)((int)runtimeAddress + 0x84);
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    local_2a8 = (float)fVar21;
    fVar21 = (ReadFloatArgument(2));
    *pfVar15 = (float)fVar19 + *pfVar15;
    pfVar15[1] = local_2a8 + pfVar15[1];
    pfVar15[2] = (float)(fVar21 + pfVar15[2]);
    return 0;
  case ENEMY_ECL_ADD_OFFSET_VELOCITY:
  case ENEMY_ECL_ADD_BASE_VELOCITY:
    iVar26 = (int)runtimeAddress + 0x58;
    if (opcode != 0x12a) {
      iVar26 = (int)runtimeAddress + 0x84;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    if ((float)0.0f < (float)fVar19) {
      *(float *)(iVar26 + 0xc) = (float)fVar19;
    }
    if (0.0f < fVar21) {
      *(float *)(iVar26 + 0x10) = (float)fVar21;
    }
    goto dispatch_copy_player_to_motion;
  // Enemy state, lifecycle and difficulty instructions.
  case ENEMY_ECL_SET_DAMAGE_HITBOX:
    fVar19 = (ReadFloatArgument(0));
    *(float *)((int)runtimeAddress + 0xb0) = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    *(float *)((int)runtimeAddress + 0xb4) = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_PLAYER_HITBOX:
    fVar19 = (ReadFloatArgument(0));
    *(float *)((int)runtimeAddress + 0xb8) = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    *(float *)((int)runtimeAddress + 0xbc) = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_FLAGS:
    uVar22 = ReadIntArgument(0);
    uVar8 = *(unsigned int *)((int)runtimeAddress + 0x1444) | (unsigned int)uVar22;
    *(unsigned int *)((int)runtimeAddress + 0x1444) = uVar8;
    if ((uVar8 & 0x10) != 0) {
      for (iVar26 = 0; iVar26 < 10; ++iVar26)
        EnemyHideManagedVm(&managedVmIds[iVar26]);
      return 0;
    }
    break;
  case ENEMY_ECL_CLEAR_FLAGS:
    uVar22 = ReadIntArgument(0);
    uVar8 = *(unsigned int *)((int)runtimeAddress + 0x1444) & ~(unsigned int)uVar22;
    *(unsigned int *)((int)runtimeAddress + 0x1444) = uVar8;
    if ((uVar8 & 0x10) == 0) {
      for (iVar26 = 0; iVar26 < 10; ++iVar26)
        EnemyShowManagedVm(&managedVmIds[iVar26]);
      return 0;
    }
    break;
  case ENEMY_ECL_SET_MOVEMENT_BOUNDS:
    *(unsigned int *)((int)runtimeAddress + 0x1444) = *(unsigned int *)((int)runtimeAddress + 0x1444) | 0x200;
    fVar19 = (ReadFloatArgument(0));
    *(float *)((int)runtimeAddress + 0x13ac) = (float)fVar19;
    fVar19 = (ReadFloatArgument(1));
    *(float *)((int)runtimeAddress + 0x13b0) = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    *(float *)((int)runtimeAddress + 0x13b4) = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    *(float *)((int)runtimeAddress + 0x13b8) = (float)fVar19;
    return 0;
  case ENEMY_ECL_CLEAR_MOVEMENT_BOUNDS:
    *(unsigned int *)((int)runtimeAddress + 0x1444) = *(unsigned int *)((int)runtimeAddress + 0x1444) & 0xfffffdff;
    return 0;
  case ENEMY_ECL_CLEAR_ITEM_DROPS:
    puVar5 = (unsigned int *)((int)runtimeAddress + 0x13d0);
    for (iVar26 = 0xc; iVar26 != 0; iVar26 = iVar26 + -1) {
      *puVar5 = 0;
      puVar5 = puVar5 + 1;
    }
    return 0;
  case ENEMY_ECL_ADD_ITEM_DROP:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    if ((char)((unsigned int)*(unsigned int *)((int)runtimeAddress + 0x1444) >> 8) < '\0') {
      if (iVar26 == 1) {
        iVar26 = 10;
      }
      else if (iVar26 == 4) {
        iVar26 = 0xb;
      }
    }
    uVar22 = ReadIntArgument(1);
    *(int *)((int)runtimeAddress + 0x13cc + iVar26 * 4) = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_DROP_AREA:
    fVar19 = (ReadFloatArgument(1));
    fVar21 = (ReadFloatArgument(0));
    *(float *)((int)runtimeAddress + 0x1400) = (float)fVar21;
    *(float *)((int)runtimeAddress + 0x1404) = (float)fVar19;
    return 0;
  case ENEMY_ECL_DROP_ITEMS:
    EnemyDropItemCounts(&worldMotion.position, &itemDropType);
    return 0;
  case ENEMY_ECL_SET_MAIN_DROP:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    if ((char)((unsigned int)*(unsigned int *)((int)runtimeAddress + 0x1444) >> 8) < '\0') {
      if (iVar26 == 1) {
        *(unsigned int *)((int)runtimeAddress + 0x13cc) = 10;
        return 0;
      }
      if (iVar26 == 4) {
        iVar26 = 0xb;
      }
    }
    *(int *)((int)runtimeAddress + 0x13cc) = iVar26;
    return 0;
  case ENEMY_ECL_SET_LIFE:
    uVar22 = ReadIntArgument(0);
    *(int *)((int)runtimeAddress + 0x13c0) = (int)uVar22;
    *(int *)((int)runtimeAddress + 0x13c4) = (int)uVar22;
    iVar26 = reinterpret_cast<int>(g_EnemyVisualState);
    *(unsigned int *)(reinterpret_cast<int>(g_EnemyVisualState) + 0x9e94) = 0;
    *(unsigned int *)(iVar26 + 0x9e98) = 0;
    *(unsigned int *)(iVar26 + 0x9e9c) = 0;
    *(unsigned int *)(iVar26 + 0x9ea0) = 0;
    *(unsigned int *)(iVar26 + 0x9ea4) = 0;
    *(unsigned int *)(iVar26 + 0x9ea8) = 0;
    *(unsigned int *)(iVar26 + 0x9eac) = 0;
    *(unsigned int *)(iVar26 + 0x9eb0) = 0;
    *(unsigned int *)((int)runtimeAddress + 0x13c8) = *(unsigned int *)((int)runtimeAddress + 0x13c0);
    return 0;
  case ENEMY_ECL_SET_BOSS_SLOT:
    uVar22 = ReadIntArgument(0);
    iVar26 = reinterpret_cast<int>(g_EnemyManager);
    iVar27 = (int)uVar22;
    if (iVar27 < 0) {
      if ((char)((unsigned int)*(unsigned int *)((int)runtimeAddress + 0x1444) >> 8) < '\0') {
        *(unsigned int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10 + *(int *)((int)runtimeAddress + 0x1450) * 4) = 0;
      }
      *(unsigned int *)((int)runtimeAddress + 0x1444) = *(unsigned int *)((int)runtimeAddress + 0x1444) & 0xffff7fff;
      return 0;
    }
    *(unsigned int *)((int)runtimeAddress + 0x1444) = *(unsigned int *)((int)runtimeAddress + 0x1444) | 0x8000;
    *(unsigned int *)(iVar26 + 0x10 + iVar27 * 4) = *(unsigned int *)((int)runtimeAddress + 0x14d8);
    *(int *)((int)runtimeAddress + 0x1450) = iVar27;
    return 0;
  case ENEMY_ECL_RESET_UPDATE_TIMER:
    EnemySetTimerCurrent(&updateTimer, 0);
    return 0;
  case ENEMY_ECL_SET_INTERRUPT:
    uVar22 = ReadIntArgument(2);
    uVar23 = ReadIntArgument(1);
    uVar24 = ReadIntArgument(0);
    EnemyConfigureInterrupt((int)uVar23,(int)uVar24,(int)uVar22);
    return 0;
  case ENEMY_ECL_SET_INVULNERABILITY:
    uVar22 = ReadIntArgument(0);
    EnemySetTimerCurrent(&damageReductionTimer, (int)uVar22);
    return 0;
  case ENEMY_ECL_PLAY_SOUND:
    EnemyPlaySound(ReadIntArgument(0));
    return 0;
  case ENEMY_ECL_SET_SCREEN_SHAKE:
    uVar22 = ReadIntArgument(2);
    uVar6 = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(1);
    uVar4 = (unsigned int)uVar22;
    uVar28 = 0;
    uVar22 = ReadIntArgument(0);
    EnemySetScreenShake(1,(int)uVar22,uVar4,uVar6,uVar28);
    return 0;
  case ENEMY_ECL_READ_DIALOG:
    ReadIntArgument(0);
    EnemyReadDialog();
    EnemyCancelAllBullets(0);
    EnemyApplyBulletClear();
    EnemyKillAll(g_EnemyManager);
    return 0;
  case ENEMY_ECL_WAIT_DIALOG:
    if ((*(int *)(reinterpret_cast<int>(g_EnemyVisualState) + 0x9eb8) != 0) &&
       (*(int *)(*(int *)(reinterpret_cast<int>(g_EnemyVisualState) + 0x9eb8) + 0x78) == 0)) {
      return 0xffffffff;
    }
    break;
  case ENEMY_ECL_WAIT_BOSS_CLEAR:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0xffffffff;
    }
    break;
  case ENEMY_ECL_SET_TIMEOUT:
    uVar22 = ReadIntArgument(0);
    *(int *)(((int)uVar22 + 0x24a) * 0x10 + *(int *)((int)runtimeAddress + 0x14d8)) = iVar27 + 0x18;
    return 0;
  case ENEMY_ECL_START_SPELL:
  case ENEMY_ECL_START_SPELL_2:
  case ENEMY_ECL_START_SPELL_DIFFICULTY:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2:
    local_2a8 = *(float *)(iVar27 + 0x1c);
    iVar17 = 0;
    bVar3 = 0x77;
    local_29d = '\a';
    iVar11 = iVar27 + 0x20;
    if (0 < (int)local_2a8) {
      do {
        local_90.bytes[iVar17] =
            reinterpret_cast<const unsigned char *>(iVar27 + 0x20)[iVar17] ^ bVar3;
        bVar3 = bVar3 + local_29d;
        local_29d = local_29d + '\x10';
        iVar17 = iVar17 + 1;
        iVar11 = (int)local_2a8;
      } while (iVar17 < (int)local_2a8);
    }
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    opcode = *(short *)(iVar27 + 4);
    if (opcode == 0x165) {
      iVar26 = iVar26 + g_EnemyDifficulty;
    }
    else if (opcode == 0x166) {
      iVar26 = iVar26 + -1 + g_EnemyDifficulty;
      iVar27 = g_EnemyDifficulty;
    }
    else {
      if (opcode == 0x167) {
        iVar26 = iVar26 + -2 + g_EnemyDifficulty;
      }
    }
    ReadIntArgument(2);
    uVar22 = ReadIntArgument(1);
    EnemyBeginSpell(reinterpret_cast<int>(g_EnemyGameState),iVar26,reinterpret_cast<char *>(local_90.bytes),(int)uVar22);
    return 0;
  case ENEMY_ECL_END_SPELL:
    EnemyEndSpell(g_EnemyGameState);
    return 0;
  case ENEMY_ECL_SET_CHAPTER:
    uVar22 = ReadIntArgument(0);
    EnemySetChapter((int)uVar22);
    return 0;
  case ENEMY_ECL_KILL_ALL_ENEMIES:
    EnemyKillAll(g_EnemyManager);
    return 0;
  case ENEMY_ECL_SET_BULLET_PROTECTION_RANGE:
    fVar19 = (ReadFloatArgument(0));
    *(float *)((int)runtimeAddress + 0x1454) = (float)(fVar19 * fVar19);
    break;
  case ENEMY_ECL_SET_LIFE_MARKER:
    uVar22 = ReadIntArgument(2);
    fVar19 = (ReadFloatArgument(1));
    local_2a8 = (float)fVar19;
    iVar26 = *(int *)((int)runtimeAddress + 0x13c4);
    uVar23 = ReadIntArgument(0);
    iVar27 = reinterpret_cast<int>(g_EnemyVisualState);
    *(int *)(reinterpret_cast<int>(g_EnemyVisualState) + 0x9e98 + (int)uVar23 * 8) = (int)uVar22;
    *(float *)(iVar27 + 0x9e94 + (int)uVar23 * 8) = local_2a8 / (float)iVar26;
    return 0;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_3:
    if (g_EnemyRank < 0x200) {
      fVar9 = 0.0;
      if (g_EnemyRank < -0x1ff) goto dispatch_store_selected_float;
    }
    else {
      fVar9 = EnemyEncodeOperandIndex(2);
    }
    goto dispatch_read_selected_float;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_5:
    if (599 < g_EnemyRank) {
dispatch_rank_band_3:
      fVar9 = EnemyEncodeOperandIndex(4);
      goto dispatch_store_selected_float;
    }
    if (199 < g_EnemyRank) goto dispatch_rank_band_2;
    if (g_EnemyRank < -200) {
      if (g_EnemyRank < -400) {
        fVar9 = 0.0;
        goto dispatch_store_selected_float;
      }
      goto dispatch_difficulty_float_1;
    }
dispatch_rank_band_1:
    fVar9 = EnemyEncodeOperandIndex(2);
dispatch_store_selected_float:
    fVar19 = (ReadFloatArgument(EnemyDecodeOperandIndex(fVar9)));
    pfVar15 = (float *)((int)ResolveFloatArgument(0));
    *pfVar15 = (float)fVar19;
    return 0;
  case ENEMY_ECL_INTERPOLATE_FLOAT_BY_RANK:
    fVar19 = (ReadFloatArgument(1));
    fVar21 = (ReadFloatArgument(2));
    local_2a8 = (float)fVar21;
    pfVar15 = (float *)((int)ResolveFloatArgument(0));
    *pfVar15 = (local_2a8 - (float)fVar19) * ((float)g_EnemyRank + 1024.0f) * 0.00048828125f +
               (float)fVar19;
    return 0;
  case ENEMY_ECL_SELECT_INT_BY_RANK_3:
    if (0x1ff < g_EnemyRank) {
dispatch_rank_int_high:
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(2);
      *puVar5 = (int)uVar22;
      return 0;
    }
    goto dispatch_rank_int_low;
  case ENEMY_ECL_SELECT_INT_BY_RANK_5:
    if (599 < g_EnemyRank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(4);
      *puVar5 = (int)uVar22;
      return 0;
    }
    if (199 < g_EnemyRank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(3);
      *puVar5 = (int)uVar22;
      return 0;
    }
    if (-0xc9 < g_EnemyRank) goto dispatch_rank_int_high;
    if (-0x191 < g_EnemyRank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(1);
      *puVar5 = (int)uVar22;
      return 0;
    }
dispatch_rank_int_low:
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    uVar22 = ReadIntArgument(0);
    *puVar5 = (int)uVar22;
    return 0;
  case ENEMY_ECL_INTERPOLATE_INT_BY_RANK:
    uVar22 = ReadIntArgument(1);
    uVar23 = ReadIntArgument(2);
    piVar7 = (int *)((int)ResolveIntArgument(0));
    iVar26 = ((int)uVar23 - (int)uVar22) * (g_EnemyRank + 0x400);
    *piVar7 = ((int)(iVar26 + (iVar26 >> 0x1f & 0x7ffU)) >> 0xb) + (int)uVar22;
    return 0;
  case ENEMY_ECL_SELECT_INT_BY_DIFFICULTY:
    switch(g_EnemyDifficulty) {
    case 0:
      iVar27 = 1;
      break;
    case 1:
      iVar27 = 2;
      break;
    case 2:
      iVar27 = 3;
      break;
    case 3:
    case 4:
      iVar27 = 4;
      break;
    default:
      goto dispatch_complete;
    }
    uVar22 = ReadIntArgument(iVar27);
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    *puVar5 = (int)uVar22;
    return 0;
  case ENEMY_ECL_SELECT_FLOAT_BY_DIFFICULTY:
    switch(g_EnemyDifficulty) {
    case 0:
dispatch_difficulty_float_1:
      fVar9 = EnemyEncodeOperandIndex(1);
      break;
    case 1:
      goto dispatch_rank_band_1;
    case 2:
dispatch_rank_band_2:
      fVar9 = EnemyEncodeOperandIndex(3);
      break;
    case 3:
      goto dispatch_rank_band_3;
    case 4:
      fVar9 = EnemyEncodeOperandIndex(4);
      break;
    default:
      goto dispatch_complete;
    }
dispatch_read_selected_float:
    fVar19 = (ReadFloatArgument(EnemyDecodeOperandIndex(fVar9)));
dispatch_store_float_result:
    pfVar15 = (float *)((int)ResolveFloatArgument(0));
    *pfVar15 = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_SPELL_TIMEOUT:
    uVar22 = ReadIntArgument(0);
    *(int *)(reinterpret_cast<int>(g_EnemyVisualState) + 0x9e90) = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_PLAYER_COLLISION_TIMER:
    uVar22 = ReadIntArgument(0);
    EnemySetTimerCurrent(&playerCollisionTimer, (int)uVar22);
    return 0;
  case ENEMY_ECL_CANCEL_LASERS:
    *(unsigned int *)(reinterpret_cast<int>(g_EnemyGameState) + 0x378c) = *(unsigned int *)(reinterpret_cast<int>(g_EnemyGameState) + 0x378c) | 8;
    return 0;
  case ENEMY_ECL_ENABLE_BOMB_SHIELD:
    EnemyEnableBombShield(g_EnemyGameState);
    return 0;
  case ENEMY_ECL_SET_GAME_SPEED_FLAG:
    uVar22 = ReadIntArgument(0);
    *(unsigned int *)((int)runtimeAddress + 0x1444) =
         *(unsigned int *)((int)runtimeAddress + 0x1444) ^
         ((int)uVar22 << 0x13 ^ *(unsigned int *)((int)runtimeAddress + 0x1444)) & 0x80000;
    return 0;
  case ENEMY_ECL_WAIT_FOR_EFFECT:
    EnemyWaitForEffect();
    return 0;
  case ENEMY_ECL_SET_ANM_MODE:
    uVar22 = ReadIntArgument(0);
    *(unsigned int *)((int)runtimeAddress + 0x1444) =
         *(unsigned int *)((int)runtimeAddress + 0x1444) ^
         ((int)uVar22 << 0x14 ^ *(unsigned int *)((int)runtimeAddress + 0x1444)) & 0x100000;
    uVar22 = ReadIntArgument(1);
    *(int *)((int)runtimeAddress + 0x1448) = (int)uVar22;
    *(unsigned int *)((int)runtimeAddress + 0x1444) = *(unsigned int *)((int)runtimeAddress + 0x1444) & 0xffdfffff;
    *(unsigned int *)((int)runtimeAddress + 0x144c) = *(unsigned int *)((int)runtimeAddress + 0xf4);
    return 0;
  case ENEMY_ECL_SET_TIMER_SCALE:
    fVar19 = (ReadFloatArgument(0));
    g_PlayerTimerScale = (float)fVar19;
    return 0;
  case ENEMY_ECL_SUBTRACT_DIFFICULTY_VALUE:
    if (g_EnemyDifficulty == 0) {
      uVar22 = ReadIntArgument(0);
      fVar9 = (float)(int)uVar22;
    }
    else if (g_EnemyDifficulty == 1) {
      uVar22 = ReadIntArgument(1);
      fVar9 = (float)(int)uVar22;
    }
    else if (g_EnemyDifficulty == 2) {
      uVar22 = ReadIntArgument(2);
      fVar9 = (float)(int)uVar22;
    }
    else {
      uVar22 = ReadIntArgument(3);
      fVar9 = (float)(int)uVar22;
    }
    pfVar15 = *(float **)(*(int *)((int)runtimeAddress + 0x14d8) + 4);
    *pfVar15 = *pfVar15 - fVar9;
    break;
  // Bullet-pattern and laser instructions.
  case ENEMY_ECL_INITIALIZE_BULLET_PATTERN:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    iVar11 = iVar26 * 0x210 + (int)runtimeAddress;
    puVar5 = (unsigned int *)(iVar11 + 0x2c4);
    for (iVar27 = 0x84; iVar27 != 0; iVar27 = iVar27 + -1) {
      *puVar5 = 0;
      puVar5 = puVar5 + 1;
    }
    *(unsigned short *)(iVar11 + 0x4b8) = 1;
    *(unsigned short *)(iVar11 + 0x4ba) = 1;
    *(unsigned int *)(iVar11 + 0x2d4) = 0;
    *(unsigned int *)(iVar11 + 0x2dc) = 0x40000000;
    *(unsigned int *)(iVar11 + 0x4c4) = 7;
    *(unsigned int *)(iVar11 + 0x4c8) = 0x18;
    *(unsigned int *)(iVar11 + 0x4c0) = 0x203;
    *(unsigned int *)((int)runtimeAddress + (iVar26 * 3 + 0x4d1) * 4) = 0;
    *(unsigned int *)((int)runtimeAddress + 0x1348 + iVar26 * 0xc) = 0;
    return 0;
  case ENEMY_ECL_FIRE_BULLET_PATTERN:
    uVar22 = ReadIntArgument(0);
    iVar27 = (int)uVar22 * 3 + 0x4d1;
    local_2b4 = *(float *)((int)runtimeAddress + iVar27 * 4) + *(float *)((int)runtimeAddress + 0x2c);
    local_2b0 = *(float *)((int)runtimeAddress + 4 + iVar27 * 4) + *(float *)((int)runtimeAddress + 0x30);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    local_2ac = *(float *)((int)runtimeAddress + iVar27 * 4 + 8) + *(float *)((int)runtimeAddress + 0x34);
    *(float *)(iVar26 + 0x2c8) = local_2b4;
    *(float *)(iVar26 + 0x2cc) = local_2b0;
    fVar9 = 0.0f;
    *(float *)(iVar26 + 0x2d0) = local_2ac;
    if ((*(float *)((int)runtimeAddress + 0x1454) == fVar9) ||
       (fVar9 = *(float *)(iVar26 + 0x2cc) - *(float *)(reinterpret_cast<int>(g_Player) + 0x3c4),
       fVar10 = *(float *)(iVar26 + 0x2c8) - *(float *)(reinterpret_cast<int>(g_Player) + 0x3c0),
       fVar9 = fVar9 * fVar9 + fVar10 * fVar10,
       *(float *)((int)runtimeAddress + 0x1454) < fVar9 != (*(float *)((int)runtimeAddress + 0x1454) == fVar9))) {
      EnemyFireBulletPattern(reinterpret_cast<int>(g_EnemyBulletManager),iVar26,reinterpret_cast<int>(g_EnemyBulletManager));
      return 0;
    }
    break;
  case ENEMY_ECL_SET_BULLET_SPRITES:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    uVar22 = ReadIntArgument(1);
    *(short *)(iVar26 + 0x2c4) = (short)uVar22;
    uVar22 = ReadIntArgument(2);
    *(short *)(iVar26 + 0x2c6) = (short)uVar22;
    return 0;
  case ENEMY_ECL_SET_BULLET_OFFSET:
    uVar22 = ReadIntArgument(0);
    fVar19 = (ReadFloatArgument(1));
    *(float *)((int)runtimeAddress + ((int)uVar22 * 3 + 0x4d1) * 4) = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    *(float *)((int)runtimeAddress + 0x1348 + (int)uVar22 * 0xc) = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_BULLET_ANGLE:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    fVar19 = (ReadFloatArgument(1));
    *(float *)(iVar26 + 0x2d4) = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    *(float *)(iVar26 + 0x2d8) = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_BULLET_SPEED:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    goto dispatch_bullet_speed_lowest;
  case ENEMY_ECL_SET_BULLET_COUNT:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    uVar22 = ReadIntArgument(1);
    *(short *)(iVar26 + 0x4b8) = (short)uVar22;
    uVar22 = ReadIntArgument(2);
    *(short *)(iVar26 + 0x4ba) = (short)uVar22;
    return 0;
  case ENEMY_ECL_SET_BULLET_AIM_MODE:
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    *(short *)((int)uVar22 * 0x210 + 0x4bc + (int)runtimeAddress) = (short)uVar23;
    return 0;
  case ENEMY_ECL_SET_BULLET_SOUND:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    uVar22 = ReadIntArgument(1);
    *(int *)(iVar26 + 0x4c4) = (int)uVar22;
    uVar22 = ReadIntArgument(2);
    *(int *)(iVar26 + 0x4c8) = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_BULLET_EXTRA:
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    iVar26 = (int)uVar22 * 0x16 + (int)uVar23;
    iVar27 = (int)runtimeAddress + iVar26 * 0x18;
    uVar22 = ReadIntArgument(2);
    *(int *)(iVar27 + 0x2f8) = (int)uVar22;
    uVar22 = ReadIntArgument(3);
    *(int *)(iVar27 + 0x2f4) = (int)uVar22;
    uVar22 = ReadIntArgument(4);
    *(int *)(iVar27 + 0x2ec) = (int)uVar22;
    uVar22 = ReadIntArgument(5);
    *(int *)(iVar27 + 0x2f0) = (int)uVar22;
    fVar19 = (ReadFloatArgument(6));
    *(float *)(iVar27 + 0x2e4) = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    *(float *)((int)runtimeAddress + (iVar26 * 3 + 0x5d) * 8) = (float)fVar19;
    return 0;
  case ENEMY_ECL_CANCEL_ALL_BULLETS:
    EnemyCancelAllBullets(1);
    EnemyApplyBulletClear();
    return 0;
  case ENEMY_ECL_COPY_BULLET_PATTERN:
    uVar22 = ReadIntArgument(1);
    uVar23 = ReadIntArgument(0);
    puVar5 = (unsigned int *)((int)uVar22 * 0x210 + 0x2c4 + (int)runtimeAddress);
    puVar14 = (unsigned int *)((int)uVar23 * 0x210 + 0x2c4 + (int)runtimeAddress);
    for (iVar26 = 0x84; iVar26 != 0; iVar26 = iVar26 + -1) {
      *puVar14 = *puVar5;
      puVar5 = puVar5 + 1;
      puVar14 = puVar14 + 1;
    }
    return 0;
  case ENEMY_ECL_FIRE_LASER_A:
  case ENEMY_ECL_FIRE_LASER_A_WITH_PATTERN:
    pfVar15 = local_288.floats;
    for (iVar27 = 0x77; iVar27 != 0; iVar27 = iVar27 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    if (opcode == 0x1af) {
      puVar5 = (unsigned int *)((int)runtimeAddress + 0x2e4);
      puVar14 = local_288.words + 11;
      for (iVar27 = 0x6c; iVar27 != 0; iVar27 = iVar27 + -1) {
        *puVar14 = *puVar5;
        puVar5 = puVar5 + 1;
        puVar14 = puVar14 + 1;
      }
    }
    local_2b4 = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    local_2b0 = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    local_2ac = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    local_288.floats[0] = local_2b4;
    local_288.floats[1] = local_2b0;
    local_288.floats[2] = local_2ac;
    uVar22 = ReadIntArgument(0);
    local_288.halves[18] = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(1);
    local_288.halves[19] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(2));
    local_288.floats[3] = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_288.floats[8] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.floats[5] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.floats[4] = (float)fVar19;
    fVar19 = (ReadFloatArgument(6));
    local_288.floats[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    local_288.floats[7] = (float)fVar19;
    local_288.words[10] |= 1;
    EnemyFireLaser(g_EnemyBulletManager, &local_288, 0);
    return 0;
  case ENEMY_ECL_FIRE_STRAIGHT_LASER:
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_WITH_PATTERN:
    pfVar15 = local_288.floats;
    for (iVar26 = 0x7e; iVar26 != 0; iVar26 = iVar26 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    local_288.floats[11] = 8.0f;
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    if (*(short *)(iVar27 + 4) == 0x1b0) {
      puVar14 = (unsigned int *)((int)runtimeAddress + 0x2e4);
      puVar13 = local_288.words + 18;
      for (iVar26 = 0x6c; iVar26 != 0; iVar26 = iVar26 + -1) {
        *puVar13 = *puVar14;
        puVar14 = puVar14 + 1;
        puVar13 = puVar13 + 1;
      }
    }
    local_2b4 = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    local_2b0 = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    local_2ac = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    local_288.floats[0] = local_2b4;
    local_288.floats[1] = local_2b0;
    local_288.floats[2] = local_2ac;
    uVar22 = ReadIntArgument(1);
    local_288.halves[32] = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(2);
    local_288.halves[33] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(3));
    local_288.floats[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.floats[9] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.floats[8] = (float)fVar19;
    uVar22 = ReadIntArgument(6);
    local_288.words[12] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(7);
    local_288.words[13] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(8);
    local_288.words[14] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(9);
    local_288.words[15] = (unsigned int)uVar22;
    fVar19 = (ReadFloatArgument(10));
    local_288.floats[10] = (float)fVar19;
    uVar22 = ReadIntArgument(0xb);
    local_288.words[17] = (unsigned int)uVar22 | 2;
    uVar6 = EnemyFireLaser(g_EnemyBulletManager, &local_288, 1);
    if (puVar5 != (unsigned int *)0x0) {
      *puVar5 = uVar6;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_OFFSET:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser((int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(2));
      fVar21 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x24) = (float)fVar21;
      *(float *)(iVar26 + 0x28) = (float)fVar19;
      *(unsigned int *)(iVar26 + 0x2c) = 0;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_ANGLE:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser((int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(2));
      fVar21 = (ReadFloatArgument(1));
      local_2b4 = (float)fVar21;
      local_2ac = 0.0;
      local_2b0 = (float)fVar19;
      EnemySetLaserPosition(
          iVar26, reinterpret_cast<const PlayerFloat3 *>(&local_2b4));
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_SPEED:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser((int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x48) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_COUNT:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser((int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x44) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_AIM:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser((int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x3c) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_SOUND:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser((int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x440) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_CANCEL_BULLET_PATTERN:
    fVar19 = (ReadFloatArgument(0));
    EnemyCancelBulletPattern((float)fVar19,1,0);
    EnemyApplyBulletCancel();
    return 0;
  case ENEMY_ECL_CLEAR_BULLET_PATTERN:
    fVar19 = (ReadFloatArgument(0));
    EnemyCancelBulletPattern((float)fVar19,0,0);
    EnemyApplyBulletCancel();
    return 0;
  case ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_3:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    if (0x1ff < g_EnemyRank) {
dispatch_bullet_speed_high:
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(5));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(6));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    iVar27 = g_EnemyRank + 0x200;
    goto dispatch_select_bullet_speed_low;
  case ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_5:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    if (599 < g_EnemyRank) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(9));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(10));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    if (199 < g_EnemyRank) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(7));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(8));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    if (-0xc9 < g_EnemyRank) goto dispatch_bullet_speed_high;
    iVar27 = g_EnemyRank + 600;
dispatch_select_bullet_speed_low:
    iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
    if (0 <= iVar27) {
      fVar19 = (ReadFloatArgument(3));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(4));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
dispatch_bullet_speed_lowest:
    fVar19 = (ReadFloatArgument(1));
    *(float *)(iVar26 + 0x2dc) = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    *(float *)(iVar26 + 0x2e0) = (float)fVar19;
    return 0;
  case ENEMY_ECL_INTERPOLATE_BULLET_SPEED_BY_RANK:
    uVar22 = ReadIntArgument(0);
    fVar19 = (ReadFloatArgument(1));
    fVar21 = (ReadFloatArgument(2));
    local_2a8 = (float)fVar21;
    fVar21 = (ReadFloatArgument(3));
    fVar20 = (ReadFloatArgument(4));
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    *(float *)(iVar26 + 0x2dc) =
         ((float)fVar21 - (float)fVar19) * ((float)g_EnemyRank + 1024.0f) * 0.00048828125f +
         (float)fVar19;
    *(float *)(iVar26 + 0x2e0) =
         (float)((fVar20 - local_2a8) * (g_EnemyRank + 1024.0f) *
                 0.00048828125f + local_2a8);
    return 0;
  case ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_3:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    if (0x1ff < g_EnemyRank) {
dispatch_bullet_count_high:
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ReadIntArgument(5);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      uVar22 = ReadIntArgument(6);
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    iVar27 = g_EnemyRank + 0x200;
    goto dispatch_select_bullet_count_low;
  case ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_5:
    uVar22 = ReadIntArgument(0);
    iVar26 = (int)uVar22;
    if (599 < g_EnemyRank) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ReadIntArgument(9);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      uVar22 = ReadIntArgument(10);
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    if (199 < g_EnemyRank) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ReadIntArgument(7);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      uVar22 = ReadIntArgument(8);
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    if (-0xc9 < g_EnemyRank) goto dispatch_bullet_count_high;
    iVar27 = g_EnemyRank + 600;
dispatch_select_bullet_count_low:
    iVar11 = *(int *)((int)runtimeAddress + 0x14d8);
    iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
    if (0 <= iVar27) {
      uVar22 = ReadIntArgument(3);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      uVar22 = ReadIntArgument(4);
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    uVar22 = ReadIntArgument(1);
    *(short *)(iVar26 + 0x4b8) = (short)uVar22;
    uVar22 = ReadIntArgument(2);
    *(short *)(iVar26 + 0x4ba) = (short)uVar22;
    return 0;
  case ENEMY_ECL_INTERPOLATE_BULLET_COUNT_BY_RANK:
    uVar22 = ReadIntArgument(0);
    uVar23 = ReadIntArgument(1);
    secondLowBulletCount = ReadIntArgument(2);
    uVar24 = ReadIntArgument(3);
    uVar25 = ReadIntArgument(4);
    iVar27 = (int)uVar22 * 0x210;
    iVar26 = ((int)uVar24 - (int)uVar23) * (g_EnemyRank + 0x400);
    *(short *)(iVar27 + 0x4b8 + (int)runtimeAddress) =
         (short)((int)(iVar26 + (iVar26 >> 0x1f & 0x7ffU)) >> 0xb) + (short)uVar23;
    iVar26 = ((int)uVar25 - secondLowBulletCount) * (g_EnemyRank + 0x400);
    *(short *)(iVar27 + (int)runtimeAddress + 0x4ba) =
         (short)((int)(iVar26 + (iVar26 >> 0x1f & 0x7ffU)) >> 0xb) +
         (short)secondLowBulletCount;
    return 0;
  case ENEMY_ECL_FIRE_LASER:
  case ENEMY_ECL_FIRE_LASER_WITH_PATTERN:
    pfVar15 = local_288.floats;
    for (iVar26 = 0x77; iVar26 != 0; iVar26 = iVar26 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    if (opcode == 0x1b1) {
      puVar5 = (unsigned int *)((int)runtimeAddress + 0x2e4);
      puVar14 = local_288.words + 11;
      for (iVar26 = 0x6c; iVar26 != 0; iVar26 = iVar26 + -1) {
        *puVar14 = *puVar5;
        puVar5 = puVar5 + 1;
        puVar14 = puVar14 + 1;
      }
    }
    local_2b4 = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    local_2b0 = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    local_2ac = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    local_288.floats[0] = local_2b4;
    local_288.floats[1] = local_2b0;
    local_288.floats[2] = local_2ac;
    uVar22 = ReadIntArgument(0);
    local_288.halves[18] = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(1);
    local_288.halves[19] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(2));
    local_288.floats[3] = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_288.floats[8] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.floats[5] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.floats[4] = (float)fVar19;
    fVar19 = (ReadFloatArgument(6));
    local_288.floats[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    local_288.floats[7] = (float)fVar19;
    local_288.words[10] &= 0xfffffffe;
    EnemyFireLaser(g_EnemyBulletManager, &local_288, 0);
    return 0;
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_2:
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_2_WITH_PATTERN:
    EnemyInitializeLaserConfig(&local_288);
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    if (*(short *)(iVar27 + 4) == 0x1b2) {
      puVar14 = (unsigned int *)((int)runtimeAddress + 0x2e4);
      puVar13 = local_288.words + 18;
      for (iVar26 = 0x6c; iVar26 != 0; iVar26 = iVar26 + -1) {
        *puVar13 = *puVar14;
        puVar14 = puVar14 + 1;
        puVar13 = puVar13 + 1;
      }
    }
    local_2b4 = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    local_2b0 = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    local_2ac = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    local_288.floats[0] = local_2b4;
    local_288.floats[1] = local_2b0;
    local_288.floats[2] = local_2ac;
    uVar22 = ReadIntArgument(1);
    local_288.halves[32] = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(2);
    local_288.halves[33] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(3));
    local_288.floats[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_288.floats[9] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    local_288.floats[8] = (float)fVar19;
    uVar22 = ReadIntArgument(6);
    local_288.words[12] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(7);
    local_288.words[13] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(8);
    local_288.words[14] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(9);
    local_288.words[15] = (unsigned int)uVar22;
    fVar19 = (ReadFloatArgument(10));
    local_288.floats[10] = (float)fVar19;
    uVar22 = ReadIntArgument(0xb);
    local_288.words[17] = (unsigned int)uVar22 & 0xfffffffd;
    uVar6 = EnemyFireLaser(g_EnemyBulletManager, &local_288, 1);
    if (puVar5 != (unsigned int *)0x0) {
      *puVar5 = uVar6;
      return 0;
    }
    break;
  case ENEMY_ECL_AIM_BULLET_AT_PLAYER:
    pfVar15 = (float *)(reinterpret_cast<int>(g_Player) + 0x3c0);
    fVar19 = (ReadFloatArgument(1));
    iVar26 = reinterpret_cast<int>(g_Player);
    fVar21 = (ReadFloatArgument(2));
    fVar19 = atan2(*(float *)(iVar26 + 0x3c4) - fVar21,
                   *pfVar15 - (float)fVar19);
    goto dispatch_store_float_result;
  case ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY:
    uVar22 = ReadIntArgument(0);
    if (g_EnemyDifficulty == 0) {
      fVar9 = EnemyEncodeOperandIndex(1);
    }
    else if (g_EnemyDifficulty == 1) {
      fVar9 = EnemyEncodeOperandIndex(2);
    }
    else {
      fVar9 = EnemyEncodeOperandIndex(3);
      if (g_EnemyDifficulty != 2) {
        fVar9 = EnemyEncodeOperandIndex(4);
      }
    }
    fVar19 = (ReadFloatArgument(EnemyDecodeOperandIndex(fVar9)));
    iVar26 = (int)uVar22 * 0x210;
    *(float *)(iVar26 + 0x2dc + (int)runtimeAddress) = (float)fVar19;
    if (g_EnemyDifficulty == 0) {
      fVar9 = EnemyEncodeOperandIndex(5);
    }
    else if (g_EnemyDifficulty == 1) {
      fVar9 = EnemyEncodeOperandIndex(6);
    }
    else {
      fVar9 = EnemyEncodeOperandIndex(7);
      if (g_EnemyDifficulty != 2) {
        fVar9 = EnemyEncodeOperandIndex(8);
      }
    }
    fVar19 = (ReadFloatArgument(EnemyDecodeOperandIndex(fVar9)));
    *(float *)(iVar26 + (int)runtimeAddress + 0x2e0) = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_BULLET_COUNT_BY_DIFFICULTY:
    uVar22 = ReadIntArgument(0);
    if (g_EnemyDifficulty == 0) {
      iVar26 = 1;
    }
    else if (g_EnemyDifficulty == 1) {
      iVar26 = 2;
    }
    else if (g_EnemyDifficulty == 2) {
      iVar26 = 3;
    }
    else {
      iVar26 = 4;
    }
    uVar23 = ReadIntArgument(iVar26);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    *(short *)(iVar26 + 0x4b8) = (short)uVar23;
    if (g_EnemyDifficulty == 0) {
      iVar27 = 5;
    }
    else if (g_EnemyDifficulty == 1) {
      iVar27 = 6;
    }
    else if (g_EnemyDifficulty == 2) {
      iVar27 = 7;
    }
    else {
      iVar27 = 8;
    }
    uVar22 = ReadIntArgument(iVar27);
    *(short *)(iVar26 + 0x4ba) = (short)uVar22;
    return 0;
  }
dispatch_complete:
  return 0;
dispatch_create_enemy_absolute_mirrored:
  iVar26 = *(int *)(iVar27 + 0x10) + 4;
  pfVar15 = local_90.floatWords;
  for (iVar11 = 0x10; iVar11 != 0; iVar11 = iVar11 + -1) {
    *pfVar15 = 0.0;
    pfVar15 = pfVar15 + 1;
  }
  iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
  fVar19 = (ReadRawFloatArgument((1), (*(float *)(iVar27 + 0x10 + iVar26 * 4))));
  local_90.spawnRequest.position.x = (float)fVar19;
  fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
  local_90.spawnRequest.position.y = (float)fVar19;
  uVar22 = ReadRawIntArgument(3, *(int *)(iVar27 + 0x18 + iVar26 * 4));
  local_90.spawnRequest.life = uVar22;
  uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
  local_90.spawnRequest.scoreReward = uVar22;
  uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
  local_90.spawnRequest.itemDropType = uVar22;
  puVar5 = (unsigned int *)((int)runtimeAddress + 0xfc);
  pbVar16 = reinterpret_cast<unsigned char *>(&local_90.spawnRequest.eclVariables);
  for (iVar26 = 8; iVar26 != 0; iVar26 = iVar26 + -1) {
    *(unsigned int *)pbVar16 = *puVar5;
    puVar5 = puVar5 + 1;
    pbVar16 = pbVar16 + 4;
  }
  local_90.spawnRequest.setFlag0800 = 1;
  goto dispatch_spawn_enemy;
dispatch_create_enemy_absolute:
  iVar26 = *(int *)(iVar27 + 0x10) + 4;
  pfVar15 = local_90.floatWords;
  for (iVar11 = 0x10; iVar11 != 0; iVar11 = iVar11 + -1) {
    *pfVar15 = 0.0;
    pfVar15 = pfVar15 + 1;
  }
  iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
  fVar19 = (ReadRawFloatArgument((1), (*(float *)(iVar27 + 0x10 + iVar26 * 4))));
  local_90.spawnRequest.position.x = (float)fVar19;
  fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
  goto dispatch_finish_spawn_position;
dispatch_copy_player_to_motion:
  iVar26 = *(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10);
  *(unsigned int *)((int)runtimeAddress + 0x58) = *(unsigned int *)(iVar26 + 0x1068);
  *(unsigned int *)((int)runtimeAddress + 0x5c) = *(unsigned int *)(iVar26 + 0x106c);
  *(unsigned int *)((int)runtimeAddress + 0x60) = *(unsigned int *)(iVar26 + 0x1070);
  return 0;
}
