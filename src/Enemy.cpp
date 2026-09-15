#include "Enemy.hpp"

#include <math.h>
#include <new>
#include <stdlib.h>
#include <string.h>

struct EnemyManagedVmView;
struct EnemyManagedVmListNodeView
{
    EnemyManagedVmView *vm;
    EnemyManagedVmListNodeView *next;
};

struct EnemyManagedVmView
{
    unsigned int id;
    unsigned char unknown004[0x10];
    EnemyManagedVmListNodeView *children;
    int childState;
    unsigned char unknown01C[0x2e4];
    unsigned int color;
    unsigned char unknown304[0x58];
    unsigned int flags;
    unsigned char unknown360[0x2a];
    short value38A;
};
typedef char EnemyManagedVmValue38AAt38A[
    (offsetof(EnemyManagedVmView, value38A) == 0x38a) ? 1 : -1];
typedef char EnemyManagedVmChildrenAt014[
    (offsetof(EnemyManagedVmView, children) == 0x14) ? 1 : -1];
typedef char EnemyManagedVmColorAt300[
    (offsetof(EnemyManagedVmView, color) == 0x300) ? 1 : -1];
typedef char EnemyManagedVmFlagsAt35C[
    (offsetof(EnemyManagedVmView, flags) == 0x35c) ? 1 : -1];

struct EnemyManagedVmRegistryView
{
    unsigned char unknown000[0x72dad4];
    EnemyManagedVmListNodeView *primaryList;
    unsigned char unknown72DAD8[0x04];
    EnemyManagedVmListNodeView *secondaryList;
};
typedef char EnemyManagedVmRegistryPrimaryAt72DAD4[
    (offsetof(EnemyManagedVmRegistryView, primaryList) == 0x72dad4) ? 1 : -1];
typedef char EnemyManagedVmRegistrySecondaryAt72DADC[
    (offsetof(EnemyManagedVmRegistryView, secondaryList) == 0x72dadc) ? 1 : -1];


struct EnemyRandomView
{
    unsigned int GetU32();
    float GetUnitFloat();
    float GetSignedUnitFloat();
};

// TH10 uses negative ECL operand selectors in the reviewed 0x00411FC0 /
// 0x00412350 readers. Names below describe only target-observed behavior;
// duplicate A/B selectors deliberately remain distinct because their original
// protocol meanings are not established by TH10 yet.
enum EnemyEclOperandSelector
{
    ENEMY_ECL_OPERAND_RANDOM_U32 = -10000,
    ENEMY_ECL_OPERAND_RANDOM_UNIT = -9999,
    ENEMY_ECL_OPERAND_RANDOM_ANGLE = -9998,
    ENEMY_ECL_OPERAND_WORLD_X_A = -9997,
    ENEMY_ECL_OPERAND_POSITION_Y_A = -9996,
    ENEMY_ECL_OPERAND_OFFSET_X_A = -9995,
    ENEMY_ECL_OPERAND_OFFSET_Y_A = -9994,
    ENEMY_ECL_OPERAND_BASE_X_A = -9993,
    ENEMY_ECL_OPERAND_BASE_Y_A = -9992,
    ENEMY_ECL_OPERAND_PLAYER_X_A = -9991,
    ENEMY_ECL_OPERAND_PLAYER_Y_A = -9990,
    ENEMY_ECL_OPERAND_ANGLE_TO_PLAYER_WORLD = -9989,
    ENEMY_ECL_OPERAND_UPDATE_TIMER = -9988,
    ENEMY_ECL_OPERAND_RANDOM_SIGNED = -9987,
    ENEMY_ECL_OPERAND_FLAG_10000 = -9986,
    ENEMY_ECL_OPERAND_INT_0 = -9985,
    ENEMY_ECL_OPERAND_INT_1 = -9984,
    ENEMY_ECL_OPERAND_INT_2 = -9983,
    ENEMY_ECL_OPERAND_INT_3 = -9982,
    ENEMY_ECL_OPERAND_FLOAT_0 = -9981,
    ENEMY_ECL_OPERAND_FLOAT_1 = -9980,
    ENEMY_ECL_OPERAND_FLOAT_2 = -9979,
    ENEMY_ECL_OPERAND_FLOAT_3 = -9978,
    ENEMY_ECL_OPERAND_WORLD_X_B = -9977,
    ENEMY_ECL_OPERAND_POSITION_Y_B = -9976,
    ENEMY_ECL_OPERAND_OFFSET_X_B = -9975,
    ENEMY_ECL_OPERAND_OFFSET_Y_B = -9974,
    ENEMY_ECL_OPERAND_BASE_X_B = -9973,
    ENEMY_ECL_OPERAND_BASE_Y_B = -9972,
    ENEMY_ECL_OPERAND_OFFSET_VALUE1C = -9971,
    ENEMY_ECL_OPERAND_BASE_VALUE1C = -9970,
    ENEMY_ECL_OPERAND_OFFSET_VALUE18 = -9969,
    ENEMY_ECL_OPERAND_BASE_VALUE18 = -9968,
    ENEMY_ECL_OPERAND_OFFSET_VALUE20 = -9967,
    ENEMY_ECL_OPERAND_BASE_VALUE20 = -9966,
    ENEMY_ECL_OPERAND_PLAYER_X_B = -9965,
    ENEMY_ECL_OPERAND_PLAYER_Y_B = -9964,
    ENEMY_ECL_OPERAND_SPECIAL_ENEMY_X = -9963,
    ENEMY_ECL_OPERAND_SPECIAL_ENEMY_Y = -9962,
    ENEMY_ECL_OPERAND_PRIMARY_VM_VALUE38A = -9961,
    ENEMY_ECL_OPERAND_RANK = -9960,
    ENEMY_ECL_OPERAND_DIFFICULTY = -9959,
    ENEMY_ECL_OPERAND_WORLD_VELOCITY_ANGLE = -9958,
    ENEMY_ECL_OPERAND_ONE = -9957,
    ENEMY_ECL_OPERAND_ANGLE_TO_PLAYER_OFFSET = -9956,
    ENEMY_ECL_OPERAND_ANGLE_TO_PLAYER_BASE = -9955,
    ENEMY_ECL_OPERAND_LIFE = -9954,
    ENEMY_ECL_OPERAND_DIFFICULTY_IS_0 = -9953,
    ENEMY_ECL_OPERAND_DIFFICULTY_IS_1 = -9952,
    ENEMY_ECL_OPERAND_DIFFICULTY_IS_2 = -9951,
    ENEMY_ECL_OPERAND_DIFFICULTY_IS_3 = -9950
};

typedef int (__fastcall *EnemyChainCallback)(EnemyManagerView *manager);

// Shared 0x24-byte chain-node shape established independently by the Player
// and Enemy registration sites. Only fields consumed by this packet are named.
struct EnemyCallbackNodeView
{
    int priority;
    unsigned int flags;
    EnemyChainCallback callback;
    void *unknown00C;
    void *unknown010;
    EnemyCallbackNodeView *self14;
    void *unknown018;
    void *unknown01C;
    EnemyManagerView *owner;
};
typedef char EnemyCallbackNodeViewSizeIs24[
    (sizeof(EnemyCallbackNodeView) == 0x24) ? 1 : -1];
typedef char EnemyCallbackNodeViewCallbackAt08[
    (offsetof(EnemyCallbackNodeView, callback) == 0x08) ? 1 : -1];
typedef char EnemyCallbackNodeViewSelfAt14[
    (offsetof(EnemyCallbackNodeView, self14) == 0x14) ? 1 : -1];
typedef char EnemyCallbackNodeViewOwnerAt20[
    (offsetof(EnemyCallbackNodeView, owner) == 0x20) ? 1 : -1];

struct EnemyConstructorDwordView
{
    unsigned int value;
    EnemyConstructorDwordView() : value(0) {}
};
typedef char EnemyConstructorDwordViewSizeIs04[
    (sizeof(EnemyConstructorDwordView) == 0x04) ? 1 : -1];

// Target allocation at 0x40D28D is exactly 0x1098 bytes. The resource has a
// two-slot base vtable and a three-slot derived vtable: slot 0 is inherited,
// slot 1 is overridden by target 0x40D400, and slot 2 is added at 0x40CD20.
// The names below describe behavior only; original type identifiers are unknown.
struct EnemyEclResourceBaseView
{
    virtual int AddScriptData(void *scriptData);
    virtual int LoadPackage(const unsigned char *packageData);

