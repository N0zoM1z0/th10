#include "Main.hpp"
#include "VmIdInterrupt.hpp"
#include "EnemySoundQueue.hpp"
#include "ItemManager.hpp"
#include "EnemyGameState.hpp"
#include "EnemyPrimaryResources.hpp"
#include "Enemy.hpp"
#include "EnemyCancel.hpp"
#include "EnemyLaser.hpp"
#include "EclVm.hpp"
#include "Rng.hpp"
#include "Gui.hpp"
#include "BulletManager.hpp"

#include <math.h>
#include <new>
#include <string.h>


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


struct EnemyManagedVmView;
struct EnemyBulletPatternCopyView
{
    unsigned int words[0x84];
};
typedef char EnemyBulletPatternCopyViewSizeIs210[
    (sizeof(EnemyBulletPatternCopyView) == 0x210) ? 1 : -1];


extern int g_EnemyDifficulty;
extern EnemyManagerView *g_EnemyManager;
extern EnemyGameStateView *g_EnemyGameState;
extern void *g_EnemyBulletManager;
extern PlayerFloat3 g_EnemyGlobalPositionOffset;
extern PlayerFloat3 g_EnemyInterpolationOrigin;
extern EnemyFloat2 g_EnemyInterpolationBasis;
extern unsigned char g_EnemyViewMatrix;
extern unsigned char g_EnemyProjectionMatrix;
extern unsigned char g_EnemyViewport;
extern unsigned char g_EnemyWorldMatrix;

extern PlayerFloat3 *__stdcall D3DXVec3Project(
    PlayerFloat3 *output, const PlayerFloat3 *input, const void *viewport,
    const void *projection, const void *view, const void *world);
extern "C" AnmFloat3View *__stdcall D3DXVec3Normalize(
    AnmFloat3View *output, const AnmFloat3View *input);
extern "C" AnmMatrixView *__stdcall D3DXMatrixLookAtLH(
    AnmMatrixView *output, const AnmFloat3View *eye,
    const AnmFloat3View *at, const AnmFloat3View *up);
extern "C" AnmMatrixView *__stdcall D3DXMatrixPerspectiveFovLH(
    AnmMatrixView *output, float fov, float aspect,
    float nearPlane, float farPlane);

__declspec(noinline) void EnemyConfigureViewportOwner(
    AnmViewportOwnerView *viewport)
{
    if (g_AnmRenderManagerView != 0) {
        g_AnmRenderManagerView->FlushVertexBuffer();
    }

    float *values = reinterpret_cast<float *>(viewport);
    AnmFloat3View at;
    AnmFloat3View eye;
    at.x = values[3] + values[0];
    at.y = values[4] + values[1];
    at.z = values[5] + values[2];
    eye.x = values[0] + values[15];
    eye.y = values[1] + values[16];
    eye.z = values[2] + values[17];

    D3DXMatrixLookAtLH(
        &viewport->viewMatrix, &eye, &at,
        reinterpret_cast<AnmFloat3View *>(
            reinterpret_cast<unsigned char *>(viewport) + 0x18));

    unsigned int width = viewport->viewport.width;
    unsigned int height = viewport->viewport.height;
    D3DXMatrixPerspectiveFovLH(
        &viewport->projectionMatrix, values[0x48 / 4],
        static_cast<float>(width) / static_cast<float>(height),
        30.0f, 1800.0f);

    g_MainSupervisorView.d3dDevice->vtable->SetTransform(
        g_MainSupervisorView.d3dDevice, 2, &viewport->viewMatrix);
    g_MainSupervisorView.d3dDevice->vtable->SetTransform(
        g_MainSupervisorView.d3dDevice, 3, &viewport->projectionMatrix);

    const AnmFloat3View *up =
        reinterpret_cast<const AnmFloat3View *>(
            reinterpret_cast<const unsigned char *>(viewport) + 0x18);
    AnmFloat3View cross;
    cross.x = up->z * values[4] - up->y * values[5];
    cross.y = up->x * values[5] - up->z * values[3];
    cross.z = up->y * values[3] - up->x * values[4];
    viewport->cameraRight = cross;
    D3DXVec3Normalize(&viewport->cameraRight, &viewport->cameraRight);

    if (g_AnmRenderManagerView != 0) {
        g_AnmRenderManagerView->screenShakeX = values[0xE8 / 4];
        g_AnmRenderManagerView->screenShakeY = values[0xEC / 4];
    }
}

__declspec(noinline) void EnemyPrepareProjection(
    MainSupervisorView *supervisor, int viewportIndex)
{
    AnmViewportOwnerView *viewport =
        reinterpret_cast<AnmViewportOwnerView *>(
            reinterpret_cast<unsigned char *>(supervisor) +
            0x154 + viewportIndex * 0x118);
    supervisor->activeViewport = viewport;
    EnemyConfigureViewportOwner(viewport);
    supervisor->d3dDevice->vtable->SetViewport(
        supervisor->d3dDevice, &supervisor->activeViewport->viewport);
    supervisor->viewportConfigured = viewportIndex;
}
extern float g_AnmGameSpeed;

static __declspec(noinline) void EnemyInitializePositionInterpolation(
    EnemyPositionInterpolationView *interpolation)
{
    unsigned int *words = reinterpret_cast<unsigned int *>(interpolation);
    unsigned int flags = words[0x40 / 4];
    if ((flags & 1) == 0) {
        flags |= 1;
        words[0x34 / 4] = 0;
        words[0x30 / 4] = static_cast<unsigned int>(-999999);
        words[0x38 / 4] = 0;
        words[0x3c / 4] = reinterpret_cast<unsigned int>(&g_AnmGameSpeed);
        words[0x40 / 4] = flags;
    }
    words[0x34 / 4] = 0;
    words[0x38 / 4] = 0;
    words[0x30 / 4] = static_cast<unsigned int>(-1);
}
extern void EnemySpawnCancelEffect(
    float *position, int kind, int color, float angle, float scale);
extern int *EnemyCreateCancelVm(void *resource);
extern int EnemyResolveCancelVm(int id);
extern float g_EnemyCancelHalfScale;
extern float g_EnemyPlayfieldMinX;
extern float g_EnemyPlayfieldMaxX;
extern float g_EnemyPlayfieldMinY;
extern float g_EnemyPlayfieldMaxY;
extern float g_EnemyCancelHighThreshold;
extern float g_EnemyCancelUpperThreshold;
extern unsigned int g_EnemyCancelColorHigh[];
extern unsigned int g_EnemyCancelColorMid[];
extern unsigned int g_EnemyCancelColorLow[];

__declspec(noinline) unsigned int EnemyCancelManagerView::CancelAllBullets(int mode)
{
    AnmFloat3View halfSize = cancelSize * g_EnemyCancelHalfScale;
    AnmFloat3View minimum = cancelCenter - halfSize;
    AnmFloat3View maximum = cancelCenter + halfSize;
    unsigned char *bullet = bulletStorage;
    for (int remaining = 2000; remaining != 0;
         --remaining, bullet += 0x7f0) {
        short state = *reinterpret_cast<short *>(bullet + 0x446);
        if (state == 0 || state == 3) {
            continue;
        }

        float *position = reinterpret_cast<float *>(bullet + 0x3b4);
        float radiusX =
            *reinterpret_cast<float *>(bullet + 0x3f0) *
            g_EnemyCancelHalfScale;
        float radiusY =
            *reinterpret_cast<float *>(bullet + 0x3f4) *
            g_EnemyCancelHalfScale;
        if (minimum.x <= position[0] + radiusX &&
            position[0] - radiusX <= maximum.x &&
            minimum.y <= position[1] + radiusY &&
            position[1] - radiusY <= maximum.y) {
            *reinterpret_cast<unsigned int *>(bullet) |= 8;
            if (mode != 0) {
                EnemySpawnCancelEffect(
                    position, 8, -1, -1.5707964f, 0.6f);
            }

            int *vmId = EnemyCreateCancelVm(primaryEnemyResource);
            int vm = EnemyResolveCancelVm(*vmId);
            unsigned int colorOwner =
                *reinterpret_cast<unsigned int *>(bullet + 0x39c);
            if (colorOwner != 0) {
                float value = *reinterpret_cast<float *>(colorOwner + 0x34);
                short colorIndex =
                    *reinterpret_cast<short *>(bullet + 0x7ec);
                unsigned int color;
                if (value > g_EnemyCancelHighThreshold) {
                    color = g_EnemyCancelColorHigh[colorIndex];
                }
                else if (value > g_EnemyCancelUpperThreshold) {
                    color = g_EnemyCancelColorMid[colorIndex];
                }
                else {
                    color = g_EnemyCancelColorLow[colorIndex];
                }
                *reinterpret_cast<unsigned int *>(vm + 0x2fc) = color;
            }
            *reinterpret_cast<unsigned int *>(bullet + 0x450) = 0;
        }
    }
    return 0;
}

struct EnemyBulletPositionView
{
    float x;
    float y;
    float z;

    __declspec(noinline) int IsOutsidePlayfield(float marginX, float marginY);
};


__declspec(noinline) int EnemyBulletPositionView::IsOutsidePlayfield(
    float marginX, float marginY)
{
    if (!(x + marginX <= g_EnemyPlayfieldMaxX) &&
        !(x - marginX >= g_EnemyPlayfieldMinX) &&
        !(y + marginY <= g_EnemyPlayfieldMaxY) &&
        !(y - marginY >= g_EnemyPlayfieldMinY)) {
        return 0;
    }
    return 1;
}

__declspec(noinline) unsigned int EnemyCancelBulletRecord(
    unsigned char *bullet)
{
    short state = *reinterpret_cast<short *>(bullet + 0x446);
    if (state != 2 && state != 1) {
        return 0;
    }

    EnemyBulletPositionView *position =
        reinterpret_cast<EnemyBulletPositionView *>(bullet + 0x3b4);
    if (position->IsOutsidePlayfield(8.0f, 8.0f) == 0) {
        *reinterpret_cast<short *>(bullet + 0x446) = 3;
        *reinterpret_cast<short *>(bullet + 0x30c) = 1;
        int zero = 0;
        int effectScript = *reinterpret_cast<int *>(bullet + 0x438);
        if (effectScript >= 0) {
            g_EnemyPrimaryResourceOwner->primaryEnemyResource->
                CreateVmAtWorldVariant0(
                    effectScript,
                    reinterpret_cast<const AnmFloat3View *>(
                        bullet + 0x3b4));
        }

        unsigned int *timerFlags =
            reinterpret_cast<unsigned int *>(bullet + 0x408);
        unsigned int timerFlagValue = *timerFlags;
        if ((timerFlagValue & 1u) == 0) {
            timerFlagValue |= 1u;
            *reinterpret_cast<int *>(bullet + 0x3fc) = zero;
            *reinterpret_cast<int *>(bullet + 0x3f8) = -999999;
            *reinterpret_cast<float *>(bullet + 0x400) = 0.0f;
            *reinterpret_cast<float **>(bullet + 0x404) = &g_AnmGameSpeed;
            *timerFlags = timerFlagValue;
        }
        *reinterpret_cast<int *>(bullet + 0x3fc) = zero;
        *reinterpret_cast<float *>(bullet + 0x400) = 0.0f;
        *reinterpret_cast<int *>(bullet + 0x3f8) = -1;
        return 1;
    }
    *reinterpret_cast<short *>(bullet + 0x446) = 3;
    *reinterpret_cast<short *>(bullet + 0x30c) = 1;
    *reinterpret_cast<unsigned int *>(bullet) |= 8;
    return 1;
}

