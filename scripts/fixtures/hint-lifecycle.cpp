// Deterministic host contracts for extracted, actual owner lifecycle source.
// Dependency seams observe live objects before real deletion. No repeated cleanup,
// destroyed-owner reads, allocation failures, or null-dereference paths are used.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <cstdint>
#include <type_traits>
#define __stdcall
#define __fastcall
#define __declspec(value)
#include "AnmVmId.hpp"

#define CHECK(expression) do { if (!(expression)) { \
    std::fprintf(stderr, "HINT_LIFECYCLE_CHECK_FAILED line %d: %s\n", __LINE__, #expression); \
    std::exit(1); } } while (0)

@ANM_FLOAT3@
@ANM_LAYER_NODE@
struct AnmVmView {
    int id;
    unsigned int flags35C;
    AnmVmLayerNodeView layerNode;
};
struct AnmRenderManagerView {
    AnmVmLayerNodeView *primaryVmListHead;
    AnmVmLayerNodeView *secondaryVmListHead;
    AnmVmView *FindVm(AnmVmIdView id);
    void MarkVmForDeletion(AnmVmIdView id);
    void MarkVmForDeletionActual(AnmVmIdView id);
};
struct CRITICAL_SECTION { int tag; };
struct MainSupervisorView {
    signed char hintMode;
    CRITICAL_SECTION criticalSections[7];
    unsigned char criticalSectionLockCounts[7];
};
MainSupervisorView g_MainSupervisorView;
AnmRenderManagerView *g_AnmRenderManagerView;
#include "Hint.hpp"
HintOwnerView *g_GameOwner477814;
namespace th10 { Chain *g_Chain; }
void EnterCriticalSection(CRITICAL_SECTION *section);
void LeaveCriticalSection(CRITICAL_SECTION *section);
void ClearHintListsActual(HintOwnerView *owner);

