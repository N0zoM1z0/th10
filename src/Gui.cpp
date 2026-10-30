#include "GuiScore.hpp"
#include "EnemySoundQueue.hpp"
#include "Main.hpp"
#include "Gui.hpp"
#include "Chain.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "GameStageDescriptor.hpp"
#include "ResultMenu.hpp"
#include "ReplayManager.hpp"

#include <math.h>
#include <stdlib.h>
#include <new>


// These views expose game-state fields read by the reviewed GUI cores.
struct GuiGameOwnerView
{
    unsigned char unknown000[0x378c];
    unsigned int flags378C;
};

typedef char GuiCanonicalPlayerDrawPosition[
    (offsetof(Player, drawPosition) + offsetof(PlayerFloat3, x) == 0x3c0 &&
     offsetof(Player, drawPosition) + offsetof(PlayerFloat3, y) == 0x3c4) ? 1 : -1];
typedef char GuiCanonicalEnemyFields[
    (offsetof(EnemyFullObjectView, runtime) +
         offsetof(EnemyRuntimeView, worldMotion) +
         offsetof(EnemyMotionView, position) + offsetof(PlayerFloat3, x) == 0x1068 &&
     offsetof(EnemyFullObjectView, runtime) + offsetof(EnemyRuntimeView, life) == 0x23fc &&
     offsetof(EnemyFullObjectView, runtime) + offsetof(EnemyRuntimeView, unknown13C4) == 0x2400 &&
     offsetof(EnemyFullObjectView, runtime) + offsetof(EnemyRuntimeView, value13C8) == 0x2404 &&
     offsetof(EnemyFullObjectView, runtime) + offsetof(EnemyRuntimeView, flags) == 0x2480) ? 1 : -1];
typedef char GuiCanonicalEnemyManagerSlotAt10[
    (offsetof(EnemyManagerView, specialEnemySlots) == 0x10) ? 1 : -1];


extern EnemyManagerView *g_EnemyManager;
// Existing full stage-state byte storage contract at target 0x474c40.
extern unsigned char g_GameStageState[];
extern GuiGameOwnerView *g_GuiGameOwner;
extern unsigned char *g_GuiPersistentData;

extern int g_GuiHighScore;
extern short g_GuiPower;
extern int g_GuiCounter4C4C;
extern int g_GuiState4C58;
extern int g_GuiCharacter;
extern int g_GuiShotType;
extern int g_GuiPowerLevel;
extern int g_GuiDifficulty;
extern int g_GuiStage;
extern int g_GuiMessageState;
extern int g_GuiMessageStateAux;
extern int g_GuiValue4C90;
extern int g_GuiValue4C94;
extern int g_GuiValue4C9C;
extern unsigned int g_GuiGameFlags;
extern unsigned int g_GuiInputCurrent;
extern unsigned int g_GuiInputEdges;
extern unsigned int g_GuiSupervisorFlags;
extern int g_GuiGlobalMode;

// Existing maintained global definitions, retained in their original TUs.
extern void EnemyMarkPendingInterrupt(int *id);
extern void PlayerSetShotVmInterrupt2(unsigned int *id);
extern void ResultMenuPendingThree(AnmVmIdView *id);
extern AnmVmIdView ResultMenuFindScript(AnmVmIdView *parent, int script);
extern int __stdcall FrontEndPlayMusic(int channel, int value);
namespace th10 { namespace leaf {
unsigned char *Fn00417010(unsigned char *payload);
void Fn004175E0(unsigned char *scoreState);
} }
// Target-derived logical interfaces; implementations/private ABI remain open.
struct GameChainOwnerView;
extern GameChainOwnerView *g_GameOwner477830;
// 0x477830 is the existing GameManager chain-prefix alias for this menu.
// The target passes this same pointer to 0x423370 in EDI.
extern void *__stdcall StageCreateTransitionEffect(int priority, int kind, int frames,
    unsigned int p3, unsigned int p4, unsigned int p5);
extern int __stdcall GameQueueScaledSoundCommand(float magnitude);



namespace
{

enum GuiMessageOpcodeView
{
    GUI_MSG_END = 0,
    GUI_MSG_SHOW_PLAYER_PORTRAIT = 1,
    GUI_MSG_SHOW_ENEMY_PORTRAIT = 2,
    GUI_MSG_SHOW_TEXT_BOX = 3,
    GUI_MSG_HIDE_PLAYER_PORTRAIT = 4,
    GUI_MSG_HIDE_ENEMY_PORTRAIT = 5,
    GUI_MSG_HIDE_TEXT = 6,
    GUI_MSG_SET_SIDE_0 = 7,
    GUI_MSG_SET_SIDE_1 = 8,
    GUI_MSG_SET_SKIPPABLE = 9,
    GUI_MSG_WAIT = 10,
    GUI_MSG_INPUT_COOLDOWN = 11,
    GUI_MSG_PLAYER_PORTRAIT_SPRITES = 12,
    GUI_MSG_ENEMY_PORTRAIT_SPRITES = 13,
    GUI_MSG_TEXT_LINE_0 = 14,
    GUI_MSG_TEXT_LINE_1 = 15,
    GUI_MSG_TEXT_LINE_ALTERNATING = 16,
    GUI_MSG_HIDE_TEXT_LINES = 17,
    GUI_MSG_EVENT_18 = 18,
    GUI_MSG_ENEMY_NAME = 19,
    GUI_MSG_COMPLETE_STAGE = 20,
    GUI_MSG_EVENT_21 = 21,
    GUI_MSG_PLAYER_PORTRAIT_EXIT = 22,
    GUI_MSG_ENEMY_PORTRAIT_EXIT = 23
};

enum
{
    GUI_VM_RENDER_LAYER = 15,
    GUI_INPUT_ADVANCE = 0x100,
    GUI_INPUT_CANCEL_MASK = 0x1001,
    GUI_ANM_DELETE_FLAG = 0x04000000
};

static void DeleteVm(AnmVmIdView *id)
{
    if (id->value != 0)
        g_AnmRenderManagerView->MarkVmForDeletion(id->value);
    id->value = 0;
}

static AnmVmView *ResolveVm(AnmVmIdView *id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(id->value);
    if (vm == NULL)
        id->value = 0;
    return vm;
}



}


