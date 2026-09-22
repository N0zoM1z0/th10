#include "Enemy.hpp"
#include "EclVm.hpp"
#include "Main.hpp"
#include "Rng.hpp"

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

struct EnemyGameStateView
{
    unsigned char unknown000[0x774];
    int managedVmId;
    unsigned char unknown778[0x378c - 0x778];
    unsigned int flags378C;
};
typedef char EnemyGameStateVmIdAt774[
    (offsetof(EnemyGameStateView, managedVmId) == 0x774) ? 1 : -1];
typedef char EnemyGameStateFlagsAt378C[
    (offsetof(EnemyGameStateView, flags378C) == 0x378c) ? 1 : -1];
struct EnemyPrimaryResourceOwnerView
{
    unsigned char unknown000000[0x3e0b50];
    AnmLoadedView *primaryEnemyResource;
};

struct EnemyManagedVmView;
struct EnemyVisualStateView
{
    unsigned char unknown000[0x9eb8];
    void *activeDialog;
    unsigned char *dialogTable;
};
struct EnemyLaserRequestScratch;
struct EnemyBulletPatternCopyView
{
    unsigned int words[0x84];
};
typedef char EnemyBulletPatternCopyViewSizeIs210[
    (sizeof(EnemyBulletPatternCopyView) == 0x210) ? 1 : -1];

struct EnemyCancelManagerView
{
    unsigned char unknown000000[0x44];
    AnmFloat3View cancelCenter;
    AnmFloat3View cancelSize;
    unsigned int unknown05C;
    unsigned char bulletStorage[0x3e0af0];
    void *primaryEnemyResource;

    __declspec(noinline) unsigned int CancelAllBullets(int mode);
    __declspec(noinline) unsigned int CancelBulletPattern(
        const PlayerFloat3 *position, float radius, int mode, int flags);
};
typedef char EnemyCancelManagerCenterAt44[
    (offsetof(EnemyCancelManagerView, cancelCenter) == 0x44) ? 1 : -1];
typedef char EnemyCancelManagerSizeAt50[
    (offsetof(EnemyCancelManagerView, cancelSize) == 0x50) ? 1 : -1];
typedef char EnemyCancelManagerBulletsAt60[
    (offsetof(EnemyCancelManagerView, bulletStorage) == 0x60) ? 1 : -1];
typedef char EnemyCancelManagerResourceAt3E0B50[
    (offsetof(EnemyCancelManagerView, primaryEnemyResource) == 0x3e0b50) ? 1 : -1];

extern int g_EnemyDifficulty;
extern int g_EnemyRank;
extern EnemyManagerView *g_EnemyManager;
extern EnemyGameStateView *g_EnemyGameState;
extern EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;
extern EnemyVisualStateView *g_EnemyVisualState;
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

