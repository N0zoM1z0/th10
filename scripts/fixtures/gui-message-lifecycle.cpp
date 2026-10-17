// Fresh local host integration, rebuilt after workspace recovery. Complete
// maintained headers/owners are retained. Only the enumerated timer, SDK,
// dependency lifecycle and callback seams below are substituted. No ABI claim.
#define __stdcall
#define __fastcall
#define __cdecl
#define __thiscall
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <utility>
#include "owner.inc"
#include "EnemyLaser.hpp"
#include "BulletManager.hpp"
#include "shared-declarations.inc"

static void Check(bool condition, const char *name)
{
    if (!condition) { std::fprintf(stderr, "CHECK %s\n", name); std::exit(1); }
}

// Ordinary-new observation inherited from the full actual-header constructor
// fixture. No raw prefilling, fabricated object, placement message construction,
// class-specific allocator, or substitute constructor exception cleanup.
struct MessageAllocationObservation
{
    bool armed = false;
    void *pointer = nullptr;
    unsigned allocations = 0, frees = 0;
};
static MessageAllocationObservation messageMemory;
void *operator new(std::size_t size)
{
    void *pointer = std::malloc(size == 0 ? 1 : size);
    if (!pointer) throw std::bad_alloc();
    if (messageMemory.armed && !messageMemory.pointer && size == sizeof(GuiMessageVmView))
    {
        messageMemory.pointer = pointer;
        ++messageMemory.allocations;
    }
    return pointer;
}
void operator delete(void *pointer) noexcept
{
    if (messageMemory.armed && pointer && pointer == messageMemory.pointer)
    {
        ++messageMemory.frees;
        Check(messageMemory.frees == 1, "allocation.free_once");
    }
    std::free(pointer);
}
void operator delete(void *pointer, std::size_t) noexcept { ::operator delete(pointer); }

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
    Live(const Live&) = delete;
    Live &operator=(const Live&) = delete;
};

float g_AnmGameSpeed = 1.0f;
AnmRenderManagerView *g_AnmRenderManagerView;
AsciiManagerView *g_AsciiManagerView;
EnemyManagerView *g_EnemyManager;
void *g_EnemyBulletManager;
EnemyLaserManagerView *g_EnemyLaserManager;
int g_EnemyDialogIndex, g_EnemyDialogState;
GuiView *g_GuiView;
Player *g_Player;
GuiPlayerView *g_GuiPlayer;
GuiGameOwnerView *g_GuiGameOwner;
GuiModeOwnerView *g_GuiModeOwner;
unsigned char *g_GuiPersistentData;
int g_GuiHighScore, g_GuiCounter4C4C, g_GuiState4C58, g_GuiCharacter;
int g_GuiShotType, g_GuiPowerLevel, g_GuiDifficulty, g_GuiStage;
int g_GuiMessageState, g_GuiMessageStateAux, g_GuiValue4C90, g_GuiValue4C94, g_GuiValue4C9C;
int g_GuiGlobalMode;
short g_GuiPower;
unsigned int g_GuiGameFlags, g_GuiInputCurrent, g_GuiInputEdges, g_GuiSupervisorFlags;
GuiScoreView g_GuiScore;
MainSupervisorView g_MainSupervisorView;
GameStageUpdateView *g_GameStageUpdate;
GameChainOwnerView *g_GameOwner477830;
// Unexecuted full-opcode extern storage exists only for linking; no owner or
// layout claim is made for it. Actual WAIT uses the canonical sound object.
unsigned char g_GameStageState[0x4000];
alignas(EnemySoundQueueView) unsigned char g_MainSoundOwner[0x52d0];
EnemySoundCueMetadataView g_EnemySoundCueMetadata[134];
unsigned int g_FrontEndSupervisorFlags;
unsigned char *g_FrontEndProfileData;

