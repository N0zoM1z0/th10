#!/usr/bin/env python3
"""Actual bounded Player helpers; synthetic lookup and allocated-row fixtures."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
def definition(text,signature):
 a=text.index(signature);b=text.index('{',a);depth=1;i=b+1
 while depth:
  depth+=(text[i]=='{')-(text[i]=='}');i+=1
 return text[a:i]
class DamagePrerequisites(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_interrupt_and_effect_rows(self):
  source=(ROOT/'src/Player.cpp').read_text();header=(ROOT/'src/Player.hpp').read_text()
  types='\n'.join(definition(header,'struct '+n+'\n')+';' for n in ['PlayerFloat3','PlayerTimerView','PlayerEffectMotionView','PlayerEffectRowView'])
  functions=definition(source,'PlayerEffectRowView *PlayerSpawnEffectRow(')+definition(source,'void PlayerSetShotVmInterrupt2(')
  program=r'''
#include <cassert>
#include <cstring>
#include <cstddef>
#define __declspec(x)
#define __forceinline inline
float g_AnmGameSpeed=0.5f;
'''+types+r'''
struct Player{PlayerEffectRowView effectRows[33];};
struct AnmVmLayerNodeView{void*owner;AnmVmLayerNodeView*next,*previous;};
struct AnmVmView{short pendingInterrupt;AnmVmLayerNodeView layerNode;unsigned flags;};
struct AnmVmIdView{unsigned value;AnmVmIdView(unsigned n):value(n){}};
static AnmVmView vm,child1,child2;static bool missing;static unsigned seen;static int lookups;
struct AnmRenderManagerView{AnmVmView*FindVm(AnmVmIdView id){seen=id.value;++lookups;return missing||id.value==0?NULL:&vm;}}manager;
AnmRenderManagerView*g_AnmRenderManagerView=&manager;
'''+functions+r'''
// Static zero initialization occurs before Timer constructors inspect flags.
static Player player;
static void resetRows(){std::memset(&player,0,sizeof(player));}
static void resetVms(){vm={};child1={};child2={};vm.pendingInterrupt=7;child1.pendingInterrupt=8;child2.pendingInterrupt=9;vm.flags=0xabcdef;missing=false;lookups=0;vm.layerNode.next=&child1.layerNode;child1.layerNode.owner=&child1;child1.layerNode.next=&child2.layerNode;child2.layerNode.owner=&child2;}
int main(){
 unsigned id=17;resetVms();PlayerSetShotVmInterrupt2(&id);assert(id==17&&seen==17&&lookups==1&&vm.pendingInterrupt==2&&child1.pendingInterrupt==2&&child2.pendingInterrupt==2&&vm.flags==0xabcdef);
 resetVms();vm.layerNode.previous=&child1.layerNode;PlayerSetShotVmInterrupt2(&id);assert(id==17&&vm.pendingInterrupt==2&&child1.pendingInterrupt==8&&child2.pendingInterrupt==9);
 resetVms();missing=true;PlayerSetShotVmInterrupt2(&id);assert(id==17&&lookups==1&&vm.pendingInterrupt==7&&child1.pendingInterrupt==8);
 resetVms();id=0;PlayerSetShotVmInterrupt2(&id);assert(id==0&&seen==0&&lookups==1&&vm.pendingInterrupt==7);
 resetRows();PlayerFloat3 p={3,4,5};auto*r=PlayerSpawnEffectRow(&player,&p,32,1.4f,13,60);assert(r==&player.effectRows[0]);assert(r->position.x==3&&r->position.y==4&&r->position.z==5&&r->radius==32&&r->radiusDelta==1.4f);assert(r->motion.activeFlags==3&&r->motion.damage==60&&r->motion.hitAccumulator==0&&r->motion.hitCap==999999&&r->motion.collisionInterval==4);assert(r->motion.timer.current==13&&r->motion.timer.previous==12&&r->motion.timer.subframe==13&&r->motion.timer.flags==1&&r->motion.timer.scale==&g_AnmGameSpeed);assert(r->motion.velocity.x==0&&r->motion.velocity.y==0&&r->motion.velocity.z==0&&r->motion.angle==0&&r->motion.angularStep==0&&r->motion.timerStep==0&&r->motion.timerStepDelta==0&&r->motion.motionFlags==0);
 resetRows();for(int i=0;i<31;++i){player.effectRows[i].motion.activeFlags=1;player.effectRows[i].radius=100+i;}player.effectRows[31].motion.activeFlags=2;player.effectRows[32].radius=999;assert(PlayerSpawnEffectRow(&player,&p,7,2,-3,-9)==&player.effectRows[31]);for(int i=0;i<31;++i)assert(player.effectRows[i].radius==100+i&&player.effectRows[i].motion.activeFlags==1);assert(player.effectRows[32].radius==999);assert(player.effectRows[31].motion.timer.current==-3&&player.effectRows[31].motion.timer.previous==-4&&player.effectRows[31].motion.timer.subframe==-3);
 resetRows();for(int i=0;i<32;++i)player.effectRows[i].motion.activeFlags=1;player.effectRows[32].radius=77;unsigned char before[sizeof(player)];std::memcpy(before,&player,sizeof(player));assert(PlayerSpawnEffectRow(&player,&p,9,8,7,6)==&player.effectRows[32]);assert(std::memcmp(before,&player,sizeof(player))==0);
 resetRows();player.effectRows[0].position={9,8,7};r=PlayerSpawnEffectRow(&player,&player.effectRows[0].position,3,2,1,0);assert(r==&player.effectRows[0]&&r->position.x==0&&r->position.y==0&&r->position.z==0); // Target clear precedes the aliased source read.
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='player-prereqs-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
