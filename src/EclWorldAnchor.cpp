#include "AnmManager.hpp"
#include "EclWorldAnchor.hpp"

// Target globals: a persistent 3-float screen-space anchor at 0x497D80 and
// its state bit at 0x497D8C. Data allocation and original names remain open.
extern AnmFloat3View g_EclWorldAnchor;
extern unsigned int g_EclWorldAnchorFlags;
extern float g_EclWorldOffsetX;
extern float g_EclWorldOffsetY;

// Target 0x00418810..0x0041884F: return the published world-to-screen point.
// Native compiler-private input uses EAX, not a proved C++ source ABI.
__declspec(noinline) AnmFloat3View *__fastcall PublishEclWorldAnchor(const AnmFloat3View *position)
{
    const unsigned int enabled = 1u;
    const unsigned char prior =
        *reinterpret_cast<const unsigned char *>(&g_EclWorldAnchorFlags);
    if ((prior & enabled) == 0)
        g_EclWorldAnchorFlags |= enabled;
    const volatile float &worldX = position->x;
    const volatile float &worldY = position->y;
    g_EclWorldAnchor.x = worldX + g_EclWorldOffsetX;
    g_EclWorldAnchor.y = worldY + g_EclWorldOffsetY;
    g_EclWorldAnchor.z = position->z;
    return &g_EclWorldAnchor;
}
