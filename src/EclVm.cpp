#include "EclVm.hpp"

// Interpreter source informed by th10-decomp (MIT), commit db6797d245afd2e58b2cd31b60d2857499d3aa7c.
// See docs/licenses/th10-decomp-MIT.txt; target/native evidence remains authoritative.

#include <math.h>
#include <stdlib.h>
#include <string.h>


// This runtime service is a separate target function in the
// 0x0044DF70-0x004506D0 corridor. Its declaration lets this file retain the
// complete VM dispatch while its implementation remains a separate unit.
extern float __stdcall EnemyWrapAngle(float angle);

struct EclVmPolarVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
};

typedef char EclVmPolarVectorViewSizeIs0C[
    (sizeof(EclVmPolarVectorView) == 0x0c) ? 1 : -1];

void EclVmPolarVectorView::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp [eax]
        fmul magnitude
        fstp [eax + 4]
    }
#else
    x = static_cast<float>(cos(angle)) * magnitude;
    y = static_cast<float>(sin(angle)) * magnitude;
#endif
}


int EclVmStackView::Push(
    unsigned char type, int size, const void *value)
{
    const unsigned int nextStackTopBits =
        static_cast<unsigned int>(stackTop) + static_cast<unsigned int>(size);
    // VC7.1 i386 preserves the low 32-bit signed pattern on conversion.
    // That conversion is compiler-specific but avoids a mismatched int alias.
    const int nextStackTop = static_cast<int>(nextStackTopBits);
    if (nextStackTop >= 0x1000)
        return -1;

    // The native stack type slot and value write are addressed as Win32
    // low 32-bit words, even outside an abstract C++ array extent.
    if (type != 0)
    {
        reinterpret_cast<EclVmStackTypeSlotView *>(
            reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(data) +
                static_cast<unsigned int>(stackTop)))->type = type;
        // Publishing the native type-slot advance copies the four word
        // bits; an out-of-range unsigned-to-signed cast is not required.
        const unsigned int typeTopBits = static_cast<unsigned int>(stackTop) +
            static_cast<unsigned int>(sizeof(EclVmStackTypeSlotView));
        memcpy(&stackTop, &typeTopBits, sizeof(stackTop));
    }

    memcpy(reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(data) +
        static_cast<unsigned int>(stackTop)), value, size);
    // The serialized value size can wrap at the native 32-bit boundary.
    // Preserve the observed bit pattern when publishing the new stack top.
    const unsigned int valueTopBits = static_cast<unsigned int>(stackTop) +
        static_cast<unsigned int>(size);
    memcpy(&stackTop, &valueTopBits, sizeof(stackTop));
    return 0;
}

int EclVmStackView::Pop(unsigned char type, int size, void *value)
{
    if (stackTop - size < 0)
        return -1;

    unsigned char *const stackData = data;
    stackTop -= size;
    unsigned char *const valueAddress = data + stackTop;
    memcpy(value, valueAddress, size);

    if (type != 0)
    {
        stackTop -= static_cast<int>(sizeof(EclVmStackTypeSlotView));
        const EclVmStackTypeSlotView *const typeAddress =
            reinterpret_cast<const EclVmStackTypeSlotView *>(
                stackData + stackTop);
        const unsigned char storedType = typeAddress->type;
        if (storedType != 'f')
        {
            if (storedType == 'i' && type == 'f')
            {
                reinterpret_cast<EclVmScalar *>(value)->real = static_cast<float>(
                    reinterpret_cast<EclVmScalar *>(value)->integer);
            }
        }
        else if (type == 'i')
        {
            reinterpret_cast<EclVmScalar *>(value)->integer = static_cast<int>(
                reinterpret_cast<EclVmScalar *>(value)->real);
        }
    }

    return 0;
}

int EclVmStackView::EnterFrame(int localBytes)
{
    // Retail uses native 32-bit stack base/top words for the saved
    // frame slot and the four-byte counter advance.
    const int previousTop = stackTop;
    // The serialized stack top is an i386 word. Preserve the exact signed
    // bit pattern after modulo-32 addition without converting an out-of-range
    // unsigned result to int (which is implementation-defined in C++).
    const unsigned int nativeBits =
        static_cast<unsigned int>(previousTop) +
        static_cast<unsigned int>(localBytes);
    int localTop;
    memcpy(&localTop, &nativeBits, sizeof(localTop));
    if (localTop >= 0x1000)
        return -1;

    stackTop = localTop;
    if (static_cast<int>(static_cast<unsigned int>(stackTop) +
        static_cast<unsigned int>(sizeof(EclVmStackSavedWordView))) < 0x1000)
    {
        reinterpret_cast<EclVmStackSavedWordView *>(
            reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(data) +
                static_cast<unsigned int>(stackTop)))->value = frameBase;
        // Frame-base records advance the stack top as a native dword;
        // retain the bit pattern even across the signed boundary.
        const unsigned int storedTopBits = static_cast<unsigned int>(stackTop) +
            static_cast<unsigned int>(sizeof(EclVmStackSavedWordView));
        memcpy(&stackTop, &storedTopBits, sizeof(stackTop));
    }
    frameBase = previousTop;
    return 0;
}

int EclVmStackView::LeaveFrame()
{
    const int previousFrame = frameBase;
    if (static_cast<int>(static_cast<unsigned int>(stackTop) -
        static_cast<unsigned int>(sizeof(EclVmStackSavedWordView))) >= 0)
    {
        stackTop = static_cast<int>(static_cast<unsigned int>(stackTop) -
            static_cast<unsigned int>(sizeof(EclVmStackSavedWordView)));
        frameBase = reinterpret_cast<const EclVmStackSavedWordView *>(
            reinterpret_cast<const unsigned char *>(
                reinterpret_cast<unsigned int>(data) +
                static_cast<unsigned int>(stackTop)))->value;
    }
    stackTop = previousFrame;
    return 0;
}


// Both thread-control paths invalidate the same instruction cursor in the
// context owned by a thread-list node. Keep the node and its context typed.
static __forceinline void EclStopThreadContext(EclVmContext *stoppedContext)
{
    stoppedContext->instruction = NULL;
}

