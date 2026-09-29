// Natural C++ candidate adapted from th10-decomphelp-forN0/src/GameErrorContext.cpp.
// The target-bound Log body is retained here only after direct IDA review and
// canonical VC7.1 replay; Fatal remains an unproven private-ABI candidate.
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
    void Fatal(char *fmt, ...);
};

extern CRITICAL_SECTION g_CriticalSections[7];
extern u8 g_LogLockDepth;

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

void GameErrorContext::Fatal(char *fmt, ...)
{
    char tmpBuffer[0x200];
    size_t tmpBufferSize;
    va_list args;

    EnterCriticalSection(&g_CriticalSections[3]);
    g_LogLockDepth++;
    va_start(args, fmt);
    vsprintf(tmpBuffer, fmt, args);
    tmpBufferSize = strlen(tmpBuffer);
    if (this->m_BufferEnd + tmpBufferSize < this->m_Buffer + 0x1fff)
    {
        strcpy(this->m_BufferEnd, tmpBuffer);
        this->m_BufferEnd += tmpBufferSize;
        this->m_BufferEnd[0] = '\0';
    }
    this->m_Fatal = 1;
    LeaveCriticalSection(&g_CriticalSections[3]);
    g_LogLockDepth--;
}

} // namespace th10