    void *GetScriptData(int index);

    int loadedScriptCount;
    int lookupCount;
    void *scriptData[32];
    void *lookupTable;
    unsigned char unknown090[0x1000];
    EnemyConstructorDwordView tailState1090;
    EnemyConstructorDwordView tailState1094;

    EnemyEclResourceBaseView()
    {
        memset(this, 0, sizeof(*this));
    }

    ~EnemyEclResourceBaseView()
    {
        if (lookupTable != NULL)
        {
            free(lookupTable);
            lookupTable = NULL;
        }
    }
};

typedef char EnemyEclResourceBaseViewSizeIs1098[
    (sizeof(EnemyEclResourceBaseView) == 0x1098) ? 1 : -1];
typedef char EnemyEclResourceBaseScriptDataAt00C[
    (offsetof(EnemyEclResourceBaseView, scriptData) == 0x0c) ? 1 : -1];
typedef char EnemyEclResourceBaseLookupTableAt08C[
    (offsetof(EnemyEclResourceBaseView, lookupTable) == 0x8c) ? 1 : -1];
typedef char EnemyEclResourceBaseTailState1090[
    (offsetof(EnemyEclResourceBaseView, tailState1090) == 0x1090) ? 1 : -1];
typedef char EnemyEclResourceBaseTailState1094[
    (offsetof(EnemyEclResourceBaseView, tailState1094) == 0x1094) ? 1 : -1];

struct EnemyEclResourceView : EnemyEclResourceBaseView
{
    EnemyEclResourceView() {}
    ~EnemyEclResourceView();

    virtual int LoadPackage(const unsigned char *packageData);
    virtual int LoadFile(const char *filename);
};
typedef char EnemyEclResourceViewSizeIs1098[
    (sizeof(EnemyEclResourceView) == 0x1098) ? 1 : -1];

struct EnemyAnimationOwnerView
{
    unsigned char unknown000000[0x3ad090];
    void *resourceSlots[4];
};
typedef char EnemyAnimationOwnerResourceSlotsAt3AD090[
    (offsetof(EnemyAnimationOwnerView, resourceSlots) == 0x3ad090) ? 1 : -1];

struct EnemyPrimaryResourceOwnerView
{
    unsigned char unknown000000[0x3e0b50];
    void *primaryEnemyResource;
};
typedef char EnemyPrimaryResourceAt3E0B50[
    (offsetof(EnemyPrimaryResourceOwnerView, primaryEnemyResource) == 0x3e0b50) ? 1 : -1];

struct EnemyCriticalSectionView
{
    unsigned char storage[0x18];
};

extern PlayerFloat3 g_EnemyGlobalPositionOffset;
extern float g_EnemyPlayfieldMinX;
extern float g_EnemyPlayfieldMaxX;
extern float g_EnemyPlayfieldMinY;
extern float g_EnemyPlayfieldMaxY;
extern float g_EnemyDirectionNegativeThreshold;
extern float g_EnemyDirectionPositiveThreshold;
extern int g_EnemyDamageProcessedFlag;
extern float g_PlayerTimerScale;
extern int g_EnemySpawnLayerIndex;
extern EnemyManagerView *g_EnemyManager;
extern EnemyManagedVmRegistryView *g_EnemyManagedVmRegistry;

extern void *g_EnemyCallbackManager;
extern EnemyCriticalSectionView g_EnemyCallbackCriticalSection;
extern unsigned char g_EnemyCallbackLockDepth;
extern unsigned char g_EnemyLifecycleFlags;
extern EnemyAnimationOwnerView *g_EnemyAnimationOwner;
extern EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;
extern char g_EnemyEclFilenameBuffer[];
extern EnemyRandomView g_EnemyRandom;
extern int g_EnemyRank;
extern int g_EnemyDifficulty;

struct EnemyAnimationModeView
{
    unsigned char unknown000[0x28];
    int value28;
};
struct EnemyGameStateView
{
    unsigned char unknown000[0x378c];
    unsigned int flags378C;
};
struct EnemyVisualStateView
{
    unsigned char unknown000[0x9da4];
    unsigned int flags9DA4;
};
extern EnemyAnimationModeView *g_EnemyAnimationMode;
extern EnemyGameStateView *g_EnemyGameState;
extern EnemyVisualStateView *g_EnemyVisualState;

// Descriptive interfaces for target-observed callees. Several target machine
// boundaries use EAX/ESI/EDI live-ins; these declarations deliberately express
// logical source values instead of claiming those private conventions.
void EnemyEvaluatePositionInterpolation(
    EnemyPositionInterpolationView *interpolation, PlayerFloat3 *out);
void EnemyEvaluateScalarInterpolation(
    EnemyScalarInterpolationView *interpolation, EnemyFloat2 *out);
float EnemyWrapAngle(float angle);
void EnemySetVectorFromPolar(PlayerFloat3 *out, float angle, float magnitude);
void EnemyAdvanceMotion(EnemyMotionView *motion);
void EnemySetAnimationScript(EnemyRuntimeView *enemy, int script);
int EnemyRunEcl(EnemyFullObjectView *owner, float scale);
const unsigned char *__fastcall EnemyResolveRuntimeCallback(
    EnemyFullObjectView *owner);
void EnemyRunCallbackEcl(
    EnemyFullObjectView *owner, const unsigned char *callbackName);
void EnemyInstallCallbackEcl(
    EnemyFullObjectView *owner, const unsigned char *callbackName);
void EnemyAddScoreReward(int value);
void EnemyCheckPlayerCollision(
    const PlayerFloat3 *position, const EnemyFloat2 *size, Player *player);
void EnemySetManagedVmPositionWithOffset(
    unsigned int vmId, const PlayerFloat3 *position);
void EnemySetManagedVmPositionExact(
    unsigned int vmId, const PlayerFloat3 *position);
EnemyManagedVmView *EnemyResolveManagedVm(unsigned int vmId);
EnemyManagedVmView *EnemyResolveManagedVmId(unsigned int *vmId);
float EnemyAngleFromPlayer(Player *player, const PlayerFloat3 *position);
void EnemySpawnDamageEffect(int kind, float positionX);
void EnemyAdvanceTimer(PlayerTimerView *timer, float amount);
void EnemyPrepareRuntimeStorage(EnemyRuntimeView *runtime);
EnemyEclInstructionView *EnemyLookupEclSubroutine(
    EnemyEclResourceView *scriptDatabase, const char *name);
void EnemyPlayDeathSound(int soundId, float positionX);
void EnemySpawnDeathEffect(
    const PlayerFloat3 *position, void *resource, int scriptId);
void EnemySpawnItem(
    const PlayerFloat3 *position, int itemType, int owner, float angle, float speed);
void EnemyDropItemCounts(
    const PlayerFloat3 *position, int *itemDropBlock);
void EnemyPlaySound(int soundId);

extern "C" void __stdcall EnterCriticalSection(EnemyCriticalSectionView *section);
extern "C" void __stdcall LeaveCriticalSection(EnemyCriticalSectionView *section);
void __fastcall EnemyUnlinkCallbackNode(
    EnemyCallbackNodeView *node, void *manager);
void EnemyRegisterUpdateCallbackNode(
    EnemyCallbackNodeView *node, int priority, void *manager);
void EnemyRegisterDrawCallbackNode(
    EnemyCallbackNodeView *node, int priority, void *manager);
void * __fastcall EnemyLoadAnimationResource(
    int resourceIndex, EnemyAnimationOwnerView *owner, const char *filename);
void EnemyDestroyAnimationResource(
    EnemyAnimationOwnerView *owner, void *resource);
void EnemyReportResourceLoadError();
// Target 0x0044B360 receives filename in EAX plus stack sizeOut/mode; this
// logical declaration preserves all three source values without claiming that
// private register assignment.
void *EnemyLoadFileBytes(
    const char *filename, unsigned int *sizeOut, int mode);
void EnemyManagerClear(EnemyManagerView *manager);

// Target 0x0040C800-0x0040C80E is the retained constructor for the embedded
// 0x1008-byte operand-storage state. The two cursors are the only fields that
// this constructor initializes; the containing ECL host owns the rest.
EnemyEclContextView::EnemyEclContextView()
    : operandStackOffset(0), localStorageOffset(0)
{
}

