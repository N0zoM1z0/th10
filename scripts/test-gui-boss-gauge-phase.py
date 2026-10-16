#!/usr/bin/env python3
"""Exercise the actual lexical HUD boss-gauge phase with synthetic ANM sinks.

Valid VM list/root-child structures and native nonnull active-path preconditions.
Callback replacement is synthetic evidence of load order, not a reachable game bug.
"""
from pathlib import Path
import argparse, shutil, subprocess, tempfile, unittest
SOURCE=Path(__file__).resolve().parents[1]/'src/Gui.cpp'
def phase_source():
    s=SOURCE.read_text();start=s.index('// HUD boss-gauge phase begin:')
    b=s.index('{',start);d=1;e=b+1
    while d:d+=(s[e]=='{')-(s[e]=='}');e+=1
    return s[b:e]
PROGRAM=r'''
#include <cstdio>
#include <cmath>
#include <climits>
#include <vector>
#define CHECK(x) do { if(!(x)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#x);return 1; } } while(0)
struct AnmVmView;
struct AnmVmLayerNodeView { void *owner; AnmVmLayerNodeView *next,*previous; };
struct AnmVmIdView { int value; };
struct AnmVmView { int id; unsigned flags35C,renderLayer; short pendingInterrupt; AnmVmLayerNodeView managerNode,layerNode; };
struct Loaded { int tag; void InitializeVm(AnmVmView *,int); };
typedef Loaded AnmLoadedView;
struct GuiEnemyView { int life,maximumLife; };
struct GuiEnemyManagerView { GuiEnemyView *activeBoss; };
struct GuiPlayerView { float positionX,positionY; };
struct GaugeValue { float value,untouched; };
struct GuiView { void *message; int bossLife; float bossGaugeTarget,bossGaugeDisplayed; unsigned guiFlags;
    int bossGaugeSegmentCount; AnmVmIdView bossGaugeVmId,bossGaugeSegmentVmIds[10]; GaugeValue bossGaugeValues[4]; Loaded *frontAnm;
};
struct AnmRenderManagerView { int tag;AnmVmLayerNodeView *primaryVmListHead,*primaryVmListTail,*secondaryVmListHead;
    AnmVmIdView nextVmId;AnmVmView *AllocateVm(); };
static GuiEnemyManagerView *g_GuiEnemyManager;
static GuiPlayerView *g_GuiPlayer;
static AnmRenderManagerView *g_AnmRenderManagerView;
static int g_GuiStage,g_GuiMessageState;
static const unsigned GUI_VM_RENDER_LAYER=15;
static AnmVmView storage[128];static int used;
struct Event { int kind,owner,script;AnmVmView *vm; };
static std::vector<Event> events;
static GuiView *activeGui;static Loaded *newResource;static AnmRenderManagerView *newManager;
static bool mutateAllocate,mutateInitialize;static bool initValid;
AnmVmView *AnmRenderManagerView::AllocateVm() {
    AnmVmView *vm=&storage[used++];*vm=AnmVmView();vm->flags35C=0x55;
    events.push_back({1,tag,0,vm});
    if(mutateAllocate) {activeGui->frontAnm=newResource;mutateAllocate=false;}
    return vm;
}
void Loaded::InitializeVm(AnmVmView *vm,int script) {
    events.push_back({2,tag,script,vm});
    if(vm->renderLayer!=15 || vm->flags35C!=0x40000055u) initValid=false;
    if(mutateInitialize) {g_AnmRenderManagerView=newManager;activeGui->bossGaugeSegmentCount=2;mutateInitialize=false;}
}
static void Exercise(GuiView *gui) @PHASE@
static void Reset(GuiView &gui,GuiEnemyManagerView &enemy,GuiEnemyView &boss,GuiPlayerView &player,AnmRenderManagerView &manager,Loaded &loaded) {
    gui=GuiView();boss={50,100};enemy={&boss};player={0,100};manager=AnmRenderManagerView();manager.tag=1;loaded.tag=1;
    gui.frontAnm=&loaded;g_GuiEnemyManager=&enemy;g_GuiPlayer=&player;g_AnmRenderManagerView=&manager;
    g_GuiStage=1;g_GuiMessageState=0;used=0;events.clear();activeGui=&gui;
    mutateAllocate=mutateInitialize=false;newManager=nullptr;newResource=nullptr;initValid=true;
}
static void AddPrimary(AnmRenderManagerView &manager,AnmVmView &vm,int id) {
    vm=AnmVmView();vm.id=id;vm.managerNode={&vm,nullptr,manager.primaryVmListTail};vm.layerNode={&vm,nullptr,nullptr};
    if(manager.primaryVmListTail) manager.primaryVmListTail->next=&vm.managerNode;else manager.primaryVmListHead=&vm.managerNode;
    manager.primaryVmListTail=&vm.managerNode;
}
static int CreationAndLifetime() {
    GuiView gui;GuiEnemyManagerView enemy;GuiEnemyView boss;GuiPlayerView player;AnmRenderManagerView manager,other;Loaded loaded,otherLoaded{2};
    Reset(gui,enemy,boss,player,manager,loaded);other=AnmRenderManagerView();other.tag=2;other.nextVmId.value=200;
    manager.nextVmId.value=100;newManager=&other;newResource=&otherLoaded;mutateAllocate=mutateInitialize=true;
    Exercise(&gui);CHECK(initValid);CHECK(events.size()==6);
    CHECK(events[0].kind==1 && events[0].owner==1);CHECK(events[1].kind==2 && events[1].owner==1 && events[1].script==0x85);
    CHECK(events[2].kind==1 && events[2].owner==2);CHECK(events[3].owner==2 && events[3].script==0x5b);
    CHECK(events[4].kind==1 && events[4].owner==2);CHECK(events[5].owner==2 && events[5].script==0x5c);
    CHECK(gui.bossGaugeVmId.value==201 && gui.bossGaugeSegmentVmIds[0].value==202 && gui.bossGaugeSegmentVmIds[1].value==203);
    CHECK(manager.primaryVmListHead==nullptr && manager.nextVmId.value==100);
    CHECK(other.primaryVmListHead==&storage[0].managerNode && other.primaryVmListTail==&storage[2].managerNode);
    for(int i=0;i<3;++i) {CHECK(storage[i].id==201+i);CHECK(storage[i].managerNode.owner==&storage[i]);
        CHECK(storage[i].managerNode.previous==(i?&storage[i-1].managerNode:nullptr));
        CHECK(storage[i].managerNode.next==(i<2?&storage[i+1].managerNode:nullptr));}
    Reset(gui,enemy,boss,player,manager,loaded);manager.nextVmId.value=-1;
    Exercise(&gui);CHECK(gui.bossGaugeVmId.value==1 && manager.nextVmId.value==1 && storage[0].id==1);
    Reset(gui,enemy,boss,player,manager,loaded);manager.nextVmId.value=INT_MAX;
    Exercise(&gui);CHECK(gui.bossGaugeVmId.value==INT_MIN && storage[0].id==INT_MIN);
    // Nonempty list appends; already nonzero HUD IDs suppress reallocation even if missing.
    Reset(gui,enemy,boss,player,manager,loaded);AnmVmView existing;AddPrimary(manager,existing,99);manager.nextVmId.value=99;
    gui.bossGaugeSegmentCount=3;gui.bossGaugeSegmentVmIds[1].value=999;
    Exercise(&gui);CHECK(events.size()==6);CHECK(gui.bossGaugeVmId.value==100);CHECK(gui.bossGaugeSegmentVmIds[0].value==101);
    CHECK(gui.bossGaugeSegmentVmIds[1].value==999 && gui.bossGaugeSegmentVmIds[2].value==102);
    CHECK(existing.managerNode.next==&storage[0].managerNode && storage[0].managerNode.previous==&existing.managerNode);
    // Target-supported insertion cursor with a successor: list links are valid,
    // but primaryVmListTail intentionally denotes an interior node here.
    // This checks the native splice contract, not a normal tail-is-last claim.
    Reset(gui,enemy,boss,player,manager,loaded);
    AnmVmView before,cursor,successor;
    AddPrimary(manager,before,10);AddPrimary(manager,cursor,11);AddPrimary(manager,successor,12);
    manager.primaryVmListTail=&cursor.managerNode;manager.nextVmId.value=12;
    Exercise(&gui);CHECK(events.size()==2);CHECK(gui.bossGaugeVmId.value==13);
    CHECK(manager.primaryVmListHead==&before.managerNode);
    CHECK(before.managerNode.next==&cursor.managerNode);
    CHECK(cursor.managerNode.next==&storage[0].managerNode);
    CHECK(storage[0].managerNode.previous==&cursor.managerNode);
    CHECK(storage[0].managerNode.next==&successor.managerNode);
    CHECK(successor.managerNode.previous==&storage[0].managerNode);
    CHECK(successor.managerNode.next==nullptr);
    CHECK(manager.primaryVmListTail==&storage[0].managerNode);
    return 0;
}
static int InterruptsAndGates() {
    GuiView gui;GuiEnemyManagerView enemy;GuiEnemyView boss;GuiPlayerView player;AnmRenderManagerView manager;Loaded loaded;
    for(int action=0;action<4;++action) {
        Reset(gui,enemy,boss,player,manager,loaded);
        AnmVmView primary,duplicate,segment,child,nonroot,suppressed,parent;
        AddPrimary(manager,primary,10);AddPrimary(manager,segment,20);AddPrimary(manager,nonroot,30);AddPrimary(manager,parent,40);
        duplicate=AnmVmView();duplicate.id=10;duplicate.managerNode={&duplicate,nullptr,nullptr};manager.secondaryVmListHead=&duplicate.managerNode;
        child=AnmVmView();child.layerNode={&child,nullptr,&primary.layerNode};primary.layerNode.next=&child.layerNode;
        suppressed=AnmVmView();suppressed.layerNode={&suppressed,nullptr,&nonroot.layerNode};
        parent.layerNode.next=&nonroot.layerNode;
        nonroot.layerNode.previous=&parent.layerNode;nonroot.layerNode.next=&suppressed.layerNode;
        gui.bossGaugeVmId.value=10;gui.bossGaugeSegmentCount=2;
        gui.bossGaugeSegmentVmIds[0].value=20;gui.bossGaugeSegmentVmIds[1].value=30;gui.bossGaugeSegmentVmIds[2].value=987;
        if(action==0) {player={-65,64};} // Enter side: interrupts3 then remove slot2.
        if(action==1) {gui.guiFlags=8;player={0,80};} // Exit side: interrupts2.
        if(action==2) {gui.message=&gui;} // Hide only main; segment IDs preserved.
        if(action==3) {enemy.activeBoss=nullptr;}
        gui.bossGaugeTarget=0.66f;gui.bossLife=1234;
        for(int j=0;j<4;++j)gui.bossGaugeValues[j]={0.75f,123.0f};
        Exercise(&gui);CHECK(events.empty());
        CHECK(primary.pendingInterrupt==(action==0?3:action==1?2:1));CHECK(child.pendingInterrupt==primary.pendingInterrupt);
        CHECK(duplicate.pendingInterrupt==0);CHECK(suppressed.pendingInterrupt==0);
        CHECK(segment.pendingInterrupt==(action<2?primary.pendingInterrupt:0));CHECK(nonroot.pendingInterrupt==segment.pendingInterrupt);
        if(action<2) {CHECK(gui.bossGaugeVmId.value==10);CHECK(gui.bossGaugeSegmentVmIds[2].value==0);
            CHECK((gui.guiFlags&8)==(action==0?8u:0u));}
        else {CHECK(gui.bossGaugeVmId.value==0 && gui.bossGaugeDisplayed==0);
            CHECK(gui.bossGaugeTarget==0.66f && gui.bossLife==1234);
            CHECK(gui.bossGaugeSegmentVmIds[0].value==20 && gui.bossGaugeSegmentVmIds[2].value==987);
            for(int j=0;j<4;++j) CHECK(gui.bossGaugeValues[j].value==0 && gui.bossGaugeValues[j].untouched==123);}
    }
    // Exact side-panel boundaries: X<-64/Y<=64 enter, Y>=80 or X>0 exit.
    const float xs[]={-64,-65,-65,0,0,1};
    const float ys[]={64,64,65,79,80,79};
    const unsigned before[]={0,0,0,8,8,8};
    const unsigned after[]={0,8,0,8,0,0};
    for(int i=0;i<6;++i) {
        Reset(gui,enemy,boss,player,manager,loaded);gui.guiFlags=before[i];
        gui.bossGaugeVmId.value=999;player={xs[i],ys[i]};Exercise(&gui);
        CHECK((gui.guiFlags&8)==after[i]);CHECK(events.empty());
    }
    // Secondary-only lookup and zero/missing IDs have no manufactured invalidation.
    Reset(gui,enemy,boss,player,manager,loaded);AnmVmView secondary{};secondary.id=70;secondary.managerNode={&secondary,nullptr,nullptr};
    AnmVmView removedChild{};
    secondary.layerNode={&secondary,&removedChild.layerNode,nullptr};
    removedChild.layerNode={&removedChild,nullptr,&secondary.layerNode};
    manager.secondaryVmListHead=&secondary.managerNode;gui.bossGaugeVmId.value=70;gui.bossGaugeSegmentCount=0;
    gui.bossGaugeSegmentVmIds[0].value=70;player={0,100};Exercise(&gui);
    CHECK(secondary.pendingInterrupt==1 && gui.bossGaugeSegmentVmIds[0].value==0 && gui.bossGaugeVmId.value==70);
    CHECK(removedChild.pendingInterrupt==1);
    CHECK(secondary.layerNode.next==&removedChild.layerNode && removedChild.layerNode.previous==&secondary.layerNode);
    // The same removal path must suppress child propagation for a nonroot VM.
    secondary.pendingInterrupt=0;removedChild.pendingInterrupt=0;
    AnmVmView removalParent{};removalParent.layerNode={&removalParent,&secondary.layerNode,nullptr};
    secondary.layerNode.previous=&removalParent.layerNode;
    gui.bossGaugeSegmentVmIds[0].value=70;Exercise(&gui);
    CHECK(secondary.pendingInterrupt==1 && removedChild.pendingInterrupt==0);
    CHECK(gui.bossGaugeSegmentVmIds[0].value==0 && gui.bossGaugeVmId.value==70);
    // Null enemy-manager gate touches only the hidden gauge state, without allocation.
    Reset(gui,enemy,boss,player,manager,loaded);g_GuiEnemyManager=nullptr;Exercise(&gui);CHECK(events.empty());
    // Null ANM manager is only permitted here with inactive boss and zero gauge ID.
    g_AnmRenderManagerView=nullptr;Exercise(&gui);CHECK(events.empty());
    return 0;
}
static int StagesAndInterpolation() {
    GuiView gui;GuiEnemyManagerView enemy;GuiEnemyView boss;GuiPlayerView player;AnmRenderManagerView manager;Loaded loaded;
    const int low[]={0x85,0x85,-1,-1,0x89,0x8b,0x8c,0x8c,0x85};
    const int high[]={0x85,0x86,0x87,0x88,0x8a,0x8b,0x8c,0x8d,0x85};
    for(int stage=0;stage<=8;++stage)for(int state=23;state<=24;++state) {
        Reset(gui,enemy,boss,player,manager,loaded);g_GuiStage=stage;g_GuiMessageState=state;gui.bossGaugeSegmentCount=1;
        Exercise(&gui);const int script=state==23?low[stage]:high[stage];
        CHECK(events.size()==unsigned(script<0?2:4));CHECK(events[1].script==(script<0?0x5b:script));
        CHECK(gui.bossGaugeVmId.value==(script<0?0:1));CHECK(gui.bossGaugeSegmentVmIds[0].value==(script<0?1:2));
        CHECK(gui.bossLife==50 && gui.bossGaugeTarget==0.5f && gui.bossGaugeDisplayed==0.025f);
    }
    for(int count: {-1,0,1,10}) {
        Reset(gui,enemy,boss,player,manager,loaded);gui.bossGaugeSegmentCount=count;gui.bossGaugeVmId.value=999;
        gui.bossGaugeDisplayed=0.49f;Exercise(&gui);CHECK(gui.bossGaugeDisplayed==0.5f);
        CHECK(events.size()==unsigned(count>0?count*2:0));
    }
    Reset(gui,enemy,boss,player,manager,loaded);gui.bossGaugeVmId.value=999;gui.bossGaugeDisplayed=0.8f;Exercise(&gui);CHECK(gui.bossGaugeDisplayed==0.5f);
    return 0;
}
int main() {if(CreationAndLifetime() || InterruptsAndGates() || StagesAndInterpolation())return 1;return 0;}
'''
class BossGaugePhase(unittest.TestCase):
    def run_phase(self,source):
        compiler=shutil.which('c++')
        if not compiler:self.skipTest('host C++ compiler unavailable')
        with tempfile.TemporaryDirectory(prefix='th10-gauge-') as tmp:
            p=Path(tmp)/'test.cpp';exe=Path(tmp)/'test';p.write_text(PROGRAM.replace('@PHASE@',source))
            built=subprocess.run([compiler,'-std=c++11','-O0','-fsanitize=undefined','-fno-sanitize-recover=undefined',str(p),'-o',str(exe)],capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr)
            return subprocess.run([str(exe)],capture_output=True,text=True)
    def test_actual_phase(self):
        r=self.run_phase(phase_source());self.assertEqual(r.returncode,0,r.stdout+r.stderr)
    def test_mutations(self):
        s=phase_source()
        changes=[
            ('manager-reload','manager = g_AnmRenderManagerView;\n','/* omitted publication reload */\n'),
            ('capture-resource','loaded->InitializeVm(vm,','gui->frontAnm->InitializeVm(vm,'),
            ('zero-id','if (manager->nextVmId.value == 0)','if (false)'),
            ('primary-precedence','AnmVmLayerNodeView *node = manager->primaryVmListHead;', 'AnmVmLayerNodeView *node = manager->secondaryVmListHead;'),
            ('child-root','if (vm->layerNode.previous == NULL)','if (vm->layerNode.previous != NULL)'),
            ('missing-id-clear','gui->bossGaugeSegmentVmIds[segment].value = 0;','/* missing segment clear */'),
            ('creation-layer','vm->renderLayer = GUI_VM_RENDER_LAYER;','vm->renderLayer = 0;'),
            ('removed-root','vm->pendingInterrupt = 1;', 'vm->pendingInterrupt = 2;'),
            ('removed-child','static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 1;', 'static_cast<AnmVmView *>(node->owner)->pendingInterrupt = 2;'),
            ('splice-next','node->next = tail->next;', 'node->next = NULL;'),
            ('splice-backlink','tail->next->previous = node;', 'tail->next->previous = tail;'),
            ('wrong-stage7','0x8d : 0x8c','0x8e : 0x8d'),
        ]
        for name,old,new in changes:
            with self.subTest(mutation=name):
                self.assertIn(old,s)
                # Do not alter the manager declaration when testing post-init reload.
                candidate=s.replace('    '+old,'    '+new) if name=='manager-reload' else s.replace(old,new,1) if name in ('removed-root','removed-child') else s.replace(old,new)
                r=self.run_phase(candidate);self.assertEqual(r.returncode,1,r.stdout+r.stderr)
                self.assertRegex(r.stderr,r'line [0-9]+: ');self.assertNotIn('runtime error:',r.stderr)
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,default=SOURCE)
    args,rest=p.parse_known_args();SOURCE=args.source;unittest.main(argv=[__file__]+rest)
