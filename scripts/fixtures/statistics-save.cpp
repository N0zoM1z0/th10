// Local host-only contract: actual owner source + canonical declarations + actual
// FileSystem write/close bodies. Compressor, Encrypt, open and Win32 are seams.
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#define __stdcall
#define __fastcall
#define CHECK(expression) do { if (!(expression)) { std::fprintf(stderr, "STATISTICS_CHECK_FAILED line %d: %s\n", __LINE__, #expression); std::exit(1); } } while (0)

@CANONICAL_STATISTICS_HEADER@
@CANONICAL_CALLEE_HEADERS@

// Host-only storage constructor seam. Real file loading/initialization is outside
// this save contract; scenarios explicitly initialize every observed field.
GameStatisticsOwner::GameStatisticsOwner() {}

// Explicit native-host representation. The canonical owner has two native
// pointers, so its record block moves by 8 bytes on host64. Production assertions
// are not asserted true on this host and the production declarations are intact.
static_assert(sizeof(void *) == 8, "fixture requires an explicit host64 seam");
static_assert(sizeof(unsigned int) == 4 && sizeof(int) == 4 && sizeof(unsigned short) == 2, "target scalar widths");
static_assert(offsetof(GameStatisticsOwner, header) == 0 && offsetof(GameStatisticsOwner, loadedPayload) == 8, "host pointer fields");
static_assert(offsetof(GameStatisticsOwner, records) == 16, "host record offset");
static_assert(offsetof(GameStatisticsOwner, tail) == 0x1d874, "host tail offset");
static_assert(sizeof(GameStatisticsOwner) == 0x1dcc0, "host tail padding");
static_assert(offsetof(GameStatisticsRecord, recordIndex) == 0xc && offsetof(GameStatisticsRecord, checksum) == 4, "record fields");
static_assert(offsetof(GameStatisticsTail, payload) == 8 && offsetof(GameStatisticsTail, checksum) == 4, "tail fields");

using HANDLE = std::intptr_t;
using DWORD = unsigned int;
using BOOL = int;
struct CRITICAL_SECTION { int unused; };
constexpr HANDLE INVALID_HANDLE_VALUE = -1;
HANDLE gReplayFileHandle;
CRITICAL_SECTION gReplayFileCriticalSection;
unsigned char gReplayFileOpenCount;
GameStatisticsOwner *g_GameStatisticsRows;
BOOL WriteFile(HANDLE, const void *, DWORD, DWORD *, void *);
BOOL CloseHandle(HANDLE);
void LeaveCriticalSection(CRITICAL_SECTION *);