namespace fixture {
enum Kind { Allocate, AddCalc, AddDraw, Load, Enter, Unregister, Leave, Mark,
            Export, Clear, DeleteEntry, DeleteOwner, Update };
struct Event {
    Kind kind;
    const void *pointer;
    const void *receiver;
    th10::ChainCallback callback;
    int value;
    int depth;
};
Event events[256];
int eventCount;
th10::Chain chains[3];
th10::ChainElem elements[2];
unsigned int allocatedFlags[2] = {0xABCDEFDFu, 0x1234567Fu};
int allocationCount;
HintOwnerView *activeOwner;
HintOwnerView *expectedUpdateOwner;
int updateResult, updateCalls;
bool switchAfterCalc, switchAfterUnregister, switchAfterMark;
bool switchOnEnter, switchManagerOnLastUnregister;
th10::Chain *chainBeforeEnter;
AnmRenderManagerView *expectedManager;
bool criticalHeld, cleanupExpected, clearSeen, forceFactoryFailure, ownerDeleted;
int clearCalls, exportCalls, factoryInitializeCalls, ownerDeleteCalls;
th10::ChainElem *expectedUnregister[2];
int expectedUnregisterCount, unregisterCalls;
int initialIds[10], expectedMarkIds[10], expectedMarkSlots[10], expectedMarkCount, markCalls;
AnmRenderManagerView manager, alternateManager, afterMarkManager;
AnmVmView vms[16];
AnmVmLayerNodeView managerNodes[16];
int vmCount;
struct EntryRecord { HintEntryView *pointer; int stage; bool exported; bool deleted; };
EntryRecord entries[32];
int entryCount, entriesDeleted;
int originalDepth = 3;

Event &Record(Kind kind, int value = 0, const void *pointer = 0, const void *receiver = 0) {
    CHECK(eventCount < 256);
    Event &event = events[eventCount++];
    event = Event{kind, pointer, receiver, 0, value, g_MainSupervisorView.criticalSectionLockCounts[0]};
    return event;
}
int Count(Kind kind) {
    int result = 0;
    for (int i = 0; i < eventCount; ++i) if (events[i].kind == kind) ++result;
    return result;
}
int Index(Kind kind, int ordinal = 0) {
    for (int i = 0; i < eventCount; ++i)
        if (events[i].kind == kind && ordinal-- == 0) return i;
    CHECK(false && "required event missing");
    return -1;
}
void Reset(int mode = 0) {
    CHECK(!criticalHeld);
    CHECK(entryCount == entriesDeleted);
    eventCount = allocationCount = updateCalls = clearCalls = exportCalls = 0;
    factoryInitializeCalls = ownerDeleteCalls = unregisterCalls = markCalls = 0;
    expectedUnregisterCount = expectedMarkCount = entryCount = entriesDeleted = vmCount = 0;
    switchAfterCalc = switchAfterUnregister = switchAfterMark = false;
    switchOnEnter = switchManagerOnLastUnregister = false; chainBeforeEnter = 0; expectedManager = &manager;
    cleanupExpected = clearSeen = forceFactoryFailure = ownerDeleted = false;
    activeOwner = expectedUpdateOwner = 0;
    std::memset(elements, 0, sizeof(elements));
    std::memset(&g_MainSupervisorView, 0, sizeof(g_MainSupervisorView));
    std::memset(initialIds, 0, sizeof(initialIds));
    std::memset(vms, 0, sizeof(vms));
    std::memset(managerNodes, 0, sizeof(managerNodes));
    manager = AnmRenderManagerView{0, 0}; alternateManager = AnmRenderManagerView{0, 0};
    g_MainSupervisorView.hintMode = static_cast<signed char>(mode);
    g_MainSupervisorView.criticalSectionLockCounts[0] = static_cast<unsigned char>(originalDepth);
    for (int i = 0; i < 7; ++i) g_MainSupervisorView.criticalSections[i].tag = i + 30;
    g_AnmRenderManagerView = &manager; th10::g_Chain = &chains[0]; g_GameOwner477814 = 0;
}
void ExpectCleanup(HintOwnerView *owner) {
    CHECK(!cleanupExpected);
    activeOwner = owner; cleanupExpected = true;
    if (owner->updateChain) expectedUnregister[expectedUnregisterCount++] = owner->updateChain;
    if (owner->drawChain) expectedUnregister[expectedUnregisterCount++] = owner->drawChain;
    for (int i = 0; i < 10; ++i) {
        initialIds[i] = owner->vmIds[i].value;
        if (initialIds[i] != 0) {
            expectedMarkIds[expectedMarkCount] = initialIds[i];
            expectedMarkSlots[expectedMarkCount++] = i;
        }
    }
}
void CheckAfterChains() {
    CHECK(unregisterCalls == expectedUnregisterCount);
    CHECK(!criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == originalDepth);
}
void CheckAllIdsCleared(HintOwnerView *owner) {
    CHECK(markCalls == expectedMarkCount);
    for (int i = 0; i < 10; ++i) CHECK(owner->vmIds[i].value == 0);
}
void DeleteEntryObserved(HintEntryView *entry) {
    CHECK(clearSeen);
    CHECK(g_GameOwner477814 == activeOwner);
    CHECK(entriesDeleted < entryCount);
    EntryRecord &record = entries[entriesDeleted];
    CHECK(!record.deleted);
    CHECK(entry == record.pointer);
    CHECK(entry->link.payload == entry);
    Record(DeleteEntry, entriesDeleted, entry);
    record.deleted = true; record.pointer = 0; ++entriesDeleted;
    delete entry;
}
void DeleteOwnerObserved(HintOwnerView *owner) {
    CHECK(!ownerDeleted);
    CHECK(owner == activeOwner);
    CHECK(clearCalls == 1);
    CHECK(g_GameOwner477814 == 0);
    CHECK(entriesDeleted == entryCount);
    Record(DeleteOwner);
    ++ownerDeleteCalls; ownerDeleted = true; activeOwner = 0;
    delete owner;
}
// Only the factory call boundary is substituted. This calls the real source
// Initialize, then injects a dependency result so its otherwise-unreachable
// failure branch can execute. It is not evidence that native Initialize fails.
int InitializeForFactory(HintOwnerView *owner) {
    ++factoryInitializeCalls;
    const int actual = owner->Initialize();
    CHECK(actual == 0);
    if (forceFactoryFailure) {
        ExpectCleanup(owner);
        return -19;
    }
    return actual;
}
void AddEntries(HintOwnerView *owner) {
    // Allocation order matches actual per-stage loaded-then-export traversal.
    for (int stage = 0; stage < 8; ++stage) for (int group = 0; group < 2; ++group) {
        HintLinkView *tail = group ? &owner->exportStageLists[stage] : &owner->loadedStageLists[stage];
        const int count = stage == 3 ? 2 : 1;
        for (int i = 0; i < count; ++i) {
            CHECK(entryCount < 32);
            HintEntryView *entry = new HintEntryView;
            entry->remaining = 10 + entryCount;
            entries[entryCount++] = EntryRecord{entry, stage, group != 0, false};
            tail->next = &entry->link; entry->link.previous = tail; tail = &entry->link;
        }
    }
}
AnmVmView &AddVm(int id, bool secondary) {
    CHECK(vmCount < 16);
    const int index = vmCount++;
    AnmVmView &vm = vms[index]; vm.id = id; vm.flags35C = 0x80000121u + index;
    vm.layerNode.owner = &vm;
    AnmVmLayerNodeView &node = managerNodes[index]; node.owner = &vm;
    AnmVmLayerNodeView **head = secondary ? &manager.secondaryVmListHead : &manager.primaryVmListHead;
    if (*head) {
        AnmVmLayerNodeView *tail = *head;
        while (tail->next) tail = tail->next;
        tail->next = &node; node.previous = tail;
    } else *head = &node;
    return vm;
}
void LinkChild(AnmVmView &parentOrSibling, AnmVmView &child) {
    parentOrSibling.layerNode.next = &child.layerNode;
    child.layerNode.previous = &parentOrSibling.layerNode;
}
}

