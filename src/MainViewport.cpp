#include "Main.hpp"
#include "MainViewport.hpp"
#include <math.h>

extern "C" AnmMatrixView *__stdcall D3DXMatrixLookAtLH(
    AnmMatrixView *output, const AnmFloat3View *eye,
    const AnmFloat3View *at, const AnmFloat3View *up);
extern "C" AnmMatrixView *__stdcall D3DXMatrixPerspectiveFovLH(
    AnmMatrixView *output, float fieldOfView, float aspect, float nearZ, float farZ);

// The two records at supervisor+154/+26C have118-byte stride. Reuse the
// maintained matrix/viewport prefix; allocation and original class are open.
struct MainCameraRecordView
{
    AnmViewportOwnerView prefix;
    unsigned int unknown0E4;
    float screenShakeX, screenShakeY;
    unsigned char unknown0F0[0x28];
};
typedef char MainCameraRecordLayout[
    (sizeof(MainCameraRecordView) == 0x118 &&
     offsetof(MainCameraRecordView, screenShakeX) == 0xe8 &&
     offsetof(MainCameraRecordView, screenShakeY) == 0xec) ? 1 : -1];

// Target421480/287: positive screen coordinates and downward up vector.
// Width/height convert separately before the aspect division.
void MainConfigureCamera(AnmViewportOwnerView *camera)
{
    if (g_AnmRenderManagerView != NULL)
        g_AnmRenderManagerView->FlushVertexBuffer();
    float width = static_cast<float>(camera->viewport.width);
    float height = static_cast<float>(camera->viewport.height);
    float aspect = width / height;
    AnmFloat3View eye(width * 0.5f, height * 0.5f,
        static_cast<float>(height * 0.5f / tan(3.1415927f / 20.0)));
    AnmFloat3View at(width * 0.5f, height * 0.5f, 0.0f);
    AnmFloat3View up(0.0f, -1.0f, 0.0f);
    D3DXMatrixLookAtLH(&camera->viewMatrix, &eye, &at, &up);
    D3DXMatrixPerspectiveFovLH(&camera->projectionMatrix,
        3.1415927f / 10.0f, aspect, 1.0f, 10000.0f);
    g_Direct3DDevice->vtable->SetTransform(g_Direct3DDevice, 2, &camera->viewMatrix);
    g_Direct3DDevice->vtable->SetTransform(g_Direct3DDevice, 3, &camera->projectionMatrix);
    if (g_AnmRenderManagerView != NULL)
    {
        const MainCameraRecordView *record =
            reinterpret_cast<const MainCameraRecordView *>(camera);
        g_AnmRenderManagerView->screenShakeX = record->screenShakeX;
        g_AnmRenderManagerView->screenShakeY = record->screenShakeY;
    }
}

// Target405300/60. No index check is added; target callers use0/1.
void MainConfigureGameplayViewport(MainSupervisorView *supervisor, int index)
{
    AnmViewportOwnerView *camera = reinterpret_cast<AnmViewportOwnerView *>(
        reinterpret_cast<unsigned char *>(supervisor) + 0x154 + index * 0x118);
    supervisor->activeViewport = camera;
    MainConfigureCamera(camera);
    supervisor->d3dDevice->vtable->SetViewport(supervisor->d3dDevice,
        &supervisor->activeViewport->viewport);
    supervisor->viewportConfigured = index;
}

// Target420CD0/53. The full cache word is tested and cleared. Main's
// per-frame invalidation at4394C2 also writes a complete DWORD.
long StageDisableSupervisorFog(MainSupervisorView *supervisor)
{
    if (supervisor->cachedFogEnabled != 0)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        supervisor->cachedFogEnabled = 0;
        return supervisor->d3dDevice->vtable->SetRenderState(supervisor->d3dDevice, 28, 0);
    }
    return 0;
}
