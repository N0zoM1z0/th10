#include "AnmManager.hpp"

#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AsciiManagerView *g_AsciiManagerView;

// These two target helpers are also inlined by AnmVmView::Initialize. Their
// chained assignments preserve VC7.1's target-observed right-to-left stores.
void AnmMatrixView::SetIdentity()
{
    values[1] = values[2] = values[3] =
        values[4] = values[6] = values[7] =
        values[8] = values[9] = values[11] =
        values[12] = values[13] = values[14] = 0.0f;
    values[0] = values[5] = values[10] = values[15] = 1.0f;
}

void AnmVmTimerView::Initialize()
{
    current = 0;
    previous = -999999;
    subframe = 0.0f;
}

// Target 0x00402050-0x004020A7 first runs the nine embedded timer
// constructors, then clears the complete 0x3AC-byte VM and marks its active
// sprite index invalid.
AnmVmView::AnmVmView()
{
    memset(this, 0, sizeof(AnmVmView));
    activeSpriteIndex = -1;
}

// Target 0x00401FF0-0x00402011 owns the optional generated-vertex buffer.
AnmVmView::~AnmVmView()
{
    if (generatedVertices != NULL)
        free(generatedVertices);
    generatedVertices = NULL;
}

// Target 0x00401DE0-0x00401EF0 resets the reusable VM while retaining the
// externally owned pointers at +0x20 and +0x340..+0x348.
void AnmVmView::Initialize()
{
    float savedX = preservedPosition.x;
    float savedY = preservedPosition.y;
    float savedZ = preservedPosition.z;
    void *savedOwner = persistentOwner020;

    memset(this, 0, sizeof(AnmVmView));

    preservedPosition.x = savedX;
    preservedPosition.y = savedY;
    persistentOwner020 = savedOwner;
    preservedPosition.z = savedZ;

    primaryColor.value = 0xffffffff;
    scaleX = 1.0f;
    scaleY = 1.0f;
    matrix23C.SetIdentity();
    flags35C = 7;
    timer05C.Initialize();

    state0B4 = 0;
    state100 = 0;
    state12C = 0;
    state178 = 0;
    state1B4 = 0;
    state200 = 0;
    state22C = 0;

    listSelf004 = this;
    unknown008 = NULL;
    unknown00C = NULL;
    layerSelf010 = this;
    unknown014 = NULL;
    unknown018 = NULL;
}

// Target 0x00401000-0x004010FA receives this through a private ESI register.
// C++ construction accounts for the manager vtable and both embedded VM
// constructors before the body clears the complete manager state.
AsciiManagerView::AsciiManagerView()
{
    memset(this, 0, sizeof(AsciiManagerView));
    flags004 |= 2;
    g_AsciiManagerView = this;
    color = 0xffffffff;
    scaleX = 1.0f;
    scaleY = 1.0f;
    unknown8984 = 0;
    spaceWidth = 9;
}

// The manager's single-slot table at 0x0046CB14 points to target 0x00401C90.
size_t AsciiManagerView::GetSize()
{
    return sizeof(AsciiManagerView);
}

// Target 0x00401440-0x00401484 keeps the allocated manager in ESI, allowing
// LTCG to lower construction to the private ESI receiver used at 0x00401000.
AsciiManagerView *AsciiManagerCreate()
{
    AsciiManagerView *manager = new AsciiManagerView;
    if (manager->Initialize() != 0)
    {
        delete manager;
        return NULL;
    }
    return manager;
}

