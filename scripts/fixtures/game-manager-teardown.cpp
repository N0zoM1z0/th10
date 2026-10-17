// HOST-ONLY fixture. No declarations or mocks in this file are native source.
// Actual caller source is lexically inserted below; mock boundaries test ordering,
// publications, object identity, legal lifetime, and deterministic lock sequencing.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <new>
#include <type_traits>
#include <vector>
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(value)
#define CHECK(expression) do { if (!(expression)) { \
    std::fprintf(stderr, "GAME_TEARDOWN_CHECK_FAILED line %d: %s\n", __LINE__, #expression); \
    std::exit(1); } } while (0)

typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned int ZunBool;
@CHAIN_DECLARATIONS@
@VM_ID_DECLARATIONS@
@FLOAT3_DECLARATION@
struct PlayerTimerView { int previous, current; float subframe; float *scale; unsigned int flags; };
@GAME_DECLARATIONS@
// Opaque native type only; the host does not construct or destroy GuiView.
struct GuiView;
@REPLAY_DECLARATIONS@
struct StageScriptView { ~StageScriptView(); };
struct Player { ~Player(); };
struct EnemyManagerView { ~EnemyManagerView(); };
struct HintOwnerView { unsigned int untouched; };
// The exact accessed items type and count are retained. This is deliberately a
// byte-store seam for the otherwise unused nontrivial ANM-backed item records.
// It tests caller wipe bounds, not ItemManager native layout or member cleanup.
struct ItemRecordView { unsigned char bytes[0x3f0]; };
struct ItemManagerView {
    unsigned char prefix[0x14];
    ItemRecordView items[0x896];
    unsigned char suffix[0x10];
    ~ItemManagerView();
};
static_assert(offsetof(ItemManagerView, items) == 0x14, "host byte-store seam offset");
static_assert(sizeof(((ItemManagerView *)0)->items) == 0x21cea0, "host byte-store seam length");
struct CRITICAL_SECTION { int tag; };
struct MainSupervisorView {
    unsigned int options;
    signed char hintMode;
    CRITICAL_SECTION criticalSections[7];
    unsigned char criticalSectionLockCounts[7];
};
struct AnmLoadedView {
    AnmVmIdView CreateVmAtScreenVariant0(int scriptIndex, const AnmFloat3View *position);
};
struct AsciiManagerView {
    AnmLoadedView *asciiAnm;
    int auxiliaryVm89A4;
    int EnsureSelectionVm(float x, float y);
};
@GLOBALS_AND_BOUNDARIES@

