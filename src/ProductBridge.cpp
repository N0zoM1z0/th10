// Product-build bridge for TH10 v1.00a whole-program link.
//
// Wires:
// 1. Placeholder-named cross-TU declarations to their already-reconstructed
//    exact owners in src/.
// 2. The 4 small runtime callbacks (0x00420000, 0x004200D0, 0x0042BC30,
//    0x0042BE10) referenced by SupervisorModes.cpp and SptTest.cpp.
// 3. Virtual method dispatch for EnemyLaserConstructionBaseView,
//    EnemyLaserType0StorageView, and EnemyLaserType1StorageView to their
//    exact implementations in Laser*.cpp / EnemyLaser.cpp / DecomphelpLeafAccessors.cpp.
// 4. Cancel-effect helpers used by EnemyCancelManagerView::CancelAllBullets.

#include <math.h>
#include <stdlib.h>

#include "AnmManager.hpp"
#include "BulletManager.hpp"
#include "EnemyGameState.hpp"
#include "EnemyLaser.hpp"
#include "EnemyLaserLine.hpp"
#include "EnemyLaserType1.hpp"
#include "GameUiVmInterrupts.hpp"
#include "ItemManager.hpp"
#include "Player.hpp"
#include "ScriptViewerOwners.hpp"
#include "StageScript.hpp"

struct MainSupervisorView;
struct SptTestView
{
    void ClearScriptNames();
    int RegisterTasks();
};
struct ScriptViewerWorkerView
{
    void Stop() throw();
};
namespace th10
{
extern unsigned char g_ChainNestCounter;
} // namespace th10
extern SptTestView *g_SptTestView;
struct EnemyCriticalSectionView;
extern EnemyCriticalSectionView g_EnemyCallbackCriticalSection;

// ---------------------------------------------------------------------------
// External declarations for already-reconstructed exact owners
// ---------------------------------------------------------------------------
extern void *ScriptViewerCreateBomb();
extern BulletManagerView *CreateBulletManager();
extern EnemyGameStateView *CreateEnemyGameState();
extern int GameInitializeRuntimeSystems();
extern ScriptViewerPopupOwnerView *GameCreateOwner42B660();
extern void TextReleaseFontResources();
extern unsigned short InputReadWinmmJoystickCapabilities();
extern "C" void ClearKeyboardHighBits();

namespace th10
{
namespace leaf
{
extern int Fn004200C0();
extern void Fn0041BEF0();
extern int Fn0041BF10();
extern int Fn0041BF20();
extern void Fn0041BF90();
extern void Fn0041BFA0();
extern void Fn0041BFB0();
extern void Fn0041BFC0();
extern void Fn0041BFD0();
extern void Fn0041BFE0();
extern void Fn0041BFF0();
extern void Fn0041C000();
extern void Fn0041C010();
extern void Fn0041C020();
extern int Fn0041D870();
extern int Fn0041EAF0();
} // namespace leaf
} // namespace th10

// Globals used by the 4 runtime callbacks
extern AnmRenderManagerView *g_AnmRenderManagerView;
extern D3d9DeviceView *g_Direct3DDevice;
extern int g_StageScriptGlobal4923A8;
extern float g_TransitionShakeX;
extern float g_TransitionShakeY;
extern SptTestView *g_SptTestView;
extern ItemManagerView *g_ItemManager;

struct GameChainOwnerView;
struct GameAdditionalChainView;
struct EnemyAnimationOwnerView;

// ---------------------------------------------------------------------------
// 1. Placeholder-named functions forwarded to exact owners
// ---------------------------------------------------------------------------
void __stdcall GameCleanupLoader405620(GameChainOwnerView *owner)
{
    reinterpret_cast<ScriptViewerBombOwnerView *>(owner)->~ScriptViewerBombOwnerView();
}

void *GameCreateLoaderOwner()
{
    return ScriptViewerCreateBomb();
}

void __stdcall GameCleanupAnimation405F70(GameChainOwnerView *owner)
{
    reinterpret_cast<BulletManagerView *>(owner)->~BulletManagerView();
}

void *GameCreateProjectilePool()
{
    return CreateBulletManager();
}

void *ScriptViewerCreateBullets()
{
    return CreateBulletManager();
}

void __stdcall GameCleanupOwner408AF0(GameAdditionalChainView *owner)
{
    reinterpret_cast<EnemyGameStateView *>(owner)->~EnemyGameStateView();
}

