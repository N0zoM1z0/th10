#pragma once

#define _WIN32_WINNT 0x0500

#include <stddef.h>

// AnmManager.hpp is intentionally independent of the Windows SDK and declares
// three imported functions against void pointers. Hide those declarations in
// this translation unit before including the SDK's typed declarations.
#define EnterCriticalSection Th10AnmEnterCriticalSection
#define LeaveCriticalSection Th10AnmLeaveCriticalSection
#define Sleep Th10AnmSleep
#include "AnmManager.hpp"
#undef EnterCriticalSection
#undef LeaveCriticalSection
#undef Sleep

#include <windows.h>
#include "ScriptViewerWorker.hpp"
#include "GameConfiguration.hpp"

enum MainRenderResult
{
    MAIN_RENDER_KEEP_RUNNING = 0,
    MAIN_RENDER_EXIT = 1,
    MAIN_RENDER_RESTART = 2,
    MAIN_RENDER_ERROR = -1
};

struct MainD3d9InterfaceView;
struct MainD3d9DeviceView;

struct MainD3d9DisplayModeView
{
    unsigned int width;
    unsigned int height;
    unsigned int refreshRate;
    unsigned int format;
};

struct MainD3d9PresentParametersView
{
    unsigned int backBufferWidth;
    unsigned int backBufferHeight;
    unsigned int backBufferFormat;
    unsigned int backBufferCount;
    unsigned int multiSampleType;
    unsigned int multiSampleQuality;
    unsigned int swapEffect;
    HWND deviceWindow;
    int windowed;
    int enableAutoDepthStencil;
    unsigned int autoDepthStencilFormat;
    unsigned int flags;
    unsigned int fullScreenRefreshRate;
    unsigned int presentationInterval;
};

typedef char MainD3d9PresentParametersSizeIs38[
    (sizeof(MainD3d9PresentParametersView) == 0x38) ? 1 : -1];

typedef unsigned long (__stdcall *MainD3d9Release)(void *object);
typedef long (__stdcall *MainD3d9GetAdapterDisplayMode)(
    MainD3d9InterfaceView *d3d, unsigned int adapter,
    MainD3d9DisplayModeView *displayMode);
typedef long (__stdcall *MainD3d9CheckDeviceFormat)(
    MainD3d9InterfaceView *d3d, unsigned int adapter,
    unsigned int deviceType, unsigned int adapterFormat,
    unsigned int usage, unsigned int resourceType,
    unsigned int checkFormat);
typedef long (__stdcall *MainD3d9CreateDevice)(
    MainD3d9InterfaceView *d3d, unsigned int adapter,
    unsigned int deviceType, HWND focusWindow,
    unsigned int behaviorFlags,
    MainD3d9PresentParametersView *presentParameters,
    MainD3d9DeviceView **device);

struct MainD3d9InterfaceVtableView
{
    void *QueryInterface;
    void *AddRef;
    MainD3d9Release Release;                       // +0x08
    void *slots0CTo1C[5];
    MainD3d9GetAdapterDisplayMode GetAdapterDisplayMode; // +0x20
    void *slot24;
    MainD3d9CheckDeviceFormat CheckDeviceFormat;  // +0x28
    void *slots2CTo3C[5];
    MainD3d9CreateDevice CreateDevice;             // +0x40
};

struct MainD3d9InterfaceView
{
    MainD3d9InterfaceVtableView *vtable;
};

typedef long (__stdcall *MainD3d9DeviceCall014)(MainD3d9DeviceView *device);

typedef long (__stdcall *MainD3d9TestCooperativeLevel)(
    MainD3d9DeviceView *device);
typedef long (__stdcall *MainD3d9GetDeviceCaps)(
    MainD3d9DeviceView *device, void *caps);
typedef long (__stdcall *MainD3d9Reset)(
    MainD3d9DeviceView *device,
    MainD3d9PresentParametersView *presentParameters);
typedef long (__stdcall *MainD3d9Present)(
    MainD3d9DeviceView *device, const void *sourceRectangle,
    const void *destinationRectangle, HWND destinationWindow,
    const void *dirtyRegion);
typedef long (__stdcall *MainD3d9BeginScene)(MainD3d9DeviceView *device);
typedef long (__stdcall *MainD3d9EndScene)(MainD3d9DeviceView *device);
typedef long (__stdcall *MainD3d9SetTransform)(
    MainD3d9DeviceView *device, unsigned int state, const void *matrix);
typedef long (__stdcall *MainD3d9SetViewport)(
    MainD3d9DeviceView *device, const AnmViewportView *viewport);
typedef long (__stdcall *MainD3d9GetViewport)(
    MainD3d9DeviceView *device, AnmViewportView *viewport);
