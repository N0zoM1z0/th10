#include "Main.hpp"
#include "Sound.hpp"
#include "Chain.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GameStatistics.hpp"
#include "ScriptViewer.hpp"
#include "Startup.hpp"
#include <process.h>
#include <stdlib.h>
#include <string.h>

extern AnmLoadedView *g_FrontEndMusicCommentAnm;
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
        if ((g_MainSupervisorView.configuration.options & 0x10) == 0)
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
    g_MainSupervisorView.nextGameMode = 3;
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


// Target41F850..41F8CF. Member construction precedes the full-owner clear.
StartupWorkerView::StartupWorkerView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2;
    g_StartupWorker = this;
}

// Target41FAC0..41FB4B. The target expands the worker launch protocol here.
int StartupWorkerView::RegisterTasks()
{
    th10::ChainElem *element = th10::Chain::AllocElem(reinterpret_cast<th10::ChainCallback>(OnUpdate));
    element->flags &= ~2u;
    element->arg = this;
    th10::g_Chain->AddToCalcChain(element, 3);
    calcElement = element;
    element = th10::Chain::AllocElem(reinterpret_cast<th10::ChainCallback>(OnDraw));
    element->flags &= ~2u;
    element->arg = this;
    th10::g_Chain->AddToDrawChain(element, 2);
    drawElement = element;
    ScriptViewerWorkerView &threadWorker = worker;
    threadWorker.Stop();
    threadWorker.entry018 = reinterpret_cast<void *>(StartupThread);
    threadWorker.running = 1;
    threadWorker.stopRequested = 0;
    threadWorker.thread = reinterpret_cast<void *>(_beginthreadex(
        NULL, 0, reinterpret_cast<unsigned (__stdcall *)(void *)>(StartupThread),
        this, 0, &threadWorker.threadId));
    return 0;
}

// Target41FD00..41FD44. RegisterTasks is still called after a null allocation.
StartupWorkerView *CreateStartupWorker()
{
    StartupWorkerView *owner = new StartupWorkerView;
    if (owner->RegisterTasks() != 0)
    {
        delete owner;
        return NULL;
    }
    return owner;
}

// Target41F930..41F98A: matching front/bullet preloads are slots6/7.
int ReleaseStartupResources()
{
    g_AnmRenderManagerView->ReleaseAnm(6);
    g_AnmRenderManagerView->ReleaseAnm(7);
    return 0;
}

static void RemoveStartupElement(th10::ChainElem *element, th10::Chain *chain)
{
    if (element == NULL)
        return;
    EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
    ++g_MainSupervisorView.criticalSectionLockCounts[0];
    chain->UnregisterElem(element);
    LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
    --g_MainSupervisorView.criticalSectionLockCounts[0];
}

// Target41FB50..41FCF0, including the automatic VM/worker member cleanup.
StartupWorkerView::~StartupWorkerView()
{
    worker.Stop();
    RemoveStartupElement(calcElement, th10::g_Chain);
    RemoveStartupElement(drawElement, th10::g_Chain);
    ReleaseStartupResources();
    g_AnmRenderManagerView->ReleaseAnm(1);
    AsciiManagerView *ascii = g_AsciiManagerView;
    g_StartupWorker = NULL;
    if (ascii != NULL)
        delete ascii;
    g_AnmRenderManagerView->ReleaseAnm(0);
    GameFinalizeStatistics42B1E0(g_GameStatisticsRows);
    delete g_GameStatisticsRows;
    g_GameStatisticsRows = NULL;
}

// Direct callback entries41FEB0/41FEF0 are not present in Ghidra's inventory.
int __fastcall StartupWorkerView::OnUpdate(StartupWorkerView *owner)
{
    if ((owner->flags & 2) != 0)
    {
        static_cast<th10::ChainElem *>(g_AsciiManagerView->calcChainElement)->flags |= 2;
        static_cast<th10::ChainElem *>(g_AsciiManagerView->drawChainElement0)->flags |= 2;
        static_cast<th10::ChainElem *>(g_AsciiManagerView->drawChainElement)->flags |= 2;
        g_MainSupervisorView.flags &= ~0x1000u;
        g_MainSupervisorView.nextGameMode = 4;
        owner->flags &= ~2u;
    }
    return 1;
}

int __fastcall StartupWorkerView::OnDraw(StartupWorkerView *owner)
{
    return owner->Draw();
}

// Target41FDD0..41FEAB: resource capture precedes allocation and VM setup.
int __stdcall StartupWorkerView::Draw()
{
    if (signatureReady == 1)
    {
        AnmLoadedView *loaded = signatureAnm;
        AnmVmView *created = g_AnmRenderManagerView->AllocateVm();
        created->renderLayer = 15;
        created->useAlternateRng = 1;
        loaded->InitializeVm(created, 0);
        signatureVmId = g_AnmRenderManagerView->AddVmVariant0(created).value;
        ++signatureReady;
    }
    if (textReady == 1)
    {
        AnmFloat3View position(480.0f, 392.0f, 0.0f);
        int *id = &g_AsciiManagerView->auxiliaryVm89A4;
        if (*id == 0)
            *id = g_AsciiManagerView->asciiAnm->CreateVmAtScreenVariant0(6, &position).value;
        ++textReady;
    }
    ++frames;
    return 1;
}