th10::ChainElem * __stdcall th10::Chain::AllocElem(th10::ChainCallback callback) {
    using namespace fixture;
    CHECK(allocationCount < 2);
    if (!activeOwner) activeOwner = g_GameOwner477814;
    CHECK(activeOwner != 0 && g_GameOwner477814 == activeOwner);
    CHECK(callback == (allocationCount == 0 ? HintOwnerView::OnUpdate : HintOwnerView::OnDraw));
    if (allocationCount == 1) CHECK(activeOwner->updateChain == &elements[0]);
    th10::ChainElem *element = &elements[allocationCount];
    element->callback = callback; element->flags = allocatedFlags[allocationCount];
    element->arg = 0;
    Record(Allocate, allocationCount, element).callback = callback;
    ++allocationCount;
    return element;
}
int __stdcall th10::Chain::AddToCalcChain(ChainElem *element, int priority, Chain *chain) {
    using namespace fixture;
    CHECK(element == &elements[0]);
    CHECK(element->arg == activeOwner);
    CHECK(element->flags == (allocatedFlags[0] & ~2u));
    CHECK(priority == 25);
    CHECK(chain == &chains[0]);
    Record(AddCalc, priority, element, chain);
    if (switchAfterCalc) g_Chain = &chains[1];
    return -11; // Registration status is ignored by the actual initializer.
}
int __stdcall th10::Chain::AddToDrawChain(ChainElem *element, int priority, Chain *chain) {
    using namespace fixture;
    CHECK(element == &elements[1]);
    CHECK(element->arg == activeOwner);
    CHECK(element->flags == (allocatedFlags[1] & ~2u));
    CHECK(priority == 44);
    CHECK(chain == (switchAfterCalc ? &chains[1] : &chains[0]));
    CHECK(activeOwner->updateChain == &elements[0]);
    Record(AddDraw, priority, element, chain);
    return -12;
}
void th10::Chain::UnregisterElem(ChainElem *element, Chain *chain) {
    using namespace fixture;
    CHECK(cleanupExpected);
    CHECK(criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == originalDepth + 1);
    CHECK(unregisterCalls < expectedUnregisterCount);
    CHECK(element == expectedUnregister[unregisterCalls]);
    CHECK(chain == chainBeforeEnter);
    CHECK(chain == (switchAfterUnregister && unregisterCalls != 0 ? &chains[2] : &chains[0]));
    CHECK(g_GameOwner477814 == activeOwner);
    Record(Unregister, unregisterCalls, element, chain);
    ++unregisterCalls;
    if (switchAfterUnregister) g_Chain = &chains[2];
    if (switchManagerOnLastUnregister && unregisterCalls == expectedUnregisterCount) {
        alternateManager = manager; expectedManager = &alternateManager;
        g_AnmRenderManagerView = &alternateManager;
    }
}
void EnterCriticalSection(CRITICAL_SECTION *section) {
    using namespace fixture;
    CHECK(section == &g_MainSupervisorView.criticalSections[0]);
    CHECK(!criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == originalDepth);
    Record(Enter, 0, section); criticalHeld = true;
    chainBeforeEnter = th10::g_Chain;
    if (switchOnEnter) th10::g_Chain = &chains[1];
}
void LeaveCriticalSection(CRITICAL_SECTION *section) {
    using namespace fixture;
    CHECK(section == &g_MainSupervisorView.criticalSections[0]);
    CHECK(criticalHeld);
    CHECK(g_MainSupervisorView.criticalSectionLockCounts[0] == originalDepth + 1);
    CHECK(Count(Unregister) == Count(Enter));
    Record(Leave, 0, section); criticalHeld = false;
}
int __stdcall LoadHintFile(HintOwnerView *owner, const char *path, int mode) {
    using namespace fixture;
    CHECK(owner == activeOwner && g_GameOwner477814 == owner);
    CHECK(owner->updateChain == &elements[0] && owner->drawChain == &elements[1]);
    CHECK(owner->counter10 == 0);
    const int index = Count(Load);
    CHECK(index < 2);
    CHECK(std::strcmp(path, index == 0 ? "hint/hint_auto.txt" : "hint/hint_user.txt") == 0);
    CHECK(mode == index);
    Record(Load, mode, owner);
    return index == 0 ? -31 : 47; // Both statuses are intentionally ignored.
}
void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id) {
    using namespace fixture;
    CheckAfterChains();
    CHECK(markCalls < expectedMarkCount);
    CHECK(id.value == expectedMarkIds[markCalls]);
    CHECK(this == expectedManager);
    const int slot = expectedMarkSlots[markCalls];
    for (int i = 0; i < slot; ++i) CHECK(activeOwner->vmIds[i].value == 0);
    for (int i = slot; i < 10; ++i) CHECK(activeOwner->vmIds[i].value == initialIds[i]);
    CHECK(g_GameOwner477814 == activeOwner);
    Record(Mark, id.value, activeOwner, this); ++markCalls;
    MarkVmForDeletionActual(id);
    if (switchAfterMark) g_AnmRenderManagerView = &afterMarkManager;
}
int __stdcall ExportAutomaticHints(HintOwnerView *owner, const char *path) {
    using namespace fixture;
    CHECK(owner == activeOwner);
    CheckAfterChains(); CheckAllIdsCleared(owner);
    CHECK(g_MainSupervisorView.hintMode == 2);
    CHECK(g_GameOwner477814 == owner);
    CHECK(!clearSeen && entriesDeleted == 0);
    CHECK(std::strcmp(path, "hint/hint_auto.txt") == 0);
    for (int i = 0; i < entryCount; ++i) {
        CHECK(!entries[i].deleted);
        CHECK(entries[i].pointer->remaining == 10 + i);
    }
    Record(Export, 0, owner); ++exportCalls;
    return -23; // Cleanup does not branch on exporter status.
}
void ClearHintLists(HintOwnerView *owner) {
    using namespace fixture;
    CHECK(cleanupExpected);
    CHECK(owner == activeOwner);
    CHECK(!clearSeen);
    CHECK(g_GameOwner477814 == owner);
    CheckAfterChains(); CheckAllIdsCleared(owner);
    CHECK(exportCalls == (g_MainSupervisorView.hintMode == 2 ? 1 : 0));
    clearSeen = true; ++clearCalls; Record(Clear, 0, owner);
    ClearHintListsActual(owner);
}
int HintOwnerView::Update() {
    using namespace fixture;
    CHECK(this == expectedUpdateOwner);
    Record(fixture::Update, updateResult, this); ++updateCalls;
    return updateResult;
}

