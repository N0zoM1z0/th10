// HOST FIXTURE, not production or an independently reconstructed effect.
// All callbacks above are extracted from the proposal on every run.
// Scheduler/renderer/allocation/recursive-lock behavior below are named seams.

namespace th10 {
CRITICAL_SECTION g_CriticalSections[7];
unsigned char g_ChainNestCounter=0;
Chain *g_Chain=nullptr;
}
float g_AnmGameSpeed=1.0f;
RngView g_RngView={}, g_AlternateRngView={};
ResultGameManagerView *g_ResultGameManager=nullptr;
int g_GameStartupCounter=0;
AnmViewportView g_TransitionViewport={};
float g_TransitionShakeX=0, g_TransitionShakeY=0;
AnmRenderManagerView *g_AnmRenderManagerView=nullptr;
D3d9DeviceView *g_Direct3DDevice=nullptr;

namespace host {
int assertions=0;
void check(bool value,const char *id) {
    ++assertions;
    if(!value) throw std::runtime_error(std::string("ASSERT:")+id);
}
#define CHECK(value,id) host::check((value),(id))
struct Allocation { std::string kind; bool alive; };
std::map<std::uintptr_t,Allocation> allocations;
int allocatedEffects=0,freedEffects=0,allocatedNodes=0,freedNodes=0;
bool failEffectAllocation=false;
th10::ChainElem *pendingNode=nullptr;
int maxDepth=0;
std::vector<int> enterDepths,leaveNesting;
std::function<void()> enterHook,leaveHook,flushHook;
bool inHook=false;
th10::Chain chainA={},chainB={};
ResultGameManagerView game={};
AnmRenderManagerView managerA={1},managerB={2};
D3d9DeviceView deviceA={},deviceB={};
std::vector<std::string> renderEvents;
AnmFloatRectView lastRectangle={};
unsigned int lastColor=0;
AnmViewportView lastViewport={};
int lastDrawManager=0,lastViewportDevice=0;
struct Snapshot {
    unsigned int flags,unknown04,unknown14,timerFlags;
    int kind,frames,alpha,released;
    unsigned int value0,value1,value2;
    int timerCurrent,timerPrevious;
    float timerSubframe;
    float *timerScale;
    std::uintptr_t update,draw,node;
    th10::ChainCallback deleted;
};
std::vector<Snapshot> registrations;
std::uintptr_t address(const void *p) { return reinterpret_cast<std::uintptr_t>(p); }
bool alive(std::uintptr_t p) { auto i=allocations.find(p);return i!=allocations.end()&&i->second.alive; }
void remember(void *p,const char *kind) { CHECK(p!=nullptr,"HOST_ALLOC_OK");allocations[address(p)]={kind,true}; }
void observeRegistration() {
    if(!pendingNode||!pendingNode->arg||(pendingNode->flags&2)==0)return;
    auto i=allocations.find(address(pendingNode->arg));
    if(i==allocations.end()||i->second.kind!="effect"||!i->second.alive)return;
    auto *e=static_cast<TransitionEffectView *>(pendingNode->arg);
    registrations.push_back({e->flags,e->unknown04,e->unknown14,e->timer.flags,e->kind,e->frames,e->alpha,e->released,e->value0,e->value1,e->value2,e->timer.current,e->timer.previous,e->timer.subframe,e->timer.scale,address(e->updateNode),address(e->drawNode),address(pendingNode),e->updateNode?e->updateNode->deletedCallback:nullptr});
    pendingNode=nullptr;
}
int setViewport(D3d9DeviceView *device,AnmViewportView *viewport) {
    renderEvents.push_back("viewport");lastViewport=*viewport;lastViewportDevice=device->id;
    return 0;
}
HostDeviceVtable vtable={setViewport};
void reset() {
    for(const auto &a:allocations)CHECK(!a.second.alive,"HOST_NO_UNFINISHED_OWNERS");
    allocations.clear();
    CHECK(th10::g_CriticalSections[0].depth==0&&th10::g_ChainNestCounter==0,"HOST_LOCK_BALANCED");
    // Reconstruct real sentinels in place; copying a temporary would stale self.
    chainA.~Chain();chainB.~Chain();
    std::memset(&chainA,0,sizeof(chainA));std::memset(&chainB,0,sizeof(chainB));
    new(&chainA) th10::Chain;new(&chainB) th10::Chain;
    th10::g_Chain=&chainA;game={};g_ResultGameManager=&game;
    g_AnmGameSpeed=1;g_GameStartupCounter=0;g_RngView={};
    g_TransitionShakeX=91;g_TransitionShakeY=92;
    g_TransitionViewport={};g_TransitionViewport.x=7;g_TransitionViewport.y=9;
    g_TransitionViewport.width=123;g_TransitionViewport.height=234;
    g_TransitionViewport.minZ=.25f;g_TransitionViewport.maxZ=.75f;
    deviceA={&vtable,1};deviceB={&vtable,2};g_Direct3DDevice=&deviceA;
    g_AnmRenderManagerView=&managerA;
    allocatedEffects=freedEffects=allocatedNodes=freedNodes=0;failEffectAllocation=false;
    pendingNode=nullptr;maxDepth=0;enterDepths.clear();leaveNesting.clear();registrations.clear();
    enterHook={};leaveHook={};flushHook={};inHook=false;renderEvents.clear();lastColor=0;
    lastDrawManager=lastViewportDevice=0;
}
TransitionEffectView *make(int kind,int frames=4,unsigned int a=0x00112233,unsigned int b=0x44556677,unsigned int c=0x8899aabb,int priority=37) {
    auto *e=static_cast<TransitionEffectView *>(StageCreateTransitionEffect(priority,kind,frames,a,b,c));
    CHECK(e!=nullptr,"FACTORY_NON_NULL");return e;
}
void time(TransitionEffectView *e,int current,float subframe) {
    e->timer.current=current;e->timer.previous=current-1;e->timer.subframe=subframe;
}
int update(TransitionEffectView *e) { return e->updateNode->callback(e); }
int draw(TransitionEffectView *e) { return e->drawNode->callback(e); }
void close(TransitionEffectView *e) { DestroyTransitionEffect(e); }
void finite(float actual,float expected,const char *id) {
    CHECK(std::isfinite(actual)&&actual==expected,id);
}
void geometry(bool screen,const char *id) {
    CHECK(lastRectangle.left==(screen?0.0f:32.0f)&&lastRectangle.top==(screen?0.0f:16.0f)&&lastRectangle.right==(screen?640.0f:416.0f)&&lastRectangle.bottom==(screen?480.0f:464.0f),id);
}
}

