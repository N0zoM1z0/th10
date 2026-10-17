#ifndef INPUT_CANDIDATE_WINMM_AXES_HPP
#define INPUT_CANDIDATE_WINMM_AXES_HPP
#include <climits>
namespace input_candidate {
typedef char winmm_axis_word[sizeof(unsigned int)==4 && UINT_MAX==0xffffffffu ? 1:-1];
typedef char winmm_axis_mask[sizeof(unsigned short)==2 && USHRT_MAX==65535 ? 1:-1];
unsigned short WinMMAxes(unsigned short incoming,unsigned int x,unsigned int y,
                        unsigned int xMin,unsigned int xMax,unsigned int yMin,unsigned int yMax);
}
#endif
