// Actual-source host contract fixture. The runner includes the lexical tail of
// Hint.cpp and genuine entry/owner constructors, replacing only the delete
// dependency with a live-observation seam; Hint.hpp, Chain.hpp, AnmVmId.hpp
// and the shared effect view are included unchanged.
// The large ANM and Player dependencies are instrumented declaration seams, not
// engine emulation or native ABI/layout proof; their called signatures are audited
// against canonical headers by the runner. Draw methods never expand a format.
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <new>
#include <cstdint>
#include <type_traits>
#define __stdcall
#define __fastcall
#define __declspec(x)
#include "AnmVmId.hpp"

#define CHECK(expression) do { if (!(expression)) { \
    std::fprintf(stderr, "HINT_FRAME_CHECK_FAILED line %d: %s\n", __LINE__, #expression); \
    std::exit(1); } } while (0)

struct AnmFloat2View { float x, y; };
@ANM_FLOAT3@
union AnmColorView {
    unsigned int value;
    struct { unsigned char blue, green, red, alpha; };
};
struct AnmVmAlphaInterpolationView { int duration; };
struct AnmVmView {
    float scaleX, scaleY, spriteWidth, spriteHeight;
    AnmVmAlphaInterpolationView primaryAlphaInterpolation;
    AnmColorView primaryColor;
    int intVar0, intVar1;
    AnmFloat3View position, positionOffset;
    unsigned int flags35C;
    unsigned char glyphWidth, glyphHeight;
    void StartScaleInterpolation(const AnmFloat2View *initial,
        const AnmFloat2View *final, int duration, unsigned char mode);
};
struct AnmLoadedView {
    AnmVmIdView CreateVmAtWorldVariant0(int scriptIndex, const AnmFloat3View *position);
};
struct AnmRenderManagerView {
    AnmVmView *FindVm(AnmVmIdView id);
    void MarkVmForDeletion(AnmVmIdView id);
    void DrawTextLeft(AnmVmView *vm, unsigned int color, const char *format, ...);
    void DrawTextRight(AnmVmView *vm, unsigned int color, const char *format, ...);
    void DrawTextCentered(AnmVmView *vm, unsigned int color, const char *format, ...);
};
struct PlayerFloat3 { float x, y, z; };
struct Player { PlayerFloat3 drawPosition; };
#include "Hint.hpp"
// Canonical global storage supplied by the host harness, as at the game link boundary.
HintOwnerView *g_GameOwner477814;
#include "GameEffectUpdate.hpp"
static_assert(std::is_trivially_copyable<HintEntryView>::value, "entry snapshot requires trivial copying");
AnmRenderManagerView *g_AnmRenderManagerView;
Player *g_Player;
GameEffectUpdateView *g_GameEffectUpdate;
int g_GameStartupMode, g_GameFrameRestorePending;

