#include "Player.hpp"

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct PlayerVm;

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
    unsigned char unknown000[0x28];
    int value28;
};

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

struct PlayerLinkedObjectView
{
    void *vtable;
    unsigned int unknown004;
    PlayerLinkedObjectView *next;
    int value0C;
};

struct PlayerLinkedListOwnerView
{
    unsigned char unknown000[0x18];
    PlayerLinkedObjectView *head;
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
extern PlayerHostileManagerView *g_PlayerHostileManager;
extern PlayerLinkedListOwnerView *g_PlayerLinkedListOwner;
extern PlayerUpdateGate10 *g_PlayerUpdateGate10;

// Descriptive interfaces for target-observed draw callees. The target machine
// boundaries use private register conventions; these declarations are
// maintained source spellings, not original ABI claims.
void PlayerDrawManagedVm(PlayerDrawVmView *vm);
void PlayerDrawAuxiliaryRectangle(const float *bounds, unsigned int color);
void PlayerSetMainVmMovementScript(Player *player, int scriptIndex);
void PlayerSetManagedVmPosition(unsigned int vmId, const PlayerFloat3 *position);
void PlayerSetManagedVmScreenPosition(
    unsigned int vmId, const PlayerFloat3 *position);

// Descriptive interfaces for target-observed Player update callees. Their
// original identifiers and private register conventions remain unproven.
void PlayerPlaySound(int soundId);
void PlayerNotifyPowerTransition(PlayerUpdateGate28 *gate);
void PlayerUpdatePowerDisplay(int value, int valueAgain, int percent);
void PlayerStartPowerDisplayTimer(int frames);
void PlayerEmitStateTransitionEffect(float value, int mode, int variant);
void PlayerCommitStateTransitionEffect(float value, int mode);
void PlayerFinalizeState3Transition();
void PlayerInvokeHostileCleanup(PlayerHostileRowView *row);
void PlayerInvokeLinkedObjectCallback(PlayerLinkedObjectView *object);
void PlayerTransitionToState2(Player *player);
float PlayerAngleToPoint(Player *player, const PlayerFloat3 *point);
void PlayerSpawnDirectionalEffect(
    const PlayerFloat3 *position, int kind, unsigned int color,
    float angle, float scale);
void PlayerRequestRuntimeTransition(int value);
void PlayerFallbackRuntimeTransition();
PlayerEffectRowView *PlayerSpawnEffectRow(
    Player *player, const PlayerFloat3 *position,
    float radius, float radiusDelta, int frames, int damage);
void PlayerSetEffectVelocityFromPolar(
    PlayerFloat3 *velocity, float angularStep, float angle);
float PlayerWrapEffectAngle(float value);
void PlayerAdvanceEffectRegion(PlayerFloat3 *position);
void PlayerUpdateDrawVmState(PlayerDrawVmView *vm);
void PlayerUpdateInputAction(Player *player);
int PlayerUpdateShots(Player *player);

// Descriptive interfaces for the target-observed shot-motion helpers. Their
// physical callees use private register contracts; these ordinary declarations
// are maintained source spellings, not original ABI claims.
float PlayerWrapShotAngle(float value);
void PlayerSetShotVelocityFromPolar(
    PlayerFloat3 *velocity, float angle, float magnitude);
void PlayerAdvanceShotMotion(PlayerShotMotionView *motion);
void PlayerSetShotVmDeleteState1(unsigned int *vmId);
void PlayerSetShotVmDeleteState2(unsigned int *vmId);
void PlayerSetShotVmDeleteState3(unsigned int *vmId);
PlayerVm *PlayerResolveShotVmAndClearMissing(unsigned int *vmId);
void PlayerMarkShotVmPendingAndClear(unsigned int *vmId);
void PlayerMarkShotVmPending(unsigned int vmId);
int PlayerTimerHasIntervalTick(const PlayerTimerView *timer, int interval);
int PlayerGetAuxiliaryDamageAtPosition(const PlayerFloat3 *position);

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
PlayerVm *PlayerCreateManagedVm(void *resource, int scriptIndex, int layer);
PlayerVm *PlayerAllocateManagedVm();
void PlayerInitializeManagedVmScript(PlayerVm *vm, int scriptIndex);
void PlayerRegisterManagedVm(PlayerVm *vm, unsigned int *idOut);
void PlayerSetManagedVmDeleteState(unsigned int *vmId, unsigned short state);

// Neutral lifecycle globals and interfaces recovered around Player creation,
// reset and teardown. Private register contracts are deliberately hidden here.
extern unsigned int g_PlayerLifecycleFlags;
struct GuiView;
extern GuiView *g_GuiView;
extern int g_PlayerLivesDisplayCount;
extern unsigned char g_PlayerCallbackLockDepth;
struct PlayerCriticalSectionView { unsigned char storage[0x18]; };
extern PlayerCriticalSectionView g_PlayerCallbackCriticalSection;
extern "C" void __stdcall EnterCriticalSection(PlayerCriticalSectionView *section);
extern "C" void __stdcall LeaveCriticalSection(PlayerCriticalSectionView *section);
void __fastcall PlayerUnlinkCallbackNode(
    PlayerCallbackNodeView *node, void *manager);
void PlayerMarkManagedVmPending(unsigned int vmId);
void GuiSetLivesDisplayCount(GuiView *gui, int count);
void PlayerMarkResourceVmsPending(void *resource);
void PlayerDestroyAnimationCacheContents(void *cache);

// Descriptive interfaces for the Player initializer's observed callees. Only
// PlayerLoadAnimationResource has a conventional machine spelling supported by
// the target (ECX, EDX, then one stack argument). The others hide private
// register contracts and are not original ABI claims.
void * __fastcall PlayerLoadAnimationResource(
    int loadFlags, void *owner, const char *filename);
void PlayerReportInitializationError();
int PlayerLoadOptionData(Player *player, const char *filename);
PlayerCallbackNodeView * __stdcall PlayerAllocateCallbackNode(
    PlayerChainCallback callback);
void PlayerRegisterUpdateCallbackNode(
    PlayerCallbackNodeView *node, int priority, void *manager);
void PlayerRegisterDrawCallbackNode(
    PlayerCallbackNodeView *node, int priority, void *manager);
void PlayerInitializeEmbeddedDrawVm(
    PlayerDrawVmView *vm, void *resource, int scriptIndex);

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
    if (id == 0 || g_PlayerVmManager == NULL)
        return NULL;

