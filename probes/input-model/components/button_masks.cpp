#include "button_masks.hpp"
namespace input_candidate {
unsigned int WinMMButtons(const ButtonBindings& bindings, unsigned int buttons, unsigned int incoming) {
    const short indices[5] = {bindings.action1, bindings.action2, bindings.action4, bindings.action8, bindings.action256};
    const unsigned int actions[5] = {1u, 2u, 4u, 8u, 256u};
    unsigned int result = incoming;
    for (unsigned int i = 0; i < 5; ++i) {
        if (indices[i] >= 0 && (buttons & (1u << indices[i])) != 0)
            result |= actions[i];
    }
    return result;
}
unsigned int DirectInputButtons(const ButtonBindings& bindings, const unsigned char buttons[128], unsigned int incoming) {
    const short indices[5] = {bindings.action1, bindings.action2, bindings.action4, bindings.action8, bindings.action256};
    const unsigned int actions[5] = {1u, 2u, 4u, 8u, 256u};
    unsigned int result = incoming;
    for (unsigned int i = 0; i < 5; ++i) {
        if (indices[i] >= 0 && (buttons[indices[i]] & 0x80u) != 0)
            result |= actions[i];
    }
    return result;
}
}
