#include "input_owner.hpp"
#include <cstring>
namespace input_owner_candidate {
unsigned short MapJoystick(Environment& environment, unsigned int incoming,
                           unsigned int bindingIndex, unsigned int rawPad) {
    const input_candidate::ButtonBindings& bindings = environment.records[bindingIndex].bindings;
    if ((environment.modeFlags & 0x400u) == 0) {
        WinMMPosition state;
        std::memset(&state, 0, sizeof(state));
        state.size = sizeof(state);
        state.flags = 0xffu;
        if (environment.win32.joyGetPosEx(rawPad != 0 ? 1u : 0u, state) != 0)
            return static_cast<unsigned short>(incoming);
        const unsigned int buttons = input_candidate::WinMMButtons(bindings, state.buttons, incoming);
        const AxisCapabilities& caps = environment.capabilities[rawPad];
        return input_candidate::WinMMAxes(static_cast<unsigned short>(buttons), state.x, state.y,
                                         caps.xMin, caps.xMax, caps.yMin, caps.yMax);
    }
    JoystickDevice& device = *environment.joysticks[rawPad];
    if (device.Poll() < 0) {
        int status = device.Acquire();
        for (unsigned int retry = 0; retry < 400u && static_cast<unsigned int>(status) == 0x8007001eu; ++retry)
            status = device.Acquire();
        return static_cast<unsigned short>(incoming);
    }
    DIJoystickState state;
    std::memset(&state, 0, sizeof(state));
    if (device.GetDeviceState(sizeof(state), state) < 0)
        return static_cast<unsigned short>(incoming);
    const unsigned int buttons = input_candidate::DirectInputButtons(bindings, state.buttons, incoming);
    return input_candidate::DirectInputAxes(static_cast<unsigned short>(buttons),
                                           state.position.x, state.position.y,
                                           environment.thresholdWord0, environment.thresholdWord1);
}
unsigned short UpdateInput(Environment& environment, unsigned int bindingIndex) {
    unsigned short keyboardMask = 0;
    if (environment.keyboardEnabled) {
        KeyboardState state; // Target does not initialize the Win32 buffer before the call.
        if ((environment.modeFlags & 0x200u) == 0) {
            environment.win32.GetKeyboardState(state); // BOOL does not select a failure branch.
            keyboardMask = input_candidate::Win32KeyMask(state.keys);
        } else if (environment.keyboard.GetDeviceState(sizeof(state), state) != 0) {
            environment.keyboard.Acquire();
        } else {
            keyboardMask = input_candidate::DirectInputKeyMask(state.keys);
        }
    }
    const unsigned short now = MapJoystick(environment, keyboardMask, bindingIndex, 0);
    return input_candidate::update_input(environment.records[bindingIndex].transition, now);
}
}
