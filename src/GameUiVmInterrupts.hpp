#pragma once
#include <stddef.h>

// The caller's original parent class is not identified; only the +0x6FC
// state field is target-proven. Keep it separate from MainSupervisorView.
struct AnmLoadedView;
struct GameUiVmTransitionView
{
    unsigned char unknown000[0x3c8];
    AnmLoadedView *transitionAnm;
    unsigned char unknown3CC[0x330];
    int transitionState;
};
typedef char GameUiVmTransitionResourcesAndState[
    (offsetof(GameUiVmTransitionView, transitionAnm) == 0x3c8 &&
     offsetof(GameUiVmTransitionView, transitionState) == 0x6fc) ? 1 : -1];

void __stdcall GameUiTransitionInitialize(GameUiVmTransitionView *owner);
void __stdcall GameUiTransitionInitializeSecondary(GameUiVmTransitionView *owner);
void __stdcall GameUiSendVmInterrupt1(GameUiVmTransitionView *owner);
void __stdcall GameUiSendVmInterrupt2(GameUiVmTransitionView *owner);
