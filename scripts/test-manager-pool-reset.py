#!/usr/bin/env python3
"""Exercise maintained pool-reset bodies against independent target fixtures.

Host sinks test source effects and ordering, not Windows/i386 ABI or codegen.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def body(path, signature):
    source = (ROOT / path).read_text()
    start = source.index(signature)
    opening = source.index('{', start)
    end, depth = opening + 1, 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


class ManagerPoolReset(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_clear_bodies(self):
        program = r'''
#include <cassert>
#include <cstring>
#include <vector>
#define __stdcall
struct AnmLoadedView {int id;};
struct AnmRenderManagerView {AnmLoadedView *loadedAnms[33];void MarkLoadedVmsForDeletion(AnmLoadedView *);};
static AnmRenderManagerView renderer;
static AnmRenderManagerView *g_AnmRenderManagerView=&renderer;
static std::vector<AnmLoadedView *> marked;
static std::vector<int> events;
struct BulletRuntimeView {unsigned char prefix[0x446];unsigned short state;unsigned char tail[0x3a8];};
static_assert(sizeof(BulletRuntimeView)==0x7f0,"fixture record size");
struct BulletManagerView {
 unsigned char preserved[40];BulletRuntimeView *bulletCursor;
 int activeBulletCount;BulletRuntimeView bullets[2001];AnmLoadedView *bulletAnm;
 void ResetPool();
};
static BulletManagerView bullets;
static bool observeBulletOrder;
void AnmRenderManagerView::MarkLoadedVmsForDeletion(AnmLoadedView *loaded){
 assert(this==&renderer);marked.push_back(loaded);events.push_back(-1);
 if(observeBulletOrder)assert(bullets.bullets[0].state==0xa5a5);
}
struct EnemyFullObjectView;
struct EnemyListNodeView {EnemyFullObjectView *enemy;EnemyListNodeView *next,*previous;};
struct EnemyFullObjectView {
 int id;EnemyListNodeView *node;
 EnemyFullObjectView(int value,EnemyListNodeView *owned):id(value),node(owned){}
 virtual ~EnemyFullObjectView(){
  events.push_back(id);
  // A destructor may detach its node. The loop must already have saved next.
  if(node){node->next=nullptr;node->enemy=nullptr;}
 }
};
struct EnemyCallbackNodeView {unsigned flags;};
struct EnemyManagerView {
 EnemyListNodeView *enemyListHead;int spawnCounter,activeEnemyCount;
 EnemyFullObjectView *specialEnemySlots[8];
 EnemyCallbackNodeView *updateCallbackNode,*drawCallbackNode;
 void SuspendCallbacks();
};
'''
        program += body('src/BulletLifecycle.cpp', 'void BulletManagerView::ResetPool()')
        program += body('src/Enemy.cpp', 'void __stdcall EnemyManagerClear(')
        program += body('src/Enemy.cpp', 'void EnemyManagerView::SuspendCallbacks()')
        program += r'''
int main(){
 AnmLoadedView resources[]={{1},{2},{3}};
 for(int pass=0;pass<2;++pass){
  std::memset(&bullets,0xa5,sizeof bullets);bullets.bulletAnm=pass?nullptr:&resources[0];
  bullets.activeBulletCount=55;observeBulletOrder=true;marked.clear();events.clear();
  bullets.ResetPool();
  assert(marked==std::vector<AnmLoadedView *>({pass?nullptr:&resources[0]}));
  assert(bullets.bulletCursor==&bullets.bullets[0]&&bullets.activeBulletCount==55);
  assert(bullets.bulletAnm==(pass?nullptr:&resources[0]));
  for(unsigned char c:bullets.preserved)assert(c==0xa5);
  const unsigned char *bytes=reinterpret_cast<const unsigned char *>(bullets.bullets);
  for(unsigned i=0;i<sizeof bullets.bullets;++i)
   assert(bytes[i]==(i==2000*0x7f0+0x446?5:0));
 }
 observeBulletOrder=false;
 renderer.loadedAnms[9]=&resources[0];renderer.loadedAnms[10]=nullptr;
 renderer.loadedAnms[11]=&resources[1];renderer.loadedAnms[12]=&resources[2];
 EnemyListNodeView nodes[3]{};
 nodes[0].enemy=new EnemyFullObjectView(1,&nodes[0]);nodes[0].next=&nodes[1];
 nodes[1].next=&nodes[2];nodes[2].enemy=new EnemyFullObjectView(2,&nodes[2]);
 EnemyFullObjectView sentinel(99,nullptr);
 EnemyCallbackNodeView update{0xa},draw{0x12};
 EnemyManagerView manager{};manager.enemyListHead=nodes;manager.spawnCounter=7;
 manager.activeEnemyCount=99;manager.specialEnemySlots[0]=manager.specialEnemySlots[1]=&sentinel;
 manager.updateCallbackNode=&update;manager.drawCallbackNode=&draw;
 marked.clear();events.clear();EnemyManagerClear(&manager);
 assert(marked==std::vector<AnmLoadedView *>({&resources[0],nullptr,&resources[1],&resources[2]}));
 assert(events==std::vector<int>({-1,-1,-1,-1,1,2}));
 assert(manager.spawnCounter==0&&manager.activeEnemyCount==99);
 assert(manager.specialEnemySlots[0]==nullptr&&manager.specialEnemySlots[1]==&sentinel);
 assert(manager.enemyListHead==nodes&&nodes[0].next==nullptr&&nodes[2].enemy==nullptr);
 assert(update.flags==8&&draw.flags==0x10);
 // Empty lists and either absent callback preserve the same four resource calls.
 for(int mask=0;mask<4;++mask){
  manager.enemyListHead=nullptr;manager.spawnCounter=3;
  update.flags=draw.flags=0x22;
  manager.updateCallbackNode=(mask&1)?&update:nullptr;
  manager.drawCallbackNode=(mask&2)?&draw:nullptr;
  marked.clear();events.clear();EnemyManagerClear(&manager);
  assert(marked.size()==4&&events==std::vector<int>({-1,-1,-1,-1}));
  assert(update.flags==((mask&1)?0x20:0x22)&&draw.flags==((mask&2)?0x20:0x22));
  update.flags=draw.flags=0x20;manager.SuspendCallbacks();
  assert(update.flags==((mask&1)?0x22:0x20)&&draw.flags==((mask&2)?0x22:0x20));
 }
}
'''
        with tempfile.TemporaryDirectory(prefix='th10-manager-reset-') as tmp:
            source = Path(tmp) / 'fixture.cpp'
            binary = Path(tmp) / 'fixture'
            source.write_text(program)
            subprocess.run(['c++', '-std=c++17', '-O2', str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