struct EnemyCircleDisplacementView : PlayerFloat3
{
    EnemyCircleDisplacementView(float xValue, float yValue, float zValue)
    {
        x = xValue;
        y = yValue;
        z = zValue;
    }
};

__declspec(noinline) unsigned int EnemyCancelManagerView::CancelBulletPattern(
    const PlayerFloat3 *position, float radius, int mode, int flags)
{
    BulletRuntimeView *bullet =
        reinterpret_cast<BulletRuntimeView *>(bulletStorage);
    for (int remaining = 2000; remaining != 0;
         --remaining, ++bullet) {
        short state = bullet->state;
        if (state == 0 || state == 3) {
            continue;
        }
        if (flags != 0 && bullet->cancelBehavior != 0) {
            continue;
        }

        PlayerFloat3 displacement = EnemyCircleDisplacementView(
            bullet->position.x - position->x,
            bullet->position.y - position->y,
            bullet->position.z - position->z);
        float effectiveRadius =
            bullet->collisionWidth *
                0.5f +
            radius;
        if (!(displacement.x * displacement.x +
                displacement.y * displacement.y > effectiveRadius * effectiveRadius)) {
            EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(bullet));
            EnemyBulletPositionView *bulletPosition =
                reinterpret_cast<EnemyBulletPositionView *>(&bullet->position);
            if (bulletPosition->IsOutsidePlayfield(2.0f, 2.0f) == 0 &&
                mode != 0) {
                g_ItemManager->Spawn(
                    reinterpret_cast<const AnmFloat3View *>(bulletPosition),
                    8, -1, -1.5707964f, 0.6f);
            }
        }
    }
    return 0;
}
extern void __stdcall EnemyBeginSpell(int gameState, int spellId, char *name, int value);


extern void EnemyEndSpell(EnemyGameStateView *gameState);
__declspec(noinline) void EnemyEnableBombShield(
    EnemyGameStateView *gameState)
{
    unsigned int *flags = &gameState->flags378C;
    *flags |= 0x10;
    g_AnmRenderManagerView->MarkVmForDeletion(
        *reinterpret_cast<AnmVmIdView *>(&gameState->managedVmId));
    gameState->managedVmId = 0;
}
static __declspec(noinline) void EnemySetLaserPosition(
    int laser, const PlayerFloat3 *position)
{
    *reinterpret_cast<PlayerFloat3 *>(laser + 0x430) = *position;
}
static __declspec(noinline) int EnemyFindLaser(
    EnemyLaserManagerView *manager, int id)
{
    EnemyLaserNodeView *laser = manager->head;
    while (laser != 0) {
        if (laser->id == id) {
            return reinterpret_cast<int>(laser);
        }
        laser = laser->next;
    }
    return 0;
}
extern void __stdcall EnemyKillAll(EnemyManagerView *manager);
static __declspec(noinline) void __stdcall EnemyConfigureInterrupt(
    EnemyFullObjectView *owner, int slot, int lifeThreshold,
    int timerThreshold, const unsigned char *callbackName)
{
    unsigned char *record =
        reinterpret_cast<unsigned char *>(owner) + slot * 0x10;
    *reinterpret_cast<int *>(record + 0x2494) = lifeThreshold;
    if (lifeThreshold >= 0) {
        *reinterpret_cast<int *>(record + 0x2498) = timerThreshold;
        *reinterpret_cast<const unsigned char **>(record + 0x249c) =
            callbackName;
        *reinterpret_cast<const unsigned char **>(
            reinterpret_cast<unsigned char *>(owner) +
            (slot + 0x24a) * 0x10) = callbackName;
    }
}
static __declspec(noinline) void EnemyInitializeScalarInterpolation(
    EnemyScalarInterpolationView *interpolation)
{
    unsigned int *words = reinterpret_cast<unsigned int *>(interpolation);
    unsigned int flags = words[0x30 / 4];
    if ((flags & 1) == 0) {
        flags |= 1;
        words[0x24 / 4] = 0;
        words[0x20 / 4] = static_cast<unsigned int>(-999999);
        words[0x28 / 4] = 0;
        words[0x2c / 4] = reinterpret_cast<unsigned int>(&g_AnmGameSpeed);
        words[0x30 / 4] = flags;
    }
    words[0x24 / 4] = 0;
    words[0x28 / 4] = 0;
    words[0x20 / 4] = static_cast<unsigned int>(-1);
}
struct EnemyChapterStateView
{
    unsigned char unknown000[0x0c];
    int spellBonusBase;
    unsigned char unknown010[0x18];
    int statisticsGroupA;
    int statisticsGroupB;
    unsigned char unknown030[0x0c];
    int stage;
    unsigned char unknown040[4];
    int chapter;
    int unknown048;
    int chapterTimer;
    unsigned char unknown050[8];
    int rank;
};
typedef char EnemySpellStatisticsStateOffsets[
    (offsetof(EnemyChapterStateView, spellBonusBase) == 0x0c &&
     offsetof(EnemyChapterStateView, statisticsGroupA) == 0x28 &&
     offsetof(EnemyChapterStateView, statisticsGroupB) == 0x2c) ? 1 : -1];
typedef char EnemyStageAndChapterOffsets[
    (offsetof(EnemyChapterStateView, stage) == 0x3c &&
     offsetof(EnemyChapterStateView, chapter) == 0x44 &&
     offsetof(EnemyChapterStateView, chapterTimer) == 0x4c) ? 1 : -1];

typedef char EnemyRankOffsetIs58[(offsetof(EnemyChapterStateView, rank) == 0x58) ? 1 : -1];
extern EnemyChapterStateView g_EnemyChapterState;

static __declspec(noinline) void EnemySetChapter(
    EnemyChapterStateView *state, int chapter)
{
    if (state->chapter == chapter) {
        state->chapter = chapter;
        return;
    }
    state->chapter = chapter;
    state->chapterTimer = 0;
}
extern void __stdcall EnemySetMotionAngle(EnemyMotionView *motion, float angle);
extern int g_EnemyDialogIndex;
extern int g_EnemyDialogState;

static void EnemyReadDialog(GuiView *gui, int index)
{
    GuiMessageVmView *dialog = new GuiMessageVmView(
        static_cast<unsigned char *>(gui->messageFile) +
        *reinterpret_cast<int *>(static_cast<unsigned char *>(gui->messageFile) +
            4 + index * 8));
    gui->message = dialog;
    dialog->messageIndex = index;
    int nextIndex = index + 1;
    int previousIndex = g_EnemyDialogIndex;
    g_EnemyDialogIndex = nextIndex;
    if (previousIndex != nextIndex) {
        g_EnemyDialogState = 0;
    }
}
static __declspec(noinline) AnmFloat3View *EnemyGetAnmPosition(
    const AnmFloat3View *position)
{
    static AnmFloat3View result;
    result.x = position->x + 224.0f;
    result.y = position->y + 16.0f;
    result.z = position->z;
    return &result;
}
static __declspec(noinline) unsigned int EnemyWaitForEffect(
    EnemyEffectWaitManager *manager)
{
    EnemyEffectWaitNode *node = manager->head;
    while (node != 0) {
        EnemyEffectWaitNode *next = node->next;
        unsigned char flag = node->flag50;
        if (node->state != 1 && flag == 0) {
            node->flag50 = 1;
        }
        node = next;
    }
    return 0;
}
__declspec(noinline) unsigned int EnemyEffectWaitManager::ApplyPatternCancel(
    const PlayerFloat3 *position, float radius, int mode)
{
    cancelPosition = *position;
    unsigned int total = 0;
    EnemyEffectWaitNode *node = head;
    while (node != 0) {
        EnemyEffectWaitNode *next = node->next;
        if (node->state != 1) {
            total += node->ApplyPatternCancel(position, radius, mode);
        }
        node = next;
    }
    return total;
}

__declspec(noinline) unsigned int EnemyApplyBulletClear(
    EnemyEffectWaitManager *manager, int mode)
{
    EnemyEffectWaitNode *node = manager->head;
    while (node != 0) {
        EnemyEffectWaitNode *next = node->next;
        if (node->state != 1) {
            node->ApplyClear(mode);
        }
        node = next;
    }
    return 1;
}
extern unsigned int * __stdcall EnemySetScreenShake(
    int enabled, int duration, unsigned int horizontal,
    unsigned int vertical, unsigned int flags);
struct EnemySoundCueMetadataView
{
    int unknown000;
    unsigned short unknown004;
    short cueValue;
};



extern EnemySoundCueMetadataView g_EnemySoundCueMetadata[];

void EnemySoundQueueView::QueueSoundCue(int soundId, float positionX)
{
    float scaledPosition = positionX * 5.208333492279053f;
    int cueValue = g_EnemySoundCueMetadata[soundId].cueValue;
    int sample = static_cast<int>(scaledPosition);
    int slot = 0;
    while (slot < 12) {
        int activeId = activeSoundIds[slot];
        if (activeId < 0) {
            break;
        }
        if (activeId == soundId) {
            int count = sampleCounts[slot];
            if (count >= 128) {
                return;
            }
            samples[slot][count] = sample;
            sampleCounts[slot] = sampleCounts[slot] + 1;
            return;
        }
        ++slot;
    }
    if (slot < 12) {
        activeSoundIds[slot] = soundId;
        cueValues[soundId] = cueValue;
        samples[slot][0] = sample;
        ++sampleCounts[slot];
    }
}
void EnemySoundQueueView::QueueSoundSample(int soundId, int sample)
{
    int cueValue = g_EnemySoundCueMetadata[soundId].cueValue;
    int slot = 0;
    while (slot < 12) {
        int activeId = activeSoundIds[slot];
        if (activeId < 0) {
            break;
        }
        if (activeId == soundId) {
            int count = sampleCounts[slot];
            if (count >= 128) {
                return;
            }
            samples[slot][count] = sample;
            sampleCounts[slot] = sampleCounts[slot] + 1;
            return;
        }
        ++slot;
    }
    if (slot < 12) {
        activeSoundIds[slot] = soundId;
        cueValues[soundId] = cueValue;
        samples[slot][0] = sample;
        ++sampleCounts[slot];
    }
}

extern __declspec(noinline) float __stdcall EnemyWrapAngle(float angle);

// Target pointer slots: playback at 477838 and statistics at 47783C.
// Statistics indices and bonus input use receiver-relative slots in the
// partial view at 474C40 above; original data/class owners remain unknown.
extern unsigned char *g_EnemySpellPlaybackState;
extern unsigned char *g_EnemySpellStatistics;

// Complete maintained body for target 0x00409280, including every stage arm.
// The four-stack-argument stdcall surface is target-proven; original source
// class, filename and declaration shape remain unknown.
static void CreateSpellStageAnimation(AnmLoadedView *resource, int script);

