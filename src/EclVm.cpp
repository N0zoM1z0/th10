#include "EclVm.hpp"

// FORMAT source shape informed by th10-decomp (MIT), commit c6791d82dc9f02e55eca45a5314f900a85481efd.
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


EclVmStackView::EclVmStackView()
    : stackTop(0), frameBase(0)
{
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
    unsigned char *const valueAddress = stackData + stackTop;
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

static __forceinline int PopInt(EclVmContext *context)
{
    int value;
    context->stack.Pop('i', sizeof(value), &value);
    return value;
}

static __forceinline float PopFloat(EclVmContext *context)
{
    EclVmScalar value;
    EclVmStackView *const stackView = &context->stack;
    stackView->Pop('f', sizeof(value.real), &value.real);
    return value.real;
}

// Target Run expands these scalar wrappers but retains calls to the separate
// EclVmStackView::Push owner.  Limit nested inlining only across this pair.
#pragma inline_depth(1)
static __forceinline int PushInt(EclVmContext *context, const int &value)
{
    return context->stack.Push('i', sizeof(value), &value);
}

static __forceinline int PushFloat(EclVmContext *context, float value)
{
    return context->stack.Push('f', sizeof(value), &value);
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
            if (*reinterpret_cast<const int *>(&topBits) >= 0)
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
    unsigned int flagIndex, EclVmContext *context, float value)
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
static void AddEclSubroutineCount(int &count, int added)
{
    // Preserve native modulo-DWORD addition without a mismatched int alias.
    const unsigned int bits = static_cast<unsigned int>(count) +
        static_cast<unsigned int>(added);
    memcpy(&count, &bits, sizeof count);
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

static __forceinline int EclPredecrementNativeWord(int &value)
{
    const unsigned int nativeWord = static_cast<unsigned int>(value) - 1u;
    memcpy(&value, &nativeWord, sizeof value);
    return value;
}

int EclVmScriptDatabase::AddScriptData(void *scriptData)
{
    const unsigned int SCPT_MAGIC = 0x54504353u;
    (*EclFileSlotAtWord(&files[0], fileCount)) = static_cast<EclVmScriptFileHeader *>(scriptData);
    EclVmScriptFileHeader *volatile *const fileSlot = EclFileSlotAtWord(&files[0], fileCount);
    EclVmScriptFileHeader *file = *fileSlot;
    if (file->magic != SCPT_MAGIC)
    {
        *fileSlot = NULL;
        return -1;
    }
    if (file->version != 1)
    {
        *fileSlot = NULL;
        return -1;
    }

    file = *fileSlot;
    // The input parameter becomes the offset cursor. Keep its loop reads
    // in that stack slot; ordinary writes can still feed the name calculation.
    void *const volatile &offsetCursor = scriptData;
    EclVmSubroutineEntry *oldEntries = subroutines;
    EclVmSubroutineEntry *previousEntries = subroutines;
    scriptData = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(file) +
        static_cast<unsigned int>(sizeof(EclVmScriptFileHeader)) +
        static_cast<unsigned int>(file->includeLength));
    const char *name = reinterpret_cast<const char *>(
        reinterpret_cast<unsigned int>(scriptData) +
        static_cast<unsigned int>(file->subroutineCount) *
        static_cast<unsigned int>(sizeof(EclVmSerializedSubroutineOffsetView)));
    AddEclSubroutineCount(subroutineCount, file->subroutineCount);
    EclVmSubroutineEntry *const newEntries =
        static_cast<EclVmSubroutineEntry *>(
        malloc(subroutineCount * sizeof(EclVmSubroutineEntry)));
    subroutines = newEntries;

    if (oldEntries == NULL)
    {
        int index = 0;
        if (subroutineCount > 0)
        {
            do
            {
                EclMutableEntryAtWord(subroutines, index)->header =
                    reinterpret_cast<unsigned char *>(
                        reinterpret_cast<unsigned int>((*EclFileSlotAtWord(&files[0], fileCount))) +
                        static_cast<unsigned int>(static_cast<EclVmSerializedSubroutineOffsetView *>(offsetCursor)->instructionOffset));
                EclMutableEntryAtWord(subroutines, index)->name = name;
                name = reinterpret_cast<const char *>(
                    reinterpret_cast<unsigned int>(name) +
                    static_cast<unsigned int>(strlen(name)) + 1u);
                const unsigned int indexBits =
                    static_cast<unsigned int>(index) + 1u;
                memcpy(&index, &indexBits, sizeof index);
                scriptData = reinterpret_cast<void *>(
                    reinterpret_cast<unsigned int>(offsetCursor) +
                    static_cast<unsigned int>(sizeof(EclVmSerializedSubroutineOffsetView)));
            } while (index < subroutineCount);
        }
    }
    else
    {
        // This size is also native 32-bit subtraction, including wrap.
        int sortedCount;
        // The native difference is a DWORD, even when its signed bit is set.
        const unsigned int countBits =
            static_cast<unsigned int>(subroutineCount) -
            static_cast<unsigned int>((*EclFileSlotAtWord(&files[0], fileCount))->subroutineCount);
        memcpy(&sortedCount, &countBits, sizeof sortedCount);
        memcpy(newEntries, oldEntries,
               sortedCount * sizeof(EclVmSubroutineEntry));
        if (previousEntries != NULL)
            free(previousEntries);

        int added = 0;
        if ((*EclFileSlotAtWord(&files[0], fileCount))->subroutineCount > 0u)
        {
            do
            {
                int insertionIndex = 0;
                if (sortedCount > 0)
                {
                    EclVmSubroutineEntry *entry = subroutines;
                    do
                    {
                        if (strcmp(name, entry->name) <= 0)
                            break;
                        const unsigned int insertionIndexBits =
                            static_cast<unsigned int>(insertionIndex) + 1u;
                        memcpy(&insertionIndex, &insertionIndexBits, sizeof insertionIndex);
                        entry = reinterpret_cast<EclVmSubroutineEntry *>(
                            reinterpret_cast<unsigned int>(entry) +
                            static_cast<unsigned int>(sizeof(EclVmSubroutineEntry)));
                    } while (insertionIndex < sortedCount);
                }

                int index = subroutineCount;
                while (EclPredecrementNativeWord(index) > insertionIndex)
                {
                    EclVmSubroutineEntry *const destination =
                        EclMutableEntryAtWord(subroutines, index);
                    const EclVmSubroutineEntry *const previous =
                        EclMutableEntryAtWord(subroutines,
                            index - 1);
                    *destination = *previous;
                }

                EclMutableEntryAtWord(subroutines, insertionIndex)->header =
                    reinterpret_cast<unsigned char *>(
                        reinterpret_cast<unsigned int>((*EclFileSlotAtWord(&files[0], fileCount))) +
                        static_cast<unsigned int>(static_cast<EclVmSerializedSubroutineOffsetView *>(offsetCursor)->instructionOffset));
                EclMutableEntryAtWord(subroutines, insertionIndex)->name = name;
                name = reinterpret_cast<const char *>(
                    reinterpret_cast<unsigned int>(name) +
                    static_cast<unsigned int>(strlen(name)) + 1u);
                scriptData = reinterpret_cast<void *>(
                    reinterpret_cast<unsigned int>(offsetCursor) +
                    static_cast<unsigned int>(sizeof(EclVmSerializedSubroutineOffsetView)));
                const unsigned int sortedCountBits =
                    static_cast<unsigned int>(sortedCount) + 1u;
                memcpy(&sortedCount, &sortedCountBits, sizeof sortedCount);
                const unsigned int addedBits =
                    static_cast<unsigned int>(added) + 1u;
                memcpy(&added, &addedBits, sizeof added);
            } while (added < (*EclFileSlotAtWord(&files[0], fileCount))->subroutineCount);
        }
    }

    // The original increments the file-slot index as one i386 word, even
    // across the signed boundary. Publish the wrapped word after retaining
    // the old index for the package-load dispatch and result.
    const int result = fileCount;
    const unsigned int fileCountBits =
        static_cast<unsigned int>(fileCount) + 1u;
    memcpy(&fileCount, &fileCountBits, sizeof fileCount);
    file = (*EclFileSlotAtWord(&files[0], result));
    if (file->includeLength != 0)
    {
        LoadPackage(
            reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(file) +
                static_cast<unsigned int>(sizeof(EclVmScriptFileHeader))));
    }
    return result;
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


int EclVmContext::StartSubroutine(
    EclVmContext *caller, unsigned int firstArgument)
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
    if (argumentIndex < (*callerInstruction)->operandCount)
    {
        unsigned int valueOffset = metadataOffset +
            static_cast<int>(offsetof(EclVmCallArgumentView, integer));
        unsigned char *argument = reinterpret_cast<unsigned char *>(
            reinterpret_cast<unsigned int>(destination->stack.data) +
            static_cast<unsigned int>(argumentOffset));
        for (; argumentIndex < (*callerInstruction)->operandCount;
             argument = reinterpret_cast<unsigned char *>(
                 reinterpret_cast<unsigned int>(argument) + sizeof(int)),
             argumentIndex = static_cast<int>(static_cast<unsigned int>(argumentIndex) + 1u),
             valueOffset += sizeof(EclVmCallArgumentView),
             metadataOffset += sizeof(EclVmCallArgumentView))
        {
            const char sourceType =
                reinterpret_cast<const EclVmCallArgumentView *>(
                    reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>((*callerInstruction)->operands) +
                    metadataOffset))->sourceType;

            // CALL arguments follow a counted name; scalarOperands[1] is
            // only their first-word overlay, not a bounded C++ array.
            // Preserve the original low-32-bit dword byte-address walk.
            if (sourceType == 'f' || sourceType == 'g')
            {
                EclVmScalar value;
                value.integer =
                    OperandWord(*callerInstruction,
                        valueOffset / sizeof(int))->integer;
                value.real = ReadFloatValue(argumentIndex, caller, value.real);
                if (reinterpret_cast<const EclVmCallArgumentView *>(
                    reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>((*callerInstruction)->operands) +
                    metadataOffset))->destinationType == 'f')
                    reinterpret_cast<EclVmScalar *>(argument)->real = value.real;
                else
                    reinterpret_cast<EclVmScalar *>(argument)->integer =
                        static_cast<int>(value.real);
            }
            else
            {
                EclVmScalar value;
                value.integer =
                    OperandWord(*callerInstruction,
                        valueOffset / sizeof(int))->integer;
                value.integer = caller->ReadIntValue(
                    argumentIndex, value.integer);
                if (reinterpret_cast<const EclVmCallArgumentView *>(
                    reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>((*callerInstruction)->operands) +
                    metadataOffset))->destinationType != 'f')
                    reinterpret_cast<EclVmScalar *>(argument)->integer = value.integer;
                else
                    reinterpret_cast<EclVmScalar *>(argument)->real =
                        static_cast<float>(value.integer);
            }

        }
    }

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
            0, sizeof(caller->currentTime), &caller->currentTime);
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

    EclVmContext *const previousContext = caller->host->activeContext;
    caller->host->activeContext = destination;
    EclVmHost *const host = caller->host;
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
        node->context->instruction = NULL;
        node = next;
    }
}


