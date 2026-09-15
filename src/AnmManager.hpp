#pragma once

#include "D3d9View.hpp"
#include "Rng.hpp"

#include <stddef.h>

#if defined(_MSC_VER)
#define TH10_ANM_NOINLINE __declspec(noinline)
#else
#define TH10_ANM_NOINLINE
#endif

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

    AnmFloat3View operator+(const AnmFloat3View &other) const
    {
        return AnmFloat3View(
            x + other.x, y + other.y, z + other.z);
    }

    AnmFloat3View operator*(float scale) const
    {
        return AnmFloat3View(x * scale, y * scale, z * scale);
    }

    AnmFloat3View &operator+=(const AnmFloat3View &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    void FromAngleMagnitude(float angle, float magnitude);

    float x;
    float y;
    float z;
};

typedef char AnmFloat3ViewSizeIs0C[
    (sizeof(AnmFloat3View) == 0x0c) ? 1 : -1];

struct AnmFloat4View
{
    float x;
    float y;
    float z;
    float w;
};

typedef char AnmFloat4ViewSizeIs10[
    (sizeof(AnmFloat4View) == 0x10) ? 1 : -1];

struct AnmFloat2View
{
    AnmFloat2View operator-(const AnmFloat2View &other) const
    {
        AnmFloat2View result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    AnmFloat2View operator+(const AnmFloat2View &other) const
    {
        AnmFloat2View result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    AnmFloat2View operator*(float scale) const
    {
        AnmFloat2View result;
        result.x = x * scale;
        result.y = y * scale;
        return result;
    }

    AnmFloat2View &operator+=(const AnmFloat2View &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    float x;
    float y;
};

typedef char AnmFloat2ViewSizeIs08[
    (sizeof(AnmFloat2View) == 0x08) ? 1 : -1];

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

extern float g_AnmGameSpeed;

enum AnmOpcodeView
{
    ANM_OP_END = -1,
    ANM_OP_NOP = 0,
    ANM_OP_DELETE = 1,
    ANM_OP_STATIC = 2,
    ANM_OP_SPRITE = 3,
    ANM_OP_JUMP = 4,
    ANM_OP_JUMP_DEC = 5,
    ANM_OP_I_SET = 6,
    ANM_OP_F_SET = 7,
    ANM_OP_I_ADD = 8,
    ANM_OP_F_ADD = 9,
    ANM_OP_I_SUB = 10,
    ANM_OP_F_SUB = 11,
    ANM_OP_I_MUL = 12,
    ANM_OP_F_MUL = 13,
    ANM_OP_I_DIV = 14,
    ANM_OP_F_DIV = 15,
    ANM_OP_I_MOD = 16,
    ANM_OP_F_MOD = 17,
    ANM_OP_I_SET_ADD = 18,
    ANM_OP_F_SET_ADD = 19,
    ANM_OP_I_SET_SUB = 20,
    ANM_OP_F_SET_SUB = 21,
    ANM_OP_I_SET_MUL = 22,
    ANM_OP_F_SET_MUL = 23,
    ANM_OP_I_SET_DIV = 24,
    ANM_OP_F_SET_DIV = 25,
    ANM_OP_I_SET_MOD = 26,
    ANM_OP_F_SET_MOD = 27,
    ANM_OP_I_JUMP_EQ = 28,
    ANM_OP_F_JUMP_EQ = 29,
    ANM_OP_I_JUMP_NE = 30,
    ANM_OP_F_JUMP_NE = 31,
    ANM_OP_I_JUMP_LT = 32,
    ANM_OP_F_JUMP_LT = 33,
    ANM_OP_I_JUMP_LE = 34,
    ANM_OP_F_JUMP_LE = 35,
    ANM_OP_I_JUMP_GT = 36,
    ANM_OP_F_JUMP_GT = 37,
    ANM_OP_I_JUMP_GE = 38,
    ANM_OP_F_JUMP_GE = 39,
    ANM_OP_I_SET_RANDOM = 40,
    ANM_OP_F_SET_RANDOM = 41,
    ANM_OP_F_SIN = 42,
    ANM_OP_F_COS = 43,
    ANM_OP_F_TAN = 44,
    ANM_OP_F_ACOS = 45,
    ANM_OP_F_ATAN = 46,
    ANM_OP_NORMALIZE_ANGLE = 47,
    ANM_OP_POSITION = 48,
    ANM_OP_ROTATION = 49,
    ANM_OP_SCALE = 50,
    ANM_OP_ALPHA1 = 51,
    ANM_OP_COLOR1 = 52,
    ANM_OP_ANGULAR_VELOCITY = 53,
    ANM_OP_SCALE_GROWTH = 54,
    ANM_OP_ALPHA1_TIME_LINEAR = 55,
    ANM_OP_POSITION_TIME = 56,
    ANM_OP_COLOR1_TIME = 57,
    ANM_OP_ALPHA1_TIME = 58,
    ANM_OP_ROTATION_TIME = 59,
    ANM_OP_SCALE_TIME = 60,
    ANM_OP_FLIP_X = 61,
    ANM_OP_FLIP_Y = 62,
    ANM_OP_STOP = 63,
    ANM_OP_INTERRUPT_LABEL = 64,
    ANM_OP_RENDER_STATE = 65,
    ANM_OP_BLEND_MODE = 66,
    ANM_OP_RENDER_MODE = 67,
    ANM_OP_RENDER_LAYER = 68,
    ANM_OP_STOP_HIDE = 69,
    ANM_OP_U_SCROLL = 70,
    ANM_OP_V_SCROLL = 71,
    ANM_OP_VISIBLE = 72,
    ANM_OP_Z_WRITE_DISABLE = 73,
    ANM_OP_FLAG13 = 74,
    ANM_OP_WAIT = 75,
    ANM_OP_COLOR2 = 76,
    ANM_OP_ALPHA2 = 77,
    ANM_OP_COLOR2_TIME = 78,
    ANM_OP_ALPHA2_TIME = 79,
    ANM_OP_USE_SECONDARY_COLOR = 80,
    ANM_OP_RETURN = 81,
    ANM_OP_FLAG27 = 82,
    ANM_OP_COMMIT_POSITION = 83,
    ANM_OP_ALLOC_VERTICES = 84,
    ANM_OP_FLAG28 = 85,
    ANM_OP_UNIT_SPEED = 86,
    ANM_OP_ALTERNATE_RNG = 87,
    ANM_OP_CREATE_CHILD_88 = 88,
    ANM_OP_POINT_TEXTURE_FILTER = 89,
    ANM_OP_CREATE_CHILD_90 = 90,
    ANM_OP_CREATE_CHILD_91 = 91,
    ANM_OP_CREATE_CHILD_92 = 92
};

enum AnmInterpolationModeView
{
    ANM_INTERPOLATION_LINEAR = 0,
    ANM_INTERPOLATION_EASE_IN = 1,
    ANM_INTERPOLATION_EASE_IN_CUBIC = 2,
    ANM_INTERPOLATION_EASE_IN_QUARTIC = 3,
    ANM_INTERPOLATION_EASE_OUT = 4,
    ANM_INTERPOLATION_EASE_OUT_CUBIC = 5,
    ANM_INTERPOLATION_EASE_OUT_QUARTIC = 6,
    ANM_INTERPOLATION_ADD = 7,
    ANM_INTERPOLATION_HERMITE = 8,
    ANM_INTERPOLATION_EASE_IN_OUT = 9,
    ANM_INTERPOLATION_EASE_IN_OUT_CUBIC = 10,
    ANM_INTERPOLATION_EASE_IN_OUT_QUARTIC = 11,
    ANM_INTERPOLATION_EASE_OUT_IN = 12,
    ANM_INTERPOLATION_EASE_OUT_IN_CUBIC = 13,
    ANM_INTERPOLATION_EASE_OUT_IN_QUARTIC = 14,
    ANM_INTERPOLATION_CONSTANT_ZERO = 15,
    ANM_INTERPOLATION_CONSTANT_ONE = 16,
    ANM_INTERPOLATION_ACCELERATE = 17
};

float CalculateAnmInterpolation(int mode, float current, float duration);

struct AnmRawInstructionView
{
    short opcode;
    unsigned short size;
    short time;
    unsigned short variableMask;
    union
    {
        int intArgs[10];
        float floatArgs[10];
        unsigned short shortArgs[20];
        unsigned char byteArgs[40];
    };
};

typedef char AnmRawInstructionArgumentsAt08[
    (offsetof(AnmRawInstructionView, intArgs) == 0x08) ? 1 : -1];

struct AnmInt3View
{
    AnmInt3View() {}
    AnmInt3View(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    int x;
    int y;
    int z;
};

typedef char AnmInt3ViewSizeIs0C[
    (sizeof(AnmInt3View) == 0x0c) ? 1 : -1];

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
    void Add(float value);
    void Decrement(int value)
    {
        Add(static_cast<float>(-value));
    }
    int Tick()
    {
        previous = current;
        if (*scale <= 0.99f || *scale >= 1.01f)
        {
            subframe += *scale;
            current = static_cast<int>(subframe);
        }
        else
        {
            ++current;
            subframe += 1.0f;
        }
        return current;
    }
    void SetCurrent(int value)
    {
        if ((flags & 1u) == 0)
        {
            current = 0;
            previous = -999999;
            subframe = 0.0f;
            scale = &g_AnmGameSpeed;
            flags |= 1u;
        }

        current = value;
        subframe = static_cast<float>(value);
        previous = value - 1;
    }
};
typedef char AnmVmTimerViewSizeIs14[
    (sizeof(AnmVmTimerView) == 0x14) ? 1 : -1];

struct AnmVmFloat3InterpolationView
{
    AnmFloat3View initial;
    AnmFloat3View final;
    AnmFloat3View initialTangent;
    AnmFloat3View finalTangent;
    AnmVmTimerView timer;
    int duration;
    int mode;

    AnmFloat3View *Evaluate(AnmFloat3View *output);
};

typedef char AnmVmFloat3InterpolationViewSizeIs4C[
    (sizeof(AnmVmFloat3InterpolationView) == 0x4c) ? 1 : -1];

struct AnmVmColorInterpolationView
{
    AnmInt3View initial;
    AnmInt3View final;
    AnmInt3View initialTangent;
    AnmInt3View finalTangent;
    AnmVmTimerView timer;
    int duration;
    int mode;

    AnmInt3View *Evaluate(AnmInt3View *output);
};

typedef char AnmVmColorInterpolationViewSizeIs4C[
    (sizeof(AnmVmColorInterpolationView) == 0x4c) ? 1 : -1];

struct AnmVmAlphaInterpolationView
{
    int initial;
    int final;
    int initialTangent;
    int finalTangent;
    AnmVmTimerView timer;
    int duration;
    int mode;

    int Evaluate();
};

typedef char AnmVmAlphaInterpolationViewSizeIs2C[
    (sizeof(AnmVmAlphaInterpolationView) == 0x2c) ? 1 : -1];

struct AnmVmFloat2InterpolationView
{
    AnmFloat2View initial;
    AnmFloat2View final;
    AnmFloat2View initialTangent;
    AnmFloat2View finalTangent;
    AnmVmTimerView timer;
    int duration;
    int mode;

    AnmFloat2View *Evaluate(AnmFloat2View *output);
};

typedef char AnmVmFloat2InterpolationViewSizeIs3C[
    (sizeof(AnmVmFloat2InterpolationView) == 0x3c) ? 1 : -1];

struct AnmMatrixView
{
    float values[16];

    void SetIdentity();
};
typedef char AnmMatrixViewSizeIs40[
    (sizeof(AnmMatrixView) == 0x40) ? 1 : -1];

struct AnmSpriteView;
struct AnmVmView;
struct AnmLoadedView;

struct AnmVmIdView
{
    AnmVmIdView() { value = 0; }

    int operator==(AnmVmIdView other)
    {
        return value == other.value;
    }

    int operator++(int)
    {
        return value++;
    }

    AnmVmView *GetVm();

    int value;
};

typedef char AnmVmIdViewSizeIs04[
    (sizeof(AnmVmIdView) == 0x04) ? 1 : -1];

struct AnmVmLayerNodeView
{
    void InsertAfter(AnmVmLayerNodeView *node);

    void *owner;
    AnmVmLayerNodeView *next;
    AnmVmLayerNodeView *previous;
};

typedef char AnmVmLayerNodeViewSizeIs0C[
    (sizeof(AnmVmLayerNodeView) == 0x0c) ? 1 : -1];

typedef int (__fastcall *AnmVmCallback)(AnmVmView *vm);
int __fastcall UpdatePulsingRadialTrail(AnmVmView *vm);
int __fastcall DrawPulsingRadialTrail(AnmVmView *vm);

// Exact-size view for the TH10 ANM virtual machine. The lifecycle, script
// executor, interpolation and draw paths establish the named fields below;
// offsets that still lack a reviewed consumer remain opaque.
struct AnmVmView
{
    AnmVmView();
    ~AnmVmView();
    void Initialize();
    int InitializePulsingRadialTrail();
    float GetFloatVar(float variable);
    int GetIntVar(int variable);
    float *GetFloatVarPtr(
        float *value, unsigned short variableMask,
        unsigned int argumentNumber);
    int *GetIntVarPtr(
        int *value, unsigned short variableMask,
        unsigned int argumentNumber);
    void StartScaleInterpolation(
        const AnmFloat2View *initial, const AnmFloat2View *final,
        int duration, unsigned char mode);
    void StartSecondaryColorInterpolation(
        const AnmColorView *initial, const AnmColorView *final,
        int duration, unsigned char mode);
    void StartSecondaryAlphaInterpolation(
        int duration, unsigned char mode,
        unsigned char initial, unsigned char final);
    void StartPrimaryColorInterpolation(
        const AnmColorView *initial, const AnmColorView *final,
        int duration, unsigned char mode);
    void StartPrimaryAlphaInterpolation(
        int duration, unsigned char mode,
        unsigned char initial, unsigned char final);

    int id;
    AnmVmLayerNodeView managerNode;
    AnmVmLayerNodeView layerNode;
    AnmVmView *nextInDrawLayer;
    unsigned int renderLayer;
    AnmFloat3View rotation;
    AnmFloat3View angularVelocity;
    float scaleX;
    float scaleY;
    AnmFloat2View scaleGrowth;
    float spriteWidth;
    float spriteHeight;
    float uvScrollX;
    float uvScrollY;
    AnmVmTimerView scriptTimer;
    AnmVmFloat3InterpolationView positionInterpolation;
    AnmVmColorInterpolationView primaryColorInterpolation;
    AnmVmAlphaInterpolationView primaryAlphaInterpolation;
    AnmVmFloat3InterpolationView rotationInterpolation;
    AnmVmFloat2InterpolationView scaleInterpolation;
    AnmVmColorInterpolationView secondaryColorInterpolation;
    AnmVmAlphaInterpolationView secondaryAlphaInterpolation;
    float uvScrollVelocityX;
    float uvScrollVelocityY;
    AnmMatrixView matrix23C;
    AnmMatrixView matrix27C;
    AnmMatrixView textureMatrix2BC;
    AnmColorView primaryColor;
    AnmColorView secondaryColor;
    short pendingInterrupt;
    short unknown306;
    AnmLoadedView *anmFile;
    union
    {
        int intVar0;
        int generatedVertexCount;
    };
    int intVar1;
    int intVar2;
    int intVar3;
    float floatVar0;
    float floatVar1;
    float floatVar2;
    float floatVar3;
    int counterVar0;
    int counterVar1;
    AnmFloat3View position;
    AnmFloat3View positionOffset;
    AnmFloat3View alternatePosition;
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
            unsigned int unknownFlags06 : 2;
            unsigned int useAlternatePosition : 1;
            unsigned int flipX : 1;
            unsigned int flipY : 1;
            unsigned int zWriteDisabled : 1;
            unsigned int stopped : 1;
            unsigned int flag13 : 1;
            unsigned int useStaticMatrix : 1;
            unsigned int useSecondaryColor : 1;
            unsigned int unknownFlag16 : 1;
            unsigned int scriptDisabled : 1;
            unsigned int renderStateA : 2;
            unsigned int renderStateB : 2;
            unsigned int renderMode : 4;
            unsigned int unknownFlag26 : 1;
            unsigned int flag27 : 1;
            unsigned int flag28 : 1;
            unsigned int useUnitSpeed : 1;
            unsigned int useAlternateRng : 1;
            unsigned int usePointTextureFilter : 1;
        };
    };
    unsigned char unknown360[0x008];
    AnmVmTimerView interruptReturnTimer;
    AnmRawInstructionView *interruptReturnInstruction;
    int timeOfLastSpriteSet;
    short activeSpriteIndex;
    short anmFileIndex;
    short baseSpriteIndex;
    short scriptIndex;
    AnmRawInstructionView *beginningOfScript;
    AnmRawInstructionView *currentInstruction;
    AnmSpriteView *loadedSprite;
    AnmVmCallback positionCallback;
    AnmVmCallback drawCallback;
    unsigned char unknown3A0[0x00c];
};

typedef char AnmVmViewSizeIs3AC[
    (sizeof(AnmVmView) == 0x3ac) ? 1 : -1];
typedef char AnmVmListNodesAt004[
    (offsetof(AnmVmView, managerNode) == 0x004 &&
     offsetof(AnmVmView, layerNode) == 0x010 &&
     offsetof(AnmVmView, nextInDrawLayer) == 0x01c) ? 1 : -1];
typedef char AnmVmGeneratedVerticesAt358[
    (offsetof(AnmVmView, generatedVertices) == 0x358) ? 1 : -1];
typedef char AnmVmLastTimerFlagsAt378[
    (offsetof(AnmVmView, interruptReturnTimer.flags) == 0x378) ? 1 : -1];
typedef char AnmVmActiveSpriteAt384[
    (offsetof(AnmVmView, activeSpriteIndex) == 0x384) ? 1 : -1];
typedef char AnmVmPositionAt334[
    (offsetof(AnmVmView, position) == 0x334) ? 1 : -1];
typedef char AnmVmVariablesAt30C[
    (offsetof(AnmVmView, intVar0) == 0x30c &&
     offsetof(AnmVmView, floatVar0) == 0x31c &&
     offsetof(AnmVmView, counterVar0) == 0x32c) ? 1 : -1];
typedef char AnmVmRotationAt024[
    (offsetof(AnmVmView, rotation) == 0x024) ? 1 : -1];
typedef char AnmVmLoadedSpriteAt394[
    (offsetof(AnmVmView, loadedSprite) == 0x394) ? 1 : -1];
typedef char AnmVmCallbacksAt398[
    (offsetof(AnmVmView, positionCallback) == 0x398 &&
     offsetof(AnmVmView, drawCallback) == 0x39c) ? 1 : -1];
typedef char AnmVmColorsAt2FC[
    (offsetof(AnmVmView, primaryColor) == 0x2fc &&
     offsetof(AnmVmView, secondaryColor) == 0x300) ? 1 : -1];
typedef char AnmVmTextureMatrixAt2BC[
    (offsetof(AnmVmView, textureMatrix2BC) == 0x2bc) ? 1 : -1];
typedef char AnmVmFlagsAt35C[
    (offsetof(AnmVmView, flags35C) == 0x35c) ? 1 : -1];
typedef char AnmVmScriptStateAt304[
    (offsetof(AnmVmView, pendingInterrupt) == 0x304 &&
     offsetof(AnmVmView, anmFile) == 0x308 &&
     offsetof(AnmVmView, interruptReturnInstruction) == 0x37c &&
     offsetof(AnmVmView, timeOfLastSpriteSet) == 0x380 &&
     offsetof(AnmVmView, beginningOfScript) == 0x38c &&
     offsetof(AnmVmView, currentInstruction) == 0x390) ? 1 : -1];
typedef char AnmVmInterpolationLayoutAt070[
    (offsetof(AnmVmView, positionInterpolation) == 0x070 &&
     offsetof(AnmVmView, positionInterpolation.timer) == 0x0a0 &&
     offsetof(AnmVmView, primaryColorInterpolation) == 0x0bc &&
     offsetof(AnmVmView, primaryAlphaInterpolation) == 0x108 &&
     offsetof(AnmVmView, rotationInterpolation) == 0x134 &&
     offsetof(AnmVmView, scaleInterpolation) == 0x180 &&
     offsetof(AnmVmView, secondaryColorInterpolation) == 0x1bc &&
     offsetof(AnmVmView, secondaryAlphaInterpolation) == 0x208 &&
     offsetof(AnmVmView, uvScrollVelocityX) == 0x234) ? 1 : -1];

struct AnmSpriteView
{
    unsigned char unknown000[0x004];
    void *texture;
    unsigned char unknown008[0x010];
    float textureHeight;
    float textureWidth;
    float uStart;
    float vStart;
    float uEnd;
    float vEnd;
    float height;
    float width;
    float horizontalScale;
    float verticalScale;
    unsigned int unknown040;
};

typedef char AnmSpriteViewSizeIs44[
    (sizeof(AnmSpriteView) == 0x44) ? 1 : -1];
typedef char AnmSpriteUvAt20[
    (offsetof(AnmSpriteView, uStart) == 0x20 &&
     offsetof(AnmSpriteView, vEnd) == 0x2c) ? 1 : -1];

struct AnmLoadedView
{
    short anmFileIndex;
    unsigned char unknown002[0x106];
    void *rawData;
    unsigned char unknown10C[0x00c];
    AnmSpriteView *sprites;
    AnmRawInstructionView **scripts;
    void *unknown120;
    int pendingLoadCount;

