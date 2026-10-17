#include "key_masks.hpp"

namespace input_candidate {
namespace {
bool IsDown(unsigned char key) {
    return (key & 0x80u) != 0;
}
}

unsigned short Win32KeyMask(const unsigned char keys[256]) {
    unsigned short mask = 0;
    if (IsDown(keys[0x5A])) mask |= 0x0001;
    if (IsDown(keys[0x58])) mask |= 0x0002;
    if (IsDown(keys[0x10])) mask |= 0x0004;
    if (IsDown(keys[0x1B])) mask |= 0x0008;
    if (IsDown(keys[0x26]) || IsDown(keys[0x68])) mask |= 0x0010;
    if (IsDown(keys[0x28]) || IsDown(keys[0x62])) mask |= 0x0020;
    if (IsDown(keys[0x25]) || IsDown(keys[0x64])) mask |= 0x0040;
    if (IsDown(keys[0x27]) || IsDown(keys[0x66])) mask |= 0x0080;
    if (IsDown(keys[0x11])) mask |= 0x0100;
    if (IsDown(keys[0x51])) mask |= 0x0200;
    if (IsDown(keys[0x53])) mask |= 0x0400;
    if (IsDown(keys[0x24]) || IsDown(keys[0x50])) mask |= 0x0800;
    if (IsDown(keys[0x0D])) mask |= 0x1000;
    if (IsDown(keys[0x44])) mask |= 0x2000;
    if (IsDown(keys[0x52])) mask |= 0x4000;
    if (IsDown(keys[0x69])) mask |= 0x0090;
    if (IsDown(keys[0x67])) mask |= 0x0050;
    if (IsDown(keys[0x61])) mask |= 0x0060;
    if (IsDown(keys[0x63])) mask |= 0x00A0;
    return mask;
}

unsigned short DirectInputKeyMask(const unsigned char keys[256]) {
    unsigned short mask = 0;
    if (IsDown(keys[0x2C])) mask |= 0x0001;
    if (IsDown(keys[0x2D])) mask |= 0x0002;
    if (IsDown(keys[0x2A])) mask |= 0x0004;
    if (IsDown(keys[0x01])) mask |= 0x0008;
    if (IsDown(keys[0xC8]) || IsDown(keys[0x48])) mask |= 0x0010;
    if (IsDown(keys[0x50]) || IsDown(keys[0xD0])) mask |= 0x0020;
    if (IsDown(keys[0xCB]) || IsDown(keys[0x4B])) mask |= 0x0040;
    if (IsDown(keys[0xCD]) || IsDown(keys[0x4D])) mask |= 0x0080;
    if (IsDown(keys[0x1D])) mask |= 0x0100;
    if (IsDown(keys[0x10])) mask |= 0x0200;
    if (IsDown(keys[0x1F])) mask |= 0x0400;
    if (IsDown(keys[0xC7]) || IsDown(keys[0x19])) mask |= 0x0800;
    if (IsDown(keys[0x1C])) mask |= 0x1000;
    if (IsDown(keys[0x20])) mask |= 0x2000;
    if (IsDown(keys[0x13])) mask |= 0x4000;
    if (IsDown(keys[0x49])) mask |= 0x0090;
    if (IsDown(keys[0x47])) mask |= 0x0050;
    if (IsDown(keys[0x4F])) mask |= 0x0060;
    if (IsDown(keys[0x51])) mask |= 0x00A0;
    return mask;
}
}
