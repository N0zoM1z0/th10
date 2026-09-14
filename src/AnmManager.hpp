#pragma once

#include <stddef.h>

// TH10's small timer-like members clear the active bit when constructed. The
// surrounding ANM VM constructor exposes nine such flag words at independent
// target-proven offsets.
struct AnmVmTimerView
{
    int previous;
    int current;
    float subframe;
    float *scale;
    unsigned int flags;

    AnmVmTimerView() { flags &= ~1u; }
    void Initialize();
};
typedef char AnmVmTimerViewSizeIs14[
    (sizeof(AnmVmTimerView) == 0x14) ? 1 : -1];

struct AnmMatrixView
{
    float values[16];

    void SetIdentity();
};
typedef char AnmMatrixViewSizeIs40[
    (sizeof(AnmMatrixView) == 0x40) ? 1 : -1];

// Exact-size view for the TH10 ANM virtual machine. Only fields established by
// the lifecycle seam are named; the animation/render state between them stays
// opaque until its consumers are reviewed.
struct AnmVmView
{
    AnmVmView();
    ~AnmVmView();
    void Initialize();

    void *unknown000;
    AnmVmView *listSelf004;
    void *unknown008;
    void *unknown00C;
    AnmVmView *layerSelf010;
    void *unknown014;
    void *unknown018;
    unsigned char unknown01C[0x004];
    void *persistentOwner020;
    unsigned char unknown024[0x018];
    float scaleX;
    float scaleY;
    unsigned char unknown044[0x018];
    AnmVmTimerView timer05C;
    unsigned char unknown070[0x030];
    AnmVmTimerView timer0A0;
    int state0B4;
    unsigned char unknown0B8[0x034];
    AnmVmTimerView timer0EC;
    int state100;
    unsigned char unknown104[0x014];
    AnmVmTimerView timer118;
    int state12C;
    unsigned char unknown130[0x034];
    AnmVmTimerView timer164;
    int state178;
    unsigned char unknown17C[0x024];
    AnmVmTimerView timer1A0;
    int state1B4;
    unsigned char unknown1B8[0x034];
    AnmVmTimerView timer1EC;
    int state200;
    unsigned char unknown204[0x014];
    AnmVmTimerView timer218;
    int state22C;
    unsigned char unknown230[0x00c];
    AnmMatrixView matrix23C;
    unsigned char unknown27C[0x080];
    int value2FC;
    unsigned char unknown300[0x008];
    void *anmFile308;
    unsigned char unknown30C[0x034];
    void *preserved340;
    void *preserved344;
    void *preserved348;
    unsigned char unknown34C[0x00c];
    void *generatedVertices;
    unsigned short flags35C;
    unsigned char unknown35E[0x00a];
    AnmVmTimerView timer368;
    unsigned char unknown37C[0x008];
    short activeSpriteIndex;
    unsigned char unknown386[0x026];
};

typedef char AnmVmViewSizeIs3AC[
    (sizeof(AnmVmView) == 0x3ac) ? 1 : -1];
typedef char AnmVmGeneratedVerticesAt358[
    (offsetof(AnmVmView, generatedVertices) == 0x358) ? 1 : -1];
typedef char AnmVmLastTimerFlagsAt378[
    (offsetof(AnmVmView, timer368.flags) == 0x378) ? 1 : -1];
typedef char AnmVmActiveSpriteAt384[
    (offsetof(AnmVmView, activeSpriteIndex) == 0x384) ? 1 : -1];

// The manager embeds two lifecycle-proven VMs at +0x14 and +0x3C0. The large
// middle region remains opaque until its individual ANM consumers are bounded.
struct AnmManagerView
{
    AnmManagerView();
    ~AnmManagerView();
    virtual size_t GetSize();
    int Initialize();
    static int __fastcall OnUpdate(AnmManagerView *manager);
    static int __fastcall DrawLayer0(AnmManagerView *manager);
    static int __fastcall DrawLayer1(AnmManagerView *manager);

    unsigned int flags004;
    int unknown008;
    void *calcChainElement;
    void *drawChainElement0;
    AnmVmView primaryVm014;
    AnmVmView secondaryVm3C0;
    unsigned char unknown76C[0x8200];
    int debugMessageCount;
    int secondaryDebugCounter;
    int captureAnmIndex;
    float defaultScaleX;
    float defaultScaleY;
    int currentRenderState;
    int unknown8984;
    int debugMessageColor;
    int currentDrawLayer;
    int updateCounter;
    void *asciiAnm;
    void *captureAnm;
    void *textAnm;
    unsigned char unknown89A0[0x008];
    void *drawChainElement;
};

typedef char AnmManagerViewSizeIs89AC[
    (sizeof(AnmManagerView) == 0x89ac) ? 1 : -1];
typedef char AnmManagerPrimaryVmAt014[
    (offsetof(AnmManagerView, primaryVm014) == 0x014) ? 1 : -1];
typedef char AnmManagerSecondaryVmAt3C0[
    (offsetof(AnmManagerView, secondaryVm3C0) == 0x3c0) ? 1 : -1];
typedef char AnmManagerCaptureIndexAt8974[
    (offsetof(AnmManagerView, captureAnmIndex) == 0x8974) ? 1 : -1];

extern AnmManagerView *g_AnmManagerView;

AnmManagerView *AnmManagerCreate();

int __stdcall AnmManagerDrawLayer0(AnmManagerView *manager);
int __stdcall AnmManagerDrawLayer1(AnmManagerView *manager);

typedef int (__fastcall *AnmChainCallback)(AnmManagerView *manager);

struct AnmChainElementView
{
    void *unknown000;
    unsigned int flags;
    unsigned char unknown008[0x018];
    AnmManagerView *argument;
};

void * __fastcall AnmLoadResource(int slot, void *fileSystem, const char *path);
AnmChainElementView * __stdcall AnmCreateChainElement(AnmChainCallback callback);
void __fastcall AnmAddCalcChainElement(
    AnmChainElementView *element, int priority, void *chain);
void __fastcall AnmAddDrawChainElement(
    AnmChainElementView *element, int priority, void *chain);
void __fastcall AnmRemoveChainElement(AnmChainElementView *element, void *chain);
void __fastcall AnmLoadedSetScript(void *anm, AnmVmView *vm, int scriptIndex);
void __fastcall AnmReleaseResource(void *resource);

struct AnmErrorLoggerView
{
    void Log(const char *message);
};

extern void *g_AnmFileSystemView;
extern void *g_AnmChainView;
extern AnmErrorLoggerView g_AnmErrorLoggerView;
extern unsigned char g_AnmChainCriticalSection[];
extern unsigned char g_AnmChainMutationDepth;

extern "C" void __stdcall EnterCriticalSection(void *criticalSection);
extern "C" void __stdcall LeaveCriticalSection(void *criticalSection);
