#include "ReplayManager.hpp"

#include "FileSystem.hpp"
#include "Lzss.hpp"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace FileSystem
{
// Descriptive source-level interfaces for the observed write-file seam.
// OpenWriteFile is target-observed at 0x0044B620. The writer contains the
// WriteToOpenFile/CloseWriteFile behavior inline; their original source-level
// inlining and physical owner remain unknown.
int OpenWriteFile(const char *filename);
void WriteToOpenFile(const void *data, unsigned int size);
void CloseWriteFile();
}

#pragma pack(push, 4)
struct ReplayTimingSource
{
    unsigned char unknown000[0x24];
    double lagNumerator;
    double lagDenominator;
};
#pragma pack(pop)
typedef char ReplayTimingSourceNumeratorAt24[
    (offsetof(ReplayTimingSource, lagNumerator) == 0x24) ? 1 : -1];
typedef char ReplayTimingSourceDenominatorAt2C[
    (offsetof(ReplayTimingSource, lagDenominator) == 0x2c) ? 1 : -1];

struct ReplayUserDataHeader
{
    unsigned int magic;
    int size;
    unsigned char type;
    unsigned char unknown009[3];
};
typedef char ReplayUserDataHeaderSizeIs0C[
    (sizeof(ReplayUserDataHeader) == 0x0c) ? 1 : -1];

// Maintained descriptive external names. Their physical data owners are not
// established by this packet.
extern ReplayTimingSource *g_ReplayTimingSource;
extern int g_ReplayScore;
extern const char *g_ReplayCharacterNames[];
extern const char *g_ReplayDifficultyNames[];
extern const char g_ReplayUserInfoText[];
extern const char g_ReplayUserCommentText[];

struct ReplayInputSource
{
    unsigned int currentInput;
};

struct ReplayInputPair
{
    unsigned short current;
    unsigned short previous;
};

struct ReplayFpsSource
{
    unsigned char unknown000[0x34];
    float currentFps;
};

struct ReplayRuntimeState
{
    unsigned char unknown000[0x24];
    unsigned char replaySnapshot[0x34];
    unsigned int flags;
};
typedef char ReplayRuntimeStateSnapshotAt24[
    (offsetof(ReplayRuntimeState, replaySnapshot) == 0x24) ? 1 : -1];
typedef char ReplayRuntimeStateFlagsAt58[
    (offsetof(ReplayRuntimeState, flags) == 0x58) ? 1 : -1];

typedef int (*ReplayChainCallback)(ReplayManager *manager);

struct ReplayChainElement
{
    int priority;
    unsigned int isHeapAllocated : 1;
    unsigned int unknownFlag1 : 1;
    unsigned int unknownFlags : 30;
    ReplayChainCallback callback;
    ReplayChainCallback addedCallback;
    ReplayChainCallback deletedCallback;
    ReplayListNode link;
    ReplayManager *argument;

    ReplayChainElement()
    {
        priority = 0;
        isHeapAllocated = 0;
        callback = NULL;
        addedCallback = NULL;
        deletedCallback = NULL;
        link.value = this;
        link.next = NULL;
        link.previous = NULL;
    }
};
typedef char ReplayChainElementSizeIs24[
    (sizeof(ReplayChainElement) == 0x24) ? 1 : -1];
typedef char ReplayChainElementCallbackAt08[
    (offsetof(ReplayChainElement, callback) == 0x08) ? 1 : -1];
typedef char ReplayChainElementLinkAt14[
    (offsetof(ReplayChainElement, link) == 0x14) ? 1 : -1];
typedef char ReplayChainElementArgumentAt20[
    (offsetof(ReplayChainElement, argument) == 0x20) ? 1 : -1];