// Target 0x00415E90-0x0041700F is the complete 24-opcode stage-message
// interpreter plus its compiler-owned selector tables.  TH08 supplies only
// broad ancestry: TH10 has a compact heap-owned 0x90-byte VM and a different
// opcode assignment, portrait-child scheme and stage-clear path.
int __stdcall GuiMessageVmView::Run(GuiMessageVmView *message)
{
    if (message->inputCooldown > 0)
        --message->inputCooldown;

    if ((message->flags & 1) != 0 &&
        (g_GuiInputCurrent & GUI_INPUT_ADVANCE) != 0)
    {
        message->instructionTimer.SetCurrent(GuiMessageInstructionTime(message->currentInstruction));
    }

    int instructionTime = GuiMessageInstructionTime(message->currentInstruction);
    while (message->instructionTimer.current >= instructionTime)
    {
        GuiMessageInstructionView *instruction = message->currentInstruction;
        // Retain the target's physical dispatch groups, starting with text output.
        switch (GuiMessageInstructionOpcode(instruction))
        {
        case GUI_MSG_TEXT_LINE_0:
        {
            AnmVmIdView *id = &message->vmIds[GUI_MESSAGE_TEXT_LINE_0];
            AnmRenderManagerView *manager = g_AnmRenderManagerView;
            AnmVmView *line = manager->FindVm(*id);
            if (line == NULL)
                id->value = 0;
            unsigned char *text = th10::leaf::Fn00417010(
                GuiMessageInstructionPayload(message->currentInstruction));
            manager->DrawTextLeft(line, message->textColors[message->activeSide],
                reinterpret_cast<const char *>(text));
            PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&id->value));
            break;
        }
        case GUI_MSG_TEXT_LINE_1:
        {
            AnmVmIdView *id = &message->vmIds[GUI_MESSAGE_TEXT_LINE_1];
            AnmRenderManagerView *manager = g_AnmRenderManagerView;
            AnmVmView *line = manager->FindVm(*id);
            if (line == NULL)
                id->value = 0;
            unsigned char *text = th10::leaf::Fn00417010(
                GuiMessageInstructionPayload(message->currentInstruction));
            manager->DrawTextLeft(line, message->textColors[message->activeSide],
                reinterpret_cast<const char *>(text));
            PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&id->value));
            break;
        }
        case GUI_MSG_TEXT_LINE_ALTERNATING:
            if (message->alternatingLine == 0)
            {
                {
                    AnmVmIdView *id = &message->vmIds[GUI_MESSAGE_TEXT_LINE_0];
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    AnmVmView *line = manager->FindVm(*id);
                    if (line == NULL)
                        id->value = 0;
                    manager->DrawTextLeft(line, message->textColors[message->activeSide], " ");
                }
                {
                    AnmVmIdView *id = &message->vmIds[GUI_MESSAGE_TEXT_LINE_1];
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    AnmVmView *line = manager->FindVm(*id);
                    if (line == NULL)
                        id->value = 0;
                    manager->DrawTextLeft(line, message->textColors[message->activeSide], " ");
                }
                {
                    AnmVmIdView *id = &message->vmIds[GUI_MESSAGE_TEXT_LINE_0];
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    AnmVmView *line = manager->FindVm(*id);
                    if (line == NULL)
                        id->value = 0;
                    unsigned char *text = th10::leaf::Fn00417010(
                        GuiMessageInstructionPayload(message->currentInstruction));
                    manager = g_AnmRenderManagerView;
                    manager->DrawTextLeft(line, message->textColors[message->activeSide],
                        reinterpret_cast<const char *>(text));
                }
                PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value));
                ResultMenuPendingThree(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1]);
                ++message->alternatingLine;
            }
            else
            {
                {
                    AnmVmIdView *id = &message->vmIds[GUI_MESSAGE_TEXT_LINE_1];
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    AnmVmView *line = manager->FindVm(*id);
                    if (line == NULL)
                        id->value = 0;
                    unsigned char *text = th10::leaf::Fn00417010(
                        GuiMessageInstructionPayload(message->currentInstruction));
                    manager->DrawTextLeft(line, message->textColors[message->activeSide],
                        reinterpret_cast<const char *>(text));
                }
                PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value));
                message->alternatingLine = 0;
            }
            break;

        case GUI_MSG_HIDE_TEXT_LINES:
            ResultMenuPendingThree(&message->vmIds[GUI_MESSAGE_TEXT_LINE_0]);
            ResultMenuPendingThree(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1]);
            break;

        case GUI_MSG_SHOW_TEXT_BOX:
            {
                {
                    AnmLoadedView *loaded = g_GuiView->frontAnm;
                    message->vmIds[GUI_MESSAGE_TEXT_BOX].value = loaded->CreateVmVariant0(0x5a, GUI_VM_RENDER_LAYER).value;
                }
            }
            break;

        case GUI_MSG_SHOW_PLAYER_PORTRAIT:
            switch (g_GuiCharacter)
            {
            case 0:
            {
                AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_Player->resource);
                message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value = loaded->CreateVmVariant0(0x1d, GUI_VM_RENDER_LAYER).value;
                break;
            }
            case 1:
            {
                AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_Player->resource);
                message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value = loaded->CreateVmVariant0(0x1d, GUI_VM_RENDER_LAYER).value;
                break;
            }
            }
            break;

        case GUI_MSG_SHOW_ENEMY_PORTRAIT:
            if (g_GuiStage == 6)
            {
                switch (g_GuiCharacter)
                {
                case 0:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x20, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                case 1:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x20, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                }
            }
            else if (g_GuiStage == 7 && message->messageIndex < 1)
            {
                switch (g_GuiCharacter)
                {
                case 0:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x25, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                case 1:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x25, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                }
            }
            else
            {
                switch (g_GuiCharacter)
                {
                case 0:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_Player->resource);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x1e, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                case 1:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_Player->resource);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x1e, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                }
            }
            break;

        case GUI_MSG_PLAYER_PORTRAIT_EXIT:
            message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].SetInterrupt(7);
            break;

        case GUI_MSG_ENEMY_PORTRAIT_EXIT:
            message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].SetInterrupt(7);
            break;
        case GUI_MSG_HIDE_PLAYER_PORTRAIT:
            EnemyMarkPendingInterrupt(&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value);
            message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value = 0;
            break;

        case GUI_MSG_HIDE_ENEMY_PORTRAIT:
            EnemyMarkPendingInterrupt(&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value);
            message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = 0;
            EnemyMarkPendingInterrupt(&message->vmIds[GUI_MESSAGE_ENEMY_NAME].value);
            break;

        case GUI_MSG_HIDE_TEXT:
            EnemyMarkPendingInterrupt(&message->vmIds[GUI_MESSAGE_TEXT_BOX].value);
            EnemyMarkPendingInterrupt(&message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value);
            EnemyMarkPendingInterrupt(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value);
            break;

        case GUI_MSG_SET_SIDE_0:
        {
            ResultMenuPendingThree(&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT]);
            PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value));
            message->activeSide = 0;
            AnmRenderManagerView *manager = g_AnmRenderManagerView;
            manager->SetVmPosition(
                message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value,
                &message->textPositions[0]);
            manager->SetVmPosition(
                message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value,
                &message->textPositions[message->activeSide]);
            message->alternatingLine = 0;
            break;
        }

        case GUI_MSG_SET_SIDE_1:
        {
            ResultMenuPendingThree(&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]);
            PlayerSetShotVmInterrupt2(reinterpret_cast<unsigned int *>(&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value));
            message->activeSide = 1;
            AnmRenderManagerView *manager = g_AnmRenderManagerView;
            manager->SetVmPosition(
                message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value,
                &message->textPositions[1]);
            manager->SetVmPosition(
                message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value,
                &message->textPositions[message->activeSide]);
            message->alternatingLine = 0;
            break;
        }

        case GUI_MSG_SET_SKIPPABLE:
            message->flags ^= (static_cast<unsigned int>(
                GuiMessageInstructionPayload(instruction)[0]) ^ message->flags) & 1;
            break;

        case GUI_MSG_PLAYER_PORTRAIT_SPRITES:
        {
            switch (g_GuiCharacter)
            {
                case 0:
                {
                    const int first = GuiMessageInstructionIntArgument(message->currentInstruction)
                        + 0x34;
                    AnmVmIdView child0 = ResultMenuFindScript(
                        &message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 0x17);
                    child0.SetSprite(first);
                    const int second = GuiMessageInstructionIntArgument(message->currentInstruction)
                        + 0x3c;
                    AnmVmIdView child1 = ResultMenuFindScript(
                        &message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 0x1a);
                    child1.SetSprite(second);
                    break;
                }
                case 1:
                {
                    const int first = GuiMessageInstructionIntArgument(message->currentInstruction)
                        + 0x2d;
                    AnmVmIdView child0 = ResultMenuFindScript(
                        &message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 0x17);
                    child0.SetSprite(first);
                    const int second = GuiMessageInstructionIntArgument(message->currentInstruction)
                        + 0x35;
                    AnmVmIdView child1 = ResultMenuFindScript(
                        &message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 0x1a);
                    child1.SetSprite(second);
                    break;
                }
            }
            break;
        }

        case GUI_MSG_ENEMY_PORTRAIT_SPRITES:
        {
            switch (g_GuiStage)
            {
                case 1:
                {
                    switch (g_GuiCharacter)
                    {
                    case 0:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0xf;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x17;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    case 1:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0xf;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x17;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    }
                    break;
                }
                case 2:
                {
                    switch (g_GuiCharacter)
                    {
                    case 0:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x15;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x1e;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    case 1:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x15;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x1e;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    }
                    break;
                }
                case 3:
                {
                    switch (g_GuiCharacter)
                    {
                    case 0:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x22;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x2b;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    case 1:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x22;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x2b;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    }
                    break;
                }
                case 4:
                {
                    switch (g_GuiCharacter)
                    {
                    case 0:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x25;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x2e;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    case 1:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x25;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x2e;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    }
                    break;
                }
                case 5:
                {
                    switch (g_GuiCharacter)
                    {
                    case 0:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x16;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x1f;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    case 1:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x16;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x1f;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    }
                    break;
                }
                case 6:
                {
                    switch (g_GuiCharacter)
                    {
                    case 0:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x30;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1c).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x39;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1f).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    case 1:
                    {
                        const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x30;
                        AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1c).SetSpriteWithAnm(resource, first);
                        resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x39;
                        ResultMenuFindScript(
                            &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1f).SetSpriteWithAnm(resource, second);
                        break;
                    }
                    }
                    break;
                }
                case 7:
                {
                    if (message->messageIndex >= 1)
                    {
                        switch (g_GuiCharacter)
                        {
                        case 0:
                        {
                            const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x2e;
                            AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                            resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x37;
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                            break;
                        }
                        case 1:
                        {
                            const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x2e;
                            AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x18).SetSpriteWithAnm(resource, first);
                            resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x37;
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x1b).SetSpriteWithAnm(resource, second);
                            break;
                        }
                        }
                    }
                    else
                    {
                        switch (g_GuiCharacter)
                        {
                        case 0:
                        {
                            const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x44;
                            AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x21).SetSpriteWithAnm(resource, first);
                            resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x4d;
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x24).SetSpriteWithAnm(resource, second);
                            break;
                        }
                        case 1:
                        {
                            const int first = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x44;
                            AnmLoadedView *resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x21).SetSpriteWithAnm(resource, first);
                            resource = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                            const int second = GuiMessageInstructionIntArgument(message->currentInstruction) + 0x4d;
                            ResultMenuFindScript(
                                &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0x24).SetSpriteWithAnm(resource, second);
                            break;
                        }
                        }
                    }
                    break;
                }
            }
            break;
        }

        case GUI_MSG_WAIT:
            if (message->waitTimer.current < 1)
                message->waitTimer.SetCurrent(GuiMessageInstructionIntArgument(instruction));
            message->waitTimer.Add(-1.0f);
            if ((g_GuiInputEdges & GUI_INPUT_CANCEL_MASK) == 0 &&
                message->waitTimer.current > 0)
            {
                if ((message->flags & 1) == 0 ||
                    (g_GuiInputCurrent & GUI_INPUT_ADVANCE) == 0)
                {
                    return 0;
                }

                message->waitTimer.SetCurrent(0);
                message->alternatingLine = 0;
            }
            else
            {
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(0, 0);
                message->waitTimer.SetCurrent(0);
                message->alternatingLine = 0;
            }
            break;

        case GUI_MSG_INPUT_COOLDOWN:
            message->inputCooldown = 1;
            break;

        case GUI_MSG_EVENT_18:
            FrontEndPlayMusic(1, g_GameStageUpdate->value28);
            {
                AnmLoadedView *loaded = g_GuiView->stageUiAnm;
                loaded->CreateVmVariant0(2, GUI_VM_RENDER_LAYER).value;
            }
            break;

        case GUI_MSG_ENEMY_NAME:
        {
            switch (g_GuiStage)
            {
                case 1:
                    {
                        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(0xd, GUI_VM_RENDER_LAYER).value;
                    }
                    break;
                case 2:
                    {
                        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(0x10, GUI_VM_RENDER_LAYER).value;
                    }
                    break;
                case 3:
                    {
                        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(0x14, GUI_VM_RENDER_LAYER).value;
                    }
                    break;
                case 4:
                    {
                        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(0x15, GUI_VM_RENDER_LAYER).value;
                    }
                    break;
                case 5:
                    {
                        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(0xe, GUI_VM_RENDER_LAYER).value;
                    }
                    break;
                case 6:
                    {
                        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(0x23, GUI_VM_RENDER_LAYER).value;
                    }
                    break;
                case 7:
                    {
                        AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                        message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(0x1d, GUI_VM_RENDER_LAYER).value;
                    }
                    break;
            }
            break;
        }

        case GUI_MSG_COMPLETE_STAGE:
        {
            if (g_GuiDifficulty != 4)
            {
                unsigned char *profile = g_GuiPersistentData;
                const int shot = g_GuiShotType + g_GuiCharacter * 3;
                profile[shot * 0x437c + 0x4e1 +
                    (g_GuiStage + g_GuiDifficulty * 6) * 8] = 1;
                profile[(g_GuiShotType + g_GuiCharacter * 3) * 0x437c + 0x4e0 +
                    (g_GuiStage + g_GuiDifficulty * 6) * 8] = 1;
            }
            if ((g_GuiGameFlags & 0x10) != 0)
            {
                GameCompleteStageTransition(
                    reinterpret_cast<ResultMenuView *>(g_GameOwner477830));
                break;
            }
            if (g_GuiStage == 6)
            {
                g_GuiView->guiFlags |= 0x20;
                g_GuiScore.Add(g_GuiCounter4C4C * 1000);
                switch (g_GuiDifficulty)
                {
                case 0:
                    g_GuiScore.Add(g_GuiPowerLevel * 20000000);
                    g_GuiScore.Add(static_cast<int>(g_GuiPower) * 100000);
                    break;
                case 1:
                    g_GuiScore.Add(g_GuiPowerLevel * 25000000);
                    g_GuiScore.Add(static_cast<int>(g_GuiPower) * 100000);
                    break;
                case 2:
                    g_GuiScore.Add(g_GuiPowerLevel * 35000000);
                    g_GuiScore.Add(static_cast<int>(g_GuiPower) * 200000);
                    break;
                case 3:
                    g_GuiScore.Add(g_GuiPowerLevel * 40000000);
                    g_GuiScore.Add(static_cast<int>(g_GuiPower) * 300000);
                    break;
                case 4:
                    g_GuiScore.Add(g_GuiPowerLevel * 40000000);
                    g_GuiScore.Add(static_cast<int>(g_GuiPower) * 400000);
                    break;
                }
                if (g_ReplayManager->mode == 1)
                {
                    goto CompleteReplayStage;
                }
                StageCreateTransitionEffect(0x31, 5, 120, 0, 0, 0);
                GuiView *gui = g_GuiView;
                gui->endingCounter = 0;
                gui->guiFlags |= 0x10;
                {
                    unsigned char *counter = g_GuiPersistentData +
                        (g_GuiCharacter * 3 + g_GuiShotType) * 0x437c + 0x4d0 +
                        g_GuiDifficulty * 4;
                    int value;
                    memcpy(&value, counter, sizeof(value));
                    if (value < 99999)
                    {
                        ++value;
                        memcpy(counter, &value, sizeof(value));
                    }
                }
            }
            else if (g_GuiStage == 7)
            {
                g_GuiView->guiFlags |= 0x20;
                g_GuiScore.Add(g_GuiCounter4C4C * 1000);
                g_GuiScore.Add(g_GuiPowerLevel * 40000000);
                g_GuiScore.Add(static_cast<int>(g_GuiPower) * 400000);
                if (g_ReplayManager->mode == 1)
                {
                    goto CompleteReplayStage;
                }
                GameCompleteStageTransition(
                    reinterpret_cast<ResultMenuView *>(g_GameOwner477830));
                {
                    unsigned char *counter = g_GuiPersistentData +
                        (g_GuiCharacter * 3 + g_GuiShotType) * 0x437c + 0x4d0 +
                        g_GuiDifficulty * 4;
                    int value;
                    memcpy(&value, counter, sizeof(value));
                    if (value < 99999)
                    {
                        ++value;
                        memcpy(counter, &value, sizeof(value));
                    }
                }
            }
            else
            {
                GuiView *gui = g_GuiView;
                g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14.value);
                gui->specialVmId9E14.value = 0;
                {
                    AnmLoadedView *loaded = gui->frontAnm;
                    gui->specialVmId9E14.value = loaded->CreateVmVariant0(0x4c, GUI_VM_RENDER_LAYER).value;
                }
                g_MainSupervisorView.SetNextGameMode(0x0b);
                th10::leaf::Fn004175E0(g_GameStageState);
            }
            break;

        CompleteReplayStage:
            g_MainSupervisorView.SetNextGameMode(4);
            break;
        }

        case GUI_MSG_EVENT_21:
            if (g_GuiStage != 6)
                GameQueueScaledSoundCommand(2.0f);
            else
                GameQueueScaledSoundCommand(8.0f);
            break;

        case GUI_MSG_END:
            if (g_GuiMessageState != 0)
                g_GuiMessageStateAux = 0;
            g_GuiMessageState = 0;
            return -1;

        }

        message->currentInstruction = GuiMessageInstructionNext(message->currentInstruction);
        instructionTime = GuiMessageInstructionTime(message->currentInstruction);
    }

    message->instructionTimer.Tick();
    return 0;
}


