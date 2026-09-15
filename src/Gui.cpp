#include "Gui.hpp"

#include <math.h>
#include <stdlib.h>


// These views expose only fields read by the two reviewed GUI cores.  The
// containing Player, Enemy and game-state owners remain separate campaigns.
struct GuiPlayerView
{
    unsigned char unknown000[0x10];
    AnmLoadedView *portraitAnm;
    unsigned char unknown014[0x3ac];
    float positionX;
    float positionY;
};

struct GuiEnemyView
{
    unsigned char unknown000[0x1068];
    float positionX;
    unsigned char unknown106C[0x1390];
    int life;
    int maximumLife;
    int gaugeThresholdValue;
    unsigned char unknown2408[0x78];
    unsigned int flags;
};

struct GuiEnemyManagerView
{
    unsigned char unknown000[0x10];
    GuiEnemyView *activeBoss;
    unsigned char unknown014[0x24];
    AnmLoadedView *portraitAnm;
};

struct GuiGameOwnerView
{
    unsigned char unknown000[0x378c];
    unsigned int flags378C;
};

struct GuiModeOwnerView
{
    unsigned char unknown000[0x10];
    int state;
};

struct GuiScoreView
{
    int unknown00;
    int score;

    void Add(int amount)
    {
        score += amount / 10;
        if (score > 999999999)
            score = 999999999;
    }
};

typedef char GuiPlayerPositionAt3C0[
    (offsetof(GuiPlayerView, positionX) == 0x3c0) ? 1 : -1];
typedef char GuiEnemyFieldsAt1068[
    (offsetof(GuiEnemyView, positionX) == 0x1068 &&
     offsetof(GuiEnemyView, life) == 0x23fc &&
     offsetof(GuiEnemyView, maximumLife) == 0x2400 &&
     offsetof(GuiEnemyView, gaugeThresholdValue) == 0x2404 &&
     offsetof(GuiEnemyView, flags) == 0x2480) ? 1 : -1];
typedef char GuiEnemyManagerFieldsAt10[
    (offsetof(GuiEnemyManagerView, activeBoss) == 0x10 &&
     offsetof(GuiEnemyManagerView, portraitAnm) == 0x38) ? 1 : -1];


extern GuiPlayerView *g_GuiPlayer;
extern GuiEnemyManagerView *g_GuiEnemyManager;
extern GuiGameOwnerView *g_GuiGameOwner;
extern GuiModeOwnerView *g_GuiModeOwner;
extern unsigned char *g_GuiPersistentData;

extern GuiScoreView g_GuiScore;
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

extern char *GuiDecryptMessageText(const unsigned char *encrypted);
extern void GuiPlaySound(int soundId);
extern void GuiSetGameMode(int mode);
extern void GuiBeginStageTransition(int kind);
extern void GuiCreateScreenEffect(
    int type, int duration, int color, int p3, int p4);
extern void GuiRecordStageEvent(void *owner, int value);
extern void GuiCreateRandomStageEffect(float magnitude);


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

static int CreateGuiVm(AnmLoadedView *loaded, int scriptIndex)
{
    if (loaded == NULL)
        return 0;
    return loaded->CreateVmVariant0(scriptIndex, GUI_VM_RENDER_LAYER).value;
}

static void InterruptVm(AnmVmIdView *id, short interrupt)
{
    if (id->value != 0)
        g_AnmRenderManagerView->SetVmPendingInterrupt(id->value, interrupt);
}

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

static int FindChildVmId(
    AnmVmIdView *parentId, short activeSpriteIndex)
{
    AnmVmView *parent = ResolveVm(parentId);
    if (parent == NULL)
        return 0;

    AnmVmLayerNodeView *node = &parent->layerNode;
    while (node != NULL)
    {
        AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
        if (vm != NULL && vm->activeSpriteIndex == activeSpriteIndex)
            return vm->id;
        node = node->next;
    }
    return 0;
}

static void SetChildSprite(
    AnmVmIdView *parentId, short childSpriteIndex,
    AnmLoadedView *spriteOwner, int spriteIndex)
{
    const int childId = FindChildVmId(parentId, childSpriteIndex);
    AnmVmView *child = g_AnmRenderManagerView->FindVm(childId);
    if (child != NULL && spriteOwner != NULL)
        spriteOwner->SetSprite(child, spriteIndex);
}