namespace fixture {
enum Kind { Mark, Create, Lookup, Scale, TextLeft, TextRight, TextCenter, Delete };
struct Event {
    Kind kind;
    int value;
    std::uintptr_t pointer;
    const void *receiver;
    const void *target;
    AnmFloat3View position;
    AnmFloat2View initial, final;
    int duration, mode;
};
Event events[1024];
int eventCount;
struct EntryRecord {
    HintEntryView *pointer;
    HintLinkView *link;
    HintEntryView snapshot;
    bool deleted;
};
EntryRecord records[128];
int recordCount;
struct VmRecord { int id; AnmVmView vm; };
VmRecord vms[128];
int vmCount, nextId;
HintOwnerView *owner;
AnmRenderManagerView manager, alternateManagers[5];
AnmLoadedView loaded;
GameEffectUpdateView effect;
Player player;
unsigned int initialFlags;
AnmFloat3View createdOffset;
bool useCreatedOffset;
int mustBeClearedSlot;
bool allowDelete;
int afterCreateSlot, afterScaleSlot, afterTextSlot;
AnmRenderManagerView *afterCreateManager, *afterScaleManager, *afterTextManager, *afterDeleteManager;
bool changeScaleOnCreate;
float scaleAfterCreate;

Event &Record(Kind kind, int value = 0, const void *pointer = 0, const void *receiver = 0) {
    CHECK(eventCount < 1024);
    Event &event = events[eventCount++];
    std::memset(&event, 0, sizeof(event));
    event.kind = kind; event.value = value; event.pointer = reinterpret_cast<std::uintptr_t>(pointer); event.receiver = receiver;
    return event;
}
int Count(Kind kind) {
    int result = 0;
    for (int i = 0; i < eventCount; ++i) if (events[i].kind == kind) ++result;
    return result;
}
Event &Nth(Kind kind, int n = 0) {
    for (int i = 0; i < eventCount; ++i) if (events[i].kind == kind && n-- == 0) return events[i];
    CHECK(false && "required event missing");
    return events[0];
}
int Index(Kind kind, int n = 0) {
    for (int i = 0; i < eventCount; ++i) if (events[i].kind == kind && n-- == 0) return i;
    CHECK(false && "required event index missing");
    return -1;
}
AnmVmView &AddVm(int id) {
    CHECK(vmCount < 128);
    VmRecord &record = vms[vmCount++];
    std::memset(&record, 0, sizeof(record));
    record.id = id;
    record.vm.scaleX = record.vm.scaleY = 1;
    record.vm.spriteWidth = record.vm.spriteHeight = 20;
    record.vm.primaryColor.alpha = 171;
    record.vm.intVar1 = 255;
    record.vm.flags35C = initialFlags;
    return record.vm;
}
AnmVmView *Vm(int id) {
    for (int i = 0; i < vmCount; ++i) if (vms[i].id == id) return &vms[i].vm;
    return 0;
}
bool EqualFloat(float a, float b) {
    if (std::isnan(a) || std::isnan(b)) return std::isnan(a) && std::isnan(b);
    return a == b && (a != 0 || std::signbit(a) == std::signbit(b));
}
void Reset(HintOwnerView &value) {
    // Deleted objects are represented only by a live snapshot/record index.
    // Release entries retained by freeEntries==0 using matching ordinary delete.
    for (int i = 0; i < recordCount; ++i) if (!records[i].deleted) delete records[i].pointer;
    recordCount = vmCount = eventCount = 0;
    value.~HintOwnerView();
    new (&value) HintOwnerView;
    owner = &value;
    CHECK(g_GameOwner477814 == &value);
    for (int stage = 0; stage < 8; ++stage) {
        CHECK(value.loadedStageLists[stage].payload == &value);
        CHECK(value.exportStageLists[stage].payload == &value);
    }
    g_AnmRenderManagerView = &manager; g_Player = &player; g_GameEffectUpdate = &effect;
    effect.loadedResource = &loaded;
    player.drawPosition = PlayerFloat3{0, 0, 0};
    g_GameStartupMode = 0; g_GameFrameRestorePending = 7;
    nextId = 100; initialFlags = 0xDEADBEEF;
    useCreatedOffset = false; mustBeClearedSlot = -1; allowDelete = false;
    afterCreateSlot = afterScaleSlot = afterTextSlot = -9999;
    afterCreateManager = afterScaleManager = afterTextManager = afterDeleteManager = 0;
    changeScaleOnCreate = false; scaleAfterCreate = 1;
    for (int i = 0; i < 10; ++i) {
        value.cachedPositions[i][0] = -1000 - i;
        value.cachedPositions[i][1] = -2000 - i;
        value.cachedPositions[i][2] = -3000 - i;
        value.cachedWidths[i] = -4000 - i;
    }
}
HintEntryView *Entry(int count = 0, int base = 0) {
    CHECK(recordCount < 128);
    HintEntryView *entry = new HintEntryView;
    EntryRecord &record = records[recordCount++];
    record.pointer = entry; record.link = &entry->link; record.deleted = false;
    entry->link.payload = entry;
    entry->count = count; entry->base = base;
    entry->position.x = 3; entry->position.y = 5; entry->position.z = 7;
    entry->scale = 1; entry->textWidth = 40;
    entry->time = 120; entry->remaining = 29;
    entry->blue = 11; entry->green = 22; entry->red = 33; entry->alpha = 255;
    std::strcpy(entry->text, "literal %s %n %%");
    return entry;
}
EntryRecord &RecordFor(const HintEntryView *entry) {
    for (int i = 0; i < recordCount; ++i) if (records[i].pointer == entry) return records[i];
    CHECK(false && "entry not registered");
    return records[0];
}
void Append(HintLinkView &head, HintEntryView *entry) {
    HintLinkView *tail = &head;
    while (tail->next) tail = tail->next;
    tail->next = &entry->link; entry->link.previous = tail;
}
// The runner substitutes this call for exactly one delete dependency after
// applying each mutation. Observe live fields, save a snapshot, then actually
// delete using the ordinary matching allocator. Never inspect ended lifetimes.
void DeleteEntryObserved(HintEntryView *entry) {
    for (int i = 0; i < recordCount; ++i) if (records[i].pointer == entry) {
        EntryRecord &record = records[i];
        CHECK(allowDelete);
        CHECK(!record.deleted);
        CHECK(entry->link.next == 0);
        CHECK(entry->link.previous == 0);
        for (int stage = 0; stage < 8; ++stage) {
            for (HintLinkView *node = owner->loadedStageLists[stage].next; node; node = node->next)
                CHECK(node != record.link);
        }
        record.snapshot = *entry;
        record.deleted = true;
        record.pointer = 0;
        record.link = 0;
        Record(Delete, i);
        delete entry;
        if (afterDeleteManager) g_AnmRenderManagerView = afterDeleteManager;
        return;
    }
    CHECK(false && "deletion target not registered");
}
}

AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(int scriptIndex, const AnmFloat3View *position) {
    using namespace fixture;
    CHECK(this == &loaded);
    if (mustBeClearedSlot >= 0) CHECK(owner->vmIds[mustBeClearedSlot].value == 0);
    Event &event = Record(Create, scriptIndex, position, this); event.position = *position;
    AnmVmView &vm = AddVm(nextId);
    vm.position = AnmFloat3View{-900, -800, -700};
    vm.positionOffset = useCreatedOffset ? createdOffset : AnmFloat3View{position->x + 224, position->y + 16, position->z};
    if (afterCreateSlot != -9999) owner->nextVmSlot = afterCreateSlot;
    if (afterCreateManager) g_AnmRenderManagerView = afterCreateManager;
    if (changeScaleOnCreate) {
        bool found = false;
        for (int i = 0; i < recordCount; ++i) if (!records[i].deleted && &records[i].pointer->position == position) {
            records[i].pointer->scale = scaleAfterCreate;
            found = true;
        }
        CHECK(found); // The creator receives the real entry position address.
    }
    return AnmVmIdView(nextId++);
}
AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id) {
    fixture::Record(fixture::Lookup, id.value, 0, this);
    return fixture::Vm(id.value);
}
void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id) {
    fixture::Record(fixture::Mark, id.value, 0, this);
}
void AnmVmView::StartScaleInterpolation(const AnmFloat2View *initial,
    const AnmFloat2View *final, int duration, unsigned char mode) {
    fixture::Event &event = fixture::Record(fixture::Scale, 0, this);
    event.initial = *initial; event.final = *final; event.duration = duration; event.mode = mode;
    if (fixture::afterScaleSlot != -9999) fixture::owner->nextVmSlot = fixture::afterScaleSlot;
    if (fixture::afterScaleManager) g_AnmRenderManagerView = fixture::afterScaleManager;
}
void AnmRenderManagerView::DrawTextLeft(AnmVmView *vm, unsigned int color, const char *format, ...) {
    fixture::Event &event = fixture::Record(fixture::TextLeft, static_cast<int>(color), format, this);
    event.target = vm;
    if (fixture::afterTextSlot != -9999) fixture::owner->nextVmSlot = fixture::afterTextSlot;
    if (fixture::afterTextManager) g_AnmRenderManagerView = fixture::afterTextManager;
}
void AnmRenderManagerView::DrawTextRight(AnmVmView *vm, unsigned int color, const char *format, ...) {
    fixture::Event &event = fixture::Record(fixture::TextRight, static_cast<int>(color), format, this);
    event.target = vm;
    if (fixture::afterTextSlot != -9999) fixture::owner->nextVmSlot = fixture::afterTextSlot;
    if (fixture::afterTextManager) g_AnmRenderManagerView = fixture::afterTextManager;
}
void AnmRenderManagerView::DrawTextCentered(AnmVmView *vm, unsigned int color, const char *format, ...) {
    fixture::Event &event = fixture::Record(fixture::TextCenter, static_cast<int>(color), format, this);
    event.target = vm;
    if (fixture::afterTextSlot != -9999) fixture::owner->nextVmSlot = fixture::afterTextSlot;
    if (fixture::afterTextManager) g_AnmRenderManagerView = fixture::afterTextManager;
}

