#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#include "FrontEnd.hpp"
#include "ReplayManager.hpp"
#include "ReplayChain.hpp"
#include "InputState.hpp"
#include "EnemySoundQueue.hpp"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <cmath>
#include <functional>
#include <map>
#include <string>
#include <vector>
struct Failure { const char *label; };
#define CHECK(test,label) do { if(!(test)) throw Failure{label}; } while(0)
static std::vector<std::string> events;
static std::function<void(const std::string&)> after;
static void event(const std::string &s) { events.push_back(s); if(after) after(s); }
#include "helpers.inc"
static EnemySoundQueueView soundOwner,otherSoundOwner;
#define g_MainSoundOwner reinterpret_cast<unsigned char *>(&soundOwner)
#define g_AlternateSoundOwner reinterpret_cast<unsigned char *>(&otherSoundOwner)
static void HostObserveQueue(EnemySoundQueueView *owner,int sound,int sample) { CHECK(owner==&soundOwner,"queue-owner");CHECK(sample==0,"queue-sample");event("sound:"+std::to_string(sound)); }
#include "sound.inc"
static void CreateVm(FrontEndControllerView *,int slot) { event("create:" + std::to_string(slot)); }
void DeleteVm(FrontEndControllerView *,int slot) { event("delete-vm:" + std::to_string(slot)); }
void FrontEndMarkVmInterrupt(FrontEndControllerView *,int slot,short interrupt) { event("interrupt:" + std::to_string(slot) + ":" + std::to_string(interrupt)); }
static int *auxiliaryAddress;
void EnemyMarkPendingInterrupt(int *id) { CHECK(id==auxiliaryAddress,"close-auxiliary"); event("auxiliary"); }
int FrontEndLoadMusic(int slot,const char *path) { CHECK(slot==0 && std::string(path)=="bgm/th10_02.wav","close-music");event("music-load");return -1; }
int FrontEndPlayMusic(int slot,int index) { CHECK(slot==0 && index==0,"close-music");event("music-play");return -1; }
void FrontEndControllerView::SetScreenStateTarget(int value) {screenState=value;stateTimer.current=0;event("phase:"+std::to_string(value));}
void FrontEndControllerView::SetScreenTarget(int value) {screen=value;screenState=0;stateTimer.current=0;event("screen:"+std::to_string(value));}
FrontEndInputView g_FrontEndInput;
unsigned char g_FrontEndStageRecords[9][0x30];
unsigned char *g_FrontEndSelectedStageRecord;
int g_FrontEndSelectedStage,g_FrontEndSelectedStageMirror;
static unsigned char profile[0x1da00]; unsigned char *g_FrontEndProfileData=profile;
const char *g_FrontEndScoreEntryAlphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-=.,!?@:;[]()_/{}|~^#$%&*   ";
const char *g_ReplayCharacterNames[]={"ReimuA","ReimuB","ReimuC","MarisaA","MarisaB","MarisaC"};
const char *g_ReplayDifficultyNames[]={"Easy","Normal","Hard","Lunatic","Extra"};
const char *g_ReplayCompletionNames[]={"-","1","2","3","4","5","6","7","All"};
ReplayManager *g_ReplayManager;
static ReplayDataHeader data[80];
static std::map<ReplayManager*,int> live;
static int nextIdentity;
ReplayStageState::ReplayStageState() {}
ReplayStageState::~ReplayStageState() {}
ReplayManager::ReplayManager() { std::memset(this,0,sizeof(*this));live[this]=++nextIdentity;replayData=&data[nextIdentity]; }
ReplayManager::~ReplayManager() { int id=live.at(this);live.erase(this);if(g_ReplayManager==this) g_ReplayManager=NULL;event("destroy:"+std::to_string(id)); }
static ReplayManager *makeReplay() { return new ReplayManager; }
static bool failLoad;static int readResult;
int ReplayManager::LoadReplay(const char *path) { CHECK(mode==REPLAY_MANAGER_LOAD_ONLY,"factory-load-mode");event("load:"+std::string(path));return failLoad?-1:readResult; }
int ReplayManager::SaveReplay(const char *path,const char *name) { CHECK(this==g_ReplayManager,"save-receiver");event("save:"+std::string(path)+":"+name);return -1; }
// The maintained null-safe wrapper is included separately by the runner below.
static std::int32_t HostTime(std::int32_t *value) { *value=12345;event("time");return *value; }
static std::tm date;
static std::tm *HostLocalTime(std::int32_t *value) { CHECK(*value==12345,"draw-date-input");event("localtime");return &date; }
#define time HostTime
#define localtime HostLocalTime
AnmVmView::AnmVmView() { std::memset(this,0,sizeof(*this)); }
AnmVmView::~AnmVmView() {}
AsciiManagerView::AsciiManagerView() {}
AsciiManagerView::~AsciiManagerView() {}
size_t AsciiManagerView::GetSize() { return sizeof(*this); }
#include "runtime.inc"
static ReplayRuntimeState runtime;ReplayRuntimeState *g_ReplayRuntime=&runtime;
static int processResult;int ReplayManager::ProcessFrame(){event("process-frame");return processResult;}
static AsciiManagerView asciiA,asciiB;
AsciiManagerView *g_AsciiManagerView=&asciiA;
struct Text {AsciiManagerView *owner;AnmFloat3View position;unsigned int color;int shadow;std::string value;};
static std::vector<Text> texts;
void AsciiManagerView::AddFormatText(AnmFloat3View *position,const char *format,...) {
 char buffer[1024];va_list args;va_start(args,format);vsnprintf(buffer,sizeof(buffer),format,args);va_end(args);
 texts.push_back({this,*position,color,drawShadow,buffer});event("text:"+std::to_string(texts.size()));
}
#include "owner.inc"
#include "replay-delete.inc"
static FrontEndControllerView c;
static int at(const std::string &value) { auto p=std::find(events.begin(),events.end(),value);CHECK(p!=events.end(),"event-present");return int(p-events.begin()); }
static bool has(const std::string &value) {return std::find(events.begin(),events.end(),value)!=events.end();}
static void reset(int state=2) {
 after={};while(!live.empty())delete live.begin()->first;nextIdentity=0;
 std::memset(&c,0,sizeof(c));c.screen=16;c.screenState=state;c.stateTimer.current=10;c.stateTimer.subframe=10;
 c.cursor.count=25;c.cursor.wraps=1;c.scoreEntryKeyboardCursor.count=91;c.scoreEntryKeyboardCursor.wraps=1;
 c.cursor.saveDepth=1;c.cursor.savedCurrent[0]=4;c.cursor.savedCount[0]=7;
 std::strcpy(c.scoreEntryName,"        ");std::memset(profile,0,sizeof(profile));std::strcpy((char*)profile+0x1d878,"        ");
 std::memset(data,0,sizeof(data));g_ReplayManager=makeReplay();data[1].timestamp=12345;
 soundOwner={};otherSoundOwner={};std::fill(soundOwner.activeSoundIds,soundOwner.activeSoundIds+12,-1);std::fill(otherSoundOwner.activeSoundIds,otherSoundOwner.activeSoundIds+12,-1);for(int i=0;i<134;++i)g_EnemySoundCueMetadata[i].cueValue=i+500;
 g_FrontEndInput={};failLoad=false;readResult=0;runtime={};g_ReplayRuntime=&runtime;processResult=1;g_FrontEndSelectedStage=g_FrontEndSelectedStageMirror=-1;g_FrontEndSelectedStageRecord=NULL;
 asciiA.color=0xff123456;asciiA.drawShadow=3;asciiB.color=0xff654321;asciiB.drawShadow=7;g_AsciiManagerView=&asciiA;
 auxiliaryAddress=&c.difficultyAuxVmId.value;date={};date.tm_year=106;date.tm_mon=8;date.tm_mday=17;date.tm_hour=13;date.tm_min=4;
 texts.clear();events.clear();
}
static void update() {
 CHECK(FrontEndControllerView::UpdateReplaySave(&c)==1,"update-result");
 int expected[134]={};for(const auto&s:events)if(s.rfind("sound:",0)==0)++expected[std::stoi(s.substr(6))];
 for(int id=0;id<134;++id)if(expected[id]) {int slot=0;while(slot<12&&soundOwner.activeSoundIds[slot]!=id)++slot;CHECK(slot<12&&soundOwner.sampleCounts[slot]==expected[id]&&soundOwner.cueValues[id]==id+500,"queue-recorded");for(int n=0;n<expected[id];++n)CHECK(soundOwner.samples[slot][n]==0,"queue-recorded");}
}
static void draw() { CHECK(FrontEndControllerView::DrawReplaySave(&c)==1,"draw-result"); }
static bool close(float a,float b) {return std::abs(a-b)<0.001f;}
static void suite() {
 for(int state:{-1,5,100}) {reset(state);update();CHECK(events.empty(),"unknown-phase");draw();CHECK(events.empty()&&asciiA.color==0xff123456&&asciiA.drawShadow==3,"unknown-draw");}
 reset(0);update();CHECK(c.screenState==1&&c.cursor.count==25&&c.cursor.current==0&&c.cursor.wraps==1,"initialize-cursor");
 CHECK(g_FrontEndSelectedStageRecord==g_FrontEndStageRecords[8]&&g_FrontEndSelectedStage==8&&g_FrontEndSelectedStageMirror==8,"initialize-stage");
 CHECK(at("load:th10_01.rpy")<at("load:th10_25.rpy")&&at("load:th10_25.rpy")<at("create:105")&&at("create:105")<at("phase:1"),"initialize-order");
 for(int i=0;i<25;++i)CHECK(c.replayFiles[i]!=NULL,"initialize-loads");
 reset(0);failLoad=true;after=[](const std::string&s){if(s=="phase:1")c.stateTimer.current=7;};update();CHECK(c.screenState==2,"opening-fallthrough");
 reset(1);c.stateTimer.current=6;update();CHECK(c.screenState==1&&events.empty(),"opening-seven");c.stateTimer.current=7;update();CHECK(c.screenState==2,"opening-seven");
 reset();g_FrontEndInput.pressed=0x1001|0xa;update();CHECK(c.screenState==4&&!has("time")&&events==std::vector<std::string>({"phase:4","sound:11"}),"slot-cancel-priority");
 reset();g_FrontEndInput.repeated=0x1001|0xa;update();CHECK(events.empty()&&c.screenState==2,"edge-only");
 reset();g_FrontEndInput.repeated=0x10;update();CHECK(c.cursor.current==24&&c.cursor.previous==0&&has("sound:12"),"slot-wrap");
 reset();g_FrontEndInput.pressed=1;c.cursor.current=4;std::strcpy((char*)profile+0x1d878,"ABC     ");update();
 CHECK(c.screenState==3&&c.selectedReplay==4&&data[1].timestamp==12345&&data[1].completionState==8&&c.scoreEntryNameCursor==3&&c.scoreEntryKeyboardCursor.current==90,"select-name");
 reset();ReplayManager *recorded=g_ReplayManager;auto *changedHeader=&data[3];
 after=[&](const std::string&s){if(s=="time")recorded->replayData=changedHeader;};g_FrontEndInput.pressed=1;update();
 CHECK(data[1].timestamp==12345&&data[1].completionState==0&&changedHeader->completionState==8,"time-live-header");
 reset();recorded=g_ReplayManager;auto *replacement=makeReplay();
 after=[&](const std::string&s){if(s=="time")g_ReplayManager=replacement;};g_FrontEndInput.pressed=1;update();
 CHECK(recorded->replayData->completionState==8&&replacement->replayData->completionState==0,"time-stable-owner");
 for(auto move:std::vector<std::pair<int,int>>{{0x10,78},{0x20,13},{0x40,12},{0x80,1}}){reset(3);g_FrontEndInput.repeated=move.first;update();CHECK(c.scoreEntryKeyboardCursor.current==move.second&&has("sound:12"),"grid-wrap");}
 reset(3);c.scoreEntryKeyboardCursor.current=12;g_FrontEndInput.repeated=0x80;update();CHECK(c.scoreEntryKeyboardCursor.current==0,"grid-right-wrap");
 reset(3);c.scoreEntryNameCursor=7;std::strcpy(c.scoreEntryName,"ABCDEFG ");c.scoreEntryKeyboardCursor.current=25;g_FrontEndInput.pressed=1;update();CHECK(std::string(c.scoreEntryName)=="ABCDEFGZ"&&c.scoreEntryNameCursor==8&&c.scoreEntryKeyboardCursor.current==90,"name-eighth");
 c.scoreEntryKeyboardCursor.current=0;update();CHECK(std::string(c.scoreEntryName)=="ABCDEFGA"&&c.scoreEntryNameCursor==8,"name-overwrite");
 reset(3);c.scoreEntryKeyboardCursor.current=88;g_FrontEndInput.pressed=1;update();CHECK(c.scoreEntryNameCursor==1&&c.scoreEntryName[0]==' ',"name-space");c.scoreEntryKeyboardCursor.current=89;update();CHECK(c.scoreEntryNameCursor==0,"name-backspace");
 reset(3);c.scoreEntryKeyboardCursor.current=89;g_FrontEndInput.pressed=1|8;update();CHECK(c.screenState==3&&events.empty()&&c.scoreEntryNameCursor==0,"empty-backspace-exit");
 reset(3);g_FrontEndInput.pressed=1|8;update();CHECK(c.screenState==3&&c.scoreEntryNameCursor==0&&has("sound:10")&&has("sound:11"),"confirm-then-cancel");
 reset(3);g_FrontEndInput.pressed=8;update();CHECK(c.screenState==2&&!has("sound:11"),"empty-cancel");
 reset(3);c.scoreEntryKeyboardCursor.current=90;c.cursor.current=0;c.replayFiles[0]=makeReplay();std::strcpy(c.scoreEntryName,"TEST    ");g_FrontEndInput.pressed=1;
 after=[](const std::string&s){if(s.rfind("save:",0)==0)c.cursor.current=2;else if(s.rfind("load:",0)==0)c.cursor.current=7;};update();
 CHECK(c.replayFiles[2]!=NULL&&c.cursor.current==7&&c.replayFiles[7]==NULL,"load-sampled-slot");
 CHECK(at("destroy:2")<at("save:th10_01.rpy:TEST    ")&&at("save:th10_01.rpy:TEST    ")<at("load:th10_01.rpy")&&c.screenState==2&&std::string((char*)profile+0x1d878)=="TEST    ","save-ignore-failure");
 reset(3);c.scoreEntryKeyboardCursor.current=90;failLoad=true;g_FrontEndInput.pressed=1;update();CHECK(c.replayFiles[0]==NULL&&c.screenState==2&&has("sound:44")&&has("sound:10"),"reload-failure");
 reset(4);c.stateTimer.current=5;update();CHECK(events.empty(),"close-six");
 c.stateTimer.current=6;for(int i=0;i<50;++i)c.replayFiles[i]=makeReplay();update();
 CHECK(c.screen==2&&c.cursor.current==4&&c.cursor.count==7&&c.cursor.saveDepth==0,"close-state");
 for(auto*p:c.replayFiles)CHECK(p==NULL,"close-clear-fifty");CHECK(live.size()==25&&!has("destroy:27"),"close-delete-twenty-five");
 CHECK(at("delete-vm:105")<at("interrupt:90:8")&&at("interrupt:91:8")<at("delete-vm:94")&&at("auxiliary")<at("screen:2")&&at("destroy:1")<at("music-load")&&at("music-play")<at("destroy:2"),"close-order");
 reset(2);draw();CHECK(texts.size()==25&&texts[0].value=="No.01 -------- --/--/-- --:-- ------- ------- --- ---%"&&texts[0].color==0xffffff00&&texts[24].color==0xff808080&&close(texts[24].position.y,440)&&texts[0].shadow==1&&asciiA.color==0xffffffff&&asciiA.drawShadow==0,"draw-empty-list");
 reset(2);c.replayFiles[0]=makeReplay();auto *header=c.replayFiles[0]->replayData;header->timestamp=12345;std::strcpy(header->replayName,"PLAYER  ");header->character=1;header->shotType=2;header->difficulty=3;header->completionState=7;header->slowRate=12.5f;
 after=[&](const std::string&s){if(s=="localtime"){g_AsciiManagerView=&asciiB;c.replayFiles[0]->replayData=&data[3];}};draw();
 CHECK(texts[0].owner==&asciiB&&texts[0].value=="No.01 PLAYER   06/09/17 13:04 MarisaC Lunatic 7 12.5%"&&texts[0].color==0xff654321,"draw-stable-header-fresh-ascii");CHECK(asciiA.drawShadow==1&&asciiB.drawShadow==0,"draw-current-cleanup");
 reset(3);c.selectedReplay=4;c.stateTimer.current=5;c.stateTimer.subframe=5.5f;data[1].completionState=2;draw();CHECK(texts.size()==1&&close(texts[0].position.y,195)&&texts[0].value.find("Easy All")!=std::string::npos,"preview-completion");
 CHECK(texts[0].color==0xff123456&&asciiA.color==0xffffffff&&asciiA.drawShadow==0,"preview-inherited-color");
 reset(3);c.stateTimer.current=9;c.stateTimer.subframe=9.5f;c.scoreEntryNameCursor=8;c.scoreEntryKeyboardCursor.current=90;
 after=[](const std::string&s){if(s=="text:1")c.stateTimer.current=10;else if(s=="text:2")g_AsciiManagerView=&asciiB;};draw();
 CHECK(texts.size()==94,"draw-live-timer");CHECK(texts[1].position.x==112&&texts[1].position.y==240&&texts[2].position.x==175&&texts[2].value=="_"&&texts[2].color==0xffffff00&&texts[2].owner==&asciiB,"draw-name-caret");
 CHECK(texts[3].position.x==212&&texts[15].position.x==428&&texts[16].position.x==212&&texts[16].position.y==376,"draw-grid-layout");
 CHECK((unsigned char)texts[91].value[0]==0x81&&(unsigned char)texts[92].value[0]==0x7f&&(unsigned char)texts[93].value[0]==0x80&&texts[93].color==0xffffff00&&asciiB.color==0xffffffff&&asciiB.drawShadow==0,"draw-control-glyphs");
 for(int status:{0,1,-1,7,-2147483647}) {
  reset();readResult=status;auto *current=g_ReplayManager;auto *loaded=ReplayManager::Load("th10_01.rpy");
  CHECK(g_ReplayManager==current&&has("load:th10_01.rpy"),"factory-global-owner");
  CHECK((loaded!=NULL)==(status==0),"factory-read-result");
  CHECK((live.size()==2)==(status==0)&&has("destroy:2")==bool(status),"factory-failure-delete");
 }
 for(bool active:{false,true})for(int mode:{-1,0,1,2,7})for(int fps:{0,29,30,49,50,60,255}) {
  reset();g_ReplayRuntime=active?&runtime:NULL;g_ReplayManager->mode=mode;g_ReplayManager->replayFps=fps;
  CHECK(g_ReplayManager->DrawFps()==1,"fps-result");bool visible=active&&mode==1;
  CHECK(texts.size()==(visible?1u:0u),"fps-gate");
  if(visible) {
   auto &t=texts[0];char expected[16];std::snprintf(expected,sizeof(expected),"%3d",fps);
   CHECK(t.value==expected&&t.position.x==383&&t.position.y==450&&t.position.z==0,"fps-format-position");
   unsigned int color=fps<30?0xff5050ffu:fps<50?0xffa0a0ffu:0xffffffffu;
   CHECK(t.color==color&&asciiA.color==0xffffffffu&&asciiA.drawShadow==3,"fps-color");
  } else CHECK(asciiA.color==0xff123456&&asciiA.drawShadow==3,"fps-inactive-color");
 }
 reset();g_ReplayManager->mode=1;g_ReplayManager->replayFps=29;
 after=[](const std::string&s){if(s=="text:1"){g_AsciiManagerView=&asciiB;g_ReplayManager->replayFps=77;}};
 CHECK(g_ReplayManager->DrawFps()==1,"fps-result");
 CHECK(texts.size()==1&&texts[0].value==" 29"&&asciiA.color==0xff5050ffu&&asciiB.color==0xffffffffu,"fps-current-ascii");
 for(int result:{-1,0,1,6}) {reset();processResult=result;ReplayChainCallback callback=ReplayProcessFrameCallback;CHECK(callback(g_ReplayManager)==result&&events==std::vector<std::string>({"process-frame"}),"process-callback-forward");}
 for(bool active:{false,true})for(unsigned flags:{0u,4u,8u})for(int mode:{-1,0,1,2})for(unsigned input:{0u,0x100u,0x200u})for(int counter:{-5,-4,-1,0,1,3,4,5}) {
  reset();g_ReplayRuntime=active?&runtime:NULL;runtime.flags=flags;g_ReplayManager->mode=mode;g_ReplayManager->frameCounter=counter;g_FrontEndInput.current=input;g_FrontEndInput.pressed=0x100;
  ReplayChainCallback callback=ReplayPlaybackFrameControlCallback;bool skip=active&&!(flags&4)&&mode==1&&(input&0x100)&&counter%4!=0;
  CHECK(callback(g_ReplayManager)==(skip?6:1)&&events.empty(),"frame-callback-gates");
 }
 for(bool active:{false,true})for(unsigned flags:{0u,4u,8u})for(int mode:{0,1,2}) {
  reset();g_ReplayRuntime=active?&runtime:NULL;runtime.flags=flags;g_ReplayManager->mode=mode;g_ReplayManager->replayFps=29;
  ReplayChainCallback callback=ReplayDrawFpsCallback;CHECK(callback(g_ReplayManager)==1,"draw-callback-result");
  CHECK(texts.size()==(active&&!(flags&4)&&mode==1?1u:0u),"draw-callback-gate");
 }
 after={};while(!live.empty())delete live.begin()->first;
}
int main() {try {suite();}catch(Failure f) {after={};while(!live.empty())delete live.begin()->first;std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
