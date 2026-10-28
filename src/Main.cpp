#include "GameErrorContext.hpp"
#include "Main.hpp"
#include "MainViewport.hpp"
#include "FileSystem.hpp"
#include "Midi.hpp"

#include <shlobj.h>
#include <direct.h>
#include <mmsystem.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <winnls32.h>

enum MainD3d9Constants
{
    MAIN_D3D_SDK_VERSION = 0x20,
    MAIN_D3D_DEVICE_HAL = 1,
    MAIN_D3D_DEVICE_REF = 2,
    MAIN_D3D_CREATE_SOFTWARE_VERTEXPROCESSING = 0x20,
    MAIN_D3D_CREATE_HARDWARE_VERTEXPROCESSING = 0x40,
    MAIN_D3D_FORMAT_X8R8G8B8 = 0x16,
    MAIN_D3D_FORMAT_R5G6B5 = 0x17,
    MAIN_D3D_FORMAT_A8R8G8B8 = 0x15,
    MAIN_D3D_FORMAT_D16 = 0x50,
    MAIN_D3D_RESOURCE_TEXTURE = 3,
    MAIN_D3D_SWAP_DISCARD = 1,
    MAIN_D3D_SWAP_COPY = 3,
    MAIN_D3D_PRESENT_LOCKABLE_BACKBUFFER = 1,
    MAIN_D3D_PRESENT_INTERVAL_ONE = 1,
    MAIN_D3D_PRESENT_INTERVAL_IMMEDIATE = 0x80000000u,
    MAIN_D3D_TRANSFORM_VIEW = 2,
    MAIN_D3D_TRANSFORM_PROJECTION = 3
};

enum MainSupervisorFlags
{
    MAIN_FLAG_HARDWARE_VERTEX_PROCESSING = 0x001,
    MAIN_FLAG_LOCKABLE_BACKBUFFER = 0x002,
    MAIN_FLAG_32BIT_TEXTURES = 0x004,
    MAIN_FLAG_DEVICE_NEEDS_RESET = 0x010,
    MAIN_FLAG_NO_STARTUP_TITLE = 0x040,
    MAIN_FLAG_CLOSE_REQUESTED = 0x080
};

extern "C" MainD3d9InterfaceView *__stdcall Direct3DCreate9(
    unsigned int sdkVersion);
extern "C" AnmMatrixView *__stdcall D3DXMatrixLookAtLH(
    AnmMatrixView *output, const AnmFloat3View *eye,
    const AnmFloat3View *at, const AnmFloat3View *up);
extern "C" AnmMatrixView *__stdcall D3DXMatrixPerspectiveFovLH(
    AnmMatrixView *output, float fov, float aspect,
    float nearPlane, float farPlane);

extern const char g_MainWindowTitle[];
extern const char g_MainStartupLog[];
extern const char g_MainRestartLog[];
extern const char g_MainAlreadyRunningError[];
extern const char g_MainD3dCreateError[];
extern const char g_MainD3d32BitLog[];
extern const char g_MainD3dRefresh60Log[];
extern const char g_MainD3dImmediateLog[];
extern const char g_MainD3dHardwareLog[];
extern const char g_MainD3dSoftwareLog[];
extern const char g_MainD3dReferenceLog[];
extern const char g_MainD3dHardwareUnavailableLog[];
extern const char g_MainD3dSoftwareUnavailableLog[];
extern const char g_MainD3dRefreshFailedLog[];
extern const char g_MainD3dImmediateFailedLog[];
extern const char g_MainD3dFatalError[];
extern const char g_MainD3dTextureOperationWarning[];
extern const char g_MainD3dTextureSizeWarning[];
extern const char g_MainD3dTextureFormatWarning[];

extern void MainLog(const char *message);
extern void MainFatal(const char *message);
extern void MainResetLog();
extern void MainFlushLog();
extern void MainInitializeSystemParameters();
extern int MainLoadConfiguration(
    const char *path, MainSupervisorView *supervisor);
extern INT_PTR CALLBACK MainConfigurationDialogProc(
    HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);
extern void *MainCreateCallbackOwner();
extern void MainDestroyCallbackOwner(void *owner);
extern int MainRegisterChain(MainSupervisorView *supervisor);
extern void MainReleaseChain(MainSupervisorView *supervisor);
extern int __stdcall MainRunCalcChain(void *owner);
extern int __stdcall MainRunDrawChain(void *owner);
extern void MainInitializeControllerCaps();
extern void MainResetInput();
extern void MainStopReplayScan();
extern void __stdcall MainSetScreenColor(unsigned int color);
// Target 0x00438A30 releases and clears the 32 primary surfaces.
// Outlining reproduces the two physical target calls; original spelling is unknown.
__declspec(noinline) void MainReleaseAnmSurfaces(AnmRenderManagerView *manager)
{
    for (int i = 0; i < 32; ++i)
    {
        if (manager->surfaces[i] != NULL)
        {
            manager->surfaces[i]->vtable->Release(manager->surfaces[i]);
            manager->surfaces[i] = NULL;
        }
    }
}
extern void MainServiceAnmScreenshots(AnmRenderManagerView *manager);
int MainTakeScreenshot(MainSupervisorView *supervisor, const char *path);
extern void MainStartSoundThread(unsigned char *soundOwner, HWND window);
extern int MainProcessSoundQueues(unsigned char *soundOwner);
extern void MainRequestSoundThreadStop(unsigned char *soundOwner);
extern int MainJoinSoundThread(unsigned char *soundOwner);
extern int MainReleaseSound(unsigned char *soundOwner);
extern void MainStopMusic(void *musicOwner);
extern void MainDestroyMusic(void *musicOwner);
extern void MainFreeRegisteredAllocation(void *allocation);

static __forceinline int MainD3dFailed(long result)
{
    return result < 0;
}

void MainSupervisorView::SetNextGameMode(int mode)
{
    if ((flags & 0x1000u) == 0)
        nextGameMode = mode;
    else
        nextGameMode = 2;
}


// A local partial view; only target-observed screenshot fields are named.
struct MainScreenshotContextView
{
    unsigned char unknown000[8];
    D3d9DeviceView *device;
    unsigned char unknown00C[0xe0];
    unsigned int format;
    unsigned char unknown0F0[0x41c];
    int pending;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFO *bitmapInfo;
    unsigned char *pixels;
    char filename[256];
};
typedef char MainScreenshotFieldOffsets[
    (offsetof(MainScreenshotContextView, device) == 8 &&
     offsetof(MainScreenshotContextView, format) == 0xec &&
     offsetof(MainScreenshotContextView, pending) == 0x50c &&
     offsetof(MainScreenshotContextView, fileHeader) == 0x510 &&
     offsetof(MainScreenshotContextView, bitmapInfo) == 0x520 &&
     offsetof(MainScreenshotContextView, pixels) == 0x524 &&
     offsetof(MainScreenshotContextView, filename) == 0x528 &&
     sizeof(BITMAPFILEHEADER) == 14) ? 1 : -1];
extern char g_MainScreenshotUnexpectedFormat[];
extern char g_MainScreenshotAllocationError[];
extern char g_MainScreenshot16BitUnsupported[];
extern unsigned long g_MainScreenshotThreadHandle;
// Target 0x00420540 writes the captured BMP and retires its two buffers.
void __cdecl MainScreenshotWorker(void *argument)
{
    MainScreenshotContextView *state =
        reinterpret_cast<MainScreenshotContextView *>(&g_MainSupervisorView);
    FileSystem::OpenWriteFile(state->filename);
    FileSystem::WriteToOpenFile(&state->fileHeader, sizeof(state->fileHeader));
    FileSystem::WriteToOpenFile(state->bitmapInfo, sizeof(BITMAPINFOHEADER));
    FileSystem::WriteToOpenFile(state->pixels, 640 * 480 * 3);
    FileSystem::CloseWriteFile();
    if (state->bitmapInfo != NULL)
    {
        free(state->bitmapInfo);
        state->bitmapInfo = NULL;
    }
    if (state->pixels != NULL)
    {
        free(state->pixels);
        state->pixels = NULL;
    }
    g_MainScreenshotThreadHandle = 0;
}