typedef long (__stdcall *MainD3d9SetRenderState)(
    MainD3d9DeviceView *device, unsigned int state, unsigned int value);
typedef long (__stdcall *MainD3d9SetTexture)(
    MainD3d9DeviceView *device, unsigned int stage, void *texture);
typedef long (__stdcall *MainD3d9SetTextureStageState)(
    MainD3d9DeviceView *device, unsigned int stage,
    unsigned int state, unsigned int value);
typedef long (__stdcall *MainD3d9SetSamplerState)(
    MainD3d9DeviceView *device, unsigned int sampler,
    unsigned int state, unsigned int value);

struct MainD3d9DeviceVtableView
{
    void *QueryInterface;
    void *AddRef;
    MainD3d9Release Release;                       // +0x008
    MainD3d9TestCooperativeLevel TestCooperativeLevel; // +0x00C
    void *slot010;
    MainD3d9DeviceCall014 Call014;                // +0x014
    void *slot018;
    MainD3d9GetDeviceCaps GetDeviceCaps;           // +0x01C
    void *slots020To03C[8];
    MainD3d9Reset Reset;                           // +0x040
    MainD3d9Present Present;                       // +0x044
    void *slots048To0A0[23];
    MainD3d9BeginScene BeginScene;                 // +0x0A4
    MainD3d9EndScene EndScene;                     // +0x0A8
    void *slot0AC;
    MainD3d9SetTransform SetTransform;             // +0x0B0
    void *slots0B4To0B8[2];
    MainD3d9SetViewport SetViewport;               // +0x0BC
    MainD3d9GetViewport GetViewport;               // +0x0C0
    void *slots0C4To0E0[8];
    MainD3d9SetRenderState SetRenderState;         // +0x0E4
    void *slots0E8To100[7];
    MainD3d9SetTexture SetTexture;                 // +0x104
    void *slot108;
    MainD3d9SetTextureStageState SetTextureStageState; // +0x10C
    void *slot110;
    MainD3d9SetSamplerState SetSamplerState;       // +0x114
};

struct MainD3d9DeviceView
{
    MainD3d9DeviceVtableView *vtable;
};

typedef char MainD3d9InterfaceCreateDeviceAt40[
    (offsetof(MainD3d9InterfaceVtableView, CreateDevice) == 0x40) ? 1 : -1];
typedef char MainD3d9DeviceCallAt14[
    (offsetof(MainD3d9DeviceVtableView, Call014) == 0x14) ? 1 : -1];
typedef char MainD3d9DeviceResetPresentAt40[
    (offsetof(MainD3d9DeviceVtableView, Reset) == 0x40 &&
     offsetof(MainD3d9DeviceVtableView, Present) == 0x44) ? 1 : -1];
typedef char MainD3d9DeviceSceneAtA4[
    (offsetof(MainD3d9DeviceVtableView, BeginScene) == 0xa4 &&
     offsetof(MainD3d9DeviceVtableView, EndScene) == 0xa8) ? 1 : -1];
typedef char MainD3d9DeviceViewportAtBC[
    (offsetof(MainD3d9DeviceVtableView, SetViewport) == 0xbc &&
     offsetof(MainD3d9DeviceVtableView, GetViewport) == 0xc0) ? 1 : -1];
typedef char MainD3d9DeviceStateAtE4[
    (offsetof(MainD3d9DeviceVtableView, SetRenderState) == 0xe4 &&
     offsetof(MainD3d9DeviceVtableView, SetTexture) == 0x104 &&
     offsetof(MainD3d9DeviceVtableView, SetTextureStageState) == 0x10c &&
     offsetof(MainD3d9DeviceVtableView, SetSamplerState) == 0x114) ? 1 : -1];

#pragma pack(push, 4)
struct GameWindowView
{
    HWND window;                         // +0x00
    int windowIsClosing;                 // +0x04
    HINSTANCE instance;                  // +0x08
    int windowIsActive;                  // +0x0C
    int windowIsInactive;                // +0x10
    signed char framesSinceRedraw;        // +0x14
    unsigned char padding015[3];          // +0x15
    LARGE_INTEGER performanceFrequency;  // +0x18
    LARGE_INTEGER performanceStart;      // +0x20
    unsigned char usesRelativePath;      // +0x28
    unsigned char unknown029[3];         // +0x29
    int screenSaveActive;                // +0x2C
    int lowPowerActive;                  // +0x30
    int powerOffActive;                  // +0x34
    double currentTimestamp;             // +0x38
    double lastTimestamp;                // +0x40
    double lastFrameTime;                // +0x48
    double timeOrigin;                   // +0x50

