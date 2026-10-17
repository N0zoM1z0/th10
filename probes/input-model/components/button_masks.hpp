#ifndef INPUT_CANDIDATE_BUTTON_MASKS_HPP
#define INPUT_CANDIDATE_BUTTON_MASKS_HPP
#include <climits>
namespace input_candidate {
typedef char button_bytes[CHAR_BIT == 8 && UCHAR_MAX == 255 ? 1 : -1];
typedef char button_shorts[sizeof(short) == 2 && SHRT_MIN == -32768 && SHRT_MAX == 32767 ? 1 : -1];
typedef char button_words[sizeof(unsigned int) == 4 && UINT_MAX == 0xffffffffu ? 1 : -1];
// Logical values only: no claim of target record layout or API ABI.
struct ButtonBindings {
    short action1;
    short action2;
    short action4;
    short action8;
    short action256;
};
// Each binding must be negative or 0..31 (WinMM), negative or 0..127 (DI).
// DI buttons points to 128 readable bytes; configuration/buffer remain stable.
unsigned int WinMMButtons(const ButtonBindings& bindings, unsigned int buttons, unsigned int incoming);
unsigned int DirectInputButtons(const ButtonBindings& bindings, const unsigned char buttons[128], unsigned int incoming);
}
#endif