#include "hint-frame-source.inc"

using namespace fixture;

static void ConsumeEligibilityAndLinks() {
    HintOwnerView owner;
    Reset(owner);
    CHECK(owner.ConsumeEntries(0, 1) == 0);
    CHECK(eventCount == 0);
    // Held, base mismatch, due via decrement, mismatch already due, and signed
    // negative counts; a retained middle node exercises both neighbor patches.
    HintEntryView *held = Entry(2, 0), *mismatch = Entry(1, 99);
    HintEntryView *due = Entry(1, 7), *already = Entry(0, 99), *negative = Entry(-3, 99);
    HintLinkView &head = owner.loadedStageLists[2];
    Append(head, due); Append(head, held); Append(head, already); Append(head, mismatch); Append(head, negative);
    CHECK(owner.ConsumeEntries(head.next, 0) == 0);
    CHECK(due->count == 0); CHECK(held->count == 1); CHECK(mismatch->count == 1);
    CHECK(already->count == 0); CHECK(negative->count == -3);
    CHECK(Count(Create) == 3); CHECK(Count(Delete) == 0);
    CHECK(Count(Mark) == 0);
    CHECK(head.next == &held->link); CHECK(held->link.previous == &head);
    CHECK(held->link.next == &mismatch->link); CHECK(mismatch->link.previous == &held->link);
    CHECK(mismatch->link.next == 0);
    CHECK(due->link.next == 0 && due->link.previous == 0);
    CHECK(already->link.next == 0 && already->link.previous == 0);
    CHECK(negative->link.next == 0 && negative->link.previous == 0);
    CHECK(due->remaining == 29 && already->remaining == 29 && negative->remaining == 29);
    CHECK(owner.nextVmSlot == 3);
    EntryRecord &heldRecord = RecordFor(held);
    allowDelete = true;
    CHECK(owner.ConsumeEntries(head.next, 1) == 0);
    CHECK(Count(Delete) == 1);
    CHECK(heldRecord.snapshot.count == 0 && heldRecord.snapshot.remaining == 29);
    CHECK(head.next == &mismatch->link && mismatch->link.previous == &head);
    CHECK(heldRecord.deleted && !RecordFor(mismatch).deleted);

    // A nonzero free value other than one, and a detached head without previous.
    Reset(owner);
    HintEntryView *first = Entry(-1, 0), *second = Entry(0, 7);
    first->link.next = &second->link; second->link.previous = &first->link;
    EntryRecord &firstRecord = RecordFor(first), &secondRecord = RecordFor(second);
    allowDelete = true;
    CHECK(owner.ConsumeEntries(&first->link, -7) == 0);
    CHECK(Count(Create) == 2 && Count(Delete) == 2);
    CHECK(firstRecord.snapshot.count == -2);
    CHECK(secondRecord.snapshot.count == -1);
    CHECK(Index(Delete, 0) > Index(Scale, 0));
    CHECK(Index(Delete, 0) < Index(Create, 1));
}