    int SetSprite(AnmVmView *vm, int spriteIndex);
    void InitializeVm(AnmVmView *vm, int scriptIndex);
    void InitializeAndExecuteScriptIndex(AnmVmView *vm, int scriptIndex);
    void SetAndExecuteScriptIndex(AnmVmView *vm, int scriptIndex);
    void SetAndExecuteScriptIdx(AnmVmView *vm, int scriptIndex);
    AnmVmIdView CreateVmVariant0(int scriptIndex, unsigned int renderLayer);
    AnmVmIdView CreateVmAtScreenVariant0(
        int scriptIndex, const AnmFloat3View *position);
    AnmVmIdView CreateVmAtWorldVariant0(
        int scriptIndex, const AnmFloat3View *position);
    AnmVmIdView CreateVmVariant1(int scriptIndex, unsigned int renderLayer);
    AnmVmIdView CreateVmAtScreenVariant1(
        int scriptIndex, const AnmFloat3View *position);
    AnmVmIdView CreateVmAtWorldVariant1(
        int scriptIndex, const AnmFloat3View *position);
    AnmVmIdView CreateVmVariant2(int scriptIndex, unsigned int renderLayer);
    AnmVmIdView CreateVmAtScreenVariant2(
        int scriptIndex, const AnmFloat3View *position);
    AnmVmIdView CreateVmAtWorldVariant2(
        int scriptIndex, const AnmFloat3View *position);
    AnmVmIdView CreateVmVariant3(int scriptIndex, unsigned int renderLayer);
    AnmVmIdView CreateVmAtScreenVariant3(
        int scriptIndex, const AnmFloat3View *position);
    AnmVmIdView CreateVmAtWorldVariant3(
        int scriptIndex, const AnmFloat3View *position);
};

typedef char AnmLoadedSpritesAt118[
    (offsetof(AnmLoadedView, sprites) == 0x118 &&
     offsetof(AnmLoadedView, scripts) == 0x11c &&
     offsetof(AnmLoadedView, pendingLoadCount) == 0x124) ? 1 : -1];

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
    union
    {
        AnmColorView diffuse;
        unsigned int color;
    };
    union
    {
        AnmFloat2View uv;
        struct
        {
            float u;
            float v;
        };
    };
};