// Target 0x00420670. Keep the observed format and allocation error behavior.
int MainTakeScreenshot(MainSupervisorView *supervisor, const char *path)
{
    MainScreenshotContextView *state =
        reinterpret_cast<MainScreenshotContextView *>(supervisor);
    D3d9SurfaceView *surface;
    D3d9LockedRectView locked;
    int row;
    int outputOffset;
    while (state->pending != 0)
        Sleep(10);
    surface = NULL;
    state->device->vtable->GetBackBuffer(
        state->device, 0, 0, 0, &surface);
    memset(&state->fileHeader, 0, sizeof(state->fileHeader));
    state->fileHeader.bfType = 0x4d42;
    state->fileHeader.bfOffBits = 54;
    state->fileHeader.bfSize = 54;
    strcpy(state->filename, path);

    switch (state->format)
    {
    case MAIN_D3D_FORMAT_X8R8G8B8:
        state->bitmapInfo = static_cast<BITMAPINFO *>(malloc(sizeof(BITMAPINFO)));
        if (state->bitmapInfo == NULL)
        {
            th10::g_GameErrorContext.Log(g_MainScreenshotAllocationError);
            goto out;
        }
        memset(state->bitmapInfo, 0, sizeof(BITMAPINFO));
        state->pixels = static_cast<unsigned char *>(malloc(640 * 480 * 3));
        if (state->pixels == NULL)
        {
            th10::g_GameErrorContext.Log(g_MainScreenshotAllocationError);
            goto out;
        }
        state->fileHeader.bfSize += 640 * 480 * 3;
        state->bitmapInfo->bmiHeader.biBitCount = 24;
        state->bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        state->bitmapInfo->bmiHeader.biWidth = 640;
        state->bitmapInfo->bmiHeader.biHeight = 480;
        state->bitmapInfo->bmiHeader.biPlanes = 1;
        state->bitmapInfo->bmiHeader.biCompression = BI_RGB;
        surface->vtable->LockRect(surface, &locked, NULL, 0);
        for (row = 479, outputOffset = 0; row > -1; --row, outputOffset += 640 * 3)
        {
            unsigned char *destination = state->pixels + outputOffset;
            unsigned char *source = static_cast<unsigned char *>(locked.bits) + locked.pitch * row;
            for (int column = 0; column < 640; ++column)
            {
                *reinterpret_cast<unsigned short *>(destination) =
                    *reinterpret_cast<unsigned short *>(source);
                destination[2] = source[2];
                source += 4;
                destination += 3;
            }
        }
        surface->vtable->UnlockRect(surface);
        g_MainScreenshotThreadHandle = _beginthread(MainScreenshotWorker, 0, NULL);
        break;
    case MAIN_D3D_FORMAT_R5G6B5:
        th10::g_GameErrorContext.Log(g_MainScreenshot16BitUnsupported);
        break;
    default:
        th10::g_GameErrorContext.Log(g_MainScreenshotUnexpectedFormat);
        return 1;
    }
    goto out;
out:
    if (surface != NULL)
        surface->vtable->Release(surface);
    return 0;
}


struct MainAnmCaptureRequestView
{
    int surfaceIndex;
    int anmIndex;
    int textureSourceX, textureSourceY, textureSourceWidth, textureSourceHeight;
    int textureDestinationX, textureDestinationY;
    int textureDestinationWidth, textureDestinationHeight;
    int textureIndex;
    int surfaceSourceX, surfaceSourceY, surfaceSourceWidth, surfaceSourceHeight;
    int surfaceDestinationX, surfaceDestinationY;
    int surfaceDestinationWidth, surfaceDestinationHeight;
};
typedef char MainAnmCaptureRequestOffsets[
    (sizeof(MainAnmCaptureRequestView) == 0x4c &&
     offsetof(MainAnmCaptureRequestView, textureIndex) == 0x28 &&
     offsetof(MainAnmCaptureRequestView, surfaceSourceX) == 0x2c) ? 1 : -1];

// Target 0x00438A60. This local view names only the observed request prefix;
// the older shared-header coordinate labels are provisional.
__declspec(noinline) void MainServiceAnmScreenshots(AnmRenderManagerView *manager)
{
    MainAnmCaptureRequestView *request =
        reinterpret_cast<MainAnmCaptureRequestView *>(manager);
    if (request->anmIndex >= 0)
    {
        manager->CaptureToTexture(
            request->anmIndex, request->textureIndex,
            request->textureSourceX, request->textureSourceY,
            request->textureSourceWidth, request->textureSourceHeight,
            request->textureDestinationX, request->textureDestinationY,
            request->textureDestinationWidth, request->textureDestinationHeight);
        request->anmIndex = -1;
    }
    if (request->surfaceIndex >= 0)
    {
        manager->CaptureToSurface(
            request->surfaceIndex,
            request->surfaceSourceX, request->surfaceSourceY,
            request->surfaceSourceWidth, request->surfaceSourceHeight,
            request->surfaceDestinationX, request->surfaceDestinationY,
            request->surfaceDestinationWidth, request->surfaceDestinationHeight);
        request->surfaceIndex = -1;
    }
}

