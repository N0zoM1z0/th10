#include "Main.hpp"
#include "EnemyGameState.hpp"
#include "Enemy.hpp"
#include "EnemyCancel.hpp"
#include "EnemyLaser.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"
#include "VmIdInterrupt.hpp"
#include "Player.hpp"
#include "Chain.hpp"
#include "GameErrorContext.hpp"
#include "FileSystem.hpp"
#include "AnmVmId.hpp"
#include "BulletManager.hpp"
#include "GameScoreState.hpp"
#include "Hint.hpp"
#include "ItemManager.hpp"
#include "ItemPowerDisplay.hpp"

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)

struct PlayerVm;
struct AnmRenderManagerView;
extern AnmRenderManagerView *g_AnmRenderManagerView;

struct PlayerVmListNode
{
    PlayerVm *value;
    PlayerVmListNode *next;
    PlayerVmListNode *previous;
};

struct PlayerVmSpriteView
{
    unsigned char unknown000[0x30];
    float extent30;
    float extent34;
};

struct PlayerVm
{
    unsigned int id;
    unsigned char unknown004[0x10];
    PlayerVmListNode *children;
    int childOwner;
    unsigned char unknown01C[0x04];
    int layer;
    unsigned char unknown024[0x08];
    float angle2C;
    unsigned char unknown030[0x0c];
    float scale3C;
    float scale40;
    unsigned char unknown044[0x2c0];
    unsigned short deleteState;
    unsigned char unknown306[0x3a];
    PlayerFloat3 position;
    unsigned char unknown34C[0x10];
    unsigned int flags;
    unsigned char unknown360[0x34];
    PlayerVmSpriteView *sprite;
};
typedef char PlayerVmChildrenAt14[
    (offsetof(PlayerVm, children) == 0x14) ? 1 : -1];
typedef char PlayerVmLayerAt20[
    (offsetof(PlayerVm, layer) == 0x20) ? 1 : -1];
typedef char PlayerVmAngleAt2C[
    (offsetof(PlayerVm, angle2C) == 0x2c) ? 1 : -1];
typedef char PlayerVmScaleAt3C[
    (offsetof(PlayerVm, scale3C) == 0x3c) ? 1 : -1];
typedef char PlayerVmDeleteStateAt304[
    (offsetof(PlayerVm, deleteState) == 0x304) ? 1 : -1];
typedef char PlayerVmPositionAt340[
    (offsetof(PlayerVm, position) == 0x340) ? 1 : -1];
typedef char PlayerVmFlagsAt35C[
    (offsetof(PlayerVm, flags) == 0x35c) ? 1 : -1];
typedef char PlayerVmSpriteAt394[
    (offsetof(PlayerVm, sprite) == 0x394) ? 1 : -1];

// Bounded shot-hit seam into the maintained full ANM VM and four-byte ID.
// Reinterpreting the legacy ID storage remains an explicit partial lifetime view.
typedef char PlayerShotHitVmLayoutMatchesAnm[
    (sizeof(AnmVmView) == 0x3ac && sizeof(AnmVmIdView) == sizeof(unsigned int) &&
     offsetof(PlayerVm, angle2C) == offsetof(AnmVmView, rotation) +
         offsetof(AnmFloat3View, z) &&
     offsetof(PlayerVm, flags) == offsetof(AnmVmView, flags35C)) ? 1 : -1];

struct PlayerVmManager
{
    unsigned char unknown000[0x3ad08c];
    void *playerCache;
    unsigned char unknown3AD090[0x380a44];
    PlayerVmListNode *primaryHead;
    PlayerVmListNode *primaryTail;
    PlayerVmListNode *secondaryHead;
    PlayerVmListNode *secondaryTail;
};
typedef char PlayerVmManagerCacheAt3AD08C[
    (offsetof(PlayerVmManager, playerCache) == 0x3ad08c) ? 1 : -1];
typedef char PlayerVmManagerPrimaryHeadAt72DAD4[
    (offsetof(PlayerVmManager, primaryHead) == 0x72dad4) ? 1 : -1];
typedef char PlayerVmManagerSecondaryHeadAt72DADC[
    (offsetof(PlayerVmManager, secondaryHead) == 0x72dadc) ? 1 : -1];

typedef int (__fastcall *PlayerChainCallback)(Player *player);

// Maintained view of the 0x24-byte callback-list node allocated by the target.
// Only fields used by Player registration/lifetime evidence are exposed.
struct PlayerCallbackNodeView
{
    int priority;
    unsigned int flags;
    PlayerChainCallback callback;
    unsigned char unknown00C[0x14];
    Player *owner;
};
typedef char PlayerCallbackNodeViewSizeIs24[
    (sizeof(PlayerCallbackNodeView) == 0x24) ? 1 : -1];
typedef char PlayerCallbackNodeViewCallbackAt08[
    (offsetof(PlayerCallbackNodeView, callback) == 0x08) ? 1 : -1];
typedef char PlayerCallbackNodeViewOwnerAt20[
    (offsetof(PlayerCallbackNodeView, owner) == 0x20) ? 1 : -1];
typedef char PlayerFixedPositionPairContiguous[
    (offsetof(Player, positionY) == offsetof(Player, positionX) + sizeof(int)) ? 1 : -1];
// Existing shared storage begins at target 0x474C40; the selection words are
// observed at +0x28/+0x2C. This is a storage view, not an original owner claim.
typedef char PlayerSelectionStorageAt28[
    (offsetof(GameScoreStateView, unknown28) == 0x28) ? 1 : -1];
typedef char PlayerCallbackNodeMatchesChain[
    (sizeof(PlayerCallbackNodeView) == sizeof(th10::ChainElem) &&
     offsetof(PlayerCallbackNodeView, flags) == offsetof(th10::ChainElem, flags) &&
     offsetof(PlayerCallbackNodeView, callback) == offsetof(th10::ChainElem, callback) &&
     offsetof(PlayerCallbackNodeView, owner) == offsetof(th10::ChainElem, arg)) ? 1 : -1];

// Neutral maintained views for the target-observed auxiliary draw gate. Their
// original object and field identifiers are not established by this packet.
struct PlayerDrawGate54
{
    unsigned char unknown000[0x54];
    signed char value54;
};
struct PlayerDrawGate10
{
    unsigned char unknown000[0x10];
    int value10;
    unsigned char unknown014[0x4c];
    int value60;
};
struct PlayerDrawGate9EB8
{
    unsigned char unknown000[0x9eb8];
    int value9EB8;
};
struct PlayerDrawGate4
{
    unsigned char unknown000[0x04];
    int value4;
};

struct PlayerUpdateGate28
{
    unsigned char unknown000[0x14];
    PlayerTimerView timer;
    int value28;
    AnmVmIdView vmId;
    PlayerFloat3 position;
    float radius;
    float value40;
    int mode;

    int StartPowerTransition();
    int GetDamageAtPosition(const PlayerFloat3 *targetPosition);
};
typedef char PlayerAuxiliaryDamageOffsets[
    (offsetof(PlayerUpdateGate28, position) == 0x30 &&
     offsetof(PlayerUpdateGate28, radius) == 0x3c &&
     offsetof(PlayerUpdateGate28, mode) == 0x44 &&
     sizeof(PlayerUpdateGate28) == 0x48) ? 1 : -1];

struct PlayerUpdateGate10
{
    unsigned char unknown000[0x10];
    int value10;
};

struct PlayerHostileRowView
{
    unsigned char unknown000[0x04];
    int value4;
    unsigned char unknown008[0x43e];
    unsigned short state446;
    unsigned char unknown448[0x3a8];
};
typedef char PlayerHostileRowViewSizeIs7F0[
    (sizeof(PlayerHostileRowView) == 0x7f0) ? 1 : -1];
typedef char PlayerHostileRowStateAt446[
    (offsetof(PlayerHostileRowView, state446) == 0x446) ? 1 : -1];

struct PlayerHostileManagerView
{
    unsigned char unknown000[0x60];
    PlayerHostileRowView rows[2000];
};

extern PlayerDrawGate54 *g_PlayerDrawGate54;
extern PlayerDrawGate10 *g_PlayerDrawGate10;
extern PlayerDrawGate9EB8 *g_PlayerDrawGate9EB8;
extern PlayerDrawGate4 *g_PlayerDrawGate4;
extern int g_PlayerAuxiliaryDrawSpan;
extern unsigned int g_PlayerInputBits;
extern int g_PlayerRuntimeGateValue;
extern int g_PlayerPeriodicCounter;
extern float g_PlayerTimerScale;
extern PlayerUpdateGate28 *g_PlayerUpdateGate28;
extern PlayerUpdateGate10 *g_PlayerUpdateGate10;

// Descriptive interfaces for target-observed draw callees. The target machine
// boundaries use private register conventions; these declarations are
// maintained source spellings, not original ABI claims.
void PlayerDrawManagedVm(PlayerDrawVmView *vm);
void PlayerDrawAuxiliaryRectangle(const float *bounds, unsigned int color);

// Descriptive interfaces for target-observed Player update callees. Their
// original identifiers and private register conventions remain unproven.
void PlayerTransitionToState2(Player *player);
float PlayerAngleToPoint(Player *player, const PlayerFloat3 *point)
{
    float dx = point->x - player->drawPosition.x;
    float dy = point->y - player->drawPosition.y;
    if (dy == 0.0f && dx == 0.0f)
        return 1.5707964f;
    return (float)atan2(dy, dx);
}
void PlayerFallbackRuntimeTransition();
// Target 0x427B50 scans 32 usable rows; exhaustion returns row 32 unchanged.
PlayerEffectRowView *Player::SpawnEffectRow(
    const PlayerFloat3 *position,
    float radius, float radiusDelta, int frames, int damage)
{
    PlayerEffectRowView *row = effectRows;
    for (int i = 0; i < 32; ++i, ++row)
    {
        if ((row->motion.activeFlags & 1u) != 0)
            continue;

        memset(row, 0, sizeof(*row));
        row->motion.activeFlags |= 3u;
        // Preserve the target's second clear before reading possibly overlapping XYZ.
        PlayerFloat3 *const rowPosition = &row->position;
        memset(rowPosition, 0, 0x2c);
        rowPosition->x = position->x;
        rowPosition->y = position->y;
        rowPosition->z = position->z;
        row->radius = radius;
        row->radiusDelta = radiusDelta;
        row->motion.timer.SetCurrent(frames);
        row->motion.damage = damage;
        row->motion.hitAccumulator = 0;
        row->motion.hitCap = 999999;
        row->motion.collisionInterval = 4;
        return row;
    }
    return row;
}
int PlayerUpdateInputAction(Player *player);
int __stdcall PlayerUpdateShots(Player *player);

// Shared target 0x44BC70 uses a float stack argument and RET 4. The vector,
// movement and pending-interrupt calls use their maintained source interfaces.
float __stdcall EnemyWrapAngle(float value);
// Target 0x40C480 publishes pending interrupt 2 without executing or clearing ID.
void PlayerSetShotVmInterrupt2(unsigned int *vmId)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(AnmVmIdView(*vmId));
    if (vm == NULL)
        return;
    vm->pendingInterrupt = 2;
    if (vm->layerNode.previous != NULL)
        return;
    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 2;
        node = node->next;
    }
}
extern void ResultMenuPendingThree(AnmVmIdView *id);
extern int g_GameStatisticsIndex0; // target 0x474C68, shared GameManager identity

// Target 0x4059F0: read-only auxiliary damage, private ECX owner/EAX position.
int PlayerUpdateGate28::GetDamageAtPosition(const PlayerFloat3 *targetPosition)
{
    if (value28 == 0)
        return 0;
    const float dy = targetPosition->y - position.y;
    const float dx = targetPosition->x - position.x;
    if (dx * dx + dy * dy < radius * radius)
    {
        if ((g_EnemyGameState->flags378C & 1u) != 0)
            return mode == 0 ? 3 : 38;
        return g_PlayerDrawGate10->value10 != 0 ? 38 : 5;
    }
    if ((g_EnemyGameState->flags378C & 1u) != 0)
    {
        if (mode == 1)
            return g_GameStatisticsIndex0 != 0 ? 23 : 26;
    }
    else if (g_PlayerDrawGate10->value10 != 0 && mode == 0)
    {
        return g_GameStatisticsIndex0 == 0 ? 20 : 16;
    }
    return 0;
}

