#include "AnmRectangle.hpp"
#include "AnmManager.hpp"

// Target 0x0043BDA0-0x0043BF82, complete 483-byte owner.
// Target-private EDI rectangle / EBX packed color / plain RET. The ordinary
// source signature is semantic; linked LTCG must establish that private ABI.
void DrawColoredRectangle(AnmFloatRectView *rectangle, unsigned int color)
{
    g_AnmRenderManagerView->FlushVertexBuffer();

    AnmUntexturedVertexView vertices[4];
    vertices[0].x = rectangle->left;
    vertices[0].y = rectangle->top;
    vertices[0].z = 0.0f;
    vertices[1].x = rectangle->right;
    vertices[1].y = rectangle->top;
    vertices[1].z = 0.0f;
    vertices[2].x = rectangle->left;
    vertices[2].y = rectangle->bottom;
    vertices[2].z = 0.0f;
    vertices[3].x = rectangle->right;
    vertices[3].y = rectangle->bottom;
    vertices[3].z = 0.0f;
    vertices[3].w = vertices[2].w = vertices[1].w = vertices[0].w = 1.0f;
    vertices[3].color = vertices[2].color = vertices[1].color = vertices[0].color = color;

    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_ALPHAOP, D3D9_VIEW_TOP_SELECTARG1);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_COLOROP, D3D9_VIEW_TOP_SELECTARG1);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_ALPHAARG1, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_COLORARG1, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetRenderState(
        g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND, D3D9_VIEW_BLEND_INVSRCALPHA);
    g_Direct3DDevice->vtable->SetFVF(
        g_Direct3DDevice, D3D9_VIEW_FVF_XYZRHW | D3D9_VIEW_FVF_DIFFUSE);
    g_Direct3DDevice->vtable->DrawPrimitiveUP(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLESTRIP, 2, vertices,
        sizeof(AnmUntexturedVertexView));

    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    manager->currentVertexShader = 0xff;
    manager->currentSprite = NULL;
    manager->currentTexture = NULL;
    manager->currentColorOperation = 0xff;
    manager->currentBlendMode = 3;
    manager->currentZWrite = 0xff;

    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_ALPHAOP, D3D9_VIEW_TOP_MODULATE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_COLOROP, D3D9_VIEW_TOP_MODULATE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_ALPHAARG1, D3D9_VIEW_TA_TEXTURE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0, D3D9_VIEW_TSS_COLORARG1, D3D9_VIEW_TA_TEXTURE);
}
