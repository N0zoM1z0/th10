#pragma once
#include <stddef.h>

// The caller's original parent class is not identified; only the +0x6FC
// state field is target-proven. Keep it separate from MainSupervisorView.
struct GameUiVmTransitionView
{
    unsigned char unknown000[0x6fc];
    int transitionState;
};
typedef char GameUiVmTransitionStateAt6FC[
    offsetof(GameUiVmTransitionView, transitionState) == 0x6fc ? 1 : -1];

void __stdcall GameUiSendVmInterrupt1(GameUiVmTransitionView *owner);
void __stdcall GameUiSendVmInterrupt2(GameUiVmTransitionView *owner);