// Target 0x0040C6E0-0x0040C70C releases the allocation payload and list node
// for every temporary block owned by one ECL host. ResetEclState deliberately
// clears the head separately, matching the target's reusable release/reset
// pair at both callback-subroutine transition sites.
void EnemyEclHostBaseView::ReleaseEclAllocations()
{
    EnemyOwnedAllocationNodeView *node = ownedAllocations;
    while (node != NULL)
    {
        EnemyOwnedAllocationNodeView *next = node->next;
        free(node->allocation);
        free(node);
        node = next;
    }
}

// Target 0x0040C730-0x0040C776 restores the embedded context as the active
// context without touching its 0x1000-byte operand-storage payload.
void EnemyEclHostBaseView::ResetEclState()
{
    flags1028 &= ~1u;
    embeddedEclContext.value00 = 0;
    embeddedEclContext.currentInstruction = NULL;
    embeddedEclContext.operandResolver = this;
    embeddedEclContext.unknown1010 = -1;
    value1020 = 0;
    activeEclContext = &embeddedEclContext;
    eclContextMirror = &embeddedEclContext;
    ownedAllocations = NULL;
    value1038 = 0;
}

// The base virtual table at 0x0046D0D8 supplies neutral defaults for hosts that
// do not provide Enemy-specific operand access. The empty constructor delegates
// its only target-visible work to the embedded context constructor.
EnemyEclHostBaseView::EnemyEclHostBaseView()
{
}

int EnemyEclHostBaseView::DispatchEclInstruction()
{
    return 0;
}

int EnemyEclHostBaseView::ReadIntOperand(int operand)
{
    (void)operand;
    return 0;
}

int *EnemyEclHostBaseView::ResolveIntOperand(int operand)
{
    (void)operand;
    return NULL;
}

float EnemyEclHostBaseView::ReadFloatOperand(int operand)
{
    (void)operand;
    return 0.0f;
}

float *EnemyEclHostBaseView::ResolveFloatOperand(int operand)
{
    (void)operand;
    return NULL;
}

EnemyEclHostBaseView::~EnemyEclHostBaseView()
{
    ReleaseEclAllocations();
}

// Base ECL resources accept script data through slot zero but do not know how
// to interpret a package. The derived Enemy resource overrides this slot.
int EnemyEclResourceBaseView::LoadPackage(const unsigned char *packageData)
{
    (void)packageData;
    return 0;
}

// Target 0x0040C820-0x0040C824 indexes the target-proven 32-entry script table.
void *EnemyEclResourceBaseView::GetScriptData(int index)
{
    return scriptData[index];
}

// Target 0x0040E760-0x0040E76A is the primary Enemy vtable slot-zero entry.
// VC7.1 naturally lowers this ordinary member forwarding expression to
// ADD ECX,0x103C / JMP EnemyRuntimeView::DispatchEclInstruction. The runtime
// body at 0x0040E770 remains independently unreconstructed.
int EnemyFullObjectView::DispatchEclInstruction()
{
    return runtime.DispatchEclInstruction();
}

int EnemyFullObjectView::ReadIntOperand(int operand)
{
    switch (operand)
    {
    case ENEMY_ECL_OPERAND_RANDOM_U32:
        return (int)g_EnemyRandom.GetU32();
    case ENEMY_ECL_OPERAND_RANDOM_UNIT:
        return (int)g_EnemyRandom.GetUnitFloat();
    case ENEMY_ECL_OPERAND_WORLD_X_A:
    case ENEMY_ECL_OPERAND_WORLD_X_B:
        return (int)runtime.worldMotion.position.x;
    case ENEMY_ECL_OPERAND_POSITION_Y_A:
    case ENEMY_ECL_OPERAND_POSITION_Y_B:
        return (int)runtime.worldMotion.position.y;
    case ENEMY_ECL_OPERAND_OFFSET_X_A:
    case ENEMY_ECL_OPERAND_OFFSET_X_B:
        return (int)runtime.offsetMotion.position.x;
    case ENEMY_ECL_OPERAND_OFFSET_Y_A:
    case ENEMY_ECL_OPERAND_OFFSET_Y_B:
        return (int)runtime.offsetMotion.position.y;
    case ENEMY_ECL_OPERAND_BASE_X_A:
    case ENEMY_ECL_OPERAND_BASE_X_B:
        return (int)runtime.baseMotion.position.x;
    case ENEMY_ECL_OPERAND_BASE_Y_A:
    case ENEMY_ECL_OPERAND_BASE_Y_B:
        return (int)runtime.baseMotion.position.y;
    case ENEMY_ECL_OPERAND_PLAYER_X_A:
    case ENEMY_ECL_OPERAND_PLAYER_X_B:
        return (int)g_Player->drawPosition.x;
    case ENEMY_ECL_OPERAND_PLAYER_Y_A:
    case ENEMY_ECL_OPERAND_PLAYER_Y_B:
        return (int)g_Player->drawPosition.y;
    case ENEMY_ECL_OPERAND_UPDATE_TIMER:
        return runtime.updateTimer.current;
    case ENEMY_ECL_OPERAND_RANDOM_SIGNED:
        return (int)g_EnemyRandom.GetSignedUnitFloat();
    case ENEMY_ECL_OPERAND_FLAG_10000:
        return (runtime.flags >> 16) & 1;
    case ENEMY_ECL_OPERAND_INT_0:
        return runtime.eclVariables.integers[0];
    case ENEMY_ECL_OPERAND_INT_1:
        return runtime.eclVariables.integers[1];
    case ENEMY_ECL_OPERAND_INT_2:
        return runtime.eclVariables.integers[2];
    case ENEMY_ECL_OPERAND_INT_3:
        return runtime.eclVariables.integers[3];
    case ENEMY_ECL_OPERAND_FLOAT_0:
        return (int)runtime.eclVariables.floats[0];
    case ENEMY_ECL_OPERAND_FLOAT_1:
        return (int)runtime.eclVariables.floats[1];
    case ENEMY_ECL_OPERAND_FLOAT_2:
        return (int)runtime.eclVariables.floats[2];
    case ENEMY_ECL_OPERAND_FLOAT_3:
        return (int)runtime.eclVariables.floats[3];
    case ENEMY_ECL_OPERAND_OFFSET_VALUE1C:
        return (int)runtime.offsetMotion.value1C;
    case ENEMY_ECL_OPERAND_BASE_VALUE1C:
        return (int)runtime.baseMotion.value1C;
    case ENEMY_ECL_OPERAND_OFFSET_VALUE18:
        return (int)runtime.offsetMotion.value18;
    case ENEMY_ECL_OPERAND_BASE_VALUE18:
        return (int)runtime.baseMotion.value18;
    case ENEMY_ECL_OPERAND_OFFSET_VALUE20:
        return (int)runtime.offsetMotion.value20;
    case ENEMY_ECL_OPERAND_BASE_VALUE20:
        return (int)runtime.baseMotion.value20;
    case ENEMY_ECL_OPERAND_SPECIAL_ENEMY_X:
        return (int)g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position.x;
    case ENEMY_ECL_OPERAND_SPECIAL_ENEMY_Y:
        return (int)g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position.y;
    case ENEMY_ECL_OPERAND_PRIMARY_VM_VALUE38A:
        return EnemyResolveManagedVmId(&runtime.managedVmIds[0])->value38A;
    case ENEMY_ECL_OPERAND_RANK:
        return g_EnemyRank;
    case ENEMY_ECL_OPERAND_DIFFICULTY:
        return g_EnemyDifficulty;
    case ENEMY_ECL_OPERAND_WORLD_VELOCITY_ANGLE:
        return (int)atan2(runtime.worldMotion.velocity.y, runtime.worldMotion.velocity.x);
    case ENEMY_ECL_OPERAND_ONE:
        return 1;
    case ENEMY_ECL_OPERAND_LIFE:
        return runtime.life;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_0:
        return g_EnemyDifficulty == 0;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_1:
        return g_EnemyDifficulty == 1;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_2:
        return g_EnemyDifficulty == 2;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_3:
        return g_EnemyDifficulty == 3;
    default:
        return 0;
    }
}

