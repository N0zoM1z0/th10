#define __stdcall
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#include "EnemyLaserLine.hpp"
#include "ItemManager.hpp"
#include "EnemyPrimaryResources.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <limits>
#include <vector>
struct Failure{const char *label;};
#define CHECK(x,label) do{if(!(x))throw Failure{label};}while(0)
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
AnmLoadedView::AnmLoadedView(){std::memset(this,0,sizeof(*this));}
AnmRenderManagerView::AnmRenderManagerView(){}
AnmRenderManagerView::~AnmRenderManagerView(){}
ItemManagerView::ItemManagerView(){}
ItemManagerView::~ItemManagerView(){}
static EnemyLaserLineView line;
static AnmLoadedView resourceA,resourceB,primaryResource;
static EnemyLaserManagerStorageView managerA,managerB;
EnemyLaserManagerView *g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&managerA);
static EnemyPrimaryResourceOwnerView owner;EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner=&owner;
static AnmRenderManagerView rendererA,rendererB,rendererC;
AnmRenderManagerView *g_AnmRenderManagerView=&rendererA;
static ItemManagerView itemsA,itemsB,skippedItems;
ItemManagerView *g_ItemManager=&itemsA;static ItemManagerView *skipItemManager=&skippedItems;
float g_PlayerTimerScale=1;short g_PlayerPower=0;
struct Bind{AnmLoadedView *resource;AnmVmView *vm;int script;AnmFloat3View point;};
static std::vector<Bind> binds;static std::function<void()> afterScript;
void AnmVmView::Initialize(){} // HOST pure reset role, allocator fallback unexercised.
void AnmVmView::InitializeForLoadedScript(AnmLoadedView*,int){throw Failure{"item-kind"};}
void AnmLoadedView::InitializeAndExecuteScriptIndex(AnmVmView *vm,int script){
 CHECK(line.unknown00C==77,"retire-after-scripts");
 CHECK((vm->flags35C&0x40000000u)!=0&&vm->renderLayer==0,"created-vm-flags");
 CHECK(vm->position.x==81&&vm->position.y==82&&vm->position.z==83,"world-offset-field");
 binds.push_back({this,vm,script,vm->positionOffset});
 if(afterScript)afterScript(); // HOST execution role; real script callbacks can change live globals.
}
void EnemyLaserVectorView::FromAngleMagnitude(float a,float n){x=std::cos(a)*n;y=std::sin(a)*n;} // HOST polar: XY only, no Z read/store/callback.
void ItemVectorView::FromAngleMagnitude(float a,float n){x=std::cos(a)*n;y=std::sin(a)*n;}
#define UNUSED_VOID(name) void EnemyLaserLineView::name(){throw Failure{"unexpected-slot"};}
UNUSED_VOID(RunCommands) UNUSED_VOID(UnknownSlot10) UNUSED_VOID(UnknownSlot1C) UNUSED_VOID(UnknownSlot20)
UNUSED_VOID(Motion0001) UNUSED_VOID(Motion0010) UNUSED_VOID(Motion0020) UNUSED_VOID(Motion0040)
UNUSED_VOID(Motion0100) UNUSED_VOID(Motion0080) UNUSED_VOID(Bounce) UNUSED_VOID(Motion100000)
UNUSED_VOID(Motion200000) UNUSED_VOID(Motion4000000)
int EnemyLaserLineView::Initialize(const EnemyLaserType0RequestView*){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::Update(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::Draw(){throw Failure{"unexpected-slot"};}
int EnemyLaserLineView::CheckCollisionBox(EnemyLaserVectorView*,EnemyLaserVectorView*,int){throw Failure{"unexpected-slot"};}
#include "helpers.inc"
#include "owner.inc"
static bool near(float a,float b){return std::abs(a-b)<.0001f;}
static void reset(){
 afterScript={};binds.clear();line.unknown00C=77;line.position={0,0,7};line.angle=0;line.length=42;line.request.color=-2;line.deletionCounter=31;line.id=92;line.flags=0x102;line.width=3;line.velocity={2,3,4};
 managerA.effectResource=&resourceA;managerB.effectResource=&resourceB;g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&managerA);owner.primaryEnemyResource=&primaryResource;
 for(auto *r:{&rendererA,&rendererB,&rendererC}){r->primaryVmListHead=r->primaryVmListTail=nullptr;r->nextVmPoolIndex=0;r->nextVmId=AnmVmIdView();std::fill(std::begin(r->vmPoolUsed),std::end(r->vmPoolUsed),0);for(int i=0;i<16;++i){auto &v=r->vmPool[i];v.position=AnmFloat3View(81,82,83);v.flags35C=0x208;v.renderLayer=11;v.id=-1;}}
 g_AnmRenderManagerView=&rendererA;
 for(auto *m:{&itemsA,&itemsB,&skippedItems}){m->delayedItemCursor=0;m->delayedSpawnRequests=0;for(int i=0;i<32;++i){auto &r=m->items[150+i];r.active=0;r.kind=-4;r.spriteKind=-5;r.timer.flags=0;r.timer.scale=nullptr;r.unknown3C4=99;}}
 g_ItemManager=&itemsA;g_PlayerTimerScale=1;g_PlayerPower=0;
}
static void cases(){
 reset();int n=line.Clear(0);CHECK(n==3&&binds.size()==3,"sample-count");CHECK(near(binds[0].point.x,230),"first-sample");CHECK(binds[0].point.z==7&&binds[1].point.z==7&&binds[2].point.z==7,"sample-z");CHECK(near(binds[1].point.x,242)&&near(binds[2].point.x,254),"sample-stride");CHECK(binds[0].point.y==16,"world-offsets");CHECK(binds[0].script==13&&binds[1].script==13,"signed-live-script");CHECK(binds[0].resource==&resourceA,"live-resource");CHECK(itemsA.delayedSpawnRequests==0,"mode-gate");CHECK(line.unknown00C==1,"retired-at-end");CHECK(line.id==92&&line.flags==0x102&&line.deletionCounter==31&&line.velocity.z==4,"preserved-line-state");
 CHECK(rendererA.primaryVmListHead==&rendererA.vmPool[0].managerNode&&rendererA.primaryVmListTail==&rendererA.vmPool[2].managerNode,"primary-list-order");CHECK(rendererA.vmPool[1].managerNode.previous==&rendererA.vmPool[0].managerNode&&rendererA.vmPool[1].managerNode.next==&rendererA.vmPool[2].managerNode&&rendererA.vmPool[2].id==3,"primary-list-links");
 reset();line.position.z=-0.0f;line.Clear(0);CHECK(std::signbit(binds[0].point.z)&&!std::signbit(binds[1].point.z),"zero-z-addition");
 reset();line.angle=1.5707964f;CHECK(line.Clear(0)==3,"sample-count");CHECK(near(binds[1].point.y,34)&&near(binds[2].point.y,46),"angled-stride");
 for(float length:{-1.f,0.f,12.f}){reset();line.length=length;CHECK(line.Clear(0)==0&&binds.empty()&&line.unknown00C==1,"initial-boundary");}
 reset();line.length=std::numeric_limits<float>::quiet_NaN();CHECK(line.Clear(0)==0&&binds.empty(),"initial-unordered");
 reset();line.length=24;CHECK(line.Clear(0)==1&&binds.size()==1,"tail-boundary");
 reset();line.length=36;afterScript=[](){line.length=std::numeric_limits<float>::quiet_NaN();if(binds.size()>1)line.length=0;};CHECK(line.Clear(0)==1,"live-unordered-length");
 reset();line.length=13;CHECK(line.Clear(-7)==1,"sample-count");auto &item=itemsA.items[150];CHECK(item.active==5&&item.kind==8&&item.spriteKind==8,"item-kind");CHECK(near(item.velocity.x,0)&&near(item.velocity.y,-.6f)&&item.velocity.z==0,"item-velocity");CHECK(item.worldPosition.x==6&&item.worldPosition.y==0&&item.worldPosition.z==7,"item-point");CHECK(item.timer.current==0&&item.timer.subframe==0&&item.timer.previous==-1&&item.timer.scale==&g_PlayerTimerScale&&item.unknown3C4==0,"item-timer");
 reset();line.length=13;line.position.x=-229.5;line.Clear(1);CHECK(itemsA.delayedSpawnRequests==1,"expanded-bounds");
 for(auto p:{AnmFloat3View(-230,0,7),AnmFloat3View(218,0,7),AnmFloat3View(0,-32,7),AnmFloat3View(0,480,7)}){reset();line.position={p.x,p.y,p.z};line.length=13;line.Clear(1);CHECK(itemsA.delayedSpawnRequests==0,"strict-bounds");}
 reset();line.position.x=std::numeric_limits<float>::quiet_NaN();line.length=13;line.Clear(1);CHECK(itemsA.delayedSpawnRequests==1&&std::isnan(itemsA.items[150].worldPosition.x),"unordered-bounds");
 reset();afterScript=[](){line.length=0;};CHECK(line.Clear(0)==1,"live-length");
 reset();afterScript=[](){line.request.color=-7;g_EnemyLaserManager=reinterpret_cast<EnemyLaserManagerView*>(&managerB);line.position={300,400,500};line.angle=2;};CHECK(line.Clear(0)==3,"sample-count");CHECK(binds[1].script==3&&binds[2].script==3,"signed-live-script");CHECK(binds[1].resource==&resourceB&&binds[2].resource==&resourceB,"live-resource");CHECK(near(binds[1].point.x,242)&&binds[1].point.y==16&&binds[1].point.z==7,"captured-geometry");
 reset();line.length=13;afterScript=[](){g_ItemManager=&itemsB;g_AnmRenderManagerView=&rendererB;};line.Clear(1);CHECK(itemsA.delayedSpawnRequests==0&&itemsB.delayedSpawnRequests==1&&skippedItems.delayedSpawnRequests==0,"live-item-manager");CHECK(rendererA.primaryVmListHead==nullptr&&rendererB.primaryVmListHead==&rendererA.vmPool[0].managerNode,"post-script-renderer");
 reset();afterScript=[](){g_AnmRenderManagerView=binds.size()==1?&rendererB:&rendererC;};line.Clear(0);CHECK(binds[0].vm==&rendererA.vmPool[0]&&binds[1].vm==&rendererB.vmPool[0]&&binds[2].vm==&rendererC.vmPool[0],"live-allocation-owner");CHECK(rendererB.primaryVmListHead==&rendererA.vmPool[0].managerNode&&rendererC.primaryVmListTail==&rendererC.vmPool[0].managerNode,"live-registration-owner");
 reset();line.length=13;rendererA.nextVmId=AnmVmIdView(-1);line.Clear(0);CHECK(rendererA.vmPool[0].id==1&&rendererA.nextVmId.value==1,"zero-id-skipped");
 reset();line.length=13;itemsA.items[150].active=9;line.Clear(1);CHECK(itemsA.delayedSpawnRequests==1&&itemsA.delayedItemCursor==1&&itemsA.items[150].active==9,"occupied-item-ring");
}
int main(){try{cases();}catch(const Failure&f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