void __stdcall EnemyBeginSpell(int gameState, int spellId, char *name, int value)
{
    EnemyGameStateView *state =
        reinterpret_cast<EnemyGameStateView *>(gameState);
    state->spellTimer.SetCurrent(0);
    state->spellId = spellId;
    strcpy(state->spellName, name);
    state->flags378C = (state->flags378C & ~0x18u) | 3u;

    if (*reinterpret_cast<int *>(g_EnemySpellPlaybackState + 0x10) != 1) {
        unsigned char *statistics = g_EnemySpellStatistics;
        int spellOffset = spellId * 0x90;
        unsigned char *record = statistics +
            (g_EnemyChapterState.statisticsGroupA * 3 +
             g_EnemyChapterState.statisticsGroupB) *
                0x437c + spellOffset + 0x5a4;
        strcpy(reinterpret_cast<char *>(record), name);
        record = statistics +
            (g_EnemyChapterState.statisticsGroupA * 3 +
             g_EnemyChapterState.statisticsGroupB) *
                0x437c + spellOffset + 0x5a4;
        int count = *reinterpret_cast<int *>(record + 0x84);
        if (count < 99999) {
            *reinterpret_cast<int *>(record + 0x84) = count + 1;
        }
        record = statistics + spellOffset + 0x19a8c;
        strcpy(reinterpret_cast<char *>(record), name);
        if (*reinterpret_cast<int *>(record + 0x84) < 99999) {
            ++*reinterpret_cast<int *>(record + 0x84);
        }
    }

    for (int i = 0; i < 8; ++i) {
        g_GuiView->frontAnm->InitializeAndExecuteScriptIndex(
            &state->frontVms[i], i + 0x3a);
    }
    for (int i = 0; i < 5; ++i) {
        g_GuiView->frontAnm->InitializeAndExecuteScriptIndex(
            &state->secondaryFrontVms[i], i + 0x42);
    }

    state->panelVmIds[0] = g_AsciiManagerView->asciiAnm->CreateVmVariant0(1, 15);
    state->panelVmIds[1] = g_AsciiManagerView->textAnm->CreateVmVariant0(0x48, 15);
    state->panelVmIds[2] = g_AsciiManagerView->asciiAnm->CreateVmVariant0(2, 15);
    AnmVmView *nameVm = g_AnmRenderManagerView->FindVm(state->panelVmIds[1]);
    if (nameVm == 0) {
        state->panelVmIds[1].value = 0;
    }
    g_AnmRenderManagerView->DrawTextRight(nameVm, 0x00ffffffu, name);
    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
        QueueSoundSample(14, 0);
    state->managedVmId = g_EnemyPrimaryResourceOwner->primaryEnemyResource->
        CreateVmVariant0(0x1a1, 15).value;
    state->effectPosition = *reinterpret_cast<AnmFloat3View *>(
        reinterpret_cast<unsigned char *>(g_EnemyManager->specialEnemySlots[0]) +
        0x1068);
    g_AnmRenderManagerView->SetVmWorldPosition(
        AnmVmIdView(state->managedVmId), &state->effectPosition);

    AnmVmView *vm = g_AnmRenderManagerView->FindVm(
        AnmVmIdView(state->managedVmId));
    if (vm == 0) {
        state->managedVmId = 0;
    }
    AnmVmLayerNodeView *node = &vm->layerNode;
    AnmVmView *child = 0;
    while (node != 0) {
        AnmVmView *candidate = static_cast<AnmVmView *>(node->owner);
        if (candidate->scriptIndex == 0x19f) {
            child = candidate;
            break;
        }
        node = node->next;
    }
    child->intVar2 = value;

    vm = g_AnmRenderManagerView->FindVm(AnmVmIdView(state->managedVmId));
    if (vm == 0) {
        state->managedVmId = 0;
    }
    node = &vm->layerNode;
    child = 0;
    while (node != 0) {
        AnmVmView *candidate = static_cast<AnmVmView *>(node->owner);
        if (candidate->scriptIndex == 0x1a0) {
            child = candidate;
            break;
        }
        node = node->next;
    }
    child->intVar2 = value;
    state->spellDuration = value;
    int bonus = (g_EnemyChapterState.stage * 3 + 10) *
        g_EnemyChapterState.spellBonusBase * 10;
    state->currentSpellBonus = bonus;
    state->initialSpellBonus = bonus;
    if (bonus >= 100000000) {
        state->initialSpellBonus = 99999999;
    }
    g_EnemyPrimaryResourceOwner->primaryEnemyResource->
        CreateVmVariant0(0x1ab, 15);

    AnmLoadedView *stageResource;
    switch (g_EnemyChapterState.stage) {
    case 1:
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[0].InitializeForLoadedScript(stageResource, 0x0c);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[1].InitializeForLoadedScript(stageResource, 0x0b);
        if (spellId >= 2) {
            stageResource = reinterpret_cast<AnmLoadedView *>(
                g_EnemyManager->effectResources[2]);
            CreateSpellStageAnimation(stageResource, 0x0e);
        }
        else {
            stageResource = reinterpret_cast<AnmLoadedView *>(
                g_EnemyManager->effectResources[2]);
            CreateSpellStageAnimation(stageResource, 0x0f);
        }
        break;
    case 2:
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[0].InitializeForLoadedScript(stageResource, 0x0e);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[1].InitializeForLoadedScript(stageResource, 0x0f);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        CreateSpellStageAnimation(stageResource, 0x11);
        break;
    case 3:
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[0].InitializeForLoadedScript(stageResource, 0x12);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[1].InitializeForLoadedScript(stageResource, 0x13);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        CreateSpellStageAnimation(stageResource, 0x15);
        break;
    case 4:
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[0].InitializeForLoadedScript(stageResource, 0x13);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[1].InitializeForLoadedScript(stageResource, 0x14);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        CreateSpellStageAnimation(stageResource, 0x16);
        break;
    case 5:
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[0].InitializeForLoadedScript(stageResource, 0x0c);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[1].InitializeForLoadedScript(stageResource, 0x0d);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        CreateSpellStageAnimation(stageResource, 0x0f);
        break;
    case 6:
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[0].InitializeForLoadedScript(stageResource, 0x21);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        state->stageVms[1].InitializeForLoadedScript(stageResource, 0x22);
        stageResource = reinterpret_cast<AnmLoadedView *>(
            g_EnemyManager->effectResources[2]);
        CreateSpellStageAnimation(stageResource, 0x24);
        break;
    case 7:
        if (g_EnemyChapterState.chapter < 24) {
            stageResource = reinterpret_cast<AnmLoadedView *>(
                g_EnemyManager->effectResources[2]);
            state->stageVms[0].InitializeForLoadedScript(stageResource, 0x26);
            stageResource = reinterpret_cast<AnmLoadedView *>(
                g_EnemyManager->effectResources[2]);
            CreateSpellStageAnimation(stageResource, 0x28);
        }
        else {
            stageResource = reinterpret_cast<AnmLoadedView *>(
                g_EnemyManager->effectResources[2]);
            state->stageVms[0].InitializeForLoadedScript(stageResource, 0x1b);
            stageResource = reinterpret_cast<AnmLoadedView *>(
                g_EnemyManager->effectResources[2]);
            state->stageVms[1].InitializeForLoadedScript(stageResource, 0x1c);
            stageResource = reinterpret_cast<AnmLoadedView *>(
                g_EnemyManager->effectResources[2]);
            CreateSpellStageAnimation(stageResource, 0x1e);
        }
        break;
    default:
        return;
    }
}

static void CreateSpellStageAnimation(AnmLoadedView *resource, int script)
{
    AnmVmView *stageVm = g_AnmRenderManagerView->AllocateVm();
    stageVm->renderLayer = 15;
    stageVm->flags35C |= 0x40000000u;
    resource->InitializeVm(stageVm, script);
    // Target keeps this final insertion out of line after the initial creations.
    // This scoped compiler policy does not establish original TU/settings.
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
    g_AnmRenderManagerView->AddVmVariant0(stageVm);
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
}

struct EnemyDropVectorView
{
    float x;
    float y;
    float z;

    __declspec(noinline) void FromAngleRadii(
        float angle, float radiusX, float radiusY)
    {
#if defined(_MSC_VER) && defined(_M_IX86)
        __asm
        {
            mov eax, this
            fld angle
            fsincos
            fmul radiusX
            fstp [eax]
            fmul radiusY
            fstp [eax + 4]
        }
#else
        x = static_cast<float>(cos(angle)) * radiusX;
        y = static_cast<float>(sin(angle)) * radiusY;
#endif
    }
};

// Scatter storage/order informed by th10-decomp (MIT), commit
// c6791d82dc9f02e55eca45a5314f900a85481efd; TH10 target/native evidence is authoritative.
// See docs/licenses/th10-decomp-MIT.txt.
__declspec(noinline) void __stdcall EnemyDropItemCountsCore(
    const PlayerFloat3 *position, int *itemDropBlock)
{
    float angle = g_RngView.GetRandomF32Signed() * 3.1415927f;

    for (int itemIndex = 0; itemIndex < 11; ++itemIndex) {
        for (int i = 0; i < itemDropBlock[itemIndex + 1]; ++i) {
            EnemyDropVectorView offset;
            offset.FromAngleRadii(
                angle,
                reinterpret_cast<float *>(itemDropBlock)[13],
                reinterpret_cast<float *>(itemDropBlock)[14]);

            unsigned int radiusBits = g_RngView.GetRandomU32();
            // VC7.1 supplies the target unsigned high-bit correction once.
            float radiusScale = static_cast<float>(radiusBits);
            const float height = position->z;
            radiusScale =
                radiusScale * 1.1641532182693481e-10f + 0.5f;

            offset.x *= radiusScale;
            offset.y *= radiusScale;
            offset.x += position->x;
            offset.y += position->y;
            offset.z = height;

            g_ItemManager->Spawn(
                reinterpret_cast<const AnmFloat3View *>(&offset),
                itemIndex + 1, 0xffffffffu, -1.5707964f, 2.2f);

            unsigned int angleBits = g_RngView.GetRandomU32();
            float angleRandom = static_cast<float>(angleBits);
            angleRandom =
                angleRandom * 4.656612873077393e-10f - 1.0f;
            angle = EnemyWrapAngle(
                angleRandom * 0.7853982f + angle + 1.5707964f);
        }
    }

    memset(itemDropBlock + 1, 0, 12 * sizeof(int));
}

static __declspec(noinline) void EnemyDropItemCounts(
    const PlayerFloat3 *position, int *itemDropBlock)
{
    if (*itemDropBlock > 0) {
        g_ItemManager->Spawn(
            reinterpret_cast<const AnmFloat3View *>(position),
            *itemDropBlock, -1, -1.5707964f, 2.2f);
    }
    EnemyDropItemCountsCore(position, itemDropBlock);
    *itemDropBlock = 0;
}

extern float EnemyRandomAngle();

static float EnemyAbsoluteFloat(float value)
{
    return value < 0.0f ? -value : value;
}