static void ConsumeScalesAndAlignment() {
    const float scales[] = {-1, 0, 0.5f, std::nextafter(1.0f, 0.0f), 1,
        std::nextafter(1.0f, 2.0f), 1.5f, std::nextafter(2.0f, 1.0f), 2,
        std::nextafter(2.0f, 3.0f), 3, std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()};
    HintOwnerView owner;
    for (float scale : scales) for (int alignment = -1; alignment <= 3; ++alignment) {
        Reset(owner);
        owner.nextVmSlot = 9; owner.vmIds[9] = AnmVmIdView(87);
        mustBeClearedSlot = 9;
        HintEntryView *entry = Entry(); entry->scale = scale; entry->alignment = alignment;
        Append(owner.loadedStageLists[0], entry);
        const float expectedY = static_cast<float>(5.0L + static_cast<long double>(scale) * 8.0L);
        useCreatedOffset = true; createdOffset = AnmFloat3View{101, 202, 303};
        CHECK(owner.ConsumeEntries(owner.loadedStageLists[0].next, 0) == 0);
        CHECK(Count(Mark) == 1 && Nth(Mark).value == 87);
        CHECK(Index(Mark) < Index(Create));
        CHECK(Index(Create) < Index(Lookup));
        CHECK(Index(Lookup) < Index(Scale));
        CHECK(Count(Create) == 1 && Count(Scale) == 1);
        CHECK(EqualFloat(Nth(Create).position.y, expectedY));
        CHECK(EqualFloat(entry->position.y, expectedY));
        CHECK(Nth(Create).position.x == 3 && Nth(Create).position.z == 7);
        CHECK(Nth(Create).pointer == reinterpret_cast<std::uintptr_t>(&entry->position));
        CHECK(owner.vmIds[9].value == 100 && owner.nextVmSlot == 0);
        CHECK(entry->remaining == 29);
        const bool small = scale <= 1.0f;
        const bool medium = scale > 1.0f && scale <= 2.0f;
        const int expectedScript = small ? 11 : 21;
        CHECK(Nth(Create).value == expectedScript);
        AnmVmView *vm = Vm(100); CHECK(vm != 0);
        const unsigned char glyph = small ? 15 : medium ? static_cast<unsigned char>(static_cast<int>(scale * 15.0f)) : 30;
        CHECK(vm->glyphWidth == glyph && vm->glyphHeight == glyph);
        const float initial = static_cast<float>(static_cast<long double>(scale) * (small ? 1.5L : 0.75L));
        const float final = small ? scale : static_cast<float>(static_cast<long double>(scale) * 0.5L);
        CHECK(EqualFloat(Nth(Scale).initial.x, initial));
        CHECK(Nth(Scale).initial.y == 0);
        CHECK(EqualFloat(Nth(Scale).final.x, final));
        CHECK(EqualFloat(Nth(Scale).final.y, final));
        CHECK(Nth(Scale).duration == 8 && Nth(Scale).mode == 4);
        CHECK(vm->primaryColor.blue == 11 && vm->primaryColor.green == 22 && vm->primaryColor.red == 33);
        CHECK(vm->primaryColor.alpha == 0);
        CHECK(vm->intVar0 == 120 && vm->intVar1 == 255);
        CHECK(entry->link.next == 0 && entry->link.previous == 0);
        CHECK(owner.loadedStageLists[0].next == 0);
        if (alignment >= 0 && alignment <= 2) {
            const Kind kind = alignment == 0 ? TextCenter : alignment == 1 ? TextLeft : TextRight;
            CHECK(Count(kind) == 1);
            CHECK(Nth(kind).pointer == reinterpret_cast<std::uintptr_t>(entry->text));
            CHECK(Nth(kind).value == 0x00FFFFFF && Nth(kind).target == vm && Nth(kind).receiver == &manager);
            CHECK(owner.cachedPositions[9][0] == (alignment == 0 ? 101 : alignment == 1 ? 121 : 81));
            CHECK(owner.cachedPositions[9][1] == 202 && owner.cachedPositions[9][2] == 303);
            CHECK(owner.cachedWidths[9] == 40);
            const unsigned int flags = alignment == 0 ? initialFlags : alignment == 1 ?
                (initialFlags & ~0x00080000u) | 0x00040000u : (initialFlags & ~0x00040000u) | 0x00080000u;
            CHECK(vm->flags35C == flags);
        } else {
            CHECK(Count(TextLeft) + Count(TextRight) + Count(TextCenter) == 0);
            CHECK(owner.cachedPositions[9][0] == -1009 && owner.cachedPositions[9][1] == -2009 && owner.cachedPositions[9][2] == -3009);
            CHECK(owner.cachedWidths[9] == -4009);
            CHECK(vm->flags35C == initialFlags);
        }
    }
}

