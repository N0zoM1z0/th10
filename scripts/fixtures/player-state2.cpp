// Actual extracted bodies with explicit host role projections and boundary seams.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <initializer_list>
#define __forceinline inline
#define __declspec(x)
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "PLAYER_STATE2_CHECK_FAILED line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)
@DECLARATIONS@
float g_AnmGameSpeed = 1.0f;
struct PlayerTimerView { int previous, current; float subframe; float *scale; unsigned flags; @TIMER@ };
struct HintOwnerView {
    HintLinkView exportStageLists[8];
    HintEntryView *AddAutomaticEntry(const char *, const PlayerFloat3 *);
    void AddCaution(const char *, const PlayerFloat3 *);
};
struct AnmVmView {};
struct PlayerDrawVmView {};
struct AnmLoadedView { void InitializeAndExecuteScriptIndex(AnmVmView *, int); };
struct Option { int state; unsigned primaryVmId, secondaryVmId, canary; };
struct Player { void *resource; PlayerDrawVmView drawVm; PlayerFloat3 drawPosition; int runtimeState; PlayerTimerView updateTimer1, highlightTimer; Option options[4]; int optionCount; };
struct PanelVm { unsigned flags35C; };
struct EnemyGameStateView { PlayerTimerView spellTimer; int currentSpellBonus; unsigned flags378C; PanelVm frontVms[8]; };
struct PlayerVm;
struct PlayerVmListNode { PlayerVm *value; PlayerVmListNode *next, *previous; };
struct PlayerVm { PlayerVmListNode *children; int childOwner; unsigned short deleteState; };
struct Gate { int value10; };
struct GuiView {};
GameScoreStateView g_GameScoreState = {};
volatile int g_PlayerLivesDisplayCount;
int g_GameStartupMode, g_GameFrameRestorePending, g_GameFrameCounter0, g_GameFrameCounter1;
GuiView *g_GuiView;
Gate *g_PlayerUpdateGate10;
HintOwnerView *g_GameOwner477814;
EnemyGameStateView *g_EnemyGameState;
Player *active;
EnemyGameStateView *allocationPanel;
PlayerVm *lookup[3];
bool armed;
int guiCalls, initCalls, faithBeforeCall, livesBeforeCall;
static PlayerVm *FindPlayerVm(unsigned id) { return id < 3 ? lookup[id] : nullptr; }
void GuiSetLivesDisplayCount(GuiView *, int count) {
    CHECK(count == livesBeforeCall - 1 && count == g_PlayerLivesDisplayCount);
    CHECK(g_GameScoreState.faith == faithBeforeCall);
    CHECK(active->runtimeState == 4 && initCalls == 0);
    ++guiCalls;
}
void AnmLoadedView::InitializeAndExecuteScriptIndex(AnmVmView *vm, int script) {
    CHECK(this == active->resource && vm == reinterpret_cast<AnmVmView *>(&active->drawVm) && script == 0);
    CHECK(active->runtimeState == 2 && active->updateTimer1.current == 0 && active->highlightTimer.current == 180);
    active->drawPosition = {11.5f, 22.25f, 3.0f};
    ++initCalls;
}
void *operator new(std::size_t n) {
    void *p = std::malloc(n); if (!p) throw std::bad_alloc();
    if (armed && n == sizeof(HintEntryView)) {
        armed = false;
        CHECK(initCalls == 1 && active->optionCount == 0);
        g_EnemyGameState = allocationPanel;
        g_GameScoreState.rank = 1000;
    }
    return p;
}
void operator delete(void *p) noexcept { std::free(p); }
void operator delete(void *p, std::size_t) noexcept { std::free(p); }
@SUPPORT@
@BODIES@
static void panelInit(EnemyGameStateView &p, int frames) {
    p.spellTimer.current = frames; p.currentSpellBonus = 1234; p.flags378C = 0x17;
    for (int i=0;i<8;++i) p.frontVms[i].flags35C = 0x13u + (i << 4);
}
int main() {
    int livesValues[] = {-1,0,1,2}; int faithValues[] = {0,4999,5000,5001,5010,90000,99999};
    for (int lives : livesValues) for (int faith : faithValues) for (int frames : {59,60})
    for (int gate : {0,1}) for (int initialized : {0,1}) for (int base : {0,3}) for (int rank : {-1024,0,500,3000}) {
        HintOwnerView owner = {}; Player player = {}; EnemyGameStateView first = {}, second = {};
        HintEntryView old[3]; PlayerVm a = {}, b = {}, childA = {}, childB = {};
        PlayerVmListNode na = {&childA,nullptr,nullptr}, nb = {&childB,nullptr,nullptr};
        a.children=&na; b.children=&nb; b.childOwner=1; a.deleteState=b.deleteState=7; childA.deleteState=childB.deleteState=9;
        lookup[0]=nullptr; lookup[1]=&a; lookup[2]=&b;
        Gate gameGate = {gate}; GuiView gui; AnmLoadedView resource;
        panelInit(first,frames); panelInit(second,frames);
        active=&player; allocationPanel=&second; g_EnemyGameState=&first;
        g_PlayerUpdateGate10=&gameGate; g_GuiView=&gui; g_GameOwner477814=&owner;
        g_PlayerLivesDisplayCount=lives; livesBeforeCall=lives; guiCalls=initCalls=0;
        g_GameScoreState.faith=faith; g_GameScoreState.rank=rank;
        int expectedFaith=faith-(faith-5000)/3; if (expectedFaith<5000) expectedFaith=5000;
        faithBeforeCall=expectedFaith;
        g_GameStartupMode=3; g_GameFrameRestorePending=base; g_GameFrameCounter0=120; g_GameFrameCounter1=20;
        old[0].remaining=-1; old[1].remaining=0; old[2].remaining=5;
        for (auto &entry : old) owner.exportStageLists[3].Append(&entry.link);
        player.resource=&resource; player.runtimeState=4; player.optionCount=4;
        player.updateTimer1.flags=0x20|initialized; player.highlightTimer.flags=0x40|initialized;
        float oldScale=2; player.updateTimer1.scale=player.highlightTimer.scale=&oldScale;
        for (int i=0;i<4;++i) { auto &option=player.options[i]; option.state=2; option.primaryVmId=i==3 ? 0 : 1; option.secondaryVmId=i==3 ? 99 : 2; option.canary=0xa5; }
        armed=gate!=1;
        PlayerTransitionToState2(&player);
        CHECK(g_GameScoreState.faith == expectedFaith);
        CHECK(g_PlayerLivesDisplayCount==lives-1 && guiCalls==(lives>0));
        CHECK(player.runtimeState==2 && initCalls==1 && player.optionCount==0);
        CHECK(player.updateTimer1.previous==-1 && player.updateTimer1.current==0 && player.updateTimer1.subframe==0);
        CHECK(player.highlightTimer.previous==179 && player.highlightTimer.current==180 && player.highlightTimer.subframe==180);
        CHECK(player.updateTimer1.scale==(initialized ? &oldScale : &g_AnmGameSpeed));
        CHECK(player.highlightTimer.scale==(initialized ? &oldScale : &g_AnmGameSpeed));
        CHECK(player.updateTimer1.flags==0x21 && player.highlightTimer.flags==0x41);
        for (int i=0;i<4;++i) { const auto &option=player.options[i]; CHECK(option.state==0 && option.primaryVmId==(i==3 ? 0u : 1u) && option.secondaryVmId==(i==3 ? 99u : 2u) && option.canary==0xa5); }
        CHECK(a.deleteState==1 && b.deleteState==1 && childA.deleteState==1 && childB.deleteState==9);
        EnemyGameStateView *panel=gate!=1 ? &second : &first;
        CHECK(g_EnemyGameState==panel);
        CHECK(panel->currentSpellBonus==(frames>=60 ? 0 : 1234));
        CHECK(panel->flags378C==(frames>=60 ? 0x15u : 0x17u));
        for (int i=0;i<8;++i) {
            unsigned expected=0x13u+(i<<4); if (frames>=60 && i!=2) expected&=~2u;
            CHECK(panel->frontVms[i].flags35C == expected);
        }
        int expectedRank=(gate!=1 ? 1000 : rank)-1024;
        if (expectedRank>1024) expectedRank=1024;
        if (expectedRank< -1024) expectedRank=-1024;
        CHECK(g_GameScoreState.rank == expectedRank);
        CHECK(old[0].remaining==-1);
        CHECK(old[1].remaining == 0);
        CHECK(old[2].remaining==(gate!=1 ? 4 : 5));
        if (gate!=1) {
            auto *entry=static_cast<HintEntryView *>(old[2].link.next->payload);
            CHECK(entry->count == (base == 0 ? 60 : 1));
            CHECK(entry->time==90 && entry->color==0xffff8080u && entry->remaining==30);
            CHECK(entry->position.x==11.5f && entry->position.y==22.25f && entry->position.z==3.0f);
            CHECK(std::strcmp(entry->text,"Caution!")==0 && entry->link.next==nullptr && entry->link.previous==&old[2].link);
            delete entry;
        } else CHECK(old[2].link.next==nullptr);
    }
    HintOwnerView owner = {}; PlayerFloat3 position = {1,2,3}; char text[81]; std::memset(text,'Q',80); text[80]=0;
    g_GameStartupMode=7; g_GameFrameRestorePending=0; g_GameFrameCounter0=123; armed=false;
    HintEntryView *entry=owner.AddAutomaticEntry(text,&position);
    CHECK(entry->text[64] == 0);
    CHECK(std::strlen(entry->text)==64 && entry->remaining==30 && entry->time==300 && entry->count==123);
    CHECK(entry->alignment==0 && entry->base==0 && entry->color==0xffffffffu && entry->scale==1.0f && entry->textWidth==0);
    CHECK(owner.exportStageLists[7].next==&entry->link && owner.exportStageLists[6].next==nullptr);
    delete entry;
    std::puts("PASS player-state2 source contracts");
}
