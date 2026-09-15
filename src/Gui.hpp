#ifndef TH10_RECONSTRUCTION_GUI_HPP
#define TH10_RECONSTRUCTION_GUI_HPP

#include "AnmManager.hpp"

#include <stddef.h>


// TH10 message records are variable sized.  The byte at +3 measures the
// payload beginning at +4; the next record is therefore payload + size.
struct GuiMessageInstructionView
{
    unsigned short time;
    unsigned char opcode;
    unsigned char payloadSize;
    unsigned char payload[1];

    GuiMessageInstructionView *Next()
    {
        return reinterpret_cast<GuiMessageInstructionView *>(
            payload + payloadSize);
    }

    int IntArgument() const
    {
        return *reinterpret_cast<const int *>(payload);
    }
};

typedef char GuiMessageInstructionPayloadAt04[
    (offsetof(GuiMessageInstructionView, payload) == 0x04) ? 1 : -1];


enum GuiMessageVmIdSlot
{
    GUI_MESSAGE_PLAYER_PORTRAIT = 0,
    GUI_MESSAGE_ENEMY_PORTRAIT = 1,
    GUI_MESSAGE_TEXT_BOX = 2,
    GUI_MESSAGE_TEXT_LINE_0 = 3,
    GUI_MESSAGE_TEXT_LINE_1 = 4,
    GUI_MESSAGE_ENEMY_NAME = 5,
    GUI_MESSAGE_VM_COUNT = 6
};


// Target 0x00415B00 constructs this exact 0x90-byte message interpreter.
// The three timers and every named field below are independently consumed by
// Run at 0x00415E90 or by the owning GUI updater at 0x00414900.
struct GuiMessageVmView
{
    int messageIndex;
    AnmVmTimerView lifetimeTimer;
    AnmVmTimerView instructionTimer;
    AnmVmTimerView waitTimer;
    AnmVmIdView vmIds[GUI_MESSAGE_VM_COUNT];
    int unknown58;
    GuiMessageInstructionView *currentInstruction;
    AnmFloat3View textPositions[2];
    int inputCooldown;
    unsigned int flags;
    int alternatingLine;
    int activeSide;
    unsigned int textColors[2];

    static int __stdcall Run(GuiMessageVmView *message);
};

typedef char GuiMessageVmSizeIs90[
    (sizeof(GuiMessageVmView) == 0x90) ? 1 : -1];
typedef char GuiMessageVmTimersAt04[
    (offsetof(GuiMessageVmView, lifetimeTimer) == 0x04 &&
     offsetof(GuiMessageVmView, instructionTimer) == 0x18 &&
     offsetof(GuiMessageVmView, waitTimer) == 0x2c) ? 1 : -1];
typedef char GuiMessageVmIdsAt40[
    (offsetof(GuiMessageVmView, vmIds) == 0x40) ? 1 : -1];
typedef char GuiMessageVmInstructionAt5C[
    (offsetof(GuiMessageVmView, currentInstruction) == 0x5c) ? 1 : -1];
typedef char GuiMessageVmPositionsAt60[
    (offsetof(GuiMessageVmView, textPositions) == 0x60) ? 1 : -1];
typedef char GuiMessageVmControlAt78[
    (offsetof(GuiMessageVmView, inputCooldown) == 0x78 &&
     offsetof(GuiMessageVmView, flags) == 0x7c &&
     offsetof(GuiMessageVmView, textColors) == 0x88) ? 1 : -1];


struct GuiBossGaugeValueView
{
    float value;
    float unknown04;
};


// Exact-size maintained view of the TH10 stage GUI owner.  Construction at
// 0x00413810 proves the six embedded VM arrays and the separately initialized
// VM at +0x9A48.  Unknown scalar fields retain offset-based names.
struct GuiView
{
    unsigned int ownerFlags;
    int unknown04;
    void *updateCallback;
    void *drawCallback;
    AnmVmView displayVms0[10];
    AnmVmView displayVms1[10];
    AnmVmView displayVms2[9];
    AnmVmView displayVms3[4];
    AnmVmView displayVms4[2];
    AnmVmView displayVms5[7];
    AnmVmView bossPositionVm;
    AnmVmIdView valueVmIds[8];
    AnmVmIdView specialVmId9E14;
    AnmVmIdView specialVmId9E18;
    int unknown9E1C;
    int unknown9E20;
    AnmVmIdView bossGaugeVmId;
    AnmVmIdView bossGaugeSegmentVmIds[10];
    AnmVmIdView specialVmId9E50;
    AnmVmIdView specialVmId9E54;
    AnmVmIdView specialVmId9E58;
    AnmVmIdView specialVmId9E5C;
    AnmVmTimerView frameTimer;
    int value9E74;
    int value9E78;
    int value9E7C;
    AnmLoadedView *stageUiAnm;
    float bossGaugeDisplayed;
    float bossGaugeTarget;
    int bossLife;
    int bossGaugeSegmentCount;
    GuiBossGaugeValueView bossGaugeValues[4];
    unsigned int guiFlags;
    GuiMessageVmView *message;
    void *messageFile;
    int spellSeconds;
    int previousSpellSeconds;
    AnmLoadedView *frontAnm;
    int endingCounter;

    static int __stdcall UpdateStageElements(GuiView *gui);
};

typedef char GuiViewSizeIs9ED0[
    (sizeof(GuiView) == 0x9ed0) ? 1 : -1];
typedef char GuiViewVmArraysAt10[
    (offsetof(GuiView, displayVms0) == 0x0010 &&
     offsetof(GuiView, displayVms1) == 0x24c8 &&
     offsetof(GuiView, displayVms2) == 0x4980 &&
     offsetof(GuiView, displayVms3) == 0x6a8c &&
     offsetof(GuiView, displayVms4) == 0x793c &&
     offsetof(GuiView, displayVms5) == 0x8094 &&
     offsetof(GuiView, bossPositionVm) == 0x9a48) ? 1 : -1];
typedef char GuiViewVmIdsAt9DF4[
    (offsetof(GuiView, valueVmIds) == 0x9df4 &&
     offsetof(GuiView, bossGaugeVmId) == 0x9e24 &&
     offsetof(GuiView, bossGaugeSegmentVmIds) == 0x9e28) ? 1 : -1];
typedef char GuiViewTailAt9E60[
    (offsetof(GuiView, frameTimer) == 0x9e60 &&
     offsetof(GuiView, stageUiAnm) == 0x9e80 &&
     offsetof(GuiView, bossGaugeDisplayed) == 0x9e84 &&
     offsetof(GuiView, bossGaugeValues) == 0x9e94 &&
     offsetof(GuiView, guiFlags) == 0x9eb4 &&
     offsetof(GuiView, message) == 0x9eb8 &&
     offsetof(GuiView, frontAnm) == 0x9ec8 &&
     offsetof(GuiView, endingCounter) == 0x9ecc) ? 1 : -1];


extern GuiView *g_GuiView;

#endif
