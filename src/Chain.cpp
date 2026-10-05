// Natural C++ candidate adapted from th10-decomphelp-forN0/src/Chain.cpp.
// Chain::UnregisterElem and Chain::RemoveAllFromList are canonical exact; see
// docs/DECOMPHELP_REPO_REVIEW.md and config/match-units.toml.
#include "Chain.hpp"
#include <windows.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

namespace th10
{

extern CRITICAL_SECTION g_CriticalSections[7];
extern u8 g_ChainNestCounter;

__declspec(noinline) ChainElem * __stdcall Chain::AllocElem(ChainCallback callback)
{
    ChainElem *elem;
    ChainCallback cb;
    u32 flags;
    u32 flagsMasked;
    u32 flagsOrOne;

    elem = new ChainElem;
    if (elem != NULL)
    {
        flags = *(u32 *)((u8 *)elem + 0x4);
        *(u32 *)((u8 *)elem + 0x8) = 0;
        *(u32 *)((u8 *)elem + 0xc) = 0;
        *(u32 *)((u8 *)elem + 0x10) = 0;
        *(u32 *)((u8 *)elem + 0x0) = 0;
        flagsMasked = flags & ~1;
        *(u32 *)((u8 *)elem + 0x4) = flagsMasked;
        elem->self = elem;
        elem->next = NULL;
        elem->prev = NULL;
    }
    else
    {
        elem = NULL;
    }

    cb = callback;
    *(ChainCallback *)((u8 *)elem + 0x0c) = NULL;
    *(ChainCallback *)((u8 *)elem + 0x10) = NULL;
    flagsOrOne = *(u32 *)((u8 *)elem + 0x4) | 1;
    *(ChainCallback *)((u8 *)elem + 0x8) = cb;
    *(u32 *)((u8 *)elem + 0x4) = flagsOrOne;
    return elem;
}


__declspec(noinline) i32 __stdcall Chain::AddToCalcChain(ChainElem *elem, i32 prio, Chain *chain)
{
    i32 result;
    result = 0;
    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }
    EnterCriticalSection(&g_CriticalSections[0]);
    g_ChainNestCounter++;
    {
        ListNode *cur;
        ListNode *node;
        elem->prio = prio;
        cur = &chain->calcList;
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
    LeaveCriticalSection(&g_CriticalSections[0]);
    g_ChainNestCounter--;
    return result;
}

__declspec(noinline) i32 __stdcall Chain::AddToDrawChain(ChainElem *elem, i32 prio, Chain *chain)
{
    i32 result;
    result = 0;
    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }
    EnterCriticalSection(&g_CriticalSections[0]);
    g_ChainNestCounter++;
    {
        ListNode *cur;
        ListNode *node;
        elem->prio = prio;
        cur = &chain->drawList;
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
    LeaveCriticalSection(&g_CriticalSections[0]);
    g_ChainNestCounter--;
    return result;
}

void Chain::UnregisterElem(ChainElem *elem, Chain *chain)
{
    ListNode *node;
    if (elem == NULL)
        return;
    node = &chain->calcList;
    while (node != NULL)
    {
        if (node->elem == elem)
            goto found;
        node = node->next;
    }
    node = &chain->drawList;
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
        free(elem);
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
            Chain::UnregisterElem(elem, chain);
            LeaveCriticalSection(&g_CriticalSections[0]);
            g_ChainNestCounter--;
        }
        node = next;
    }
}

ChainElem *Chain::RegisterCalc(ChainCallback callback, i32 prio, void *arg)
{
    ChainElem *elem;
    elem = Chain::AllocElem(callback);
    elem->arg = arg;
    elem->flags |= 2;
    Chain::AddToCalcChain(elem, prio, g_Chain);
    return elem;
}

ChainElem *Chain::RegisterDraw(ChainCallback callback, i32 prio, void *arg)
{
    ChainElem *elem;
    elem = Chain::AllocElem(callback);
    elem->arg = arg;
    elem->flags |= 2;
    Chain::AddToDrawChain(elem, prio, g_Chain);
    return elem;
}

} // namespace th10