int *EnemyFullObjectView::ResolveIntOperand(int operand)
{
    switch (operand)
    {
    case ENEMY_ECL_OPERAND_INT_0: return &runtime.eclVariables.integers[0];
    case ENEMY_ECL_OPERAND_INT_1: return &runtime.eclVariables.integers[1];
    case ENEMY_ECL_OPERAND_INT_2: return &runtime.eclVariables.integers[2];
    case ENEMY_ECL_OPERAND_INT_3: return &runtime.eclVariables.integers[3];
    default: return NULL;
    }
}

float EnemyFullObjectView::ReadFloatOperand(int operand)
{
    switch (operand)
    {
    case ENEMY_ECL_OPERAND_RANDOM_U32:
        return (float)g_EnemyRandom.GetU32();
    case ENEMY_ECL_OPERAND_RANDOM_UNIT:
        return g_EnemyRandom.GetUnitFloat();
    case ENEMY_ECL_OPERAND_RANDOM_ANGLE:
        return g_EnemyRandom.GetSignedUnitFloat() * 3.1415927f;
    case ENEMY_ECL_OPERAND_WORLD_X_A:
    case ENEMY_ECL_OPERAND_WORLD_X_B:
        return runtime.worldMotion.position.x;
    case ENEMY_ECL_OPERAND_POSITION_Y_A:
    case ENEMY_ECL_OPERAND_POSITION_Y_B:
        return runtime.worldMotion.position.y;
    case ENEMY_ECL_OPERAND_OFFSET_X_A:
    case ENEMY_ECL_OPERAND_OFFSET_X_B:
        return runtime.offsetMotion.position.x;
    case ENEMY_ECL_OPERAND_OFFSET_Y_A:
    case ENEMY_ECL_OPERAND_OFFSET_Y_B:
        return runtime.offsetMotion.position.y;
    case ENEMY_ECL_OPERAND_BASE_X_A:
    case ENEMY_ECL_OPERAND_BASE_X_B:
        return runtime.baseMotion.position.x;
    case ENEMY_ECL_OPERAND_BASE_Y_A:
    case ENEMY_ECL_OPERAND_BASE_Y_B:
        return runtime.baseMotion.position.y;
    case ENEMY_ECL_OPERAND_PLAYER_X_A:
    case ENEMY_ECL_OPERAND_PLAYER_X_B:
        return g_Player->drawPosition.x;
    case ENEMY_ECL_OPERAND_PLAYER_Y_A:
    case ENEMY_ECL_OPERAND_PLAYER_Y_B:
        return g_Player->drawPosition.y;
    case ENEMY_ECL_OPERAND_ANGLE_TO_PLAYER_WORLD:
        return EnemyAngleFromPlayer(g_Player, &runtime.worldMotion.position);
    case ENEMY_ECL_OPERAND_UPDATE_TIMER:
        return runtime.updateTimer.subframe;
    case ENEMY_ECL_OPERAND_RANDOM_SIGNED:
        return g_EnemyRandom.GetSignedUnitFloat();
    case ENEMY_ECL_OPERAND_FLAG_10000:
        return (float)((runtime.flags >> 16) & 1);
    case ENEMY_ECL_OPERAND_INT_0:
        return (float)runtime.eclVariables.integers[0];
    case ENEMY_ECL_OPERAND_INT_1:
        return (float)runtime.eclVariables.integers[1];
    case ENEMY_ECL_OPERAND_INT_2:
        return (float)runtime.eclVariables.integers[2];
    case ENEMY_ECL_OPERAND_INT_3:
        return (float)runtime.eclVariables.integers[3];
    case ENEMY_ECL_OPERAND_FLOAT_0:
        return runtime.eclVariables.floats[0];
    case ENEMY_ECL_OPERAND_FLOAT_1:
        return runtime.eclVariables.floats[1];
    case ENEMY_ECL_OPERAND_FLOAT_2:
        return runtime.eclVariables.floats[2];
    case ENEMY_ECL_OPERAND_FLOAT_3:
        return runtime.eclVariables.floats[3];
    case ENEMY_ECL_OPERAND_OFFSET_VALUE1C:
        return runtime.offsetMotion.value1C;
    case ENEMY_ECL_OPERAND_BASE_VALUE1C:
        return runtime.baseMotion.value1C;
    case ENEMY_ECL_OPERAND_OFFSET_VALUE18:
        return runtime.offsetMotion.value18;
    case ENEMY_ECL_OPERAND_BASE_VALUE18:
        return runtime.baseMotion.value18;
    case ENEMY_ECL_OPERAND_OFFSET_VALUE20:
        return runtime.offsetMotion.value20;
    case ENEMY_ECL_OPERAND_BASE_VALUE20:
        return runtime.baseMotion.value20;
    case ENEMY_ECL_OPERAND_SPECIAL_ENEMY_X:
        return g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position.x;
    case ENEMY_ECL_OPERAND_SPECIAL_ENEMY_Y:
        return g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position.y;
    case ENEMY_ECL_OPERAND_RANK:
        return (float)g_EnemyRank;
    case ENEMY_ECL_OPERAND_DIFFICULTY:
        return (float)g_EnemyDifficulty;
    case ENEMY_ECL_OPERAND_WORLD_VELOCITY_ANGLE:
        return (float)atan2(runtime.worldMotion.velocity.y, runtime.worldMotion.velocity.x);
    case ENEMY_ECL_OPERAND_ONE:
        return 1.0f;
    case ENEMY_ECL_OPERAND_ANGLE_TO_PLAYER_OFFSET:
        return EnemyAngleFromPlayer(g_Player, &runtime.offsetMotion.position);
    case ENEMY_ECL_OPERAND_ANGLE_TO_PLAYER_BASE:
        return EnemyAngleFromPlayer(g_Player, &runtime.baseMotion.position);
    case ENEMY_ECL_OPERAND_LIFE:
        return (float)runtime.life;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_0:
        return (float)g_EnemyDifficulty == 0.0f ? 1.0f : 0.0f;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_1:
        return (float)g_EnemyDifficulty == 1.0f ? 1.0f : 0.0f;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_2:
        return (float)g_EnemyDifficulty == 2.0f ? 1.0f : 0.0f;
    case ENEMY_ECL_OPERAND_DIFFICULTY_IS_3:
        return (float)g_EnemyDifficulty == 3.0f ? 1.0f : 0.0f;
    default:
        return 0.0f;
    }
}

float *EnemyFullObjectView::ResolveFloatOperand(int operand)
{
    switch (operand)
    {
    case ENEMY_ECL_OPERAND_FLOAT_0: return &runtime.eclVariables.floats[0];
    case ENEMY_ECL_OPERAND_FLOAT_1: return &runtime.eclVariables.floats[1];
    case ENEMY_ECL_OPERAND_FLOAT_2: return &runtime.eclVariables.floats[2];
    case ENEMY_ECL_OPERAND_FLOAT_3: return &runtime.eclVariables.floats[3];
    default: return NULL;
    }
}