// TH10_MAIN_FUNCTION: 0x00438AD0 WinMain
// Target 0x00438AD0-0x0043903C owns process startup, the device-loss loop,
// restart teardown and final process cleanup. The physical ABI consumes all
// four WinMain arguments and ends in RET 0x10.
int WINAPI WinMain(
    HINSTANCE instance, HINSTANCE previousInstance,
    LPSTR commandLine, int showCommand)
{
    MSG message;
    int renderResult = MAIN_RENDER_KEEP_RUNNING;
    int i;

    (void)previousInstance;
    (void)commandLine;
    (void)showCommand;

    g_GameWindowView.instance = instance;
    {
        MainAllocationRegistryView *registry = new MainAllocationRegistryView;
        if (registry != NULL)
        {
            memset(registry, 0, sizeof(MainAllocationRegistryView));
            registry->hasEntries = 0;
        }
        g_MainAllocationRegistry = registry;
    }

    for (i = 0; i < 7; ++i)
        InitializeCriticalSection(&g_MainSupervisorView.criticalSections[i]);
    th10::g_GameErrorContext.Log(const_cast<char *>(g_MainStartupLog));

    if (GameWindowView::CheckForRunningGameInstance() == MAIN_RENDER_ERROR)
        goto stop;

    g_MainSupervisorView.instance = instance;
    MainInitializeSystemParameters();
    if (MainLoadConfiguration("th10.cfg", &g_MainSupervisorView) != 0)
        goto stop;

    if ((g_MainSupervisorView.options & 0x100) != 0)
    {
        INT_PTR dialogResult = DialogBoxParamA(
            instance, MAKEINTRESOURCEA(203), NULL,
            MainConfigurationDialogProc, 0);
        g_MainSupervisorView.windowed = dialogResult != 6;
    }
    GameWindowView::CalcExecutableChecksum();

restart:
    g_MainCallbackOwner = MainCreateCallbackOwner();
    if (GameWindowView::InitD3DInterface() != 0)
        goto stop;

    if (GameWindowView::CreateGameWindow(instance) != 0)
        goto stop;

    memset(g_MainSoundOwner, 0, 0x52d0);
    MainStartSoundThread(g_MainSoundOwner, g_GameWindowView.window);
    if (GameWindowView::InitD3DRendering() != 0)
        goto stop;

    MainInitializeControllerCaps();
    MainResetInput();
    g_AnmRenderManagerView = new AnmRenderManagerView;

    if (g_MainSupervisorView.windowed == 0)
    {
        WINNLSEnableIME(NULL, FALSE);
        ShowCursor(FALSE);
        SetCursor(NULL);
    }

    g_GameWindowView.timeOrigin = 0.0;
    g_GameWindowView.lastTimestamp =
        g_GameWindowView.currentTimestamp =
        g_GameWindowView.lastFrameTime =
        g_GameWindowView.timeOrigin = GameWindowView::GetTimestamp();
    SetForegroundWindow(g_GameWindowView.window);

    renderResult = MainRegisterChain(&g_MainSupervisorView);
    if (renderResult != MAIN_RENDER_KEEP_RUNNING)
    {
        if (renderResult != MAIN_RENDER_ERROR)
            renderResult = MAIN_RENDER_RESTART;
        goto releaseGame;
    }

    renderResult = MAIN_RENDER_KEEP_RUNNING;
    g_GameWindowView.framesSinceRedraw = -4;
    while (g_GameWindowView.windowIsClosing == 0)
    {
        if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        else
        {
            long deviceStatus =
                g_MainSupervisorView.d3dDevice->vtable->TestCooperativeLevel(
                    g_MainSupervisorView.d3dDevice);
            if (deviceStatus == 0)
            {
                renderResult = GameWindowView::Render(&g_GameWindowView);
                if (renderResult != MAIN_RENDER_KEEP_RUNNING)
                    break;
                g_MainSupervisorView.flags &= ~MAIN_FLAG_DEVICE_NEEDS_RESET;
            }
            else if (deviceStatus == (long)0x88760869)
            {
                MainReleaseAnmSurfaces(g_AnmRenderManagerView);
                if (g_MainSupervisorView.d3dDevice->vtable->Reset(
                        g_MainSupervisorView.d3dDevice,
                        &g_MainSupervisorView.presentParameters) != 0)
                {
                    break;
                }
                GameWindowView::ResetRenderState();
                g_MainSupervisorView.screenTransitionCountdown = 3;
                g_MainSupervisorView.flags |= MAIN_FLAG_DEVICE_NEEDS_RESET;
            }
        }
    }

releaseGame:
    MainReleaseChain(&g_MainSupervisorView);
    if (g_MainCallbackOwner != NULL)
    {
        MainDestroyCallbackOwner(g_MainCallbackOwner);
        g_MainCallbackOwner = NULL;
    }
    while (MainProcessSoundQueues(g_MainSoundOwner) != 0)
        ;

stop:
    MainRequestSoundThreadStop(g_MainSoundOwner);
    MainJoinSoundThread(g_MainSoundOwner);
    MainReleaseSound(g_MainSoundOwner);

    delete g_AnmRenderManagerView;
    g_AnmRenderManagerView = NULL;

    if (g_MainSupervisorView.d3dDevice != NULL)
    {
        g_MainSupervisorView.d3dDevice->vtable->Reset(
            g_MainSupervisorView.d3dDevice,
            &g_MainSupervisorView.presentParameters);
        if (g_MainSupervisorView.d3dDevice != NULL)
        {
            g_MainSupervisorView.d3dDevice->vtable->Release(
                g_MainSupervisorView.d3dDevice);
            g_MainSupervisorView.d3dDevice = NULL;
        }
    }
    if (g_MainSupervisorView.d3dInterface != NULL)
    {
        g_MainSupervisorView.d3dInterface->vtable->Release(
            g_MainSupervisorView.d3dInterface);
        g_MainSupervisorView.d3dInterface = NULL;
    }
    if (g_GameWindowView.window != NULL)
    {
        ShowWindow(g_GameWindowView.window, SW_HIDE);
        MoveWindow(g_GameWindowView.window, 0, 0, 0, 0, FALSE);
        DestroyWindow(g_GameWindowView.window);
        g_GameWindowView.window = NULL;
    }
    ShowCursor(TRUE);

    if (renderResult == MAIN_RENDER_RESTART)
    {
        th10::g_GameErrorContext.m_BufferEnd =
            th10::g_GameErrorContext.m_Buffer;
        th10::g_GameErrorContext.m_Buffer[0] = 0;
        th10::g_GameErrorContext.Log(const_cast<char *>(g_MainRestartLog));
        if (g_MainSupervisorView.windowed == 0)
            WINNLSEnableIME(NULL, TRUE);
        for (i = 0; i < 60; ++i)
        {
            if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
        }
        goto restart;
    }

    FileSystem::WriteFile(
        "th10.cfg", g_MainSupervisorView.serializedConfiguration, 0x34);
    if (g_MainSupervisorView.musicOwner != NULL)
    {
        static_cast<MidiOutputView *>(g_MainSupervisorView.musicOwner)
            ->StopPlayback();
        if (g_MainSupervisorView.musicOwner != NULL)
            MainDestroyMusic(g_MainSupervisorView.musicOwner);
        g_MainSupervisorView.musicOwner = NULL;
    }
    th10::g_GameErrorContext.Flush();

    for (i = 0; i < 7; ++i)
        DeleteCriticalSection(&g_MainSupervisorView.criticalSections[i]);
    SystemParametersInfoA(
        SPI_SETSCREENSAVEACTIVE, g_GameWindowView.screenSaveActive,
        NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETLOWPOWERACTIVE, g_GameWindowView.lowPowerActive,
        NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETPOWEROFFACTIVE, g_GameWindowView.powerOffActive,
        NULL, SPIF_SENDCHANGE);
    WINNLSEnableIME(NULL, TRUE);

    {
        MainAllocationRegistryView *registry = g_MainAllocationRegistry;
        if (registry != NULL)
        {
            if (registry->hasEntries != 0)
            {
                for (i = 0; i < 0x2800; ++i)
                {
                    if (registry->entries[i] != NULL)
                        free(registry->entries[i]);
                }
            }
            delete registry;
        }
    }
    return 0;
}

// TH10_MAIN_FUNCTION: 0x0043A1B0 GameWindowView::ActivateWindow
// Retained HWND wrapper; WindowProc contains the same action inline.
void GameWindowView::ActivateWindow(HWND window)
{
    SetForegroundWindow(window);
}