static void DrawMessageText(
    GuiMessageVmView *message, AnmVmIdView *lineId,
    const char *text)
{
    AnmVmView *line = ResolveVm(lineId);
    if (line != NULL)
    {
        g_AnmRenderManagerView->DrawTextLeft(
            line, message->textColors[message->activeSide], text);
    }
}

static void SetMessageSide(GuiMessageVmView *message, int side)
{
    if (side == 0)
    {
        InterruptVm(&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 3);
        InterruptVm(&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 2);
    }
    else
    {
        InterruptVm(&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 3);
        InterruptVm(&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 2);
    }

    message->activeSide = side;
    g_AnmRenderManagerView->SetVmPosition(
        message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value,
        &message->textPositions[side]);
    g_AnmRenderManagerView->SetVmPosition(
        message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value,
        &message->textPositions[1 - side]);
    message->alternatingLine = 0;
}

static void SetPlayerPortraitSprites(
    GuiMessageVmView *message, int baseSprite)
{
    const int firstOffset = g_GuiCharacter == 0 ? 0x34 : 0x2d;
    const int secondOffset = g_GuiCharacter == 0 ? 0x3c : 0x35;
    const int child0 = FindChildVmId(
        &message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 0x17);
    const int child1 = FindChildVmId(
        &message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 0x1a);
    AnmVmView *vm0 = g_AnmRenderManagerView->FindVm(child0);
    AnmVmView *vm1 = g_AnmRenderManagerView->FindVm(child1);
    if (vm0 != NULL && vm0->anmFile != NULL)
        vm0->anmFile->SetSprite(vm0, baseSprite + firstOffset);
    if (vm1 != NULL && vm1->anmFile != NULL)
        vm1->anmFile->SetSprite(vm1, baseSprite + secondOffset);
}

static void SetEnemyPortraitSprites(
    GuiMessageVmView *message, int baseSprite)
{
    static const unsigned char spriteOffsets[7][2] = {
        {0x0f, 0x17}, {0x15, 0x1e}, {0x22, 0x2b}, {0x25, 0x2e},
        {0x16, 0x1f}, {0x30, 0x39}, {0x44, 0x4d}
    };
    static const unsigned char childIndices[7][2] = {
        {0x18, 0x1b}, {0x18, 0x1b}, {0x18, 0x1b}, {0x18, 0x1b},
        {0x18, 0x1b}, {0x1c, 0x1f}, {0x21, 0x24}
    };

    if (g_GuiStage < 1 || g_GuiStage > 7 || g_GuiEnemyManager == NULL)
        return;

    int row = g_GuiStage - 1;
    int offset0 = spriteOffsets[row][0];
    int offset1 = spriteOffsets[row][1];
    int child0 = childIndices[row][0];
    int child1 = childIndices[row][1];
    if (g_GuiStage == 7 && message->messageIndex >= 1)
    {
        offset0 = 0x2e;
        offset1 = 0x37;
        child0 = 0x18;
        child1 = 0x1b;
    }

    SetChildSprite(
        &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT],
        static_cast<short>(child0), g_GuiEnemyManager->portraitAnm,
        baseSprite + offset0);
    SetChildSprite(
        &message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT],
        static_cast<short>(child1), g_GuiEnemyManager->portraitAnm,
        baseSprite + offset1);
}

static void AddStageClearScore()
{
    static const int powerBonuses[5] = {
        20000000, 25000000, 35000000, 40000000, 40000000
    };
    static const int itemBonuses[5] = {
        100000, 100000, 200000, 300000, 400000
    };

    g_GuiScore.Add(g_GuiCounter4C4C * 1000);
    if (g_GuiDifficulty >= 0 && g_GuiDifficulty < 5)
    {
        g_GuiScore.Add(g_GuiPowerLevel * powerBonuses[g_GuiDifficulty]);
        g_GuiScore.Add(static_cast<int>(g_GuiPower) * itemBonuses[g_GuiDifficulty]);
    }
}

