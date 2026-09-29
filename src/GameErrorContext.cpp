// Natural C++ candidate adapted from th10-decomphelp-forN0/src/GameErrorContext.cpp.
// The target-bound Log and Fatal bodies are retained here only after direct
// IDA review and canonical VC7.1 replay. Fatal uses a natural same-TU entry
// context to preserve the target-observed private EDI receiver.
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char u8;

namespace th10
{

struct GameErrorContext
{
    char m_Buffer[0x2000];
    char *m_BufferEnd;
    u8 m_Fatal;

    char * __fastcall Log(char *fmt, ...);
    char * __fastcall Fatal(char *fmt, ...);
};

extern CRITICAL_SECTION g_CriticalSections[7];
extern u8 g_LogLockDepth;
extern GameErrorContext g_GameErrorContext;
extern char TH_ERR_DATA_CORRUPTED[];

char * __fastcall GameErrorContext::Log(char *fmt, ...)
{
    char tmpBuffer[0x2000];
    size_t tmpBufferSize;
    va_list args;
    GameErrorContext *self;

    self = this;

    EnterCriticalSection(&g_CriticalSections[3]);
    g_LogLockDepth++;
    va_start(args, fmt);
    vsprintf(tmpBuffer, fmt, args);
    tmpBufferSize = strlen(tmpBuffer);
    if (self->m_BufferEnd + tmpBufferSize < self->m_Buffer + 0x1fff)
    {
        strcpy(self->m_BufferEnd, tmpBuffer);
        self->m_BufferEnd += tmpBufferSize;
        self->m_BufferEnd[0] = '\0';
    }
    LeaveCriticalSection(&g_CriticalSections[3]);
    g_LogLockDepth--;
    return fmt;
}

char * __fastcall GameErrorContext::Fatal(char *fmt, ...)
{
    char tmpBuffer[0x200];
    size_t tmpBufferSize;
    va_list args;
    GameErrorContext *self;

    self = this;

    EnterCriticalSection(&g_CriticalSections[3]);
    g_LogLockDepth++;
    va_start(args, fmt);
    vsprintf(tmpBuffer, fmt, args);
    tmpBufferSize = strlen(tmpBuffer);
    if (self->m_BufferEnd + tmpBufferSize < self->m_Buffer + 0x1fff)
    {
        strcpy(self->m_BufferEnd, tmpBuffer);
        self->m_BufferEnd += tmpBufferSize;
        self->m_BufferEnd[0] = '\0';
    }
    self->m_Fatal = 1;
    LeaveCriticalSection(&g_CriticalSections[3]);
    g_LogLockDepth--;
    return fmt;
}

// Diagnostic-only natural entry context for the target's private Fatal
// receiver; the entry itself is not a target mapping.
int ProbeFatalRoot()
{
    return (int)g_GameErrorContext.Fatal(TH_ERR_DATA_CORRUPTED);
}

} // namespace th10
