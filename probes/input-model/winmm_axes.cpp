#include "winmm_axes.hpp"
namespace input_candidate {
unsigned short WinMMAxes(unsigned short incoming,unsigned int x,unsigned int y,
                        unsigned int xMin,unsigned int xMax,unsigned int yMin,unsigned int yMax) {
    const unsigned int xMid = (xMin + xMax) >> 1;
    const unsigned int xQuarter = (xMax - xMin) >> 2;
    const unsigned int xLower = xMid - xQuarter;
    const unsigned int xUpper = xMid + xQuarter;
    const unsigned int yMid = (yMin + yMax) >> 1;
    const unsigned int yQuarter = (yMax - yMin) >> 2;
    const unsigned int yLower = yMid - yQuarter;
    const unsigned int yUpper = yMid + yQuarter;
    unsigned short result = incoming;
    if (x < xLower) result = static_cast<unsigned short>(result | 0x40u);
    if (x > xUpper) result = static_cast<unsigned short>(result | 0x80u);
    if (y < yLower) result = static_cast<unsigned short>(result | 0x10u);
    if (y > yUpper) result = static_cast<unsigned short>(result | 0x20u);
    return result;
}
}
