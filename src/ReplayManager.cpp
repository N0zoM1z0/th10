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

extern void *g_ReplayRuntime;
extern ReplayInputSource g_ReplayInputSource;
extern ReplayInputPair g_ReplayInput;
extern unsigned short g_ReplayInputHoldCounter;
extern unsigned short g_ReplayAuxiliaryInput;
extern unsigned short g_ReplayInputFlags;
extern unsigned int g_ReplayInputBehaviorFlags;
extern ReplayFpsSource *g_ReplayFpsSource;

// These names describe independently reviewed TH10 helpers. Their source/TU
// ownership is not promoted by this lifecycle packet.
void ReplayUpdateInputSource(ReplayInputSource *source);
bool ReplayAppendRecord(ReplayFrameData *frameData, unsigned short input,
                        unsigned short auxiliaryInput, unsigned short inputFlags);
ReplayListNode *ReplayAllocateFrameData(ReplayManager *manager, int stage);
void ReplayClearStageFrameData(ReplayManager *manager, int stage);
void ReplayCutChain(void *element);

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
