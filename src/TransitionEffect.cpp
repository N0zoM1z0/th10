#include "Main.hpp" // Existing ANM import shielding before Windows SDK.
#include "TransitionEffect.hpp"
#include "AnmRectangle.hpp"
#include <windows.h>
#include <new>
#include <string.h>

namespace th10
{
extern CRITICAL_SECTION g_CriticalSections[7];
extern unsigned char g_ChainNestCounter;
}

// Same existing partial view used in FrontEnd.cpp. Its extern remains a
// pointer-alias hypothesis for target477810, not physical storage closure.
struct ResultGameChainView { unsigned int unknown00; unsigned int flags; };
struct ResultGameManagerView
{
    unsigned char unknown000[8];
    ResultGameChainView *updateChain;
    void *drawChain;
    AnmVmTimerView timer;
    unsigned char unknown024[0x34];
    unsigned int flags58;
    int initializationMode;
};
extern ResultGameManagerView *g_ResultGameManager;
extern int g_GameStartupCounter;

// Direct target storage, deliberately not silently replaced with a field of
// whichever render-manager pointer happens to be current. Physical aliases to
// existing supervisor/renderer owners must be established by integration.
extern AnmViewportView g_TransitionViewport; // 491CF4, retain minZ/maxZ
extern float g_TransitionShakeX;             // 491E64
extern float g_TransitionShakeY;             // 491E68
extern unsigned int __fastcall AnmRandomU32InRange(RngView *, unsigned int);

// 43BD40-43BD9C. All callbacks use the existing scheduler callback contract.
static int __fastcall TransitionFadeOut(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    if (g_GameStartupCounter != 0)
        return 7;
    if (effect->frames != 0)
    {
        effect->alpha = static_cast<int>(255.0f -
            effect->timer.subframe * 255.0f / effect->frames);
        if (effect->alpha < 0)
            effect->alpha = 0;
    }
    if (effect->timer.current >= effect->frames)
        return 7;
    effect->timer.Tick();
    return 1;
}

// 43C1A0-43C225. Rectangle renderer has its own later global-manager reload.
static int __fastcall TransitionDrawScreen(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmFloatRectView rectangle = {0.0f, 0.0f, 640.0f, 480.0f};
    manager->FlushVertexBuffer();
    g_TransitionViewport.x = 0;
    g_TransitionViewport.y = 0;
    D3d9DeviceView *device = g_Direct3DDevice;
    g_TransitionViewport.width = 640;
    g_TransitionViewport.height = 480;
    device->vtable->SetViewport(device, &g_TransitionViewport);
    DrawColoredRectangle(&rectangle,
        (static_cast<unsigned int>(effect->alpha) << 24) | effect->value0);
    return 1;
}

// 43C230-43C2B4; the terminal frame has two extra ticks.
static int __fastcall TransitionFadeIn(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    if (g_GameStartupCounter != 0)
        return 7;
    if (effect->frames != 0)
    {
        if (effect->timer.current < effect->frames)
            effect->alpha = static_cast<int>(
                effect->timer.subframe * 255.0f / effect->frames);
        else
            effect->alpha = 255;
        if (effect->alpha < 0)
            effect->alpha = 0;
    }
    // Target is 32-bit wrapping addition. Unsigned addition avoids C++ signed
    // overflow; the cast follows the target compiler's two's-complement model.
    const int end = static_cast<int>(static_cast<unsigned int>(effect->frames) + 2u);
    if (effect->timer.current >= end)
        return 7;
    ResultGameManagerView *game = g_ResultGameManager;
    if (game == NULL || (game->flags58 & 5u) == 0)
        effect->timer.Tick();
    return 1;
}

// 43C2C0-43C303.
static int __fastcall TransitionDrawPlayfield(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    AnmFloatRectView rectangle = {32.0f, 16.0f, 416.0f, 464.0f};
    DrawColoredRectangle(&rectangle,
        (static_cast<unsigned int>(effect->alpha) << 24) | effect->value0);
    return 1;
}

// 43C310-43C3BE. No startup-abort or game-manager pause gate in this mode.
static int __fastcall TransitionDim(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    AnmVmTimerView &timer = effect->timer;
    if (effect->released == 0)
    {
        if (effect->frames != 0 && timer.current <= effect->frames)
            effect->alpha = static_cast<int>(
                timer.subframe * 128.0f / effect->frames);
    }
    else
    {
        if (timer.current <= 8)
            effect->alpha = 128 - static_cast<int>(timer.subframe * 16.0f);
        else
            return 7;
    }
    timer.Tick();
    return 1;
}

