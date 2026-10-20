// Target 0x00441E50; candidate /GL /Ob1 TU, not a production-owner claim.
#include "AnmManager.hpp"

AnmInt3View AnmInt3View::operator*(float scale) const
{
    return AnmInt3View(
        static_cast<int>(x * scale),
        static_cast<int>(z * scale),
        static_cast<int>(y * scale));
}
