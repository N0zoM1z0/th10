unsigned char *__stdcall CompressData(unsigned char *input, int inputSize, int *outputSize) {
    using namespace fixture;
    CHECK(input == scratchPointer + 0x18);
    CHECK(inputSize == static_cast<int>(expectedPayload.size()));
    CHECK(outputSize == &headerStorage[inputHeaderIndex].value.compressedSize);
    CHECK(headerStorage[inputHeaderIndex].value.uncompressedSize == inputSize);
    CHECK(std::memcmp(scratchPointer, &initialHeader, 0x18) == 0);
    VerifyRecords();
    CHECK(std::memcmp(input, expectedPayload.data(), expectedPayload.size()) == 0);
    CHECK(events == std::vector<std::string>({"allocate"}));
    events.push_back("compress");
    compressedPointer = NewAllocation(256, true);
    for (unsigned i = 0; i < 256; ++i) compressedPointer[i] = static_cast<unsigned char>(0xa3u + i * 19u);
    *outputSize = 31;
    if (scenario.swapAfterCompress) owner->header = &headerStorage[1].value;
    return compressedPointer;
}

unsigned char *FileSystem::Encrypt(unsigned char *data, int size, unsigned char key,
                                   unsigned char step, int chunkSize, int maxBytes) {
    using namespace fixture;
    CHECK(data == compressedPointer);
    CHECK(size == owner->header->compressedSize);
    CHECK(key == 0xac && step == 0x35 && chunkSize == 0x10 && maxBytes == size);
    CHECK(owner->header->fileSize == static_cast<unsigned int>(owner->header->compressedSize + 0x18));
    CHECK(events == std::vector<std::string>({"allocate", "compress"}));
    CHECK(size >= 0 && size <= 256);
    for (int i = 0; i < size; ++i) data[i] ^= static_cast<unsigned char>(0xacu + i * 0x35u);
    events.push_back("encrypt");
    if (scenario.swapAfterEncrypt) owner->header = &headerStorage[2].value;
    return ignoredEncryptResult;
}

int __stdcall FileSystem::OpenWriteFile(const char *path) {
    using namespace fixture;
    CHECK(std::strcmp(path, "scoreth10.dat") == 0);
    CHECK(events == std::vector<std::string>({"allocate", "compress", "encrypt"}));
    CHECK(allocations.size() == 2 && allocations[0].live && allocations[1].live);
    events.push_back("open");
    gReplayFileHandle = scenario.openedHandle;
    if (scenario.swapAfterOpen) owner->header = &headerStorage[3].value;
    return scenario.openResult;
}

BOOL WriteFile(HANDLE handle, const void *data, DWORD size, DWORD *bytesWritten, void *overlapped) {
    using namespace fixture;
    CHECK(events == std::vector<std::string>({"allocate", "compress", "encrypt", "open"}));
    CHECK(overlapped == nullptr && bytesWritten != nullptr);
    CHECK(handle == gReplayFileHandle);
    if (writeIndex == 0) {
        CHECK(data == expectedHeaderWrite);
        CHECK(size == 0x18);
        CHECK(std::memcmp(data, &expectedHeaderSnapshot, 0x18) == 0);
    } else {
        CHECK(writeIndex == 1);
        CHECK(data == compressedPointer);
        CHECK(size == static_cast<unsigned>(expectedPayloadWriteSize));
    }
    observedIo.push_back(WriteEvent(handle, size, writeIndex));
    if (scenario.changeHandleAt == writeIndex) gReplayFileHandle = scenario.changedHandle;
    if (writeIndex == 0 && scenario.swapAfterHeaderWrite) owner->header = &headerStorage[4].value;
    *bytesWritten = ((scenario.shortMask >> writeIndex) & 1u) ? size - 1 : size;
    BOOL result = ((scenario.falseBoolMask >> writeIndex) & 1u) ? 0 : 1;
    ++writeIndex;
    return result;
}

BOOL CloseHandle(HANDLE handle) {
    using namespace fixture;
    observedIo.push_back(CloseEvent(handle, gReplayFileOpenCount));
    return 0; // Return value is deliberately ignored by actual protocol.
}
void LeaveCriticalSection(CRITICAL_SECTION *section) {
    using namespace fixture;
    CHECK(section == &gReplayFileCriticalSection);
    observedIo.push_back(LeaveEvent(gReplayFileOpenCount));
}
