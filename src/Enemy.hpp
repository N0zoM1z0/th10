#ifndef TH10_RECONSTRUCTION_ENEMY_HPP
#define TH10_RECONSTRUCTION_ENEMY_HPP

#include "Player.hpp"
#include "AnmManager.hpp" // Defines the embedded AnmFloat3View layout.
#include "EclVm.hpp"

#include <stddef.h>

struct EnemyFullObjectView;
struct EnemyEclHostBaseView;
struct EnemyListNodeView;
struct EnemyManagerView;
struct AnmLoadedView;

extern float g_AnmGameSpeed;

struct EnemyFloat2
{
    EnemyFloat2() {}
    EnemyFloat2(float x, float y) : x(x), y(y) {}

    EnemyFloat2 operator-(const EnemyFloat2 &other) const
    {
        return EnemyFloat2(x - other.x, y - other.y);
    }
    EnemyFloat2 operator+(const EnemyFloat2 &other) const
    {
        return EnemyFloat2(x + other.x, y + other.y);
    }
    EnemyFloat2 operator*(float scale) const
    {
        return EnemyFloat2(x * scale, y * scale);
    }

    float x;
    float y;
};

typedef char EnemyFloat2SizeIs08[(sizeof(EnemyFloat2) == 0x08) ? 1 : -1];

// ECL/ANM position transfers copy the three raw 32-bit scalar words,
// including IEEE-754 sign and NaN payload bits, without applying any
// arithmetic or implicit float conversion. This is only a storage overlay.
struct EnemyPositionDwordView
{
    unsigned int word[3];
};
typedef char EnemyPositionDwordLayoutIs0C[
    (sizeof(EnemyPositionDwordView) == 0x0c &&
     sizeof(PlayerFloat3) == 0x0c &&
     sizeof(AnmFloat3View) == 0x0c &&
     offsetof(EnemyPositionDwordView, word) == 0 &&
     sizeof(((EnemyPositionDwordView *)0)->word[0]) == 4) ? 1 : -1];

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
    union
    {
        float value20;
        unsigned int value20Bits;
    };
    union
    {
        float value24;
        unsigned int value24Bits;
    };
    unsigned int flags;
};
typedef char EnemyMotionViewSizeIs2C[(sizeof(EnemyMotionView) == 0x2c) ? 1 : -1];
typedef char EnemyMotionVelocityAt0C[(offsetof(EnemyMotionView, velocity) == 0x0c) ? 1 : -1];
typedef char EnemyMotionFlagsAt28[(offsetof(EnemyMotionView, flags) == 0x28) ? 1 : -1];
// Circle interpolation copies the raw binary32 words at +0x20 and +0x24
// through the VM without x87 round-trips.
typedef char EnemyMotionCircleScalarBitOffsets[
    (offsetof(EnemyMotionView, value20) == 0x20 &&
     offsetof(EnemyMotionView, value20Bits) == 0x20 &&
     offsetof(EnemyMotionView, value24) == 0x24 &&
     offsetof(EnemyMotionView, value24Bits) == 0x24) ? 1 : -1];

struct EnemyFloat2NativeBitView
{
    unsigned int words[2];
};
typedef char EnemyFloat2NativeBitViewIs08[
    (sizeof(EnemyFloat2NativeBitView) == sizeof(EnemyFloat2) &&
     sizeof(EnemyFloat2NativeBitView) == 0x08) ? 1 : -1];

// Target 0x44C5D0 is shared by Enemy and Player shot motion. The original
// vector class name and source owner remain unknown.
struct EnemyRuntimeVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
};
typedef char EnemyRuntimeVectorViewSizeIs0C[
    (sizeof(EnemyRuntimeVectorView) == 0x0c) ? 1 : -1];

void EnemyAdvanceMotion(EnemyMotionView *motion);