    PlayerVmListNode *node;
    for (node = g_PlayerVmManager->primaryHead; node != NULL; node = node->next)
    {
        if (node->value != NULL && node->value->id == id)
            return node->value;
    }
    for (node = g_PlayerVmManager->secondaryHead; node != NULL; node = node->next)
    {
        if (node->value != NULL && node->value->id == id)
            return node->value;
    }
    return NULL;
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
            if (node->value != NULL)
                node->value->flags |= PLAYER_VM_PENDING_FLAG;
        }
    }
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
            if (node->value != NULL)
                node->value->deleteState = state;
        }
    }
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

static unsigned int CreatePrimaryPlayerOptionVm(Player *player, int scriptIndex)
{
    PlayerVm *vm = PlayerAllocateManagedVm();
    unsigned int id = 0;

    if (vm == NULL)
        return 0;

    vm->layer = PLAYER_VM_LAYER;
    vm->flags |= PLAYER_VM_RUNTIME_FLAG;
    PlayerInitializeManagedVmScript(vm, scriptIndex);
    PlayerRegisterManagedVm(vm, &id);
    return id;
}

static int GetPrimaryPlayerOptionScript()
{
    switch (g_PlayerShotType)
    {
    case 0:
        return PLAYER_OPTION_PRIMARY_SCRIPT_0;
    case 1:
        return PLAYER_OPTION_PRIMARY_SCRIPT_1;
    case 2:
        return PLAYER_OPTION_PRIMARY_SCRIPT_2;
    default:
        return -1;
    }
}

static int GetSecondaryPlayerOptionScript()
{
    switch (g_PlayerShotType + g_PlayerCharacter * 3)
    {
    case 0:
    case 3:
        return PLAYER_OPTION_SECONDARY_SCRIPT_0;
    case 1:
    case 4:
        return PLAYER_OPTION_SECONDARY_SCRIPT_1;
    case 2:
    case 5:
        return PLAYER_OPTION_SECONDARY_SCRIPT_2;
    default:
        return -1;
    }
}

// Maintained spelling of the observed ECX-bound option callback boundary. This
// models the target machine ABI; the original source declaration and the
// wrapper/body optimizer ownership remain unknown.
static int __fastcall PlayerOptionTrailCallback(PlayerOptionRuntime *option)
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
    return 0;
}

// Maintained spelling of the second observed ECX-bound option callback. The
// target switches the primary VM delete state only when the option mode changes.
static int __fastcall PlayerOptionSpecialCallback(PlayerOptionRuntime *option)
{
    Player *player = g_Player;
    const int optionMode = player->optionMode;

    if (optionMode == 0)
    {
        if (option->previousMode != 0)
            PlayerSetManagedVmDeleteState(&option->primaryVmId, 6);

        option->replayPair3 = option->replayPair1;
        option->previousMode = 0;
    }
    else
    {
        if (option->previousMode == 0)
            PlayerSetManagedVmDeleteState(&option->primaryVmId, 3);

        option->replayPair0 = option->replayPair3;
        option->previousMode = optionMode;
    }
    return 0;
}

// Maintained source for the Player constructor/reset/factory/destructor seam.
// The target optimizer uses private ESI/stack/register boundaries for several
// bodies; these natural C++ declarations do not claim those machine ABIs.
Player::Player()
{
    memset(this, 0, sizeof(*this));
    g_Player = this;
}

void PlayerResetRuntimeState(Player *player)
{
    player->runtimeState = 1;

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

    if ((player->updateTimer2.flags & 1) == 0)
    {
        player->updateTimer2.current = 0;
        player->updateTimer2.previous = 0xfff0bdc1;
        player->updateTimer2.subframe = 0.0f;
        player->updateTimer2.scale = &g_PlayerTimerScale;
        player->updateTimer2.flags |= 1;
    }
    player->updateTimer2.current = 0;
    player->updateTimer2.subframe = 0.0f;
    player->updateTimer2.previous = -1;

    PlayerMarkManagedVmPending(player->modeVmId);
    player->modeVmId = 0;
    GuiSetLivesDisplayCount(g_GuiView, g_PlayerLivesDisplayCount);
}

