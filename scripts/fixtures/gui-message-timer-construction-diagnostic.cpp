// Diagnostic reproduction only: inherited uninitialized-flags coverage fails.
// This is not a passing regression or mutation control. See construction docs.
#include <new>
#include <cstring>
#include <cstdio>
struct Timer {
    int previous;
    int current;
    float subframe;
    float *scale;
    unsigned int flags;
    Timer() { flags &= ~1u; }
};
int main() {
    alignas(Timer) unsigned char storage[sizeof(Timer)];
    std::memset(storage, 0xa5, sizeof(storage));
    Timer *timer = new (storage) Timer;
    if (timer->flags != 0xa5a5a5a4u) {
        std::fprintf(stderr, "CHECK timer.constructor_flags\n");
        return 1;
    }
    return 0;
}
