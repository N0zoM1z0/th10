#include "ResultMenu.hpp"
#include "Chain.hpp"
#include <new>
#include <string.h>

// The controller is published at the original 0x477830 owner slot.  Keep
// that owner's existing type identity used by GameManager's cleanup caller.
struct GameChainOwnerView;
extern GameChainOwnerView *g_GameOwner477830;
extern void __stdcall GameCleanupController422220(GameChainOwnerView *owner);

// Original 0x422A90 adapts the update callback to ResultMenuUpdate (0x4223F0).
int __fastcall ResultMenuUpdateCallback(ResultMenuView *owner)
{
    return ResultMenuUpdate(owner);
}

// Original 0x4220E0: the timer and two cursor subobjects are initialized
// before the owning object is cleared. The observed clear spans all 0x2c8
// bytes; it must not be reduced to selected member initialization.
ResultMenuView::ResultMenuView()
{
    memset(this, 0, sizeof(*this));
    unknown000 |= 2;
    g_GameOwner477830 = reinterpret_cast<GameChainOwnerView *>(this);
}

// Original 0x422150: both frame callbacks are registered, then the timer is
// reset. The second callback is the already maintained exact seven-byte
// ResultMenuDrawRowsCallback at 0x422AA0.
int ResultMenuView::Initialize()
{
    th10::ChainElem *elem = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&ResultMenuUpdateCallback));
    elem->flags &= ~2u;
    elem->arg = this;
    th10::g_Chain->AddToCalcChain(elem, 9);
    *reinterpret_cast<th10::ChainElem **>(&unknown008[0]) = elem;

    elem = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&ResultMenuDrawRowsCallback));
    elem->arg = this;
    elem->flags &= ~2u;
    th10::g_Chain->AddToDrawChain(elem, 46);
    *reinterpret_cast<th10::ChainElem **>(&unknown008[4]) = elem;
    timer.SetCurrent(0);
    return 0;
}

// Original 0x422360 calls the actual constructor, initializer and the
// separately reviewed (still source-absent) controller cleanup. Preserve
// the retail null-allocation branch, not an invented failure success path.
void *GameCreateController()
{
    ResultMenuView *owner = new ResultMenuView;
    if (owner->Initialize() != 0)
    {
        if (owner != NULL)
        {
            GameCleanupController422220(
                reinterpret_cast<GameChainOwnerView *>(owner));
            ::operator delete(owner);
        }
        return NULL;
    }
    return owner;
}
