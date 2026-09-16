#include "EclVm.hpp"

#include <math.h>
#include <stdlib.h>
#include <string.h>


// This runtime service is a separate target function in the
// 0x0044DF70-0x004506D0 corridor. Its declaration lets this file retain the
// complete VM dispatch while its implementation remains a separate unit.
extern float EclVmNormalizeAngle(float angle);


EclVmStackView::EclVmStackView()
    : stackTop(0), frameBase(0)
{
}


int EclVmStackView::Push(
    unsigned char type, int size, const void *value)
{
    if (stackTop + size >= 0x1000)
        return -1;

    if (type != 0)
    {
        data[stackTop] = type;
        stackTop += 4;
    }

    memcpy(data + stackTop, value, size);
    stackTop += size;
    return 0;
}

int EclVmStackView::Pop(unsigned char type, int size, void *value)
{
    if (stackTop - size < 0)
        return -1;

    stackTop -= size;
    memcpy(value, data + stackTop, size);

    if (type != 0)
    {
        stackTop -= 4;
        const unsigned char storedType = data[stackTop];
        if (storedType != 'f')
        {
            if (storedType == 'i' && type == 'f')
            {
                *reinterpret_cast<float *>(value) = static_cast<float>(
                    *reinterpret_cast<int *>(value));
            }
        }
        else if (type == 'i')
        {
            *reinterpret_cast<int *>(value) = static_cast<int>(
                *reinterpret_cast<float *>(value));
        }
    }

    return 0;
}

int EclVmStackView::EnterFrame(int localBytes)
{
    const int previousTop = stackTop;
    const int localTop = previousTop + localBytes;
    if (localTop >= 0x1000)
        return -1;

    stackTop = localTop;
    if (stackTop + 4 < 0x1000)
    {
        *reinterpret_cast<int *>(data + stackTop) = frameBase;
        stackTop += 4;
    }
    frameBase = previousTop;
    return 0;
}

