#!/usr/bin/env python3
"""Focused extracted GUI creation-family host/source contracts.
No target layout/ABI, native execution, allocation-null, vptr or exactness claim.
Real GuiView declaration and function bodies execute with explicit test-only ANM,
timer-construction, resource, chain, filesystem, Win32 and destructor seams.
"""
from pathlib import Path
import argparse
import os
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]

def need(ok, message):
    if not ok:
        raise RuntimeError(message)

def block(text, anchor, semi=False):
    need(text.count(anchor) == 1, 'unique anchor: ' + anchor)
    start = text.index(anchor)
    pos = text.index('{', start)
    depth = 1
    end = pos + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[start:end + int(semi)]

def compact(text):
    return re.sub(r'\s+', '', text)

def structure(bodies):
    need(compact(bodies[0]) == compact('''GuiView::GuiView() {
        memset(this, 0, sizeof(*this)); ownerFlags |= 2; g_GuiView = this; }'''), 'constructor memset/publication contract')
    need(compact(bodies[3]) == compact('''void *ScriptViewerCreateGui() {
        GuiView *gui = new GuiView;
        if (gui->Initialize()) { delete gui; return NULL; } return gui; }'''),
        'factory saved owner/unchecked allocation contract')
    load = bodies[1]
    need(load.index('const char *filename =') < load.index('g_StageFilePath[0] = 0;'), 'filename capture order')
    need(load.index('capturedStage =') < load.index('spellSeconds = -1;'), 'stage capture order')

PREFIX = r'''
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <functional>
#include <vector>
#include <string>
#include <cstddef>
#define __stdcall
#define __fastcall
static void Check(bool ok,const char *name){if(!ok){fprintf(stderr,"CHECK %s\n",name);exit(1);}}
static int members=0, timerCtors=0, destructors=0, locks=0;
// No vptr, raw initialized flags or production ANM resources: explicit host seams.
struct AnmVmView { int sentinel; AnmVmView():sentinel(++members){} };
struct AnmVmIdView { int value; };
struct AnmFloat3View {float x,y,z;};
struct AnmVmTimerView {int previous,current;float subframe;float *scale;unsigned flags;
 AnmVmTimerView():previous(0),current(0),subframe(0),scale(nullptr),flags(0){++timerCtors;} };
struct AnmLoadedView {int id;};
struct GuiMessageVmView;
static float g_AnmGameSpeed=1;
alignas(int) static unsigned char stageBytes[64];
static unsigned char *g_GameStageState=stageBytes;
alignas(void*) static unsigned char routeBytes[128];
static void *g_ScriptViewerStageRoute=routeBytes;
static char pathBytes[256];
static char *g_StageFilePath=pathBytes; // array-name binding seam, enables live alias witness
static void *g_GuiRetainedMessageFile491BE8=nullptr;
static std::function<void(int)> preloadHook;
static std::function<void()> openHook,allocHook,calcHook,drawHook;
static std::vector<std::string> events;
static AnmLoadedView front{6},stage{28};
static bool failFront=false,failStage=false,failOpen=false;
static int fileToken, retainedToken;
struct AnmRenderManagerView {
 AnmLoadedView *PreloadAnm(int slot,const char *name){
  events.push_back(slot==6?"front":"stage");
  Check(std::string(name)==(slot==6?"front.anm":"stage.anm"),"preload.name");
  if(preloadHook)preloadHook(slot);
  return slot==6?(failFront?nullptr:&front):(failStage?nullptr:&stage);
 }
};
static AnmRenderManagerView manager,*g_AnmRenderManagerView=&manager;
namespace FileSystem { static void *OpenFile(char *path,void *size,int flags){
 events.push_back("open");Check(std::string(path)=="message.dat"&&!size&&!flags,"open.args");
 if(openHook)openHook();return failOpen?nullptr:&fileToken;
}}
static int GuiUpdateCallback(void*){return 0;} static int GuiDrawCallback(void*){return 0;}
namespace th10 {
struct ChainElem {unsigned flags=0x96;void *arg=nullptr;int(*callback)(void*)=nullptr;};
struct Chain {static ChainElem *AllocElem(int(*)(void*));static void AddToCalcChain(ChainElem*,int,Chain*);static void AddToDrawChain(ChainElem*,int,Chain*);};
static Chain firstChain,secondChain,*g_Chain=&firstChain;
static ChainElem tasks[2];static int allocations=0;
static int g_CriticalSections[7];static unsigned char g_LogLockDepth=0;
}
static void EnterCriticalSection(int *p){Check(p==&th10::g_CriticalSections[3],"log.lock");++locks;}
static void LeaveCriticalSection(int *p){Check(p==&th10::g_CriticalSections[3]&&th10::g_LogLockDepth==1,"log.unlock");--locks;}
'''

