#pragma once

#include <stddef.h>

// Maintained reconstruction types. Target source strings retain the names
// ReplayFileHeader, ReplayDataHeader, ReplayStageDataHeader, ReplayRecData,
// ReplayFrameData, and ReplayInf, but they do not establish original C++
// declarations or physical translation-unit ownership by themselves.
struct ReplayFileHeader
{
    unsigned int magic;
    unsigned short version;
    unsigned char unknown006[0x06];
    unsigned int userDataOffset;
    unsigned char unknown010[0x0c];
    unsigned int compressedPayloadSize;
    unsigned int decompressedPayloadSize;
};
typedef char ReplayFileHeaderSizeIs24[
    (sizeof(ReplayFileHeader) == 0x24) ? 1 : -1];
typedef char ReplayFileHeaderUserDataOffsetAt0C[
    (offsetof(ReplayFileHeader, userDataOffset) == 0x0c) ? 1 : -1];
typedef char ReplayFileHeaderCompressedSizeAt1C[
    (offsetof(ReplayFileHeader, compressedPayloadSize) == 0x1c) ? 1 : -1];
typedef char ReplayFileHeaderDecompressedSizeAt20[
    (offsetof(ReplayFileHeader, decompressedPayloadSize) == 0x20) ? 1 : -1];

struct ReplayDataHeader
{
    char replayName[9];
    unsigned char unknown009[3];
    int timestamp;
    int score;
    unsigned char unknown014[0x34];
    float slowRate;
    int stageCount;
    int character;
    int shotType;
    int difficulty;
    int completionState;
    unsigned char unknown060[4];
};
typedef char ReplayDataHeaderSizeIs64[
    (sizeof(ReplayDataHeader) == 0x64) ? 1 : -1];
typedef char ReplayDataHeaderSlowRateAt48[
    (offsetof(ReplayDataHeader, slowRate) == 0x48) ? 1 : -1];
typedef char ReplayDataHeaderStageCountAt4C[
    (offsetof(ReplayDataHeader, stageCount) == 0x4c) ? 1 : -1];

struct ReplayStageDataHeader
{
    short stageIndex;
    unsigned short unknown002;
    int recordCount;
    int payloadSize;
    unsigned char unknown00c[0x1b8];
};
typedef char ReplayStageDataHeaderSizeIs1C4[
    (sizeof(ReplayStageDataHeader) == 0x1c4) ? 1 : -1];
typedef char ReplayStageDataHeaderRecordCountAt04[
    (offsetof(ReplayStageDataHeader, recordCount) == 0x04) ? 1 : -1];
typedef char ReplayStageDataHeaderPayloadSizeAt08[
    (offsetof(ReplayStageDataHeader, payloadSize) == 0x08) ? 1 : -1];

struct ReplayRecData
{
    unsigned char data[6];
};
typedef char ReplayRecDataSizeIs06[
    (sizeof(ReplayRecData) == 0x06) ? 1 : -1];

struct ReplayFrameData
{
    ReplayRecData records[3600];
    ReplayRecData *recordEnd;
    unsigned char fpsSamples[3600];
    unsigned char *fpsEnd;
};
typedef char ReplayFrameDataRecordEndAt5460[
    (offsetof(ReplayFrameData, recordEnd) == 0x5460) ? 1 : -1];
typedef char ReplayFrameDataFpsAt5464[
    (offsetof(ReplayFrameData, fpsSamples) == 0x5464) ? 1 : -1];
typedef char ReplayFrameDataFpsEndAt6274[
    (offsetof(ReplayFrameData, fpsEnd) == 0x6274) ? 1 : -1];
typedef char ReplayFrameDataSizeIs6278[
    (sizeof(ReplayFrameData) == 0x6278) ? 1 : -1];

struct ReplayBufferLink
{
    ReplayFrameData *frameData;
    ReplayBufferLink *next;
};

struct ReplayStageBuffer
{
    ReplayBufferLink *head;
    void *unknown004;
    void *unknown008;
};
typedef char ReplayStageBufferSizeIs0C[
    (sizeof(ReplayStageBuffer) == 0x0c) ? 1 : -1];

class ReplayManager
{
  public:
    int SaveReplay(const char *replayPath, const char *replayName);

    unsigned char unknown000[0x10];
    int mode;
    ReplayFileHeader *fileHeader;
    ReplayDataHeader *replayData;
    ReplayStageDataHeader *stageHeaders[8];
    unsigned int unknown03c;
    ReplayStageBuffer stageBuffers[8];
    // The target allocates and clears 0x2D4 bytes for the replay object.
    // Fields at and beyond +0xA0 are outside this packet and remain unknown.
    unsigned char unknown0A0[0x234];
};
typedef char ReplayManagerSizeIs2D4[
    (sizeof(ReplayManager) == 0x2d4) ? 1 : -1];
typedef char ReplayManagerFileHeaderAt14[
    (offsetof(ReplayManager, fileHeader) == 0x14) ? 1 : -1];
typedef char ReplayManagerReplayDataAt18[
    (offsetof(ReplayManager, replayData) == 0x18) ? 1 : -1];
typedef char ReplayManagerStageHeadersAt1C[
    (offsetof(ReplayManager, stageHeaders) == 0x1c) ? 1 : -1];
typedef char ReplayManagerStageBuffersAt40[
    (offsetof(ReplayManager, stageBuffers) == 0x40) ? 1 : -1];
