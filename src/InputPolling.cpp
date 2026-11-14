#include "InputPolling.hpp"
#include <string.h>

extern "C" __declspec(dllimport) int __stdcall GetKeyboardState(
    unsigned char *state);
extern "C" __declspec(dllimport) unsigned int __stdcall joyGetPosEx(
    unsigned int id, InputJoystickInfoView *info);

static unsigned int __stdcall MergeWinMmButton(unsigned short *out, short button,
    unsigned short action, unsigned int buttons)
{
    if (button < 0)
        return 0;
    // Retail VC7.1 emits SHL EAX, CL, whose native count uses CL's low
    // five bits. Do not introduce an explicit AND: it changes the target
    // 43-byte helper. The original binary's wider-count behavior is x86-
    // specific, rather than a promise of portable C++ shift semantics.
    unsigned int mask = 1u << static_cast<unsigned int>(button);
    *out |= (buttons & mask) ? action : 0;
    return (buttons & mask) ? action : 0;
}

static unsigned int MergeDirectButton(unsigned short *out, short button,
    unsigned short action, const signed char *buttons)
{
    if (button < 0)
        return 0;
    *out |= (buttons[button] & 0x80) ? action : 0;
    return (buttons[button] & 0x80) ? action : 0;
}

// Target44A190: the actual PollInputFrame caller permits LTCG to lower the
// mask to ECX while retaining two stack arguments and RET8.
unsigned short __stdcall MergeJoystickInput(
    unsigned short buttons, int bindingIndex, int padIndex)
{
    unsigned short merged = buttons;
    InputJoystickInfoView info;
    InputDirectJoystickStateView state;
    InputBindingSetView *binding =
        reinterpret_cast<InputBindingSetView *>(&g_FrontEndInput) + bindingIndex;
    if ((g_FrontEndSupervisorFlags & 0x400) == 0)
    {
        memset(&info, 0, sizeof(info));
        info.size = sizeof(info);
        info.flags = 0xff;
        if (joyGetPosEx(padIndex != 0, &info) != 0)
            return merged;
        MergeWinMmButton(&merged, binding->controllerMappings[0], 1, info.buttons);
        MergeWinMmButton(&merged, binding->controllerMappings[1], 2, info.buttons);
        MergeWinMmButton(&merged, binding->controllerMappings[3], 8, info.buttons);
        MergeWinMmButton(&merged, binding->controllerMappings[2], 4, info.buttons);
        MergeWinMmButton(&merged, binding->controllerMappings[8], 0x100, info.buttons);

        unsigned int xQuarter = (g_InputJoystickCaps[padIndex].xMax -
            g_InputJoystickCaps[padIndex].xMin) >> 2;
        merged |= info.x > ((g_InputJoystickCaps[padIndex].xMin +
            g_InputJoystickCaps[padIndex].xMax) >> 1) + xQuarter ? 0x80 : 0;
        merged |= info.x < ((g_InputJoystickCaps[padIndex].xMin +
            g_InputJoystickCaps[padIndex].xMax) >> 1) - xQuarter ? 0x40 : 0;
        unsigned int yQuarter = (g_InputJoystickCaps[padIndex].yMax -
            g_InputJoystickCaps[padIndex].yMin) >> 2;
        merged |= info.y > ((g_InputJoystickCaps[padIndex].yMin +
            g_InputJoystickCaps[padIndex].yMax) >> 1) + yQuarter ? 0x20 : 0;
        merged |= info.y < ((g_InputJoystickCaps[padIndex].yMin +
            g_InputJoystickCaps[padIndex].yMax) >> 1) - yQuarter ? 0x10 : 0;
        return merged;
    }

    if (g_InputPadDevices[padIndex]->vtable->Poll(g_InputPadDevices[padIndex]) < 0)
    {
        int retries = 0;
        int result = g_InputPadDevices[padIndex]->vtable->Acquire(
            g_InputPadDevices[padIndex]);
        while (result == static_cast<int>(0x8007001e))
        {
            result = g_InputPadDevices[padIndex]->vtable->Acquire(
                g_InputPadDevices[padIndex]);
            if (++retries >= 400)
                return merged;
        }
        return merged;
    }
    memset(&state, 0, sizeof(state));
    if (g_InputPadDevices[padIndex]->vtable->GetDeviceState(
        g_InputPadDevices[padIndex], sizeof(state), &state) < 0)
        return merged;
    MergeDirectButton(&merged, binding->controllerMappings[0], 1, state.buttons);
    MergeDirectButton(&merged, binding->controllerMappings[1], 2, state.buttons);
    MergeDirectButton(&merged, binding->controllerMappings[3], 8, state.buttons);
    MergeDirectButton(&merged, binding->controllerMappings[2], 4, state.buttons);
    MergeDirectButton(&merged, binding->controllerMappings[8], 0x100, state.buttons);
    merged |= state.x < -g_InputJoystickXThreshold ? 0x40 : 0;
    merged |= state.y < -g_InputJoystickYThreshold ? 0x10 : 0;
    merged |= state.x > g_InputJoystickXThreshold ? 0x80 : 0;
    merged |= state.y > g_InputJoystickYThreshold ? 0x20 : 0;
    return merged;
}

