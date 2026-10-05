#ifndef TH10_GAME_ERROR_CONTEXT_HPP
#define TH10_GAME_ERROR_CONTEXT_HPP

namespace th10
{
struct GameErrorContext
{
    char m_Buffer[0x2000];
    char *m_BufferEnd;
    unsigned char m_Fatal;

    char * __fastcall Log(char *fmt, ...);
    char * __fastcall Fatal(char *fmt, ...);
};

extern GameErrorContext g_GameErrorContext;
}

#endif