th10::ChainElem *HostNewNode() {
    void *p=std::malloc(sizeof(th10::ChainElem));
    std::memset(p,0,sizeof(th10::ChainElem));
    auto *node=new(p) th10::ChainElem;
    node->flags=0x80u;
    host::remember(node,"node");++host::allocatedNodes;host::pendingNode=node;return node;
}
TransitionEffectView *HostNewEffect() {
    if(host::failEffectAllocation)return nullptr;
    void *p=std::malloc(sizeof(TransitionEffectView));
    std::memset(p,0xa5,sizeof(TransitionEffectView));
    auto *effect=new(p) TransitionEffectView;
    host::remember(effect,"effect");++host::allocatedEffects;return effect;
}
void HostFree(void *p) {
    if(!p)return;
    auto i=host::allocations.find(host::address(p));
    CHECK(i!=host::allocations.end()&&i->second.alive,"HOST_NO_DOUBLE_FREE");
    if(i->second.kind=="node")++host::freedNodes;else ++host::freedEffects;
    i->second.alive=false;
    if(host::pendingNode==p)host::pendingNode=nullptr;
    std::free(p);
}
void HostDeleteEffect(TransitionEffectView *effect) { effect->~TransitionEffectView();HostFree(effect); }
void EnterCriticalSection(CRITICAL_SECTION *lock) {
    lock->mutex.lock();++lock->depth;
    host::maxDepth=std::max(host::maxDepth,lock->depth);host::enterDepths.push_back(lock->depth);
    host::observeRegistration();
    if(host::enterHook&&!host::inHook){host::inHook=true;host::enterHook();host::inHook=false;}
}
void LeaveCriticalSection(CRITICAL_SECTION *lock) {
    CHECK(lock->depth>0,"HOST_LEAVE_LOCK_HELD");host::leaveNesting.push_back(th10::g_ChainNestCounter);
    if(host::leaveHook&&!host::inHook){host::inHook=true;host::leaveHook();host::inHook=false;}
    --lock->depth;lock->mutex.unlock();
}
void AnmRenderManagerView::FlushVertexBuffer() {
    host::renderEvents.push_back("flush:"+std::to_string(id));
    if(host::flushHook)host::flushHook();
}
void DrawColoredRectangle(AnmFloatRectView *rectangle,unsigned int color) {
    // Renderer body is intentionally NOT copied/reimplemented here. This seam
    // observes arguments and the known late global-manager read only.
    host::renderEvents.push_back("draw");host::lastRectangle=*rectangle;host::lastColor=color;
    host::lastDrawManager=g_AnmRenderManagerView->id;
}