// The target selector table at 0x0044FCB4 covers opcodes 0x00-0x57.  Values
// omitted here route to the host's extended dispatcher.
enum EclVmOpcode
{
    ECL_VM_NOP = 0x00,
    ECL_VM_TERMINATE = 0x01,
    ECL_VM_RETURN = 0x0A,
    ECL_VM_CALL = 0x0B,
    ECL_VM_JUMP = 0x0C,
    ECL_VM_JUMP_IF_FALSE = 0x0D,
    ECL_VM_JUMP_IF_TRUE = 0x0E,
    ECL_VM_SPAWN_THREAD = 0x0F,
    ECL_VM_SPAWN_THREAD_WITH_ID = 0x10,
    ECL_VM_STOP_THREAD = 0x11,
    ECL_VM_SET_THREAD_FLAG = 0x12,
    ECL_VM_CLEAR_THREAD_FLAG = 0x13,
    ECL_VM_SET_THREAD_CONTROL = 0x14,
    ECL_VM_STOP_ALL_THREADS = 0x15,
    ECL_VM_EVALUATE_FORMAT_OPERANDS = 0x1E,
    ECL_VM_ENTER_FRAME = 0x28,
    ECL_VM_LEAVE_FRAME = 0x29,
    ECL_VM_PUSH_INT = 0x2A,
    ECL_VM_STORE_INT = 0x2B,
    ECL_VM_PUSH_FLOAT = 0x2C,
    ECL_VM_STORE_FLOAT = 0x2D,
    ECL_VM_ADD_INT = 0x32,
    ECL_VM_ADD_FLOAT = 0x33,
    ECL_VM_SUBTRACT_INT = 0x34,
    ECL_VM_SUBTRACT_FLOAT = 0x35,
    ECL_VM_MULTIPLY_INT = 0x36,
    ECL_VM_MULTIPLY_FLOAT = 0x37,
    ECL_VM_DIVIDE_INT = 0x38,
    ECL_VM_DIVIDE_FLOAT = 0x39,
    ECL_VM_MODULO_INT = 0x3A,
    ECL_VM_EQUAL_INT = 0x3B,
    ECL_VM_EQUAL_FLOAT = 0x3C,
    ECL_VM_NOT_EQUAL_INT = 0x3D,
    ECL_VM_NOT_EQUAL_FLOAT = 0x3E,
    ECL_VM_LESS_INT = 0x3F,
    ECL_VM_LESS_FLOAT = 0x40,
    ECL_VM_LESS_EQUAL_INT = 0x41,
    ECL_VM_LESS_EQUAL_FLOAT = 0x42,
    ECL_VM_GREATER_INT = 0x43,
    ECL_VM_GREATER_FLOAT = 0x44,
    ECL_VM_GREATER_EQUAL_INT = 0x45,
    ECL_VM_GREATER_EQUAL_FLOAT = 0x46,
    ECL_VM_NOT_INT = 0x47,
    ECL_VM_NOT_FLOAT = 0x48,
    ECL_VM_LOGICAL_OR = 0x49,
    ECL_VM_LOGICAL_AND = 0x4A,
    ECL_VM_BITWISE_XOR = 0x4B,
    ECL_VM_BITWISE_OR = 0x4C,
    ECL_VM_BITWISE_AND = 0x4D,
    ECL_VM_POST_DECREMENT_INT = 0x4E,
    ECL_VM_SINE = 0x4F,
    ECL_VM_COSINE = 0x50,
    ECL_VM_POLAR_TO_CARTESIAN = 0x51,
    ECL_VM_NORMALIZE_ANGLE = 0x52,
    ECL_VM_SUBTRACT_TIME = 0x53,
    ECL_VM_NEGATE_INT = 0x54,
    ECL_VM_NEGATE_FLOAT_STORAGE = 0x55,
    ECL_VM_LENGTH_SQUARED = 0x56,
    ECL_VM_POINT_ANGLE = 0x57,
};

namespace
{

// The common four-byte scalar storage is declared in EclVm.hpp.

// Each ECL instruction is a variable-length byte stream. The one-element
// scalarOperands declaration locates its first word, not the length of an
// allocated array. Native i386 computes every indexed word address modulo
// 2^32 even for script-provided indices past that initial declaration.
static const EclVmScalar *OperandWord(
    const EclVmInstruction *instruction, unsigned int index)
{
    return reinterpret_cast<const EclVmScalar *>(
        reinterpret_cast<unsigned int>(instruction) +
        static_cast<unsigned int>(offsetof(EclVmInstruction, scalarOperands)) +
        index * static_cast<unsigned int>(sizeof(EclVmScalar)));
}

static int OperandInt(const EclVmInstruction *instruction, unsigned int index)
{
    return OperandWord(instruction, index)->integer;
}

static float OperandFloat(
    const EclVmInstruction *instruction, unsigned int index)
{
    return OperandWord(instruction, index)->real;
}

static bool IsOperandIndirect(
    const EclVmInstruction *instruction, unsigned int index)
{
    return (instruction->operandMask.bits & (1U << index)) != 0;
}

// The interpreter expands this integer wrapper and retains the
// call to EclVmStackView::Push.
#pragma inline_depth(1)
static __forceinline int PushInt(EclVmContext *context, const int &value)
{
    return context->stack.Push('i', sizeof(value), &value);
}

#pragma inline_depth(16)


} // namespace


// Original VM stack-local reads form low 32-bit i386 address words from
// the actual stack byte buffer, signed frame base and script operand.
// Keep the original signed indirect/index decisions and read order.
int EclVmContext::ReadIntValue(unsigned int flagIndex, int value)
{
    if (IsOperandIndirect(instruction, flagIndex))
    {
        if (value >= 0)
        {
            EclVmStackView *const operandStack = &stack;
            return reinterpret_cast<const EclVmScalar *>(
                reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(operandStack->data) +
                    static_cast<unsigned int>(operandStack->frameBase) +
                    static_cast<unsigned int>(value)))->integer;
        }
        if (value == -1)
        {
            stack.Pop('i', sizeof(value), &value);
        }
        else
        {
            return host->ReadEclInt(value);
        }
    }
    return value;
}

