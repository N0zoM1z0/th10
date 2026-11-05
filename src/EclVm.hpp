#ifndef TH10_ECL_VM_HPP
#define TH10_ECL_VM_HPP

#include <stddef.h>


// The VM value stack prefixes each typed scalar with a four-byte tag slot.
// Push writes only the first byte and skips the other three untouched;
// Pop reads that first byte. This is an observed stack wire layout.
struct EclVmStackTypeSlotView
{
    unsigned char type;
    unsigned char unknown001[3];
};
typedef char EclVmStackTypeSlotIs04[
    (sizeof(EclVmStackTypeSlotView) == sizeof(int) &&
     offsetof(EclVmStackTypeSlotView, type) == 0) ? 1 : -1];

// A saved stack dword is used for the caller's frame base (ENTER/LEAVE)
// and for StartSubroutine's separately preserved word. Unlike a typed
// scalar it has no four-byte type prefix; its full value is overwritten.
struct EclVmStackSavedWordView
{
    int value;
};
typedef char EclVmStackSavedWordNativeLayout[
    (sizeof(EclVmStackSavedWordView) == sizeof(int) &&
     offsetof(EclVmStackSavedWordView, value) == 0) ? 1 : -1];

// FORMAT's counted arguments are eight-byte records: a tag followed by
// an opaque three-byte gap and the original 32-bit scalar bit representation.
struct EclVmFormatArgumentView
{
    char type;
    unsigned char unknown001[3];
    union
    {
        int integer;
        float real;
    };
};
typedef char EclVmFormatArgumentSize08[
    (sizeof(EclVmFormatArgumentView) == 0x08 &&
     offsetof(EclVmFormatArgumentView, integer) == 0x04) ? 1 : -1];

// The FORMAT opcode begins with a byte count followed by its inline text;
// the remaining packed arguments start after the counted string. This is a
// flexible on-disk prefix, NOT a fixed-size owned C++ allocation.
struct EclVmFormatInlinePrefixView
{
    int inlineBytes;
    char text[1];
};
typedef char EclVmFormatInlinePrefixTextAt04[
    (offsetof(EclVmFormatInlinePrefixView, inlineBytes) == 0x00 &&
     offsetof(EclVmFormatInlinePrefixView, text) == sizeof(int)) ? 1 : -1];

// CALL operand descriptors use a source type, a requested destination
// type, two unknown bytes, then a 32-bit scalar. Unlike FORMAT arguments,
// both tag bytes are independently read by StartSubroutine.
struct EclVmCallArgumentView
{
    unsigned char sourceType;
    unsigned char destinationType;
    unsigned char unknown002[2];
    union
    {
        int integer;
        float real;
    };
};
typedef char EclVmCallArgumentNativeLayout[
    (sizeof(EclVmCallArgumentView) == 8 &&
     offsetof(EclVmCallArgumentView, sourceType) == 0 &&
     offsetof(EclVmCallArgumentView, destinationType) == 1 &&
     offsetof(EclVmCallArgumentView, integer) == 4) ? 1 : -1];

struct EclVmInstruction
{
    int time;
    short opcode;
    unsigned short size;
    unsigned short operandFlags;
    unsigned char difficultyMask;
    unsigned char operandCount;
    unsigned char unknown0C[4];
    // Variable-length operands share their first 32-bit word with the raw
    // byte stream; further words follow at dword stride. Encoding length
    // continues to come from the instruction's explicit byte size field.
    union
    {
        unsigned char operands[1];
        int integerOperands[1];
        float floatOperands[1];
    };

    // The encoded size and branch operands are measured in bytes, not
    // EclVmInstruction objects. Keep signed offsets for backward jumps.
    EclVmInstruction *AtByteOffset(int distance)
    {
        return reinterpret_cast<EclVmInstruction *>(
            reinterpret_cast<unsigned char *>(this) + distance);
    }
};
typedef char EclVmInstructionHeaderOffsets[
    (offsetof(EclVmInstruction, opcode) == 0x04 &&
     offsetof(EclVmInstruction, size) == 0x06 &&
     offsetof(EclVmInstruction, difficultyMask) == 0x0A &&
     offsetof(EclVmInstruction, operands) == 0x10 &&
     offsetof(EclVmInstruction, integerOperands) == 0x10 &&
     offsetof(EclVmInstruction, floatOperands) == 0x10) ? 1 : -1];

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
    // Native ECL thread signal opcodes set/clear only bit zero of +0x1020.
    // The other 31 bits are deliberately unnamed and left untouched.
    union
    {
        unsigned int flags;
        struct
        {
            unsigned int threadFlag0 : 1;
            unsigned int unknownFlags1To31 : 31;
        };
    };

    int ReadInt(unsigned int index);
    float ReadFloat(unsigned int index);
    int ReadIntValue(unsigned int flagIndex, int value);
    static float __stdcall ReadFloatValue(
        unsigned int flagIndex, EclVmContext *context, float value);
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

// A serialized subroutine has sixteen bytes of native header before its
// first variable-length instruction. The prefix contents are still opaque;
// the 0x10 instruction origin is established by the exact search leaf.
struct EclVmSubroutineStreamView
{
    unsigned char unknown000[0x10];
    EclVmInstruction firstInstruction[1];
};
typedef char EclVmSubroutineFirstInstructionAt10[
    (offsetof(EclVmSubroutineStreamView, firstInstruction) == 0x10) ? 1 : -1];

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
// An SCPT file holds one 32-bit instruction offset per subroutine after
// its 0x24-byte header and counted include payload. The counted name stream
// immediately follows the offsets; it is not a C++ pointer array.
struct EclVmSerializedSubroutineOffsetView
{
    unsigned int instructionOffset;
};
typedef char EclVmSerializedSubroutineOffsetIs04[
    (sizeof(EclVmSerializedSubroutineOffsetView) == sizeof(unsigned int) &&
     offsetof(EclVmSerializedSubroutineOffsetView, instructionOffset) == 0 &&
     sizeof(EclVmScriptFileHeader) == 0x24) ? 1 : -1];

struct EnemyEclHostBaseView;
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
    EnemyEclHostBaseView *CreateHost(const char *name);
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