int AsciiManagerView::Initialize()
{
    AnmChainElementView *element;

    asciiAnm = AnmLoadResource(2, g_AnmFileSystemView, "ascii.anm");
    if (asciiAnm == NULL)
        goto fail;
    textAnm = AnmLoadResource(0, g_AnmFileSystemView, "text.anm");
    if (textAnm == NULL)
        goto fail;
    captureAnm = AnmLoadResource(3, g_AnmFileSystemView, "capture.anm");
    if (captureAnm == NULL)
        goto fail;

    element = AnmCreateChainElement(OnUpdate);
    element->flags &= ~2u;
    element->argument = this;
    AnmAddCalcChainElement(element, 4, g_AnmChainView);
    calcChainElement = element;

    element = AnmCreateChainElement(OnDrawLowPriority);
    element->flags &= ~2u;
    element->argument = this;
    AnmAddDrawChainElement(element, 0x30, g_AnmChainView);
    drawChainElement0 = element;

    element = AnmCreateChainElement(OnDrawHighPriority);
    element->flags &= ~2u;
    element->argument = this;
    AnmAddDrawChainElement(element, 0x26, g_AnmChainView);
    drawChainElement = element;

    primaryVm014.Initialize();
    primaryVm014.anmFile308 = asciiAnm;
    AnmLoadedSetScript(asciiAnm, &primaryVm014, 0);

    secondaryVm3C0.Initialize();
    secondaryVm3C0.anmFile308 = asciiAnm;
    AnmLoadedSetScript(asciiAnm, &secondaryVm3C0, 0x62);
    return 0;

fail:
    // CP932: "data is corrupted" followed by CRLF.
    g_AnmErrorLoggerView.Log(
        "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea"
        "\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
    return -1;
}

static void RemoveAnmChainElement(AnmChainElementView *element)
{
    if (element == NULL)
        return;
    EnterCriticalSection(g_AnmChainCriticalSection);
    ++g_AnmChainMutationDepth;
    AnmRemoveChainElement(element, g_AnmChainView);
    LeaveCriticalSection(g_AnmChainCriticalSection);
    --g_AnmChainMutationDepth;
}

static void ReleaseAnmResourceSlot(unsigned int offset)
{
    void **slot = reinterpret_cast<void **>(
        static_cast<unsigned char *>(g_AnmFileSystemView) + offset);
    if (*slot == NULL)
        return;
    AnmReleaseResource(*slot);
    free(*slot);
    *slot = NULL;
}

AsciiManagerView::~AsciiManagerView()
{
    RemoveAnmChainElement(
        static_cast<AnmChainElementView *>(calcChainElement));
    RemoveAnmChainElement(
        static_cast<AnmChainElementView *>(drawChainElement0));
    RemoveAnmChainElement(
        static_cast<AnmChainElementView *>(drawChainElement));

    ReleaseAnmResourceSlot(0x3ad074);
    ReleaseAnmResourceSlot(0x3ad06c);
    ReleaseAnmResourceSlot(0x3ad078);
    g_AsciiManagerView = NULL;
}

// Target 0x004014D0-0x004014E9 is a retained member form of the queue reset.
int AsciiManagerView::ResetStrings()
{
    numStrings = 0;
    numGuiStrings = 0;
    ++frameCounter;
    return 1;
}

// Target 0x004014F0-0x00401509 is the calculation-chain callback form.
int __fastcall AsciiManagerView::OnUpdate(AsciiManagerView *manager)
{
    manager->numStrings = 0;
    manager->numGuiStrings = 0;
    ++manager->frameCounter;
    return 1;
}

// Target 0x00401510-0x00401516 adapts the fastcall chain ABI to the stack
// receiver used by the primary ANM draw-layer owner at 0x00401760.
int __fastcall AsciiManagerView::OnDrawLowPriority(AsciiManagerView *manager)
{
    return AsciiManagerDrawStrings(manager);
}

// Target 0x00401520-0x00401529 preserves the manager in EBX for the secondary
// draw owner at 0x00401A50. The natural wrapper keeps that optimizer choice open.
int __fastcall AsciiManagerView::OnDrawHighPriority(AsciiManagerView *manager)
{
    return AsciiManagerDrawGuiStrings(manager);
}

// Target 0x00401530-0x004015B3 appends one of at most 256 regular strings.
// Its register-only EAX/EBX text and position arguments are an LTCG lowering
// of this ordinary member call, as shown by both variadic formatter callers.
void AsciiManagerView::AddString(
    AnmFloat3View *position, const char *text)
{
    AsciiManagerStringView *nextString;

    if (numStrings >= 256)
        return;

    nextString = &strings[numStrings];
    ++numStrings;
    strcpy(nextString->text, text);
    nextString->position = *position;
    nextString->color = color;
    nextString->scaleX = scaleX;
    nextString->scaleY = scaleY;
    nextString->viewportIndex = viewportIndex;
    nextString->smallFont = 0;
    nextString->drawShadow = drawShadow;
}

// Target 0x004015C0-0x0040162C is the analogous 64-entry GUI queue append.
// This CC-delimited owner is absent from Ghidra's current function inventory.
void AsciiManagerView::AddGuiString(
    AnmFloat3View *position, const char *text)
{
    AsciiManagerStringView *nextString;

    if (numGuiStrings >= 64)
        return;

    nextString = &guiStrings[numGuiStrings];
    ++numGuiStrings;
    strcpy(nextString->text, text);
    nextString->position = *position;
    nextString->color = color;
    nextString->scaleX = scaleX;
    nextString->scaleY = scaleY;
    nextString->viewportIndex = viewportIndex;
    nextString->smallFont = 0;
}

// Target 0x00401630-0x00401684 formats into the target-observed 512-byte
// stack buffer and then appends to the regular queue.
void AsciiManagerView::AddFormatText(
    AnmFloat3View *position, const char *format, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    AddString(position, buffer);
    va_end(args);
}

// Target 0x00401690-0x004016F3 uses the same regular queue and marks the
// newly appended record to select the second 0x62-glyph font bank.
void AsciiManagerView::AddSmallFormatText(
    AnmFloat3View *position, const char *format, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    AddString(position, buffer);
    va_end(args);
    strings[numStrings - 1].smallFont = 1;
}

// Target 0x00401700-0x0040175D appends to the GUI queue and returns the
// formatted byte count. The compiler inlines strlen after the queue call.
int AsciiManagerView::AddGuiFormatText(
    AnmFloat3View *position, const char *format, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    AddGuiString(position, buffer);
    va_end(args);
    return strlen(buffer);
}

static void ConfigureAsciiViewport(int viewportIndex)
{
    if (viewportIndex != 0)
        AsciiConfigureBackgroundViewport(0);
    else
        AsciiConfigureBackgroundViewport(1);
}

// Target 0x004423C0-0x004423D8 multiplies two color channels with 7-bit
// normalization and saturates the result to one byte.
unsigned char __fastcall MixAnmColor(
    unsigned char source, unsigned char multiplier)
{
    unsigned int mixed = (source * multiplier) / 128u;
    if (mixed >= 256u)
        mixed = 255u;
    return static_cast<unsigned char>(mixed);
}

// Target 0x004425A0-0x00442668 batches sprites until either the blend mode or
// texture filter changes. The VM remains live in EDI in the target LTCG seam.
void AnmRenderManagerView::SetRenderStateForVm(AnmVmView *vm)
{
    if (currentBlendMode != vm->blendMode)
    {
        FlushVertexBuffer();
        currentBlendMode = static_cast<unsigned char>(vm->blendMode);

        switch (currentBlendMode)
        {
        case 0:
            g_Direct3DDevice->vtable->SetRenderState(
                g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND,
                D3D9_VIEW_BLEND_INVSRCALPHA);
            break;
        case 1:
            g_Direct3DDevice->vtable->SetRenderState(
                g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND,
                D3D9_VIEW_BLEND_ONE);
            break;
        }
    }

    if (currentTextureFilter != vm->usePointTextureFilter)
    {
        FlushVertexBuffer();
        currentTextureFilter =
            static_cast<unsigned char>(vm->usePointTextureFilter);

        if (currentTextureFilter == 0)
        {
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MAGFILTER,
                D3D9_VIEW_TEXF_LINEAR);
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MINFILTER,
                D3D9_VIEW_TEXF_LINEAR);
        }
        else
        {
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MAGFILTER,
                D3D9_VIEW_TEXF_POINT);
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MINFILTER,
                D3D9_VIEW_TEXF_POINT);
        }
    }

    ++renderStateChangesThisFrame;
}

