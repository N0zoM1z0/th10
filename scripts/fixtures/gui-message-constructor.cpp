// LOCAL HOST POST-CONSTRUCTION SEMANTIC FIXTURE. Complete maintained data
// declarations and helper/member-constructor/typed-caller bodies are retained. Target
// layout assertions are omitted. The driver EXPLICITLY REPLACES THREE timer
// constructors in temporary headers with flags(0) initialization; inherited
// maintained raw-integer default-construction validity is NOT tested or established.
// Actual inline SetCurrent/Add/Tick methods remain unchanged. All out-of-line
// dependency implementations below are HOST SEAMS, not production behavior.
// No real ANM allocation, rendering, laser cancellation, or target code runs.
#define __stdcall
#define __fastcall
#define __cdecl
#define __thiscall
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include <utility>
#include <vector>
#include "Gui.hpp"
#include "Enemy.hpp"
#include "EnemyLaser.hpp"
#include "BulletManager.hpp"

static void Check(bool condition, const char *name)
{
    if (!condition) { std::fprintf(stderr, "CHECK %s\n", name); std::exit(1); }
}

// HOST ALLOCATION OBSERVER: global ordinary allocation remains malloc-backed.
// Only the first message-sized allocation while armed is tracked, before any
// constructor callback runs. This does not provide class-specific allocation,
// placement-construction wrappers, or constructor exception cleanup of its own.
struct MessageAllocationObservation
{
    bool armed = false;
    void *pointer = nullptr;
    unsigned allocations = 0;
    unsigned frees = 0;
    unsigned completeDestructors = 0;
};
static MessageAllocationObservation messageMemory;
void *operator new(std::size_t size)
{
    void *pointer = std::malloc(size == 0 ? 1 : size);
    if (pointer == nullptr) throw std::bad_alloc();
    if (messageMemory.armed && messageMemory.pointer == nullptr && size == sizeof(GuiMessageVmView))
    {
        messageMemory.pointer = pointer;
        ++messageMemory.allocations;
    }
    return pointer;
}
void operator delete(void *pointer) noexcept
{
    if (messageMemory.armed && pointer != nullptr && pointer == messageMemory.pointer)
    {
        ++messageMemory.frees;
        Check(messageMemory.frees == 1, "exception.free_once");
    }
    std::free(pointer);
}
void operator delete(void *pointer, std::size_t) noexcept { ::operator delete(pointer); }

// All host objects use aligned, writable storage and actual C++ lifetimes.
// The three temporary-header timer constructors initialize their flags members;
// every other consumed field is explicitly assigned AFTER construction. Unknown
// unused members are never read. Raw-byte prefilling is not an initialization
// argument and no original flags-read constructor executes.
template<class T> class Live
{
    T *object;
public:
    template<class... Args> explicit Live(Args&&... args)
    {
        void *storage = ::operator new(sizeof(T));
        object = new (storage) T(std::forward<Args>(args)...);
    }
    ~Live() { object->~T(); ::operator delete(object); }
    T *get() const { return object; }
    T *operator->() const { return object; }
    T &operator*() const { return *object; }
    Live(const Live&) = delete;
    Live &operator=(const Live&) = delete;
};

float g_AnmGameSpeed = 1.0f;
AnmRenderManagerView *g_AnmRenderManagerView;
AsciiManagerView *g_AsciiManagerView;
EnemyManagerView *g_EnemyManager;
void *g_EnemyBulletManager;
EnemyLaserManagerView *g_EnemyLaserManager;
int g_EnemyDialogIndex;
int g_EnemyDialogState;
GuiView *g_GuiView;
// Live alternative owner used ONLY by the owner-selection negative control.
EnemyLaserManagerView *g_HostDecoyLaserManager;

