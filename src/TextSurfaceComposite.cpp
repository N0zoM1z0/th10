#include "TextRenderer.hpp"
#include "D3d9View.hpp"

// Target 0x00437750: blend the GDI bitmap into a locked D3D9 surface.
// Native private ABI: EAX=destination row, EDX=bitmap buffer, ECX=column,
// followed by (surface, pixelWidth, rows) on the stack / RET 0x0C. This
// descriptive C++ parameter ordering is not a claim about the original
// compiler-private source signature.
// The source matches the original format-dependent pixel semantics,
// including its unusual four-bit-alpha channel and saturation operations.
bool __fastcall BlendTextBitmapToSurface(
    int column, TextRenderBufferView *buffer, int row,
    D3d9SurfaceView *surface, int pixelWidth, int rows)
{
    if (buffer->bitmap == NULL)
        return false;

    D3d9SurfaceDescriptionView description;
    surface->vtable->GetDesc(surface, &description);

    D3d9RectView rectangle;
    rectangle.left = rectangle.top = 0;
    rectangle.right = buffer->width;
    rectangle.bottom = buffer->height;

    D3d9LockedRectView locked;
    if (surface->vtable->LockRect(surface, &locked, &rectangle, 0) != 0)
        return false;

    if (description.format == buffer->format)
    {
        unsigned char *sourceRow = static_cast<unsigned char *>(buffer->pixels);
        const int sourcePitch = buffer->pitch;
        const int destPitch = locked.pitch;
        unsigned char *base = static_cast<unsigned char *>(locked.bits);

        switch (buffer->format)
        {
        case 0x15: // A8R8G8B8, three channels, preserve destination alpha
            {
                unsigned char *destinationRow = base + row * destPitch + column * 4;
                unsigned char *dest = destinationRow;
                unsigned char *src = sourceRow;
                for (int remainingRows = rows; remainingRows > 0; --remainingRows)
                {
                    for (int remainingPixels = pixelWidth; remainingPixels > 0;
                         --remainingPixels, src += 4, dest += 4)
                    {
                        const unsigned int opacity = src[3];
                        dest[0] = static_cast<unsigned char>(
                            dest[0] + ((static_cast<unsigned int>(src[0]) -
                                        static_cast<unsigned int>(dest[0])) * opacity >> 8));
                        dest[1] = static_cast<unsigned char>(
                            dest[1] + ((static_cast<unsigned int>(src[1]) -
                                        static_cast<unsigned int>(dest[1])) * opacity >> 8));
                        dest[2] = static_cast<unsigned char>(
                            dest[2] + ((static_cast<unsigned int>(src[2]) -
                                        static_cast<unsigned int>(dest[2])) * opacity >> 8));
                    }
                    src += sourcePitch - pixelWidth * 4;
                    dest += destPitch - pixelWidth * 4;
                }
            }
            break;

        case 0x19: // A1R5G5B5: copy whole 16-bit pixel only if top bit set
            {
                unsigned char *destinationRow = base + row * destPitch + column * 2;
                unsigned short *dest = reinterpret_cast<unsigned short *>(destinationRow);
                const unsigned short *src = reinterpret_cast<const unsigned short *>(sourceRow);
                for (int remainingRows = rows; remainingRows > 0; --remainingRows)
                {
                    for (int remainingPixels = pixelWidth; remainingPixels > 0;
                         --remainingPixels, ++src, ++dest)
                    {
                        const unsigned short pixel = *src;
                        if (static_cast<short>(pixel) < 0)
                            *dest = pixel;
                    }
                    src += (sourcePitch - pixelWidth * 2) / 2;
                    dest += (destPitch - pixelWidth * 2) / 2;
                }
            }
            break;

        case 0x1A: // A4R4G4B4; reproduce target's byte/nibble operations
            {
                unsigned char *destinationRow = base + row * destPitch + column * 2;
                unsigned char *dest = destinationRow;
                unsigned char *src = sourceRow;
                for (int remainingRows = rows; remainingRows > 0; --remainingRows)
                {
                    for (int remainingPixels = pixelWidth; remainingPixels > 0;
                         --remainingPixels, src += 2, dest += 2)
                    {
                        const unsigned int alpha = src[1] >> 4;
                        const int sourceHigh = src[0] >> 4;
                        const int destHigh = dest[0] >> 4;
                        const int high = destHigh + ((sourceHigh - destHigh) *
                                                     static_cast<int>(alpha) >> 4);
                        // The target stores the new high nibble first;
                        // its subsequent low-nibble expression reads zero
                        // from that intermediate, not the old dest low nibble.
                        dest[0] = static_cast<unsigned char>(high << 4);
                        dest[0] = static_cast<unsigned char>(dest[0] |
                            (((src[0] & 15) * alpha) >> 4));

                        const unsigned int originalHighByte = dest[1];
                        unsigned int newAlpha =
                            ((originalHighByte >> 2) & 0x3c) + alpha;
                        if (newAlpha > 15)
                            newAlpha = 15;
                        const int priorLow = originalHighByte & 15;
                        const int replacementLow = priorLow +
                            (((static_cast<int>(src[1] & 15) - priorLow) *
                               static_cast<int>(alpha)) >> 4);
                        dest[1] = static_cast<unsigned char>(
                            (newAlpha << 4) | replacementLow);
                    }
                    src += sourcePitch - pixelWidth * 2;
                    dest += destPitch - pixelWidth * 2;
                }
            }
            break;
        }
    }
    surface->vtable->UnlockRect(surface);
    return true;
}
