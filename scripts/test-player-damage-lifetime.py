#!/usr/bin/env python3
"""Actual damage shot loop with mutable callback and explicit boundary-stub tests."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class DamageLifetime(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_descriptor_lifetimes(self):
  text=(ROOT/'src/Player.cpp').read_text();a=text.index('int Player::CalculateDamageToTarget(');b=text.index('    damage += g_PlayerUpdateGate28->GetDamageAtPosition',a)
  function=text[a:b]+'    return damage;\n}\n'
  program=r'''
#include <cassert>
#include <cstddef>
#include <vector>
struct PlayerFloat3{float x,y,z;};struct Player;struct PlayerShotRuntimeView;
struct PlayerShotDescriptorView{float hitboxExtentX,hitboxExtentY;unsigned char type;short damage,hitAnimationScript;int(*collisionCallback)(Player*,PlayerShotRuntimeView*,const PlayerFloat3*);};
struct PlayerTimerView{int current,previous;int HasIntervalTick(int)const;};
struct PlayerShotRuntimeView{int state;PlayerShotDescriptorView const*descriptor;struct{PlayerFloat3 position;float speedOrAngleStep;}motion;int collisionVmTransitionPending,collidedThisFrame;unsigned primaryVmId;PlayerTimerView timer;};
struct AnmVmView{struct{float z;}rotation;unsigned flags35C,id;};
struct AnmVmIdView{int value;AnmVmView*GetVm();void Release();};
struct AnmLoadedView{AnmVmIdView CreateVmVariant0(int,unsigned);};
static AnmLoadedView loaded;
struct Player{PlayerTimerView updateTimer1;PlayerShotRuntimeView shots[128];void*resource;int CalculateDamageToTarget(const PlayerFloat3*,const PlayerFloat3*,int*);};
enum{PLAYER_SHOT_INACTIVE=0,PLAYER_SHOT_HIT_TRANSITION=2,PLAYER_SHOT_TYPE_3=3,PLAYER_VM_LAYER=15,PLAYER_VM_SHOT_ANGLE_DIRTY=4};
static Player player;static PlayerShotDescriptorView a,b,c,d,e;static AnmVmView oldVm,newVm;static int mode,resolveCount,effectDamage;static std::vector<int> events;
static PlayerFloat3 position={0,10,0},size={10,10,0};
int collision(Player*p,PlayerShotRuntimeView*s,const PlayerFloat3*t){assert(p==&player&&s==&player.shots[0]&&t==&position);events.push_back(1);s->descriptor=&b;return mode==4;}
void PlayerSetShotVmInterrupt2(unsigned*id){assert(id==&player.shots[0].primaryVmId);events.push_back(2);if(mode==2)player.shots[0].descriptor=&c;}
int PlayerTimerView::HasIntervalTick(int n)const{assert(n==4&&this==&player.shots[0].timer);events.push_back(3);
 // Mode5 is deliberately an adversarial boundary stub. The real interval helper does not mutate descriptors.
 if(mode==5){b.type=1;b.damage=99;player.shots[0].descriptor=&c;}return current!=previous&&current%n==0;}
AnmVmView*AnmVmIdView::GetVm(){assert(reinterpret_cast<void*>(this)==static_cast<void*>(&player.shots[0].primaryVmId));events.push_back(4);if(++resolveCount==1){assert(value==9);return &oldVm;}assert(value==12);if(mode==3)player.shots[0].descriptor=&e;return &newVm;}
void AnmVmIdView::Release(){assert(value==9);events.push_back(5);value=0;if(mode==3)player.shots[0].descriptor=&c;}
AnmVmIdView AnmLoadedView::CreateVmVariant0(int script,unsigned layer){events.push_back(6);assert(this==&loaded&&player.resource==this&&layer==15&&player.shots[0].primaryVmId==0);assert(script==(mode==3?82:10));if(mode==3)player.shots[0].descriptor=&d;return AnmVmIdView{12};}
void PlayerSpawnEffectRow(Player*p,const PlayerFloat3*v,float radius,float delta,int frames,int damage){assert(p==&player&&v==&player.shots[0].motion.position&&radius==32&&delta==1.3999999761581421f&&frames==13);events.push_back(7);effectDamage=damage;}
'''+function+r'''
void setup(int m){mode=m;player={};events.clear();resolveCount=0;effectDamage=-1;a={4,4,1,6,5,collision};b={4,4,3,21,5,NULL};c={4,4,3,33,77,NULL};d={4,4,2,30,9,NULL};e={4,4,2,60,9,NULL};oldVm={0.75f,0,9};newVm={-1,0,12};player.resource=&loaded;player.updateTimer1={1,0};auto&s=player.shots[0];s.state=1;s.descriptor=&a;s.motion.position={0,10,0};s.motion.speedOrAngleStep=8;s.timer={4,3};s.primaryVmId=9;}
int main(){int hit=99;
 setup(1);assert(player.CalculateDamageToTarget(&position,&size,&hit)==21&&hit==0);assert(events==std::vector<int>({1,2,3}));assert(player.shots[0].state==1&&player.shots[0].collidedThisFrame==1);
 setup(2);assert(player.CalculateDamageToTarget(&position,&size,NULL)==33);assert(events==std::vector<int>({1,2,3}));
 setup(3);b.type=1;b.damage=11;assert(player.CalculateDamageToTarget(&position,&size,NULL)==11);assert(events==std::vector<int>({1,2,4,5,6,4,7}));assert(effectDamage==20&&newVm.rotation.z==0.75f&&newVm.flags35C==4&&player.shots[0].primaryVmId==12);assert(player.shots[0].state==2&&player.shots[0].motion.position.z==0.1f&&player.shots[0].motion.speedOrAngleStep==1);
 setup(4);assert(player.CalculateDamageToTarget(&position,&size,NULL)==0);assert(events==std::vector<int>({1})&&player.shots[0].collidedThisFrame==0);
 setup(5);c.type=2;c.damage=30;assert(player.CalculateDamageToTarget(&position,&size,NULL)==99);assert(events==std::vector<int>({1,2,3,7}));assert(effectDamage==10&&resolveCount==0&&player.shots[0].state==1);
 setup(1);player.shots[0].timer.current=5;assert(player.CalculateDamageToTarget(&position,&size,NULL)==0);assert(events==std::vector<int>({1,2,3}));
 setup(1);player.shots[0].collisionVmTransitionPending=1;assert(player.CalculateDamageToTarget(&position,&size,NULL)==21);assert(events==std::vector<int>({1,3}));
 setup(1);player.updateTimer1.previous=1;hit=99;assert(player.CalculateDamageToTarget(&position,&size,&hit)==0&&hit==99&&events.empty());
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='player-damage-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
