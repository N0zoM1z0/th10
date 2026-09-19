#ifndef TH10_RECONSTRUCTION_ENEMY_HPP
#define TH10_RECONSTRUCTION_ENEMY_HPP

#include "Player.hpp"

#include <stddef.h>

struct EnemyFullObjectView;
struct EnemyEclHostBaseView;
struct EnemyListNodeView;
struct EnemyManagerView;
struct EnemyVisualStateView;

struct EnemyFloat2
{
    float x;
    float y;
};
typedef char EnemyFloat2SizeIs08[(sizeof(EnemyFloat2) == 0x08) ? 1 : -1];

// Maintained view of the 0x2C movement records consumed by the reviewed TH10
// hostile runtime owner. Position, velocity and the flag word are target-proven;
// the four scalar motion parameters retain offset-based names because their
// original semantics remain unresolved.
struct EnemyMotionView
{
    PlayerFloat3 position;
    PlayerFloat3 velocity;
    float value18;
    float value1C;
    float value20;
    float value24;
    unsigned int flags;
};
typedef char EnemyMotionViewSizeIs2C[(sizeof(EnemyMotionView) == 0x2c) ? 1 : -1];
typedef char EnemyMotionVelocityAt0C[(offsetof(EnemyMotionView, velocity) == 0x0c) ? 1 : -1];
typedef char EnemyMotionFlagsAt28[(offsetof(EnemyMotionView, flags) == 0x28) ? 1 : -1];

// Opaque interpolation records with only the target-observed duration/mode
// controls exposed. Their helper ABIs are private register conventions.
struct EnemyPositionInterpolationView
{
    unsigned char unknown000[0x44];
    int duration;
    int mode;
};
typedef char EnemyPositionInterpolationSizeIs4C[
    (sizeof(EnemyPositionInterpolationView) == 0x4c) ? 1 : -1];
typedef char EnemyPositionInterpolationDurationAt44[
    (offsetof(EnemyPositionInterpolationView, duration) == 0x44) ? 1 : -1];

struct EnemyScalarInterpolationView
{
    unsigned char unknown000[0x34];
    int duration;
    int mode;
};
typedef char EnemyScalarInterpolationSizeIs3C[
    (sizeof(EnemyScalarInterpolationView) == 0x3c) ? 1 : -1];
typedef char EnemyScalarInterpolationDurationAt34[
    (offsetof(EnemyScalarInterpolationView, duration) == 0x34) ? 1 : -1];

// The manager list embeds this exact 12-byte node at full enemy +0x116C
// (runtime +0x130). Spawn links it and teardown unlinks it.
struct EnemyListNodeView
{
    EnemyFullObjectView *enemy;
    EnemyListNodeView *next;
    EnemyListNodeView *previous;
};
typedef char EnemyListNodeViewSizeIs0C[
    (sizeof(EnemyListNodeView) == 0x0c) ? 1 : -1];

struct EnemyCallbackThresholdView
{
    // Target 0x004127A0 consumes eight records with this exact field order.
    int lifeThreshold;
    int timerThreshold;
    const unsigned char *lifeCallbackName;
    const unsigned char *timerCallbackName;
};
typedef char EnemyCallbackThresholdViewSizeIs10[
    (sizeof(EnemyCallbackThresholdView) == 0x10) ? 1 : -1];

// The TH10 operand lvalue resolvers prove this exact 0x20-byte runtime block:
// four writable integer variables followed by four writable float variables.
// Original source identifiers remain unknown.
struct EnemyEclVariableView
{
    int integers[4];
    float floats[4];
};
typedef char EnemyEclVariableViewSizeIs20[
    (sizeof(EnemyEclVariableView) == 0x20) ? 1 : -1];
typedef char EnemyEclVariableFloatsAt10[
    (offsetof(EnemyEclVariableView, floats) == 0x10) ? 1 : -1];

struct EnemyRuntimeView
{
    // Target 0x0040E770 is the runtime-tail implementation reached by the
    // full-object virtual forwarding entry at 0x0040E760. Its maintained body
    // covers every target-active opcode; compiler exactness remains unknown.
    int DispatchEclInstruction();
    __declspec(noinline) int ReadIntArgument(int index);
    int *ResolveIntArgument(int index);
    float ReadFloatArgument(int index);
    float *ResolveFloatArgument(int index);
    int ReadRawIntArgument(int index, int rawValue);
    float ReadRawFloatArgument(int index, float rawValue);