// Model the disabled scalar trace suggested by FORMAT's unused text copy.
// Argument evaluation remains observable; the original trace definition is unknown.
static void EclDebugTrace(const char *format)
{
}

static void EclDebugTrace(const char *format, int value)
{
}

static void EclDebugTrace(const char *format, float value)
{
}


int EclVmContext::Run(float timeDelta)
{
    EclVmInstruction **const instructionCursor = &instruction;

    if (*instructionCursor == NULL)
        return -1;

    while (static_cast<float>((*instructionCursor)->time) <= currentTime) {
        EclVmInstruction *current = *instructionCursor;
        int instructionOffset;
        EclVmInstruction *advanceInstruction;

        if ((difficultyMask & current->difficultyMask) != 0) {
            int floatResult;
            int logicalResult;
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
                    instruction = NULL;
                    return -1;
                }

                stack.Pop(
                    0, sizeof(*instructionCursor), instructionCursor);
                stack.Pop(0, sizeof(currentTime), &currentTime);
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
                // Retail ADD ECX,4 then SHR ECX,2 uses the low 32-bit word
                // even if the encoded signed inline length would overflow.
                const unsigned int operandIndex =
                    (static_cast<unsigned int>(current->spawnThreadOperands.inlineBytes)
                        + 4u) >> 2;
                host->SpawnThread(
                    ReadIntValue(1, OperandInt(current, operandIndex)), 1);
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
                    thread->context->threadFlag0 = 1;
                break;
            }

            case ECL_VM_CLEAR_THREAD_FLAG:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                    thread->context->threadFlag0 = 0;
                break;
            }

            case ECL_VM_SET_THREAD_CONTROL:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                {
                    EclVmContext *const threadContext = thread->context;
                    const int control = ReadInt(1);
                    threadContext->threadControl = control;
                }
                break;
            }

            case ECL_VM_CALL:
                if (StartSubroutine(this, 0) != 0)
                    return -1;
                continue;

            case ECL_VM_JUMP_IF_TRUE:
                if (PopInt(this) != 0)
                    goto jump_instruction;
                break;

            case ECL_VM_JUMP_IF_FALSE:
                if (PopInt(this) == 0)
                    goto jump_instruction;
                break;

            case ECL_VM_JUMP:
jump_instruction:
            {
                advanceInstruction = *instructionCursor;
                currentTime = static_cast<float>(advanceInstruction->jumpOperands.targetTime);
                instructionOffset = advanceInstruction->jumpOperands.instructionByteOffset;
                goto advance_instruction;
            }

            case ECL_VM_SUBTRACT_TIME:
                currentTime -= static_cast<float>(ReadInt(0));
                break;

            case ECL_VM_ENTER_FRAME:
                stack.EnterFrame(ReadInt(0));
                break;

            case ECL_VM_LEAVE_FRAME:
                stack.LeaveFrame();
                break;

            case ECL_VM_PUSH_INT:
                PushInt(this, ReadInt(0));
                break;

            case ECL_VM_STORE_INT:
                stack.Pop('i', sizeof(EclVmScalar),
                    &reinterpret_cast<EclVmScalar *>(ResolveInt(0))->integer);
                break;

            case ECL_VM_PUSH_FLOAT:
                PushFloat(this, ReadFloat(0));
                break;

            case ECL_VM_STORE_FLOAT:
                stack.Pop('f', sizeof(EclVmScalar),
                    &reinterpret_cast<EclVmScalar *>(ResolveFloat(0))->real);
                break;

            case ECL_VM_ADD_INT:
            {
                EclVmScalar right;
                stack.Pop('i', sizeof(right.integer), &right.integer);
                EclVmScalar left;
                stack.Pop('i', sizeof(left.integer), &left.integer);
                // The native integer VM operates on 32-bit words: add,
                // subtract and multiply wrap even at the signed limits.
                left.bits += right.bits;
                stack.Push('i', sizeof(left.integer), &left.integer);
                break;
            }

            case ECL_VM_SUBTRACT_INT:
            {
                EclVmScalar right;
                stack.Pop('i', sizeof(right.integer), &right.integer);
                EclVmScalar left;
                stack.Pop('i', sizeof(left.integer), &left.integer);
                left.bits -= right.bits;
                stack.Push('i', sizeof(left.integer), &left.integer);
                break;
            }

            case ECL_VM_MULTIPLY_INT:
            {
                EclVmScalar right;
                stack.Pop('i', sizeof(right.integer), &right.integer);
                EclVmScalar left;
                stack.Pop('i', sizeof(left.integer), &left.integer);
                left.bits *= right.bits;
                stack.Push('i', sizeof(left.integer), &left.integer);
                break;
            }

            case ECL_VM_DIVIDE_INT:
            {
                EclVmScalar right;
                stack.Pop('i', sizeof(right.integer), &right.integer);
                EclVmScalar left;
                stack.Pop('i', sizeof(left.integer), &left.integer);
                left.integer /= right.integer;
                stack.Push('i', sizeof(left.integer), &left.integer);
                break;
            }

            case ECL_VM_MODULO_INT:
            {
                EclVmScalar right;
                stack.Pop('i', sizeof(right.integer), &right.integer);
                EclVmScalar left;
                stack.Pop('i', sizeof(left.integer), &left.integer);
                left.integer %= right.integer;
                stack.Push('i', sizeof(left.integer), &left.integer);
                break;
            }

            case ECL_VM_ADD_FLOAT:
            {
                EclVmScalar right;
                stack.Pop('f', sizeof(right.real), &right.real);
                EclVmScalar left;
                stack.Pop('f', sizeof(left.real), &left.real);
                left.real += right.real;
                stack.Push('f', sizeof(left.real), &left.real);
                break;
            }

            case ECL_VM_SUBTRACT_FLOAT:
            {
                EclVmScalar right;
                stack.Pop('f', sizeof(right.real), &right.real);
                EclVmScalar left;
                stack.Pop('f', sizeof(left.real), &left.real);
                left.real -= right.real;
                stack.Push('f', sizeof(left.real), &left.real);
                break;
            }

            case ECL_VM_MULTIPLY_FLOAT:
            {
                EclVmScalar right;
                stack.Pop('f', sizeof(right.real), &right.real);
                EclVmScalar left;
                stack.Pop('f', sizeof(left.real), &left.real);
                left.real *= right.real;
                stack.Push('f', sizeof(left.real), &left.real);
                break;
            }

            case ECL_VM_DIVIDE_FLOAT:
            {
                EclVmScalar right;
                stack.Pop('f', sizeof(right.real), &right.real);
                EclVmScalar left;
                stack.Pop('f', sizeof(left.real), &left.real);
                left.real /= right.real;
                stack.Push('f', sizeof(left.real), &left.real);
                break;
            }

            case ECL_VM_EQUAL_INT:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer == right.integer);
                break;
            }

            case ECL_VM_NOT_EQUAL_INT:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer != right.integer);
                break;
            }

            case ECL_VM_LESS_INT:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer < right.integer);
                break;
            }

            case ECL_VM_LESS_EQUAL_INT:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer <= right.integer);
                break;
            }

            case ECL_VM_GREATER_INT:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer > right.integer);
                break;
            }

            case ECL_VM_GREATER_EQUAL_INT:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer >= right.integer);
                break;
            }

            case ECL_VM_NOT_INT:
                PushInt(this, !PopInt(this));
                break;

            case ECL_VM_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                floatResult = left == right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_NOT_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                floatResult = left != right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_LESS_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                floatResult = left < right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_LESS_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                floatResult = left <= right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_GREATER_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                floatResult = left > right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_GREATER_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                floatResult = left >= right;
                PushInt(this, floatResult);
                break;
            }

            case ECL_VM_NOT_FLOAT:
                floatResult = PopFloat(this) == 0.0f;
                PushInt(this, floatResult);
                break;

            case ECL_VM_LOGICAL_OR:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                logicalResult = left.integer != 0 || right.integer != 0;
                PushInt(this, logicalResult);
                break;
            }

            case ECL_VM_LOGICAL_AND:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                logicalResult = left.integer != 0 && right.integer != 0;
                PushInt(this, logicalResult);
                break;
            }

            case ECL_VM_BITWISE_XOR:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer ^ right.integer);
                break;
            }

            case ECL_VM_BITWISE_OR:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer | right.integer);
                break;
            }

            case ECL_VM_BITWISE_AND:
            {
                EclVmScalar right;
                right.integer = PopInt(this);
                EclVmScalar left;
                left.integer = PopInt(this);
                PushInt(this, left.integer & right.integer);
                break;
            }

            case ECL_VM_NEGATE_INT:
            {
                EclVmScalar value;
                stack.Pop('i', sizeof(value.integer), &value.integer);
                value.bits = 0u - value.bits;
                stack.Push('i', sizeof(value.integer), &value.integer);
                break;
            }

            case ECL_VM_NEGATE_FLOAT_STORAGE:
            {
                // 0x0044F851 executes integer NEG on the raw float dword and
                // then pushes that dword with type 'f'.  This is not x87 FCHS.
                EclVmScalar value;
                stack.Pop('f', sizeof(value), &value);
                // Raw DWORD negation is modulo32, including negative-zero bits.
                value.bits = 0u - value.bits;
                stack.Push('f', sizeof(value), &value);
                break;
            }

            case ECL_VM_POST_DECREMENT_INT:
            {
                EclVmScalar value;
                value.integer = ReadInt(0);
                int *destination = ResolveInt(0);
                // Native DWORD decrement, published through the actual int
                // destination. The pinned VC7.1 Win32 conversion preserves
                // the low 32 bits; no unrelated-type pointer alias is used.
                const unsigned int nextWord = value.bits - 1u;
                *destination = static_cast<int>(nextWord);
                stack.Push('i', sizeof(value.integer), &value.integer);
                break;
            }

            case ECL_VM_SINE:
                PushFloat(this, static_cast<float>(sin(PopFloat(this))));
                break;

            case ECL_VM_COSINE:
                PushFloat(this, static_cast<float>(cos(PopFloat(this))));
                break;

            case ECL_VM_POLAR_TO_CARTESIAN:
            {
                EclVmPolarVectorView vector;
                vector.FromAngleMagnitude(
                    EnemyWrapAngle(ReadFloat(2)), ReadFloat(3));
                *ResolveFloat(0) = vector.x;
                *ResolveFloat(1) = vector.y;
                break;
            }

            case ECL_VM_LENGTH_SQUARED:
            {
                // Retail calls ResolveFloat(0) after the two source reads,
                // but before either square is evaluated. A direct assignment
                // expression leaves that side-effect ordering implicit.
                // The original x87 sequence multiplies y first, then x.
                const volatile float x = ReadFloat(1);
                const float y = ReadFloat(2);
                float *const output = ResolveFloat(0);
                *output = y * y + x * x;
                break;
            }

            case ECL_VM_NORMALIZE_ANGLE:
                *ResolveFloat(0) = EnemyWrapAngle(ReadFloat(0));
                break;

            case ECL_VM_POINT_ANGLE:
            {
                const float dx = ReadFloat(2) - ReadFloat(0);
                const float dy = ReadFloat(3) - ReadFloat(1);
                *ResolveFloat(0) =
                    static_cast<float>(atan2(dy, dx));
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
                    case 'f':
                    {
                        const int inlineBytes =
                            reinterpret_cast<const EclVmFormatInlinePrefixView *>(
                                instruction->operands)->inlineBytes;
                        const EclVmFormatArgumentView *argument =
                            reinterpret_cast<const EclVmFormatArgumentView *>(
                                reinterpret_cast<unsigned int>(
                                    instruction->operands) +
                                static_cast<unsigned int>(sizeof(int)) +
                                static_cast<unsigned int>(inlineBytes) +
                                static_cast<unsigned int>(argumentIndex) *
                                static_cast<unsigned int>(sizeof(EclVmFormatArgumentView)));
                        // FORMAT is a counted scalar word stream, not a
                        // C++ one-element scalarOperands array. The original
                        // i386 index * 4 address arithmetic wraps at 32 bits.
                        const char argumentType = argument->type;
                        if (argumentType == 'f' || argumentType == 'g') {
                            EclDebugTrace(scratch, ReadFloatValue(
                                argumentIndex + 1u, this,
                                reinterpret_cast<const EclVmScalar *>(
                                    reinterpret_cast<unsigned int>(instruction) +
                                    static_cast<unsigned int>(
                                        offsetof(EclVmInstruction, scalarOperands)) +
                                    (static_cast<unsigned int>(
                                        inlineBytes / static_cast<int>(sizeof(int))) +
                                     2u + static_cast<unsigned int>(argumentIndex) *
                                        static_cast<unsigned int>(
                                            sizeof(EclVmFormatArgumentView) /
                                            sizeof(int))) * 4u)->real));
                            ++argumentIndex;
                        } else {
                            EclDebugTrace(scratch, ReadIntValue(
                                argumentIndex + 1u,
                                reinterpret_cast<const EclVmScalar *>(
                                    reinterpret_cast<unsigned int>(instruction) +
                                    static_cast<unsigned int>(
                                        offsetof(EclVmInstruction, scalarOperands)) +
                                    (static_cast<unsigned int>(
                                        inlineBytes / static_cast<int>(sizeof(int))) +
                                     2u + static_cast<unsigned int>(argumentIndex) *
                                        static_cast<unsigned int>(
                                            sizeof(EclVmFormatArgumentView) /
                                            sizeof(int))) * 4u)->integer));
                            ++argumentIndex;
                        }
                        break;
                    }
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

        advanceInstruction = *instructionCursor;
        instructionOffset = advanceInstruction->size;
advance_instruction:
        *instructionCursor = advanceInstruction->AtByteOffset(
            instructionOffset);
    }

    currentTime += timeDelta;
    return 0;
}