extern const float g_AnmHalfPixel = 0.5f;

// Target 0x00442670-0x00442AC6 applies screen shake and the D3D9 half-pixel
// convention, rejects quads outside the active viewport, updates batched
// texture/render state, resolves VM color, and queues the resulting vertices.
int AnmRenderManagerView::DrawInner(AnmVmView *vm, int flags)
{
    AnmColorView color;
    float maxX;
    float minX;
    float maxY;
    float minY;

    g_AnmQuadVertices[0].x += screenShakeX;
    g_AnmQuadVertices[0].y += screenShakeY;
    g_AnmQuadVertices[1].x += screenShakeX;
    g_AnmQuadVertices[1].y += screenShakeY;
    g_AnmQuadVertices[2].x += screenShakeX;
    g_AnmQuadVertices[2].y += screenShakeY;
    g_AnmQuadVertices[3].x += screenShakeX;
    g_AnmQuadVertices[3].y += screenShakeY;

    if ((flags & 1) != 0)
    {
#if defined(_MSC_VER) && defined(_M_IX86)
        // VC7.1 has no C/C++ intrinsic that emits the target's four FRNDINT
        // operations while retaining all values on the x87 stack. This narrow
        // source-level x87 block is also the form preserved by the adjacent
        // engine family; every address below remains a normal linker field.
        __asm
        {
            fld g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmQuadVertices[1 * TYPE g_AnmQuadVertices].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].y
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmQuadVertices[2 * TYPE g_AnmQuadVertices].y
            frndint
            fsub g_AnmHalfPixel
            fst g_AnmQuadVertices[2 * TYPE g_AnmQuadVertices].y
            fstp g_AnmQuadVertices[3 * TYPE g_AnmQuadVertices].y
            fst g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].y
            fstp g_AnmQuadVertices[1 * TYPE g_AnmQuadVertices].y
            fst g_AnmQuadVertices[1 * TYPE g_AnmQuadVertices].x
            fstp g_AnmQuadVertices[3 * TYPE g_AnmQuadVertices].x
            fst g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].x
            fstp g_AnmQuadVertices[2 * TYPE g_AnmQuadVertices].x
        }