void *GameCreateOwner408C90()
{
    return CreateEnemyGameState();
}

int MainDrawLateCallback()
{
    return static_cast<int>(th10::leaf::Fn004200C0());
}

int __fastcall MainAddedCallback(MainSupervisorView *)
{
    return GameInitializeRuntimeSystems();
}

void __stdcall GameStartSoundState(unsigned char *owner)
{
    GameUiSendVmInterrupt1(reinterpret_cast<GameUiVmTransitionView *>(owner));
}

void __stdcall GameStopSoundState(unsigned char *owner)
{
    GameUiSendVmInterrupt2(reinterpret_cast<GameUiVmTransitionView *>(owner));
}

void __stdcall GameCleanupOwner42B570(GameChainOwnerView *owner)
{
    reinterpret_cast<ScriptViewerPopupOwnerView *>(owner)->~ScriptViewerPopupOwnerView();
}

// GameManager.cpp declares `extern void *GameCreateOwner42B660();` whereas
// ScriptViewerPopupCreate.cpp defines `ScriptViewerPopupOwnerView *GameCreateOwner42B660()`.
// ScriptViewerCreatePopups forwards to the same owner.
void *ScriptViewerCreatePopups()
{
    return GameCreateOwner42B660();
}

void MainReleaseTextResources()
{
    TextReleaseFontResources();
}

void *__fastcall EnemyLoadAnimationResource(
    int index, EnemyAnimationOwnerView *owner, const char *path)
{
    return reinterpret_cast<AnmRenderManagerView *>(owner)->PreloadAnm(index, path);
}

void MainInitializeControllerCaps()
{
    InputReadWinmmJoystickCapabilities();
}

void MainResetInput()
{
    ClearKeyboardHighBits();
}

// ---------------------------------------------------------------------------
// 2. Runtime callbacks (0x00420000, 0x004200D0, 0x0042BC30, 0x0042BE10)
// ---------------------------------------------------------------------------
// 0x00420000: MainDrawCallback
int __fastcall MainDrawCallback(MainSupervisorView *supervisor)
{
    unsigned char *anm = reinterpret_cast<unsigned char *>(g_AnmRenderManagerView);
    *reinterpret_cast<unsigned int *>(anm + 0x3ada70) = 0;
    *reinterpret_cast<unsigned int *>(anm + 0x3ada64) = 0;
    anm[0x3ada69] = 0xff;
    anm[0x3ada68] = 3;
    anm[0x3ada6b] = 0xff;
    anm[0x3ada6c] = 0xff;
    *reinterpret_cast<unsigned int *>(anm + 0x73245c) = 0;
    *reinterpret_cast<unsigned int *>(anm + 0x732458) = 0x80808080u;
    anm[0x3ada6e] = 0xff;
    *reinterpret_cast<unsigned int *>(anm + 0x60) = 0;
    *reinterpret_cast<unsigned int *>(anm + 0x5c) = 0;
    anm[0x3ada6a] = 0xff;

    unsigned char *supBytes = reinterpret_cast<unsigned char *>(supervisor);
    StageCameraStateView *camera = reinterpret_cast<StageCameraStateView *>(supBytes + 0x26c);
    *reinterpret_cast<StageCameraStateView **>(supBytes + 0x384) = camera;
    camera->ConfigureCamera();

    // IDirect3DDevice9::SetViewport (vtable +0xBC)
    void *device = *reinterpret_cast<void **>(supBytes + 8);
    void **vtbl = *reinterpret_cast<void ***>(device);
    typedef long(__stdcall *SetViewportFn)(void *, const void *);
    reinterpret_cast<SetViewportFn>(vtbl[0xbc / 4])(
        device,
        reinterpret_cast<const unsigned char *>(*reinterpret_cast<StageCameraStateView **>(supBytes + 0x384)) + 0xcc);

    *reinterpret_cast<unsigned int *>(supBytes + 0x388) = 1;

    // IDirect3DDevice9::Clear (vtable +0xAC)
    void *d3dDev = g_Direct3DDevice;
    void **d3dVtbl = *reinterpret_cast<void ***>(d3dDev);
    typedef long(__stdcall *ClearFn)(void *, unsigned long, const void *, unsigned long, unsigned long, float, unsigned long);
    reinterpret_cast<ClearFn>(d3dVtbl[0xac / 4])(
        d3dDev, 0, NULL, 1, static_cast<unsigned long>(g_StageScriptGlobal4923A8), 1.0f, 0);
    return 1;
}