    static LRESULT __stdcall WindowProc(
        HWND window, UINT message, WPARAM wparam, LPARAM lparam);
    static void ActivateWindow(HWND window);
    static void Present();
    static int __stdcall Render(GameWindowView *window);
    static double GetTimestamp();
    static int CreateGameWindow(HINSTANCE instance);
    static int InitD3DInterface();
    static int InitD3DRendering();
    static void ResetRenderState();
    static int CheckForRunningGameInstance();
    static int CalcExecutableChecksum();
    static int __stdcall ResolveShortcut(
        const char *shortcutPath, char *destination, int destinationSize);
};
#pragma pack(pop)

typedef char GameWindowViewSizeIs58[
    (sizeof(GameWindowView) == 0x58) ? 1 : -1];
typedef char GameWindowViewPerformanceFrequencyAt18[
    (offsetof(GameWindowView, performanceFrequency) == 0x18) ? 1 : -1];
typedef char GameWindowViewRelativePathAt28[
    (offsetof(GameWindowView, usesRelativePath) == 0x28 &&
     sizeof(((GameWindowView *)0)->usesRelativePath) == 1 &&
     offsetof(GameWindowView, screenSaveActive) == 0x2c) ? 1 : -1];
typedef char GameWindowViewCurrentTimestampAt38[
    (offsetof(GameWindowView, currentTimestamp) == 0x38 &&
     offsetof(GameWindowView, timeOrigin) == 0x50) ? 1 : -1];

struct StartupWorkerView;

// Only the Release and offset20 calls are observed by Main cleanup.
struct MainInputObjectView;
struct MainInputVtableView
{
    void *slots000To004[2];
    unsigned long (__stdcall *Release)(MainInputObjectView *object);
    void *slots00CTo01C[5];
    long (__stdcall *Call020)(MainInputObjectView *object);
};
struct MainInputObjectView { MainInputVtableView *vtable; };
struct MidiTimerView;

// Target constructor clears784 bytes through clearColor; preserve that view extent.
#pragma pack(push, 4)
struct MainSupervisorView
{
    MainSupervisorView();
    void SetNextGameMode(int mode);
    int StartModeThread(int (*entry)(), void *context);
    int UpdateGameMode();
    int ReleaseGame();
    void LeaveLock(int index);
    static int __fastcall OnUpdate(MainSupervisorView *supervisor);

    HINSTANCE instance;                         // +0x000
    MainD3d9InterfaceView *d3dInterface;        // +0x004
    MainD3d9DeviceView *d3dDevice;              // +0x008
    MainInputObjectView *inputInterface;       // +0x00C
    MainInputObjectView *inputDevice0;         // +0x010
    MainInputObjectView *inputDevice1;         // +0x014
    unsigned char unknown018[0x030];
    HWND gameWindow;                            // +0x048
    AnmMatrixView viewMatrix;                   // +0x04C
    AnmMatrixView projectionMatrix;             // +0x08C
    AnmViewportView viewport;                   // +0x0CC
    MainD3d9PresentParametersView presentParameters; // +0x0E4
    MidiTimerView *timerOwner;                 // +0x11C
    GameConfigurationView configuration;          // +0x120
    unsigned char unknown154[0x118];
    AnmViewportOwnerView gameplayViewport;      // +0x26C
    unsigned char unknown350[0x034];
    AnmViewportOwnerView *activeViewport;       // +0x384
    int viewportConfigured;                     // +0x388
    int currentGameMode;                        // +0x38C
    int nextGameMode;                           // +0x390
    int previousGameMode;                       // +0x394
    unsigned int unknown398;
    int newGameSession;                         // +0x39C
    unsigned char unknown3A0[0x00C];
    int screenTransitionCountdown;              // +0x3AC
    unsigned char unknown3B0[0x004];
    int disableVsync;                           // +0x3B4
    int couldSetRefreshRate;                    // +0x3B8
    int lastRenderTime;                         // +0x3BC
    void *musicOwner;                           // +0x3C0
    unsigned char unknown3C4[0x008];
    unsigned int flags;                         // +0x3CC
    unsigned char unknown3D0[0x008];
    unsigned char d3dCaps[0x130];               // +0x3D8
    unsigned char unknown508[0x124];
    ScriptViewerWorkerView modeWorker;           // +0x62C; running at+0x63C
    int updateGate;                             // +0x648
    CRITICAL_SECTION criticalSections[7];       // +0x64C
    unsigned char criticalSectionLockCounts[7]; // +0x6F4
    unsigned char unknown6FB[0x055];
    unsigned int cachedFogEnabled;              // +0x750, full-word cache
    unsigned char unknown754[0x010];
    void *buffer764;                           // +0x764
    StartupWorkerView *startupWorker;            // +0x768
    unsigned char unknown76C[0x00C];
    double lastFrameDuration;                   // +0x778
    unsigned int clearColor;                    // +0x780
};
#pragma pack(pop)

