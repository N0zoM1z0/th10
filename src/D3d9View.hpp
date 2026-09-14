#pragma once

#include <stddef.h>

// The pinned VC7.1 Platform SDK predates Direct3D 9. This target-bound prefix
// provides only the IDirect3DDevice9 slots currently used by maintained TH10
// source while preserving their real COM vtable indices.
struct D3d9DeviceView;

typedef long (__stdcall *D3d9SetTextureStageState)(
    D3d9DeviceView *device,
    unsigned int stage,
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
    void *slots000To108[67];
    D3d9SetTextureStageState SetTextureStageState; // +0x10C
    void *slots110To148[15];
    D3d9DrawPrimitiveUp DrawPrimitiveUP;           // +0x14C
    void *slots150To160[5];
    D3d9SetFvf SetFVF;                             // +0x164
};

struct D3d9DeviceView
{
    D3d9DeviceVtableView *vtable;
};

typedef char D3d9SetTextureStageStateAt10C[
    (offsetof(D3d9DeviceVtableView, SetTextureStageState) == 0x10c) ? 1 : -1];
typedef char D3d9DrawPrimitiveUpAt14C[
    (offsetof(D3d9DeviceVtableView, DrawPrimitiveUP) == 0x14c) ? 1 : -1];
typedef char D3d9SetFvfAt164[
    (offsetof(D3d9DeviceVtableView, SetFVF) == 0x164) ? 1 : -1];

enum D3d9ViewConstants
{
    D3D9_VIEW_TSS_COLORARG2 = 3,
    D3D9_VIEW_TSS_ALPHAARG2 = 6,
    D3D9_VIEW_TA_DIFFUSE = 0,
    D3D9_VIEW_PT_TRIANGLELIST = 4,
    D3D9_VIEW_FVF_XYZRHW = 0x004,
    D3D9_VIEW_FVF_DIFFUSE = 0x040,
    D3D9_VIEW_FVF_TEX1 = 0x100
};
