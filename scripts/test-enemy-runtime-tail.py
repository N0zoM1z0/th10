#!/usr/bin/env python3
"""Actual tail/ANM setter/timer source, synthetic lookup and sound boundaries."""
from pathlib import Path
import runpy,shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
definition=runpy.run_path(str(ROOT/'scripts/test-enemy-motion.py'))['definition']
class EnemyTail(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_cached_manager_and_tail(self):
  text=(ROOT/'src/Enemy.cpp').read_text();a=text.index('    AnmRenderManagerView *const manager = g_AnmRenderManagerView;');b=text.index('\n    return 0;\n}',a);block=text[a:b]
  anm=(ROOT/'src/AnmManager.cpp').read_text()
  setters=definition(anm,'void __stdcall AnmRenderManagerView::SetVmPosition(')+definition(anm,'void __stdcall AnmRenderManagerView::SetVmWorldPosition(')
  timer=definition(anm,'void AnmVmTimerView::Add(')
  program=r'''
#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
#define __stdcall
struct AnmFloat3View {float x,y,z;};using PlayerFloat3=AnmFloat3View;
struct AnmVmLayerNodeView {void*owner;AnmVmLayerNodeView*next,*previous;};
struct AnmVmView {unsigned color,flags;AnmFloat3View positionOffset;AnmVmLayerNodeView layerNode;};using EnemyManagedVmView=AnmVmView;
struct AnmVmIdView {unsigned value;AnmVmIdView(unsigned v):value(v){}};
struct AnmRenderManagerView {AnmVmView*FindVm(AnmVmIdView);void SetVmPosition(AnmVmIdView,const AnmFloat3View*);void SetVmWorldPosition(AnmVmIdView,const AnmFloat3View*);};
struct AnmVmTimerView {int previous,current;float subframe;float*scale;unsigned flags;void Add(float);};
struct EnemyFullObjectView;
struct EnemyRuntimeView {unsigned flags,managedVmIds[8];struct{PlayerFloat3 position;}worldMotion;EnemyFullObjectView*owner;int damageFlashFrames,value13C8;AnmVmTimerView damageReductionTimer,playerCollisionTimer,updateTimer;};
struct EnemyFullObjectView {EnemyRuntimeView runtime;};
struct Player {EnemyFullObjectView*trackedEnemy;PlayerFloat3 drawPosition;unsigned char trackedEnemyValid;} player;Player*g_Player=&player;
struct Visual{unsigned flags9DA4;}visual;Visual*g_EnemyVisualState=&visual;
struct Game{unsigned flags378C;}game;Game*g_EnemyGameState=&game;
struct EnemySoundQueueView{void QueueSoundCue(int,float);};
static EnemySoundQueueView soundOwner;
unsigned char *g_MainSoundOwner=reinterpret_cast<unsigned char*>(&soundOwner);
static AnmRenderManagerView original,replacement;AnmRenderManagerView*g_AnmRenderManagerView=&original;
static AnmVmView vms[8],child;static std::vector<unsigned> lookups;
static EnemyFullObjectView owner;static bool missing=false;static int sound=-1;static float scale=0.5f;
AnmVmView*AnmRenderManagerView::FindVm(AnmVmIdView id){assert(this==&original);lookups.push_back(id.value);g_AnmRenderManagerView=&replacement;if(missing||id.value==0)return NULL;assert(id.value<=8);return &vms[id.value-1];}
void EnemySoundQueueView::QueueSoundCue(int id,float x){assert(reinterpret_cast<unsigned char*>(this)==g_MainSoundOwner&&x==10&&lookups.size()==9);assert(owner.runtime.damageReductionTimer.current==3&&owner.runtime.playerCollisionTimer.current==4);sound=id;owner.runtime.damageReductionTimer.subframe=5;owner.runtime.damageReductionTimer.current=5;}
'''+setters+timer+r'''
static void runTail(EnemyRuntimeView*enemy){
'''+block+r'''
}
static void setup(){owner={};player={};visual={};game={};lookups.clear();sound=-1;missing=false;g_AnmRenderManagerView=&original;scale=0.5f;child={};
 auto&e=owner.runtime;e.owner=&owner;e.flags=1;e.worldMotion.position={10,20,30};
 for(int i=0;i<8;++i){e.managedVmIds[i]=i+1;vms[i]={};vms[i].positionOffset={-1,-2,-3};}
 vms[0].layerNode.next=&child.layerNode;child.layerNode.owner=&child;
 e.damageReductionTimer={-1,3,3,&scale,1};e.playerCollisionTimer={-1,4,4,&scale,1};e.updateTimer={-1,6,6,&scale,1};}
static void checkLookups(){assert(lookups==std::vector<unsigned>({1,2,3,4,5,6,7,8,1}));assert(g_AnmRenderManagerView==&replacement);}
int main(){
 setup();runTail(&owner.runtime);checkLookups();for(auto&v:vms)assert(v.positionOffset.x==234&&v.positionOffset.y==36&&v.positionOffset.z==30);assert(child.positionOffset.x==234&&child.positionOffset.y==36);assert(sound==-1&&owner.runtime.damageReductionTimer.current==2&&owner.runtime.damageReductionTimer.previous==3&&owner.runtime.damageReductionTimer.subframe==2.5f);assert(owner.runtime.updateTimer.previous==6&&owner.runtime.updateTimer.current==6&&owner.runtime.updateTimer.subframe==6.5f);
 setup();owner.runtime.flags|=0x40000;vms[0].layerNode.previous=&child.layerNode;child.positionOffset={7,8,9};runTail(&owner.runtime);checkLookups();for(auto&v:vms)assert(v.positionOffset.x==10&&v.positionOffset.y==20&&v.positionOffset.z==30);assert(child.positionOffset.x==7&&child.positionOffset.y==8);
 setup();missing=true;runTail(&owner.runtime);checkLookups();assert(owner.runtime.managedVmIds[0]==0&&vms[0].positionOffset.x==-1);
 setup();owner.runtime.flags|=0x2000;runTail(&owner.runtime);assert(sound==0x13&&vms[0].color==0xff0000ffu&&(vms[0].flags&0x8000)&&owner.runtime.damageFlashFrames==4);assert(owner.runtime.damageReductionTimer.previous==5&&owner.runtime.damageReductionTimer.current==4&&owner.runtime.damageReductionTimer.subframe==4.5f);
 setup();owner.runtime.flags|=0xa000;owner.runtime.value13C8=899;runTail(&owner.runtime);assert(sound==0x23);
 setup();owner.runtime.flags|=0xa000;game.flags378C=1;owner.runtime.value13C8=300;runTail(&owner.runtime);assert(sound==0x13);
 setup();owner.runtime.flags|=0xa000;game.flags378C=1;owner.runtime.value13C8=299;runTail(&owner.runtime);assert(sound==0x23);
 setup();owner.runtime.flags|=0xa000;game.flags378C=9;runTail(&owner.runtime);assert(sound==0x13);
 setup();owner.runtime.damageFlashFrames=2;owner.runtime.flags|=0xa000;vms[0].flags=0x8000;visual.flags9DA4=0x8000;runTail(&owner.runtime);assert(sound==-1&&owner.runtime.damageFlashFrames==1&&vms[0].flags==0&&visual.flags9DA4==0);
 setup();owner.runtime.damageReductionTimer.current=0;owner.runtime.playerCollisionTimer.current=-1;runTail(&owner.runtime);assert(owner.runtime.damageReductionTimer.subframe==3&&owner.runtime.playerCollisionTimer.subframe==4);
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='enemy-tail-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
