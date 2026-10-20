#!/usr/bin/env python3
"""Actual auxiliary damage source; host branches, not native x87 equivalence."""
from pathlib import Path
import re,runpy,shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
definition=runpy.run_path(str(ROOT/'scripts/test-player-damage-prerequisites.py'))['definition']
class AuxiliaryDamage(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_read_only_return_table(self):
  s=(ROOT/'src/Player.cpp').read_text();h=(ROOT/'src/Player.hpp').read_text();g=(ROOT/'src/EnemyGameState.hpp').read_text()
  types=definition(h,'struct PlayerFloat3\n')+';'+definition(s,'struct PlayerUpdateGate28\n')+';'+definition(s,'struct PlayerDrawGate10\n')+';'
  body=definition(s,'int PlayerUpdateGate28::GetDamageAtPosition(')
  program=r'''
#include <cassert>
#include <cstring>
#include <cmath>
#include <limits>
#define __stdcall
#define __cdecl
#define __fastcall
#define __declspec(x)
#include "EnemyGameState.hpp"
'''+types+r'''
// HOST constructors supply defined prefill; only the actual damage body runs.
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
static EnemyGameStateView game;EnemyGameStateView*g_EnemyGameState=&game;
static PlayerDrawGate10 draw;PlayerDrawGate10*g_PlayerDrawGate10=&draw;
int g_GameStatisticsIndex0;
'''+body+r'''
int main(){
 PlayerUpdateGate28 owner={};g_EnemyGameState=NULL;g_PlayerDrawGate10=NULL;assert(owner.GetDamageAtPosition(NULL)==0);g_EnemyGameState=&game;g_PlayerDrawGate10=&draw;
 owner.value28=-1;owner.radius=5;owner.position={0,0,17};
 for(unsigned flags:{0u,1u,2u,3u})for(int mode:{-1,0,1,2})for(int gate:{0,1,-1})for(int statistic:{0,1,-1}){
  game.flags378C=flags;owner.mode=mode;draw.value10=gate;g_GameStatisticsIndex0=statistic;
  int inside=(flags&1)?(mode==0?3:38):(gate?38:5);
  int outside=(flags&1)?(mode==1?(statistic?23:26):0):((gate&&mode==0)?(statistic?16:20):0);
  PlayerFloat3 p={0,0,-999};assert(owner.GetDamageAtPosition(&p)==inside);
  p={3,4,0};assert(owner.GetDamageAtPosition(&p)==outside); // equality is outside
  p={4,4,0};assert(owner.GetDamageAtPosition(&p)==outside);
  p={std::nextafter(5.0f,0.0f),0,0};assert(owner.GetDamageAtPosition(&p)==inside);
  p={std::nextafter(5.0f,std::numeric_limits<float>::infinity()),0,0};assert(owner.GetDamageAtPosition(&p)==outside);
  p={std::numeric_limits<float>::quiet_NaN(),0,0};assert(owner.GetDamageAtPosition(&p)==outside);
  p={0,std::numeric_limits<float>::quiet_NaN(),0};assert(owner.GetDamageAtPosition(&p)==outside);
  p={0,0,0};owner.radius=std::numeric_limits<float>::quiet_NaN();assert(owner.GetDamageAtPosition(&p)==outside);
  owner.radius=-5;assert(owner.GetDamageAtPosition(&p)==inside);owner.radius=0;assert(owner.GetDamageAtPosition(&p)==outside);owner.radius=5;
  assert(owner.value28==-1&&owner.mode==mode&&owner.position.z==17&&game.flags378C==flags&&draw.value10==gate&&g_GameStatisticsIndex0==statistic);
 }
 owner.value28=0;owner.radius=std::numeric_limits<float>::quiet_NaN();assert(owner.GetDamageAtPosition(NULL)==0);
}
'''
  program=program.replace('#include <limits>','#include <limits>\n#include <initializer_list>')
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='player-aux-') as d:
   p=Path(d);pending=['EnemyGameState.hpp'];seen=set()
   while pending:
    name=pending.pop()
    if name in seen:continue
    seen.add(name);text=(ROOT/'src'/name).read_text()
    pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"',text,re.M))
    text=re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;','// HOST native assertion omitted.',text)
    text=text.replace('AnmVmTimerView() { flags &= ~1u; }','AnmVmTimerView() : flags(0) {}')
    (p/name).write_text(text)
   (p/'test.cpp').write_text(program)
   subprocess.run(['nice','-n','10','c++','-std=c++17','-O0','-I',str(p),str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
