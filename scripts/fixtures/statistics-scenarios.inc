namespace fixture {
void Run(const Scenario &s) {
    Initialize(s);
    int result = GameFinalizeStatistics42B1E0(owner);
    CHECK(result == (s.openResult == 0 ? 0 : -1));
    VerifyRecords();
    CheckGuards();
    CHECK(owner->loadedPayload == loadedPayloadSentinel);
    CHECK(g_GameStatisticsRows == owner);
    CHECK(headerStorage[inputHeaderIndex].value.uncompressedSize == static_cast<int>(expectedPayload.size()));
    CHECK(headerStorage[inputHeaderIndex].value.compressedSize == 31);
    for (unsigned h = 0; h < expectedHeaders.size(); ++h)
        CHECK(std::memcmp(&headerStorage[h].value, &expectedHeaders[h], 0x18) == 0);
    CHECK(allocations.size() == 2);
    if (s.openResult != 0) {
        CHECK(allocations[0].live && allocations[1].live);
        CHECK(freeIndex == 0);
        CHECK(events == std::vector<std::string>({"allocate", "compress", "encrypt", "open"}));
    } else {
        CHECK(!allocations[0].live && !allocations[1].live);
        CHECK(freeIndex == 2);
        CHECK(events == std::vector<std::string>({"allocate", "compress", "encrypt", "open", "free-compressed", "free-scratch"}));
    }
    CHECK(writeIndex == expectedApiWrites);
    CHECK(observedIo == expectedIo);
    CHECK(gReplayFileHandle == expectedFinalHandle);
    CHECK(gReplayFileOpenCount == expectedFinalCount);
    Reclaim();
    ++completed;
}
void Accessors() {
    Initialize(Scenario{});
    for (unsigned i = 0; i < 7; ++i) {
        auto *record = reinterpret_cast<unsigned char *>(&owner->records[i]);
        for (int stage = 0; stage < 8; ++stage) {
            int score; signed char startup;
            std::memcpy(&score, record + stage * 0xf0 + 0x10, sizeof(score));
            std::memcpy(&startup, record + stage * 0xf0 + 0x15, sizeof(startup));
            CHECK(owner->records[i].StageInitialScore(stage) == score);
            CHECK(owner->records[i].StageStartupValue(stage) == startup);
        }
        CHECK(reinterpret_cast<unsigned char *>(&owner->records[i].sessionStarts) == record + 0x4c0);
        CHECK(reinterpret_cast<unsigned char *>(&owner->records[i].activeFrames) == record + 0x4c4);
    }
    ++completed;
}
void NullHeader() {
    Initialize(Scenario{});
    owner->header = nullptr;
    auto before = ownerStorage;
    int result = GameFinalizeStatistics42B1E0(owner);
    CHECK(result == -1);
    CHECK(events.empty() && allocations.empty());
    CHECK(std::memcmp(&ownerStorage, &before, sizeof(before)) == 0);
    CHECK(gReplayFileHandle == 62 && gReplayFileOpenCount == 1);
    ++completed;
}
} // namespace fixture
int main() {
    using namespace fixture;
    Accessors();
    NullHeader();
    for (unsigned mask = 0; mask < 128; ++mask) { Scenario s; s.mask = mask; Run(s); }
    for (int result : {-1, 1, 5}) { Scenario s; s.mask = 0x55; s.openResult = result; Run(s); }
    for (unsigned shorts = 0; shorts < 4; ++shorts)
        for (unsigned falseBools = 0; falseBools < 4; ++falseBools)
            for (unsigned count : {0u, 1u, 255u}) {
                Scenario s; s.mask = 0x7f; s.shortMask = shorts; s.falseBoolMask = falseBools;
                s.initialCount = static_cast<unsigned char>(count); Run(s);
            }
    for (int changeAt : {0, 1})
        for (HANDLE changed : {INVALID_HANDLE_VALUE, static_cast<HANDLE>(93)})
            for (unsigned shorts = 0; shorts < 4; ++shorts) {
                Scenario s; s.mask = 0x22; s.changeHandleAt = changeAt; s.changedHandle = changed;
                s.shortMask = shorts; s.initialCount = 0; Run(s);
            }
    { Scenario s; s.mask = 0x7f; s.openedHandle = INVALID_HANDLE_VALUE; Run(s); }
    for (unsigned swaps = 1; swaps < 32; ++swaps) {
        Scenario s; s.mask = 0x7f; s.swapAfterCompress = (swaps & 1u) != 0;
        s.swapAfterEncrypt = (swaps & 2u) != 0; s.swapAfterOpen = (swaps & 4u) != 0;
        s.swapAfterHeaderWrite = (swaps & 8u) != 0; s.swapAfterAllocate = (swaps & 16u) != 0; Run(s);
    }
    std::printf("PASS statistics save contract: %u scenarios, all 128 record masks, actual write/close protocol\n", completed);
}