// Maintained source for the reviewed 0x0040DC80-0x0040E5EB hostile runtime
// update owner. Allocation/constructor evidence proves that the sole target
// argument points to the 0x14DC-byte tail at full object +0x103C. The target
// body independently loads that pointer from the stack and returns with RET 4.
int __stdcall EnemyRuntimeUpdate(EnemyRuntimeView *enemy)
{
    if ((enemy->flags & 0x400u) != 0)
        return 0;

    enemy->flags |= 0x400u;
    enemy->previousMotion = enemy->worldMotion;

    EnemyFloat2 interpolation;
    if (enemy->scalarInterpolations[0].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[0], &interpolation);
        enemy->offsetMotion.value1C = EnemyWrapAngle(interpolation.x);
        enemy->offsetMotion.value18 = interpolation.y;
    }
    if (enemy->scalarInterpolations[2].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[2], &interpolation);
        enemy->offsetMotion.value20 = interpolation.x;
        enemy->offsetMotion.value24 = interpolation.y;
    }
    if (enemy->scalarInterpolations[1].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[1], &interpolation);
        enemy->baseMotion.value1C = EnemyWrapAngle(interpolation.x);
        enemy->baseMotion.value18 = interpolation.y;
    }
    if (enemy->scalarInterpolations[3].duration != 0)
    {
        EnemyEvaluateScalarInterpolation(
            &enemy->scalarInterpolations[3], &interpolation);
        enemy->baseMotion.value20 = interpolation.x;
        enemy->baseMotion.value24 = interpolation.y;
    }

    if (enemy->positionInterpolations[0].duration == 0)
    {
        if ((enemy->offsetMotion.flags & 1u) == 0)
        {
            EnemySetVectorFromPolar(
                &enemy->offsetMotion.velocity,
                enemy->offsetMotion.value1C,
                enemy->offsetMotion.value18);
            enemy->offsetMotion.velocity.z = 0.0f;
        }
        else
        {
            enemy->offsetMotion.value20 += enemy->offsetMotion.value24;
            enemy->offsetMotion.value1C = EnemyWrapAngle(
                enemy->offsetMotion.value1C + enemy->offsetMotion.value18);
        }
    }
    else
    {
        PlayerFloat3 target;
        EnemyEvaluatePositionInterpolation(
            &enemy->positionInterpolations[0], &target);
        enemy->offsetMotion.velocity.x = target.x - enemy->offsetMotion.position.x;
        enemy->offsetMotion.velocity.y = target.y - enemy->offsetMotion.position.y;
        enemy->offsetMotion.velocity.z = target.z - enemy->offsetMotion.position.z;
    }

    if (enemy->positionInterpolations[1].duration == 0)
    {
        if ((enemy->baseMotion.flags & 1u) == 0)
        {
            EnemySetVectorFromPolar(
                &enemy->baseMotion.velocity,
                enemy->baseMotion.value1C,
                enemy->baseMotion.value18);
            enemy->baseMotion.velocity.z = 0.0f;
        }
        else
        {
            enemy->baseMotion.value20 += enemy->baseMotion.value24;
            enemy->baseMotion.value1C = EnemyWrapAngle(
                enemy->baseMotion.value1C + enemy->baseMotion.value18);
        }
    }
    else
    {
        PlayerFloat3 target;
        EnemyEvaluatePositionInterpolation(
            &enemy->positionInterpolations[1], &target);
        enemy->baseMotion.velocity.x = target.x - enemy->baseMotion.position.x;
        enemy->baseMotion.velocity.y = target.y - enemy->baseMotion.position.y;
        enemy->baseMotion.velocity.z = target.z - enemy->baseMotion.position.z;
    }

    EnemyAdvanceMotion(&enemy->offsetMotion);
    if ((enemy->flags & 0x40000u) != 0)
    {
        enemy->baseMotion.position.x += g_EnemyGlobalPositionOffset.x;
        enemy->baseMotion.position.y += g_EnemyGlobalPositionOffset.y;
        enemy->baseMotion.position.z += g_EnemyGlobalPositionOffset.z;
    }
    EnemyAdvanceMotion(&enemy->baseMotion);

    enemy->worldMotion.velocity.x =
        enemy->baseMotion.position.x + enemy->offsetMotion.position.x -
        enemy->worldMotion.position.x;
    enemy->worldMotion.velocity.y =
        enemy->baseMotion.position.y + enemy->offsetMotion.position.y -
        enemy->worldMotion.position.y;
    enemy->worldMotion.velocity.z =
        enemy->baseMotion.position.z + enemy->offsetMotion.position.z -
        enemy->worldMotion.position.z;
    EnemyAdvanceMotion(&enemy->worldMotion);

    if ((enemy->flags & 0x200u) != 0)
    {
        const float halfWidth = enemy->movementBoundsSizeX * 0.5f;
        const float minimumX = enemy->movementBoundsCenterX - halfWidth;
        const float maximumX = enemy->movementBoundsCenterX + halfWidth;
        if (enemy->worldMotion.position.x < minimumX)
            enemy->worldMotion.position.x = minimumX;
        else if (maximumX < enemy->worldMotion.position.x)
            enemy->worldMotion.position.x = maximumX;

        const float halfHeight = enemy->movementBoundsSizeY * 0.5f;
        const float minimumY = enemy->movementBoundsCenterY - halfHeight;
        const float maximumY = enemy->movementBoundsCenterY + halfHeight;
        if (enemy->worldMotion.position.y < minimumY)
            enemy->worldMotion.position.y = minimumY;
        else if (maximumY < enemy->worldMotion.position.y)
            enemy->worldMotion.position.y = maximumY;

        enemy->offsetMotion.position.x =
            enemy->worldMotion.position.x - enemy->baseMotion.position.x;
        enemy->offsetMotion.position.y =
            enemy->worldMotion.position.y - enemy->baseMotion.position.y;
        enemy->offsetMotion.position.z =
            enemy->worldMotion.position.z - enemy->baseMotion.position.z;
    }

    const float visibilityHalfWidth = enemy->visibilityExtentX * 0.5f;
    const float visibilityHalfHeight = enemy->visibilityExtentY * 0.5f;
    if (enemy->worldMotion.position.x + visibilityHalfWidth < g_EnemyPlayfieldMinX ||
        g_EnemyPlayfieldMaxX < enemy->worldMotion.position.x - visibilityHalfWidth ||
        enemy->worldMotion.position.y + visibilityHalfHeight < g_EnemyPlayfieldMinY ||
        g_EnemyPlayfieldMaxY < enemy->worldMotion.position.y - visibilityHalfHeight)
    {
        if ((enemy->flags & 0x100u) != 0 && (enemy->flags & 4u) == 0)
            return -1;
    }
    else
    {
        enemy->flags |= 0x100u;
    }

    if ((enemy->flags & 0x100000u) != 0)
    {
        if (g_EnemyAnimationMode->value28 != 0)
        {
            if ((enemy->flags & 0x200000u) == 0)
            {
                enemy->animationBaseScript = enemy->animationScriptPrimary;
                EnemySetAnimationScript(enemy, enemy->animationScriptPrimary);
                enemy->flags |= 0x200001u;
            }
        }
        else if ((enemy->flags & 0x200000u) != 0)
        {
            enemy->animationBaseScript = enemy->animationScriptAlternate;
            EnemySetAnimationScript(enemy, enemy->animationScriptAlternate);
            enemy->flags &= 0xffdffffeu;
        }
    }

    const float timerScale = *enemy->updateTimer.scale;
    if (EnemyRunEcl(enemy->owner, timerScale) != 0)
        return -1;

    enemy->flags &= ~0x2000u;
    if ((enemy->flags & 0x11u) == 0)
    {
        int damage = g_Player->CalculateDamageToTarget(
            &enemy->worldMotion.position,
            reinterpret_cast<const PlayerFloat3 *>(&enemy->damageHitbox),
            NULL);
        if (g_Player->runtimeState == 2 || g_Player->runtimeState == 0)
            damage /= 5;

        if (damage != 0)
        {
            if ((g_EnemyGameState->flags378C & 1u) != 0 &&
                (enemy->flags & 0x8000u) != 0)
            {
                damage /= 5;
                if (damage < 1)
                    damage = 1;
            }

            if ((enemy->flags & 8u) == 0 &&
                enemy->damageReductionTimer.current < 1)
            {
                enemy->life -= damage;
            }

            const unsigned char *callback =
                EnemyResolveRuntimeCallback(enemy->owner);
            if (callback != NULL)
            {
                enemy->owner->ReleaseEclAllocations();
                enemy->owner->ResetEclState();
                EnemyRunCallbackEcl(enemy->owner, callback);
                if (EnemyRunEcl(enemy->owner, timerScale) != 0)
                    return -1;
            }

            if (enemy->life < 1 && (enemy->flags & 0x40u) == 0)
            {
                EnemyAddScoreReward(enemy->scoreReward);
                if (EnemyFinalizeDeath(enemy->owner) != 0)
                    return 1;
            }
            enemy->flags |= 0x2000u;
        }
        g_EnemyDamageProcessedFlag = 1;
    }

    const unsigned char *callback = EnemyResolveRuntimeCallback(enemy->owner);
    if (callback != NULL)
    {
        enemy->owner->ReleaseEclAllocations();
        enemy->owner->ResetEclState();
        EnemyInstallCallbackEcl(enemy->owner, callback);
    }

    if ((enemy->flags & 0x12u) == 0 &&
        enemy->playerCollisionTimer.current < 1)
    {
        EnemyCheckPlayerCollision(
            &enemy->worldMotion.position, &enemy->playerCollisionHitbox, g_Player);
    }

    if ((enemy->flags & 0x1000u) != 0)
    {
        int direction;
        if (g_EnemyDirectionNegativeThreshold <= enemy->worldMotion.velocity.x)
        {
            if (enemy->worldMotion.velocity.x <= g_EnemyDirectionPositiveThreshold)
                direction = 0;
            else
                direction = 1;
        }
        else
        {
            direction = -1;
        }

        const int previousDirection = enemy->animationDirection;
        if (previousDirection != direction)
        {
            int scriptOffset = 0;
            if (previousDirection == -1)
                scriptOffset = 3 - (direction != 0);
            else if (previousDirection == 0)
                scriptOffset = (direction != -1) + 1;
            else if (previousDirection == 1)
                scriptOffset = direction == 0 ? 4 : 1;

            enemy->animationDirection = direction;
            EnemySetAnimationScript(
                enemy, enemy->animationBaseScript + scriptOffset);
        }
    }

    int i;
    if ((enemy->flags & 0x40000u) == 0)
    {
        for (i = 0; i < 8; ++i)
            EnemySetManagedVmPositionWithOffset(
                enemy->managedVmIds[i], &enemy->worldMotion.position);
    }
    else
    {
        for (i = 0; i < 8; ++i)
            EnemySetManagedVmPositionExact(
                enemy->managedVmIds[i], &enemy->worldMotion.position);
    }

    if ((enemy->flags & 0x11u) == 0 &&
        (enemy->flags & 0xc0000u) == 0)
    {
        EnemyFullObjectView *tracked = g_Player->trackedEnemy;
        const float playerX = g_Player->drawPosition.x;
        if (tracked == NULL ||
            (float)fabs(enemy->worldMotion.position.x - playerX) >
                (float)fabs(tracked->runtime.worldMotion.position.x - playerX))
        {
            if (g_Player->trackedEnemyValid == 0)
                g_Player->trackedEnemy = enemy->owner;
            g_Player->trackedEnemyValid = 1;
        }
    }

    EnemyManagedVmView *primaryVm = EnemyResolveManagedVm(enemy->managedVmIds[0]);
    if (primaryVm == NULL)
        enemy->managedVmIds[0] = 0;

    if (enemy->damageFlashFrames != 0)
    {
        primaryVm->flags &= ~0x8000u;
        if ((enemy->flags & 0x8000u) != 0)
            g_EnemyVisualState->flags9DA4 &= ~0x8000u;
        enemy->damageFlashFrames--;
    }
    else if ((enemy->flags & 0x2000u) != 0)
    {
        primaryVm->color = 0xff0000ffu;
        primaryVm->flags |= 0x8000u;
        enemy->damageFlashFrames = 4;

        int effectKind = 0x13;
        if ((enemy->flags & 0x8000u) != 0)
        {
            const unsigned int gameFlags = g_EnemyGameState->flags378C;
            if ((gameFlags & 1u) == 0 || (gameFlags & 8u) == 0)
            {
                if ((gameFlags & 1u) == 0)
                {
                    if (enemy->value13C8 < 900)
                        effectKind = 0x23;
                }
                else if (enemy->value13C8 < 300)
                {
                    effectKind = 0x23;
                }
            }
        }
        EnemySpawnDamageEffect(effectKind, enemy->worldMotion.position.x);
    }

    if (enemy->damageReductionTimer.current > 0)
        EnemyAdvanceTimer(&enemy->damageReductionTimer, -1.0f);
    if (enemy->playerCollisionTimer.current > 0)
        EnemyAdvanceTimer(&enemy->playerCollisionTimer, -1.0f);

    enemy->updateTimer.previous = enemy->updateTimer.current;
    const float updateScale = *enemy->updateTimer.scale;
    if (updateScale > 0.9900000095367432f &&
        updateScale < 1.0099999904632568f)
    {
        ++enemy->updateTimer.current;
        enemy->updateTimer.subframe += 1.0f;
    }
    else
    {
        enemy->updateTimer.subframe += updateScale;
        enemy->updateTimer.current = (int)enemy->updateTimer.subframe;
    }

    return 0;
}