static __declspec(noinline) void EnemyConfigureViewportOwner(
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

static __declspec(noinline) void EnemyPrepareProjection(
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
extern unsigned int EnemyFireBulletPattern(int manager, int pattern, int owner);
extern void EnemySpawnItem(
    const PlayerFloat3 *position, int itemType, int owner,
    float angle, float speed);
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
    if (x + marginX <= g_EnemyPlayfieldMaxX &&
        g_EnemyPlayfieldMinX < x - marginX &&
        y + marginY <= g_EnemyPlayfieldMaxY &&
        g_EnemyPlayfieldMinY < y - marginY) {
        return 0;
    }
    return 1;
}

static __declspec(noinline) unsigned int EnemyCancelBulletRecord(
    unsigned char *bullet)
{
    short state = *reinterpret_cast<short *>(bullet + 0x446);
    if (state != 2 && state != 1) {
        return 0;
    }

    EnemyBulletPositionView *position =
        reinterpret_cast<EnemyBulletPositionView *>(bullet + 0x3b4);
    int outside = position->IsOutsidePlayfield(8.0f, 8.0f);
    *reinterpret_cast<short *>(bullet + 0x30c) = 1;
    *reinterpret_cast<short *>(bullet + 0x446) = 3;
    if (outside == 0) {
        int effectScript = *reinterpret_cast<int *>(bullet + 0x438);
        if (effectScript >= 0) {
            g_EnemyPrimaryResourceOwner->primaryEnemyResource->
                CreateVmAtWorldVariant0(
                    effectScript,
                    reinterpret_cast<const AnmFloat3View *>(
                        bullet + 0x3b4));
        }

        unsigned int *flags = reinterpret_cast<unsigned int *>(bullet + 0x408);
        if ((*flags & 1) == 0) {
            *reinterpret_cast<unsigned int *>(bullet + 0x3fc) = 0;
            *reinterpret_cast<unsigned int *>(bullet + 0x3f8) = 0xfff0bdc1u;
            *reinterpret_cast<unsigned int *>(bullet + 0x400) = 0;
            *reinterpret_cast<unsigned int *>(bullet + 0x404) =
                reinterpret_cast<unsigned int>(&g_AnmGameSpeed);
            *flags |= 1;
        }
        *reinterpret_cast<unsigned int *>(bullet + 0x3fc) = 0;
        *reinterpret_cast<unsigned int *>(bullet + 0x400) = 0;
        *reinterpret_cast<unsigned int *>(bullet + 0x3f8) = 0xffffffffu;
        return 1;
    }
    *reinterpret_cast<unsigned int *>(bullet) |= 8;
    return 1;
}

__declspec(noinline) unsigned int EnemyCancelManagerView::CancelBulletPattern(
    const PlayerFloat3 *position, float radius, int mode, int flags)
{
    unsigned char *bullet = bulletStorage;
    for (int remaining = 2000; remaining != 0;
         --remaining, bullet += 0x7f0) {
        short state = *reinterpret_cast<short *>(bullet + 0x446);
        if (state == 0 || state == 3) {
            continue;
        }
        if (flags != 0 && *reinterpret_cast<int *>(bullet + 4) != 0) {
            continue;
        }

        EnemyBulletPositionView *bulletPosition =
            reinterpret_cast<EnemyBulletPositionView *>(bullet + 0x3b4);
        float dx = bulletPosition->x - position->x;
        float dy = bulletPosition->y - position->y;
        float effectiveRadius =
            *reinterpret_cast<float *>(bullet + 0x3f0) *
                g_EnemyCancelHalfScale +
            radius;
        if (dx * dx + dy * dy <= effectiveRadius * effectiveRadius) {
            EnemyCancelBulletRecord(bullet);
            if (bulletPosition->IsOutsidePlayfield(2.0f, 2.0f) == 0 &&
                mode != 0) {
                EnemySpawnItem(
                    reinterpret_cast<PlayerFloat3 *>(bulletPosition),
                    8, -1, -1.5707964f, 0.6f);
            }
        }
    }
    return 0;
}
extern void __stdcall EnemyBeginSpell(int gameState, int spellId, char *name, int value);

__declspec(noinline) void EnemyMarkPendingInterrupt(int *id)
{
    AnmVmView *vm =
        g_AnmRenderManagerView->FindVm(AnmVmIdView(*id));
    if (vm == 0) {
        return;
    }

    vm->pendingInterrupt = 1;
    if (vm->layerNode.previous != 0) {
        return;
    }

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != 0) {
        static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
        node = node->next;
    }
}

extern void EnemyEndSpell(EnemyGameStateView *gameState);
static __declspec(noinline) void EnemyEnableBombShield(
    EnemyGameStateView *gameState)
{
    gameState->flags378C |= 0x10;
    g_AnmRenderManagerView->MarkVmForDeletion(gameState->managedVmId);
    gameState->managedVmId = 0;
}
extern void EnemyInitializeLaserConfig(EnemyLaserRequestScratch *config);
static __declspec(noinline) void EnemySetLaserPosition(
    int laser, const PlayerFloat3 *position)
{
    *reinterpret_cast<PlayerFloat3 *>(laser + 0x430) = *position;
}
struct EnemyLaserNodeView
{
    unsigned char unknown000[0x08];
    EnemyLaserNodeView *next;
    unsigned char unknown00C[0x54 - 0x0c];
    int id;
};
struct EnemyLaserManagerView
{
    unsigned char unknown000[0x18];
    EnemyLaserNodeView *head;
};
typedef char EnemyLaserNodeNextAt08[
    (offsetof(EnemyLaserNodeView, next) == 0x08) ? 1 : -1];
typedef char EnemyLaserNodeIdAt54[
    (offsetof(EnemyLaserNodeView, id) == 0x54) ? 1 : -1];
typedef char EnemyLaserManagerHeadAt18[
    (offsetof(EnemyLaserManagerView, head) == 0x18) ? 1 : -1];

