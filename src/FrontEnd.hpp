#ifndef TH10_RECONSTRUCTION_FRONT_END_HPP
#define TH10_RECONSTRUCTION_FRONT_END_HPP

#include "AnmManager.hpp"

#include <stddef.h>


// The three target cursor helpers at 0x0044BE20/70/A0 establish this complete
// 0xD8-byte layout. Their original type name and source owner remain unknown.
struct FrontEndCursorView
{
    int current;
    int previous;
    int count;
    int savedCurrent[16];
    int savedCount[16];
    int saveDepth;
    int disabledEntries[16];
    int wraps;
    int disabledEntryCount;

    void SetCurrent(int value)
    {
        if (count == 0)
            current = 0;
        else if (count <= value)
            current = count - 1;
        else
            current = value;
    }

    void DisableEntry(int value)
    {
        disabledEntries[disabledEntryCount] = value;
        ++disabledEntryCount;
    }

    int Move(int amount);
    void Push();
    void Pop();
};

typedef char FrontEndCursorSizeIsD8[
    (sizeof(FrontEndCursorView) == 0xd8) ? 1 : -1];
typedef char FrontEndCursorSaveStateAt0C[
    (offsetof(FrontEndCursorView, savedCurrent) == 0x0c &&
     offsetof(FrontEndCursorView, savedCount) == 0x4c &&
     offsetof(FrontEndCursorView, saveDepth) == 0x8c) ? 1 : -1];
typedef char FrontEndCursorDisabledAt90[
    (offsetof(FrontEndCursorView, disabledEntries) == 0x90 &&
     offsetof(FrontEndCursorView, wraps) == 0xd0 &&
     offsetof(FrontEndCursorView, disabledEntryCount) == 0xd4) ? 1 : -1];


struct FrontEndTransitionOwnerView
{
    int unknown000;
    unsigned int flags004;
};


enum FrontEndScreenView
{
    FRONT_END_SCREEN_INITIALIZE = 0,
    FRONT_END_SCREEN_MAIN_MENU = 1,
    FRONT_END_SCREEN_MAIN_MENU_RETURN = 2,
    FRONT_END_SCREEN_START_GAME = 3,
    FRONT_END_SCREEN_OPTIONS = 4,
    FRONT_END_SCREEN_KEY_CONFIG = 5,
    FRONT_END_SCREEN_DIFFICULTY = 6,
    FRONT_END_SCREEN_CHARACTER = 7,
    FRONT_END_SCREEN_SHOT_TYPE = 8,
    FRONT_END_SCREEN_STAGE = 9,
    FRONT_END_SCREEN_START_GAME_10 = 10,
    FRONT_END_SCREEN_REPLAY = 11,
    FRONT_END_SCREEN_PRACTICE = 12,
    FRONT_END_SCREEN_START_GAME_13 = 13,
    FRONT_END_SCREEN_MUSIC_ROOM = 14,
    FRONT_END_SCREEN_SPECIAL = 15,
    FRONT_END_SCREEN_RESULT = 16
};


enum FrontEndOptionsStateView
{
    FRONT_END_OPTIONS_INITIALIZE = 0,
    FRONT_END_OPTIONS_OPENING = 1,
    FRONT_END_OPTIONS_ACTIVE = 2,
    FRONT_END_OPTIONS_UNUSED = 3,
    FRONT_END_OPTIONS_CLOSING = 4
};


enum FrontEndKeyConfigStateView
{
    FRONT_END_KEY_CONFIG_INITIALIZE = 0,
    FRONT_END_KEY_CONFIG_OPENING = 1,
    FRONT_END_KEY_CONFIG_ACTIVE = 2,
    FRONT_END_KEY_CONFIG_UNUSED = 3,
    FRONT_END_KEY_CONFIG_CLOSING = 4
};

enum FrontEndSelectionStateView
{
    FRONT_END_SELECTION_INITIALIZE = 0,
    FRONT_END_SELECTION_OPENING = 1,
    FRONT_END_SELECTION_ACTIVE = 2,
    FRONT_END_SELECTION_CONFIRMED = 3,
    FRONT_END_SELECTION_CLOSING = 4
};


// TH10 stores all nine controller bindings as adjacent signed shorts. Target
// input masks identify the first four actions and skip. The middle directional
// names are adjacent-supported and remain provisional; all four values are
// nevertheless preserved in the target's persisted 0x12-byte mapping.
struct FrontEndControllerMappingView
{
    short shotButton;
    short bombButton;
    short focusButton;
    short menuButton;
    short upButton;
    short downButton;
    short leftButton;
    short rightButton;
    short skipButton;
};

typedef char FrontEndControllerMappingSizeIs12[
    (sizeof(FrontEndControllerMappingView) == 0x12) ? 1 : -1];


// This maintained partial view reaches the highest VM slot consumed by the
// reviewed update paths and the key-configuration scratch bindings. It does
// not claim the complete front-end allocation.
struct FrontEndControllerView
{
    unsigned char unknown000[0x010];
    FrontEndTransitionOwnerView *transitionOwner;
    AnmLoadedView *frontAnm;
    int unknown018;
    int screen;
    int screenState;
    FrontEndCursorView cursor;
    unsigned char unknown0FC[0x1b4];
    AnmVmTimerView stateTimer;
    AnmVmIdView vmIds[0x98];
    unsigned char unknown524[0x0ac];
    AnmVmIdView difficultyAuxVmId;
    unsigned char unknown5D4[0x531c];
    int savedDifficulty;
    unsigned char unknown58F4[0x0d8];
    short keyConfigBindings[5];

    int Update();
    static int __stdcall UpdateOptions(FrontEndControllerView *controller);
    void RefreshOptionsDisplay();
    static int __stdcall UpdateKeyConfig(FrontEndControllerView *controller);
    void RefreshKeyConfigDisplay();
    void AssignKeyConfigBinding(int bindingIndex, int controllerButton);
    static int __stdcall UpdateDifficulty(FrontEndControllerView *controller);
    static int __stdcall UpdateCharacter(FrontEndControllerView *controller);
    static int __stdcall UpdateShotType(FrontEndControllerView *controller);
    static int __stdcall UpdateStage(FrontEndControllerView *controller);
    int DrawStageScores();
};

typedef char FrontEndControllerStateAt1C[
    (offsetof(FrontEndControllerView, screen) == 0x1c &&
     offsetof(FrontEndControllerView, screenState) == 0x20 &&
     offsetof(FrontEndControllerView, cursor) == 0x24) ? 1 : -1];
typedef char FrontEndControllerTimerAt2B0[
    (offsetof(FrontEndControllerView, stateTimer) == 0x2b0) ? 1 : -1];
typedef char FrontEndControllerVmIdsAt2C4[
    (offsetof(FrontEndControllerView, vmIds) == 0x2c4 &&
     offsetof(FrontEndControllerView, vmIds) + sizeof(AnmVmIdView) == 0x2c8 &&
     offsetof(FrontEndControllerView, vmIds) +
         sizeof(AnmVmIdView) * 0x97 == 0x520) ? 1 : -1];
typedef char FrontEndControllerSelectionFields[
    (offsetof(FrontEndControllerView, difficultyAuxVmId) == 0x5d0 &&
     offsetof(FrontEndControllerView, savedDifficulty) == 0x58f0) ? 1 : -1];
typedef char FrontEndControllerKeyConfigBindingsAt59CC[
    (offsetof(FrontEndControllerView, keyConfigBindings) == 0x59cc) ? 1 : -1];

#endif