// Explicit host dependency lifecycle bodies. Message constructor/destructor
// remain actual source, not seams. Complete member declarations remain live.
AnmVmView::AnmVmView() {}
AnmVmView::~AnmVmView() {}
AnmLoadedView::AnmLoadedView() {}
AnmRenderManagerView::AnmRenderManagerView() {}
AnmRenderManagerView::~AnmRenderManagerView() {}
AsciiManagerView::AsciiManagerView() {}
AsciiManagerView::~AsciiManagerView() {}
size_t AsciiManagerView::GetSize() { return sizeof(*this); }
GuiView::~GuiView() {}
EnemyManagerView::EnemyManagerView() {}
EnemyManagerView::~EnemyManagerView() {}
// Vtable/RTTI linkage for the complete canonical node referenced by the full
// constructor. Its live owner list is empty; no laser callback is executed.
void EnemyEffectWaitNode::UnknownVirtual0() { Check(false, "unused.laser0"); }
void EnemyEffectWaitNode::UnknownVirtual1() { Check(false, "unused.laser1"); }
void EnemyEffectWaitNode::UnknownVirtual2() { Check(false, "unused.laser2"); }
void EnemyEffectWaitNode::UnknownVirtual3() { Check(false, "unused.laser3"); }
void EnemyEffectWaitNode::UnknownVirtual4() { Check(false, "unused.laser4"); }
void EnemyEffectWaitNode::UnknownVirtual6() { Check(false, "unused.laser6"); }
unsigned int EnemyEffectWaitNode::ApplyPatternCancel(const PlayerFloat3 *, float, int)
{ Check(false, "unused.laser_pattern"); return 0; }
unsigned int EnemyEffectWaitNode::ApplyClear(int)
{ Check(false, "unused.laser_clear"); return 0; }

struct HostCallbackException {};
struct HostContext
{
    GuiView *gui = nullptr;
    AnmRenderManagerView *manager = nullptr;
    AnmRenderManagerView *alternateManager = nullptr;
    AnmLoadedView *text = nullptr;
    AnmVmView *allocations[2] = {};
    GuiMessageVmView *message = nullptr;
    bool failSecondInitialize = false, deleting = false;
    unsigned allocationsSeen = 0, initialized = 0, inserted = 0, deleteCalls = 0;
    int expectedIds[GUI_MESSAGE_VM_COUNT] = {};
};
static HostContext *host;
static void BeforePublication()
{
    Check(host && host->gui->message == nullptr && g_EnemyDialogIndex == 71 &&
          g_EnemyDialogState == 93, "factory.no_early_publication");
    Check(messageMemory.pointer && messageMemory.allocations == 1 &&
          messageMemory.frees == 0, "constructor.actual_allocation");
}

// Explicit constructor callbacks. Insertion links canonical live nodes; every
// subsequent lookup and deletion executes the real maintained implementation.
AnmVmView *AnmRenderManagerView::AllocateVm()
{
    BeforePublication();
    Check(this == host->manager && host->allocationsSeen < 2, "constructor.allocate");
    return host->allocations[host->allocationsSeen++];
}
void AnmLoadedView::InitializeVm(AnmVmView *vm, int script)
{
    BeforePublication();
    Check(this == host->text && script == int(host->initialized) &&
          vm == host->allocations[host->initialized], "constructor.initialize");
    Check(vm->renderLayer == 15 && vm->flags35C == (0x40000000u | (0x100u + host->initialized)),
          "constructor.fields_before_initialize");
    ++host->initialized;
    if (host->failSecondInitialize && script == 1) throw HostCallbackException();
}
AnmVmIdView AnmRenderManagerView::AddVmVariant0(AnmVmView *vm)
{
    BeforePublication();
    Check(this == host->manager && host->inserted < 2 && vm == host->allocations[host->inserted],
          "constructor.insert");
    vm->id = host->inserted == 0 ? 104 : 105;
    vm->managerNode.owner = vm;
    vm->managerNode.previous = primaryVmListTail;
    vm->managerNode.next = nullptr;
    if (primaryVmListTail) primaryVmListTail->next = &vm->managerNode;
    else primaryVmListHead = &vm->managerNode;
    primaryVmListTail = &vm->managerNode;
    ++host->inserted;
    return AnmVmIdView(vm->id);
}
unsigned int EnemyCancelBulletRecord(unsigned char *)
{ Check(false, "constructor.empty_bullets"); return 0; }
AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(int, const AnmFloat3View *)
{ Check(false, "constructor.empty_enemies"); return AnmVmIdView(0); }

