#include "TextRenderer.hpp"

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
