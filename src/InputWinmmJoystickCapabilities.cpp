#include "InputPolling.hpp"
#include "GameErrorContext.hpp"

extern "C" __declspec(dllimport) unsigned int __stdcall joyGetPosEx(
    unsigned int id, InputJoystickInfoView *info);
extern "C" __declspec(dllimport) unsigned int __stdcall joyGetDevCapsA(
    unsigned int id, InputJoystickCapsView *caps, unsigned int size);
extern char g_InputJoystickNotDetectedMessage[];

// Original TH10 0x0044A0C0: probe WINMM slots 0 and 1 and load joystick
// capabilities only after one position query succeeds. Return is a 16-bit
// availability flag; the high half of EAX is unspecified by this routine.
unsigned short InputReadWinmmJoystickCapabilities()
{
    InputJoystickInfoView info;
    info.size = sizeof(info);
    info.flags = 0xff;
    if (joyGetPosEx(0, &info) != 0 && joyGetPosEx(1, &info) != 0)
    {
        th10::g_GameErrorContext.Log(g_InputJoystickNotDetectedMessage);
        return 1;
    }
    joyGetDevCapsA(0, g_InputJoystickCaps, sizeof(InputJoystickCapsView));
    return 0;
}