// TH10_MAIN_FUNCTION: 0x004390E0 GameWindowView::WindowProc
// The target stores this standard four-argument callback in WNDCLASSA and all
// exits use RET 0x10; the extra decompiler `this` parameter is not physical.
LRESULT __stdcall GameWindowView::WindowProc(
    HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        ActivateWindow(window);
        break;
    case MM_MOM_DONE:
        if (g_MainSupervisorView.musicOwner != NULL)
        {
            static_cast<MidiOutputView *>(g_MainSupervisorView.musicOwner)
                ->UnprepareHeader(reinterpret_cast<MIDIHDR *>(lparam));
        }
        break;
    case WM_SETCURSOR:
        if (g_MainSupervisorView.windowed == 0 &&
            g_GameWindowView.windowIsInactive == 0)
        {
            ShowCursor(FALSE);
            SetCursor(NULL);
        }
        else
        {
            SetCursor(LoadCursorA(NULL, IDC_ARROW));
            ShowCursor(TRUE);
        }
        return 1;
    case WM_CLOSE:
        g_MainSupervisorView.flags |= MAIN_FLAG_CLOSE_REQUESTED;
        return 1;
    case WM_ERASEBKGND:
        return 1;
    case WM_ACTIVATEAPP:
        g_GameWindowView.windowIsInactive = wparam == 0;
        g_GameWindowView.windowIsActive = (int)wparam;
        break;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

// TH10_MAIN_FUNCTION: 0x004391F0 GameWindowView::Present
// Target 0x004391F0-0x004392DB owns Present/reset recovery and the bounded
// snapshot/th%.3d.bmp filename search.
void GameWindowView::Present()
{
    char screenshotPath[256];
    int screenshotIndex;

    if (MainD3dFailed(g_MainSupervisorView.d3dDevice->vtable->Present(
            g_MainSupervisorView.d3dDevice, NULL, NULL, NULL, NULL)))
    {
        MainReleaseAnmSurfaces(g_AnmRenderManagerView);
        g_MainSupervisorView.d3dDevice->vtable->Reset(
            g_MainSupervisorView.d3dDevice,
            &g_MainSupervisorView.presentParameters);
        ResetRenderState();
        g_MainSupervisorView.screenTransitionCountdown = 2;
    }

    MainServiceAnmScreenshots(g_AnmRenderManagerView);
    if ((g_MainInputEdges & 0x0800) != 0)
    {
        _mkdir("snapshot");
        for (screenshotIndex = 0; screenshotIndex < 1000; ++screenshotIndex)
        {
            sprintf(
                screenshotPath, "snapshot/th%.3d.bmp", screenshotIndex);
            if (!FileSystem::CheckIfFileAlreadyExists(screenshotPath))
                break;
        }
        if (screenshotIndex < 1000)
            MainTakeScreenshot(&g_MainSupervisorView, screenshotPath);
    }
}

// TH10_MAIN_FUNCTION: 0x00439390 GameWindowView::Render
// The physical target entry receives one GameWindowView pointer on the stack
// and ends in RET 4.
int __stdcall GameWindowView::Render(GameWindowView *window)
{
    int calcResult;

    window->currentTimestamp = GetTimestamp();
    if (window->currentTimestamp < window->lastTimestamp)
        window->lastFrameTime = window->currentTimestamp;
    window->lastTimestamp = window->currentTimestamp;

    if (window->currentTimestamp <= window->lastFrameTime)
    {
        Sleep(0);
        return MAIN_RENDER_KEEP_RUNNING;
    }

    do
    {
        window->lastFrameTime += 1.0 / 60.0;
    } while (window->lastFrameTime < window->currentTimestamp);

    g_AnmRenderManagerView->FlushVertexBuffer();
    g_MainSupervisorView.activeViewport =
        &g_MainSupervisorView.gameplayViewport;
    MainConfigureCamera(g_MainSupervisorView.activeViewport);
    g_MainSupervisorView.d3dDevice->vtable->SetViewport(
        g_MainSupervisorView.d3dDevice,
        &g_MainSupervisorView.activeViewport->viewport);
    g_MainSupervisorView.viewportConfigured = 1;

    calcResult = MainRunCalcChain(g_MainCallbackOwner);
    MainProcessSoundQueues(g_MainSoundOwner);
    if (calcResult == 0)
    {
        MainStopReplayScan();
        return MAIN_RENDER_EXIT;
    }
    if (calcResult == -1)
    {
        MainStopReplayScan();
        return MAIN_RENDER_RESTART;
    }

    ++window->framesSinceRedraw;
    if ((int)window->framesSinceRedraw >=
        (int)g_MainSupervisorView.frameskip + 1)
    {
        g_MainSupervisorView.d3dDevice->vtable->BeginScene(
            g_MainSupervisorView.d3dDevice);
        g_AnmRenderManagerView->spritesToDraw = 0;
        g_AnmRenderManagerView->vertexBufferEnd =
            g_AnmRenderManagerView->vertexBuffer;
        g_AnmRenderManagerView->vertexBufferStart =
            g_AnmRenderManagerView->vertexBuffer;
        g_MainSupervisorView.cachedFogEnabled = 0xff;
        StageDisableSupervisorFog(&g_MainSupervisorView);
        MainRunDrawChain(g_MainCallbackOwner);
        g_AnmRenderManagerView->FlushVertexBuffer();
        g_MainSupervisorView.d3dDevice->vtable->SetTexture(
            g_MainSupervisorView.d3dDevice, 0, NULL);
        g_MainSupervisorView.d3dDevice->vtable->EndScene(
            g_MainSupervisorView.d3dDevice);
        window->framesSinceRedraw = 0;
        Present();
    }

    g_MainSupervisorView.lastFrameDuration =
        GetTimestamp() - window->currentTimestamp;
    return MAIN_RENDER_KEEP_RUNNING;
}

// TH10_MAIN_FUNCTION: 0x00439540 GameWindowView::GetTimestamp
// Target 0x00439540-0x00439659 serializes access through critical section 5,
// uses QPC when available and otherwise converts unsigned timeGetTime values.
double GameWindowView::GetTimestamp()
{
    LARGE_INTEGER performanceCounterValue;
    double timestampValue;
    double timestamp;

    EnterCriticalSection(&g_MainSupervisorView.criticalSections[5]);
    ++g_MainSupervisorView.criticalSectionLockCounts[5];

    if (g_GameWindowView.performanceFrequency.QuadPart != 0)
    {
        QueryPerformanceCounter(&performanceCounterValue);
        timestampValue =
            (double)(performanceCounterValue.QuadPart -
                     g_GameWindowView.performanceStart.QuadPart) /
            (double)g_GameWindowView.performanceFrequency.QuadPart;
        if (g_GameWindowView.timeOrigin > timestampValue)
            g_GameWindowView.timeOrigin = timestampValue;
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[5]);
        --g_MainSupervisorView.criticalSectionLockCounts[5];
        return timestampValue - g_GameWindowView.timeOrigin;
    }

    timeBeginPeriod(1);
    timestamp = (double)timeGetTime();
    timeEndPeriod(1);
    if (g_GameWindowView.timeOrigin > timestamp)
        g_GameWindowView.timeOrigin = timestamp;
    timestampValue = g_GameWindowView.timeOrigin * 1000.0;
    timestampValue = (timestamp - timestampValue) / 1000.0;
    LeaveCriticalSection(&g_MainSupervisorView.criticalSections[5]);
    --g_MainSupervisorView.criticalSectionLockCounts[5];
    return timestampValue;
}

