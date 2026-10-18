#ifndef TH10_RECONSTRUCTION_INPUT_HPP
#define TH10_RECONSTRUCTION_INPUT_HPP

#include "InputState.hpp"

// Natural source interfaces. Actual /GL caller context determines native ABI.
unsigned short UpdateInput(unsigned int bindingSet);
// DI requires rawController == 0; WinMM requires a valid initialized caps index.
unsigned short MapJoystickInput(unsigned int incomingMask,
                                unsigned int bindingSet,
                                unsigned int rawController);

#endif
