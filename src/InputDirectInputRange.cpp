#include "InputPolling.hpp"
#include <stddef.h>

// Target 0x43BB30 is supplied as the DirectInput EnumObjects callback by
// 0x43B8D0. Only the original type word at +0x18 is consumed here.
struct InputDirectObjectInstanceView
{
    unsigned char unknown000[0x18];
    unsigned int type;
};

// DIPROPRANGE: 16-byte header followed by two signed joystick limits.
struct InputAxisRangePropertyView
{
    unsigned int size;
    unsigned int headerSize;
    unsigned int object;
    unsigned int how;
    int minimum;
    int maximum;
};
typedef char InputAxisRangeSize18[
    (sizeof(InputAxisRangePropertyView) == 0x18 &&
     offsetof(InputDirectObjectInstanceView, type) == 0x18) ? 1 : -1];

// The vtable slot at +0x18 corresponds to SetProperty on the original
// DirectInput device. Keep this separate from Acquire/GetDeviceState/Poll.
typedef int (__stdcall *InputSetPropertyFn)(InputDeviceView *device,
    const void *property, const InputAxisRangePropertyView *range);
struct InputSetPropertyVtableView
{
    void *priorSlots[6];
    InputSetPropertyFn SetProperty;
};


int __stdcall InputEnumAxisRangeCallback(
    const InputDirectObjectInstanceView *object, void *context)
{
    (void)context;
    unsigned int type = object->type;
    InputAxisRangePropertyView range;
    if ((type & 3u) != 0)
    {
        range.size = sizeof(range);
        range.headerSize = 16;
        range.object = type;
        range.how = 2;
        range.minimum = -1000;
        range.maximum = 1000;
        InputDeviceView *device = g_InputPadDevices[0];
        InputSetPropertyVtableView *vtable =
            reinterpret_cast<InputSetPropertyVtableView *>(device->vtable);
        if (vtable->SetProperty(device, reinterpret_cast<const void *>(4), &range) < 0)
            return 0;
    }
    return 1;
}
