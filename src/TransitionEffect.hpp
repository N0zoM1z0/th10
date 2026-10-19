#ifndef TH10_TRANSITION_EFFECT_HPP
#define TH10_TRANSITION_EFFECT_HPP
#include "AnmManager.hpp"
#include "Chain.hpp"
#include <stddef.h>

// Target allocation is 0x44 bytes, zeroed as seventeen dwords. This is an
// observed storage view, not a recovered original class or ownership claim.
struct TransitionEffectView
{
    unsigned int flags;
    unsigned int unknown04;
    th10::ChainElem *updateNode;
    th10::ChainElem *drawNode;
    int kind;
    unsigned int unknown14;
    int alpha;
    int frames;
    unsigned int value0;
    unsigned int value1;
    unsigned int value2;
    int released;
    AnmVmTimerView timer;

    TransitionEffectView();
    void Initialize(int drawPriority, int effectKind, int duration,
        unsigned int first, unsigned int second, unsigned int third);
    void Release();
    void Cleanup();
};
typedef char TransitionEffectPhysicalLayout[
    (sizeof(TransitionEffectView) == 0x44 &&
     offsetof(TransitionEffectView, updateNode) == 8 &&
     offsetof(TransitionEffectView, drawNode) == 0x0c &&
     offsetof(TransitionEffectView, kind) == 0x10 &&
     offsetof(TransitionEffectView, alpha) == 0x18 &&
     offsetof(TransitionEffectView, frames) == 0x1c &&
     offsetof(TransitionEffectView, value0) == 0x20 &&
     offsetof(TransitionEffectView, released) == 0x2c &&
     offsetof(TransitionEffectView, timer) == 0x30) ? 1 : -1];

// Target passes priority in EBX and five stack arguments with RET20.
// This callee-pop spelling is a maintained compiler model, not the original
// source annotation or a whole-product ABI claim.
void *__stdcall StageCreateTransitionEffect(int drawPriority, int kind, int frames,
    unsigned int value0, unsigned int value1, unsigned int value2);
void DestroyTransitionEffect(TransitionEffectView *effect);
#endif
