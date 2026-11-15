// Target 0x00441E50. Its own /GL /Ob1 TU keeps the four real calls from RGB.
#include "AnmManager.hpp"

AnmInt3View AnmInt3View::operator*(float scale) const
{
    return AnmInt3View(
        static_cast<int>(x * scale),
        static_cast<int>(z * scale),
        static_cast<int>(y * scale));
}
