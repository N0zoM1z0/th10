#include "Main.hpp"
#include "GameErrorContext.hpp"
#include "InputPolling.hpp"

// Original 0x43B8D0: device initialization is part of the input supervisor.
// The offsets are from its original EAX receiver, not a new allocation.
struct InputDeviceOwnerView
{
    unsigned char unknown000[0x0c];
    void *directInput8;                 // +0x0c / global 0x491c34
    InputDeviceView *keyboard;          // +0x10 / global 0x491c38
    InputDeviceView *joystick;          // +0x14 / global 0x491c3c
    unsigned char unknown018[0x48-0x18];
    HWND window;                        // +0x48
    unsigned char unknown04C[0x150-0x4c];
    unsigned int flags;                 // +0x150
    void ReleaseDirectInput8();
};
typedef char InputDeviceOwnerOffsets[
    (offsetof(InputDeviceOwnerView, directInput8)==0x0c &&
     offsetof(InputDeviceOwnerView, joystick)==0x14 &&
     offsetof(InputDeviceOwnerView, window)==0x48 &&
     offsetof(InputDeviceOwnerView, flags)==0x150) ? 1 : -1];

struct InputDirect8Interface;
struct InputDirect8Methods
{
    void *query, *addref, *release;
    long (__stdcall *CreateDevice)(InputDirect8Interface *, const void *, InputDeviceView **, void *);
    long (__stdcall *EnumDevices)(InputDirect8Interface *, unsigned int,
        int (__stdcall *)(const unsigned char *, void *), void *, unsigned int);
};
struct InputDirect8Interface { InputDirect8Methods *vtable; };

struct InputDirectDeviceMethods
{
    void *query, *addref;
    unsigned long (__stdcall *Release)(InputDeviceView *);
    long (__stdcall *GetCapabilities)(InputDeviceView *, void *);
    long (__stdcall *EnumObjects)(InputDeviceView *,
        int (__stdcall *)(const void *, void *), void *, unsigned int);
    void *GetProperty;
    void *SetProperty;
    long (__stdcall *Acquire)(InputDeviceView *);
    void *Unacquire;
    void *GetDeviceState;
    void *GetDeviceData;
    long (__stdcall *SetDataFormat)(InputDeviceView *, const void *);
    void *SetEventNotification;
    long (__stdcall *SetCooperativeLevel)(InputDeviceView *, HWND, unsigned int);
};

extern "C" long __stdcall DirectInput8Create(HINSTANCE, unsigned int,
    const void *, void **, void *);
extern unsigned char g_InputDirectInputIID[];
extern unsigned char g_InputKeyboardGuid[];
extern unsigned char g_InputKeyboardDataFormat[];
extern unsigned char g_InputJoystickDataFormat[];
extern unsigned char g_InputPadCapabilitiesRaw[44];
extern char g_InputCreateFailureLog[];
extern char g_InputKeyboardFormatFailureLog[];
extern char g_InputKeyboardCoopFailureLog[];
extern char g_InputKeyboardAcquireLog[];
extern char g_InputDeviceEnumeratedLog[];
extern char g_InputJoystickEnumeratedLog[];
extern int __stdcall InputEnumPadDeviceCallback(const unsigned char *,void *);
struct InputDirectObjectInstanceView;
extern int __stdcall InputEnumAxisRangeCallback(const InputDirectObjectInstanceView *, void *);

static inline InputDirectDeviceMethods *InputMethods(InputDeviceView *device)
{
    return reinterpret_cast<InputDirectDeviceMethods *>(device->vtable);
}

// Target original 0x00421D20: conditional release and clear of the parent
// DirectInput8 pointer. The original incoming receiver is held in ESI, which
// the actual InputInitializeDevices caller may promote under VC7.1 /GL.
__declspec(noinline) void InputDeviceOwnerView::ReleaseDirectInput8()
{
    InputDeviceView *const input =
        reinterpret_cast<InputDeviceView *>(directInput8);
    if (input != NULL)
    {
        InputMethods(input)->Release(input);
        directInput8 = NULL;
    }
}

