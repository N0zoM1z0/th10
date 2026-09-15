#pragma once

#include <stddef.h>

// The pinned VC7.1 Platform SDK predates Direct3D 9. This target-bound prefix
// provides only the IDirect3DDevice9 slots currently used by maintained TH10
// source while preserving their real COM vtable indices.
struct D3d9DeviceView;
struct D3d9VertexBufferView;
struct D3d9TextureView;
struct D3d9SurfaceView;

struct D3d9LockedRectView
{
    int pitch;
    void *bits;
};

struct D3d9SurfaceDescriptionView
{
    unsigned int format;
    unsigned int resourceType;
    unsigned int usage;
    unsigned int pool;
    unsigned int multisampleType;
    unsigned int multisampleQuality;
    unsigned int width;
    unsigned int height;
};

typedef long (__stdcall *D3d9CreateVertexBuffer)(
    D3d9DeviceView *device,
    unsigned int length,
    unsigned int usage,
    unsigned int fvf,
    unsigned int pool,
    D3d9VertexBufferView **vertexBuffer,
    void *sharedHandle);

typedef long (__stdcall *D3d9SetRenderState)(
    D3d9DeviceView *device,
    unsigned int state,
    unsigned int value);
typedef long (__stdcall *D3d9SetTransform)(
    D3d9DeviceView *device,
    unsigned int state,
    const void *matrix);
typedef long (__stdcall *D3d9SetTexture)(
    D3d9DeviceView *device,
    unsigned int stage,
    void *texture);
typedef long (__stdcall *D3d9SetTextureStageState)(
    D3d9DeviceView *device,
    unsigned int stage,
    unsigned int state,
    unsigned int value);
typedef long (__stdcall *D3d9SetSamplerState)(
    D3d9DeviceView *device,
    unsigned int sampler,
    unsigned int state,
    unsigned int value);
typedef long (__stdcall *D3d9DrawPrimitiveUp)(
    D3d9DeviceView *device,
    unsigned int primitiveType,
    unsigned int primitiveCount,
    const void *vertices,
    unsigned int stride);
typedef long (__stdcall *D3d9DrawPrimitive)(
    D3d9DeviceView *device,
    unsigned int primitiveType,
    unsigned int startVertex,
    unsigned int primitiveCount);
typedef long (__stdcall *D3d9SetFvf)(
    D3d9DeviceView *device,
    unsigned int fvf);
typedef long (__stdcall *D3d9SetVertexShader)(
    D3d9DeviceView *device,
    void *vertexShader);
typedef long (__stdcall *D3d9SetStreamSource)(
    D3d9DeviceView *device,
    unsigned int stream,
    void *streamData,
    unsigned int offsetInBytes,
    unsigned int stride);

struct D3d9DeviceVtableView
{
    void *slots000To064[26];
    D3d9CreateVertexBuffer CreateVertexBuffer;   // +0x068
    void *slots06CTo0AC[17];
    D3d9SetTransform SetTransform;               // +0x0B0
    void *slots0B4To0E0[12];
    D3d9SetRenderState SetRenderState;           // +0x0E4
    void *slots0E8To100[7];
    D3d9SetTexture SetTexture;                   // +0x104
    void *slot108;
    D3d9SetTextureStageState SetTextureStageState; // +0x10C
    void *slot110;
    D3d9SetSamplerState SetSamplerState;         // +0x114
    void *slots118To140[11];
    D3d9DrawPrimitive DrawPrimitive;             // +0x144
    void *slot148;
    D3d9DrawPrimitiveUp DrawPrimitiveUP;           // +0x14C
    void *slots150To160[5];
    D3d9SetFvf SetFVF;                             // +0x164
    void *slots168To16C[2];
    D3d9SetVertexShader SetVertexShader;          // +0x170
    void *slots174To18C[7];
    D3d9SetStreamSource SetStreamSource;         // +0x190
};

typedef long (__stdcall *D3d9VertexBufferLock)(
    D3d9VertexBufferView *buffer,
    unsigned int offset,
    unsigned int size,
    void **data,
    unsigned int flags);
typedef long (__stdcall *D3d9VertexBufferUnlock)(
    D3d9VertexBufferView *buffer);

struct D3d9VertexBufferVtableView
{
    void *slots000To028[11];
    D3d9VertexBufferLock Lock;                    // +0x02C
    D3d9VertexBufferUnlock Unlock;                // +0x030
};

struct D3d9VertexBufferView
{
    D3d9VertexBufferVtableView *vtable;
};

typedef long (__stdcall *D3d9TextureGetSurfaceLevel)(
    D3d9TextureView *texture,
    unsigned int level,
    D3d9SurfaceView **surface);

struct D3d9TextureVtableView
{
    void *slots000To044[18];
    D3d9TextureGetSurfaceLevel GetSurfaceLevel;   // +0x048
};

struct D3d9TextureView
{
    D3d9TextureVtableView *vtable;
};

typedef unsigned long (__stdcall *D3d9SurfaceRelease)(
    D3d9SurfaceView *surface);
typedef long (__stdcall *D3d9SurfaceGetDescription)(
    D3d9SurfaceView *surface,
    D3d9SurfaceDescriptionView *description);
typedef long (__stdcall *D3d9SurfaceLockRect)(
    D3d9SurfaceView *surface,
    D3d9LockedRectView *locked,
    const void *rectangle,
    unsigned int flags);
typedef long (__stdcall *D3d9SurfaceUnlockRect)(
    D3d9SurfaceView *surface);