__declspec(noinline) double __stdcall PlayerFloor(float value)
{
    return floor(value);
}

__declspec(noinline) int PlayerTimerView::HasIntervalTick(int interval) const
{
    return current != previous && (current % interval) == 0;
}

struct PlayerOptionPosition
{
    float x;
    float y;
    float z;
};
typedef char PlayerOptionPositionSizeIs0C[
    (sizeof(PlayerOptionPosition) == 0x0c) ? 1 : -1];

extern short g_PlayerPower;
extern int g_GameScore;
extern int g_PlayerCharacter;
extern int g_PlayerShotType;
extern int g_PlayerOptionPositionBase[];
extern float g_PlayerOptionCoordinateScale;
extern PlayerVmManager *g_PlayerVmManager;
extern void *g_PlayerCallbackManager;
extern PlayerOptionDataView *g_PlayerSharedOptionData;
extern const char *g_PlayerShotDataFilenames[6];
extern float g_PlayerOptionValue04ByCharacter[2];
extern float g_PlayerOptionValue08ByCharacter[2];
extern float g_PlayerOptionValue0CByCharacter[2];
extern float g_PlayerExtent2ByCharacter[2];

// Descriptive interfaces for independently observed VM-manager callees. Their
// own original identifiers, source conventions, and physical owners remain
// unproven by this packet.
struct EnemyPrimaryResourceOwnerView;
extern EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;

// Neutral lifecycle globals and interfaces recovered around Player creation,
// reset and teardown. Private register contracts are deliberately hidden here.
extern unsigned int g_PlayerLifecycleFlags;
struct GuiView;
extern GuiView *g_GuiView;
extern volatile int g_PlayerLivesDisplayCount;
extern unsigned char g_PlayerCallbackLockDepth;
struct PlayerCriticalSectionView { unsigned char storage[0x18]; };
typedef char PlayerCriticalSectionMatchesSdk[
    (sizeof(PlayerCriticalSectionView) == sizeof(CRITICAL_SECTION)) ? 1 : -1];
extern PlayerCriticalSectionView g_PlayerCallbackCriticalSection;
void __fastcall PlayerUnlinkCallbackNode(
    PlayerCallbackNodeView *node, void *manager);
void PlayerMarkManagedVmPending(unsigned int vmId);
void PlayerMarkManagedVmPendingWithManager(
    AnmRenderManagerView *manager, unsigned int vmId);
void GuiSetLivesDisplayCount(GuiView *gui, int count);
void GuiSetLivesDisplayCountMember(GuiView *gui, int count);
void PlayerMarkResourceVmsPending(void *resource);
void PlayerDestroyAnimationCacheContents(void *cache);

// Ordinary source spelling for the .sht loader reviewed at 0x426520.
// The target/native linked graph uses ESI Player and EAX filename; the original
// declaration and physical translation unit remain unproven.
int PlayerLoadOptionData(Player *player, const char *filename);

int __fastcall PlayerUpdateCallback(Player *player);
int __fastcall PlayerDrawCallback(Player *player);

enum
{
    PLAYER_OPTION_ACTIVE = 2,
    PLAYER_VM_LAYER = 0x0f,
    PLAYER_VM_PENDING_FLAG = 0x04000000,
    PLAYER_VM_RUNTIME_FLAG = 0x40000000,
    PLAYER_OPTION_PRIMARY_SCRIPT_0 = 0x11,
    PLAYER_OPTION_PRIMARY_SCRIPT_1 = 0x12,
    PLAYER_OPTION_PRIMARY_SCRIPT_2 = 0x13,
    PLAYER_OPTION_SECONDARY_SCRIPT_0 = 0x14,
    PLAYER_OPTION_SECONDARY_SCRIPT_1 = 0x15,
    PLAYER_OPTION_SECONDARY_SCRIPT_2 = 0x16,
    PLAYER_DIRECTION_NONE = 0,
    PLAYER_DIRECTION_UP = 1,
    PLAYER_DIRECTION_DOWN = 2,
    PLAYER_DIRECTION_LEFT = 3,
    PLAYER_DIRECTION_RIGHT = 4,
    PLAYER_DIRECTION_UP_LEFT = 5,
    PLAYER_DIRECTION_UP_RIGHT = 6,
    PLAYER_DIRECTION_DOWN_LEFT = 7,
    PLAYER_DIRECTION_DOWN_RIGHT = 8,
    PLAYER_MODE_VM_LAYER = 9,
    PLAYER_MODE_VM_SCRIPT = 0x160,
    PLAYER_MAIN_VM_MOVE_LEFT = 1,
    PLAYER_MAIN_VM_STOP_LEFT = 2,
    PLAYER_MAIN_VM_MOVE_RIGHT = 3,
    PLAYER_MAIN_VM_STOP_RIGHT = 4,
    PLAYER_SHOT_INACTIVE = 0,
    PLAYER_SHOT_ACTIVE = 1,
    PLAYER_SHOT_HIT_TRANSITION = 2,
    PLAYER_SHOT_TYPE_3 = 3,
    PLAYER_SHOT_POLAR_MOTION = 1,
    PLAYER_VM_SHOT_ANGLE_SOURCE = 0x08000000,
    PLAYER_VM_SHOT_ANGLE_DIRTY = 4,
};

static PlayerVm *FindPlayerVm(unsigned int id)
{
    return reinterpret_cast<PlayerVm *>(
        g_AnmRenderManagerView->FindVm(AnmVmIdView(id)));
}

static void MarkPlayerVmFlag(unsigned int id)
{
    PlayerVm *vm = FindPlayerVm(id);
    if (vm == NULL)
        return;

    vm->flags |= PLAYER_VM_PENDING_FLAG;
    if (vm->childOwner == 0)
    {
        PlayerVmListNode *node;
        for (node = vm->children; node != NULL; node = node->next)
        {
            node->value->flags |= PLAYER_VM_PENDING_FLAG;
        }
    }
}

// The shot update's inlined release resolves both VM lists, marks a live root
// and its children, then clears the ID even when its input was already zero.
static __forceinline void ReleasePlayerShotVm(AnmVmIdView *id)
{
    const AnmVmIdView key = *id;
    AnmVmView *vm = NULL;
    if (key.value != 0)
    {
        AnmVmLayerNodeView *entry = g_AnmRenderManagerView->primaryVmListHead;
        while (entry != NULL)
        {
            if (static_cast<AnmVmView *>(entry->owner)->id == key.value)
                break;
            entry = entry->next;
        }
        if (entry == NULL)
        {
            entry = g_AnmRenderManagerView->secondaryVmListHead;
            while (entry != NULL)
            {
                if (static_cast<AnmVmView *>(entry->owner)->id == key.value)
                    break;
                entry = entry->next;
            }
        }
        if (entry != NULL)
            vm = static_cast<AnmVmView *>(entry->owner);
    }
    if (vm != NULL)
    {
        vm->flags35C |= PLAYER_VM_PENDING_FLAG;
        if (vm->layerNode.previous == NULL)
        {
            AnmVmLayerNodeView *node = vm->layerNode.next;
            while (node != NULL)
            {
                static_cast<AnmVmView *>(node->owner)->flags35C |= PLAYER_VM_PENDING_FLAG;
                node = node->next;
            }
        }
    }
    id->value = 0;
}

static void MarkPlayerVmDeleteState(unsigned int id, unsigned short state)
{
    PlayerVm *vm = FindPlayerVm(id);
    if (vm == NULL)
        return;

    vm->deleteState = state;
    if (vm->childOwner == 0)
    {
        PlayerVmListNode *node;
        for (node = vm->children; node != NULL; node = node->next)
        {
            node->value->deleteState = state;
        }
    }
}

// Target4269D0. The maintained update caller enters state2 after its state4
// timer reaches eight. Resource, timer, option and panel storage remain the
// existing shared views; this body grants no original type/TU or exact claim.
void PlayerTransitionToState2(Player *player)
{
    const int faithLoss = ((g_GameScoreState.faith - 5000) / 3) * 10;
    const int lives = g_PlayerLivesDisplayCount - 1;
    g_PlayerLivesDisplayCount = lives;
    g_GameScoreState.DecayFaith(faithLoss);
    if (lives >= 0)
        GuiSetLivesDisplayCount(g_GuiView, lives);

    player->runtimeState = 2;
    player->updateTimer1.SetCurrent(0);
    player->highlightTimer.SetCurrent(180);
    reinterpret_cast<AnmLoadedView *>(player->resource)->InitializeAndExecuteScriptIndex(
        reinterpret_cast<AnmVmView *>(&player->drawVm), 0);
    for (int i = 0; i < 4; ++i)
    {
        player->options[i].state = 0;
        MarkPlayerVmDeleteState(player->options[i].primaryVmId, 1);
        MarkPlayerVmDeleteState(player->options[i].secondaryVmId, 1);
    }
    player->optionCount = 0;
    if (g_PlayerUpdateGate10->value10 != 1)
        g_GameOwner477814->AddCaution("Caution!",
            &player->drawPosition);

    EnemyGameStateView *panel = g_EnemyGameState;
    if (panel->spellTimer.current >= 60)
    {
        panel->currentSpellBonus = 0;
        panel->flags378C &= ~2u;
        for (int slot = 0; slot < 8; ++slot)
        {
            // Target clears seven VM flags; the third VM is deliberately kept.
            if (slot != 2)
                panel->frontVms[slot].flags35C &= ~2u;
        }
    }
    g_GameScoreState.AddRank(-0x400);
}

static int ScalePlayerOptionCoordinate(float value)
{
    return (int)(value * g_PlayerOptionCoordinateScale);
}

static const PlayerOptionPosition *GetPlayerOptionPosition(
    Player *player, unsigned int tableOffset, int optionCount, int optionIndex)
{
    int index = g_PlayerOptionPositionBase[optionCount] + optionIndex;
    return (const PlayerOptionPosition *)
        ((const unsigned char *)player->optionData + tableOffset) + index;
}

static void LoadPlayerOptionPair(
    PlayerPositionPair *pair, const PlayerOptionPosition *position)
{
    pair->x = ScalePlayerOptionCoordinate(position->x);
    pair->y = ScalePlayerOptionCoordinate(position->y);
}

// Maintained spelling of the observed ECX-bound option callback boundary. This
// models the target machine ABI; the original source declaration and the
// wrapper/body optimizer ownership remain unknown.
__declspec(noinline) void PlayerOptionTrailCallbackBody(
    PlayerOptionRuntime *option);

int __fastcall PlayerOptionTrailCallback(PlayerOptionRuntime *option)
{
    PlayerOptionTrailCallbackBody(option);
    return 0;
}

__declspec(noinline) void PlayerOptionTrailCallbackBody(
    PlayerOptionRuntime *option)
{
    Player *player = g_Player;
    const int optionMode = player->optionMode;

    option->replayPair0 =
        player->replayPositionHistory[(option->optionIndex + 1) * 8];
    if (optionMode == 0)
    {
        option->replayPair2.x = option->replayPair0.x - player->positionX;
        option->replayPair2.y = option->replayPair0.y - player->positionY;
    }
    else
    {
        player->replayPositionHistory[(option->optionIndex + 1) * 8].x =
            player->positionX + option->replayPair2.x;
        player->replayPositionHistory[(option->optionIndex + 1) * 8].y =
            player->positionY + option->replayPair2.y;

        float fraction = 0.125f;
        for (int i = option->optionIndex * 8 + 1;
             i <= option->optionIndex * 8 + 7; ++i)
        {
            PlayerPositionPair &sample = player->replayPositionHistory[i];
            sample.x = (int)(
                (player->replayPositionHistory[(option->optionIndex + 1) * 8].x -
                 player->replayPositionHistory[option->optionIndex * 8].x) *
                    fraction +
                player->replayPositionHistory[option->optionIndex * 8].x);
            sample.y = (int)(
                (player->replayPositionHistory[(option->optionIndex + 1) * 8].y -
                 player->replayPositionHistory[option->optionIndex * 8].y) *
                    fraction +
                player->replayPositionHistory[option->optionIndex * 8].y);
            fraction += 0.125f;
        }
    }

    option->replayPair0.x = player->positionX + option->replayPair2.x;
    option->replayPair0.y = player->positionY + option->replayPair2.y;
    option->previousMode = optionMode;
}

// Maintained spelling of the second observed ECX-bound option callback. The
// target sends interrupt 6/3 to the primary VM when the option mode changes.
__declspec(noinline) void PlayerOptionSpecialCallbackBody(
    PlayerOptionRuntime *option);

