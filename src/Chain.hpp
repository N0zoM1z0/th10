#ifndef TH10_CHAIN_HPP
#define TH10_CHAIN_HPP

namespace th10
{

// Evidenced by MainRunCalcChain / MainRunDrawChain (src/Chain.cpp) and
// SupervisorModes.cpp. Note: MainRunDrawChain handles values 0, 2, 3, 4, 5;
// values 6 (restart) and 7 (invoke deletedCallback) are calc-chain specific.
enum ChainCallbackResult
{
    CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_NO_DELETE = 0,
    CHAIN_CALLBACK_RESULT_CONTINUE                      = 1,
    CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN                 = 2,
    CHAIN_CALLBACK_RESULT_BREAK                         = 3,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS             = 4,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR               = 5,
    CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB        = 6,
    CHAIN_CALLBACK_RESULT_INVOKE_DELETED_CALLBACK       = 7
};

typedef int (__fastcall *ChainCallback)(void *arg);

struct ChainElem
{
    ChainElem();
    ~ChainElem();

    int prio;
    union
    {
        unsigned int flags;
        struct
        {
            unsigned int heapAllocated : 1;
            unsigned int pendingRemoval : 1;
            unsigned int otherFlags : 30;
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

    __declspec(noinline) Chain();
    __declspec(noinline) ~Chain();

    ListNode *CalcList()
    {
        return reinterpret_cast<ListNode *>(&calcHead.self);
    }
    ListNode *DrawList()
    {
        return reinterpret_cast<ListNode *>(&drawHead.self);
    }

    __declspec(noinline) static ChainElem * __stdcall AllocElem(ChainCallback callback);
    __declspec(noinline) int AddToCalcChain(ChainElem *elem, int prio);
    __declspec(noinline) int AddToDrawChain(ChainElem *elem, int prio);
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
