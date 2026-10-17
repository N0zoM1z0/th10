#include "di_axes.hpp"
namespace input_candidate {
unsigned short DirectInputAxes(unsigned short incoming, int x, int y, short thresholdX, short thresholdY) {
    const int tx = static_cast<int>(thresholdX);
    const int ty = static_cast<int>(thresholdY);
    unsigned short result = incoming;
    if (x < -tx) result = static_cast<unsigned short>(result | 0x40u);
    if (x > tx) result = static_cast<unsigned short>(result | 0x80u);
    if (y < -ty) result = static_cast<unsigned short>(result | 0x10u);
    if (y > ty) result = static_cast<unsigned short>(result | 0x20u);
    return result;
}
}
