#!/usr/bin/env python3
"""Source-extracted HUD message cleanup checks; host UBSan, not game execution.

Plain VM structures test final deletion flags, root/child traversal, list priority,
IDs and free order. Run owner replacement and free observation are synthetic seams,
not claims that real Run/free change gui->message. No callbacks are added to walks.
Manager retention and intra-walk ordering are target/source-topology evidence only.
"""
from pathlib import Path
import argparse, shutil, subprocess, tempfile, unittest
SOURCE=Path(__file__).resolve().parents[1]/'src/Gui.cpp'
def phase_source():
    s=SOURCE.read_text();a=s.index('// HUD message phase begin:');b=s.index('{',a);e=b+1;d=1
    while d:d+=(s[e]=='{')-(s[e]=='}');e+=1
    return s[b:e]
PROGRAM=r'''
#include <cstdio>
#include <cstddef>
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"line %d: %s\n",__LINE__,#x);return 1;}}while(0)
struct AnmVmLayerNodeView {void *owner;AnmVmLayerNodeView *next,*previous;};
struct AnmVmView {int id;unsigned flags35C;AnmVmLayerNodeView layerNode;};
struct AnmVmIdView {int value;};
struct Timer {int ticks;void Tick(){++ticks;}};
enum {GUI_MESSAGE_PLAYER_PORTRAIT,GUI_MESSAGE_ENEMY_PORTRAIT,GUI_MESSAGE_TEXT_BOX,GUI_MESSAGE_TEXT_LINE_0,GUI_MESSAGE_TEXT_LINE_1,GUI_MESSAGE_ENEMY_NAME,GUI_MESSAGE_VM_COUNT};
static const unsigned GUI_ANM_DELETE_FLAG=0x04000000;
struct GuiMessageVmView {Timer lifetimeTimer;AnmVmIdView vmIds[6];static int Run(GuiMessageVmView *);};
struct GuiView {GuiMessageVmView *message;};
struct AnmRenderManagerView {AnmVmLayerNodeView *primaryVmListHead,*secondaryVmListHead;};
static AnmRenderManagerView *g_AnmRenderManagerView;
static GuiView *activeGui;static GuiMessageVmView *replacement,*runArg,*freed;
static bool replaceOwner,freeValid;static int runResult,runCount,freeCount;
int GuiMessageVmView::Run(GuiMessageVmView *p){runArg=p;++runCount;if(replaceOwner)activeGui->message=replacement;return runResult;}
static void ObservedFree(GuiMessageVmView *p){++freeCount;freed=p;if(activeGui->message!=p)freeValid=false;for(int i=0;i<6;++i)if(p->vmIds[i].value!=0)freeValid=false;}
#define free ObservedFree
static void Exercise(GuiView *gui) @PHASE@
#undef free
static void Reset(GuiView &g,GuiMessageVmView &m,AnmRenderManagerView &a){g={&m};m=GuiMessageVmView();a=AnmRenderManagerView();activeGui=&g;g_AnmRenderManagerView=&a;replacement=runArg=freed=NULL;replaceOwner=false;freeValid=true;runResult=1;runCount=freeCount=0;}
static int OwnerCases(){
 GuiView g;GuiMessageVmView m,n;AnmRenderManagerView a;
 Reset(g,m,a);g.message=NULL;Exercise(&g);CHECK(runCount==0&&freeCount==0);
 Reset(g,m,a);runResult=0;Exercise(&g);CHECK(runArg==&m&&m.lifetimeTimer.ticks==1&&g.message==&m&&freeCount==0);
 Reset(g,m,a);n=GuiMessageVmView();replaceOwner=true;replacement=&n;runResult=0;Exercise(&g);CHECK(m.lifetimeTimer.ticks==1&&n.lifetimeTimer.ticks==0&&g.message==&n&&freeCount==0);
 Reset(g,m,a);n=GuiMessageVmView();replaceOwner=true;replacement=&n;m.vmIds[0].value=88;n.vmIds[0].value=99;Exercise(&g);CHECK(runArg==&m&&freed==&n&&freeCount==1&&freeValid&&g.message==NULL&&m.vmIds[0].value==88);
 Reset(g,m,a);replaceOwner=true;Exercise(&g);CHECK(runCount==1&&freeCount==0&&g.message==NULL);
 return 0;
}
static int ListsAndTrees(){
 for(int slot=0;slot<6;++slot)for(int secondary=0;secondary<2;++secondary)for(int root=0;root<2;++root){
  GuiView g;GuiMessageVmView m;AnmRenderManagerView a;Reset(g,m,a);
  AnmVmView hit={70,0x55,{}},child={71,0x123,{}},last={72,0x456,{}},duplicate={70,0x789,{}},miss={20,0xabc,{}};
  miss.layerNode={&miss,root?NULL:&hit.layerNode,NULL};
  hit.layerNode={&hit,&child.layerNode,root?NULL:&miss.layerNode};
  child.layerNode={&child,&last.layerNode,&hit.layerNode};
  last.layerNode={&last,NULL,&child.layerNode};
  AnmVmLayerNodeView dup={&duplicate,NULL,NULL},node={&hit,NULL,NULL},head={&miss,secondary?NULL:&node,NULL};
  node.previous=secondary?NULL:&head;
  a.primaryVmListHead=&head;a.secondaryVmListHead=secondary?&node:&dup;
  m.vmIds[slot].value=70;Exercise(&g);
  CHECK(hit.flags35C==0x04000055u);CHECK(child.flags35C==(root?0x04000123u:0x123u));CHECK(last.flags35C==(root?0x04000456u:0x456u));
  CHECK(duplicate.flags35C==0x789u&&miss.flags35C==0xabcu);CHECK(freed==&m&&freeCount==1&&freeValid&&g.message==NULL);
 }
 GuiView g;GuiMessageVmView m;AnmRenderManagerView a;Reset(g,m,a);
 AnmVmView vm[6]={};AnmVmLayerNodeView nodes[6]={};
 for(int i=0;i<6;++i){vm[i].id=i+1;vm[i].flags35C=unsigned(i+1);nodes[i]={&vm[i],i==5?NULL:&nodes[i+1],i==0?NULL:&nodes[i-1]};m.vmIds[i].value=i+1;}
 a.primaryVmListHead=&nodes[0];Exercise(&g);CHECK(freeValid&&freed==&m&&g.message==NULL);for(int i=0;i<6;++i)CHECK(vm[i].flags35C==(0x04000000u|unsigned(i+1)));
 // Zero never selects a list VM with ID zero; stale IDs still clear.
 Reset(g,m,a);vm[0]={0,0x12,{}};nodes[0]={&vm[0],NULL,NULL};a.primaryVmListHead=&nodes[0];m.vmIds[4].value=900;Exercise(&g);CHECK(vm[0].flags35C==0x12&&freeValid&&g.message==NULL);
 return 0;
}
int main(){if(OwnerCases()||ListsAndTrees())return 1;return 0;}
'''
class MessagePhase(unittest.TestCase):
 def run_phase(self,s):
  compiler=shutil.which('c++')
  if not compiler:self.skipTest('host C++ unavailable')
  with tempfile.TemporaryDirectory(prefix='th10-message-') as t:
   p=Path(t)/'test.cpp';e=Path(t)/'test';p.write_text(PROGRAM.replace('@PHASE@',s))
   r=subprocess.run([compiler,'-std=c++11','-O0','-fsanitize=undefined','-fno-sanitize-recover=undefined',str(p),'-o',str(e)],capture_output=True,text=True);self.assertEqual(r.returncode,0,r.stderr)
   return subprocess.run([str(e)],capture_output=True,text=True)
 def test_actual_phase(self):
  r=self.run_phase(phase_source());self.assertEqual(r.returncode,0,r.stdout+r.stderr)
 def test_mutations(self):
  s=phase_source()
  changes=[('success-reload','message->lifetimeTimer.Tick();','gui->message->lifetimeTimer.Tick();'),('failure-stale','finishedMessage = gui->message;','finishedMessage = message;'),('free-entry','free(finishedMessage);','free(message);'),('clear-before-free','free(finishedMessage);','gui->message = NULL; free(finishedMessage);'),('missing-owner-clear','gui->message = NULL;','/* missing clear */'),('skip-zero','if (id != 0)','if (true)'),('primary-priority','node = manager->primaryVmListHead;','node = manager->secondaryVmListHead;'),('root-rule','vm->layerNode.previous == NULL','vm->layerNode.previous != NULL'),('replace-flags','vm->flags35C |= GUI_ANM_DELETE_FLAG;','vm->flags35C = GUI_ANM_DELETE_FLAG;'),('child-flag','static_cast<AnmVmView *>(node->owner)->flags35C |= GUI_ANM_DELETE_FLAG;','static_cast<AnmVmView *>(node->owner)->flags35C |= 0;')]
  changes += [('clear-slot-'+name,'finishedMessage->vmIds[GUI_MESSAGE_'+name+'].value = 0;','/* missing ID clear */') for name in ['PLAYER_PORTRAIT','ENEMY_PORTRAIT','TEXT_BOX','TEXT_LINE_0','TEXT_LINE_1','ENEMY_NAME']]
  for name,old,new in changes:
   with self.subTest(mutation=name):
    self.assertIn(old,s);r=self.run_phase(s.replace(old,new));self.assertEqual(r.returncode,1,r.stdout+r.stderr);self.assertRegex(r.stderr,r'line [0-9]+: ');self.assertNotIn('runtime error:',r.stderr)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,default=SOURCE);args,rest=p.parse_known_args();SOURCE=args.source;unittest.main(argv=[__file__]+rest)
