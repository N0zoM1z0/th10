#ifndef TH10_RESULT_MENU_HPP
#define TH10_RESULT_MENU_HPP
#include "FrontEnd.hpp"

// Target-observed prefix through the pause ANM resource; allocation/constructor remain open.
struct ResultMenuView
{
    int unknown000;
    int state;
    unsigned char unknown008[8];
    AnmVmTimerView timer;
    FrontEndCursorView cursor;
    FrontEndCursorView keyboard;
    AnmVmIdView mainVm;
    AnmVmIdView secondaryVm;
    AnmVmIdView auxiliaryVm;
    int nameIndex;
    int completed;
    int scoreUnavailable;
    ReplayManager *replayFiles[25];
    unsigned char unknown250[0x64];
    char name[9];
    unsigned char unknown2BD[3];
    float savedSpeed;
    AnmLoadedView *frontAnm;
};
typedef char ResultMenuTimerAt10[(offsetof(ResultMenuView, timer)==0x10)?1:-1];
typedef char ResultMenuCursorsAt24FC[(offsetof(ResultMenuView,cursor)==0x24 && offsetof(ResultMenuView,keyboard)==0xfc)?1:-1];
typedef char ResultMenuIdsAt1D4[(offsetof(ResultMenuView,mainVm)==0x1d4 && offsetof(ResultMenuView,secondaryVm)==0x1d8)?1:-1];
typedef char ResultMenuFlagsAt1E0[(offsetof(ResultMenuView,nameIndex)==0x1e0 && offsetof(ResultMenuView,completed)==0x1e4 && offsetof(ResultMenuView,scoreUnavailable)==0x1e8)?1:-1];
typedef char ResultMenuFilesAt1EC[(offsetof(ResultMenuView,replayFiles)==0x1ec)?1:-1];
typedef char ResultMenuNameAt2B4[(offsetof(ResultMenuView,name)==0x2b4 && offsetof(ResultMenuView,savedSpeed)==0x2c0)?1:-1];
typedef char ResultMenuPauseFields[(offsetof(ResultMenuView,auxiliaryVm)==0x1dc && offsetof(ResultMenuView,frontAnm)==0x2c4 && sizeof(ResultMenuView)==0x2c8)?1:-1];
// Logical interface for target 0x423370; native EDI receiver remains open.
void GameCompleteStageTransition(ResultMenuView *menu);
void __stdcall ResultMenuUpdateResult(ResultMenuView *menu);
int ResultMenuUpdate(ResultMenuView *menu);
#endif
