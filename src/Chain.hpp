#ifndef TH10_CHAIN_HPP
#define TH10_CHAIN_HPP

namespace th10
{

typedef int (__fastcall *ChainCallback)(void *arg);

struct ChainElem
{
    ChainElem();

    int prio;
    union
    {
        unsigned int flags;
        struct
        {
            unsigned int heapAllocated : 1;
            unsigned int otherFlags : 31;
        } flagBits;
    };
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
    ChainElem calcHead;
    ChainElem drawHead;

    Chain();

    ListNode *CalcList()
    {
        return reinterpret_cast<ListNode *>(&calcHead.self);
    }
    ListNode *DrawList()
    {
        return reinterpret_cast<ListNode *>(&drawHead.self);
    }

    __declspec(noinline) static ChainElem * __stdcall AllocElem(ChainCallback callback);
    __declspec(noinline) static int __stdcall AddToCalcChain(ChainElem *elem, int prio, Chain *chain);
    __declspec(noinline) static int __stdcall AddToDrawChain(ChainElem *elem, int prio, Chain *chain);
    void UnregisterElem(ChainElem *elem);
    static void __stdcall RemoveAllFromList(unsigned char *listBase, Chain *chain);
    static ChainElem *__stdcall RegisterCalc(void *arg, int prio, ChainCallback callback);
    static ChainElem *__stdcall RegisterDraw(void *arg, int prio, ChainCallback callback);
};

extern Chain *g_Chain;

} // namespace th10

#if defined(_MSC_VER) && defined(_M_IX86)
typedef char ChainElemSizeIs24[sizeof(th10::ChainElem) == 0x24 ? 1 : -1];
typedef char ChainSizeIs48[sizeof(th10::Chain) == 0x48 ? 1 : -1];
#endif

#endif