// The encoded -1 operand pops a scalar and its four-byte type slot.
// Retail i386 forms both stack positions by 32-bit subtraction/addition;
// preserve the signed nonnegative test before consuming either word.
int EclVmContext::ReadInt(unsigned int index)
{
    if (IsOperandIndirect(instruction, index))
    {
        const int value = OperandInt(instruction, index);
        if (value >= 0)
            return reinterpret_cast<const EclVmScalar *>(
                reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(stack.data) +
                    static_cast<unsigned int>(stack.frameBase) +
                    static_cast<unsigned int>(value)))->integer;
        if (value == -1)
        {
            const unsigned int topBits =
                static_cast<unsigned int>(stack.stackTop) - 4u;
            // The VM compares this native DWORD as one signed i386 word.
            // VC7.1 preserves its two's-complement bits on conversion, without
            // loading an unsigned local through a different typed pointer.
            if (static_cast<int>(topBits) >= 0)
            {
                stack.stackTop = static_cast<int>(
                    static_cast<unsigned int>(stack.stackTop) - 4u);
                index = reinterpret_cast<const EclVmScalar *>(
                    reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(stack.data) +
                    static_cast<unsigned int>(stack.stackTop)))->bits;
                // Preserve the popped native DWORD without writing through
                // a differently typed pointer into the signed stack top.
                const unsigned int nextBits =
                    static_cast<unsigned int>(stack.stackTop) - 4u;
                memcpy(&stack.stackTop, &nextBits, sizeof stack.stackTop);
                // The caller has just popped a value DWORD; the preceding
                // four-byte slot carries its one-byte type tag.
                if (reinterpret_cast<const EclVmStackTypeSlotView *>(
                        reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(stack.data) +
                    static_cast<unsigned int>(stack.stackTop)))->type == 'f')
                    return static_cast<int>(
                        reinterpret_cast<const EclVmScalar *>(&index)->real);
            }
            return static_cast<int>(index);
        }
        return host->ReadEclInt(value);
    }
    return OperandInt(instruction, index);
}

float __stdcall EclVmContext::ReadFloatValue(
    EclVmContext *context, unsigned int flagIndex, float value)
{
    if (IsOperandIndirect(context->instruction, flagIndex))
    {
        if (value >= 0.0f)
            return reinterpret_cast<const EclVmScalar *>(
                reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(context->stack.data) +
                    static_cast<unsigned int>(context->stack.frameBase) +
                    static_cast<unsigned int>(static_cast<int>(value))))->real;
        if (value == -1.0f)
        {
            context->stack.Pop('f', sizeof(value), &value);
        }
        else
        {
            return context->host->ReadEclFloat(static_cast<int>(value));
        }
    }
    return value;
}

float EclVmContext::ReadFloat(unsigned int index)
{
    if (IsOperandIndirect(instruction, index))
    {
        if (OperandFloat(instruction, index) >= 0.0f)
            return reinterpret_cast<const EclVmScalar *>(
                reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(stack.data) +
                    static_cast<unsigned int>(stack.frameBase) +
                    static_cast<unsigned int>(static_cast<int>(OperandFloat(instruction, index)))))->real;
        if (OperandFloat(instruction, index) == -1.0f)
        {
            float result;
            stack.Pop('f', sizeof(result), &result);
            return result;
        }
        return host->ReadEclFloat(
            static_cast<int>(OperandFloat(instruction, index)));
    }
    return OperandFloat(instruction, index);
}

// Original ECL lvalue resolvers return i386 address words formed from
// the stack byte base, signed frame-base and serialized operand dword.
// Keep the original indirect guards and real host resolver fallbacks.
int *EclVmContext::ResolveInt(unsigned int index)
{
    if (IsOperandIndirect(instruction, index))
    {
        if (OperandInt(instruction, index) >= 0)
        {
            EclVmStackView *const operandStack = &stack;
            return &reinterpret_cast<EclVmScalar *>(
                reinterpret_cast<unsigned char *>(
                    reinterpret_cast<unsigned int>(operandStack->data) +
                    static_cast<unsigned int>(operandStack->frameBase) +
                    static_cast<unsigned int>(OperandInt(instruction, index))))->integer;
        }
        return host->ResolveEclInt(OperandInt(instruction, index));
    }
    return NULL;
}

float *EclVmContext::ResolveFloat(unsigned int index)
{
    if (IsOperandIndirect(instruction, index))
    {
        if (OperandFloat(instruction, index) >= 0.0f)
        {
            EclVmStackView *const operandStack = &stack;
            return &reinterpret_cast<EclVmScalar *>(
                reinterpret_cast<unsigned char *>(
                    reinterpret_cast<unsigned int>(operandStack->data) +
                    static_cast<unsigned int>(operandStack->frameBase) +
                    static_cast<unsigned int>(static_cast<int>(OperandFloat(instruction, index)))))->real;
        }
        return host->ResolveEclFloat(
            static_cast<int>(OperandFloat(instruction, index)));
    }
    return NULL;
}


// Count-reference source lead: levzzz5154/th10-decomp, MIT (c) 2026 levzzz,
// d1f31212514781c928fba145d8207397c8fb5650,
// matching2/msvc/src/script/sptlib/ecl_name_table.cpp.
// See docs/licenses/th10-decomp-MIT.txt.
static __forceinline void AddEclSubroutineCount(int &count, int added)
{
    // At most 32 valid files contribute 16-bit counts, so this sum is below INT_MAX.
    count += added;
}


// Native SCPT file-slot words are indexed in one flat Win32 address space.
// The parser does not range-check fileCount before forming a slot pointer.
static __forceinline EclVmScriptFileHeader **EclFileSlotAtWord(
    EclVmScriptFileHeader **base, int index)
{
    return reinterpret_cast<EclVmScriptFileHeader **>(
        reinterpret_cast<unsigned int>(base) +
        static_cast<unsigned int>(index) * sizeof(EclVmScriptFileHeader *));
}

// Serialized sorted subroutine entries have an eight-byte Win32 stride.
static __forceinline EclVmSubroutineEntry *EclMutableEntryAtWord(
    EclVmSubroutineEntry *base, int index)
{
    return reinterpret_cast<EclVmSubroutineEntry *>(
        reinterpret_cast<unsigned int>(base) +
        static_cast<unsigned int>(index) * sizeof(EclVmSubroutineEntry));
}

