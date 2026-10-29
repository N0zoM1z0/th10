#include "Main.hpp"
#include "StageScript.hpp"
#include "MainViewport.hpp"
#include "TransitionEffect.hpp"
#include <string.h>

// Official D3DX9 array/translation contracts; the target import thunks are
// 0x452188 and 0x452182 respectively. No host projection substitute is used.
extern "C" AnmMatrixView *__stdcall D3DXMatrixTranslation(
    AnmMatrixView *output, float x, float y, float z);
extern "C" AnmFloat3View *__stdcall D3DXVec3ProjectArray(
    AnmFloat3View *output, unsigned int outputStride,
    const AnmFloat3View *input, unsigned int inputStride,
    const AnmViewportView *viewport, const AnmMatrixView *projection,
    const AnmMatrixView *view, const AnmMatrixView *world, unsigned int count);

extern StageCameraStateView g_StagePublishedCamera; // 0x491D7C, alias storage open
// Viewport and fog caches use the maintained Main+388/+750 members.
extern "C" AnmMatrixView *__stdcall D3DXMatrixLookAtLH(
    AnmMatrixView *output, const AnmFloat3View *eye,
    const AnmFloat3View *at, const AnmFloat3View *up);
extern "C" AnmMatrixView *__stdcall D3DXMatrixPerspectiveFovLH(
    AnmMatrixView *output, float fieldOfView, float aspect, float nearZ, float farZ);
extern "C" AnmFloat3View *__stdcall D3DXVec3Normalize(
    AnmFloat3View *output, const AnmFloat3View *input);

// Alias-safe inline cross product following the D3DX vector contract.
inline AnmFloat3View *StageCrossProduct(AnmFloat3View *output,
    const AnmFloat3View *first, const AnmFloat3View *second)
{
    AnmFloat3View value;
    value.x = first->y * second->z - first->z * second->y;
    value.y = first->z * second->x - first->x * second->z;
    value.z = first->x * second->y - first->y * second->x;
    *output = value;
    return output;
}

// Target 0x4215A0-0x4216E9 uses EDI camera, plain RET. The real draw
// callers supply the private receiver convention under the pinned LTCG graph.
void StageCameraStateView::ConfigureCamera()
{
    if (g_AnmRenderManagerView != NULL)
        g_AnmRenderManagerView->FlushVertexBuffer();
    AnmFloat3View at(this->direction.x + this->position.x,
        this->direction.y + this->position.y,
        this->direction.z + this->position.z);
    AnmFloat3View eye(this->rockingOffset + this->position.x,
        this->rockingOffsetY + this->position.y,
        this->rockingOffsetZ + this->position.z);
    D3DXMatrixLookAtLH(&this->viewMatrix, &eye, &at, &this->up);
    D3DXMatrixPerspectiveFovLH(&this->projectionMatrix, this->value048,
        static_cast<float>(this->viewport.width) /
            static_cast<float>(this->viewport.height), 30.0f, 1800.0f);
    D3d9DeviceView *device = g_Direct3DDevice;
    device->vtable->SetTransform(device, 2, &this->viewMatrix);
    device = g_Direct3DDevice;
    device->vtable->SetTransform(device, 3, &this->projectionMatrix);
    StageCrossProduct(&this->cameraRight, &this->direction, &this->up);
    D3DXVec3Normalize(&this->cameraRight, &this->cameraRight);
    // Reload the renderer after external calls, as the target does.
    if (g_AnmRenderManagerView != NULL)
    {
        g_AnmRenderManagerView->screenShakeX = this->value0E8;
        g_AnmRenderManagerView->screenShakeY = this->value0EC;
    }
}


