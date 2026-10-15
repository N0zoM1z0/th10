#!/usr/bin/env python3
"""Run the maintained Player state switch and timer setter with synthetic sinks.

This does not execute game IO or prove native ABI, x87 precision, or the common
post-switch effect/collision/timer tail.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[1]

class PlayerUpdateState(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_state_switch(self):
        source=(ROOT/'src/Player.cpp').read_text()
        start=source.index('int __stdcall PlayerUpdateCallbackBody(Player *player)')
        start=source.index('    switch (player->runtimeState)',start)
        end=source.index('\n    for (i = 0; i < 32; ++i)',start)
        state=source[start:end]
        header=(ROOT/'src/Player.hpp').read_text()
        start=header.index('struct PlayerTimerView\n{')
        end=header.index('\n};',start)+3
        timer=header[start:end]
        program=r"""
#include <cassert>
#include <vector>
#include <string>
#include <cstring>
#define __forceinline inline
#define __declspec(x)
float g_AnmGameSpeed=1.0f;
"""+timer+r"""
struct PlayerFloat3 {float x=0,y=0,z=0;};
struct Pair {int x=0,y=0;};
struct Option {int resetFlag=0;};
struct Player {
 int runtimeState=0,positionX=0,positionY=0,optionCount=0;
 PlayerTimerView updateTimer1,highlightTimer;
 PlayerFloat3 drawPosition,savedDrawPosition;
 Option options[4]; Pair replayPositionHistory[33];
};
struct PlayerHostileRowView {int state446=0,value4=0;};
struct Hostile {PlayerHostileRowView rows[2000];} hostile;
Hostile *g_PlayerHostileManager=&hostile;
struct PlayerLinkedObjectView {PlayerLinkedObjectView *next=nullptr;int value0C=0;};
struct Linked {PlayerLinkedObjectView *head=nullptr;} linked;
Linked *g_PlayerLinkedListOwner=&linked;
struct Gate {int value9EB8=0,value28=0,value10=0;} gate9,gate28,gate10;
Gate *g_PlayerDrawGate9EB8=&gate9,*g_PlayerUpdateGate28=&gate28,*g_PlayerUpdateGate10=&gate10;
short g_PlayerPower=40;
unsigned g_PlayerInputBits=2;
int g_PlayerRuntimeGateValue=0;
float g_PlayerTimerScale=1.0f;
static Player *active;
static std::vector<std::string> events;
static int expectedPower,expectedState,expectedUpdate,expectedHighlight;
static void PlayerNotifyPowerTransition(Gate *g){
 assert(g==&gate28&&active->runtimeState==expectedState);
 assert(g_PlayerPower==expectedPower);
 assert(active->updateTimer1.current==expectedUpdate);
 assert(active->highlightTimer.current==expectedHighlight);
 events.push_back("notify");
}
static void RebuildPlayerOptions(Player *p){
 assert(p==active&&g_PlayerPower==expectedPower-20);
 assert(events==std::vector<std::string>({"notify"}));
 p->optionCount=g_PlayerPower/20;events.push_back("options");
}
static void PlayerUpdatePowerDisplay(int a,int b,int c){
 assert(events==std::vector<std::string>({"notify","options"}));
 assert(a==g_PlayerPower/20&&a==b&&c==(g_PlayerPower%20)*100/20);
 assert(active->optionCount==a);events.push_back("display");
}
static void PlayerStartPowerDisplayTimer(int n){assert(n==3000);events.push_back("display-timer");}
static void PlayerEmitStateTransitionEffect(float,int,int){}
static void PlayerCommitStateTransitionEffect(float,int){}
static void PlayerInvokeHostileCleanup(PlayerHostileRowView*){}
static void PlayerInvokeLinkedObjectCallback(PlayerLinkedObjectView*){}
static void PlayerUpdateMovementAndOptions(Player*){events.push_back("movement");}
static void PlayerTransitionToState2(Player*p){assert(p==active&&p->updateTimer1.current==8);events.push_back("state2");p->runtimeState=2;p->updateTimer1.SetCurrent(10);}
static float PlayerAngleToPoint(Player*,const PlayerFloat3*){assert(false);return 0;}
static void PlayerSpawnDirectionalEffect(const PlayerFloat3*,int,unsigned,float,float){assert(false);}
static void PlayerRequestRuntimeTransition(int n){assert(n==4);events.push_back("runtime4");}
static void PlayerFallbackRuntimeTransition(){events.push_back("fallback");}
static void PlayerSpawnEffectRow(Player*p,const PlayerFloat3*pos,float a,float b,int c,int d){
 assert(p==active&&pos==&p->drawPosition&&a==32&&b==16&&c==30&&d==150);
 assert(p->runtimeState==0&&g_PlayerTimerScale==1);assert(p->drawPosition.x==12&&p->drawPosition.y==34&&p->drawPosition.z==56);
 p->drawPosition={13,35,57};events.push_back("respawn");
}
static void PlayerFinalizeState3Transition(){}
static void run(Player *player){int i;
"""+state+r"""
}
static void setup(Player &p,int state,int time,int power){
 p.runtimeState=state;p.updateTimer1.flags=0;p.highlightTimer.flags=0;
 p.updateTimer1.SetCurrent(time);p.highlightTimer.SetCurrent(5);
 active=&p;events.clear();g_PlayerPower=power;g_PlayerInputBits=2;
 gate28.value28=0;g_PlayerUpdateGate28=&gate28;gate9.value9EB8=0;g_PlayerRuntimeGateValue=0;gate10.value10=0;
 expectedState=state;expectedPower=power;
}
int main(){
 Player p;
 // Successful state-4 transition: real timer methods execute before notify.
 setup(p,4,7,40);expectedUpdate=60;expectedHighlight=200;
 float custom=0.5f;p.updateTimer1.scale=&custom;
 p.highlightTimer.flags=0; // lazy initialization must select the maintained speed
 run(&p);
 assert(events==std::vector<std::string>({"notify","options","display"}));
 assert(p.runtimeState==1&&g_PlayerPower==20&&p.optionCount==1);
 assert(p.updateTimer1.current==60&&p.updateTimer1.previous==59&&p.updateTimer1.subframe==60);
 assert(p.highlightTimer.current==200&&p.highlightTimer.previous==199&&p.highlightTimer.subframe==200);
 assert(p.updateTimer1.scale==&custom&&p.highlightTimer.scale==&g_AnmGameSpeed);
 // Threshold boundary, and independently suppressed gate/input paths.
 setup(p,4,7,19);run(&p);assert(events.empty()&&p.runtimeState==4&&g_PlayerPower==19&&p.updateTimer1.current==7&&p.highlightTimer.current==5);
 setup(p,4,7,20);expectedUpdate=60;expectedHighlight=200;run(&p);assert(g_PlayerPower==0&&p.optionCount==0&&p.runtimeState==1);
 setup(p,4,7,40);g_PlayerInputBits=0;run(&p);assert(events.empty()&&g_PlayerPower==40);
 setup(p,4,7,40);gate28.value28=1;run(&p);assert(events.empty()&&g_PlayerPower==40);
 setup(p,4,7,40);g_PlayerUpdateGate28=nullptr;run(&p);assert(events.empty()&&g_PlayerPower==40);
 setup(p,4,8,40);run(&p);assert(events==std::vector<std::string>({"state2"})&&p.runtimeState==2&&g_PlayerPower==40&&p.updateTimer1.current==10);
 // State 1 sets only the highlight timer, before power and option publication.
 setup(p,1,31,40);expectedUpdate=31;expectedHighlight=270;run(&p);
 assert(events==std::vector<std::string>({"notify","options","display","display-timer","movement"}));
 assert(p.updateTimer1.current==31&&p.highlightTimer.current==270&&p.highlightTimer.previous==269);
 setup(p,1,31,19);run(&p);assert(events==std::vector<std::string>({"movement"})&&g_PlayerPower==19&&p.highlightTimer.current==5);
 setup(p,1,31,40);gate9.value9EB8=1;run(&p);assert(events==std::vector<std::string>({"movement"})&&g_PlayerPower==40&&p.highlightTimer.current==5);
 setup(p,1,31,40);g_PlayerInputBits=0;run(&p);assert(events==std::vector<std::string>({"movement"})&&g_PlayerPower==40&&p.highlightTimer.current==5);
 // Suppressed respawn paths leave both timers and position unchanged.
 setup(p,2,29,40);p.drawPosition={12,34,56};run(&p);assert(events.empty()&&p.runtimeState==2&&p.drawPosition.y==34&&p.updateTimer1.current==29&&p.highlightTimer.current==5);
 setup(p,2,30,40);g_PlayerRuntimeGateValue=-1;gate10.value10=1;run(&p);assert(events==std::vector<std::string>({"runtime4"})&&p.runtimeState==2&&p.updateTimer1.current==30&&p.highlightTimer.current==5);
 setup(p,2,30,40);g_PlayerRuntimeGateValue=-1;run(&p);assert(events==std::vector<std::string>({"fallback"})&&p.runtimeState==2&&p.updateTimer1.current==30&&p.highlightTimer.current==5);
 // Respawn sets highlight=280 and update=0 after saved position publication.
 setup(p,2,30,40);p.drawPosition={12,34,56};g_PlayerTimerScale=0.25f;
 run(&p);
 assert(events==std::vector<std::string>({"respawn"}));
 assert(p.runtimeState==0&&p.savedDrawPosition.x==13&&p.savedDrawPosition.y==35&&p.savedDrawPosition.z==57);
 assert(p.positionX==0&&p.positionY==48000&&p.drawPosition.x==0&&p.drawPosition.y==480&&p.drawPosition.z==57);
 assert(p.highlightTimer.current==280&&p.highlightTimer.previous==279&&p.highlightTimer.subframe==280);
 assert(p.updateTimer1.current==0&&p.updateTimer1.previous==-1&&p.updateTimer1.subframe==0);
}
"""
        (ROOT/'build').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='player-state-') as d:
            p=Path(d);(p/'test.cpp').write_text(program)
            subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

if __name__=='__main__':
    unittest.main()
