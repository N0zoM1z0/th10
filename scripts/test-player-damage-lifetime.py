#!/usr/bin/env python3
"""Actual damage shot loop with mutable callback and explicit boundary-stub tests."""
from pathlib import Path
import shutil,struct,subprocess,tempfile,unittest
from fractions import Fraction
ROOT=Path(__file__).resolve().parents[1]
class DamageLifetime(unittest.TestCase):
 def test_precision_model_witness(self):
  # Exact dyadic model of the reviewed x87 instructions, not host execution proof.
  half_extent=Fraction(3,2**26)
  extended_min=Fraction(1)-half_extent
  rounded_min=Fraction.from_float(struct.unpack('<f',struct.pack('<f',float(extended_min)))[0])
  target_y=Fraction(1)-Fraction(1,2**24)
  self.assertEqual(rounded_min,target_y)
  self.assertGreater(extended_min,target_y)
  # Target/repaired spill-reload accepts equality; old retained minY rejects it.
  self.assertFalse(rounded_min>target_y)
  self.assertTrue(extended_min>target_y)

 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_descriptor_lifetimes(self):
  text=(ROOT/'src/Player.cpp').read_text();a=text.index('int Player::CalculateDamageToTarget(');b=text.index('// Target 0x00425730',a)
  function=text[a:b]
  program=r'''
#include <cassert>
#include <cstddef>
#include <vector>
#include <cmath>
#include <limits>
struct PlayerFloat3{float x,y,z;};struct Player;struct PlayerShotRuntimeView;
struct PlayerShotDescriptorView{float hitboxExtentX,hitboxExtentY;unsigned char type;short damage,hitAnimationScript;int(*collisionCallback)(Player*,PlayerShotRuntimeView*,const PlayerFloat3*);};
struct PlayerTimerView{int current,previous;int HasIntervalTick(int)const;};
struct PlayerShotRuntimeView{int state;PlayerShotDescriptorView const*descriptor;struct{PlayerFloat3 position;float speedOrAngleStep;}motion;int collisionVmTransitionPending,collidedThisFrame;unsigned primaryVmId;PlayerTimerView timer;};
struct AnmVmView{struct{float z;}rotation;unsigned flags35C,id;};
struct AnmVmIdView{int value;AnmVmView*GetVm();void Release();};
struct AnmLoadedView{AnmVmIdView CreateVmVariant0(int,unsigned);};
static AnmLoadedView loaded;
struct PlayerEffectMotionView{unsigned activeFlags;PlayerTimerView timer;int collisionInterval,damage,hitAccumulator,hitCap;};
struct PlayerEffectRowView{PlayerFloat3 position;float radius,angle,extentX,extentY;PlayerEffectMotionView motion;};
struct Auxiliary{int GetDamageAtPosition(const PlayerFloat3*){return 0;}};
static Auxiliary auxiliary;static Auxiliary*g_PlayerUpdateGate28=&auxiliary;static int g_GameScore;
struct Player{PlayerEffectRowView effectRows[33];PlayerTimerView updateTimer1;PlayerShotRuntimeView shots[128];void*resource;int CalculateDamageToTarget(const PlayerFloat3*,const PlayerFloat3*,int*);};
enum{PLAYER_SHOT_INACTIVE=0,PLAYER_SHOT_HIT_TRANSITION=2,PLAYER_SHOT_TYPE_3=3,PLAYER_VM_LAYER=15,PLAYER_VM_SHOT_ANGLE_DIRTY=4};
static Player player;static PlayerShotDescriptorView a,b,c,d,e;static AnmVmView oldVm,newVm;static int mode,resolveCount,effectDamage,sizeMutationIndex;static std::vector<int> events;
static PlayerFloat3 position={0,10,0},size={10,10,0};
int collision(Player*p,PlayerShotRuntimeView*s,const PlayerFloat3*t){assert(p==&player&&s==&player.shots[0]&&t==&position);events.push_back(1);if(mode==6){if(sizeMutationIndex!=1)size.x=20;if(sizeMutationIndex!=0)size.y=20;return 1;}s->descriptor=&b;return mode==4;}
void PlayerSetShotVmInterrupt2(unsigned*id){assert(id==&player.shots[0].primaryVmId);events.push_back(2);if(mode==2)player.shots[0].descriptor=&c;}
int PlayerTimerView::HasIntervalTick(int n)const{assert(n==4&&this==&player.shots[0].timer);events.push_back(3);
 // Mode5 is deliberately an adversarial boundary stub. The real interval helper does not mutate descriptors.
 if(mode==5){b.type=1;b.damage=99;player.shots[0].descriptor=&c;}return current!=previous&&current%n==0;}
AnmVmView*AnmVmIdView::GetVm(){assert(reinterpret_cast<void*>(this)==static_cast<void*>(&player.shots[0].primaryVmId));events.push_back(4);if(++resolveCount==1){assert(value==9);return &oldVm;}assert(value==12);if(mode==3)player.shots[0].descriptor=&e;return &newVm;}
void AnmVmIdView::Release(){assert(value==9);events.push_back(5);value=0;if(mode==3)player.shots[0].descriptor=&c;}
AnmVmIdView AnmLoadedView::CreateVmVariant0(int script,unsigned layer){events.push_back(6);assert(this==&loaded&&player.resource==this&&layer==15&&player.shots[0].primaryVmId==0);assert(script==(mode==3?82:10));if(mode==3)player.shots[0].descriptor=&d;return AnmVmIdView{12};}
void PlayerSpawnEffectRow(Player*p,const PlayerFloat3*v,float radius,float delta,int frames,int damage){assert(p==&player&&v==&player.shots[0].motion.position&&radius==32&&delta==1.3999999761581421f&&frames==13);events.push_back(7);effectDamage=damage;}
'''+function+r'''
void setup(int m){mode=m;sizeMutationIndex=-1;player={};events.clear();resolveCount=0;effectDamage=-1;a={4,4,1,6,5,collision};b={4,4,3,21,5,NULL};c={4,4,3,33,77,NULL};d={4,4,2,30,9,NULL};e={4,4,2,60,9,NULL};oldVm={0.75f,0,9};newVm={-1,0,12};player.resource=&loaded;player.updateTimer1={1,0};auto&s=player.shots[0];s.state=1;s.descriptor=&a;s.motion.position={0,10,0};s.motion.speedOrAngleStep=8;s.timer={4,3};s.primaryVmId=9;}
int main(){int hit=99;
 setup(1);assert(player.CalculateDamageToTarget(&position,&size,&hit)==21&&hit==0);assert(events==std::vector<int>({1,2,3}));assert(player.shots[0].state==1&&player.shots[0].collidedThisFrame==1);
 setup(2);assert(player.CalculateDamageToTarget(&position,&size,NULL)==33);assert(events==std::vector<int>({1,2,3}));
 setup(3);b.type=1;b.damage=11;assert(player.CalculateDamageToTarget(&position,&size,NULL)==11);assert(events==std::vector<int>({1,2,4,5,6,4,7}));assert(effectDamage==20&&newVm.rotation.z==0.75f&&newVm.flags35C==4&&player.shots[0].primaryVmId==12);assert(player.shots[0].state==2&&player.shots[0].motion.position.z==0.1f&&player.shots[0].motion.speedOrAngleStep==1);
 setup(4);assert(player.CalculateDamageToTarget(&position,&size,NULL)==0);assert(events==std::vector<int>({1})&&player.shots[0].collidedThisFrame==0);
 setup(5);c.type=2;c.damage=30;assert(player.CalculateDamageToTarget(&position,&size,NULL)==99);assert(events==std::vector<int>({1,2,3,7}));assert(effectDamage==10&&resolveCount==0&&player.shots[0].state==1);
 setup(1);player.shots[0].timer.current=5;assert(player.CalculateDamageToTarget(&position,&size,NULL)==0);assert(events==std::vector<int>({1,2,3}));
 setup(1);player.shots[0].collisionVmTransitionPending=1;assert(player.CalculateDamageToTarget(&position,&size,NULL)==21);assert(events==std::vector<int>({1,3}));
 setup(1);player.updateTimer1.previous=1;hit=99;assert(player.CalculateDamageToTarget(&position,&size,&hit)==0&&hit==99&&events.empty());
 // The mutable callback updates a real non-const targetSize object through
 // its valid global alias. Rotated rows read the new size; axis-aligned rows
 // deliberately retain the original bounds computed before the callback.
 for(int axis=0;axis!=2;++axis){
  setup(6);a.type=3;position={0,0,0};size={2,2,0};player.shots[0].motion.position=position;
  auto&r=player.effectRows[0];r.position={8,0,0};r.extentX=r.extentY=2;
  r.angle=axis?0.0f:1.5707963705062866f;r.motion.activeFlags=1;
  r.motion.damage=7;r.motion.hitCap=100;r.motion.collisionInterval=4;
  assert(player.CalculateDamageToTarget(&position,&size,NULL)==(axis?0:7));
  assert(events==std::vector<int>({1})&&size.x==20&&size.y==20);
  assert(r.motion.hitAccumulator==(axis?0:7)&&r.motion.damage==7);
 }
 // Isolate each fresh component: the other dimension is already broad and
 // remains unchanged, so retaining either old component would reject its case.
 for(int dimension=0;dimension!=2;++dimension){
  setup(6);sizeMutationIndex=dimension;a.type=3;position={0,0,0};
  size=dimension?PlayerFloat3{20,2,0}:PlayerFloat3{2,20,0};
  player.shots[0].motion.position=position;
  auto&r=player.effectRows[0];r.position=dimension?PlayerFloat3{8,0,0}:PlayerFloat3{0,8,0};
  r.extentX=r.extentY=2;r.angle=1.5707963705062866f;r.motion.activeFlags=1;
  r.motion.damage=7;r.motion.hitCap=100;r.motion.collisionInterval=4;
  assert(player.CalculateDamageToTarget(&position,&size,NULL)==7);
  assert(events==std::vector<int>({1})&&size.x==20&&size.y==20);
  assert(r.motion.hitAccumulator==7&&r.motion.damage==7);
 }
 // Actual maintained source under the host compiler: validates the finite fixture,
 // not pinned x87 equivalence (the old scalar source can also pass on SSE hosts).
 setup(1);a.type=3;a.collisionCallback=NULL;a.damage=21;a.hitboxExtentX=10;
 a.hitboxExtentY=std::ldexp(3.0f,-25);player.shots[0].motion.position={0,1,0};
 position={0,1.0f-std::ldexp(1.0f,-24),0};size={0,0,0};
 assert(player.CalculateDamageToTarget(&position,&size,NULL)==21);assert(events==std::vector<int>({2,3}));
 events.clear();player.shots[0].collisionVmTransitionPending=0;
 position.y=std::nextafter(position.y,0.0f);
 assert(player.CalculateDamageToTarget(&position,&size,NULL)==0&&events.empty());
 // Unordered comparisons do not reject a type-3 shot in these four bound tests.
 position.y=std::numeric_limits<float>::quiet_NaN();
 assert(player.CalculateDamageToTarget(&position,&size,NULL)==21);assert(events==std::vector<int>({2,3}));

}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='player-damage-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