// Host linker observation only: each call forwards to the unchanged real
// maintained method in dependencies.cpp. No deletion model is substituted.
extern "C" void __real__ZN20AnmRenderManagerView17MarkVmForDeletionE11AnmVmIdView(
    AnmRenderManagerView *, AnmVmIdView);
extern "C" void __wrap__ZN20AnmRenderManagerView17MarkVmForDeletionE11AnmVmIdView(
    AnmRenderManagerView *manager, AnmVmIdView id)
{
    Check(host && host->deleting && host->message && manager == host->manager,
          "destructor.cached_manager");
    Check(host->gui->message == host->message && messageMemory.frees == 0,
          "destructor.owner_still_published");
    const unsigned index = host->deleteCalls;
    Check(index < GUI_MESSAGE_VM_COUNT && id.value == host->expectedIds[index],
          "destructor.six_ids_once_in_order");
    for (unsigned i = 0; i < index; ++i)
        Check(host->message->vmIds[i].value == 0, "destructor.prior_ids_cleared");
    ++host->deleteCalls;
    // Synthetic callback mutation uses another complete live manager. Cached
    // receiver behavior must survive; actual FindVm/Mark use their this owner.
    g_AnmRenderManagerView = host->alternateManager;
    __real__ZN20AnmRenderManagerView17MarkVmForDeletionE11AnmVmIdView(manager, id);
}

// Rejecting link boundaries for unexecuted cases of the complete 24-opcode Run.
AnmVmIdView AnmLoadedView::CreateVmVariant0(int, unsigned)
{ Check(false, "unused.create"); return AnmVmIdView(0); }
int AnmLoadedView::SetSprite(AnmVmView *, int) { Check(false, "unused.sprite"); return 0; }
void AnmRenderManagerView::DrawTextLeft(AnmVmView *, unsigned, const char *, ...)
{ Check(false, "unused.draw"); }
void FrontEndQueueSoundCommand(const char *, int, int) { Check(false, "unused.music"); }
void GameCompleteStageTransition(ResultMenuView *) { Check(false, "unused.transition"); }
int GameQueueScaledSoundCommand(float) { Check(false, "unused.scaled_sound"); return 0; }
void *StageCreateTransitionEffect(int, int, int, unsigned, unsigned, unsigned)
{ Check(false, "unused.effect"); return nullptr; }
namespace th10 { namespace leaf {
unsigned char *Fn00417010(unsigned char *) { Check(false, "unused.decode"); return nullptr; }
void Fn004175E0(unsigned char *) { Check(false, "unused.advance"); }
} }

#include "construction.inc"

static void TimerAt(const AnmVmTimerView &timer, int current, int previous, float subframe,
                    const char *name)
{
    Check(timer.current == current && timer.previous == previous && timer.subframe == subframe &&
          timer.flags == 1 && timer.scale == &g_AnmGameSpeed, name);
}

