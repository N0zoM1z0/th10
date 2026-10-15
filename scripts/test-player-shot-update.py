#!/usr/bin/env python3
"""Actual shot-update descriptor lifetimes with explicit synthetic boundaries."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class ShotUpdate(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_update_descriptor_reloads(self):
  s=(ROOT/'src/Player.cpp').read_text();a=s.index('int PlayerUpdateShots(Player *player)\n{');b=s.index('// Maintained source for the 0x00428630',a);body=s[a:b]
  program=r'''
#include <cassert>
#include <vector>
struct PlayerFloat3{float x,y,z;};struct Player;struct PlayerShotRuntimeView;
struct PlayerShotDescriptorView{unsigned char type;signed char sourceIndex;void(*updateCallback)(Player*,PlayerShotRuntimeView*);};
struct PlayerTimerView{int current,previous;float subframe;float*scale;};
struct PlayerShotMotionView{unsigned flags;PlayerFloat3 position,velocity;float angle,speedOrAngleStep,polarMagnitude,polarMagnitudeDelta;};
struct PlayerShotRuntimeView{int state;const PlayerShotDescriptorView*descriptor;PlayerShotMotionView motion;unsigned primaryVmId,secondaryVmId;int collidedThisFrame,collisionVmTransitionPending;PlayerTimerView timer;};
struct Sprite{float extent30,extent34;};
struct PlayerVm{Sprite*sprite;float scale3C,scale40;PlayerFloat3 position;unsigned flags;float angle2C;};
struct Player{PlayerShotRuntimeView shots[128];PlayerTimerView updateTimer0;int optionCount,shotSourceActive[4];};
struct Gate{int value9EB8;};static Gate gate;static Gate*g_PlayerDrawGate9EB8=&gate;static Gate*g_PlayerDrawGate10=&gate;
enum{PLAYER_SHOT_INACTIVE=0,PLAYER_SHOT_ACTIVE=1,PLAYER_SHOT_HIT_TRANSITION=2,PLAYER_SHOT_TYPE_3=3,PLAYER_SHOT_POLAR_MOTION=1,PLAYER_VM_SHOT_ANGLE_SOURCE=0x8000000,PLAYER_VM_SHOT_ANGLE_DIRTY=4};
static Player player;static PlayerShotDescriptorView a,b;static PlayerVm vm;static Sprite sprite;static float scale=1;static int mode,culls;static std::vector<int>events;
void updateA(Player*p,PlayerShotRuntimeView*s){assert(p==&player&&s==&player.shots[0]);events.push_back(1);s->descriptor=&b;s->motion.position.x=500;}
void updateB(Player*p,PlayerShotRuntimeView*s){assert(p==&player&&s==&player.shots[0]);events.push_back(2);}
void PlayerSetShotVmDeleteState1(unsigned*id){assert(id==&player.shots[0].primaryVmId||id==&player.shots[0].secondaryVmId);events.push_back(3);
 // Adversarial boundary fixture only: the real interrupt helper is not claimed to replace descriptors.
 if(mode==3||mode==4)player.shots[0].descriptor=&b;}
void PlayerSetShotVmDeleteState3(unsigned*id){assert(id==&player.shots[0].primaryVmId);events.push_back(4);if(mode==5)player.shots[0].descriptor=&b;}
void PlayerSetShotVelocityFromPolar(PlayerFloat3*v,float,float){events.push_back(5);*v={0,0,0};}
float PlayerWrapShotAngle(float x){return x;}
void PlayerAdvanceShotMotion(PlayerShotMotionView*m){assert(m==&player.shots[0].motion);events.push_back(6);}
PlayerVm*FindPlayerVm(unsigned id){events.push_back(7);return id==9?&vm:NULL;}
void PlayerMarkShotVmPending(unsigned id){assert(id==9||id==10);events.push_back(8);}
struct PlayerShotBoundsView{int IsOutsidePlayfield(float,float){++culls;events.push_back(9);return 1;}};
'''+body+r'''
void setup(){player={};gate={};g_PlayerDrawGate9EB8=&gate;g_PlayerDrawGate10=&gate;mode=0;culls=0;events.clear();a={1,1,updateA};b={3,2,NULL};sprite={1,1};vm={&sprite,1,1,{},0,0};player.optionCount=4;player.updateTimer0.current=1;for(int&i:player.shotSourceActive)i=1;auto&s=player.shots[0];s.state=1;s.descriptor=&a;s.primaryVmId=9;s.motion.position={0,10,0};s.timer={10,9,10,&scale};}
int main(){
 // Real mutable update-callback contract: descriptor changes alter the later cull decision.
 setup();assert(PlayerUpdateShots(&player)==0);assert(culls==0&&player.shots[0].state==1);assert(events==std::vector<int>({1,5,6,7}));assert(vm.position.x==724&&player.shots[0].timer.current==11);
 setup();a.type=3;b.type=1;PlayerUpdateShots(&player);assert(culls==1&&player.shots[0].state==0&&player.shots[0].primaryVmId==0);assert(events==std::vector<int>({1,5,6,7,9,8}));assert(player.shots[0].timer.current==10);
 setup();a.updateCallback=NULL;PlayerUpdateShots(&player);assert(culls==1&&events==std::vector<int>({5,6,7,9,8}));
 // The two interrupt-pair paths clear the reloaded descriptor's source slot.
 for(int m=3;m<=4;++m){setup();mode=m;a.type=3;b.updateCallback=updateB;if(m==3)player.updateTimer0.current=-1;else gate.value9EB8=1;PlayerUpdateShots(&player);assert(player.shotSourceActive[0]==1&&player.shotSourceActive[1]==0);assert(player.shots[0].state==2&&culls==0);assert(events==std::vector<int>({3,3,2,5,6,7}));}
 // A descriptor reload after interrupt3 also selects the current callback (or null).
 for(int hasCallback=0;hasCallback!=2;++hasCallback){setup();mode=5;a.type=3;b.updateCallback=hasCallback?updateB:NULL;player.shots[0].collisionVmTransitionPending=1;PlayerUpdateShots(&player);assert(player.shots[0].collisionVmTransitionPending==0&&culls==0);assert(events==(hasCallback?std::vector<int>({4,2,5,6,7}):std::vector<int>({4,5,6,7})));}
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='player-shot-update-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