    EnemyMotionView previousMotion;
    EnemyMotionView worldMotion;
    EnemyMotionView offsetMotion;
    EnemyMotionView baseMotion;
    EnemyFloat2 damageHitbox;
    EnemyFloat2 playerCollisionHitbox;
    unsigned int managedVmIds[10];
    int value0E8;
    int value0EC;
    int value0F0;
    int animationBaseScript;
    int animationDirection;
    // Target operand lvalue resolvers prove four writable integer variables at
    // full Enemy +0x1138..+0x1144 and four writable float variables at
    // +0x1148..+0x1154. This exact 0x20-byte block begins runtime +0x0FC.
    EnemyEclVariableView eclVariables;
    PlayerTimerView updateTimer;
    EnemyListNodeView listNode;
    EnemyPositionInterpolationView positionInterpolations[2];
    EnemyScalarInterpolationView scalarInterpolations[4];
    unsigned char unknown2C4[0x10e0];
    float visibilityExtentX;
    float visibilityExtentY;
    float movementBoundsCenterX;
    float movementBoundsCenterY;
    float movementBoundsSizeX;
    float movementBoundsSizeY;
    int scoreReward;
    int life;
    int unknown13C4;
    // The callback resolver writes life minus the next active life threshold.
    int value13C8;
    int itemDropType;
    int itemDropCounts[11];
    int unknown13FC;
    float value1400;
    float value1404;
    int deathSoundId;
    int deathEffectScript;
    int deathEffectResourceIndex;
    int damageFlashFrames;
    int unknown1418;
    PlayerTimerView damageReductionTimer;
    PlayerTimerView playerCollisionTimer;
    unsigned int flags;
    int animationScriptPrimary;
    int animationScriptAlternate;
    int managerSlot;
    int unknown1454;
    EnemyCallbackThresholdView callbackThresholds[8];
    EnemyFullObjectView *owner;
};

typedef char EnemyRuntimeViewSizeIs14DC[
    (sizeof(EnemyRuntimeView) == 0x14dc) ? 1 : -1];
typedef char EnemyRuntimeWorldMotionAt02C[
    (offsetof(EnemyRuntimeView, worldMotion) == 0x02c) ? 1 : -1];
typedef char EnemyRuntimeOffsetMotionAt058[
    (offsetof(EnemyRuntimeView, offsetMotion) == 0x058) ? 1 : -1];
typedef char EnemyRuntimeBaseMotionAt084[
    (offsetof(EnemyRuntimeView, baseMotion) == 0x084) ? 1 : -1];
typedef char EnemyRuntimeDamageHitboxAt0B0[
    (offsetof(EnemyRuntimeView, damageHitbox) == 0x0b0) ? 1 : -1];
typedef char EnemyRuntimePlayerHitboxAt0B8[
    (offsetof(EnemyRuntimeView, playerCollisionHitbox) == 0x0b8) ? 1 : -1];
typedef char EnemyRuntimeManagedVmsAt0C0[
    (offsetof(EnemyRuntimeView, managedVmIds) == 0x0c0) ? 1 : -1];
typedef char EnemyRuntimeValue0ECAt0EC[
    (offsetof(EnemyRuntimeView, value0EC) == 0x0ec) ? 1 : -1];
typedef char EnemyRuntimeEclVariablesAt0FC[
    (offsetof(EnemyRuntimeView, eclVariables) == 0x0fc) ? 1 : -1];
typedef char EnemyRuntimeEclFloatVariablesAt10C[
    (offsetof(EnemyRuntimeView, eclVariables) +
         offsetof(EnemyEclVariableView, floats) == 0x10c) ? 1 : -1];
typedef char EnemyRuntimeUpdateTimerAt11C[
    (offsetof(EnemyRuntimeView, updateTimer) == 0x11c) ? 1 : -1];
typedef char EnemyRuntimeListNodeAt130[
    (offsetof(EnemyRuntimeView, listNode) == 0x130) ? 1 : -1];
typedef char EnemyRuntimePositionInterpolationAt13C[
    (offsetof(EnemyRuntimeView, positionInterpolations) == 0x13c) ? 1 : -1];