// TH10's Enemy interpolation records use the same four-value plus timer
// layout observed independently in their evaluator and initializer bodies.
// The target runtime default constructor clears bit zero in each embedded timer,
// so the natural timer constructor preserves that compiler-generated aggregate
// construction path.
struct EnemyInterpolationTimerView
{
    EnemyInterpolationTimerView() { flags &= ~1u; }
    void Tick()
    {
        previous = current;
        if (*scale > 0.99f && *scale < 1.01f)
        {
            // Original i386 INC stores the wrapped low 32-bit counter word.
            current = static_cast<int>(static_cast<unsigned int>(current) + 1u);
            subframe += 1.0f;
        }
        else
        {
            subframe = *scale + subframe;
            current = static_cast<int>(subframe);
        }
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
        // Native DEC preserves the Win32 counter word even at INT_MIN.
        previous = static_cast<int>(static_cast<unsigned int>(value) - 1u);
    }

    int previous;
    int current;
    float subframe;
    float *scale;
    unsigned int flags;
};
typedef char EnemyInterpolationTimerSizeIs14[
    (sizeof(EnemyInterpolationTimerView) == 0x14) ? 1 : -1];

struct EnemyPositionInterpolationView
{
    PlayerFloat3 initial;
    PlayerFloat3 final;
    PlayerFloat3 initialTangent;
    PlayerFloat3 finalTangent;
    EnemyInterpolationTimerView timer;
    int duration;
    int mode;
};
typedef char EnemyPositionInterpolationSizeIs4C[
    (sizeof(EnemyPositionInterpolationView) == 0x4c) ? 1 : -1];
typedef char EnemyPositionInterpolationTimerAt30[
    (offsetof(EnemyPositionInterpolationView, timer) == 0x30) ? 1 : -1];
typedef char EnemyPositionInterpolationDurationAt44[
    (offsetof(EnemyPositionInterpolationView, duration) == 0x44) ? 1 : -1];

struct EnemyScalarInterpolationView
{
    EnemyFloat2 *Evaluate(EnemyFloat2 *output);

    EnemyFloat2 initial;
    EnemyFloat2 final;
    EnemyFloat2 initialTangent;
    EnemyFloat2 finalTangent;
    EnemyInterpolationTimerView timer;
    int duration;
    int mode;
};
typedef char EnemyScalarInterpolationSizeIs3C[
    (sizeof(EnemyScalarInterpolationView) == 0x3c) ? 1 : -1];
typedef char EnemyScalarInterpolationTimerAt20[
    (offsetof(EnemyScalarInterpolationView, timer) == 0x20) ? 1 : -1];
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

// The eight target bullet-pattern records occupy runtime +0x2C4..+0x1343.
// Their constructor clears all 0x210 bytes and then writes -1 at +0x204.
// The dispatcher independently proves the 0x210 stride.
// Opcode store offsets are independently decoded from the retail dispatcher.
// Field names express observed ECL roles, not historical C++ identifiers.
struct EnemyBulletPatternFieldsView
{
    unsigned short spriteId;       // +0x000, SET_BULLET_SPRITES(1)
    unsigned short spriteVariant;  // +0x002, SET_BULLET_SPRITES(2)
    float spawnPositionX;         // +0x004, FIRE_BULLET_PATTERN
    float spawnPositionY;         // +0x008
    float spawnPositionZ;         // +0x00c
    union
    {
        float angleA;
        unsigned int angleABits;
    };                  // +0x010, SET_BULLET_ANGLE(1)
    float angleB;                  // +0x014, SET_BULLET_ANGLE(2)
    union
    {
        float speedA;
        unsigned int speedABits;
    };                  // +0x018
    float speedB;                  // +0x01c
    unsigned char unknown020[0x1f4 - 0x020];
    unsigned short countA;        // +0x1f4, SET_BULLET_COUNT(1)
    unsigned short countB;        // +0x1f6, SET_BULLET_COUNT(2)
    unsigned short aimMode;       // +0x1f8, SET_BULLET_AIM_MODE
    unsigned char unknown1FA[2];
    unsigned int flags1FC;        // +0x1fc, INITIALIZE_BULLET_PATTERN
    int soundId;                  // +0x200, SET_BULLET_SOUND(1)
    int soundVariant;             // +0x204, SET_BULLET_SOUND(2)
    unsigned char unknown208[0x210 - 0x208];
};
typedef char EnemyBulletPatternFieldsViewSizeIs210[
    (sizeof(EnemyBulletPatternFieldsView) == 0x210) ? 1 : -1];
