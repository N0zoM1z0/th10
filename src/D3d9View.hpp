#pragma once

#include <stddef.h>

// The pinned VC7.1 Platform SDK predates Direct3D 9. This target-bound prefix
// provides only the IDirect3DDevice9 slots currently used by maintained TH10
// source while preserving their real COM vtable indices.
struct D3d9DeviceView;

typedef long (__stdcall *D3d9SetRenderState)(
    D3d9DeviceView *device,
    unsigned int state,
    unsigned int value);
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
typedef long (__stdcall *D3d9SetFvf)(
    D3d9DeviceView *device,
    unsigned int fvf);

struct D3d9DeviceVtableView
{
    void *slots000To0E0[57];
    D3d9SetRenderState SetRenderState;           // +0x0E4
    void *slots0E8To100[7];
    D3d9SetTexture SetTexture;                   // +0x104
    void *slot108;
    D3d9SetTextureStageState SetTextureStageState; // +0x10C
    void *slot110;
    D3d9SetSamplerState SetSamplerState;         // +0x114
    void *slots118To148[13];
    D3d9DrawPrimitiveUp DrawPrimitiveUP;           // +0x14C
    void *slots150To160[5];
    D3d9SetFvf SetFVF;                             // +0x164
};

struct D3d9DeviceView
{
    D3d9DeviceVtableView *vtable;
};

typedef char D3d9SetRenderStateAt0E4[
    (offsetof(D3d9DeviceVtableView, SetRenderState) == 0x0e4) ? 1 : -1];
typedef char D3d9SetTextureAt104[
    (offsetof(D3d9DeviceVtableView, SetTexture) == 0x104) ? 1 : -1];
typedef char D3d9SetTextureStageStateAt10C[
    (offsetof(D3d9DeviceVtableView, SetTextureStageState) == 0x10c) ? 1 : -1];
typedef char D3d9SetSamplerStateAt114[
    (offsetof(D3d9DeviceVtableView, SetSamplerState) == 0x114) ? 1 : -1];
typedef char D3d9DrawPrimitiveUpAt14C[
    (offsetof(D3d9DeviceVtableView, DrawPrimitiveUP) == 0x14c) ? 1 : -1];
typedef char D3d9SetFvfAt164[
    (offsetof(D3d9DeviceVtableView, SetFVF) == 0x164) ? 1 : -1];

enum D3d9ViewConstants
{
    D3D9_VIEW_RS_DESTBLEND = 20,
    D3D9_VIEW_BLEND_ONE = 2,
    D3D9_VIEW_BLEND_INVSRCALPHA = 6,
    D3D9_VIEW_TSS_COLORARG2 = 3,
    D3D9_VIEW_TSS_ALPHAARG2 = 6,
    D3D9_VIEW_TA_DIFFUSE = 0,
    D3D9_VIEW_SAMP_MAGFILTER = 5,
    D3D9_VIEW_SAMP_MINFILTER = 6,
    D3D9_VIEW_TEXF_POINT = 1,
    D3D9_VIEW_TEXF_LINEAR = 2,
    D3D9_VIEW_PT_TRIANGLELIST = 4,
    D3D9_VIEW_FVF_XYZRHW = 0x004,
    D3D9_VIEW_FVF_DIFFUSE = 0x040,
    D3D9_VIEW_FVF_TEX1 = 0x100
};