int __fastcall PlayerOptionSpecialCallback(PlayerOptionRuntime *option)
{
    PlayerOptionSpecialCallbackBody(option);
    return 0;
}

__declspec(noinline) void PlayerOptionSpecialCallbackBody(
    PlayerOptionRuntime *option)
{
    Player *player = g_Player;
    const int optionMode = player->optionMode;
    // The retail EDI body performs each replay-coordinate load/store pair in
    // sequence; retain that target-observed field order in the VC7.1 LTCG
    // lowering instead of allowing the two coordinates to be batched.
    volatile int *previousMode = &option->previousMode;

    if (optionMode == 0)
    {
        if (*previousMode != 0)
            reinterpret_cast<AnmVmIdView *>(&option->primaryVmId)->SetInterrupt(6);

        volatile int *replayPair3 =
            reinterpret_cast<volatile int *>(&option->replayPair3);
        const volatile int *replayPair1 =
            reinterpret_cast<const volatile int *>(&option->replayPair1);
        replayPair3[0] = replayPair1[0];
        replayPair3[1] = replayPair1[1];
    }
    else
    {
        if (*previousMode == 0)
            reinterpret_cast<AnmVmIdView *>(&option->primaryVmId)->SetInterrupt(3);

        volatile int *replayPair0 =
            reinterpret_cast<volatile int *>(&option->replayPair0);
        const volatile int *replayPair3 =
            reinterpret_cast<const volatile int *>(&option->replayPair3);
        replayPair0[0] = replayPair3[0];
        replayPair0[1] = replayPair3[1];
    }

    *previousMode = optionMode;
}

// Maintained source for the Player constructor/reset/factory/destructor seam.
// The target optimizer uses private ESI/stack/register boundaries for several
// bodies; these natural C++ declarations do not claim those machine ABIs.
PlayerDrawVmView::~PlayerDrawVmView()
{
    if (ownedData358 != NULL)
        free(ownedData358);
    ownedData358 = NULL;
}

Player::Player()
{
    memset(this, 0, sizeof(*this));
    g_Player = this;
}

void PlayerResetRuntimeState(Player *player)
{
    AnmRenderManagerView *manager;
    volatile int *runtimeState = &player->runtimeState;
    *runtimeState = 1;

    if ((player->updateTimer0.flags & 1) == 0)
    {
        player->updateTimer0.current = 0;
        player->updateTimer0.previous = 0xfff0bdc1;
        player->updateTimer0.subframe = 0.0f;
        player->updateTimer0.scale = &g_PlayerTimerScale;
        player->updateTimer0.flags |= 1;
    }
    player->updateTimer0.current = -1;
    player->updateTimer0.subframe = -1.0f;
    player->updateTimer0.previous = -2;
    _ReadWriteBarrier();

    if ((player->updateTimer1.flags & 1) == 0)
    {
        player->updateTimer1.current = 0;
        player->updateTimer1.previous = 0xfff0bdc1;
        player->updateTimer1.subframe = 0.0f;
        player->updateTimer1.scale = &g_PlayerTimerScale;
        player->updateTimer1.flags |= 1;
    }
    player->updateTimer1.current = 0;
    player->updateTimer1.subframe = 0.0f;
    player->updateTimer1.previous = -1;
    _ReadWriteBarrier();

    if ((player->updateTimer2.flags & 1) == 0)
    {
        player->updateTimer2.current = 0;
        player->updateTimer2.previous = 0xfff0bdc1;
        player->updateTimer2.subframe = 0.0f;
        player->updateTimer2.scale = &g_PlayerTimerScale;
        player->updateTimer2.flags |= 1;
    }
    manager = g_AnmRenderManagerView;
    player->updateTimer2.current = 0;
    player->updateTimer2.subframe = 0.0f;
    player->updateTimer2.previous = -1;
    _ReadWriteBarrier();

    GuiView *gui = g_GuiView;
    volatile unsigned int *modeVmId = &player->modeVmId;
    PlayerMarkManagedVmPendingWithManager(manager, *modeVmId);
    *modeVmId = 0;
    *modeVmId = 0;
    GuiSetLivesDisplayCountMember(gui, g_PlayerLivesDisplayCount);
}

static void RemovePlayerCallbackNode(PlayerCallbackNodeView *node)
{
    if (node == NULL)
        return;

    EnterCriticalSection(
        reinterpret_cast<LPCRITICAL_SECTION>(&g_PlayerCallbackCriticalSection));
    ++g_PlayerCallbackLockDepth;
    PlayerUnlinkCallbackNode(node, g_PlayerCallbackManager);
    LeaveCriticalSection(
        reinterpret_cast<LPCRITICAL_SECTION>(&g_PlayerCallbackCriticalSection));
    --g_PlayerCallbackLockDepth;
}

Player::~Player()
{
    RemovePlayerCallbackNode(updateCallbackNode);
    RemovePlayerCallbackNode(drawCallbackNode);
    g_Player = NULL;

    if ((g_PlayerLifecycleFlags & 1) != 0)
    {
        PlayerMarkResourceVmsPending(resource);
        g_PlayerSharedOptionData = optionData;
    }
    else
    {
        if (g_PlayerVmManager->playerCache != NULL)
        {
            PlayerDestroyAnimationCacheContents(g_PlayerVmManager->playerCache);
            free(g_PlayerVmManager->playerCache);
            g_PlayerVmManager->playerCache = NULL;
        }
        if (optionData != NULL)
        {
            free(optionData);
            optionData = NULL;
        }
        g_PlayerSharedOptionData = NULL;
    }

}

Player *PlayerCreate()
{
    Player *player = new Player;
    if (PlayerInitialize(player) != 0)
    {
        delete player;
        return NULL;
    }
    return player;
}

// Target 0x426520: loaded .sht offsets and callback indices are fixed up in place.
// Table bases are observed TH10 storage views; lengths and original owners remain unknown.
extern void *g_PlayerShotSpawnCallbacks[];
extern PlayerShotUpdateCallback g_PlayerShotUpdateCallbacks[];
extern void *g_PlayerShotUnknown2CCallbacks[];
extern PlayerShotCollisionCallback g_PlayerShotCollisionCallbacks[];

struct PlayerShotDataEntryBoundaryView
{
    PlayerShotDescriptorView *descriptors;
    unsigned int unknown04;
};
typedef char PlayerShotDataEntryBoundarySizeIs08[
    (sizeof(PlayerShotDataEntryBoundaryView) == 8) ? 1 : -1];

int PlayerLoadOptionData(Player *player, const char *filename)
{
    player->optionData = reinterpret_cast<PlayerOptionDataView *>(
        FileSystem::OpenFile(filename, NULL, 0));
    if (player->optionData == NULL)
        return -1;

    const double diagonalScale = sin(0.7853982f);
    player->optionData->diagonalSpeedMode0 = static_cast<float>(
        diagonalScale * player->optionData->axisSpeedMode0);
    player->optionData->diagonalSpeedMode1 = static_cast<float>(
        diagonalScale * player->optionData->axisSpeedMode1);
    // The target repeats both writes, reloading the header pointer between them.
    player->optionData->diagonalSpeedMode0 = static_cast<float>(
        diagonalScale * player->optionData->axisSpeedMode0);
    player->optionData->diagonalSpeedMode1 = static_cast<float>(
        diagonalScale * player->optionData->axisSpeedMode1);

    for (int i = 0; i < player->optionData->entryCount; ++i)
    {
        reinterpret_cast<unsigned int &>(
            reinterpret_cast<PlayerShotDataEntryBoundaryView *>(
                reinterpret_cast<unsigned char *>(player->optionData) + 0x110)[i].descriptors) +=
            reinterpret_cast<unsigned int>(player->optionData);
        PlayerShotDescriptorView *descriptor =
            reinterpret_cast<PlayerShotDataEntryBoundaryView *>(
                reinterpret_cast<unsigned char *>(player->optionData) + 0x110)[i].descriptors;
        while (descriptor->fireInterval >= 0)
        {
            descriptor->spawnCallback = g_PlayerShotSpawnCallbacks[
                reinterpret_cast<unsigned int>(descriptor->spawnCallback)];
            descriptor->updateCallback = g_PlayerShotUpdateCallbacks[
                reinterpret_cast<unsigned int>(descriptor->updateCallback)];
            descriptor->unknown2C = g_PlayerShotUnknown2CCallbacks[
                reinterpret_cast<unsigned int>(descriptor->unknown2C)];
            descriptor->collisionCallback = g_PlayerShotCollisionCallbacks[
                reinterpret_cast<unsigned int>(descriptor->collisionCallback)];
            ++descriptor;
        }
    }
    return 0;
}

static AnmFloat3View AddPlayerPositionExtent(
    const AnmFloat3View &extent, const AnmFloat3View &position)
{
    AnmFloat3View result;
    result.x = extent.x + position.x;
    result.y = *static_cast<volatile const float *>(&extent.y) + position.y;
    result.z = extent.z + position.z;
    return result;
}