struct D3d9SurfaceVtableView
{
    void *QueryInterface;
    void *AddRef;
    D3d9SurfaceRelease Release;                   // +0x008
    void *slots00CTo02C[9];
    D3d9SurfaceGetDescription GetDesc;            // +0x030
    D3d9SurfaceLockRect LockRect;                 // +0x034
    D3d9SurfaceUnlockRect UnlockRect;             // +0x038
};

struct D3d9SurfaceView
{
    D3d9SurfaceVtableView *vtable;
};

struct D3d9DeviceView
{
    D3d9DeviceVtableView *vtable;

    long SetTexture(unsigned int stage, void *texture)
    {
        return vtable->SetTexture(this, stage, texture);
    }
};

typedef char D3d9SetRenderStateAt0E4[
    (offsetof(D3d9DeviceVtableView, SetRenderState) == 0x0e4) ? 1 : -1];
typedef char D3d9SetTransformAt0B0[
    (offsetof(D3d9DeviceVtableView, SetTransform) == 0x0b0) ? 1 : -1];
typedef char D3d9CreateVertexBufferAt068[
    (offsetof(D3d9DeviceVtableView, CreateVertexBuffer) == 0x068) ? 1 : -1];
typedef char D3d9SetTextureAt104[
    (offsetof(D3d9DeviceVtableView, SetTexture) == 0x104) ? 1 : -1];
typedef char D3d9SetTextureStageStateAt10C[
    (offsetof(D3d9DeviceVtableView, SetTextureStageState) == 0x10c) ? 1 : -1];
typedef char D3d9SetSamplerStateAt114[
    (offsetof(D3d9DeviceVtableView, SetSamplerState) == 0x114) ? 1 : -1];
typedef char D3d9DrawPrimitiveUpAt14C[
    (offsetof(D3d9DeviceVtableView, DrawPrimitiveUP) == 0x14c) ? 1 : -1];
typedef char D3d9DrawPrimitiveAt144[
    (offsetof(D3d9DeviceVtableView, DrawPrimitive) == 0x144) ? 1 : -1];
typedef char D3d9SetFvfAt164[
    (offsetof(D3d9DeviceVtableView, SetFVF) == 0x164) ? 1 : -1];
typedef char D3d9SetVertexShaderAt170[
    (offsetof(D3d9DeviceVtableView, SetVertexShader) == 0x170) ? 1 : -1];
typedef char D3d9SetStreamSourceAt190[
    (offsetof(D3d9DeviceVtableView, SetStreamSource) == 0x190) ? 1 : -1];
typedef char D3d9VertexBufferLockAt02C[
    (offsetof(D3d9VertexBufferVtableView, Lock) == 0x02c &&
     offsetof(D3d9VertexBufferVtableView, Unlock) == 0x030) ? 1 : -1];
typedef char D3d9TextureSurfaceAt048[
    (offsetof(D3d9TextureVtableView, GetSurfaceLevel) == 0x048) ? 1 : -1];
typedef char D3d9SurfaceMethodsAt030[
    (offsetof(D3d9SurfaceVtableView, GetDesc) == 0x030 &&
     offsetof(D3d9SurfaceVtableView, LockRect) == 0x034 &&
     offsetof(D3d9SurfaceVtableView, UnlockRect) == 0x038) ? 1 : -1];

enum D3d9ViewConstants
{
    D3D9_VIEW_RS_DESTBLEND = 20,
    D3D9_VIEW_RS_ZWRITEENABLE = 14,
    D3D9_VIEW_RS_TEXTUREFACTOR = 60,
    D3D9_VIEW_BLEND_ONE = 2,
    D3D9_VIEW_BLEND_INVSRCALPHA = 6,
    D3D9_VIEW_TSS_COLOROP = 1,
    D3D9_VIEW_TSS_COLORARG1 = 2,
    D3D9_VIEW_TSS_COLORARG2 = 3,
    D3D9_VIEW_TSS_ALPHAOP = 4,
    D3D9_VIEW_TSS_ALPHAARG1 = 5,
    D3D9_VIEW_TSS_ALPHAARG2 = 6,
    D3D9_VIEW_TA_DIFFUSE = 0,
    D3D9_VIEW_TA_TEXTURE = 2,
    D3D9_VIEW_TA_TFACTOR = 3,
    D3D9_VIEW_TOP_SELECTARG1 = 2,
    D3D9_VIEW_TOP_MODULATE = 4,
    D3D9_VIEW_SAMP_MAGFILTER = 5,
    D3D9_VIEW_SAMP_MINFILTER = 6,
    D3D9_VIEW_TEXF_POINT = 1,
    D3D9_VIEW_TEXF_LINEAR = 2,
    D3D9_VIEW_PT_TRIANGLELIST = 4,
    D3D9_VIEW_PT_TRIANGLESTRIP = 5,
    D3D9_VIEW_PT_TRIANGLEFAN = 6,
    D3D9_VIEW_TS_TEXTURE0 = 16,
    D3D9_VIEW_TS_WORLD = 256,
    D3D9_VIEW_FVF_XYZ = 0x002,
    D3D9_VIEW_FVF_XYZRHW = 0x004,
    D3D9_VIEW_FVF_DIFFUSE = 0x040,
    D3D9_VIEW_FVF_TEX1 = 0x100,
    D3D9_VIEW_POOL_MANAGED = 1,
    D3D9_VIEW_FMT_UNKNOWN = 0,
    D3D9_VIEW_FMT_A8R8G8B8 = 21,
    D3D9_VIEW_FMT_A1R5G5B5 = 25,
    D3D9_VIEW_FMT_A4R4G4B4 = 26
};