int EclVmStackView::LeaveFrame()
{
    const int previousFrame = frameBase;
    if (stackTop - 4 >= 0)
    {
        stackTop -= 4;
        frameBase = *reinterpret_cast<int *>(data + stackTop);
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

union EclVmScalar
{
    int integer;
    unsigned int bits;
    float real;
    EclVmInstruction *instruction;
};

static int OperandInt(const EclVmInstruction *instruction, unsigned int index)
{
    return reinterpret_cast<const int *>(instruction->operands)[index];
}

static float OperandFloat(
    const EclVmInstruction *instruction, unsigned int index)
{
    return reinterpret_cast<const float *>(instruction->operands)[index];
}

static bool IsOperandIndirect(
    const EclVmInstruction *instruction, unsigned int index)
{
    return (instruction->operandFlags & (1U << index)) != 0;
}

static __forceinline int PopInt(EclVmContext *context)
{
    int value;
    context->stack.Pop('i', sizeof(value), &value);
    return value;
}

static __forceinline float PopFloat(EclVmContext *context)
{
    float value;
    context->stack.Pop('f', sizeof(value), &value);
    return value;
}

// Target Run expands these scalar wrappers but retains calls to the separate
// EclVmStackView::Push owner.  Limit nested inlining only across this pair.
#pragma inline_depth(1)
static __forceinline int PushInt(EclVmContext *context, int value)
{
    return context->stack.Push('i', sizeof(value), &value);
}

static __forceinline int PushFloat(EclVmContext *context, float value)
{
    return context->stack.Push('f', sizeof(value), &value);
}
#pragma inline_depth(16)

static __forceinline void EvaluateFormatOperands(EclVmContext *context)
{
    const EclVmInstruction *instruction = context->instruction;
    const char *format = reinterpret_cast<const char *>(instruction) + 0x14;
    const char *cursor = format;
    char *scratch = static_cast<char *>(malloc(0x400));
    scratch[0] = '\0';

    unsigned char flagIndex = 1;
    int metadataOffset = 0;
    int valueWord = 6;
    while (cursor != NULL) {
        const char *percent = strchr(cursor, '%');
        if (percent == NULL)
            break;

        strcpy(scratch, cursor);
        scratch[percent - cursor] = '\0';
        const char conversion = percent[1];
        if (conversion != '%' && (conversion == 'd' || conversion == 'f')) {
            const int inlineBytes = OperandInt(instruction, 0);
            const char argumentType = *(
                reinterpret_cast<const char *>(instruction)
                + 0x14 + inlineBytes + metadataOffset);
            const int raw = reinterpret_cast<const int *>(instruction)[
                inlineBytes / 4 + valueWord];
            if (argumentType == 'f' || argumentType == 'g') {
                EclVmScalar value;
                value.integer = raw;
                context->ReadFloatValue(flagIndex, value.real);
            } else {
                context->ReadIntValue(flagIndex, raw);
            }
            metadataOffset += 8;
            valueWord += 2;
            ++flagIndex;
        }
        cursor = percent + 2;
    }
    free(scratch);
}

} // namespace


int EclVmContext::ReadIntValue(unsigned int flagIndex, int value)
{
    if (IsOperandIndirect(instruction, flagIndex))
    {
        if (value >= 0)
        {
            EclVmStackView *const operandStack = &stack;
            return *reinterpret_cast<int *>(
                operandStack->data + operandStack->frameBase + value);
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

int EclVmContext::ReadInt(unsigned int index)
{
    if (IsOperandIndirect(instruction, index))
    {
        const int value = OperandInt(instruction, index);
        if (value >= 0)
            return *reinterpret_cast<int *>(
                stack.data + stack.frameBase + value);
        if (value == -1)
        {
            int result;
            stack.Pop('i', sizeof(result), &result);
            return result;
        }
        return host->ReadEclInt(value);
    }
    return OperandInt(instruction, index);
}

float EclVmContext::ReadFloatValue(unsigned int flagIndex, float value)
{
    if (IsOperandIndirect(instruction, flagIndex))
    {
        if (value >= 0.0f)
            return *reinterpret_cast<float *>(
                stack.data + stack.frameBase + static_cast<int>(value));
        if (value == -1.0f)
        {
            stack.Pop('f', sizeof(value), &value);
        }
        else
        {
            return host->ReadEclFloat(static_cast<int>(value));
        }
    }
    return value;
}

float EclVmContext::ReadFloat(unsigned int index)
{
    if (IsOperandIndirect(instruction, index))
    {
        if (OperandFloat(instruction, index) >= 0.0f)
            return *reinterpret_cast<float *>(
                stack.data + stack.frameBase
                + static_cast<int>(OperandFloat(instruction, index)));
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

int *EclVmContext::ResolveInt(unsigned int index)
{
    if (IsOperandIndirect(instruction, index))
    {
        if (OperandInt(instruction, index) >= 0)
        {
            EclVmStackView *const operandStack = &stack;
            return reinterpret_cast<int *>(
                operandStack->data + operandStack->frameBase
                + OperandInt(instruction, index));
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
            return reinterpret_cast<float *>(
                operandStack->data + operandStack->frameBase
                + static_cast<int>(OperandFloat(instruction, index)));
        }
        return host->ResolveEclFloat(
            static_cast<int>(OperandFloat(instruction, index)));
    }
    return NULL;
}


int EclVmScriptDatabase::AddScriptData(void *scriptData)
{
    const unsigned int SCPT_MAGIC = 0x54504353u;
    files[fileCount] = static_cast<EclVmScriptFileHeader *>(scriptData);
    EclVmScriptFileHeader **const fileSlot = &files[fileCount];
    EclVmScriptFileHeader *file = *fileSlot;
    if (file->magic != SCPT_MAGIC || file->version != 1)
    {
        *fileSlot = NULL;
        return -1;
    }

    file = *fileSlot;
    EclVmSubroutineEntry *const previousEntries = subroutines;
    scriptData = reinterpret_cast<unsigned char *>(file)
        + sizeof(EclVmScriptFileHeader) + file->includeLength;
    unsigned int *offsets = static_cast<unsigned int *>(scriptData);
    const int totalCount = subroutineCount + file->subroutineCount;
    const char *name = reinterpret_cast<const char *>(
        offsets + file->subroutineCount);
    subroutineCount = totalCount;
    EclVmSubroutineEntry *const newEntries =
        static_cast<EclVmSubroutineEntry *>(
        malloc(subroutineCount * sizeof(EclVmSubroutineEntry)));
    subroutines = newEntries;

    if (previousEntries == NULL)
    {
        int index = 0;
        if (subroutineCount > 0)
        {
            do
            {
                subroutines[index].header =
                    reinterpret_cast<unsigned char *>(files[fileCount]) + *offsets;
                subroutines[index].name = name;
                name += strlen(name) + 1;
                ++index;
                ++offsets;
            } while (index < subroutineCount);
        }
    }
    else
    {
        int sortedCount = subroutineCount
            - files[fileCount]->subroutineCount;
        memcpy(newEntries, previousEntries,
               sortedCount * sizeof(EclVmSubroutineEntry));
        if (previousEntries != NULL)
            free(previousEntries);

        int added = 0;
        if (files[fileCount]->subroutineCount != 0)
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
                        ++insertionIndex;
                        ++entry;
                    } while (insertionIndex < sortedCount);
                }

                int index = subroutineCount;
                while (--index > insertionIndex)
                    subroutines[index] = subroutines[index - 1];

                subroutines[insertionIndex].header =
                    reinterpret_cast<unsigned char *>(files[fileCount]) + *offsets;
                subroutines[insertionIndex].name = name;
                name += strlen(name) + 1;
                ++offsets;
                ++sortedCount;
                ++added;
            } while (added < files[fileCount]->subroutineCount);
        }
    }

    const int result = fileCount++;
    file = files[result];
    if (file->includeLength != 0)
    {
        LoadPackage(
            reinterpret_cast<unsigned char *>(file)
            + sizeof(EclVmScriptFileHeader));
    }
    return result;
}


EclVmInstruction *EclVmScriptDatabase::FindSubroutine(const char *name)
{
    int lower = 0;
    int upper = subroutineCount - 1;
    while (lower <= upper)
    {
        const int middle = (upper - lower) / 2 + lower;
        const int comparison = strcmp(name, subroutines[middle].name);
        if (comparison == 0)
        {
            return reinterpret_cast<EclVmInstruction *>(
                subroutines[middle].header + 0x10);
        }
        if (comparison < 0)
            upper = middle - 1;
        else
            lower = middle + 1;
    }
    return NULL;
}


int EclVmStartSubroutine(
    EclVmContext *destination, EclVmContext *caller,
    unsigned int firstArgument)
{
    EclVmInstruction **const callerInstruction = &caller->instruction;
    EclVmInstruction *const call = *callerInstruction;
    const int inlineBytes = OperandInt(call, 0);
    int metadataOffset = inlineBytes + firstArgument * 4 + 4;
    const int previousTop = destination->stack.stackTop;
    int argumentOffset = previousTop + 8;
    int preservedValue = 0;

    if (previousTop == 0)
    {
        destination->stack.Push(0, sizeof(preservedValue), &preservedValue);
        argumentOffset = destination->stack.stackTop + 8;
    }

    int argumentIndex = firstArgument + 1;
    int valueOffset = metadataOffset + 4;
    unsigned char *argument = destination->stack.data + argumentOffset;
    while (argumentIndex < (*callerInstruction)->operandCount)
    {
        const char sourceType =
            (*callerInstruction)->operands[metadataOffset];

        if (sourceType == 'f' || sourceType == 'g')
        {
            EclVmScalar value;
            value.integer = *reinterpret_cast<const int *>(
                (*callerInstruction)->operands + (valueOffset & ~3));
            value.real = caller->ReadFloatValue(argumentIndex, value.real);
            if ((*callerInstruction)->operands[metadataOffset + 1] == 'f')
                *reinterpret_cast<float *>(argument) = value.real;
            else
                *reinterpret_cast<int *>(argument) =
                    static_cast<int>(value.real);
        }
        else
        {
            EclVmScalar value;
            value.integer = *reinterpret_cast<const int *>(
                (*callerInstruction)->operands + (valueOffset & ~3));
            value.integer = caller->ReadIntValue(
                argumentIndex, value.integer);
            if ((*callerInstruction)->operands[metadataOffset + 1] == 'f')
                *reinterpret_cast<float *>(argument) =
                    static_cast<float>(value.integer);
            else
                *reinterpret_cast<int *>(argument) = value.integer;
        }

        argument += 4;
        ++argumentIndex;
        valueOffset += 8;
        metadataOffset += 8;
    }

    if (previousTop == 0)
    {
        destination->stack.stackTop = 4;
        destination->stack.Push(0, sizeof(preservedValue), &preservedValue);
    }
    else
    {
        destination->stack.Pop(0, sizeof(preservedValue), &preservedValue);
        destination->stack.stackTop = previousTop;
        *reinterpret_cast<int *>(
            destination->stack.data + previousTop - 4) = preservedValue;
        destination->stack.Push(
            0, sizeof(caller->currentTime), &caller->currentTime);
    }
    destination->stack.Push(
        0, sizeof(*callerInstruction), callerInstruction);

    EclVmHost *const host = caller->host;
    EclVmContext *const previousContext = host->activeContext;
    host->activeContext = destination;
    host->activeContext->instruction = host->scriptDatabase->FindSubroutine(
        reinterpret_cast<const char *>(*callerInstruction) + 0x14);
    host->activeContext->currentTime = 0.0f;
    if (host->activeContext->instruction == NULL)
    {
        *callerInstruction = NULL;
        return -1;
    }
    host->activeContext = previousContext;
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

    EclVmStartSubroutine(context, activeContext, firstArgument);
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


int EclVmContext::Run(float timeDelta)
{
    EclVmInstruction **const instructionCursor = &instruction;
    int instructionOffset;

    if (*instructionCursor == NULL)
        return -1;

    while (*instructionCursor != NULL
           && static_cast<float>((*instructionCursor)->time) <= currentTime) {
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
                host->SpawnThread(
                    ReadIntValue(
                        1,
                        OperandInt(
                            current,
                            static_cast<unsigned int>(
                                OperandInt(current, 0) + 4) >> 2)),
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
                    thread->context->flags |= 1;
                break;
            }

            case ECL_VM_CLEAR_THREAD_FLAG:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                    thread->context->flags &= ~1U;
                break;
            }

            case ECL_VM_SET_THREAD_CONTROL:
            {
                EclVmThreadNode *thread = host->FindThread(ReadInt(0));
                if (thread != NULL)
                    thread->context->threadControl = ReadInt(1);
                break;
            }

            case ECL_VM_CALL:
                if (EclVmStartSubroutine(this, this, 0) != 0)
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
                currentTime = static_cast<float>(OperandInt(current, 1));
                instructionOffset = OperandInt(current, 0);
                goto advance_instruction;

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
                *ResolveInt(0) = PopInt(this);
                break;

            case ECL_VM_PUSH_FLOAT:
                PushFloat(this, ReadFloat(0));
                break;

            case ECL_VM_STORE_FLOAT:
                *ResolveFloat(0) = PopFloat(this);
                break;

            case ECL_VM_ADD_INT:
            {
                int right;
                stack.Pop('i', sizeof(right), &right);
                int left;
                stack.Pop('i', sizeof(left), &left);
                left += right;
                stack.Push('i', sizeof(left), &left);
                break;
            }

            case ECL_VM_SUBTRACT_INT:
            {
                int right;
                stack.Pop('i', sizeof(right), &right);
                int left;
                stack.Pop('i', sizeof(left), &left);
                left -= right;
                stack.Push('i', sizeof(left), &left);
                break;
            }

            case ECL_VM_MULTIPLY_INT:
            {
                int right;
                stack.Pop('i', sizeof(right), &right);
                int left;
                stack.Pop('i', sizeof(left), &left);
                left *= right;
                stack.Push('i', sizeof(left), &left);
                break;
            }

            case ECL_VM_DIVIDE_INT:
            {
                int right;
                stack.Pop('i', sizeof(right), &right);
                int left;
                stack.Pop('i', sizeof(left), &left);
                left /= right;
                stack.Push('i', sizeof(left), &left);
                break;
            }

            case ECL_VM_MODULO_INT:
            {
                int right;
                stack.Pop('i', sizeof(right), &right);
                int left;
                stack.Pop('i', sizeof(left), &left);
                left %= right;
                stack.Push('i', sizeof(left), &left);
                break;
            }

            case ECL_VM_ADD_FLOAT:
            {
                float right;
                stack.Pop('f', sizeof(right), &right);
                float left;
                stack.Pop('f', sizeof(left), &left);
                left += right;
                stack.Push('f', sizeof(left), &left);
                break;
            }

            case ECL_VM_SUBTRACT_FLOAT:
            {
                float right;
                stack.Pop('f', sizeof(right), &right);
                float left;
                stack.Pop('f', sizeof(left), &left);
                left -= right;
                stack.Push('f', sizeof(left), &left);
                break;
            }

            case ECL_VM_MULTIPLY_FLOAT:
            {
                float right;
                stack.Pop('f', sizeof(right), &right);
                float left;
                stack.Pop('f', sizeof(left), &left);
                left *= right;
                stack.Push('f', sizeof(left), &left);
                break;
            }

            case ECL_VM_DIVIDE_FLOAT:
            {
                float right;
                stack.Pop('f', sizeof(right), &right);
                float left;
                stack.Pop('f', sizeof(left), &left);
                left /= right;
                stack.Push('f', sizeof(left), &left);
                break;
            }

            case ECL_VM_EQUAL_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left == right);
                break;
            }

            case ECL_VM_NOT_EQUAL_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left != right);
                break;
            }

            case ECL_VM_LESS_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left < right);
                break;
            }

            case ECL_VM_LESS_EQUAL_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left <= right);
                break;
            }

            case ECL_VM_GREATER_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left > right);
                break;
            }

            case ECL_VM_GREATER_EQUAL_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left >= right);
                break;
            }

            case ECL_VM_NOT_INT:
                PushInt(this, !PopInt(this));
                break;

            case ECL_VM_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushInt(this, left == right);
                break;
            }

            case ECL_VM_NOT_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushInt(this, left != right);
                break;
            }

            case ECL_VM_LESS_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushInt(this, left < right);
                break;
            }

            case ECL_VM_LESS_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushInt(this, left <= right);
                break;
            }

            case ECL_VM_GREATER_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushInt(this, left > right);
                break;
            }

            case ECL_VM_GREATER_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushInt(this, left >= right);
                break;
            }

            case ECL_VM_NOT_FLOAT:
                PushInt(this, PopFloat(this) == 0.0f);
                break;

            case ECL_VM_LOGICAL_OR:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left != 0 || right != 0);
                break;
            }

            case ECL_VM_LOGICAL_AND:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left != 0 && right != 0);
                break;
            }

            case ECL_VM_BITWISE_XOR:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left ^ right);
                break;
            }

            case ECL_VM_BITWISE_OR:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left | right);
                break;
            }

            case ECL_VM_BITWISE_AND:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left & right);
                break;
            }

            case ECL_VM_NEGATE_INT:
            {
                int value;
                stack.Pop('i', sizeof(value), &value);
                value = -value;
                stack.Push('i', sizeof(value), &value);
                break;
            }

            case ECL_VM_NEGATE_FLOAT_STORAGE:
            {
                // 0x0044F851 executes integer NEG on the raw float dword and
                // then pushes that dword with type 'f'.  This is not x87 FCHS.
                EclVmScalar value;
                stack.Pop('f', sizeof(value), &value);
                value.integer = -value.integer;
                stack.Push('f', sizeof(value), &value);
                break;
            }

            case ECL_VM_POST_DECREMENT_INT:
            {
                int value = ReadInt(0);
                int *destination = ResolveInt(0);
                *destination = value - 1;
                stack.Push('i', sizeof(value), &value);
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
                const float magnitude = ReadFloat(3);
                const float angle = EclVmNormalizeAngle(ReadFloat(2));
                *ResolveFloat(0) =
                    static_cast<float>(cos(angle) * magnitude);
                *ResolveFloat(1) =
                    static_cast<float>(sin(angle) * magnitude);
                break;
            }

            case ECL_VM_LENGTH_SQUARED:
            {
                const float x = ReadFloat(1);
                const float y = ReadFloat(2);
                *ResolveFloat(0) = x * x + y * y;
                break;
            }

            case ECL_VM_NORMALIZE_ANGLE:
                *ResolveFloat(0) = EclVmNormalizeAngle(ReadFloat(0));
                break;

            case ECL_VM_POINT_ANGLE:
            {
                const float dx = ReadFloat(2) - ReadFloat(0);
                const float dy = ReadFloat(3) - ReadFloat(1);
                *ResolveFloat(0) =
                    static_cast<float>(atan2(dy, dx));
                break;
            }

            case ECL_VM_EVALUATE_FORMAT_OPERANDS:
                EvaluateFormatOperands(this);
                break;

            default:
                if (host->DispatchEclInstruction() == -1)
                    return 0;
                break;
            }
        }

        instructionOffset = (*instructionCursor)->size;
advance_instruction:
        *instructionCursor = reinterpret_cast<EclVmInstruction *>(
            reinterpret_cast<unsigned char *>(*instructionCursor)
            + instructionOffset);
    }

    currentTime += timeDelta;
    return 0;
}
