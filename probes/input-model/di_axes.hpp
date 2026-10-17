#ifndef INPUT_CANDIDATE_DI_AXES_HPP
#define INPUT_CANDIDATE_DI_AXES_HPP
#include <climits>
namespace input_candidate {
typedef char di_axis_word[sizeof(int)==4 && INT_MIN==(-2147483647-1) && INT_MAX==2147483647 ? 1:-1];
typedef char di_axis_threshold[sizeof(short)==2 && SHRT_MIN==-32768 && SHRT_MAX==32767 && USHRT_MAX==65535 ? 1:-1];
unsigned short DirectInputAxes(unsigned short incoming, int x, int y, short thresholdX, short thresholdY);
}
#endif
