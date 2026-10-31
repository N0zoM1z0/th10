#include "Main.hpp"
#include "ReplayManager.hpp"
#include "Player.hpp"
#include <new>
#include <string.h>

// The frame allocation is 0x6284 bytes; pointer initialization follows its
// complete reset, as observed in the inlined constructor inside 42AA50.
ReplayFrameData::ReplayFrameData()
{
    memset(this, 0, sizeof(*this));
    ReplayRecData **cursor = &recordEnd;
    *cursor = records;
    fpsEnd = fpsSamples;
    link.value = this;
    link.next = NULL;
    link.previous = NULL;
}

// Target 42AA50 has two meaningful operands and consumes its stack manager.
// The private LTCG interface carries stage in ESI; no register is declared.
ReplayListNode *__stdcall ReplayAllocateFrameData(ReplayManager *manager, int stage)
{
    ReplayFrameData *data = new ReplayFrameData;
    ReplayListNode *node = &data->link;
    ReplayListNode *tail = &manager->stageFrameLists[stage];
    while (tail->next != NULL)
        tail = tail->next;
    if (tail->next != NULL)
    {
        node->next = tail->next;
        tail->next->previous = node;
    }
    tail->next = node;
    node->previous = tail;
    return node;
}

// Target 42AB20 captures the following link before releasing the current
// payload and preserves both conditional unlink writes and link clearing.
void ReplayClearStageFrameData(ReplayManager *manager, int stage)
{
    ReplayListNode *node = manager->stageFrameLists[stage].next;
    while (node != NULL)
    {
        ReplayListNode *next = node->next;
        ReplayFrameData *data = static_cast<ReplayFrameData *>(node->value);
        if (data != NULL)
        {
            if (data->link.next != NULL)
                data->link.next->previous = data->link.previous;
            if (data->link.previous != NULL)
                data->link.previous->next = data->link.next;
            data->link.next = NULL;
            data->link.previous = NULL;
            ::operator delete(data);
        }
        node = next;
    }
}

// Target 428E10 copies signed fixed-point coordinates, publishes their float
// counterparts and marks all four option positions for reset.
void ReplayRestoreStagePosition(Player *runtime, const unsigned int *position)
{
    *reinterpret_cast<PlayerPositionPair *>(&runtime->positionX) =
        *reinterpret_cast<const PlayerPositionPair *>(position);
    runtime->drawPosition.x = static_cast<float>(runtime->positionX) * 0.01f;
    runtime->drawPosition.y = static_cast<float>(runtime->positionY) * 0.01f;
    for (int i = 0; i < 4; ++i)
        runtime->options[i].resetFlag = 1;
}

// Target 42A8A0 appends all three record words before advancing the cursor.
// The signed record count selects whether the caller allocates another block.
int __stdcall ReplayAppendRecord(ReplayFrameData *frameData, unsigned short input,
                                unsigned short auxiliaryInput, unsigned short inputFlags)
{
    frameData->recordEnd->input = input;
    frameData->recordEnd->auxiliaryInput = auxiliaryInput;
    frameData->recordEnd->inputFlags = inputFlags;
    ++frameData->recordEnd;
    return frameData->recordEnd - frameData->records >= 3600;
}
