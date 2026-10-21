// Actual case/union source with an explicit host stack seam, not native layout.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define CHECK(x, id) do { if (!(x)) { std::fprintf(stderr, "ECL_RAW_NEG_CHECK_FAILED %s\n", id); std::exit(1); } } while (0)
struct EclVmInstruction;
@UNION@
static_assert(sizeof(int) == 4 && sizeof(unsigned int) == 4, "32-bit scalar fields");
enum { ECL_VM_NEGATE_FLOAT_STORAGE = 0x55 };

struct StackSeam
{
    unsigned int input, output;
    unsigned int poppedBytes;
    int pops, pushes;

    void Pop(char tag, unsigned int bytes, void *destination)
    {
        CHECK(tag == 'f', "pop_float_tag");
        CHECK(bytes >= 4 && bytes <= sizeof(EclVmScalar), "pop_storage");
        CHECK(pops++ == 0 && pushes == 0, "pop_order");
        poppedBytes = bytes;
        // The unused pointer member can widen the union on this host. Initialize
        // that host storage and project its first dword; native width is separate.
        std::memset(destination, 0, bytes);
        std::memcpy(destination, &input, 4);
    }

    void Push(char tag, unsigned int bytes, const void *source)
    {
        CHECK(tag == 'f', "push_float_tag");
        CHECK(pops == 1 && pushes++ == 0, "push_order");
        CHECK(bytes == poppedBytes, "matching_storage");
        std::memcpy(&output, source, 4);
    }
};

static unsigned int Execute(unsigned int input)
{
    StackSeam stack = {input, 0, 0, 0, 0};
    switch (0x55)
    {
@ARM@
    }
    CHECK(stack.pops == 1 && stack.pushes == 1, "one_pop_one_push");
    return stack.output;
}

int main()
{
    // Literal NEG32 results cover zero, signed endpoints, IEEE payloads and
    // unrelated bit patterns. These are raw words, not floating arithmetic.
    const struct { unsigned int input, expected; } cases[] = {
        {0x00000000u, 0x00000000u}, {0x00000001u, 0xffffffffu},
        {0x00000002u, 0xfffffffeu}, {0x7fffffffu, 0x80000001u},
        {0x80000000u, 0x80000000u}, {0x80000001u, 0x7fffffffu},
        {0xffffffffu, 0x00000001u}, {0x3f800000u, 0xc0800000u},
        {0xbf800000u, 0x40800000u}, {0x7f800000u, 0x80800000u},
        {0xff800000u, 0x00800000u}, {0x7fc00000u, 0x80400000u},
        {0xffc00000u, 0x00400000u}, {0x00000080u, 0xffffff80u},
        {0x80000080u, 0x7fffff80u}, {0x12345678u, 0xedcba988u},
        {0xdeadbeefu, 0x21524111u},
    };
    for (const auto &test : cases)
    {
        const unsigned int result = Execute(test.input);
        CHECK(result == test.expected, "word_negate");
        CHECK(Execute(result) == test.input, "involution");
    }
    std::puts("PASS ECL raw negation");
}
