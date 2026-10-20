#define __stdcall
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#include "EnemyLaserLine.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <limits>
#include <string>
#include <vector>
struct Failure{const char *label;};
#define CHECK(x,label) do{if(!(x))throw Failure{label};}while(0)
float g_AnmGameSpeed=1;static float privateSpeed=2;
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
AnmLoadedView::AnmLoadedView(){std::memset(this,0,sizeof(*this));}
Player::Player(){std::memset(this,0,sizeof(*this));}
Player::~Player(){}
PlayerDrawVmView::~PlayerDrawVmView(){}
static Player player;Player *g_Player=&player;
static EnemyLaserLineView line;
static EnemyLaserType0RequestView input;
static AnmLoadedView resourceA,resourceB;
static EnemyLaserManagerStorageView managerA,managerB;
EnemyLaserManagerView *g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&managerA);
static EnemyPrimaryResourceOwnerView owner;EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner=&owner;
static EnemySoundQueueView soundQueue;unsigned char *g_MainSoundOwner=reinterpret_cast<unsigned char*>(&soundQueue);
struct EnemySoundCueMetadataView{int unknown000;unsigned short unknown004;short cueValue;};
EnemySoundCueMetadataView g_EnemySoundCueMetadata[134];
int g_BulletSpriteScriptBase[8]={10,30,100,400,500,600,700,800};
static std::vector<std::string> events;static std::function<void(const std::string&)> after;
struct Bind{AnmLoadedView *resource;AnmVmView *vm;int script;};static std::vector<Bind> binds;
struct Polar{float angle,magnitude,z;};static std::vector<Polar> polars;
static std::string which(AnmVmView *v){CHECK(v==&line.primaryVm||v==&line.secondaryVm,"vm-receiver");return v==&line.primaryVm?"primary":"secondary";}
static void event(const std::string&s){events.push_back(s);if(after)after(s);}
void AnmVmView::Initialize(){auto slot=which(this);CHECK(line.unknown00C==2,"state-before-vm");flags35C=0xffffffffu;pendingInterrupt=51;events.push_back("reset:"+slot);/* HOST pure entry observation; no callbacks. */}
void AnmLoadedView::InitializeAndExecuteScriptIndex(AnmVmView *vm,int script){binds.push_back({this,vm,script});event("bind:"+which(vm));}
int AnmRenderManagerView::ExecuteScript(AnmVmView *vm){CHECK(vm->pendingInterrupt==2,"pending-before-execute");event("exec:"+which(vm));return -1;}
void EnemyLaserVectorView::FromAngleMagnitude(float a,float n){polars.push_back({a,n,z});x=std::cos(a)*n;y=std::sin(a)*n;/* HOST role: observation only, no callbacks or Z store. */}
#define UNUSED_VOID(name) void EnemyLaserLineView::name(){throw Failure{"unexpected-slot"};}
UNUSED_VOID(UnknownSlot10) UNUSED_VOID(UnknownSlot14) UNUSED_VOID(UnknownSlot1C) UNUSED_VOID(UnknownSlot20)
UNUSED_VOID(Motion0001) UNUSED_VOID(Motion0010) UNUSED_VOID(Motion0020) UNUSED_VOID(Motion0100) UNUSED_VOID(Motion0080)
UNUSED_VOID(Bounce) UNUSED_VOID(Motion100000) UNUSED_VOID(Motion200000) UNUSED_VOID(Motion4000000)
int EnemyLaserLineView::Update(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::Draw(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::CheckCollisionBox(EnemyLaserVectorView*,EnemyLaserVectorView*,int){throw Failure{"unexpected-slot"};}
#include "helpers.inc"
#include "owner.inc"
static bool near(float a,float b){return std::abs(a-b)<.0001f;}
static void reset(){
 after={};events.clear();binds.clear();polars.clear();g_AnmGameSpeed=1;line.unknown00C=77;line.id=88;line.flags=0x400;line.commandIndex=7;line.soundId=-1;line.position={-1,-2,-3};line.velocity={1,2,19};line.angle=1;line.growSpeed=4;line.length=11;line.width=71;line.traveled=3;
 line.holdTimer.flags=0x80;line.holdTimer.scale=&privateSpeed;line.holdTimer.current=-4;line.holdTimer.previous=99;line.holdTimer.subframe=8;
 std::memset(&input,0,sizeof(input));input.position={12,34,56};input.angle=.5;input.maximumLength=333;input.initialLength=11;input.terminalDistance=444;input.width=6;input.speed=5;input.type=2;input.color=7;input.flags=1;for(unsigned i=0;i<108;++i)input.patternWords[i]=0xabc000+i;
 std::memset(&line.request,0,sizeof(line.request));for(auto &p:line.patterns){p.timer.current=0;p.timer.previous=-1;p.timer.subframe=0;p.timer.flags=0x80;p.timer.scale=&privateSpeed;for(auto &w:p.values)w.bits=0;}
 managerA.effectResource=&resourceA;managerB.effectResource=&resourceB;g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&managerA);owner.primaryEnemyResource=&resourceA;g_EnemyPrimaryResourceOwner=&owner;g_MainSoundOwner=reinterpret_cast<unsigned char*>(&soundQueue);std::memset(&soundQueue,0,sizeof(soundQueue));std::fill(std::begin(soundQueue.activeSoundIds),std::end(soundQueue.activeSoundIds),-1);for(int i=0;i<134;++i)g_EnemySoundCueMetadata[i].cueValue=short(i+100);
}
static void init(){CHECK(line.Initialize(&input)==0,"initialize-return");}
static void initCases(){
 reset();init();CHECK(std::memcmp(&line.request,&input,sizeof(input))==0,"complete-request-copy");
 CHECK((events==std::vector<std::string>{"reset:primary","bind:primary","exec:primary","reset:secondary","bind:secondary","exec:secondary"}),"vm-step-order");CHECK(binds.size()==2&&binds[0].resource==&resourceA&&binds[1].resource==&resourceA&&binds[0].script==107&&binds[1].script==266,"signed-script-selection");
 CHECK(line.primaryVm.flags35C==((0xffffffdfu|0x10u)&0xfc63ffffu|0x600000u),"primary-flag-mask");CHECK(line.secondaryVm.flags35C==((0xffffffdfu|0x10u)&0xfc7fffffu|0x400000u),"secondary-flag-mask");
 CHECK(line.holdTimer.current==30&&line.holdTimer.previous==29&&line.holdTimer.subframe==30&&line.holdTimer.flags==0x81&&line.holdTimer.scale==&g_AnmGameSpeed,"startup-timer");CHECK(line.position.x==12&&line.position.y==34&&line.position.z==56&&line.length==11&&line.width==6&&line.growSpeed==5&&line.soundId==24&&line.traveled==.01f,"geometry-slots");CHECK(line.velocity.z==19&&polars.size()==1&&polars[0].z==19&&near(line.velocity.x,std::cos(.5f)*5)&&near(line.velocity.y,std::sin(.5f)*5),"polar-z-preserved");CHECK(line.flags==0x400&&line.commandIndex==7&&line.id==88,"preserved-existing-state");
 reset();input.flags=0;init();CHECK((line.primaryVm.flags35C&0x20)!=0,"primary-gated-bit");CHECK((line.secondaryVm.flags35C&0x30)==0x10,"secondary-unconditional-bit");
 reset();input.color=-2;init();CHECK(binds[0].script==98&&binds[1].script==257,"signed-script-selection");
 reset();line.holdTimer.flags=0x81;init();CHECK(line.holdTimer.scale==&privateSpeed,"startup-retains-scale");
 reset();input.flags=0;after=[](const std::string&s){if(s=="bind:primary"){g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&managerB);input.position={-10,-20,-30};input.color=6;}if(s=="exec:primary"){line.primaryVm.flags35C=0x123456efu;line.request.flags=1;line.request.color=-2;}if(s=="exec:secondary"){line.request.position={4,5,6};line.request.initialLength=-1;line.request.width=8;line.request.speed=9;line.request.angle=2;line.velocity.z=37;}};init();CHECK(line.position.x==4&&line.position.y==5&&line.position.z==6&&line.width==8&&line.growSpeed==9&&line.angle==2&&line.length==-1&&line.traveled==0&&line.velocity.z==37,"late-owned-geometry");CHECK(binds[0].resource==&resourceA&&binds[1].resource==&resourceB&&binds[0].script==107&&binds[1].script==257,"live-resource-owned-script");CHECK(line.primaryVm.flags35C==((0x123456efu&~0x20u|0x10u)&0xfc63ffffu|0x600000u),"flags-after-execution");
 for(float n:{0.f,-0.f,-1.f,1.f,std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()}){reset();input.initialLength=n;init();if(std::isnan(n))CHECK(line.traveled==0&&!std::signbit(line.traveled),"unordered-length");else CHECK(line.traveled==(n>0?.01f:0.f)&&!std::signbit(line.traveled),"length-sentinel");}
 reset();line.request=input;CHECK(line.Initialize(&line.request)==0&&line.request.patternWords[107]==0xabc06b,"self-request-copy");
}
static void motionCases(){
 for(float speed:{.5f,1.f,2.f}){reset();g_AnmGameSpeed=speed;line.flags=0;line.commandIndex=0;std::memset(&line.request,0,sizeof(line.request));auto &c=line.request.commands[0];c.opcode=0x40;c.allowWhileActive=1;c.arguments[0].scalar=.5;c.arguments[1].scalar=8;c.arguments[2].integer=4;c.arguments[3].integer=2;line.RunCommands();auto &m=line.patterns[3];CHECK(m.values[1].scalar==.5&&m.values[5].integer==4&&m.values[6].integer==2,"parser-motion-words");m.timer.current=3;m.timer.subframe=3.5;line.Motion0040();CHECK(near(polars.back().magnitude,.5f)&&line.growSpeed==4&&line.angle==1&&m.values[7].integer==0,"fractional-deceleration");CHECK(near(m.timer.subframe,3.5f+speed),"post-polar-tick");}
 reset();auto &m=line.patterns[3];line.flags=0x41;line.soundId=0;m.timer.current=4;m.timer.subframe=4;m.values[5].integer=4;m.values[6].integer=1;m.values[7].integer=0;m.values[0].scalar=8;m.values[1].scalar=4;line.Motion0040();CHECK(soundQueue.activeSoundIds[0]==0&&soundQueue.samples[0][0]==0,"zero-sound-id");CHECK(line.flags==1&&m.values[7].integer==1,"repeat-limit");CHECK(polars.size()==1&&line.velocity.z==19,"turn-after-clear");CHECK(line.angle==5&&polars.back().angle==5,"unwrapped-angle");CHECK(line.growSpeed==8&&polars.back().magnitude==8,"new-cycle-speed");CHECK(m.timer.current==1&&m.timer.previous==0&&m.timer.subframe==1,"reset-before-tick");
 reset();auto &n=line.patterns[3];n.values[5].integer=0;n.values[6].integer=0;n.values[0].scalar=3;n.values[1].scalar=-.5;line.flags=0x40;line.Motion0040();CHECK(line.flags==0&&line.angle==.5&&polars.back().magnitude==3&&n.timer.current==1,"zero-duration-no-divide");
 reset();auto &q=line.patterns[3];q.values[5].integer=2;q.values[6].integer=3;q.values[0].scalar=6;q.values[1].scalar=.25;line.flags=0x40;q.timer.current=2;q.timer.subframe=2;line.Motion0040();CHECK(line.flags==0x40&&q.values[7].integer==1&&soundQueue.activeSoundIds[0]==-1,"continue-cycle-negative-sound");
 reset();auto &f=line.patterns[3];f.values[5].integer=1;f.values[6].integer=1;f.values[0].scalar=7;f.timer.current=1;f.timer.subframe=1;line.flags=0x40;line.soundId=28;std::fill(std::begin(soundQueue.activeSoundIds),std::end(soundQueue.activeSoundIds),28);std::fill(std::begin(soundQueue.sampleCounts),std::end(soundQueue.sampleCounts),128);line.Motion0040();CHECK(line.flags==0&&line.growSpeed==7&&f.timer.current==1,"ignored-sound-failure");
}
int main(){try{initCases();motionCases();}catch(const Failure&f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
