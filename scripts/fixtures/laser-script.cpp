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
static AnmLoadedView resourceA,resourceB;
static EnemyPrimaryResourceOwnerView ownerA,ownerB;EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner=&ownerA;
static EnemySoundQueueView soundQueue;unsigned char *g_MainSoundOwner=reinterpret_cast<unsigned char*>(&soundQueue);
struct EnemySoundCueMetadataView{int unknown000;unsigned short unknown004;short cueValue;};
EnemySoundCueMetadataView g_EnemySoundCueMetadata[134];
int g_BulletSpriteScriptBase[4]={10,30,100,400};
static std::vector<std::string> events;static std::function<void(const std::string&)> after;
static AnmLoadedView *startedResource;static int startedScript;
static void event(const std::string&s){events.push_back(s);if(after)after(s);}
void AnmVmView::Initialize(){CHECK(this==&line.primaryVm,"embedded-vm-receiver");events.push_back("reset-vm");/* HOST pure observation, no callback. */}
void AnmLoadedView::InitializeAndExecuteScriptIndex(AnmVmView *vm,int script){CHECK(vm==&line.primaryVm,"embedded-vm-receiver");startedResource=this;startedScript=script;event("start-script");}
void EnemyLaserVectorView::FromAngleMagnitude(float a,float n){x=std::cos(a)*n;y=std::sin(a)*n;/* HOST math role: Z remains untouched. */}
#define UNUSED_VOID(name) void EnemyLaserLineView::name(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::Clear(int){throw Failure{"unexpected-slot"};}
UNUSED_VOID(UnknownSlot10) UNUSED_VOID(UnknownSlot1C) UNUSED_VOID(UnknownSlot20)
UNUSED_VOID(Motion0001) UNUSED_VOID(Motion0010) UNUSED_VOID(Motion0020) UNUSED_VOID(Motion0040) UNUSED_VOID(Motion0100) UNUSED_VOID(Motion0080)
UNUSED_VOID(Bounce) UNUSED_VOID(Motion100000) UNUSED_VOID(Motion200000) UNUSED_VOID(Motion4000000)
int EnemyLaserLineView::Update(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::Initialize(const EnemyLaserType0RequestView *){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::Draw(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::CheckCollisionBox(EnemyLaserVectorView*,EnemyLaserVectorView*,int){throw Failure{"unexpected-slot"};}
#include "helpers.inc"
#include "owner.inc"
static bool near(float a,float b){return std::abs(a-b)<.0001f;}
static void reset(){
 after={};events.clear();startedResource=NULL;startedScript=-1;line.commandIndex=0;line.flags=0;line.unknown00C=77;line.id=88;line.soundId=-1;line.position={12,34,56};line.angle=.25f;line.growSpeed=7;line.width=71;
 for(auto &cmd:line.request.commands){cmd={};}
 for(auto &p:line.patterns){p.timer.current=77;p.timer.previous=-9;p.timer.subframe=11.5f;p.timer.scale=&privateSpeed;p.timer.flags=0x80;for(auto &w:p.values)w.bits=0x11223344;}
 player.drawPosition={13,35,99};g_Player=&player;g_AnmGameSpeed=1;ownerA.primaryEnemyResource=&resourceA;ownerB.primaryEnemyResource=&resourceB;g_EnemyPrimaryResourceOwner=&ownerA;
 g_BulletSpriteScriptBase[2]=100;std::memset(&soundQueue,0,sizeof(soundQueue));std::fill(std::begin(soundQueue.activeSoundIds),std::end(soundQueue.activeSoundIds),-1);g_EnemySoundCueMetadata[28].cueValue=77;
 line.primaryVm.position=AnmFloat3View(8,9,10);line.primaryVm.positionOffset=AnmFloat3View(11,12,13);line.primaryVm.alternatePosition=AnmFloat3View(14,15,16);
}
static EnemyLaserCommandView &command(int i,unsigned opcode,int gate=1){auto &c=line.request.commands[i];c.opcode=opcode;c.allowWhileActive=gate;return c;}
static void run(){line.RunCommands();}
static void suite(){
 reset();run();CHECK(line.commandIndex==0&&line.flags==0,"zero-opcode");
 reset();for(int i=0;i<18;++i)command(i,0xabcdefu);run();CHECK(line.commandIndex==18,"eighteen-record-limit");
 reset();line.commandIndex=18;g_Player=NULL;g_EnemyPrimaryResourceOwner=NULL;run();CHECK(line.commandIndex==18,"upper-pc-short-circuit");
 reset();line.flags=0x20;command(0,0x1000000,0).arguments[2].integer=22;run();CHECK(line.id==88&&line.commandIndex==0,"active-gate");
 reset();command(0,0x80000000u);command(1,0x80000000u);command(2,0x1000000u).arguments[2].bits=0xffffffffu;run();CHECK(line.id==0xffffffffu&&line.commandIndex==3,"same-frame-skip");
 reset();line.flags=0x40;command(0,1,-1);command(1,0x1000000,0).arguments[2].integer=22;run();CHECK(line.flags==0x41&&line.commandIndex==1&&line.id==88,"preserve-flags");CHECK(line.patterns[0].timer.current==0&&line.patterns[0].timer.previous==-1&&line.patterns[0].timer.flags==0x81&&line.patterns[0].timer.scale==&g_AnmGameSpeed&&line.patterns[0].values[4].integer==0,"timer-lazy-initialize");
 reset();command(0,0x10).arguments[1].scalar=-990;command(0,0x10).arguments[0].scalar=3;run();CHECK(line.patterns[1].values[1].scalar==.25f,"angle-negative-sentinel");
 reset();command(0,0x10).arguments[1].scalar=990;command(0,0x10).arguments[0].scalar=3;run();CHECK(near(line.patterns[1].values[1].scalar,.7853982f),"angle-positive-sentinel");
 reset();auto &literal=command(0,0x10);literal.arguments[0].scalar=3;literal.arguments[1].scalar=-.5;literal.arguments[2].integer=-7;line.patterns[1].values[4].scalar=19;run();CHECK(line.patterns[1].values[0].scalar==3&&line.patterns[1].values[1].scalar==-.5&&near(line.patterns[1].values[2].scalar,std::cos(-.5f)*3)&&near(line.patterns[1].values[3].scalar,std::sin(-.5f)*3)&&line.patterns[1].values[4].scalar==19&&line.patterns[1].values[5].integer==-7,"literal-angle-polar-xy");
 reset();player.drawPosition={12,34,0};command(0,0x10).arguments[1].scalar=991;run();CHECK(near(line.patterns[1].values[1].scalar,1.5707964f),"zero-delta-aim");
 reset();command(0,0x10).arguments[1].scalar=std::numeric_limits<float>::quiet_NaN();run();CHECK(std::isnan(line.patterns[1].values[1].scalar),"unordered-literal-angle");
 for(unsigned opcode:{0x10u,0x20u}){reset();line.soundId=28;command(0,opcode);run();CHECK(soundQueue.activeSoundIds[0]==-1,"first-command-sound");reset();line.commandIndex=1;line.soundId=28;command(1,opcode);run();CHECK(soundQueue.activeSoundIds[0]==28&&soundQueue.samples[0][0]==0&&soundQueue.sampleCounts[0]==1,"later-command-sound");reset();line.commandIndex=1;command(1,opcode);run();CHECK(soundQueue.activeSoundIds[0]==-1,"negative-sound-id");}
 reset();auto &polar=command(0,0x20);polar.arguments[0].scalar=1.25;polar.arguments[1].scalar=-2.5;polar.arguments[2].integer=17;polar.arguments[3].integer=99;run();CHECK(line.patterns[2].values[0].scalar==1.25&&line.patterns[2].values[1].scalar==-2.5&&line.patterns[2].values[5].integer==17&&line.patterns[2].timer.current==0,"polar-fields");
 reset();auto &width=command(0,0x40);width.arguments[0].integer=0x40000000;width.arguments[1].scalar=5;run();CHECK(line.patterns[3].values[1].integer==0x40000000,"raw-mode-word");
 for(unsigned opcode:{0x40u,0x80u,0x100u}){reset();auto &c=command(0,opcode);c.arguments[1].scalar=-999;c.arguments[2].integer=12;c.arguments[3].integer=-4;run();CHECK(line.patterns[3].values[0].scalar==7,"width-sentinel");CHECK(line.flags==opcode&&line.patterns[3].values[5].integer==12&&line.patterns[3].values[6].integer==-4&&line.patterns[3].values[7].integer==0,"width-fields");}
 for(float n:{-1000.f,std::numeric_limits<float>::quiet_NaN()}){reset();command(0,0x80).arguments[1].scalar=n;run();CHECK(line.patterns[3].values[0].scalar==7,"grow-speed-fallback");}
 reset();command(0,0x100).arguments[1].scalar=-998.5;run();CHECK(line.patterns[3].values[0].scalar==-998.5,"literal-width-value");
 reset();auto &start=command(0,0x4000);start.arguments[2].integer=2;start.arguments[3].integer=7;run();CHECK((events==std::vector<std::string>{"reset-vm","start-script"})&&startedResource==&resourceA&&startedScript==107&&line.primaryVm.scriptIndex==107,"embedded-script");CHECK(line.primaryVm.position.x==0&&line.primaryVm.position.y==0&&line.primaryVm.position.z==0&&line.primaryVm.positionOffset.x==0&&line.primaryVm.alternatePosition.z==0&&line.primaryVm.glyphWidth==16&&line.primaryVm.glyphHeight==16,"actual-vm-wrapper");
 reset();command(0,0x4000).arguments[2].integer=2;command(6,0x1000000).arguments[2].integer=55;after=[](const std::string&s){if(s=="start-script"){g_EnemyPrimaryResourceOwner=&ownerB;g_BulletSpriteScriptBase[2]=999;line.commandIndex=5;}};run();CHECK(line.commandIndex==7&&line.id==55,"live-pc-after-script");CHECK(startedResource==&resourceA&&startedScript==100,"captured-resource-script");
 reset();command(0,0x4000);command(1,0x1000000,0).arguments[2].integer=55;after=[](const std::string&s){if(s=="start-script")line.flags=1;};run();CHECK(line.commandIndex==1&&line.id==88,"live-gate-after-script");
 reset();command(0,0x8000).arguments[2].integer=16777217;line.patterns[5].timer.flags=0x81;run();CHECK(line.patterns[5].timer.current==16777217&&line.patterns[5].timer.previous==16777216&&line.patterns[5].timer.subframe==16777216.f&&line.patterns[5].timer.scale==&privateSpeed,"wait-integer");
 for(unsigned opcode:{0x100000u,0x200000u}){reset();command(0,opcode).arguments[2].integer=-3;run();CHECK(line.flags==opcode&&line.patterns[6].timer.current==-3&&line.patterns[6].timer.previous==-4&&line.patterns[6].timer.subframe==-3&&line.patterns[7].timer.current==77,"shared-wait-record");}
 reset();command(0,0x10000);run();CHECK(line.unknown00C==3&&line.flags==0,"state-word");
 reset();command(0,0x20000).arguments[2].integer=28;run();CHECK(soundQueue.activeSoundIds[0]==28&&soundQueue.samples[0][0]==62&&soundQueue.cueValues[28]==77,"panned-sound");
 reset();command(0,0x2000000).arguments[2].integer=2;command(1,0x1000000).arguments[2].integer=99;command(2,0x1000000).arguments[2].integer=123;run();CHECK(line.commandIndex==3&&line.id==123,"jump-target");
 for(unsigned opcode:{0x400u,0x800u,0x8000000u}){reset();auto &c=command(0,opcode);c.arguments[0].scalar=5;c.arguments[2].integer=2;run();CHECK(c.arguments[2].integer==1,"mutable-command-count");CHECK(line.flags==opcode&&line.patterns[4].values[0].scalar==5&&line.patterns[4].values[5].integer==0&&line.patterns[4].values[6].integer==0,"count-fields");line.commandIndex=0;run();CHECK(c.arguments[2].integer==0,"mutable-command-count");line.commandIndex=0;line.flags=0;line.patterns[4].values[5].integer=99;run();CHECK(line.flags==0&&c.arguments[2].integer==0&&line.patterns[4].values[5].integer==99,"count-zero");}
 reset();auto &n=command(0,0x800);n.arguments[0].scalar=std::numeric_limits<float>::quiet_NaN();n.arguments[2].integer=1;run();CHECK(line.patterns[4].values[0].scalar==7,"unordered-growth");
 reset();auto &move=command(0,0x4000000);move.arguments[0].scalar=2.5;move.arguments[1].scalar=-8.5;move.arguments[2].integer=42;move.arguments[3].integer=99;run();CHECK(line.flags==0x4000000&&line.patterns[8].values[0].scalar==2.5&&line.patterns[8].values[1].scalar==-8.5&&line.patterns[8].values[5].integer==42&&line.patterns[8].timer.current==0,"point-motion-fields");
 for(unsigned opcode:{2u,0xffffffffu,0x80000001u}){reset();command(0,opcode);run();CHECK(line.commandIndex==1&&line.flags==0&&line.id==88,"unknown-opcode-default");}
}
int main(){try{suite();}catch(const Failure&f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
