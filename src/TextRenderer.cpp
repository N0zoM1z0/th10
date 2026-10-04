#include "TextRenderer.hpp"
#include "D3d9View.hpp"
#include "Rng.hpp"

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

struct TextRenderFormatInfo
{
    int format;
    int bitCount;
    unsigned int alphaMask;
    unsigned int redMask;
    unsigned int greenMask;
    unsigned int blueMask;
};
typedef char TextRenderFormatInfoSizeIs18[
    sizeof(TextRenderFormatInfo) == 0x18 ? 1 : -1];
extern TextRenderFormatInfo g_TextRenderFormats[];

static TextRenderFormatInfo *FindTextRenderFormat(int format)
{
    unsigned int index = 0;
    while (g_TextRenderFormats[index].format != -1 &&
           g_TextRenderFormats[index].format != format)
        ++index;
    // TH10 rejects an explicit -1 request, but otherwise returns the row
    // reached by the search, including the sentinel for unsupported formats.
    if (format == -1)
        return NULL;
    return &g_TextRenderFormats[index];
}

// Target 0x00436A30 is a dependency of the allocation owner below. Its ESI
// receiver is a private compiler contract, not a source calling convention.
bool TextRenderBufferView::ReleaseBuffer()
{
    if (deviceContext != NULL)
    {
        SelectObject(deviceContext, previousBitmap);
        DeleteDC(deviceContext);
        DeleteObject(bitmap);
        width = 0;
        height = 0;
        deviceContext = NULL;
        bitmap = NULL;
        previousBitmap = NULL;
        pixels = NULL;
        format = static_cast<unsigned int>(-1);
        return true;
    }
    return false;
}

// Target 0x00436AF0. The 108-byte scratch object is the SDK's real V4 DIB
// header, not a padded BITMAPINFO. Signed divisions and the extra DIB row are
// observed target behavior; only the requested rows are cleared afterward.
bool TextRenderBufferView::TryAllocateBuffer(
    int requestedWidth, int requestedHeight, int requestedFormat)
{
    ReleaseBuffer();
    BITMAPV4HEADER info;
    memset(&info, 0, sizeof(info));
    TextRenderFormatInfo *formatInfo = FindTextRenderFormat(requestedFormat);
    if (formatInfo == NULL)
        return false;

    int rowPitch = ((requestedWidth * formatInfo->bitCount / 8 + 3) / 4) * 4;
    info.bV4Size = sizeof(info);
    info.bV4Width = requestedWidth;
    info.bV4Height = -(requestedHeight + 1);
    info.bV4Planes = 1;
    info.bV4BitCount = static_cast<WORD>(formatInfo->bitCount);
    info.bV4SizeImage = rowPitch * requestedHeight;
    if (requestedFormat != 0x18 && requestedFormat != 0x16)
    {
        info.bV4V4Compression = BI_BITFIELDS;
        info.bV4RedMask = formatInfo->redMask;
        info.bV4GreenMask = formatInfo->greenMask;
        info.bV4BlueMask = formatInfo->blueMask;
        info.bV4AlphaMask = formatInfo->alphaMask;
    }

    void *newPixels;
    HBITMAP newBitmap = CreateDIBSection(
        NULL, reinterpret_cast<const BITMAPINFO *>(&info), DIB_RGB_COLORS,
        &newPixels, NULL, 0);
    if (newBitmap == NULL)
        return false;
    memset(newPixels, 0, info.bV4SizeImage);
    HDC newContext = CreateCompatibleDC(NULL);
    HGDIOBJ oldBitmap = SelectObject(newContext, newBitmap);
    deviceContext = newContext;
    bitmap = newBitmap;
    pixels = newPixels;
    imageSize = info.bV4SizeImage;
    previousBitmap = oldBitmap;
    width = requestedWidth;
    height = requestedHeight;
    format = requestedFormat;
    pitch = rowPitch;
    return true;
}

// CreateFontA expects the target's CP932 face name: fullwidth MS Gothic.
#define TEXT_FONT_FACE_CP932 \
    "\x82\x6c\x82\x72 \x83\x53\x83\x56\x83\x62\x83\x4e"

// Target 0x00437A00. Failure of both DIB attempts does not skip the observed
// RNG-prefix initialization or the fifteen individual font creations.
void TextHelperView::CreateTextBuffer()
{
    if (!g_TextRenderBuffer.TryAllocateBuffer(1024, 64, 0x1a))
        g_TextRenderBuffer.TryAllocateBuffer(1024, 64, 0x15);
    for (unsigned int i = 0; i < 256; ++i)
        g_TextRenderBuffer.unknown000[i] =
            static_cast<unsigned char>(g_RngView.GetRandomU16() >> 9);
    g_TextFont17 = CreateFontA(
        32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont18 = CreateFontA(
        34, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont19 = CreateFontA(
        36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont20 = CreateFontA(
        38, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont21 = CreateFontA(
        40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont22 = CreateFontA(
        42, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont23 = CreateFontA(
        44, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont24 = CreateFontA(
        46, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont25 = CreateFontA(
        48, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont26 = CreateFontA(
        50, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont27 = CreateFontA(
        52, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont28 = CreateFontA(
        54, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont29 = CreateFontA(
        56, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont30 = CreateFontA(
        58, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
    g_TextFont31 = CreateFontA(
        60, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, TEXT_FONT_FACE_CP932);
}

#undef TEXT_FONT_FACE_CP932
