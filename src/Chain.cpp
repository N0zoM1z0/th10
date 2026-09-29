// Natural C++ candidate adapted from th10-decomphelp-forN0/src/Chain.cpp.
// Chain::UnregisterElem and Chain::RemoveAllFromList are canonical exact; see
// docs/DECOMPHELP_REPO_REVIEW.md and config/match-units.toml.
#include <windows.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;
typedef i32 (__fastcall *ChainCallback)(void *arg);

namespace th10
{

struct ChainElem
{
    u32 prio;
    u32 flags;
    ChainCallback callback;
    ChainCallback addedCallback;
    ChainCallback deletedCallback;
    ChainElem *self;
    ChainElem *next;
    ChainElem *prev;
    void *arg;
};

struct ListNode
{
    ChainElem *elem;
    ListNode *next;
    ListNode *prev;
};

struct Chain
{
    u8 unk0000[0x14];
    ListNode calcList;
    u8 unk0020[0x18];
    ListNode drawList;

    __declspec(noinline) static ChainElem * __stdcall AllocElem(ChainCallback callback);
    __declspec(noinline) static i32 AddToCalcChain(ChainElem *elem, u32 prio, Chain *chain);
    __declspec(noinline) static i32 AddToDrawChain(ChainElem *elem, u32 prio, Chain *chain);
    static void UnregisterElem(ChainElem *elem, Chain *chain);
    static void __stdcall RemoveAllFromList(u8 *listBase, Chain *chain);
    static ChainElem *RegisterCalc(ChainCallback callback, u32 prio, void *arg);
    static ChainElem *RegisterDraw(ChainCallback callback, u32 prio, void *arg);
};

extern CRITICAL_SECTION g_CriticalSections[7];
extern u8 g_ChainNestCounter;
extern Chain *g_Chain;

__declspec(noinline) ChainElem * __stdcall Chain::AllocElem(ChainCallback callback)
{
    ChainElem *elem;
    ChainCallback cb;
    u32 flags;
    u32 flagsMasked;
    u32 flagsOrOne;

    elem = (ChainElem *)malloc(0x24);
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

static i32 FireAddedAndInsert(ChainElem *elem, u32 prio, ListNode *head)
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
        ListNode *first;
        ListNode *node;
        elem->prio = prio;
        cur = head;
        while (cur->next != NULL)
        {
            ListNode *n;
            n = cur->next;
            if (n->elem->prio >= prio)
                break;
            cur = n;
        }
        first = cur->next;
        node = (ListNode *)&elem->self;
        if (first != NULL)
        {
            node->next = first;
            first->prev = node;
        }
        cur->next = node;
        node->prev = cur;
    }
    g_ChainNestCounter--;
    LeaveCriticalSection(&g_CriticalSections[0]);
    return result;
}

__declspec(noinline) i32 Chain::AddToCalcChain(ChainElem *elem, u32 prio, Chain *chain)
{
    return FireAddedAndInsert(elem, prio, &chain->calcList);
}

__declspec(noinline) i32 Chain::AddToDrawChain(ChainElem *elem, u32 prio, Chain *chain)
{
    return FireAddedAndInsert(elem, prio, &chain->drawList);
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

ChainElem *Chain::RegisterCalc(ChainCallback callback, u32 prio, void *arg)
{
    ChainElem *elem;
    elem = Chain::AllocElem(callback);
    elem->arg = arg;
    elem->flags |= 2;
    Chain::AddToCalcChain(elem, prio, g_Chain);
    return elem;
}

ChainElem *Chain::RegisterDraw(ChainCallback callback, u32 prio, void *arg)
{
    ChainElem *elem;
    elem = Chain::AllocElem(callback);
    elem->arg = arg;
    elem->flags |= 2;
    Chain::AddToDrawChain(elem, prio, g_Chain);
    return elem;
}

} // namespace th10
