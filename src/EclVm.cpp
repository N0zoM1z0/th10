#include "EclVm.hpp"

#include <math.h>
#include <stdlib.h>
#include <string.h>


// These small runtime services are separate target functions in the
// 0x0044DF70-0x004506D0 corridor.  Their logical declarations let this file
// retain the complete VM dispatch while their private register ABIs remain a
// separate reconstruction unit.
extern int EclVmStartSubroutine(
    EclVmContext *destination, EclVmContext *caller,
    unsigned int firstArgument);
extern void EclVmSpawnThread(
    EclVmHost *host, int threadId, unsigned int firstArgument);
extern EclVmContext *EclVmFindThread(EclVmHost *host, int threadId);
extern void EclVmStopAllThreads(EclVmHost *host);
extern float EclVmNormalizeAngle(float angle);


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
    const int valueOffset = stackTop - size;
    if (valueOffset < 0)
        return -1;

    stackTop = valueOffset;
    memcpy(value, data + valueOffset, size);

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
    const int previousFrameOffset = stackTop - 4;
    if (previousFrameOffset >= 0)
    {
        stackTop = previousFrameOffset;
        frameBase = *reinterpret_cast<int *>(data + previousFrameOffset);
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

static int PopInt(EclVmContext *context)
{
    int value;
    context->stack.Pop('i', sizeof(value), &value);
    return value;
}

static float PopFloat(EclVmContext *context)
{
    float value;
    context->stack.Pop('f', sizeof(value), &value);
    return value;
}

static int PushInt(EclVmContext *context, int value)
{
    return context->stack.Push('i', sizeof(value), &value);
}

static int PushFloat(EclVmContext *context, float value)
{
    return context->stack.Push('f', sizeof(value), &value);
}

static void Jump(EclVmContext *context, const EclVmInstruction *instruction)
{
    context->currentTime = static_cast<float>(OperandInt(instruction, 1));
    context->instruction = reinterpret_cast<EclVmInstruction *>(
        reinterpret_cast<unsigned char *>(context->instruction)
        + OperandInt(instruction, 0));
}

static void EvaluateFormatOperands(EclVmContext *context)
{
    const EclVmInstruction *instruction = context->instruction;
    const char *format = reinterpret_cast<const char *>(instruction) + 0x14;
    const char *cursor = format;
    char *scratch = static_cast<char *>(malloc(0x400));
    scratch[0] = '\0';

    unsigned int flagIndex = 1;
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
        const int value = OperandInt(instruction, index);
        if (value >= 0)
            return reinterpret_cast<int *>(
                stack.data + stack.frameBase + value);
        return host->ResolveEclInt(value);
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


int EclVmContext::Run(float timeDelta)
{
    if (instruction == NULL)
        return -1;

    while (instruction != NULL
           && static_cast<float>(instruction->time) <= currentTime) {
        EclVmInstruction *current = instruction;
        bool advance = true;

        if ((difficultyMask & current->difficultyMask) != 0) {
            switch (current->opcode) {
            case ECL_VM_NOP:
                break;

            case ECL_VM_TERMINATE:
                instruction = NULL;
                return -1;

            case ECL_VM_RETURN:
            {
                stack.LeaveFrame();
                if (stack.stackTop == 0) {
                    instruction = NULL;
                    return -1;
                }

                EclVmScalar value;
                value.bits = 0;
                stack.Pop(0, sizeof(value), &value);
                instruction = value.instruction;
                stack.Pop(0, sizeof(value), &value);
                currentTime = value.real;
                if (instruction == NULL)
                    return -1;
                current = instruction;
                break;
            }

            case ECL_VM_CALL:
                if (EclVmStartSubroutine(this, this, 0) != 0)
                    return -1;
                continue;

            case ECL_VM_JUMP:
                Jump(this, current);
                advance = false;
                break;

            case ECL_VM_JUMP_IF_FALSE:
                if (PopInt(this) == 0) {
                    Jump(this, current);
                    advance = false;
                }
                break;

            case ECL_VM_JUMP_IF_TRUE:
                if (PopInt(this) != 0) {
                    Jump(this, current);
                    advance = false;
                }
                break;

            case ECL_VM_SPAWN_THREAD:
                EclVmSpawnThread(host, -1, 0);
                break;

            case ECL_VM_SPAWN_THREAD_WITH_ID:
            {
                const unsigned int idIndex =
                    static_cast<unsigned int>((OperandInt(current, 0) + 4) >> 2);
                const int id = ReadIntValue(
                    1, OperandInt(current, idIndex));
                EclVmSpawnThread(host, id, 1);
                break;
            }

            case ECL_VM_STOP_THREAD:
            {
                EclVmContext *thread = EclVmFindThread(host, ReadInt(0));
                if (thread != NULL)
                    thread->instruction = NULL;
                break;
            }

            case ECL_VM_SET_THREAD_FLAG:
            {
                EclVmContext *thread = EclVmFindThread(host, ReadInt(0));
                if (thread != NULL)
                    thread->flags |= 1;
                break;
            }

            case ECL_VM_CLEAR_THREAD_FLAG:
            {
                EclVmContext *thread = EclVmFindThread(host, ReadInt(0));
                if (thread != NULL)
                    thread->flags &= ~1U;
                break;
            }

            case ECL_VM_SET_THREAD_CONTROL:
            {
                EclVmContext *thread = EclVmFindThread(host, ReadInt(0));
                if (thread != NULL)
                    thread->threadControl = ReadInt(1);
                break;
            }

            case ECL_VM_STOP_ALL_THREADS:
                EclVmStopAllThreads(host);
                break;

            case ECL_VM_EVALUATE_FORMAT_OPERANDS:
                EvaluateFormatOperands(this);
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
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left + right);
                break;
            }

            case ECL_VM_ADD_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushFloat(this, left + right);
                break;
            }

            case ECL_VM_SUBTRACT_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left - right);
                break;
            }

            case ECL_VM_SUBTRACT_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushFloat(this, left - right);
                break;
            }

            case ECL_VM_MULTIPLY_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left * right);
                break;
            }

            case ECL_VM_MULTIPLY_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushFloat(this, left * right);
                break;
            }

            case ECL_VM_DIVIDE_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left / right);
                break;
            }

            case ECL_VM_DIVIDE_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushFloat(this, left / right);
                break;
            }

            case ECL_VM_MODULO_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left % right);
                break;
            }

            case ECL_VM_EQUAL_INT:
            {
                const int right = PopInt(this);
                const int left = PopInt(this);
                PushInt(this, left == right);
                break;
            }

            case ECL_VM_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
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

            case ECL_VM_NOT_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
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

            case ECL_VM_LESS_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
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

            case ECL_VM_LESS_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
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

            case ECL_VM_GREATER_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
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

            case ECL_VM_GREATER_EQUAL_FLOAT:
            {
                const float right = PopFloat(this);
                const float left = PopFloat(this);
                PushInt(this, left >= right);
                break;
            }

            case ECL_VM_NOT_INT:
                PushInt(this, !PopInt(this));
                break;

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

            case ECL_VM_POST_DECREMENT_INT:
            {
                const int value = ReadInt(0);
                *ResolveInt(0) = value - 1;
                PushInt(this, value);
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

            case ECL_VM_NORMALIZE_ANGLE:
                *ResolveFloat(0) = EclVmNormalizeAngle(ReadFloat(0));
                break;

            case ECL_VM_SUBTRACT_TIME:
                currentTime -= static_cast<float>(ReadInt(0));
                break;

            case ECL_VM_NEGATE_INT:
                PushInt(this, -PopInt(this));
                break;

            case ECL_VM_NEGATE_FLOAT_STORAGE:
            {
                // 0x0044F851 executes integer NEG on the raw float dword and
                // then pushes that dword with type 'f'.  This is not x87 FCHS.
                EclVmScalar value;
                value.real = PopFloat(this);
                value.integer = -value.integer;
                stack.Push('f', sizeof(value), &value);
                break;
            }

            case ECL_VM_LENGTH_SQUARED:
            {
                const float x = ReadFloat(1);
                const float y = ReadFloat(2);
                *ResolveFloat(0) = x * x + y * y;
                break;
            }

            case ECL_VM_POINT_ANGLE:
            {
                const float dx = ReadFloat(2) - ReadFloat(0);
                const float dy = ReadFloat(3) - ReadFloat(1);
                *ResolveFloat(0) =
                    static_cast<float>(atan2(dy, dx));
                break;
            }

            default:
                if (host->DispatchEclInstruction() == -1)
                    return 0;
                break;
            }
        }

        if (advance) {
            instruction = reinterpret_cast<EclVmInstruction *>(
                reinterpret_cast<unsigned char *>(current) + current->size);
        }
    }

    currentTime += timeDelta;
    return 0;
}
