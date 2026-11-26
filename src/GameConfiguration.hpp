#pragma once
#include <stddef.h>

// Directly evidenced by TH10 v1.00a th10.cfg validation and startup log strings
// in src/MainConfiguration.cpp and src/Main.cpp.
enum GameConfigurationFormat
{
    GAME_CONFIG_FORMAT_VERSION = 0x100003
};

enum GameConfigurationOptionFlags
{
    GAME_CONFIG_FORCE_16BIT_TEXTURES        = 0x001, // "16Bit のテクスチャの使用を強制します"
    GAME_CONFIG_FORCE_REFERENCE_RASTERIZER  = 0x002, // "リファレンスラスタライザを強制します"
    GAME_CONFIG_DISABLE_FOG                 = 0x004, // "フォグの使用を抑制します"
    GAME_CONFIG_DISABLE_DIRECTINPUT         = 0x008, // "パッド、キーボードの入力に DirectInput を使用しません"
    GAME_CONFIG_LOAD_BGM_IN_MEMORY          = 0x010, // "ＢＧＭをメモリに読み込みます"
    GAME_CONFIG_DISABLE_VSYNC               = 0x020, // "垂直同期を取りません"
    GAME_CONFIG_DISABLE_TEXT_AUTO_DETECT    = 0x040, // "文字描画の環境を自動検出しません"
    GAME_CONFIG_ASK_DISPLAY_MODE_ON_STARTUP = 0x100  // DialogBoxParamA(..., 203, ...) in WinMain
};

// The shared serialized 0x34-byte settings layout; unknown fields keep offsets.
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