#else
        maxX = static_cast<float>(floor(g_AnmQuadVertices[0].x + 0.5f)) -
            g_AnmHalfPixel;
        minX = static_cast<float>(floor(g_AnmQuadVertices[1].x + 0.5f)) -
            g_AnmHalfPixel;
        maxY = static_cast<float>(floor(g_AnmQuadVertices[0].y + 0.5f)) -
            g_AnmHalfPixel;
        minY = static_cast<float>(floor(g_AnmQuadVertices[2].y + 0.5f)) -
            g_AnmHalfPixel;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y = minY;
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y = maxY;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x = minX;
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x = maxX;
#endif
    }

    g_AnmQuadVertices[0].u = g_AnmQuadVertices[2].u =
        vm->loadedSprite->uStart + vm->uvScrollX;
    g_AnmQuadVertices[1].u = g_AnmQuadVertices[3].u =
        vm->loadedSprite->uEnd + vm->uvScrollX;
    g_AnmQuadVertices[0].v = g_AnmQuadVertices[1].v =
        vm->loadedSprite->vStart + vm->uvScrollY;
    g_AnmQuadVertices[2].v = g_AnmQuadVertices[3].v =
        vm->loadedSprite->vEnd + vm->uvScrollY;

    maxX = g_AnmQuadVertices[0].x > g_AnmQuadVertices[1].x
        ? g_AnmQuadVertices[0].x : g_AnmQuadVertices[1].x;
    maxX = g_AnmQuadVertices[2].x > maxX
        ? g_AnmQuadVertices[2].x : maxX;
    maxX = g_AnmQuadVertices[3].x > maxX
        ? g_AnmQuadVertices[3].x : maxX;

    maxY = g_AnmQuadVertices[0].y > g_AnmQuadVertices[1].y
        ? g_AnmQuadVertices[0].y : g_AnmQuadVertices[1].y;
    maxY = g_AnmQuadVertices[2].y > maxY
        ? g_AnmQuadVertices[2].y : maxY;
    maxY = g_AnmQuadVertices[3].y > maxY
        ? g_AnmQuadVertices[3].y : maxY;

    minX = g_AnmQuadVertices[0].x < g_AnmQuadVertices[1].x
        ? g_AnmQuadVertices[0].x : g_AnmQuadVertices[1].x;
    minX = g_AnmQuadVertices[2].x < minX
        ? g_AnmQuadVertices[2].x : minX;
    minX = g_AnmQuadVertices[3].x < minX
        ? g_AnmQuadVertices[3].x : minX;

    minY = g_AnmQuadVertices[0].y < g_AnmQuadVertices[1].y
        ? g_AnmQuadVertices[0].y : g_AnmQuadVertices[1].y;
    minY = g_AnmQuadVertices[2].y < minY
        ? g_AnmQuadVertices[2].y : minY;
    minY = g_AnmQuadVertices[3].y < minY
        ? g_AnmQuadVertices[3].y : minY;

    if (maxX < g_AnmViewportOwner->viewport.x ||
        maxY < g_AnmViewportOwner->viewport.y ||
        minX > g_AnmViewportOwner->viewport.x +
            g_AnmViewportOwner->viewport.width ||
        minY > g_AnmViewportOwner->viewport.y +
            g_AnmViewportOwner->viewport.height)
        return 0;

    if (currentTexture != vm->loadedSprite->texture)
    {
        currentTexture = vm->loadedSprite->texture;
        FlushVertexBuffer();
        g_Direct3DDevice->vtable->SetTexture(
            g_Direct3DDevice, 0, currentTexture);
    }

    if (currentVertexShader != 1)
    {
        FlushVertexBuffer();
        currentVertexShader = 1;
    }

    if ((flags & 2) == 0)
    {
        color.value = vm->useSecondaryColor
            ? vm->secondaryColor.value : vm->primaryColor.value;
        if (useMixColor)
        {
            color.red = MixAnmColor(color.red, mixColor.red);
            color.green = MixAnmColor(color.green, mixColor.green);
            color.blue = MixAnmColor(color.blue, mixColor.blue);
            color.alpha = MixAnmColor(color.alpha, mixColor.alpha);
        }

        g_AnmQuadVertices[0].color = color.value;
        g_AnmQuadVertices[1].color = color.value;
        g_AnmQuadVertices[2].color = color.value;
        g_AnmQuadVertices[3].color = color.value;
    }

    SetRenderStateForVm(vm);
    AddSpriteToDrawBuffer(g_AnmQuadVertices);
    return 0;
}

// Target 0x00442F30-0x00442F4C resets the shared packed-vertex range. This
// CC-delimited retained owner is absent from Ghidra's current inventory.
void AnmRenderManagerView::ClearVertexBuffer()
{
    spritesToDraw = 0;
    vertexBufferStart = vertexBufferEnd = vertexBuffer;
}