extern ReplayRuntimeState *g_ReplayRuntime;
extern ReplayInputSource g_ReplayInputSource;
extern ReplayInputPair g_ReplayInput;
extern unsigned short g_ReplayInputHoldCounter;
extern unsigned short g_ReplayAuxiliaryInput;
extern unsigned short g_ReplayInputFlags;
extern unsigned int g_ReplayInputBehaviorFlags;
extern ReplayFpsSource *g_ReplayFpsSource;
extern int g_ReplayCurrentStage;
extern int g_ReplayCharacter;
extern int g_ReplayShotType;
extern int g_ReplayDifficulty;
extern unsigned short g_ReplayStageSeed;
extern int g_ReplayStageSeedCounter;
extern unsigned int g_ReplayStageFlags;
extern unsigned int g_ReplayStageValue00C;
extern unsigned short g_ReplayStageValue010;
extern unsigned int g_ReplayStageValue014;
extern unsigned int g_ReplayStageValue018;
extern unsigned int g_ReplayStageValue01C;
extern unsigned int g_ReplayStageValue020;
extern unsigned int g_ReplayStageValue1B4;
extern unsigned int g_ReplayStageValue1B8;
extern unsigned char g_ReplayStageTimer[];
extern void *g_ReplayChainManager;

// These names describe independently reviewed TH10 helpers. Their source/TU
// ownership is not promoted by this lifecycle packet.
void ReplayUpdateInputSource(ReplayInputSource *source);
bool ReplayAppendRecord(ReplayFrameData *frameData, unsigned short input,
                        unsigned short auxiliaryInput, unsigned short inputFlags);
ReplayListNode *ReplayAllocateFrameData(ReplayManager *manager, int stage);
void ReplayClearStageFrameData(ReplayManager *manager, int stage);
void ReplayCutChain(ReplayChainElement *element);
void ReplayInitializeStageTimer(void *timer, int value);
void ReplaySetStageTimerValue(void *timer, int value);
int ReplayAddToCalcChain(void *chainManager, ReplayChainElement *element, int priority);
int ReplayAddToDrawChain(void *chainManager, ReplayChainElement *element, int priority);
int ReplayProcessFrameCallback(ReplayManager *manager);
int ReplayPlaybackFrameControlCallback(ReplayManager *manager);
int ReplayDrawFpsCallback(ReplayManager *manager);

ReplayStageState::ReplayStageState()
{
    memset(this, 0, sizeof(*this));
    recordCursor = recordStart;
    fpsCursor = fpsStart;
    link.value = this;
}

ReplayStageState::~ReplayStageState()
{
    if (link.next != NULL)
        link.next->previous = link.previous;
    if (link.previous != NULL)
        link.previous->next = link.next;
    link.next = NULL;
    link.previous = NULL;
}

ReplayManager::ReplayManager()
{
    memset(this, 0, sizeof(*this));
}

ReplayManager::~ReplayManager()
{
    free(fileHeader);

    for (int stage = 0; stage < 8; stage++)
        ReplayClearStageFrameData(this, stage);

    free(replayData);
    replayData = NULL;

    for (int stage = 0; stage < 8; stage++)
    {
        free(stageHeaders[stage]);
        stageHeaders[stage] = NULL;
    }

    if (updateChain != NULL)
        ReplayCutChain(updateChain);
    if (playbackChain != NULL)
        ReplayCutChain(playbackChain);
    if (drawChain != NULL)
        ReplayCutChain(drawChain);

    if (this == g_ReplayManager)
        g_ReplayManager = NULL;
}