// HOST SEAMS: complete types and actual inline timer methods come from
// maintained headers, except the three explicit constructor seams. These
// no-op out-of-line dependencies are not claims
// about production constructor/destructor or ECL behavior.
AnmVmView::AnmVmView() {}
AnmVmView::~AnmVmView() {}
AnmLoadedView::AnmLoadedView() {}
AnmRenderManagerView::AnmRenderManagerView() {}
AnmRenderManagerView::~AnmRenderManagerView() {}
AsciiManagerView::AsciiManagerView() {}
AsciiManagerView::~AsciiManagerView() {}
size_t AsciiManagerView::GetSize() { return sizeof(*this); }
GuiMessageVmView::~GuiMessageVmView()
{
    if (messageMemory.armed && this == messageMemory.pointer)
        ++messageMemory.completeDestructors;
}
GuiView::~GuiView() {}
EnemyManagerView::EnemyManagerView() {}
EnemyManagerView::~EnemyManagerView() {}
BulletManagerView::~BulletManagerView() {} // Host dependency sink; manager cleanup is outside this test.
EnemyEclContextView::EnemyEclContextView() {}
EnemyBulletPatternView::EnemyBulletPatternView() {}
EnemyEclHostBaseView::EnemyEclHostBaseView() {}
EnemyEclHostBaseView::~EnemyEclHostBaseView() {}
EnemyFullObjectView::EnemyFullObjectView(const char *) {}
EnemyFullObjectView::~EnemyFullObjectView() {}
#define HOST_ECL_VIRTUALS(T) \
int T::DispatchEclInstruction() { return 0; } \
int T::ReadIntOperand(int value) { return value; } \
int *T::ResolveIntOperand(int) { return &value1038; } \
float T::ReadFloatOperand(int value) { return static_cast<float>(value); } \
float *T::ResolveFloatOperand(int) { return &g_AnmGameSpeed; }
HOST_ECL_VIRTUALS(EnemyEclHostBaseView)
HOST_ECL_VIRTUALS(EnemyFullObjectView)
#undef HOST_ECL_VIRTUALS
void EnemyEffectWaitNode::UnknownVirtual0() {}
void EnemyEffectWaitNode::UnknownVirtual1() {}
void EnemyEffectWaitNode::UnknownVirtual2() {}
void EnemyEffectWaitNode::UnknownVirtual3() {}
void EnemyEffectWaitNode::UnknownVirtual4() {}
void EnemyEffectWaitNode::UnknownVirtual6() {}
unsigned int EnemyEffectWaitNode::ApplyPatternCancel(const PlayerFloat3 *, float, int) { return 0; }
unsigned int EnemyEffectWaitNode::ApplyClear(int) { Check(false, "laser.virtual_dispatch"); return 0; }

struct InitCall { AnmLoadedView *resource; AnmVmView *vm; int script; unsigned layer, flags; };
struct EffectCall { AnmLoadedView *resource; int script; const void *position; PlayerFloat3 value; };
struct HostContext
{
    bool constructor = false;
    const char *throwEvent = nullptr;
    size_t throwOccurrence = 0;
    GuiView *callerGui = nullptr;
    int originalDialogIndex = 71;
    int originalDialogState = 93;
    AnmRenderManagerView *managers[4] = {};
    AnmLoadedView *text[3] = {};
    AnmVmView *allocations[2] = {};
    AnmVmView *lookups[4] = {};
    BulletManagerView *bullets = nullptr;
    std::vector<AnmRenderManagerView *> allocatedBy, insertedBy, foundBy;
    std::vector<AnmVmView *> insertedVms;
    std::vector<int> lookupIds, bulletIndices, laserNodes, laserModes;
    std::vector<InitCall> initialized;
    std::vector<EffectCall> effects;
    std::vector<std::pair<const void *, EnemyListNodeView *> > effectMutations;
    std::vector<std::string> order;
};
static HostContext *host;

struct HostCallbackException {};
static void MaybeThrow(const char *event, size_t occurrence)
{
    if (host->callerGui != nullptr)
        Check(host->callerGui->message == nullptr &&
              g_EnemyDialogIndex == host->originalDialogIndex &&
              g_EnemyDialogState == host->originalDialogState,
              "caller.no_early_publication");
    if (host->throwEvent != nullptr && std::strcmp(event, host->throwEvent) == 0 &&
        occurrence == host->throwOccurrence)
        throw HostCallbackException();
}