int EclVmScriptDatabase::AddScriptData(void *scriptData)
{
    const unsigned int SCPT_MAGIC = 0x54504353u;
    files[fileCount] = static_cast<EclVmScriptFileHeader *>(scriptData);
    EclVmScriptFileHeader **slot = &files[fileCount];
    if (files[fileCount]->magic != SCPT_MAGIC)
    {
        *slot = NULL;
        return -1;
    }
    if (files[fileCount]->version != 1)
    {
        *slot = NULL;
        return -1;
    }
    int *data = reinterpret_cast<int *>(
        reinterpret_cast<char *>(files[fileCount]) +
        files[fileCount]->includeLength + sizeof(EclVmScriptFileHeader));
    const char *names = reinterpret_cast<const char *>(
        data + files[fileCount]->subroutineCount);
    EclVmSubroutineEntry *oldEntries = subroutines;
    EclVmSubroutineEntry *previousEntries = subroutines;
    AddEclSubroutineCount(subroutineCount, files[fileCount]->subroutineCount);
    const unsigned int size = subroutineCount * sizeof(EclVmSubroutineEntry);
    subroutines = static_cast<EclVmSubroutineEntry *>(malloc(size));
    if (oldEntries == NULL)
    {
        for (int index = 0; index < subroutineCount; ++index, ++data)
        {
            subroutines[index].header = reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(files[fileCount]) +
                static_cast<unsigned int>(*data));
            subroutines[index].name = names;
            names += strlen(names) + 1;
        }
    }
    else
    {
        int sub = files[fileCount]->subroutineCount;
        int kept = subroutineCount - sub;
        memcpy(subroutines, oldEntries, sizeof(EclVmSubroutineEntry) * kept);
        if (previousEntries)
            free((oldEntries == previousEntries) ? oldEntries : previousEntries);
        for (int merged = 0; merged < files[fileCount]->subroutineCount; ++merged, ++data)
        {
            int index;
            for (index = 0; index < kept; ++index)
            {
                if (strcmp(names, subroutines[index].name) <= 0)
                    break;
            }
            for (int i = subroutineCount - 1; i > index; --i)
                subroutines[i] = subroutines[i - 1];
            subroutines[index].header = reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(files[fileCount]) +
                static_cast<unsigned int>(*data));
            subroutines[index].name = names;
            names += strlen(names) + 1;
            ++kept;
        }
    }
    const int registered = fileCount++;
    if (files[registered]->includeLength != 0)
        LoadPackage(reinterpret_cast<unsigned char *>(files[registered]) +
                    sizeof(EclVmScriptFileHeader));
    return registered;
}

// Script lookup indices address an eight-byte native record with i386
// low-32-bit base plus stride arithmetic, even for corrupt table counts.
static __forceinline const EclVmSubroutineEntry *EclSubroutineAtWord(
    const EclVmSubroutineEntry *base, int index)
{
    return reinterpret_cast<const EclVmSubroutineEntry *>(
        reinterpret_cast<unsigned int>(base) +
        static_cast<unsigned int>(index) * sizeof(EclVmSubroutineEntry));
}

EclVmInstruction *EclVmScriptDatabase::FindSubroutine(const char *name)
{
    // Retail i386 keeps every bound update and midpoint in a 32-bit word.
    // Preserve signed comparisons/division without signed-overflow UB.
    int lower = 0;
    int upper = static_cast<int>(
        static_cast<unsigned int>(subroutineCount) - 1u);
    while (lower <= upper)
    {
        const int difference = static_cast<int>(
            static_cast<unsigned int>(upper) -
            static_cast<unsigned int>(lower));
        const int middle = static_cast<int>(
            static_cast<unsigned int>(difference / 2) +
            static_cast<unsigned int>(lower));
        const EclVmSubroutineEntry *const mid = EclSubroutineAtWord(subroutines, middle);
        const int comparison = strcmp(name, mid->name);
        if (comparison == 0)
        {
            return reinterpret_cast<EclVmInstruction *>(
                reinterpret_cast<unsigned int>(mid->header) +
                static_cast<unsigned int>(
                    offsetof(EclVmSubroutineStreamView, firstInstruction)));
        }
        if (comparison < 0)
            upper = static_cast<int>(
                static_cast<unsigned int>(middle) - 1u);
        else
            lower = static_cast<int>(
                static_cast<unsigned int>(middle) + 1u);
    }
    return NULL;
}


extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)

static const EclVmCallArgumentView *CallArgument(
    const EclVmInstruction *instruction, unsigned int offset)
{
    return reinterpret_cast<const EclVmCallArgumentView *>(
        reinterpret_cast<unsigned int>(instruction->operands) + offset);
}