// Maintained source for the Player initialization owner at
// 0x004247F0-0x00424D8A. Its sole target caller places the newly allocated
// Player in EBX immediately before the call. This ordinary source parameter is
// a maintainable spelling of that private machine boundary.
int PlayerInitialize(Player *player)
{
    const int &character =
        *reinterpret_cast<const int *>(g_GameScoreState.unknown28);
    const int &shotType =
        *reinterpret_cast<const int *>(g_GameScoreState.unknown28 + 4);
    const char *animationFilename =
        character == 0 ? "pl00.anm" : "pl01.anm";
    player->resource = reinterpret_cast<AnmRenderManagerView *>(g_PlayerVmManager)->
        PreloadAnm(8, animationFilename);
    if (player->resource == NULL)
    {
        th10::g_GameErrorContext.Log("\x8e\xa9\x8b\x40\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x0d\x0a");
        return -1;
    }

    if (g_PlayerSharedOptionData != NULL)
    {
        player->optionData = g_PlayerSharedOptionData;
        g_PlayerSharedOptionData = NULL;
    }
    else
    {
        const int dataIndex = shotType + character * 3;
        if (PlayerLoadOptionData(
                player, g_PlayerShotDataFilenames[dataIndex]) != 0)
        {
            th10::g_GameErrorContext.Log("\x8e\xa9\x8b\x40\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x0d\x0a");
            return -1;
        }
    }

    th10::ChainElem *updateNode = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(PlayerUpdateCallback));
    // Capture flags and the current owner before publishing the node fields.
    const unsigned int updateFlags = updateNode->flags;
    th10::Chain *const updateManager = reinterpret_cast<th10::Chain *>(
        *reinterpret_cast<void *volatile *>(&g_PlayerCallbackManager));
    updateNode->arg = player;
    updateNode->flags = updateFlags & ~2u;
    updateManager->AddToCalcChain(updateNode, 0x10);
    player->updateCallbackNode = reinterpret_cast<PlayerCallbackNodeView *>(updateNode);

    th10::ChainElem *drawNode = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(PlayerDrawCallback));
    drawNode->flags &= ~2u;
    // Complete the flags update before publishing the callback argument.
    _ReadWriteBarrier();
    drawNode->arg = player;
    th10::g_Chain->AddToDrawChain(drawNode, 0x16);
    player->drawCallbackNode = reinterpret_cast<PlayerCallbackNodeView *>(drawNode);

    // Target 0x4248B1 calls the real VM initializer at 0x404F30 with
    // Player+0x14, resource at +0x10, and script index zero.
    reinterpret_cast<AnmVmView *>(&player->drawVm)->InitializeForLoadedScript(
        reinterpret_cast<AnmLoadedView *>(player->resource), 0);

    PlayerOptionDataView *data = player->optionData;
    player->drawPosition.x = 0.0f;
    player->drawPosition.y = 400.0f;
    player->positionX = 0;
    player->positionY = 40000;
    player->axisSpeedMode0 = (int)(data->axisSpeedMode0 * 100.0f);
    player->axisSpeedMode1 = (int)(data->axisSpeedMode1 * 100.0f);
    player->diagonalSpeedMode0 = (int)(data->diagonalSpeedMode0 * 100.0f);
    player->diagonalSpeedMode1 = (int)(data->diagonalSpeedMode1 * 100.0f);

    for (int i = 0; i < 33; ++i)
    {
        // The source position is a contiguous two-word storage view.
        player->replayPositionHistory[i] =
            *reinterpret_cast<const PlayerPositionPair *>(&player->positionX);
    }

    if ((player->updateTimer0.flags & 1) == 0)
    {
        player->updateTimer0.current = 0;
        player->updateTimer0.previous = 0xfff0bdc1;
        player->updateTimer0.subframe = 0.0f;
        player->updateTimer0.scale = &g_PlayerTimerScale;
        player->updateTimer0.flags |= 1;
    }
    player->updateTimer0.subframe = -1.0f;
    player->updateTimer0.previous = -2;
    // Complete the preceding stores before publishing the current timer value.
    _ReadWriteBarrier();
    player->updateTimer0.current = -1;
    // Keep the observed timer publication before following data reads.
    _ReadWriteBarrier();

    player->optionData->value04 = g_PlayerOptionValue04ByCharacter[character];
    player->optionData->value0C = g_PlayerOptionValue0CByCharacter[character];
    player->optionData->value08 = g_PlayerOptionValue08ByCharacter[character];

    player->extent0.x = player->extent0.y = player->optionData->value04 * 0.5f;
    player->extent0.z = 5.0f;
    player->extent1.x = player->extent1.y = g_PlayerOptionValue0CByCharacter[character] * 0.5f;
    player->extent1.z = 5.0f;
    player->extent2.x = player->extent2.y = g_PlayerExtent2ByCharacter[character] * 0.5f;
    player->extent2.z = 5.0f;

    const AnmFloat3View &positionValue =
        *reinterpret_cast<const AnmFloat3View *>(&player->drawPosition);
    const AnmFloat3View &extentValue0 =
        *reinterpret_cast<const AnmFloat3View *>(&player->extent0);
    const AnmFloat3View &extentValue1 =
        *reinterpret_cast<const AnmFloat3View *>(&player->extent1);
    const AnmFloat3View &extentValue2 =
        *reinterpret_cast<const AnmFloat3View *>(&player->extent2);
    *reinterpret_cast<AnmFloat3View *>(&player->derivedMin0) =
        positionValue - extentValue0;
    *reinterpret_cast<AnmFloat3View *>(&player->derivedMax0) =
        AddPlayerPositionExtent(extentValue0, positionValue);
    *reinterpret_cast<AnmFloat3View *>(&player->derivedVectors[0]) =
        positionValue - extentValue1;
    *reinterpret_cast<AnmFloat3View *>(&player->derivedVectors[1]) =
        extentValue1 + positionValue;
    *reinterpret_cast<AnmFloat3View *>(&player->derivedVectors[2]) =
        positionValue - extentValue2;
    *reinterpret_cast<AnmFloat3View *>(&player->derivedVectors[3]) =
        extentValue2 + positionValue;
    *reinterpret_cast<AnmFloat3View *>(&player->derivedVectors[4]) =
        positionValue - extentValue2;
    *reinterpret_cast<AnmFloat3View *>(&player->derivedVectors[5]) =
        extentValue2 + positionValue;

    if ((player->updateTimer1.flags & 1) == 0)
    {
        player->updateTimer1.current = 0;
        player->updateTimer1.previous = 0xfff0bdc1;
        player->updateTimer1.subframe = 0.0f;
        player->updateTimer1.scale = &g_PlayerTimerScale;
        player->updateTimer1.flags |= 1;
    }
    player->updateTimer1.current = 0;
    player->updateTimer1.subframe = 0.0f;
    player->updateTimer1.previous = -1;
    // Match the reset path: finish writes before reading the next timer flags.
    _ReadWriteBarrier();

    if ((player->highlightTimer.flags & 1) == 0)
    {
        player->highlightTimer.current = 0;
        player->highlightTimer.previous = 0xfff0bdc1;
        player->highlightTimer.subframe = 0.0f;
        player->highlightTimer.scale = &g_PlayerTimerScale;
        player->highlightTimer.flags |= 1;
    }
    player->highlightTimer.current = 120;
    player->highlightTimer.subframe = 120.0f;
    player->highlightTimer.previous = 119;
    // Finish timer writes before preparing the option-rebuild call.
    _ReadWriteBarrier();
    player->optionTransitionFrames = 30;

    RebuildPlayerOptions(player);
    return 0;
}

// Maintained source for the Player movement/options owner at 0x004250B0.
// The target body receives Player in live-in EDI from its sole caller. This
// ordinary source parameter is a maintainable spelling of that behavior, not a
// claim about the original declaration or the optimizer-owned machine ABI.
// Explicit embedded-storage ABI view; this assertion does not establish C++
// object-lifetime/aliasing closure between the legacy draw view and AnmVmView.
typedef char PlayerMovementVmAt14[(offsetof(Player, drawVm) == 0x14) ? 1 : -1];
typedef char PlayerMovementVmSize[(sizeof(PlayerDrawVmView) == sizeof(AnmVmView)) ? 1 : -1];

int PlayerUpdateMovementAndOptions(Player *player)
{
    int horizontalSpeed = 0;
    int verticalSpeed = 0;

    if ((g_PlayerInputBits & 0x50) == 0x50)
        player->movementDirection = PLAYER_DIRECTION_UP_LEFT;
    else if ((g_PlayerInputBits & 0x60) == 0x60)
        player->movementDirection = PLAYER_DIRECTION_DOWN_LEFT;
    else if ((g_PlayerInputBits & 0x90) == 0x90)
        player->movementDirection = PLAYER_DIRECTION_UP_RIGHT;
    else if ((g_PlayerInputBits & 0xa0) == 0xa0)
        player->movementDirection = PLAYER_DIRECTION_DOWN_RIGHT;
    else if ((g_PlayerInputBits & 0x20) != 0)
        player->movementDirection = PLAYER_DIRECTION_DOWN;
    else if ((g_PlayerInputBits & 0x10) != 0)
        player->movementDirection = PLAYER_DIRECTION_UP;
    else if ((g_PlayerInputBits & 0x40) != 0)
        player->movementDirection = PLAYER_DIRECTION_LEFT;
    else if ((g_PlayerInputBits & 0x80) != 0)
        player->movementDirection = PLAYER_DIRECTION_RIGHT;
    else
        player->movementDirection = PLAYER_DIRECTION_NONE;

    if (g_PlayerDrawGate10 == NULL ||
        g_PlayerDrawGate10->value60 == 0 ||
        player->updateTimer2.current < 4)
    {
        player->optionMode = 0;
        player->optionTransitionFrames = 30;
    }
    else
    {
        player->optionMode = (g_PlayerInputBits >> 2) & 1;
        if (g_PlayerShotType + g_PlayerCharacter * 3 == 5)
        {
            if ((g_PlayerInputBits & 4) != 0)
                player->optionTransitionFrames = 0;
            else if (player->optionTransitionFrames < 30)
                ++player->optionTransitionFrames;
        }
    }

    if (player->optionMode != 0)
    {
        if (player->modeVmId == 0)
        {
            AnmLoadedView *loaded = reinterpret_cast<BulletManagerView *>(
                g_EnemyPrimaryResourceOwner)->bulletAnm;
            AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
            vm->renderLayer = PLAYER_MODE_VM_LAYER;
            vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
            loaded->InitializeVm(
                vm, PLAYER_MODE_VM_SCRIPT);
            player->modeVmId = g_AnmRenderManagerView->AddVmVariant0(vm).value;
        }

        // Lexical order follows the target's mode-1 switch body order. The
        // compiler emits the eight-entry table separately after the function.
        switch (player->movementDirection)
        {
        case PLAYER_DIRECTION_RIGHT:
            horizontalSpeed = player->axisSpeedMode1;
            break;
        case PLAYER_DIRECTION_LEFT:
            horizontalSpeed = -player->axisSpeedMode1;
            break;
        case PLAYER_DIRECTION_UP:
            verticalSpeed = -player->axisSpeedMode1;
            break;
        case PLAYER_DIRECTION_DOWN:
            verticalSpeed = player->axisSpeedMode1;
            break;
        case PLAYER_DIRECTION_UP_LEFT:
            horizontalSpeed = -player->diagonalSpeedMode1;
            verticalSpeed = horizontalSpeed;
            break;
        case PLAYER_DIRECTION_DOWN_LEFT:
            verticalSpeed = player->diagonalSpeedMode1;
            horizontalSpeed = -verticalSpeed;
            break;
        case PLAYER_DIRECTION_UP_RIGHT:
            horizontalSpeed = player->diagonalSpeedMode1;
            verticalSpeed = -horizontalSpeed;
            break;
        case PLAYER_DIRECTION_DOWN_RIGHT:
            horizontalSpeed = player->diagonalSpeedMode1;
            verticalSpeed = horizontalSpeed;
            break;
        default:
            break;
        }
    }
    else
    {
        if (FindPlayerVm(player->modeVmId) != NULL)
            g_AnmRenderManagerView->SetVmPendingInterrupt(
                AnmVmIdView(player->modeVmId), 1);
        player->modeVmId = 0;

        // Same target-observed lexical block order as the mode-1 switch.
        switch (player->movementDirection)
        {
        case PLAYER_DIRECTION_RIGHT:
            horizontalSpeed = player->axisSpeedMode0;
            break;
        case PLAYER_DIRECTION_LEFT:
            horizontalSpeed = -player->axisSpeedMode0;
            break;
        case PLAYER_DIRECTION_UP:
            verticalSpeed = -player->axisSpeedMode0;
            break;
        case PLAYER_DIRECTION_DOWN:
            verticalSpeed = player->axisSpeedMode0;
            break;
        case PLAYER_DIRECTION_UP_LEFT:
            horizontalSpeed = -player->diagonalSpeedMode0;
            verticalSpeed = horizontalSpeed;
            break;
        case PLAYER_DIRECTION_DOWN_LEFT:
            verticalSpeed = player->diagonalSpeedMode0;
            horizontalSpeed = -verticalSpeed;
            break;
        case PLAYER_DIRECTION_UP_RIGHT:
            horizontalSpeed = player->diagonalSpeedMode0;
            verticalSpeed = -horizontalSpeed;
            break;
        case PLAYER_DIRECTION_DOWN_RIGHT:
            horizontalSpeed = player->diagonalSpeedMode0;
            verticalSpeed = horizontalSpeed;
            break;
        default:
            break;
        }
    }

    if (horizontalSpeed < 0 && player->previousHorizontalSpeed >= 0)
        static_cast<AnmLoadedView *>(player->resource)->
            InitializeAndExecuteScriptIndex(
                reinterpret_cast<AnmVmView *>(&player->drawVm),
                PLAYER_MAIN_VM_MOVE_LEFT);
    else if (horizontalSpeed > 0 && player->previousHorizontalSpeed <= 0)
        static_cast<AnmLoadedView *>(player->resource)->
            InitializeAndExecuteScriptIndex(
                reinterpret_cast<AnmVmView *>(&player->drawVm),
                PLAYER_MAIN_VM_MOVE_RIGHT);
    else if (horizontalSpeed == 0 && player->previousHorizontalSpeed < 0)
        static_cast<AnmLoadedView *>(player->resource)->
            InitializeAndExecuteScriptIndex(
                reinterpret_cast<AnmVmView *>(&player->drawVm),
                PLAYER_MAIN_VM_STOP_LEFT);
    else if (horizontalSpeed == 0 && player->previousHorizontalSpeed > 0)
        static_cast<AnmLoadedView *>(player->resource)->
            InitializeAndExecuteScriptIndex(
                reinterpret_cast<AnmVmView *>(&player->drawVm),
                PLAYER_MAIN_VM_STOP_RIGHT);

    player->previousHorizontalSpeed = horizontalSpeed;
    player->previousVerticalSpeed = verticalSpeed;
    player->movementDeltaX = (int)((float)horizontalSpeed * g_PlayerTimerScale);
    player->movementDeltaY = (int)((float)verticalSpeed * g_PlayerTimerScale);
    player->positionX += player->movementDeltaX;
    player->positionY += player->movementDeltaY;

    if (player->positionX < -18400)
        player->positionX = -18400;
    else if (player->positionX > 18400)
        player->positionX = 18400;
    if (player->positionY < 3200)
        player->positionY = 3200;
    else if (player->positionY > 43200)
        player->positionY = 43200;

    player->drawPosition.x = (float)player->positionX * 0.01f;
    player->drawPosition.y = (float)player->positionY * 0.01f;

    if (FindPlayerVm(player->modeVmId) == NULL)
    {
        player->modeVmId = 0;
    }
    else
    {
        AnmFloat3View position;
        position.x = player->drawPosition.x + 224.0f;
        position.y = player->drawPosition.y + 16.0f;
        position.z = player->drawPosition.z;
        g_AnmRenderManagerView->SetVmPosition(
            AnmVmIdView(player->modeVmId), &position);
    }

    if (player->optionMode == 0 &&
        (horizontalSpeed != 0 || verticalSpeed != 0))
    {
        int historyIndex = 32;
        const int historyLimit = player->optionCount * 8;
        for (; historyIndex > historyLimit; --historyIndex)
            player->replayPositionHistory[historyIndex] =
                player->replayPositionHistory[historyLimit];
        for (; historyIndex > 0; --historyIndex)
            player->replayPositionHistory[historyIndex] =
                player->replayPositionHistory[historyIndex - 1];
    }
    player->replayPositionHistory[0].x = player->positionX;
    player->replayPositionHistory[0].y = player->positionY;

    for (int i = 0; i < 4; ++i)
    {
        PlayerOptionRuntime &option = player->options[i];
        if (option.state == 0)
            continue;

        const PlayerPositionPair &offset =
            player->optionMode == 0 ? option.replayPair2 : option.replayPair3;
        option.replayPair0.x = player->positionX + offset.x;
        option.replayPair0.y = player->positionY + offset.y;

        if (option.updateCallback != NULL)
            option.updateCallback(&option);

        if (option.resetFlag == 0)
        {
            if (player->optionTransitionFrames >= 30)
            {
                const int stepX =
                    ((option.replayPair0.x - option.replayPair1.x) *
                     player->optionTransitionFrames) / 100;
                const int stepY =
                    ((option.replayPair0.y - option.replayPair1.y) *
                     player->optionTransitionFrames) / 100;
                if (stepX != 0 || stepY != 0)
                {
                    option.replayPair1.x += stepX;
                    option.replayPair1.y += stepY;
                }
                else
                {
                    option.replayPair1 = option.replayPair0;
                }
            }
        }
        else
        {
            option.resetFlag = 0;
            option.replayPair1 = option.replayPair0;
        }

        AnmFloat3View optionPosition;
        optionPosition.x = (float)option.replayPair1.x * 0.01f;
        optionPosition.y = (float)option.replayPair1.y * 0.01f;
        optionPosition.z = 0.0f;
        AnmRenderManagerView *manager = g_AnmRenderManagerView;
        manager->SetVmWorldPosition(AnmVmIdView(option.primaryVmId), &optionPosition);
        manager->SetVmWorldPosition(AnmVmIdView(option.secondaryVmId), &optionPosition);
    }

    return 0;
}

