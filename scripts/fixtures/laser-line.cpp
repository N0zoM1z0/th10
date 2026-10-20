#define __stdcall
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#include "EnemyLaserLine.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"
#include "Gui.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <limits>
#include <new>
#include <string>
#include <vector>
struct Failure{const char *label;};
#define CHECK(x,label) do{if(!(x))throw Failure{label};}while(0)
static std::vector<std::string> events;
static std::function<void(const std::string&)> after;
static void event(const std::string&s){events.push_back(s);if(after)after(s);}
float g_AnmGameSpeed=1;
// HOST lifetime/prefill roles; full target constructors are outside this suite.
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
AnmLoadedView::AnmLoadedView(){std::memset(this,0,sizeof(*this));}
AnmRenderManagerView::AnmRenderManagerView(){}
AnmRenderManagerView::~AnmRenderManagerView(){}
Player::Player(){std::memset(this,0,sizeof(*this));}
Player::~Player(){}
PlayerDrawVmView::~PlayerDrawVmView(){}
GuiMessageVmView::GuiMessageVmView(const unsigned char *){}
GuiMessageVmView::~GuiMessageVmView(){}
GuiView::~GuiView(){}
static Player playerA,playerB;Player *g_Player=&playerA;
static GuiView gui;GuiView *g_GuiView=&gui;
static EnemyPrimaryResourceOwnerView resourceOwner;EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner=&resourceOwner;
static AnmLoadedView resource;
static EnemySoundQueueView soundQueue;unsigned char *g_MainSoundOwner=reinterpret_cast<unsigned char*>(&soundQueue);
struct EnemySoundCueMetadataView{int unknown000;unsigned short unknown004;short cueValue;};
EnemySoundCueMetadataView g_EnemySoundCueMetadata[134];
static AnmRenderManagerView managerA,managerB;AnmRenderManagerView *g_AnmRenderManagerView=&managerA;
static AnmSpriteView sprite;
// Target values are separately checked by report-laser-line.py.
extern const float g_EnemyLaserPlayfieldMinimumX=-192.0f;
extern const float g_EnemyLaserPlayfieldMaximumX=192.0f;
extern const float g_EnemyLaserPlayfieldMinimumY=0.0f;
extern const float g_EnemyLaserPlayfieldMaximumY=448.0f;
void EnemyLaserVectorView::FromAngleMagnitude(float a,float n){x=std::cos(a)*n;y=std::sin(a)*n;/* HOST math role deliberately leaves Z untouched. */}
void PlayerHandleCollisionHit(Player *p){CHECK(p==g_Player,"hit-receiver");event("hit");}
void EnemyLaserLineView::RunCommands(){event("commands");}
int EnemyLaserLineView::Initialize(const EnemyLaserType0RequestView *){throw Failure{"unexpected-slot"};}
void EnemyLaserLineView::UnknownSlot10(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::Clear(int){throw Failure{"unexpected-slot"};}
void EnemyLaserLineView::UnknownSlot1C(){throw Failure{"unexpected-slot"};}
void EnemyLaserLineView::UnknownSlot20(){throw Failure{"unexpected-slot"};}
void EnemyLaserLineView::Motion0001(){event("m1");}
void EnemyLaserLineView::Motion0020(){event("m20");}
void EnemyLaserLineView::Motion0040(){event("m40");}
void EnemyLaserLineView::Motion0100(){event("m100");}
void EnemyLaserLineView::Motion0080(){event("m80");}
void EnemyLaserLineView::Bounce(){event("bounce");}
void EnemyLaserLineView::Motion100000(){event("m100000");}
void EnemyLaserLineView::Motion200000(){event("m200000");}
void EnemyLaserLineView::Motion4000000(){event("m4000000");}
struct HostLine:EnemyLaserLineView{void Motion0010()override{event("m10");}};
static HostLine line;
int EnemyLaserLineView::CheckCollisionBox(EnemyLaserVectorView *p,EnemyLaserVectorView *s,int mode){CHECK(p==reinterpret_cast<EnemyLaserVectorView*>(&g_Player->drawPosition)&&s->x==32&&s->y==32&&s->z==0&&mode==0,"hit-box-contract");event("box");return -1;}
int AnmRenderManagerView::ExecuteScript(AnmVmView *vm){event(vm==&line.primaryVm?"exec:primary":"exec:secondary");return -1;}
int AnmRenderManagerView::Draw(AnmVmView *vm){event(std::string(this==&managerA?"drawA:":"drawB:")+(vm==&line.primaryVm?"primary":"secondary"));return -1;}
AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(int script,const AnmFloat3View *p){CHECK(this==&resource&&script==434&&p==reinterpret_cast<AnmFloat3View*>(&g_Player->drawPosition),"graze-resource-contract");event("graze");AnmVmIdView id;id.value=123;return id;}
#include "helpers.inc"
#include "owner.inc"
static bool near(float a,float b){return std::abs(a-b)<.0001f;}
static void timer(AnmVmTimerView &t,int n){t.current=n;t.previous=n-1;t.subframe=float(n);t.scale=&g_AnmGameSpeed;t.flags=1;}
static void reset(){
 after={};events.clear();g_AnmGameSpeed=1;g_Player=&playerA;g_GuiView=&gui;gui.message=NULL;g_AnmRenderManagerView=&managerA;
 for(Player *p:{&playerA,&playerB}){p->drawPosition={5000,5000,0};p->extent0={1,1,1};p->runtimeState=0;p->highlightTimer.current=0;}
 line.flags=0;line.position={0,100,3};line.velocity={2,4,6};line.angle=0;line.length=100;line.width=20;line.growSpeed=2;line.traveled=0;line.request.maximumLength=200;line.request.terminalDistance=0;
 timer(line.age,0);timer(line.holdTimer,1);new(&line.patterns[1])EnemyLaserPatternRecordView;for(auto &p:line.patterns)timer(p.timer,0);
 // HOST establish a live acceleration view in the opaque record storage.
 static_assert(sizeof(EnemyLaserAccelerationRecordView)==sizeof(EnemyLaserPatternRecordView),"host record storage");
 auto *m=new(&line.patterns[1])EnemyLaserAccelerationRecordView;timer(m->timer,0);m->growAcceleration=3;m->velocityAcceleration={4,6,8};m->duration=10;
 line.primaryVm.loadedSprite=&sprite;sprite.width=16;sprite.height=4;line.primaryVm.scaleX=line.primaryVm.scaleY=-9;line.primaryVm.flags35C=0x100;
 line.primaryVm.position=AnmFloat3View(0,0,0);line.primaryVm.positionOffset=AnmFloat3View(71,72,73);line.secondaryVm.position=AnmFloat3View(0,0,0);
 resourceOwner.primaryEnemyResource=&resource;g_EnemyPrimaryResourceOwner=&resourceOwner;g_MainSoundOwner=reinterpret_cast<unsigned char*>(&soundQueue);std::memset(&soundQueue,0,sizeof(soundQueue));std::fill(std::begin(soundQueue.activeSoundIds),std::end(soundQueue.activeSoundIds),-1);g_EnemySoundCueMetadata[28].cueValue=77;
}
static int update(){return line.Update();}
static void contactCases(){
 reset();PlayerFloat3 p={0,0,17};playerA.drawPosition={30,0,0};CHECK(playerA.CheckLaserContact(&p,0,10,80)==1,"highlight-gate");
 for(int state:{2,4,3}){playerA.runtimeState=state;CHECK(playerA.CheckLaserContact(&p,0,10,80)==0,"player-state-gate");}
 playerA.runtimeState=0;playerA.highlightTimer.current=1;CHECK(playerA.CheckLaserContact(&p,0,10,80)==0,"highlight-gate");playerA.highlightTimer.current=0;
 // Non-null live storage suffices for the message-presence gate; no dereference.
 GuiMessageVmView message(NULL);gui.message=&message;CHECK(playerA.CheckLaserContact(&p,0,10,80)==0,"message-gate");gui.message=NULL;
 playerA.drawPosition={30,-15,0};CHECK(playerA.CheckLaserContact(&p,0,10,80)==0,"graze-lower-y");playerA.drawPosition.y=-14;CHECK(playerA.CheckLaserContact(&p,0,10,80)==2,"graze-boundary");
 playerA.drawPosition={-1,0,0};CHECK(playerA.CheckLaserContact(&p,0,10,80)==1,"longitudinal-inclusive");playerA.drawPosition.x=81;CHECK(playerA.CheckLaserContact(&p,0,10,80)==1,"longitudinal-inclusive");playerA.drawPosition.x=81.25;CHECK(playerA.CheckLaserContact(&p,0,10,80)==0,"longitudinal-outside");
 playerA.drawPosition={std::numeric_limits<float>::quiet_NaN(),0,0};CHECK(playerA.CheckLaserContact(&p,0,10,80)==1,"unordered-contact");
 playerA.drawPosition={0,30,0};CHECK(playerA.CheckLaserContact(&p,1.5707964f,10,80)==1,"rotation-contact");
}
static void suite(){
 contactCases();
 reset();after=[](const std::string&s){if(s=="commands")line.flags=1;};update();CHECK(events.size()>=2&&events[0]=="commands"&&events[1]=="m1","commands-first");
 reset();line.flags=0x11;after=[](const std::string&s){if(s=="m1"){line.flags&=~0x10u;line.flags|=0x20;}};update();CHECK((events==std::vector<std::string>{"commands","m1","m20","exec:primary","exec:secondary"}),"live-motion-flags");
 reset();line.flags=0x1|0x10|0x20|0x40|0x100|0x80|0x800|0x100000|0x200000|0x4000000;update();CHECK((events==std::vector<std::string>{"commands","m1","m10","m20","m40","m100","m80","bounce","m100000","m200000","m4000000","exec:primary","exec:secondary"}),"motion-slot-order");
 reset();line.flags=0x8000;update();CHECK((line.flags&0x8000)==0,"flash-boundary");
 for(float speed:{.5f,1.f,2.f}){reset();g_AnmGameSpeed=speed;line.flags=0x8000;timer(line.patterns[5].timer,3);timer(line.holdTimer,5);update();CHECK(near(line.length,100+2*speed),"growth-speed");CHECK(near(line.holdTimer.subframe,5-speed),"hold-speed");CHECK(near(line.patterns[5].timer.subframe,3-speed),"flash-speed");}
 reset();line.request.maximumLength=101;update();CHECK(line.length==101&&line.traveled==0,"growth-cap");
 reset();line.request.maximumLength=100;line.request.terminalDistance=101;line.position={0,100,3};CHECK(update()==0&&line.traveled==2&&line.length==99&&line.request.maximumLength==99&&line.position.x==2&&line.position.y==104&&line.position.z==9,"terminal-trim-motion");
 reset();line.request.maximumLength=100;line.request.terminalDistance=2;CHECK(update()==1&&(events==std::vector<std::string>{"commands"}),"terminal-zero");
 reset();timer(line.holdTimer,0);line.position={-250,100,0};CHECK(update()==0,"both-bounds");reset();timer(line.holdTimer,0);line.position={-500,100,0};CHECK(update()==1,"outside-bounds");
 reset();line.length=16;line.request.maximumLength=16;line.growSpeed=0;playerA.drawPosition={5,100,0};update();CHECK(std::find(events.begin(),events.end(),"hit")==events.end(),"collision-length-gate");
 reset();playerA.drawPosition={30,100,0};after=[](const std::string&s){if(s=="hit"){g_Player=&playerB;playerB.drawPosition={31,100,0};}};update();CHECK((events==std::vector<std::string>{"commands","hit","box","exec:primary","exec:secondary"}),"fresh-hit-player");
 for(int age:{-5,0,5,6}){reset();timer(line.age,age);playerA.drawPosition={30,110,0};after=[](const std::string&s){if(s=="graze")line.position.x=12;};update();bool expected=age%5==0;CHECK((std::find(events.begin(),events.end(),"graze")!=events.end())==expected,"signed-graze-period");if(expected)CHECK(soundQueue.activeSoundIds[0]==28&&soundQueue.samples[0][0]==62&&soundQueue.cueValues[28]==77,"sound-after-create");}
 reset();update();CHECK(near(line.primaryVm.scaleX,1.25f)&&near(line.primaryVm.scaleY,25.5f),"scale-axes");CHECK(line.primaryVm.flags35C==0x108,"scale-flags");
 reset();after=[](const std::string&s){if(s=="exec:primary")line.traveled=1;};update();CHECK(events.back()=="exec:primary","strict-tail-zero");
 reset();line.traveled=std::numeric_limits<float>::quiet_NaN();update();CHECK(events.back()=="exec:primary","unordered-tail");
 reset();line.Draw();CHECK(near(line.primaryVm.position.x,224)&&line.primaryVm.positionOffset.x==71,"draw-position-field");CHECK(near(line.primaryVm.position.y,116)&&line.primaryVm.position.z==3&&near(line.primaryVm.rotation.z,1.5707964f)&&(line.primaryVm.flags35C&4),"draw-offsets");
 reset();after=[](const std::string&s){if(s=="drawA:primary"){g_AnmRenderManagerView=&managerB;line.position={5,6,7};}};line.Draw();CHECK((events==std::vector<std::string>{"drawA:primary","drawB:secondary"})&&line.secondaryVm.position.x==229&&line.secondaryVm.position.y==22&&line.secondaryVm.position.z==7,"fresh-draw-owner-position");
 reset();after=[](const std::string&s){if(s=="drawA:primary")line.traveled=1;};line.Draw();CHECK(events.size()==1,"draw-tail-after-call");
 reset();line.flags=0x11;auto &m=*reinterpret_cast<EnemyLaserAccelerationRecordView*>(&line.patterns[1]);m.timer.current=10;m.timer.subframe=10;line.EnemyLaserLineView::Motion0010();CHECK(line.flags==1&&line.growSpeed==2,"motion-duration");CHECK(m.timer.current==11,"motion-tick");
 reset();auto &n=*reinterpret_cast<EnemyLaserAccelerationRecordView*>(&line.patterns[1]);g_AnmGameSpeed=.5;line.EnemyLaserLineView::Motion0010();CHECK(near(line.growSpeed,3.5)&&near(line.velocity.x,4)&&near(line.velocity.y,7)&&near(line.velocity.z,10)&&near(line.angle,std::atan2(7.f,4.f))&&near(n.timer.subframe,.5),"motion-acceleration");
 reset();auto &q=*reinterpret_cast<EnemyLaserAccelerationRecordView*>(&line.patterns[1]);q.velocityAcceleration={0,0,0};line.velocity={0,0,9};line.angle=2;line.EnemyLaserLineView::Motion0010();CHECK(line.angle==2,"motion-near-zero");
}
int main(){try{suite();}catch(const Failure&f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