int EclVmContext::StartSubroutine(
    EclVmContext *volatile caller, unsigned int firstArgument)
{
    EclVmContext *destination = this;
    EclVmInstruction **const callerInstruction = &caller->instruction;
    EclVmInstruction *const call = *callerInstruction;
    const int inlineBytes = call->callOperands.inlineNameBytes;
    // The CALL metadata offset is a serialized i386 address word, not a
    // signed counter. Advancing it modulo 2^32 preserves unchecked script
    // byte offsets without an implementation-defined signed conversion.
    unsigned int metadataOffset =
        static_cast<unsigned int>(inlineBytes) +
        firstArgument * 4u + 4u;
    int preservedValue = 0;
    const int previousTop = destination->stack.stackTop;
    int argumentOffset = static_cast<int>(
        static_cast<unsigned int>(previousTop) + 8u);

    if (previousTop == 0)
    {
        destination->stack.Push(0, sizeof(preservedValue), &preservedValue);
        argumentOffset = 12;
    }

    int argumentIndex = firstArgument + 1;
    const int initialCount = (*callerInstruction)->operandCount;
    if (argumentIndex < initialCount)
    {
        EclVmScalar *volatile argument = reinterpret_cast<EclVmScalar *>(
            reinterpret_cast<unsigned int>(destination->stack.data) +
            static_cast<unsigned int>(argumentOffset));
        unsigned int valueOffset = metadataOffset +
            static_cast<int>(offsetof(EclVmCallArgumentView, integer));
        volatile unsigned int &valueCursor = valueOffset;
        for (int operandCount = initialCount; argumentIndex < operandCount;)
        {
            const EclVmInstruction *const instruction = *callerInstruction;
            const char sourceType = CallArgument(instruction, metadataOffset)->sourceType;

            // CALL arguments follow a counted name; scalarOperands[1] is
            // only their first-word overlay, not a bounded C++ array.
            // Preserve the original low-32-bit dword byte-address walk.
            if (sourceType == 'f' || sourceType == 'g')
            {
                EclVmScalar value;
                value.integer =
                    OperandWord(instruction,
                        valueOffset / sizeof(int))->integer;
                EclVmContext *const source = caller;
                const float resolved = ReadFloatValue(source, argumentIndex, value.real);
                if (CallArgument(*callerInstruction, metadataOffset)->destinationType == 'f')
                    argument->real = resolved;
                else
                    argument->integer =
                        static_cast<int>(resolved);
            }
            else
            {
                EclVmContext *const source = caller;
                const int operand =
                    OperandInt(instruction, valueOffset / sizeof(int));
                const int resolved = source->ReadIntValue(argumentIndex, operand);
                if (CallArgument(*callerInstruction, metadataOffset)->destinationType == 'f')
                {
                    const float converted = static_cast<float>(resolved);
                    EclVmScalar *const output = argument;
                    output->real = converted;
                }
                else
                {
                    argument->integer = resolved;
                    _ReadWriteBarrier();
                }
            }

            EclVmScalar *const previousArgument = argument;
            EclVmInstruction *volatile &instructionSlot = *callerInstruction;
            const EclVmInstruction *const nextInstruction = instructionSlot;
            const unsigned int previousValueOffset = valueCursor;
            argument = reinterpret_cast<EclVmScalar *>(
                reinterpret_cast<unsigned int>(previousArgument) + sizeof(EclVmScalar));
            operandCount = nextInstruction->operandCount;
            ++argumentIndex;
            valueCursor = previousValueOffset + sizeof(EclVmCallArgumentView);
            metadataOffset += sizeof(EclVmCallArgumentView);
        }
    }

    EclVmContext *const frameCaller = caller;
    if (previousTop != 0)
    {
        destination->stack.Pop(0, sizeof(preservedValue), &preservedValue);
        destination->stack.stackTop = previousTop;
        reinterpret_cast<EclVmStackSavedWordView *>(
            reinterpret_cast<unsigned int>(destination->stack.data) +
            static_cast<unsigned int>(previousTop) -
            static_cast<unsigned int>(sizeof(EclVmStackSavedWordView)))->value =
                preservedValue;
        destination->stack.Push(
            0, sizeof(frameCaller->currentTime), &frameCaller->currentTime);
        destination->stack.Push(
            0, sizeof(*callerInstruction), callerInstruction);
    }
    else
    {
        preservedValue = 0;
        destination->stack.stackTop = 4;
        destination->stack.Push(0, sizeof(preservedValue), &preservedValue);
        destination->stack.Push(0, sizeof(preservedValue), &preservedValue);
    }

    EclVmContext *const previousContext = frameCaller->host->activeContext;
    frameCaller->host->activeContext = destination;
    EclVmHost *const host = frameCaller->host;
    // Subroutine names follow the initial serialized 32-bit CALL operand.
    // The instruction prefix owns the +0x10 byte-stream start.
    host->activeContext->instruction = host->scriptDatabase->FindSubroutine(
        (*callerInstruction)->callOperands.subroutineName);
    host->activeContext->currentTime = 0.0f;
    if (host->activeContext->instruction == NULL)
    {
        *callerInstruction = NULL;
        return -1;
    }
    caller->host->activeContext = previousContext;
    return 0;
}


int EclVmHost::Run(float timeDelta)
{
    int first = 1;
    EclVmThreadNode *node = &threadList;
    while (node != NULL)
    {
        EclVmThreadNode *next = node->next;
        activeContext = node->context;
        if (first)
        {
            if (activeContext->Run(timeDelta) != 0)
                return -1;
            first = false;
        }
        else if (activeContext->Run(timeDelta) != 0)
        {
            delete activeContext;
            if (node->next != NULL)
                node->next->previous = node->previous;
            if (node->previous != NULL)
                node->previous->next = node->next;
            node->next = NULL;
            node->previous = NULL;
            delete node;
        }
        node = next;
    }
    activeContext = &embeddedContext;
    return 0;
}


struct EnemyFullObjectView;

// Enemy.cpp keeps its target-specific full-object view separate from this
// generic VM layer. Both views share the target-proven EclVmHost prefix.
int EnemyRunEcl(EnemyFullObjectView *owner, float timeDelta)
{
    return reinterpret_cast<EclVmHost *>(owner)->Run(timeDelta);
}


void EclVmHost::SpawnThread(int threadId, unsigned int firstArgument)
{
    EclVmContext *context = new EclVmContext;
    EclVmThreadNode *node = new EclVmThreadNode;

    context->threadId = threadId;
    context->host = this;
    context->currentTime = 0.0f;
    context->instruction = NULL;
    context->difficultyMask = activeContext->difficultyMask;

    EclVmThreadNode *const list = &threadList;
    node->context = context;
    node->next = NULL;
    node->previous = NULL;
    if (list->next != NULL)
    {
        node->next = list->next;
        list->next->previous = node;
    }
    list->next = node;
    node->previous = list;

    context->StartSubroutine(activeContext, firstArgument);
}

EclVmThreadNode *EclVmHost::FindThread(int threadId)
{
    EclVmThreadNode *node = &threadList;
    while (node != NULL)
    {
        if (node->context->threadId == threadId)
            return node;
        node = node->next;
    }
    return NULL;
}

void EclVmHost::StopAllThreads()
{
    EclVmThreadNode *node = threadList.next;
    while (node != NULL)
    {
        EclVmThreadNode *next = node->next;
        EclStopThreadContext(node->context);
        node = next;
    }
}


// Model the disabled scalar trace suggested by FORMAT's unused text copy.
// Argument evaluation remains observable; the original trace definition is unknown.
// The debug sink is inactive in the retail build.
static void EclDebugTrace(const char *format, ...)
{
}