static void SetupUpdate(HintOwnerView &owner, float playerX, float playerY, int alpha = 255) {
    Reset(owner);
    g_Player->drawPosition.x = playerX - 224;
    g_Player->drawPosition.y = playerY - 16;
    owner.currentBase = g_GameFrameRestorePending;
    owner.vmIds[0] = AnmVmIdView(61);
    owner.cachedPositions[0][0] = owner.cachedPositions[0][1] = 0;
    owner.cachedWidths[0] = 40;
    AnmVmView &vm = AddVm(61);
    vm.intVar1 = alpha;
}

static void UpdateProximity() {
    struct Case { float x, y; int expected255; };
    const Case cases[] = {
        {0,0,63}, {19,9,63}, {-19,-9,63},
        {20,0,64}, {0,10,64}, {-20,0,64}, {0,-10,64},
        {36,0,159}, {0,26,159}, {-36,0,159}, {0,-26,159},
        {52,0,255}, {0,42,255}, {-52,0,255}, {0,-42,255},
        {20,10,255}, {21,11,255}, {40,40,255}, {100,0,255}, {0,100,255}};
    HintOwnerView owner;
    for (const Case &test : cases) {
        SetupUpdate(owner, test.x, test.y);
        CHECK(owner.Update() == 1);
        CHECK(Vm(61)->primaryColor.alpha == test.expected255);
    }
    const int alphas[] = {0, 1, 2, 3, 254, 255, -1, -3, -255, 256, 511};
    const float xs[] = {0,20,36,52};
    for (int alpha : alphas) for (float x : xs) {
        SetupUpdate(owner, x, 0, alpha);
        int expected;
        if (x < 20) expected = static_cast<unsigned char>(alpha) >> 2;
        else if (x < 52) {
            const int q = (3 * alpha) / 4;
            expected = static_cast<unsigned char>(static_cast<int>(static_cast<long double>(alpha) - (52.0L - x) * q * 0.03125L));
        } else expected = static_cast<unsigned char>(alpha);
        CHECK(owner.Update() == 1);
        CHECK(Vm(61)->primaryColor.alpha == expected);
    }
    // Unordered comparisons fall through to full alpha without unsafe conversion.
    SetupUpdate(owner, 0, 0);
    owner.cachedPositions[0][0] = std::numeric_limits<float>::quiet_NaN();
    CHECK(owner.Update() == 1); CHECK(Vm(61)->primaryColor.alpha == 255);
    SetupUpdate(owner, 0, 0);
    owner.cachedPositions[0][1] = std::numeric_limits<float>::quiet_NaN();
    CHECK(owner.Update() == 1); CHECK(Vm(61)->primaryColor.alpha == 255);
}