typedef char AnmRenderVertexViewSizeIs1C[
    (sizeof(AnmRenderVertexView) == 0x1c) ? 1 : -1];

struct PulsingRadialTrailDataView
{
    AnmRenderVertexView vertices[33];
    float radii[33];
    float radialVelocities[33];
    AnmFloat2View uvVelocity;
    unsigned int unknown4AC;
};

typedef char PulsingRadialTrailDataViewSizeIs4B0[
    (sizeof(PulsingRadialTrailDataView) == 0x4b0) ? 1 : -1];

struct AnmUntexturedVertexView
{
    float x;
    float y;
    float z;
    float w;
    unsigned int color;
};

typedef char AnmUntexturedVertexViewSizeIs14[
    (sizeof(AnmUntexturedVertexView) == 0x14) ? 1 : -1];

// The manager owns one allocation through a single-pointer member. Its
// destructor is ordered between the draw-layer VM array and the inline VM
// pool by the target's manager destructor.
struct AnmOwnedAllocationView
{
    ~AnmOwnedAllocationView();

    void *data;
};

typedef char AnmOwnedAllocationViewSizeIs04[
    (sizeof(AnmOwnedAllocationView) == 0x04) ? 1 : -1];

// Only the renderer fields established by the shared-buffer clear/flush seam
// are named. The 0x20000 packed vertices account exactly for the span between
// the target-observed buffer base and its end/start cursors.
struct AnmRenderManagerView
{
    unsigned char unknown000[0x04c];
    unsigned int scriptsStartedThisFrame;
    unsigned char unknown050[0x004];
    unsigned int renderStateChangesThisFrame;
    unsigned int flushesThisFrame;
    float screenShakeX;
    float screenShakeY;
    unsigned int vmsProcessedThisFrame;
    AnmVmView vmPool[0x1000];
    unsigned char vmPoolUsed[0x1000];
    int nextVmPoolIndex;
    unsigned char unknown3AD06C[0x084];
    AnmMatrixView cachedWorldMatrix;
    unsigned char unknown3AD130[0x358];
    AnmOwnedAllocationView ownedAllocation;
    unsigned char unknown3AD48C[0x5d4];
    unsigned int currentTextureFactor;
    void *currentTexture;
    unsigned char currentBlendMode;
    unsigned char currentColorOperation;
    unsigned char currentVertexShader;
    unsigned char currentZWrite;
    unsigned char unknown3ADA6C[0x002];
    unsigned char currentTextureFilter;
    unsigned char unknown3ADA6F;
    AnmSpriteView *currentSprite;
    void *quadVertexBuffer;
    AnmUntexturedVertexView untexturedVertices[4];
    unsigned int spritesToDraw;
    AnmRenderVertexView vertexBuffer[0x20000];
    AnmRenderVertexView *vertexBufferEnd;
    AnmRenderVertexView *vertexBufferStart;
    AnmVmLayerNodeView *primaryVmListHead;
    AnmVmLayerNodeView *primaryVmListTail;
    AnmVmLayerNodeView *secondaryVmListHead;
    AnmVmLayerNodeView *secondaryVmListTail;
    AnmVmView drawLayerSentinels[20];
    AnmVmIdView nextVmId;
    AnmColorView mixColor;
    int useMixColor;