// Target 0x00414900-0x004157FB is the central stage-HUD updater.  It advances
// 22 embedded display VMs, maintains the player-side panel, power digits, boss
// gauge/segments, the heap-owned message VM, spell timer and boss marker, and
// finally advances the GUI frame timer.
// Target 0x00413790-0x004137DC mirrors the current life count into the
// nine preconstructed HUD life VMs.  LTCG keeps the GuiView owner in EAX and
// the count in ECX in the reviewed Player reset/death caller family.
void GuiSetLivesDisplayCount(GuiView *gui, int count)
{
    int i;
    for (i = 0; i < count; ++i)
        gui->displayVms2[i].flags35C |= 2;
    for (; i < 9; ++i)
        gui->displayVms2[i].flags35C &= ~2u;
}

void GuiView::SetLivesDisplayCount(int count)
{
    GuiSetLivesDisplayCount(this, count);
}

void GuiSetLivesDisplayCountMember(GuiView *gui, int count)
{
    gui->SetLivesDisplayCount(count);
}

int __stdcall GuiView::UpdateStageElements(GuiView *gui)
{
    int i;
    if ((gui->guiFlags & 0x10) != 0)
    {
        ++gui->endingCounter;
        if (gui->endingCounter >= 120)
            g_GuiGlobalMode = (g_GuiSupervisorFlags & 0x1000) != 0 ? 2 : 14;
    }

    for (i = 0; i < 9; ++i)
        AnmRenderManagerView::ExecuteScript(&gui->displayVms2[i]);
    for (i = 0; i < 4; ++i)
        AnmRenderManagerView::ExecuteScript(&gui->displayVms3[i]);
    for (i = 0; i < 2; ++i)
        AnmRenderManagerView::ExecuteScript(&gui->displayVms4[i]);

    if ((gui->guiFlags & 1) == 0)
    {
        if (g_Player != NULL &&
            g_Player->drawPosition.y > 416.0f &&
            g_Player->drawPosition.x < -128.0)
        {
            for (i = 0; i < 7; ++i)
                gui->displayVms5[i].pendingInterrupt = 3;
            gui->guiFlags |= 1;
        }
    }
    else if (g_Player != NULL &&
             (g_Player->drawPosition.y < 400.0f ||
              g_Player->drawPosition.x > -112.0))
    {
        for (i = 0; i < 7; ++i)
            gui->displayVms5[i].pendingInterrupt = 2;
        gui->guiFlags &= ~1u;
    }

    // The target latches the counter before executing the first panel VM.
    int value = g_GuiCounter4C4C;
    int divisor = 10000;
    AnmRenderManagerView::ExecuteScript(&gui->displayVms5[0]);
    for (i = 1; i < 6; ++i)
    {
        const int digit = value / divisor;
        value %= divisor;
        gui->frontAnm->SetSprite(&gui->displayVms5[i], digit + 0x1e);
        divisor /= 10;
        AnmRenderManagerView::ExecuteScript(&gui->displayVms5[i]);
    }
    AnmRenderManagerView::ExecuteScript(&gui->displayVms5[6]);

    // HUD boss-gauge phase begin: target 0x00414ACF-0x0041513B.
    {
        EnemyFullObjectView *boss = g_EnemyManager == NULL
            ? NULL : g_EnemyManager->specialEnemySlots[0];
        if (boss != NULL && gui->message == NULL)
        {
            const int bossLife = boss->runtime.life;
            gui->bossLife = bossLife;
            gui->bossGaugeTarget = static_cast<float>(bossLife) /
                static_cast<float>(boss->runtime.unknown13C4);
            if (gui->bossGaugeDisplayed < gui->bossGaugeTarget)
                gui->bossGaugeDisplayed += 0.025f;
            if (gui->bossGaugeDisplayed > gui->bossGaugeTarget)
                gui->bossGaugeDisplayed = gui->bossGaugeTarget;

            // The native active-boss path requires a live player and ANM owner.
            AnmRenderManagerView *manager = g_AnmRenderManagerView;
            if ((gui->guiFlags & 8) != 0)
            {
                if (g_Player->drawPosition.y >= 80.0f ||
                    g_Player->drawPosition.x > 0.0f)
                {
                    for (int segment = 0; segment < gui->bossGaugeSegmentCount; ++segment)
                    {
                        const int id = gui->bossGaugeSegmentVmIds[segment].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->pendingInterrupt = 2;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 2;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    {
                        const int id = gui->bossGaugeVmId.value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->pendingInterrupt = 2;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 2;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    gui->guiFlags &= ~8u;
                }
            }
            else if (g_Player->drawPosition.y <= 64.0f &&
                     g_Player->drawPosition.x < -64.0f)
            {
                for (int segment = 0; segment < gui->bossGaugeSegmentCount; ++segment)
                {
                    const int id = gui->bossGaugeSegmentVmIds[segment].value;
                    if (id != 0)
                    {
                        AnmVmLayerNodeView *node = manager->primaryVmListHead;
                        while (node != NULL &&
                               static_cast<AnmVmView *>(node->owner)->id != id)
                            node = node->next;
                        if (node == NULL)
                        {
                            node = manager->secondaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                        }
                        if (node != NULL)
                        {
                            AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                            if (vm != NULL)
                            {
                                vm->pendingInterrupt = 3;
                                if (vm->layerNode.previous == NULL)
                                {
                                    node = vm->layerNode.next;
                                    while (node != NULL)
                                    {
                                        static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 3;
                                        node = node->next;
                                    }
                                }
                            }
                        }
                    }
                }
                {
                    const int id = gui->bossGaugeVmId.value;
                    if (id != 0)
                    {
                        AnmVmLayerNodeView *node = manager->primaryVmListHead;
                        while (node != NULL &&
                               static_cast<AnmVmView *>(node->owner)->id != id)
                            node = node->next;
                        if (node == NULL)
                        {
                            node = manager->secondaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                        }
                        if (node != NULL)
                        {
                            AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                            if (vm != NULL)
                            {
                                vm->pendingInterrupt = 3;
                                if (vm->layerNode.previous == NULL)
                                {
                                    node = vm->layerNode.next;
                                    while (node != NULL)
                                    {
                                        static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 3;
                                        node = node->next;
                                    }
                                }
                            }
                        }
                    }
                }
                gui->guiFlags |= 8;
            }

            if (gui->bossGaugeVmId.value == 0)
            {
                // HUD boss-gauge stage selector begin.
                int script = 0x85;
                switch (g_GuiStage)
                {
                case 1:
                    if (g_GuiMessageState >= 0x18)
                        script = 0x86;
                    break;
                case 2:
                    if (g_GuiMessageState < 0x18)
                        goto BossGaugeCreationDone;
                    script = 0x87;
                    break;
                case 3:
                    if (g_GuiMessageState < 0x18)
                        goto BossGaugeCreationDone;
                    script = 0x88;
                    break;
                case 4:
                    script = g_GuiMessageState >= 0x18 ? 0x8a : 0x89;
                    break;
                case 5:
                    script = 0x8b;
                    break;
                case 6:
                    script = 0x8c;
                    break;
                case 7:
                    script = g_GuiMessageState >= 0x18 ? 0x8d : 0x8c;
                    break;
                }

                // HUD boss-gauge stage selector end.
                {
                    // Allocate using the retained manager, initialize using the captured resource,
                    // then publish through the freshly loaded global manager.
                    AnmLoadedView *loaded = gui->frontAnm;
                    AnmVmView *vm = manager->AllocateVm();
                    vm->flags35C |= 0x40000000u;
                    vm->renderLayer = GUI_VM_RENDER_LAYER;
                    loaded->InitializeVm(vm, script);
                    manager = g_AnmRenderManagerView;
                    AnmVmLayerNodeView *node = &vm->managerNode;
                    node->owner = vm;
                    node->next = NULL;
                    node->previous = NULL;
                    if (manager->primaryVmListHead == NULL)
                        manager->primaryVmListHead = node;
                    else
                    {
                        AnmVmLayerNodeView *tail = manager->primaryVmListTail;
                        if (tail->next != NULL)
                        {
                            node->next = tail->next;
                            tail->next->previous = node;
                        }
                        tail->next = node;
                        node->previous = tail;
                    }
                    manager->primaryVmListTail = node;
                    AnmVmIdView *incrementReceiver = &manager->nextVmId;
                    (*incrementReceiver)++;
                    if (manager->nextVmId == AnmVmIdView())
                        manager->nextVmId++;
                    vm->id = manager->nextVmId.value;
                    gui->bossGaugeVmId.value = manager->nextVmId.value;
                }
            }
        BossGaugeCreationDone:
            for (int segment = 0; segment < 10u; ++segment)
            {
                if (segment < gui->bossGaugeSegmentCount)
                {
                    if (gui->bossGaugeSegmentVmIds[segment].value == 0)
                    {
                        // Allocate using the retained manager, initialize using the captured resource,
                        // then publish through the freshly loaded global manager.
                        AnmLoadedView *loaded = gui->frontAnm;
                        AnmVmView *vm = manager->AllocateVm();
                        vm->flags35C |= 0x40000000u;
                        vm->renderLayer = GUI_VM_RENDER_LAYER;
                        loaded->InitializeVm(vm, segment + 0x5b);
                        manager = g_AnmRenderManagerView;
                        AnmVmLayerNodeView *node = &vm->managerNode;
                        node->owner = vm;
                        node->next = NULL;
                        node->previous = NULL;
                        if (manager->primaryVmListHead == NULL)
                            manager->primaryVmListHead = node;
                        else
                        {
                            AnmVmLayerNodeView *tail = manager->primaryVmListTail;
                            if (tail->next != NULL)
                            {
                                node->next = tail->next;
                                tail->next->previous = node;
                            }
                            tail->next = node;
                            node->previous = tail;
                        }
                        manager->primaryVmListTail = node;
                        AnmVmIdView *incrementReceiver = &manager->nextVmId;
                        (*incrementReceiver)++;
                        if (manager->nextVmId == AnmVmIdView())
                            manager->nextVmId++;
                        vm->id = manager->nextVmId.value;
                        gui->bossGaugeSegmentVmIds[segment].value = manager->nextVmId.value;
                    }
                }
                else
                {
                    {
                        const int id = gui->bossGaugeSegmentVmIds[segment].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->pendingInterrupt = 1;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                            gui->bossGaugeSegmentVmIds[segment].value = 0;
                        }
                    }
                }
            }
        }
        else
        {
            {
                const int id = gui->bossGaugeVmId.value;
                if (id != 0)
                {
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    AnmVmLayerNodeView *node = manager->primaryVmListHead;
                    while (node != NULL &&
                           static_cast<AnmVmView *>(node->owner)->id != id)
                        node = node->next;
                    if (node == NULL)
                    {
                        node = manager->secondaryVmListHead;
                        while (node != NULL &&
                               static_cast<AnmVmView *>(node->owner)->id != id)
                            node = node->next;
                    }
                    if (node != NULL)
                    {
                        AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                        if (vm != NULL)
                        {
                            vm->pendingInterrupt = 1;
                            if (vm->layerNode.previous == NULL)
                            {
                                node = vm->layerNode.next;
                                while (node != NULL)
                                {
                                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;
                                    node = node->next;
                                }
                            }
                        }
                    }
                    gui->bossGaugeVmId.value = 0;
                }
            }
            gui->bossGaugeDisplayed = 0.0f;
            for (int value = 0; value < 4; ++value)
                gui->bossGaugeValues[value].value = 0.0f;
        }
    }
    // HUD boss-gauge phase end.

    // HUD message phase begin: target 0x0041513C-0x00415499.
    {
        GuiMessageVmView *message = gui->message;
        if (message != NULL)
        {
            if (GuiMessageVmView::Run(message) == 0)
                message->lifetimeTimer.Tick();
            else
            {
                // Run may replace the owner; retain this cleanup receiver through free.
                GuiMessageVmView *finishedMessage = gui->message;
                if (finishedMessage != NULL)
                {
                    AnmRenderManagerView *manager = g_AnmRenderManagerView;
                    {
                        const int id = finishedMessage->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->flags35C |= GUI_ANM_DELETE_FLAG;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->flags35C |= GUI_ANM_DELETE_FLAG;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                        memset(&finishedMessage->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 0, sizeof(AnmVmIdView));
                    }
                    {
                        const int id = finishedMessage->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->flags35C |= GUI_ANM_DELETE_FLAG;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->flags35C |= GUI_ANM_DELETE_FLAG;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                        memset(&finishedMessage->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 0, sizeof(AnmVmIdView));
                    }
                    {
                        const int id = finishedMessage->vmIds[GUI_MESSAGE_TEXT_BOX].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->flags35C |= GUI_ANM_DELETE_FLAG;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->flags35C |= GUI_ANM_DELETE_FLAG;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                        memset(&finishedMessage->vmIds[GUI_MESSAGE_TEXT_BOX], 0, sizeof(AnmVmIdView));
                    }
                    {
                        const int id = finishedMessage->vmIds[GUI_MESSAGE_TEXT_LINE_0].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->flags35C |= GUI_ANM_DELETE_FLAG;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->flags35C |= GUI_ANM_DELETE_FLAG;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                        memset(&finishedMessage->vmIds[GUI_MESSAGE_TEXT_LINE_0], 0, sizeof(AnmVmIdView));
                    }
                    {
                        const int id = finishedMessage->vmIds[GUI_MESSAGE_TEXT_LINE_1].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->flags35C |= GUI_ANM_DELETE_FLAG;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->flags35C |= GUI_ANM_DELETE_FLAG;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                        memset(&finishedMessage->vmIds[GUI_MESSAGE_TEXT_LINE_1], 0, sizeof(AnmVmIdView));
                    }
                    {
                        const int id = finishedMessage->vmIds[GUI_MESSAGE_ENEMY_NAME].value;
                        if (id != 0)
                        {
                            AnmVmLayerNodeView *node = manager->primaryVmListHead;
                            while (node != NULL &&
                                   static_cast<AnmVmView *>(node->owner)->id != id)
                                node = node->next;
                            if (node == NULL)
                            {
                                node = manager->secondaryVmListHead;
                                while (node != NULL &&
                                       static_cast<AnmVmView *>(node->owner)->id != id)
                                    node = node->next;
                            }
                            if (node != NULL)
                            {
                                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                                if (vm != NULL)
                                {
                                    vm->flags35C |= GUI_ANM_DELETE_FLAG;
                                    if (vm->layerNode.previous == NULL)
                                    {
                                        node = vm->layerNode.next;
                                        while (node != NULL)
                                        {
                                            static_cast<AnmVmView *>(node->owner)->flags35C |= GUI_ANM_DELETE_FLAG;
                                            node = node->next;
                                        }
                                    }
                                }
                            }
                        }
                        memset(&finishedMessage->vmIds[GUI_MESSAGE_ENEMY_NAME], 0, sizeof(AnmVmIdView));
                    }
                    ::operator delete(finishedMessage);
                }
                gui->message = NULL;
            }
        }
    }
    // HUD message phase end.

    // HUD spell-countdown and boss-marker phase begin: target 0x0041549A.
    {
        EnemyManagerView *manager = g_EnemyManager;
        if (manager != NULL)
        {
            if (gui->spellSeconds >= 0 && manager->specialEnemySlots[0] != NULL &&
                gui->message == NULL)
            {
                if (gui->spellSeconds < gui->previousSpellSeconds)
                {
                    if (gui->spellSeconds <= 5)
                    {
                        gui->displayVms4[0].pendingInterrupt = 9;
                        gui->displayVms4[1].pendingInterrupt = 9;
                        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                            QueueSoundSample(0x24, 0);
                    }
                    else if (gui->spellSeconds <= 10)
                    {
                        gui->displayVms4[0].pendingInterrupt = 8;
                        gui->displayVms4[1].pendingInterrupt = 8;
                        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                            QueueSoundSample(0x1b, 0);
                    }
                }
                else if (gui->spellSeconds > gui->previousSpellSeconds)
                {
                    gui->displayVms4[0].pendingInterrupt = 7;
                    gui->displayVms4[1].pendingInterrupt = 7;
                }
                if (gui->spellSeconds != gui->previousSpellSeconds)
                {
                    gui->frontAnm->SetSprite(
                        &gui->displayVms4[0], gui->spellSeconds / 10 + 8);
                    gui->frontAnm->SetSprite(
                        &gui->displayVms4[1], gui->spellSeconds % 10 + 8);
                }
                manager = g_EnemyManager;
                gui->previousSpellSeconds = gui->spellSeconds;
            }

            // Select the live boss after countdown calls, then retain it across the
            // boss marker's ExecuteScript, matching the target's ESI lifetime.
            EnemyFullObjectView *boss = manager->specialEnemySlots[0];
            if (boss != NULL && ((~(boss->runtime.flags >> 4)) & 1u) != 0 &&
                ((~boss->runtime.flags) & 1u) != 0)
            {
                if ((g_GuiGameOwner->flags378C & 1) != 0)
                {
                    const unsigned int phase = (gui->guiFlags >> 1) & 3;
                    if (phase == 0)
                    {
                        if (boss->runtime.value13C8 < 2000)
                        {
                            gui->bossPositionVm.pendingInterrupt = 7;
                            gui->guiFlags = (gui->guiFlags & ~4u) | 2;
                        }
                    }
                    else if (phase == 1)
                    {
                        if (boss->runtime.value13C8 < 1000)
                        {
                            gui->bossPositionVm.pendingInterrupt = 8;
                            gui->guiFlags = (gui->guiFlags & ~2u) | 4;
                        }
                    }
                    else if (phase == 2)
                    {
                        if (boss->runtime.value13C8 < 400)
                        {
                            gui->bossPositionVm.pendingInterrupt = 9;
                            gui->guiFlags |= 6;
                        }
                    }
                    else if (phase == 3)
                    {
                        if (boss->runtime.value13C8 > 400)
                        {
                            gui->bossPositionVm.pendingInterrupt = 10;
                            gui->guiFlags &= ~6u;
                        }
                    }
                }
                else
                {
                    const unsigned int phase = (gui->guiFlags >> 1) & 3;
                    if (phase == 0)
                    {
                        if (boss->runtime.value13C8 < 700)
                        {
                            gui->bossPositionVm.pendingInterrupt = 7;
                            gui->guiFlags = (gui->guiFlags & ~4u) | 2;
                        }
                    }
                    else if (phase == 1)
                    {
                        if (boss->runtime.value13C8 < 400)
                        {
                            gui->bossPositionVm.pendingInterrupt = 8;
                            gui->guiFlags = (gui->guiFlags & ~2u) | 4;
                        }
                    }
                    else if (phase == 2)
                    {
                        if (boss->runtime.value13C8 < 200)
                        {
                            gui->bossPositionVm.pendingInterrupt = 9;
                            gui->guiFlags |= 6;
                        }
                    }
                    else if (phase == 3)
                    {
                        if (boss->runtime.value13C8 > 200)
                        {
                            gui->bossPositionVm.pendingInterrupt = 10;
                            gui->guiFlags &= ~6u;
                        }
                    }
                }

                AnmRenderManagerView::ExecuteScript(&gui->bossPositionVm);
                gui->bossPositionVm.positionOffset.x = boss->runtime.worldMotion.position.x + 224.0f;
                gui->bossPositionVm.positionOffset.y = 480.0f;
                const float distance = fabs(
                    static_cast<double>(boss->runtime.worldMotion.position.x) - g_Player->drawPosition.x);
                if (distance >= 64.0)
                    gui->bossPositionVm.primaryColor.alpha = 0xff;
                else
                    gui->bossPositionVm.primaryColor.alpha =
                        static_cast<unsigned char>(
                            64 - static_cast<int>(distance * -2.984375f));
                if (boss->runtime.worldMotion.position.x < -192.0f || boss->runtime.worldMotion.position.x > 192.0f)
                    gui->bossPositionVm.primaryColor.alpha = 0;
            }
        }
    }
    // HUD spell-countdown and boss-marker phase end: target 0x00415764.
    gui->frameTimer.Tick();
    return 1;
}


// Registered update-chain callback at target 0x00415AE0.
// Original source-written versus compiler-emitted adapter remains unproved.
int __fastcall GuiUpdateCallback(void *owner)
{
    return GuiView::UpdateStageElements(static_cast<GuiView *>(owner));
}

// Stage/session fields read by the complete reset owner. Existing game globals
// retain their shared names; this partial owner view exposes only target +0x5C.
struct GuiResetSessionView
{
    unsigned char unknown000[0x5c];
    int value5C;
};
extern GuiResetSessionView *g_GuiResetSession; // target 0x00477810
extern volatile int g_PlayerLivesDisplayCount; // target 0x00474C70
extern int g_GameTransitionState; // target 0x00491FB8
extern unsigned int g_GameRuntimeFlags; // target 0x00474CA0
extern int g_GameNewSession; // target 0x00491FC4
extern int g_GameStartingStage; // target 0x00474C74
extern int g_GameStartupMode; // target 0x00474C7C
extern int g_GameStartupValue474C90;

struct GuiResetCallbackView
{
    unsigned int unknown000;
    unsigned int flags;
};
typedef char GuiResetCallbackFlagsAt004[
    (offsetof(GuiResetCallbackView, flags) == 4) ? 1 : -1];
typedef char GuiResetSpecialIdsAt9E50[
    (offsetof(GuiView, specialVmId9E50) == 0x9e50 &&
     offsetof(GuiView, specialVmId9E54) == 0x9e54 &&
     offsetof(GuiView, specialVmId9E58) == 0x9e58 &&
     offsetof(GuiView, specialVmId9E5C) == 0x9e5c &&
     offsetof(GuiView, bossGaugeSegmentCount) == 0x9e90) ? 1 : -1];

// Target 0x00413BC0-0x0041436C, called by both real stage-reset paths in
// GameManagerUpdateBody. The target uses EDI for gui and plain RET. No game
// callbacks, resource loads, or allocation side effects are executed by tests.
void GameResetGui(GuiView *gui)
{
    if (gui->updateCallback != NULL)
        static_cast<GuiResetCallbackView *>(gui->updateCallback)->flags |= 2;
    if (gui->drawCallback != NULL)
        static_cast<GuiResetCallbackView *>(gui->drawCallback)->flags |= 2;

    if (gui->specialVmId9E58.value == 0)
        gui->specialVmId9E58 = gui->frontAnm->CreateVmVariant0(0, 15);
    if (gui->specialVmId9E5C.value == 0)
    {
        // The target discards this returned ID. Do not silently repair the
        // missing +0x9E5C publication or suppress repeated future allocation.
        gui->frontAnm->CreateVmVariant0(1, 15);
    }

    if ((gui->displayVms0[0].flags35C & 1) == 0)
    {
        for (int i = 0; i < 10; ++i)
        {
            gui->frontAnm->InitializeAndExecuteScriptIndex(
                &gui->displayVms0[i], i + 10);
            gui->frontAnm->InitializeAndExecuteScriptIndex(
                &gui->displayVms1[i], i + 20);
        }
        for (int i = 0; i < 9; ++i)
            gui->frontAnm->InitializeAndExecuteScriptIndex(
                &gui->displayVms2[i], i + 30);
        for (int i = 0; i < 4; ++i)
            gui->frontAnm->InitializeAndExecuteScriptIndex(
                &gui->displayVms3[i], i + 47);
        for (int i = 0; i < 2; ++i)
            gui->frontAnm->InitializeAndExecuteScriptIndex(
                &gui->displayVms4[i], i + 80);
        for (int i = 0; i < 7; ++i)
            gui->frontAnm->InitializeAndExecuteScriptIndex(
                &gui->displayVms5[i], i + 51);
    }

    // The target does not clamp the life count to the nine-element array.
    gui->SetLivesDisplayCount(g_PlayerLivesDisplayCount);
    int power = g_GuiPower;
    int fraction = (power % 20) * 100 / 20;
    gui->frontAnm->SetSprite(&gui->displayVms3[0], power / 20 + 8);
    gui->frontAnm->SetSprite(&gui->displayVms3[2], fraction / 10 + 8);
    gui->frontAnm->SetSprite(&gui->displayVms3[3], fraction % 10 + 8);

    if (g_GameTransitionState != 8 && (g_GameRuntimeFlags & 0x20) == 0)
    {
        gui->stageUiAnm->CreateVmVariant0(0, 15);
        gui->stageUiAnm->CreateVmVariant0(1, 15);
    }
    if ((g_GameRuntimeFlags & 0x20) != 0)
        gui->frontAnm->CreateVmVariant0(0x71, 15);

    g_AsciiManagerView->asciiAnm->InitializeAndExecuteScriptIndex(
        &gui->bossPositionVm, 0);
    if (g_GameStartupMode == 1 && g_GuiResetSession->value5C == 0 &&
        g_GameStartupValue474C90 == 0)
        gui->frontAnm->CreateVmVariant0(0x4f, 15);

    if (g_GameNewSession != 0)
    {
        gui->specialVmId9E50 = gui->frontAnm->CreateVmVariant0(
            g_GameStartingStage + 0x66, 15);
        // Reset interrupt region 1: target-local two-list/root-child walk.
    {
        AnmVmView *vm = g_AnmRenderManagerView->FindVm(gui->specialVmId9E50);
        if (vm != NULL)
        {
            vm->pendingInterrupt = 3;
            if (vm->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 3;
                    node = node->next;
                }
            }
        }
    }
    }
    gui->specialVmId9E54 = gui->frontAnm->CreateVmVariant0(
        g_GameStartingStage + 0x6b, 15);
    // Both target interrupt walks select +0x9E50, including this final walk.
    // Reset interrupt region 2: target-local two-list/root-child walk.
    {
        AnmVmView *vm = g_AnmRenderManagerView->FindVm(gui->specialVmId9E50);
        if (vm != NULL)
        {
            vm->pendingInterrupt = 3;
            if (vm->layerNode.previous == NULL)
            {
                AnmVmLayerNodeView *node = vm->layerNode.next;
                while (node != NULL)
                {
                    static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 3;
                    node = node->next;
                }
            }
        }
    }
    gui->bossGaugeSegmentCount = 0;
}