SEAMS = r'''
static GuiView *g_GuiView=nullptr;
static GuiView *expectedDeleted=nullptr;
static void ObserveMemset(void *p,int value,size_t size){
 Check(members==43&&timerCtors==1,"ctor.automatic_members");
 Check(value==0&&size==sizeof(GuiView),"ctor.whole_memset");
 auto *g=static_cast<GuiView*>(p);
 Check(g->displayVms0[0].sentinel==1&&g->displayVms0[9].sentinel==10&&g->displayVms1[0].sentinel==11&&g->displayVms2[0].sentinel==21&&g->displayVms3[0].sentinel==30&&g->displayVms4[0].sentinel==34&&g->displayVms5[0].sentinel==36&&g->bossPositionVm.sentinel==43,"ctor.member_order");
 Check(g_GuiView!=p,"ctor.publish_after_memset");
 std::memset(p,value,size);
}
GuiView::~GuiView(){Check(this==expectedDeleted,"factory.saved_delete");++destructors;}
th10::ChainElem *th10::Chain::AllocElem(int(*cb)(void*)){
 Check(allocations<2,"chain.count");events.push_back("alloc");
 ChainElem *task=&tasks[allocations++];task->callback=cb;if(allocHook)allocHook();return task;
}
void th10::Chain::AddToCalcChain(ChainElem *task,int priority,Chain *chain){
 Check(task==&tasks[0]&&task->callback==GuiUpdateCallback&&task->flags==0x94&&task->arg==g_GuiView&&priority==0x18&&chain==g_Chain,"chain.calc");
 Check(g_GuiView->updateCallback==nullptr,"chain.calc_before_publish");events.push_back("calc");if(calcHook)calcHook();
}
void th10::Chain::AddToDrawChain(ChainElem *task,int priority,Chain *chain){
 Check(task==&tasks[1]&&task->callback==GuiDrawCallback&&task->flags==0x94&&task->arg==g_GuiView&&priority==0x2b&&chain==g_Chain,"chain.draw");
 Check(g_GuiView->updateCallback==&tasks[0]&&g_GuiView->drawCallback==nullptr,"chain.draw_before_publish");events.push_back("draw");if(drawHook)drawHook();
}
namespace th10 {GameErrorContext g_GameErrorContext;}
'''