// Target 0x0044BC70-0x0044BCCD wraps one angle into the signed-pi interval.
// The shared counter and 32-iteration guard are both visible in the target.
__declspec(noinline) float __stdcall EnemyWrapAngle(float angle)
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

int EnemyRuntimeView::ReadIntArgument(int index)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ReadInt(index);
}

int *EnemyRuntimeView::ResolveIntArgument(int index)
{
    return reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ResolveInt(index);
}

float EnemyRuntimeView::ReadFloatArgument(int index)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ReadFloat(index);
}

float *EnemyRuntimeView::ResolveFloatArgument(int index)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ResolveFloat(index);
}

int EnemyRuntimeView::ReadRawIntArgument(int index, int rawValue)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return host->activeContext->ReadIntValue(index, rawValue);
}

float EnemyRuntimeView::ReadRawFloatArgument(int index, float rawValue)
{
    EclVmHost *const host = reinterpret_cast<EclVmHost *>(owner);
    return EclVmContext::ReadFloatValue(index, host->activeContext, rawValue);
}

#define ENEMY_READ_INT_DIRECT(index) \
    (reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ReadInt(index))

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
  EnemyFloat2 initialPolar;
  EnemyFloat2 finalPolar;
  float local_2c8;
  float local_2c4;
  float local_2bc;
  PlayerFloat3 vectorScratch;
  float local_2a8;
  float local_2a4;
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
  case ENEMY_ECL_CREATE_ENEMY_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0;
    }
  case ENEMY_ECL_CREATE_ENEMY:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
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
      vectorScratch.x = (float)(fVar19 + fVar9);
      vectorScratch.y = ReadRawFloatArgument(
          2, *(float *)(iVar27 + 0x14 + iVar26 * 4)) + fVar10;
      vectorScratch.z = *(float *)((int)runtimeAddress + 0x34);
      EnemyPrepareProjection(&g_MainSupervisorView, 0);
      D3DXVec3Project(&local_90.spawnRequest.position, &vectorScratch,
                      &g_EnemyViewport, &g_EnemyProjectionMatrix,
                      &g_EnemyViewMatrix, &g_EnemyWorldMatrix);
      local_90.spawnRequest.position.x = local_90.spawnRequest.position.x - 224.0f;
      local_90.spawnRequest.position.z = 0.0;
      fVar19 = local_90.spawnRequest.position.y - 16.0f;
    }
    local_90.spawnRequest.position.y = (float)fVar19;
    uVar22 = ReadRawIntArgument(3, *(int *)(iVar27 + 0x18 + iVar26 * 4));
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
dispatch_spawn_enemy:
    EnemySpawn(
        g_EnemyManager, reinterpret_cast<const char *>(iVar27 + 0x14),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0;
    }
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE:
dispatch_create_enemy_absolute:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
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
    local_90.spawnRequest.eclVariables = eclVariables;
    goto dispatch_spawn_enemy;
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET:
dispatch_create_enemy_global:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    local_90.spawnRequest.position.x =
        ReadRawFloatArgument(
            1, *(float *)(iVar27 + 0x10 + iVar26 * 4)) +
        g_EnemyGlobalPositionOffset.x;
    local_90.spawnRequest.position.y =
        ReadRawFloatArgument(
            2, *(float *)(iVar27 + 0x14 + iVar26 * 4)) +
        g_EnemyGlobalPositionOffset.y;
    local_90.spawnRequest.position.z =
        ReadRawFloatArgument(
            3, *(float *)(iVar27 + 0x18 + iVar26 * 4));
    uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(6, *(int *)(iVar27 + 0x24 + iVar26 * 4));
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
    local_90.spawnRequest.setFlag40000 = local_90.spawnRequest.setFlag40000 | 1;
    EnemySpawn(
        g_EnemyManager, reinterpret_cast<const char *>(iVar27 + 0x14),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) == 0) goto dispatch_create_enemy_global;
    break;
  case ENEMY_ECL_CREATE_ENEMY_MIRRORED:
