#ifndef TH10_RECONSTRUCTION_INPUT_POLLING_HPP
#define TH10_RECONSTRUCTION_INPUT_POLLING_HPP

#include "InputState.hpp"

// Complete observed6A record, separate from the older partial FrontEnd view.
// All members are words; no inferred packing or replacement global is needed.
// The original array count, defining storage and C++ lifetime remain open.
struct InputBindingSetView
{
    unsigned short current, previous, repeated, pressed, released;
    unsigned short holdCounters[16];
    unsigned short unknown02A;
    unsigned short previousRaw, previousHeld, repeatFlags;
    unsigned short frameMask032, frameMask034, unknown036;
    unsigned short repeatCounters[16];
    short controllerMappings[9];
};
typedef char InputBindingSetObservedLayout[
    (sizeof(InputBindingSetView) == 0x6a &&
     offsetof(InputBindingSetView, holdCounters) == 0xa &&
     offsetof(InputBindingSetView, previousRaw) == 0x2c &&
     offsetof(InputBindingSetView, repeatCounters) == 0x38 &&
     offsetof(InputBindingSetView, controllerMappings) == 0x58) ? 1 : -1];

struct InputJoystickInfoView
{
    unsigned int size, flags;
    unsigned int x, y, z, r, u, v;
    unsigned int buttons, buttonNumber, pov, reserved1, reserved2;
};

struct InputJoystickCapsView
{
    unsigned short manufacturer, product;
    char name[32];
    unsigned int xMin, xMax, yMin, yMax, zMin, zMax;
    unsigned int buttonCount, periodMin, periodMax;
    unsigned int rMin, rMax, uMin, uMax, vMin, vMax;
    unsigned int flags, maxAxes, axes, maxButtons;
    char registryKey[32], driver[260];
};

struct InputDirectJoystickStateView
{
    int x, y, z, rx, ry, rz;
    int sliders[2];
    unsigned int pov[4];
    signed char buttons[128];
    int velocity[8], acceleration[8], force[8];
};

struct InputDeviceView;
struct InputDeviceVtableView
{
    void *slots00To18[7];
    int (__stdcall *Acquire)(InputDeviceView *device);
    void *Unacquire;
    int (__stdcall *GetDeviceState)(InputDeviceView *device,
        unsigned int size, void *state);
    void *slots28To60[15];
    int (__stdcall *Poll)(InputDeviceView *device);
};
struct InputDeviceView { InputDeviceVtableView *vtable; };

typedef char InputPollingObservedLayouts[
    (sizeof(InputJoystickInfoView) == 0x34 &&
     sizeof(InputJoystickCapsView) == 0x194 &&
     offsetof(InputJoystickCapsView, xMin) == 0x24 &&
     offsetof(InputJoystickCapsView, yMax) == 0x30 &&
     sizeof(InputDirectJoystickStateView) == 0x110 &&
     offsetof(InputDirectJoystickStateView, buttons) == 0x30 &&
     offsetof(InputDeviceVtableView, Acquire) == 0x1c &&
     offsetof(InputDeviceVtableView, GetDeviceState) == 0x24 &&
     offsetof(InputDeviceVtableView, Poll) == 0x64) ? 1 : -1];

extern unsigned int g_FrontEndSupervisorFlags;
extern int g_InputEnabled;
extern InputDeviceView *g_InputKeyboardDevice;
extern InputDeviceView *g_InputPadDevices[];
extern InputJoystickCapsView g_InputJoystickCaps[];
extern short g_InputJoystickXThreshold, g_InputJoystickYThreshold;
extern signed char g_InputJoystickButtons[128];

unsigned short __stdcall MergeJoystickInput(
    int bindingIndex, int padIndex, unsigned short buttons);
signed char *__fastcall FrontEndGetControllerState(int padIndex);
void __fastcall PollInputFrame(int bindingIndex);

#endif