// HOST SEAMS: callbacks change globals but every returned object remains live.
// In particular all four FindVm calls return DISTINCT, NONNULL live VMs.
AnmVmView *AnmRenderManagerView::AllocateVm()
{
    const size_t n = host->allocatedBy.size();
    host->allocatedBy.push_back(this);
    host->order.push_back("allocate");
    g_AnmRenderManagerView = host->managers[n == 0 ? 2 : 0];
    g_AsciiManagerView->textAnm = host->text[2];
    MaybeThrow("allocate", n);
    return host->allocations[n % 2];
}
void AnmLoadedView::InitializeVm(AnmVmView *vm, int script)
{
    const size_t n = host->initialized.size();
    host->initialized.push_back({this, vm, script, vm->renderLayer, vm->flags35C});
    host->order.push_back("initialize");
    g_AnmRenderManagerView = host->managers[n == 0 ? 1 : 3];
    g_AsciiManagerView->textAnm = host->text[1];
    MaybeThrow("initialize", n);
}
AnmVmIdView AnmRenderManagerView::AddVmVariant0(AnmVmView *vm)
{
    const size_t n = host->insertedBy.size();
    host->insertedBy.push_back(this);
    host->insertedVms.push_back(vm);
    host->order.push_back("insert");
    g_AnmRenderManagerView = host->managers[2];
    vm->id = n == 0 ? 101 : 202;
    MaybeThrow("insert", n);
    return AnmVmIdView(n == 0 ? 101 : 202);
}
AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)
{
    const size_t n = host->foundBy.size();
    host->foundBy.push_back(this);
    host->lookupIds.push_back(id.value);
    host->order.push_back("find");
    g_AnmRenderManagerView = host->managers[n % 3];
    MaybeThrow("find", n);
    return host->lookups[n % 4];
}
unsigned int EnemyCancelBulletRecord(unsigned char *record)
{
    BulletRuntimeView *bullet = reinterpret_cast<BulletRuntimeView *>(record);
    const int index = static_cast<int>(bullet - host->bullets->bullets);
    Check(index >= 0 && index <= 2000, "bullet.valid_record");
    host->bulletIndices.push_back(index);
    host->order.push_back("bullet");
    // Explicit boundary seam only: the real cancellation dependency accepts
    // states 1/2. Other nonzero/non-3 states must still be passed to this seam.
    const bool canceled = bullet->state == 1 || bullet->state == 2;
    if (canceled) bullet->state = 3;
    MaybeThrow("bullet", host->bulletIndices.size() - 1);
    return canceled ? 1 : 0;
}
AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(int script, const AnmFloat3View *position)
{
    PlayerFloat3 value;
    // The real helper passes PlayerFloat3 storage through the ANM pointer view.
    // Read bytes, without pretending the two distinct C++ types alias legally.
    std::memcpy(&value, position, sizeof(value));
    host->effects.push_back({this, script, position, value});
    host->order.push_back("effect");
    for (const auto &entry : host->effectMutations)
        if (entry.first == position) entry.second->next = nullptr;
    MaybeThrow("effect", host->effects.size() - 1);
    return AnmVmIdView(303);
}
struct HostLaser : EnemyEffectWaitNode
{
    int label = 0;
    bool changeNext = false;
    unsigned int ApplyClear(int mode) override
    {
        host->laserNodes.push_back(label);
        host->laserModes.push_back(mode);
        host->order.push_back("laser");
        if (changeNext) next = nullptr;
        MaybeThrow("laser", host->laserNodes.size() - 1);
        return static_cast<unsigned int>(label + 10);
    }
};

#include "gui-message-extracted.inc"

// Additional bounded GUI field snippets use the actual host declarations.
// The driver explicitly maps pre-existing 32-bit runtime scalar accesses to
// their maintained fields and wraps switch control flow. These are NOT full
// ECL/callback-owner execution or target layout evidence.
static std::vector<std::string> fieldReads;
static EnemyRuntimeView *fieldEnemy;
static int fieldMode;
static unsigned int HostReadInt(int index)
{
    fieldReads.push_back("int" + std::to_string(index));
    if (fieldMode == 0 && index == 0) { fieldEnemy->unknown13C4 = 999; return 2; }
    if (fieldMode == 0 && index == 2) return 0x11223344u;
    if (fieldMode == 1 && index == 0) return 123;
    if (fieldMode == 2 && index == 0) return 7;
    Check(false, "gui_snippet.arguments");
    return 0;
}
static float HostReadFloat(int index)
{
    fieldReads.push_back("float" + std::to_string(index));
    Check(fieldMode == 0 && index == 1, "gui_snippet.arguments");
    return 100.0f;
}
#include "gui-field-snippets.inc"