// GUI lifecycle begin: complete owners 0x4136C0, 0x414370 and 0x4145F0.
// This is the observed shared message-file retention slot, not a new allocation.
extern void *g_GuiRetainedMessageFile491BE8;

// Target 0x004136C0-0x00413720. The owning caller releases the allocation.
GuiMessageVmView::~GuiMessageVmView()
{
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]);
    vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value = 0;
    manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_PORTRAIT]);
    vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = 0;
    manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_BOX]);
    vmIds[GUI_MESSAGE_TEXT_BOX].value = 0;
    manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_LINE_0]);
    vmIds[GUI_MESSAGE_TEXT_LINE_0].value = 0;
    manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_LINE_1]);
    vmIds[GUI_MESSAGE_TEXT_LINE_1].value = 0;
    manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_NAME]);
    vmIds[GUI_MESSAGE_ENEMY_NAME].value = 0;
}

// Target 0x00414370-0x0041454B. This also serves live GUI resource teardown.
void __stdcall GuiCleanupResources(GuiView *gui)
{
    if ((g_GuiGameFlags & 9u) == 0)
    {
        AnmLoadedView **slot = &g_AnmRenderManagerView->loadedAnms[28];
        if (*slot != NULL)
        {
            (*slot)->Release();
            ::operator delete(*slot);
            *slot = NULL;
        }
    }
    else
        g_AnmRenderManagerView->MarkLoadedVmsForDeletion(gui->stageUiAnm);

    GuiMessageVmView *message = gui->message;
    gui->stageUiAnm = NULL;
    if (message != NULL)
    {
        delete message;
        gui->message = NULL;
    }
    if ((g_GuiGameFlags & 9u) == 0)
    {
        if (gui->messageFile != NULL)
        {
            free(gui->messageFile);
            gui->messageFile = NULL;
        }
        gui->messageFile = NULL;
        g_GuiRetainedMessageFile491BE8 = NULL;
    }
    else
        g_GuiRetainedMessageFile491BE8 = gui->messageFile;

    th10::ChainElem *update = static_cast<th10::ChainElem *>(gui->updateCallback);
    if (update != NULL)
        update->flags &= ~2u;
    g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14);
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    gui->specialVmId9E14.value = 0;
    manager->MarkVmForDeletion(gui->specialVmId9E18);
    gui->specialVmId9E18.value = 0;
    manager->MarkVmForDeletion(gui->bossGaugeVmId);
    gui->bossGaugeVmId.value = 0;
    for (int i = 0; i < 10; ++i)
        gui->bossGaugeSegmentVmIds[i].value = 0;
    for (int i = 0; i < 8; ++i)
    {
        const int id = gui->valueVmIds[i].value;
        if (id != 0)
        {
            AnmVmLayerNodeView *node = manager->primaryVmListHead;
            while (node != NULL && static_cast<AnmVmView *>(node->owner)->id != id)
                node = node->next;
            if (node == NULL)
            {
                node = manager->secondaryVmListHead;
                while (node != NULL && static_cast<AnmVmView *>(node->owner)->id != id)
                    node = node->next;
            }
            if (node != NULL)
            {
                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                if (vm != NULL)
                {
                    vm->flags35C |= 0x04000000u;
                    if (vm->layerNode.previous == NULL)
                    {
                        AnmVmLayerNodeView *child = vm->layerNode.next;
                        while (child != NULL)
                        {
                            static_cast<AnmVmView *>(child->owner)->flags35C |= 0x04000000u;
                            child = child->next;
                        }
                    }
                }
            }
        }
        gui->valueVmIds[i].value = 0;
    }
    gui->bossGaugeSegmentCount = 0;
}

