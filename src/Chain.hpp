#ifndef TH10_CHAIN_HPP
#define TH10_CHAIN_HPP

namespace th10
{

typedef int (__fastcall *ChainCallback)(void *arg);

struct ChainElem
{
    int prio;
    unsigned int flags;
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
    unsigned char unk0000[0x14];
    ListNode calcList;
    unsigned char unk0020[0x18];
    ListNode drawList;

    __declspec(noinline) static ChainElem * __stdcall AllocElem(ChainCallback callback);
    __declspec(noinline) static int __stdcall AddToCalcChain(ChainElem *elem, int prio, Chain *chain);
    __declspec(noinline) static int __stdcall AddToDrawChain(ChainElem *elem, int prio, Chain *chain);
    static void UnregisterElem(ChainElem *elem, Chain *chain);
    static void __stdcall RemoveAllFromList(unsigned char *listBase, Chain *chain);
    static ChainElem *RegisterCalc(ChainCallback callback, int prio, void *arg);
    static ChainElem *RegisterDraw(ChainCallback callback, int prio, void *arg);
};

extern Chain *g_Chain;

} // namespace th10

#endif