// These C-linkage names describe four target-referenced float slots. The
// original global names and data owner remain unknown.
extern "C" {
    extern const float g_PlayerShotPlayfieldMinimumX;
    extern const float g_PlayerShotPlayfieldMaximumX;
    extern const float g_PlayerShotPlayfieldMinimumY;
    extern const float g_PlayerShotPlayfieldMaximumY;
}

// Target 0x00428D70 returns zero while the shot bounds overlap the playable
// rectangle and one otherwise. The address names below are local descriptions;
// the original constant-owner names remain unknown.
struct PlayerShotBoundsView
{
    float x;
    float y;

    int IsOutsidePlayfield(float extentX, float extentY);
};
typedef char PlayerShotBoundsViewSizeIs08[
    (sizeof(PlayerShotBoundsView) == 0x08) ? 1 : -1];

int PlayerShotBoundsView::IsOutsidePlayfield(
    float extentX, float extentY)
{
    if (!(x + extentX <= g_PlayerShotPlayfieldMinimumX) &&
        !(x - extentX >= g_PlayerShotPlayfieldMaximumX) &&
        !(y + extentY <= g_PlayerShotPlayfieldMinimumY) &&
        !(y - extentY >= g_PlayerShotPlayfieldMaximumY))
    {
        return 0;
    }
    return 1;
}

// Both loops and the zero-ID path are inlined in target0x427E90.
static __forceinline AnmVmView *LookupPlayerSpawnedShotVm(AnmVmIdView id)
{
    if (id.value == 0)
        return NULL;
    AnmVmLayerNodeView *node = g_AnmRenderManagerView->primaryVmListHead;
    while (node != NULL)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == id.value)
            return static_cast<AnmVmView *>(node->owner);
        node = node->next;
    }
    node = g_AnmRenderManagerView->secondaryVmListHead;
    while (node != NULL)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == id.value)
            return static_cast<AnmVmView *>(node->owner);
        node = node->next;
    }
    return NULL;
}

static __forceinline AnmVmView *FindPlayerSpawnedShotVm(AnmVmIdView *id)
{
    AnmVmView *vm = LookupPlayerSpawnedShotVm(*id);
    if (vm == NULL)
        id->value = 0;
    return vm;
}

typedef char PlayerShotOptionReplaySourceAt3244[
    (offsetof(Player, options) + offsetof(PlayerOptionRuntime, replayPair1) -
     sizeof(PlayerOptionRuntime) == 0x3244) ? 1 : -1];

// Target 0x427E90..0x42815C. The original producer name and TU are unknown.
// It selects one of the existing128 rows and initializes its descriptor/VMs.
// The ordinary member currently lowers to three stack values and RET12.
extern unsigned char g_MainSoundOwner[0x52d0]; // shared Main.hpp declaration
typedef void (__fastcall *PlayerShotSpawnCallback)(
    Player *player, PlayerShotRuntimeView *shot, int frame);

int Player::SpawnShot(const PlayerShotDescriptorView *descriptor, int frame)
{
    Player *player = this;
    PlayerShotRuntimeView *shot = &player->shots[0];
    if (descriptor->type == PLAYER_SHOT_TYPE_3 &&
        player->shotSourceActive[descriptor->sourceIndex - 1] != 0)
        return 0;
    for (int count = 0; count < 128; ++count, ++shot)
    {
        if (shot->state != PLAYER_SHOT_INACTIVE)
            continue;

        shot->state = PLAYER_SHOT_ACTIVE;
        shot->descriptor = descriptor;
        shot->timer.SetCurrent(0);
        if (descriptor->sourceIndex == 0)
        {
            shot->motion.position = player->drawPosition;
        }
        else
        {
            const PlayerPositionPair *source =
                &player->options[descriptor->sourceIndex - 1].replayPair1;
            PlayerFloat3 point;
            point.x = static_cast<float>(source->x) * 0.01f;
            point.y = static_cast<float>(source->y) * 0.01f;
            point.z = 0.0f;
            shot->motion.position = point;
        }
        if (descriptor->type == PLAYER_SHOT_TYPE_3)
            player->shotSourceActive[descriptor->sourceIndex - 1] = 1;
        shot->motion.speedOrAngleStep = descriptor->speed;
        shot->motion.angle = EnemyWrapAngle(descriptor->angle);
        PlayerShotMotionView *motion = &shot->motion;
        if ((motion->flags & PLAYER_SHOT_POLAR_MOTION) == 0)
        {
            reinterpret_cast<EnemyRuntimeVectorView *>(
                &motion->velocity)->FromAngleMagnitude(
                    motion->angle, motion->speedOrAngleStep);
            motion->velocity.z = 0.0f;
        }
        else
        {
            motion->polarMagnitude += motion->polarMagnitudeDelta;
            motion->angle = EnemyWrapAngle(motion->speedOrAngleStep + motion->angle);
        }
        shot->motion.position.x += descriptor->spawnOffsetX - shot->motion.velocity.x;
        shot->motion.position.y += descriptor->spawnOffsetY - shot->motion.velocity.y;

        int script = descriptor->animationScript + 5;
        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(player->resource);
        AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
        vm->renderLayer = PLAYER_VM_LAYER;
        vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
        loaded->InitializeVm(vm, script);
        AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
        shot->primaryVmId = id.value;
        vm = FindPlayerSpawnedShotVm(
            reinterpret_cast<AnmVmIdView *>(&shot->primaryVmId));
        if ((vm->flags35C & PLAYER_VM_SHOT_ANGLE_SOURCE) != 0)
        {
            reinterpret_cast<PlayerVm *>(vm)->angle2C = descriptor->angle;
            vm->flags35C |= PLAYER_VM_SHOT_ANGLE_DIRTY;
        }
        if (descriptor->type == PLAYER_SHOT_TYPE_3)
        {
            loaded = static_cast<AnmLoadedView *>(player->resource);
            vm = g_AnmRenderManagerView->AllocateVm();
            vm->renderLayer = PLAYER_VM_LAYER;
            vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
            loaded->InitializeVm(vm, 16);
            AnmVmIdView secondary = g_AnmRenderManagerView->AddVmVariant0(vm);
            shot->secondaryVmId = secondary.value;
        }
        else
        {
            shot->secondaryVmId = 0;
        }
        if (descriptor->spawnCallback != NULL)
            reinterpret_cast<PlayerShotSpawnCallback>(descriptor->spawnCallback)(
                player, shot, frame);
        if (descriptor->soundId >= 0)
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundCue(
                descriptor->soundId, shot->motion.position.x);
        return 0;
    }
    return 0;
}

// Target 0x428160: power/mode select one fixed-up descriptor list, with a
// negative fireInterval sentinel. Each eligible row consumes the current frame.
int PlayerEmitShotRows(Player *player, int frame)
{
    int index = static_cast<unsigned short>(g_PlayerPower) / 20;
    if (index >= 5)
        index = 4;
    if (player->optionMode != 0)
        index += 5;
    PlayerShotDescriptorView *row =
        reinterpret_cast<PlayerShotDataEntryBoundaryView *>(
            reinterpret_cast<unsigned char *>(player->optionData) + 0x110)[index].descriptors;
    while (row->fireInterval >= 0)
    {
        if (frame % row->fireInterval == row->fireFrame)
            player->SpawnShot(row, frame);
        ++row;
    }
    return 0;
}

// Target 0x4281D0: update the input-driven shot timer only in the active state.
int PlayerUpdateInputAction(Player *player)
{
    if (player->runtimeState == 1)
    {
        if (player->updateTimer0.current < 0)
        {
            if ((g_PlayerInputBits & 1u) == 0)
                return 0;
            reinterpret_cast<AnmVmTimerView *>(&player->updateTimer0)->SetCurrent(0);
        }
        if (player->updateTimer0.HasChanged())
            PlayerEmitShotRows(player, player->updateTimer0.current);
        if (player->updateTimer0.current >= 15)
        {
            if ((g_PlayerInputBits & 1u) != 0)
            {
                reinterpret_cast<AnmVmTimerView *>(&player->updateTimer0)->Add(-15.0f);
                return 0;
            }
            reinterpret_cast<AnmVmTimerView *>(&player->updateTimer0)->SetCurrent(-1);
            return 0;
        }
        reinterpret_cast<AnmVmTimerView *>(&player->updateTimer0)->Tick();
        return 0;
    }
    player->trackedEnemy = NULL;
    player->trackedEnemyValid = 0;
    return 0;
}

// Both target callers pass the same 0x2C motion layout to 0x44C2A0.
typedef char PlayerShotMotionMatchesEnemyMotion[
    (sizeof(PlayerShotMotionView) == sizeof(EnemyMotionView) &&
     offsetof(PlayerShotMotionView, position) == offsetof(EnemyMotionView, position) &&
     offsetof(PlayerShotMotionView, velocity) == offsetof(EnemyMotionView, velocity) &&
     offsetof(PlayerShotMotionView, speedOrAngleStep) == offsetof(EnemyMotionView, value18) &&
     offsetof(PlayerShotMotionView, angle) == offsetof(EnemyMotionView, value1C) &&
     offsetof(PlayerShotMotionView, polarMagnitude) == offsetof(EnemyMotionView, value20) &&
     offsetof(PlayerShotMotionView, polarMagnitudeDelta) == offsetof(EnemyMotionView, value24) &&
     offsetof(PlayerShotMotionView, flags) == offsetof(EnemyMotionView, flags)) ? 1 : -1];

