#include "TextRenderer.hpp"
#include "D3d9View.hpp"

#include <string.h>

struct TextArgb4444PixelView
{
    unsigned short blue : 4;
    unsigned short green : 4;
    unsigned short red : 4;
    unsigned short alpha : 4;
};
typedef char TextArgb4444PixelSizeIs2[
    sizeof(TextArgb4444PixelView) == 2 ? 1 : -1];

static void AccumulateTextArgb8888Neighbor(
    unsigned int *sums, unsigned char *pixel, unsigned int *count)
{
    if (pixel[3] != 0)
    {
        sums[0] += pixel[2];
        sums[1] += pixel[1];
        sums[2] += pixel[0];
        ++*count;
    }
}

static void AccumulateTextArgb4444Neighbor(
    unsigned int *sums, TextArgb4444PixelView *pixel, unsigned int *count)
{
    if (pixel->alpha != 0)
    {
        sums[0] += pixel->red;
        sums[1] += pixel->green;
        sums[2] += pixel->blue;
        ++*count;
    }
}

// Target 0x00436DA0, complete 946-byte owner. Unlike texture-surface bleed,
// the cursor continues across rows. Vertical neighbors use signed pitch
// divided by the pixel size with truncation toward zero. Only transparent
// destinations are written, and all sampled neighbors have nonzero alpha.
bool __stdcall TextRenderBufferApplyAlphaBleed(
    TextRenderBufferView *buffer, unsigned int processedRows)
{
    switch (buffer->format)
    {
    case 0x15:
        {
            unsigned int *pixel = static_cast<unsigned int *>(buffer->pixels);
            for (unsigned int y = 0; y < processedRows; ++y)
            {
                for (unsigned int x = 0; x < buffer->width; ++pixel, ++x)
                {
                    unsigned char *components =
                        reinterpret_cast<unsigned char *>(pixel);
                    if (components[3] == 0)
                    {
                        unsigned int sums[3];
                        sums[0] = sums[1] = sums[2] = 0;
                        unsigned int count = 0;
                        if (x > 0)
                            AccumulateTextArgb8888Neighbor(sums,
                                reinterpret_cast<unsigned char *>(pixel - 1),
                                &count);
                        if (x < buffer->width - 1)
                            AccumulateTextArgb8888Neighbor(sums,
                                reinterpret_cast<unsigned char *>(pixel + 1),
                                &count);
                        if (y > 0)
                            AccumulateTextArgb8888Neighbor(sums,
                                reinterpret_cast<unsigned char *>(
                                    pixel - buffer->pitch / 4), &count);
                        if (y < buffer->height - 1)
                            AccumulateTextArgb8888Neighbor(sums,
                                reinterpret_cast<unsigned char *>(
                                    pixel + buffer->pitch / 4), &count);
                        if (count > 1)
                        {
                            sums[0] /= count;
                            sums[1] /= count;
                            sums[2] /= count;
                        }
                        components[2] = static_cast<unsigned char>(sums[0]);
                        components[1] = static_cast<unsigned char>(sums[1]);
                        components[0] = static_cast<unsigned char>(sums[2]);
                    }
                }
            }
        }
        break;
    case 0x1a:
        {
            TextArgb4444PixelView *pixel =
                static_cast<TextArgb4444PixelView *>(buffer->pixels);
            for (unsigned int y = 0; y < processedRows; ++y)
            {
                for (unsigned int x = 0; x < buffer->width; ++pixel, ++x)
                {
                    if (pixel->alpha == 0)
                    {
                        unsigned int sums[3];
                        sums[0] = sums[1] = sums[2] = 0;
                        unsigned int count = 0;
                        if (x > 0)
                            AccumulateTextArgb4444Neighbor(
                                sums, pixel - 1, &count);
                        if (x < buffer->width - 1)
                            AccumulateTextArgb4444Neighbor(
                                sums, pixel + 1, &count);
                        if (y > 0)
                            AccumulateTextArgb4444Neighbor(
                                sums, pixel - buffer->pitch / 2, &count);
                        if (y < buffer->height - 1)
                            AccumulateTextArgb4444Neighbor(
                                sums, pixel + buffer->pitch / 2, &count);
                        if (count > 1)
                        {
                            sums[0] /= count;
                            sums[1] /= count;
                            sums[2] /= count;
                        }
                        // TH10 halves each averaged four-bit channel here.
                        sums[0] >>= 1;
                        sums[1] >>= 1;
                        sums[2] >>= 1;
                        pixel->red = static_cast<unsigned char>(sums[0]);
                        pixel->green = static_cast<unsigned char>(sums[1]);
                        pixel->blue = static_cast<unsigned char>(sums[2]);
                    }
                }
            }
        }
        break;
    }
    return true;
}

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