// Target 0x00442F50-0x00442FD4 submits two triangle-list primitives per
// queued sprite, advances the submitted range, and records one flush.
void AnmRenderManagerView::FlushVertexBuffer()
{
    if (spritesToDraw == 0)
        return;

    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetFVF(
        g_Direct3DDevice,
        D3D9_VIEW_FVF_XYZRHW |
        D3D9_VIEW_FVF_DIFFUSE |
        D3D9_VIEW_FVF_TEX1);
    g_Direct3DDevice->vtable->DrawPrimitiveUP(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLELIST,
        spritesToDraw * 2, vertexBufferStart,
        sizeof(AnmRenderVertexView));

    vertexBufferStart = vertexBufferEnd;
    spritesToDraw = 0;
    ++flushesThisFrame;
}

// Target 0x00442FE0-0x0044307C expands one four-corner quad into the six
// packed vertices consumed as two triangles by FlushVertexBuffer.
int AnmRenderManagerView::AddSpriteToDrawBuffer(
    AnmRenderVertexView *vertices)
{
    vertexBufferEnd[0] = vertices[0];
    vertexBufferEnd[1] = vertices[1];
    vertexBufferEnd[2] = vertices[2];
    vertexBufferEnd[3] = vertices[1];
    vertexBufferEnd[4] = vertices[2];
    vertexBufferEnd[5] = vertices[3];
    vertexBufferEnd += 6;
    ++spritesToDraw;
    return 0;
}

// Target 0x00443080-0x00443282 places one axis-aligned quad. Centered axes
// are rounded down before the common render-state/draw path is entered.
int AnmRenderManagerView::DrawNoRotation(AnmVmView *vm)
{
    float spriteWidth;
    float spriteHeight;
    float spriteHalfHeight;

    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;
    spriteHalfHeight = spriteHeight * 0.5f;

    switch ((vm->flags35C >> 18) & 3)
    {
    case 1:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x +
            spriteWidth;
        break;
    case 0:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            static_cast<float>(floor(
                vm->position.x + vm->preservedPosition.x +
                vm->spriteOffset.x - spriteWidth * 0.5f));
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            g_AnmQuadVertices[0].x + spriteWidth;
        break;
    case 2:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x -
            spriteWidth;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x;
        break;
    }

    switch ((vm->flags35C >> 20) & 3)
    {
    case 1:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            vm->position.y + vm->preservedPosition.y + vm->spriteOffset.y;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            vm->position.y + vm->preservedPosition.y + vm->spriteOffset.y +
            spriteHeight;
        break;
    case 0:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            static_cast<float>(floor(
                vm->position.y + vm->preservedPosition.y +
                vm->spriteOffset.y - spriteHalfHeight));
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            g_AnmQuadVertices[0].y + spriteHeight;
        break;
    case 2:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            vm->position.y + vm->preservedPosition.y + vm->spriteOffset.y -
            spriteHeight;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            vm->position.y + vm->preservedPosition.y + vm->spriteOffset.y;
        break;
    }

    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->spriteOffset.z + vm->preservedPosition.z + vm->position.z;
    return DrawInner(vm, 1);
}

// Target 0x00443290-0x00443475 preserves sub-pixel centered coordinates and
// requests the non-rounded common draw path.
int AnmRenderManagerView::DrawNoRotationNoRound(AnmVmView *vm)
{
    float spriteWidth;
    float spriteHeight;
    float spriteHalfHeight;

    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;
    spriteHalfHeight = spriteHeight * 0.5f;

    switch ((vm->flags35C >> 18) & 3)
    {
    case 1:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x +
            spriteWidth;
        break;
    case 0:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x -
            spriteWidth * 0.5f;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            g_AnmQuadVertices[0].x + spriteWidth;
        break;
    case 2:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x -
            spriteWidth;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->preservedPosition.x + vm->spriteOffset.x;
        break;
    }

    switch ((vm->flags35C >> 20) & 3)
    {
    case 1:
    {
        float y = vm->spriteOffset.y;
        y += vm->position.y;
        y += vm->preservedPosition.y;
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            y;
        y = vm->spriteOffset.y;
        y += vm->position.y;
        y += spriteHeight;
        y += vm->preservedPosition.y;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            y;
        break;
    }
    case 0:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            vm->position.y + vm->preservedPosition.y + vm->spriteOffset.y -
            spriteHalfHeight;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            g_AnmQuadVertices[0].y + spriteHeight;
        break;
    case 2:
    {
        float y = vm->spriteOffset.y;
        y += vm->position.y;
        y += vm->preservedPosition.y;
        y -= spriteHeight;
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            y;
        y = vm->spriteOffset.y;
        y += vm->position.y;
        y += vm->preservedPosition.y;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            y;
        break;
    }
    }

    // TH10's no-round path intentionally reads spriteOffset.y here; the
    // rounded sibling at 0x00443080 reads spriteOffset.z.
    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->spriteOffset.y + vm->preservedPosition.z + vm->position.z;
    return DrawInner(vm, 0);
}

