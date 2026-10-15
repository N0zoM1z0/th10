#!/usr/bin/env python3
"""Actual movement/options owner with maintained ANM position/interrupt bodies."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
def function(text,marker):
 a=text.index(marker);i=text.index('{',a);depth=1;j=i+1
 while depth:
  depth+=(text[j]=='{')-(text[j]=='}');j+=1
 return text[a:j]
class MovementAnm(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_identity_seams(self):
  player=(ROOT/'src/Player.cpp').read_text();anm=(ROOT/'src/AnmManager.cpp').read_text()
  body=function(player,'int PlayerUpdateMovementAndOptions(Player *player)')
  helpers='\n'.join(function(anm,'void __stdcall AnmRenderManagerView::'+n+'(') for n in ['SetVmPosition','SetVmWorldPosition','SetVmPendingInterrupt'])
  program=r'''
#include <cassert>
#include <vector>
#define __stdcall
struct AnmFloat3View{float x,y,z;};using PlayerFloat3=AnmFloat3View;
struct AnmVmIdView{int value;AnmVmIdView(int v=0):value(v){}};
struct AnmVmLayerNodeView{void*owner;AnmVmLayerNodeView*next,*previous;};
struct AnmVmView{AnmFloat3View positionOffset;AnmVmLayerNodeView layerNode;short pendingInterrupt;int renderLayer;unsigned flags35C;};
struct PlayerPositionPair{int x,y;};
struct PlayerOptionRuntime{int state;PlayerPositionPair replayPair0,replayPair1,replayPair2,replayPair3;void(*updateCallback)(PlayerOptionRuntime*);int resetFlag;unsigned primaryVmId,secondaryVmId;};
struct Player{int movementDirection,optionMode,optionTransitionFrames,modeVmId;struct{int current;}updateTimer2;void*resource;AnmVmView drawVm;int axisSpeedMode0,axisSpeedMode1,diagonalSpeedMode0,diagonalSpeedMode1,previousHorizontalSpeed,previousVerticalSpeed,movementDeltaX,movementDeltaY,positionX,positionY;AnmFloat3View drawPosition;int optionCount;PlayerPositionPair replayPositionHistory[33];PlayerOptionRuntime options[4];};
struct AnmLoadedView{void InitializeAndExecuteScriptIndex(AnmVmView*,int);void InitializeVm(AnmVmView*,int){assert(false);}};
struct BulletManagerView{AnmLoadedView*bulletAnm;};static void*g_EnemyPrimaryResourceOwner;
struct AnmRenderManagerView{int identity;AnmVmView*FindVm(AnmVmIdView);AnmVmView*AllocateVm(){assert(false);return NULL;}AnmVmIdView AddVmVariant0(AnmVmView*){assert(false);return {};}
void SetVmPosition(AnmVmIdView,const AnmFloat3View*);void SetVmWorldPosition(AnmVmIdView,const AnmFloat3View*);void SetVmPendingInterrupt(AnmVmIdView,short);};
static AnmRenderManagerView first{1},second{2};static AnmRenderManagerView*g_AnmRenderManagerView=&first;
struct Gate{int value60;};static Gate gate;static Gate*g_PlayerDrawGate10=&gate;
static unsigned g_PlayerInputBits;static int g_PlayerShotType,g_PlayerCharacter;static float g_PlayerTimerScale=1;
enum{PLAYER_DIRECTION_NONE=0,PLAYER_DIRECTION_UP=1,PLAYER_DIRECTION_DOWN=2,PLAYER_DIRECTION_LEFT=3,PLAYER_DIRECTION_RIGHT=4,PLAYER_DIRECTION_UP_LEFT=5,PLAYER_DIRECTION_UP_RIGHT=6,PLAYER_DIRECTION_DOWN_LEFT=7,PLAYER_DIRECTION_DOWN_RIGHT=8,PLAYER_MODE_VM_LAYER=9,PLAYER_MODE_VM_SCRIPT=352,PLAYER_VM_RUNTIME_FLAG=0x40000000,PLAYER_MAIN_VM_MOVE_LEFT=1,PLAYER_MAIN_VM_STOP_LEFT=2,PLAYER_MAIN_VM_MOVE_RIGHT=3,PLAYER_MAIN_VM_STOP_RIGHT=4};
static Player p;static AnmLoadedView resource;static AnmVmView primary,secondary,child,modeVm;static AnmVmLayerNodeView childNode;static bool mutate;static std::vector<int> ids,receivers,scripts;
AnmVmView*AnmRenderManagerView::FindVm(AnmVmIdView id){ids.push_back(id.value);receivers.push_back(identity);if(id.value==101){if(mutate){g_AnmRenderManagerView=&second;p.options[0].secondaryVmId=202;}return &primary;}if(id.value==202)return &secondary;if(id.value==77)return &modeVm;return NULL;}
AnmVmView*FindPlayerVm(unsigned id){return g_AnmRenderManagerView->FindVm(AnmVmIdView(id));}
void AnmLoadedView::InitializeAndExecuteScriptIndex(AnmVmView*vm,int script){assert(this==&resource&&vm==&p.drawVm);scripts.push_back(script);}
'''+helpers+'\n'+body+r'''
void reset(){p={};p.resource=&resource;p.axisSpeedMode0=100;p.axisSpeedMode1=50;p.diagonalSpeedMode0=70;p.diagonalSpeedMode1=35;p.positionX=10000;p.positionY=20000;p.updateTimer2.current=4;gate={0};g_PlayerDrawGate10=&gate;g_PlayerInputBits=0;g_PlayerCharacter=g_PlayerShotType=0;g_PlayerTimerScale=1;g_AnmRenderManagerView=&first;primary={};secondary={};child={};modeVm={};mutate=false;ids.clear();receivers.clear();scripts.clear();}
int main(){
 struct Case{unsigned input;int prior,script;};Case cases[]={{0x40,0,1},{0x80,0,3},{0,-1,2},{0,1,4},{0,0,0}};
 for(auto c:cases){reset();g_PlayerInputBits=c.input;p.previousHorizontalSpeed=c.prior;assert(PlayerUpdateMovementAndOptions(&p)==0);assert(scripts==(c.script?std::vector<int>{c.script}:std::vector<int>{}));}
 reset();p.modeVmId=77;modeVm.layerNode.next=&childNode;childNode={&child,NULL,NULL};PlayerUpdateMovementAndOptions(&p);assert(modeVm.pendingInterrupt==1&&child.pendingInterrupt==1&&p.modeVmId==0);assert(scripts.empty());
 reset();gate.value60=1;g_PlayerInputBits=4;p.modeVmId=77;p.drawPosition.z=3;PlayerUpdateMovementAndOptions(&p);assert(modeVm.positionOffset.x==324&&modeVm.positionOffset.y==216&&modeVm.positionOffset.z==3);assert(p.modeVmId==77&&scripts.empty());
 reset();auto&o=p.options[0];o.state=1;o.resetFlag=1;o.primaryVmId=101;o.secondaryVmId=999;o.replayPair2={200,300};p.optionCount=1;mutate=true;childNode={&child,NULL,NULL};primary.layerNode.next=&childNode;
 PlayerUpdateMovementAndOptions(&p);assert(ids==std::vector<int>({0,0,101,202}));assert(receivers==std::vector<int>({1,1,1,1}));assert(g_AnmRenderManagerView==&second);assert(o.secondaryVmId==202);
 for(auto*v:{&primary,&child,&secondary})assert(v->positionOffset.x==326&&v->positionOffset.y==219&&v->positionOffset.z==0);
 assert(scripts.empty());
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='player-movement-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