typedef char EnemyBulletPatternHeaderOffsets[
    (offsetof(EnemyBulletPatternFieldsView, spriteId) == 0 &&
     offsetof(EnemyBulletPatternFieldsView, spriteVariant) == 2 &&
     offsetof(EnemyBulletPatternFieldsView, spawnPositionX) == 4 &&
     offsetof(EnemyBulletPatternFieldsView, spawnPositionY) == 8 &&
     offsetof(EnemyBulletPatternFieldsView, spawnPositionZ) == 0x0c &&
     offsetof(EnemyBulletPatternFieldsView, angleA) == 0x10 &&
     offsetof(EnemyBulletPatternFieldsView, angleABits) == 0x10 &&
     offsetof(EnemyBulletPatternFieldsView, angleB) == 0x14 &&
     offsetof(EnemyBulletPatternFieldsView, speedA) == 0x18 &&
     offsetof(EnemyBulletPatternFieldsView, speedABits) == 0x18 &&
     offsetof(EnemyBulletPatternFieldsView, speedB) == 0x1c) ? 1 : -1];
typedef char EnemyBulletPatternTailOffsets[
    (offsetof(EnemyBulletPatternFieldsView, countA) == 0x1f4 &&
     offsetof(EnemyBulletPatternFieldsView, countB) == 0x1f6 &&
     offsetof(EnemyBulletPatternFieldsView, aimMode) == 0x1f8 &&
     offsetof(EnemyBulletPatternFieldsView, flags1FC) == 0x1fc &&
     offsetof(EnemyBulletPatternFieldsView, soundId) == 0x200 &&
     offsetof(EnemyBulletPatternFieldsView, soundVariant) == 0x204) ? 1 : -1];

struct EnemyBulletPatternView
{
    EnemyBulletPatternView();

    union
    {
        unsigned int words[0x84];
        EnemyBulletPatternFieldsView fields;
    };
};
typedef char EnemyBulletPatternViewSizeIs210[
    (sizeof(EnemyBulletPatternView) == 0x210) ? 1 : -1];

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