struct Fixture
{
    HostContext context;
    Live<AnmRenderManagerView> manager, alternateManager;
    Live<AnmLoadedView> text;
    Live<AsciiManagerView> ascii;
    Live<BulletManagerView> bullets;
    Live<EnemyEffectWaitManager> lasers;
    Live<EnemyManagerView> enemies;
    Live<GuiView> gui;
    AnmVmView vms[8];
    // Real aligned int table plus bounded live unsigned-char record subarray.
    // The factory's int read addresses actual int storage. No instruction
    // object, record-sidecar seam or fabricated lifetime is involved.
    struct DialogFile { int table[8]; unsigned char records[12]; } file = {};
    static const int index = 2;
    explicit Fixture(bool timedEnd = false)
    {
        host = &context;
        context.gui = gui.get(); context.manager = manager.get();
        context.alternateManager = alternateManager.get(); context.text = text.get();
        context.allocations[0] = &vms[3]; context.allocations[1] = &vms[4];
        g_AnmGameSpeed = 1.0f; g_AnmRenderManagerView = manager.get();
        g_AsciiManagerView = ascii.get(); g_EnemyBulletManager = bullets.get();
        g_EnemyLaserManager = reinterpret_cast<EnemyLaserManagerView *>(lasers.get());
        g_EnemyManager = enemies.get(); g_GuiView = gui.get();
        g_EnemyDialogIndex = 71; g_EnemyDialogState = 93;
        g_GuiInputCurrent = 0; g_GuiInputEdges = 0;
        g_GuiMessageState = 7; g_GuiMessageStateAux = 9;
        ascii->textAnm = text.get();
        manager->primaryVmListHead = manager->primaryVmListTail = nullptr;
        manager->secondaryVmListHead = manager->secondaryVmListTail = nullptr;
        alternateManager->primaryVmListHead = alternateManager->primaryVmListTail = nullptr;
        alternateManager->secondaryVmListHead = alternateManager->secondaryVmListTail = nullptr;
        for (int i = 0; i < 2000; ++i) bullets->bullets[i].state = 0;
        lasers->head = nullptr; enemies->enemyListHead = nullptr;
        enemies->timer.SetCurrent(50);
        gui->message = nullptr; gui->messageFile = &file;
        file.table[1 + index * 2] = offsetof(DialogFile, records);
        file.records[0] = 0; file.records[1] = 0;
        file.records[2] = 10; file.records[3] = 4; // WAIT with four-byte int payload.
        const int wait = timedEnd ? 1 : 3;
        std::memcpy(file.records + 4, &wait, sizeof(wait));
        file.records[8] = timedEnd ? 2 : 0; file.records[9] = 0;
        file.records[10] = 0; file.records[11] = 0; // END, no payload.
        for (int i = 0; i < 8; ++i)
        {
            vms[i].id = 101 + i; vms[i].flags35C = 0x100u + i;
            vms[i].glyphWidth = 31; vms[i].glyphHeight = 47;
            vms[i].managerNode.owner = &vms[i];
            vms[i].managerNode.previous = vms[i].managerNode.next = nullptr;
            vms[i].layerNode.owner = &vms[i];
            vms[i].layerNode.previous = vms[i].layerNode.next = nullptr;
        }
        vms[3].flags35C = 0x100; vms[4].flags35C = 0x101;
        EnemySoundQueueView *sound = new (g_MainSoundOwner) EnemySoundQueueView;
        for (int i = 0; i < 12; ++i) { sound->activeSoundIds[i] = -1; sound->sampleCounts[i] = 0; }
        sound->cueValues[0] = -77; g_EnemySoundCueMetadata[0].cueValue = 17;
        messageMemory = {}; messageMemory.armed = true;
    }
    ~Fixture()
    {
        messageMemory.armed = false;
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->~EnemySoundQueueView();
    }
    GuiMessageVmView *Construct()
    {
        EnemyReadDialog(gui.get(), index);
        GuiMessageVmView *message = gui->message;
        context.message = message;
        Check(message == messageMemory.pointer && messageMemory.allocations == 1 &&
              messageMemory.frees == 0, "factory.real_new");
        Check(message->messageIndex == index && g_EnemyDialogIndex == index + 1 &&
              g_EnemyDialogState == 0, "factory.publication_and_globals");
        Check(message->currentInstruction == file.records, "constructor.byte_cursor");
        Check(context.allocationsSeen == 2 && context.initialized == 2 && context.inserted == 2,
              "constructor.callback_sequence");
        for (int i = 0; i < GUI_MESSAGE_VM_COUNT; ++i)
            Check(message->vmIds[i].value == (i == 3 ? 104 : i == 4 ? 105 : 0), "constructor.ids");
        Check(vms[3].glyphWidth == 16 && vms[3].glyphHeight == 16 &&
              vms[4].glyphWidth == 16 && vms[4].glyphHeight == 16, "constructor.actual_list_resolution");
        TimerAt(message->lifetimeTimer, 0, -1, 0, "constructor.lifetime_timer");
        TimerAt(message->instructionTimer, 0, -1, 0, "constructor.instruction_timer");
        TimerAt(message->waitTimer, 0, -1, 0, "constructor.wait_timer");
        Check(enemies->timer.current == 51 && enemies->timer.previous == 50,
              "constructor.actual_enemy_helper");
        return message;
    }
    int SoundCalls() const
    {
        const EnemySoundQueueView *sound = reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner);
        for (int i = 1; i < 12; ++i)
            Check(sound->activeSoundIds[i] == -1 && sound->sampleCounts[i] == 0, "sound.only_expected_slot");
        if (sound->sampleCounts[0] == 0)
            Check(sound->activeSoundIds[0] == -1 && sound->cueValues[0] == -77, "sound.pending_unchanged");
        else
            Check(sound->activeSoundIds[0] == 0 && sound->samples[0][0] == 0 && sound->cueValues[0] == 17,
                  "sound.actual_owner_args_metadata");
        return sound->sampleCounts[0];
    }
    void SeedOtherFourIds()
    {
        // Explicit setup, not claimed outputs of the constructor or WAIT.
        AnmVmLayerNodeView *tail = nullptr;
        for (int i : {0, 1, 2, 5})
        {
            context.message->vmIds[i].value = vms[i].id;
            vms[i].managerNode.previous = tail;
            if (tail) tail->next = &vms[i].managerNode;
            else manager->secondaryVmListHead = &vms[i].managerNode;
            tail = &vms[i].managerNode;
        }
        manager->secondaryVmListTail = tail;
        vms[0].layerNode.next = &vms[6].layerNode;
        vms[6].layerNode.previous = &vms[0].layerNode;
    }
    void Destroy(bool allIds)
    {
        GuiMessageVmView *message = context.message;
        Check(gui->message == message && messageMemory.frees == 0, "owner.before_delete");
        for (int i = 0; i < GUI_MESSAGE_VM_COUNT; ++i) context.expectedIds[i] = message->vmIds[i].value;
        context.deleting = true;
        delete message; // Actual maintained destructor followed by ordinary delete.
        context.deleting = false;
        gui->message = nullptr; // Explicit owner harness cleanup; not updater coverage.
        context.message = nullptr;
        messageMemory.armed = false;
        Check(messageMemory.allocations == 1 && messageMemory.frees == 1 &&
              context.deleteCalls == GUI_MESSAGE_VM_COUNT, "destructor.six_calls_and_one_free");
        Check(gui->message == nullptr, "owner.unpublished_after_delete");
        for (int i = 0; i < 8; ++i)
        {
            const bool expected = allIds ? i < 7 : i == 3 || i == 4;
            Check(((vms[i].flags35C & 0x04000000u) != 0) == expected,
                  "destructor.real_primary_secondary_child_deletion");
        }
    }
};