// Target 0x00443680-0x004436B1 applies the renderer's two-dimensional
// rotation and translation to one shared textured vertex. The member receiver
// is unused and disappears from the target's LTCG-internal calling convention.
void AnmRenderManagerView::TranslateRotation(
    AnmRenderVertexView *vertex, float x, float y, float sine,
    float cosine, float xOffset, float yOffset)
{
    vertex->x = x * cosine - y * sine + xOffset;
    vertex->y = x * sine + y * cosine + yOffset;
}

// Target 0x004436C0-0x0044390E is the first of two instruction-identical
// rotated-quad blocks reached by render modes 1 and 3 in the Draw dispatcher.
// TH10 accumulates all three VM position vectors on every axis before rotating
// the anchor-selected corners. A zero rotation enters the established
// sub-pixel axis-aligned path.
int AnmRenderManagerView::Draw2D(AnmVmView *vm)
{
    float rotation;
    float sine;
    float cosine;
    float xOffset;
    float yOffset;
    float spriteWidth;
    float spriteHeight;
    float vertexX[4];
    float vertexY[4];
    int i;

    if (vm->rotation.z == 0.0f)
        return DrawNoRotationNoRound(vm);

    rotation = vm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    xOffset = vm->spriteOffset.x;
    xOffset += vm->preservedPosition.x;
    xOffset += vm->position.x;
    yOffset = vm->spriteOffset.y;
    yOffset += vm->preservedPosition.y;
    yOffset += vm->position.y;
    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;

    switch (vm->renderStateA)
    {
    case 1:
        vertexX[0] = vertexX[2] = 0.0f;
        vertexX[1] = vertexX[3] = spriteWidth;
        break;
    case 0:
        vertexX[0] = vertexX[2] = -spriteWidth * 0.5f;
        vertexX[1] = vertexX[3] = spriteWidth * 0.5f;
        break;
    case 2:
        vertexX[0] = vertexX[2] = -spriteWidth;
        vertexX[1] = vertexX[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertexY[0] = vertexY[1] = 0.0f;
        vertexY[2] = vertexY[3] = spriteHeight;
        break;
    case 0:
        vertexY[0] = vertexY[1] = -spriteHeight * 0.5f;
        vertexY[2] = vertexY[3] = spriteHeight * 0.5f;
        break;
    case 2:
        vertexY[0] = vertexY[1] = -spriteHeight;
        vertexY[2] = vertexY[3] = 0.0f;
        break;
    }

    for (i = 0; i < 4; ++i)
    {
        TranslateRotation(
            &g_AnmQuadVertices[i], vertexX[i], vertexY[i], sine, cosine,
            xOffset, yOffset);
    }

    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->spriteOffset.z + vm->preservedPosition.z + vm->position.z;
    return DrawInner(vm, 0);
}

// Target 0x00443910-0x00443B5E is the render-mode 3 sibling retained as a
// separate method. Its complete target body is instruction-identical to
// Draw2D apart from address-dependent REL32 displacements. The descriptive
// name follows the mode-3 owner in TH08 and remains a cross-game hypothesis.
int AnmRenderManagerView::Draw2DRotatedOrAxisAligned(AnmVmView *vm)
{
    float rotation;
    float sine;
    float cosine;
    float xOffset;
    float yOffset;
    float spriteWidth;
    float spriteHeight;
    float vertexX[4];
    float vertexY[4];
    int i;

    if (vm->rotation.z == 0.0f)
        return DrawNoRotationNoRound(vm);

    rotation = vm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    xOffset = vm->spriteOffset.x;
    xOffset += vm->preservedPosition.x;
    xOffset += vm->position.x;
    yOffset = vm->spriteOffset.y;
    yOffset += vm->preservedPosition.y;
    yOffset += vm->position.y;
    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;

    switch (vm->renderStateA)
    {
    case 1:
        vertexX[0] = vertexX[2] = 0.0f;
        vertexX[1] = vertexX[3] = spriteWidth;
        break;
    case 0:
        vertexX[0] = vertexX[2] = -spriteWidth * 0.5f;
        vertexX[1] = vertexX[3] = spriteWidth * 0.5f;
        break;
    case 2:
        vertexX[0] = vertexX[2] = -spriteWidth;
        vertexX[1] = vertexX[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertexY[0] = vertexY[1] = 0.0f;
        vertexY[2] = vertexY[3] = spriteHeight;
        break;
    case 0:
        vertexY[0] = vertexY[1] = -spriteHeight * 0.5f;
        vertexY[2] = vertexY[3] = spriteHeight * 0.5f;
        break;
    case 2:
        vertexY[0] = vertexY[1] = -spriteHeight;
        vertexY[2] = vertexY[3] = 0.0f;
        break;
    }

    for (i = 0; i < 4; ++i)
    {
        TranslateRotation(
            &g_AnmQuadVertices[i], vertexX[i], vertexY[i], sine, cosine,
            xOffset, yOffset);
    }

    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->spriteOffset.z + vm->preservedPosition.z + vm->position.z;
    return DrawInner(vm, 0);
}

static __forceinline float AnmFloat3Length(const AnmFloat3View &value)
{
    return static_cast<float>(sqrt(
        value.x * value.x + value.y * value.y + value.z * value.z));
}

// Target 0x00443B60-0x00443F76 projects the VM's accumulated world position
// and the current camera-right reference through the active background
// viewport. The projected reference fixes the screen-space sprite scale; the
// VM's Z rotation and two anchor fields then place the shared quad. Unlike the
// adjacent TH095 implementation, TH10 stores the projected depth in every
// vertex.
int AnmRenderManagerView::ProjectCameraFacingQuad(AnmVmView *vm)
{
    float rotation;
    float sine;
    float cosine;
    float xOffset;
    float yOffset;
    float spriteHeight;
    float spriteWidth;
    AnmFloat3View projectedReference;
    AnmMatrixView worldMatrix;
    AnmFloat3View projectedPosition;
    AnmFloat3View origin;
    AnmFloat3View delta;
    float vertexX[4];
    float vertexY[4];
    int i;

    rotation = vm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    // Both target sites use one x87 FSINCOS and pop cosine before sine. VC7.1
    // has no C/C++ intrinsic that retains this instruction shape.
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    // These two local identifiers retain the original build-6030 LTCG stack
    // coloring: this first vector supplies the zero projection input, while
    // origin below receives the projected camera-right reference.
    projectedReference.x = 0.0f;
    projectedReference.y = 0.0f;
    projectedReference.z = 0.0f;

    worldMatrix.SetIdentity();
    worldMatrix.values[12] =
        vm->spriteOffset.x + vm->preservedPosition.x + vm->position.x;
    worldMatrix.values[13] =
        vm->spriteOffset.y + vm->preservedPosition.y + vm->position.y;
    worldMatrix.values[14] =
        vm->spriteOffset.z + vm->preservedPosition.z + vm->position.z;

    D3DXVec3Project(
        &projectedPosition, &projectedReference,
        &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);
    if (projectedPosition.z < 0.0f || projectedPosition.z > 1.0f)
        return -1;

    D3DXVec3Project(
        &origin, &g_AnmViewportOwner->cameraRight,
        &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);

    delta = origin - projectedPosition;
    xOffset = AnmFloat3Length(delta) * 0.5f;
    spriteWidth = vm->spriteWidth * vm->scaleX * xOffset;
    spriteHeight = vm->spriteHeight * vm->scaleY * xOffset;
    xOffset = projectedPosition.x;
    yOffset = projectedPosition.y;
    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            projectedPosition.z;

#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    switch (vm->renderStateA)
    {
    case 1:
        vertexX[0] = vertexX[2] = 0.0f;
        vertexX[1] = vertexX[3] = spriteWidth;
        break;
    case 0:
        vertexX[0] = vertexX[2] = -spriteWidth * 0.5f;
        vertexX[1] = vertexX[3] = spriteWidth * 0.5f;
        break;
    case 2:
        vertexX[0] = vertexX[2] = -spriteWidth;
        vertexX[1] = vertexX[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertexY[0] = vertexY[1] = 0.0f;
        vertexY[2] = vertexY[3] = spriteHeight;
        break;
    case 0:
        vertexY[0] = vertexY[1] = -spriteHeight * 0.5f;
        vertexY[2] = vertexY[3] = spriteHeight * 0.5f;
        break;
    case 2:
        vertexY[0] = vertexY[1] = -spriteHeight;
        vertexY[2] = vertexY[3] = 0.0f;
        break;
    }

    for (i = 0; i < 4; ++i)
    {
        TranslateRotation(
            &g_AnmQuadVertices[i], vertexX[i], vertexY[i], sine, cosine,
            xOffset, yOffset);
    }

    return 0;
}

// Target 0x00443F80-0x00443FA0 draws the projected quad through the common
// non-rounded renderer path and propagates projection failure.
int AnmRenderManagerView::DrawCameraFacingQuad(AnmVmView *vm)
{
    if (ProjectCameraFacingQuad(vm) != 0)
        return -1;
    return DrawInner(vm, 0);
}

// Target 0x00401760-0x00401A41 renders the 256-entry regular queue. Every
// field read below is independently visible in the target body; the renderer
// and viewport helper names remain semantic until their own owners are closed.
int __stdcall AsciiManagerDrawStrings(AsciiManagerView *manager)
{
    float spaceWidth;
    float lineHeight;
    int currentViewport = 1;
    int index;
    AsciiManagerStringView *string = manager->strings;

    // The target writes the complete containing dword and establishes the
    // two high render-state bits as well as the low visible bit.
    *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) =
        (*reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) &
         0xffd7ffffu) | 0x00140001u;

    for (index = 0; index < manager->numStrings; ++index, ++string)
    {
        unsigned char *text;

        manager->primaryVm014.position = string->position;
        manager->primaryVm014.scaleX = string->scaleX;
        manager->primaryVm014.scaleY = string->scaleY;
        *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) |= 8;

        if (string->smallFont == 1)
        {
            spaceWidth = 7.0f;
            lineHeight = 9.0f;
        }
        else
        {
            spaceWidth = manager->spaceWidth * string->scaleX;
            lineHeight = 14.0f;
        }

        if (currentViewport != string->viewportIndex)
        {
            g_AnmRenderManagerView->FlushVertexBuffer();
            currentViewport = string->viewportIndex;
            ConfigureAsciiViewport(currentViewport);
        }

        text = reinterpret_cast<unsigned char *>(string->text);
        while (*text != '\0')
        {
            if (*text == '\n')
            {
                manager->primaryVm014.position.y +=
                    lineHeight * string->scaleY;
                manager->primaryVm014.position.x = string->position.x;
            }
            else if (*text != ' ')
            {
                AnmSpriteView *sprite =
                    &manager->asciiAnm->sprites[
                        string->smallFont * 0x62 + *text - ' '];
                manager->primaryVm014.loadedSprite = sprite;
                manager->primaryVm014.spriteWidth = sprite->width;
                manager->primaryVm014.spriteHeight = sprite->height;

                if (string->drawShadow != 0)
                {
                    manager->primaryVm014.primaryColor.value =
                        static_cast<int>(string->color & 0xff000000u);
                    reinterpret_cast<unsigned char *>(
                        &manager->primaryVm014.primaryColor.value)[3] =
                            static_cast<unsigned char>(string->color >> 25);
                    manager->primaryVm014.position.x += 2.0f;
                    manager->primaryVm014.position.y += 2.0f;
                    g_AnmRenderManagerView->DrawNoRotation(
                        &manager->primaryVm014);
                    manager->primaryVm014.position.x -= 2.0f;
                    manager->primaryVm014.position.y -= 2.0f;
                }

                manager->primaryVm014.primaryColor.value = string->color;
                g_AnmRenderManagerView->DrawNoRotation(
                    &manager->primaryVm014);
            }
            manager->primaryVm014.position.x += spaceWidth;
            ++text;
        }
    }

    if (currentViewport != 0)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        ConfigureAsciiViewport(0);
    }
    return 1;
}

