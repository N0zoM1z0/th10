// Existing complete text-decode and stage-state leaves used by GUI messages.
// Bodies and retained annotations moved unchanged from DecomphelpLeafAccessors.
#include "DecomphelpLeafTypes.hpp"

#pragma optimize("gty", on)
extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)
#ifndef LEAF_GLOB
#define LEAF_GLOB(a) ((u8 *)(uintptr_t)(a))
#endif

namespace th10 { namespace leaf {

__declspec(noinline) u8 * Fn00417010(u8 *p0)
{
    u8 key = 0x77;
    u8 increment = 7;
    u8 *output = LEAF_GLOB(0x497d40u);
    u8 decoded;
    do {
        decoded = static_cast<u8>(key ^ *p0);
        *output++ = decoded;
        _ReadWriteBarrier();
        key = static_cast<u8>(key + increment);
        ++p0;
        increment = static_cast<u8>(increment + 0x10);
    } while (decoded != 0);
    return LEAF_GLOB(0x497d40u);
}

__declspec(noinline) void Fn004175E0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((i32)(*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) < 7) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) + 1;
    }
    (*(u32 *)LEAF_GLOB(0x477848u)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) * 0x30 + 0x474788;
    return;
}

} }