template <typename Result>
static __forceinline void TraceFormatArgument(
    EclVmContext *vm, const char *scratch, unsigned int argumentIndex)
{
    const int inlineBytes =
        reinterpret_cast<const EclVmFormatInlinePrefixView *>(
            vm->instruction->operands)->inlineBytes;
    const EclVmFormatArgumentView *argument =
        reinterpret_cast<const EclVmFormatArgumentView *>(
            reinterpret_cast<unsigned int>(
                vm->instruction->operands) +
            static_cast<unsigned int>(sizeof(int)) +
            static_cast<unsigned int>(inlineBytes) +
            static_cast<unsigned int>(argumentIndex) *
            static_cast<unsigned int>(sizeof(EclVmFormatArgumentView)));
    // FORMAT is a counted scalar word stream, not a
    // C++ one-element scalarOperands array. The original
    // i386 index * 4 address arithmetic wraps at 32 bits.
    const char argumentType = argument->type;
    if (argumentType == 'f' || argumentType == 'g') {
        EclDebugTrace(scratch, static_cast<Result>(EclVmContext::ReadFloatValue(
            vm, argumentIndex + 1u,
            reinterpret_cast<const EclVmScalar *>(
                reinterpret_cast<unsigned int>(vm->instruction) +
                static_cast<unsigned int>(
                    offsetof(EclVmInstruction, scalarOperands)) +
                (static_cast<unsigned int>(
                    inlineBytes / static_cast<int>(sizeof(int))) +
                 2u + static_cast<unsigned int>(argumentIndex) *
                    static_cast<unsigned int>(
                        sizeof(EclVmFormatArgumentView) /
                        sizeof(int))) * 4u)->real)));
    } else {
        EclDebugTrace(scratch, static_cast<Result>(vm->ReadIntValue(
            argumentIndex + 1u,
            reinterpret_cast<const EclVmScalar *>(
                reinterpret_cast<unsigned int>(vm->instruction) +
                static_cast<unsigned int>(
                    offsetof(EclVmInstruction, scalarOperands)) +
                (static_cast<unsigned int>(
                    inlineBytes / static_cast<int>(sizeof(int))) +
                 2u + static_cast<unsigned int>(argumentIndex) *
                    static_cast<unsigned int>(
                        sizeof(EclVmFormatArgumentView) /
                        sizeof(int))) * 4u)->integer)));
    }
}


__forceinline bool EclVmStackView::PopWord(void *destination)
{
    const int wordBytes = sizeof(int);
    if (stackTop - wordBytes >= 0)
    {
        stackTop -= wordBytes;
        const int valueOffset = stackTop;
        const EclVmScalar *source = reinterpret_cast<const EclVmScalar *>(data + valueOffset);
        const unsigned int word = source->bits;
        static_cast<EclVmScalar *>(destination)->bits = word;
        return true;
    }
    return false;
}

__forceinline void EclVmStackView::PopInt(int *destination)
{
    if (PopWord(destination))
    {
        stackTop -= sizeof(EclVmStackTypeSlotView);
        const unsigned char tag = data[stackTop];
        if (tag == 'f')
        {
            const float sourceValue = reinterpret_cast<const EclVmScalar *>(destination)->real;
            *destination = static_cast<int>(sourceValue);
        }
    }
}

__forceinline void EclVmStackView::PopFloat(float *destination)
{
    if (PopWord(destination))
    {
        stackTop -= static_cast<int>(sizeof(EclVmStackTypeSlotView));
        const unsigned char tag = data[stackTop];
        if (tag == 'f')
        {
            // The stored word already has the requested representation.
        }
        else if (tag == 'i')
        {
            const int sourceValue = reinterpret_cast<const EclVmScalar *>(destination)->integer;
            const float converted = static_cast<float>(sourceValue);
            *destination = converted;
        }
    }
}

static __forceinline void PopInt(EclVmContext *context, int *destination)
{
    context->stack.PopInt(destination);
}

static __forceinline void PopFloat(EclVmContext *context, float *destination)
{
    context->stack.PopFloat(destination);
}

int EclVmContext::Run(float timeDelta)
{
    if (instruction == NULL)
        return -1;
    EclVmInstruction **instructionCursor = &instruction;
    while (static_cast<float>(instruction->time) <= currentTime) {
        EclVmInstruction *current = *instructionCursor;

        if ((difficultyMask & current->difficultyMask) != 0) {
            switch (current->opcode) {
            case ECL_VM_NOP:
                break;

            case ECL_VM_TERMINATE:
                *instructionCursor = NULL;
                return -1;

            case ECL_VM_RETURN:
            {
                stack.LeaveFrame();
                if (stack.stackTop == 0) {
                    *instructionCursor = NULL;
                    return -1;
                }

                {
                    EclVmStackView *frame = &stack;
                    frame->PopWord(instructionCursor);
                }
                stack.PopWord(&currentTime);
                if (*instructionCursor == NULL)
                    return -1;
                break;
            }

            case ECL_VM_SPAWN_THREAD:
                host->SpawnThread(-1, 0);
                break;

            case ECL_VM_STOP_ALL_THREADS:
                host->StopAllThreads();
                break;

            case ECL_VM_SPAWN_THREAD_WITH_ID:
            {
                // Round the counted name to a word index using the native
                // unsigned addition before dividing by four.
                const unsigned int operandBase =
                    reinterpret_cast<unsigned int>(current->scalarOperands);
                host->SpawnThread(
                    ReadIntValue(
                        1, reinterpret_cast<const EclVmScalar *>(
                            operandBase +
                            ((static_cast<unsigned int>(
                                current->spawnThreadOperands.inlineBytes) + 4u) >> 2) *
                                sizeof(EclVmScalar))->integer),
                    1);
                break;
            }

            case ECL_VM_STOP_THREAD:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                    thread->context->instruction = NULL;
                break;
            }

            case ECL_VM_SET_THREAD_FLAG:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                {
                    EclVmContext *const child = thread->context;
                    child->threadFlag0 = 1;
                }
                break;
            }

            case ECL_VM_CLEAR_THREAD_FLAG:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                {
                    EclVmContext *const child = thread->context;
                    child->threadFlag0 = 0;
                }
                break;
            }

            case ECL_VM_SET_THREAD_CONTROL:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                {
                    EclVmContext *const threadContext = thread->context;
                    threadContext->threadControl = ReadInt(1);
                }
                break;
            }

            case ECL_VM_CALL:
                if (StartSubroutine(this, 0))
                    return -1;
                continue;

            case ECL_VM_JUMP_IF_TRUE:
            {
                int condition;
                PopInt(this, &condition);
                if (condition != 0)
                    goto jump_instruction;
                break;
            }

            case ECL_VM_JUMP_IF_FALSE:
            {
                int condition;
                PopInt(this, &condition);
                if (condition != 0)
                    break;
            }

            case ECL_VM_JUMP:
