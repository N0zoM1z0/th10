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

class EclVmHost;
struct EclVmScriptDatabase;
struct EclVmThreadNode;

struct EclVmStackView
{
    unsigned char data[0x1000];
    int stackTop;
    int frameBase;

    EclVmStackView();
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

    int ReadInt(unsigned int index);
    float ReadFloat(unsigned int index);
    int ReadIntValue(unsigned int flagIndex, int value);
    float ReadFloatValue(unsigned int flagIndex, float value);
    int *ResolveInt(unsigned int index);
    __declspec(noinline) float *ResolveFloat(unsigned int index);
    int Run(float timeDelta);
    int StartSubroutine(EclVmContext *caller, unsigned int firstArgument);
};

struct EclVmThreadNode
{
    EclVmContext *context;
    EclVmThreadNode *next;
    EclVmThreadNode *previous;
};

struct EclVmSubroutineEntry
{
    const char *name;
    unsigned char *header;
};

struct EclVmScriptFileHeader
{
    unsigned int magic;
    unsigned short version;
    unsigned short includeLength;
    unsigned int includeOffset;
    unsigned int zero0C;
    unsigned short subroutineCount;
    unsigned short unknown12;
    unsigned int zero14[4];
};

struct EclVmScriptDatabase
{
    virtual int AddScriptData(void *scriptData);
    virtual int LoadPackage(const unsigned char *packageData) = 0;

    int fileCount;
    int subroutineCount;
    EclVmScriptFileHeader *files[32];
    EclVmSubroutineEntry *subroutines;
    unsigned char unknown090[0x1008];

    EclVmInstruction *FindSubroutine(const char *name);
};

class EclVmHost
{
public:
    virtual int DispatchEclInstruction() = 0;
    virtual int ReadEclInt(int id) = 0;
    virtual int *ResolveEclInt(int id) = 0;
    virtual float ReadEclFloat(int id) = 0;
    virtual float *ResolveEclFloat(int id) = 0;

    int Run(float timeDelta);
    void SpawnThread(int threadId, unsigned int firstArgument);
    EclVmThreadNode *FindThread(int threadId);
    void StopAllThreads();

    EclVmContext *activeContext;
    EclVmContext embeddedContext;
    EclVmScriptDatabase *scriptDatabase;
    EclVmThreadNode threadList;
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
typedef char EclVmThreadNode_size[
    sizeof(EclVmThreadNode) == 0x0C ? 1 : -1];
typedef char EclVmSubroutineEntry_size[
    sizeof(EclVmSubroutineEntry) == 0x08 ? 1 : -1];
typedef char EclVmScriptFileHeader_include_length_offset[
    offsetof(EclVmScriptFileHeader, includeLength) == 0x06 ? 1 : -1];
typedef char EclVmScriptFileHeader_subroutine_count_offset[
    offsetof(EclVmScriptFileHeader, subroutineCount) == 0x10 ? 1 : -1];
typedef char EclVmScriptFileHeader_size[
    sizeof(EclVmScriptFileHeader) == 0x24 ? 1 : -1];
typedef char EclVmScriptDatabase_file_count_offset[
    offsetof(EclVmScriptDatabase, fileCount) == 0x4 ? 1 : -1];
typedef char EclVmScriptDatabase_subroutine_count_offset[
    offsetof(EclVmScriptDatabase, subroutineCount) == 0x8 ? 1 : -1];
typedef char EclVmScriptDatabase_files_offset[
    offsetof(EclVmScriptDatabase, files) == 0x0C ? 1 : -1];
typedef char EclVmScriptDatabase_subroutines_offset[
    offsetof(EclVmScriptDatabase, subroutines) == 0x8C ? 1 : -1];
typedef char EclVmScriptDatabase_size[
    sizeof(EclVmScriptDatabase) == 0x1098 ? 1 : -1];
typedef char EclVmHost_active_context_offset[
    offsetof(EclVmHost, activeContext) == 0x4 ? 1 : -1];
typedef char EclVmHost_embedded_context_offset[
    offsetof(EclVmHost, embeddedContext) == 0x8 ? 1 : -1];
typedef char EclVmHost_script_database_offset[
    offsetof(EclVmHost, scriptDatabase) == 0x102C ? 1 : -1];
typedef char EclVmHost_thread_list_offset[
    offsetof(EclVmHost, threadList) == 0x1030 ? 1 : -1];
typedef char EclVmHost_size[sizeof(EclVmHost) == 0x103C ? 1 : -1];

#endif
