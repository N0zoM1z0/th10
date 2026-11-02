#include "InputPolling.hpp"
#include <string.h>

extern "C" __declspec(dllimport) int __stdcall GetKeyboardState(
    unsigned char *state);

// Source-model view for the original private single-buffer keyboard fetch.
// An unused receiver can arise from a real nonstatic member; the 256-byte
// output is the register-passed second fastcall argument in EDX.
struct InputKeyboardSnapshotView
{
    int __fastcall ReadKeyboardState(unsigned char *state);
};

// TH10 0x44B010: use Win32 keys or DirectInput depending on supervisor flags.
int __fastcall InputKeyboardSnapshotView::ReadKeyboardState(
    unsigned char *state)
{
    memset(state, 0, 256);
    if (g_InputEnabled == 0)
        return 0;
    if ((g_FrontEndSupervisorFlags & 0x200u) == 0)
    {
        GetKeyboardState(state);
        return 0;
    }
    int result = g_InputKeyboardDevice->vtable->GetDeviceState(
        g_InputKeyboardDevice, 256, state);
    if (result == static_cast<int>(0x8007001e))
    {
        g_InputKeyboardDevice->vtable->Acquire(g_InputKeyboardDevice);
        return 1;
    }
    if (result != 0)
        g_InputKeyboardDevice->vtable->Acquire(g_InputKeyboardDevice);
    return 1;
}