// TH10_MAIN_FUNCTION: 0x00439730 GameWindowView::CreateGameWindow
// WinMain supplies HINSTANCE in live-in EBX in the linked target. The natural
// source signature records its logical input while leaving exactness unknown.
int GameWindowView::CreateGameWindow(HINSTANCE instance)
{
    WNDCLASSA windowClass;
    int width;
    int height;

    memset(&windowClass, 0, sizeof(windowClass));
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = WindowProc;
    g_GameWindowView.windowIsActive = TRUE;
    g_GameWindowView.windowIsInactive = FALSE;
    windowClass.lpszClassName = "BASE";
    RegisterClassA(&windowClass);

    if (g_MainSupervisorView.windowed == 0)
    {
        width = 640;
        height = 480;
        g_GameWindowView.window = CreateWindowExA(
            0, "BASE", g_MainWindowTitle, WS_OVERLAPPEDWINDOW,
            0, 0, width, height, NULL, NULL, instance, NULL);
    }
    else
    {
        width = GetSystemMetrics(SM_CXDLGFRAME) * 2 + 640;
        height = GetSystemMetrics(SM_CYDLGFRAME) * 2 +
                 GetSystemMetrics(SM_CYCAPTION) + 480;
        g_GameWindowView.window = CreateWindowExA(
            0, "BASE", g_MainWindowTitle,
            WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, instance, NULL);
    }

    g_MainSupervisorView.gameWindow = g_GameWindowView.window;
    if (g_GameWindowView.window == NULL)
        return 1;

    SendMessageA(
        g_GameWindowView.window, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    Sleep(16);
    SendMessageA(
        g_GameWindowView.window, WM_SYSCOMMAND, SC_RESTORE, 0);
    return 0;
}

// TH10_MAIN_FUNCTION: 0x00439700 GameWindowView::InitD3DInterface
// The target keeps this externally visible helper while WinMain contains the
// same sequence inline. Its logger call uses a private LTCG register seam.
int GameWindowView::InitD3DInterface()
{
    g_MainSupervisorView.d3dInterface = Direct3DCreate9(MAIN_D3D_SDK_VERSION);
    if (g_MainSupervisorView.d3dInterface == NULL)
    {
        th10::g_GameErrorContext.Fatal(const_cast<char *>(g_MainD3dCreateError));
        return 1;
    }
    return 0;
}

// TH10_MAIN_FUNCTION: 0x00439890 GameWindowView::InitD3DRendering
// Target 0x00439890-0x00439D16 owns the complete D3D9 fallback ladder and
// publishes the 14-dword presentation parameters only after device creation.
int GameWindowView::InitD3DRendering()
{
    MainD3d9DisplayModeView displayMode;
    MainD3d9PresentParametersView presentParameters;
    AnmFloat3View eye;
    AnmFloat3View at;
    AnmFloat3View up;
    int failedToSetRefreshRate = 0;
    bool usingHardwareRenderer = 1;

    memset(&presentParameters, 0, sizeof(presentParameters));
    g_MainSupervisorView.d3dInterface->vtable->GetAdapterDisplayMode(
        g_MainSupervisorView.d3dInterface, 0, &displayMode);

    if (g_MainSupervisorView.windowed == 0)
    {
        if ((g_MainSupervisorView.options & 1) != 0)
        {
            presentParameters.backBufferFormat = MAIN_D3D_FORMAT_R5G6B5;
            g_MainSupervisorView.colorMode = 1;
        }
        else if (g_MainSupervisorView.colorMode == -1)
        {
            presentParameters.backBufferFormat = MAIN_D3D_FORMAT_X8R8G8B8;
            g_MainSupervisorView.colorMode = 0;
            th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3d32BitLog));
        }
        else
        {
            presentParameters.backBufferFormat =
                MAIN_D3D_FORMAT_X8R8G8B8 +
                (g_MainSupervisorView.colorMode != 0);
        }

        if (g_GameWindowView.usesRelativePath != 0)
            g_MainSupervisorView.disableVsync = 1;

        if (g_MainSupervisorView.disableVsync == 0)
        {
            presentParameters.fullScreenRefreshRate = 60;
            presentParameters.presentationInterval =
                MAIN_D3D_PRESENT_INTERVAL_ONE;
            presentParameters.swapEffect = MAIN_D3D_SWAP_DISCARD;
            th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dRefresh60Log));
        }
        else
        {
            presentParameters.fullScreenRefreshRate = 0;
            presentParameters.swapEffect = MAIN_D3D_SWAP_DISCARD;
            presentParameters.presentationInterval =
                (unsigned int)MAIN_D3D_PRESENT_INTERVAL_IMMEDIATE;
            th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dImmediateLog));
        }
    }
    else
    {
        presentParameters.backBufferFormat = displayMode.format;
        presentParameters.swapEffect = MAIN_D3D_SWAP_DISCARD;
        presentParameters.windowed = TRUE;
    }

    g_MainSupervisorView.flags |= MAIN_FLAG_LOCKABLE_BACKBUFFER;
    presentParameters.backBufferWidth = 640;
    presentParameters.backBufferHeight = 480;
    presentParameters.enableAutoDepthStencil = TRUE;
    presentParameters.autoDepthStencilFormat = MAIN_D3D_FORMAT_D16;
    presentParameters.flags = MAIN_D3D_PRESENT_LOCKABLE_BACKBUFFER;
    g_MainSupervisorView.couldSetRefreshRate = 1;

    for (;;)
    {
        if ((g_MainSupervisorView.options & 2) == 0)
        {
            if (!MainD3dFailed(
                    g_MainSupervisorView.d3dInterface->vtable->CreateDevice(
                        g_MainSupervisorView.d3dInterface, 0,
                        MAIN_D3D_DEVICE_HAL, g_GameWindowView.window,
                        MAIN_D3D_CREATE_HARDWARE_VERTEXPROCESSING,
                        &presentParameters,
                        &g_MainSupervisorView.d3dDevice)))
            {
                th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dHardwareLog));
                g_MainSupervisorView.flags |=
                    MAIN_FLAG_HARDWARE_VERTEX_PROCESSING;
                break;
            }
            if (failedToSetRefreshRate)
                th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dHardwareUnavailableLog));

            if (!MainD3dFailed(
                    g_MainSupervisorView.d3dInterface->vtable->CreateDevice(
                        g_MainSupervisorView.d3dInterface, 0,
                        MAIN_D3D_DEVICE_HAL, g_GameWindowView.window,
                        MAIN_D3D_CREATE_SOFTWARE_VERTEXPROCESSING,
                        &presentParameters,
                        &g_MainSupervisorView.d3dDevice)))
            {
                th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dSoftwareLog));
                g_MainSupervisorView.flags &=
                    ~MAIN_FLAG_HARDWARE_VERTEX_PROCESSING;
                break;
            }
            if (failedToSetRefreshRate)
                th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dSoftwareUnavailableLog));
        }

        if (!MainD3dFailed(
                g_MainSupervisorView.d3dInterface->vtable->CreateDevice(
                    g_MainSupervisorView.d3dInterface, 0,
                    MAIN_D3D_DEVICE_REF, g_GameWindowView.window,
                    MAIN_D3D_CREATE_SOFTWARE_VERTEXPROCESSING,
                    &presentParameters,
                    &g_MainSupervisorView.d3dDevice)))
        {
            th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dReferenceLog));
            g_MainSupervisorView.flags &=
                ~MAIN_FLAG_HARDWARE_VERTEX_PROCESSING;
            usingHardwareRenderer = 0;
            break;
        }

        if (g_MainSupervisorView.disableVsync == 0)
        {
            th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dRefreshFailedLog));
            presentParameters.fullScreenRefreshRate = 0;
            g_MainSupervisorView.couldSetRefreshRate = 0;
            failedToSetRefreshRate = 1;
            continue;
        }
        if (presentParameters.presentationInterval ==
            (unsigned int)MAIN_D3D_PRESENT_INTERVAL_IMMEDIATE)
        {
            th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dImmediateFailedLog));
            presentParameters.presentationInterval =
                MAIN_D3D_PRESENT_INTERVAL_ONE;
            presentParameters.swapEffect = MAIN_D3D_SWAP_COPY;
            continue;
        }

        th10::g_GameErrorContext.Fatal(const_cast<char *>(g_MainD3dFatalError));
        if (g_MainSupervisorView.d3dInterface != NULL)
        {
            g_MainSupervisorView.d3dInterface->vtable->Release(
                g_MainSupervisorView.d3dInterface);
            g_MainSupervisorView.d3dInterface = NULL;
        }
        return 1;
    }

    memcpy(
        &g_MainSupervisorView.presentParameters,
        &presentParameters, sizeof(presentParameters));

    at.x = 320.0f;
    at.y = -240.0f;
    at.z = 0.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    eye.x = 320.0f;
    eye.y = -240.0f;
    eye.z = -(240.0f / (float)tan(3.1415927f / 12.0));
    D3DXMatrixLookAtLH(
        &g_MainSupervisorView.viewMatrix, &eye, &at, &up);
    D3DXMatrixPerspectiveFovLH(
        &g_MainSupervisorView.projectionMatrix,
        3.1415927f / 6.0f, 4.0f / 3.0f, 100.0f, 10000.0f);
    g_MainSupervisorView.d3dDevice->vtable->SetTransform(
        g_MainSupervisorView.d3dDevice, MAIN_D3D_TRANSFORM_VIEW,
        &g_MainSupervisorView.viewMatrix);
    g_MainSupervisorView.d3dDevice->vtable->SetTransform(
        g_MainSupervisorView.d3dDevice, MAIN_D3D_TRANSFORM_PROJECTION,
        &g_MainSupervisorView.projectionMatrix);
    g_MainSupervisorView.d3dDevice->vtable->GetViewport(
        g_MainSupervisorView.d3dDevice, &g_MainSupervisorView.viewport);
    g_MainSupervisorView.d3dDevice->vtable->GetDeviceCaps(
        g_MainSupervisorView.d3dDevice, g_MainSupervisorView.d3dCaps);

    if ((g_MainSupervisorView.d3dCaps[0x90] & 0x40) == 0)
        th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dTextureOperationWarning));
    if (*reinterpret_cast<unsigned int *>(
            &g_MainSupervisorView.d3dCaps[0x58]) <= 0x100)
    {
        th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dTextureSizeWarning));
    }

    if ((g_MainSupervisorView.options & 1) == 0 && usingHardwareRenderer)
    {
        if (g_MainSupervisorView.d3dInterface->vtable->CheckDeviceFormat(
                g_MainSupervisorView.d3dInterface, 0,
                MAIN_D3D_DEVICE_HAL, displayMode.format, 0,
                MAIN_D3D_RESOURCE_TEXTURE,
                MAIN_D3D_FORMAT_A8R8G8B8) == 0)
        {
            g_MainSupervisorView.flags |= MAIN_FLAG_32BIT_TEXTURES;
        }
        else
        {
            g_MainSupervisorView.options |= 1;
            g_MainSupervisorView.flags &= ~MAIN_FLAG_32BIT_TEXTURES;
            th10::g_GameErrorContext.Log(const_cast<char *>(g_MainD3dTextureFormatWarning));
        }
    }

    ResetRenderState();
    MainSetScreenColor(0xff000000);
    g_GameWindowView.windowIsClosing = 0;
    g_MainSupervisorView.lastRenderTime = 0;
    return 0;
}