int ReplayManager::Initialize(int replayMode, const char *path)
{
    mode = replayMode;

    if (mode == REPLAY_MANAGER_RECORD)
    {
        g_ReplayManager = this;

        ReplayClearStageFrameData(this, g_ReplayCurrentStage);
        currentFrameLink = ReplayAllocateFrameData(this, g_ReplayCurrentStage);

        fileHeader = new ReplayFileHeader;
        memset(fileHeader, 0, sizeof(*fileHeader));
        fileHeader->magic = 0x72303174;
        fileHeader->version = 5;
        fileHeader->gameVersion = 0x100;

        replayData = new ReplayDataHeader;
        memset(replayData, 0, sizeof(*replayData));

        ReplayStageDataHeader *stageHeader = new ReplayStageDataHeader;
        memset(stageHeader, 0, sizeof(*stageHeader));
        stageHeaders[g_ReplayCurrentStage] = stageHeader;

        replayData->character = g_ReplayCharacter;
        replayData->shotType = g_ReplayShotType;
        replayData->difficulty = g_ReplayDifficulty;
        if (g_ReplayRuntime != NULL)
        {
            memcpy(replayData->runtimeSnapshot, g_ReplayRuntime->replaySnapshot,
                   sizeof(replayData->runtimeSnapshot));
        }

        stageHeader->stageIndex = (short)g_ReplayCurrentStage;
        stageHeader->unknown002 = g_ReplayStageSeed;
        g_ReplayStageSeedCounter = 0;
        stageHeader->unknownFlag1C0 = g_ReplayStageFlags;
        if (g_ReplayStageFlags != 0)
        {
            stageHeader->unknown024 = 0;
            stageHeader->unknown028 = 0;
        }
        stageHeader->unknown00C = g_ReplayStageValue00C;
        stageHeader->unknown010 = g_ReplayStageValue010;
        stageHeader->unknown014 = g_ReplayStageValue014;
        stageHeader->unknown018 = g_ReplayStageValue018;
        stageHeader->unknown01C = g_ReplayStageValue01C;
        stageHeader->unknown020 = g_ReplayStageValue020;
        stageHeader->unknown1B4 = g_ReplayStageValue1B4;
        replayData->unknown060 = g_ReplayStageValue1B4;

        updateChain = new ReplayChainElement;
        updateChain->callback = ReplayProcessFrameCallback;
        updateChain->argument = this;
        updateChain->unknownFlag1 = 0;
        updateChain->isHeapAllocated = 1;
        ReplayAddToCalcChain(g_ReplayChainManager, updateChain, 0x0b);

        playbackChain = new ReplayChainElement;
        playbackChain->callback = ReplayPlaybackFrameControlCallback;
        playbackChain->argument = this;
        playbackChain->unknownFlag1 = 0;
        playbackChain->isHeapAllocated = 1;
        ReplayAddToCalcChain(g_ReplayChainManager, playbackChain, 0x1b);

        drawChain = new ReplayChainElement;
        drawChain->callback = ReplayDrawFpsCallback;
        drawChain->argument = this;
        drawChain->unknownFlag1 = 0;
        drawChain->isHeapAllocated = 1;
        ReplayAddToDrawChain(g_ReplayChainManager, drawChain, 5);

        activeStage = g_ReplayCurrentStage;
        return 0;
    }

    if (mode == REPLAY_MANAGER_PLAYBACK)
    {
        g_ReplayManager = this;
        if (LoadReplay(path) != 0)
            return -1;

        memcpy(g_ReplayRuntime->replaySnapshot, replayData->runtimeSnapshot,
               sizeof(replayData->runtimeSnapshot));

        ReplayStageState &stageState = stageStates[g_ReplayCurrentStage];
        ReplayStageDataHeader *stageHeader = stageState.header;
        stageState.recordCursor = stageState.recordStart;
        stageState.recordIndex = 0;
        stageState.fpsCursor = stageState.fpsStart;

        g_ReplayCharacter = replayData->character;
        g_ReplayShotType = replayData->shotType;
        g_ReplayDifficulty = replayData->difficulty;
        g_ReplayStageSeed = stageHeader->unknown002;
        g_ReplayStageSeedCounter = 0;
        g_ReplayStageValue00C = stageHeader->unknown00C;
        g_ReplayStageValue010 = stageHeader->unknown010;
        ReplayInitializeStageTimer(g_ReplayStageTimer,
                                   stageHeader->unknown014 * 10);
        ReplaySetStageTimerValue(g_ReplayStageTimer, stageHeader->unknown018);
        g_ReplayStageValue01C = stageHeader->unknown01C;
        g_ReplayStageValue020 = stageHeader->unknown020;
        g_ReplayStageValue1B4 = stageHeader->unknown1B4;
        g_ReplayStageValue1B8 = stageHeader->unknown1B8;

        updateChain = new ReplayChainElement;
        updateChain->callback = ReplayProcessFrameCallback;
        updateChain->argument = this;
        updateChain->unknownFlag1 = 0;
        updateChain->isHeapAllocated = 1;
        ReplayAddToCalcChain(g_ReplayChainManager, updateChain, 0x0b);

        playbackChain = new ReplayChainElement;
        playbackChain->callback = ReplayPlaybackFrameControlCallback;
        playbackChain->argument = this;
        playbackChain->unknownFlag1 = 0;
        playbackChain->isHeapAllocated = 1;
        ReplayAddToCalcChain(g_ReplayChainManager, playbackChain, 0x1b);

        drawChain = new ReplayChainElement;
        drawChain->callback = ReplayDrawFpsCallback;
        drawChain->argument = this;
        drawChain->unknownFlag1 = 0;
        drawChain->isHeapAllocated = 1;
        ReplayAddToDrawChain(g_ReplayChainManager, drawChain, 5);

        activeStage = -1;
        return 0;
    }

    if (mode == REPLAY_MANAGER_LOAD_ONLY)
        return LoadReplay(path) == 0 ? 0 : -1;

    return 0;
}