// DirectInput8Create and the two enumeration callbacks have their real
// external code/data boundaries. The success/failure cleanup is not omitted.
__declspec(noinline) int InputInitializeDevices(InputDeviceOwnerView *owner)
{
    HINSTANCE instance = reinterpret_cast<HINSTANCE>(GetWindowLongA(owner->window,-6));
    if ((owner->flags & 0x08u) != 0)
        return -1;

    if (DirectInput8Create(instance,0x800,g_InputDirectInputIID,
            &owner->directInput8,NULL) < 0)
    {
        owner->directInput8 = NULL;
        th10::g_GameErrorContext.Log(g_InputCreateFailureLog);
        return -1;
    }

    InputDirect8Interface *input = static_cast<InputDirect8Interface *>(owner->directInput8);
    if (input->vtable->CreateDevice(input,g_InputKeyboardGuid,&owner->keyboard,NULL) < 0)
    {
        if (owner->directInput8)
        {
            InputDirectDeviceMethods *methods=InputMethods(reinterpret_cast<InputDeviceView *>(owner->directInput8));
            methods->Release(reinterpret_cast<InputDeviceView *>(owner->directInput8));
            owner->directInput8=NULL;
        }
        th10::g_GameErrorContext.Log(g_InputCreateFailureLog);
        return -1;
    }
    InputDeviceView *keyboard = owner->keyboard;
    if (InputMethods(keyboard)->SetDataFormat(keyboard,g_InputKeyboardDataFormat) < 0)
    {
        if (keyboard)
        {
            InputMethods(keyboard)->Release(keyboard);
            owner->keyboard=NULL;
        }
        if (owner->directInput8)
        {
            InputDirectDeviceMethods *methods=InputMethods(reinterpret_cast<InputDeviceView *>(owner->directInput8));
            methods->Release(reinterpret_cast<InputDeviceView *>(owner->directInput8));
            owner->directInput8=NULL;
        }
        th10::g_GameErrorContext.Log(g_InputKeyboardFormatFailureLog);
        return -1;
    }
    keyboard=owner->keyboard;
    if (InputMethods(keyboard)->SetCooperativeLevel(keyboard,owner->window,0x16) < 0)
    {
        if (keyboard)
        {
            InputMethods(keyboard)->Release(keyboard);
            owner->keyboard=NULL;
        }
        owner->ReleaseDirectInput8();
        th10::g_GameErrorContext.Log(g_InputKeyboardCoopFailureLog);
        return -1;
    }
    keyboard=owner->keyboard;
    InputMethods(keyboard)->Acquire(keyboard);
    th10::g_GameErrorContext.Log(g_InputKeyboardAcquireLog);
    input=static_cast<InputDirect8Interface *>(owner->directInput8);
    input->vtable->EnumDevices(input,4,&InputEnumPadDeviceCallback,NULL,1);
    if (owner->joystick)
    {
        InputDeviceView *device=owner->joystick;
        InputMethods(device)->SetDataFormat(device,g_InputJoystickDataFormat);
        InputMethods(device)->SetCooperativeLevel(device,owner->window,10);
        *reinterpret_cast<unsigned int *>(g_InputPadCapabilitiesRaw) = 44;
        InputMethods(device)->GetCapabilities(device,g_InputPadCapabilitiesRaw);
        InputMethods(device)->EnumObjects(device,
            reinterpret_cast<int (__stdcall *)(const void *,void *)>(&InputEnumAxisRangeCallback),NULL,0);
        th10::g_GameErrorContext.Log(g_InputJoystickEnumeratedLog);
    }
    return 0;
}

// Original 0x43BA90: input initialization thread entry. Keep its two observed
// input-device availability bits separate from the enumerator callback.
int InputDeviceInitThread()
{
    g_FrontEndSupervisorFlags &= ~0x600u;
    InputInitializeDevices(
        reinterpret_cast<InputDeviceOwnerView *>(&g_MainSupervisorView));
    unsigned int flags = g_FrontEndSupervisorFlags;
    flags ^= (flags ^ ((g_InputKeyboardDevice != NULL) << 9)) & 0x200u;
    flags ^= (flags ^ ((g_InputPadDevices[0] != NULL) << 10)) & 0x400u;
    g_FrontEndSupervisorFlags = flags;
    return flags;
}