// Shared original 0x3C-byte item-drop packet. Enemy construction initializes
// both ellipse radii; ECL opcodes configure counts and area; the item-scatter
// core consumes the same address. Opaque word +0x30 remains unexplained.
struct EnemyItemDropCountRadiusView
{
    int mainItemType;
    int countsByKind[11];
    int unknown030;
    float radiusX;
    float radiusY;
};
typedef char EnemyItemDropCountRadiusLayout[
    (sizeof(EnemyItemDropCountRadiusView) == 0x3c &&
     offsetof(EnemyItemDropCountRadiusView, countsByKind) == 0x04 &&
     offsetof(EnemyItemDropCountRadiusView, radiusX) == 0x34 &&
     offsetof(EnemyItemDropCountRadiusView, radiusY) == 0x38) ? 1 : -1];

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
    EnemyPositionInterpolationView positionInterpolation0;
    EnemyPositionInterpolationView positionInterpolation1;
    EnemyScalarInterpolationView scalarInterpolation0;
    EnemyScalarInterpolationView scalarInterpolation1;
    EnemyScalarInterpolationView scalarInterpolation2;
    EnemyScalarInterpolationView scalarInterpolation3;
    EnemyBulletPatternView bulletPatterns[8];
    PlayerFloat3 bulletPatternOffsets[8];
    float visibilityExtentX;
    float visibilityExtentY;
    float movementBoundsCenterX;
    float movementBoundsCenterY;
    float movementBoundsSizeX;
    float movementBoundsSizeY;
    int scoreReward;
    int life;
    // Set with life; opcode 0x15B divides gauge markers by this baseline.
    int lifeGaugeReference;
    // Updated with (life - next callback threshold); before threshold setup
    // the ECL SET_LIFE path initializes this to the full current life.
    int lifeToNextCallback;
    // ECL opcodes and lifecycle use typed packet members, while the native
    // ADD_ITEM_DROP opcode writes a raw indexed DWORD into the same packet.
    // Both views start at runtime +0x13CC; the indexed opcode does not clamp.
    union
    {
        EnemyItemDropCountRadiusView itemDrops;
        int itemDropRawWords[sizeof(EnemyItemDropCountRadiusView) / sizeof(int)];
    };
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
    // ECL opcode 0x15A stores the squared protection radius here as binary32.
    float bulletProtectionRadiusSquared;
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
    (offsetof(EnemyRuntimeView, positionInterpolation0) == 0x13c &&
     offsetof(EnemyRuntimeView, positionInterpolation1) == 0x188) ? 1 : -1];
typedef char EnemyRuntimeScalarInterpolationAt1D4[
    (offsetof(EnemyRuntimeView, scalarInterpolation0) == 0x1d4 &&
     offsetof(EnemyRuntimeView, scalarInterpolation1) == 0x210 &&
     offsetof(EnemyRuntimeView, scalarInterpolation2) == 0x24c &&
     offsetof(EnemyRuntimeView, scalarInterpolation3) == 0x288) ? 1 : -1];
typedef char EnemyRuntimeBulletPatternsAt2C4[
    (offsetof(EnemyRuntimeView, bulletPatterns) == 0x2c4) ? 1 : -1];
typedef char EnemyRuntimeBulletPatternOffsetsAt1344[
    (offsetof(EnemyRuntimeView, bulletPatternOffsets) == 0x1344) ? 1 : -1];
typedef char EnemyRuntimeVisibilityExtentAt13A4[
    (offsetof(EnemyRuntimeView, visibilityExtentX) == 0x13a4) ? 1 : -1];
typedef char EnemyRuntimeScoreAt13BC[
    (offsetof(EnemyRuntimeView, scoreReward) == 0x13bc) ? 1 : -1];
typedef char EnemyRuntimeLifeAt13C0[
    (offsetof(EnemyRuntimeView, life) == 0x13c0) ? 1 : -1];
typedef char EnemyRuntimeLifeGaugeReferenceAt13C4[
    (offsetof(EnemyRuntimeView, lifeGaugeReference) == 0x13c4) ? 1 : -1];
typedef char EnemyRuntimeNextCallbackLifeAt13C8[
    (offsetof(EnemyRuntimeView, lifeToNextCallback) == 0x13c8) ? 1 : -1];
typedef char EnemyRuntimeItemDropPacketAt13CC[
    (offsetof(EnemyRuntimeView, itemDrops) == 0x13cc &&
     offsetof(EnemyRuntimeView, itemDropRawWords) == 0x13cc &&
     sizeof(EnemyItemDropCountRadiusView) == 0x3c &&
     sizeof(((EnemyRuntimeView *)0)->itemDropRawWords) == 0x3c) ? 1 : -1];
typedef char EnemyRuntimeItemDropCountsAt13D0[
    (offsetof(EnemyRuntimeView, itemDrops) +
     offsetof(EnemyItemDropCountRadiusView, countsByKind) == 0x13d0) ? 1 : -1];