static EnemyCallbackNodeView *CreateEnemyCallbackNode(EnemyChainCallback callback)
{
    EnemyCallbackNodeView *node = static_cast<EnemyCallbackNodeView *>(
        ::operator new(sizeof(EnemyCallbackNodeView)));
    if (node != NULL)
    {
        node->priority = 0;
        node->flags &= ~1u;
        node->callback = NULL;
        node->unknown00C = NULL;
        node->unknown010 = NULL;
        node->self14 = node;
        node->unknown018 = NULL;
        node->unknown01C = NULL;
        node->callback = callback;
    }
    return node;
}

static void RemoveEnemyCallbackNode(EnemyCallbackNodeView *node)
{
    if (node == NULL)
        return;

    EnterCriticalSection(&g_EnemyCallbackCriticalSection);
    ++g_EnemyCallbackLockDepth;
    EnemyUnlinkCallbackNode(node, g_EnemyCallbackManager);
    LeaveCriticalSection(&g_EnemyCallbackCriticalSection);
    --g_EnemyCallbackLockDepth;
}

// Target 0x0040CD20-0x0040CD7E is the filename-loading virtual entry of the
// 0x1098-byte ECL resource object. It carries the object in ECX, one filename
// on the stack, and RET 4. The maintained member/type spelling is descriptive.
int EnemyEclResourceView::LoadFile(const char *filename)
{
    g_EnemyEclFilenameBuffer[0] = '\0';
    strcat(g_EnemyEclFilenameBuffer, filename);

    void *scriptData = EnemyLoadFileBytes(
        g_EnemyEclFilenameBuffer, NULL, 0);
    const int result = AddScriptData(scriptData);
    return result < 0 ? -1 : 0;
}

// Target 0x0040D400-0x0040D4EE is reached from the second slot of the derived
// ECL-resource vtable. The package begins with an ANIM string list, followed by
// a four-byte-aligned ECLI string list. Animation slots start at target index 9;
// loaded resources are mirrored into manager effectResources[1..].
int EnemyEclResourceView::LoadPackage(const unsigned char *packageData)
{
    const unsigned int ANIM_MAGIC = 0x4d494e41u;
    const unsigned int ECLI_MAGIC = 0x494c4345u;
    const unsigned int *header =
        reinterpret_cast<const unsigned int *>(packageData);
    if (header[0] != ANIM_MAGIC)
        return 0;

    unsigned int animationCount = header[1];
    const char *cursor = reinterpret_cast<const char *>(packageData + 8);
    for (unsigned int i = 0; i < animationCount; ++i)
    {
        void *loaded = EnemyLoadAnimationResource(
            static_cast<int>(i + 9), g_EnemyAnimationOwner, cursor);
        g_EnemyManager->effectResources[i + 1] = loaded;
        if (loaded == NULL)
        {
            EnemyReportResourceLoadError();
            return -1;
        }
        cursor += strlen(cursor) + 1;
    }

    const unsigned int offset = static_cast<unsigned int>(
        cursor - reinterpret_cast<const char *>(packageData));
    const unsigned int remainder = offset & 3u;
    if (remainder != 0)
        cursor += 4u - remainder;

    header = reinterpret_cast<const unsigned int *>(cursor);
    if (header[0] == ECLI_MAGIC)
    {
        const unsigned int eclCount = header[1];
        cursor += 8;
        for (unsigned int i = 0; i < eclCount; ++i)
        {
            LoadFile(cursor);
            cursor += strlen(cursor) + 1;
        }
    }

    return 0;
}

// Target 0x0040D680-0x0040D6A3 is the derived destruction path. Its only
// source-visible work is the inherited +0x8C owned-table release; VC7 owns the
// observed transition back to the base vtable rather than maintained source.

EnemyEclResourceView::~EnemyEclResourceView()
{
}

// Target 0x0040D260-0x0040D27C is retained independently and is also inlined
// byte-for-byte into the factory at 0x0040D6C1. The nested timer constructor
// clears +0x50 before this source body clears the full 0x68-byte manager,
// marks flags bit 1, and publishes the process-global owner.
EnemyManagerView::EnemyManagerView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2u;
    g_EnemyManager = this;
}