dispatch_create_enemy_mirrored:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    fVar9 = *(float *)((int)runtimeAddress + 0x2c);
    iVar26 /= 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    local_90.spawnRequest.position.x =
        ReadRawFloatArgument(
            1, *(float *)(iVar27 + 0x10 + iVar26 * 4)) + fVar9;
    fVar10 = *(float *)((int)runtimeAddress + 0x30);
    local_90.spawnRequest.position.y =
        ReadRawFloatArgument(
            2, *(float *)(iVar27 + 0x14 + iVar26 * 4)) + fVar10;
    uVar22 = ReadRawIntArgument(3, *(int *)(iVar27 + 0x18 + iVar26 * 4));
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
    local_90.spawnRequest.setFlag0800 = 1;
    EnemySpawn(
        g_EnemyManager, reinterpret_cast<const char *>(iVar27 + 0x14),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED:
    goto dispatch_create_enemy_absolute_mirrored;
  case ENEMY_ECL_CREATE_ENEMY_MIRRORED_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) == 0) goto dispatch_create_enemy_mirrored;
    break;
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE_MIRRORED_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0;
    }
    goto dispatch_create_enemy_absolute_mirrored;
  case ENEMY_ECL_SELECT_ANM_RESOURCE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    *(int *)((int)runtimeAddress + 0xe8) = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_ANM_SCRIPT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->Release();
    if ((int)uVar22 < 0) {
      return 0;
    }
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          reinterpret_cast<AnmLoadedView *>(
              g_EnemyManager->effectResources[value0E8])->
              CreateVmVariant2((int)uVar22, 5);
      managedVmIds[iVar26] = createdVm.value;
    }
    uVar6 = managedVmIds[iVar26];
    if (iVar26 == 0) {
      uVar22 = ENEMY_READ_INT_DIRECT(1);
      *(int *)((int)runtimeAddress + 0xf0) = (int)uVar22;
      *(unsigned int *)((int)runtimeAddress + 0xec) = *(unsigned int *)((int)runtimeAddress + 0xe8);
      uVar6 = managedVmIds[iVar26];
    }
    iVar27 = reinterpret_cast<int>(
        reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->GetVm());
    goto dispatch_update_primary_anm_bounds;
  case ENEMY_ECL_PLAY_ANM:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          reinterpret_cast<AnmLoadedView *>(
              g_EnemyManager->effectResources[(int)uVar22])->
              CreateVmVariant2((int)uVar23, 6);
      iVar26 = reinterpret_cast<int>(createdVm.GetVm());
    }
    if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          EnemyGetAnmPosition(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position)));
      *(unsigned int *)(iVar26 + 0x340) = *puVar5;
      *(unsigned int *)(iVar26 + 0x344) = puVar5[1];
      *(unsigned int *)(iVar26 + 0x348) = puVar5[2];
      return 0;
    }
    *(unsigned int *)(iVar26 + 0x340) = *(unsigned int *)((int)runtimeAddress + 0x2c);
    *(unsigned int *)(iVar26 + 0x344) = *(unsigned int *)((int)runtimeAddress + 0x30);
    *(unsigned int *)(iVar26 + 0x348) = *(unsigned int *)((int)runtimeAddress + 0x34);
    return 0;
  case ENEMY_ECL_PLAY_ANM_HIGH:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          reinterpret_cast<AnmLoadedView *>(
              g_EnemyManager->effectResources[(int)uVar22])->
              CreateVmVariant0((int)uVar23, 6);
      iVar26 = reinterpret_cast<int>(createdVm.GetVm());
    }
    if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          EnemyGetAnmPosition(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position)));
      *(unsigned int *)(iVar26 + 0x340) = *puVar5;
      *(unsigned int *)(iVar26 + 0x344) = puVar5[1];
      *(unsigned int *)(iVar26 + 0x348) = puVar5[2];
      return 0;
    }
    *(unsigned int *)(iVar26 + 0x340) = *(unsigned int *)((int)runtimeAddress + 0x2c);
    *(unsigned int *)(iVar26 + 0x344) = *(unsigned int *)((int)runtimeAddress + 0x30);
    *(unsigned int *)(iVar26 + 0x348) = *(unsigned int *)((int)runtimeAddress + 0x34);
    return 0;
  case ENEMY_ECL_PLAY_ANM_ABSOLUTE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    reinterpret_cast<AnmLoadedView *>(
        g_EnemyManager->effectResources[(int)uVar22])->
        CreateVmVariant2((int)uVar23, 6);
    return 0;
  case ENEMY_ECL_PLAY_ANM_ROTATED:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    {
      AnmVmIdView createdVm =
          reinterpret_cast<AnmLoadedView *>(
              g_EnemyManager->effectResources[(int)uVar22])->
              CreateVmVariant2((int)uVar23, 6);
      iVar26 = reinterpret_cast<int>(createdVm.GetVm());
    }
    if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x40000) == 0) {
      puVar5 = reinterpret_cast<unsigned int *>(
          EnemyGetAnmPosition(reinterpret_cast<const AnmFloat3View *>(&worldMotion.position)));
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
  case ENEMY_ECL_SET_MAIN_ANM_SCRIPT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    iVar27 = (int)uVar22;
    reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->Release();
    {
      AnmVmIdView createdVm =
          reinterpret_cast<AnmLoadedView *>(
              g_EnemyManager->effectResources[value0E8])->
              CreateVmVariant2(iVar27, 5);
      managedVmIds[iVar26] = createdVm.value;
    }
    uVar6 = managedVmIds[iVar26];
    iVar11 = reinterpret_cast<int>(
        reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->GetVm());
    if (iVar26 == 0) {
      *(float *)((int)runtimeAddress + 0x13a4) = *(float *)(iVar11 + 0x50) * *(float *)(iVar11 + 0x40);
      *(float *)((int)runtimeAddress + 0x13a8) = *(float *)(iVar11 + 0x4c) * *(float *)(iVar11 + 0x3c);
    }
    if ((*(unsigned char *)((int)runtimeAddress + 0x1444) & 0x10) != 0) {
      reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->ClearFlag2();
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
  case ENEMY_ECL_PLAY_SELECTED_ANM:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->Release();
    {
      AnmVmIdView createdVm =
          reinterpret_cast<AnmLoadedView *>(
              g_EnemyManager->effectResources[value0E8])->
              CreateVmVariant2(animationBaseScript + 5, 5);
      managedVmIds[iVar26] = createdVm.value;
    }
    iVar27 = reinterpret_cast<int>(
        reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->GetVm());
dispatch_update_primary_anm_bounds:
    if (iVar26 == 0) {
      *(float *)((int)runtimeAddress + 0x13a4) = *(float *)(iVar27 + 0x50) * *(float *)(iVar27 + 0x40);
      *(float *)((int)runtimeAddress + 0x13a8) = *(float *)(iVar27 + 0x4c) * *(float *)(iVar27 + 0x3c);
    }
    if ((*(unsigned char *)((int)runtimeAddress + 0x1444) & 0x10) != 0) {
      reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->ClearFlag2();
      return 0;
    }
    break;
  // Movement and interpolation instructions.
  case ENEMY_ECL_SET_OFFSET_POSITION:
  case ENEMY_ECL_SET_BASE_POSITION:
  {
    selectedMotion = &offsetMotion;
    if (opcode != 0x118) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    if (-999999.0 < (float)fVar19) {
      selectedMotion->position.x = (float)fVar19;
    }
    if (fVar21 > -999999.0) {
      selectedMotion->position.y = (float)fVar21;
    }
    selectedMotion->flags &= 0xfffffffe;
    *reinterpret_cast<AnmFloat3View *>(&worldMotion.position) =
        AnmFloat3View(baseMotion.position.x + offsetMotion.position.x,
                      baseMotion.position.y + offsetMotion.position.y,
                      baseMotion.position.z + offsetMotion.position.z);
    return 0;
  }
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
  case ENEMY_ECL_INTERPOLATE_OFFSET_POSITION:
  case ENEMY_ECL_INTERPOLATE_BASE_POSITION:
    selectedMotion = &offsetMotion;
    if (opcode != ENEMY_ECL_INTERPOLATE_OFFSET_POSITION) {
      selectedMotion = &baseMotion;
      positionInterpolation = &positionInterpolation1;
    }
    else {
      positionInterpolation = &positionInterpolation0;
    }
    positionInterpolationValues = reinterpret_cast<float *>(positionInterpolation);
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    positionInterpolation->duration = uVar22;
    positionInterpolation->initialTangent = g_EnemyInterpolationOrigin;
    positionInterpolation->finalTangent = g_EnemyInterpolationOrigin;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    positionInterpolation->mode = uVar22;
    positionInterpolation->initial = selectedMotion->position;
    if (-999999.0 < local_2c4) {
      vectorScratch.y = local_2c4;
    }
    else {
      vectorScratch.y = selectedMotion->position.y;
    }
    if (-999999.0 < local_2c8) {
      vectorScratch.x = local_2c8;
    }
    else {
      vectorScratch.x = selectedMotion->position.x;
    }
    vectorScratch.z = 0.0;
    positionInterpolation->final = vectorScratch;
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
    if (-999999.0 < local_2bc) {
      if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x800) != 0) {
        fVar21 = EnemyWrapAngle(local_2bc - 1.5707964f);
        fVar21 = EnemyWrapAngle((float)(1.5707964f - fVar21));
        local_2bc = (float)fVar21;
      }
      EnemySetMotionAngle(selectedMotion, local_2bc);
    }
    if (-999999.0 < (float)fVar19) {
      selectedMotion->value18 = (float)fVar19;
    }
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_POLAR:
  case ENEMY_ECL_INTERPOLATE_BASE_POLAR:
  {
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolation0;
    if (opcode == 0x11d) {
      firstScalarInterpolation = &scalarInterpolation0;
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolation1;
    }
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    firstScalarInterpolation->mode = uVar22;

    if (uVar22 != 7) {
      if (!(-999999.0 < local_2c8)) {
        finalPolar.x = selectedMotion->value1C;
      }
      else {
        finalPolar.x = local_2c8;
        if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x800) != 0) {
          fVar19 = EnemyWrapAngle((float)(finalPolar.x - 1.5707964f));
          finalPolar.x = (float)EnemyWrapAngle((float)(1.5707964f - fVar19));
        }
      }
      if (!(-999999.0 < local_2c4)) {
        finalPolar.y = selectedMotion->value18;
      }
      else {
        finalPolar.y = local_2c4;
      }
    }
    else {
      if (-999999.0 < local_2c8) {
        finalPolar.x = local_2c8;
      }
      else {
        finalPolar.x = 0.0;
      }
      if (-999999.0 < local_2c4) {
        finalPolar.y = local_2c4;
      }
      else {
        finalPolar.y = 0.0;
      }
    }

    initialPolar.x = selectedMotion->value1C;
    initialPolar.y = selectedMotion->value18;
    if (3.1415927f <= static_cast<float>(fabs(initialPolar.x - finalPolar.x))) {
      if (initialPolar.x < finalPolar.x) {
        initialPolar.x = initialPolar.x + 6.2831855f;
      }
      else {
        finalPolar.x = finalPolar.x + 6.2831855f;
      }
    }
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent.x = g_EnemyInterpolationBasis.x;
    goto dispatch_initialize_polar_interpolation;
  }
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
    if (-999999.0 < (float)fVar19) {
      EnemySetMotionAngle(selectedMotion, (float)fVar19);
    }
    if (-999999.0 < (float)fVar21) {
      selectedMotion->value18 = (float)fVar21;
    }
    if (-999999.0 < local_2a8) {
      selectedMotion->value20 = local_2a8;
    }
    if (-999999.0 < (float)fVar20) {
      selectedMotion->value24 = (float)fVar20;
    }
    selectedMotion->flags |= 1;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_CIRCLE:
  case ENEMY_ECL_INTERPOLATE_BASE_CIRCLE:
    selectedMotion = &offsetMotion;
    if (opcode != 0x121) {
      selectedMotion = &baseMotion;
    }
    firstScalarInterpolation = &scalarInterpolation0;
    if (opcode == 0x121) {
      secondScalarInterpolation = &scalarInterpolation2;
    }
    else {
      firstScalarInterpolation = &scalarInterpolation1;
      secondScalarInterpolation = &scalarInterpolation3;
    }
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    local_2bc = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));

    if (-999999.0 < local_2c4) {
      local_298 = local_2c4;
    }
    else {
      local_298 = selectedMotion->value18;
    }
    if (-999999.0 < local_2c8) {
      local_29c = local_2c8;
    }
    else {
      local_29c = selectedMotion->value1C;
    }

    local_2a4 = selectedMotion->value18;
    local_2a8 = selectedMotion->value1C;

    if (!(fVar19 > -999999.0)) {
      fVar19 = selectedMotion->value24;
    }
    if (-999999.0 < local_2bc) {
      vectorScratch.x = local_2bc;
    }
    else {
      vectorScratch.x = selectedMotion->value20;
    }
    vectorScratch.y = (float)fVar19;
    uVar6 = *reinterpret_cast<unsigned int *>(&selectedMotion->value24);
    uVar4 = *reinterpret_cast<unsigned int *>(&selectedMotion->value20);
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    firstScalarValues = reinterpret_cast<float *>(firstScalarInterpolation);
    firstScalarInterpolation->duration = (int)uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->initial.x = local_2a8;
    firstScalarInterpolation->final.y = local_298;
    firstScalarInterpolation->mode = (int)uVar23;
    firstScalarInterpolation->initial.y = local_2a4;
    firstScalarInterpolation->final.x = local_29c;
    EnemyInitializeScalarInterpolation(firstScalarInterpolation);
    secondScalarValues = reinterpret_cast<float *>(secondScalarInterpolation);
    secondScalarInterpolation->duration = (int)uVar22;
    secondScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    secondScalarInterpolation->finalTangent = g_EnemyInterpolationBasis;
    *reinterpret_cast<unsigned int *>(&secondScalarInterpolation->initial.x) = uVar4;
    secondScalarInterpolation->final.y = vectorScratch.y;
    secondScalarInterpolation->mode = (int)uVar23;
    *reinterpret_cast<unsigned int *>(&secondScalarInterpolation->initial.y) = uVar6;
    secondScalarInterpolation->final.x = vectorScratch.x;
    EnemyInitializeScalarInterpolation(secondScalarInterpolation);
    selectedMotion->velocity = selectedMotion->position;
    selectedMotion->flags |= 1;
    return 0;
  // Movement and game-state instructions in target case order.
  case ENEMY_ECL_ADD_OFFSET_VELOCITY:
  case ENEMY_ECL_ADD_BASE_VELOCITY:
    iVar26 = (int)runtimeAddress + 0x58;
    if (opcode != 0x12a) {
      iVar26 = (int)runtimeAddress + 0x84;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    if (-999999.0 < (float)fVar19) {
      *(float *)(iVar26 + 0xc) = (float)fVar19;
    }
    if (fVar21 > -999999.0) {
      *(float *)(iVar26 + 0x10) = (float)fVar21;
    }
    goto dispatch_copy_player_to_motion;
  case ENEMY_ECL_COPY_PLAYER_TO_OFFSET:
    goto dispatch_copy_player_to_motion;
  case ENEMY_ECL_COPY_PLAYER_TO_BASE:
    iVar26 = *(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10);
    baseMotion.position =
        *reinterpret_cast<PlayerFloat3 *>(iVar26 + 0x1068);
    return 0;
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
  case ENEMY_ECL_SET_BULLET_PROTECTION_RANGE:
    fVar19 = (ReadFloatArgument(0));
    *(float *)((int)runtimeAddress + 0x1454) = (float)(fVar19 * fVar19);
    break;
  case ENEMY_ECL_MOVE_RANDOM:
  case ENEMY_ECL_MOVE_RANDOM_BASE:
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolation0;
    if (opcode == 0x124) {
      firstScalarInterpolation = &scalarInterpolation0;
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolation1;
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
        fVar19 = fVar19 * 3.1415927f * 0.5f;
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
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    firstScalarInterpolation->mode = uVar22;
    finalPolar.y = 0.0;
    finalPolar.x = local_2bc;
    initialPolar.x = local_2bc;
    fVar19 = (ReadFloatArgument(2));
    initialPolar.y = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent.x = g_EnemyInterpolationBasis.x;
dispatch_initialize_polar_interpolation:
    firstScalarInterpolation->finalTangent.y = g_EnemyInterpolationBasis.y;
    firstScalarInterpolation->initial = initialPolar;
    firstScalarInterpolation->final = finalPolar;
    EnemyInitializeScalarInterpolation(firstScalarInterpolation);
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_SET_LIFE_MARKER:
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    fVar19 = (ReadFloatArgument(1));
    local_2a8 = (float)fVar19;
    iVar26 = *(int *)((int)runtimeAddress + 0x13c4);
    uVar23 = ENEMY_READ_INT_DIRECT(0);
    g_GuiView->bossGaugeValues[(int)uVar23].color = (unsigned int)uVar22;
    g_GuiView->bossGaugeValues[(int)uVar23].value = local_2a8 / (float)iVar26;
    return 0;
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
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar8 = *(unsigned int *)((int)runtimeAddress + 0x1444) | (unsigned int)uVar22;
    *(unsigned int *)((int)runtimeAddress + 0x1444) = uVar8;
    if ((uVar8 & 0x10) != 0) {
      for (iVar26 = 0; iVar26 < 10; ++iVar26)
        reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->ClearFlag2();
      return 0;
    }
    break;
  case ENEMY_ECL_CLEAR_FLAGS:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar8 = *(unsigned int *)((int)runtimeAddress + 0x1444) & ~(unsigned int)uVar22;
    *(unsigned int *)((int)runtimeAddress + 0x1444) = uVar8;
    if ((uVar8 & 0x10) == 0) {
      for (iVar26 = 0; iVar26 < 10; ++iVar26)
        reinterpret_cast<AnmVmIdView *>(&managedVmIds[iVar26])->SetFlag2();
      return 0;
    }
    break;
  case ENEMY_ECL_CLEAR_ITEM_DROPS:
    memset(itemDropCounts, 0, sizeof(itemDropCounts) + sizeof(unknown13FC));
    return 0;
  case ENEMY_ECL_ADD_ITEM_DROP:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if ((char)((unsigned int)*(unsigned int *)((int)runtimeAddress + 0x1444) >> 8) < '\0') {
      if (iVar26 == 1) {
        iVar26 = 10;
      }
      else if (iVar26 == 4) {
        iVar26 = 0xb;
      }
    }
    uVar22 = ENEMY_READ_INT_DIRECT(1);
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
    uVar22 = ENEMY_READ_INT_DIRECT(0);
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
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    *(int *)((int)runtimeAddress + 0x13c0) = (int)uVar22;
    *(int *)((int)runtimeAddress + 0x13c4) = (int)uVar22;
    g_GuiView->bossGaugeValues[0].value = 0.0f;
    g_GuiView->bossGaugeValues[0].color = 0;
    g_GuiView->bossGaugeValues[1].value = 0.0f;
    g_GuiView->bossGaugeValues[1].color = 0;
    g_GuiView->bossGaugeValues[2].value = 0.0f;
    g_GuiView->bossGaugeValues[2].color = 0;
    g_GuiView->bossGaugeValues[3].value = 0.0f;
    g_GuiView->bossGaugeValues[3].color = 0;
    *(unsigned int *)((int)runtimeAddress + 0x13c8) = *(unsigned int *)((int)runtimeAddress + 0x13c0);
    return 0;
  case ENEMY_ECL_SET_BOSS_SLOT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar27 = (int)uVar22;
    if (iVar27 < 0) {
      if ((char)(flags >> 8) < 0) {
        g_EnemyManager->specialEnemySlots[managerSlot] = 0;
      }
      flags &= 0xffff7fff;
      return 0;
    }
    uVar4 = flags;
    iVar26 = reinterpret_cast<int>(g_EnemyManager);
    uVar4 |= 0x8000;
    flags = uVar4;
    reinterpret_cast<EnemyManagerView *>(iVar26)->specialEnemySlots[iVar27] = owner;
    managerSlot = iVar27;
    return 0;
  case ENEMY_ECL_SET_INVULNERABILITY:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    damageReductionTimer.SetCurrent((int)uVar22);
    return 0;
  case ENEMY_ECL_SET_PLAYER_COLLISION_TIMER:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    playerCollisionTimer.SetCurrent((int)uVar22);
    return 0;
  case ENEMY_ECL_KILL_ALL_ENEMIES:
    EnemyKillAll(g_EnemyManager);
    return 0;
  case ENEMY_ECL_SELECT_INT_BY_DIFFICULTY:
    switch(g_EnemyDifficulty) {
    case 0:
      uVar22 = ENEMY_READ_INT_DIRECT(1);
      break;
    case 1:
      uVar22 = ENEMY_READ_INT_DIRECT(2);
      break;
    case 2:
      uVar22 = ENEMY_READ_INT_DIRECT(3);
      break;
    case 3:
    case 4:
      uVar22 = ENEMY_READ_INT_DIRECT(4);
      break;
    default:
      goto dispatch_complete;
    }
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    *puVar5 = (int)uVar22;
    return 0;
  case ENEMY_ECL_SELECT_FLOAT_BY_DIFFICULTY:
    switch(g_EnemyDifficulty) {
    case 0:
      fVar19 = ReadFloatArgument(1);
      goto dispatch_store_float_result;
    case 1:
      fVar21 = ReadFloatArgument(2);
      goto dispatch_store_float_result_a;
    case 2:
dispatch_difficulty_float_index_3:
      fVar19 = ReadFloatArgument(3);
      goto dispatch_store_float_result;
    case 3:
dispatch_difficulty_float_index_4_a:
      fVar21 = ReadFloatArgument(4);
      goto dispatch_store_float_result_a;
    case 4:
      fVar19 = ReadFloatArgument(4);
      goto dispatch_store_float_result;
    default:
      goto dispatch_complete;
    }
  case ENEMY_ECL_INITIALIZE_BULLET_PATTERN:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
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
  case ENEMY_ECL_COPY_BULLET_PATTERN:
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    uVar23 = ENEMY_READ_INT_DIRECT(0);
    *reinterpret_cast<EnemyBulletPatternCopyView *>(
        (int)runtimeAddress + 0x2c4 + (int)uVar23 * 0x210) =
        *reinterpret_cast<const EnemyBulletPatternCopyView *>(
            (int)runtimeAddress + 0x2c4 + (int)uVar22 * 0x210);
    return 0;
  case ENEMY_ECL_FIRE_BULLET_PATTERN:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar27 = (int)uVar22 * 3 + 0x4d1;
    vectorScratch.x = *(float *)((int)runtimeAddress + iVar27 * 4) + *(float *)((int)runtimeAddress + 0x2c);
    vectorScratch.y = *(float *)((int)runtimeAddress + 4 + iVar27 * 4) + *(float *)((int)runtimeAddress + 0x30);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    vectorScratch.z = *(float *)((int)runtimeAddress + iVar27 * 4 + 8) + *(float *)((int)runtimeAddress + 0x34);
    *(float *)(iVar26 + 0x2c8) = vectorScratch.x;
    *(float *)(iVar26 + 0x2cc) = vectorScratch.y;
    fVar9 = 0.0f;
    *(float *)(iVar26 + 0x2d0) = vectorScratch.z;
    if ((*(float *)((int)runtimeAddress + 0x1454) == fVar9) ||
       (fVar9 = *(float *)(iVar26 + 0x2cc) - *(float *)(reinterpret_cast<int>(g_Player) + 0x3c4),
       fVar10 = *(float *)(iVar26 + 0x2c8) - *(float *)(reinterpret_cast<int>(g_Player) + 0x3c0),
       fVar9 = fVar9 * fVar9 + fVar10 * fVar10,
       *(float *)((int)runtimeAddress + 0x1454) <= fVar9)) {
      reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->
          SpawnBulletPattern(reinterpret_cast<BulletSpawnDescriptorView *>(
              iVar26 + 0x2c4));
      return 0;
    }
    break;
  case ENEMY_ECL_SET_BULLET_SPRITES:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    *(short *)(iVar26 + 0x2c4) = (short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    *(short *)(iVar26 + 0x2c6) = (short)uVar22;
    return 0;
  case ENEMY_ECL_SET_BULLET_OFFSET:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    fVar19 = (ReadFloatArgument(1));
    *(float *)((int)runtimeAddress + ((int)uVar22 * 3 + 0x4d1) * 4) = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    *(float *)((int)runtimeAddress + 0x1348 + (int)uVar22 * 0xc) = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_BULLET_ANGLE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    fVar19 = (ReadFloatArgument(1));
    *(float *)(iVar26 + 0x2d4) = (float)fVar19;
    fVar19 = (ReadFloatArgument(2));
    *(float *)(iVar26 + 0x2d8) = (float)fVar19;
    return 0;
  case ENEMY_ECL_SET_BULLET_SPEED:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    goto dispatch_bullet_speed_lowest;
  case ENEMY_ECL_SET_BULLET_COUNT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    *(short *)(iVar26 + 0x4b8) = (short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    *(short *)(iVar26 + 0x4ba) = (short)uVar22;
    return 0;
  case ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY:
  {
    int speedIndex;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    if (g_EnemyDifficulty == 0) {
      speedIndex = 1;
    }
    else if (g_EnemyDifficulty == 1) {
      speedIndex = 2;
    }
    else {
      speedIndex = 3;
      if (g_EnemyDifficulty != 2) {
        speedIndex = 4;
      }
    }
    fVar19 = ReadFloatArgument(speedIndex);
    iVar26 = (int)uVar22 * 0x210;
    *(float *)(iVar26 + 0x2dc + (int)runtimeAddress) = (float)fVar19;
    if (g_EnemyDifficulty == 0) {
      speedIndex = 5;
    }
    else if (g_EnemyDifficulty == 1) {
      speedIndex = 6;
    }
    else {
      speedIndex = 7;
      if (g_EnemyDifficulty != 2) {
        speedIndex = 8;
      }
    }
    fVar19 = ReadFloatArgument(speedIndex);
    *(float *)(iVar26 + (int)runtimeAddress + 0x2e0) = (float)fVar19;
    return 0;
  }
  case ENEMY_ECL_SET_BULLET_COUNT_BY_DIFFICULTY:
  {
    int countIndex;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    if (g_EnemyDifficulty == 0) {
      countIndex = 1;
    }
    else if (g_EnemyDifficulty == 1) {
      countIndex = 2;
    }
    else {
      countIndex = 3;
      if (g_EnemyDifficulty != 2) {
        countIndex = 4;
      }
    }
    uVar23 = ENEMY_READ_INT_DIRECT(countIndex);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    *(short *)(iVar26 + 0x4b8) = (short)uVar23;
    if (g_EnemyDifficulty == 0) {
      countIndex = 5;
    }
    else if (g_EnemyDifficulty == 1) {
      countIndex = 6;
    }
    else {
      countIndex = 7;
      if (g_EnemyDifficulty != 2) {
        countIndex = 8;
      }
    }
    uVar22 = ENEMY_READ_INT_DIRECT(countIndex);
    *(short *)(iVar26 + 0x4ba) = (short)uVar22;
    return 0;
  }
  case ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_3:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 0x200) {
dispatch_bullet_speed_high:
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(5));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(6));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    iVar27 = g_EnemyChapterState.rank + 0x200;
    goto dispatch_select_bullet_speed_low;
  case ENEMY_ECL_SET_BULLET_SPEED_BY_RANK_5:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 600) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(9));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(10));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    if (g_EnemyChapterState.rank >= 200) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(7));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(8));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    if (g_EnemyChapterState.rank >= -200) goto dispatch_bullet_speed_high;
    iVar27 = g_EnemyChapterState.rank + 600;
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
  {
    int patternIndex = ENEMY_READ_INT_DIRECT(0);
    float lowSpeedA = ReadFloatArgument(1);
    float lowSpeedB = ReadFloatArgument(2);
    float highSpeedA = ReadFloatArgument(3);
    float highSpeedB = ReadFloatArgument(4);
    bulletPatterns[patternIndex].fields.speedA =
         (highSpeedA - lowSpeedA) * ((float)g_EnemyChapterState.rank + 1024.0f) *
         0.00048828125f + lowSpeedA;
    bulletPatterns[patternIndex].fields.speedB =
         (highSpeedB - lowSpeedB) * ((float)g_EnemyChapterState.rank + 1024.0f) *
         0.00048828125f + lowSpeedB;
    return 0;
  }
  case ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_3:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 0x200) {
dispatch_bullet_count_high:
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ENEMY_READ_INT_DIRECT(5);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      uVar22 = ENEMY_READ_INT_DIRECT(6);
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    iVar27 = g_EnemyChapterState.rank + 0x200;
    goto dispatch_select_bullet_count_low;
  case ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_5:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyChapterState.rank >= 600) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ENEMY_READ_INT_DIRECT(9);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      {
        EclVmHost *const countHost = reinterpret_cast<EclVmHost *>(owner);
        uVar22 = countHost->activeContext->ReadInt(10);
      }
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    if (g_EnemyChapterState.rank >= 200) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ENEMY_READ_INT_DIRECT(7);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      {
        EclVmHost *const countHost = reinterpret_cast<EclVmHost *>(owner);
        uVar22 = countHost->activeContext->ReadInt(8);
      }
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    if (g_EnemyChapterState.rank >= -200) goto dispatch_bullet_count_high;
    iVar27 = g_EnemyChapterState.rank + 600;
