#ifndef TH10_ECL_VM_HPP
#define TH10_ECL_VM_HPP

#include <stddef.h>


struct EclVmInstruction
{
    int time;
    short opcode;
    unsigned short size;
    unsigned short operandFlags;
    unsigned char difficultyMask;
    unsigned char operandCount;
    unsigned char unknown0C[4];
    unsigned char operands[1];
};

class EclVmHost
{
public:
    virtual int DispatchEclInstruction() = 0;
    virtual int ReadEclInt(int id) = 0;
    virtual int *ResolveEclInt(int id) = 0;
    virtual float ReadEclFloat(int id) = 0;
    virtual float *ResolveEclFloat(int id) = 0;
};

struct EclVmStackView
{
    unsigned char data[0x1000];
    int stackTop;
    int frameBase;

    int Push(unsigned char type, int size, const void *value);
    int Pop(unsigned char type, int size, void *value);
    int EnterFrame(int localBytes);
    int LeaveFrame();
};

struct EclVmContext
{
    float currentTime;
    EclVmInstruction *instruction;
    EclVmStackView stack;
    int threadId;
    EclVmHost *host;
    int threadControl;
    unsigned char difficultyMask;
    unsigned char unknown101D[3];
    unsigned int flags;

    int Run(float timeDelta);
};

typedef char EclVmInstruction_opcode_offset[
    offsetof(EclVmInstruction, opcode) == 0x4 ? 1 : -1];
typedef char EclVmInstruction_size_offset[
    offsetof(EclVmInstruction, size) == 0x6 ? 1 : -1];
typedef char EclVmInstruction_operand_flags_offset[
    offsetof(EclVmInstruction, operandFlags) == 0x8 ? 1 : -1];
typedef char EclVmInstruction_difficulty_offset[
    offsetof(EclVmInstruction, difficultyMask) == 0xA ? 1 : -1];
typedef char EclVmInstruction_operand_count_offset[
    offsetof(EclVmInstruction, operandCount) == 0xB ? 1 : -1];
typedef char EclVmInstruction_operands_offset[
    offsetof(EclVmInstruction, operands) == 0x10 ? 1 : -1];

typedef char EclVmContext_instruction_offset[
    offsetof(EclVmContext, instruction) == 0x4 ? 1 : -1];
typedef char EclVmContext_stack_offset[
    offsetof(EclVmContext, stack) == 0x8 ? 1 : -1];
typedef char EclVmContext_stack_top_offset[
    offsetof(EclVmContext, stack) + offsetof(EclVmStackView, stackTop) == 0x1008
        ? 1 : -1];
typedef char EclVmContext_frame_base_offset[
    offsetof(EclVmContext, stack) + offsetof(EclVmStackView, frameBase) == 0x100C
        ? 1 : -1];
typedef char EclVmContext_thread_id_offset[
    offsetof(EclVmContext, threadId) == 0x1010 ? 1 : -1];
typedef char EclVmContext_host_offset[
    offsetof(EclVmContext, host) == 0x1014 ? 1 : -1];
typedef char EclVmContext_thread_control_offset[
    offsetof(EclVmContext, threadControl) == 0x1018 ? 1 : -1];
typedef char EclVmContext_difficulty_offset[
    offsetof(EclVmContext, difficultyMask) == 0x101C ? 1 : -1];
typedef char EclVmContext_flags_offset[
    offsetof(EclVmContext, flags) == 0x1020 ? 1 : -1];
typedef char EclVmContext_size[sizeof(EclVmContext) == 0x1024 ? 1 : -1];
typedef char EclVmStackView_size[sizeof(EclVmStackView) == 0x1008 ? 1 : -1];

#endif