static void TestPendingThenEnd(bool allIds)
{
    Fixture f;
    GuiMessageVmView *message = f.Construct();
    if (allIds) f.SeedOtherFourIds();
    message->inputCooldown = 2; message->alternatingLine = 1;
    for (int expected : {2, 1})
    {
        Check(GuiMessageVmView::Run(message) == 0, "wait.pending_return");
        Check(message->currentInstruction == f.file.records && f.gui->message == message &&
              messageMemory.frees == 0 && f.context.deleteCalls == 0, "wait.owner_cursor_live");
        TimerAt(message->waitTimer, expected, expected + 1, float(expected), "wait.actual_add");
        TimerAt(message->instructionTimer, 0, -1, 0, "wait.no_instruction_tick");
        TimerAt(message->lifetimeTimer, 0, -1, 0, "wait.lifetime_owned_outside_run");
        Check(message->inputCooldown == expected - 1 && message->alternatingLine == 1 && f.SoundCalls() == 0 &&
              g_GuiMessageState == 7 && g_GuiMessageStateAux == 9, "wait.pending_state");
    }
    Check(GuiMessageVmView::Run(message) == -1, "end.terminal_return");
    Check(message->currentInstruction == f.file.records + 8 && f.gui->message == message &&
          messageMemory.frees == 0 && f.context.deleteCalls == 0, "end.owner_live_until_delete");
    Check(f.SoundCalls() == 1 && message->alternatingLine == 0 &&
          g_GuiMessageState == 0 && g_GuiMessageStateAux == 0, "end.sound_and_state_clear");
    TimerAt(message->waitTimer, 0, -1, 0, "end.wait_reset");
    TimerAt(message->instructionTimer, 0, -1, 0, "end.no_instruction_tick");
    TimerAt(message->lifetimeTimer, 0, -1, 0, "end.lifetime_unchanged");
    f.Destroy(allIds);
}