// Maintained source for the 128-row Player shot-update owner at
// 0x00428280-0x004285EB. The target receives one stack Player* and returns zero
// with RET 4. The 0x004282AA-0x004282AF gap is unreachable compiler alignment,
// not a separate data owner. Shared callees are backed by maintained source;
// the original owner name, translation unit and inline boundaries remain unknown.
int __stdcall PlayerUpdateShots(Player *player)
{
    PlayerShotRuntimeView *shot = &player->shots[0];
    for (int remainingShots = 128; remainingShots != 0; --remainingShots, ++shot)
    {
        if (shot->state == PLAYER_SHOT_INACTIVE)
            continue;

        const PlayerShotDescriptorView *descriptor = shot->descriptor;
        if (descriptor->type == PLAYER_SHOT_TYPE_3 &&
            shot->state == PLAYER_SHOT_ACTIVE &&
            (player->updateTimer0.current < 0 ||
             (int)descriptor->sourceIndex - 1 >= player->optionCount))
        {
            EnemyMarkPendingInterrupt(reinterpret_cast<int *>(&shot->primaryVmId));
            EnemyMarkPendingInterrupt(reinterpret_cast<int *>(&shot->secondaryVmId));
            descriptor = shot->descriptor;
            shot->state = PLAYER_SHOT_HIT_TRANSITION;
            player->shotSourceActive[(int)descriptor->sourceIndex - 1] = 0;
        }

        descriptor = shot->descriptor;
        if (descriptor->type == PLAYER_SHOT_TYPE_3 &&
            shot->state == PLAYER_SHOT_ACTIVE &&
            ((g_PlayerDrawGate9EB8 != NULL &&
              g_PlayerDrawGate9EB8->value9EB8 != 0) ||
             g_PlayerDrawGate10 == NULL))
        {
            shot->state = PLAYER_SHOT_HIT_TRANSITION;
            EnemyMarkPendingInterrupt(reinterpret_cast<int *>(&shot->primaryVmId));
            EnemyMarkPendingInterrupt(reinterpret_cast<int *>(&shot->secondaryVmId));
            descriptor = shot->descriptor;
            player->shotSourceActive[(int)descriptor->sourceIndex - 1] = 0;
        }

        descriptor = shot->descriptor;
        if (descriptor->type == PLAYER_SHOT_TYPE_3 &&
            shot->collidedThisFrame == 0 &&
            shot->state == PLAYER_SHOT_ACTIVE &&
            shot->collisionVmTransitionPending == 1)
        {
            ResultMenuPendingThree(
                reinterpret_cast<AnmVmIdView *>(&shot->primaryVmId));
            shot->collisionVmTransitionPending = 0;
        }
        descriptor = shot->descriptor;
        shot->collidedThisFrame = 0;

        PlayerShotUpdateCallback updateCallback = descriptor->updateCallback;
        if (updateCallback != NULL)
            updateCallback(player, shot);

        PlayerShotMotionView *motion = &shot->motion;
        if ((motion->flags & PLAYER_SHOT_POLAR_MOTION) == 0)
        {
            reinterpret_cast<EnemyRuntimeVectorView *>(
                &motion->velocity)->FromAngleMagnitude(
                    motion->angle, motion->speedOrAngleStep);
            motion->velocity.z = 0.0f;
        }
        else
        {
            motion->polarMagnitude += motion->polarMagnitudeDelta;
            motion->angle = EnemyWrapAngle(
                motion->speedOrAngleStep + motion->angle);
        }

        EnemyAdvanceMotion(reinterpret_cast<EnemyMotionView *>(motion));

        PlayerVm *primaryVm = reinterpret_cast<PlayerVm *>(
            g_AnmRenderManagerView->FindVm(*reinterpret_cast<AnmVmIdView *>(&shot->primaryVmId)));
        if (primaryVm == NULL)
        {
            shot->state = PLAYER_SHOT_INACTIVE;
            ReleasePlayerShotVm(reinterpret_cast<AnmVmIdView *>(&shot->secondaryVmId));
            shot->primaryVmId = 0;
            shot->secondaryVmId = 0;
            continue;
        }

        // Reload after update callbacks and motion, as in the target.
        descriptor = shot->descriptor;
        if (descriptor->type != PLAYER_SHOT_TYPE_3 &&
            shot->timer.current >= 10)
        {
            const float extentX =
                primaryVm->sprite->extent34 * primaryVm->scale3C;
            const float extentY =
                primaryVm->sprite->extent30 * primaryVm->scale40;
            if (reinterpret_cast<PlayerShotBoundsView *>(
                    &motion->position)->IsOutsidePlayfield(
                    extentX, extentY) != 0)
            {
                g_AnmRenderManagerView->MarkVmForDeletion(*reinterpret_cast<AnmVmIdView *>(&shot->primaryVmId));
                shot->primaryVmId = 0;
                shot->state = PLAYER_SHOT_INACTIVE;
                continue;
            }
        }

        primaryVm->position.x = motion->position.x + 224.0f;
        primaryVm->position.y = shot->motion.position.y + 16.0f;
        primaryVm->position.z = shot->motion.position.z;

        if (reinterpret_cast<AnmVmIdView *>(&shot->secondaryVmId)->value != 0)
        {
            PlayerVm *secondaryVm = reinterpret_cast<PlayerVm *>(
                g_AnmRenderManagerView->FindVm(*reinterpret_cast<AnmVmIdView *>(&shot->secondaryVmId)));
            if (secondaryVm == NULL)
            {
                shot->secondaryVmId = 0;
            }
            else
            {
                secondaryVm->position.x = motion->position.x + 224.0f;
                secondaryVm->position.y = shot->motion.position.y + 16.0f;
                secondaryVm->position.z = shot->motion.position.z;
            }
        }

        if ((primaryVm->flags & PLAYER_VM_SHOT_ANGLE_SOURCE) != 0)
        {
            primaryVm->angle2C = shot->motion.angle;
            primaryVm->flags |= PLAYER_VM_SHOT_ANGLE_DIRTY;
        }

        shot->timer.previous = shot->timer.current;
        if (*shot->timer.scale > 0.99f && *shot->timer.scale < 1.01f)
        {
            ++shot->timer.current;
            shot->timer.subframe += 1.0f;
        }
        else
        {
            shot->timer.Add(*shot->timer.scale);
        }
    }

    return 0;
}

// Maintained source for the 0x00428630-0x00428AC1 Player damage owner. The
// logical source shape is a Player member with three arguments. The target's
// optimized private boundary rotates those four logical values: optionalHitFlag
// is live in ECX, while Player*, targetPosition, and targetSize are stack-bound,
// and the body returns with RET 12. This spelling does not claim the original
// identifier, translation unit, or production optimizer profile.
int Player::CalculateDamageToTarget(
    const PlayerFloat3 *targetPosition,
    const PlayerFloat3 *targetSize,
    int *optionalHitFlag)
{
    int damage = 0;

    if (updateTimer1.current == updateTimer1.previous)
        return 0;

    PlayerShotRuntimeView *shot = &shots[0];
    PlayerFloat3 targetMin;
    PlayerFloat3 targetMax;
    targetPosition->GetPlanarBounds(&targetMin, &targetMax, &targetSize->x);

    if (optionalHitFlag != NULL)
        *optionalHitFlag = 0;

    // Collision bounds use the existing three-component vector slots;
    // this two-dimensional overlap test only consumes X and Y.

    for (int remainingShots = 128; remainingShots != 0; --remainingShots, ++shot)
    {
        if (shot->state == PLAYER_SHOT_INACTIVE ||
            shot->state == PLAYER_SHOT_HIT_TRANSITION)
        {
            continue;
        }

        const PlayerShotDescriptorView *descriptor = shot->descriptor;
        PlayerFloat3 shotMin;
        PlayerFloat3 shotMax;
        shot->motion.position.GetPlanarBounds(
            &shotMin, &shotMax, &descriptor->hitboxExtentX);

        if (shotMin.y > targetMax.y || shotMin.x > targetMax.x ||
            shotMax.y < targetMin.y || shotMax.x < targetMin.x)
        {
            continue;
        }

        if (descriptor->type != PLAYER_SHOT_TYPE_3 && shotMin.y < 0.0f)
            continue;
        if (descriptor->type == PLAYER_SHOT_TYPE_3 && targetMax.y < 0.0f)
            continue;

        if (descriptor->collisionCallback != NULL &&
            descriptor->collisionCallback(this, shot, targetPosition) != 0)
        {
            continue;
        }

        if (shot->collisionVmTransitionPending == 0)
        {
            PlayerSetShotVmInterrupt2(&shot->primaryVmId);
            shot->collisionVmTransitionPending = 1;
        }
        descriptor = shot->descriptor;
        shot->collidedThisFrame = 1;
        const unsigned char collisionType = descriptor->type;

        if (collisionType != PLAYER_SHOT_TYPE_3 ||
            shot->timer.HasIntervalTick(4) != 0)
        {
            damage += descriptor->damage;
        }

        if (collisionType != PLAYER_SHOT_TYPE_3)
        {
            AnmVmIdView *const vmId =
                reinterpret_cast<AnmVmIdView *>(&shot->primaryVmId);
            AnmVmView *vm = vmId->GetVm();
            const float savedAngle = vm->rotation.z;

            vmId->Release();

            shot->primaryVmId =
                static_cast<AnmLoadedView *>(resource)->CreateVmVariant0(
                    shot->descriptor->hitAnimationScript + 5, PLAYER_VM_LAYER).value;
            AnmVmView *hitVm = vmId->GetVm();
            hitVm->rotation.z = savedAngle;
            hitVm->flags35C |= PLAYER_VM_SHOT_ANGLE_DIRTY;

            shot->motion.position.z = 0.1f;
            shot->state = PLAYER_SHOT_HIT_TRANSITION;
            shot->motion.speedOrAngleStep *= 0.125f;
        }

        descriptor = shot->descriptor;
        if (descriptor->type == 2)
        {
            SpawnEffectRow(
                &shot->motion.position,
                32.0f, 1.3999999761581421f, 13, descriptor->damage / 3);
        }
    }

    damage += g_PlayerUpdateGate28->GetDamageAtPosition(targetPosition);

    PlayerEffectRowView *row = &effectRows[0];
    for (int remainingRows = 32; remainingRows != 0; --remainingRows, ++row)
    {
        PlayerEffectMotionView *motion = &row->motion;
        if ((motion->activeFlags & 1) == 0)
            continue;

        if (motion->timer.current != motion->timer.previous &&
            motion->timer.current % motion->collisionInterval == 0)
        {
            continue;
        }

        if ((motion->activeFlags & 2) == 0)
        {
            if (row->angle == 0.0f)
            {
                const float halfExtentX = row->extentX * 0.5f;
                if (row->position.x - halfExtentX > targetMax.x ||
                    row->position.x + halfExtentX < targetMin.x)
                {
                    continue;
                }
                const float halfExtentY = row->extentY * 0.5f;
                if (row->position.y - halfExtentY > targetMax.y ||
                    row->position.y + halfExtentY < targetMin.y)
                {
                    continue;
                }
            }
            else
            {
                const float dx = targetPosition->x - row->position.x;
                const float dy = targetPosition->y - row->position.y;
                const float sine = (float)sin(-(double)row->angle);
                const float cosine = (float)cos(-(double)row->angle);
                PlayerFloat3 rotated;
                rotated.x = cosine * dx - sine * dy;
                rotated.y = sine * dx + cosine * dy;
                // Rotated collision reads the current target size after the
                // shot callbacks; the axis-aligned bounds are cached earlier.
                const float rotatedTargetHalfX = targetSize->x * 0.5f;
                if (row->extentX * -0.5f > rotatedTargetHalfX + rotated.x ||
                    row->extentX * 0.5f < rotated.x - rotatedTargetHalfX)
                {
                    continue;
                }
                const float rotatedTargetHalfY = targetSize->y * 0.5f;
                if (row->extentY * -0.5f > rotatedTargetHalfY + rotated.y ||
                    row->extentY * 0.5f < rotated.y - rotatedTargetHalfY)
                {
                    continue;
                }
            }
        }
        else
        {
            const float radius = row->radius;
            const float dy = row->position.y - targetPosition->y;
            const float dx = row->position.x - targetPosition->x;
            if (radius * radius < dx * dx + dy * dy)
                continue;
        }

        damage += motion->damage;
        motion->hitAccumulator += motion->damage;
        if (motion->hitCap > motion->hitAccumulator)
            continue;
        motion->damage = 0;
    }

    if (damage != 0)
    {
        g_GameScore += (damage / 10 + 10) / 10;
        if (g_GameScore >= 1000000000)
            g_GameScore = 999999999;
    }

    return damage;
}