// Target 0x00401A50-0x00401C89 renders the 64-entry GUI queue. The sole draw
// mode split is the exact floating-point comparison against scale 1.0.
int __stdcall AsciiManagerDrawGuiStrings(AsciiManagerView *manager)
{
    float spaceWidth;
    int currentViewport = 1;
    int index;
    AsciiManagerStringView *string = manager->guiStrings;

    *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) =
        (*reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) &
         0xffd7ffffu) | 0x00140001u;

    for (index = 0; index < manager->numGuiStrings; ++index, ++string)
    {
        unsigned char *text;

        manager->primaryVm014.position = string->position;
        manager->primaryVm014.scaleX = string->scaleX;
        manager->primaryVm014.scaleY = string->scaleY;
        *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) |= 8;
        spaceWidth = manager->spaceWidth * string->scaleX;

        if (currentViewport != string->viewportIndex)
        {
            g_AnmRenderManagerView->FlushVertexBuffer();
            currentViewport = string->viewportIndex;
            ConfigureAsciiViewport(currentViewport);
        }

        text = reinterpret_cast<unsigned char *>(string->text);
        while (*text != '\0')
        {
            if (*text == '\n')
            {
                manager->primaryVm014.position.y += 14.0f * string->scaleY;
                manager->primaryVm014.position.x = string->position.x;
            }
            else if (*text != ' ')
            {
                AnmSpriteView *sprite =
                    &manager->asciiAnm->sprites[*text - ' '];
                manager->primaryVm014.loadedSprite = sprite;
                manager->primaryVm014.primaryColor.value = string->color;
                if (manager->primaryVm014.scaleX == 1.0f)
                    g_AnmRenderManagerView->DrawNoRotation(
                        &manager->primaryVm014);
                else
                    g_AnmRenderManagerView->DrawNoRotationNoRound(
                        &manager->primaryVm014);
            }
            manager->primaryVm014.position.x += spaceWidth;
            ++text;
        }
    }

    if (currentViewport != 0)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        ConfigureAsciiViewport(0);
    }
    return 1;
}