typedef char EnemyRuntimeItemDropRadiiAt1400[
    (offsetof(EnemyRuntimeView, itemDrops) +
     offsetof(EnemyItemDropCountRadiusView, radiusX) == 0x1400 &&
     offsetof(EnemyRuntimeView, itemDrops) +
     offsetof(EnemyItemDropCountRadiusView, radiusY) == 0x1404) ? 1 : -1];
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
typedef char EnemyRuntimeBulletProtectionAt1454[
    (offsetof(EnemyRuntimeView, bulletProtectionRadiusSquared) == 0x1454) ? 1 : -1];
typedef char EnemyRuntimeCallbackThresholdsAt1458[
    (offsetof(EnemyRuntimeView, callbackThresholds) == 0x1458) ? 1 : -1];
typedef char EnemyRuntimeOwnerAt14D8[
    (offsetof(EnemyRuntimeView, owner) == 0x14d8) ? 1 : -1];

// The five START_SPELL opcodes share four encoded 32-bit arguments,
// followed immediately by an encrypted byte string. The instruction's
// own length remains the only record-length authority: this is an
// overlapping wire prefix, not a fixed-size name array.
struct EnemyEclSpellInlineOperandsView
{
    int spellId;
    int argument1;
    int argument2;
    int encryptedNameLength;
    unsigned char encryptedName[1];
};
typedef char EnemyEclSpellInlineOperandsOffsets[
    (offsetof(EnemyEclSpellInlineOperandsView, encryptedNameLength) == 0x0c &&
     offsetof(EnemyEclSpellInlineOperandsView, encryptedName) == 0x10) ? 1 : -1];

// SET_INTERRUPT stores its callback's inline name after four operand
// dwords; SET_TIMEOUT's name follows two. The intervening words have not
// been fully classified, so the layouts stop at their observed byte starts.
struct EnemyEclInterruptInlineOperandsView
{
    int callbackSlot;
    int lifeThreshold;
    int timerThreshold;
    int unknownWord3;
    unsigned char callbackName[1];
};
struct EnemyEclTimeoutInlineOperandsView
{
    int callbackSlot;
    int unknownWord1;
    unsigned char callbackName[1];
};
typedef char EnemyEclCallbackInlineNameOffsets[
    (offsetof(EnemyEclInterruptInlineOperandsView, callbackName) == 0x10 &&
     offsetof(EnemyEclTimeoutInlineOperandsView, callbackName) == 0x08) ? 1 : -1];

// All CREATE_ENEMY variants encode a byte count at payload +0 and the
// inline ECL subroutine name at +4. The subsequent scalar operand words
// are addressed dynamically using (name byte count + 4) / sizeof(int),
// not by applying sizeof this C++ view to the serialized byte stream.
struct EnemyEclSpawnInlineOperandsView
{
    int inlineNameByteCount;
    unsigned char subroutineName[1];
};
typedef char EnemyEclSpawnInlineNameOffset[
    (offsetof(EnemyEclSpawnInlineOperandsView, inlineNameByteCount) == 0 &&
     offsetof(EnemyEclSpawnInlineOperandsView, subroutineName) == 4) ? 1 : -1];

// Minimal variable-length instruction header proven by the TH10 ECL dispatcher
// and the generic typed-operand helpers. Names describe target-observed roles;
// the original source type and the fields not exposed here remain unknown.
struct EnemyEclInstructionView
{
    // This is the very same variable-length wire instruction consumed by
    // EclVmContext::Run; the Enemy dispatcher only needs its extended opcodes.
    int time;
    unsigned short opcode;
    unsigned short size;
    union
    {
        unsigned short operandMask;
        EclVmOperandIndirectMaskView indirectOperands;
    };
    unsigned char difficultyMask;
    unsigned char operandCount;
    unsigned char unknown00C[4];
    // ECL operand words are shared between integer and IEEE-754 access.
    // The actual instruction size remains variable and encoded separately.
    union
    {
        int operands[1];
        float floatOperands[1];
        // The counted inline-name tail contains the same binary32/INT32
        // four-byte words already used by the generic ECL scalar reader.
        EclVmScalar scalarOperands[1];
        EnemyEclSpawnInlineOperandsView spawnOperands;
        EnemyEclSpellInlineOperandsView spellOperands;
        EnemyEclInterruptInlineOperandsView interruptOperands;
        EnemyEclTimeoutInlineOperandsView timeoutOperands;
    };
};
typedef char EnemyEclInstructionOpcodeAt004[
    (offsetof(EnemyEclInstructionView, opcode) == 0x004) ? 1 : -1];