typedef char EnemyRuntimeScalarInterpolationAt1D4[
    (offsetof(EnemyRuntimeView, scalarInterpolations) == 0x1d4) ? 1 : -1];
typedef char EnemyRuntimeVisibilityExtentAt13A4[
    (offsetof(EnemyRuntimeView, visibilityExtentX) == 0x13a4) ? 1 : -1];
typedef char EnemyRuntimeScoreAt13BC[
    (offsetof(EnemyRuntimeView, scoreReward) == 0x13bc) ? 1 : -1];
typedef char EnemyRuntimeLifeAt13C0[
    (offsetof(EnemyRuntimeView, life) == 0x13c0) ? 1 : -1];
typedef char EnemyRuntimeItemDropAt13CC[
    (offsetof(EnemyRuntimeView, itemDropType) == 0x13cc) ? 1 : -1];
typedef char EnemyRuntimeItemDropCountsAt13D0[
    (offsetof(EnemyRuntimeView, itemDropCounts) == 0x13d0) ? 1 : -1];
typedef char EnemyRuntimeValue1400At1400[
    (offsetof(EnemyRuntimeView, value1400) == 0x1400) ? 1 : -1];
typedef char EnemyRuntimeValue1404At1404[
    (offsetof(EnemyRuntimeView, value1404) == 0x1404) ? 1 : -1];
typedef char EnemyRuntimeDeathSoundAt1408[
    (offsetof(EnemyRuntimeView, deathSoundId) == 0x1408) ? 1 : -1];
typedef char EnemyRuntimeDeathEffectAt140C[
    (offsetof(EnemyRuntimeView, deathEffectScript) == 0x140c) ? 1 : -1];
typedef char EnemyRuntimeDamageFlashAt1414[
    (offsetof(EnemyRuntimeView, damageFlashFrames) == 0x1414) ? 1 : -1];
typedef char EnemyRuntimeDamageTimerAt141C[
    (offsetof(EnemyRuntimeView, damageReductionTimer) == 0x141c) ? 1 : -1];
typedef char EnemyRuntimeCollisionTimerAt1430[
    (offsetof(EnemyRuntimeView, playerCollisionTimer) == 0x1430) ? 1 : -1];
typedef char EnemyRuntimeFlagsAt1444[
    (offsetof(EnemyRuntimeView, flags) == 0x1444) ? 1 : -1];
typedef char EnemyRuntimeManagerSlotAt1450[
    (offsetof(EnemyRuntimeView, managerSlot) == 0x1450) ? 1 : -1];
typedef char EnemyRuntimeCallbackThresholdsAt1458[
    (offsetof(EnemyRuntimeView, callbackThresholds) == 0x1458) ? 1 : -1];
typedef char EnemyRuntimeOwnerAt14D8[
    (offsetof(EnemyRuntimeView, owner) == 0x14d8) ? 1 : -1];

// Minimal variable-length instruction header proven by the TH10 ECL dispatcher
// and the generic typed-operand helpers. Names describe target-observed roles;
// the original source type and the fields not exposed here remain unknown.
struct EnemyEclInstructionView
{
    int value00;
    unsigned short opcode;
    unsigned short value06;
    unsigned short operandMask;
    unsigned char unknown00A[0x06];
    int operands[1];
};
typedef char EnemyEclInstructionOpcodeAt004[
    (offsetof(EnemyEclInstructionView, opcode) == 0x004) ? 1 : -1];
typedef char EnemyEclInstructionOperandMaskAt008[
    (offsetof(EnemyEclInstructionView, operandMask) == 0x008) ? 1 : -1];
typedef char EnemyEclInstructionOperandsAt010[
    (offsetof(EnemyEclInstructionView, operands) == 0x010) ? 1 : -1];

// The generic TH10 ECL operand helpers prove that the full Enemy embeds this
// execution context at +0x008. The active-context pointer is stored separately
// at full Enemy +0x004. +0x1014 points back to the Enemy operand-resolver
// owner, whose vtable supplies the four typed value/lvalue operations.
struct EnemyEclContextView
{
    EnemyEclContextView();