extern EnemyLaserManagerView *g_EnemyLaserManager;

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
extern int EnemySpawnFromEclInstruction(
    EnemyManagerView *manager, const void *subroutineName,
    const EnemySpawnRequestView *request);
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
    unsigned char unknown000[0x44];
    int chapter;
    int unknown048;
    int chapterTimer;
};

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
extern void *__stdcall EnemyInitializeDialog(
    void *storage, const unsigned char *record);
extern int g_EnemyDialogIndex;
extern int g_EnemyDialogState;

static __declspec(noinline) void EnemyReadDialog(
    EnemyVisualStateView *visualState, int index)
{
    void *storage = ::operator new(0x90);
    void *dialog = 0;
    if (storage != 0) {
        unsigned char *table = visualState->dialogTable;
        int offset = *reinterpret_cast<int *>(table + 4 + index * 8);
        dialog = EnemyInitializeDialog(storage, table + offset);
    }
    visualState->activeDialog = dialog;
    *reinterpret_cast<int *>(dialog) = index;
    int nextIndex = index + 1;
    if (g_EnemyDialogIndex != nextIndex) {
        g_EnemyDialogState = 0;
    }
    g_EnemyDialogIndex = nextIndex;
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
extern unsigned int EnemyFireLaser(
    void *manager, EnemyLaserRequestScratch *request, int type);
struct EnemyEffectWaitNode
{
    virtual void UnknownVirtual0();
    virtual void UnknownVirtual1();
    virtual void UnknownVirtual2();
    virtual void UnknownVirtual3();
    virtual void UnknownVirtual4();
    virtual unsigned int ApplyClear(int mode);
    virtual void UnknownVirtual6();
    virtual unsigned int ApplyPatternCancel(
        const PlayerFloat3 *position, float radius, int mode);

    unsigned int unknown004;
    EnemyEffectWaitNode *next;
    int state;
    unsigned char unknown010[0x40];
    unsigned char flag50;
};

struct EnemyEffectWaitManager
{
    unsigned char unknown000[0x18];
    EnemyEffectWaitNode *head;
    unsigned char unknown01C[0x440 - 0x1c];
    PlayerFloat3 cancelPosition;

    __declspec(noinline) unsigned int ApplyPatternCancel(
        const PlayerFloat3 *position, float radius, int mode);
};

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

static __declspec(noinline) unsigned int EnemyApplyBulletClear(
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

struct EnemySoundQueueView
{
    unsigned char unknown000[0x408];
    int cueValues[134];
    int activeSoundIds[12];
    int sampleCounts[12];
    int samples[12][128];

    __declspec(noinline) void QueueSoundCue(int soundId, float positionX);
    __declspec(noinline) void QueueSoundSample(int soundId, int sample);
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

extern float __stdcall EnemyWrapAngle(float angle);

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

static __declspec(noinline) void EnemyDropItemCountsCore(
    const PlayerFloat3 *position, int *itemDropBlock)
{
    int *counts = itemDropBlock + 1;
    float angle = g_RngView.GetRandomF32Signed() * 3.1415927f;

    for (int itemIndex = 0; itemIndex < 11; ++itemIndex) {
        for (int i = 0; i < counts[itemIndex]; ++i) {
            EnemyDropVectorView offset;
            offset.FromAngleRadii(
                angle,
                reinterpret_cast<float *>(itemDropBlock)[13],
                reinterpret_cast<float *>(itemDropBlock)[14]);

            unsigned int radiusBits = g_RngView.GetRandomU32();
            float radiusScale = static_cast<float>(radiusBits);
            if (static_cast<int>(radiusBits) < 0) {
                radiusScale += 4294967296.0f;
            }
            radiusScale =
                radiusScale * 1.1641532182693481e-10f + 0.5f;

            PlayerFloat3 dropPosition;
            dropPosition.x = position->x + offset.x * radiusScale;
            dropPosition.y = position->y + offset.y * radiusScale;
            dropPosition.z = position->z;

            EnemySpawnItem(
                &dropPosition, itemIndex + 1, -1, -1.5707964f, 2.2f);

            unsigned int angleBits = g_RngView.GetRandomU32();
            float angleRandom = static_cast<float>(angleBits);
            if (static_cast<int>(angleBits) < 0) {
                angleRandom += 4294967296.0f;
            }
            angleRandom =
                angleRandom * 4.656612873077393e-10f - 1.0f;
            angle = EnemyWrapAngle(
                angleRandom * 0.7853982f + angle + 1.5707964f);
        }
    }

    memset(counts, 0, 12 * sizeof(int));
}

static __declspec(noinline) void EnemyDropItemCounts(
    const PlayerFloat3 *position, int *itemDropBlock)
{
    if (*itemDropBlock > 0) {
        EnemySpawnItem(
            position, *itemDropBlock, -1, -1.5707964f, 2.2f);
    }
    EnemyDropItemCountsCore(position, itemDropBlock);
    *itemDropBlock = 0;
}

extern float EnemyRandomAngle();

struct EnemyLaserRequestScratch
{
    unsigned int field000;
    unsigned int field001;
    unsigned int field002;
    unsigned int field003;
    unsigned int field004;
    unsigned int field005;
    unsigned int field006;
    unsigned int field007;
    unsigned int field008;
    unsigned int field009;
    unsigned int field010;
    unsigned int field011;
    unsigned int field012;
    unsigned int field013;
    unsigned int field014;
    unsigned int field015;
    unsigned int field016;
    unsigned int field017;
    unsigned int field018;
    unsigned int field019;
    unsigned int field020;
    unsigned int field021;
    unsigned int field022;
    unsigned int field023;
    unsigned int field024;
    unsigned int field025;
    unsigned int field026;
    unsigned int field027;
    unsigned int field028;
    unsigned int field029;
    unsigned int field030;
    unsigned int field031;
    unsigned int field032;
    unsigned int field033;
    unsigned int field034;
    unsigned int field035;
    unsigned int field036;
    unsigned int field037;
    unsigned int field038;
    unsigned int field039;
    unsigned int field040;
    unsigned int field041;
    unsigned int field042;
    unsigned int field043;
    unsigned int field044;
    unsigned int field045;
    unsigned int field046;
    unsigned int field047;
    unsigned int field048;
    unsigned int field049;
    unsigned int field050;
    unsigned int field051;
    unsigned int field052;
    unsigned int field053;
    unsigned int field054;
    unsigned int field055;
    unsigned int field056;
    unsigned int field057;
    unsigned int field058;
    unsigned int field059;
    unsigned int field060;
    unsigned int field061;
    unsigned int field062;
    unsigned int field063;
    unsigned int field064;
    unsigned int field065;
    unsigned int field066;
    unsigned int field067;
    unsigned int field068;
    unsigned int field069;
    unsigned int field070;
    unsigned int field071;
    unsigned int field072;
    unsigned int field073;
    unsigned int field074;
    unsigned int field075;
    unsigned int field076;
    unsigned int field077;
    unsigned int field078;
    unsigned int field079;
    unsigned int field080;
    unsigned int field081;
    unsigned int field082;
    unsigned int field083;
    unsigned int field084;
    unsigned int field085;
    unsigned int field086;
    unsigned int field087;
    unsigned int field088;
    unsigned int field089;
    unsigned int field090;
    unsigned int field091;
    unsigned int field092;
    unsigned int field093;
    unsigned int field094;
    unsigned int field095;
    unsigned int field096;
    unsigned int field097;
    unsigned int field098;
    unsigned int field099;
    unsigned int field100;
    unsigned int field101;
    unsigned int field102;
    unsigned int field103;
    unsigned int field104;
    unsigned int field105;
    unsigned int field106;
    unsigned int field107;
    unsigned int field108;
    unsigned int field109;
    unsigned int field110;
    unsigned int field111;
    unsigned int field112;
    unsigned int field113;
    unsigned int field114;
    unsigned int field115;
    unsigned int field116;
    unsigned int field117;
    unsigned int field118;
    unsigned int field119;
    unsigned int field120;
    unsigned int field121;
    unsigned int field122;
    unsigned int field123;
    unsigned int field124;
    unsigned int field125;
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

int EnemyRuntimeView::ReadIntArgument(int index)
{
    return reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ReadInt(index);
}

int *EnemyRuntimeView::ResolveIntArgument(int index)
{
    return reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ResolveInt(index);
}

float EnemyRuntimeView::ReadFloatArgument(int index)
{
    EclVmContext *context =
        reinterpret_cast<EclVmContext *>(owner->activeEclContext);
    return context->ReadFloat(index);
}

float *EnemyRuntimeView::ResolveFloatArgument(int index)
{
    return reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ResolveFloat(index);
}

int EnemyRuntimeView::ReadRawIntArgument(int index, int rawValue)
{
    return reinterpret_cast<EclVmContext *>(owner->activeEclContext)->ReadIntValue(index, rawValue);
}

float EnemyRuntimeView::ReadRawFloatArgument(int index, float rawValue)
{
    EclVmContext *context =
        reinterpret_cast<EclVmContext *>(owner->activeEclContext);
    return context->ReadFloatValue(index, rawValue);
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
  case ENEMY_ECL_CREATE_ENEMY_ABSOLUTE:
dispatch_create_enemy_absolute:
  iVar26 = *(int *)(iVar27 + 0x10) + 4;
  memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
  iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
  fVar19 = (ReadRawFloatArgument((1), (*(float *)(iVar27 + 0x10 + iVar26 * 4))));
  local_90.spawnRequest.position.x = (float)fVar19;
  fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
  goto dispatch_finish_spawn_position;
  case ENEMY_ECL_CREATE_ENEMY_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) != 0) {
      return 0;
    }
  case ENEMY_ECL_CREATE_ENEMY:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
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
      vectorScratch.x = (float)(fVar19 + fVar9);
      fVar19 = (ReadRawFloatArgument((2), (*(float *)(iVar27 + 0x14 + iVar26 * 4))));
      vectorScratch.z = *(float *)((int)runtimeAddress + 0x34);
      vectorScratch.y = (float)(fVar19 + fVar10);
      EnemyPrepareProjection(&g_MainSupervisorView, 0);
      D3DXVec3Project(&local_90.spawnRequest.position, &vectorScratch,
                      &g_EnemyViewport, &g_EnemyProjectionMatrix,
                      &g_EnemyViewMatrix, &g_EnemyWorldMatrix);
      local_90.spawnRequest.position.x = local_90.spawnRequest.position.x - 224.0f;
      local_90.spawnRequest.position.z = 0.0;
      fVar19 = local_90.spawnRequest.position.y - 16.0f;
    }
dispatch_finish_spawn_position:
    local_90.spawnRequest.position.y = (float)fVar19;
    uVar22 = ReadRawIntArgument(3, *(int *)(iVar27 + 0x18 + iVar26 * 4));
    local_90.spawnRequest.life = uVar22;
    uVar22 = ReadRawIntArgument(4, *(int *)(iVar27 + 0x1c + iVar26 * 4));
    local_90.spawnRequest.scoreReward = uVar22;
    uVar22 = ReadRawIntArgument(5, *(int *)(iVar27 + 0x20 + iVar26 * 4));
    local_90.spawnRequest.itemDropType = uVar22;
    local_90.spawnRequest.eclVariables = eclVariables;
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
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET:
dispatch_create_enemy_global:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
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
    EnemySpawnFromEclInstruction(
        g_EnemyManager, reinterpret_cast<const void *>(iVar27 + 0x14),
        &local_90.spawnRequest);
    return 0;
  case ENEMY_ECL_CREATE_ENEMY_AT_GLOBAL_OFFSET_IF_NO_BOSS:
    if (*(int *)(reinterpret_cast<int>(g_EnemyManager) + 0x10) == 0) goto dispatch_create_enemy_global;
    break;
  case ENEMY_ECL_CREATE_ENEMY_MIRRORED:
dispatch_create_enemy_mirrored:
    iVar26 = *(int *)(iVar27 + 0x10) + 4;
    fVar9 = *(float *)((int)runtimeAddress + 0x2c);
    memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
    iVar26 = (int)(iVar26 + (iVar26 >> 0x1f & 3U)) >> 2;
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
    EnemySpawnFromEclInstruction(
        g_EnemyManager, reinterpret_cast<const void *>(iVar27 + 0x14),
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
    selectedMotion = &offsetMotion;
    if (opcode != 0x118) {
      selectedMotion = &baseMotion;
    }
    fVar19 = (ReadFloatArgument(0));
    fVar21 = (ReadFloatArgument(1));
    if (0.0 < (float)fVar19) {
      selectedMotion->position.x = (float)fVar19;
    }
    if (0.0 < fVar21) {
      selectedMotion->position.y = (float)fVar21;
    }
    selectedMotion->flags &= 0xfffffffe;
    *reinterpret_cast<AnmFloat3View *>(&worldMotion.position) =
        *reinterpret_cast<const AnmFloat3View *>(&baseMotion.position) +
        *reinterpret_cast<const AnmFloat3View *>(&offsetMotion.position);
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
    if (local_2c4 <= 0.0) {
      vectorScratch.y = selectedMotion->position.y;
    }
    else {
      vectorScratch.y = local_2c4;
    }
    if (local_2c8 <= 0.0) {
      vectorScratch.x = selectedMotion->position.x;
    }
    else {
      vectorScratch.x = local_2c8;
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
    if (0.0 < local_2bc) {
      if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x800) != 0) {
        fVar21 = EnemyWrapAngle(local_2bc - 1.5707964f);
        fVar21 = EnemyWrapAngle((float)(1.5707964f - fVar21));
        local_2bc = (float)fVar21;
      }
      EnemySetMotionAngle(selectedMotion, local_2bc);
    }
    if (0.0 < (float)fVar19) {
      selectedMotion->value18 = (float)fVar19;
    }
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_INTERPOLATE_OFFSET_POLAR:
  case ENEMY_ECL_INTERPOLATE_BASE_POLAR:
    selectedMotion = &offsetMotion;
    firstScalarInterpolation = &scalarInterpolation0;
    if (opcode == 0x11d) {
      firstScalarInterpolation = &scalarInterpolation0;
    }
    else {
      selectedMotion = &baseMotion;
      firstScalarInterpolation = &scalarInterpolation1;
    }
    firstScalarValues = reinterpret_cast<float *>(firstScalarInterpolation);
    fVar19 = (ReadFloatArgument(2));
    local_2c8 = (float)fVar19;
    fVar19 = (ReadFloatArgument(3));
    local_2c4 = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    firstScalarInterpolation->mode = uVar22;

    if (uVar22 == 7) {
      local_29c = local_2c8;
      if (local_29c <= 0.0) {
        local_29c = 0.0;
      }
      local_298 = local_2c4;
      if (local_298 <= 0.0) {
        local_298 = 0.0;
      }
    }
    else {
      if (local_2c8 <= 0.0) {
        local_29c = selectedMotion->value1C;
      }
      else {
        local_29c = local_2c8;
        if ((*(unsigned int *)((int)runtimeAddress + 0x1444) & 0x800) != 0) {
          fVar19 = EnemyWrapAngle((float)(local_29c - 1.5707964f));
          local_29c = (float)EnemyWrapAngle((float)(1.5707964f - fVar19));
        }
      }
      if (local_2c4 <= 0.0) {
        local_298 = selectedMotion->value18;
      }
      else {
        local_298 = local_2c4;
      }
    }

    local_2a8 = selectedMotion->value1C;
    local_2a4 = selectedMotion->value18;
    if (3.1415927f <= static_cast<float>(fabs(local_2a8 - local_29c))) {
      if (local_29c <= local_2a8) {
        local_29c = local_29c + -3.1415927f;
      }
      else {
        local_2a8 = local_2a8 + -3.1415927f;
      }
    }
    local_2c8 = local_29c;
    local_2c4 = local_298;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent.x = g_EnemyInterpolationBasis.x;
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
    if (0.0 < (float)fVar19) {
      EnemySetMotionAngle(selectedMotion, (float)fVar19);
    }
    if (0.0 < (float)fVar21) {
      selectedMotion->value18 = (float)fVar21;
    }
    if (0.0 < local_2a8) {
      selectedMotion->value20 = local_2a8;
    }
    if (0.0 < (float)fVar20) {
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

    if (local_2c4 <= 0.0) {
      local_298 = selectedMotion->value18;
    }
    else {
      local_298 = local_2c4;
    }
    if (local_2c8 <= 0.0) {
      local_29c = selectedMotion->value1C;
    }
    else {
      local_29c = local_2c8;
    }

    local_2a4 = selectedMotion->value18;
    local_2a8 = selectedMotion->value1C;

    if (fVar19 <= 0.0) {
      fVar19 = selectedMotion->value24;
    }
    if (local_2bc <= 0.0) {
      vectorScratch.x = selectedMotion->value20;
    }
    else {
      vectorScratch.x = local_2bc;
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
    if (0.0 < (float)fVar19) {
      *(float *)(iVar26 + 0xc) = (float)fVar19;
    }
    if (0.0 < fVar21) {
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
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    firstScalarInterpolation->mode = uVar22;
    local_2c4 = 0.0;
    local_2c8 = local_2bc;
    vectorScratch.x = local_2bc;
    fVar19 = (ReadFloatArgument(2));
    vectorScratch.y = (float)fVar19;
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    firstScalarInterpolation->duration = uVar22;
    firstScalarInterpolation->initialTangent = g_EnemyInterpolationBasis;
    firstScalarInterpolation->finalTangent.x = g_EnemyInterpolationBasis.x;
    fVar9 = vectorScratch.x;
    fVar10 = vectorScratch.y;
dispatch_initialize_polar_interpolation:
    firstScalarInterpolation->finalTangent.y = g_EnemyInterpolationBasis.y;
    firstScalarInterpolation->initial.x = fVar9;
    firstScalarInterpolation->initial.y = fVar10;
    firstScalarInterpolation->final.x = local_2c8;
    firstScalarInterpolation->final.y = local_2c4;
    EnemyInitializeScalarInterpolation(firstScalarInterpolation);
    selectedMotion->flags &= 0xfffffffe;
    return 0;
  case ENEMY_ECL_SET_LIFE_MARKER:
    uVar22 = ENEMY_READ_INT_DIRECT(2);
    fVar19 = (ReadFloatArgument(1));
    local_2a8 = (float)fVar19;
    iVar26 = *(int *)((int)runtimeAddress + 0x13c4);
    uVar23 = ENEMY_READ_INT_DIRECT(0);
    iVar27 = reinterpret_cast<int>(g_EnemyVisualState);
    *(int *)(reinterpret_cast<int>(g_EnemyVisualState) + 0x9e98 + (int)uVar23 * 8) = (int)uVar22;
    *(float *)(iVar27 + 0x9e94 + (int)uVar23 * 8) = local_2a8 / (float)iVar26;
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
      EnemyFireBulletPattern(reinterpret_cast<int>(g_EnemyBulletManager),iVar26,reinterpret_cast<int>(g_EnemyBulletManager));
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
    if (g_EnemyRank >= 0x200) {
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
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyRank >= 600) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(9));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(10));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    if (g_EnemyRank >= 200) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      fVar19 = (ReadFloatArgument(7));
      *(float *)(iVar26 + 0x2dc) = (float)fVar19;
      fVar19 = (ReadFloatArgument(8));
      *(float *)(iVar26 + 0x2e0) = (float)fVar19;
      return 0;
    }
    if (g_EnemyRank >= -200) goto dispatch_bullet_speed_high;
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
    uVar22 = ENEMY_READ_INT_DIRECT(0);
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
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyRank >= 0x200) {
dispatch_bullet_count_high:
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ENEMY_READ_INT_DIRECT(5);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      uVar22 = ENEMY_READ_INT_DIRECT(6);
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    iVar27 = g_EnemyRank + 0x200;
    goto dispatch_select_bullet_count_low;
  case ENEMY_ECL_SET_BULLET_COUNT_BY_RANK_5:
    uVar22 = ENEMY_READ_INT_DIRECT(0);
    iVar26 = (int)uVar22;
    if (g_EnemyRank >= 600) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ENEMY_READ_INT_DIRECT(9);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      {
        EclVmContext *countContext =
            reinterpret_cast<EclVmContext *>(owner->activeEclContext);
        uVar22 = countContext->ReadInt(10);
      }
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    if (g_EnemyRank >= 200) {
      iVar26 = iVar26 * 0x210 + (int)runtimeAddress;
      uVar22 = ENEMY_READ_INT_DIRECT(7);
      *(short *)(iVar26 + 0x4b8) = (short)uVar22;
      {
        EclVmContext *countContext =
            reinterpret_cast<EclVmContext *>(owner->activeEclContext);
        uVar22 = countContext->ReadInt(8);
      }
      *(short *)(iVar26 + 0x4ba) = (short)uVar22;
      return 0;
    }
    if (g_EnemyRank >= -200) goto dispatch_bullet_count_high;
    iVar27 = g_EnemyRank + 600;
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
    iVar26 = ((int)uVar24 - (int)uVar23) * (g_EnemyRank + 0x400);
    *(short *)(iVar27 + 0x4b8 + (int)runtimeAddress) =
         (short)((int)(iVar26 + (iVar26 >> 0x1f & 0x7ffU)) >> 0xb) + (short)uVar23;
    iVar26 = ((int)uVar25 - secondLowBulletCount) * (g_EnemyRank + 0x400);
    *(short *)(iVar27 + (int)runtimeAddress + 0x4ba) =
         (short)((int)(iVar26 + (iVar26 >> 0x1f & 0x7ffU)) >> 0xb) +
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
    EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyBulletManager), 1);
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
    EnemyReadDialog(g_EnemyVisualState, (int)uVar22);
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelAllBullets(0);
    EnemyApplyBulletClear(reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyBulletManager), 0);
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
  case ENEMY_ECL_START_SPELL:
  case ENEMY_ECL_START_SPELL_2:
  case ENEMY_ECL_START_SPELL_DIFFICULTY:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_1:
  case ENEMY_ECL_START_SPELL_DIFFICULTY_MINUS_2:
    // Preserve the target's shared float scratch lifetime for this field.
    // Normalizing it to an unsigned-int temporary changes the VC7.1 frame.
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
    uVar22 = ENEMY_READ_INT_DIRECT(0);
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
    ENEMY_READ_INT_DIRECT(2);
    uVar22 = ENEMY_READ_INT_DIRECT(1);
    EnemyBeginSpell(reinterpret_cast<int>(g_EnemyGameState),iVar26,reinterpret_cast<char *>(local_90.bytes),(int)uVar22);
    return 0;
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
      puVar5 = (unsigned int *)((int)runtimeAddress + 0x2e4);
      puVar14 = reinterpret_cast<unsigned int *>(&local_288) + 11;
      for (iVar27 = 0x6c; iVar27 != 0; iVar27 = iVar27 + -1) {
        *puVar14 = *puVar5;
        puVar5 = puVar5 + 1;
        puVar14 = puVar14 + 1;
      }
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
    EnemyFireLaser(g_EnemyBulletManager, &local_288, 0);
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
    uVar6 = EnemyFireLaser(g_EnemyBulletManager, &local_288, 1);
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
    EnemyFireLaser(g_EnemyBulletManager, &local_288, 0);
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
    uVar6 = EnemyFireLaser(g_EnemyBulletManager, &local_288, 1);
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
        g_EnemyBulletManager)->ApplyPatternCancel(
            &worldMotion.position, (float)fVar19, 1);
    return 0;
  case ENEMY_ECL_CLEAR_BULLET_PATTERN:
    fVar19 = (ReadFloatArgument(0));
    reinterpret_cast<EnemyCancelManagerView *>(
        g_EnemyPrimaryResourceOwner)->CancelBulletPattern(
            &worldMotion.position, (float)fVar19, 0, 0);
    reinterpret_cast<EnemyEffectWaitManager *>(
        g_EnemyBulletManager)->ApplyPatternCancel(
            &worldMotion.position, (float)fVar19, 0);
    return 0;
  case ENEMY_ECL_SET_CHAPTER:
    uVar22 = ReadIntArgument(0);
    EnemySetChapter(&g_EnemyChapterState, (int)uVar22);
    return 0;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_3:
    if (g_EnemyRank >= 0x200) {
      iVar27 = 2;
      goto dispatch_rank_float_tail_b;
    }
    iVar27 = 0;
    if (g_EnemyRank > -0x200) goto dispatch_rank_float_tail_b;
    goto dispatch_rank_float_tail_a;
  case ENEMY_ECL_SELECT_FLOAT_BY_RANK_5:
    if (g_EnemyRank >= 600) {
      goto dispatch_difficulty_float_index_4_a;
    }
    if (g_EnemyRank >= 200) {
      goto dispatch_difficulty_float_index_3;
    }
    if (g_EnemyRank >= -200) {
      fVar19 = ReadFloatArgument(2);
      pfVar15 = ResolveFloatArgument(0);
      *pfVar15 = (float)fVar19;
      return 0;
    }
    if (g_EnemyRank < -400) {
      fVar21 = ReadFloatArgument(0);
      goto dispatch_rank5_float_low_store;
    }
    fVar21 = ReadFloatArgument(1);
dispatch_rank5_float_low_store:
    positionInterpolationValues = ResolveFloatArgument(0);
    *positionInterpolationValues = (float)fVar21;
    return 0;
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
  case ENEMY_ECL_SET_SPELL_TIMEOUT:
    uVar22 = ReadIntArgument(0);
    *(int *)(reinterpret_cast<int>(g_EnemyVisualState) + 0x9e90) = (int)uVar22;
    return 0;
  case ENEMY_ECL_WAIT_FOR_EFFECT:
    EnemyWaitForEffect(
        reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyBulletManager));
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
  memset(local_90.floatWords, 0, sizeof(local_90.floatWords));
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
