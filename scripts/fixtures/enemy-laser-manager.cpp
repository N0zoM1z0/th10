// Explicit host-only nonnative storage roles. No native vtable/layout claim.
#define __stdcall
#define __fastcall
#define __declspec(x)
#include <cassert>
#include <cstring>
#include <cstdio>
#include <vector>
#include "Chain.hpp"
#include "GameErrorContext.hpp"
#include "expected.inc"
static int baseCalls;
struct EnemyLaserConstructionBaseView { EnemyLaserConstructionBaseView *previous,*next; int id; EnemyLaserConstructionBaseView(){++baseCalls;} int Unlink(); };
using EnemyLaserCommonStorageView=EnemyLaserConstructionBaseView;
struct AnmLoadedView {};
struct EnemyLaserManagerStorageView;
using EnemyLaserManagerView=EnemyLaserManagerStorageView;
EnemyLaserManagerView *g_EnemyLaserManager;
struct EnemyLaserManagerStorageView { unsigned char prefix[8]; th10::ChainElem *updateTask,*drawTask; EnemyLaserConstructionBaseView sentinel; EnemyLaserCommonStorageView *tail; int count; unsigned nextId; AnmLoadedView *effectResource; EnemyLaserManagerStorageView(); int Initialize(); };
struct MainRole { int criticalSections[7]; unsigned char criticalSectionLockCounts[7]; } g_MainSupervisorView;
struct RenderRole { AnmLoadedView *PreloadAnm(int,const char*); } renderer,*g_AnmRenderManagerView=&renderer;
static AnmLoadedView resource;
static bool failPreload,mutate;
static std::vector<int> events;
static th10::Chain chains[4];
static th10::ChainElem tasks[3];
static int allocCount,unregisterCount,unlinkCount;
static EnemyLaserManagerStorageView *active;
namespace th10 { Chain *g_Chain=&chains[0]; GameErrorContext g_GameErrorContext; ChainElem::ChainElem(){std::memset(this,0,sizeof(*this));} Chain::Chain(){} }
int EnemyLaserUpdateCallback(void*){return 0;} int EnemyLaserDrawCallback(void*){return 0;}
AnmLoadedView *RenderRole::PreloadAnm(int slot,const char *name){assert(slot==7&&!std::strcmp(name,"bullet.anm"));if(!active)active=g_EnemyLaserManager;assert(g_EnemyLaserManager==active);events.push_back(1);return failPreload?nullptr:&resource;}
char *th10::GameErrorContext::Log(char *fmt,...){assert(!active->effectResource);assert(std::strlen(fmt)==50);assert(!std::memcmp(fmt,expectedError,51));events.push_back(2);return nullptr;}
th10::ChainElem *th10::Chain::AllocElem(ChainCallback cb){auto *t=&tasks[allocCount++];t->flags=0xf;t->callback=cb;return t;}
int th10::Chain::AddToCalcChain(ChainElem *t,int p,Chain *c){assert(t==&tasks[0]&&p==0x13&&c==&chains[0]);assert(t->flags==0xd&&t->arg==active&&t->callback==EnemyLaserUpdateCallback);assert(!active->updateTask);if(mutate){active->updateTask=&tasks[2];g_Chain=&chains[1];}events.push_back(3);return -1;}
int th10::Chain::AddToDrawChain(ChainElem *t,int p,Chain *c){assert(active->updateTask==&tasks[0]);assert(t==&tasks[1]&&p==0x1b&&c==&chains[mutate?1:0]);assert(t->flags==0xd&&t->arg==active&&t->callback==EnemyLaserDrawCallback);if(mutate)active->drawTask=&tasks[2];events.push_back(4);return -1;}
static th10::Chain *captured;
void EnterCriticalSection(int *p){assert(p==&g_MainSupervisorView.criticalSections[0]);captured=th10::g_Chain;if(mutate){th10::g_Chain=&chains[3];if(!unregisterCount)active->updateTask=&tasks[2];else active->drawTask=&tasks[1];}events.push_back(5);}
void LeaveCriticalSection(int*){assert(g_MainSupervisorView.criticalSectionLockCounts[0]==0);events.push_back(7);}
void th10::Chain::UnregisterElem(ChainElem *t,Chain *c){assert(c==captured);assert(g_MainSupervisorView.criticalSectionLockCounts[0]==0);assert(t==&tasks[unregisterCount? (mutate?2:1):0]);++unregisterCount;if(mutate){active->drawTask=&tasks[2];g_Chain=&chains[2];}events.push_back(6);}
static EnemyLaserCommonStorageView alternate;
int EnemyLaserConstructionBaseView::Unlink(){++unlinkCount;events.push_back(8);if(mutate&&id==1){previous->next=next;if(next)next->previous=previous;previous=&alternate;alternate.next=this;next=nullptr;}return -1;}
#include "owner.inc"
static void reset(){events.clear();allocCount=unregisterCount=unlinkCount=0;failPreload=false;mutate=false;th10::g_Chain=&chains[0];g_MainSupervisorView.criticalSectionLockCounts[0]=255;}
int main(){
 reset(); int before=baseCalls; EnemyLaserManagerStorageView m;active=&m;assert(baseCalls==before+1&&g_EnemyLaserManager==&m);assert(!m.sentinel.next&&!m.sentinel.previous&&!m.tail&&!m.count&&!m.nextId&&!m.effectResource&&!m.updateTask&&!m.drawTask);
 failPreload=true;m.effectResource=&resource;assert(m.Initialize()==-1);assert((events==std::vector<int>{1,2})&&!m.effectResource&&!allocCount);GameCleanupLinked41C1C0(reinterpret_cast<GameChainOwnerView*>(&m));assert(!g_EnemyLaserManager&&!unregisterCount);
 for(bool change:{false,true}){reset();mutate=change;EnemyLaserManagerStorageView live;active=&live;assert(live.Initialize()==0);assert(live.updateTask==&tasks[0]&&live.drawTask==&tasks[1]&&live.tail==&live.sentinel&&live.effectResource==&resource);
 auto *a=new EnemyLaserCommonStorageView;auto *b=new EnemyLaserCommonStorageView;a->id=1;b->id=2;live.sentinel.next=a;a->previous=&live.sentinel;a->next=b;b->previous=a;b->next=nullptr;
 GameCleanupLinked41C1C0(reinterpret_cast<GameChainOwnerView*>(&live));assert(!g_EnemyLaserManager&&unregisterCount==2&&unlinkCount==2&&g_MainSupervisorView.criticalSectionLockCounts[0]==255);assert(live.effectResource==&resource);assert((events==std::vector<int>{1,3,4,5,6,7,5,6,7,8,8}));assert(!live.sentinel.next);if(change)assert(!alternate.next);
 }
 // Factory success/failure: Preload observes constructor publication.
 // Allocation-null is a structural-only hazard; never execute null-this UB.
 reset();active=nullptr;auto *created=static_cast<EnemyLaserManagerStorageView*>(GameCreateOwner41C290());assert(created==active&&created==g_EnemyLaserManager);GameCleanupLinked41C1C0(reinterpret_cast<GameChainOwnerView*>(created));::operator delete(created);
 reset();active=nullptr;failPreload=true;assert(!GameCreateOwner41C290()&&!g_EnemyLaserManager&&allocCount==0);
 std::puts("PASS manager lifecycle host roles");
}