// Target 0x403090-0x40384F. Object/this ECX, position EDX, camera EAX,
// distance on stack and RET4. The full 1984-byte owner includes both tests.
int StageObjectView::BoundsCulled(const AnmFloat3View *position,
    const StageCameraStateView *camera, float distanceSquared) const
{
    // Use the first corner as the translated center/delta for the distance
    // test, then replace every corner before projection. Vector assignments
    // retain the target float rounding of center and delta components.
    AnmFloat3View corners[8];
    AnmFloat3View &delta = corners[0];
    delta = this->position + *position;
    AnmFloat3View eye(camera->position.x + camera->rockingOffset,
        camera->position.y + camera->rockingOffsetY,
        camera->position.z + camera->rockingOffsetZ);
    delta = delta - eye;
    // In this pinned graph the ordinary XYZ dot expression evaluates Z,Y,X,
    // matching the target x87 accumulation; host reassociation is not proof.
    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z > distanceSquared)
        return 1;

    AnmFloat3View half;
    half = this->extent * 0.5f;
    corners[0].x = half.x + this->position.x;
    corners[0].y = half.y + this->position.y;
    corners[0].z = half.z + this->position.z;
    corners[1].x = half.x + this->position.x;
    corners[1].y = half.y + this->position.y;
    corners[1].z = this->position.z - half.z;
    corners[2].x = half.x + this->position.x;
    corners[2].y = this->position.y - half.y;
    corners[2].z = half.z + this->position.z;
    corners[3].x = half.x + this->position.x;
    corners[3].y = this->position.y - half.y;
    corners[3].z = this->position.z - half.z;
    corners[4].x = this->position.x - half.x;
    corners[4].y = half.y + this->position.y;
    corners[4].z = half.z + this->position.z;
    corners[5].x = this->position.x - half.x;
    corners[5].y = half.y + this->position.y;
    corners[5].z = this->position.z - half.z;
    corners[6].x = this->position.x - half.x;
    corners[6].y = this->position.y - half.y;
    corners[6].z = half.z + this->position.z;
    corners[7].x = this->position.x - half.x;
    corners[7].y = this->position.y - half.y;
    corners[7].z = this->position.z - half.z;
    AnmMatrixView world;
    world.SetIdentity();
    D3DXMatrixTranslation(&world, position->x, position->y, position->z);
    AnmFloat3View projected[8];
    D3DXVec3ProjectArray(projected, sizeof(AnmFloat3View), corners,
        sizeof(AnmFloat3View), &camera->viewport, &camera->projectionMatrix,
        &camera->viewMatrix, &world, 8);
    float minimumX = 424.0f, maximumX = 24.0f;
    float minimumY = 472.0f, maximumY = 8.0f;
    for (int i = 0; i < 8; ++i)
    {
        if (projected[i].z >= 0.0f && projected[i].z <= 1.0f)
        {
            if (projected[i].x < minimumX)
                minimumX = projected[i].x;
            else if (projected[i].x > maximumX)
                maximumX = projected[i].x;
            if (projected[i].y < minimumY)
                minimumY = projected[i].y;
            else if (projected[i].y > maximumY)
                maximumY = projected[i].y;
        }
    }
    return maximumX >= 32.0f && minimumX <= 416.0f &&
        maximumY >= 16.0f && minimumY <= 464.0f ? 0 : 1;
}

// Target 0x403A30-0x403C7F: stack stage/layer, RET8. The real early/late
// callers preserve this interface; bounds bytes are a separate obligation.
int __stdcall StageDrawInstances(StageScriptView *stage, int layer)
{
    StageInstanceView *instance = stage->instances;
    g_StagePublishedCamera.InstallViewport();
    g_AnmRenderManagerView->currentCameraMode = 1;
    for (; instance->objectIndex >= 0; ++instance)
    {
        StageObjectView *object = stage->objects[instance->objectIndex];
        if (static_cast<signed char>(object->layer) != layer)
            continue;
        AnmFloat3View position(instance->position.x,
            instance->position.y, instance->position.z);
        if (object->BoundsCulled(&position, &g_StagePublishedCamera,
                stage->cullDistanceSquared) != 0)
        {
            ++stage->culledObjectCount;
            continue;
        }
        object->flags |= 2;
        StageObjectPrimitiveView *primitive = &object->firstPrimitive;
        while (primitive->type >= 0)
        {
            AnmVmView *vm = &stage->objectVms[primitive->vmIndex];
            if (primitive->type == 0)
            {
                StageQuadPrimitiveView *quad = reinterpret_cast<StageQuadPrimitiveView *>(primitive);
                if ((vm->flags35C & 0x3c00000u) >= 0x1000000u)
                {
                    vm->positionOffset.x = quad->position.x + instance->position.x;
                    vm->positionOffset.y = quad->position.y + instance->position.y;
                    vm->positionOffset.z = quad->position.z + instance->position.z;
                    if (quad->width != 0.0f)
                    {
                        vm->scaleX = quad->width / vm->loadedSprite->width;
                        vm->flags35C |= 8;
                    }
                    if (quad->height != 0.0f)
                    {
                        vm->scaleY = quad->height / vm->loadedSprite->height;
                        vm->flags35C |= 8;
                    }
                }
                if ((vm->flags35C & 0x3c00000u) == 0x2000000u)
                    g_MainSupervisorView.SetFogEnabled(1);
                else
                    g_MainSupervisorView.SetFogEnabled(0);
                g_AnmRenderManagerView->Draw(vm);
                ++stage->drawCount;
            }
            primitive = reinterpret_cast<StageObjectPrimitiveView *>(
                reinterpret_cast<unsigned char *>(primitive) + primitive->byteSize);
        }
        ++stage->visibleObjectCount;
    }
    return 0;
}