dispatch_select_bullet_count_low:
    iVar11 = *(int *)((int)runtimeAddress + 0x14d8);
    iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
    if (0 <= iVar27) {
      uVar22 = ENEMY_READ_INT_DIRECT(3);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      uVar22 = ENEMY_READ_INT_DIRECT(4);
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    *(short *)(iVar26 + 0x4b8) = (short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    *(short *)(iVar26 + 0x4ba) = (short)uVar22;
    return 0;
  case ENEMY_ECL_INTERPOLATE_BULLET_COUNT_BY_RANK:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    secondLowBulletCount = ENEMY_READ_INT_DIRECT(2);
    uVar24 = ENEMY_READ_INT_DIRECT(3);
    uVar25 = ENEMY_READ_INT_DIRECT(4);
    iVar27 = (int)uVar22 * 0x210;
    iVar26 = ((int)uVar24 - (int)uVar23) * (g_EnemyChapterState.rank + 0x400);
    *(short *)(iVar27 + 0x4b8 + (int)runtimeAddress) =
         (short)(iVar26 / 2048) + (short)uVar23;
    iVar26 = ((int)uVar25 - secondLowBulletCount) * (g_EnemyChapterState.rank + 0x400);
    *(short *)(iVar27 + (int)runtimeAddress + 0x4ba) =
         (short)(iVar26 / 2048) +
         (short)secondLowBulletCount;
    return 0;
  case ENEMY_ECL_SET_BULLET_AIM_MODE:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    *(short *)((int)uVar22 * 0x210 + 0x4bc + (int)runtimeAddress) = (short)uVar23;
    return 0;
  case ENEMY_ECL_SET_BULLET_SOUND:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22 * 0x210 + (int)runtimeAddress;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    *(int *)(iVar26 + 0x4c4) = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    *(int *)(iVar26 + 0x4c8) = (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_BULLET_EXTRA:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    iVar26 = (int)uVar22 * 0x16 + (int)uVar23;
    iVar27 = (int)runtimeAddress + iVar26 * 0x18;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    *(int *)(iVar27 + 0x2f8) = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(3);
    *(int *)(iVar27 + 0x2f4) = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(4);
    *(int *)(iVar27 + 0x2ec) = (int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(5);
    *(int *)(iVar27 + 0x2f0) = (int)uVar22;
    fVar19 = (ReadFloatArgument(6));
    *(float *)(iVar27 + 0x2e4) = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    *(float *)((int)runtimeAddress + (iVar26 * 3 + 0x5d) * 8) = (float)fVar19;
    return 0;
  case ENEMY_ECL_RESET_UPDATE_TIMER:
    updateTimer.SetCurrent(0);
    return 0;
  case ENEMY_ECL_SET_INTERRUPT:
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    uVar23 = ENEMY_READ_INT_DIRECT(1);
    uVar24 = ENEMY_READ_INT_DIRECT(0);
    EnemyConfigureInterrupt(
        owner, (int)uVar24, (int)uVar23, (int)uVar22,
        reinterpret_cast<const unsigned char *>(iVar27 + 0x20));
    return 0;
  case ENEMY_ECL_SET_TIMEOUT:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    *(int *)(((int)uVar22 + 0x24a) * 0x10 + *(int *)((int)runtimeAddress + 0x14d8)) = iVar27 + 0x18;
    return 0;
  case ENEMY_ECL_CANCEL_ALL_BULLETS:
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelAllBullets(1);
    EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager), 1);
    return 0;
  case ENEMY_ECL_PLAY_SOUND:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
        QueueSoundCue((int)uVar22, worldMotion.position.x);
    return 0;
  case ENEMY_ECL_SET_SCREEN_SHAKE:
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    uVar6 = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    uVar4 = (unsigned int)uVar22;
    uVar28 = 0;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    EnemySetScreenShake(1,(int)uVar22,uVar4,uVar6,uVar28);
    return 0;
  case ENEMY_ECL_READ_DIALOG:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    EnemyReadDialog(g_GuiView, (int)uVar22);
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelAllBullets(0);
    EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager), 0);
    EnemyKillAll(g_EnemyManager);
    return 0;
  case ENEMY_ECL_WAIT_DIALOG:
    if (g_GuiView->message != 0 &&
       g_GuiView->message->inputCooldown == 0) {
      return 0xffffffff;
    }
    break;
  case ENEMY_ECL_WAIT_BOSS_CLEAR:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0xffffffff;
    }
    break;
  case ENEMY_ECL_START_SPELL:
  case ENEMY_ECL_START_SPELL_2:
  case ENEMY_ECL_START_SPELL_DIFFICULTY:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2:
  {
    const unsigned char *encryptedName =
        reinterpret_cast<const unsigned char *>(iVar27 + 0x20);
    const int spellNameLength = *(int *)(iVar27 + 0x1c);
    iVar17 = 0;
    bVar3 = 0x77;
    local_29d = '\a';
    if (0 < spellNameLength) {
      do {
        local_90.bytes[iVar17] =
            encryptedName[iVar17] ^ bVar3;
        bVar3 = bVar3 + local_29d;
        local_29d = local_29d + '\x10';
        iVar17 = iVar17 + 1;
      } while (iVar17 < spellNameLength);
    }
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    switch (*(short *)(iVar27 + 4)) {
    case ENEMY_ECL_START_SPELL_DIFFICULTY:
      iVar26 += g_EnemyDifficulty;
      break;
    case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1:
      iVar26 += g_EnemyDifficulty - 1;
      break;
    case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2:
      iVar26 += g_EnemyDifficulty - 2;
      break;
    }
    ENEMY_READ_INT_DIRECT(2);
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    EnemyBeginSpell(reinterpret_cast<int>(g_EnemyGameState),iVar26,reinterpret_cast<char *>(local_90.bytes),(int)uVar22);
    return 0;
  }
  case ENEMY_ECL_END_SPELL:
    EnemyEndSpell(g_EnemyGameState);
    return 0;
  case ENEMY_ECL_CANCEL_LASERS:
    *(unsigned int *)(reinterpret_cast<int>(g_EnemyGameState) + 0x378c) = *(unsigned int *)(reinterpret_cast<int>(g_EnemyGameState) + 0x378c) | 8;
    return 0;
  case ENEMY_ECL_ENABLE_BOMB_SHIELD:
    EnemyEnableBombShield(g_EnemyGameState);
    return 0;
  case ENEMY_ECL_SET_GAME_SPEED_FLAG:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    *(unsigned int *)((int)runtimeAddress + 0x1444) =
         *(unsigned int *)((int)runtimeAddress + 0x1444) ^
         ((int)uVar22 << 0x13 ^ *(unsigned int *)((int)runtimeAddress + 0x1444)) & 0x80000;
    return 0;
  case ENEMY_ECL_FIRE_LASER_A:
  case ENEMY_ECL_FIRE_LASER_A_WITH_PATTERN:
    pfVar15 = reinterpret_cast<float *>(&local_288);
    for (iVar27 = 0x77; iVar27 != 0; iVar27 = iVar27 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    if (opcode == 0x1af) {
      memcpy(local_288.type0.patternWords,
             &bulletPatterns[0].words[8],
             sizeof(local_288.type0.patternWords));
    }
    vectorScratch.x = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    vectorScratch.y = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    vectorScratch.z = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    *reinterpret_cast<PlayerFloat3 *>(&local_288) = vectorScratch;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    reinterpret_cast<unsigned short *>(&local_288)[18] = (unsigned short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    reinterpret_cast<unsigned short *>(&local_288)[19] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(2));
    reinterpret_cast<float *>(&local_288)[3] = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    reinterpret_cast<float *>(&local_288)[8] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    reinterpret_cast<float *>(&local_288)[5] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    reinterpret_cast<float *>(&local_288)[4] = (float)fVar19;
    fVar19 = (ReadFloatArgument(6));
    reinterpret_cast<float *>(&local_288)[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    reinterpret_cast<float *>(&local_288)[7] = (float)fVar19;
    reinterpret_cast<unsigned int *>(&local_288)[10] |= 1;
    EnemyFireLaser(g_EnemyLaserManager, &local_288, 0);
    return 0;
  case ENEMY_ECL_FIRE_STRAIGHT_LASER:
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_WITH_PATTERN:
    pfVar15 = reinterpret_cast<float *>(&local_288);
    for (iVar26 = 0x7e; iVar26 != 0; iVar26 = iVar26 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    reinterpret_cast<float *>(&local_288)[11] = 8.0f;
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    if (*(short *)(iVar27 + 4) == 0x1b0) {
      memcpy(local_288.type1.patternWords,
             &bulletPatterns[0].words[8],
             sizeof(local_288.type1.patternWords));
    }
    vectorScratch.x = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    vectorScratch.y = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    vectorScratch.z = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    *reinterpret_cast<PlayerFloat3 *>(&local_288) = vectorScratch;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    reinterpret_cast<unsigned short *>(&local_288)[32] = (unsigned short)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    reinterpret_cast<unsigned short *>(&local_288)[33] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(3));
    reinterpret_cast<float *>(&local_288)[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    reinterpret_cast<float *>(&local_288)[9] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    reinterpret_cast<float *>(&local_288)[8] = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(6);
    reinterpret_cast<unsigned int *>(&local_288)[12] = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(7);
    reinterpret_cast<unsigned int *>(&local_288)[13] = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(8);
    reinterpret_cast<unsigned int *>(&local_288)[14] = (unsigned int)uVar22;
    uVar22 = ENEMY_READ_INT_DIRECT(9);
    reinterpret_cast<unsigned int *>(&local_288)[15] = (unsigned int)uVar22;
    fVar19 = (ReadFloatArgument(10));
    reinterpret_cast<float *>(&local_288)[10] = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(0xb);
    reinterpret_cast<unsigned int *>(&local_288)[17] = (unsigned int)uVar22 | 2;
    uVar6 = EnemyFireLaser(g_EnemyLaserManager, &local_288, 1);
    if (puVar5 != (unsigned int *)0x0) {
      *puVar5 = uVar6;
      return 0;
    }
    break;
  case ENEMY_ECL_FIRE_LASER:
  case ENEMY_ECL_FIRE_LASER_WITH_PATTERN:
    pfVar15 = reinterpret_cast<float *>(&local_288);
    for (iVar26 = 0x77; iVar26 != 0; iVar26 = iVar26 + -1) {
      *pfVar15 = 0.0;
      pfVar15 = pfVar15 + 1;
    }
    if (opcode == 0x1b1) {
      memcpy(
          reinterpret_cast<unsigned int *>(&local_288) + 11,
          reinterpret_cast<const void *>(runtimeAddress + 0x2e4),
          0x6c * sizeof(unsigned int));
    }
    vectorScratch.x = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    vectorScratch.y = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    vectorScratch.z = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    reinterpret_cast<float *>(&local_288)[0] = vectorScratch.x;
    reinterpret_cast<float *>(&local_288)[1] = vectorScratch.y;
    reinterpret_cast<float *>(&local_288)[2] = vectorScratch.z;
    uVar22 = ReadIntArgument(0);
    reinterpret_cast<unsigned short *>(&local_288)[18] = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(1);
    reinterpret_cast<unsigned short *>(&local_288)[19] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(2));
    reinterpret_cast<float *>(&local_288)[3] = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    reinterpret_cast<float *>(&local_288)[8] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    reinterpret_cast<float *>(&local_288)[5] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    reinterpret_cast<float *>(&local_288)[4] = (float)fVar19;
    fVar19 = (ReadFloatArgument(6));
    reinterpret_cast<float *>(&local_288)[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(7));
    reinterpret_cast<float *>(&local_288)[7] = (float)fVar19;
    reinterpret_cast<unsigned int *>(&local_288)[10] &= 0xfffffffe;
    EnemyFireLaser(g_EnemyLaserManager, &local_288, 0);
    return 0;
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_2:
  case ENEMY_ECL_FIRE_STRAIGHT_LASER_2_WITH_PATTERN:
    EnemyInitializeLaserConfig(&local_288);
    puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
    if (*(short *)(iVar27 + 4) == 0x1b2) {
      puVar14 = (unsigned int *)((int)runtimeAddress + 0x2e4);
      puVar13 = reinterpret_cast<unsigned int *>(&local_288) + 18;
      for (iVar26 = 0x6c; iVar26 != 0; iVar26 = iVar26 + -1) {
        *puVar13 = *puVar14;
        puVar14 = puVar14 + 1;
        puVar13 = puVar13 + 1;
      }
    }
    vectorScratch.x = *(float *)((int)runtimeAddress + 0x1344) + *(float *)((int)runtimeAddress + 0x2c);
    vectorScratch.y = *(float *)((int)runtimeAddress + 0x1348) + *(float *)((int)runtimeAddress + 0x30);
    vectorScratch.z = *(float *)((int)runtimeAddress + 0x134c) + *(float *)((int)runtimeAddress + 0x34);
    reinterpret_cast<float *>(&local_288)[0] = vectorScratch.x;
    reinterpret_cast<float *>(&local_288)[1] = vectorScratch.y;
    reinterpret_cast<float *>(&local_288)[2] = vectorScratch.z;
    uVar22 = ReadIntArgument(1);
    reinterpret_cast<unsigned short *>(&local_288)[32] = (unsigned short)uVar22;
    uVar22 = ReadIntArgument(2);
    reinterpret_cast<unsigned short *>(&local_288)[33] = (unsigned short)uVar22;
    fVar19 = (ReadFloatArgument(3));
    reinterpret_cast<float *>(&local_288)[6] = (float)fVar19;
    fVar19 = (ReadFloatArgument(4));
    reinterpret_cast<float *>(&local_288)[9] = (float)fVar19;
    fVar19 = (ReadFloatArgument(5));
    reinterpret_cast<float *>(&local_288)[8] = (float)fVar19;
    uVar22 = ReadIntArgument(6);
    reinterpret_cast<unsigned int *>(&local_288)[12] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(7);
    reinterpret_cast<unsigned int *>(&local_288)[13] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(8);
    reinterpret_cast<unsigned int *>(&local_288)[14] = (unsigned int)uVar22;
    uVar22 = ReadIntArgument(9);
    reinterpret_cast<unsigned int *>(&local_288)[15] = (unsigned int)uVar22;
    fVar19 = (ReadFloatArgument(10));
    reinterpret_cast<float *>(&local_288)[10] = (float)fVar19;
    uVar22 = ReadIntArgument(0xb);
    reinterpret_cast<unsigned int *>(&local_288)[17] = (unsigned int)uVar22 & 0xfffffffd;
    uVar6 = EnemyFireLaser(g_EnemyLaserManager, &local_288, 1);
    if (puVar5 != (unsigned int *)0x0) {
      *puVar5 = uVar6;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_OFFSET:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(2));
      fVar21 = (ReadFloatArgument(1));
      vectorScratch.x = (float)fVar21;
      vectorScratch.y = (float)fVar19;
      vectorScratch.z = 0.0f;
      *reinterpret_cast<PlayerFloat3 *>(iVar26 + 0x24) = vectorScratch;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_ANGLE:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(2));
      fVar21 = (ReadFloatArgument(1));
      vectorScratch.x = (float)fVar21;
      vectorScratch.z = 0.0;
      vectorScratch.y = (float)fVar19;
      EnemySetLaserPosition(
          iVar26, reinterpret_cast<const PlayerFloat3 *>(&vectorScratch.x));
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_SPEED:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x48) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_COUNT:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x44) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_AIM:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x3c) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_SET_LASER_SOUND:
    uVar22 = ReadIntArgument(0);
    iVar26 = EnemyFindLaser(g_EnemyLaserManager, (int)uVar22);
    if (iVar26 != 0) {
      fVar19 = (ReadFloatArgument(1));
      *(float *)(iVar26 + 0x440) = (float)fVar19;
      return 0;
    }
    break;
  case ENEMY_ECL_CANCEL_BULLET_PATTERN:
    fVar19 = (ReadFloatArgument(0));
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelBulletPattern(
            &worldMotion.position, (float)fVar19, 1, 0);
    reinterpret_cast<EnemyEffectWaitManager *>(
        g_EnemyLaserManager)->ApplyPatternCancel(
            &worldMotion.position, (float)fVar19, 1);
    return 0;
  case ENEMY_ECL_CLEAR_BULLET_PATTERN:
    fVar19 = (ReadFloatArgument(0));
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelBulletPattern(
            &worldMotion.position, (float)fVar19, 0, 0);
    reinterpret_cast<EnemyEffectWaitManager *>(
        g_EnemyLaserManager)->ApplyPatternCancel(
            &worldMotion.position, (float)fVar19, 0);
    return 0;
  case ENEMY_ECL_SET_CHAPTER:
    uVar22 = ReadIntArgument(0);
    EnemySetChapter(&g_EnemyChapterState, (int)uVar22);
    return 0;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_3:
    if (g_EnemyChapterState.rank >= 0x200) {
      iVar27 = 2;
      goto dispatch_rank_float_tail_b;
    }
    iVar27 = 0;
    if (g_EnemyChapterState.rank > -0x200) goto dispatch_rank_float_tail_b;
    goto dispatch_rank_float_tail_a;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_5:
    if (g_EnemyChapterState.rank >= 600) {
      goto dispatch_difficulty_float_index_4_a;
    }
    if (g_EnemyChapterState.rank >= 200) {
      goto dispatch_difficulty_float_index_3;
    }
    if (g_EnemyChapterState.rank >= -200) {
      iVar27 = 2;
      goto dispatch_rank_float_tail_a;
    }
    if (g_EnemyChapterState.rank < -400) {
      iVar27 = 0;
      goto dispatch_rank_float_tail_a;
    }
    iVar27 = 1;
    goto dispatch_rank_float_tail_b;