static void UpdateStageCountersAndSlots() {
    HintOwnerView owner;
    Reset(owner);
    owner.currentBase = 6; owner.counter10 = 40; owner.counter14 = 90;
    g_GameStartupMode = 3;
    HintEntryView *selected = Entry(1, 7), *wrongStage = Entry(1, 7), *exported = Entry(1, 7);
    Append(owner.loadedStageLists[3], selected);
    Append(owner.loadedStageLists[1], wrongStage);
    Append(owner.exportStageLists[3], exported);
    EntryRecord &selectedRecord = RecordFor(selected);
    allowDelete = true;
    CHECK(owner.Update() == 1);
    CHECK(Count(Create) == 1 && Count(Delete) == 1);
    CHECK(selectedRecord.deleted);
    CHECK(wrongStage->count == 1 && exported->count == 1);
    CHECK(owner.counter10 == 41 && owner.counter14 == 1);
    CHECK(owner.currentBase == 6);
    CHECK(owner.Update() == 1);
    CHECK(owner.counter10 == 42 && owner.counter14 == 1);
    owner.currentBase = g_GameFrameRestorePending;
    CHECK(owner.Update() == 1);
    CHECK(owner.counter10 == 43 && owner.counter14 == 2);

    Reset(owner);
    owner.currentBase = g_GameFrameRestorePending;
    for (int i = 0; i < 10; ++i) {
        owner.vmIds[i] = AnmVmIdView(200 + i);
        owner.cachedPositions[i][0] = 224;
        owner.cachedPositions[i][1] = 16;
        owner.cachedWidths[i] = 40;
        if (i != 3) {
            AnmVmView &vm = AddVm(200 + i);
            vm.intVar1 = 4 * i;
            if (i == 4) vm.primaryAlphaInterpolation.duration = 3;
            if (i == 5) vm.primaryAlphaInterpolation.duration = -1;
        }
    }
    owner.vmIds[8] = AnmVmIdView(0);
    CHECK(owner.Update() == 1);
    CHECK(owner.vmIds[3].value == 0 && owner.vmIds[8].value == 0);
    for (int i = 0; i < 10; ++i) {
        if (i == 3 || i == 8) continue;
        CHECK(owner.vmIds[i].value == 200 + i);
        CHECK(Vm(200 + i)->primaryColor.alpha == ((i == 4 || i == 5) ? 171 : i));
    }
    CHECK(owner.counter10 == 1 && owner.counter14 == 1);
}

// Synthetic callbacks expose the source's audited reload/caching boundaries.
// They establish sensitivity, not that the real engine mutates owner/global state.
static void CallbackReloadContracts() {
    HintOwnerView owner;
    Reset(owner);
    owner.nextVmSlot = 1; owner.vmIds[1] = AnmVmIdView(87); mustBeClearedSlot = 1;
    HintEntryView *entry = Entry();
    Append(owner.loadedStageLists[0], entry);
    afterCreateSlot = 2; afterScaleSlot = 3; afterTextSlot = 4;
    afterCreateManager = &alternateManagers[0];
    CHECK(owner.ConsumeEntries(owner.loadedStageLists[0].next, 0) == 0);
    CHECK(Nth(Create).value == 3);
    CHECK(Nth(Create).pointer == reinterpret_cast<std::uintptr_t>(&entry->position));
    CHECK(owner.vmIds[1].value == 0 && owner.vmIds[2].value == 100);
    CHECK(Nth(Lookup).receiver == &alternateManagers[0]);
    CHECK(owner.cachedWidths[2] == -4002 && owner.cachedWidths[3] == 40);
    CHECK(owner.nextVmSlot == 5);

    Reset(owner);
    entry = Entry(); Append(owner.loadedStageLists[0], entry);
    afterTextSlot = -2; // All indexed owner accesses are already complete.
    CHECK(owner.ConsumeEntries(owner.loadedStageLists[0].next, 0) == 0);
    CHECK(owner.nextVmSlot == -1); // unsigned(-1) %10 would give5

    const float startingScales[] = {1.0f, 1.5f, 3.0f};
    const int expectedGlyphs[] = {15, 45, 30};
    for (int i = 0; i < 3; ++i) {
        Reset(owner);
        entry = Entry(); entry->scale = startingScales[i];
        Append(owner.loadedStageLists[0], entry);
        changeScaleOnCreate = true;
        scaleAfterCreate = i == 1 ? 3.0f : 1.5f;
        afterScaleManager = &alternateManagers[1];
        CHECK(owner.ConsumeEntries(owner.loadedStageLists[0].next, 0) == 0);
        CHECK(Vm(100)->glyphWidth == expectedGlyphs[i]);
        CHECK(Nth(TextCenter).receiver == (i == 1 ? &alternateManagers[1] : &manager));
        CHECK(EqualFloat(Nth(Scale).initial.x, scaleAfterCreate * (i == 0 ? 1.5f : 0.75f)));
    }

    Reset(owner);
    HintEntryView *first = Entry(), *second = Entry();
    Append(owner.loadedStageLists[0], first); Append(owner.loadedStageLists[0], second);
    owner.vmIds[0] = AnmVmIdView(80); owner.vmIds[1] = AnmVmIdView(81);
    afterTextManager = &alternateManagers[2];
    CHECK(owner.ConsumeEntries(owner.loadedStageLists[0].next, 0) == 0);
    CHECK(Nth(Mark, 0).receiver == &manager);
    CHECK(Nth(Mark, 1).receiver == &alternateManagers[2]);

    Reset(owner);
    first = Entry(); second = Entry(); first->alignment = second->alignment = 7;
    Append(owner.loadedStageLists[0], first); Append(owner.loadedStageLists[0], second);
    owner.vmIds[0] = AnmVmIdView(80); owner.vmIds[1] = AnmVmIdView(81);
    allowDelete = true; afterDeleteManager = &alternateManagers[3];
    CHECK(owner.ConsumeEntries(owner.loadedStageLists[0].next, 1) == 0);
    CHECK(Nth(Mark, 0).receiver == &manager);
    CHECK(Nth(Mark, 1).receiver == &alternateManagers[3]);
}

