#include "TextRenderer.hpp"

// D3DFMT_A1R5G5B5 pixel: the high bit is alpha; the low 15 bits are color.
struct TextPixel1555
{
    unsigned short colors : 15;
    unsigned short alpha : 1;
};

// Target 0x00437160 (157-byte owner): invert GDI bitmap alpha for the
// three supported pixel formats. The A1R5G5B5 path also clears the 15-bit
// color whenever the new alpha becomes zero; the other formats keep RGB.
bool TextRenderBufferView::InvertAlpha(int processedRows)
{
    int byteCount = pitch * processedRows;
    unsigned char *bytes = static_cast<unsigned char *>(pixels);

    switch (format)
    {
    case 0x1a:  // A4R4G4B4: flip the alpha nibble of every pixel.
        for (int index = 1; index < byteCount; index += 2)
            bytes[index] ^= 0xf0;
        break;

    case 0x19:  // A1R5G5B5: toggle alpha; clear color when it becomes 0.
        if (byteCount <= 0)
            break;
        for (int index = 0; index < byteCount; index += 2, bytes += 2)
        {
            TextPixel1555 *pixel = reinterpret_cast<TextPixel1555 *>(bytes);
            pixel->alpha = !pixel->alpha;
            if (!pixel->alpha)
                pixel->colors = 0;
        }
        break;

    case 0x15:  // A8R8G8B8: invert the alpha byte.
        for (int index = 3; index < byteCount; index += 4)
            bytes[index] = ~bytes[index];
        break;

    default:
        return false;
    }
    return true;
}