// TH10_MAIN_FUNCTION: 0x00439D20 GameWindowView::ResetRenderState
// All 33 D3D9 state writes below are target-observed in this order. The entry
// has no explicit argument; its initial PUSH ECX is one compiler scratch word.
void GameWindowView::ResetRenderState()
{
    float stateValue;
    AnmRenderManagerView *manager;

#define MAIN_SET_RENDER_STATE(state, value) \
    g_MainSupervisorView.d3dDevice->vtable->SetRenderState( \
        g_MainSupervisorView.d3dDevice, state, value)
#define MAIN_SET_TEXTURE_STAGE_STATE(state, value) \
    g_MainSupervisorView.d3dDevice->vtable->SetTextureStageState( \
        g_MainSupervisorView.d3dDevice, 0, state, value)
#define MAIN_SET_SAMPLER_STATE(state, value) \
    g_MainSupervisorView.d3dDevice->vtable->SetSamplerState( \
        g_MainSupervisorView.d3dDevice, 0, state, value)

    MAIN_SET_RENDER_STATE(0x07, 0x00000001);
    MAIN_SET_RENDER_STATE(0x89, 0x00000000);
    MAIN_SET_RENDER_STATE(0x16, 0x00000001);
    MAIN_SET_RENDER_STATE(0x1b, 0x00000001);
    MAIN_SET_RENDER_STATE(0x09, 0x00000002);
    MAIN_SET_RENDER_STATE(0x13, 0x00000005);
    MAIN_SET_RENDER_STATE(0x14, 0x00000006);
    MAIN_SET_RENDER_STATE(0x17, 0x00000008);
    MAIN_SET_RENDER_STATE(0x0f, 0x00000001);
    MAIN_SET_RENDER_STATE(0x18, 0x00000001);
    MAIN_SET_RENDER_STATE(0x19, 0x00000007);
    MAIN_SET_RENDER_STATE(0x1c, 0x00000001);
    stateValue = 1.0f;
    MAIN_SET_RENDER_STATE(
        0x26, *reinterpret_cast<unsigned int *>(&stateValue));
    MAIN_SET_RENDER_STATE(0x23, 0x00000000);
    MAIN_SET_RENDER_STATE(0x8c, 0x00000003);
    MAIN_SET_RENDER_STATE(0x22, 0xffa0a0a0);
    stateValue = 1000.0f;
    MAIN_SET_RENDER_STATE(
        0x24, *reinterpret_cast<unsigned int *>(&stateValue));
    stateValue = 5000.0f;
    MAIN_SET_RENDER_STATE(
        0x25, *reinterpret_cast<unsigned int *>(&stateValue));
    MAIN_SET_RENDER_STATE(0xa1, 0x00000000);

    MAIN_SET_TEXTURE_STAGE_STATE(0x04, 0x00000004);
    MAIN_SET_TEXTURE_STAGE_STATE(0x05, 0x00000002);
    MAIN_SET_TEXTURE_STAGE_STATE(0x06, 0x00000003);
    MAIN_SET_TEXTURE_STAGE_STATE(0x01, 0x00000004);
    MAIN_SET_TEXTURE_STAGE_STATE(0x02, 0x00000002);
    MAIN_SET_TEXTURE_STAGE_STATE(0x03, 0x00000003);
    MAIN_SET_TEXTURE_STAGE_STATE(0x18, 0x00000002);
    MAIN_SET_TEXTURE_STAGE_STATE(0x0b, 0x00000000);

    MAIN_SET_SAMPLER_STATE(0x07, 0x00000000);
    MAIN_SET_SAMPLER_STATE(0x05, 0x00000002);
    MAIN_SET_SAMPLER_STATE(0x06, 0x00000002);
    MAIN_SET_SAMPLER_STATE(0x03, 0x00000003);
    MAIN_SET_SAMPLER_STATE(0x01, 0x00000001);
    MAIN_SET_SAMPLER_STATE(0x02, 0x00000001);

    manager = g_AnmRenderManagerView;
    if (manager != NULL)
    {
        manager->currentBlendMode = 3;
        manager->currentColorOperation = 0xff;
        manager->currentVertexShader = 0xff;
        manager->currentTexture = NULL;
        manager->currentCameraMode = 0xff;
    }

#undef MAIN_SET_RENDER_STATE
#undef MAIN_SET_TEXTURE_STAGE_STATE
#undef MAIN_SET_SAMPLER_STATE
}