    int value00;
    EnemyEclInstructionView *currentInstruction;
    unsigned char unknown008[0x1000];
    int operandStackOffset;
    int localStorageOffset;
    int unknown1010;
    EnemyEclHostBaseView *operandResolver;
};
typedef char EnemyEclContextSizeIs1018[
    (sizeof(EnemyEclContextView) == 0x1018) ? 1 : -1];
typedef char EnemyEclContextInstructionAt004[
    (offsetof(EnemyEclContextView, currentInstruction) == 0x004) ? 1 : -1];
typedef char EnemyEclContextOperandStackAt1008[
    (offsetof(EnemyEclContextView, operandStackOffset) == 0x1008) ? 1 : -1];
typedef char EnemyEclContextLocalStorageAt100C[
    (offsetof(EnemyEclContextView, localStorageOffset) == 0x100c) ? 1 : -1];
typedef char EnemyEclContextResolverAt1014[
    (offsetof(EnemyEclContextView, operandResolver) == 0x1014) ? 1 : -1];

struct EnemyEclResourceView;
struct EnemyCallbackNodeView;

struct EnemyOwnedAllocationNodeView
{
    void *allocation;
    EnemyOwnedAllocationNodeView *next;
};
typedef char EnemyOwnedAllocationNodeViewSizeIs08[
    (sizeof(EnemyOwnedAllocationNodeView) == 0x08) ? 1 : -1];

// The six-slot target vtable at 0x0046D0D8 supplies default ECL execution and
// typed operand interfaces plus a virtual destructor. Its 0x103C-byte object
// is also the exact prefix inherited by the Enemy-specific 0x2518-byte host.
struct EnemyEclHostBaseView
{
    EnemyEclHostBaseView();
    virtual int DispatchEclInstruction();
    virtual int ReadIntOperand(int operand);
    virtual int *ResolveIntOperand(int operand);
    virtual float ReadFloatOperand(int operand);
    virtual float *ResolveFloatOperand(int operand);
    virtual ~EnemyEclHostBaseView();

    void ReleaseEclAllocations();
    void ResetEclState();

    EnemyEclContextView *activeEclContext;
    EnemyEclContextView embeddedEclContext;
    int value1020;
    unsigned char spawnLayerMask;
    unsigned char unknown1025[0x03];
    unsigned int flags1028;
    EnemyEclResourceView *scriptDatabase;
    EnemyEclContextView *eclContextMirror;
    EnemyOwnedAllocationNodeView *ownedAllocations;
    int value1038;
};
typedef char EnemyEclHostBaseViewSizeIs103C[
    (sizeof(EnemyEclHostBaseView) == 0x103c) ? 1 : -1];
typedef char EnemyEclHostBaseActiveContextAt004[
    (offsetof(EnemyEclHostBaseView, activeEclContext) == 0x004) ? 1 : -1];
typedef char EnemyEclHostBaseEmbeddedContextAt008[
    (offsetof(EnemyEclHostBaseView, embeddedEclContext) == 0x008) ? 1 : -1];
typedef char EnemyEclHostBaseAllocationsAt1034[
    (offsetof(EnemyEclHostBaseView, ownedAllocations) == 0x1034) ? 1 : -1];

// TH10 allocation and constructor clearing jointly establish this full-object
// extent. The Enemy-specific host overrides all five ECL operand interfaces
// and appends the exact 0x14DC runtime tail to the base host.
struct EnemyFullObjectView : EnemyEclHostBaseView
{
    EnemyFullObjectView(const char *eclSubroutineName);
    virtual int DispatchEclInstruction();
    virtual int ReadIntOperand(int operand);
    virtual int *ResolveIntOperand(int operand);
    virtual float ReadFloatOperand(int operand);
    virtual float *ResolveFloatOperand(int operand);
    virtual ~EnemyFullObjectView();

    EnemyRuntimeView runtime;
};
typedef char EnemyFullObjectViewSizeIs2518[
    (sizeof(EnemyFullObjectView) == 0x2518) ? 1 : -1];
typedef char EnemyFullObjectActiveEclContextAt004[
    (offsetof(EnemyFullObjectView, activeEclContext) == 0x004) ? 1 : -1];
typedef char EnemyFullObjectEmbeddedEclContextAt008[
    (offsetof(EnemyFullObjectView, embeddedEclContext) == 0x008) ? 1 : -1];