namespace fixture {
enum Id { Replay, Primary, Secondary, Owner830, Gui, PlayerId, Owner6F0,
          Item, Owner81C, Hint, Owner840, Enemy, Owner6FC, Owner6EC, Owner6F4, Game, NoId };
enum Kind { Stats, CreateVm, Selection, Advance, Cleanup, Release, ResetGui,
            ClearEnemy, Enter, Unregister, Leave, Sound };
struct Event { Kind kind; Id id; bool operator==(const Event &other) const { return kind == other.kind && id == other.id; } };
struct Allocation {
    void *pointer; Id id; bool live, cleaned; void (*dispose)(void *);
};
std::vector<Event> events, expected;
Allocation allocations[24];
int allocationCount, released[NoId], cleaned[NoId], cases;
bool finalizing, criticalHeld;
unsigned char initialDepth;
th10::Chain chains[5];
GameChainElementView managerElements[4], ownerElements[4];
ReplayChainElement replayElements[3];
GameManager *activeManager;
HintOwnerView *savedHint;
ItemManagerView *savedItem;
StageScriptView *savedPrimary, *savedSecondary;
GameChainOwnerView *saved830, *saved6F0;
ReplayManager *savedReplay;
AnmLoadedView loaded;
AsciiManagerView ascii, alternateAscii;
th10::Chain *chainBeforeEnter;
GameChainElementView *elementBeforeEnter;
int enterCount, unregisterCount, soundCount, selectionCount, createCount;
unsigned int expectedInitialFlags, expectedFinalFlags;
int expectedTransition, expectedOpcode;
float expectedX, expectedY;
bool expectCreation, checkExactWipe, switchChains;
enum Effect { None, StatsChangesFlags, VmChangesTransition, SelectionChangesTransition,
              SelectionChangesCompare, PrimaryClearsSecondary, GuiChangesFlags,
              FinalCleanupChangesFlags, SoundChangesFlags, VmChangesAscii, VmChangesFlags, SelectionChangesFlags };
Effect effect;
struct Scenario {
    int transition = 0;
    unsigned int flags = 0x80000003u;
    bool equal = true;
    bool existingVm = false;
    int missing = -1;
    int chainMask = 3;
    unsigned int options = 0;
    unsigned char depth = 3;
    int hintMode = 2;
    unsigned int callbackMask = 63;
    bool mutateChains = false;
    Effect effect = None;
};

Allocation &Find(const void *pointer) {
    for (int i = 0; i < allocationCount; ++i)
        if (allocations[i].pointer == pointer) return allocations[i];
    CHECK(false && "unregistered owner address");
    std::abort();
}
template<class T> T *Allocate(Id id) {
    T *pointer = new T();
    CHECK(allocationCount < 24);
    allocations[allocationCount++] = Allocation{pointer, id, true, false,
        [](void *storage) { delete static_cast<T *>(storage); }};
    return pointer;
}
// Trivial allocation identity for the opaque GUI boundary, not a GUI layout.
struct GuiAllocationToken { unsigned char unused; };
GuiView *AllocateGui() { return reinterpret_cast<GuiView *>(Allocate<GuiAllocationToken>(Gui)); }
void Record(Kind kind, Id id = NoId) { events.push_back(Event{kind, id}); }
void ClearPublishedOwner(Id id);
void ObserveGuiDestructor(GuiView *owner);
void Clean(void *pointer) {
    if (finalizing) return;
    Allocation &allocation = Find(pointer);
    CHECK(allocation.live && !allocation.cleaned);
    CHECK(!criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == initialDepth);
    allocation.cleaned = true;
    ++cleaned[allocation.id];
    Record(Cleanup, allocation.id);
    ClearPublishedOwner(allocation.id);
}
template<class T> constexpr bool NaturalDestructor =
    std::is_same<T, ReplayManager>::value || std::is_same<T, StageScriptView>::value ||
    std::is_same<T, Player>::value || std::is_same<T, ItemManagerView>::value ||
    std::is_same<T, EnemyManagerView>::value;
template<class T> void HostDelete(T *pointer) {
    if (pointer == NULL) return; // C++ delete-null contract, also exercised by mutant.
    Allocation &allocation = Find(pointer);
    CHECK(allocation.live);
    CHECK(NaturalDestructor<T> || allocation.cleaned);
    pointer->~T();
    CHECK(allocation.cleaned);
    CHECK(!criticalHeld);
    if (allocation.id == Hint) CHECK(g_GameOwner477814 == NULL);
    Record(Release, allocation.id);
    ++released[allocation.id];
    allocation.live = false;
    allocation.pointer = NULL;
    ::operator delete(pointer);
}
template<class T> void HostFree(T *pointer) {
    static_assert(std::is_trivially_destructible<T>::value, "raw release seam needs trivial view");
    if (pointer == NULL) return;
    Allocation &allocation = Find(pointer);
    CHECK(allocation.live && allocation.cleaned);
    CHECK(!criticalHeld);
    Record(Release, allocation.id);
    ++released[allocation.id];
    allocation.live = false;
    allocation.pointer = NULL;
    ::operator delete(pointer);
}
// Recover the genuine host allocation type without invoking a GUI destructor.
void HostFree(GuiView *pointer) { HostFree(reinterpret_cast<GuiAllocationToken *>(pointer)); }
void Finish() {
    finalizing = true;
    for (int i = 0; i < allocationCount; ++i)
        if (allocations[i].live) {
            allocations[i].dispose(allocations[i].pointer);
            allocations[i].live = false;
        }
    finalizing = false;
}
void AddPair(Id id) { expected.push_back(Event{Cleanup, id}); expected.push_back(Event{Release, id}); }
void Reset(const Scenario &scenario);
void Run(const Scenario &scenario);
}

// Definitions below are host-only observing mocks for genuine native boundaries.
ReplayManager::~ReplayManager() { fixture::Clean(this); }
StageScriptView::~StageScriptView() { fixture::Clean(this); }
Player::~Player() { fixture::Clean(this); }
ItemManagerView::~ItemManagerView() { fixture::Clean(this); }
EnemyManagerView::~EnemyManagerView() { fixture::Clean(this); }
@BOUNDARY_DEFINITIONS@