dispatch_rank_float_tail_b:
    fVar19 = ReadFloatArgument(iVar27);
dispatch_store_float_result:
    // Keep this owner lifetime explicit: the shipped shared tail reuses EBX
    // as the full owner immediately before ResolveFloat.
    iVar26 = reinterpret_cast<int>(owner);
    pfVar15 = reinterpret_cast<EclVmContext *>(
        *reinterpret_cast<int *>(iVar26 + 4))->ResolveFloat(0);
    *pfVar15 = (float)fVar19;
    return 0;
dispatch_rank_float_tail_a:
    fVar21 = ReadFloatArgument(iVar27);
dispatch_store_float_result_a:
    positionInterpolationValues = ResolveFloatArgument(0);
    *positionInterpolationValues = (float)fVar21;
    return 0;
  case ENEMY_ECL_INTERPOLATE_FLOAT_BY_RANK:
    fVar19 = (ReadFloatArgument(1));
    fVar21 = (ReadFloatArgument(2));
    local_2a8 = (float)fVar21;
    pfVar15 = (float *)((int)ResolveFloatArgument(0));
    *pfVar15 = (local_2a8 - (float)fVar19) * ((float)g_EnemyChapterState.rank + 1024.0f) * 0.00048828125f +
               (float)fVar19;
    return 0;
  case ENEMY_ECL_SELECT_INT_BY_RANK_3:
    if (0x1ff < g_EnemyChapterState.rank) {
dispatch_rank_int_high:
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(2);
      *puVar5 = (int)uVar22;
      return 0;
    }
    goto dispatch_rank_int_low;
  case ENEMY_ECL_SELECT_INT_BY_RANK_5:
    if (599 < g_EnemyChapterState.rank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(4);
      *puVar5 = (int)uVar22;
      return 0;
    }
    if (199 < g_EnemyChapterState.rank) {
      puVar5 = (unsigned int *)((int)ResolveIntArgument(0));
      uVar22 = ReadIntArgument(3);
      *puVar5 = (int)uVar22;
      return 0;
    }
    if (-0xc9 < g_EnemyChapterState.rank) goto dispatch_rank_int_high;
    if (-0x191 < g_EnemyChapterState.rank) {
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
    iVar26 = ((int)uVar23 - (int)uVar22) * (g_EnemyChapterState.rank + 0x400);
    *piVar7 = (iVar26 / 2048) + (int)uVar22;
    return 0;
  case ENEMY_ECL_SET_SPELL_TIMEOUT:
    uVar22 = ReadIntArgument(0);
    g_GuiView->bossGaugeSegmentCount = (int)uVar22;
    return 0;
  case ENEMY_ECL_WAIT_FOR_EFFECT:
    EnemyWaitForEffect(
        reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager));
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
    g_AnmGameSpeed = (float)fVar19;
    return 0;
  case ENEMY_ECL_AIM_BULLET_AT_PLAYER:
    pfVar15 = (float *)(reinterpret_cast<int>(g_Player) + 0x3c0);
    fVar19 = (ReadFloatArgument(1));
    iVar26 = reinterpret_cast<int>(g_Player);
    fVar21 = (ReadFloatArgument(2));
    fVar19 = atan2(*(float *)(iVar26 + 0x3c4) - fVar21,
                   *pfVar15 - (float)fVar19);
    goto dispatch_store_float_result;
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
  }
dispatch_complete:
  return 0;
dispatch_create_enemy_absolute_mirrored:
  iVar26 = *(int *)(iVar27 + 0x10) + 4;
  iVar26 /= 4;
  memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
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
  local_90.spawnRequest.eclVariables = eclVariables;
  local_90.spawnRequest.setFlag0800 = 1;
  goto dispatch_spawn_enemy;
dispatch_copy_player_to_motion:
  iVar26 = *(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10);
  *(unsigned int *)((int)runtimeAddress + 0x58) = *(unsigned int *)(iVar26 + 0x1068);
  *(unsigned int *)((int)runtimeAddress + 0x5c) = *(unsigned int *)(iVar26 + 0x106c);
  *(unsigned int *)((int)runtimeAddress + 0x60) = *(unsigned int *)(iVar26 + 0x1070);
  return 0;
}
