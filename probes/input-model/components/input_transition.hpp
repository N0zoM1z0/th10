#ifndef INPUT_TRANSITION_HPP
#define INPUT_TRANSITION_HPP
#include <climits>
#include <cstddef>
namespace input_candidate {
struct InputPrefix {
    unsigned short current;
    unsigned short previous;
    unsigned short repeated;
    unsigned short pressed;
    unsigned short released;
    unsigned short counters[16];
};
typedef char require_octets[CHAR_BIT == 8 ? 1 : -1];
typedef char require_word[USHRT_MAX == 65535 && sizeof(unsigned short) == 2 ? 1 : -1];
typedef char require_size[sizeof(InputPrefix) == 42 ? 1 : -1];
typedef char require_current[offsetof(InputPrefix, current) == 0 ? 1 : -1];
typedef char require_previous[offsetof(InputPrefix, previous) == 2 ? 1 : -1];
typedef char require_repeated[offsetof(InputPrefix, repeated) == 4 ? 1 : -1];
typedef char require_pressed[offsetof(InputPrefix, pressed) == 6 ? 1 : -1];
typedef char require_released[offsetof(InputPrefix, released) == 8 ? 1 : -1];
typedef char require_counters[offsetof(InputPrefix, counters) == 10 ? 1 : -1];
unsigned short update_input(InputPrefix& state, unsigned short now);
}
#endif
