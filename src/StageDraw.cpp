#include "StageScript.hpp"

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
extern int g_StageViewportConfigured; // 0x491FB0, same storage as Main +0x388
// Target accesses a full dword. Main's existing byte view at this address is
// not widened here; neither extern supplies allocated/alias-coherent storage.
extern int g_StageFogEnabled; // 0x492378
extern "C" AnmMatrixView *__stdcall D3DXMatrixLookAtLH(
    AnmMatrixView *output, const AnmFloat3View *eye,
    const AnmFloat3View *at, const AnmFloat3View *up);
extern "C" AnmMatrixView *__stdcall D3DXMatrixPerspectiveFovLH(
    AnmMatrixView *output, float fieldOfView, float aspect, float nearZ, float farZ);
extern "C" AnmFloat3View *__stdcall D3DXVec3Normalize(
    AnmFloat3View *output, const AnmFloat3View *input);

// Target 0x4215A0-0x4216E9 uses EDI camera, plain RET. Genuine draw caller
// visibility is retained; source alone does not establish private ABI equality.
void StageConfigureCamera(StageCameraStateView *camera)
{
    if (g_AnmRenderManagerView != NULL)
        g_AnmRenderManagerView->FlushVertexBuffer();
    AnmFloat3View at = camera->direction + camera->position;
    AnmFloat3View eye(camera->rockingOffset + camera->position.x,
        camera->rockingOffsetY + camera->position.y,
        camera->rockingOffsetZ + camera->position.z);
    D3DXMatrixLookAtLH(&camera->viewMatrix, &eye, &at, &camera->up);
    D3DXMatrixPerspectiveFovLH(&camera->projectionMatrix, camera->value048,
        static_cast<float>(camera->viewport.width) /
            static_cast<float>(camera->viewport.height), 30.0f, 1800.0f);
    g_Direct3DDevice->vtable->SetTransform(g_Direct3DDevice, 2, &camera->viewMatrix);
    g_Direct3DDevice->vtable->SetTransform(g_Direct3DDevice, 3, &camera->projectionMatrix);
    AnmFloat3View right(
        camera->up.z * camera->direction.y - camera->up.y * camera->direction.z,
        camera->up.x * camera->direction.z - camera->up.z * camera->direction.x,
        camera->up.y * camera->direction.x - camera->up.x * camera->direction.y);
    camera->cameraRight = right;
    D3DXVec3Normalize(&camera->cameraRight, &camera->cameraRight);
    // Reload the renderer after external calls, as the target does.
    if (g_AnmRenderManagerView != NULL)
    {
        g_AnmRenderManagerView->screenShakeX = camera->value0E8;
        g_AnmRenderManagerView->screenShakeY = camera->value0EC;
    }
}


// Target 0x403090-0x40384F. Camera EAX, object ECX, position EDX, distance
// on stack, RET4. Natural source below does not impose that private convention.
int StageBoundsCulled(const StageCameraStateView *camera,
    const StageObjectView *object, const AnmFloat3View *position,
    float distanceSquared)
{
    AnmFloat3View center = object->position + *position;
    AnmFloat3View eye(camera->position.x + camera->rockingOffset,
        camera->position.y + camera->rockingOffsetY,
        camera->position.z + camera->rockingOffsetZ);
    AnmFloat3View delta = center - eye;
    if (delta.z * delta.z + delta.y * delta.y + delta.x * delta.x > distanceSquared)
        return 1;

    AnmFloat3View half = object->extent * 0.5f;
    AnmFloat3View maximum = half + object->position;
    AnmFloat3View minimum = object->position - half;
    AnmFloat3View corners[8] = {
        AnmFloat3View(maximum.x, maximum.y, maximum.z),
        AnmFloat3View(maximum.x, maximum.y, minimum.z),
        AnmFloat3View(maximum.x, minimum.y, maximum.z),
        AnmFloat3View(maximum.x, minimum.y, minimum.z),
        AnmFloat3View(minimum.x, maximum.y, maximum.z),
        AnmFloat3View(minimum.x, maximum.y, minimum.z),
        AnmFloat3View(minimum.x, minimum.y, maximum.z),
        AnmFloat3View(minimum.x, minimum.y, minimum.z)
    };
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
        if (projected[i].z >= 0.0f && projected[i].z < 1.0f)
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

// Target 0x403A30-0x403C7F: two stack arguments, RET8. This genuine caller
// supplies the bounds object's actual use; upstream draw registration is open.
int __stdcall StageDrawInstances(StageScriptView *stage, int layer)
{
    StageInstanceView *instance = stage->instances;
    g_AnmViewportOwner = reinterpret_cast<AnmViewportOwnerView *>(&g_StagePublishedCamera);
    StageConfigureCamera(&g_StagePublishedCamera);
    g_Direct3DDevice->vtable->SetViewport(g_Direct3DDevice,
        &g_AnmViewportOwner->viewport);
    g_StageViewportConfigured = 0;
    g_AnmRenderManagerView->currentCameraMode = 1;
    for (; instance->objectIndex >= 0; ++instance)
    {
        StageObjectView *object = stage->objects[instance->objectIndex];
        if (static_cast<signed char>(object->layer) != layer)
            continue;
        AnmFloat3View position = instance->position;
        if (StageBoundsCulled(&g_StagePublishedCamera, object, &position,
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
                    vm->position.x = quad->position.x + instance->position.x;
                    vm->position.y = quad->position.y + instance->position.y;
                    vm->position.z = quad->position.z + instance->position.z;
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
                int fog = (vm->flags35C & 0x3c00000u) == 0x2000000u ? 1 : 0;
                if (g_StageFogEnabled != fog)
                {
                    g_AnmRenderManagerView->FlushVertexBuffer();
                    g_StageFogEnabled = fog;
                    g_Direct3DDevice->vtable->SetRenderState(g_Direct3DDevice, 28, fog);
                }
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