namespace fixture {
constexpr std::size_t ScratchSize = 0x200000;
constexpr std::size_t GuardSize = 32;
struct Allocation { unsigned char *raw; unsigned char *data; std::size_t size; bool live; bool compressed; };
template <typename T> struct Guarded { unsigned char prefix[GuardSize]; T value; unsigned char suffix[GuardSize]; };
struct Scenario {
    unsigned mask = 0;
    int openResult = 0;
    HANDLE openedHandle = 71;
    unsigned char initialCount = 1;
    unsigned shortMask = 0;
    unsigned falseBoolMask = 0;
    int changeHandleAt = -1;
    HANDLE changedHandle = 93;
    bool swapAfterAllocate = false;
    bool swapAfterCompress = false;
    bool swapAfterEncrypt = false;
    bool swapAfterOpen = false;
    bool swapAfterHeaderWrite = false;
};
Scenario scenario;
Guarded<GameStatisticsOwner> ownerStorage;
std::array<Guarded<GameStatisticsFileHeader>, 6> headerStorage;
GameStatisticsOwner *owner;
GameStatisticsFileHeader initialHeader;
std::array<GameStatisticsFileHeader, 6> expectedHeaders;
GameStatisticsFileHeader expectedHeaderSnapshot;
std::array<GameStatisticsRecord, 7> expectedRecords;
GameStatisticsTail expectedTail;
std::vector<unsigned char> expectedPayload;
std::vector<Allocation> allocations;
std::vector<std::string> events;
std::vector<std::string> expectedIo;
std::vector<std::string> observedIo;
unsigned char *scratchPointer;
unsigned char *compressedPointer;
unsigned char ignoredEncryptResult[256];
unsigned char loadedPayloadSentinel[16];
unsigned inputHeaderIndex;
int writeIndex;
int expectedApiWrites;
int freeIndex;
int expectedPayloadWriteSize;
GameStatisticsFileHeader *expectedHeaderWrite;
HANDLE expectedFinalHandle;
unsigned char expectedFinalCount;
unsigned completed;

void CheckGuards() {
    for (unsigned char byte : ownerStorage.prefix) CHECK(byte == 0xa5);
    for (unsigned char byte : ownerStorage.suffix) CHECK(byte == 0xa5);
    for (const auto &header : headerStorage) {
        for (unsigned char byte : header.prefix) CHECK(byte == 0xa5);
        for (unsigned char byte : header.suffix) CHECK(byte == 0xa5);
    }
    for (const Allocation &a : allocations) {
        CHECK(std::all_of(a.raw, a.raw + GuardSize, [](unsigned char c) { return c == 0xa5; }));
        CHECK(std::all_of(a.data + a.size, a.data + a.size + GuardSize, [](unsigned char c) { return c == 0xa5; }));
    }
}
unsigned char *NewAllocation(std::size_t size, bool compressed) {
    unsigned char *raw = static_cast<unsigned char *>(std::malloc(size + 2 * GuardSize));
    CHECK(raw != nullptr);
    std::memset(raw, 0xa5, size + 2 * GuardSize);
    unsigned char *data = raw + GuardSize;
    std::memset(data, 0x87, size);
    allocations.push_back({raw, data, size, true, compressed});
    return data;
}
void *Allocate(std::size_t size) {
    CHECK(size == ScratchSize);
    CHECK(allocations.empty());
    events.push_back("allocate");
    if (scenario.swapAfterAllocate) owner->header = &headerStorage[5].value;
    scratchPointer = NewAllocation(size, false);
    return scratchPointer;
}
void Release(void *pointer) {
    CHECK(pointer == (freeIndex == 0 ? compressedPointer : scratchPointer));
    auto found = std::find_if(allocations.begin(), allocations.end(), [pointer](const Allocation &a) { return a.data == pointer; });
    CHECK(found != allocations.end() && found->live);
    CheckGuards();
    found->live = false;
    events.push_back(found->compressed ? "free-compressed" : "free-scratch");
    ++freeIndex;
    // Reclaim only after owner return, allowing guards/leak contracts to be
    // checked safely without permitting wrong frees or dereferencing freed data.
}
unsigned int Read32(const void *source) { unsigned int value; std::memcpy(&value, source, 4); return value; }
void Put32(void *target, unsigned int value) { std::memcpy(target, &value, 4); }
unsigned int Sum(const unsigned char *bytes, unsigned size) {
    unsigned int sum = 0;
    for (unsigned i = 0; i != size; ++i) sum += static_cast<unsigned int>(bytes[i]);
    return sum;
}
void VerifyRecords() {
    for (unsigned i = 0; i < 7; ++i) {
        const GameStatisticsRecord &record = owner->records[i];
        const GameStatisticsRecord &expectedRecord = expectedRecords[i];
        CHECK(record.recordIndex == expectedRecord.recordIndex);
        CHECK(record.checksum == expectedRecord.checksum);
        CHECK(std::memcmp(&record, &expectedRecord, sizeof(record)) == 0);
    }
    CHECK(owner->tail.checksum == expectedTail.checksum);
    CHECK(std::memcmp(&owner->tail, &expectedTail, sizeof(expectedTail)) == 0);
}
std::string WriteEvent(HANDLE handle, unsigned size, int index) {
    return "write:" + std::to_string(handle) + ":" + std::to_string(size) + ":" + std::to_string(index);
}
std::string CloseEvent(HANDLE handle, unsigned count) {
    return "close:" + std::to_string(handle) + ":" + std::to_string(count);
}
std::string LeaveEvent(unsigned count) { return "leave:" + std::to_string(count); }
void PlanIo() {
    expectedIo.clear();
    HANDLE handle = scenario.openedHandle;
    unsigned char count = scenario.initialCount;
    int index = 0;
    if (scenario.openResult == 0) {
        for (int stage = 0; stage < 2; ++stage) {
            if (handle == INVALID_HANDLE_VALUE) continue;
            unsigned size = stage == 0 ? 0x18u : static_cast<unsigned>(expectedPayloadWriteSize);
            expectedIo.push_back(WriteEvent(handle, size, index));
            if (scenario.changeHandleAt == index) handle = scenario.changedHandle;
            if ((scenario.shortMask >> index) & 1u) {
                expectedIo.push_back(CloseEvent(handle, count));
                expectedIo.push_back(LeaveEvent(count));
                --count;
            }
            ++index;
        }
        if (handle != INVALID_HANDLE_VALUE) {
            expectedIo.push_back(CloseEvent(handle, count));
            expectedIo.push_back(LeaveEvent(count));
            --count;
        }
    }
    expectedApiWrites = index;
    expectedFinalHandle = handle;
    expectedFinalCount = count;
}
void Initialize(const Scenario &s) {
    scenario = s;
    owner = &ownerStorage.value;
    // Deliberate byte-pattern fixture initialization, including guard regions.
    std::fill_n(reinterpret_cast<unsigned char *>(&ownerStorage), sizeof(ownerStorage), 0xa5);
    for (unsigned h = 0; h < headerStorage.size(); ++h) {
        std::memset(&headerStorage[h], 0xa5, sizeof(headerStorage[h]));
        auto &header = headerStorage[h].value;
        header.unknown00 = 0xd801c300u + h;
        header.fileSize = 0xb5000010u + h;
        header.unknown08 = 0x91cde000u + h;
        header.unknown0C = 0xfeac7600u + h;
        header.compressedSize = 31 + h * 7;
        header.uncompressedSize = 0x13570000 + h;
    }
    owner->header = &headerStorage[0].value;
    owner->loadedPayload = loadedPayloadSentinel;
    g_GameStatisticsRows = owner;
    inputHeaderIndex = s.swapAfterAllocate ? 5u : 0u;
    initialHeader = headerStorage[inputHeaderIndex].value;
    for (unsigned h = 0; h < headerStorage.size(); ++h) expectedHeaders[h] = headerStorage[h].value;
    expectedPayload.clear();
    for (unsigned i = 0; i < 7; ++i) {
        auto *bytes = reinterpret_cast<unsigned char *>(&owner->records[i]);
        for (unsigned j = 0; j < sizeof(GameStatisticsRecord); ++j)
            bytes[j] = static_cast<unsigned char>((j * 73u + i * 31u + 0x83u) & 255u);
        owner->records[i].magic = (s.mask & (1u << i)) ? 0x5243 : static_cast<unsigned short>(0x5240 + (i & 1u));
        owner->records[i].recordIndex = 0xd9e3ab70u + i;
        owner->records[i].checksum = 0xca9db100u + i;
        expectedRecords[i] = owner->records[i];
        if (s.mask & (1u << i)) {
            auto *expected = reinterpret_cast<unsigned char *>(&expectedRecords[i]);
            Put32(expected + 0xc, i);
            Put32(expected + 4, Sum(expected + 8, 0x4374));
            expectedPayload.insert(expectedPayload.end(), expected, expected + 0x437c);
        }
    }
    auto *tail = reinterpret_cast<unsigned char *>(&owner->tail);
    for (unsigned j = 0; j < sizeof(GameStatisticsTail); ++j)
        tail[j] = static_cast<unsigned char>((j * 41u + 0xbfu) & 255u);
    owner->tail.checksum = 0xdf15ab19u;
    expectedTail = owner->tail;
    auto *expected = reinterpret_cast<unsigned char *>(&expectedTail);
    Put32(expected + 4, Sum(expected + 8, 0x440));
    expectedPayload.insert(expectedPayload.end(), expected, expected + 0x448);
    allocations.clear();
    allocations.reserve(2);
    events.clear();
    observedIo.clear();
    scratchPointer = compressedPointer = nullptr;
    writeIndex = freeIndex = 0;
    gReplayFileHandle = 62;
    gReplayFileOpenCount = s.initialCount;
    unsigned headerIndex = s.swapAfterOpen ? 3 : s.swapAfterEncrypt ? 2 : s.swapAfterCompress ? 1 : inputHeaderIndex;
    expectedHeaders[inputHeaderIndex].uncompressedSize = static_cast<int>(expectedPayload.size());
    expectedHeaders[inputHeaderIndex].compressedSize = 31;
    unsigned sizeHeaderIndex = s.swapAfterCompress ? 1u : inputHeaderIndex;
    expectedHeaders[sizeHeaderIndex].fileSize = expectedHeaders[sizeHeaderIndex].compressedSize + 0x18;
    expectedHeaderWrite = &headerStorage[headerIndex].value;
    expectedHeaderSnapshot = expectedHeaders[headerIndex];
    expectedPayloadWriteSize = s.swapAfterHeaderWrite ? headerStorage[4].value.compressedSize : expectedHeaders[headerIndex].compressedSize;
    PlanIo();
}
void Reclaim() {
    for (const Allocation &a : allocations) std::free(a.raw);
    allocations.clear();
}
} // namespace fixture

#define malloc fixture::Allocate
#define free fixture::Release
@ACTUAL_STATISTICS_SOURCE@
#undef malloc
#undef free

namespace FileSystem {
@ACTUAL_WRITE_CLOSE_SOURCE@
} // namespace FileSystem

@MOCK_BOUNDARIES@
@SCENARIOS@
