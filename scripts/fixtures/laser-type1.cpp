#define __stdcall
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#include "EnemyLaserType1.hpp"
#include "Enemy.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"
#include "Gui.hpp"
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
static std::vector<std::string> events;static std::function<void(const std::string&)> after;static bool initializing=false;
static void event(const std::string&s){events.push_back(s);if(after)after(s);}
float g_AnmGameSpeed=1;static float privateSpeed=2;
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
AnmLoadedView::AnmLoadedView(){std::memset(this,0,sizeof(*this));}
AnmRenderManagerView::AnmRenderManagerView(){}
AnmRenderManagerView::~AnmRenderManagerView(){}
Player::Player(){}
Player::~Player(){}
PlayerDrawVmView::~PlayerDrawVmView(){}
EnemyManagerView::EnemyManagerView(){}
EnemyManagerView::~EnemyManagerView(){}
EnemyEclContextView::EnemyEclContextView(){}
EnemyBulletPatternView::EnemyBulletPatternView(){}
EnemyEclHostBaseView::EnemyEclHostBaseView(){}
EnemyEclHostBaseView::~EnemyEclHostBaseView(){}
EnemyFullObjectView::EnemyFullObjectView(const char*){}
EnemyFullObjectView::~EnemyFullObjectView(){}
#define ENEMY_STUBS(C) int C::DispatchEclInstruction(){throw Failure{"unexpected-enemy"};} int C::ReadIntOperand(int){throw Failure{"unexpected-enemy"};} int *C::ResolveIntOperand(int){throw Failure{"unexpected-enemy"};} float C::ReadFloatOperand(int){throw Failure{"unexpected-enemy"};} float *C::ResolveFloatOperand(int){throw Failure{"unexpected-enemy"};}
ENEMY_STUBS(EnemyEclHostBaseView) ENEMY_STUBS(EnemyFullObjectView)
static EnemyLaserType1View line;static EnemyLaserType1RequestView input;
static Player playerA,playerB;Player *g_Player=&playerA;GuiView *g_GuiView=nullptr;
static EnemyManagerView enemyManager;EnemyManagerView *g_EnemyManager=&enemyManager;
static EnemyFullObjectView enemy(nullptr);
static AnmLoadedView resourceA,resourceB,grazeResource;
static EnemyLaserManagerStorageView laserManagerA,laserManagerB;
EnemyLaserManagerView *g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&laserManagerA);
static EnemyPrimaryResourceOwnerView resourceOwner;EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner=&resourceOwner;
static AnmRenderManagerView rendererA,rendererB;AnmRenderManagerView *g_AnmRenderManagerView=&rendererA;
static AnmSpriteView sprite;
static EnemySoundQueueView queue;unsigned char *g_MainSoundOwner=reinterpret_cast<unsigned char*>(&queue);
struct EnemySoundCueMetadataView{int unknown000;unsigned short unknown004;short cueValue;};EnemySoundCueMetadataView g_EnemySoundCueMetadata[134];
int g_BulletSpriteScriptBase[8]={10,30,100,400,500,600,700,800};
struct Bind{AnmLoadedView *resource;AnmVmView *vm;int script;};static std::vector<Bind> binds;
static std::string which(AnmVmView *vm){CHECK(vm==&line.primaryVm||vm==&line.secondaryVm,"vm-receiver");return vm==&line.primaryVm?"primary":"secondary";}
void AnmVmView::Initialize(){CHECK(line.state==3,"state-before-reset");flags35C=0xffffffffu;pendingInterrupt=51;events.push_back("reset:"+which(this));/* HOST pure observation, no callbacks. */}
void AnmLoadedView::InitializeAndExecuteScriptIndex(AnmVmView *vm,int script){binds.push_back({this,vm,script});event("bind:"+which(vm));}
int AnmRenderManagerView::ExecuteScript(AnmVmView *vm){if(initializing)CHECK(vm->pendingInterrupt==2,"pending-before-execute");event("exec:"+which(vm));return -1;}
int AnmRenderManagerView::Draw(AnmVmView *vm){event(std::string(this==&rendererA?"drawA:":"drawB:")+which(vm));return -1;}
AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(int script,const AnmFloat3View *p){CHECK(this==&grazeResource&&script==434&&p==reinterpret_cast<const AnmFloat3View*>(&g_Player->drawPosition),"graze-resource-contract");event("graze");return AnmVmIdView(7);}
void EnemyLaserVectorView::FromAngleMagnitude(float a,float n){x=std::cos(a)*n;y=std::sin(a)*n;/* HOST XY-only polar role, no callback or Z access. */}
void PlayerHandleCollisionHit(Player *p){CHECK(p==g_Player,"hit-receiver");event("hit");}
void EnemyLaserType1View::RunCommands(){throw Failure{"unexpected-slot"};}
void EnemyLaserType1View::UnknownSlot10(){throw Failure{"unexpected-slot"};}
int EnemyLaserType1View::Clear(int){throw Failure{"unexpected-slot"};}
void EnemyLaserType1View::UnknownSlot1C(){throw Failure{"unexpected-slot"};}
void EnemyLaserType1View::UnknownSlot20(){throw Failure{"unexpected-slot"};}
int EnemyLaserType1View::CheckCollisionBox(EnemyLaserVectorView *p,EnemyLaserVectorView *size,int mode){CHECK(p==reinterpret_cast<EnemyLaserVectorView*>(&g_Player->drawPosition)&&size->x==32&&size->y==32&&size->z==0&&mode==0,"hit-box-contract");event("box");return -1;}
#include "helpers.inc"
#include "owner.inc"
static bool near(float a,float b){return std::abs(a-b)<.0001f;}
static void timer(AnmVmTimerView &t,int n){t.previous=n-1;t.current=n;t.subframe=float(n);t.scale=&privateSpeed;t.flags=0x81;}
static void reset(){
 after={};events.clear();binds.clear();initializing=false;g_AnmGameSpeed=1;line.state=2;line.position={0,100,3};line.velocity={7,8,9};line.angle=0;line.length=100;line.width=20;line.growSpeed=2;line.traveled=0;line.id=77;line.flags=0x500;line.commandIndex=19;line.soundId=-1;line.deletionCounter=41;timer(line.age,1);timer(line.holdTimer,9);
 std::memset(&line.request,0,sizeof(line.request));line.request.maximumLength=200;line.request.width=40;line.request.growFrames=10;line.request.waitFrames=2;line.request.holdFrames=100;line.request.shrinkFrames=5;
 line.primaryVm.loadedSprite=&sprite;sprite.width=16;sprite.height=4;line.primaryVm.flags35C=0x100;line.primaryVm.scaleX=line.primaryVm.scaleY=-9;line.primaryVm.position=AnmFloat3View(0,0,0);line.primaryVm.positionOffset=AnmFloat3View(71,72,73);line.secondaryVm.position=AnmFloat3View(0,0,0);
 std::memset(&input,0,sizeof(input));input.position={12,34,56};input.angle=.5;input.initialLength=11;input.maximumLength=333;input.width=9;input.growSpeed=5;input.type=2;input.color=7;input.velocity={2,4,6};for(unsigned i=0;i<108;++i)input.patternWords[i]=0xabc000+i;
 for(auto *p:{&playerA,&playerB}){p->drawPosition={5000,5000,0};p->extent0={1,1,1};p->runtimeState=0;p->highlightTimer.current=0;}g_Player=&playerA;g_GuiView=nullptr;
 enemyManager.specialEnemySlots[0]=nullptr;g_EnemyManager=&enemyManager;enemy.runtime.worldMotion.position={20,40,60};
 laserManagerA.effectResource=&resourceA;laserManagerB.effectResource=&resourceB;g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&laserManagerA);resourceOwner.primaryEnemyResource=&grazeResource;g_EnemyPrimaryResourceOwner=&resourceOwner;g_AnmRenderManagerView=&rendererA;g_MainSoundOwner=reinterpret_cast<unsigned char*>(&queue);std::memset(&queue,0,sizeof(queue));std::fill(std::begin(queue.activeSoundIds),std::end(queue.activeSoundIds),-1);g_EnemySoundCueMetadata[28].cueValue=77;
}
static void init(){initializing=true;CHECK(line.Initialize(&input)==0,"initialize-return");initializing=false;}
static void initCases(){
 reset();init();CHECK(std::memcmp(&line.request,&input,sizeof(input))==0,"complete-copy");CHECK((events==std::vector<std::string>{"reset:primary","bind:primary","exec:primary","reset:secondary","bind:secondary","exec:secondary"}),"vm-step-order");CHECK(binds.size()==2&&binds[0].script==107&&binds[1].script==266,"script-selection");CHECK(line.primaryVm.flags35C==((0xffffffffu&0xfc63ffffu)|0x600000u),"primary-mask");CHECK(line.secondaryVm.flags35C==(((0xffffffdfu|0x10u)&0xfc7fffffu)|0x400000u),"secondary-mask");CHECK(line.position.x==12&&line.position.y==34&&line.position.z==56&&line.angle==.5&&line.length==11&&line.width==2&&line.growSpeed==5&&line.soundId==24,"initial-geometry");CHECK(line.age.current==1&&line.holdTimer.current==9&&line.traveled==0&&line.velocity.z==9&&line.flags==0x500&&line.id==77&&line.deletionCounter==41&&line.commandIndex==19,"preserved-initial-state");
 reset();input.flags44=2;init();CHECK((line.primaryVm.flags35C&0x30)==0x10,"primary-gated-bit2");reset();input.flags44=1;init();CHECK((line.primaryVm.flags35C&0x30)==0x30,"primary-gated-bit2");
 reset();input.color=-7;init();CHECK(binds[0].script==93&&binds[1].script==252,"signed-script-selection");
 reset();after=[](const std::string&s){if(s=="exec:primary"){g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&laserManagerB);line.request.color=-4;line.request.flags44=2;}if(s=="exec:secondary"){line.request.position={31,32,33};line.request.angle=2;line.request.initialLength=44;line.request.growSpeed=6;}};init();CHECK(binds[1].resource==&resourceB&&binds[1].script==255,"live-secondary-resource");CHECK((line.primaryVm.flags35C&0x30)==0x10,"live-primary-flags");CHECK(line.position.x==31&&line.position.y==32&&line.position.z==33&&line.angle==2&&line.length==44&&line.growSpeed==6,"late-owned-geometry");
}
static int update(){return line.Update();}
static void updateCases(){
 for(float speed:{.5f,1.f,2.f}){reset();g_AnmGameSpeed=speed;line.request.angularVelocity=.2f;line.request.velocity={2,4,6};CHECK(update()==0,"update-return");CHECK(near(line.length,100+2*speed),"length-speed");CHECK(near(line.angle,.2f*speed),"angular-speed");CHECK(near(line.position.x,2*speed)&&near(line.position.y,100+4*speed)&&near(line.position.z,3+6*speed),"request-motion");CHECK(line.age.current==1&&line.age.subframe==1&&line.holdTimer.current==9,"external-age-unchanged");}
 reset();line.request.maximumLength=101;update();CHECK(line.length==101,"length-cap");
 reset();line.request.flags44=1;line.request.velocity={1,2,3};enemyManager.specialEnemySlots[0]=&enemy;g_AnmGameSpeed=.5;update();CHECK(line.position.x==20.5&&line.position.y==41&&line.position.z==61.5,"attach-before-motion");
 reset();line.request.flags44=1;line.request.velocity={1,2,3};update();CHECK(line.position.x==1&&line.position.y==102&&line.position.z==6,"null-attachment");
 reset();line.state=3;line.width=2;timer(line.age,2);line.request.waitFrames=2;update();CHECK(line.state==4&&line.width==2,"phase-wait");CHECK(line.age.current==0&&line.age.previous==-1&&line.age.subframe==0,"phase-timer-reset");
 reset();line.state=3;timer(line.age,1);update();CHECK(line.state==3&&line.width==20&&line.age.current==1,"phase-wait-before");
 reset();line.state=4;timer(line.age,2);line.age.subframe=2.5;update();CHECK(line.state==4&&near(line.width,10),"fractional-growth");
 reset();line.state=4;line.request.growFrames=0;line.request.holdFrames=0;timer(line.age,0);update();CHECK(line.state==5&&line.width==40&&line.age.current==0,"phase-fallthrough");
 reset();line.state=5;timer(line.age,2);line.age.subframe=2.5;update();CHECK(near(line.width,20)&&line.state==5,"fractional-shrink");
 reset();line.state=5;timer(line.age,5);CHECK(update()==1&&events.empty(),"retirement");CHECK(line.primaryVm.scaleX==-9,"retirement-skips-vms");
 reset();line.state=4;line.request.growFrames=-1;line.request.holdFrames=-1;line.request.shrinkFrames=0;timer(line.age,0);CHECK(update()==1&&line.state==5,"zero-duration-chain");
 reset();line.state=2;line.width=40;playerA.drawPosition={30,111,0};update();CHECK(std::find(events.begin(),events.end(),"hit")!=events.end(),"collision-taper");
 reset();line.state=4;line.age.subframe=.1;playerA.drawPosition={30,100,0};update();CHECK(std::find(events.begin(),events.end(),"box")!=events.end(),"growth-contact");
 reset();line.length=16;line.request.maximumLength=16;line.growSpeed=0;playerA.drawPosition={5,100,0};update();CHECK(std::find(events.begin(),events.end(),"hit")==events.end(),"contact-length");
 reset();playerA.drawPosition={30,100,0};after=[](const std::string&s){if(s=="hit"){g_Player=&playerB;playerB.drawPosition={31,100,0};}};update();CHECK((events==std::vector<std::string>{"hit","box","exec:primary","exec:secondary"}),"fresh-player-after-hit");
 for(int age:{-5,0,5,6}){reset();timer(line.age,age);playerA.drawPosition={30,110,0};after=[](const std::string&s){if(s=="graze")line.position.x=12;};update();bool expected=age%5==0;CHECK((std::find(events.begin(),events.end(),"graze")!=events.end())==expected,"signed-graze-period");if(expected)CHECK(queue.activeSoundIds[0]==28&&queue.samples[0][0]==62&&queue.cueValues[28]==77,"sound-after-create");}
 reset();update();CHECK(near(line.primaryVm.scaleX,1.25)&&near(line.primaryVm.scaleY,25.5),"scale-axes");CHECK(line.primaryVm.flags35C==0x108,"scale-flags");
 reset();after=[](const std::string&s){if(s=="exec:primary")line.traveled=1;};update();CHECK(events.back()=="exec:primary","script-tail-live");reset();line.traveled=std::numeric_limits<float>::quiet_NaN();update();CHECK(events.back()=="exec:primary","script-tail-unordered");
 reset();line.length=std::numeric_limits<float>::quiet_NaN();update();CHECK(std::isnan(line.length),"unordered-growth");
}
static void drawCases(){
 reset();CHECK(line.Draw()==0,"draw-return");CHECK(line.primaryVm.position.x==224&&line.primaryVm.positionOffset.x==71,"draw-field");CHECK(line.primaryVm.position.y==116&&line.primaryVm.position.z==3&&near(line.primaryVm.rotation.z,1.5707964f)&&(line.primaryVm.flags35C&4),"draw-offsets");
 reset();after=[](const std::string&s){if(s=="drawA:primary"){g_AnmRenderManagerView=&rendererB;line.position={5,6,7};}};line.Draw();CHECK((events==std::vector<std::string>{"drawA:primary","drawB:secondary"}),"fresh-draw-owner");CHECK(line.secondaryVm.position.x==229&&line.secondaryVm.position.y==22&&line.secondaryVm.position.z==7,"fresh-draw-position");
 reset();after=[](const std::string&s){if(s=="drawA:primary")line.traveled=1;};line.Draw();CHECK(events.size()==1,"draw-live-gate");reset();line.traveled=std::numeric_limits<float>::quiet_NaN();line.Draw();CHECK(events.size()==1,"draw-unordered-gate");
}
int main(){try{initCases();updateCases();drawCases();}catch(const Failure&f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
