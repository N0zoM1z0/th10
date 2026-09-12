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
    unsigned int gameVersion;
    unsigned char unknown014[0x08];
    unsigned int compressedPayloadSize;
    unsigned int decompressedPayloadSize;
};
typedef char ReplayFileHeaderSizeIs24[
    (sizeof(ReplayFileHeader) == 0x24) ? 1 : -1];
typedef char ReplayFileHeaderUserDataOffsetAt0C[
    (offsetof(ReplayFileHeader, userDataOffset) == 0x0c) ? 1 : -1];
typedef char ReplayFileHeaderGameVersionAt10[
    (offsetof(ReplayFileHeader, gameVersion) == 0x10) ? 1 : -1];
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
    unsigned char runtimeSnapshot[0x34];
    float slowRate;
    int stageCount;
    int character;
    int shotType;
    int difficulty;
    int completionState;
    unsigned int unknown060;
};
typedef char ReplayDataHeaderSizeIs64[
    (sizeof(ReplayDataHeader) == 0x64) ? 1 : -1];
typedef char ReplayDataHeaderRuntimeSnapshotAt14[
    (offsetof(ReplayDataHeader, runtimeSnapshot) == 0x14) ? 1 : -1];
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
    unsigned int unknown00C;
    unsigned short unknown010;
    unsigned short unknown012;
    unsigned int unknown014;
    unsigned int unknown018;
    unsigned int unknown01C;
    unsigned int unknown020;
    unsigned int unknown024;
    unsigned int unknown028;
    unsigned char unknown02C[0x188];
    unsigned int unknown1B4;
    unsigned int unknown1B8;
    unsigned char unknown1BC[4];
    unsigned int unknownFlag1C0 : 1;
    unsigned int unknownFlags1C0 : 31;
};
typedef char ReplayStageDataHeaderSizeIs1C4[
    (sizeof(ReplayStageDataHeader) == 0x1c4) ? 1 : -1];
typedef char ReplayStageDataHeaderRecordCountAt04[
    (offsetof(ReplayStageDataHeader, recordCount) == 0x04) ? 1 : -1];
typedef char ReplayStageDataHeaderPayloadSizeAt08[
    (offsetof(ReplayStageDataHeader, payloadSize) == 0x08) ? 1 : -1];
typedef char ReplayStageDataHeaderUnknown00CAt0C[
    (offsetof(ReplayStageDataHeader, unknown00C) == 0x0c) ? 1 : -1];
typedef char ReplayStageDataHeaderUnknown1B4At1B4[
    (offsetof(ReplayStageDataHeader, unknown1B4) == 0x1b4) ? 1 : -1];
typedef char ReplayStageDataHeaderUnknown1B8At1B8[
    (offsetof(ReplayStageDataHeader, unknown1B8) == 0x1b8) ? 1 : -1];

struct ReplayRecData
{
    unsigned short input;
    unsigned short auxiliaryInput;
    unsigned short inputFlags;
};
typedef char ReplayRecDataSizeIs06[
    (sizeof(ReplayRecData) == 0x06) ? 1 : -1];

struct ReplayListNode
{
    void *value;
    ReplayListNode *next;
    ReplayListNode *previous;
};
typedef char ReplayListNodeSizeIs0C[
    (sizeof(ReplayListNode) == 0x0c) ? 1 : -1];

struct ReplayFrameData
{
    ReplayRecData records[3600];
    ReplayRecData *recordEnd;
    unsigned char fpsSamples[3600];
    unsigned char *fpsEnd;
    ReplayListNode link;
};
typedef char ReplayFrameDataRecordEndAt5460[
    (offsetof(ReplayFrameData, recordEnd) == 0x5460) ? 1 : -1];
typedef char ReplayFrameDataFpsAt5464[
    (offsetof(ReplayFrameData, fpsSamples) == 0x5464) ? 1 : -1];
typedef char ReplayFrameDataFpsEndAt6274[
    (offsetof(ReplayFrameData, fpsEnd) == 0x6274) ? 1 : -1];
typedef char ReplayFrameDataLinkAt6278[
    (offsetof(ReplayFrameData, link) == 0x6278) ? 1 : -1];
typedef char ReplayFrameDataSizeIs6284[
    (sizeof(ReplayFrameData) == 0x6284) ? 1 : -1];

