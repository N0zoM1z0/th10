#pragma once
#include <stddef.h>

// The shared serialized34-byte settings layout; unknown fields keep offsets.
struct GameConfigurationView
{
    unsigned int format;
    short keyBindings[9];
    unsigned short unknown016;
    unsigned short unknown018;
    signed char colorMode;
    unsigned char unknown01B;
    unsigned char unknown01C;
    unsigned char windowed;
    unsigned char frameskip;
    unsigned char unknown01F;
    signed char bgmVolume;
    signed char sfxVolume;
    signed char hintMode;
    unsigned char unknown023[0x0d];
    unsigned int options;

    GameConfigurationView() { ResetDefaults(); }
    void ResetDefaults();
};

typedef char GameConfigurationLayoutIs34[
    (sizeof(GameConfigurationView) == 0x34 &&
     offsetof(GameConfigurationView, unknown016) == 0x16 &&
     offsetof(GameConfigurationView, colorMode) == 0x1a &&
     offsetof(GameConfigurationView, bgmVolume) == 0x20 &&
     offsetof(GameConfigurationView, options) == 0x30) ? 1 : -1];
