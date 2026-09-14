#include "AnmManager.hpp"

#include <stdlib.h>
#include <string.h>

AnmManagerView *g_AnmManagerView;

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
    void *saved340 = preserved340;
    void *saved344 = preserved344;
    void *saved348 = preserved348;
    void *savedOwner = persistentOwner020;

    memset(this, 0, sizeof(AnmVmView));

    preserved340 = saved340;
    preserved344 = saved344;
    persistentOwner020 = savedOwner;
    preserved348 = saved348;

    value2FC = -1;
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
AnmManagerView::AnmManagerView()
{
    memset(this, 0, sizeof(AnmManagerView));
    flags004 |= 2;
    g_AnmManagerView = this;
    captureAnmIndex = -1;
    defaultScaleX = 1.0f;
    defaultScaleY = 1.0f;
    unknown8984 = 0;
    currentDrawLayer = 9;
}

// The manager's single-slot table at 0x0046CB14 points to target 0x00401C90.
size_t AnmManagerView::GetSize()
{
    return sizeof(AnmManagerView);
}

// Target 0x00401440-0x00401484 keeps the allocated manager in ESI, allowing
// LTCG to lower construction to the private ESI receiver used at 0x00401000.
AnmManagerView *AnmManagerCreate()
{
    AnmManagerView *manager = new AnmManagerView;
    if (manager->Initialize() != 0)
    {
        delete manager;
        return NULL;
    }
    return manager;
}

int AnmManagerView::Initialize()
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

    element = AnmCreateChainElement(DrawLayer0);
    element->flags &= ~2u;
    element->argument = this;
    AnmAddDrawChainElement(element, 0x30, g_AnmChainView);
    drawChainElement0 = element;

    element = AnmCreateChainElement(DrawLayer1);
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

AnmManagerView::~AnmManagerView()
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
    g_AnmManagerView = NULL;
}

// Target 0x004014F0-0x00401509 is the manager's calculation-chain callback.
int __fastcall AnmManagerView::OnUpdate(AnmManagerView *manager)
{
    manager->debugMessageCount = 0;
    manager->secondaryDebugCounter = 0;
    ++manager->updateCounter;
    return 1;
}

// Target 0x00401510-0x00401516 adapts the fastcall chain ABI to the stack
// receiver used by the primary ANM draw-layer owner at 0x00401760.
int __fastcall AnmManagerView::DrawLayer0(AnmManagerView *manager)
{
    return AnmManagerDrawLayer0(manager);
}

// Target 0x00401520-0x00401529 preserves the manager in EBX for the secondary
// draw owner at 0x00401A50. The natural wrapper keeps that optimizer choice open.
int __fastcall AnmManagerView::DrawLayer1(AnmManagerView *manager)
{
    return AnmManagerDrawLayer1(manager);
}