jump_instruction:
            {
                EclVmInstruction *const branch = *instructionCursor;
                currentTime = static_cast<float>(branch->jumpOperands.targetTime);
                *instructionCursor = reinterpret_cast<EclVmInstruction *>(
                    reinterpret_cast<char *>(branch) +
                    branch->jumpOperands.instructionByteOffset);
                continue;
            }

            case ECL_VM_SUBTRACT_TIME:
                currentTime -= static_cast<float>(ReadInt(0));
                break;

            // Stack frames, then typed transfers between operands and the stack.
            case ECL_VM_ENTER_FRAME:
                stack.EnterFrame(ReadInt(0));
                break;
            case ECL_VM_LEAVE_FRAME:
                stack.LeaveFrame();
                break;
            case ECL_VM_PUSH_INT: {
                int value = ReadInt(0);
                stack.Push('i', 4, &value);
                break;
            }
            case ECL_VM_STORE_INT: {
                int* destination = ResolveInt(0);
                PopInt(this, destination);
                break;
            }
            case ECL_VM_PUSH_FLOAT: {
                float value = ReadFloat(0);
                stack.Push('f', 4, &value);
                break;
            }
            case ECL_VM_STORE_FLOAT: {
                float* destination = ResolveFloat(0);
                PopFloat(this, destination);
                break;
            }

            // Integer arithmetic reuses the left operand storage for the result:
            // the integer and float operations both push from that same word.
            case ECL_VM_ADD_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                left_storage = static_cast<int>(static_cast<unsigned int>(left_storage) +
                    static_cast<unsigned int>(right_storage));
                stack.Push('i', 4, &left_storage);
                break;
            }
            case ECL_VM_SUBTRACT_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                left_storage = static_cast<int>(static_cast<unsigned int>(left_storage) -
                    static_cast<unsigned int>(right_storage));
                stack.Push('i', 4, &left_storage);
                break;
            }
            case ECL_VM_MULTIPLY_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                left_storage = static_cast<int>(static_cast<unsigned int>(left_storage) *
                    static_cast<unsigned int>(right_storage));
                stack.Push('i', 4, &left_storage);
                break;
            }
            case ECL_VM_DIVIDE_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                left_storage = left_storage / right_storage;
                stack.Push('i', 4, &left_storage);
                break;
            }
            case ECL_VM_MODULO_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                left_storage = left_storage % right_storage;
                stack.Push('i', 4, &left_storage);
                break;
            }

            // Floating-point arithmetic uses the same pop order and a float result tag.
            case ECL_VM_ADD_FLOAT: {
                float right_storage;
                PopFloat(this, &right_storage);
                float left_storage;
                PopFloat(this, &left_storage);
                left_storage = left_storage + right_storage;
                stack.Push('f', 4, &left_storage);
                break;
            }
            case ECL_VM_SUBTRACT_FLOAT: {
                float right_storage;
                PopFloat(this, &right_storage);
                float left_storage;
                PopFloat(this, &left_storage);
                left_storage = left_storage - right_storage;
                stack.Push('f', 4, &left_storage);
                break;
            }
            case ECL_VM_MULTIPLY_FLOAT: {
                float right_storage;
                PopFloat(this, &right_storage);
                float left_storage;
                PopFloat(this, &left_storage);
                left_storage = left_storage * right_storage;
                stack.Push('f', 4, &left_storage);
                break;
            }
            case ECL_VM_DIVIDE_FLOAT: {
                float right_storage;
                PopFloat(this, &right_storage);
                float left_storage;
                PopFloat(this, &left_storage);
                left_storage = left_storage / right_storage;
                stack.Push('f', 4, &left_storage);
                break;
            }

            // Integer comparisons and logical NOT produce an integer result.
            case ECL_VM_EQUAL_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                const int left_operand = left_storage;
                const int right_operand = right_storage;
                int result = left_operand == right_operand;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_NOT_EQUAL_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                const int left_operand = left_storage;
                const int right_operand = right_storage;
                int result = left_operand != right_operand;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_LESS_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                const int left_operand = left_storage;
                const int right_operand = right_storage;
                int result = left_operand < right_operand;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_LESS_EQUAL_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                const int left_operand = left_storage;
                const int right_operand = right_storage;
                int result = left_operand <= right_operand;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_GREATER_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                const int left_operand = left_storage;
                const int right_operand = right_storage;
                int result = left_operand > right_operand;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_GREATER_EQUAL_INT: {
                int right_storage;
                PopInt(this, &right_storage);
                int left_storage;
                PopInt(this, &left_storage);
                const int left_operand = left_storage;
                const int right_operand = right_storage;
                int result = left_operand >= right_operand;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_NOT_INT: {
                int value;
                PopInt(this, &value);
                int result = value == 0;
                stack.Push('i', 4, &result);
                break;
            }

            // Floating-point comparisons also push an integer, not a float.
            case ECL_VM_EQUAL_FLOAT: {
                float right_storage;
                PopFloat(this, &right_storage);
                float left_storage;
                PopFloat(this, &left_storage);
                const float left_operand = left_storage;
                const float right_operand = right_storage;
                int result = (left_operand == right_operand) ? 1 : 0;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_NOT_EQUAL_FLOAT: {
                EclVmStackView *operands = &stack;
                float right_storage;
                PopFloat(this, &right_storage);
                float left_storage;
                operands->PopFloat(&left_storage);
                const float left_operand = left_storage;
                const float right_operand = right_storage;
                int result = (left_operand != right_operand) ? 1 : 0;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_LESS_FLOAT: {
                float right_storage;
                stack.PopFloat(&right_storage);
                float left_storage;
                stack.PopFloat(&left_storage);
                const float left_operand = left_storage;
                const float right_operand = right_storage;
                int result = (left_operand < right_operand) ? 1 : 0;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_LESS_EQUAL_FLOAT:
            {
                EclVmStackView &operands = stack;
                EclVmScalar rightOperand;
                operands.Pop('f', sizeof(rightOperand.real), &rightOperand.real);
                const float right = rightOperand.real;
                EclVmScalar leftOperand;
                operands.Pop('f', sizeof(leftOperand.real), &leftOperand.real);
                const float left = leftOperand.real;
                int floatResult = left <= right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_GREATER_FLOAT:
            {
                EclVmStackView *const operands = &stack;
                EclVmScalar rightOperand;
                operands->Pop('f', sizeof(rightOperand.real), &rightOperand.real);
                const float right = rightOperand.real;
                EclVmScalar leftOperand;
                operands->Pop('f', sizeof(leftOperand.real), &leftOperand.real);
                const float left = leftOperand.real;
                int floatResult = left > right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_GREATER_EQUAL_FLOAT: {
                float right_storage;
                stack.PopFloat(&right_storage);
                float left_storage;
                stack.PopFloat(&left_storage);
                int result = (left_storage >= right_storage) ? 1 : 0;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_NOT_FLOAT: {
                float value;
                PopFloat(this, &value);
                int result = (value == 0.0f) ? 1 : 0;
                stack.Push('i', 4, &result);
                break;
            }

            // Logical and bitwise operations on integer stack words.
            case ECL_VM_LOGICAL_OR: {
                int right;
                PopInt(this, &right);
                int left;
                PopInt(this, &left);
                int result = (left || right) ? 1 : 0;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_LOGICAL_AND: {
                int right;
                PopInt(this, &right);
                int left;
                PopInt(this, &left);
                int result = (left && right) ? 1 : 0;
                stack.Push('i', 4, &result);
                break;
            }
            case ECL_VM_BITWISE_XOR:
            {
                EclVmStackView &operands = stack;
                EclVmScalar right;
                operands.Pop('i', sizeof(right.integer), &right.integer);
                EclVmScalar left;
                operands.Pop('i', sizeof(left.integer), &left.integer);
                PushInt(this, left.integer ^ right.integer);
                break;
            }

            case ECL_VM_BITWISE_OR:
            {
                EclVmScalar right;
                stack.Pop('i', sizeof(right), &right);
                EclVmScalar left;
                stack.Pop('i', sizeof(left), &left);
                PushInt(this, left.integer | right.integer);
                break;
            }

            case ECL_VM_BITWISE_AND: {
                int right;
                PopInt(this, &right);
                int left;
                PopInt(this, &left);
                int result = left & right;
                stack.Push('i', 4, &result);
                break;
            }

            // Negation and post-decrement.
            case ECL_VM_NEGATE_INT: {
                int value;
                PopInt(this, &value);
                value = static_cast<int>(0u - static_cast<unsigned int>(value));
                stack.Push('i', 4, &value);
                break;
            }
            case ECL_VM_NEGATE_FLOAT_STORAGE: {
                // Negate the integer word, then write it back with the float tag.
                EclVmScalar word;
                PopFloat(this, &word.real);
                word.bits = 0u - word.bits;
                stack.Push('f', sizeof(word), &word);
                break;
            }
            case ECL_VM_POST_DECREMENT_INT: {
                int original_value = ReadInt(0);
                int* destination = ResolveInt(0);
                *destination = static_cast<int>(static_cast<unsigned int>(original_value) - 1u);
                stack.Push('i', 4, &original_value);
                break;
            }

            // Trigonometry and vector operations.
            case ECL_VM_SINE: {
                float angle;
                PopFloat(this, &angle);
                float result = (float)sin(angle);
                stack.Push('f', 4, &result);
                break;
            }
            case ECL_VM_COSINE: {
                float angle;
                PopFloat(this, &angle);
                float result = (float)cos(angle);
                stack.Push('f', 4, &result);
                break;
            }
            case ECL_VM_POLAR_TO_CARTESIAN: {
                EclVmPolarVectorView vector;
                vector.FromAngleMagnitude(EnemyWrapAngle(ReadFloat(2)), ReadFloat(3));
                float *const x = ResolveFloat(0);
                *x = vector.x;
                *ResolveFloat(1) = vector.y;
                break;
            }
            case ECL_VM_LENGTH_SQUARED: {
                float x = ReadFloat(1);
                float y = ReadFloat(2);
                *ResolveFloat(0) = y * y + x * x;
                break;
            }
            case ECL_VM_NORMALIZE_ANGLE: {
                float* destination = ResolveFloat(0);
                *destination = EnemyWrapAngle(ReadFloat(0));
                break;
            }
            case ECL_VM_POINT_ANGLE: {
                float x = ReadFloat(2) - ReadFloat(0);
                float y = ReadFloat(3) - ReadFloat(1);
                y = (float)atan2(y, x);
                *ResolveFloat(0) = y;
                break;
            }

            // The counted FORMAT text and argument records are advanced
            // in native i386 address words; the inline length still
            // uses its original signed divide-by-four decoding.
            case ECL_VM_EVALUATE_FORMAT_OPERANDS:
            {
                // The inline format string follows the first 32-bit operand.
                // FORMAT's inline text is a counted wire region, not a C++
                // one-character array. The target forms its first byte by
                // adding the prefix width to a flat 32-bit address word.
                const unsigned int textPosition =
                    reinterpret_cast<unsigned int>(current->operands) +
                    static_cast<unsigned int>(sizeof(int));
                const char *cursor = reinterpret_cast<const char *>(textPosition);
                char *scratch = static_cast<char *>(malloc(0x400));
                scratch[0] = '\0';

                // Retail advances the FORMAT scalar argument index as one
                // native low-32-bit word before the next signed read.
                // FORMAT consumes argument records by a native 32-bit word
                // cursor. Use the unsigned count directly: this preserves
                // wraparound without casting an int local to a pointer.
                unsigned int argumentIndex = 0;
                while (cursor != NULL) {
                    const char *percent = strchr(cursor, '%');
                    if (percent == NULL)
                        break;

                    strcpy(scratch, cursor);
                    *reinterpret_cast<char *>(
                        reinterpret_cast<unsigned int>(scratch) +
                        (reinterpret_cast<unsigned int>(percent) -
                         reinterpret_cast<unsigned int>(cursor))) = '\0';
                    switch (*reinterpret_cast<const char *>(
                        reinterpret_cast<unsigned int>(percent) + 1u)) {
                    case '%':
                        EclDebugTrace(scratch);
                        break;
                    case 'd':
                        TraceFormatArgument<int>(this, scratch, argumentIndex);
                        ++argumentIndex;
                        break;
                    case 'f':
                        TraceFormatArgument<double>(this, scratch, argumentIndex);
                        ++argumentIndex;
                        break;
                    }
                    cursor = reinterpret_cast<const char *>(
                        reinterpret_cast<unsigned int>(percent) + 2u);
                }
                free(scratch);
                break;
            }

            default:
                if (host->DispatchEclInstruction() == -1)
                    return 0;
                break;
            }
        }

        instructionCursor = &instruction;
        *instructionCursor = reinterpret_cast<EclVmInstruction *>(
            reinterpret_cast<char *>(instruction) + instruction->size);
    }

    currentTime += timeDelta;
    return 0;
}