static void TestGuiFieldSnippets()
{
    Live<GuiView> gui;
    Live<EnemyFullObjectView> owner("gui-field-snippets");
    g_GuiView = gui.get();
    fieldEnemy = &owner->runtime;
    fieldEnemy->unknown13C4 = 200;
    for (int i = 0; i < 4; ++i) { gui->bossGaugeValues[i].value = -1.0f; gui->bossGaugeValues[i].color = 0x55; }
    fieldMode = 0;
    fieldReads.clear();
    Check(HostMarkerSnippet(fieldEnemy) == 0, "gui_snippet.marker_return");
    Check(fieldReads == std::vector<std::string>({"int2", "float1", "int0"}), "gui_snippet.marker_read_order");
    Check(gui->bossGaugeValues[2].value == 0.5f && gui->bossGaugeValues[2].color == 0x11223344u &&
          fieldEnemy->unknown13C4 == 999, "gui_snippet.marker_snapshot");
    for (int i : {0, 1, 3})
        Check(gui->bossGaugeValues[i].value == -1.0f && gui->bossGaugeValues[i].color == 0x55, "gui_snippet.marker_range");
    Check(host_marker_color_before_value, "gui_snippet.STATIC_color_store_order");
    fieldMode = 1;
    fieldReads.clear();
    fieldEnemy->life = 10;
    fieldEnemy->value13C8 = 456;
    Check(HostClearSnippet(fieldEnemy) == 0 && fieldEnemy->life == 123 &&
          fieldEnemy->unknown13C4 == 123 && fieldEnemy->value13C8 == 123, "gui_snippet.clear_runtime_copy");
    Check(fieldReads == std::vector<std::string>({"int0"}), "gui_snippet.clear_read");
    for (int i = 0; i < 4; ++i)
        Check(gui->bossGaugeValues[i].value == 0.0f && gui->bossGaugeValues[i].color == 0, "gui_snippet.clear_all_pairs");
    fieldMode = 2;
    fieldReads.clear();
    gui->bossGaugeSegmentCount = -1;
    Check(HostSegmentsSnippet() == 0 && gui->bossGaugeSegmentCount == 7 &&
          fieldReads == std::vector<std::string>({"int0"}), "gui_snippet.segment_count");
    const int values[][3] = {{1, 0, 1}, {60, 0, 1}, {61, 0, 2}, {6000, 0, 99},
                            {6001, 0, 99}, {1, 121, -1}, {60, 120, 0}, {60, 181, -1}};
    for (const auto &value : values)
    {
        fieldEnemy->callbackThresholds[0].timerThreshold = value[0];
        fieldEnemy->updateTimer.current = value[1];
        gui->spellSeconds = 777;
        HostCountdownSnippet(fieldEnemy, 0);
        Check(gui->spellSeconds == value[2], "gui_snippet.signed_seconds_max_only");
    }
}

static void CheckAnmTimer(const AnmVmTimerView &timer)
{
    Check(timer.current == 0 && timer.previous == -1 && timer.subframe == 0.0f &&
          timer.flags == 1 && timer.scale == &g_AnmGameSpeed, "message.timers");
}

template<class T> static void CheckActualTimerMethods()
{
    // The fixture constructor starts a valid lifetime and sets flags to zero.
    // Then exercise the ACTUAL maintained SetCurrent method on live members.
    Live<T> storage;
    T *timer = storage.get();
    timer->flags = 0xa5a5a5a4u;
    timer->SetCurrent(7);
    Check(timer->flags == 0xa5a5a5a5u && timer->scale == &g_AnmGameSpeed &&
          timer->current == 7 && timer->previous == 6 && timer->subframe == 7.0f,
          "timer.set_current_initializes");
    float alternate = 0.25f;
    timer->scale = &alternate;
    timer->SetCurrent(-2);
    Check(timer->scale == &alternate && timer->flags == 0xa5a5a5a5u &&
          timer->current == -2 && timer->previous == -3 && timer->subframe == -2.0f,
          "timer.set_current_preserves");
}

static void TestHelperTimers()
{
    HostContext context;
    host = &context;
    Live<EnemyManagerView> manager, global;
    g_EnemyManager = global.get();
    global->timer.SetCurrent(900);
    manager->enemyListHead = nullptr;
    const float scales[] = {
        std::nextafter(0.99f, -INFINITY), 0.99f, std::nextafter(0.99f, INFINITY),
        1.0f, std::nextafter(1.01f, -INFINITY), 1.01f, std::nextafter(1.01f, INFINITY),
        0.0f, 0.5f, -0.5f, -2.0f, -5.0f
    };
    for (float scale : scales)
    {
        manager->timer.SetCurrent(37);
        manager->timer.previous = -111;
        manager->timer.current = 37;
        manager->timer.subframe = 3.25f; // deliberately differs from integer current
        manager->timer.scale = &scale;
        EnemyKillAll(manager.get());
        Check(manager->timer.previous == 37, "timer.previous");
        const bool fast = scale > 0.99f && scale < 1.01f;
        const float subframe = 3.25f + (fast ? 1.0f : scale);
        const int current = fast ? 38 : static_cast<int>(subframe);
        Check(manager->timer.current == current && manager->timer.subframe == subframe,
              "timer.strict_interval");
        Check(manager->timer.flags == 1 && manager->timer.scale == &scale,
              "timer.state_preserved");
        Check(global->timer.current == 900 && global->timer.previous == 899,
              "timer.argument_owner");
    }
    Check(context.effects.empty(), "enemy.empty_list");
}

