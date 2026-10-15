#!/usr/bin/env python3
"""Maintained lifecycle/header with synthetic archive, ANM and chain contracts.
Host pointers/layout are not the 32-bit ABI; fixture offsets use host pointers.
No game file IO or real callbacks run; unsafe null-new/count paths are not run.
"""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class StageLifecycle(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_lifecycle(self):
  header=(ROOT/'src/StageScript.hpp').read_text();header=header.replace('#pragma once','').replace('#include "AnmManager.hpp"','');header=header[:header.index('typedef char StageScriptLayoutChecks[')]
  src=(ROOT/'src/StageLifecycle.cpp').read_text();src='\n'.join(l for l in src.splitlines() if not l.startswith('#include'))
  h=(ROOT/'src/AnmManager.hpp').read_text();a=h.index('struct AnmVmTimerView\n{');timer=h[a:h.index('\n};',a)+3]
  program=r'''
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#define __stdcall
#define __fastcall
float g_AnmGameSpeed=1;
struct AnmFloat3View {float x,y,z;AnmFloat3View(){} AnmFloat3View(float a,float b,float c):x(a),y(b),z(c){}};
struct AnmMatrixView {float v[16];};
struct AnmViewportView {unsigned x,y,w,h;float minZ,maxZ;};
'''+timer+r'''
struct AnmVmFloat3InterpolationView {AnmFloat3View a,b,c,d;AnmVmTimerView timer;int duration,mode;};
static std::vector<const void*> constructed,destroyed;
struct AnmVmView {unsigned char bytes[0x3ac];AnmVmView(){memset(bytes,0x5a,sizeof(bytes));constructed.push_back(this);}~AnmVmView(){destroyed.push_back(this);}};
static std::vector<const void*> deletedLoaded;
struct AnmLoadedView {void Release();~AnmLoadedView(){deletedLoaded.push_back(this);}};
'''+header+r'''
static std::vector<std::string> events;
static bool failOpen=false,failAnm=false,failVmAllocation=false,replaceSlot=false;
static AnmLoadedView *replacementLoaded=nullptr;
static int fileReads=0,vmCount=2,expectedPriority=0;
static StageScriptView *observed=nullptr;
static std::vector<void*> owned;
static void *stageMalloc(size_t n){events.push_back("malloc="+std::to_string(n));if(failVmAllocation&&n==size_t(vmCount)*sizeof(AnmVmView))return nullptr;void*p=std::malloc(n);assert(p);owned.push_back(p);return p;}
static void stageFree(void*p){assert(std::find(owned.begin(),owned.end(),p)!=owned.end());owned.erase(std::find(owned.begin(),owned.end(),p));events.push_back("free");std::free(p);}
namespace th10 {
 typedef int (*ChainCallback)(void*);
 struct ChainElem {unsigned flags;void*arg;ChainCallback callback;};
 struct Chain {static ChainElem *AllocElem(ChainCallback cb);static int AddToCalcChain(ChainElem*,int,Chain*);static int AddToDrawChain(ChainElem*,int,Chain*);static void UnregisterElem(ChainElem*,Chain*);};
 Chain base,other;Chain *g_Chain=&base;
 struct Logger {void Log(const char*){events.push_back("log");}void Fatal(const char*){events.push_back("fatal");}} g_GameErrorContext;
}
static th10::ChainElem nodes[3];static int nodeIndex=0;
StageScriptView *g_StagePrimary=nullptr,*g_StageSecondary=nullptr;
int g_GameStartupMode=1;unsigned g_GameRuntimeFlags=0;
char g_StageFilePath[512];StageCameraStateView g_StagePublishedCamera;
unsigned char g_AnmChainCriticalSection[24],g_AnmChainMutationDepth=0;
static th10::Chain *expectedRemovedChain=nullptr;
void EnterCriticalSection(void*p){assert(p==g_AnmChainCriticalSection);expectedRemovedChain=th10::g_Chain;th10::g_Chain=th10::g_Chain==&th10::base?&th10::other:&th10::base;events.push_back("enter");}
void LeaveCriticalSection(void*p){assert(p==g_AnmChainCriticalSection&&g_AnmChainMutationDepth==1);events.push_back("leave");}
th10::ChainElem *th10::Chain::AllocElem(ChainCallback cb){assert(nodeIndex<3);ChainElem*e=&nodes[nodeIndex++];e->flags=3;e->callback=cb;e->arg=nullptr;events.push_back("alloc-node");return e;}
int th10::Chain::AddToCalcChain(ChainElem*e,int p,Chain*c){assert(p==expectedPriority+12&&c==g_Chain&&e->flags==1&&e->arg==observed);assert(e->callback==reinterpret_cast<ChainCallback>(StageUpdateCallback));events.push_back("calc");return 0;}
int th10::Chain::AddToDrawChain(ChainElem*e,int p,Chain*c){assert(c==g_Chain&&e->flags==1&&e->arg==observed);if(nodeIndex==2){assert(p==expectedPriority+7&&e->callback==reinterpret_cast<ChainCallback>(StageDrawEarlyCallback));}else{assert(p==expectedPriority+10&&e->callback==reinterpret_cast<ChainCallback>(StageDrawLateCallback));}events.push_back("draw");return 0;}
void th10::Chain::UnregisterElem(ChainElem*e,Chain*c){assert(c==expectedRemovedChain&&g_AnmChainMutationDepth==1);events.push_back("remove="+std::to_string(e-nodes));}
int StageDrawEarlyCallback(StageScriptView*){return 1;}int StageDrawLateCallback(StageScriptView*){return 1;}int StageScriptView::Update(){return 1;}
struct AnmRenderManagerView {AnmLoadedView *loadedAnms[33]={};AnmLoadedView *PreloadAnm(int index,const char*path){assert(index==5&&strcmp(path,"stage.anm")==0);events.push_back("anm");if(observed==nullptr)observed=g_StagePrimary?g_StagePrimary:g_StageSecondary;if(failAnm)return nullptr;if(!loadedAnms[index])loadedAnms[index]=new AnmLoadedView;return loadedAnms[index];}} manager;
AnmRenderManagerView *g_AnmRenderManagerView=&manager;
void AnmLoadedView::Release(){events.push_back("release");if(replaceSlot){assert(manager.loadedAnms[5]==this);manager.loadedAnms[5]=new AnmLoadedView;replacementLoaded=manager.loadedAnms[5];}}
namespace FileSystem {unsigned char *OpenFile(const char*path,int*size,int mode){assert(mode==0&&strcmp(path,"stage.std")==0);++fileReads;events.push_back("open");if(failOpen)return nullptr;*size=512;unsigned char*p=static_cast<unsigned char*>(stageMalloc(*size));memset(p,0,*size);StageDataHeaderView*h=reinterpret_cast<StageDataHeaderView*>(p);h->objectCount=2;h->objectVmCount=short(vmCount);h->instancesOffset=256;h->scriptOffset=320;strcpy(h->anmPath,"stage.anm");StageObjectView**table=reinterpret_cast<StageObjectView**>(h+1);table[0]=reinterpret_cast<StageObjectView*>(192);table[1]=reinterpret_cast<StageObjectView*>(224);return p;}}
#define malloc stageMalloc
#define free stageFree
'''+src+r'''
#undef malloc
#undef free
static void reset(){assert(owned.empty());events.clear();constructed.clear();destroyed.clear();deletedLoaded.clear();nodeIndex=0;fileReads=0;observed=nullptr;failOpen=failAnm=failVmAllocation=replaceSlot=false;g_GameRuntimeFlags=0;g_StagePrimary=g_StageSecondary=nullptr;g_AnmChainMutationDepth=0;expectedPriority=0;th10::g_Chain=&th10::base;g_StagePublishedCamera.value048=0.25f;}
static std::vector<const void*> expectedDestruction(StageScriptView*s){std::vector<const void*> e;for(int i=0;i<3;++i)e.push_back(&s->auxiliaryVms[2-i]);for(int i=0;i<8;++i)e.push_back(&s->scriptVms[7-i]);return e;}
int main(){
 reset();StageScriptView*s=new StageScriptView;observed=s;
 assert(constructed.size()==11&&s->lifecycleFlags==2&&s->flags==0&&s->originalData==nullptr);
 for(int i=0;i<8;++i){assert(constructed[i]==&s->scriptVms[i]);for(unsigned char b:s->scriptVms[i].bytes)assert(b==0);}
 for(int i=0;i<3;++i)assert(constructed[8+i]==&s->auxiliaryVms[i]);
 assert(s->Initialize("stage.std",0)==0&&g_StagePrimary==s&&g_StageSecondary==nullptr);
 assert(s->originalData!=reinterpret_cast<unsigned char*>(s->data)&&fileReads==1);
 assert(reinterpret_cast<unsigned char*>(s->objects[0])==reinterpret_cast<unsigned char*>(s->data)+192);
 assert(reinterpret_cast<unsigned char*>(s->instances)==reinterpret_cast<unsigned char*>(s->data)+256&&s->scriptBase==reinterpret_cast<unsigned char*>(s->data)+320);
 assert(s->camera.position.z==-600&&s->camera.direction.y==300&&s->camera.direction.z==600&&s->camera.up.y==1&&s->camera.value048==0.25f);
 assert(s->scriptTimer.current==0&&s->scriptTimer.previous==-1&&s->flags==1&&s->cullDistanceSquared==9610000);
 assert(s->updateChain==&nodes[0]&&s->earlyDrawChain==&nodes[1]&&s->lateDrawChain==&nodes[2]);
 events.clear();const auto expected=expectedDestruction(s);delete s;assert(destroyed==expected);assert(owned.empty()&&g_StagePrimary==nullptr&&manager.loadedAnms[5]==nullptr&&g_AnmChainMutationDepth==0);
 assert(events==std::vector<std::string>({"enter","remove=0","leave","enter","remove=1","leave","enter","remove=2","leave","free","free","free","release"}));
 // Cached image is reused without re-opening, while each mutable copy is fresh.
 reset();s=new StageScriptView;s->unknown2A30=1;assert(s->LoadData("stage.std")==0);unsigned char*cache=s->originalData;stageFree(s->data);stageFree(s->objectVms);s->data=nullptr;s->objectVms=nullptr;
 assert(s->LoadData("ignored-when-cached")==0&&s->originalData==cache&&fileReads==1);delete s;assert(owned.empty());
 // Failure before registration retains published owner until normal destruction.
 reset();s=new StageScriptView;observed=s;failOpen=true;expectedPriority=3;assert(s->Initialize("stage.std",3)==-1&&g_StageSecondary==s&&nodeIndex==0);assert(events==std::vector<std::string>({"open","fatal"}));delete s;assert(g_StageSecondary==nullptr);
 reset();s=new StageScriptView;observed=s;failAnm=true;assert(s->Initialize("stage.std",0)==-1&&nodeIndex==0);assert(events[events.size()-2]=="log"&&events.back()=="fatal");delete s;assert(owned.empty());
 // Null raw VM allocation is returned as successful, as in target; never dereferenced.
 reset();s=new StageScriptView;s->unknown2A30=1;failVmAllocation=true;assert(s->LoadData("stage.std")==0&&s->objectVms==nullptr);delete s;assert(owned.empty());
 // Runtime flag retains loaded resource; unrelated published pointers remain.
 reset();s=new StageScriptView;observed=s;expectedPriority=2;assert(s->Initialize("stage.std",2)==0);StageScriptView*other=reinterpret_cast<StageScriptView*>(size_t(16));g_StagePrimary=other;g_GameRuntimeFlags=1;AnmLoadedView*kept=manager.loadedAnms[5];delete s;assert(g_StagePrimary==other&&g_StageSecondary==nullptr&&manager.loadedAnms[5]==kept);delete kept;manager.loadedAnms[5]=nullptr;
 // Release may replace the slot: source must delete/re-zero its post-call value.
 reset();s=new StageScriptView;s->unknown2A30=1;assert(s->LoadData("stage.std")==0);AnmLoadedView*old=manager.loadedAnms[5];replaceSlot=true;delete s;assert(manager.loadedAnms[5]==nullptr);assert(deletedLoaded==std::vector<const void*>({replacementLoaded}));assert(std::find(deletedLoaded.begin(),deletedLoaded.end(),old)==deletedLoaded.end());delete old;
 assert(owned.empty());
 // Real factory source, including failure destruction and null return.
 reset();s=StageCreate("stage.std",0);assert(s&&s==g_StagePrimary&&nodeIndex==3);delete s;assert(owned.empty());
 reset();failOpen=true;s=StageCreate("stage.std",0);assert(!s&&!g_StagePrimary&&destroyed.size()==11&&nodeIndex==0&&owned.empty());
 reset();failAnm=true;s=StageCreate("stage.std",0);assert(!s&&!g_StagePrimary&&destroyed.size()==11&&nodeIndex==0&&owned.empty());
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='stage-life-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