// 43C3C0-43C403; unlike TransitionDrawScreen, do not reset viewport first.
static int __fastcall TransitionDrawDimScreen(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    AnmFloatRectView rectangle = {0.0f, 0.0f, 640.0f, 480.0f};
    DrawColoredRectangle(&rectangle,
        (static_cast<unsigned int>(effect->alpha) << 24) | effect->value0);
    return 1;
}

// 43C410-43C453: distinct registered callback, same playfield geometry.
static int __fastcall TransitionDrawDimPlayfield(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    AnmFloatRectView rectangle = {32.0f, 16.0f, 416.0f, 464.0f};
    DrawColoredRectangle(&rectangle,
        (static_cast<unsigned int>(effect->alpha) << 24) | effect->value0);
    return 1;
}

// 43C460-43C4F9. Return0 deliberately does not run the delete callback.
static int __fastcall TransitionPulse(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    const int abort = g_GameStartupCounter;
    const unsigned int initialAlpha = effect->value1 >> 24;
    if (abort != 0)
        return 0;
    if (effect->timer.current < effect->frames)
    {
        effect->alpha = static_cast<int>(initialAlpha) - static_cast<int>(
            initialAlpha * effect->timer.subframe / effect->frames);
        if (effect->alpha < 0)
            effect->alpha = 0;
    }
    else
    {
        effect->alpha = 0;
        --effect->value0;
        if (static_cast<int>(effect->value0) <= 0)
            return 0;
        effect->timer.SetCurrent(0);
    }
    effect->timer.Tick();
    return 1;
}

// 43C500-43C549.
static int __fastcall TransitionDrawPulse(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    AnmFloatRectView rectangle = {32.0f, 16.0f, 416.0f, 464.0f};
    DrawColoredRectangle(&rectangle, (effect->value1 & 0x00ffffffu) |
        (static_cast<unsigned int>(effect->alpha) << 24));
    return 1;
}

// 43C550-43C706. Each axis advances twice and duplicates the SECOND half.
// Deliberately not equivalent to GetRandomU32 or AnmRandomU32InRange.
static int __fastcall TransitionShakeLinear(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    if (g_GameStartupCounter != 0)
        return 7;
    effect->timer.Tick();
    if (effect->timer.current >= effect->frames)
        return 7;
    const int difference = static_cast<int>(effect->value1 - effect->value0);
    const float amplitude = difference * effect->timer.subframe / effect->frames +
        static_cast<int>(effect->value0);
    g_RngView.GetRandomU16();
    unsigned int half = g_RngView.GetRandomU16();
    unsigned int choice = ((half << 16) | half) % 3u;
    switch (choice)
    {
    case 0: g_TransitionShakeX = 0.0f; break;
    case 1: g_TransitionShakeX = amplitude; break;
    case 2: g_TransitionShakeX = -amplitude; break;
    }
    g_RngView.GetRandomU16();
    half = g_RngView.GetRandomU16();
    choice = ((half << 16) | half) % 3u;
    switch (choice)
    {
    case 0: g_TransitionShakeY = 0.0f; break;
    case 1: g_TransitionShakeY = amplitude; break;
    case 2: g_TransitionShakeY = -amplitude; break;
    }
    return 1;
}

// 43C710-43C86C. Decay's unsigned conversions are visible in target x87 code.
static int __fastcall TransitionShakeEnvelope(void *argument)
{
    TransitionEffectView *effect = static_cast<TransitionEffectView *>(argument);
    if (g_GameStartupCounter != 0)
        return 7;
    ResultGameManagerView *game = g_ResultGameManager;
    if (game == NULL || (game->flags58 & 0x77u) != 0)
        return 1;
    effect->timer.Tick();
    float envelope;
    if (effect->timer.current < static_cast<int>(effect->value0))
        envelope = effect->timer.subframe / static_cast<int>(effect->value0);
    else if (effect->timer.current < static_cast<int>(effect->value0 + effect->value1))
        envelope = 1.0f;
    else if (effect->timer.current <
        static_cast<int>(effect->value0 + effect->value1 + effect->value2))
        envelope = (static_cast<float>(effect->value0 + effect->value1 + effect->value2) -
            effect->timer.subframe) / static_cast<float>(effect->value2);
    else
        return 7;
    const float amplitude = effect->frames * envelope;
    switch (AnmRandomU32InRange(&g_RngView, 3))
    {
    case 0: g_TransitionShakeX = 0.0f; break;
    case 1: g_TransitionShakeX = amplitude; break;
    case 2: g_TransitionShakeX = -amplitude; break;
    }
    switch (AnmRandomU32InRange(&g_RngView, 3))
    {
    case 0: g_TransitionShakeY = 0.0f; break;
    case 1: g_TransitionShakeY = amplitude; break;
    case 2: g_TransitionShakeY = -amplitude; break;
    }
    return 1;
}

