#include "Player.hpp"

#include <stddef.h>

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

// Opaque maintained code-entry symbols. The target stores these addresses in
// option records, but the wrappers' source-level callback ABI/origin is not
// established by this packet.
extern unsigned char PlayerOptionTrailCallbackEntry;
extern unsigned char PlayerOptionSpecialCallbackEntry;

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

                option.updateCallback = &PlayerOptionTrailCallbackEntry;
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
                option.updateCallback = &PlayerOptionSpecialCallbackEntry;
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
