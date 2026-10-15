#!/usr/bin/env python3
"""Execute the actual affected ECL/damage block with callback mutation sinks.
This excludes preceding movement and following collision/VM/timer work; it is
not a native x87/ABI or full-callback runtime test.
"""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class EnemyRuntimeScale(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_callback_scale_reload(self):
  text=(ROOT/'src/Enemy.cpp').read_text();a=text.index('    const float timerScale = *enemy->updateTimer.scale;');b=text.index('\n    const unsigned char *callback = EnemyResolveRuntimeCallback(enemy->owner);',a)
  block=text[a:b]
  program=r'''
#include <cassert>
#include <vector>
#include <string>
#define __stdcall
struct PlayerFloat3 {float x,y,z;};
struct Timer {float *scale;int current;};
struct Owner {void ReleaseEclAllocations();void ResetEclState();};
struct EnemyRuntimeView {unsigned flags;Timer updateTimer,damageReductionTimer;Owner*owner;struct{PlayerFloat3 position;}worldMotion;PlayerFloat3 damageHitbox;int life,scoreReward;};
static std::vector<std::string> events;
static std::vector<float> scales;
static int damage=10,mutation=0,firstResult=0,secondResult=0,deathResult=0;
static bool hasCallback=true;
static float initialScale=1,replacementScale=0.5f;
static EnemyRuntimeView *active;
void Owner::ReleaseEclAllocations(){assert(this==active->owner);events.push_back("release");}
void Owner::ResetEclState(){assert(this==active->owner);events.push_back("reset");}
struct Player {int runtimeState;int CalculateDamageToTarget(const PlayerFloat3*p,const PlayerFloat3*h,void*o){assert(p==&active->worldMotion.position&&h==&active->damageHitbox&&o==nullptr);events.push_back("damage");return damage;}} player;
Player*g_Player=&player;
struct GameState {unsigned flags378C;} game;
GameState*g_EnemyGameState=&game;int g_EnemyDamageProcessedFlag;
int EnemyRunEcl(Owner*o,float scale){assert(o==active->owner);scales.push_back(scale);events.push_back("run"+std::to_string(scales.size()));return scales.size()==1?firstResult:secondResult;}
static unsigned char name;
const unsigned char*EnemyResolveRuntimeCallback(Owner*o){assert(o==active->owner);events.push_back("resolve");return hasCallback?&name:nullptr;}
void EnemyRunCallbackEcl(Owner*o,const unsigned char*n){assert(o==active->owner&&n==&name);events.push_back("callback");assert(scales==std::vector<float>({1.0f}));if(mutation==1)*active->updateTimer.scale=0.25f;else if(mutation==2)active->updateTimer.scale=&replacementScale;}
void EnemyAddScoreReward(int n){assert(n==active->scoreReward);events.push_back("score");}
int EnemyFinalizeDeath(Owner*o){assert(o==active->owner);events.push_back("death");return deathResult;}
static int runAffectedSource(EnemyRuntimeView*enemy){
'''+block+r'''
 return 0;
}
static Owner owner;
static EnemyRuntimeView enemy;
static void setup(){events.clear();scales.clear();initialScale=1;replacementScale=0.5f;enemy={};enemy.updateTimer.scale=&initialScale;enemy.owner=&owner;enemy.life=100;enemy.scoreReward=200;active=&enemy;game.flags378C=0;player.runtimeState=1;damage=10;mutation=0;firstResult=secondResult=deathResult=0;hasCallback=true;g_EnemyDamageProcessedFlag=0;}
int main(){
 const std::vector<std::string> path={"run1","damage","resolve","release","reset","callback","run2"};
 setup();mutation=1;assert(runAffectedSource(&enemy)==0);assert(scales==std::vector<float>({1,0.25f}));assert(events==path&&enemy.life==90&&(enemy.flags&0x2000)&&g_EnemyDamageProcessedFlag==1);
 setup();mutation=2;assert(runAffectedSource(&enemy)==0);assert(scales==std::vector<float>({1,0.5f}));assert(initialScale==1&&enemy.updateTimer.scale==&replacementScale&&events==path);
 setup();mutation=2;secondResult=1;assert(runAffectedSource(&enemy)==-1);assert(scales==std::vector<float>({1,0.5f})&&events==path);assert(g_EnemyDamageProcessedFlag==0&&(enemy.flags&0x2000)==0);
 setup();firstResult=1;assert(runAffectedSource(&enemy)==-1);assert(events==std::vector<std::string>({"run1"})&&enemy.life==100&&g_EnemyDamageProcessedFlag==0);
 setup();hasCallback=false;assert(runAffectedSource(&enemy)==0);assert(events==std::vector<std::string>({"run1","damage","resolve"})&&scales.size()==1);
 setup();damage=0;assert(runAffectedSource(&enemy)==0);assert(events==std::vector<std::string>({"run1","damage"})&&scales.size()==1&&g_EnemyDamageProcessedFlag==1);
 setup();enemy.flags=1;assert(runAffectedSource(&enemy)==0);assert(events==std::vector<std::string>({"run1"})&&g_EnemyDamageProcessedFlag==0);
 setup();enemy.life=1;mutation=1;deathResult=1;assert(runAffectedSource(&enemy)==1);assert(scales==std::vector<float>({1,0.25f}));assert(events[events.size()-2]=="score"&&events.back()=="death"&&g_EnemyDamageProcessedFlag==0);
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='enemy-scale-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