typedef char MainSupervisorD3dAt004[
    (offsetof(MainSupervisorView, d3dInterface) == 0x004 &&
     offsetof(MainSupervisorView, d3dDevice) == 0x008) ? 1 : -1];
typedef char MainSupervisorCleanupFields[
    (offsetof(MainInputVtableView, Release) == 8 &&
     offsetof(MainInputVtableView, Call020) == 0x20 &&
     offsetof(MainSupervisorView, inputInterface) == 0xc &&
     offsetof(MainSupervisorView, inputDevice0) == 0x10 &&
     offsetof(MainSupervisorView, inputDevice1) == 0x14 &&
     offsetof(MainSupervisorView, timerOwner) == 0x11c &&
     offsetof(MainSupervisorView, buffer764) == 0x764) ? 1 : -1];

typedef char MainSupervisorSizeIs784[
    (sizeof(MainSupervisorView) == 0x784) ? 1 : -1];

typedef char MainSupervisorPresentationAt0E4[
    (offsetof(MainSupervisorView, presentParameters) == 0x0e4) ? 1 : -1];
typedef char MainSupervisorConfigurationOffsets[
    (offsetof(MainSupervisorView, configuration) == 0x120 &&
     offsetof(MainSupervisorView, configuration) +
     offsetof(GameConfigurationView, bgmVolume) == 0x140 &&
     offsetof(MainSupervisorView, configuration) +
     offsetof(GameConfigurationView, sfxVolume) == 0x141 &&
     offsetof(MainSupervisorView, configuration) +
     offsetof(GameConfigurationView, colorMode) == 0x13a &&
     offsetof(MainSupervisorView, configuration) +
     offsetof(GameConfigurationView, hintMode) == 0x142 &&
     offsetof(MainSupervisorView, configuration) +
     offsetof(GameConfigurationView, windowed) == 0x13d &&
     offsetof(MainSupervisorView, configuration) +
     offsetof(GameConfigurationView, frameskip) == 0x13e &&
     offsetof(MainSupervisorView, configuration) +
     offsetof(GameConfigurationView, options) == 0x150) ? 1 : -1];
typedef char MainSupervisorViewportAt26C[
    (offsetof(MainSupervisorView, gameplayViewport) == 0x26c &&
     offsetof(MainSupervisorView, activeViewport) == 0x384) ? 1 : -1];
typedef char MainSupervisorFlagsAt3CC[
    (offsetof(MainSupervisorView, nextGameMode) == 0x390 &&
     offsetof(MainSupervisorView, screenTransitionCountdown) == 0x3ac &&
     offsetof(MainSupervisorView, flags) == 0x3cc &&
     offsetof(MainSupervisorView, currentGameMode) == 0x38c &&
     offsetof(MainSupervisorView, previousGameMode) == 0x394 &&
     offsetof(MainSupervisorView, newGameSession) == 0x39c &&
     (offsetof(MainSupervisorView, modeWorker) == 0x62c &&
      offsetof(MainSupervisorView, modeWorker) +
      offsetof(ScriptViewerWorkerView, running) == 0x63c) &&
     offsetof(MainSupervisorView, updateGate) == 0x648 &&
     offsetof(MainSupervisorView, startupWorker) == 0x768 &&
     offsetof(MainSupervisorView, clearColor) == 0x780) ? 1 : -1];
typedef char MainSupervisorCriticalSectionsAt64C[
    (offsetof(MainSupervisorView, criticalSections) == 0x64c &&
     offsetof(MainSupervisorView, criticalSectionLockCounts) == 0x6f4) ? 1 : -1];
typedef char MainSupervisorFrameDurationAt778[
    (offsetof(MainSupervisorView, lastFrameDuration) == 0x778) ? 1 : -1];
typedef char MainSupervisorFogCacheAt750[
    (offsetof(MainSupervisorView, cachedFogEnabled) == 0x750 &&
     sizeof(((MainSupervisorView *)0)->cachedFogEnabled) == 4) ? 1 : -1];

struct MainAllocationRegistryView
{
    void *entries[0x2800];
    int hasEntries;
};

typedef char MainAllocationRegistrySizeIsA004[
    (sizeof(MainAllocationRegistryView) == 0xa004) ? 1 : -1];

extern GameWindowView g_GameWindowView;
extern MainSupervisorView g_MainSupervisorView;
extern MainAllocationRegistryView *g_MainAllocationRegistry;
extern void *g_MainCallbackOwner;
extern unsigned char g_MainSoundOwner[0x52d0];
extern HANDLE g_MainExclusiveMutex;
extern unsigned short g_MainInputEdges;
extern unsigned int g_MainExecutableChecksum;
extern unsigned int g_MainExecutableSize;