static void TestHelperEnemies()
{
    HostContext context;
    host = &context;
    Live<EnemyManagerView> manager, global;
    g_EnemyManager = global.get();
    manager->timer.SetCurrent(4);
    global->timer.SetCurrent(99);
    std::vector<std::unique_ptr<Live<AnmLoadedView> > > globalResources, localResources;
    for (int i = 0; i < 4; ++i)
    {
        globalResources.emplace_back(new Live<AnmLoadedView>);
        localResources.emplace_back(new Live<AnmLoadedView>);
        global->effectResources[i] = globalResources.back()->get();
        manager->effectResources[i] = localResources.back()->get();
    }
    const unsigned flags[] = {0, 0x10, 0x40, 0x4000, 0x8000, 0xc050, 0x80,
                              0xc0d0, 0x20000, 0x1, 0x100, 0x80};
    const int count = sizeof(flags) / sizeof(flags[0]);
    std::vector<std::unique_ptr<Live<EnemyFullObjectView> > > enemies;
    std::vector<EnemyListNodeView> nodes(count);
    std::vector<int> expectedEffects;
    for (int i = 0; i < count; ++i)
    {
        enemies.emplace_back(new Live<EnemyFullObjectView>("host-fixture"));
        EnemyRuntimeView &enemy = enemies.back()->get()->runtime;
        enemy.flags = flags[i];
        enemy.deathEffectScript = i == 9 || i == 11 ? -1 : i; // script 0 is valid
        enemy.deathEffectResourceIndex = i % 4;
        enemy.worldMotion.position = {float(i) + .25f, float(i) + .5f, float(i) + .75f};
        nodes[i].enemy = enemies.back()->get();
        nodes[i].next = i + 1 < count ? &nodes[i + 1] : nullptr;
        context.effectMutations.push_back({&enemy.worldMotion.position, &nodes[i]});
        if (((flags[i] & 0xc050u) == 0 || (flags[i] & 0x80u) != 0) && enemy.deathEffectScript >= 0)
            expectedEffects.push_back(i);
    }
    manager->enemyListHead = nodes.data();
    EnemyKillAll(manager.get());
    for (int i = 0; i < count; ++i)
    {
        const bool selected = (flags[i] & 0xc050u) == 0 || (flags[i] & 0x80u) != 0;
        Check(enemies[i]->get()->runtime.flags == (flags[i] | (selected ? 0x20000u : 0)),
              "enemy.flags");
    }
    Check(context.effects.size() == expectedEffects.size(), "enemy.effect_records");
    for (size_t n = 0; n < expectedEffects.size(); ++n)
    {
        const int i = expectedEffects[n];
        const EffectCall &call = context.effects[n];
        Check(call.resource == globalResources[i % 4]->get(), "enemy.global_resources");
        Check(call.script == i && call.position == &enemies[i]->get()->runtime.worldMotion.position &&
              call.value.x == float(i) + .25f && call.value.y == float(i) + .5f &&
              call.value.z == float(i) + .75f, "enemy.effect_arguments");
    }
    Check(manager->timer.current == 5 && manager->timer.previous == 4 &&
          global->timer.current == 99, "enemy.timer_after_list");
}

