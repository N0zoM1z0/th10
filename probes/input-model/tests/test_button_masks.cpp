#include "button_masks.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
using input_candidate::ButtonBindings;
static unsigned long cases = 0;
static void check(const short indices[5], unsigned int word, const unsigned char bytes[128], unsigned int incoming, bool di) {
    ButtonBindings bindings = {indices[0], indices[1], indices[2], indices[3], indices[4]};
    const ButtonBindings before = bindings;
    unsigned char original[128];
    std::memcpy(original, bytes, 128);
    unsigned int expected = incoming;
    for (unsigned int field = 0; field < 5; ++field) {
        bool down = false;
        if (indices[field] >= 0) {
            if (di) down = bytes[indices[field]] >= 128;
            else down = (word / (1u << indices[field])) % 2u == 1;
        }
        if (down) {
            switch (field) {
            case 0: expected |= 0x001u; break;
            case 1: expected |= 0x002u; break;
            case 2: expected |= 0x004u; break;
            case 3: expected |= 0x008u; break;
            case 4: expected |= 0x100u; break;
            }
        }
    }
    const unsigned int result = di ? input_candidate::DirectInputButtons(bindings, bytes, incoming) : input_candidate::WinMMButtons(bindings, word, incoming);
    if (result != expected || bindings.action1 != before.action1 || bindings.action2 != before.action2 || bindings.action4 != before.action4 || bindings.action8 != before.action8 || bindings.action256 != before.action256 || std::memcmp(original, bytes, 128) != 0) {
        std::printf("SEMANTIC_MISMATCH mode=%s case=%lu expected=%u actual=%u\n", di ? "DI" : "WinMM", cases, expected, result);
        std::exit(10);
    }
    ++cases;
}
int main() {
    unsigned char bytes[128];
    short indices[5] = {-1,-1,-1,-1,-1};
    // Every negative signed16 value in every field, with all buttons pressed.
    std::memset(bytes, 255, 128);
    for (unsigned int field = 0; field < 5; ++field) {
        for (int v = -32768; v < 0; ++v) {
            indices[field] = static_cast<short>(v);
            check(indices, 0xffffffffu, bytes, 0, false);
            check(indices, 0xffffffffu, bytes, 0, true);
        }
        indices[field] = -1;
    }
    for (unsigned int field = 0; field < 5; ++field) {
        for (unsigned int index = 0; index < 128; ++index) {
            indices[field] = static_cast<short>(index);
            for (unsigned int value = 0; value < 256; ++value) {
                std::memset(bytes, 0, 128);
                bytes[index] = static_cast<unsigned char>(value);
                check(indices, 0, bytes, 0, true);
                check(indices, 0, bytes, 0xfffffeefu, true);
            }
            if (index < 32) {
                for (unsigned int bit = 0; bit < 32; ++bit) {
                    check(indices, 1u << bit, bytes, 0, false);
                    check(indices, ~(1u << bit), bytes, 0xffff0000u, false);
                }
            }
        }
        indices[field] = -1;
    }
    // Duplicate bindings and every incoming low16 with varying high16 bits.
    for (unsigned int mask = 0; mask < 65536u; ++mask) {
        for (unsigned int field = 0; field < 5; ++field) indices[field] = 31;
        std::memset(bytes, (mask & 1u) ? 128 : 127, 128);
        const unsigned int incoming = (mask << 16) | (65535u-mask);
        check(indices, (mask & 1u) ? 0x80000000u : 0u, bytes, incoming, false);
        check(indices, 0, bytes, incoming, true);
    }
    // Independent mixed bindings and dense input patterns, deterministic uint32.
    unsigned int random = 0x17bd2093u;
    for (unsigned int n = 0; n < 4096; ++n) {
        for (unsigned int j = 0; j < 128; ++j) {
            random = random * 1664525u + 1013904223u;
            bytes[j] = static_cast<unsigned char>(random >> 24);
        }
        // Avoid implementation-defined conversion of an unsigned underflow.
        for (unsigned int field = 0; field < 5; ++field) indices[field] = static_cast<short>(static_cast<int>(bytes[field] % 33) - 1);
        check(indices, random, bytes, ~random, false);
        for (unsigned int field = 0; field < 5; ++field) indices[field] = static_cast<short>(static_cast<int>(bytes[field] % 129) - 1);
        check(indices, 0, bytes, ~random, true);
    }
    std::printf("PASS cases=%lu\n", cases);
}
