#include "Player.hpp"

#include <stddef.h>
#include <math.h>

struct PlayerVm;

struct PlayerVmListNode
{
    PlayerVm *value;
    PlayerVmListNode *next;
    PlayerVmListNode *previous;
};

struct PlayerVm
{
    unsigned int id;
    unsigned char unknown004[0x10];
    PlayerVmListNode *children;
    int childOwner;
    unsigned char unknown01C[0x04];
    int layer;
    unsigned char unknown024[0x2e0];
    unsigned short deleteState;
    unsigned char unknown306[0x56];
    unsigned int flags;
};
typedef char PlayerVmChildrenAt14[
    (offsetof(PlayerVm, children) == 0x14) ? 1 : -1];
typedef char PlayerVmLayerAt20[
    (offsetof(PlayerVm, layer) == 0x20) ? 1 : -1];
typedef char PlayerVmDeleteStateAt304[
    (offsetof(PlayerVm, deleteState) == 0x304) ? 1 : -1];
typedef char PlayerVmFlagsAt35C[
    (offsetof(PlayerVm, flags) == 0x35c) ? 1 : -1];

struct PlayerVmManager
{
    unsigned char unknown000[0x72dad4];
    PlayerVmListNode *primaryHead;
    PlayerVmListNode *primaryTail;
    PlayerVmListNode *secondaryHead;
    PlayerVmListNode *secondaryTail;
};
typedef char PlayerVmManagerPrimaryHeadAt72DAD4[
    (offsetof(PlayerVmManager, primaryHead) == 0x72dad4) ? 1 : -1];
typedef char PlayerVmManagerSecondaryHeadAt72DADC[
    (offsetof(PlayerVmManager, secondaryHead) == 0x72dadc) ? 1 : -1];

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

extern PlayerDrawGate54 *g_PlayerDrawGate54;
extern PlayerDrawGate10 *g_PlayerDrawGate10;
extern PlayerDrawGate9EB8 *g_PlayerDrawGate9EB8;
extern PlayerDrawGate4 *g_PlayerDrawGate4;
extern int g_PlayerAuxiliaryDrawSpan;

// Descriptive interfaces for target-observed draw callees. The target machine
// boundaries use private register conventions; these declarations are
// maintained source spellings, not original ABI claims.
void PlayerDrawManagedVm(PlayerDrawVmView *vm);
void PlayerDrawAuxiliaryRectangle(const float *bounds, unsigned int color);

struct PlayerOptionPosition
{
    float x;
    float y;
    float z;
};
typedef char PlayerOptionPositionSizeIs0C[
    (sizeof(PlayerOptionPosition) == 0x0c) ? 1 : -1];

extern short g_PlayerPower;
extern int g_PlayerCharacter;
extern int g_PlayerShotType;
extern int g_PlayerOptionPositionBase[];
extern float g_PlayerOptionCoordinateScale;
extern PlayerVmManager *g_PlayerVmManager;

// Descriptive interfaces for independently observed VM-manager callees. Their
// own original identifiers, source conventions, and physical owners remain
// unproven by this packet.
PlayerVm *PlayerCreateManagedVm(void *resource, int scriptIndex, int layer);
PlayerVm *PlayerAllocateManagedVm();
void PlayerInitializeManagedVmScript(PlayerVm *vm, int scriptIndex);
void PlayerRegisterManagedVm(PlayerVm *vm, unsigned int *idOut);
void PlayerSetManagedVmDeleteState(unsigned int *vmId, unsigned short state);

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
    return (const PlayerOptionPosition *)(player->optionData + tableOffset) + index;
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

// Maintained spelling of the Player callback registered in the target's second
// (draw-phase) chain. The physical target entry at 0x00426510 adapts ECX to an
// EAX-bound body at 0x00426360; source-written thunk versus compiler/LTCG
// adapter ownership remains unknown.
int __fastcall PlayerDrawCallback(Player *player)
{
    if (player->runtimeState != 2)
    {
        player->drawVm.positionX = player->drawPositionX + 224.0f;
        player->drawVm.positionY = player->drawPositionY + 16.0f;
        player->drawVm.positionZ = player->drawPositionZ;
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
                (double)(player->drawPositionX + 225.0f - 16.0f));
            bounds[2] = bounds[0] +
                (float)g_PlayerAuxiliaryDrawSpan * 0.29230770468711853f;
            bounds[1] = (float)floor(
                (double)(player->drawPositionY + 17.0f - 24.0f));
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
