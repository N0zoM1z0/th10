#ifndef TH10_SCRIPT_VIEWER_OWNERS_HPP
#define TH10_SCRIPT_VIEWER_OWNERS_HPP

#include "AnmManager.hpp"
#include "Chain.hpp"
#include <new>

// Adapted in part from levzzz5154/th10-decomp (MIT), copyright (c) 2026 levzzz,
// commit c6791d82dc9f02e55eca45a5314f900a85481efd. See docs/licenses/th10-decomp-MIT.txt.
// Capture the current owner once. If its destructor does not return, this
// protocol does not reach scalar deallocation (matching the straight-line
// target release entries); this is distinct from a host C++ delete expression.
template <class T>
inline void ScriptViewerDeleteOwned(T *object)
{
    if (object)
    {
        object->~T();
        ::operator delete(object);
    }
}

// Minimum lifecycle views. No allocation, complete original class size or
// production TU is established. Both destructor boundaries take one stack
// pointer and return with RET4; that is a measured LTCG boundary, not a source
// calling-convention annotation for these C++ destructors.
struct ScriptViewerBombOwnerView
{
    unsigned char unknown000[8];
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    ~ScriptViewerBombOwnerView();
};

struct ScriptViewerPopupOwnerView
{
    unsigned char unknown000[8];
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    unsigned int unknown010;
    unsigned int unknown014;
    AnmVmView vm;
    ~ScriptViewerPopupOwnerView();
};

typedef char ScriptViewerPopupVmOffset[
    (offsetof(ScriptViewerPopupOwnerView, vm) == 0x18 &&
     offsetof(ScriptViewerPopupOwnerView, vm) +
         offsetof(AnmVmView, generatedVertices) == 0x370) ? 1 : -1];

#endif