// Cross-check the independently reconstructed generic VM instruction.
// These are byte-stream offsets, not a fixed-size script allocation.
typedef char EnemyEclInstructionHeaderMatchesVm[
    (offsetof(EnemyEclInstructionView, time) ==
         offsetof(EclVmInstruction, time) &&
     offsetof(EnemyEclInstructionView, opcode) ==
         offsetof(EclVmInstruction, opcode) &&
     offsetof(EnemyEclInstructionView, size) ==
         offsetof(EclVmInstruction, size) &&
     offsetof(EnemyEclInstructionView, operandMask) ==
         offsetof(EclVmInstruction, operandFlags) &&
     offsetof(EnemyEclInstructionView, indirectOperands) ==
         offsetof(EclVmInstruction, operandMask) &&
     offsetof(EnemyEclInstructionView, difficultyMask) ==
         offsetof(EclVmInstruction, difficultyMask) &&
     offsetof(EnemyEclInstructionView, operandCount) ==
         offsetof(EclVmInstruction, operandCount) &&
     offsetof(EnemyEclInstructionView, operands) ==
         offsetof(EclVmInstruction, integerOperands) &&
     sizeof(((EnemyEclInstructionView *)0)->indirectOperands) == 2) ? 1 : -1];
typedef char EnemyEclInstructionInlineCallbackNameOffsets[
    (offsetof(EnemyEclInstructionView, interruptOperands) +
         offsetof(EnemyEclInterruptInlineOperandsView, callbackName) == 0x020 &&
     offsetof(EnemyEclInstructionView, timeoutOperands) +
         offsetof(EnemyEclTimeoutInlineOperandsView, callbackName) == 0x018) ? 1 : -1];
typedef char EnemyEclInstructionInlineSpawnNameOffsets[
    (offsetof(EnemyEclInstructionView, spawnOperands) == 0x010 &&
     offsetof(EnemyEclInstructionView, spawnOperands) +
       offsetof(EnemyEclSpawnInlineOperandsView, subroutineName) == 0x014) ? 1 : -1];
typedef char EnemyEclInstructionOperandMaskAt008[
    (offsetof(EnemyEclInstructionView, operandMask) == 0x008 &&
     offsetof(EnemyEclInstructionView, indirectOperands) == 0x008) ? 1 : -1];
typedef char EnemyEclInstructionOperandsAt010[
    (offsetof(EnemyEclInstructionView, operands) == 0x010 &&
     offsetof(EnemyEclInstructionView, floatOperands) == 0x010 &&
     offsetof(EnemyEclInstructionView, scalarOperands) == 0x010 &&
     offsetof(EnemyEclInstructionView, scalarOperands) ==
         offsetof(EclVmInstruction, scalarOperands) &&
     sizeof(((EnemyEclInstructionView *)0)->scalarOperands[0]) == 4 &&
     offsetof(EnemyEclInstructionView, spellOperands) == 0x010 &&
     offsetof(EnemyEclInstructionView, spellOperands) +
         offsetof(EnemyEclSpellInlineOperandsView, encryptedNameLength) == 0x01c &&
     offsetof(EnemyEclInstructionView, spellOperands) +
         offsetof(EnemyEclSpellInlineOperandsView, encryptedName) == 0x020) ? 1 : -1];

