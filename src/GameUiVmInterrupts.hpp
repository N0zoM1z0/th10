#pragma once
#include <stddef.h>

// The original owner class is not identified. Its ANM resource at +0x3C8
// and transition state at +0x6FC are target-proven; keep this view separate
// from MainSupervisorView.
struct AnmLoadedView;
struct AnmFloat3View;
struct GameUiVmTransitionView
{
    unsigned char unknown000[0x3c8];
    AnmLoadedView *transitionAnm;
    unsigned char unknown3CC[0x330];
    int transitionState;

    void Initialize(const AnmFloat3View *position);
};
typedef char GameUiVmTransitionResourcesAndState[
    (offsetof(GameUiVmTransitionView, transitionAnm) == 0x3c8 &&
     offsetof(GameUiVmTransitionView, transitionState) == 0x6fc) ? 1 : -1];

void __stdcall GameUiTransitionInitializeSecondary(
    GameUiVmTransitionView *owner, const AnmFloat3View *position);
void __stdcall GameUiSendVmInterrupt1(GameUiVmTransitionView *owner);
void __stdcall GameUiSendVmInterrupt2(GameUiVmTransitionView *owner);