// Target 0x0040D280-0x0040D3CD carries manager in EBX and one ECL-resource
// filename on the stack, returning with RET 4. It installs the process-owned
// ECL resource, registers update/draw callbacks, and initializes manager timer.
int EnemyManagerView::Initialize(const char *eclResourceFilename)
{
    EnemyManagerView *manager = this;
    manager->effectResources[0] =
        g_EnemyPrimaryResourceOwner->primaryEnemyResource;

    EnemyEclResourceView *resource = new EnemyEclResourceView;
    manager->scriptDatabase = resource;
    resource->LoadFile(eclResourceFilename);

    EnemyCallbackNodeView *updateNode =
        CreateEnemyCallbackNode(EnemyManagerUpdateCallback);
    updateNode->owner = manager;
    updateNode->flags &= ~2u;
    updateNode->flags |= 1u;
    EnemyRegisterUpdateCallbackNode(updateNode, 0x12, g_EnemyCallbackManager);
    manager->updateCallbackNode = updateNode;

    EnemyCallbackNodeView *drawNode =
        CreateEnemyCallbackNode(EnemyManagerDrawCallback);
    drawNode->owner = manager;
    drawNode->flags &= ~2u;
    drawNode->flags |= 1u;
    EnemyRegisterDrawCallbackNode(drawNode, 0x14, g_EnemyCallbackManager);
    manager->drawCallbackNode = drawNode;

    if ((manager->timer.flags & 1u) == 0)
    {
        manager->timer.previous = -999999;
        manager->timer.current = 0;
        manager->timer.subframe = 0.0f;
        manager->timer.scale = &g_PlayerTimerScale;
        manager->timer.flags |= 1u;
    }
    manager->timer.current = 0;
    manager->timer.subframe = 0.0f;
    manager->timer.previous = -1;
    return 0;
}

// Target 0x0040D530-0x0040D65B carries manager in EAX and returns with plain
// RET. It first clears live enemies/resources, removes both chain nodes, releases
// all ECL resource allocations, conditionally unloads four animation resources,
// and finally clears the global manager owner.
EnemyManagerView::~EnemyManagerView()
{
    EnemyManagerView *manager = this;
    EnemyManagerClear(manager);

    RemoveEnemyCallbackNode(manager->updateCallbackNode);
    RemoveEnemyCallbackNode(manager->drawCallbackNode);

    EnemyEclResourceView *resource = manager->scriptDatabase;
    for (int i = 0; i < 32; ++i)
    {
        if (resource->scriptData[i] != NULL)
            free(resource->scriptData[i]);
    }

    if (resource != NULL)
        delete resource;
    manager->scriptDatabase = NULL;

    if ((g_EnemyLifecycleFlags & 9u) == 0)
    {
        for (int i = 0; i < 4; ++i)
        {
            void *loaded = g_EnemyAnimationOwner->resourceSlots[i];
            if (loaded != NULL)
            {
                EnemyDestroyAnimationResource(g_EnemyAnimationOwner, loaded);
                free(loaded);
                g_EnemyAnimationOwner->resourceSlots[i] = NULL;
            }
        }
    }

    g_EnemyManager = NULL;
}

// Target 0x0040D6B0-0x0040D70E allocates exactly 0x68 bytes, runs the nested
// timer constructor before zeroing the full manager, publishes the global owner,
// then calls the EBX-bound initializer. The sole source argument is stack-bound
// and the target returns with RET 4.
EnemyManagerView * __stdcall EnemyManagerCreate(
    const char *eclResourceFilename)
{
    EnemyManagerView *manager = new EnemyManagerView;
    if (manager->Initialize(eclResourceFilename) != 0)
    {
        delete manager;
        return NULL;
    }
    return manager;
}

// Maintained logical constructor for target 0x0040D830-0x0040DAD0. The target
// machine boundary carries the full object in ESI and one stack subroutine-name
// argument with RET 4. The ordinary C++ spelling recovers the target-proven
// base/derived construction order without claiming that private machine ABI.
EnemyFullObjectView::EnemyFullObjectView(const char *eclSubroutineName)
{
    EnemyFullObjectView *enemy = this;

    EnemyPrepareRuntimeStorage(&enemy->runtime);
    memset(&enemy->runtime, 0, sizeof(enemy->runtime));

    enemy->embeddedEclContext.value00 = 0;
    enemy->embeddedEclContext.currentInstruction = NULL;
    enemy->activeEclContext = &enemy->embeddedEclContext;
    enemy->embeddedEclContext.operandResolver = enemy;
    enemy->flags1028 &= ~1u;
    enemy->value1020 = 0;
    enemy->embeddedEclContext.unknown1010 = -1;
    enemy->eclContextMirror = &enemy->embeddedEclContext;
    enemy->ownedAllocations = NULL;
    enemy->value1038 = 0;

    enemy->runtime.owner = enemy;
    enemy->runtime.listNode.enemy = enemy;
    enemy->runtime.listNode.next = NULL;
    enemy->runtime.listNode.previous = NULL;

    enemy->runtime.damageHitbox.x = 24.0f;
    enemy->runtime.damageHitbox.y = 24.0f;
    enemy->runtime.playerCollisionHitbox.x = 24.0f;
    enemy->runtime.playerCollisionHitbox.y = 24.0f;
    enemy->runtime.value1400 = 32.0f;
    enemy->runtime.value1404 = 32.0f;
    enemy->runtime.managerSlot = -1;

    if ((enemy->runtime.updateTimer.flags & 1u) == 0)
    {
        enemy->runtime.updateTimer.previous = -999999;
        enemy->runtime.updateTimer.current = 0;
        enemy->runtime.updateTimer.subframe = 0.0f;
        enemy->runtime.updateTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.updateTimer.flags |= 1u;
    }
    enemy->runtime.updateTimer.current = 0;
    enemy->runtime.updateTimer.subframe = 0.0f;
    enemy->runtime.updateTimer.previous = -1;

    if ((enemy->runtime.damageReductionTimer.flags & 1u) == 0)
    {
        enemy->runtime.damageReductionTimer.previous = -999999;
        enemy->runtime.damageReductionTimer.current = 0;
        enemy->runtime.damageReductionTimer.subframe = 0.0f;
        enemy->runtime.damageReductionTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.damageReductionTimer.flags |= 1u;
    }
    enemy->runtime.damageReductionTimer.current = 0;
    enemy->runtime.damageReductionTimer.subframe = 0.0f;
    enemy->runtime.damageReductionTimer.previous = -1;

    if ((enemy->runtime.playerCollisionTimer.flags & 1u) == 0)
    {
        enemy->runtime.playerCollisionTimer.previous = -999999;
        enemy->runtime.playerCollisionTimer.current = 0;
        enemy->runtime.playerCollisionTimer.subframe = 0.0f;
        enemy->runtime.playerCollisionTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.playerCollisionTimer.flags |= 1u;
    }
    enemy->runtime.playerCollisionTimer.current = 0;
    enemy->runtime.playerCollisionTimer.subframe = 0.0f;
    enemy->runtime.playerCollisionTimer.previous = -1;

    enemy->scriptDatabase = g_EnemyManager->scriptDatabase;
    enemy->activeEclContext->currentInstruction =
        EnemyLookupEclSubroutine(enemy->scriptDatabase, eclSubroutineName);
    enemy->activeEclContext->value00 = 0;

    enemy->runtime.life = 0;
    enemy->runtime.unknown13C4 = 0;
    enemy->runtime.value13C8 = 0;

    for (int i = 0; i < 8; ++i)
    {
        enemy->runtime.callbackThresholds[i].lifeThreshold = -1;
        enemy->runtime.callbackThresholds[i].timerThreshold = -1;
        enemy->runtime.callbackThresholds[i].lifeCallbackName = NULL;
    }

}

