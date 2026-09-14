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
void PlayerUpdateMovementAndOptions(Player *player);
void PlayerTransitionToState2(Player *player);
float PlayerAngleToPoint(Player *player, const PlayerFloat3 *point);
void PlayerSpawnDirectionalEffect(
    const PlayerFloat3 *position, int kind, unsigned int color,
    float angle, float scale);
void PlayerRequestRuntimeTransition(int value);
void PlayerFallbackRuntimeTransition();
PlayerEffectRowView *PlayerSpawnEffectRow(
    Player *player, const PlayerFloat3 *position,
    float width, float height, int frames, int value);
void PlayerAdvanceEffectMotion(
    PlayerEffectMotionView *motion, float angularStep, float angle);
float PlayerWrapEffectAngle(float value);
void PlayerFinalizeEffectState(void *state);
void PlayerUpdateDrawVmState(PlayerDrawVmView *vm);
void PlayerUpdateInputAction(Player *player);
void PlayerUpdateShots(Player *player);

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

// Maintained spelling of the Player callback registered in the target's first
// (update-phase) chain. The physical target entry at 0x00426500 pushes its
// live-in ECX Player pointer into the stack-bound 0x00425730 body. Source-written
// adapter versus compiler/LTCG ownership remains unknown.
int __fastcall PlayerUpdateCallback(Player *player)
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
            PlayerAdvanceEffectMotion(&motion, motion.angularStep, motion.angle);
            motion.resetField = 0;
        }
        else
        {
            motion.timerStep += motion.timerStepDelta;
            motion.angularStep =
                PlayerWrapEffectAngle(motion.angle + motion.angularStep);
        }

        PlayerFinalizeEffectState(row.finalizeState);
        row.valueX += row.deltaX;
        row.valueY += row.deltaY;

        motion.timerPrevious = motion.timerCurrent;
        float scale = *motion.timerScale;
        if (scale > 0.9900000095367432f && scale < 1.0099999904632568f)
            motion.timerSubframe -= 1.0f;
        else
            motion.timerSubframe -= scale * 1.0f;
        motion.timerCurrent = (int)motion.timerSubframe;
        if (motion.timerCurrent < 1)
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
        player->updateScratch = 0;
        player->updateScratchByte = 0;
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
