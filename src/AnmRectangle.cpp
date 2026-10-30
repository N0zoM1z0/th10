#include "AnmRectangle.hpp"
#include "AnmManager.hpp"

// Target 0x0043BDA0-0x0043BF82, complete 483-byte owner.
// Target-private EDI rectangle / EBX packed color / plain RET. The ordinary
// signature recovers this ABI through the real Player drawing caller.
void DrawColoredRectangle(AnmFloatRectView *rectangle, unsigned int color)
{
    g_AnmRenderManagerView->FlushVertexBuffer();

    AnmUntexturedVertexView vertices[4];
    // Construct and assign each complete XYZ value before setting W and color.
    *reinterpret_cast<AnmFloat3View *>(&vertices[0]) =
        AnmFloat3View(rectangle->left, rectangle->top, 0.0f);
    *reinterpret_cast<AnmFloat3View *>(&vertices[1]) =
        AnmFloat3View(rectangle->right, rectangle->top, 0.0f);
    *reinterpret_cast<AnmFloat3View *>(&vertices[2]) =
        AnmFloat3View(rectangle->left, rectangle->bottom, 0.0f);
    *reinterpret_cast<AnmFloat3View *>(&vertices[3]) =
        AnmFloat3View(rectangle->right, rectangle->bottom, 0.0f);
    vertices[0].w = vertices[1].w = vertices[2].w = vertices[3].w = 1.0f;
    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

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
