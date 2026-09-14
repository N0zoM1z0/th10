#pragma once

#include <stddef.h>

struct AnmFloat3View
{
    float x;
    float y;
    float z;
};

typedef char AnmFloat3ViewSizeIs0C[
    (sizeof(AnmFloat3View) == 0x0c) ? 1 : -1];

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
    unsigned char unknown044[0x008];
    float spriteWidth;
    float spriteHeight;
    unsigned char unknown054[0x008];
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
    unsigned char unknown30C[0x028];
    AnmFloat3View position;
    AnmFloat3View preservedPosition;
    AnmFloat3View spriteOffset;
    void *generatedVertices;
    unsigned short flags35C;
    unsigned char unknown35E[0x00a];
    AnmVmTimerView timer368;
    unsigned char unknown37C[0x008];
    short activeSpriteIndex;
    unsigned char unknown386[0x00e];
    void *loadedSprite;
    unsigned char unknown398[0x014];
};

typedef char AnmVmViewSizeIs3AC[
    (sizeof(AnmVmView) == 0x3ac) ? 1 : -1];
typedef char AnmVmGeneratedVerticesAt358[
    (offsetof(AnmVmView, generatedVertices) == 0x358) ? 1 : -1];
typedef char AnmVmLastTimerFlagsAt378[
    (offsetof(AnmVmView, timer368.flags) == 0x378) ? 1 : -1];
typedef char AnmVmActiveSpriteAt384[
    (offsetof(AnmVmView, activeSpriteIndex) == 0x384) ? 1 : -1];
typedef char AnmVmPositionAt334[
    (offsetof(AnmVmView, position) == 0x334) ? 1 : -1];
typedef char AnmVmLoadedSpriteAt394[
    (offsetof(AnmVmView, loadedSprite) == 0x394) ? 1 : -1];

struct AnmSpriteView
{
    unsigned char unknown000[0x030];
    float width;
    float height;
    unsigned char unknown038[0x00c];
};

typedef char AnmSpriteViewSizeIs44[
    (sizeof(AnmSpriteView) == 0x44) ? 1 : -1];

struct AnmLoadedView
{
    unsigned char unknown000[0x118];
    AnmSpriteView *sprites;
};

struct AsciiManagerStringView
{
    char text[64];
    AnmFloat3View position;
    unsigned int color;
    float scaleX;
    float scaleY;
    int unknown058;
    int viewportIndex;
    int smallFont;
    int drawShadow;
};

typedef char AsciiManagerStringViewSizeIs68[
    (sizeof(AsciiManagerStringView) == 0x68) ? 1 : -1];

// TH10 keeps the ASCII text queues, their render VM, and three ANM resources
// in one 0x89AC-byte polymorphic owner. The true ANM renderer is the separate
// object published at 0x00491C10 and consumed by DrawStrings/DrawGuiStrings.
struct AsciiManagerView
{
    AsciiManagerView();
    ~AsciiManagerView();
    virtual size_t GetSize();
    int Initialize();
    static int __fastcall OnUpdate(AsciiManagerView *manager);
    static int __fastcall OnDrawLowPriority(AsciiManagerView *manager);
    static int __fastcall OnDrawHighPriority(AsciiManagerView *manager);
    int ResetStrings();
    void AddString(AnmFloat3View *position, const char *text);
    void AddGuiString(AnmFloat3View *position, const char *text);
    void AddFormatText(AnmFloat3View *position, const char *format, ...);
    void AddSmallFormatText(AnmFloat3View *position, const char *format, ...);
    int AddGuiFormatText(AnmFloat3View *position, const char *format, ...);

    unsigned int flags004;
    int unknown008;
    void *calcChainElement;
    void *drawChainElement0;
    AnmVmView primaryVm014;
    AnmVmView secondaryVm3C0;
    AsciiManagerStringView strings[256];
    AsciiManagerStringView guiStrings[64];
    int numStrings;
    int numGuiStrings;
    unsigned int color;
    float scaleX;
    float scaleY;
    int viewportIndex;
    int unknown8984;
    int drawShadow;
    int spaceWidth;
    int frameCounter;
    AnmLoadedView *asciiAnm;
    AnmLoadedView *captureAnm;
    AnmLoadedView *textAnm;
    unsigned char unknown89A0[0x008];
    void *drawChainElement;
};

typedef char AsciiManagerViewSizeIs89AC[
    (sizeof(AsciiManagerView) == 0x89ac) ? 1 : -1];
typedef char AsciiManagerPrimaryVmAt014[
    (offsetof(AsciiManagerView, primaryVm014) == 0x014) ? 1 : -1];
typedef char AsciiManagerSecondaryVmAt3C0[
    (offsetof(AsciiManagerView, secondaryVm3C0) == 0x3c0) ? 1 : -1];
typedef char AsciiManagerStringsAt76C[
    (offsetof(AsciiManagerView, strings) == 0x76c) ? 1 : -1];
typedef char AsciiManagerGuiStringsAt6F6C[
    (offsetof(AsciiManagerView, guiStrings) == 0x6f6c) ? 1 : -1];
typedef char AsciiManagerCountsAt896C[
    (offsetof(AsciiManagerView, numStrings) == 0x896c &&
     offsetof(AsciiManagerView, numGuiStrings) == 0x8970) ? 1 : -1];
typedef char AsciiManagerColorAt8974[
    (offsetof(AsciiManagerView, color) == 0x8974) ? 1 : -1];
typedef char AsciiManagerAsciiAnmAt8994[
    (offsetof(AsciiManagerView, asciiAnm) == 0x8994) ? 1 : -1];

extern AsciiManagerView *g_AsciiManagerView;

AsciiManagerView *AsciiManagerCreate();

int __stdcall AsciiManagerDrawStrings(AsciiManagerView *manager);
int __stdcall AsciiManagerDrawGuiStrings(AsciiManagerView *manager);

typedef int (__fastcall *AnmChainCallback)(AsciiManagerView *manager);

struct AnmChainElementView
{
    void *unknown000;
    unsigned int flags;
    unsigned char unknown008[0x018];
    AsciiManagerView *argument;
};

AnmLoadedView * __fastcall AnmLoadResource(
    int slot, void *fileSystem, const char *path);
AnmChainElementView * __stdcall AnmCreateChainElement(AnmChainCallback callback);
void __fastcall AnmAddCalcChainElement(
    AnmChainElementView *element, int priority, void *chain);
void __fastcall AnmAddDrawChainElement(
    AnmChainElementView *element, int priority, void *chain);
void __fastcall AnmRemoveChainElement(AnmChainElementView *element, void *chain);
void __fastcall AnmLoadedSetScript(void *anm, AnmVmView *vm, int scriptIndex);
void __fastcall AnmReleaseResource(void *resource);

struct AnmRenderManagerView
{
    void FlushVertexBuffer();
    int DrawNoRotation(AnmVmView *vm);
    int DrawNoRotationNoRound(AnmVmView *vm);
};

extern AnmRenderManagerView *g_AnmRenderManagerView;
void __cdecl AsciiConfigureBackgroundViewport(int index);

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