// Maintained source for target 0x0040CFB0 plus its compiler-owned alignment and
// switch tables through 0x0040D1F0. The target machine boundary uses EAX for the
// 0x40-byte request and two stack arguments (manager, ECL subroutine name), then
// RET 8. The ordinary signature below preserves logical values without claiming
// that private register assignment.
EnemyFullObjectView *EnemySpawn(
    EnemyManagerView *manager,
    const char *eclSubroutineName,
    const EnemySpawnRequestView *request)
{
    EnemyFullObjectView *enemy = new EnemyFullObjectView(eclSubroutineName);

    enemy->runtime.offsetMotion.position = request->position;
    enemy->runtime.scoreReward = request->scoreReward;
    enemy->runtime.life = request->life;
    enemy->runtime.itemDropType = request->itemDropType;

    enemy->runtime.flags =
        (enemy->runtime.flags & ~0x800u) |
        ((static_cast<unsigned int>(request->setFlag0800) & 1u) << 11);
    enemy->spawnLayerMask =
        static_cast<unsigned char>(1u << g_EnemySpawnLayerIndex);

    // Spawn and the operand lvalue family share the exact 0x20-byte variable
    // block. Aggregate copy preserves all four integer and four float values.
    enemy->runtime.eclVariables = request->eclVariables;

    enemy->runtime.flags =
        (enemy->runtime.flags & ~0x40000u) |
        ((static_cast<unsigned int>(request->setFlag40000) & 1u) << 18);

    if ((enemy->runtime.damageReductionTimer.flags & 1u) == 0)
    {
        enemy->runtime.damageReductionTimer.previous = -999999;
        enemy->runtime.damageReductionTimer.current = 0;
        enemy->runtime.damageReductionTimer.subframe = 0.0f;
        enemy->runtime.damageReductionTimer.scale = &g_PlayerTimerScale;
        enemy->runtime.damageReductionTimer.flags |= 1u;
    }
    enemy->runtime.damageReductionTimer.current = 2;
    enemy->runtime.damageReductionTimer.subframe = 2.0f;
    enemy->runtime.damageReductionTimer.previous = 1;

    EnemyRuntimeUpdate(&enemy->runtime);

    if ((enemy->runtime.flags & 0x8000u) != 0)
    {
        if (enemy->runtime.itemDropType == 1)
            enemy->runtime.itemDropType = 10;
        else if (enemy->runtime.itemDropType == 4)
            enemy->runtime.itemDropType = 11;
    }

    enemy->runtime.deathSoundId = (manager->spawnCounter & 1) + 2;
    enemy->runtime.deathEffectScript = 0x167;
    if (enemy->runtime.value0EC == 1)
    {
        switch (enemy->runtime.value0F0)
        {
        case 5:
        case 25:
        case 50:
            enemy->runtime.deathEffectScript = 0x164;
            break;
        case 10:
        case 30:
        case 51:
            enemy->runtime.deathEffectScript = 0x16a;
            break;
        case 15:
        case 35:
        case 52:
            enemy->runtime.deathEffectScript = 0x16d;
            break;
        default:
            break;
        }
    }
    enemy->runtime.deathEffectResourceIndex = 0;

    EnemyListNodeView *node = &enemy->runtime.listNode;
    if (manager->enemyListHead == NULL)
    {
        manager->enemyListHead = node;
    }
    else
    {
        EnemyListNodeView *tail = manager->enemyListTail;
        if (tail->next != NULL)
        {
            node->next = tail->next;
            tail->next->previous = node;
        }
        tail->next = node;
        node->previous = tail;
    }
    manager->enemyListTail = node;
    ++manager->activeEnemyCount;
    ++manager->spawnCounter;
    return enemy;
}

// Target 0x0040D750-0x0040D7F5 carries EnemyManagerView* in EDI and returns
// with plain RET. The target callback wrapper at 0x0040D810 supplies EDI from
// its conventional ECX callback argument.
int EnemyManagerUpdate(EnemyManagerView *manager)
{
    EnemyListNodeView *node = manager->enemyListHead;
    while (node != NULL)
    {
        EnemyFullObjectView *enemy = node->enemy;
        EnemyListNodeView *next = node->next;

        if ((enemy->runtime.flags & 0x20000u) == 0)
        {
            if (EnemyRuntimeUpdate(&enemy->runtime) == 0)
                enemy->runtime.flags &= ~0x400u;
            else
                delete enemy;
        }
        else
        {
            delete enemy;
        }
        node = next;
    }

    manager->timer.previous = manager->timer.current;
    const float scale = *manager->timer.scale;
    if (scale > 0.9900000095367432f && scale < 1.0099999904632568f)
    {
        ++manager->timer.current;
        manager->timer.subframe += 1.0f;
    }
    else
    {
        manager->timer.subframe += scale;
        manager->timer.current = static_cast<int>(manager->timer.subframe);
    }
    return 1;
}

// Registered at target update priority 0x12. The machine body is a thin
// ECX-to-EDI wrapper around the private EnemyManagerUpdate owner.
int __fastcall EnemyManagerUpdateCallback(EnemyManagerView *manager)
{
    return EnemyManagerUpdate(manager);
}

// Registered through the target draw-chain registrar at priority 0x14.
int __fastcall EnemyManagerDrawCallback(EnemyManagerView *manager)
{
    (void)manager;
    return 1;
}

// Maintained destructor body for target 0x0040DAE0-0x0040DC61. The target's
// private boundary carries the full object on the stack with RET 4. The
// scalar-deleting wrapper at 0x0040CC50 owns optional object deallocation; the
// compiler then appends the recovered base-host destruction.
EnemyFullObjectView::~EnemyFullObjectView()
{
    EnemyFullObjectView *enemy = this;
    EnemyManagerView *manager = g_EnemyManager;
    EnemyListNodeView *listNode = &enemy->runtime.listNode;

    if (manager->enemyListHead == listNode)
        manager->enemyListHead = listNode->next;
    if (manager->enemyListTail == listNode)
        manager->enemyListTail = listNode->previous;
    if (listNode->next != NULL)
        listNode->next->previous = listNode->previous;
    if (listNode->previous != NULL)
        listNode->previous->next = listNode->next;
    listNode->next = NULL;
    listNode->previous = NULL;
    --manager->activeEnemyCount;

    if ((enemy->runtime.flags & 0x8000u) != 0)
        manager->specialEnemySlots[enemy->runtime.managerSlot] = NULL;

    for (int i = 0; i < 10; ++i)
    {
        const unsigned int vmId = enemy->runtime.managedVmIds[i];
        if (vmId != 0)
        {
            EnemyManagedVmView *vm = NULL;
            EnemyManagedVmListNodeView *vmNode =
                g_EnemyManagedVmRegistry->primaryList;
            while (vmNode != NULL)
            {
                if (vmNode->vm->id == vmId)
                {
                    vm = vmNode->vm;
                    break;
                }
                vmNode = vmNode->next;
            }
            if (vm == NULL)
            {
                vmNode = g_EnemyManagedVmRegistry->secondaryList;
                while (vmNode != NULL)
                {
                    if (vmNode->vm->id == vmId)
                    {
                        vm = vmNode->vm;
                        break;
                    }
                    vmNode = vmNode->next;
                }
            }

            if (vm != NULL)
            {
                vm->flags |= 0x4000000u;
                if (vm->childState == 0)
                {
                    EnemyManagedVmListNodeView *child = vm->children;
                    while (child != NULL)
                    {
                        child->vm->flags |= 0x4000000u;
                        child = child->next;
                    }
                }
            }
        }
        enemy->runtime.managedVmIds[i] = 0;
    }

    if (g_Player != NULL)
    {
        if (g_Player->trackedEnemy == enemy)
        {
            g_Player->trackedEnemy = NULL;
            g_Player->trackedEnemyValid = 0;
        }
        for (int i = 0; i < 128; ++i)
        {
            if (g_Player->shots[i].trackedEnemy == enemy)
                g_Player->shots[i].trackedEnemy = NULL;
        }
    }

}

// Reviewed target 0x0040E5F0-0x0040E691 finalization path. The direct callees
// use private register live-ins; these descriptive calls express only the
// target-observed logical values and do not claim their original declarations.
int __stdcall EnemyFinalizeDeath(EnemyFullObjectView *enemy)
{
    EnemyRuntimeView *runtime = &enemy->runtime;

    if (runtime->deathSoundId >= 0)
        EnemyPlayDeathSound(runtime->deathSoundId, runtime->worldMotion.position.x);

    if (runtime->deathEffectScript >= 0)
    {
        EnemySpawnDeathEffect(
            &runtime->worldMotion.position,
            g_EnemyManager->effectResources[runtime->deathEffectResourceIndex],
            runtime->deathEffectScript);
    }

    if (runtime->itemDropType > 0)
    {
        EnemySpawnItem(
            &runtime->worldMotion.position,
            runtime->itemDropType,
            -1,
            -1.5707964f,
            2.2f);
    }

    EnemyDropItemCounts(&runtime->worldMotion.position, &runtime->itemDropType);
    runtime->itemDropType = 0;
    EnemyPlaySound(10);
    return 1;
}