CASES = r'''
static void Reset(){
 members=timerCtors=destructors=0;g_GuiView=nullptr;expectedDeleted=nullptr;
 events.clear();preloadHook={};openHook={};allocHook={};calcHook={};drawHook={};
 failFront=failStage=failOpen=false;g_GuiRetainedMessageFile491BE8=nullptr;
 th10::g_Chain=&th10::firstChain;th10::allocations=0;for(auto &t:th10::tasks)t=th10::ChainElem{};
 g_GameStageState=stageBytes;g_ScriptViewerStageRoute=routeBytes;g_StageFilePath=pathBytes;
 std::memset(stageBytes,0,sizeof stageBytes);std::memset(routeBytes,0,sizeof routeBytes);
 const char *msg="message.dat",*anm="stage.anm";std::memcpy(routeBytes+24,&msg,sizeof msg);std::memcpy(routeBytes+32,&anm,sizeof anm);
 int num=7;std::memcpy(stageBytes+4,&num,4);
 auto &log=th10::g_GameErrorContext;std::memset(log.m_Buffer,0,sizeof log.m_Buffer);log.m_BufferEnd=log.m_Buffer;log.m_Fatal=0;
}
static GuiView *Make(){auto *g=new GuiView;Check(g_GuiView==g&&g->ownerFlags==2,"ctor.publish");
 Check(g->displayVms0[0].sentinel==0&&g->bossPositionVm.sentinel==0&&g->frameTimer.flags==0,"ctor.zero_members");return g;}
static void Drop(GuiView *g){expectedDeleted=g;delete g;}
static void Seed(GuiView *g,unsigned flags){g->frameTimer.previous=55;g->frameTimer.current=66;g->frameTimer.subframe=0.75f;g->frameTimer.flags=flags;g->frameTimer.scale=nullptr;g->spellSeconds=88;g->previousSpellSeconds=99;g->value9E78=123;}
static void Timer(GuiView *g,unsigned flags){Check(g->frameTimer.previous==-1&&g->frameTimer.current==0&&g->frameTimer.subframe==0&&g->frameTimer.flags==(flags|1u),"timer.reset");Check(g->frameTimer.scale==((flags&1)?nullptr:&g_AnmGameSpeed),"timer.scale");}
static void Error(){auto &l=th10::g_GameErrorContext;Check(l.m_BufferEnd-l.m_Buffer==22&&strstr(l.m_Buffer,"\r\n")&&l.m_Fatal==0&&locks==0&&th10::g_LogLockDepth==0,"real_log");}
int main(){
 for(unsigned flags:{0u,1u,0x80u,0x81u})for(bool retained:{false,true}){
  Reset();auto *g=Make();Seed(g,flags);if(retained)g_GuiRetainedMessageFile491BE8=&retainedToken;
  Check(g->LoadData()==0,"load.success");Timer(g,flags);
  Check(g->messageFile==(retained?&retainedToken:&fileToken)&&!g_GuiRetainedMessageFile491BE8,"load.retained_consume");
  Check(g->spellSeconds==-1&&g->previousSpellSeconds==-1&&g->value9E78==7,"load.counters");
  Check(events==(retained?std::vector<std::string>{"stage"}:std::vector<std::string>{"stage","open"}),"load.order");Drop(g);
 }
 for(bool early:{true,false}){
  Reset();auto *g=Make();Seed(g,0x80);g->messageFile=&retainedToken;failStage=early;failOpen=!early;
  Check(g->LoadData()==-1,"load.error");Error();
  Check(g->stageUiAnm==(early?nullptr:&stage)&&g->messageFile==(early?&retainedToken:nullptr),"load.error_publication");
  Check(g->frameTimer.previous==55&&g->frameTimer.current==66&&g->frameTimer.subframe==0.75f&&g->frameTimer.flags==0x80&&g->spellSeconds==88&&g->previousSpellSeconds==99&&g->value9E78==123,"load.error_preserves");Drop(g);
 }
 // Filename pointer is stored inside the path buffer before that buffer is cleared.
 Reset();auto *g=Make();g_StageFilePath=reinterpret_cast<char*>(routeBytes+24);
 Check(g->LoadData()==0,"filename.alias_capture");Drop(g);
 // Stage source aliases the first counter overwritten after capture, within the live owner.
 Reset();g=Make();g_GuiRetainedMessageFile491BE8=&retainedToken;g->spellSeconds=271;
 g_GameStageState=reinterpret_cast<unsigned char*>(g)+offsetof(GuiView,spellSeconds)-4;
 Check(g->LoadData()==0&&g->value9E78==271,"stage.alias_capture");Drop(g);
 for(int fail:{1,2,3}){
  Reset();g=Make();g->frontAnm=&stage;failFront=fail==1;failStage=fail==2;failOpen=fail==3;
  Check(g->Initialize()==-1,"initialize.error");Error();
  Check(g->frontAnm==(fail==1?nullptr:&front)&&th10::allocations==0&&!g->updateCallback&&!g->drawCallback,"initialize.error_publication");Drop(g);
 }
 Reset();g=Make();allocHook=[](){th10::g_Chain=&th10::secondChain;};
 calcHook=[](){th10::g_Chain=&th10::firstChain;};
 Check(g->Initialize()==0&&g->updateCallback==&th10::tasks[0]&&g->drawCallback==&th10::tasks[1],"initialize.success");
 Check(events==std::vector<std::string>{"front","stage","open","alloc","calc","alloc","draw"},"initialize.order");Drop(g);
 for(bool failure:{false,true}){
  Reset();failFront=failure;GuiView *captured=nullptr;
  preloadHook=[&](int slot){if(slot==6){captured=g_GuiView;Check(captured->frontAnm==nullptr,"initialize.preload_before_publish");if(failure){expectedDeleted=captured;g_GuiView=nullptr;}}};
  void *result=ScriptViewerCreateGui();
  Check(result==(failure?nullptr:captured)&&destructors==(failure?1:0),"factory.result");if(!failure)Drop(captured);
 }
 puts("PASS extracted GUI creation contracts: live aliases, failure state, automatic members, real Log, chain captures, saved factory owner");
}
'''

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--source',type=Path,default=ROOT/'src/Gui.cpp')
    ap.add_argument('--header',type=Path,default=ROOT/'src/Gui.hpp')
    ap.add_argument('--sanitizers',default='address,undefined')
    args=ap.parse_args()
    source=args.source.read_text();header=args.header.read_text()
    bodies=[block(source,a) for a in ['GuiView::GuiView()','int GuiView::LoadData()','int GuiView::Initialize()','void *ScriptViewerCreateGui()']]
    structure(bodies)
    controls=[(0,'ownerFlags |= 2;','ownerFlags |= 1;'),(3,'if (gui->Initialize())','if (gui && gui->Initialize())'),(3,'delete gui;','delete g_GuiView;'),(1,'const char *filename =','g_StageFilePath[0] = 0; const char *filename ='),(1,'capturedStage =','spellSeconds = -1; capturedStage =')]
    for index,old,new in controls:
        changed=bodies.copy();need(changed[index].count(old)==1,'mutation anchor');changed[index]=changed[index].replace(old,new)
        try: structure(changed)
        except RuntimeError: pass
        else: raise RuntimeError('structural mutant escaped: '+old)
    decl='\n'.join(block(header,a,True) for a in ['struct GuiBossGaugeValueView\n','struct GuiView\n'])
    error_header=block((ROOT/'src/GameErrorContext.hpp').read_text(),'struct GameErrorContext\n',True)
    log=block((ROOT/'src/GameErrorContext.cpp').read_text(),'char * __fastcall GameErrorContext::Log(')
    # Only the memset dependency is observed; all four algorithm bodies are extracted unchanged.
    code=PREFIX+'\n'+decl+'\nnamespace th10 {\n'+error_header+'\n}\n'+SEAMS+'\nnamespace th10 {\n'+log+'\n}\n#define memset ObserveMemset\n'+'\n'.join(bodies)+'\n#undef memset\n'+CASES
    flags=['-std=c++17','-O1','-g','-fno-omit-frame-pointer','-Wno-write-strings']
    if args.sanitizers: flags+=['-fsanitize='+args.sanitizers,'-fno-sanitize-recover=all']
    if 'address' in args.sanitizers.split(','): flags+=['-fno-pie','-no-pie']
    with tempfile.TemporaryDirectory(prefix='gui-creation-') as tmp:
        cpp=Path(tmp)/'test.cpp';exe=Path(tmp)/'test';cpp.write_text(code)
        subprocess.run([os.environ.get('CXX','g++'),*flags,str(cpp),'-o',str(exe)],check=True,timeout=60)
        subprocess.run([str(exe)],check=True,timeout=20,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
    print('PASS 5 bounded source mutation rejects; null/vptr target behavior structural only; no native/exactness credit')

if __name__=='__main__': main()