// 0x004200D0: MainDrawFinalCallback
int MainDrawFinalCallback()
{
    g_AnmRenderManagerView->FlushVertexBuffer();
    g_TransitionShakeX = 0.0f;
    g_TransitionShakeY = 0.0f;
    return 1;
}

// 0x0042BC30: SptTestCleanupOwner42BC30
void __stdcall SptTestCleanupOwner42BC30(SptTestView *view)
{
    view->ClearScriptNames();
    unsigned char *bytes = reinterpret_cast<unsigned char *>(view);

    th10::ChainElem *elem0 = *reinterpret_cast<th10::ChainElem **>(bytes + 8);
    if (elem0 != NULL)
    {
        EnterCriticalSection(&g_EnemyCallbackCriticalSection);
        ++th10::g_ChainNestCounter;
        th10::g_Chain->UnregisterElem(elem0);
        LeaveCriticalSection(&g_EnemyCallbackCriticalSection);
        --th10::g_ChainNestCounter;
    }

    th10::ChainElem *elem1 = *reinterpret_cast<th10::ChainElem **>(bytes + 0xc);
    if (elem1 != NULL)
    {
        EnterCriticalSection(&g_EnemyCallbackCriticalSection);
        ++th10::g_ChainNestCounter;
        th10::g_Chain->UnregisterElem(elem1);
        LeaveCriticalSection(&g_EnemyCallbackCriticalSection);
        --th10::g_ChainNestCounter;
    }

    void *buf = *reinterpret_cast<void **>(bytes + 0x2e0);
    if (buf != NULL)
    {
        free(buf);
        *reinterpret_cast<void **>(bytes + 0x2e0) = NULL;
    }

    void *obj2d8 = *reinterpret_cast<void **>(bytes + 0x2d8);
    if (obj2d8 != NULL)
    {
        void *inner = *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(obj2d8) + 0x8c);
        if (inner != NULL)
        {
            free(inner);
            *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(obj2d8) + 0x8c) = NULL;
        }
        operator delete(obj2d8);
        *reinterpret_cast<void **>(bytes + 0x2d8) = NULL;
    }

    void *obj2dc = *reinterpret_cast<void **>(bytes + 0x2dc);
    if (obj2dc != NULL)
    {
        struct DeletingDtorView
        {
            virtual void Slot0() {}
            virtual void Slot1() {}
            virtual void Slot2() {}
            virtual void Slot3() {}
            virtual void Slot4() {}
            virtual void Delete(int flags) {}
        };
        reinterpret_cast<DeletingDtorView *>(obj2dc)->Delete(1);
        *reinterpret_cast<void **>(bytes + 0x2dc) = NULL;
    }

    unsigned char *anmBytes = reinterpret_cast<unsigned char *>(g_AnmRenderManagerView);
    AnmLoadedView **slot0 = reinterpret_cast<AnmLoadedView **>(anmBytes + 0x3ad088);
    if (*slot0 != NULL)
    {
        (*slot0)->Release();
        operator delete(*slot0);
        *slot0 = NULL;
    }
    AnmLoadedView **slot1 = reinterpret_cast<AnmLoadedView **>(anmBytes + 0x3ad084);
    if (*slot1 != NULL)
    {
        (*slot1)->Release();
        operator delete(*slot1);
        *slot1 = NULL;
    }

    g_SptTestView = NULL;
    reinterpret_cast<ScriptViewerWorkerView *>(bytes + 0x10)->Stop();
}

// 0x0042BE10: SptTestLoadCallback
int SptTestLoadCallback()
{
    g_SptTestView->RegisterTasks();
    return 0;
}

// ---------------------------------------------------------------------------
// 3. EnemyLaser virtual method dispatch (vtables 0x46DAB0, 0x46DA60, 0x46DA10)
// ---------------------------------------------------------------------------
struct EnemyLaserTrimVector;
struct EnemyLaserType0CollisionView
{
    int CheckCollisionBox(
        const EnemyLaserTrimVector *center,
        const EnemyLaserTrimVector *size,
        int mode);
    int CheckCollisionCircle(
        const EnemyLaserTrimVector *center,
        float radius,
        int mode);
};