// Target 0x00425730-0x00426340 receives the Player owner on the stack and owns
// the complete update state machine. PlayerUpdateCallback is its separately
// retained ECX-bound chain adapter at 0x00426500.
int __stdcall PlayerUpdateCallbackBody(Player *player)
{
    int i;

    // The lexical case order follows the target's physical block order and
    // compiler-owned five-entry jump table: 0, 1, 4, 2, 3.
    switch (player->runtimeState)
    {
    case 0:
    {
        player->positionY =
            48000 - (player->updateTimer1.current * 8000) / 60;
        player->drawPosition.y = (float)player->positionY * 0.01f;

        player->options[0].resetFlag = 1;
        player->options[1].resetFlag = 1;
        player->options[2].resetFlag = 1;
        player->options[3].resetFlag = 1;

        for (i = 32; i > player->optionCount * 8; --i)
            player->replayPositionHistory[i] =
                player->replayPositionHistory[player->optionCount * 8];
        for (; i > 0; --i)
            player->replayPositionHistory[i] =
                player->replayPositionHistory[i - 1];
        player->replayPositionHistory[0].x = player->positionX;
        player->replayPositionHistory[0].y = player->positionY;

        if (player->updateTimer1.current >= 30)
        {
            PlayerHostileRowView *row =
                reinterpret_cast<PlayerHostileManagerView *>(
                    g_EnemyPrimaryResourceOwner)->rows;
            for (i = 0; i < 2000; ++i, ++row)
            {
                if (row->state446 != 0 && row->state446 != 3 && row->value4 == 0)
                    EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(row));
            }

            EnemyEffectWaitNode *object =
                reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager)->head;
            while (object != NULL)
            {
                EnemyEffectWaitNode *next = object->next;
                if (object->state != 1)
                    object->ApplyClear(0);
                object = next;
            }
        }
        else
        {
            float value =
                (float)player->updateTimer1.current * 17.066667556762695f +
                64.0f;
            reinterpret_cast<EnemyCancelManagerView *>(
                g_EnemyPrimaryResourceOwner)->CancelBulletPattern(
                    &player->savedDrawPosition, value, 0, 1);
            reinterpret_cast<EnemyCancelManagerView *>(
                g_EnemyPrimaryResourceOwner)->CancelBulletPattern(
                    &player->savedDrawPosition, value * 0.25f, 0, 0);
            reinterpret_cast<EnemyEffectWaitManager *>(
                g_EnemyLaserManager)->ApplyPatternCancel(
                    &player->savedDrawPosition, value, 0);
        }

        if (player->updateTimer1.current < 60)
            break;

        player->runtimeState = 1;
        if ((player->updateTimer1.flags & 1) == 0)
        {
            player->updateTimer1.current = 0;
            player->updateTimer1.previous = 0xfff0bdc1;
            player->updateTimer1.subframe = 0.0f;
            player->updateTimer1.scale = &g_AnmGameSpeed;
            player->updateTimer1.flags |= 1;
        }
        player->updateTimer1.current = 0;
        player->updateTimer1.subframe = 0.0f;
        player->updateTimer1.previous = -1;
        // Fall through into the target's physical state-1 block.
    }
    case 1:
        if (g_PlayerDrawGate9EB8 != NULL &&
            g_PlayerDrawGate9EB8->value9EB8 == 0 &&
            g_PlayerUpdateGate28 != NULL &&
            g_PlayerUpdateGate28->value28 == 0 &&
            g_PlayerPower / 20 != 0 &&
            (g_PlayerInputBits & 2) != 0)
        {
            reinterpret_cast<AnmVmTimerView *>(&player->highlightTimer)->SetCurrent(0x10e);
            g_PlayerUpdateGate28->StartPowerTransition();
            g_PlayerPower -= 20;
            RebuildPlayerOptions(player);
            int power = g_PlayerPower;
            g_ItemPowerDisplayOwner->UpdatePowerDisplay(
                power / 20, ((power % 20) * 100) / 20);
            g_GameScoreState.DecayFaith(3000);
        }

        if (player->updateTimer1.current < 30)
        {
            PlayerHostileRowView *row =
                reinterpret_cast<PlayerHostileManagerView *>(
                    g_EnemyPrimaryResourceOwner)->rows;
            for (i = 0; i < 2000; ++i, ++row)
            {
                if (row->state446 != 0 && row->state446 != 3 && row->value4 == 0)
                    EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(row));
            }

            EnemyEffectWaitNode *object =
                reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager)->head;
            while (object != NULL)
            {
                EnemyEffectWaitNode *next = object->next;
                if (object->state != 1)
                    object->ApplyClear(0);
                object = next;
            }
        }
        PlayerUpdateMovementAndOptions(player);
        break;

    case 4:
        if (player->updateTimer1.current >= 8)
        {
            PlayerTransitionToState2(player);
        }
        else
        {
            if (g_PlayerUpdateGate28 != NULL &&
                g_PlayerUpdateGate28->value28 == 0 &&
                g_PlayerPower / 20 != 0 &&
                (g_PlayerInputBits & 2) != 0)
            {
                reinterpret_cast<AnmVmTimerView *>(&player->updateTimer1)->SetCurrent(0x3c);
                reinterpret_cast<AnmVmTimerView *>(&player->highlightTimer)->SetCurrent(200);
                g_PlayerUpdateGate28->StartPowerTransition();
                g_PlayerPower -= 20;
                RebuildPlayerOptions(player);
                int power = g_PlayerPower;
                g_ItemPowerDisplayOwner->UpdatePowerDisplay(
                    power / 20, ((power % 20) * 100) / 20);
                player->runtimeState = 1;
            }
            break;
        }
        // Fall through after the transition, matching the target state-4 block.

    case 2:
        if (player->updateTimer1.current == 3)
        {
            g_PlayerPower -= 64;
            if (g_PlayerPower < 0)
                g_PlayerPower = 0;
            int power = g_PlayerPower;
            g_ItemPowerDisplayOwner->UpdatePowerDisplay(
                power / 20, ((power % 20) * 100) / 20);

            PlayerFloat3 target;
            target.x = 0.0f;
            target.y = player->drawPosition.y - 224.0f;
            target.z = 0.0f;
            float angle = PlayerAngleToPoint(player, &target);
            for (i = 0; i < 7; ++i)
            {
                int kind = (i == 1 || i == 3 || i == 5) ? 4 : 1;
                g_ItemManager->Spawn(
                    reinterpret_cast<const AnmFloat3View *>(&player->drawPosition),
                    kind, 0x00ffffff,
                    (float)i * 0.11219973862171173f + angle -
                        0.39269909262657166f,
                    3.0f);
            }
            RebuildPlayerOptions(player);
        }

        if (player->updateTimer1.current >= 30)
        {
            if (g_PlayerRuntimeGateValue < 0)
            {
                if (g_PlayerUpdateGate10->value10 == 1)
                    g_MainSupervisorView.SetNextGameMode(4);
                else
                    PlayerFallbackRuntimeTransition();
            }
            else
            {
                player->runtimeState = 0;
                g_AnmGameSpeed = 1.0f;
                player->SpawnEffectRow(
                    &player->drawPosition, 32.0f, 16.0f, 30, 150);
                player->savedDrawPosition = player->drawPosition;
                player->positionX = 0;
                player->positionY = 48000;
                player->drawPosition.x = 0.0f;
                player->drawPosition.y = 480.0f;
                reinterpret_cast<AnmVmTimerView *>(&player->highlightTimer)->SetCurrent(0x118);
                reinterpret_cast<AnmVmTimerView *>(&player->updateTimer1)->SetCurrent(0);
            }
        }
        break;

    case 3:
        if (player->updateTimer1.current != 4 &&
            player->updateTimer1.current == 15)
        {
            PlayerHostileRowView *row =
                reinterpret_cast<PlayerHostileManagerView *>(
                    g_EnemyPrimaryResourceOwner)->rows;
            for (i = 0; i < 2000; ++i, ++row)
            {
                if (row->state446 != 0 && row->state446 != 3)
                    EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(row));
            }
            EnemyApplyBulletClear(
                reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager), 1);
        }
        break;
    }

    PlayerEffectRowView *row = player->effectRows;
    for (i = 0; i < 32; ++i, ++row)
    {
        PlayerEffectMotionView &motion = row->motion;
        if ((motion.activeFlags & 1) == 0)
            continue;

        if ((motion.motionFlags & 1) == 0)
        {
            reinterpret_cast<EnemyRuntimeVectorView *>(
                &motion.velocity)->FromAngleMagnitude(
                    motion.angularStep, motion.angle);
            motion.velocity.z = 0.0f;
        }
        else
        {
            motion.timerStep += motion.timerStepDelta;
            motion.angularStep =
                EnemyWrapAngle(motion.angle + motion.angularStep);
        }

        EnemyAdvanceMotion(reinterpret_cast<EnemyMotionView *>(&row->position));
        row->radius += row->radiusDelta;
        row->angle += row->angleDelta;

        motion.timer.Subtract(1.0f);
        if (motion.timer.current < 1)
            motion.activeFlags &= ~1u;
    }

    if (player->highlightTimer.current > 0)
    {
        player->highlightTimer.Subtract(1.0f);

        if (player->updateTimer1.current != player->updateTimer1.previous &&
            player->updateTimer1.current % 3 == 0)
        {
            player->drawVm.color = 0xff0000ffu;
            player->drawVm.flags |= 0x8000u;
        }
        else
        {
            player->drawVm.flags &= ~0x8000u;
        }
    }
    else
    {
        player->drawVm.flags &= ~0x8000u;
    }

    AnmRenderManagerView::ExecuteScript(
        reinterpret_cast<AnmVmView *>(&player->drawVm));

    player->derivedMin0 = player->drawPosition - player->extent0;
    player->derivedMax0 = player->drawPosition + player->extent0;
    player->derivedVectors[0] = player->drawPosition - player->extent1 * 0.5f;
    player->derivedVectors[1] = player->drawPosition + player->extent1 * 0.5f;
    player->derivedVectors[2] = player->drawPosition - player->extent2;
    player->derivedVectors[3] = player->drawPosition + player->extent2;
    player->derivedVectors[4] = player->drawPosition - player->extent1;
    player->derivedVectors[5] = player->drawPosition + player->extent1;

    player->updateTimer1.Tick();
    player->updateTimer2.Tick();

    if (g_PlayerDrawGate9EB8->value9EB8 == 0 &&
        g_PlayerDrawGate10 != NULL &&
        g_PlayerDrawGate10->value60 != 0 &&
        player->updateTimer1.current % 60 == 0)
    {
        g_GameScoreState.AddRank(1);
    }

    if (g_PlayerDrawGate9EB8->value9EB8 == 0 &&
        g_PlayerDrawGate10 != NULL &&
        g_PlayerDrawGate10->value60 != 0)
    {
        PlayerUpdateInputAction(player);
    }
    else
    {
        if ((player->updateTimer0.flags & 1) == 0)
        {
            player->updateTimer0.current = 0;
            player->updateTimer0.previous = 0xfff0bdc1;
            player->updateTimer0.subframe = 0.0f;
            player->updateTimer0.scale = &g_AnmGameSpeed;
            player->updateTimer0.flags |= 1;
        }
        player->updateTimer0.current = -1;
        player->updateTimer0.subframe = -1.0f;
        player->updateTimer0.previous = -2;
        player->trackedEnemy = NULL;
        player->trackedEnemyValid = 0;
    }

    PlayerUpdateShots(player);
    return 1;
}

// Keep the registered update adapter beside its Player callback body and its
// paired draw-chain entry.
int __fastcall PlayerUpdateCallback(Player *player)
{
    return PlayerUpdateCallbackBody(player);
}


