#include "key_masks.hpp"
#include "key_weights.hpp"
#include <cstdio>
#include <cstring>

namespace {
unsigned long checked = 0;
unsigned short Expected(const unsigned char keys[256], const unsigned short weights[256]) {
    unsigned short result = 0;
    for (unsigned int i = 0; i < 256; ++i)
        if (keys[i] >= 128) result = static_cast<unsigned short>(result | weights[i]);
    return result;
}
bool Check(const unsigned char keys[256], unsigned int mode) {
    unsigned char before[256];
    std::memcpy(before, keys, sizeof before);
    const unsigned short expected = Expected(keys, mode == 0 ? win32_weights : di_weights);
    const unsigned short actual = mode == 0 ? input_candidate::Win32KeyMask(keys)
                                           : input_candidate::DirectInputKeyMask(keys);
    ++checked;
    if (actual != expected || (actual & 0x8000u) != 0 || std::memcmp(before, keys, 256) != 0) {
        std::printf("MISMATCH mode=%u case=%lu expected=%u actual=%u\n", mode, checked,
                    static_cast<unsigned int>(expected), static_cast<unsigned int>(actual));
        return false;
    }
    return true;
}
}
int main() {
    unsigned char keys[256];
    for (unsigned int mode = 0; mode < 2; ++mode) {
        // All 256 values at every individual byte: high bit and all low bits.
        for (unsigned int i = 0; i < 256; ++i) {
            for (unsigned int value = 0; value < 256; ++value) {
                std::memset(keys, 0, sizeof keys);
                keys[i] = static_cast<unsigned char>(value);
                if (!Check(keys, mode)) return 1;
            }
        }
        // Every ordered pair, including duplicate/overlapping contributions.
        for (unsigned int i = 0; i < 256; ++i) {
            for (unsigned int j = 0; j < 256; ++j) {
                std::memset(keys, 0, sizeof keys);
                keys[i] = 0x80;
                keys[j] = 0xff;
                if (!Check(keys, mode)) return 1;
            }
        }
        // Dense uniform buffers, including all-low-bit and all-down cases.
        for (unsigned int value = 0; value < 256; ++value) {
            std::memset(keys, value, sizeof keys);
            if (!Check(keys, mode)) return 1;
        }
        // Reproducible mixed buffers; bounded arithmetic independent of ulong width.
        unsigned long seed = 0x12345678ul;
        for (unsigned int sample = 0; sample < 4096; ++sample) {
            for (unsigned int i = 0; i < 256; ++i) {
                seed = (seed * 1664525ul + 1013904223ul) & 0xfffffffful;
                keys[i] = static_cast<unsigned char>(seed >> 24);
            }
            if (!Check(keys, mode)) return 1;
        }
    }
    std::printf("PASS cases=%lu\n", checked);
    return 0;
}