namespace host {
void selector() {
    const th10::ChainCallback updates[]={TransitionFadeOut,TransitionShakeLinear,TransitionFadeIn,TransitionFadeOut,TransitionPulse,TransitionFadeIn,TransitionDim,TransitionDim,TransitionShakeEnvelope};
    const th10::ChainCallback draws[]={TransitionDrawScreen,nullptr,TransitionDrawPlayfield,TransitionDrawPlayfield,TransitionDrawPulse,TransitionDrawScreen,TransitionDrawDimScreen,TransitionDrawDimPlayfield,nullptr};
    std::set<th10::ChainCallback> actualUpdates,actualDraws;
    for(int kind=0;kind<9;++kind) {
        reset();auto *e=make(kind,23,11,22,33,71);
        CHECK(e->updateNode->callback==updates[kind],"SELECTOR_UPDATE_9");
        CHECK((e->drawNode?e->drawNode->callback:nullptr)==draws[kind],"SELECTOR_DRAW_9");
        CHECK(e->updateNode->prio==14,"SELECTOR_CALC_PRIORITY14");
        CHECK(!e->drawNode||e->drawNode->prio==71,"SELECTOR_CALLER_DRAW_PRIORITY");
        CHECK(e->updateNode->arg==e&&(e->updateNode->flags&3u)==3u,"SELECTOR_CALC_ARG_FLAGS");
        CHECK(!e->drawNode||(e->drawNode->arg==e&&(e->drawNode->flags&3u)==3u),"SELECTOR_DRAW_ARG_FLAGS");
        CHECK(e->updateNode->deletedCallback==TransitionDelete&&(!e->drawNode||!e->drawNode->deletedCallback),"SELECTOR_DELETE_ONLY_UPDATE");
        CHECK(e->flags==2&&e->unknown04==0&&e->unknown14==0&&e->released==0,"FACTORY_ZERO_FLAGS_UNKNOWN");
        CHECK(e->kind==kind&&e->frames==23&&e->value0==11&&e->value1==22&&e->value2==33,"INITIALIZE_PAYLOADS");
        CHECK(e->alpha==(kind==3?255:0),"SELECTOR_KIND3_ALPHA");
        CHECK(e->timer.current==0&&e->timer.previous==-1&&e->timer.subframe==0&&e->timer.scale==&g_AnmGameSpeed&&e->timer.flags==1,"INITIALIZE_TIMER_RESET");
        CHECK(registrations.size()==(draws[kind]?2u:1u),"INIT_REGISTRATION_COUNT");
        for(std::size_t i=0;i<registrations.size();++i) {
            const Snapshot &s=registrations[i];
            CHECK(s.flags==2&&s.unknown04==0&&s.unknown14==0,"INIT_ZERO_BEFORE_REGISTRATION");
            CHECK(s.kind==0&&s.frames==0&&s.value0==0&&s.value1==0&&s.value2==0&&s.released==0,"INIT_PAYLOAD_AFTER_REGISTRATION");
            CHECK(s.timerFlags==0&&s.timerScale==nullptr&&s.timerCurrent==0&&s.timerPrevious==0&&s.timerSubframe==0,"INIT_TIMER_AFTER_REGISTRATION");
            CHECK(s.alpha==(kind==3?255:0),"INIT_ALPHA_BEFORE_REGISTRATION");
            CHECK(s.draw==0&&s.deleted==nullptr,"INIT_DELETE_AFTER_BOTH_REGISTRATIONS");
            CHECK(i==0?s.update==0:s.update==address(e->updateNode),"INIT_IMMEDIATE_UPDATE_PUBLICATION");
        }
        actualUpdates.insert(e->updateNode->callback);if(e->drawNode)actualDraws.insert(e->drawNode->callback);
        close(e);CHECK(freedEffects==1&&freedNodes==(draws[kind]?2:1),"SELECTOR_CLEANUP_OWNER_COUNT");
    }
    CHECK(actualUpdates.size()==6&&actualDraws.size()==5,"COVERAGE_ALL11_DISTINCT_CALLBACKS");
    reset();failEffectAllocation=true;
    CHECK(StageCreateTransitionEffect(1,5,4,0,0,0)==nullptr,"FACTORY_NULL_ALLOCATION");
    CHECK(allocatedEffects==0&&allocatedNodes==0&&registrations.empty(),"FACTORY_NULL_NO_REGISTRATION");
    DestroyTransitionEffect(nullptr);CHECK(TransitionDelete(nullptr)==0,"DELETE_NULL_SAFE");
}
void fades() {
    reset();auto *e=make(0,4);time(e,1,1.5f);
    CHECK(update(e)==1&&e->alpha==159&&e->timer.current==2&&e->timer.subframe==2.5f,"FADE_OUT_FRACTIONAL_TRUNC");
    time(e,4,4);CHECK(update(e)==7&&e->alpha==0&&e->timer.current==4,"FADE_OUT_FRAME_BOUNDARY");
    time(e,2,8);CHECK(update(e)==1&&e->alpha==0,"FADE_OUT_NEGATIVE_CLAMP");
    e->frames=0;e->alpha=73;time(e,0,0);CHECK(update(e)==7&&e->alpha==73,"FADE_OUT_ZERO_PRESERVES_ALPHA");
    e->frames=-2;time(e,-3,-1);CHECK(update(e)==1&&e->alpha==127,"FADE_OUT_NEGATIVE_DURATION");
    g_GameStartupCounter=1;e->alpha=63;time(e,0,0);CHECK(update(e)==7&&e->alpha==63&&e->timer.current==0,"FADE_OUT_ABORT_BEFORE_WRITES");close(e);
    reset();e=make(5,4);time(e,1,1.5f);game.flags58=1;
    CHECK(update(e)==1&&e->alpha==95&&e->timer.current==1,"FADE_IN_PAUSED_STILL_ALPHA");
    for(unsigned int flag:{1u,4u,5u}){game.flags58=flag;time(e,1,1);CHECK(update(e)==1&&e->timer.current==1,"FADE_IN_PAUSE_MASK5");}
    game.flags58=2;time(e,1,1);CHECK(update(e)==1&&e->timer.current==2,"FADE_IN_OTHER_FLAGS_TICK");
    g_ResultGameManager=nullptr;time(e,1,1);CHECK(update(e)==1&&e->timer.current==2,"FADE_IN_NULL_GAME_TICKS");
    time(e,4,4);CHECK(update(e)==1&&e->alpha==255&&e->timer.current==5,"FADE_IN_TWO_EXTRA_FIRST");
    CHECK(update(e)==1&&e->timer.current==6,"FADE_IN_TWO_EXTRA_SECOND");
    CHECK(update(e)==7&&e->timer.current==6,"FADE_IN_TWO_EXTRA_TERMINAL");
    time(e,0,-1);CHECK(update(e)==1&&e->alpha==0,"FADE_IN_NEGATIVE_CLAMP");
    time(e,0,8);CHECK(update(e)==1&&e->alpha==510,"FADE_IN_NO_HIGH_CLAMP");
    e->frames=0;e->alpha=73;time(e,0,0);CHECK(update(e)==1&&e->alpha==73,"FADE_IN_ZERO_PRESERVES_ALPHA");
    CHECK(update(e)==1&&update(e)==7,"FADE_IN_ZERO_TWO_TICKS");
    e->frames=INT_MAX;time(e,0,0);CHECK(update(e)==7,"FADE_IN_END_WRAPS_SIGNED");
    g_GameStartupCounter=-1;e->alpha=81;time(e,0,0);CHECK(update(e)==7&&e->alpha==81&&e->timer.current==0,"FADE_IN_ABORT_BEFORE_WRITES");close(e);
}
void dim() {
    for(int mode:{6,7}) {
        reset();auto *e=make(mode,4);g_GameStartupCounter=1;game.flags58=0x77;
        time(e,2,2);CHECK(update(e)==1&&e->alpha==64&&e->timer.current==3,"DIM_IGNORES_ABORT_PAUSE");
        time(e,4,4);CHECK(update(e)==1&&e->alpha==128,"DIM_INCLUSIVE_END");
        time(e,900,900);CHECK(update(e)==1&&e->alpha==128&&e->timer.current==901,"DIM_PERSISTS_UNRELEASED");
        e->frames=0;e->alpha=19;CHECK(update(e)==1&&e->alpha==19,"DIM_ZERO_PRESERVES_ALPHA");
        e->Release();CHECK(e->released==1&&e->timer.current==0&&e->timer.previous==-1&&e->timer.subframe==0,"DIM_RELEASE_TIMER_RESET");
        CHECK(update(e)==1&&e->alpha==128,"DIM_RELEASE_INITIAL_ALPHA");
        time(e,7,7.5f);CHECK(update(e)==1&&e->alpha==8,"DIM_RELEASE_FRACTIONAL");
        time(e,8,8);CHECK(update(e)==1&&e->alpha==0&&e->timer.current==9,"DIM_RELEASE_FRAME8_ACTIVE");
        CHECK(update(e)==7&&e->timer.current==9,"DIM_RELEASE_FRAME9_TERMINAL");close(e);
    }
}
void pulse() {
    reset();auto *e=make(4,4,2,0x80654321);time(e,1,1.5f);
    CHECK(update(e)==1&&e->alpha==80&&e->timer.current==2,"PULSE_HIGH_BYTE_ALPHA_TRUNC");
    time(e,4,4);CHECK(update(e)==1&&e->value0==1&&e->alpha==0&&e->timer.current==1&&e->timer.previous==0,"PULSE_REPEAT_RESET_THEN_TICK");
    time(e,4,4);CHECK(update(e)==0&&e->value0==0&&e->alpha==0&&e->timer.current==4,"PULSE_FINISH_RETURNS0");
    e->value0=0;CHECK(update(e)==0&&e->value0==0xffffffffu,"PULSE_COUNT_UNDERFLOW_SIGNED");
    e->value0=0x80000001u;CHECK(update(e)==0&&e->value0==0x80000000u,"PULSE_COUNT_HIGH_BIT_SIGNED");
    e->frames=0;e->value0=2;time(e,0,0);CHECK(update(e)==1&&e->value0==1&&e->timer.current==1,"PULSE_ZERO_PERIOD_REPEAT");
    g_GameStartupCounter=1;e->alpha=71;e->value0=9;time(e,0,0);CHECK(update(e)==0&&e->alpha==71&&e->value0==9&&e->timer.current==0,"PULSE_ABORT_RETURNS0_UNCHANGED");close(e);
}
void draw_callbacks() {
    for(int mode:{0,2,3,4,5,6,7}) {
        reset();auto *e=make(mode,4,0x80112233u,0xf0654321u);e->alpha=0x12;
        flushHook=[](){g_AnmRenderManagerView=&managerB;g_Direct3DDevice=&deviceB;};
        CHECK(draw(e)==1,"DRAW_RETURN1");geometry(mode==0||mode==5||mode==6,"DRAW_GEOMETRY_ALL5");
        CHECK(lastColor==(mode==4?0x12654321u:0x92112233u),"DRAW_COLOR_OR_VERSUS_MASK");
        if(mode==0||mode==5) {
            CHECK(renderEvents==std::vector<std::string>({"flush:1","viewport","draw"}),"DRAW_SCREEN_EVENT_ORDER");
            CHECK(lastViewport.x==0&&lastViewport.y==0&&lastViewport.width==640&&lastViewport.height==480,"DRAW_SCREEN_VIEWPORT_RESET");
            CHECK(lastViewport.minZ==.25f&&lastViewport.maxZ==.75f,"DRAW_SCREEN_DEPTH_PRESERVED");
            CHECK(lastViewportDevice==2&&lastDrawManager==2,"DRAW_SCREEN_GLOBAL_RELOAD");
        } else {
            CHECK(renderEvents==std::vector<std::string>({"draw"}),"DRAW_NONSCREEN_NO_RESET_OR_FLUSH");
            CHECK(g_TransitionViewport.x==7&&g_TransitionViewport.width==123,"DRAW_NONSCREEN_VIEWPORT_UNCHANGED");
        }
        close(e);
    }
}
void linear() {
    reset();auto *e=make(1,4,8,16);g_RngView.seed=1;
    CHECK(update(e)==1&&e->timer.current==1,"LINEAR_TICK_BEFORE_ENVELOPE");
    CHECK(g_RngView.generationCount==4&&g_RngView.seed==31808,"LINEAR_RNG_FOUR_ADVANCES");
    CHECK(g_TransitionShakeX==-10&&g_TransitionShakeY==10,"LINEAR_DUPLICATED_SECOND_HALVES");
    g_RngView.seed=7;g_RngView.generationCount=0;time(e,1,1);
    CHECK(update(e)==1&&g_TransitionShakeX==12&&g_TransitionShakeY==-12,"LINEAR_SECOND_AXIS_DUPLICATED");
    g_RngView.seed=11;time(e,0,0);CHECK(update(e)==1&&g_TransitionShakeX==-10&&g_TransitionShakeY==-10,"LINEAR_SECOND_AXIS_SEED11");
    g_RngView.seed=0;time(e,0,0);CHECK(update(e)==1&&g_TransitionShakeX==0&&g_TransitionShakeY==0,"LINEAR_ZERO_BRANCHES");
    auto gen=g_RngView.generationCount;g_TransitionShakeX=91;g_TransitionShakeY=92;time(e,3,3);
    CHECK(update(e)==7&&e->timer.current==4&&g_RngView.generationCount==gen&&g_TransitionShakeX==91&&g_TransitionShakeY==92,"LINEAR_TERMINAL_NO_RNG_NO_ZERO");
    g_GameStartupCounter=1;time(e,0,0);CHECK(update(e)==7&&e->timer.current==0&&g_RngView.generationCount==gen,"LINEAR_ABORT_NO_TICK");
    g_GameStartupCounter=0;e->frames=0;CHECK(update(e)==7&&e->timer.current==1,"LINEAR_ZERO_DURATION_NO_DIVISION");
    e->frames=4;e->value0=0xfffffff8u;e->value1=0xfffffff0u;time(e,0,0);g_RngView.seed=1;
    CHECK(update(e)==1&&g_TransitionShakeX==10&&g_TransitionShakeY==-10,"LINEAR_SIGNED_PAYLOADS");close(e);
}
void envelope() {
    reset();auto *e=make(8,12,4,2,4);
    for(unsigned int flag:{1u,2u,4u,0x10u,0x20u,0x40u,0x77u}) {
        game.flags58=flag;time(e,0,0);g_RngView.seed=1;g_RngView.generationCount=0;
        CHECK(update(e)==1&&e->timer.current==0&&g_RngView.generationCount==0&&g_TransitionShakeX==91&&g_TransitionShakeY==92,"ENVELOPE_PAUSE_MASK77");
    }
    g_ResultGameManager=nullptr;CHECK(update(e)==1&&e->timer.current==0,"ENVELOPE_NULL_GAME_PAUSES");
    g_ResultGameManager=&game;game.flags58=8;g_RngView.seed=1;
    CHECK(update(e)==1&&e->timer.current==1&&g_TransitionShakeX==3&&g_TransitionShakeY==3,"ENVELOPE_ATTACK_AND_STANDARD_RNG");
    CHECK(g_RngView.generationCount==4&&g_RngView.seed==31808,"ENVELOPE_TWO_TRUE_RANGE_CALLS");
    game.flags58=0;time(e,3,3);g_RngView.seed=1;CHECK(update(e)==1&&g_TransitionShakeX==12,"ENVELOPE_ATTACK_BOUNDARY_HOLD");
    time(e,5,5);g_RngView.seed=1;CHECK(update(e)==1&&g_TransitionShakeX==12,"ENVELOPE_HOLD_BOUNDARY_DECAY");
    time(e,7,7);g_RngView.seed=1;CHECK(update(e)==1&&g_TransitionShakeX==6,"ENVELOPE_DECAY_INTERIOR");
    time(e,8,8);g_RngView.seed=1;CHECK(update(e)==1&&g_TransitionShakeX==3,"ENVELOPE_LAST_ACTIVE_FRAME");
    const auto gen=g_RngView.generationCount;g_TransitionShakeX=91;g_TransitionShakeY=92;
    CHECK(update(e)==7&&e->timer.current==10&&g_RngView.generationCount==gen&&g_TransitionShakeX==91&&g_TransitionShakeY==92,"ENVELOPE_TERMINAL_RETAINS_OUTPUT");
    g_GameStartupCounter=1;g_ResultGameManager=nullptr;time(e,0,0);CHECK(update(e)==7&&e->timer.current==0&&g_RngView.generationCount==gen,"ENVELOPE_ABORT_PRECEDES_NULL_PAUSE");
    g_GameStartupCounter=0;g_ResultGameManager=&game;e->value0=0;e->value1=0;e->value2=0;time(e,0,0);
    CHECK(update(e)==7&&g_RngView.generationCount==gen,"ENVELOPE_ZERO_PHASES_TERMINAL");
    e->frames=10;e->value0=0xffffffffu;e->value1=10;e->value2=10;time(e,0,0);g_RngView.seed=1;
    CHECK(update(e)==1&&g_TransitionShakeX==10,"ENVELOPE_SIGNED_ATTACK_COMPARE");
    e->value0=0x80000000u;e->value1=0;e->value2=0xffffffffu;time(e,0,0);g_RngView.seed=1;
    CHECK(update(e)==1,"ENVELOPE_UNSIGNED_DECAY_ACTIVE");finite(g_TransitionShakeX,5,"ENVELOPE_UNSIGNED_DECAY_DIVISOR");
    e->value0=0x80000000u;e->value1=0;e->value2=0x40000000u;time(e,-1610612737,-1610612736.0f);g_RngView.seed=1;
    CHECK(update(e)==1,"ENVELOPE_SIGNED_SUM_COMPARE");finite(g_TransitionShakeX,45,"ENVELOPE_UNSIGNED_DECAY_TOTAL");
    // Finite representable quarter-step inputs only are exact oracles here.
    // Nonrepresentable x87 intermediate rounding is intentionally excluded.
    e->frames=-12;e->value0=4;e->value1=2;e->value2=4;time(e,0,0);g_RngView.seed=1;
    CHECK(update(e)==1&&g_TransitionShakeX==-3&&std::isfinite(g_TransitionShakeY),"ENVELOPE_SIGNED_AMPLITUDE_FINITE");close(e);
}
void source_order() {
    const std::string source=host_proposed_source;
    const auto initializer=source.find("void TransitionEffectView::Initialize(");
    const auto frames=source.find("frames = duration;",initializer),kind=source.find("kind = effectKind;",initializer);
    CHECK(initializer!=std::string::npos&&frames<kind,"INIT_FINAL_FRAMES_BEFORE_KIND");
}
void known_precision_gap() {
    reset();auto *e=make(8,10,3,2,4);g_RngView.seed=1;
    CHECK(update(e)==1,"PRECISION_GAP_ACTIVE");
    const float targetFinal=static_cast<float>(10.0L/3.0L);
    const float strictHost=3.333333492279052734375f;
    CHECK(targetFinal==3.3333332538604736328125f,"PRECISION_GAP_ORACLE");
    CHECK(g_TransitionShakeX==strictHost&&g_TransitionShakeX!=targetFinal,"KNOWN_X87_PRECISION_GAP_RETAINED");
    std::cout<<"KNOWN_GAP:kind8 attack 1/3 * 10 target_final=3.3333332538604736 host=3.3333334922790527\n";
    close(e);
}
void timer_body() {
    reset();auto *e=make(6,4);g_AnmGameSpeed=.5f;time(e,1,1.25f);
    CHECK(update(e)==1&&e->timer.previous==1&&e->timer.current==1&&e->timer.subframe==1.75f,"TIMER_FRACTIONAL_CANONICAL_BODY");
    CHECK(update(e)==1&&e->timer.current==2&&e->timer.subframe==2.25f,"TIMER_FRACTIONAL_ACCUMULATION");
    g_AnmGameSpeed=1.005f;time(e,4,4.5f);
    CHECK(update(e)==1&&e->timer.current==5&&e->timer.subframe==5.5f,"TIMER_UNIT_EPSILON_FAST_BRANCH");
    g_AnmGameSpeed=.99f;time(e,4,4);CHECK(update(e)==1&&e->timer.current==4,"TIMER_EPSILON_STRICT_BOUNDARY");
    e->timer.flags=0;e->timer.scale=nullptr;e->Release();CHECK(e->timer.scale==&g_AnmGameSpeed&&(e->timer.flags&1),"TIMER_RELEASE_LAZY_INIT");close(e);
}

// The complete maintained dispatchers now drive these lifetime scenarios.
int sentinelVisits=0,deleteCalls=0,deleteDepth=0;
int sentinel(void *) { CHECK(th10::g_CriticalSections[0].depth==0,"DISPATCH_CALLBACK_UNLOCKED");++sentinelVisits;return 1; }
int deletedSpy(void *arg) { ++deleteCalls;deleteDepth=th10::g_CriticalSections[0].depth;return TransitionDelete(arg); }

void lifetime7() {
    reset();auto *e=make(0,0);auto owner=address(e),updateAddr=address(e->updateNode),drawAddr=address(e->drawNode);
    e->updateNode->deletedCallback=deletedSpy;deleteCalls=deleteDepth=sentinelVisits=0;
    auto *next=th10::Chain::RegisterCalc(nullptr,15,sentinel);pendingNode=nullptr;maxDepth=0;enterDepths.clear();leaveNesting.clear();
    CHECK(MainRunCalcChain(th10::g_Chain)==2,"RET7_ACTUAL_DISPATCH_COUNT");
    CHECK(deleteCalls==1&&deleteDepth==1,"RET7_DELETE_CALLBACK_UNDER_LOCK");
    CHECK(maxDepth==2&&std::count(enterDepths.begin(),enterDepths.end(),2)==2,"RET7_TWO_RECURSIVE_CLEANUP_LOCKS");
    CHECK(sentinelVisits==1,"RET7_CACHED_NEXT_SURVIVES_FREE");
    CHECK(!alive(owner)&&!alive(updateAddr)&&!alive(drawAddr)&&freedEffects==1&&freedNodes==2,"RET7_FREES_OWNER_BOTH_NODES_ONCE");
    CHECK(th10::g_ChainNestCounter==0&&th10::g_CriticalSections[0].depth==0,"RET7_LOCK_NESTING_BALANCED");
    th10::Chain::UnregisterElem(next,th10::g_Chain);
}
void lifetime0() {
    reset();auto *e=make(4,0,1,0x80112233);auto owner=address(e),updateAddr=address(e->updateNode),drawAddr=address(e->drawNode);
    e->updateNode->deletedCallback=deletedSpy;deleteCalls=0;
    CHECK(MainRunCalcChain(th10::g_Chain)==1,"RET0_ACTUAL_DISPATCH_COUNT");
    CHECK(deleteCalls==0&&freedEffects==0&&freedNodes==1,"RET0_DOES_NOT_DELETE_OWNER");
    CHECK(alive(owner)&&!alive(updateAddr)&&alive(drawAddr),"RET0_OWNER_AND_DRAW_SURVIVE");
    // Observe only the pointer value in the LIVE owner. Never dereference the
    // freed update node and never run production Cleanup on this stale state.
    CHECK(address(e->updateNode)==updateAddr&&chainA.CalcList()->next==nullptr&&chainA.DrawList()->next!=nullptr,"RET0_STORED_UPDATE_POINTER_STALE");
    CHECK(draw(e)==1,"RET0_DRAW_STILL_CALLABLE");
    th10::Chain::UnregisterElem(e->drawNode,&chainA);HostDeleteEffect(e);
}
void cleanup_reload() {
    reset();auto *captured=make(5);const auto capturedUpdate=address(captured->updateNode);
    int enters=0,leaves=0;
    enterHook=[&](){if(enters++==0)th10::g_Chain=&chainB;};
    leaveHook=[&](){if(leaves++==0)th10::g_Chain=&chainA;};
    captured->Cleanup();enterHook={};leaveHook={};
    CHECK(!alive(capturedUpdate),"CLEANUP_CAPTURES_CHAIN_BEFORE_ENTER");
    CHECK(freedNodes==2,"CLEANUP_CAPTURE_COMPLETES_BOTH_NODES");HostDeleteEffect(captured);
    reset();auto *e=make(5);auto *oldDraw=e->drawNode;
    // Detach draw without freeing it, then publish it on a different chain.
    auto *n=reinterpret_cast<th10::ListNode *>(&oldDraw->self);
    chainA.DrawList()->next=nullptr;n->prev=nullptr;n->next=nullptr;
    th10::Chain::AddToDrawChain(oldDraw,37,&chainB);
    const auto updateAddr=address(e->updateNode),oldAddr=address(oldDraw);
    int hookCount=0;
    leaveHook=[&](){ if(!alive(updateAddr)&&hookCount++==0)th10::g_Chain=&chainB; };
    e->Cleanup();leaveHook={};
    CHECK(!alive(updateAddr)&&!alive(oldAddr),"CLEANUP_RELOADS_GLOBAL_CHAIN");
    CHECK(address(e->updateNode)==updateAddr&&address(e->drawNode)==oldAddr,"CLEANUP_DOES_NOT_CLEAR_POINTERS");
    CHECK(th10::g_ChainNestCounter==0,"CLEANUP_TWO_LOCK_CYCLES_BALANCED");HostDeleteEffect(e);
    reset();e=make(5);oldDraw=e->drawNode;
    auto *replacement=th10::Chain::RegisterDraw(e,50,TransitionDrawDimScreen);pendingNode=nullptr;
    const auto firstAddr=address(e->updateNode),replacementAddr=address(replacement);int changes=0;
    leaveHook=[&](){if(!alive(firstAddr)&&changes++==0)e->drawNode=replacement;};
    e->Cleanup();leaveHook={};
    CHECK(!alive(firstAddr)&&!alive(replacementAddr)&&alive(address(oldDraw)),"CLEANUP_RELOADS_DRAW_POINTER");
    th10::Chain::UnregisterElem(oldDraw,&chainA);HostDeleteEffect(e);
}
// Actual constructors on explicitly prefilled host storage. This checks field
// effects and retained bytes, not target pointer width or WinMain heap state.
void chain_heads() {
    for(unsigned char pattern:{static_cast<unsigned char>(0xa5),static_cast<unsigned char>(0x5a)}) {
        alignas(th10::Chain) unsigned char storage[sizeof(th10::Chain)];
        std::memset(storage,pattern,sizeof(storage));
        auto *chain=new(storage) th10::Chain;
        const unsigned int flags=pattern==0xa5?0xa5a5a5a4u:0x5a5a5a5au;
        for(auto *head:{&chain->calcHead,&chain->drawHead}) {
            unsigned int observedFlags;std::memcpy(&observedFlags,&head->flags,sizeof(observedFlags));
            CHECK(observedFlags==flags,"CHAIN_CTOR_OWNERSHIP_BIT_ONLY");
            CHECK(head->prio==0&&head->callback==nullptr&&head->addedCallback==nullptr&&head->deletedCallback==nullptr,"CHAIN_CTOR_PRIORITY_CALLBACKS");
            CHECK(head->self==head&&head->next==nullptr&&head->prev==nullptr,"CHAIN_CTOR_LIVE_SELF_AND_LINKS");
            unsigned char expected[sizeof(head->arg)];std::memset(expected,pattern,sizeof(expected));
            CHECK(std::memcmp(&head->arg,expected,sizeof(expected))==0,"CHAIN_CTOR_ARGUMENT_BYTES_RETAINED");
        }
        chain->~Chain();
    }
}

struct DispatchState {
    th10::ChainElem *elem=nullptr,*insert=nullptr;
    int first=1,later=1,calls=0,deleted=0;
    bool disable=false,draw=false;
};
void registerDispatch(th10::ChainElem *elem,int priority,bool draw) {
    if(draw)th10::Chain::AddToDrawChain(elem,priority,&chainA);
    else th10::Chain::AddToCalcChain(elem,priority,&chainA);
}
int dispatchCallback(void *arg) {
    auto *state=static_cast<DispatchState *>(arg);
    CHECK(th10::g_CriticalSections[0].depth==0&&th10::g_ChainNestCounter==0,"CHAIN_CALLBACK_UNLOCKED");
    ++state->calls;
    if(state->disable)state->elem->flags&=~2u;
    if(state->calls==1&&state->insert)registerDispatch(state->insert,2,state->draw);
    return state->calls==1?state->first:state->later;
}
int dispatchDeleted(void *arg) {
    auto *state=static_cast<DispatchState *>(arg);
    CHECK(th10::g_CriticalSections[0].depth==1&&th10::g_ChainNestCounter==1,"CHAIN_DELETE_CALLBACK_LOCKED");
    ++state->deleted;
    return 1;
}
void configureDispatch(th10::ChainElem &elem,DispatchState &state) {
    state.elem=&elem;elem.flags=2;elem.arg=&state;
    elem.callback=dispatchCallback;elem.deletedCallback=dispatchDeleted;
}
void dispatch_statuses() {
    reset();
    CHECK(MainRunCalcChain(&chainA)==0&&MainRunDrawChain(&chainA)==0,"CHAIN_EMPTY_COUNTS");
    for(bool draw:{false,true}) {
        auto run=[&](){return draw?MainRunDrawChain(&chainA):MainRunCalcChain(&chainA);};
        for(int status:{0,1,2,3,4,5,6,7,8,-1,INT_MIN,INT_MAX}) {
            reset();th10::ChainElem a,b;DispatchState first,last;
            configureDispatch(a,first);configureDispatch(b,last);
            first.first=status;first.draw=last.draw=draw;
            registerDispatch(&a,1,draw);registerDispatch(&b,3,draw);
            const int expected=status==3?1:status==4?0:status==5?-1:2;
            CHECK(run()==expected,"CHAIN_STATUS_RESULT");
            CHECK(first.calls==(status==2||(!draw&&status==6)?2:1),"CHAIN_RETRY_RESTART_VISITS");
            CHECK(last.calls==((status>=3&&status<=5)?0:1),"CHAIN_EARLY_EXIT_SKIPS_TAIL");
            CHECK(first.deleted==(!draw&&status==7?1:0),"CHAIN_CALC7_DRAW_DEFAULT_DELETE");
            CHECK((a.callback==nullptr)==(status==0),"CHAIN_STATUS0_UNREGISTERS_ONLY");
            CHECK(th10::g_CriticalSections[0].depth==0&&th10::g_ChainNestCounter==0,"CHAIN_STATUS_BALANCED");
            th10::Chain::UnregisterElem(&a,&chainA);th10::Chain::UnregisterElem(&b,&chainA);
        }
        for(int mode:{0,1,2}) {
            reset();th10::ChainElem a,b;DispatchState first,last;
            configureDispatch(a,first);configureDispatch(b,last);
            if(mode==0)a.callback=nullptr;
            if(mode==1)a.flags=0;
            if(mode==2){first.first=2;first.disable=true;}
            registerDispatch(&a,1,draw);registerDispatch(&b,3,draw);
            CHECK(run()==(mode==0?1:2),"CHAIN_NULL_DISABLED_COUNTS");
            CHECK(first.calls==(mode==2?1:0),"CHAIN_ENABLED_AND_RETRY_RECHECK");
            CHECK(last.calls==1,"CHAIN_DISABLED_RETAINS_NEXT");
            th10::Chain::UnregisterElem(&a,&chainA);th10::Chain::UnregisterElem(&b,&chainA);
        }
        for(int status:{3,4,5}) {
            reset();th10::ChainElem a,b,c,d;DispatchState sa,sb,sc,sd;
            configureDispatch(a,sa);configureDispatch(b,sb);configureDispatch(c,sc);configureDispatch(d,sd);sc.first=status;
            registerDispatch(&a,1,draw);registerDispatch(&b,2,draw);registerDispatch(&c,3,draw);registerDispatch(&d,4,draw);
            CHECK(run()==(status==3?1:status==4?0:-1),"CHAIN_FORCED_RESULT_REPLACES_COUNT");
            CHECK(sa.calls==1&&sb.calls==1&&sc.calls==1&&sd.calls==0,"CHAIN_FORCED_RESULT_VISITS");
            for(auto *node:{&a,&b,&c,&d})th10::Chain::UnregisterElem(node,&chainA);
        }
        reset();th10::ChainElem a,b,c;DispatchState sa,sb,sc;
        configureDispatch(a,sa);configureDispatch(b,sb);configureDispatch(c,sc);
        sa.insert=&c;sa.draw=draw;
        registerDispatch(&a,1,draw);registerDispatch(&b,3,draw);
        CHECK(run()==2&&sa.calls==1&&sb.calls==1&&sc.calls==0,"CHAIN_CACHED_NEXT_SKIPS_NEW_NODE");
        CHECK(run()==3&&sa.calls==2&&sb.calls==2&&sc.calls==1,"CHAIN_INSERTED_NODE_NEXT_PASS");
        for(auto *node:{&a,&b,&c})th10::Chain::UnregisterElem(node,&chainA);
    }
}

void unregister_no_delete() {
    reset();auto *e=make(5);deleteCalls=0;e->updateNode->deletedCallback=deletedSpy;
    auto *u=e->updateNode,*d=e->drawNode;th10::Chain::UnregisterElem(u,&chainA);
    CHECK(deleteCalls==0&&freedEffects==0,"UNREGISTER_NEVER_DELETED_CALLBACK");
    th10::Chain::UnregisterElem(d,&chainA);HostDeleteEffect(e);
    reset();e=make(5);deleteCalls=0;e->updateNode->deletedCallback=deletedSpy;
    th10::Chain::RemoveAllFromList(reinterpret_cast<unsigned char *>(chainA.CalcList()),&chainA);
    CHECK(deleteCalls==0&&freedEffects==0&&chainA.CalcList()->next==nullptr,"REMOVE_ALL_NEVER_DELETED_CALLBACK");
    th10::Chain::RemoveAllFromList(reinterpret_cast<unsigned char *>(chainA.DrawList()),&chainA);
    HostDeleteEffect(e);
}
}
int main(int argc,char **argv) {
    const std::string which=argc>1?argv[1]:"all";
    const std::vector<std::pair<std::string,std::function<void()>>> cases={
        {"dispatch_statuses",host::dispatch_statuses},{"chain_heads",host::chain_heads},{"source_order",host::source_order},{"precision_gap",host::known_precision_gap},{"selector",host::selector},{"fades",host::fades},{"dim",host::dim},{"pulse",host::pulse},
        {"draw",host::draw_callbacks},{"linear",host::linear},{"envelope",host::envelope},{"timer",host::timer_body},
        {"lifetime7",host::lifetime7},{"lifetime0",host::lifetime0},{"cleanup",host::cleanup_reload},{"unregister",host::unregister_no_delete}};
    try {
        bool found=false;
        for(const auto &test:cases)if(which=="all"||which==test.first){found=true;test.second();host::reset();std::cout<<"PASS:"<<test.first<<"\n";}
        if(!found){std::cerr<<"UNKNOWN_SCENARIO:"<<which<<"\n";return 64;}
        std::cout<<"PASS:ALL_REQUESTED assertions="<<host::assertions<<"\n";return 0;
    } catch(const std::runtime_error &e) {std::cerr<<e.what()<<"\n";return 86;}
}