static void CompleteStageFromMessage()
{
    if (g_GuiDifficulty != 4 && g_GuiPersistentData != NULL)
    {
        const int shot = g_GuiShotType + g_GuiCharacter * 3;
        g_GuiPersistentData[shot * 0x437c + 0x4e1 +
            (g_GuiStage + g_GuiDifficulty * 6) * 8] = 1;
        g_GuiPersistentData[shot * 0x437c +
            (g_GuiStage + (g_GuiDifficulty * 3 + 0x4e) * 2) * 8] = 1;
    }

    if ((g_GuiGameFlags & 0x10) != 0)
    {
        GuiBeginStageTransition(0);
        return;
    }

    if (g_GuiStage == 6 || g_GuiStage == 7)
    {
        if (g_GuiView != NULL)
            g_GuiView->guiFlags |= 0x20;
        AddStageClearScore();
        if (g_GuiModeOwner != NULL && g_GuiModeOwner->state == 1)
        {
            GuiSetGameMode(4);
            return;
        }

        if (g_GuiStage == 6)
            GuiCreateScreenEffect(5, 120, 0, 0, 0);
        else
            GuiBeginStageTransition(0);

        if (g_GuiView != NULL)
        {
            g_GuiView->endingCounter = 0;
            g_GuiView->guiFlags |= 0x10;
        }
    }
    else if (g_GuiView != NULL)
    {
        DeleteVm(&g_GuiView->specialVmId9E14);
        g_GuiView->specialVmId9E14.value =
            CreateGuiVm(g_GuiView->frontAnm, 0x4c);
        GuiSetGameMode(0x0b);
    }
}

static int BossGaugeScript()
{
    int script = 0x85;
    switch (g_GuiStage)
    {
    case 1:
        if (g_GuiMessageState > 0x17)
            script = 0x86;
        break;
    case 2:
        if (g_GuiMessageState < 0x18)
            return -1;
        script = 0x87;
        break;
    case 3:
        if (g_GuiMessageState < 0x18)
            return -1;
        script = 0x88;
        break;
    case 4:
        script = g_GuiMessageState > 0x17 ? 0x8a : 0x89;
        break;
    case 5:
        script = 0x8b;
        break;
    case 6:
        script = 0x8c;
        break;
    case 7:
        script = g_GuiMessageState > 0x17 ? 0x8e : 0x8d;
        break;
    }
    return script;
}

static void SetBossGaugeInterrupt(GuiView *gui, short interrupt)
{
    int i;
    for (i = 0; i < gui->bossGaugeSegmentCount; ++i)
        InterruptVm(&gui->bossGaugeSegmentVmIds[i], interrupt);
    InterruptVm(&gui->bossGaugeVmId, interrupt);
}

static void UpdateBossGauge(GuiView *gui)
{
    GuiEnemyView *boss = g_GuiEnemyManager == NULL
        ? NULL : g_GuiEnemyManager->activeBoss;
    if (boss == NULL || gui->message != NULL)
    {
        InterruptVm(&gui->bossGaugeVmId, 1);
        gui->bossGaugeVmId.value = 0;
        gui->bossGaugeDisplayed = 0.0f;
        int i;
        for (i = 0; i < 4; ++i)
            gui->bossGaugeValues[i].value = 0.0f;
        return;
    }

    gui->bossLife = boss->life;
    gui->bossGaugeTarget = static_cast<float>(boss->life) /
        static_cast<float>(boss->maximumLife);
    if (gui->bossGaugeDisplayed < gui->bossGaugeTarget)
        gui->bossGaugeDisplayed += 0.025f;
    if (gui->bossGaugeDisplayed > gui->bossGaugeTarget)
        gui->bossGaugeDisplayed = gui->bossGaugeTarget;

    if (g_GuiPlayer != NULL)
    {
        if ((gui->guiFlags & 8) == 0 &&
            g_GuiPlayer->positionY <= 64.0f &&
            g_GuiPlayer->positionX < -64.0f)
        {
            SetBossGaugeInterrupt(gui, 3);
            gui->guiFlags |= 8;
        }
        else if ((gui->guiFlags & 8) != 0 &&
                 (g_GuiPlayer->positionY >= 80.0f ||
                  g_GuiPlayer->positionX > 0.0f))
        {
            SetBossGaugeInterrupt(gui, 2);
            gui->guiFlags &= ~8u;
        }
    }

    if (gui->bossGaugeVmId.value == 0)
    {
        const int script = BossGaugeScript();
        if (script >= 0)
            gui->bossGaugeVmId.value = CreateGuiVm(gui->frontAnm, script);
    }

    int i;
    for (i = 0; i < 10; ++i)
    {
        if (i < gui->bossGaugeSegmentCount)
        {
            if (gui->bossGaugeSegmentVmIds[i].value == 0)
            {
                gui->bossGaugeSegmentVmIds[i].value =
                    CreateGuiVm(gui->frontAnm, i + 0x5b);
            }
        }
        else
        {
            InterruptVm(&gui->bossGaugeSegmentVmIds[i], 1);
            gui->bossGaugeSegmentVmIds[i].value = 0;
        }
    }
}

