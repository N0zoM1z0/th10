// Local native source proposal; compiler/ABI/relocation integration is pending.
// DI normal domain: rawController == 0; WinMM has its own valid-index domain.
#include "Main.hpp" // Includes Windows after the existing Anm import-declaration guards.
#include "Input.hpp"
#include <mmsystem.h>

// Declaration only; raw index requires a valid initialized capabilities element.
extern JOYCAPSA g_ControllerCapabilities[];
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#elif DIRECTINPUT_VERSION != 0x0800
#error Input requires DirectInput version 0x0800
#endif
#include <dinput.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>

#define INPUT_SDK_ASSERT(name, expression) typedef char name[(expression) ? 1 : -1]
INPUT_SDK_ASSERT(InputSdkByte, sizeof(BYTE) == 1 && CHAR_BIT == 8);
INPUT_SDK_ASSERT(InputSdkWord, sizeof(WORD) == 2 && sizeof(SHORT) == 2);
INPUT_SDK_ASSERT(InputSdkUint, sizeof(UINT) == 4 && sizeof(unsigned int) == 4 && UINT_MAX == 0xffffffffu);
INPUT_SDK_ASSERT(InputSdkDword, sizeof(DWORD) == 4 && ULONG_MAX == 0xffffffffUL);
INPUT_SDK_ASSERT(InputSdkLong, sizeof(LONG) == 4 && LONG_MIN == (-2147483647L - 1));
INPUT_SDK_ASSERT(InputSdkHresult, sizeof(HRESULT) == 4);
INPUT_SDK_ASSERT(InputSdkJoyInfo, sizeof(JOYINFOEX) == 52);
INPUT_SDK_ASSERT(InputSdkJoyFields, offsetof(JOYINFOEX, dwXpos) == 8 &&
    offsetof(JOYINFOEX, dwYpos) == 12 && offsetof(JOYINFOEX, dwButtons) == 32);
INPUT_SDK_ASSERT(InputSdkJoyCaps, sizeof(JOYCAPSA) == 404);
INPUT_SDK_ASSERT(InputSdkJoyCapsFields, offsetof(JOYCAPSA, wXmin) == 36 &&
    offsetof(JOYCAPSA, wXmax) == 40 && offsetof(JOYCAPSA, wYmin) == 44 &&
    offsetof(JOYCAPSA, wYmax) == 48);
INPUT_SDK_ASSERT(InputSdkDiState, sizeof(DIJOYSTATE2) == 272);
INPUT_SDK_ASSERT(InputSdkDiFields, offsetof(DIJOYSTATE2, lX) == 0 &&
    offsetof(DIJOYSTATE2, lY) == 4 && offsetof(DIJOYSTATE2, rgbButtons) == 48);
#undef INPUT_SDK_ASSERT

