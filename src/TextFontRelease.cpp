#include "TextRenderer.hpp"

// The original caller owns one render buffer and fifteen shared GDI fonts.
extern TextRenderBufferView g_TextRenderBuffer;
extern HFONT g_TextFont17;
extern HFONT g_TextFont18;
extern HFONT g_TextFont19;
extern HFONT g_TextFont20;
extern HFONT g_TextFont21;
extern HFONT g_TextFont22;
extern HFONT g_TextFont23;
extern HFONT g_TextFont24;
extern HFONT g_TextFont25;
extern HFONT g_TextFont26;
extern HFONT g_TextFont27;
extern HFONT g_TextFont28;
extern HFONT g_TextFont29;
extern HFONT g_TextFont30;
extern HFONT g_TextFont31;

// Original TH10 0x437D10: release bitmap state, then delete every one of the
// fifteen retained fixed font slots. No null checks or slot resets are added.
void TextReleaseFontResources()
{
    g_TextRenderBuffer.ReleaseBuffer();
    DeleteObject(g_TextFont17);
    DeleteObject(g_TextFont18);
    DeleteObject(g_TextFont19);
    DeleteObject(g_TextFont20);
    DeleteObject(g_TextFont21);
    DeleteObject(g_TextFont22);
    DeleteObject(g_TextFont23);
    DeleteObject(g_TextFont24);
    DeleteObject(g_TextFont25);
    DeleteObject(g_TextFont26);
    DeleteObject(g_TextFont27);
    DeleteObject(g_TextFont28);
    DeleteObject(g_TextFont29);
    DeleteObject(g_TextFont30);
    DeleteObject(g_TextFont31);
}