static void FinishMessage(GuiView *gui)
{
    if (gui->message == NULL)
        return;
    int i;
    for (i = 0; i < GUI_MESSAGE_VM_COUNT; ++i)
        DeleteVm(&gui->message->vmIds[i]);
    free(gui->message);
    gui->message = NULL;
}

static void UpdateSpellAndBossPosition(GuiView *gui)
{
    GuiEnemyView *boss = g_GuiEnemyManager == NULL
        ? NULL : g_GuiEnemyManager->activeBoss;
    if (boss == NULL)
        return;

    if (gui->spellSeconds >= 0 && gui->message == NULL)
    {
        if (gui->spellSeconds < gui->previousSpellSeconds)
        {
            if (gui->spellSeconds < 6)
            {
                gui->displayVms4[0].pendingInterrupt = 9;
                gui->displayVms4[1].pendingInterrupt = 9;
                GuiPlaySound(0);
            }
            else if (gui->spellSeconds < 11)
            {
                gui->displayVms4[0].pendingInterrupt = 8;
                gui->displayVms4[1].pendingInterrupt = 8;
                GuiPlaySound(0);
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
        gui->previousSpellSeconds = gui->spellSeconds;
    }

    if ((boss->flags & 0x11) != 0)
        return;

    unsigned int phase = (gui->guiFlags >> 1) & 3;
    const bool alternate =
        g_GuiGameOwner != NULL && (g_GuiGameOwner->flags378C & 1) != 0;
    static const int normalThresholds[4] = {700, 400, 200, 200};
    static const int alternateThresholds[4] = {2000, 1000, 400, 400};
    const int threshold = alternate
        ? alternateThresholds[phase] : normalThresholds[phase];
    const bool crossesThreshold = phase < 3
        ? boss->gaugeThresholdValue < threshold
        : boss->gaugeThresholdValue > threshold;
    if (crossesThreshold)
    {
        static const short interrupts[4] = {7, 8, 9, 10};
        gui->bossPositionVm.pendingInterrupt = interrupts[phase];
        if (phase == 0)
            gui->guiFlags = (gui->guiFlags & ~4u) | 2;
        else if (phase == 1)
            gui->guiFlags = (gui->guiFlags & ~2u) | 4;
        else if (phase == 2)
            gui->guiFlags |= 6;
        else
            gui->guiFlags &= ~6u;
    }

    AnmRenderManagerView::ExecuteScript(&gui->bossPositionVm);
    gui->bossPositionVm.positionOffset.x = boss->positionX + 224.0f;
    gui->bossPositionVm.positionOffset.y = 480.0f;
    if (g_GuiPlayer == NULL)
        return;

    const double distance = fabs(
        static_cast<double>(boss->positionX - g_GuiPlayer->positionX));
    if (distance < 64.0)
        gui->bossPositionVm.primaryColor.alpha =
            static_cast<unsigned char>(
                64 - static_cast<int>(distance * -2.984375f));
    else
        gui->bossPositionVm.primaryColor.alpha = 0xff;
    if (boss->positionX < -192.0f || boss->positionX > 192.0f)
        gui->bossPositionVm.primaryColor.alpha = 0;
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
        message->instructionTimer.SetCurrent(message->currentInstruction->time);
    }

    while (message->currentInstruction->time <=
           message->instructionTimer.current)
    {
        GuiMessageInstructionView *instruction = message->currentInstruction;
        const int argument = instruction->IntArgument();
        switch (instruction->opcode)
        {
        case GUI_MSG_END:
            if (g_GuiMessageState != 0)
                g_GuiMessageStateAux = 0;
            g_GuiMessageState = 0;
            return -1;

        case GUI_MSG_SHOW_PLAYER_PORTRAIT:
            if (g_GuiPlayer != NULL &&
                (g_GuiCharacter == 0 || g_GuiCharacter == 1))
            {
                message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value =
                    CreateGuiVm(g_GuiPlayer->portraitAnm, 0x1d);
            }
            break;

        case GUI_MSG_SHOW_ENEMY_PORTRAIT:
            if (g_GuiEnemyManager != NULL &&
                (g_GuiCharacter == 0 || g_GuiCharacter == 1))
            {
                AnmLoadedView *anm = g_GuiEnemyManager->portraitAnm;
                int script = 0x1e;
                if (g_GuiStage == 6)
                    script = 0x20;
                else if (g_GuiStage == 7 && message->messageIndex < 1)
                    script = 0x25;
                message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value =
                    CreateGuiVm(anm, script);
            }
            break;

        case GUI_MSG_SHOW_TEXT_BOX:
            if (g_GuiView != NULL)
            {
                message->vmIds[GUI_MESSAGE_TEXT_BOX].value =
                    CreateGuiVm(g_GuiView->frontAnm, 0x5a);
            }
            break;

        case GUI_MSG_HIDE_PLAYER_PORTRAIT:
            InterruptVm(&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 1);
            message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value = 0;
            break;

        case GUI_MSG_HIDE_ENEMY_PORTRAIT:
            InterruptVm(&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 1);
            message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = 0;
            InterruptVm(&message->vmIds[GUI_MESSAGE_ENEMY_NAME], 1);
            break;

        case GUI_MSG_HIDE_TEXT:
            InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_BOX], 1);
            InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_LINE_0], 1);
            InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1], 1);
            break;

        case GUI_MSG_SET_SIDE_0:
            SetMessageSide(message, 0);
            break;

        case GUI_MSG_SET_SIDE_1:
            SetMessageSide(message, 1);
            break;

        case GUI_MSG_SET_SKIPPABLE:
            message->flags ^= (static_cast<unsigned int>(
                instruction->payload[0]) ^ message->flags) & 1;
            break;

        case GUI_MSG_WAIT:
            if (message->waitTimer.current < 1)
                message->waitTimer.SetCurrent(argument);
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
                GuiPlaySound(0);
                message->waitTimer.SetCurrent(0);
                message->alternatingLine = 0;
            }
            break;

        case GUI_MSG_INPUT_COOLDOWN:
            message->inputCooldown = 1;
            break;

        case GUI_MSG_PLAYER_PORTRAIT_SPRITES:
            SetPlayerPortraitSprites(message, argument);
            break;

        case GUI_MSG_ENEMY_PORTRAIT_SPRITES:
            SetEnemyPortraitSprites(message, argument);
            break;

        case GUI_MSG_TEXT_LINE_0:
        case GUI_MSG_TEXT_LINE_1:
        {
            const int line = instruction->opcode - GUI_MSG_TEXT_LINE_0;
            char *text = GuiDecryptMessageText(instruction->payload);
            DrawMessageText(
                message, &message->vmIds[GUI_MESSAGE_TEXT_LINE_0 + line],
                text);
            InterruptVm(
                &message->vmIds[GUI_MESSAGE_TEXT_LINE_0 + line], 2);
            break;
        }

        case GUI_MSG_TEXT_LINE_ALTERNATING:
            if (message->alternatingLine == 0)
            {
                DrawMessageText(
                    message, &message->vmIds[GUI_MESSAGE_TEXT_LINE_0], " ");
                DrawMessageText(
                    message, &message->vmIds[GUI_MESSAGE_TEXT_LINE_1], " ");
                DrawMessageText(
                    message, &message->vmIds[GUI_MESSAGE_TEXT_LINE_0],
                    GuiDecryptMessageText(instruction->payload));
                InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_LINE_0], 2);
                InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1], 3);
                ++message->alternatingLine;
            }
            else
            {
                DrawMessageText(
                    message, &message->vmIds[GUI_MESSAGE_TEXT_LINE_1],
                    GuiDecryptMessageText(instruction->payload));
                InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1], 2);
                message->alternatingLine = 0;
            }
            break;

        case GUI_MSG_HIDE_TEXT_LINES:
            InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_LINE_0], 3);
            InterruptVm(&message->vmIds[GUI_MESSAGE_TEXT_LINE_1], 3);
            break;

        case GUI_MSG_EVENT_18:
            GuiRecordStageEvent(g_GuiPersistentData, 1);
            if (g_GuiView != NULL)
                CreateGuiVm(g_GuiView->stageUiAnm, 2);
            break;

        case GUI_MSG_ENEMY_NAME:
            if (g_GuiEnemyManager != NULL && g_GuiStage >= 1 && g_GuiStage <= 7)
            {
                static const unsigned char scripts[7] = {
                    0x0d, 0x10, 0x14, 0x15, 0x0e, 0x23, 0x1d
                };
                message->vmIds[GUI_MESSAGE_ENEMY_NAME].value =
                    CreateGuiVm(
                        g_GuiEnemyManager->portraitAnm,
                        scripts[g_GuiStage - 1]);
            }
            break;

        case GUI_MSG_COMPLETE_STAGE:
            CompleteStageFromMessage();
            break;

        case GUI_MSG_EVENT_21:
            GuiCreateRandomStageEffect(g_GuiStage == 6 ? 8.0f : 2.0f);
            break;

        case GUI_MSG_PLAYER_PORTRAIT_EXIT:
            InterruptVm(&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT], 7);
            break;

        case GUI_MSG_ENEMY_PORTRAIT_EXIT:
            InterruptVm(&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT], 7);
            break;
        }

        message->currentInstruction = instruction->Next();
    }

    message->instructionTimer.Tick();
    return 0;
}


