// Natural source candidate adapted from th10-decomphelp-forN0.  The bounded
// OnDraw body is retained here only after direct TH10 IDA boundary review;
// the other GameManager rows in that repository are not part of this source.

typedef unsigned int ZunBool;
typedef unsigned char u8;
typedef unsigned int u32;

extern "C" void *g_AnmManagerPtr;

struct GameManager
{
    u8 m_Pad[0x58];
    u32 m_Flags;

    ZunBool __fastcall OnDraw();
};

ZunBool __fastcall GameManager::OnDraw()
{
    void *pAnm;

    if ((m_Flags & 4) == 0)
    {
        pAnm = g_AnmManagerPtr;
        *(u32 *)((u8 *)pAnm + 0x50) = 0;
        *(u32 *)((u8 *)pAnm + 0x54) = 0;
        *(u32 *)((u8 *)pAnm + 0x4c) = 0;
        *(u32 *)((u8 *)pAnm + 0x58) = 0;
    }
    return 1;
}