ReplayManager *ReplayManager::Create(int replayMode, const char *path)
{
    ReplayManager *manager = new ReplayManager;
    if (manager->Initialize(replayMode, path) != 0)
    {
        if (manager != NULL)
            delete manager;
        return NULL;
    }
    return manager;
}

ReplayManager *ReplayManager::Load(const char *path)
{
    ReplayManager *manager = new ReplayManager;
    if (manager->Initialize(REPLAY_MANAGER_LOAD_ONLY, path) != 0)
    {
        if (manager != NULL)
            delete manager;
        return NULL;
    }
    return manager;
}

void ReplayManager::Destroy(ReplayManager *replayManager)
{
    ReplayManager *manager = replayManager;
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
}

int ReplayManager::ProcessFrame()
{
    if (g_ReplayRuntime == NULL)
        return 1;

    if (mode == REPLAY_MANAGER_RECORD)
    {
        g_ReplayInput.previous = g_ReplayInput.current;

        unsigned short input =
            (unsigned short)(g_ReplayInputSource.currentInput & 0x01f7);
        if ((g_ReplayInputBehaviorFlags & 0x0200) != 0)
        {
            if ((input & 0x0001) != 0)
            {
                g_ReplayInputHoldCounter++;
                if (g_ReplayInputHoldCounter >= 8)
                {
                    input |= 0x0004;
                    g_ReplayInputHoldCounter = 8;
                }
            }
            else
            {
                g_ReplayInputHoldCounter = 0;
            }
        }
        g_ReplayInput.current = input;
        ReplayUpdateInputSource(&g_ReplayInputSource);

        ReplayFrameData *frameData =
            (ReplayFrameData *)currentFrameLink->value;
        if (frameCounter % 30 == 0)
        {
            float fps = g_ReplayFpsSource->currentFps + 0.5f;
            *frameData->fpsEnd =
                fps >= 256.0f ? 0xff : (unsigned char)fps;
            frameData->fpsEnd++;
        }

        if (ReplayAppendRecord(frameData, g_ReplayInput.current,
                               g_ReplayAuxiliaryInput, g_ReplayInputFlags))
        {
            currentFrameLink = ReplayAllocateFrameData(this, activeStage);
        }
    }
    else if (activeStage >= 0)
    {
        ReplayStageState &stageState = stageStates[activeStage];
        if (stageState.recordIndex < stageState.header->recordCount)
        {
            ReplayRecData *record = (ReplayRecData *)stageState.recordCursor;
            g_ReplayInput.current = record->input;
            g_ReplayAuxiliaryInput = record->auxiliaryInput;
            g_ReplayInputFlags = record->inputFlags;
            stageState.recordCursor += sizeof(ReplayRecData);

            replayFps = *stageState.fpsCursor;
            if (frameCounter % 30 == 0)
                stageState.fpsCursor++;
        }
        else
        {
            g_ReplayInput.current = 0;
            g_ReplayAuxiliaryInput = 0;
            g_ReplayInputFlags = 0;
        }
        stageState.recordIndex++;
    }
    else
    {
        g_ReplayInput.current = 0;
        g_ReplayAuxiliaryInput = 0;
        g_ReplayInputFlags = 0;
    }

    frameCounter++;
    return 1;
}

