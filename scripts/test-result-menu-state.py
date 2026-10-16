#!/usr/bin/env python3
"""Extracted result controller and cursor/ownership seams; no game or file IO."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
def block(text,marker):
 a=text.index(marker);i=text.index('{',a);j=i+1;depth=1
 while depth:
  depth+=(text[j]=='{')-(text[j]=='}');j+=1
 return text[a:j]
class ResultMenuState(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_controller_and_prerequisites(self):
  source=(ROOT/'src/FrontEnd.cpp').read_text();header=(ROOT/'src/ResultMenu.hpp').read_text();front=(ROOT/'src/FrontEnd.hpp').read_text()
  cursor=block(front,'struct FrontEndCursorView\n')+';'
  menu=block(header,'struct ResultMenuView\n')+';'
  bodies='\n'.join(block(source,s) for s in ['int __fastcall FrontEndCursorSetCurrent(','void FrontEndCursorView::Push()','void FrontEndCursorView::Pop()','int FrontEndCursorView::Move(','int FrontEndInputView::IsRepeated(','static void ResultMenuSound(','void ResultMenuPendingSix(','void ResultMenuPendingThree(','AnmVmIdView ResultMenuFindScript(','void ResultMenuSynchronizeScore(','void ResultMenuClose(','void __stdcall ResultMenuPrepareScore(','void __stdcall ResultMenuUpdateResult(','int ResultMenuUpdate('])
  program=r'''
#include <cassert>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#define __stdcall
#define __fastcall
static std::vector<std::string> events;
static float g_AnmGameSpeed=1;
struct AnmVmTimerView{int previous,current;float subframe,*scale;unsigned flags;void SetCurrent(int v){if(!(flags&1)){scale=&g_AnmGameSpeed;flags|=1;}previous=v-1;current=v;subframe=float(v);events.push_back("timer");}};
struct AnmVmIdView{int value;AnmVmIdView(int v=0):value(v){}void SetInterrupt(short v){events.push_back("interrupt"+std::to_string(v));}void ClearFlag2(){events.push_back("clear");}void SetFlag2(){events.push_back("set");}};
struct AnmVmLayerNodeView{void*owner;AnmVmLayerNodeView*next,*previous;};
struct AnmVmView{int id;short scriptIndex,pendingInterrupt;AnmVmLayerNodeView layerNode;};
static std::map<int,AnmVmView*> vms;
struct AnmRenderManagerView{AnmVmView*FindVm(int id){return vms.count(id)?vms[id]:NULL;}};
static AnmRenderManagerView manager;static AnmRenderManagerView*g_AnmRenderManagerView=&manager;
void EnemyMarkPendingInterrupt(int*id){events.push_back("pending1:"+std::to_string(*id));}
struct ReplayDataHeader{time_t timestamp;int completionState;};
struct ReplayManager{int identity;ReplayDataHeader*replayData;ReplayManager(int i=0):identity(i),replayData(NULL){}~ReplayManager(){events.push_back("delete"+std::to_string(identity));}static ReplayManager*Load(const char*);static void Destroy(ReplayManager*p){if(p)delete p;}int SaveReplay(const char*,const char*);};
static ReplayManager currentReplay;static ReplayManager*g_ReplayManager=&currentReplay;
struct AnmLoadedView;
struct FrontEndCursorView;int FrontEndCursorSetCurrent(int,FrontEndCursorView*);
'''+cursor+'\n'+menu+r'''
struct FrontEndInputView{unsigned current;unsigned short repeated,pressed;int IsRepeated(unsigned short);};static FrontEndInputView g_FrontEndInput;
struct GuiView{int value9E78;};static GuiView gui;static GuiView*g_GuiView=&gui;
struct{int unknown00,score;}g_GameScoreState;
struct EnemySoundQueueView{void QueueSoundSample(int id,int sample){assert(sample==0);events.push_back("sound"+std::to_string(id));}};
static unsigned char g_MainSoundOwner[0x52d0];
static unsigned g_GameRuntimeFlags,g_GameConfigurationFlags;static int g_GameTransitionState;
struct ResultGameChainView{unsigned unknown00,flags;};struct ResultGameManagerView{ResultGameChainView*updateChain;AnmVmTimerView timer;};static ResultGameChainView chain;static ResultGameManagerView game;static ResultGameManagerView*g_ResultGameManager=&game;
void ResultMenuOpenPause(ResultMenuView*){events.push_back("openpause");}void ResultMenuUpdatePause(ResultMenuView*){events.push_back("updatepause");}
struct MainSupervisorView{void SetNextGameMode(int v){events.push_back("mode"+std::to_string(v));}};static MainSupervisorView g_MainSupervisorView;
static unsigned char profile[0x20000];static unsigned char*g_FrontEndProfileData=profile;
static int g_FrontEndSelectedStage,g_FrontEndSelectedStageMirror,g_ReplayCharacter,g_ReplayShotType,g_ReplayDifficulty;
static unsigned char g_FrontEndScoreEntryFormatTable[4],g_FrontEndDefaultScoreFormatTable[4];static void*g_FrontEndScoreFormatTable;
static const char*g_FrontEndScoreEntryAlphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-=.,!?@:;[]()_/{}|~^#$%&*   ";
struct FrontEndScoreTableView{unsigned char unknown000[16];};
FrontEndScoreTableView*GetFrontEndScoreTable(int shot){return reinterpret_cast<FrontEndScoreTableView*>(g_FrontEndProfileData+shot*0x437c+8);}
static int insertRow;static bool restoreStage,mutateSaveSlot,mutateLoadSlot,allSlots,mutateTime;static int saveResult;static ResultMenuView m;static ReplayDataHeader data,data2;
int FrontEndInsertScore(FrontEndScoreTableView*p){assert(p==GetFrontEndScoreTable(g_ReplayCharacter*3+g_ReplayShotType));events.push_back("insert:"+std::to_string(g_FrontEndSelectedStage));if(restoreStage)g_FrontEndSelectedStage=7;return insertRow;}
time_t ResultTime(time_t*p){events.push_back("time");*p=12345;if(mutateTime)currentReplay.replayData=&data2;return *p;}
#define time ResultTime
ReplayManager*ReplayManager::Load(const char*p){events.push_back(std::string("load:")+p);int n=0;assert(sscanf(p,"th10_%d.rpy",&n)==1&&n>=1&&n<=25);if(mutateLoadSlot)m.cursor.current=7;return allSlots||n%2?new ReplayManager(n):NULL;}
int ReplayManager::SaveReplay(const char*p,const char*n){assert(this==g_ReplayManager);events.push_back(std::string("save:")+p+":"+n);if(mutateSaveSlot)m.cursor.current=2;return saveResult;}
'''+bodies+r'''
static AnmVmView root,child,other;
void reset(){for(auto*&p:m.replayFiles){delete p;p=NULL;}m={};m.timer.scale=&g_AnmGameSpeed;m.timer.flags=1;m.timer.current=10;m.cursor.count=3;m.cursor.wraps=1;m.keyboard.count=91;m.keyboard.wraps=1;m.mainVm.value=1;m.secondaryVm.value=9;strcpy(m.name,"        ");memset(profile,0,sizeof(profile));strcpy((char*)profile+0x1d878,"        ");g_GameRuntimeFlags=g_GameConfigurationFlags=0;g_GameTransitionState=0;g_FrontEndInput={};g_FrontEndSelectedStage=g_FrontEndSelectedStageMirror=1;g_ReplayCharacter=g_ReplayShotType=g_ReplayDifficulty=0;g_FrontEndScoreFormatTable=g_FrontEndDefaultScoreFormatTable;insertRow=0;restoreStage=mutateSaveSlot=mutateLoadSlot=allSlots=mutateTime=false;saveResult=0;g_AnmGameSpeed=1;g_GameScoreState={10,20};gui={};root={1,124,0,{}};child={2,125,0,{}};other={3,126,0,{}};root.layerNode={&root,&child.layerNode,NULL};child.layerNode={&child,&other.layerNode,&root.layerNode};other.layerNode={&other,NULL,&child.layerNode};vms={{1,&root},{2,&child},{3,&other}};chain={0,2};game={&chain,{0,30,0,&g_AnmGameSpeed,1}};currentReplay.replayData=&data;data={};data2={};events.clear();}
bool has(const std::string&s){for(auto&e:events)if(e==s)return true;return false;}
int at(const std::string&s){for(unsigned i=0;i<events.size();++i)if(events[i]==s)return i;assert(false);return -1;}
int main(){
 reset();m.state=6;m.timer.current=9;g_FrontEndInput.pressed=1;ResultMenuUpdateResult(&m);assert(m.state==6&&events.empty());m.timer.current=10;ResultMenuUpdateResult(&m);assert(m.state==7&&events==std::vector<std::string>({"sound10","interrupt2","timer"}));
 reset();m.state=7;m.completed=1;g_FrontEndSelectedStage=7;ResultMenuUpdateResult(&m);assert(m.state==12&&has("insert:8")&&g_FrontEndSelectedStage==8&&g_FrontEndSelectedStageMirror==8&&g_FrontEndScoreFormatTable==g_FrontEndScoreEntryFormatTable);assert(gui.value9E78==20&&g_GameScoreState.unknown00==20);
 reset();m.completed=1;g_FrontEndSelectedStage=7;restoreStage=true;ResultMenuPrepareScore(&m);assert(has("insert:8")&&g_FrontEndSelectedStage==7&&g_FrontEndSelectedStageMirror==7&&g_FrontEndScoreFormatTable==g_FrontEndDefaultScoreFormatTable);
 reset();m.state=7;insertRow=-1;ResultMenuUpdateResult(&m);assert(m.state==8&&m.cursor.current==2&&m.scoreUnavailable==1&&root.pendingInterrupt==3&&child.pendingInterrupt==3);
 reset();m.state=7;g_GameRuntimeFlags=0x10;g_ReplayCharacter=1;g_ReplayShotType=2;g_ReplayDifficulty=3;g_FrontEndSelectedStage=4;int*record=(int*)(profile+5*0x437c+0x4dc+(4+3*6)*8);*record=15;ResultMenuUpdateResult(&m);assert(*record==20&&m.state==8&&!has("insert:4"));
 // Confirm enters the 25-slot list, then simultaneous cancel moves its cursor.
 reset();m.state=8;m.cursor.current=1;g_FrontEndInput.pressed=1|8;ResultMenuUpdateResult(&m);assert(m.state==10&&m.cursor.current==2&&m.cursor.saveDepth==1&&child.pendingInterrupt==6&&other.pendingInterrupt==0);assert(at("load:th10_01.rpy")<at("load:th10_25.rpy")&&at("load:th10_25.rpy")<at("sound11"));for(int i=0;i<25;++i)assert((m.replayFiles[i]!=NULL)==(i%2==0));
 m.timer.current=10;g_FrontEndInput.pressed=8;events.clear();ResultMenuUpdateResult(&m);assert(m.state==8&&m.cursor.current==1&&m.cursor.saveDepth==0);for(int i=0;i<25;++i)assert(m.replayFiles[i]==NULL);assert(at("delete1")<at("delete25")&&at("delete25")<at("sound11"));
 reset();m.state=10;m.completed=1;g_FrontEndInput.pressed=1|8;strcpy((char*)profile+0x1d878,"ABC     ");ResultMenuUpdateResult(&m);assert(m.state==11&&data.timestamp==12345&&data.completionState==8&&m.nameIndex==3&&m.keyboard.current==90&&!has("sound11"));
 reset();m.state=10;m.completed=1;g_GameRuntimeFlags=0x10;g_FrontEndSelectedStage=5;g_FrontEndInput.pressed=1;ResultMenuUpdateResult(&m);assert(data.completionState==5);
 reset();m.state=10;m.completed=1;mutateTime=true;g_FrontEndInput.pressed=1;ResultMenuUpdateResult(&m);assert(data.timestamp==12345&&data.completionState==0&&data2.completionState==8);
 reset();m.state=8;m.cursor.current=1;allSlots=true;g_FrontEndInput.pressed=1;ResultMenuUpdateResult(&m);for(int i=0;i<25;++i)assert(m.replayFiles[i]&&m.replayFiles[i]->identity==i+1);m.timer.current=10;g_FrontEndInput.pressed=8;events.clear();ResultMenuUpdateResult(&m);for(int i=1;i<=25;++i)assert(has("delete"+std::to_string(i)));
 reset();m.state=11;m.keyboard.current=90;m.cursor.current=1;m.replayFiles[1]=new ReplayManager(99);g_FrontEndInput.pressed=1;saveResult=-1;ResultMenuUpdateResult(&m);assert(m.state==10&&m.replayFiles[1]==NULL&&at("delete99")<at("save:th10_02.rpy:        ")&&at("save:th10_02.rpy:        ")<at("load:th10_02.rpy"));
 reset();m.state=11;m.nameIndex=7;strcpy(m.name,"ABCDEFG ");m.keyboard.current=25;g_FrontEndInput.pressed=1;ResultMenuUpdateResult(&m);assert(strcmp(m.name,"ABCDEFGZ")==0&&m.nameIndex==8&&m.keyboard.current==90);
 // Each confirm character is subsequently removed by simultaneous cancel.
 for(int state:{11,12}){reset();m.state=state;m.keyboard.current=0;g_FrontEndInput.pressed=1|8;ResultMenuUpdateResult(&m);assert(m.nameIndex==0&&strcmp(m.name,"        ")==0&&has("sound10")&&has("sound11")&&m.state==state);}
 reset();m.state=11;m.keyboard.current=89;g_FrontEndInput.pressed=1|8;ResultMenuUpdateResult(&m);assert(events.empty()&&m.state==11); // Empty backspace exits before cancel.
 reset();m.state=11;m.keyboard.current=90;m.cursor.current=0;m.replayFiles[0]=new ReplayManager(99);strcpy(m.name,"TEST    ");g_FrontEndInput.pressed=1|8;saveResult=-1;mutateSaveSlot=mutateLoadSlot=true;ResultMenuUpdateResult(&m);assert(at("delete99")<at("save:th10_01.rpy:TEST    ")&&at("save:th10_01.rpy:TEST    ")<at("load:th10_01.rpy"));assert(m.replayFiles[2]&&m.replayFiles[2]->identity==1&&m.cursor.current==7&&m.state==10&&!has("sound11"));assert(strcmp((char*)profile+0x1d878,"TEST    ")==0);m.replayFiles[0]=NULL; // Target leaves old slot dangling when the opaque save changes selection.
 reset();m.state=12;m.keyboard.current=90;m.cursor.current=2;m.nameIndex=3;strcpy(m.name,"ABC     ");g_FrontEndInput.pressed=1|8;ResultMenuUpdateResult(&m);assert(m.state==8&&m.cursor.current==0&&m.nameIndex==2&&strcmp(m.name,"AB      ")==0);assert(strcmp((char*)profile+0x1e +2*0x18,"ABC     ")==0&&strcmp((char*)profile+0x1d878,"ABC     ")==0);
 reset();m.state=12;m.scoreUnavailable=1;g_FrontEndInput.pressed=1;ResultMenuUpdateResult(&m);assert(m.state==8&&!has("sound10"));
 // Target-sized seven-row grid and all three final-row special entries.
 assert(strlen(g_FrontEndScoreEntryAlphabet)==91);
 for(int state:{11,12}){
  reset();m.state=state;m.keyboard.current=0;g_FrontEndInput.repeated=0x10;ResultMenuUpdateResult(&m);assert(m.keyboard.current==78);
  g_FrontEndInput.repeated=0x20;ResultMenuUpdateResult(&m);assert(m.keyboard.current==0);
  m.keyboard.current=78;g_FrontEndInput.repeated=0x40;ResultMenuUpdateResult(&m);assert(m.keyboard.current==90);
  g_FrontEndInput.repeated=0x80;ResultMenuUpdateResult(&m);assert(m.keyboard.current==78);
  g_FrontEndInput.repeated=0;m.keyboard.current=88;g_FrontEndInput.pressed=1;ResultMenuUpdateResult(&m);assert(m.nameIndex==1&&m.name[0]==' ');
  m.keyboard.current=89;ResultMenuUpdateResult(&m);assert(m.nameIndex==0);
 }
 for(int state:{7,10,11,12}){reset();m.state=state;m.timer.current=9;g_FrontEndInput.pressed=1|8;g_FrontEndInput.repeated=0xf0;ResultMenuUpdateResult(&m);assert(m.state==state&&events.empty()&&m.nameIndex==0&&gui.value9E78==0);}
 reset();m.state=12;m.scoreUnavailable=1;m.cursor.current=-1;m.cursor.wraps=0;g_FrontEndInput.pressed=8;ResultMenuUpdateResult(&m);assert(m.state==8&&m.cursor.current==0&&m.cursor.count==3&&m.cursor.wraps==0&&m.timer.current==0&&events==std::vector<std::string>({"timer","sound10"}));
 reset();m.state=13;m.timer.current=12;m.completed=0;m.savedSpeed=.75f;ResultMenuUpdateResult(&m);assert(m.state==21&&g_GameTransitionState==13&&g_AnmGameSpeed==.75f);
 reset();m.state=11;m.keyboard.current=0;g_FrontEndInput.repeated=0x40;ResultMenuUpdateResult(&m);assert(m.keyboard.current==12);m.keyboard.current=12;g_FrontEndInput.repeated=0x80;ResultMenuUpdateResult(&m);assert(m.keyboard.current==0);
 reset();AnmVmIdView missing(77);ResultMenuPendingThree(&missing);ResultMenuPendingSix(&missing);assert(missing.value==77);ResultMenuPendingThree(&m.mainVm);assert(root.pendingInterrupt==3&&child.pendingInterrupt==3&&other.pendingInterrupt==3);root.pendingInterrupt=child.pendingInterrupt=other.pendingInterrupt=0;AnmVmIdView childId(2);ResultMenuPendingSix(&childId);assert(root.pendingInterrupt==0&&child.pendingInterrupt==6&&other.pendingInterrupt==0);assert(ResultMenuFindScript(&m.mainVm,126).value==3&&ResultMenuFindScript(&m.mainVm,999).value==0);
 reset();m.state=13;m.timer.current=11;m.savedSpeed=.5f;ResultMenuUpdateResult(&m);assert(m.state==13&&events.empty());m.timer.current=12;m.completed=1;ResultMenuUpdateResult(&m);assert(m.state==21&&g_GameTransitionState==10&&g_AnmGameSpeed==.5f&&events==std::vector<std::string>({"timer","pending1:9","pending1:1"}));
 for(int choice:{1,2}){reset();m.state=13;m.timer.current=12;m.cursor.current=choice;ResultMenuUpdateResult(&m);assert(m.state==21&&has("mode4"));}
 reset();m.state=0;g_FrontEndInput.pressed=8;ResultMenuUpdate(&m);assert(has("openpause")&&m.timer.previous==10&&m.timer.current==11);reset();m.state=0;g_FrontEndInput.pressed=8;g_GameRuntimeFlags=0x20;ResultMenuUpdate(&m);assert(!has("openpause"));
 reset();m.state=4;ResultMenuUpdate(&m);assert(has("updatepause"));reset();m.state=9;m.timer.current=2;m.timer.subframe=2.5f;g_AnmGameSpeed=.25f;assert(ResultMenuUpdate(&m)==1&&m.timer.previous==2&&m.timer.current==2&&m.timer.subframe==2.75f);
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='result-menu-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