// Target44A4E0: caller-visible scratch buttons. The DirectInput record is
// deliberately uninitialized, and its GetDeviceState result is unchecked.
signed char *__fastcall FrontEndGetControllerState(int padIndex)
{
    InputJoystickInfoView info;
    InputDirectJoystickStateView state;
    memset(g_InputJoystickButtons, 0, sizeof(g_InputJoystickButtons));
    if ((g_FrontEndSupervisorFlags & 0x400) == 0)
    {
        memset(&info, 0, sizeof(info));
        info.size = sizeof(info);
        info.flags = 0xff;
        if (joyGetPosEx(0, &info) == 0)
        {
            unsigned int bit = 0;
            do
            {
                if (info.buttons & 1)
                    g_InputJoystickButtons[bit] = static_cast<signed char>(0x80);
                ++bit;
                info.buttons >>= 1;
            } while (bit < 32);
        }
    }
    else if (g_InputPadDevices[padIndex]->vtable->Poll(g_InputPadDevices[padIndex]) < 0)
    {
        int retries = 0;
        int result = g_InputPadDevices[padIndex]->vtable->Acquire(
            g_InputPadDevices[padIndex]);
        while (result == static_cast<int>(0x8007001e))
        {
            result = g_InputPadDevices[padIndex]->vtable->Acquire(
                g_InputPadDevices[padIndex]);
            if (++retries >= 400)
                break;
        }
    }
    else
    {
        g_InputPadDevices[padIndex]->vtable->GetDeviceState(
            g_InputPadDevices[padIndex], sizeof(state), &state);
        memcpy(g_InputJoystickButtons, state.buttons, sizeof(state.buttons));
    }
    return g_InputJoystickButtons;
}