    ~AnmRenderManagerView();
    static int __stdcall ExecuteScript(AnmVmView *vm);
    int UpdatePrimaryVms();
    int UpdateSecondaryVms();
    int DrawLayer(int layer);
    AnmVmView *AllocateVm();
    AnmVmIdView AddVmVariant0(AnmVmView *vm);
    AnmVmIdView AddVmVariant1(AnmVmView *vm);
    TH10_ANM_NOINLINE AnmVmIdView AddVmVariant2(AnmVmView *vm);
    TH10_ANM_NOINLINE AnmVmIdView AddVmVariant3(AnmVmView *vm);
    int RemoveVm(AnmVmView *vm);
    TH10_ANM_NOINLINE AnmVmView *FindVm(int id);
    void SetVmPendingInterrupt(int id, short interrupt);
    void SetVmPendingInterruptAndExecute(int id, short interrupt);
    void MarkVmForDeletion(int id);
    void SetVmPosition(int id, const AnmFloat3View *position);
    void SetVmWorldPosition(int id, const AnmFloat3View *position);
    AnmFloat3View *GetVmPosition(int id);
    void MarkLoadedVmsForDeletion(AnmLoadedView *loaded);
    void ClearVertexBuffer();
    void FlushVertexBuffer();
    int AddSpriteToDrawBuffer(AnmRenderVertexView *vertices);
    void SetRenderStateForVm3D(AnmVmView *vm);
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
    int DrawMode6(AnmVmView *vm);
    int Project3DQuad(AnmVmView *vm);
    int DrawProjected3DQuad(AnmVmView *vm);
    int DrawMode7(AnmVmView *vm);
    int Draw3D(AnmVmView *vm);
    int InitializeHorizontalTextureStrip(
        AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount);
    int InitializeVerticalTextureStrip(
        AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount);
    int SetGeneratedVertexColor(
        AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount);
    int DrawGeneratedVertices(
        AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount);
    int QueueSpriteQuad(
        AnmVmView *vm, AnmRenderVertexView *vertices);
    int DrawUntexturedTriangleStrip(
        AnmVmView *vm, AnmUntexturedVertexView *vertices, int vertexCount);
    int DrawUntexturedTriangleFan(
        AnmVmView *vm, AnmUntexturedVertexView *vertices, int vertexCount);
    int DrawTexturedTriangleFan(
        AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount);
    int Draw(AnmVmView *vm);
};