struct ThrowCase
{
    const char *event;
    size_t occurrence, allocations, initializations, insertions, finds;
    int bullets; // -1 means the complete valid range
    size_t lasers, effects;
};
static void TestConstructor(bool emptyLasers, bool useCaller = false,
                            const ThrowCase *failure = nullptr, bool sameIndex = false)
{
    HostContext context;
    host = &context;
    context.constructor = true;
    if (failure) { context.throwEvent = failure->event; context.throwOccurrence = failure->occurrence; }
    std::vector<std::unique_ptr<Live<AnmRenderManagerView> > > managers;
    std::vector<std::unique_ptr<Live<AnmLoadedView> > > resources;
    std::vector<std::unique_ptr<Live<AnmVmView> > > vms;
    for (int i = 0; i < 4; ++i) { managers.emplace_back(new Live<AnmRenderManagerView>); context.managers[i] = managers.back()->get(); }
    for (int i = 0; i < 3; ++i) { resources.emplace_back(new Live<AnmLoadedView>); context.text[i] = resources.back()->get(); }
    for (int i = 0; i < 6; ++i)
    {
        vms.emplace_back(new Live<AnmVmView>);
        if (i < 2) { context.allocations[i] = vms.back()->get(); vms.back()->get()->flags35C = 0x100u + i; }
        else { context.lookups[i - 2] = vms.back()->get(); vms.back()->get()->glyphWidth = 31; vms.back()->get()->glyphHeight = 47; }
    }
    Live<AsciiManagerView> ascii;
    ascii->textAnm = context.text[0];
    g_AsciiManagerView = ascii.get();
    g_AnmRenderManagerView = context.managers[0];
    Live<BulletManagerView> bullets;
    context.bullets = bullets.get();
    g_EnemyBulletManager = bullets.get();
    const unsigned short states[] = {0, 1, 2, 3, 7, 65535};
    std::vector<int> expectedBulletIndices;
    std::vector<unsigned short> initialStates;
    for (int i = 0; i <= 2000; ++i)
    {
        const unsigned short state = i >= 1999 ? 2 : states[i % 6];
        bullets->bullets[i].state = state;
        initialStates.push_back(state);
        if (i < 2000 && state != 0 && state != 3) expectedBulletIndices.push_back(i);
    }
    Live<EnemyEffectWaitManager> laserOwner, decoyOwner;
    Live<HostLaser> laser0, laser1, laser2, laser3, decoy;
    HostLaser *lasers[] = {laser0.get(), laser1.get(), laser2.get(), laser3.get()};
    for (int i = 0; i < 4; ++i)
    {
        lasers[i]->label = i;
        lasers[i]->state = i;
        lasers[i]->changeNext = true;
        lasers[i]->next = i < 3 ? lasers[i + 1] : nullptr;
    }
    decoy->label = 99;
    decoy->state = 0;
    decoy->next = nullptr;
    decoyOwner->head = decoy.get();
    laserOwner->head = emptyLasers ? nullptr : laser0.get();
    // Opaque published pointer is converted BACK to the same complete live
    // EnemyEffectWaitManager by the production body. No fake prefix allocation.
    g_EnemyLaserManager = reinterpret_cast<EnemyLaserManagerView *>(laserOwner.get());
    g_HostDecoyLaserManager = reinterpret_cast<EnemyLaserManagerView *>(decoyOwner.get());
    Check(static_cast<void *>(bullets.get()) != static_cast<void *>(laserOwner.get()), "fixture.distinct_owners");
    Live<EnemyManagerView> enemyOwner;
    g_EnemyManager = enemyOwner.get();
    enemyOwner->timer.SetCurrent(50);
    Live<EnemyFullObjectView> enemy("constructor-enemy");
    enemy->runtime.flags = 0;
    enemy->runtime.worldMotion.position = {2.0f, 3.0f, 4.0f};
    enemy->runtime.deathEffectScript = 6;
    enemy->runtime.deathEffectResourceIndex = 0;
    enemyOwner->effectResources[0] = context.text[2];
    EnemyListNodeView enemyNode = {};
    enemyNode.enemy = enemy.get();
    enemyOwner->enemyListHead = &enemyNode;
    // The table and record are distinct live subobjects in one real host file.
    // The caller's int offset read is aligned and addresses an actual int.
    struct HostDialogFile { int table[8]; unsigned char record[4]; } file = {};
    const int dialogIndex = 2;
    file.table[1 + dialogIndex * 2] = offsetof(HostDialogFile, record);
    Live<GuiView> gui;
    gui->message = nullptr;
    gui->messageFile = &file;
    g_GuiView = gui.get();
    if (useCaller) context.callerGui = gui.get();
    context.originalDialogIndex = sameIndex ? dialogIndex + 1 : 71;
    g_EnemyDialogIndex = context.originalDialogIndex;
    g_EnemyDialogState = context.originalDialogState;
    GuiMessageVmView *message = nullptr;
    bool caught = false;
    messageMemory = {};
    messageMemory.armed = true;
    try
    {
        if (useCaller)
        {
            EnemyReadDialog(gui.get(), dialogIndex);
            message = gui->message;
        }
        else
        {
            message = new GuiMessageVmView(reinterpret_cast<const unsigned char *>(&file.record));
        }
    }
    catch (const HostCallbackException &) { caught = true; }
    catch (...) { Check(false, "exception.type"); }
    if (failure)
    {
        // Disarm before any assertion allocations could reuse the freed address.
        messageMemory.armed = false;
        Check(caught, "exception.propagates");
        Check(messageMemory.allocations == 1 && messageMemory.frees == 1,
              "exception.free_once");
        Check(messageMemory.completeDestructors == 0, "exception.no_complete_destructor");
        Check(gui->message == nullptr && g_EnemyDialogIndex == context.originalDialogIndex &&
              g_EnemyDialogState == context.originalDialogState, "caller.throw_no_publication");
        Check(context.allocatedBy.size() == failure->allocations &&
              context.initialized.size() == failure->initializations &&
              context.insertedBy.size() == failure->insertions &&
              context.foundBy.size() == failure->finds, "exception.callback_prefix");
        for (size_t i = 0; i < failure->insertions; ++i)
            Check(context.allocations[i]->id == (i == 0 ? 101 : 202),
                  "exception.insertions_retained");
        const size_t canceled = failure->bullets < 0 ? expectedBulletIndices.size() : size_t(failure->bullets);
        Check(context.bulletIndices == std::vector<int>(expectedBulletIndices.begin(), expectedBulletIndices.begin() + canceled),
              "exception.bullet_prefix");
        for (int i = 0; i <= 2000; ++i)
        {
            bool visited = false;
            for (size_t j = 0; j < canceled; ++j) if (expectedBulletIndices[j] == i) visited = true;
            const unsigned short expected = visited && (initialStates[i] == 1 || initialStates[i] == 2) ? 3 : initialStates[i];
            Check(bullets->bullets[i].state == expected, "exception.bullets_retained");
        }
        const int laserLabels[] = {0, 2, 3};
        Check(context.laserNodes == std::vector<int>(laserLabels, laserLabels + failure->lasers), "exception.laser_prefix");
        for (size_t i = 0; i < failure->lasers; ++i)
            Check(lasers[laserLabels[i]]->next == nullptr, "exception.laser_effects_retained");
        Check(context.effects.size() == failure->effects, "exception.effects_retained");
        if (failure->effects)
            Check(context.effects[0].resource == context.text[2] && context.effects[0].script == 6 &&
                  context.effects[0].value.x == 2.0f, "exception.effects_retained");
        Check(enemyOwner->timer.current == 50 && enemyOwner->timer.previous == 49 &&
              enemy->runtime.flags == 0, "exception.stops_at_callback");
        return;
    }
    Check(!caught && message != nullptr && message == messageMemory.pointer &&
          messageMemory.allocations == 1 && messageMemory.frees == 0 &&
          messageMemory.completeDestructors == 0, "message.ordinary_new");
    if (useCaller)
    {
        Check(gui->message == message, "caller.publication");
        Check(message->messageIndex == dialogIndex, "caller.message_index");
        Check(g_EnemyDialogIndex == dialogIndex + 1 &&
              g_EnemyDialogState == (sameIndex ? context.originalDialogState : 0), "caller.global_updates");
    }
    CheckAnmTimer(message->lifetimeTimer); CheckAnmTimer(message->instructionTimer); CheckAnmTimer(message->waitTimer);
    Check(message->messageIndex == (useCaller ? dialogIndex : 0) && message->unknown58 == 0 && message->inputCooldown == 0 &&
          message->flags == 0 && message->alternatingLine == 0 && message->activeSide == 0 &&
          message->currentInstruction == file.record, "message.fields");
    Check(message->textPositions[0].x == 8 && message->textPositions[1].x == 24 &&
          message->textPositions[0].y == 0 && message->textPositions[0].z == 0 &&
          message->textPositions[1].y == 0 && message->textPositions[1].z == 0,
          "message.positions");
    Check(message->textColors[0] == 0x00f8f08fu && message->textColors[1] == 0x008088ffu, "message.colors");
    for (int i = 0; i < GUI_MESSAGE_VM_COUNT; ++i)
        Check(message->vmIds[i].value == (i == GUI_MESSAGE_TEXT_LINE_0 ? 101 : i == GUI_MESSAGE_TEXT_LINE_1 ? 202 : 0), "message.ids");
    Check(context.allocatedBy.size() == 2 && context.initialized.size() == 2 && context.insertedBy.size() == 2, "vm.operation_counts");
    Check(context.insertedBy[0] == context.managers[1] && context.insertedBy[1] == context.managers[3], "vm.insertion_managers");
    Check(context.allocatedBy[0] == context.managers[0] && context.allocatedBy[1] == context.managers[1], "vm.allocation_managers");
    for (int i = 0; i < 2; ++i)
    {
        Check(context.initialized[i].resource == context.text[i], "vm.resources");
        Check(context.initialized[i].script == i, "vm.scripts");
        Check(context.initialized[i].layer == 15, "vm.layer");
        Check(context.initialized[i].flags == (0x40000100u + i), "vm.flags");
        Check(context.initialized[i].vm == context.allocations[i] && context.insertedVms[i] == context.allocations[i], "vm.live_allocation_identity");
    }
    Check(context.foundBy.size() == 4, "vm.lookup_count");
    Check(context.lookupIds == std::vector<int>({101, 101, 202, 202}), "vm.lookup_ids");
    for (int i = 0; i < 4; ++i)
    {
        Check(context.foundBy[i] == context.managers[3], "vm.lookup_managers");
        Check(context.lookups[i]->glyphWidth == (i % 2 == 0 ? 16 : 31) &&
              context.lookups[i]->glyphHeight == (i % 2 == 0 ? 47 : 16), "vm.glyph_destinations");
    }
    Check(context.bulletIndices == expectedBulletIndices, "bullet.indices");
    for (int i = 0; i <= 2000; ++i)
    {
        const unsigned short expected = i < 2000 && (initialStates[i] == 1 || initialStates[i] == 2) ? 3 : initialStates[i];
        Check(bullets->bullets[i].state == expected, "bullet.states_and_sentinel");
    }
    Check(context.laserNodes == (emptyLasers ? std::vector<int>() : std::vector<int>({0, 2, 3})), "laser.nodes");
    for (int mode : context.laserModes) Check(mode == 0, "laser.mode");
    Check(enemyOwner->timer.current == 51 && enemyOwner->timer.previous == 50 &&
          enemy->runtime.flags == 0x20000u && context.effects.size() == 1, "message.enemy_helper");
    const std::vector<std::string> beginning = {"allocate", "initialize", "insert", "allocate", "initialize", "insert", "find", "find", "find", "find"};
    Check(context.order.size() >= beginning.size(), "message.call_order");
    for (size_t i = 0; i < beginning.size(); ++i) Check(context.order[i] == beginning[i], "message.call_order");
    size_t cursor = beginning.size();
    for (size_t i = 0; i < expectedBulletIndices.size(); ++i) Check(context.order[cursor++] == "bullet", "message.call_order");
    for (size_t i = 0; i < context.laserNodes.size(); ++i) Check(context.order[cursor++] == "laser", "message.call_order");
    Check(cursor + 1 == context.order.size() && context.order[cursor] == "effect", "message.call_order");
    // Wait snippet uses an actually constructed live message, never raw storage.
    GuiMessageVmView *published = gui->message;
    gui->message = nullptr;
    Check(HostWaitSnippet() == 0, "gui_snippet.wait_empty");
    gui->message = message;
    Check(HostWaitSnippet() == 0xffffffffu, "gui_snippet.wait_active");
    message->inputCooldown = 1;
    Check(HostWaitSnippet() == 0, "gui_snippet.wait_cooldown");
    message->inputCooldown = 0;
    gui->message = published;
    delete message;
    messageMemory.armed = false;
    Check(messageMemory.frees == 1 && messageMemory.completeDestructors == 1,
          "message.success_delete_once");
}

