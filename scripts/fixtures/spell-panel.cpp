#define __stdcall
#define __fastcall
#define __cdecl
#define __declspec(x)
#include "EnemyGameState.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <limits>
#include <map>
#include <string>
#include <vector>
struct Failure{const char *label;};
#define CHECK(x,label) do{if(!(x))throw Failure{label};}while(0)
// HOST outer roles. Native layouts are independently checked by the compiler.
struct PlayerFloat3{float x,y,z;};
struct Player{PlayerFloat3 drawPosition;};
struct GuiView{AnmLoadedView *frontAnm;};
struct StageScriptView{unsigned int flags;};
typedef StageScriptView GameChainOwnerView;
struct HostEnemy{struct{struct{PlayerFloat3 position;}worldMotion;}runtime;};
struct EnemyManagerView{HostEnemy *specialEnemySlots[8];};
static Player player;Player *g_Player=&player;
static GuiView guiA,guiB;GuiView *g_GuiView=&guiA;
static StageScriptView stage;GameChainOwnerView *g_GameAnimationSlots=&stage;
static HostEnemy enemy,otherEnemy;static EnemyManagerView enemyManager;EnemyManagerView *g_EnemyManager=&enemyManager;
static EnemyGameStateView panel;EnemyGameStateView *g_EnemyGameState=&panel;
alignas(4) static unsigned char stats[6*0x437c];unsigned char *g_EnemySpellStatistics=stats;
int g_GameStatisticsIndex0,g_GameStatisticsIndex1;float g_AnmGameSpeed=1.0f;
static float panelSpeed=1.0f;
static std::vector<std::string> events;
static std::function<void(const std::string&)> after;
static void event(const std::string&s){events.push_back(s);if(after)after(s);}
static std::map<AnmVmView*,std::string> slots;
struct SpriteCall{AnmLoadedView *resource;std::string slot;int index;};
static std::vector<SpriteCall> sprites;
static void HostObserveSprite(AnmLoadedView *resource,AnmVmView *vm,int index){sprites.push_back({resource,slots.at(vm),index});}
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
AnmLoadedView::AnmLoadedView(){std::memset(this,0,sizeof(*this));}
AnmRenderManagerView::AnmRenderManagerView(){}
AnmRenderManagerView::~AnmRenderManagerView(){}
static AnmRenderManagerView managerA,managerB;AnmRenderManagerView *g_AnmRenderManagerView=&managerA;
static AnmLoadedView resourceA,resourceB;
static AnmSpriteView spriteData[128];
static AnmRawInstructionView instruction;static AnmRawInstructionView *scriptData[128];
static AnmVmView managedA[4],managedB[4],child;
static AnmVmLayerNodeView nodesA[4],nodesB[4];
int AnmRenderManagerView::ExecuteScript(AnmVmView *vm){event("exec:"+slots.at(vm));return -1;}
#include "helpers.inc"
#include "owner.inc"
static void writeCounts(int group,int spell,int attempts,int captures){auto *p=reinterpret_cast<int*>(stats+group*0x437c+spell*0x90+0x624);p[0]=attempts;p[1]=captures;}
static void reset(int frame=0,unsigned flags=3){
 after={};events.clear();sprites.clear();slots.clear();std::memset(&panel,0,sizeof(panel));std::memset(stats,0,sizeof(stats));
 panel.flags378C=flags;panel.spellTimer.current=frame;panel.spellTimer.previous=-7;panel.spellTimer.subframe=float(frame);panel.spellTimer.scale=&panelSpeed;panel.spellTimer.flags=1;
 panel.currentSpellBonus=12345678;panel.initialSpellBonus=20000000;panel.spellDuration=600;panel.managedVmId=203;
 panel.effectPosition=AnmFloat3View(0,10,-5);player.drawPosition={0,200,0};g_Player=&player;g_GuiView=&guiA;g_GameAnimationSlots=&stage;stage.flags=0xf1;
 g_GameStatisticsIndex0=g_GameStatisticsIndex1=0;g_EnemySpellStatistics=stats;writeCounts(0,0,42,17);
 enemy.runtime.worldMotion.position={100,30,15};otherEnemy.runtime.worldMotion.position={-100,-30,-15};enemyManager={};enemyManager.specialEnemySlots[0]=&enemy;enemyManager.specialEnemySlots[1]=&otherEnemy;g_EnemyManager=&enemyManager;
 resourceA.rawData=&resourceA;resourceB.rawData=&resourceB;resourceA.pendingLoadCount=resourceB.pendingLoadCount=0;resourceA.sprites=resourceB.sprites=spriteData;resourceA.scripts=resourceB.scripts=scriptData;guiA.frontAnm=&resourceA;guiB.frontAnm=&resourceB;
 for(int i=0;i<128;++i){spriteData[i]={};spriteData[i].width=16;spriteData[i].height=32;spriteData[i].horizontalScale=spriteData[i].verticalScale=1;spriteData[i].textureWidth=spriteData[i].textureHeight=64;scriptData[i]=&instruction;}
 for(int i=0;i<2;++i)slots[&panel.stageVms[i]]="s"+std::to_string(i);
 for(int i=0;i<8;++i){slots[&panel.frontVms[i]]="b"+std::to_string(i);panel.frontVms[i].scriptIndex=77;panel.frontVms[i].scriptTimer.current=51;}
 for(int i=0;i<5;++i){slots[&panel.secondaryFrontVms[i]]="c"+std::to_string(i);panel.secondaryFrontVms[i].scriptIndex=77;panel.secondaryFrontVms[i].scriptTimer.current=51;}
 for(int i=0;i<4;++i){std::memset(&managedA[i],0,sizeof(managedA[i]));std::memset(&managedB[i],0,sizeof(managedB[i]));managedA[i].id=managedB[i].id=200+i;nodesA[i]={};nodesB[i]={};nodesA[i].owner=&managedA[i];nodesB[i].owner=&managedB[i];nodesA[i].next=i<3?&nodesA[i+1]:NULL;nodesB[i].next=i<3?&nodesB[i+1]:NULL;}
 std::memset(&child,0,sizeof(child));managedA[0].layerNode.next=&child.layerNode;child.layerNode.owner=&child;panel.panelVmIds[0].value=200;panel.panelVmIds[1].value=201;panel.panelVmIds[2].value=202;
 managerA.primaryVmListHead=&nodesA[0];managerA.secondaryVmListHead=NULL;managerB.primaryVmListHead=&nodesB[0];managerB.secondaryVmListHead=NULL;g_AnmRenderManagerView=&managerA;
 panelSpeed=g_AnmGameSpeed=1;events.clear();sprites.clear();
}
static bool near(float a,float b){return std::abs(a-b)<.0001f;}
static void update(){CHECK(EnemySpellPanelUpdateCallback(&panel)==1,"callback-return");}
static void suite(){
 reset(0,0);g_Player=NULL;g_GuiView=NULL;g_GameAnimationSlots=NULL;g_EnemyManager=NULL;g_AnmRenderManagerView=NULL;g_EnemySpellStatistics=NULL;update();CHECK(events.empty()&&sprites.empty()&&panel.spellTimer.current==0,"inactive-short-circuit");
 reset();update();CHECK(sprites.size()==12&&events.size()==15&&panel.frontVms[0].scriptIndex==77&&panel.frontVms[0].scriptTimer.current==51,"sprite-binding");
 CHECK(sprites[0].index==31&&sprites[7].index==38&&sprites[8].index==34&&sprites[9].index==32&&sprites[10].index==31&&sprites[11].index==37,"bonus-digits");
 CHECK(near(panel.effectPosition.x,5)&&near(panel.effectPosition.y,11)&&near(panel.effectPosition.z,-4)&&near(managedA[3].positionOffset.x,229)&&near(managedA[3].positionOffset.y,27),"marker-interpolation");
 reset(59);update();CHECK(stage.flags==0xf1&&panel.spellTimer.current==60,"stage-pre-timer");reset(60);update();CHECK(stage.flags==0xf0,"stage-pre-timer");
 reset(299);panel.currentSpellBonus=10005;panel.initialSpellBonus=20000;update();CHECK(panel.currentSpellBonus==10005,"decay-pre-timer");
 reset(300);panel.currentSpellBonus=10005;panel.initialSpellBonus=20000;update();CHECK(panel.currentSpellBonus==9940&&panel.initialSpellBonus==20000,"bonus-source-slots");
 reset(300,11);panel.currentSpellBonus=10005;panel.initialSpellBonus=20000;update();CHECK(panel.currentSpellBonus==10005,"bonus-freeze");panel.spellDuration=300;update();CHECK(panel.currentSpellBonus==10005,"freeze-skips-zero-denominator");
 reset(300);panel.currentSpellBonus=-12345;panel.initialSpellBonus=0;update();CHECK(panel.currentSpellBonus==-12340&&sprites[3].index==29&&sprites[4].index==28&&sprites[7].index==30,"signed-bonus-digits");
 reset(119);player.drawPosition.y=95;update();CHECK((panel.flags378C&4)!=0,"panel-post-timer");for(int i=0;i<8;++i)CHECK(panel.frontVms[i].pendingInterrupt==3,"hide-vm-ranges");for(int i=0;i<5;++i)CHECK(panel.secondaryFrontVms[i].pendingInterrupt==3,"hide-vm-ranges");for(int i=0;i<3;++i)CHECK(managedA[i].pendingInterrupt==3,"hide-id-ranges");CHECK(child.pendingInterrupt==3,"hide-child-propagation");
 reset(120);player.drawPosition.y=96;update();CHECK((panel.flags378C&4)==0,"panel-hide-boundary");reset(120,7);player.drawPosition.y=128;update();CHECK((panel.flags378C&4)!=0,"panel-show-boundary");
 reset(120,7);player.drawPosition.y=129;update();CHECK((panel.flags378C&4)==0&&managedA[0].pendingInterrupt==2&&panel.frontVms[7].pendingInterrupt==2&&panel.secondaryFrontVms[4].pendingInterrupt==2,"show-vm-ranges");
 for(unsigned flags:{3u,7u}){reset(120,flags);player.drawPosition.y=std::numeric_limits<float>::quiet_NaN();update();CHECK(panel.flags378C==flags,"unordered-panel-position");}
 reset(0,1);update();CHECK(sprites.size()==5&&sprites[0].slot=="b2"&&sprites[0].index==41&&events.size()==8,"no-bonus-symbol");
 reset();after=[](const std::string&s){if(s=="exec:b0"){panel.currentSpellBonus=0;g_GuiView=&guiB;writeCounts(5,1,100,-17);g_GameStatisticsIndex0=1;g_GameStatisticsIndex1=2;panel.spellId=1;}};update();CHECK(sprites[1].index==32&&sprites[7].index==38,"bonus-captured-value");CHECK(sprites[0].resource==&resourceA&&sprites[1].resource==&resourceB&&sprites[8].resource==&resourceB,"counter-gui-after-bonus");CHECK(sprites[8].index==39&&sprites[9].index==39&&sprites[10].index==29&&sprites[11].index==23,"counter-stats-after-bonus");
 reset();after=[](const std::string&s){if(s=="exec:c4"){g_AnmRenderManagerView=&managerB;enemy.runtime.worldMotion.position={200,50,35};}};update();CHECK(near(panel.effectPosition.x,10)&&near(panel.effectPosition.y,12)&&near(panel.effectPosition.z,-3)&&near(managedB[3].positionOffset.x,234)&&managedA[3].positionOffset.x==0,"late-world-owner");
 for(float speed:{0.5f,0.99f,1.0f,1.01f,2.0f}){reset(10);panel.spellTimer.subframe=100.5f;panelSpeed=speed;update();if(speed==1)CHECK(panel.spellTimer.previous==10&&panel.spellTimer.current==11&&near(panel.spellTimer.subframe,101.5f),"timer-path");else CHECK(panel.spellTimer.current==int(100.5f+speed),"timer-path");}
 reset();after=[](const std::string&s){if(s=="exec:s1"){panel.spellTimer.current=119;panel.spellTimer.subframe=119;player.drawPosition.y=95;}};update();CHECK((panel.flags378C&4)!=0&&stage.flags==0xf1,"timer-after-stage-execution");
 reset();resourceA.rawData=NULL;update();CHECK(events.size()==15&&sprites.size()==12&&panel.frontVms[0].activeSpriteIndex==0,"ignore-sprite-failure");
}
int main(){try{suite();}catch(const Failure&f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
