#ifndef TH10_TEXT_RENDERER_HPP
#define TH10_TEXT_RENDERER_HPP

#include <stddef.h>

// Partial view of the TH10 text bitmap owner. Unobserved storage is opaque;
// this does not assert the original class layout or full allocation size.
struct TextRenderBufferView
{
    unsigned char unknown000[0x100];
    unsigned int format;
    unsigned int width;
    unsigned int height;
    unsigned char unknown10c[4];
    int pitch;
    unsigned char unknown114[0x0c];
    void *pixels;
};

typedef char TextBufferFormatAt100[
    offsetof(TextRenderBufferView, format) == 0x100 ? 1 : -1];
typedef char TextBufferWidthAt104[
    offsetof(TextRenderBufferView, width) == 0x104 ? 1 : -1];
typedef char TextBufferHeightAt108[
    offsetof(TextRenderBufferView, height) == 0x108 ? 1 : -1];
typedef char TextBufferPitchAt110[
    offsetof(TextRenderBufferView, pitch) == 0x110 ? 1 : -1];
typedef char TextBufferPixelsAt120[
    offsetof(TextRenderBufferView, pixels) == 0x120 ? 1 : -1];

// The target takes two stack arguments and returns with RET 8. This spelling
// models that machine contract, not an original-source convention claim.
bool __stdcall TextRenderBufferApplyAlphaBleed(
    TextRenderBufferView *buffer, unsigned int processedRows);

#endif