// 43CB00-43CB7E. Windows critical sections are recursive: return7 reaches
// this while the scheduler already holds the same lock. Do not clear pointers
// or invent idempotence; mode4 can leave updateNode stale after return0.
void TransitionEffectView::Cleanup()
{
    th10::ChainElem *node = updateNode;
    th10::Chain *chain = th10::g_Chain;
    if (node != NULL)
    {
        EnterCriticalSection(&th10::g_CriticalSections[0]);
        ++th10::g_ChainNestCounter;
        th10::Chain::UnregisterElem(node, chain);
        LeaveCriticalSection(&th10::g_CriticalSections[0]);
        --th10::g_ChainNestCounter;
    }
    node = drawNode;
    chain = th10::g_Chain;
    if (node != NULL)
    {
        EnterCriticalSection(&th10::g_CriticalSections[0]);
        ++th10::g_ChainNestCounter;
        th10::Chain::UnregisterElem(node, chain);
        LeaveCriticalSection(&th10::g_CriticalSections[0]);
        --th10::g_ChainNestCounter;
    }
}

// Logical null-safe deletion owner for43C910; 43C870 is its scheduler entry.
void DestroyTransitionEffect(TransitionEffectView *effect)
{
    if (effect != NULL)
    {
        effect->Cleanup();
        delete effect;
    }
}
static int __fastcall TransitionDelete(void *argument)
{
    DestroyTransitionEffect(static_cast<TransitionEffectView *>(argument));
    return 0;
}

// 43C930-43C968.
void TransitionEffectView::Release()
{
    released = 1;
    timer.SetCurrent(0);
}

// 43C970-43CAD4 and nine selector entries43CAD8-43CAFB.
void TransitionEffectView::Initialize(int drawPriority, int effectKind, int duration,
    unsigned int first, unsigned int second, unsigned int third)
{
    using th10::Chain;
    switch (effectKind)
    {
    case 0:
        updateNode = Chain::RegisterCalc(this, 14, TransitionFadeOut);
        drawNode = Chain::RegisterDraw(this, drawPriority, TransitionDrawScreen);
        break;
    case 1:
        updateNode = Chain::RegisterCalc(this, 14, TransitionShakeLinear);
        break;
    case 2:
        updateNode = Chain::RegisterCalc(this, 14, TransitionFadeIn);
        drawNode = Chain::RegisterDraw(this, drawPriority, TransitionDrawPlayfield);
        break;
    case 3:
        alpha = 255;
        updateNode = Chain::RegisterCalc(this, 14, TransitionFadeOut);
        drawNode = Chain::RegisterDraw(this, drawPriority, TransitionDrawPlayfield);
        break;
    case 5:
        updateNode = Chain::RegisterCalc(this, 14, TransitionFadeIn);
        drawNode = Chain::RegisterDraw(this, drawPriority, TransitionDrawScreen);
        break;
    case 4:
        updateNode = Chain::RegisterCalc(this, 14, TransitionPulse);
        drawNode = Chain::RegisterDraw(this, drawPriority, TransitionDrawPulse);
        break;
    case 6:
        updateNode = Chain::RegisterCalc(this, 14, TransitionDim);
        drawNode = Chain::RegisterDraw(this, drawPriority, TransitionDrawDimScreen);
        break;
    case 7:
        updateNode = Chain::RegisterCalc(this, 14, TransitionDim);
        drawNode = Chain::RegisterDraw(this, drawPriority, TransitionDrawDimPlayfield);
        break;
    case 8:
        updateNode = Chain::RegisterCalc(this, 14, TransitionShakeEnvelope);
        break;
    }
    // Invalid kind/failed registration is intentionally not a safe-success path.
    updateNode->deletedCallback = TransitionDelete;
    timer.SetCurrent(0);
    frames = duration;
    kind = effectKind;
    value0 = first;
    value1 = second;
    value2 = third;
}

// Existing timer member construction precedes the complete observed reset.
// This natural constructor model does not identify the original class/TU.
TransitionEffectView::TransitionEffectView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2u;
}

// Target43C8B0 calls the one-size ordinary allocator at452493.
// Historical compiler output determines the observed null-return branch;
// allocation-handler and runtime behavior remain independent obligations.
void *__stdcall StageCreateTransitionEffect(int drawPriority, int kind, int frames,
    unsigned int value0, unsigned int value1, unsigned int value2)
{
    TransitionEffectView *effect = new TransitionEffectView;
    if (effect != NULL)
    {
        effect->Initialize(drawPriority, kind, frames, value0, value1, value2);
    }
    return effect;
}