static void ZeroIdGates() {
    HintOwnerView owner;
    Reset(owner);
    CHECK(owner.Update() == 1);
    CHECK(Count(Lookup) == 0); // A removed gate fails here with a valid manager.
    CHECK(owner.counter10 == 1 && owner.counter14 == 1);
    Reset(owner);
    g_AnmRenderManagerView = 0;
    CHECK(owner.Update() == 1); // Native permits empty lists plus ten zero IDs.
    CHECK(Count(Lookup) == 0);
    CHECK(owner.counter10 == 1 && owner.counter14 == 1);
}

static void PrecisionWitnesses();
int main() {
    ConsumeEligibilityAndLinks();
    ConsumeScalesAndAlignment();
    ZeroIdGates();
    UpdateProximity();
    UpdateStageCountersAndSlots();
    CallbackReloadContracts();
    PrecisionWitnesses();
    HintOwnerView cleanup; Reset(cleanup);
    std::puts("HINT_FRAME_OK");
    return 0;
}

// Filled with explicit-store and x87-aware arithmetic witnesses below.
static void PrecisionWitnesses() {
    CHECK(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559);
    // These finite witnesses agree under PC53 and PC64 round-to-nearest in
    // the separately reviewed rational model. They do not attest native CW.
    HintOwnerView owner;
    SetupUpdate(owner, 0, 0);
    g_Player->drawPosition.x = std::ldexp(1.0f, -20);
    g_Player->drawPosition.y = -16;
    owner.cachedPositions[0][0] = 224;
    owner.cachedWidths[0] = std::ldexp(1.0f, -19);
    Vm(61)->spriteHeight = 2;
    CHECK(owner.Update() == 1);
    CHECK(Vm(61)->primaryColor.alpha == 64); // rounded adjusted X would give63

    SetupUpdate(owner, 0, 1);
    owner.cachedWidths[0] = 2;
    Vm(61)->spriteHeight = 2 + std::ldexp(1.0f, -22);
    Vm(61)->scaleY = 1 - std::ldexp(1.0f, -24);
    CHECK(owner.Update() == 1);
    CHECK(Vm(61)->primaryColor.alpha == 63); // rounded h would give64

    SetupUpdate(owner, 0, 0);
    g_Player->drawPosition.y = 1;
    owner.cachedPositions[0][1] = std::ldexp(1.0f, -25);
    owner.cachedWidths[0] = 2;
    Vm(61)->spriteHeight = 34;
    CHECK(owner.Update() == 1);
    CHECK(Vm(61)->primaryColor.alpha == 64); // unrounded dy would give63

    SetupUpdate(owner, 32, 0);
    owner.cachedWidths[0] = std::ldexp(1.0f, -19);
    Vm(61)->spriteHeight = 2;
    CHECK(owner.Update() == 1);
    CHECK(Vm(61)->primaryColor.alpha == 255); // unrounded outerW would give254

}