#include "hint-support-source.inc"
#include "hint-lifecycle-source.inc"

namespace fixture {
void VerifyCleanup(HintOwnerView &owner, th10::ChainElem *update, th10::ChainElem *draw) {
    CheckAfterChains(); CheckAllIdsCleared(&owner);
    CHECK(clearCalls == 1);
    CHECK(g_GameOwner477814 == 0);
    CHECK(owner.updateChain == update && owner.drawChain == draw);
    CHECK(entriesDeleted == entryCount);
    CHECK(Count(Enter) == expectedUnregisterCount && Count(Leave) == expectedUnregisterCount);
    for (int i = 0; i < expectedUnregisterCount; ++i) {
        CHECK(Index(Enter, i) + 1 == Index(Unregister, i));
        CHECK(Index(Unregister, i) + 1 == Index(Leave, i));
        if (i != 0) CHECK(Index(Leave, i - 1) < Index(Enter, i));
    }
    if (expectedMarkCount) CHECK(Index(Mark) > (expectedUnregisterCount ? Index(Leave, expectedUnregisterCount - 1) : -1));
    if (g_MainSupervisorView.hintMode == 2) CHECK(Index(Export) < Index(Clear));
}
void TestConstructor() {
    Reset();
    alignas(HintOwnerView) unsigned char storage[sizeof(HintOwnerView)];
    std::memset(storage, 0xA5, sizeof(storage));
    HintOwnerView *owner = new (storage) HintOwnerView;
    activeOwner = owner;
    CHECK(g_GameOwner477814 == owner);
    CHECK(owner->flags == 2);
    CHECK(owner->updateChain == 0 && owner->drawChain == 0);
    CHECK(owner->counter10 == 0 && owner->counter14 == 0);
    CHECK(owner->currentBase == 0 && owner->nextVmSlot == 0);
    for (int i = 0; i < 8; ++i) {
        CHECK(owner->loadedStageLists[i].payload == owner);
        CHECK(owner->exportStageLists[i].payload == owner);
        CHECK(owner->loadedStageLists[i].next == 0 && owner->loadedStageLists[i].previous == 0);
        CHECK(owner->exportStageLists[i].next == 0 && owner->exportStageLists[i].previous == 0);
    }
    for (int i = 0; i < 10; ++i) {
        CHECK(owner->vmIds[i].value == 0);
        CHECK(owner->cachedWidths[i] == 0);
        for (int j = 0; j < 3; ++j) CHECK(owner->cachedPositions[i][j] == 0);
    }
    // Check every constructor-zeroed byte, including unknown fields and padding.
    unsigned char expected[sizeof(HintOwnerView)] = {};
    const unsigned int flags = 2;
    std::memcpy(expected + offsetof(HintOwnerView, flags), &flags, sizeof(flags));
    for (int i = 0; i < 8; ++i) {
        std::memcpy(expected + offsetof(HintOwnerView, loadedStageLists) + i * sizeof(HintLinkView), &owner, sizeof(owner));
        std::memcpy(expected + offsetof(HintOwnerView, exportStageLists) + i * sizeof(HintLinkView), &owner, sizeof(owner));
    }
    CHECK(std::memcmp(storage, expected, sizeof(storage)) == 0);
    ExpectCleanup(owner); CleanupHintOwner(owner); VerifyCleanup(*owner, 0, 0);
    owner->~HintOwnerView(); // No post-lifetime read of storage or owner.
}
void TestInitializeAndCallbacks() {
    for (int mode = 0; mode <= 2; ++mode) {
        Reset(mode);
        HintOwnerView owner; activeOwner = &owner;
        owner.counter10 = 71; owner.counter14 = 81;
        switchAfterCalc = true;
        const int status = owner.Initialize();
        CHECK(status == 0);
        CHECK(allocationCount == 2);
        CHECK(owner.updateChain == &elements[0] && owner.drawChain == &elements[1]);
        CHECK(owner.counter10 == 0 && owner.counter14 == 81);
        CHECK(Count(Load) == (mode == 0 ? 0 : 2));
        CHECK(Index(Allocate, 0) < Index(AddCalc) && Index(AddCalc) < Index(Allocate, 1));
        CHECK(Index(Allocate, 1) < Index(AddDraw));
        if (mode != 0) CHECK(Index(AddDraw) < Index(Load, 0) && Index(Load, 0) < Index(Load, 1));
        expectedUpdateOwner = &owner;
        const int results[] = {-17, 0, 73};
        for (int result : results) {
            updateResult = result;
            CHECK(elements[0].callback(elements[0].arg) == result);
        }
        CHECK(updateCalls == 3);
        CHECK(elements[1].callback(elements[1].arg) == 1);
        CHECK(HintOwnerView::OnDraw(0) == 1);
        CHECK(updateCalls == 3);
        th10::g_Chain = &chains[0];
        ExpectCleanup(&owner); CleanupHintOwner(&owner);
        VerifyCleanup(owner, &elements[0], &elements[1]);
    }
}
void TestNullableChains() {
    for (int mask = 0; mask < 4; ++mask) {
        Reset();
        HintOwnerView owner; activeOwner = &owner;
        owner.updateChain = mask & 1 ? &elements[0] : 0;
        owner.drawChain = mask & 2 ? &elements[1] : 0;
        th10::ChainElem *update = owner.updateChain, *draw = owner.drawChain;
        switchAfterUnregister = true;
        ExpectCleanup(&owner); CleanupHintOwner(&owner); VerifyCleanup(owner, update, draw);
    }
}
void TestVmTreesAndEntries() {
    for (int mode = 0; mode <= 2; ++mode) {
        Reset(mode);
        HintOwnerView owner; activeOwner = &owner;
        owner.updateChain = &elements[0]; owner.drawChain = &elements[1];
        AddEntries(&owner);
        AnmVmView &zero = AddVm(0, false);
        AnmVmView &root = AddVm(101, false);
        AnmVmView &child1 = AddVm(111, true);
        AnmVmView &child2 = AddVm(112, false);
        LinkChild(root, child1); LinkChild(child1, child2);
        AnmVmView &secondRoot = AddVm(201, true);
        AnmVmView &secondChild = AddVm(202, true);
        AnmVmView &secondSibling = AddVm(203, false);
        LinkChild(secondRoot, secondChild); LinkChild(secondChild, secondSibling);
        AddVm(303, false); AddVm(404, true);
        AnmVmView &lastRoot = AddVm(505, true);
        AnmVmView &lastChild = AddVm(506, false); LinkChild(lastRoot, lastChild);
        AnmVmView &duplicate = AddVm(101, true);
        CHECK(manager.FindVm(AnmVmIdView(0)) == 0);
        CHECK(manager.FindVm(AnmVmIdView(999)) == 0);
        CHECK(manager.FindVm(AnmVmIdView(101)) == &root);
        CHECK(manager.FindVm(AnmVmIdView(202)) == &secondChild);
        const int ids[] = {0, 101, 999, 202, 0, 303, 404, 0, -17, 505};
        for (int i = 0; i < 10; ++i) owner.vmIds[i].value = ids[i];
        switchAfterUnregister = switchAfterMark = true;
        switchOnEnter = switchManagerOnLastUnregister = true;
        ExpectCleanup(&owner); CleanupHintOwner(&owner);
        VerifyCleanup(owner, &elements[0], &elements[1]);
        CHECK(markCalls == 7);
        const bool deleted[] = {false, true, true, true, false, true, false, true, true, true, true, false};
        CHECK(vmCount == 12);
        for (int i = 0; i < vmCount; ++i) {
            const unsigned int original = 0x80000121u + i;
            CHECK(vms[i].flags35C == (original | (deleted[i] ? 0x04000000u : 0u)));
        }
        CHECK((zero.flags35C & 0x04000000u) == 0);
        CHECK((duplicate.flags35C & 0x04000000u) == 0);
        CHECK((secondSibling.flags35C & 0x04000000u) == 0);
        CHECK(exportCalls == (mode == 2 ? 1 : 0));
        CHECK(entryCount == 18 && entriesDeleted == 18);
    }
}
void TestFactory() {
    for (int fail = 0; fail < 2; ++fail) {
        Reset(2); forceFactoryFailure = fail != 0;
        HintOwnerView *result = CreateHintOwner();
        CHECK(factoryInitializeCalls == 1);
        if (fail) {
            CHECK(result == 0);
            CHECK(ownerDeleted && ownerDeleteCalls == 1);
            CHECK(clearCalls == 1 && exportCalls == 1);
            CHECK(g_GameOwner477814 == 0);
            CHECK(Index(Clear) < Index(DeleteOwner));
            // Deleted owner has no retained dereferenceable pointer here.
        } else {
            CHECK(result != 0 && result == activeOwner && g_GameOwner477814 == result);
            CHECK(!ownerDeleted && ownerDeleteCalls == 0 && clearCalls == 0);
            ExpectCleanup(result); CleanupHintOwner(result);
            VerifyCleanup(*result, &elements[0], &elements[1]);
            DeleteOwnerObserved(result);
        }
    }
}
}
int main() {
    fixture::TestConstructor();
    fixture::TestInitializeAndCallbacks();
    fixture::TestNullableChains();
    fixture::TestVmTreesAndEntries();
    fixture::TestFactory();
    std::puts("HINT_LIFECYCLE_OK");
    return 0;
}