// TH10_MAIN_FUNCTION: 0x00439FF0 GameWindowView::CheckForRunningGameInstance
// The target entry has no explicit argument. It follows .lnk chains only when
// the startup-title path exists and records a differing launch path at +0x28.
int GameWindowView::CheckForRunningGameInstance()
{
    STARTUPINFOA startupInfo;
    char consoleTitle[264];
    char moduleFilename[264];
    char *extension;

    g_MainExclusiveMutex = CreateMutexA(NULL, TRUE, "Touhou 10 App");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        th10::g_GameErrorContext.Fatal(const_cast<char *>(g_MainAlreadyRunningError));
        return -1;
    }

    startupInfo.cb = sizeof(startupInfo);
    memset(
        &startupInfo.lpReserved, 0,
        sizeof(startupInfo) - offsetof(STARTUPINFOA, lpReserved));
    GetModuleFileNameA(NULL, moduleFilename, 0x105);
    GetConsoleTitleA(consoleTitle, 0x105);
    GetStartupInfoA(&startupInfo);

    if (startupInfo.lpTitle != NULL)
    {
        extension = strrchr(startupInfo.lpTitle, '.');
        if (FileSystem::CheckIfFileAlreadyExists(startupInfo.lpTitle) &&
            extension != NULL)
        {
            if (_stricmp(extension, ".lnk") == 0)
            {
                do
                {
                    ResolveShortcut(startupInfo.lpTitle, consoleTitle, 0x104);
                    extension = strrchr(consoleTitle, '.');
                } while (_stricmp(extension, ".lnk") == 0);
            }
            else
            {
                strcpy(consoleTitle, startupInfo.lpTitle);
            }

            if (strcmp(moduleFilename, consoleTitle) != 0)
                g_GameWindowView.usesRelativePath = 1;
        }
        g_MainSupervisorView.flags &= ~MAIN_FLAG_NO_STARTUP_TITLE;
    }
    else
    {
        g_MainSupervisorView.flags |= MAIN_FLAG_NO_STARTUP_TITLE;
    }
    return (g_MainExclusiveMutex != NULL) - 1;
}

// TH10_MAIN_FUNCTION: 0x0043A290 GameWindowView::ResolveShortcut
// The sole startup caller supplies destination/capacity through LTCG's private
// EBX/EDI convention. The ordinary declaration keeps all three source inputs.
int __stdcall GameWindowView::ResolveShortcut(
    const char *shortcutPath, char *destination, int destinationSize)
{
    HRESULT result;
    IPersistFile *persistFile;
    IShellLinkA *shellLink;
    int returnValue;
    WIN32_FIND_DATAA findData;
    LPWSTR widePath;

    if (destination == NULL)
        return FALSE;

    returnValue = FALSE;
    CoInitialize(NULL);
    result = CoCreateInstance(
        CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkA,
        reinterpret_cast<void **>(&shellLink));
    if (SUCCEEDED(result))
    {
        result = shellLink->QueryInterface(
            IID_IPersistFile, reinterpret_cast<void **>(&persistFile));
        if (SUCCEEDED(result))
        {
            widePath = new WCHAR[destinationSize];
            MultiByteToWideChar(
                CP_ACP, 0, shortcutPath, -1, widePath, destinationSize);
            result = persistFile->Load(widePath, STGM_READ);
            if (SUCCEEDED(result))
            {
                result = shellLink->GetPath(
                    destination, destinationSize, &findData, 0);
                if (SUCCEEDED(result))
                    returnValue = TRUE;
            }
            delete widePath;
            persistFile->Release();
        }
        shellLink->Release();
    }
    CoUninitialize();
    return returnValue;
}

// TH10_MAIN_FUNCTION: 0x0043A3A0 MainConfigurationDialogProc
// Four-argument dialog callback; the target returns zero on every path.
INT_PTR CALLBACK MainConfigurationDialogProc(
    HWND dialog, UINT message, WPARAM wparam, LPARAM lparam)
{
    (void)lparam;
    switch (message)
    {
    case WM_INITDIALOG:
        SendMessageA(GetDlgItem(dialog, 202), BM_SETCHECK, BST_CHECKED, 0);
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wparam))
        {
        case 201:
            if (IsDlgButtonChecked(dialog, 202) == BST_CHECKED)
                g_MainSupervisorView.options |= 0x100;
            else
                g_MainSupervisorView.options &= ~0x100u;
            EndDialog(dialog, IDYES);
            break;
        case 203:
            if (IsDlgButtonChecked(dialog, 202) == BST_CHECKED)
                g_MainSupervisorView.options |= 0x100;
            else
                g_MainSupervisorView.options &= ~0x100u;
            EndDialog(dialog, IDNO);
            break;
        }
        break;
    }
    return 0;
}

// TH10_MAIN_FUNCTION: 0x0043A1C0 GameWindowView::CalcExecutableChecksum
// The target sums every complete dword except the final dword and publishes
// both the byte size and checksum for later version/replay validation.
int GameWindowView::CalcExecutableChecksum()
{
    char moduleFilename[264];
    unsigned int *fileData;
    unsigned int checksum;
    int fileSize;
    unsigned int *cursor;
    int remaining;

    if (GetModuleFileNameA(NULL, moduleFilename, 0x105) != 0)
    {
        checksum = 0;
        cursor = reinterpret_cast<unsigned int *>(
            FileSystem::OpenFile(moduleFilename, &fileSize, 1));
        fileData = cursor;
        if (cursor == NULL)
            return -1;

        for (remaining = fileSize / 4 - 1; remaining > 0; --remaining)
            checksum += *cursor++;

        free(fileData);
        g_MainExecutableSize = fileSize;
        g_MainExecutableChecksum = checksum;
        return checksum;
    }
    return -1;
}

// Target 0x004134B0-0x004135C7 is TH10's half-second FPS sampler.  The
// adjacent game family supplies only the descriptive owner name; every field,
// threshold, timer reset, and gameplay-sampling gate below is visible in TH10.
#pragma pack(push, 4)
struct FpsSampleGateView
{
    unsigned char unknown000[0x58];
    unsigned int flags58;
};

struct FpsCounterView
{
    unsigned char unknown000[0x14];
    double lastFpsTimestamp;       // +0x14
    int clockAnomalyCount;         // +0x1C
    unsigned int fpsFrameCount;    // +0x20
    double lagNumerator;           // +0x24
    double lagDenominator;         // +0x2C
    float currentFps;              // +0x34
    unsigned char unknown038[0x54];

    int CalculateFps();
    int DrawFpsCounter();
};
#pragma pack(pop)

typedef char FpsCounterViewSizeIs8C[
    (sizeof(FpsCounterView) == 0x8c) ? 1 : -1];
typedef char FpsCounterLastTimestampAt14[
    (offsetof(FpsCounterView, lastFpsTimestamp) == 0x14) ? 1 : -1];
typedef char FpsCounterFrameCountAt20[
    (offsetof(FpsCounterView, fpsFrameCount) == 0x20) ? 1 : -1];
typedef char FpsCounterCurrentFpsAt34[
    (offsetof(FpsCounterView, currentFps) == 0x34) ? 1 : -1];

extern FpsSampleGateView *g_FpsSampleGateView;
extern int g_FpsDisplayMode;

