#include "input_transition.hpp"
namespace input_candidate {
unsigned short update_input(InputPrefix& state, unsigned short now) {
    const unsigned short old = state.current;
    state.previous = old;
    state.current = now;
    state.repeated = 0;
    for (unsigned int i = 0; i < 16; ++i) {
        const unsigned short bit = static_cast<unsigned short>(1u << i);
        if ((now & bit) != 0) {
            unsigned short count = static_cast<unsigned short>(state.counters[i] + 1u);
            if (count >= 26u) {
                count = static_cast<unsigned short>(count - 8u);
                state.repeated = static_cast<unsigned short>(state.repeated | bit);
            }
            state.counters[i] = count;
        } else {
            state.counters[i] = 0;
        }
    }
    const unsigned short changed = static_cast<unsigned short>(now ^ old);
    state.pressed = static_cast<unsigned short>(changed & now);
    state.released = static_cast<unsigned short>(changed & static_cast<unsigned short>(~now));
    return now;
}
}