unsigned short MapJoystickInput(unsigned int incomingMask,
                                unsigned int bindingSet,
                                unsigned int rawController) {
    const FrontEndInputView& input = g_InputRecords[bindingSet];
    WORD mask = static_cast<WORD>(incomingMask);
    if ((g_MainSupervisorView.flags & 0x400u) == 0) {
        JOYINFOEX state;
        memset(&state, 0, sizeof(state));
        state.dwSize = sizeof(state);
        state.dwFlags = 0xffu;
        if (joyGetPosEx(rawController != 0 ? 1u : 0u, &state) != JOYERR_NOERROR)
            return mask;

        // Binding domain: negative or 0..31. No invented clamping policy.
        if (input.shotButton >= 0 && (state.dwButtons & (1UL << input.shotButton)) != 0) mask |= 0x0001;
        if (input.bombButton >= 0 && (state.dwButtons & (1UL << input.bombButton)) != 0) mask |= 0x0002;
        if (input.menuButton >= 0 && (state.dwButtons & (1UL << input.menuButton)) != 0) mask |= 0x0008;
        if (input.focusButton >= 0 && (state.dwButtons & (1UL << input.focusButton)) != 0) mask |= 0x0004;
        if (input.skipButton >= 0 && (state.dwButtons & (1UL << input.skipButton)) != 0) mask |= 0x0100;

        // Expression must select the JOYCAPSA record by RAW rawController.
        const JOYCAPSA& caps = g_ControllerCapabilities[rawController];
        const DWORD xMid = (caps.wXmin + caps.wXmax) >> 1;
        const DWORD xQuarter = (caps.wXmax - caps.wXmin) >> 2;
        const DWORD xLower = xMid - xQuarter;
        const DWORD xUpper = xMid + xQuarter;
        const DWORD yMid = (caps.wYmin + caps.wYmax) >> 1;
        const DWORD yQuarter = (caps.wYmax - caps.wYmin) >> 2;
        const DWORD yLower = yMid - yQuarter;
        const DWORD yUpper = yMid + yQuarter;
        if (state.dwXpos < xLower) mask |= 0x0040;
        if (state.dwXpos > xUpper) mask |= 0x0080;
        if (state.dwYpos < yLower) mask |= 0x0010;
        if (state.dwYpos > yUpper) mask |= 0x0020;
        return mask;
    }

    // Two proved typed cells are represented by devices[2]. In this DI branch,
    // rawController == 0 is required; no nonzero-index equivalence is claimed.
    if (FAILED((g_MainSupervisorView.devices[rawController + 1])->Poll())) {
        HRESULT status = (g_MainSupervisorView.devices[rawController + 1])->Acquire();
        for (unsigned int retry = 0; retry < 400u && status == DIERR_INPUTLOST; ++retry)
            status = (g_MainSupervisorView.devices[rawController + 1])->Acquire();
        return mask;
    }
    DIJOYSTATE2 state;
    memset(&state, 0, sizeof(state));
    if (FAILED((g_MainSupervisorView.devices[rawController + 1])->GetDeviceState(sizeof(state), &state)))
        return mask;

    // Binding domain: negative or 0..127; DI buttons use only the high bit.
    if (input.shotButton >= 0 && ((state.rgbButtons[input.shotButton] & 0x80u) != 0)) mask |= 0x0001;
    if (input.bombButton >= 0 && ((state.rgbButtons[input.bombButton] & 0x80u) != 0)) mask |= 0x0002;
    if (input.menuButton >= 0 && ((state.rgbButtons[input.menuButton] & 0x80u) != 0)) mask |= 0x0008;
    if (input.focusButton >= 0 && ((state.rgbButtons[input.focusButton] & 0x80u) != 0)) mask |= 0x0004;
    if (input.skipButton >= 0 && ((state.rgbButtons[input.skipButton] & 0x80u) != 0)) mask |= 0x0100;

    // Reviewed signed-short fields within the existing Main serialized configuration.
    const LONG thresholdX = static_cast<LONG>(g_MainSupervisorView.controllerThresholdX);
    const LONG thresholdY = static_cast<LONG>(g_MainSupervisorView.controllerThresholdY);
    if (state.lX < -thresholdX) mask |= 0x0040;
    if (state.lX > thresholdX) mask |= 0x0080;
    if (state.lY < -thresholdY) mask |= 0x0010;
    if (state.lY > thresholdY) mask |= 0x0020;
    return mask;
}

