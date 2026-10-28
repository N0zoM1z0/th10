#include "Main.hpp"
#include "MainViewport.hpp"
#include "StageScript.hpp"

// Target chain callbacks receive the registered manager in ECX. The small
// adapters and the callbacks with camera/state effects have separate owners.
int __fastcall AnmRenderManagerView::OnUpdatePrimary(AnmRenderManagerView *manager)
{
    return manager->UpdatePrimaryVms();
}
int __fastcall AnmRenderManagerView::OnUpdateSecondary(AnmRenderManagerView *manager)
{
    return manager->UpdateSecondaryVms();
}

#define ANM_PLAIN_LAYER_CALLBACK(layer) \
int __fastcall AnmRenderManagerView::DrawLayer##layer(AnmRenderManagerView *manager) \
{ \
    return manager->DrawLayer(layer); \
}
ANM_PLAIN_LAYER_CALLBACK(0)
ANM_PLAIN_LAYER_CALLBACK(1)
ANM_PLAIN_LAYER_CALLBACK(2)
ANM_PLAIN_LAYER_CALLBACK(4)
ANM_PLAIN_LAYER_CALLBACK(5)
ANM_PLAIN_LAYER_CALLBACK(6)
ANM_PLAIN_LAYER_CALLBACK(7)
ANM_PLAIN_LAYER_CALLBACK(8)
ANM_PLAIN_LAYER_CALLBACK(9)
ANM_PLAIN_LAYER_CALLBACK(10)
ANM_PLAIN_LAYER_CALLBACK(11)
ANM_PLAIN_LAYER_CALLBACK(12)
ANM_PLAIN_LAYER_CALLBACK(13)
#undef ANM_PLAIN_LAYER_CALLBACK

// Target448620/115 uses Main's first screen-camera record, then disables fog.
int __fastcall AnmRenderManagerView::DrawLayer3(AnmRenderManagerView *manager)
{
    AnmViewportOwnerView *camera = reinterpret_cast<AnmViewportOwnerView *>(
        g_MainSupervisorView.unknown154);
    g_MainSupervisorView.activeViewport = camera;
    MainConfigureCamera(camera);
    g_MainSupervisorView.d3dDevice->vtable->SetViewport(
        g_MainSupervisorView.d3dDevice, &g_MainSupervisorView.activeViewport->viewport);
    g_MainSupervisorView.viewportConfigured = 0;
    if (g_MainSupervisorView.cachedFogEnabled != 0)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        g_MainSupervisorView.cachedFogEnabled = 0;
        g_MainSupervisorView.d3dDevice->vtable->SetRenderState(
            g_MainSupervisorView.d3dDevice, 28, 0);
    }
    return manager->DrawLayer(3);
}

// Target448740/48 clears both the published camera and active renderer shake.
int __fastcall AnmRenderManagerView::DrawLayer14(AnmRenderManagerView *manager)
{
    StageCameraStateView *camera = reinterpret_cast<StageCameraStateView *>(
        g_MainSupervisorView.unknown154);
    camera->value0E8 = 0;
    camera->value0EC = 0;
    g_AnmRenderManagerView->screenShakeX = 0;
    g_AnmRenderManagerView->screenShakeY = 0;
    return manager->DrawLayer(14);
}

// Each target expands this sequence before its own layer call; these use
// Main's second screen camera, not the stage's perspective camera helper.
#define ANM_SCREEN_LAYER_CALLBACK(layer) \
int __fastcall AnmRenderManagerView::DrawLayer##layer(AnmRenderManagerView *manager) \
{ \
    AnmViewportOwnerView *camera = &g_MainSupervisorView.gameplayViewport; \
    g_MainSupervisorView.activeViewport = camera; \
    MainConfigureCamera(camera); \
    g_MainSupervisorView.d3dDevice->vtable->SetViewport( \
        g_MainSupervisorView.d3dDevice, &g_MainSupervisorView.activeViewport->viewport); \
    g_MainSupervisorView.viewportConfigured = 1; \
    return manager->DrawLayer(layer); \
}
ANM_SCREEN_LAYER_CALLBACK(15)
ANM_SCREEN_LAYER_CALLBACK(16)
ANM_SCREEN_LAYER_CALLBACK(19)
#undef ANM_SCREEN_LAYER_CALLBACK