typedef char AnmRenderFlushCountAt058[
    (offsetof(AnmRenderManagerView, scriptsStartedThisFrame) == 0x04c &&
     offsetof(AnmRenderManagerView, flushesThisFrame) == 0x058 &&
     offsetof(AnmRenderManagerView, vmsProcessedThisFrame) == 0x064) ? 1 : -1];
typedef char AnmRenderVmPoolAt068[
    (offsetof(AnmRenderManagerView, vmPool) == 0x068 &&
     offsetof(AnmRenderManagerView, vmPoolUsed) == 0x3ac068 &&
     offsetof(AnmRenderManagerView, nextVmPoolIndex) == 0x3ad068) ? 1 : -1];
typedef char AnmRenderSpritesToDrawAt3ADAC8[
    (offsetof(AnmRenderManagerView, spritesToDraw) == 0x3adac8) ? 1 : -1];
typedef char AnmRenderVertexBufferAt3ADACC[
    (offsetof(AnmRenderManagerView, vertexBuffer) == 0x3adacc) ? 1 : -1];
typedef char AnmRenderVertexCursorsAt72DACC[
    (offsetof(AnmRenderManagerView, vertexBufferEnd) == 0x72dacc &&
     offsetof(AnmRenderManagerView, vertexBufferStart) == 0x72dad0) ? 1 : -1];