void EnterCriticalSection(CRITICAL_SECTION *section) {
    using namespace fixture;
    CHECK(section == &g_MainSupervisorView.criticalSections[0]);
    CHECK(!criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == initialDepth);
    CHECK(g_GameManager == activeManager);
    criticalHeld = true;
    chainBeforeEnter = th10::g_Chain;
    Record(Enter);
    if (switchChains) {
        th10::g_Chain = &chains[enterCount == 0 ? 1 : 4];
        if (enterCount == 0) activeManager->updateChain = &managerElements[2];
    }
    ++enterCount;
}
void LeaveCriticalSection(CRITICAL_SECTION *section) {
    using namespace fixture;
    CHECK(section == &g_MainSupervisorView.criticalSections[0]);
    CHECK(criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == static_cast<unsigned char>(initialDepth + 1));
    CHECK(g_GameManager == activeManager);
    Record(Leave);
    criticalHeld = false;
}
void th10::Chain::UnregisterElem(th10::ChainElem *element, th10::Chain *chain) {
    using namespace fixture;
    CHECK(criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == static_cast<unsigned char>(initialDepth + 1));
    CHECK(chain == chainBeforeEnter);
    CHECK(element == reinterpret_cast<th10::ChainElem *>(elementBeforeEnter));
    CHECK(g_GameManager == activeManager);
    Record(Unregister);
    if (switchChains && unregisterCount == 0) {
        th10::g_Chain = &chains[3];
        activeManager->drawChain = &managerElements[3];
        elementBeforeEnter = &managerElements[3];
    } else elementBeforeEnter = activeManager->drawChain;
    ++unregisterCount;
}

@ACTUAL_OWNER_SOURCE@

namespace fixture {
@FIXTURE_SCENARIOS@
}

int main() {
    using namespace fixture;
    Scenario s;
    Run(s);
    for (int mode : {0, 1, 2}) { s = Scenario{}; s.hintMode = mode; Run(s); }
    for (int missing = Replay; missing <= Owner6F4; ++missing) { s = Scenario{}; s.missing = missing; Run(s); }
    for (int transition : {10, 11, 4, 15, 13, 14, 91}) {
        for (bool equal : {false, true}) {
            s = Scenario{}; s.transition = transition; s.equal = equal; Run(s);
        }
    }
    s = Scenario{}; s.transition = 10; s.existingVm = true; Run(s);
    s = Scenario{}; s.transition = 10; s.existingVm = true; s.equal = false; Run(s);
    for (int mask = 0; mask < 4; ++mask) { s = Scenario{}; s.chainMask = mask; Run(s); }
    s = Scenario{}; s.mutateChains = true; Run(s);
    s = Scenario{}; s.mutateChains = true; s.depth = 255; Run(s);
    s = Scenario{}; s.flags |= 0x20; Run(s);
    s = Scenario{}; s.flags |= 8; Run(s);
    s = Scenario{}; s.options = 0x10; Run(s);
    s = Scenario{}; s.effect = StatsChangesFlags; Run(s);
    s = Scenario{}; s.transition = 10; s.effect = VmChangesTransition; Run(s);
    s = Scenario{}; s.transition = 10; s.effect = VmChangesAscii; Run(s);
    s = Scenario{}; s.transition = 10; s.effect = VmChangesFlags; Run(s);
    for (int transition : {11, 13, 4}) { s = Scenario{}; s.transition = transition; s.effect = SelectionChangesFlags; Run(s); }
    s = Scenario{}; s.transition = 4; s.effect = SelectionChangesTransition; Run(s);
    s = Scenario{}; s.transition = 13; s.effect = SelectionChangesTransition; Run(s);
    s = Scenario{}; s.transition = 15; s.effect = SelectionChangesTransition; Run(s);
    s = Scenario{}; s.transition = 13; s.effect = SelectionChangesCompare; Run(s);
    s = Scenario{}; s.effect = PrimaryClearsSecondary; Run(s);
    s = Scenario{}; s.transition = 11; s.effect = GuiChangesFlags; Run(s);
    s = Scenario{}; s.effect = FinalCleanupChangesFlags; Run(s);
    s = Scenario{}; s.effect = SoundChangesFlags; Run(s);
    // Reuse retains Hint in every setting, and accepts optional null callback nodes.
    for (int mode : {0, 1, 2}) { s = Scenario{}; s.transition = 11; s.hintMode = mode; Run(s); }
    s = Scenario{}; s.transition = 11; s.missing = Secondary; Run(s);
    for (unsigned int mask : {0u, 1u, 2u, 4u, 8u, 16u, 32u}) { s = Scenario{}; s.transition = 11; s.callbackMask = mask; Run(s); }
    std::printf("PASS game teardown caller contract: %d defined-domain cases\n", cases);
}
