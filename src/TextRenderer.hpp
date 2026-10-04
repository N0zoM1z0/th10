#ifndef TH10_TEXT_RENDERER_HPP
#define TH10_TEXT_RENDERER_HPP

#include <stddef.h>
#include <windows.h>

// Partial view of the TH10 text bitmap owner. Unobserved storage is opaque;
// this does not assert the original class layout or full allocation size.
struct TextRenderBufferView
{
    unsigned char unknown000[0x100];
    unsigned int format;
    unsigned int width;
    unsigned int height;
    unsigned int imageSize;
    int pitch;
    HDC deviceContext;
    HGDIOBJ previousBitmap;
    HBITMAP bitmap;
    void *pixels;

    bool InvertAlpha(int processedRows);
    bool ReleaseBuffer();
    bool TryAllocateBuffer(int width, int height, int format);
};

typedef char TextBufferFormatAt100[
    offsetof(TextRenderBufferView, format) == 0x100 ? 1 : -1];
typedef char TextBufferWidthAt104[
    offsetof(TextRenderBufferView, width) == 0x104 ? 1 : -1];
typedef char TextBufferHeightAt108[
    offsetof(TextRenderBufferView, height) == 0x108 ? 1 : -1];
typedef char TextBufferPitchAt110[
    offsetof(TextRenderBufferView, pitch) == 0x110 ? 1 : -1];
typedef char TextBufferImageSizeAt10c[
    offsetof(TextRenderBufferView, imageSize) == 0x10c ? 1 : -1];
typedef char TextBufferDeviceContextAt114[
    offsetof(TextRenderBufferView, deviceContext) == 0x114 ? 1 : -1];
typedef char TextBufferPreviousBitmapAt118[
    offsetof(TextRenderBufferView, previousBitmap) == 0x118 ? 1 : -1];
typedef char TextBufferBitmapAt11c[
    offsetof(TextRenderBufferView, bitmap) == 0x11c ? 1 : -1];
typedef char TextBufferPixelsAt120[
    offsetof(TextRenderBufferView, pixels) == 0x120 ? 1 : -1];

// The target takes two stack arguments and returns with RET 8. This spelling
// models that machine contract, not an original-source convention claim.
bool __stdcall TextRenderBufferApplyAlphaBleed(
    TextRenderBufferView *buffer, unsigned int processedRows);

struct D3d9RectView;
struct D3d9TextureView;
namespace TextHelperView
{
void CreateTextBuffer();
void __stdcall RenderTextToTexture(
    const D3d9RectView *rectangle, int x, int glyphWidth,
    unsigned int color, const char *text, D3d9TextureView *texture);
}

#endif
