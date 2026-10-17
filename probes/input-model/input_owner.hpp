#ifndef INPUT_OWNER_CANDIDATE_HPP
#define INPUT_OWNER_CANDIDATE_HPP
#include "components/key_masks.hpp"
#include "components/input_transition.hpp"
#include "components/button_masks.hpp"
#include <cstddef>
#include "di_axes.hpp"
#include "winmm_axes.hpp"
namespace input_owner_candidate {
typedef char signed_word[sizeof(int) == 4 && INT_MAX == 2147483647 && INT_MIN == (-2147483647 - 1) ? 1 : -1];
struct KeyboardState { unsigned char keys[256]; };
struct WinMMPosition {
    unsigned int size, flags, x, y, z, r, u, v;
    unsigned int buttons, buttonNumber, pov, reserved1, reserved2;
};
struct DICoordinates { int x, y, z, rx, ry, rz, sliders[2]; };
struct DIJoystickState {
    DICoordinates position;
    unsigned int pov[4];
    unsigned char buttons[128];
    DICoordinates velocity, acceleration, force;
};
typedef char keyboard_size[sizeof(KeyboardState) == 256 ? 1 : -1];
typedef char winmm_size[sizeof(WinMMPosition) == 52 ? 1 : -1];
typedef char winmm_fields[offsetof(WinMMPosition,x) == 8 && offsetof(WinMMPosition,y) == 12 && offsetof(WinMMPosition,buttons) == 32 ? 1 : -1];
typedef char di_size[sizeof(DIJoystickState) == 272 ? 1 : -1];
typedef char di_fields[offsetof(DIJoystickState,position) == 0 && offsetof(DICoordinates,y) == 4 && offsetof(DIJoystickState,buttons) == 48 ? 1 : -1];
// Logical fixture-owned data. Neither type asserts target storage stride/layout.
struct AxisCapabilities { unsigned int xMin, xMax, yMin, yMax; };
struct InputRecord {
    input_candidate::InputPrefix transition;
    input_candidate::ButtonBindings bindings;
};
// Typed host seams, NOT declarations of the actual COM vtable or x86 call ABI.
struct KeyboardDevice {
    virtual ~KeyboardDevice() {}
    virtual int GetDeviceState(unsigned int size, KeyboardState& output) = 0;
    virtual int Acquire() = 0;
};
struct JoystickDevice {
    virtual ~JoystickDevice() {}
    virtual int Poll() = 0;
    virtual int Acquire() = 0;
    virtual int GetDeviceState(unsigned int size, DIJoystickState& output) = 0;
};
struct Win32Input {
    virtual ~Win32Input() {}
    virtual int GetKeyboardState(KeyboardState& output) = 0;
    virtual unsigned int joyGetPosEx(unsigned int id, WinMMPosition& output) = 0;
};
struct Environment {
    Win32Input& win32;
    KeyboardDevice& keyboard;
    JoystickDevice* const* joysticks;
    const AxisCapabilities* capabilities;
    InputRecord* records;
    unsigned int modeFlags;
    bool keyboardEnabled;
    short thresholdWord0, thresholdWord1;
    Environment(Win32Input& w, KeyboardDevice& k, JoystickDevice* const* j,
                const AxisCapabilities* c, InputRecord* r, unsigned int flags, bool enabled,
                short word0, short word1)
        : win32(w), keyboard(k), joysticks(j), capabilities(c), records(r),
          modeFlags(flags), keyboardEnabled(enabled), thresholdWord0(word0), thresholdWord1(word1) {}
};
// Valid indices/storage and typed API output premises are caller obligations.
unsigned short MapJoystick(Environment& environment, unsigned int incoming,
                           unsigned int bindingIndex, unsigned int rawPad);
unsigned short UpdateInput(Environment& environment, unsigned int bindingIndex);
}
#endif