// Target0x405300: ESI supervisor, EBX viewport index. The shared declaration
// keeps this selector distinct from the camera configurator at0x421480.
struct MainSupervisorView;
extern MainSupervisorView g_MainSupervisorView;
// Target0x420CD0: EDI supervisor, clears the full-dword fog cache if necessary.
// Target0x43C8B0 takes EBX priority and five stack arguments, RET20. The
// exact Early recipe includes the real factory definition for LTCG visibility.

static void StageSetRenderState(unsigned int state, unsigned int value)
{
    g_AnmRenderManagerView->FlushVertexBuffer();
    g_Direct3DDevice->vtable->SetRenderState(g_Direct3DDevice, state, value);
}
// Main owns the full-word fog cache at+750 and the device pointer at+8.
static void StageSetFogState(int value)
{
    g_MainSupervisorView.SetFogEnabled(value);
}
// This shared source operation expands at the real draw sites. A separate
// original target helper or source-class identity is not inferred.
void StageCameraStateView::InstallViewport()
{
    g_AnmViewportOwner = reinterpret_cast<AnmViewportOwnerView *>(this);
    ConfigureCamera();
    g_Direct3DDevice->vtable->SetViewport(g_Direct3DDevice, &g_AnmViewportOwner->viewport);
    g_MainSupervisorView.viewportConfigured = 0;
}
// Render-state arguments carry the member's raw DWORD, including NaN payloads.
// The target captures that word before Flush; it performs no float operation.
static unsigned int StageFloatBits(const float &value)
{
    unsigned int bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}
static void StageRestoreMixColor(StageScriptView *stage)
{
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    manager->useMixColor = 0;
    manager->mixColor.value = 0x80808080;
    if (stage->auxiliaryActive != 0)
    {
        manager->useMixColor = 1;
        manager->mixColor.value = 0xff404040;
    }
}

// Target0x402850/1099, stack owner/RET4. Registered through the real ECX
// adapter0x403060. This early draw pass owns instance layers0 through7.
int __stdcall StageDrawEarly(StageScriptView *stage)
{
    if ((stage->flags & 8) != 0)
        return 1;
    if ((stage->flags & 4) == 0 || stage->transitionTimer.current < 60)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        stage->camera.value0E8 = g_StagePublishedCamera.value0E8;
        stage->camera.value0EC = g_StagePublishedCamera.value0EC;
        g_StagePublishedCamera = stage->camera;
        g_StagePublishedCamera.InstallViewport();
        StageSetRenderState(14, 1);
        StageSetRenderState(23, 4);
        StageSetRenderState(34, stage->camera.fog.color);
        StageSetRenderState(36, StageFloatBits(stage->camera.fog.start));
        StageSetRenderState(37, StageFloatBits(stage->camera.fog.end));
        g_MainSupervisorView.d3dDevice->vtable->Clear(
            g_MainSupervisorView.d3dDevice, 0, NULL, 2, 0, 1.0f, 0);
        if ((stage->flags & 4) != 0 && stage->frame < 34)
        {
            D3d9ClearRectView rectangle = {32, 16, 416, 464};
            g_MainSupervisorView.d3dDevice->vtable->Clear(
                g_MainSupervisorView.d3dDevice, 1, &rectangle, 1, 0, 1.0f, 0);
        }
        else
        {
            D3d9ClearRectView rectangle = {32, 16, 416, 464};
            g_MainSupervisorView.d3dDevice->vtable->Clear(
                g_MainSupervisorView.d3dDevice, 1, &rectangle, 1,
                stage->camera.fog.color & 0xffffff, 1.0f, 0);
        }
    }
    if ((stage->flags & 4) != 0)
    {
        if (stage->transitionTimer.current < 30)
        {
            StageCreateTransitionEffect(15, 3, 30, 0, 0, 0);
            stage->flags |= 1;
            stage->transitionTimer.SetCurrent(1);
        }
        else
        {
            reinterpret_cast<unsigned char *>(&stage->tint)[3] = 0;
            stage->flags &= ~1u;
        }
    }
    if (reinterpret_cast<unsigned char *>(&stage->tint)[3] != 0)
    {
        unsigned int tint = stage->tint;
        g_AnmRenderManagerView->useMixColor = 1;
        g_AnmRenderManagerView->mixColor.value = tint;
    }
    stage->visibleObjectCount = 0;
    stage->culledObjectCount = 0;
    stage->drawCount = 0;
    if ((stage->flags & 1) != 0)
    {
        if (stage->scriptVms[0].loadedSprite != NULL)
        {
            MainConfigureGameplayViewport(&g_MainSupervisorView, 0);
            StageDisableSupervisorFog(&g_MainSupervisorView);
            g_AnmRenderManagerView->FlushVertexBuffer();
            StageSetRenderState(14, 0);
            for (int i = 0; i < 8; ++i)
                if (stage->scriptVms[i].loadedSprite != NULL)
                    g_AnmRenderManagerView->Draw(&stage->scriptVms[i]);
            StageSetRenderState(14, 1);
            g_StagePublishedCamera.InstallViewport();
        }
        StageSetFogState(1);
        StageDrawInstances(stage, 0);
        StageDrawInstances(stage, 1);
        StageDrawInstances(stage, 2);
        StageDrawInstances(stage, 3);
        StageDrawInstances(stage, 4);
        StageDrawInstances(stage, 5);
        StageDrawInstances(stage, 6);
        StageDrawInstances(stage, 7);
        g_AnmRenderManagerView->FlushVertexBuffer();
        if (stage->unknown1EE4 != 0)
            ++stage->unknown1EE4;
    }
    StageRestoreMixColor(stage);
    StageSetRenderState(14, 0);
    StageSetRenderState(23, 8);
    return 1;
}