// Target414570..4145E8 prepares the retained GUI for another stage.
// Uses the same reviewed ANM slot28 and message storage as resource cleanup.
void GamePrepareGuiReuse414570(GuiView *gui)
{
    GuiMessageVmView *message = gui->message;
    if (message != NULL)
    {
        delete message;
        gui->message = NULL;
    }
    if ((g_GuiGameFlags & 9u) == 0)
    {
        AnmLoadedView **slot = &g_AnmRenderManagerView->loadedAnms[28];
        if (*slot != NULL)
        {
            (*slot)->Release();
            ::operator delete(*slot);
            *slot = NULL;
        }
        gui->stageUiAnm = NULL;
        if (gui->messageFile != NULL)
        {
            free(gui->messageFile);
            gui->messageFile = NULL;
        }
        gui->messageFile = NULL;
    }
}

// Target 0x004145F0-0x0041482D. Member destruction follows this body naturally:
// bossPositionVm, then displayVms5..displayVms0, each array in reverse order.
GuiView::~GuiView()
{
    GuiCleanupResources(this);
    th10::ChainElem *element = static_cast<th10::ChainElem *>(updateCallback);
    th10::Chain *chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(element);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    element = static_cast<th10::ChainElem *>(drawCallback);
    chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(element);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    updateCallback = NULL;
    manager->MarkVmForDeletion(specialVmId9E50);
    specialVmId9E50.value = 0;
    for (int i = 0; i < 8; ++i)
    {
        const int id = valueVmIds[i].value;
        if (id != 0)
        {
            AnmVmLayerNodeView *node = manager->primaryVmListHead;
            while (node != NULL && static_cast<AnmVmView *>(node->owner)->id != id)
                node = node->next;
            if (node == NULL)
            {
                node = manager->secondaryVmListHead;
                while (node != NULL && static_cast<AnmVmView *>(node->owner)->id != id)
                    node = node->next;
            }
            if (node != NULL)
            {
                AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
                if (vm != NULL)
                {
                    vm->flags35C |= 0x04000000u;
                    if (vm->layerNode.previous == NULL)
                    {
                        AnmVmLayerNodeView *child = vm->layerNode.next;
                        while (child != NULL)
                        {
                            static_cast<AnmVmView *>(child->owner)->flags35C |= 0x04000000u;
                            child = child->next;
                        }
                    }
                }
            }
        }
        valueVmIds[i].value = 0;
    }
    manager->MarkLoadedVmsForDeletion(frontAnm);
    g_GuiView = NULL;
}
// GUI lifecycle end.

