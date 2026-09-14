#pragma once

#include "D3d9View.hpp"

#include <stddef.h>

struct AnmFloat3View
{
    AnmFloat3View() {}
    AnmFloat3View(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    AnmFloat3View operator-(const AnmFloat3View &other) const
    {
        return AnmFloat3View(
            x - other.x, y - other.y, z - other.z);
    }

    float x;
    float y;
    float z;
};

typedef char AnmFloat3ViewSizeIs0C[
    (sizeof(AnmFloat3View) == 0x0c) ? 1 : -1];

union AnmColorView
{
    unsigned int value;
    struct
    {
        unsigned char blue;
        unsigned char green;
        unsigned char red;
        unsigned char alpha;
    };
};

typedef char AnmColorViewSizeIs04[
    (sizeof(AnmColorView) == 0x04) ? 1 : -1];

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

struct AnmSpriteView;

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
    AnmFloat3View rotation;
    unsigned char unknown030[0x00c];
    float scaleX;
    float scaleY;
    unsigned char unknown044[0x008];
    float spriteWidth;
    float spriteHeight;
    float uvScrollX;
    float uvScrollY;
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
    AnmColorView primaryColor;
    AnmColorView secondaryColor;
    unsigned char unknown304[0x004];
    void *anmFile308;
    unsigned char unknown30C[0x028];
    AnmFloat3View position;
    AnmFloat3View preservedPosition;
    AnmFloat3View spriteOffset;
    void *generatedVertices;
    union
    {
        unsigned int flags35C;
        struct
        {
            unsigned int visible : 1;
            unsigned int drawEnabled : 1;
            unsigned int updateRotation : 1;
            unsigned int updateScale : 1;
            unsigned int blendMode : 2;
            unsigned int unknownFlags06 : 9;
            unsigned int useSecondaryColor : 1;
            unsigned int unknownFlags16 : 2;
            unsigned int renderStateA : 2;
            unsigned int renderStateB : 2;
            unsigned int unknownFlags22 : 9;
            unsigned int usePointTextureFilter : 1;
        };
    };
    unsigned char unknown360[0x008];
    AnmVmTimerView timer368;
    unsigned char unknown37C[0x008];
    short activeSpriteIndex;
    unsigned char unknown386[0x00e];
    AnmSpriteView *loadedSprite;
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
typedef char AnmVmRotationAt024[
    (offsetof(AnmVmView, rotation) == 0x024) ? 1 : -1];
typedef char AnmVmLoadedSpriteAt394[
    (offsetof(AnmVmView, loadedSprite) == 0x394) ? 1 : -1];
typedef char AnmVmColorsAt2FC[
    (offsetof(AnmVmView, primaryColor) == 0x2fc &&
     offsetof(AnmVmView, secondaryColor) == 0x300) ? 1 : -1];
typedef char AnmVmFlagsAt35C[
    (offsetof(AnmVmView, flags35C) == 0x35c) ? 1 : -1];

struct AnmSpriteView
{
    unsigned char unknown000[0x004];
    void *texture;
    unsigned char unknown008[0x018];
    float uStart;
    float vStart;
    float uEnd;
    float vEnd;
    float width;
    float height;
    unsigned char unknown038[0x00c];
};

typedef char AnmSpriteViewSizeIs44[
    (sizeof(AnmSpriteView) == 0x44) ? 1 : -1];
typedef char AnmSpriteUvAt20[
    (offsetof(AnmSpriteView, uStart) == 0x20 &&
     offsetof(AnmSpriteView, vEnd) == 0x2c) ? 1 : -1];

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

struct AnmRenderVertexView
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned int color;
    float u;
    float v;
};

typedef char AnmRenderVertexViewSizeIs1C[
    (sizeof(AnmRenderVertexView) == 0x1c) ? 1 : -1];

// Only the renderer fields established by the shared-buffer clear/flush seam
// are named. The 0x20000 packed vertices account exactly for the span between
// the target-observed buffer base and its end/start cursors.
struct AnmRenderManagerView
{
    unsigned char unknown000[0x054];
    unsigned int renderStateChangesThisFrame;
    unsigned int flushesThisFrame;
    float screenShakeX;
    float screenShakeY;
    unsigned char unknown064[0x3ada00];
    void *currentTexture;
    unsigned char currentBlendMode;
    unsigned char unknown3ADA69;
    unsigned char currentVertexShader;
    unsigned char unknown3ADA6B[0x003];
    unsigned char currentTextureFilter;
    unsigned char unknown3ADA6F[0x059];
    unsigned int spritesToDraw;
    AnmRenderVertexView vertexBuffer[0x20000];
    AnmRenderVertexView *vertexBufferEnd;
    AnmRenderVertexView *vertexBufferStart;
    unsigned char unknown72DAD4[0x4984];
    AnmColorView mixColor;
    int useMixColor;