extern "C" long __stdcall D3DXLoadSurfaceFromMemory(
    D3d9SurfaceView *destinationSurface, const void *destinationPalette,
    const D3d9RectView *destinationRect, const void *sourceMemory,
    unsigned int sourceFormat, unsigned int sourcePitch,
    const void *sourcePalette, const D3d9RectView *sourceRect,
    unsigned int filter, unsigned int colorKey);

// Target 0x00437DB0. The two old-font restores around the second inversion
// and bleed are both present in TH10. HRESULTs are intentionally unchecked.
void __stdcall TextHelperView::RenderTextToTexture(
    const D3d9RectView *rectangle, int x, int glyphWidth,
    unsigned int color, const char *text, D3d9TextureView *texture)
{
    HFONT font;
    if (glyphWidth <= 17)
        font = g_TextFont17;
    else if (glyphWidth <= 18)
        font = g_TextFont18;
    else if (glyphWidth <= 19)
        font = g_TextFont19;
    else if (glyphWidth <= 20)
        font = g_TextFont20;
    else if (glyphWidth <= 21)
        font = g_TextFont21;
    else if (glyphWidth <= 22)
        font = g_TextFont22;
    else if (glyphWidth <= 23)
        font = g_TextFont23;
    else if (glyphWidth <= 24)
        font = g_TextFont24;
    else if (glyphWidth <= 25)
        font = g_TextFont25;
    else if (glyphWidth <= 26)
        font = g_TextFont26;
    else if (glyphWidth <= 27)
        font = g_TextFont27;
    else if (glyphWidth <= 28)
        font = g_TextFont28;
    else if (glyphWidth <= 29)
        font = g_TextFont29;
    else if (glyphWidth <= 30)
        font = g_TextFont30;
    else
        font = g_TextFont31;

    if (glyphWidth < 17)
        glyphWidth = 17;

    memset(g_TextRenderBuffer.pixels, 0, g_TextRenderBuffer.imageSize);
    HDC hdc = g_TextRenderBuffer.deviceContext;
    HGDIOBJ previousFont = SelectObject(hdc, font);
    int processedRows = glyphWidth * 2 + 6;
    g_TextRenderBuffer.InvertAlpha(processedRows);
    SetBkMode(hdc, TRANSPARENT);
    int textLength = static_cast<int>(strlen(text));
    SetTextColor(hdc, 0);
    TextOutA(hdc, x * 2 + 2, 2, text, textLength);
    SetTextColor(hdc, color);
    TextOutA(hdc, x * 2, 0, text, textLength);
    SelectObject(hdc, previousFont);
    g_TextRenderBuffer.InvertAlpha(processedRows);
    TextRenderBufferApplyAlphaBleed(&g_TextRenderBuffer, processedRows);
    SelectObject(hdc, previousFont);

    D3d9RectView sourceRect;
    sourceRect.left = 0;
    sourceRect.top = 0;
    sourceRect.right = (rectangle->right - rectangle->left) * 2 + 22;
    sourceRect.bottom = glyphWidth * 2 + 2;
    if (sourceRect.right > 1024)
        sourceRect.right = 1024;
    D3d9SurfaceView *surface;
    texture->vtable->GetSurfaceLevel(texture, 0, &surface);
    D3DXLoadSurfaceFromMemory(
        surface, NULL, rectangle, g_TextRenderBuffer.pixels,
        g_TextRenderBuffer.format, g_TextRenderBuffer.pitch,
        NULL, &sourceRect, 4, 0);
    if (surface != NULL)
        surface->vtable->Release(surface);
}
