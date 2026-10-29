#include "Main.hpp"
#include "Sound.hpp"
#include "Chain.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GameStatistics.hpp"
#include <string.h>

// Partial view of the owner published at477820. Names are descriptive; the
// startup callback ignores the argument supplied by its thread launcher.
struct StartupWorkerView
{
    unsigned char unknown000[8];
    th10::ChainElem *calcElement;
    th10::ChainElem *drawElement;
    unsigned char unknown010[0x3e0 - 0x10];
    AnmLoadedView *signatureAnm;
    int signatureReady;
    int textReady;

    static int StartupThread();
};

typedef char StartupWorkerSignatureAt3E0[
    (offsetof(StartupWorkerView, signatureAnm) == 0x3e0) ? 1 : -1];

extern StartupWorkerView *g_StartupWorker;
extern AnmLoadedView *g_FrontEndMusicCommentAnm;
extern int g_GameTransitionState;
extern AsciiManagerView *AsciiManagerCreate();
extern GameStatisticsOwner *CreateStatistics42AF20();
int FinalizeStartupResources();

char g_StartupTextError[] =
    "error : \x95\xb6\x8e\x9a\x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n";
char g_StartupBgmError[] =
    "error : BGM \x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n";

// Target41F990..41FABA, plain RET. The thread API declaration is not used to
// infer a WINAPI callback ABI or a return value consumed by that API.
int StartupWorkerView::StartupThread()
{
    StartupWorkerView *worker = g_StartupWorker;
    worker->signatureAnm = g_AnmRenderManagerView->PreloadAnm(1, "sig.anm");
    if (worker->signatureAnm == NULL)
        goto fail;
    worker->drawElement->flags |= 2;
    worker->signatureReady = 1;
    if (AsciiManagerCreate() == NULL)
    {
        th10::g_GameErrorContext.Log(g_StartupTextError);
        goto fail;
    }
    worker->textReady = 1;
    g_FrontEndMusicCommentAnm =
        g_AnmRenderManagerView->PreloadAnm(0, "text.anm");
    if (g_FrontEndMusicCommentAnm == NULL)
        goto fail;
    reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->bgmFormats =
        reinterpret_cast<ThBgmFormat *>(
            FileSystem::OpenFile("../../bgm/thbgm.fmt", NULL, 0));
    if (reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->bgmFormats == NULL)
        th10::g_GameErrorContext.Log(g_StartupBgmError);
    reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->PreparePlayback();
    if (FileSystem::CheckIfFileAlreadyExists("thbgm.dat"))
    {
        if ((g_MainSupervisorView.options & 0x10) == 0)
            reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->OpenDiskBgm("thbgm.dat");
        else
            strcpy(reinterpret_cast<SoundPlayerView *>(g_MainSoundOwner)->unknownNameSlot,
                   "thbgm.dat");
    }
    FinalizeStartupResources();
    worker->calcElement->flags |= 2;
    CreateStatistics42AF20();
    return 0;

fail:
    g_GameTransitionState = 3;
    worker->calcElement->flags |= 2;
    return 0;
}

char g_StartupFrontError[] =
    "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n";
char g_StartupBulletError[] =
    "\x93\x47\x92\x65\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n";

// Target41F8D0..41F921. Each failed preload emits its own original diagnostic.
int FinalizeStartupResources()
{
    if (g_AnmRenderManagerView->PreloadAnm(6, "front.anm") == NULL)
    {
        th10::g_GameErrorContext.Log(g_StartupFrontError);
        return -1;
    }
    if (g_AnmRenderManagerView->PreloadAnm(7, "bullet.anm") == NULL)
    {
        th10::g_GameErrorContext.Log(g_StartupBulletError);
        return -1;
    }
    return 0;
}