// Target 0x00414900-0x004157FB is the central stage-HUD updater.  It advances
// 22 embedded display VMs, maintains the player-side panel, power digits, boss
// gauge/segments, the heap-owned message VM, spell timer and boss marker, and
// finally advances the GUI frame timer.
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

    if (g_GuiPlayer != NULL)
    {
        if ((gui->guiFlags & 1) == 0 &&
            g_GuiPlayer->positionY > 416.0f &&
            g_GuiPlayer->positionX < -128.0)
        {
            for (i = 0; i < 7; ++i)
                gui->displayVms5[i].pendingInterrupt = 3;
            gui->guiFlags |= 1;
        }
        else if ((gui->guiFlags & 1) != 0 &&
                 (g_GuiPlayer->positionY < 400.0f ||
                  g_GuiPlayer->positionX > -112.0))
        {
            for (i = 0; i < 7; ++i)
                gui->displayVms5[i].pendingInterrupt = 2;
            gui->guiFlags &= ~1u;
        }
    }

    AnmRenderManagerView::ExecuteScript(&gui->displayVms5[0]);
    int value = g_GuiCounter4C4C;
    int divisor = 10000;
    for (i = 1; i < 6; ++i)
    {
        const int digit = value / divisor;
        value %= divisor;
        if (gui->frontAnm != NULL)
            gui->frontAnm->SetSprite(&gui->displayVms5[i], digit + 0x1e);
        divisor /= 10;
        AnmRenderManagerView::ExecuteScript(&gui->displayVms5[i]);
    }
    AnmRenderManagerView::ExecuteScript(&gui->displayVms5[6]);

    UpdateBossGauge(gui);

    if (gui->message != NULL)
    {
        if (GuiMessageVmView::Run(gui->message) == 0)
            gui->message->lifetimeTimer.Tick();
        else
            FinishMessage(gui);
    }

    UpdateSpellAndBossPosition(gui);
    gui->frameTimer.Tick();
    return 1;
}