typedef char AnmRenderVmListsAt72DAD4[
    (offsetof(AnmRenderManagerView, primaryVmListHead) == 0x72dad4 &&
     offsetof(AnmRenderManagerView, secondaryVmListHead) == 0x72dadc &&
     offsetof(AnmRenderManagerView, drawLayerSentinels) == 0x72dae4 &&
     offsetof(AnmRenderManagerView, nextVmId) == 0x732454) ? 1 : -1];
typedef char AnmRenderStateCacheAt3ADA64[
    (offsetof(AnmRenderManagerView, currentTextureFactor) == 0x3ada60 &&
     offsetof(AnmRenderManagerView, currentTexture) == 0x3ada64 &&
     offsetof(AnmRenderManagerView, currentBlendMode) == 0x3ada68 &&
     offsetof(AnmRenderManagerView, currentColorOperation) == 0x3ada69 &&
     offsetof(AnmRenderManagerView, currentVertexShader) == 0x3ada6a &&
     offsetof(AnmRenderManagerView, currentZWrite) == 0x3ada6b &&
     offsetof(AnmRenderManagerView, currentTextureFilter) == 0x3ada6e) ? 1 : -1];
typedef char AnmRenderDirect3DStateAt3ADA70[
    (offsetof(AnmRenderManagerView, currentSprite) == 0x3ada70 &&
     offsetof(AnmRenderManagerView, quadVertexBuffer) == 0x3ada74) ? 1 : -1];