// Target44A5F0: keyboard state plus joystick merge, followed by the gameplay
// counters atA. This differs from the separate debug-repeat counters at38.
unsigned short __fastcall PollInputFrame(int bindingIndex)
{
    unsigned short mask = 0;
    InputBindingSetView *binding =
        reinterpret_cast<InputBindingSetView *>(&g_FrontEndInput) + bindingIndex;
    unsigned char state[256];
    if (g_InputEnabled != 0)
    {
        if ((g_FrontEndSupervisorFlags & 0x200) == 0)
        {
            GetKeyboardState(state);
            unsigned char keys = 0;
            keys = (keys >> 1) | (state[0x5a] & 0x80);
            keys = (keys >> 1) | (state[0x58] & 0x80);
            keys = (keys >> 1) | (state[0x10] & 0x80);
            keys = (keys >> 1) | (state[0x1b] & 0x80);
            keys = (keys >> 1) | ((state[0x26] | state[0x68]) & 0x80);
            keys = (keys >> 1) | ((state[0x28] | state[0x62]) & 0x80);
            keys = (keys >> 1) | ((state[0x25] | state[0x64]) & 0x80);
            keys = (keys >> 1) | ((state[0x27] | state[0x66]) & 0x80);
            unsigned short extra = state[0x52] & 0x80;
            extra = (extra << 1) | (state[0x44] & 0x80);
            extra = (extra << 1) | (state[0x0d] & 0x80);
            extra = (extra << 1) | ((state[0x24] | state[0x50]) & 0x80);
            extra = (extra << 1) | (state[0x53] & 0x80);
            extra = (extra << 1) | (state[0x51] & 0x80);
            extra = (extra << 1) | (state[0x11] & 0x80);
            mask = keys | (extra << 1);
            mask |= (state[0x69] & 0x80) ? 0x90 : 0;
            mask |= (state[0x67] & 0x80) ? 0x50 : 0;
            mask |= (state[0x61] & 0x80) ? 0x60 : 0;
            mask |= (state[0x63] & 0x80) ? 0xa0 : 0;
        }
        else
        {
            int result = g_InputKeyboardDevice->vtable->GetDeviceState(
                g_InputKeyboardDevice, sizeof(state), state);
            mask = 0;
            if (result == static_cast<int>(0x8007001e))
                g_InputKeyboardDevice->vtable->Acquire(g_InputKeyboardDevice);
            else if (result != 0)
                g_InputKeyboardDevice->vtable->Acquire(g_InputKeyboardDevice);
            else
            {
                unsigned char keys = 0;
                keys = (keys >> 1) | (state[0x2c] & 0x80);
                keys = (keys >> 1) | (state[0x2d] & 0x80);
                keys = (keys >> 1) | (state[0x2a] & 0x80);
                keys = (keys >> 1) | (state[0x01] & 0x80);
                keys = (keys >> 1) | ((state[0xc8] | state[0x48]) & 0x80);
                keys = (keys >> 1) | ((state[0xd0] | state[0x50]) & 0x80);
                keys = (keys >> 1) | ((state[0xcb] | state[0x4b]) & 0x80);
                keys = (keys >> 1) | ((state[0xcd] | state[0x4d]) & 0x80);
                unsigned short extra = state[0x13] & 0x80;
                extra = (extra << 1) | (state[0x20] & 0x80);
                extra = (extra << 1) | (state[0x1c] & 0x80);
                extra = (extra << 1) | ((state[0xc7] | state[0x19]) & 0x80);
                extra = (extra << 1) | (state[0x1f] & 0x80);
                extra = (extra << 1) | (state[0x10] & 0x80);
                extra = (extra << 1) | (state[0x1d] & 0x80);
                mask = keys | (extra << 1);
                mask |= (state[0x49] & 0x80) ? 0x90 : 0;
                mask |= (state[0x47] & 0x80) ? 0x50 : 0;
                mask |= (state[0x4f] & 0x80) ? 0x60 : 0;
                mask |= (state[0x51] & 0x80) ? 0xa0 : 0;
            }
        }
    }
    const unsigned short merged = MergeJoystickInput(mask, bindingIndex, 0);
    binding->previous = binding->current;
    binding->current = merged;
    unsigned short repeat = 1;
    unsigned short bits = merged;
    binding->repeated = 0;
    unsigned short *hold = binding->holdCounters;
    for (int index = 0; index < 16; ++index)
    {
        if (bits & 1)
        {
            if (++*hold >= 26)
            {
                binding->repeated |= repeat;
                *hold -= 8;
            }
        }
        else
            *hold = 0;
        bits >>= 1;
        ++hold;
        repeat <<= 1;
    }
    unsigned short current = binding->current;
    unsigned short changed = current ^ binding->previous;
    binding->pressed = changed & current;
    binding->released = changed & ~current;
    return merged;
}