int FpsCounterView::CalculateFps()
{
    double currentTime;
    double elapsed;

    currentTime = GameWindowView::GetTimestamp();
    if (lastFpsTimestamp > currentTime)
        lastFpsTimestamp = currentTime;

    if (currentTime - lastFpsTimestamp >= 0.5)
    {
        elapsed = currentTime - lastFpsTimestamp;
        lastFpsTimestamp += elapsed;
        currentFps = (float)((double)fpsFrameCount / elapsed);

        if (currentFps > 65.0f)
        {
            ++clockAnomalyCount;
            if (clockAnomalyCount == 2)
            {
                g_GameWindowView.lastTimestamp =
                    g_GameWindowView.currentTimestamp =
                    g_GameWindowView.lastFrameTime =
                    g_GameWindowView.timeOrigin = GameWindowView::GetTimestamp();
            }
            else if (clockAnomalyCount == 4)
            {
                g_GameWindowView.performanceFrequency.QuadPart = 0;
                g_GameWindowView.lastTimestamp =
                    g_GameWindowView.currentTimestamp =
                    g_GameWindowView.lastFrameTime =
                    g_GameWindowView.timeOrigin = GameWindowView::GetTimestamp();
                clockAnomalyCount = 0;
            }
        }
        else
        {
            clockAnomalyCount = 0;
        }

        FpsSampleGateView *gate = g_FpsSampleGateView;
        if (gate != NULL)
        {
            if ((gate->flags58 & 0x14u) == 0)
            {
                lagDenominator += 60.0;
                if (currentFps > 57.0f)
                    lagNumerator += 60.0;
                else
                    lagNumerator += currentFps;
            }
            gate->flags58 &= ~0x80u;
        }
        fpsFrameCount = 0;
    }
    return 1;
}

// Target 0x004135D0-0x00413683 is the paired FPS display owner.  It is kept
// source-present because it is the real production caller context for the exact
// CalculateFps/GetTimestamp codegen.  In the retained draw-adapter plus
// AnmManager support graph this 180-byte owner is also canonical exact.
int FpsCounterView::DrawFpsCounter()
{
    CalculateFps();
    if (g_FpsDisplayMode != 14)
    {
        AsciiManagerView *asciiManager = g_AsciiManagerView;
        float fps = currentFps;
        if (asciiManager != NULL)
        {
            unsigned int color;
            if (fps < 30.0f)
                color = 0xff5050ffu;
            else if (fps < 40.0f)
                color = 0xffa0a0ffu;
            else
                color = 0xffffffffu;

            asciiManager->color = color;
            AnmFloat3View position(590.0f, 470.0f, 0.0f);
            asciiManager->AddSmallFormatText(
                &position, "%2.1ffps", fps);
            g_AsciiManagerView->color = 0xffffffffu;
        }
    }
    fpsFrameCount += (unsigned int)g_MainSupervisorView.frameskip + 1u;
    return 1;
}

// Target 0x00413690-0x00413699 is the retained draw-chain adapter.  Its
// source-written versus compiler/LTCG-generated provenance is still unresolved;
// this maintained spelling is used only to recreate the observed optimizer
// entry context and is not itself promoted as authored/exact.
int __fastcall FpsCounterDrawCallback(FpsCounterView *counter)
{
    return counter->DrawFpsCounter();
}
// Target 0x004392E0-0x00439343 saves/disables the three OS power states and
// initializes the high-resolution timer. WinMain directly calls this owner.
void MainInitializeSystemParameters()
{
    SystemParametersInfoA(
        SPI_GETSCREENSAVEACTIVE, 0, &g_GameWindowView.screenSaveActive, 0);
    SystemParametersInfoA(
        SPI_GETLOWPOWERACTIVE, 0, &g_GameWindowView.lowPowerActive, 0);
    SystemParametersInfoA(
        SPI_GETPOWEROFFACTIVE, 0, &g_GameWindowView.powerOffActive, 0);
    SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETLOWPOWERACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETPOWEROFFACTIVE, 0, NULL, SPIF_SENDCHANGE);
    QueryPerformanceFrequency(&g_GameWindowView.performanceFrequency);
    QueryPerformanceCounter(&g_GameWindowView.performanceStart);
}

// Target 0x00439350-0x0043938D is a retained system-state restore helper.
// WinMain also owns an inlined copy; this independent CC-delimited body has no
// direct target caller and is retained without inventing one.
void MainRestoreSystemParameters()
{
    SystemParametersInfoA(
        SPI_SETSCREENSAVEACTIVE, g_GameWindowView.screenSaveActive,
        NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETLOWPOWERACTIVE, g_GameWindowView.lowPowerActive,
        NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETPOWEROFFACTIVE, g_GameWindowView.powerOffActive,
        NULL, SPIF_SENDCHANGE);
    WINNLSEnableIME(NULL, TRUE);
}

// Target 0x00439660-0x004396F7 computes the unlocked form of the window timer:
// QPC when available, timeGetTime otherwise, then clamps and subtracts origin.
double MainGetTimestampUnlocked()
{
    double timestamp;
    if (g_GameWindowView.performanceFrequency.QuadPart != 0)
    {
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        timestamp =
            (double)(current.QuadPart -
                     g_GameWindowView.performanceStart.QuadPart) /
            (double)g_GameWindowView.performanceFrequency.QuadPart;
    }
    else
    {
        timeBeginPeriod(1);
        timestamp = (double)timeGetTime();
        timeEndPeriod(1);
        timestamp *= 0.001;
    }
    if (g_GameWindowView.timeOrigin > timestamp)
        g_GameWindowView.timeOrigin = timestamp;
    return timestamp - g_GameWindowView.timeOrigin;
}

// Slot +0xAC: observed seven stack arguments including the device receiver.
// The opaque rectangle argument is always NULL in both target sites.
typedef long (__stdcall *MainD3d9ClearForScreenColor)(
    MainD3d9DeviceView *, unsigned int, const void *, unsigned int,
    unsigned int, float, unsigned int);

// Target 43BC40 / 135 bytes. The target's private color input is ESI.
// Ordinary source argument below lets /GL choose its internal convention;
// no artificial register binding or inline-control attribute is requested.
// Both Present calls occur unconditionally; Reset occurs on signed failure.
static void MainClearPresentTwice(unsigned int color)
{
    MainD3d9DeviceView *device = g_MainSupervisorView.d3dDevice;
    reinterpret_cast<MainD3d9ClearForScreenColor>(device->vtable->slot0AC)(
        device, 0, NULL, 3, color, 1.0f, 0);
    device = g_MainSupervisorView.d3dDevice;
    if (device->vtable->Present(device, NULL, NULL, NULL, NULL) < 0)
    {
        device = g_MainSupervisorView.d3dDevice;
        device->vtable->Reset(device, &g_MainSupervisorView.presentParameters);
    }

    device = g_MainSupervisorView.d3dDevice;
    reinterpret_cast<MainD3d9ClearForScreenColor>(device->vtable->slot0AC)(
        device, 0, NULL, 3, color, 1.0f, 0);
    device = g_MainSupervisorView.d3dDevice;
    if (device->vtable->Present(device, NULL, NULL, NULL, NULL) < 0)
    {
        device = g_MainSupervisorView.d3dDevice;
        device->vtable->Reset(device, &g_MainSupervisorView.presentParameters);
    }
}

// Target43BCD0 /108 bytes. The exposed stack contract is one DWORD, RET4.
// No incoming register is consumed. Original source ownership/name unknown.
void __stdcall MainSetScreenColor(unsigned int color)
{
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    if (manager != NULL)
        manager->FlushVertexBuffer();

    // Device capture precedes viewport writes in the target.
    MainD3d9DeviceView *device = g_MainSupervisorView.d3dDevice;
    g_MainSupervisorView.viewport.x = 0;
    g_MainSupervisorView.viewport.y = 0;
    g_MainSupervisorView.viewport.width = 640;
    g_MainSupervisorView.viewport.height = 480;
    g_MainSupervisorView.viewport.minZ = 0.0f;
    g_MainSupervisorView.viewport.maxZ = 1.0f;
    device->vtable->SetViewport(device, &g_MainSupervisorView.viewport);
    MainClearPresentTwice(color);
}
