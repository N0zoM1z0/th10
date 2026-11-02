#ifndef TH10_RESULT_MENU_HPP
#define TH10_RESULT_MENU_HPP
#include "FrontEnd.hpp"

// This owner constructs its VM slots only through the final memset, unlike
// AnmVmIdView's default constructor which writes a zero before the memset.
// Keep the exact four-byte representation and delegate operations to the
// existing VM-id interface without inventing an earlier initialization.
struct ResultMenuVmIdStorage
{
    ResultMenuVmIdStorage() {}
    ResultMenuVmIdStorage &operator=(AnmVmIdView id)
    {
        value = id.value;
        return *this;
    }
    operator AnmVmIdView() const { return AnmVmIdView(value); }
    void SetInterrupt(short interrupt)
    {
        reinterpret_cast<AnmVmIdView *>(this)->SetInterrupt(interrupt);
    }
    void SetFlag2() { reinterpret_cast<AnmVmIdView *>(this)->SetFlag2(); }
    void ClearFlag2() { reinterpret_cast<AnmVmIdView *>(this)->ClearFlag2(); }
    int value;
};
typedef char ResultMenuVmIdStorageSize04[(sizeof(ResultMenuVmIdStorage)==4)?1:-1];

// Target-observed prefix through the pause ANM resource; allocation/constructor remain open.
struct ResultMenuView
{
    int unknown000;
    int state;
    unsigned char unknown008[8];
    AnmVmTimerView timer;
    ScriptViewerCursorView cursor;
    ScriptViewerCursorView keyboard;
    ResultMenuVmIdStorage mainVm;
    ResultMenuVmIdStorage secondaryVm;
    ResultMenuVmIdStorage auxiliaryVm;
    int nameIndex;
    int completed;
    int scoreUnavailable;
    ReplayManager *replayFiles[25];
    unsigned char unknown250[0x64];
    char name[9];
    unsigned char unknown2BD[3];
    float savedSpeed;
    AnmLoadedView *frontAnm;

    // The original owner is allocated at 0x422360, constructed at 0x4220E0,
    // and registered on both frame chains by 0x422150.
    ResultMenuView();
    int Initialize();
};
typedef char ResultMenuTimerAt10[(offsetof(ResultMenuView, timer)==0x10)?1:-1];
typedef char ResultMenuCursorsAt24FC[(offsetof(ResultMenuView,cursor)==0x24 && offsetof(ResultMenuView,keyboard)==0xfc)?1:-1];
typedef char ResultMenuIdsAt1D4[(offsetof(ResultMenuView,mainVm)==0x1d4 && offsetof(ResultMenuView,secondaryVm)==0x1d8)?1:-1];
typedef char ResultMenuFlagsAt1E0[(offsetof(ResultMenuView,nameIndex)==0x1e0 && offsetof(ResultMenuView,completed)==0x1e4 && offsetof(ResultMenuView,scoreUnavailable)==0x1e8)?1:-1];
typedef char ResultMenuFilesAt1EC[(offsetof(ResultMenuView,replayFiles)==0x1ec)?1:-1];
typedef char ResultMenuNameAt2B4[(offsetof(ResultMenuView,name)==0x2b4 && offsetof(ResultMenuView,savedSpeed)==0x2c0)?1:-1];
typedef char ResultMenuPauseFields[(offsetof(ResultMenuView,auxiliaryVm)==0x1dc && offsetof(ResultMenuView,frontAnm)==0x2c4 && sizeof(ResultMenuView)==0x2c8)?1:-1];
// Result openings at 0x4231D0/0x423370; LTCG supplies the private EDI receiver.
void ResultMenuOpenGameOver(ResultMenuView *menu);
void GameCompleteStageTransition(ResultMenuView *menu);
void __stdcall ResultMenuUpdateResult(ResultMenuView *menu);
int ResultMenuUpdate(ResultMenuView *menu);
void __stdcall ResultMenuDrawNameEntry(ResultMenuView *menu, AnmFloat3View position);
int __stdcall ResultMenuDrawRows(ResultMenuView *menu);
int __fastcall ResultMenuDrawRowsCallback(ResultMenuView *menu);
#endif