typedef char EnemyFullObjectEclContextMirrorAt1030[
    (offsetof(EnemyFullObjectView, eclContextMirror) == 0x1030) ? 1 : -1];
typedef char EnemyFullObjectFlagsAt1028[
    (offsetof(EnemyFullObjectView, flags1028) == 0x1028) ? 1 : -1];
typedef char EnemyFullObjectOwnedAllocationsAt1034[
    (offsetof(EnemyFullObjectView, ownedAllocations) == 0x1034) ? 1 : -1];
typedef char EnemyFullObjectRuntimeAt103C[
    (offsetof(EnemyFullObjectView, runtime) == 0x103c) ? 1 : -1];

// The target factory allocates exactly 0x68 bytes for this manager and zeros
// all 0x1A dwords. Only fields used by the reviewed lifecycle seam are exposed.
struct EnemyManagerView
{
    EnemyManagerView();
    ~EnemyManagerView();
    int Initialize(const char *eclResourceFilename);

    unsigned int flags;
    unsigned char unknown004[0x04];
    EnemyCallbackNodeView *updateCallbackNode;
    EnemyCallbackNodeView *drawCallbackNode;
    EnemyFullObjectView *specialEnemySlots[8];
    void *effectResources[4];
    PlayerTimerView timer;
    EnemyEclResourceView *scriptDatabase;
    EnemyListNodeView *enemyListHead;
    EnemyListNodeView *enemyListTail;
    int activeEnemyCount;
    int spawnCounter;
};
typedef char EnemyManagerViewSizeIs68[
    (sizeof(EnemyManagerView) == 0x68) ? 1 : -1];
typedef char EnemyManagerTimerAt40[
    (offsetof(EnemyManagerView, timer) == 0x40) ? 1 : -1];
typedef char EnemyManagerScriptDatabaseAt54[
    (offsetof(EnemyManagerView, scriptDatabase) == 0x54) ? 1 : -1];
typedef char EnemyManagerListHeadAt58[
    (offsetof(EnemyManagerView, enemyListHead) == 0x58) ? 1 : -1];
typedef char EnemyManagerActiveCountAt60[
    (offsetof(EnemyManagerView, activeEnemyCount) == 0x60) ? 1 : -1];

// 0x40-byte spawn request consumed from the target EAX live-in at 0x40CFB0.
// Names are assigned only where the downstream TH10 consumer establishes the
// role; flag controls remain descriptive by their target bit.
struct EnemySpawnRequestView
{
    PlayerFloat3 position;
    int scoreReward;
    int itemDropType;
    int life;
    int setFlag0800;
    int setFlag40000;
    EnemyEclVariableView eclVariables;
};
typedef char EnemySpawnRequestViewSizeIs40[
    (sizeof(EnemySpawnRequestView) == 0x40) ? 1 : -1];
typedef char EnemySpawnRequestEclVariablesAt20[
    (offsetof(EnemySpawnRequestView, eclVariables) == 0x20) ? 1 : -1];

// Descriptive maintained interfaces for the reviewed lifecycle packet. Private
// target register ABIs that cannot be stated honestly in ordinary C++ are
// recorded beside each function in config/functions.csv.
EnemyFullObjectView *EnemySpawn(
    EnemyManagerView *manager,
    const char *eclSubroutineName,
    const EnemySpawnRequestView *request);
int __stdcall EnemyFinalizeDeath(EnemyFullObjectView *enemy);
int EnemyManagerUpdate(EnemyManagerView *manager);
int __fastcall EnemyManagerUpdateCallback(EnemyManagerView *manager);
int __fastcall EnemyManagerDrawCallback(EnemyManagerView *manager);

// Descriptive maintained interfaces for the reviewed manager/resource lifetime
// cohort. The target uses private EBX/EAX/ESI receiver live-ins for several
// bodies; those machine contracts are recorded in the function ledger rather
// than falsely expressed as ordinary source calling conventions here.
EnemyManagerView * __stdcall EnemyManagerCreate(
    const char *eclResourceFilename);

// Descriptive maintained name for the reviewed 0x0040DC80-0x0040E5EB owner.
// The target boundary is one stack EnemyRuntimeView* argument with RET 4. The
// original source identifier, member/free-function spelling and TU are unknown.
int __stdcall EnemyRuntimeUpdate(EnemyRuntimeView *enemy);

#endif
