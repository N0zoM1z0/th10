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

        for (ReplayBufferLink *link = stageBuffers[stage].head;
             link != NULL; link = link->next)
        {
            ReplayFrameData *frameData = link->frameData;
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

        for (ReplayBufferLink *link = stageBuffers[stage].head;
             link != NULL; link = link->next)
        {
            ReplayFrameData *frameData = link->frameData;
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
