#include "input_transition.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
using input_candidate::InputPrefix;
struct Guarded { unsigned char before[32]; InputPrefix state; unsigned char after[64]; };
static unsigned long cases = 0;
static void check(unsigned int value, unsigned int old, unsigned int now, unsigned int pattern) {
    Guarded actual;
    std::memset(&actual, 0xa7, sizeof(actual));
    actual.state.current = static_cast<unsigned short>(old);
    actual.state.previous = 0xb621;
    actual.state.repeated = 0xffff;
    actual.state.pressed = 0xd167;
    actual.state.released = 0x9623;
    unsigned int counters[16];
    for (unsigned int i = 0; i < 16; ++i) {
        counters[i] = pattern == 0 ? value : (value + i * 4099u) % 65536u;
        actual.state.counters[i] = static_cast<unsigned short>(counters[i]);
    }
    unsigned int repeat = 0;
    for (unsigned int i = 0; i < 16; ++i) {
        const unsigned int held = (now / (1u << i)) % 2;
        const unsigned int next = (counters[i] + 1u) % 65536u;
        repeat += held && next > 25u ? (1u << i) : 0u;
        counters[i] = held ? (next > 25u ? next - 8u : next) : 0u;
    }
    const unsigned int pressed = now & (65535u - old);
    const unsigned int released = old & (65535u - now);
    const unsigned short result = input_candidate::update_input(actual.state, static_cast<unsigned short>(now));
    const char* field = 0;
    if (result != now) field = "return";
    if (actual.state.current != now) field = "current";
    if (actual.state.previous != old) field = "previous";
    if (actual.state.repeated != repeat) field = "repeated";
    if (actual.state.pressed != pressed) field = "pressed";
    if (actual.state.released != released) field = "released";
    for (unsigned int i = 0; i < 16; ++i)
        if (actual.state.counters[i] != counters[i]) field = "counter";
    for (unsigned int i = 0; i < sizeof(actual.before); ++i)
        if (actual.before[i] != 0xa7) field = "before guard";
    for (unsigned int i = 0; i < sizeof(actual.after); ++i)
        if (actual.after[i] != 0xa7) field = "tail guard";
    if (field) {
        std::printf("SEMANTIC_MISMATCH %s value=%u old=%u now=%u pattern=%u\n", field, value, old, now, pattern);
        std::exit(10);
    }
    ++cases;
}
int main() {
    // Every counter word, in every slot, both held and clear, mixed masks.
    for (unsigned int v = 0; v < 65536u; ++v) {
        check(v, v, 65535u, 0);
        check(v, 65535u-v, 0, 0);
        check(v, v, 0xaaaau, 0);
        check(v, 65535u-v, 0x5555u, 0);
        check(v, v, 65535u-v, 1);
        for (unsigned int i = 0; i < 16; ++i)
            check(v, 65535u-(1u << i), 1u << i, 0);
    }
    // Every old/new pair for each bit, with independent neighbouring values.
    for (unsigned int i = 0; i < 16; ++i)
        for (unsigned int pair = 0; pair < 4; ++pair)
            for (unsigned int v = 0; v < 32; ++v)
                check(v, (pair & 1u) ? 1u << i : 0u, (pair & 2u) ? 1u << i : 0u, 1);
    std::printf("PASS cases=%lu\n", cases);
    return 0;
}