int main()
{
    Check(host_source_lifecycle_order, "message.lifecycle_order");
    CheckActualTimerMethods<PlayerTimerView>();
    CheckActualTimerMethods<AnmVmTimerView>();
    TestHelperTimers();
    TestHelperEnemies();
    TestGuiFieldSnippets();
    TestConstructor(false);
    TestConstructor(true);
    TestConstructor(false, true);
    TestConstructor(false, true, nullptr, true);
    const ThrowCase failures[] = {
        {"allocate",   0, 1, 0, 0, 0,  0, 0, 0},
        {"initialize", 0, 1, 1, 0, 0,  0, 0, 0},
        {"initialize", 1, 2, 2, 1, 0,  0, 0, 0},
        {"insert",     1, 2, 2, 2, 0,  0, 0, 0},
        {"find",       2, 2, 2, 2, 3,  0, 0, 0},
        {"bullet",     1, 2, 2, 2, 4,  2, 0, 0},
        {"laser",      1, 2, 2, 2, 4, -1, 2, 0},
        {"effect",     0, 2, 2, 2, 4, -1, 3, 1},
    };
    for (const ThrowCase &failure : failures)
    {
        TestConstructor(false, false, &failure);
        TestConstructor(false, true, &failure);
    }
    std::puts("host semantic fixture passed");
    return 0;
}