// Base class (vtable 0x0046DAB0)
void EnemyLaserConstructionBaseView::RunCommands() { th10::leaf::Fn0041BEF0(); }
int EnemyLaserConstructionBaseView::ApplyRequest(const EnemyLaserRequestScratch *) { return 0; }
int EnemyLaserConstructionBaseView::Update() { return th10::leaf::Fn0041BF10(); }
int EnemyLaserConstructionBaseView::Draw() { return th10::leaf::Fn0041BF20(); }
int EnemyLaserConstructionBaseView::Unlink()
{
    if (previous != NULL)
        previous->next = next;
    if (next != NULL)
        next->previous = previous;
    return 0;
}
int EnemyLaserConstructionBaseView::Clear(int) { return 0; }
int EnemyLaserConstructionBaseView::CheckBox(EnemyLaserVectorView *, EnemyLaserVectorView *, int) { return 0; }
int EnemyLaserConstructionBaseView::CheckCircle(EnemyLaserVectorView *, float, int) { return 0; }
int EnemyLaserConstructionBaseView::CheckPoint(EnemyLaserVectorView *, float) { return 0; }
void EnemyLaserConstructionBaseView::Motion0001() { th10::leaf::Fn0041BF90(); }
void EnemyLaserConstructionBaseView::Motion0010() { th10::leaf::Fn0041BFA0(); }
void EnemyLaserConstructionBaseView::Motion0020() { th10::leaf::Fn0041BFB0(); }
void EnemyLaserConstructionBaseView::Motion0040() { th10::leaf::Fn0041BFC0(); }
void EnemyLaserConstructionBaseView::Motion0100() { th10::leaf::Fn0041BFD0(); }
void EnemyLaserConstructionBaseView::Motion0080() { th10::leaf::Fn0041BFE0(); }
void EnemyLaserConstructionBaseView::Bounce() { th10::leaf::Fn0041BFF0(); }
void EnemyLaserConstructionBaseView::Motion100000() { th10::leaf::Fn0041C000(); }
void EnemyLaserConstructionBaseView::Motion200000() { th10::leaf::Fn0041C010(); }
void EnemyLaserConstructionBaseView::Motion4000000() { th10::leaf::Fn0041C020(); }

// Type0 / Line laser (vtable 0x0046DA60)
struct EnemyLaserType0StorageView : EnemyLaserConstructionBaseView
{
    virtual void RunCommands();
    virtual int ApplyRequest(const EnemyLaserRequestScratch *request);
    virtual int Update();
    virtual int Draw();
    virtual int Unlink();
    virtual int Clear(int mode);
    virtual int CheckBox(EnemyLaserVectorView *, EnemyLaserVectorView *, int);
    virtual int CheckCircle(EnemyLaserVectorView *, float, int);
    virtual int CheckPoint(EnemyLaserVectorView *, float);
    virtual void Motion0010();
    virtual void Motion0040();
    virtual void Bounce();
};

void EnemyLaserType0StorageView::RunCommands()
{
    reinterpret_cast<EnemyLaserLineView *>(this)->RunCommands();
}
int EnemyLaserType0StorageView::ApplyRequest(const EnemyLaserRequestScratch *request)
{
    return reinterpret_cast<EnemyLaserLineView *>(this)->Initialize(
        reinterpret_cast<const EnemyLaserType0RequestView *>(request));
}
int EnemyLaserType0StorageView::Update()
{
    return reinterpret_cast<EnemyLaserLineView *>(this)->Update();
}
int EnemyLaserType0StorageView::Draw()
{
    return reinterpret_cast<EnemyLaserLineView *>(this)->Draw();
}
int EnemyLaserType0StorageView::Unlink()
{
    return th10::leaf::Fn0041D870();
}
int EnemyLaserType0StorageView::Clear(int mode)
{
    return reinterpret_cast<EnemyLaserLineView *>(this)->Clear(mode);
}
int EnemyLaserType0StorageView::CheckBox(
    EnemyLaserVectorView *center, EnemyLaserVectorView *size, int mode)
{
    return reinterpret_cast<EnemyLaserType0CollisionView *>(this)->CheckCollisionBox(
        reinterpret_cast<const EnemyLaserTrimVector *>(center),
        reinterpret_cast<const EnemyLaserTrimVector *>(size),
        mode);
}
int EnemyLaserType0StorageView::CheckCircle(
    EnemyLaserVectorView *center, float radius, int mode)
{
    return reinterpret_cast<EnemyLaserType0CollisionView *>(this)->CheckCollisionCircle(
        reinterpret_cast<const EnemyLaserTrimVector *>(center),
        radius,
        mode);
}
int EnemyLaserType0StorageView::CheckPoint(EnemyLaserVectorView *point, float radius)
{
    return reinterpret_cast<EnemyLaserLineView *>(this)->CheckPoint(point, radius);
}
void EnemyLaserType0StorageView::Motion0010()
{
    reinterpret_cast<EnemyLaserLineView *>(this)->Motion0010();
}
void EnemyLaserType0StorageView::Motion0040()
{
    reinterpret_cast<EnemyLaserLineView *>(this)->Motion0040();
}