// Maintained spelling of the Player callback registered in the target's second
// (draw-phase) chain. The physical target entry at 0x00426510 adapts ECX to an
// EAX-bound body at 0x00426360; source-written thunk versus compiler/LTCG
// adapter ownership remains unknown.
int __fastcall PlayerDrawCallback(Player *player)
{
    if (player->runtimeState != 2)
    {
        player->drawVm.positionX = player->drawPosition.x + 224.0f;
        player->drawVm.positionY = player->drawPosition.y + 16.0f;
        player->drawVm.positionZ = player->drawPosition.z;
        PlayerDrawManagedVm(&player->drawVm);

        for (int i = 0; i < 4; ++i)
        {
            PlayerOptionRuntime &option = player->options[i];
            if (option.drawCallback != NULL)
                option.drawCallback(&option);
        }

        if (g_PlayerDrawGate54 != NULL &&
            g_PlayerDrawGate54->value54 < 0 &&
            g_PlayerDrawGate10 != NULL &&
            g_PlayerDrawGate9EB8 != NULL &&
            g_PlayerDrawGate10->value10 == 0 &&
            g_PlayerDrawGate9EB8->value9EB8 == 0 &&
            g_PlayerDrawGate4->value4 == 0 &&
            g_PlayerAuxiliaryDrawSpan != 0)
        {
            float bounds[4];
            bounds[0] = static_cast<float>(PlayerFloor(
                player->drawPosition.x + 225.0f - 16.0f));
            bounds[2] = bounds[0] +
                (float)g_PlayerAuxiliaryDrawSpan * 0.29230770468711853f;
            bounds[1] = static_cast<float>(PlayerFloor(
                player->drawPosition.y + 17.0f - 24.0f));
            bounds[3] = bounds[1] + 2.0f;

            PlayerDrawAuxiliaryRectangle(bounds, 0x80000000u);
            bounds[0] -= 1.0f;
            bounds[1] -= 1.0f;
            bounds[2] -= 1.0f;
            bounds[3] -= 1.0f;
            PlayerDrawAuxiliaryRectangle(bounds, 0xffffffffu);
        }
    }
    return 1;
}

void __stdcall RebuildPlayerOptions(Player *player)
{
    int i;
    const short power = g_PlayerPower;
    int optionCount = (int)power / 20;

    if (power >= 100)
    {
        for (i = 0; i < 4; ++i)
        {
            PlayerOptionRuntime &option = player->options[i];
            MarkPlayerVmFlag(option.secondaryVmId);
            option.secondaryVmId = 0;

            switch (g_PlayerShotType + g_PlayerCharacter * 3)
            {
            case 0:
                option.secondaryVmId =
                    static_cast<AnmLoadedView *>(player->resource)->
                        CreateVmVariant1(
                            PLAYER_OPTION_SECONDARY_SCRIPT_0, PLAYER_VM_LAYER).value;
                break;
            case 1:
                option.secondaryVmId =
                    static_cast<AnmLoadedView *>(player->resource)->
                        CreateVmVariant1(
                            PLAYER_OPTION_SECONDARY_SCRIPT_1, PLAYER_VM_LAYER).value;
                break;
            case 2:
                option.secondaryVmId =
                    static_cast<AnmLoadedView *>(player->resource)->
                        CreateVmVariant1(
                            PLAYER_OPTION_SECONDARY_SCRIPT_2, PLAYER_VM_LAYER).value;
                break;
            case 3:
                option.secondaryVmId =
                    static_cast<AnmLoadedView *>(player->resource)->
                        CreateVmVariant1(
                            PLAYER_OPTION_SECONDARY_SCRIPT_0, PLAYER_VM_LAYER).value;
                break;
            case 4:
                option.secondaryVmId =
                    static_cast<AnmLoadedView *>(player->resource)->
                        CreateVmVariant1(
                            PLAYER_OPTION_SECONDARY_SCRIPT_1, PLAYER_VM_LAYER).value;
                break;
            case 5:
                option.secondaryVmId =
                    static_cast<AnmLoadedView *>(player->resource)->
                        CreateVmVariant1(
                            PLAYER_OPTION_SECONDARY_SCRIPT_2, PLAYER_VM_LAYER).value;
                break;
            }
        }
    }
    else
    {
        for (i = 0; i < 4; ++i)
            MarkPlayerVmDeleteState(player->options[i].secondaryVmId, 1);
    }

    if (optionCount > 4)
        optionCount = 4;
    if (player->optionCount == optionCount)
        return;

    for (i = 0; i < optionCount; ++i)
    {
        PlayerOptionRuntime &option = player->options[i];
        option.replayPair1.x = player->positionX;
        option.replayPair1.y = player->positionY;

        MarkPlayerVmFlag(option.primaryVmId);
        option.primaryVmId = 0;
        option.optionIndex = i;

        if (g_PlayerCharacter == 0)
        {
            const PlayerOptionPosition *position0 =
                GetPlayerOptionPosition(player, 0x20, optionCount, i);
            const PlayerOptionPosition *position1 =
                GetPlayerOptionPosition(player, 0x98, optionCount, i);
            LoadPlayerOptionPair(&option.replayPair2, position0);
            LoadPlayerOptionPair(&option.replayPair3, position1);

            const PlayerPositionPair *spawnOffset =
                player->optionMode != 0 ? &option.replayPair3 : &option.replayPair2;
            option.replayPair0.x = player->positionX + spawnOffset->x;
            option.replayPair0.y = player->positionY + spawnOffset->y;
            option.replayPair1 = option.replayPair0;

            switch (g_PlayerShotType)
            {
            case 0:
            {
                AnmLoadedView *loaded =
                    static_cast<AnmLoadedView *>(player->resource);
                AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
                vm->renderLayer = PLAYER_VM_LAYER;
                vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
                loaded->InitializeVm(vm, PLAYER_OPTION_PRIMARY_SCRIPT_0);
                AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
                option.primaryVmId = id.value;
            }
                break;
            case 1:
            {
                AnmLoadedView *loaded =
                    static_cast<AnmLoadedView *>(player->resource);
                AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
                vm->renderLayer = PLAYER_VM_LAYER;
                vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
                loaded->InitializeVm(vm, PLAYER_OPTION_PRIMARY_SCRIPT_1);
                AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
                option.primaryVmId = id.value;
            }
                break;
            case 2:
            {
                AnmLoadedView *loaded =
                    static_cast<AnmLoadedView *>(player->resource);
                AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
                vm->renderLayer = PLAYER_VM_LAYER;
                vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
                loaded->InitializeVm(vm, PLAYER_OPTION_PRIMARY_SCRIPT_2);
                AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
                option.primaryVmId = id.value;
            }
                break;
            }
        }
        else if (g_PlayerCharacter == 1)
        {
            switch (g_PlayerShotType)
            {
            case 0:
            {
                const PlayerOptionPosition *position1 =
                    GetPlayerOptionPosition(player, 0x98, optionCount, i);
                if (player->optionMode == 0)
                {
                    const PlayerOptionPosition *position0 =
                        GetPlayerOptionPosition(player, 0x20, optionCount, i);
                    LoadPlayerOptionPair(&option.replayPair2, position0);
                }
                LoadPlayerOptionPair(&option.replayPair3, position1);

                option.replayPair0 = g_Player->replayPositionHistory[(i + 1) * 8];
                if (player->optionMode != 0 && option.state == 0)
                {
                    if (i == 0)
                        LoadPlayerOptionPair(&option.replayPair2, position1);
                    else
                        option.replayPair2 = player->options[i - 1].replayPair2;
                }

                option.updateCallback = PlayerOptionTrailCallback;
                {
                    AnmLoadedView *loaded =
                        static_cast<AnmLoadedView *>(player->resource);
                    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
                    vm->renderLayer = PLAYER_VM_LAYER;
                    vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
                    loaded->InitializeVm(vm, PLAYER_OPTION_PRIMARY_SCRIPT_0);
                    AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
                    option.primaryVmId = id.value;
                }
                option.replayPair1 = player->replayPositionHistory[i * 8];
                break;
            }
            case 1:
            {
                const PlayerOptionPosition *position0 =
                    GetPlayerOptionPosition(player, 0x20, optionCount, i);
                const PlayerOptionPosition *position1 =
                    GetPlayerOptionPosition(player, 0x98, optionCount, i);
                LoadPlayerOptionPair(&option.replayPair2, position0);
                LoadPlayerOptionPair(&option.replayPair3, position1);

                const PlayerPositionPair *spawnOffset =
                    player->optionMode != 0 ? &option.replayPair3 : &option.replayPair2;
                option.replayPair0.x = player->positionX + spawnOffset->x;
                option.replayPair0.y = player->positionY + spawnOffset->y;
                {
                    AnmLoadedView *loaded =
                        static_cast<AnmLoadedView *>(player->resource);
                    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
                    vm->renderLayer = PLAYER_VM_LAYER;
                    vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
                    loaded->InitializeVm(vm, PLAYER_OPTION_PRIMARY_SCRIPT_1);
                    AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
                    option.primaryVmId = id.value;
                }
                break;
            }
            case 2:
            {
                const PlayerOptionPosition *position0 =
                    GetPlayerOptionPosition(player, 0x20, optionCount, i);
                LoadPlayerOptionPair(&option.replayPair2, position0);
                if (player->optionMode == 0 || option.state == 0)
                {
                    option.replayPair0.x = player->positionX + option.replayPair2.x;
                    option.replayPair0.y = player->positionY + option.replayPair2.y;
                    option.replayPair3 = option.replayPair0;
                }

                {
                    AnmLoadedView *loaded =
                        static_cast<AnmLoadedView *>(player->resource);
                    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
                    vm->renderLayer = PLAYER_VM_LAYER;
                    vm->flags35C |= PLAYER_VM_RUNTIME_FLAG;
                    loaded->InitializeVm(vm, PLAYER_OPTION_PRIMARY_SCRIPT_2);
                    AnmVmIdView id = g_AnmRenderManagerView->AddVmVariant0(vm);
                    option.primaryVmId = id.value;
                }
                if (player->optionMode != 0)
                    reinterpret_cast<AnmVmIdView *>(
                        &option.primaryVmId)->SetInterrupt(3);
                option.updateCallback = PlayerOptionSpecialCallback;
                break;
            }
            }
        }

        option.state = PLAYER_OPTION_ACTIVE;
    }

    for (; i < 4; ++i)
    {
        PlayerOptionRuntime &option = player->options[i];
        option.state = 0;
        MarkPlayerVmDeleteState(option.primaryVmId, 1);
    }

    player->optionCount = optionCount;
    for (i = 0; i < 4; ++i)
        player->options[i].resetFlag = 1;
}

// Target 0x004285F0 computes XY bounds while preserving possible input/output
// overlap. The full vector's Z component is outside this planar operation.
void PlayerFloat3::GetPlanarBounds(
    PlayerFloat3 *minimum, PlayerFloat3 *maximum, const float *extent) const
{
    minimum->x = x - extent[0] * 0.5f;
    minimum->y = y - extent[1] * 0.5f;
    maximum->x = x + extent[0] * 0.5f;
    maximum->y = y + extent[1] * 0.5f;
}

void PlayerTimerView::Subtract(float value)
{
    previous = current;
    if (*scale > 0.99f && *scale < 1.01f)
        subframe -= value;
    else
        subframe -= value * *scale;
    current = static_cast<int>(subframe);
}

// Target405500: invalidate the timed spell bonus, retaining front VM slot two.
void EnemyGameStateView::InvalidateSpellBonus()
{
    if (spellTimer.current < 60)
        return;
    currentSpellBonus = 0;
    flags378C &= ~2u;
    for (int slot = 0; slot < 8; ++slot)
    {
        if (slot != 2)
            frontVms[slot].flags35C &= ~2u;
    }
}

// Target405860: begin the input-triggered power transition and its effect VM.
int PlayerUpdateGate28::StartPowerTransition()
{
    if (value28 != 0)
        return -1;
    value28 = 1;
    timer.SetCurrent(0);
    radius = 32.0f;
    value40 = 4.0f;
    position = g_Player->drawPosition;

    EnemyGameStateView *state = g_EnemyGameState;
    int script;
    if (((state->flags378C & 1u) == 0 ||
            state->spellId < 93 || state->spellId > 96) &&
        state->spellId != 109)
        script = g_PlayerCharacter != 0 ? 407 : 400;
    else
        script = 441;
    vmId = g_EnemyPrimaryResourceOwner->primaryEnemyResource->CreateVmAtWorldVariant0(
        script, reinterpret_cast<const AnmFloat3View *>(&position));
    reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundCue(
        38, position.x);
    int power = g_PlayerPower;
    g_ItemPowerDisplayOwner->UpdatePowerDisplay(
        power / 20, ((power % 20) * 100) / 20);
    EnemyGameStateView *panel = g_EnemyGameState;
    panel->InvalidateSpellBonus();
    g_GameScoreState.AddRank(-128);
    if ((panel->flags378C & 1u) != 0 && panel->spellTimer.current >= 60)
        mode = 1;
    else
        mode = 0;
    return 0;
}
