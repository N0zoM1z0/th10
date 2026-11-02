#include "InputPolling.hpp"

// The DirectInput8 owner at original 0x491C34 owns device creation.
struct InputDirectInput8View;
struct InputDirectInput8VtableView
{
    void *queryInterface;
    void *addRef;
    void *release;
    int (__stdcall *CreateDevice)(InputDirectInput8View *input,
        const void *deviceGuid, InputDeviceView **output, void *outer);
};
struct InputDirectInput8View
{
    InputDirectInput8VtableView *vtable;
};
extern InputDirectInput8View *g_InputDirectInput8;

// Original 0x43BAF0: EnumDevices callback scans until one candidate can be
// instantiated. The incoming device instance's GUID begins at offset +4.
int __stdcall InputEnumPadDeviceCallback(
    const unsigned char *deviceDescription, void *context)
{
    (void)context;
    if (g_InputPadDevices[0] == NULL)
    {
        InputDirectInput8View *input = g_InputDirectInput8;
        if (input->vtable->CreateDevice(input,
                deviceDescription + 4, &g_InputPadDevices[0], NULL) < 0)
            return 1;
    }
    return 0;
}
