#include <windows.h>

extern "C" void ClearKeyboardHighBits()
{
    unsigned char keyState[256];
    GetKeyboardState(keyState);
    for (int i = 0; i < 256; ++i)
        keyState[i] &= 0x7f;
    SetKeyboardState(keyState);
}