static void TestScheduledEnd()
{
    Fixture f(true);
    GuiMessageVmView *message = f.Construct();
    Check(GuiMessageVmView::Run(message) == 0 && message->currentInstruction == f.file.records + 8,
          "scheduled.wait_advances_to_future_end");
    TimerAt(message->instructionTimer, 1, 0, 1, "scheduled.first_tick");
    Check(GuiMessageVmView::Run(message) == 0, "scheduled.pending");
    TimerAt(message->instructionTimer, 2, 1, 2, "scheduled.second_tick");
    Check(GuiMessageVmView::Run(message) == -1, "scheduled.end_timestamp");
    TimerAt(message->instructionTimer, 2, 1, 2, "scheduled.end_no_tick");
    TimerAt(message->lifetimeTimer, 0, -1, 0, "scheduled.lifetime_unchanged");
    Check(f.SoundCalls() == 1, "scheduled.sound_once");
    f.Destroy(false);
}

static void TestFactoryFailure()
{
    Fixture f;
    f.context.failSecondInitialize = true;
    bool caught = false;
    try { EnemyReadDialog(f.gui.get(), Fixture::index); }
    catch (const HostCallbackException &) { caught = true; }
    messageMemory.armed = false;
    Check(caught && f.gui->message == nullptr && g_EnemyDialogIndex == 71 &&
          g_EnemyDialogState == 93, "exception.no_publication");
    Check(messageMemory.allocations == 1 && messageMemory.frees == 1 && f.context.deleteCalls == 0,
          "exception.one_free_no_complete_destructor");
    Check(f.context.allocationsSeen == 2 && f.context.initialized == 2 && f.context.inserted == 1 &&
          f.manager->primaryVmListHead == &f.vms[3].managerNode, "exception.prior_effects_retained");
}

int main()
{
    Check(CHAR_BIT == 8 && sizeof(unsigned short) == 2 && sizeof(int) == 4, "host.scalar_widths");
    const unsigned int one = 1;
    Check(*reinterpret_cast<const unsigned char *>(&one) == 1, "host.little_endian");
    TestPendingThenEnd(false);
    TestPendingThenEnd(true);
    TestScheduledEnd();
    TestFactoryFailure();
    std::puts("actual constructor/run/destructor lifecycle passed");
    return 0;
}
