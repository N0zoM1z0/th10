#include "Main.hpp"
#include "StageScript.hpp"
#include "Chain.hpp"
#include "TransitionEffect.hpp"

struct GameChainOwnerView;
struct GameMenuView;

typedef char StageAnimationSlotLayout[
    (offsetof(th10::ChainElem, flags) == 4 &&
     offsetof(StageObjectPrimitiveView, byteSize) == 2 &&
     offsetof(StageObjectPrimitiveView, unknown004) == 4 &&
     offsetof(StageObjectPrimitiveView, vmIndex) == 6 &&
     offsetof(StageScriptView, objectVms) == 0x17c &&
     offsetof(StageScriptView, transitionTimer) == 0x2a1c) ? 1 : -1];

// Target404450, called by the game update through its stage-owner view.
// Original source ownership is unknown; the observed stage layout supplies
// the object records, embedded VM bank and script-reset fields.
void __stdcall GamePrepareAnimationSlots(GameChainOwnerView *owner)
{
    StageScriptView *stage = reinterpret_cast<StageScriptView *>(owner);
    stage->updateChain->flags |= 2;
    stage->earlyDrawChain->flags |= 2;
    stage->lateDrawChain->flags |= 2;

    int vmIndex = 0;
    for (int objectIndex = 0; objectIndex < stage->data->objectCount; ++objectIndex)
    {
        stage->objects[objectIndex]->flags = 1;
        StageObjectPrimitiveView *primitive =
            &stage->objects[objectIndex]->firstPrimitive;
        while (primitive->type >= 0)
        {
            stage->objectVms[vmIndex].InitializeForLoadedScript(
                stage->anm, primitive->unknown004);
            primitive->vmIndex = static_cast<short>(vmIndex);
            ++vmIndex;
            primitive = reinterpret_cast<StageObjectPrimitiveView *>(
                reinterpret_cast<unsigned char *>(primitive) + primitive->byteSize);
        }
    }
    stage->instruction = reinterpret_cast<StageInstructionView *>(stage->scriptBase);
}

// Target404530 starts the alternate stage owner's30-frame transition.
void GameEnterMenuState(GameMenuView *owner)
{
    StageScriptView *stage = reinterpret_cast<StageScriptView *>(owner);
    // The target retains this real factory call at404540.
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif
    StageCreateTransitionEffect(15, 2, 30, 0, 0, 0);
#if defined(_MSC_VER)
#pragma inline_depth(16)
#endif
    stage->transitionTimer.SetCurrent(30);
    stage->flags |= 2;
}