int ReplayManager::SaveReplay(const char *replayPath, const char *replayName)
{
    strcpy(replayData->replayName, replayName);
    int replayNameLength = strlen(replayName);
    for (int i = replayNameLength; i < 8; i++)
        replayData->replayName[i] = ' ';

    _mkdir("replay");

    char fullPath[256];
    sprintf(fullPath, "replay/%s", replayPath);

    int allocationSize = sizeof(ReplayDataHeader);
    int stageCount = 0;
    int firstStage = 0;
    int lastStage = 0;

    for (int stage = 0; stage < 8; stage++)
    {
        ReplayStageDataHeader *stageHeader = stageHeaders[stage];
        if (stageHeader == NULL)
            continue;

        if (firstStage == 0)
            firstStage = stage;

        stageHeader->payloadSize = 0;
        allocationSize += sizeof(ReplayStageDataHeader);

        for (ReplayListNode *link = stageFrameLists[stage].next;
             link != NULL; link = link->next)
        {
            ReplayFrameData *frameData = (ReplayFrameData *)link->value;
            int recordCount = frameData->recordEnd - frameData->records;
            int recordBytes = recordCount * sizeof(ReplayRecData);
            int fpsBytes = frameData->fpsEnd - frameData->fpsSamples;

            allocationSize += recordBytes + fpsBytes;
            stageHeader->payloadSize += recordBytes + fpsBytes;
            stageHeader->recordCount += recordCount;
        }

        stageCount++;
        lastStage = stage;
    }

    replayData->stageCount = stageCount;
    replayData->score = g_ReplayScore;
    replayData->slowRate =
        100.0f -
        (float)(g_ReplayTimingSource->lagNumerator /
                g_ReplayTimingSource->lagDenominator) *
            100.0f;

    unsigned char *uncompressedData =
        (unsigned char *)malloc(allocationSize);
    memcpy(uncompressedData, replayData, sizeof(ReplayDataHeader));

    int uncompressedSize = sizeof(ReplayDataHeader);
    for (int stage = 0; stage < 8; stage++)
    {
        ReplayStageDataHeader *stageHeader = stageHeaders[stage];
        if (stageHeader == NULL)
            continue;

        memcpy(uncompressedData + uncompressedSize, stageHeader,
               sizeof(ReplayStageDataHeader));
        uncompressedSize += sizeof(ReplayStageDataHeader);

        for (ReplayListNode *link = stageFrameLists[stage].next;
             link != NULL; link = link->next)
        {
            ReplayFrameData *frameData = (ReplayFrameData *)link->value;
            int recordCount = frameData->recordEnd - frameData->records;
            int recordBytes = recordCount * sizeof(ReplayRecData);
            memcpy(uncompressedData + uncompressedSize, frameData->records,
                   recordBytes);
            uncompressedSize += recordBytes;

            int fpsBytes = frameData->fpsEnd - frameData->fpsSamples;
            memcpy(uncompressedData + uncompressedSize, frameData->fpsSamples,
                   fpsBytes);
            uncompressedSize += fpsBytes;
        }
    }

    int compressedSize;
    unsigned char *compressedData =
        CompressData(uncompressedData, uncompressedSize, &compressedSize);
    free(uncompressedData);

    FileSystem::Encrypt(compressedData, compressedSize, 0x3d, 0x7a, 0x80,
                        compressedSize);
    FileSystem::Encrypt(compressedData, compressedSize, 0xaa, 0xe1, 0x400,
                        compressedSize);

    fileHeader->decompressedPayloadSize = uncompressedSize;
    fileHeader->compressedPayloadSize = compressedSize;
    fileHeader->userDataOffset = compressedSize + sizeof(ReplayFileHeader);

    FileSystem::OpenWriteFile(fullPath);
    FileSystem::WriteToOpenFile(fileHeader, sizeof(ReplayFileHeader));
    FileSystem::WriteToOpenFile(compressedData, compressedSize);
    if (compressedData != NULL)
        free(compressedData);

    unsigned char *userData = (unsigned char *)malloc(0xffff);
    memset(userData, 0, 0xffff);

    ReplayUserDataHeader *userDataHeader =
        (ReplayUserDataHeader *)userData;
    userDataHeader->magic = 0x52455355;
    userDataHeader->type = 0;

    char *userDataCursor = (char *)(userDataHeader + 1);
    userDataCursor += sprintf(userDataCursor, g_ReplayUserInfoText);
    userDataCursor += sprintf(userDataCursor, "Version %s\r\n", "1.00a");
    userDataCursor +=
        sprintf(userDataCursor, "Name %s\r\n", replayData->replayName);

    tm *localTime = localtime((time_t *)&replayData->timestamp);
    userDataCursor += sprintf(
        userDataCursor, "Date %.2d/%.2d/%.2d %.2d:%.2d\r\n",
        localTime->tm_year % 100, localTime->tm_mon + 1, localTime->tm_mday,
        localTime->tm_hour, localTime->tm_min);

    userDataCursor += sprintf(
        userDataCursor, "Chara %s\r\n",
        g_ReplayCharacterNames[replayData->character * 3 + replayData->shotType]);
    userDataCursor += sprintf(
        userDataCursor, "Rank %s\r\n",
        g_ReplayDifficultyNames[replayData->difficulty]);

    if (replayData->completionState > 7)
    {
        if (firstStage == 7)
            userDataCursor += sprintf(userDataCursor, "Extra Stage Clear\r\n");
        else
            userDataCursor += sprintf(userDataCursor, "Stage All Clear\r\n");
    }
    else if (firstStage == lastStage)
    {
        if (firstStage == 7)
            userDataCursor += sprintf(userDataCursor, "Extra Stage\r\n");
        else
            userDataCursor +=
                sprintf(userDataCursor, "Stage %d\r\n", firstStage);
    }
    else
    {
        // The retail text uses the Shift-JIS wave-dash between stage numbers.
        userDataCursor += sprintf(userDataCursor, "Stage %d \x81\x60 %d\r\n",
                                  firstStage, lastStage);
    }

    userDataCursor +=
        sprintf(userDataCursor, "Score %d\r\n", replayData->score);
    userDataCursor += sprintf(userDataCursor, "Slow Rate %2.2f\r\n",
                              replayData->slowRate);
    userDataCursor++;

    int userDataSize = userDataCursor - (char *)userData;
    int userDataRemainder = userDataSize % 4;
    if (userDataRemainder != 0)
        userDataCursor += 4 - userDataRemainder;

    userDataSize = userDataCursor - (char *)userData;
    userDataHeader->size = userDataSize;
    FileSystem::WriteToOpenFile(userData, userDataSize);

    memset(userData, 0, 0xffff);
    userDataHeader = (ReplayUserDataHeader *)userData;
    userDataHeader->magic = 0x52455355;
    userDataHeader->type = 1;
    userDataCursor = (char *)(userDataHeader + 1);
    userDataCursor += sprintf(userDataCursor, g_ReplayUserCommentText);
    userDataCursor++;

    userDataSize = userDataCursor - (char *)userData;
    userDataRemainder = userDataSize % 4;
    if (userDataRemainder != 0)
        userDataCursor += 4 - userDataRemainder;

    userDataSize = userDataCursor - (char *)userData;
    userDataHeader->size = userDataSize;
    FileSystem::WriteToOpenFile(userData, userDataSize);

    free(userData);
    FileSystem::CloseWriteFile();
    return 0;
}
