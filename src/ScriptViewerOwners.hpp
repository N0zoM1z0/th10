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

// Reviewed lifecycle views. The Bomb has a target-measured 0x48-byte
// allocation and timer layout; the Popup has a target-measured 0xB884 layout.
// Original physical translation units and class declarations are unknown.
// Both destructor boundaries take one stack pointer and return with RET4;
// these are measured LTCG boundaries, not source ABI annotations.
struct ScriptViewerBombOwnerView
{
    unsigned int flags;
    unsigned int unknown004;
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    unsigned int unknown010;
    AnmVmTimerView timer;
    int mode28;
    // VM id word. The constructor clears it only through its memset, so the
    // slot avoids AnmVmIdView's default constructor; uses read it as an id.
    unsigned int vmId2C;
    AnmFloat3View position30;
    float collisionRadius;
    unsigned char opaque040[8];
    ScriptViewerBombOwnerView();
    __declspec(noinline) int RegisterTasks();
    __declspec(noinline) int Update();
    __declspec(noinline) int EmitAuxiliary();
    ~ScriptViewerBombOwnerView();
};
typedef char ScriptViewerBombOwnerSize48[
    (sizeof(ScriptViewerBombOwnerView) == 0x48) ? 1 : -1];
typedef char ScriptViewerBombTimerAt014[
    (offsetof(ScriptViewerBombOwnerView, timer) == 0x14 &&
     offsetof(ScriptViewerBombOwnerView, timer.flags) == 0x24) ? 1 : -1];

// Target record: 0x40 bytes at owner+0x3C4, with its timer at +0x1C.
// The record array is intentionally not a constructed C++ member: original
// outer-object initialization clears it after initializing its VM member.
struct ScriptViewerPopupRecord
{
    unsigned char digits[12];
    AnmFloat3View position;
    unsigned int color;
    AnmVmTimerView timer;
    unsigned char reserved30[8];
    unsigned char active;
    unsigned char digitCount;
    unsigned char reserved3A[6];
};
typedef char PopupRecordSize40[(sizeof(ScriptViewerPopupRecord) == 0x40) ? 1 : -1];
typedef char PopupRecordTimerAt1C[(offsetof(ScriptViewerPopupRecord, timer) == 0x1C) ? 1 : -1];

struct ScriptViewerPopupOwnerView
{
    unsigned char unknown000[8];
    th10::ChainElem *updateTask;
    th10::ChainElem *drawTask;
    // Shared ASCII ANM file installed by Initialize, used by Draw's digit sprites.
    AnmLoadedView *loadedDigitAnm;
    int nextPopupIndex;
    AnmVmView vm;
    unsigned char opaque3C4[0xb884 - 0x3c4];
    // Keep the physical record storage unconstructed: the retail outer
    // constructor clears it after the ANM VM is initialized.
    ScriptViewerPopupRecord *PopupRecords()
    {
        return reinterpret_cast<ScriptViewerPopupRecord *>(opaque3C4);
    }
    __declspec(noinline) ScriptViewerPopupOwnerView();
    __declspec(noinline) int Initialize();
    __declspec(noinline) int Update();
    __declspec(noinline) int Draw();
    void EmitValuePopup(const AnmFloat3View *position, int value, unsigned int color);
    ~ScriptViewerPopupOwnerView();
};

typedef char ScriptViewerPopupOwnerTargetSizeB884[
    (sizeof(ScriptViewerPopupOwnerView) == 0xb884) ? 1 : -1];
typedef char ScriptViewerPopupRecordStorageAt3C4[
    (offsetof(ScriptViewerPopupOwnerView, opaque3C4) == 0x3c4 &&
     723 * sizeof(ScriptViewerPopupRecord) == 0xb884 - 0x3c4) ? 1 : -1];
typedef char ScriptViewerPopupDigitAnmAt010[
    (offsetof(ScriptViewerPopupOwnerView, loadedDigitAnm) == 0x10) ? 1 : -1];
typedef char ScriptViewerPopupVmOffset[
    (offsetof(ScriptViewerPopupOwnerView, vm) == 0x18 &&
     offsetof(ScriptViewerPopupOwnerView, vm) +
         offsetof(AnmVmView, generatedVertices) == 0x370) ? 1 : -1];

#endif