// Target0x402CA0/929, stack owner/RET4; registered ECX adapter0x403070.
int __stdcall StageDrawLate(StageScriptView *stage)
{
    if ((stage->flags & 8) != 0)
        return 1;
    if ((stage->flags & 4) == 0 || stage->transitionTimer.current < 60)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        stage->camera.value0E8 = g_StagePublishedCamera.value0E8;
        stage->camera.value0EC = g_StagePublishedCamera.value0EC;
        g_StagePublishedCamera = stage->camera;
        g_StagePublishedCamera.InstallViewport();
        StageSetFogState(1);
        StageSetRenderState(14, 0);
        StageSetRenderState(23, 8);
        g_AnmRenderManagerView->DrawLayer(17);
        StageSetRenderState(23, 4);
        StageSetFogState(0);
        g_AnmRenderManagerView->DrawLayer(18);
        StageSetRenderState(23, 4);
        StageSetRenderState(34, stage->camera.fog.color);
        StageSetRenderState(36, StageFloatBits(stage->camera.fog.start));
        StageSetRenderState(37, StageFloatBits(stage->camera.fog.end));
    }
    if ((stage->flags & 4) != 0 && stage->transitionTimer.current >= 30)
        reinterpret_cast<unsigned char *>(&stage->tint)[3] = 0;
    if ((stage->flags & 1) != 0)
    {
        StageSetRenderState(14, 0);
        StageSetFogState(1);
        StageDrawInstances(stage, 8);
        StageDrawInstances(stage, 9);
        StageDrawInstances(stage, 10);
        StageDrawInstances(stage, 11);
        g_AnmRenderManagerView->FlushVertexBuffer();
    }
    StageRestoreMixColor(stage);
    StageSetRenderState(14, 0);
    StageSetRenderState(23, 8);
    if (stage->transitionTimer.current > 0)
    {
        stage->transitionTimer.Add(-1.0f);
        if (stage->transitionTimer.current <= 0)
        {
            reinterpret_cast<unsigned char *>(&stage->tint)[3] = 0xff;
            if ((stage->flags & 2) != 0)
                stage->flags |= 8;
            stage->tint = 0x00ffffff;
            stage->flags &= ~6u;
        }
    }
    StageSetRenderState(14, 0);
    StageSetRenderState(23, 8);
    StageSetFogState(0);
    return 1;
}
int __fastcall StageDrawEarlyCallback(StageScriptView *stage)
{
    return StageDrawEarly(stage);
}
int __fastcall StageDrawLateCallback(StageScriptView *stage)
{
    return StageDrawLate(stage);
}