// Type1 / Infinite laser (vtable 0x0046DA10)
struct EnemyLaserType1StorageView : EnemyLaserConstructionBaseView
{
    virtual int ApplyRequest(const EnemyLaserRequestScratch *request);
    virtual int Update();
    virtual int Draw();
    virtual int Unlink();
    virtual int Clear(int mode);
    virtual int CheckBox(EnemyLaserVectorView *, EnemyLaserVectorView *, int);
    virtual int CheckCircle(EnemyLaserVectorView *, float, int);
    virtual int CheckPoint(EnemyLaserVectorView *, float);
};

int EnemyLaserType1StorageView::ApplyRequest(const EnemyLaserRequestScratch *request)
{
    return reinterpret_cast<EnemyLaserType1View *>(this)->Initialize(
        reinterpret_cast<const EnemyLaserType1RequestView *>(request));
}
int EnemyLaserType1StorageView::Update()
{
    return reinterpret_cast<EnemyLaserType1View *>(this)->Update();
}
int EnemyLaserType1StorageView::Draw()
{
    return reinterpret_cast<EnemyLaserType1View *>(this)->Draw();
}
int EnemyLaserType1StorageView::Unlink()
{
    return th10::leaf::Fn0041EAF0();
}
int EnemyLaserType1StorageView::Clear(int mode)
{
    return reinterpret_cast<EnemyLaserType1View *>(this)->Clear(mode);
}
int EnemyLaserType1StorageView::CheckBox(
    EnemyLaserVectorView *center, EnemyLaserVectorView *size, int mode)
{
    return reinterpret_cast<EnemyLaserType1View *>(this)->CheckCollisionBox(
        center, size, mode);
}
int EnemyLaserType1StorageView::CheckCircle(
    EnemyLaserVectorView *center, float radius, int mode)
{
    return reinterpret_cast<EnemyLaserType1View *>(this)->CheckCollisionCircle(
        center, radius, mode);
}
// 0x0041F670: rotated local-space point check for Type1 laser
int EnemyLaserType1StorageView::CheckPoint(EnemyLaserVectorView *point, float radius)
{
    const float dx = point->x - eclPosition.x;
    const float dy = point->y - eclPosition.y;
    const float negAngle = -eclAngle;
    const float s = sinf(negAngle);
    const float c = cosf(negAngle);
    const float rx = dx * c - dy * s;
    const float ry = dx * s + dy * c;
    const float length = *reinterpret_cast<const float *>(
        reinterpret_cast<const unsigned char *>(this) + 0x40);
    const float width = eclValue44;
    if (rx - radius <= length &&
        rx + radius >= 0.0f &&
        ry - radius >= width * -0.5f &&
        ry + radius <= width * 0.5f)
    {
        return 2;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// 4. Cancel-effect helpers referenced by EnemyCancelManagerView::CancelAllBullets
// ---------------------------------------------------------------------------
void EnemySpawnCancelEffect(
    float *position, int kind, int color, float angle, float scale)
{
    if (g_ItemManager != NULL)
    {
        g_ItemManager->Spawn(
            reinterpret_cast<const AnmFloat3View *>(position),
            kind, color, angle, scale);
    }
}

int *EnemyCreateCancelVm(void *resource)
{
    static int s_vmId = 0;
    s_vmId = 0;
    (void)resource;
    return &s_vmId;
}

int EnemyResolveCancelVm(int id)
{
    if (g_AnmRenderManagerView == NULL)
        return 0;
    AnmVmIdView vmId;
    vmId.value = id;
    return reinterpret_cast<int>(g_AnmRenderManagerView->FindVm(vmId));
}
