#ifndef TH10_RECONSTRUCTION_ANM_RECTANGLE_HPP
#define TH10_RECONSTRUCTION_ANM_RECTANGLE_HPP

#include <stddef.h>

// Four floating screen bounds consumed by the untextured rectangle renderer.
// Unlike D3d9RectView, these coordinates retain fractional values.
struct AnmFloatRectView
{
    float left;
    float top;
    float right;
    float bottom;
};
typedef char AnmFloatRectViewSizeIs10[
    (sizeof(AnmFloatRectView) == 0x10 &&
     offsetof(AnmFloatRectView, left) == 0 &&
     offsetof(AnmFloatRectView, top) == 4 &&
     offsetof(AnmFloatRectView, right) == 8 &&
     offsetof(AnmFloatRectView, bottom) == 12) ? 1 : -1];

// Complete target owner 0x0043BDA0. Its EDI/EBX private ABI is observed;
// this natural declaration must still be tested in a genuine caller graph.
void DrawColoredRectangle(AnmFloatRectView *rectangle, unsigned int color);


#endif
