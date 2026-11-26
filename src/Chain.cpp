// Initial list helpers adapted from external commit
// 649147241f6a931c849d905ca9badc264049db5b.
// Chain::UnregisterElem and Chain::RemoveAllFromList are canonical exact; see
// docs/KNOWLEDGE_BASE.md and config/match-units.toml.
#include "Chain.hpp"
#include "Main.hpp"
#include <windows.h>
#include <stdlib.h>
#include <new>
extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;
// Retail ChainElem callback slots: offsets independently established by
// the 0x449ED0 AllocElem and 0x449A90 destructor owners.
typedef char ChainCallbackSlotOffsets[
    (offsetof(th10::ChainElem, callback) == 0x08 &&
     offsetof(th10::ChainElem, addedCallback) == 0x0c &&
     offsetof(th10::ChainElem, deletedCallback) == 0x10) ? 1 : -1];

namespace th10
{

extern CRITICAL_SECTION g_CriticalSections[7];
extern u8 g_ChainNestCounter;

ChainElem::ChainElem()
{
    const unsigned int previousFlags = flags;
    callback = NULL;
    addedCallback = NULL;
    deletedCallback = NULL;
    prio = 0;
    // Preserve every flag except the allocation bit.
    flags = previousFlags & ~1u;
    // Publish the self link before clearing the two remaining list links.
    ChainElem *volatile &publishedSelf = self;
    publishedSelf = this;
    ChainElem *volatile &publishedNext = next;
    publishedNext = NULL;
    ChainElem *volatile &publishedPrev = prev;
    publishedPrev = NULL;
}

ChainElem::~ChainElem()
{
    callback = NULL;
    addedCallback = NULL;
    deletedCallback = NULL;
}

Chain::Chain()
{
}

// Target438880..4388E6 destroys the two sentinels after removing their lists.
Chain::~Chain()
{
    g_MainSupervisorView.modeWorker.Stop();
    RemoveAllFromList(reinterpret_cast<u8 *>(&calcHead), this);
    RemoveAllFromList(reinterpret_cast<u8 *>(&drawHead), this);
}

__declspec(noinline) ChainElem * __stdcall Chain::AllocElem(ChainCallback callback)
{
    ChainElem *elem;
    ChainCallback cb;

    elem = new ChainElem;

    cb = callback;
    elem->addedCallback = NULL;
    elem->deletedCallback = NULL;
    // Capture the stored flags before publishing heap ownership.
    const volatile unsigned int &oldFlags = elem->flags;
    const unsigned int heapFlags = oldFlags | 1u;
    elem->callback = cb;
    elem->flags = heapFlags;
    return elem;
}


__declspec(noinline) i32 Chain::AddToCalcChain(ChainElem *elem, i32 prio)
{
    Chain *chain = this;
    i32 result;
    result = 0;
    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }
    EnterCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN]++;
    {
        ListNode *cur;
        ListNode *node;
        elem->prio = prio;
        cur = chain->CalcList();
        while (cur->next != NULL)
        {
            if (cur->next->elem->prio >= prio)
                break;
            cur = cur->next;
        }
        node = (ListNode *)&elem->self;
        if (cur->next != NULL)
        {
            node->next = cur->next;
            cur->next->prev = node;
        }
        cur->next = node;
        node->prev = cur;
    }
    LeaveCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN]--;
    return result;
}

__declspec(noinline) i32 Chain::AddToDrawChain(ChainElem *elem, i32 prio)
{
    Chain *chain = this;
    i32 result;
    result = 0;
    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }
    EnterCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN]++;
    {
        ListNode *cur;
        ListNode *node;
        elem->prio = prio;
        cur = chain->DrawList();
        while (cur->next != NULL)
        {
            if (cur->next->elem->prio >= prio)
                break;
            cur = cur->next;
        }
        node = (ListNode *)&elem->self;
        if (cur->next != NULL)
        {
            node->next = cur->next;
            cur->next->prev = node;
        }
        cur->next = node;
        node->prev = cur;
    }
    LeaveCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN]--;
    return result;
}

void Chain::UnregisterElem(ChainElem *elem)
{
    ListNode *node;
    if (elem == NULL)
        return;
    node = CalcList();
    while (node != NULL)
    {
        if (node->elem == elem)
            goto found;
        node = node->next;
    }
    node = DrawList();
    while (node != NULL)
    {
        if (node->elem == elem)
            goto found;
        node = node->next;
    }
    return;
found:
    {
        ListNode *const prev = node->prev;
        ListNode *next;
        ListNode *prev2;
        if (prev == NULL)
            return;
        next = node->next;
        if (next != NULL)
            next->prev = prev;
        prev2 = node->prev;
        if (prev2 != NULL)
            prev2->next = node->next;
    }
    node->next = NULL;
    node->prev = NULL;
    elem->callback = NULL;
    if (elem->flags & 1)
    {
        elem->callback = NULL;
        elem->addedCallback = NULL;
        elem->deletedCallback = NULL;
        ::operator delete(elem);
    }
}