struct ReplayStageState
{
    unsigned char *recordStart;
    unsigned char *recordCursor;
    unsigned char *fpsStart;
    unsigned char *fpsCursor;
    ReplayStageDataHeader *header;
    int recordIndex;
    ReplayListNode link;

    ReplayStageState();
    ~ReplayStageState();
};
typedef char ReplayStageStateSizeIs24[
    (sizeof(ReplayStageState) == 0x24) ? 1 : -1];
typedef char ReplayStageStateHeaderAt10[
    (offsetof(ReplayStageState, header) == 0x10) ? 1 : -1];
typedef char ReplayStageStateRecordIndexAt14[
    (offsetof(ReplayStageState, recordIndex) == 0x14) ? 1 : -1];
typedef char ReplayStageStateLinkAt18[
    (offsetof(ReplayStageState, link) == 0x18) ? 1 : -1];

enum ReplayManagerMode
{
    REPLAY_MANAGER_RECORD = 0,
    REPLAY_MANAGER_PLAYBACK = 1,
    REPLAY_MANAGER_LOAD_ONLY = 2,
};

struct ReplayChainElement;

class ReplayManager
{
  public:
    ReplayManager();
    ~ReplayManager();

    int Initialize(int mode, const char *replayPath);
    int LoadReplay(const char *replayPath);
    int SaveReplay(const char *replayPath, const char *replayName);

    static ReplayManager *Create(int mode, const char *replayPath);
    static ReplayManager *Load(const char *replayPath);
    static void Destroy(ReplayManager *replayManager);
    int ProcessFrame();

    // Names below are maintained descriptions of target-observed roles. The
    // original member identifiers and physical data owner remain unproven.
    unsigned char unknown000[0x08];
    ReplayChainElement *updateChain;
    ReplayChainElement *drawChain;
    int mode;
    ReplayFileHeader *fileHeader;
    ReplayDataHeader *replayData;
    ReplayStageDataHeader *stageHeaders[8];
    ReplayListNode stageFrameLists[8];
    ReplayListNode *currentFrameLink;
    ReplayStageState stageStates[8];
    unsigned char *decompressedPayload;
    unsigned char replayFps;
    unsigned char unknown1C5[3];
    int frameCounter;
    ReplayChainElement *playbackChain;
    int activeStage;
    char replayPath[0x100];
};
typedef char ReplayManagerSizeIs2D4[
    (sizeof(ReplayManager) == 0x2d4) ? 1 : -1];
typedef char ReplayManagerModeAt10[
    (offsetof(ReplayManager, mode) == 0x10) ? 1 : -1];
typedef char ReplayManagerFileHeaderAt14[
    (offsetof(ReplayManager, fileHeader) == 0x14) ? 1 : -1];
typedef char ReplayManagerReplayDataAt18[
    (offsetof(ReplayManager, replayData) == 0x18) ? 1 : -1];
typedef char ReplayManagerStageHeadersAt1C[
    (offsetof(ReplayManager, stageHeaders) == 0x1c) ? 1 : -1];
typedef char ReplayManagerStageFrameListsAt3C[
    (offsetof(ReplayManager, stageFrameLists) == 0x3c) ? 1 : -1];
typedef char ReplayManagerCurrentFrameLinkAt9C[
    (offsetof(ReplayManager, currentFrameLink) == 0x9c) ? 1 : -1];
typedef char ReplayManagerStageStatesAtA0[
    (offsetof(ReplayManager, stageStates) == 0xa0) ? 1 : -1];
typedef char ReplayManagerDecompressedPayloadAt1C0[
    (offsetof(ReplayManager, decompressedPayload) == 0x1c0) ? 1 : -1];
typedef char ReplayManagerReplayFpsAt1C4[
    (offsetof(ReplayManager, replayFps) == 0x1c4) ? 1 : -1];
typedef char ReplayManagerFrameCounterAt1C8[
    (offsetof(ReplayManager, frameCounter) == 0x1c8) ? 1 : -1];
typedef char ReplayManagerPlaybackChainAt1CC[
    (offsetof(ReplayManager, playbackChain) == 0x1cc) ? 1 : -1];
typedef char ReplayManagerActiveStageAt1D0[
    (offsetof(ReplayManager, activeStage) == 0x1d0) ? 1 : -1];
typedef char ReplayManagerReplayPathAt1D4[
    (offsetof(ReplayManager, replayPath) == 0x1d4) ? 1 : -1];

extern ReplayManager *g_ReplayManager;