unsigned short UpdateInput(unsigned int bindingSet) {
    WORD keyboardMask = 0;
    if (g_GameWindowView.windowIsActive != 0) {
        BYTE keys[256]; // Deliberately uninitialized; defined API output is required.
        if ((g_MainSupervisorView.flags & 0x200u) == 0) {
            GetKeyboardState(keys); // BOOL does not select a target failure branch.
            if (((keys[0x5A] & 0x80u) != 0)) keyboardMask |= 0x0001;
            if (((keys[0x58] & 0x80u) != 0)) keyboardMask |= 0x0002;
            if (((keys[0x10] & 0x80u) != 0)) keyboardMask |= 0x0004;
            if (((keys[0x1B] & 0x80u) != 0)) keyboardMask |= 0x0008;
            if (((keys[0x26] & 0x80u) != 0) || ((keys[0x68] & 0x80u) != 0)) keyboardMask |= 0x0010;
            if (((keys[0x28] & 0x80u) != 0) || ((keys[0x62] & 0x80u) != 0)) keyboardMask |= 0x0020;
            if (((keys[0x25] & 0x80u) != 0) || ((keys[0x64] & 0x80u) != 0)) keyboardMask |= 0x0040;
            if (((keys[0x27] & 0x80u) != 0) || ((keys[0x66] & 0x80u) != 0)) keyboardMask |= 0x0080;
            if (((keys[0x11] & 0x80u) != 0)) keyboardMask |= 0x0100;
            if (((keys[0x51] & 0x80u) != 0)) keyboardMask |= 0x0200;
            if (((keys[0x53] & 0x80u) != 0)) keyboardMask |= 0x0400;
            if (((keys[0x24] & 0x80u) != 0) || ((keys[0x50] & 0x80u) != 0)) keyboardMask |= 0x0800;
            if (((keys[0x0D] & 0x80u) != 0)) keyboardMask |= 0x1000;
            if (((keys[0x44] & 0x80u) != 0)) keyboardMask |= 0x2000;
            if (((keys[0x52] & 0x80u) != 0)) keyboardMask |= 0x4000;
            if (((keys[0x69] & 0x80u) != 0)) keyboardMask |= 0x0090;
            if (((keys[0x67] & 0x80u) != 0)) keyboardMask |= 0x0050;
            if (((keys[0x61] & 0x80u) != 0)) keyboardMask |= 0x0060;
            if (((keys[0x63] & 0x80u) != 0)) keyboardMask |= 0x00A0;
        } else if ((g_MainSupervisorView.devices[0])->GetDeviceState(sizeof(keys), keys) != 0) {
            (g_MainSupervisorView.devices[0])->Acquire();
        } else {
            if (((keys[0x2C] & 0x80u) != 0)) keyboardMask |= 0x0001;
            if (((keys[0x2D] & 0x80u) != 0)) keyboardMask |= 0x0002;
            if (((keys[0x2A] & 0x80u) != 0)) keyboardMask |= 0x0004;
            if (((keys[0x01] & 0x80u) != 0)) keyboardMask |= 0x0008;
            if (((keys[0xC8] & 0x80u) != 0) || ((keys[0x48] & 0x80u) != 0)) keyboardMask |= 0x0010;
            if (((keys[0x50] & 0x80u) != 0) || ((keys[0xD0] & 0x80u) != 0)) keyboardMask |= 0x0020;
            if (((keys[0xCB] & 0x80u) != 0) || ((keys[0x4B] & 0x80u) != 0)) keyboardMask |= 0x0040;
            if (((keys[0xCD] & 0x80u) != 0) || ((keys[0x4D] & 0x80u) != 0)) keyboardMask |= 0x0080;
            if (((keys[0x1D] & 0x80u) != 0)) keyboardMask |= 0x0100;
            if (((keys[0x10] & 0x80u) != 0)) keyboardMask |= 0x0200;
            if (((keys[0x1F] & 0x80u) != 0)) keyboardMask |= 0x0400;
            if (((keys[0xC7] & 0x80u) != 0) || ((keys[0x19] & 0x80u) != 0)) keyboardMask |= 0x0800;
            if (((keys[0x1C] & 0x80u) != 0)) keyboardMask |= 0x1000;
            if (((keys[0x20] & 0x80u) != 0)) keyboardMask |= 0x2000;
            if (((keys[0x13] & 0x80u) != 0)) keyboardMask |= 0x4000;
            if (((keys[0x49] & 0x80u) != 0)) keyboardMask |= 0x0090;
            if (((keys[0x47] & 0x80u) != 0)) keyboardMask |= 0x0050;
            if (((keys[0x4F] & 0x80u) != 0)) keyboardMask |= 0x0060;
            if (((keys[0x51] & 0x80u) != 0)) keyboardMask |= 0x00A0;
        }
    }

    const WORD now = MapJoystickInput(keyboardMask, bindingSet, 0);
    FrontEndInputView& input = g_InputRecords[bindingSet];
    const WORD old = input.current;
    input.previous = old;
    input.current = now;
    input.repeated = 0;
    for (unsigned int i = 0; i < 16; ++i) {
        const WORD bit = static_cast<WORD>(1u << i);
        if ((now & bit) != 0) {
            WORD count = static_cast<WORD>(input.counters[i] + 1u);
            if (count >= 26u) {
                count = static_cast<WORD>(count - 8u);
                input.repeated = static_cast<WORD>(input.repeated | bit);
            }
            input.counters[i] = count;
        } else {
            input.counters[i] = 0;
        }
    }
    const WORD changed = static_cast<WORD>(now ^ old);
    input.pressed = static_cast<WORD>(changed & now);
    input.released = static_cast<WORD>(changed & static_cast<WORD>(~now));
    return now;
}