    void ClearVertexBuffer();
    void FlushVertexBuffer();
    int AddSpriteToDrawBuffer(AnmRenderVertexView *vertices);
    void SetRenderStateForVm(AnmVmView *vm);
    int DrawInner(AnmVmView *vm, int roundToPixel);
    int DrawNoRotation(AnmVmView *vm);
    int DrawNoRotationNoRound(AnmVmView *vm);
    int Draw2D(AnmVmView *vm);
    int Draw2DRotatedOrAxisAligned(AnmVmView *vm);
    void TranslateRotation(
        AnmRenderVertexView *vertex, float x, float y, float sine,
        float cosine, float xOffset, float yOffset);
    int ProjectCameraFacingQuad(AnmVmView *vm);
    int DrawCameraFacingQuad(AnmVmView *vm);
};

typedef char AnmRenderFlushCountAt058[
    (offsetof(AnmRenderManagerView, flushesThisFrame) == 0x058) ? 1 : -1];
typedef char AnmRenderSpritesToDrawAt3ADAC8[
    (offsetof(AnmRenderManagerView, spritesToDraw) == 0x3adac8) ? 1 : -1];
typedef char AnmRenderVertexBufferAt3ADACC[
    (offsetof(AnmRenderManagerView, vertexBuffer) == 0x3adacc) ? 1 : -1];
typedef char AnmRenderVertexCursorsAt72DACC[
    (offsetof(AnmRenderManagerView, vertexBufferEnd) == 0x72dacc &&
     offsetof(AnmRenderManagerView, vertexBufferStart) == 0x72dad0) ? 1 : -1];
typedef char AnmRenderStateCacheAt3ADA64[
    (offsetof(AnmRenderManagerView, currentTexture) == 0x3ada64 &&
     offsetof(AnmRenderManagerView, currentBlendMode) == 0x3ada68 &&
     offsetof(AnmRenderManagerView, currentVertexShader) == 0x3ada6a &&
     offsetof(AnmRenderManagerView, currentTextureFilter) == 0x3ada6e) ? 1 : -1];
typedef char AnmRenderMixColorAt732458[
    (offsetof(AnmRenderManagerView, mixColor) == 0x732458 &&
     offsetof(AnmRenderManagerView, useMixColor) == 0x73245c) ? 1 : -1];

struct AnmViewportView
{
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    float minZ;
    float maxZ;
};

struct AnmViewportOwnerView
{
    unsigned char unknown000[0x030];
    AnmFloat3View cameraRight;
    unsigned char unknown03C[0x010];
    AnmMatrixView viewMatrix;
    AnmMatrixView projectionMatrix;
    AnmViewportView viewport;
};

typedef char AnmViewportAt0CC[
    (offsetof(AnmViewportOwnerView, viewport) == 0x0cc) ? 1 : -1];
typedef char AnmViewportCameraRightAt030[
    (offsetof(AnmViewportOwnerView, cameraRight) == 0x030) ? 1 : -1];
typedef char AnmViewportMatricesAt04C[
    (offsetof(AnmViewportOwnerView, viewMatrix) == 0x04c &&
     offsetof(AnmViewportOwnerView, projectionMatrix) == 0x08c) ? 1 : -1];

extern AnmRenderManagerView *g_AnmRenderManagerView;
extern D3d9DeviceView *g_Direct3DDevice;
extern AnmRenderVertexView g_AnmQuadVertices[4];
extern AnmViewportOwnerView *g_AnmViewportOwner;
unsigned char __fastcall MixAnmColor(
    unsigned char source, unsigned char multiplier);
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

extern "C" AnmFloat3View *__stdcall D3DXVec3Project(
    AnmFloat3View *output, const AnmFloat3View *input,
    const AnmViewportView *viewport, const AnmMatrixView *projection,
    const AnmMatrixView *view, const AnmMatrixView *world);

extern "C" void __stdcall EnterCriticalSection(void *criticalSection);
extern "C" void __stdcall LeaveCriticalSection(void *criticalSection);