// GUI creation family: ctor413810 (origin indeterminate), Initialize413980,
// LoadData413A20 and factory414830. Keep target failure/publication order.
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
extern void *g_ScriptViewerStageRoute;
extern char g_StageFilePath[];
extern int __fastcall GuiDrawCallback(void *owner);

GuiView::GuiView()
{
    memset(this, 0, sizeof(*this));
    ownerFlags |= 2;
    g_GuiView = this;
}

int GuiView::LoadData()
{
    int capturedStage;
    const unsigned char *route = static_cast<const unsigned char *>(g_ScriptViewerStageRoute);
    stageUiAnm = g_AnmRenderManagerView->PreloadAnm(28, *reinterpret_cast<const char *const *>(route + 0x20));
    if (!stageUiAnm)
        goto failed;
    if (g_GuiRetainedMessageFile491BE8)
    {
        messageFile = g_GuiRetainedMessageFile491BE8;
        g_GuiRetainedMessageFile491BE8 = NULL;
    }
    else
    {
        route = static_cast<const unsigned char *>(g_ScriptViewerStageRoute);
        const int index = *reinterpret_cast<const int *>(g_GameStageState + 0x28);
        const char *filename = reinterpret_cast<const char *const *>(route + 0x18)[index];
        g_StageFilePath[0] = 0;
        strcat(g_StageFilePath, filename);
        messageFile = FileSystem::OpenFile(g_StageFilePath, NULL, 0);
        if (!messageFile)
            goto failed;
    }
    if (!(frameTimer.flags & 1))
    {
        frameTimer.current = 0;
        frameTimer.previous = -999999;
        frameTimer.subframe = 0.0f;
        frameTimer.scale = &g_AnmGameSpeed;
        frameTimer.flags |= 1;
    }
    frameTimer.current = 0;
    frameTimer.subframe = 0.0f;
    frameTimer.previous = -1;
    capturedStage = *reinterpret_cast<const int *>(g_GameStageState + 4);
    spellSeconds = -1;
    previousSpellSeconds = -1;
    value9E78 = capturedStage;
    return 0;
failed:
    th10::g_GameErrorContext.Log("\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
    return -1;
}

int GuiView::Initialize()
{
    frontAnm = g_AnmRenderManagerView->PreloadAnm(6, "front.anm");
    if (!frontAnm)
    {
        th10::g_GameErrorContext.Log("\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    if (LoadData()) return -1;
    th10::ChainElem *task = th10::Chain::AllocElem(GuiUpdateCallback);
    task->flags &= ~2u;
    th10::Chain *chain = th10::g_Chain;
    task->arg = this;
    chain->AddToCalcChain(task, 0x18);
    updateCallback = task;
    task = th10::Chain::AllocElem(GuiDrawCallback);
    task->flags &= ~2u;
    chain = th10::g_Chain;
    task->arg = this;
    chain->AddToDrawChain(task, 0x2b);
    drawCallback = task;
    return 0;
}

void *ScriptViewerCreateGui()
{
    GuiView *gui = new GuiView;
    if (gui->Initialize())
    {
        delete gui;
        return NULL;
    }
    return gui;
}