typedef char AnmRenderCachedWorldMatrixAt3AD0F0[
    (offsetof(AnmRenderManagerView, cachedWorldMatrix) == 0x3ad0f0) ? 1 : -1];
typedef char AnmRenderOwnedAllocationAt3AD488[
    (offsetof(AnmRenderManagerView, ownedAllocation) == 0x3ad488) ? 1 : -1];
typedef char AnmRenderUntexturedVerticesAt3ADA78[
    (offsetof(AnmRenderManagerView, untexturedVertices) == 0x3ada78) ? 1 : -1];
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

struct AnmPhotoBlendView
{
    float nearDistance;
    float farDistance;
    float blue;
    float green;
    float red;
    float unknown14;
    AnmColorView farColor;
};

typedef char AnmPhotoBlendViewSizeIs1C[
    (sizeof(AnmPhotoBlendView) == 0x1c) ? 1 : -1];
typedef char AnmPhotoBlendFarColorAt18[
    (offsetof(AnmPhotoBlendView, farColor) == 0x18) ? 1 : -1];

extern AnmRenderManagerView *g_AnmRenderManagerView;
extern D3d9DeviceView *g_Direct3DDevice;
extern AnmRenderVertexView g_AnmQuadVertices[4];
extern AnmViewportOwnerView *g_AnmViewportOwner;
extern AnmFloat3View g_AnmBackgroundCameraPosition;
extern AnmFloat3View g_AnmPosition491DA0;
extern AnmFloat3View g_AnmPositionOffsetDelta;
extern AnmPhotoBlendView g_AnmPhotoBlend;

float __stdcall AddNormalizeAngle(float angle, float delta);

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
extern "C" AnmFloat4View *__stdcall D3DXVec4Transform(
    AnmFloat4View *output, const AnmFloat4View *input,
    const AnmMatrixView *matrix);
extern "C" AnmMatrixView *__stdcall D3DXMatrixRotationX(
    AnmMatrixView *output, float angle);
extern "C" AnmMatrixView *__stdcall D3DXMatrixRotationY(
    AnmMatrixView *output, float angle);
extern "C" AnmMatrixView *__stdcall D3DXMatrixRotationZ(
    AnmMatrixView *output, float angle);
extern "C" AnmMatrixView *__stdcall D3DXMatrixMultiply(
    AnmMatrixView *output, const AnmMatrixView *left,
    const AnmMatrixView *right);

extern "C" void __stdcall EnterCriticalSection(void *criticalSection);
extern "C" void __stdcall LeaveCriticalSection(void *criticalSection);

#undef TH10_ANM_NOINLINE
