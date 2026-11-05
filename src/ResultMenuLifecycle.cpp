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
    elem->flagBits.pendingRemoval = 0;
    elem->arg = this;
    th10::g_Chain->AddToCalcChain(elem, 9);
    updateTask = elem;

    elem = th10::Chain::AllocElem(
        reinterpret_cast<th10::ChainCallback>(&ResultMenuDrawRowsCallback));
    elem->arg = this;
    elem->flagBits.pendingRemoval = 0;
    th10::g_Chain->AddToDrawChain(elem, 46);
    drawTask = elem;
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

// Target 0x00422200: mark both frame callback elements for removal.
// This only sets the flag; the full controller cleanup at 0x422220 later
// unregisters them under the scheduler critical section.
void ResultMenuMarkTasks422200(ResultMenuView *owner)
{
    th10::ChainElem *task = owner->updateTask;
    if (task != NULL)
        task->flagBits.pendingRemoval = 1;
    task = owner->drawTask;
    if (task != NULL)
        task->flagBits.pendingRemoval = 1;
}