static void RemovePlayerCallbackNode(PlayerCallbackNodeView *node)
{
    if (node == NULL)
        return;

    EnterCriticalSection(&g_PlayerCallbackCriticalSection);
    ++g_PlayerCallbackLockDepth;
    PlayerUnlinkCallbackNode(node, g_PlayerCallbackManager);
    LeaveCriticalSection(&g_PlayerCallbackCriticalSection);
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

    if (drawVm.ownedData358 != NULL)
        free(drawVm.ownedData358);
    drawVm.ownedData358 = NULL;
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

// Maintained source for the Player initialization owner at
// 0x004247F0-0x00424D8A. Its sole target caller places the newly allocated
// Player in EBX immediately before the call. This ordinary source parameter is
// a maintainable spelling of that private machine boundary.
int PlayerInitialize(Player *player)
{
    const char *animationFilename =
        g_PlayerCharacter != 0 ? "pl01.anm" : "pl00.anm";
    player->resource = PlayerLoadAnimationResource(
        8, g_PlayerVmManager, animationFilename);
    if (player->resource == NULL)
    {
        PlayerReportInitializationError();
        return -1;
    }

    if (g_PlayerSharedOptionData != NULL)
    {
        player->optionData = g_PlayerSharedOptionData;
        g_PlayerSharedOptionData = NULL;
    }
    else
    {
        const int dataIndex = g_PlayerShotType + g_PlayerCharacter * 3;
        if (PlayerLoadOptionData(
                player, g_PlayerShotDataFilenames[dataIndex]) != 0)
        {
            PlayerReportInitializationError();
            return -1;
        }
    }

    PlayerCallbackNodeView *updateNode =
        PlayerAllocateCallbackNode(PlayerUpdateCallback);
    updateNode->owner = player;
    updateNode->flags &= ~2u;
    PlayerRegisterUpdateCallbackNode(updateNode, 0x10, g_PlayerCallbackManager);
    player->updateCallbackNode = updateNode;

    PlayerCallbackNodeView *drawNode =
        PlayerAllocateCallbackNode(PlayerDrawCallback);
    drawNode->flags &= ~2u;
    drawNode->owner = player;
    PlayerRegisterDrawCallbackNode(drawNode, 0x16, g_PlayerCallbackManager);
    player->drawCallbackNode = drawNode;

    PlayerInitializeEmbeddedDrawVm(&player->drawVm, player->resource, 0);

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
        player->replayPositionHistory[i].x = player->positionX;
        player->replayPositionHistory[i].y = player->positionY;
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
    player->updateTimer0.current = -1;

    data->value04 = g_PlayerOptionValue04ByCharacter[g_PlayerCharacter];
    data->value0C = g_PlayerOptionValue0CByCharacter[g_PlayerCharacter];
    data->value08 = g_PlayerOptionValue08ByCharacter[g_PlayerCharacter];

    player->extent0.x = data->value04 * 0.5f;
    player->extent0.y = player->extent0.x;
    player->extent0.z = 5.0f;
    player->extent1.x =
        g_PlayerOptionValue0CByCharacter[g_PlayerCharacter] * 0.5f;
    player->extent1.y = player->extent1.x;
    player->extent1.z = 5.0f;
    player->extent2.x =
        g_PlayerExtent2ByCharacter[g_PlayerCharacter] * 0.5f;
    player->extent2.y = player->extent2.x;
    player->extent2.z = 5.0f;

    player->derivedMin0.x = player->drawPosition.x - player->extent0.x;
    player->derivedMin0.y = player->drawPosition.y - player->extent0.y;
    player->derivedMin0.z = player->drawPosition.z - player->extent0.z;
    player->derivedMax0.x = player->drawPosition.x + player->extent0.x;
    player->derivedMax0.y = player->drawPosition.y + player->extent0.y;
    player->derivedMax0.z = player->drawPosition.z + player->extent0.z;

    player->derivedVectors[0].x = player->drawPosition.x - player->extent1.x;
    player->derivedVectors[0].y = player->drawPosition.y - player->extent1.y;
    player->derivedVectors[0].z = player->drawPosition.z - player->extent1.z;
    player->derivedVectors[1].x = player->drawPosition.x + player->extent1.x;
    player->derivedVectors[1].y = player->drawPosition.y + player->extent1.y;
    player->derivedVectors[1].z = player->drawPosition.z + player->extent1.z;
    player->derivedVectors[2].x = player->drawPosition.x - player->extent2.x;
    player->derivedVectors[2].y = player->drawPosition.y - player->extent2.y;
    player->derivedVectors[2].z = player->drawPosition.z - player->extent2.z;
    player->derivedVectors[3].x = player->drawPosition.x + player->extent2.x;
    player->derivedVectors[3].y = player->drawPosition.y + player->extent2.y;
    player->derivedVectors[3].z = player->drawPosition.z + player->extent2.z;
    player->derivedVectors[4].x = player->drawPosition.x - player->extent2.x;
    player->derivedVectors[4].y = player->drawPosition.y - player->extent2.y;
    player->derivedVectors[4].z = player->drawPosition.z - player->extent2.z;
    player->derivedVectors[5].x = player->drawPosition.x + player->extent2.x;
    player->derivedVectors[5].y = player->drawPosition.y + player->extent2.y;
    player->derivedVectors[5].z = player->drawPosition.z + player->extent2.z;

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
    player->optionTransitionFrames = 30;

    RebuildPlayerOptions(player);
    return 0;
}

// Maintained source for the Player movement/options owner at 0x004250B0.
// The target body receives Player in live-in EDI from its sole caller. This
// ordinary source parameter is a maintainable spelling of that behavior, not a
// claim about the original declaration or the optimizer-owned machine ABI.
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
            PlayerVm *vm = PlayerAllocateManagedVm();
            vm->layer = PLAYER_MODE_VM_LAYER;
            vm->flags |= PLAYER_VM_RUNTIME_FLAG;
            PlayerInitializeManagedVmScript(vm, PLAYER_MODE_VM_SCRIPT);
            PlayerRegisterManagedVm(vm, &player->modeVmId);
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
            PlayerSetManagedVmDeleteState(&player->modeVmId, 1);
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
        PlayerSetMainVmMovementScript(player, PLAYER_MAIN_VM_MOVE_LEFT);
    else if (horizontalSpeed > 0 && player->previousHorizontalSpeed <= 0)
        PlayerSetMainVmMovementScript(player, PLAYER_MAIN_VM_MOVE_RIGHT);
    else if (horizontalSpeed == 0 && player->previousHorizontalSpeed < 0)
        PlayerSetMainVmMovementScript(player, PLAYER_MAIN_VM_STOP_LEFT);
    else if (horizontalSpeed == 0 && player->previousHorizontalSpeed > 0)
        PlayerSetMainVmMovementScript(player, PLAYER_MAIN_VM_STOP_RIGHT);

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
        PlayerFloat3 position;
        position.x = player->drawPosition.x + 224.0f;
        position.y = player->drawPosition.y + 16.0f;
        position.z = player->drawPosition.z;
        PlayerSetManagedVmPosition(player->modeVmId, &position);
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

        PlayerFloat3 optionPosition;
        optionPosition.x = (float)option.replayPair1.x * 0.01f;
        optionPosition.y = (float)option.replayPair1.y * 0.01f;
        optionPosition.z = 0.0f;
        PlayerSetManagedVmScreenPosition(option.primaryVmId, &optionPosition);
        PlayerSetManagedVmScreenPosition(option.secondaryVmId, &optionPosition);
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

// Maintained source for the 128-row Player shot-update owner at
// 0x00428280-0x004285EB. The target receives one stack Player* and returns zero
// with RET 4. The 0x004282AA-0x004282AF gap is unreachable compiler alignment,
// not a separate data owner. Helper declarations above hide private machine
// ABIs rather than claiming original source conventions.
int PlayerUpdateShots(Player *player)
{
    int i;

    for (i = 0; i < 128; ++i)
    {
        PlayerShotRuntimeView *shot = &player->shots[i];
        if (shot->state == PLAYER_SHOT_INACTIVE)
            continue;

        const PlayerShotDescriptorView *descriptor = shot->descriptor;
        if (descriptor->type == PLAYER_SHOT_TYPE_3 &&
            shot->state == PLAYER_SHOT_ACTIVE &&
            (player->updateTimer0.current < 0 ||
             player->optionCount <= (int)descriptor->sourceIndex - 1))
        {
            PlayerSetShotVmDeleteState1(&shot->primaryVmId);
            PlayerSetShotVmDeleteState1(&shot->secondaryVmId);
            shot->state = PLAYER_SHOT_HIT_TRANSITION;
            player->shotSourceActive[(int)descriptor->sourceIndex - 1] = 0;
        }

        if (descriptor->type == PLAYER_SHOT_TYPE_3 &&
            shot->state == PLAYER_SHOT_ACTIVE &&
            ((g_PlayerDrawGate9EB8 != NULL &&
              g_PlayerDrawGate9EB8->value9EB8 != 0) ||
             g_PlayerDrawGate10 == NULL))
        {
            shot->state = PLAYER_SHOT_HIT_TRANSITION;
            PlayerSetShotVmDeleteState1(&shot->primaryVmId);
            PlayerSetShotVmDeleteState1(&shot->secondaryVmId);
            player->shotSourceActive[(int)descriptor->sourceIndex - 1] = 0;
        }

        if (descriptor->type == PLAYER_SHOT_TYPE_3 &&
            shot->collidedThisFrame == 0 &&
            shot->state == PLAYER_SHOT_ACTIVE &&
            shot->collisionVmTransitionPending == 1)
        {
            PlayerSetShotVmDeleteState3(&shot->primaryVmId);
            shot->collisionVmTransitionPending = 0;
        }
        shot->collidedThisFrame = 0;

        if (descriptor->updateCallback != NULL)
            descriptor->updateCallback(player, shot);

        if ((shot->motion.flags & PLAYER_SHOT_POLAR_MOTION) == 0)
        {
            PlayerSetShotVelocityFromPolar(
                &shot->motion.velocity,
                shot->motion.angle,
                shot->motion.speedOrAngleStep);
            shot->motion.velocity.z = 0.0f;
        }
        else
        {
            shot->motion.polarMagnitude += shot->motion.polarMagnitudeDelta;
            shot->motion.angle = PlayerWrapShotAngle(
                shot->motion.speedOrAngleStep + shot->motion.angle);
        }

        PlayerAdvanceShotMotion(&shot->motion);

        PlayerVm *primaryVm = FindPlayerVm(shot->primaryVmId);
        if (primaryVm == NULL)
        {
            shot->state = PLAYER_SHOT_INACTIVE;
            if (shot->secondaryVmId != 0)
                PlayerMarkShotVmPending(shot->secondaryVmId);
            shot->primaryVmId = 0;
            shot->secondaryVmId = 0;
            continue;
        }

        if (descriptor->type != PLAYER_SHOT_TYPE_3 &&
            shot->timer.current >= 10)
        {
            const float extentX =
                primaryVm->sprite->extent34 * primaryVm->scale3C;
            const float extentY =
                primaryVm->sprite->extent30 * primaryVm->scale40;
            if (reinterpret_cast<PlayerShotBoundsView *>(
                    &shot->motion.position)->IsOutsidePlayfield(
                    extentX, extentY) != 0)
            {
                PlayerMarkShotVmPending(shot->primaryVmId);
                shot->primaryVmId = 0;
                shot->state = PLAYER_SHOT_INACTIVE;
                continue;
            }
        }

        primaryVm->position.x = shot->motion.position.x + 224.0f;
        primaryVm->position.y = shot->motion.position.y + 16.0f;
        primaryVm->position.z = shot->motion.position.z;

        if (shot->secondaryVmId != 0)
        {
            PlayerVm *secondaryVm = FindPlayerVm(shot->secondaryVmId);
            if (secondaryVm == NULL)
            {
                shot->secondaryVmId = 0;
            }
            else
            {
                secondaryVm->position.x = shot->motion.position.x + 224.0f;
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
        const float timerScale = *shot->timer.scale;
        if (timerScale > 0.9900000095367432f &&
            timerScale < 1.0099999904632568f)
        {
            ++shot->timer.current;
            shot->timer.subframe += 1.0f;
        }
        else
        {
            shot->timer.subframe += timerScale;
            shot->timer.current = (int)shot->timer.subframe;
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
    int i;

    if (updateTimer1.current == updateTimer1.previous)
        return 0;

    const float targetHalfX = targetSize->x * 0.5f;
    const float targetHalfY = targetSize->y * 0.5f;
    const float targetMinX = targetPosition->x - targetHalfX;
    const float targetMinY = targetPosition->y - targetHalfY;
    const float targetMaxX = targetPosition->x + targetHalfX;
    const float targetMaxY = targetPosition->y + targetHalfY;

    if (optionalHitFlag != NULL)
        *optionalHitFlag = 0;

    for (i = 0; i < 128; ++i)
    {
        PlayerShotRuntimeView *shot = &shots[i];
        if (shot->state == PLAYER_SHOT_INACTIVE ||
            shot->state == PLAYER_SHOT_HIT_TRANSITION)
        {
            continue;
        }

        const PlayerShotDescriptorView *descriptor = shot->descriptor;
        const float shotHalfX = descriptor->hitboxExtentX * 0.5f;
        const float shotHalfY = descriptor->hitboxExtentY * 0.5f;
        const float shotMinX = shot->motion.position.x - shotHalfX;
        const float shotMinY = shot->motion.position.y - shotHalfY;
        const float shotMaxX = shot->motion.position.x + shotHalfX;
        const float shotMaxY = shot->motion.position.y + shotHalfY;

        if (shotMinY > targetMaxY || shotMinX > targetMaxX ||
            shotMaxY < targetMinY || shotMaxX < targetMinX)
        {
            continue;
        }

        if (descriptor->type != PLAYER_SHOT_TYPE_3 && shotMinY < 0.0f)
            continue;
        if (descriptor->type == PLAYER_SHOT_TYPE_3 && targetMaxY < 0.0f)
            continue;

        if (descriptor->collisionCallback != NULL &&
            descriptor->collisionCallback(this, shot, targetPosition) != 0)
        {
            continue;
        }

        if (shot->collisionVmTransitionPending == 0)
        {
            PlayerSetShotVmDeleteState2(&shot->primaryVmId);
            shot->collisionVmTransitionPending = 1;
        }
        shot->collidedThisFrame = 1;

        if (descriptor->type != PLAYER_SHOT_TYPE_3 ||
            PlayerTimerHasIntervalTick(&shot->timer, 4) != 0)
        {
            damage += descriptor->damage;
        }

        if (descriptor->type != PLAYER_SHOT_TYPE_3)
        {
            PlayerVm *vm =
                PlayerResolveShotVmAndClearMissing(&shot->primaryVmId);
            const float savedAngle = vm->angle2C;

            PlayerMarkShotVmPendingAndClear(&shot->primaryVmId);

            PlayerVm *createdVm = PlayerCreateManagedVm(
                resource, descriptor->hitAnimationScript + 5, PLAYER_VM_LAYER);
            shot->primaryVmId = createdVm->id;
            PlayerVm *hitVm =
                PlayerResolveShotVmAndClearMissing(&shot->primaryVmId);
            hitVm->angle2C = savedAngle;
            hitVm->flags |= PLAYER_VM_SHOT_ANGLE_DIRTY;

            shot->motion.position.z = 0.1f;
            shot->state = PLAYER_SHOT_HIT_TRANSITION;
            shot->motion.speedOrAngleStep *= 0.125f;
        }

        if (descriptor->type == 2)
        {
            PlayerSpawnEffectRow(
                this, &shot->motion.position,
                32.0f, 1.3999999761581421f, 13, descriptor->damage / 3);
        }
    }

    damage += PlayerGetAuxiliaryDamageAtPosition(targetPosition);

    for (i = 0; i < 32; ++i)
    {
        PlayerEffectRowView *row = &effectRows[i];
        PlayerEffectMotionView *motion = &row->motion;
        if ((motion->activeFlags & 1) == 0)
            continue;

        if (motion->timer.current != motion->timer.previous &&
            motion->timer.current % motion->collisionInterval == 0)
        {
            continue;
        }

        if ((motion->activeFlags & 2) != 0)
        {
            const float dx = row->position.x - targetPosition->x;
            const float dy = row->position.y - targetPosition->y;
            if (row->radius * row->radius < dx * dx + dy * dy)
                continue;
        }
        else if (row->angle == 0.0f)
        {
            const float halfExtentX = row->extentX * 0.5f;
            const float halfExtentY = row->extentY * 0.5f;
            if (row->position.x - halfExtentX > targetMaxX ||
                row->position.x + halfExtentX < targetMinX ||
                row->position.y - halfExtentY > targetMaxY ||
                row->position.y + halfExtentY < targetMinY)
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
            const float rotatedX = cosine * dx - sine * dy;
            const float rotatedY = cosine * dy + sine * dx;
            const float halfExtentX = row->extentX * 0.5f;
            const float halfExtentY = row->extentY * 0.5f;
            if (-halfExtentX > targetHalfX + rotatedX ||
                halfExtentX < rotatedX - targetHalfX ||
                -halfExtentY > targetHalfY + rotatedY ||
                halfExtentY < rotatedY - targetHalfY)
            {
                continue;
            }
        }

        damage += motion->damage;
        motion->hitAccumulator += motion->damage;
        if (motion->hitAccumulator >= motion->hitCap)
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

        int historyLimit = player->optionCount * 8;
        for (i = 32; i > historyLimit; --i)
            player->replayPositionHistory[i] =
                player->replayPositionHistory[historyLimit];
        for (i = historyLimit; i > 0; --i)
            player->replayPositionHistory[i] =
                player->replayPositionHistory[i - 1];
        player->replayPositionHistory[0].x = player->positionX;
        player->replayPositionHistory[0].y = player->positionY;

        if (player->updateTimer1.current < 30)
        {
            float value =
                (float)player->updateTimer1.current * 17.066667556762695f +
                64.0f;
            PlayerEmitStateTransitionEffect(value, 0, 1);
            PlayerEmitStateTransitionEffect(value * 0.25f, 0, 0);
            PlayerCommitStateTransitionEffect(value, 0);
        }
        else
        {
            for (i = 0; i < 2000; ++i)
            {
                PlayerHostileRowView &row = g_PlayerHostileManager->rows[i];
                if (row.state446 != 0 && row.state446 != 3 && row.value4 == 0)
                    PlayerInvokeHostileCleanup(&row);
            }

            PlayerLinkedObjectView *object = g_PlayerLinkedListOwner->head;
            while (object != NULL)
            {
                PlayerLinkedObjectView *next = object->next;
                if (object->value0C != 1)
                    PlayerInvokeLinkedObjectCallback(object);
                object = next;
            }
        }

        if (player->updateTimer1.current < 60)
            break;

        player->runtimeState = 1;
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
            PlayerPlaySound(0x10e);
            PlayerNotifyPowerTransition(g_PlayerUpdateGate28);
            g_PlayerPower -= 20;
            RebuildPlayerOptions(player);
            int power = g_PlayerPower;
            PlayerUpdatePowerDisplay(
                power / 20, power / 20, ((power % 20) * 100) / 20);
            PlayerStartPowerDisplayTimer(3000);
        }

        if (player->updateTimer1.current < 30)
        {
            for (i = 0; i < 2000; ++i)
            {
                PlayerHostileRowView &row = g_PlayerHostileManager->rows[i];
                if (row.state446 != 0 && row.state446 != 3 && row.value4 == 0)
                    PlayerInvokeHostileCleanup(&row);
            }

            PlayerLinkedObjectView *object = g_PlayerLinkedListOwner->head;
            while (object != NULL)
            {
                PlayerLinkedObjectView *next = object->next;
                if (object->value0C != 1)
                    PlayerInvokeLinkedObjectCallback(object);
                object = next;
            }
        }
        PlayerUpdateMovementAndOptions(player);
        break;

    case 4:
        if (player->updateTimer1.current < 8)
        {
            if (g_PlayerUpdateGate28 != NULL &&
                g_PlayerUpdateGate28->value28 == 0 &&
                g_PlayerPower / 20 != 0 &&
                (g_PlayerInputBits & 2) != 0)
            {
                PlayerPlaySound(0x3c);
                PlayerPlaySound(200);
                PlayerNotifyPowerTransition(g_PlayerUpdateGate28);
                g_PlayerPower -= 20;
                int power = g_PlayerPower;
                PlayerUpdatePowerDisplay(
                    power / 20, power / 20, ((power % 20) * 100) / 20);
                player->runtimeState = 1;
            }
            break;
        }
        PlayerTransitionToState2(player);
        // Fall through after the transition, matching the target state-4 block.

    case 2:
        if (player->updateTimer1.current == 3)
        {
            g_PlayerPower -= 64;
            if (g_PlayerPower < 0)
                g_PlayerPower = 0;
            int power = g_PlayerPower;
            PlayerUpdatePowerDisplay(
                power / 20, power / 20, ((power % 20) * 100) / 20);

            PlayerFloat3 target;
            target.x = 0.0f;
            target.y = player->drawPosition.y - 224.0f;
            target.z = 0.0f;
            float angle = PlayerAngleToPoint(player, &target);
            for (i = 0; i < 7; ++i)
            {
                int kind = (i == 1 || i == 3 || i == 5) ? 4 : 1;
                PlayerSpawnDirectionalEffect(
                    &player->drawPosition, kind, 0x00ffffff,
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
                    PlayerRequestRuntimeTransition(4);
                else
                    PlayerFallbackRuntimeTransition();
            }
            else
            {
                player->runtimeState = 0;
                g_PlayerTimerScale = 1.0f;
                PlayerSpawnEffectRow(
                    player, &player->drawPosition, 32.0f, 16.0f, 30, 150);
                player->savedDrawPosition = player->drawPosition;
                player->positionX = 0;
                player->positionY = 48000;
                player->drawPosition.x = 0.0f;
                player->drawPosition.y = 240.0f;
                PlayerPlaySound(0x118);
                PlayerPlaySound(0);
            }
        }
        break;

    case 3:
        if (player->updateTimer1.current != 4 &&
            player->updateTimer1.current == 15)
        {
            for (i = 0; i < 2000; ++i)
            {
                PlayerHostileRowView &row = g_PlayerHostileManager->rows[i];
                if (row.state446 != 0 && row.state446 != 3)
                    PlayerInvokeHostileCleanup(&row);
            }
            PlayerFinalizeState3Transition();
        }
        break;
    }

    for (i = 0; i < 32; ++i)
    {
        PlayerEffectRowView &row = player->effectRows[i];
        PlayerEffectMotionView &motion = row.motion;
        if ((motion.activeFlags & 1) == 0)
            continue;

        if ((motion.motionFlags & 1) == 0)
        {
            PlayerSetEffectVelocityFromPolar(
                &motion.velocity, motion.angularStep, motion.angle);
            motion.velocity.z = 0.0f;
        }
        else
        {
            motion.timerStep += motion.timerStepDelta;
            motion.angularStep =
                PlayerWrapEffectAngle(motion.angle + motion.angularStep);
        }

        PlayerAdvanceEffectRegion(&row.position);
        row.radius += row.radiusDelta;
        row.angle += row.angleDelta;

        motion.timer.previous = motion.timer.current;
        float scale = *motion.timer.scale;
        if (scale > 0.9900000095367432f && scale < 1.0099999904632568f)
            motion.timer.subframe -= 1.0f;
        else
            motion.timer.subframe -= scale * 1.0f;
        motion.timer.current = (int)motion.timer.subframe;
        if (motion.timer.current < 1)
            motion.activeFlags &= ~1u;
    }

    if (player->highlightTimer.current > 0)
    {
        player->highlightTimer.previous = player->highlightTimer.current;
        float scale = *player->highlightTimer.scale;
        if (scale > 0.9900000095367432f && scale < 1.0099999904632568f)
            player->highlightTimer.subframe -= 1.0f;
        else
            player->highlightTimer.subframe -= scale * 1.0f;
        player->highlightTimer.current = (int)player->highlightTimer.subframe;

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

    PlayerUpdateDrawVmState(&player->drawVm);

    player->derivedMin0.x = player->drawPosition.x - player->extent0.x;
    player->derivedMin0.y = player->drawPosition.y - player->extent0.y;
    player->derivedMin0.z = player->drawPosition.z - player->extent0.z;
    player->derivedMax0.x = player->drawPosition.x + player->extent0.x;
    player->derivedMax0.y = player->drawPosition.y + player->extent0.y;
    player->derivedMax0.z = player->drawPosition.z + player->extent0.z;

    player->derivedVectors[0].x = player->drawPosition.x - player->extent1.x * 0.5f;
    player->derivedVectors[0].y = player->drawPosition.y - player->extent1.y * 0.5f;
    player->derivedVectors[0].z = player->drawPosition.z - player->extent1.z * 0.5f;
    player->derivedVectors[1].x = player->drawPosition.x + player->extent1.x * 0.5f;
    player->derivedVectors[1].y = player->drawPosition.y + player->extent1.y * 0.5f;
    player->derivedVectors[1].z = player->drawPosition.z + player->extent1.z * 0.5f;
    player->derivedVectors[2].x = player->drawPosition.x - player->extent2.x;
    player->derivedVectors[2].y = player->drawPosition.y - player->extent2.y;
    player->derivedVectors[2].z = player->drawPosition.z - player->extent2.z;
    player->derivedVectors[3].x = player->drawPosition.x + player->extent2.x;
    player->derivedVectors[3].y = player->drawPosition.y + player->extent2.y;
    player->derivedVectors[3].z = player->drawPosition.z + player->extent2.z;
    player->derivedVectors[4].x = player->drawPosition.x - player->extent1.x;
    player->derivedVectors[4].y = player->drawPosition.y - player->extent1.y;
    player->derivedVectors[4].z = player->drawPosition.z - player->extent1.z;
    player->derivedVectors[5].x = player->drawPosition.x + player->extent1.x;
    player->derivedVectors[5].y = player->drawPosition.y + player->extent1.y;
    player->derivedVectors[5].z = player->drawPosition.z + player->extent1.z;

    player->updateTimer1.previous = player->updateTimer1.current;
    float timerScale = *player->updateTimer1.scale;
    if (timerScale > 0.9900000095367432f &&
        timerScale < 1.0099999904632568f)
    {
        ++player->updateTimer1.current;
        player->updateTimer1.subframe += 1.0f;
    }
    else
    {
        player->updateTimer1.subframe += timerScale;
        player->updateTimer1.current = (int)player->updateTimer1.subframe;
    }

    player->updateTimer2.previous = player->updateTimer2.current;
    timerScale = *player->updateTimer2.scale;
    if (timerScale > 0.9900000095367432f &&
        timerScale < 1.0099999904632568f)
    {
        ++player->updateTimer2.current;
        player->updateTimer2.subframe += 1.0f;
    }
    else
    {
        player->updateTimer2.subframe += timerScale;
        player->updateTimer2.current = (int)player->updateTimer2.subframe;
    }

    if (g_PlayerDrawGate9EB8->value9EB8 == 0 &&
        g_PlayerDrawGate10 != NULL &&
        g_PlayerDrawGate10->value60 != 0 &&
        player->updateTimer1.current % 60 == 0)
    {
        ++g_PlayerPeriodicCounter;
        if (g_PlayerPeriodicCounter > 0x400)
            g_PlayerPeriodicCounter = 0x400;
        else if (g_PlayerPeriodicCounter < -0x400)
            g_PlayerPeriodicCounter = -0x400;
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
            player->updateTimer0.scale = &g_PlayerTimerScale;
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
            bounds[0] = (float)floor(
                (double)(player->drawPosition.x + 225.0f - 16.0f));
            bounds[2] = bounds[0] +
                (float)g_PlayerAuxiliaryDrawSpan * 0.29230770468711853f;
            bounds[1] = (float)floor(
                (double)(player->drawPosition.y + 17.0f - 24.0f));
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

void RebuildPlayerOptions(Player *player)
{
    int i;
    const bool highPowerCleanup = g_PlayerPower >= 100;
    const int secondaryScript = GetSecondaryPlayerOptionScript();

    for (i = 0; i < 4; ++i)
    {
        PlayerOptionRuntime &option = player->options[i];
        if (highPowerCleanup)
            MarkPlayerVmFlag(option.secondaryVmId);
        else
            MarkPlayerVmDeleteState(option.secondaryVmId, 1);

        option.secondaryVmId = 0;
        if (secondaryScript >= 0)
        {
            PlayerVm *vm = PlayerCreateManagedVm(
                player->resource, secondaryScript, PLAYER_VM_LAYER);
            if (vm != NULL)
                option.secondaryVmId = vm->id;
        }
    }

    int optionCount = (int)g_PlayerPower / 20;
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

            int script = GetPrimaryPlayerOptionScript();
            if (script >= 0)
                option.primaryVmId = CreatePrimaryPlayerOptionVm(player, script);
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

                option.replayPair0 = player->replayPositionHistory[(i + 1) * 8];
                if (player->optionMode != 0 && option.state == 0)
                {
                    if (i == 0)
                        LoadPlayerOptionPair(&option.replayPair2, position1);
                    else
                        option.replayPair2 = player->options[i - 1].replayPair2;
                }

                option.updateCallback = PlayerOptionTrailCallback;
                option.primaryVmId = CreatePrimaryPlayerOptionVm(
                    player, PLAYER_OPTION_PRIMARY_SCRIPT_0);
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
                option.primaryVmId = CreatePrimaryPlayerOptionVm(
                    player, PLAYER_OPTION_PRIMARY_SCRIPT_1);
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

                option.primaryVmId = CreatePrimaryPlayerOptionVm(
                    player, PLAYER_OPTION_PRIMARY_SCRIPT_2);
                if (player->optionMode != 0)
                    PlayerSetManagedVmDeleteState(&option.primaryVmId, 3);
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