// The generic TH10 ECL operand helpers prove that the full Enemy embeds this
// execution context at +0x008. The active-context pointer is stored separately
// at full Enemy +0x004. +0x1014 points back to the Enemy operand-resolver
// owner, whose vtable supplies the four typed value/lvalue operations.
struct EnemyEclContextView
{
    EnemyEclContextView();

    // TH10 generic ECL Run interprets this first word as binary32 time;
    // existing reset paths write integer zero to the identical four bytes.
    union
    {
        int value00;
        float currentTime;
    };
    EnemyEclInstructionView *currentInstruction;
    unsigned char unknown008[0x1000];
    int operandStackOffset;
    int localStorageOffset;
    int unknown1010;
    EnemyEclHostBaseView *operandResolver;
};
typedef char EnemyEclContextSizeIs1018[
    (sizeof(EnemyEclContextView) == 0x1018) ? 1 : -1];
typedef char EnemyEclContextTimeAt000[
    (offsetof(EnemyEclContextView, value00) == 0 &&
     offsetof(EnemyEclContextView, currentTime) == 0) ? 1 : -1];
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
    int SetSubroutine(const char *name);

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
    __declspec(noinline) void SuspendCallbacks();

    unsigned int flags;
    unsigned char unknown004[0x04];
    EnemyCallbackNodeView *updateCallbackNode;
    EnemyCallbackNodeView *drawCallbackNode;
    EnemyFullObjectView *specialEnemySlots[8];
    // The four 32-bit resource slots are retained for all existing ECL
    // loaders. Their third slot is independently used as a stage ANM.
    union
    {
        void *effectResources[4];
        // Original dynamic 0..3 resource selectors feed ANM VM creators;
        // keep the raw alias for the package loader and other writers.
        AnmLoadedView *loadedAnmResources[4];
        struct
        {
            void *unknownResource0;
            void *unknownResource1;
            AnmLoadedView *stageAnimationAnm;
            void *unknownResource3;
        } typedEffectResources;
    };
    PlayerTimerView timer;
    EnemyEclResourceView *scriptDatabase;
    EnemyListNodeView *enemyListHead;
    EnemyListNodeView *enemyListTail;
    int activeEnemyCount;
    int spawnCounter;
};
typedef char EnemyManagerViewSizeIs68[
    (sizeof(EnemyManagerView) == 0x68) ? 1 : -1];
typedef char EnemyManagerStageAnimationAt38[
    (offsetof(EnemyManagerView, effectResources) == 0x30 &&
     offsetof(EnemyManagerView, loadedAnmResources) == 0x30 &&
     offsetof(EnemyManagerView, loadedAnmResources) +
         2 * sizeof(AnmLoadedView *) == 0x38 &&
     offsetof(EnemyManagerView, typedEffectResources.stageAnimationAnm) == 0x38 &&
     offsetof(EnemyManagerView, timer) == 0x40) ? 1 : -1];
typedef char EnemyManagerTimerAt40[
    (offsetof(EnemyManagerView, timer) == 0x40) ? 1 : -1];
typedef char EnemyManagerScriptDatabaseAt54[
    (offsetof(EnemyManagerView, scriptDatabase) == 0x54) ? 1 : -1];
typedef char EnemyManagerListHeadAt58[
    (offsetof(EnemyManagerView, enemyListHead) == 0x58) ? 1 : -1];
typedef char EnemyManagerActiveCountAt60[
    (offsetof(EnemyManagerView, activeEnemyCount) == 0x60) ? 1 : -1];

// Maintained stack-bound interface for the complete 0x409F90 clear operation.
// Target callers push the manager and the callee returns with RET 4.
void __stdcall EnemyManagerClear(EnemyManagerView *manager);

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

// Descriptive maintained interfaces for the reviewed lifecycle packet. The
// __stdcall declaration records callee cleanup; the target's LTCG promotion of
// the request into EAX remains recorded beside the function in the ledger.
EnemyFullObjectView *__stdcall EnemySpawn(
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