void __stdcall Chain::RemoveAllFromList(u8 *listBase, Chain *chain)
{
    ListNode *node;
    node = *(ListNode **)(listBase + 0x18);
    while (node != NULL)
    {
        ChainElem *elem;
        ListNode *next;
        elem = node->elem;
        next = node->next;
        if (elem != NULL)
        {
            EnterCriticalSection(&g_CriticalSections[0]);
            g_ChainNestCounter++;
            chain->UnregisterElem(elem);
            LeaveCriticalSection(&g_CriticalSections[0]);
            g_ChainNestCounter--;
        }
        node = next;
    }
}

ChainElem *__stdcall Chain::RegisterCalc(void *arg, i32 prio, ChainCallback callback)
{
    ChainElem *elem;
    elem = Chain::AllocElem(callback);
    elem->arg = arg;
    elem->flags |= 2;
    g_Chain->AddToCalcChain(elem, prio);
    return elem;
}

ChainElem *__stdcall Chain::RegisterDraw(void *arg, i32 prio, ChainCallback callback)
{
    ChainElem *elem;
    elem = Chain::AllocElem(callback);
    elem->arg = arg;
    elem->flags |= 2;
    g_Chain->AddToDrawChain(elem, prio);
    return elem;
}

} // namespace th10

// TH10 frame dispatchers. The names are descriptive; original source ownership
// is unknown. Both target entries take one stack argument and end in RET 4.
int __stdcall MainRunCalcChain(void *owner)
{
    th10::Chain *chain = static_cast<th10::Chain *>(owner);
    th10::ListNode *node;
    th10::ListNode *next;
    th10::ChainElem *elem;
    int updatedCount;
    int result;

    EnterCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    ++g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
restart:
    updatedCount = 0;
    node = chain->CalcList()->next;
    while (node != NULL)
    {
        next = node->next;
        elem = node->elem;
        if (elem->callback != NULL)
        {
execute_again:
            if (elem->flags & 2)
            {
                LeaveCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
                --g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
                result = elem->callback(elem->arg);
                EnterCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
                ++g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
                switch (result)
                {
                case th10::CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_NO_DELETE:
                    chain->UnregisterElem(elem);
                    ++updatedCount;
                    node = next;
                    continue;
                case th10::CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                    goto execute_again;
                case th10::CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                    updatedCount = 0;
                    goto loop_exit;
                case th10::CHAIN_CALLBACK_RESULT_BREAK:
                    updatedCount = 1;
                    goto loop_exit;
                case th10::CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                    updatedCount = -1;
                    goto loop_exit;
                case th10::CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB:
                    goto restart;
                case th10::CHAIN_CALLBACK_RESULT_INVOKE_DELETED_CALLBACK:
                    if (elem->deletedCallback != NULL)
                        elem->deletedCallback(elem->arg);
                    break;
                }
            }
            ++updatedCount;
        }
        node = next;
    }
loop_exit:
    LeaveCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    --g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
    return updatedCount;
}

int __stdcall MainRunDrawChain(void *owner)
{
    th10::Chain *chain = static_cast<th10::Chain *>(owner);
    th10::ListNode *node;
    th10::ListNode *next;
    th10::ChainElem *elem;
    int updatedCount;
    int result;

    EnterCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    ++g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
    updatedCount = 0;
    node = chain->DrawList()->next;
    while (node != NULL)
    {
        next = node->next;
        elem = node->elem;
        if (elem->callback != NULL)
        {
execute_again:
            if (elem->flags & 2)
            {
                LeaveCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
                --g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
                result = elem->callback(elem->arg);
                EnterCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
                ++g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
                switch (result)
                {
                case th10::CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_NO_DELETE:
                    chain->UnregisterElem(elem);
                    break;
                case th10::CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                    goto execute_again;
                case th10::CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                    updatedCount = 0;
                    goto loop_exit;
                case th10::CHAIN_CALLBACK_RESULT_BREAK:
                    updatedCount = 1;
                    goto loop_exit;
                case th10::CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                    updatedCount = -1;
                    goto loop_exit;
                }
            }
            ++updatedCount;
        }
        node = next;
    }
loop_exit:
    LeaveCriticalSection(&g_MainSupervisorView.criticalSections[SUPERVISOR_LOCK_CHAIN]);
    --g_MainSupervisorView.criticalSectionLockCounts[SUPERVISOR_LOCK_CHAIN];
    return updatedCount;
}
