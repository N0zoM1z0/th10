// Host fixture, deliberately not a target-layout translation unit.
#include <algorithm>
#include <array>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#define __stdcall
#define __fastcall
#define WINAPI
#define TH10_ANM_NOINLINE
using std::string;
[[noreturn]] void fail(const char* id) { std::cerr << "CONTRACT_FAILURE: " << id << '\n'; std::exit(86); }
#define CHECK(test,id) do { if (!(test)) fail(id); } while (0)
std::vector<string> events;
std::function<void(const string&)> callback;
void event(string text) { events.push_back(text); if(callback) callback(text); }
void expect(std::initializer_list<string> expected, const char* id) { CHECK(events==std::vector<string>(expected),id); }
float g_AnmGameSpeed=0.5f;
struct FrontEndCursorView;
int FrontEndCursorSetCurrent(int,FrontEndCursorView*);
struct AnmLoadedView;
struct AnmVmView;
class ReplayManager {};
using LPDIRECTSOUNDBUFFER=void*;
using HWND=void*;
using HANDLE=void*;
using LPVOID=void*;
using DWORD=std::uint32_t;
using ULONG=std::uint32_t;
using BYTE=unsigned char;
struct CSoundManager;
struct CSound;
struct ThBgmFormat;
// @ACTUAL_STRUCTURES@

// These are explicit dependency seams: ANM allocation, initialization and
// publication are not claimed to execute the real allocator/script engine.
struct AnmSpriteView { float height=47.9f,width=31.2f,sourceY=-5.8f,sourceX=3.7f; };
struct AnmLoadedView { int anmFileIndex=71; string tag; void InitializeVm(AnmVmView*,int); };
struct AnmVmView { int renderLayer=0; unsigned flags35C=0x21; AnmSpriteView* loadedSprite=nullptr; AnmLoadedView* anmFile=nullptr; };
struct AnmRenderManagerView {
 string tag; AnmVmView* vm=nullptr; AnmVmView* lastAdded=nullptr; int nextId=0;
 int captureSurfaceIndex=-1,captureDestinationX=901,captureSourceX=902,captureDestinationY=903,
 captureDestinationWidth=904,captureSourceHeight=905,captureSourceY=906,captureDestinationHeight=907,
 captureSourceWidth=908,captureRequestState=909;
 AnmVmView* FindVm(int id) { auto* result=vm;event(tag+":find:"+std::to_string(id));return result; }
 AnmVmView* AllocateVm() { auto* result=vm;event(tag+":allocate");return result; }
 AnmVmIdView AddVmVariant1(AnmVmView* value) { CHECK(value!=nullptr,"valid-added-vm");lastAdded=value;event(tag+":add");return AnmVmIdView(nextId); }
 void MarkVmForDeletion(int id) { event(tag+":delete:"+std::to_string(id)); }
 int RequestCaptureRectangle(int,int,int,int,int);
};
void AnmLoadedView::InitializeVm(AnmVmView* vm,int script) {
 CHECK(vm->renderLayer==15,"initialized-render-layer");
 CHECK(vm->flags35C==0x40000021,"initialized-flags-preserve");
 vm->anmFile=this;event(tag+":initialize:"+std::to_string(script));
}
struct Ascii { AnmLoadedView* captureAnm=nullptr; } ascii;
struct GuiView { AnmLoadedView* frontAnm=nullptr;unsigned guiFlags=0;int endingCounter=0;AnmVmIdView specialVmId9E14; } gui;
// Static storage zero-initialization precedes the actual timer constructor.
// Never default-initialize a timer on uninitialized automatic storage here.
ResultGameManagerView game,otherGame;
ResultMenuView menu;
ResultGameManagerView* g_ResultGameManager=&game;
Ascii* g_AsciiManagerView=&ascii;
GuiView* g_GuiView=&gui;
AnmRenderManagerView* g_AnmRenderManagerView=nullptr;
unsigned g_GameConfigurationFlags=0,g_GameRuntimeFlags=0,g_FrontEndSupervisorFlags=0;
int g_GameTransitionState=77;
unsigned char profile[0x1da00],profileOther[0x1da00];
unsigned char* g_FrontEndProfileData=profile;
SoundPlayerView sound;
// Observable forwarding seam, not the production extern-storage wrapper.
// The actual SoundPlayerView::QueueCommand is executed for every call.
void FrontEndQueueSoundCommand(const char* path,int opcode,int argument) {
 event("queue:"+string(path)+":"+std::to_string(opcode)+":"+std::to_string(argument));
 sound.QueueCommand(path,opcode,argument);
}

// The canonical global declaration remains pointer-to-incomplete chain view.
// We round-trip a pointer to a live ResultMenuView; no chain-view dereference.
struct GameChainOwnerView;
GameChainOwnerView* g_GameOwner477830=nullptr;
int g_GuiDifficulty=4,g_GuiShotType=0,g_GuiCharacter=0,g_GuiStage=1;
unsigned char* g_GuiPersistentData=profile;
unsigned g_GuiGameFlags=0;
struct ModeOwner { int state=0; } modeOwner;
ModeOwner* g_GuiModeOwner=&modeOwner;
struct Supervisor { void SetNextGameMode(int mode) { event("mode:"+std::to_string(mode)); } } g_MainSupervisorView;
void AddStageClearScore() { event("clear-score"); }
void IncrementStageClearCount() { event("clear-count"); }
void* StageCreateTransitionEffect(int a,int b,int c,unsigned d,unsigned e,unsigned f) {
 CHECK(a==0x31 && b==5 && c==120 && d==0 && e==0 && f==0,"transition-effect-arguments");event("transition-effect");return nullptr;
}
struct Score { void Add(int value) { event("score:"+std::to_string(value)); } } g_GuiScore;
int g_GuiCounter4C4C=0,g_GuiPowerLevel=0;
float g_GuiPower=0;
int CreateGuiVm(AnmLoadedView*,int script) { event("create-gui:"+std::to_string(script));return 555; }
unsigned char stageState[1];unsigned char* g_GameStageState=stageState;
namespace th10 { namespace leaf { void Fn004175E0(unsigned char*) { event("advance-stage"); } } }
// @ACTUAL_BODIES@

AnmSpriteView sprite,spriteB,spriteC;
AnmLoadedView capture,front,other;
AnmVmView va,vb,vc;
AnmRenderManagerView a,b,c;
float privateScale=.125f;
std::array<unsigned char,sizeof(ResultMenuView)> menuBefore;
void checkMenuBytes(bool normal) {
 std::array<unsigned char,sizeof(ResultMenuView)> current,allowed{};
 std::memcpy(current.data(),&menu,sizeof(menu));
 if(normal) {
#define ALLOW_MEMBER(member) std::fill(allowed.begin()+offsetof(ResultMenuView,member),allowed.begin()+offsetof(ResultMenuView,member)+sizeof(menu.member),1)
  ALLOW_MEMBER(state);ALLOW_MEMBER(timer);ALLOW_MEMBER(mainVm);ALLOW_MEMBER(secondaryVm);
  ALLOW_MEMBER(completed);ALLOW_MEMBER(savedSpeed);ALLOW_MEMBER(frontAnm);
#undef ALLOW_MEMBER
 }
 for(std::size_t i=0;i<current.size();++i)
  CHECK(allowed[i] || current[i]==menuBefore[i],"unlisted-menu-bytes-unchanged");
}
void reset(int mode=0,bool active=false) {
 callback={};events.clear();g_ResultGameManager=&game;game.flags58=0x85;game.initializationMode=mode;
 otherGame.flags58=0x44;otherGame.initializationMode=1;
 capture={71,"capture"};front={72,"front"};other={73,"other"};
 sprite={47.9f,31.2f,-5.8f,3.7f};spriteB={61.9f,45.2f,-9.8f,8.7f};spriteC={83.9f,69.2f,-13.8f,11.7f};
 va={};vb={};vc={};va.loadedSprite=&sprite;vb.loadedSprite=&spriteB;vc.loadedSprite=&spriteC;
 va.anmFile=&capture;vb.anmFile=&front;vc.anmFile=&other;
 a={};b={};c={};a.tag="a";b.tag="b";c.tag="c";a.vm=&va;b.vm=&vb;c.vm=&vc;
 a.nextId=101;b.nextId=202;c.nextId=303;
 g_AnmRenderManagerView=&a;ascii.captureAnm=&capture;gui.frontAnm=&front;gui.guiFlags=0;gui.endingCounter=97;
 g_AnmGameSpeed=.5f;g_GameTransitionState=77;g_GameConfigurationFlags=0;g_GameRuntimeFlags=0x1000;g_FrontEndSupervisorFlags=0;
 // Reset existing live objects fieldwise; no temporary timer construction.
 menu.state=9;menu.unknown000=0x1234;menu.timer.previous=34;menu.timer.current=35;menu.timer.subframe=35.5f;
 menu.timer.scale=&privateScale;menu.timer.flags=active?0xa5u:0xa4u;
 menu.mainVm.value=11;menu.secondaryVm.value=22;menu.auxiliaryVm.value=33;
 menu.completed=7;menu.scoreUnavailable=8;menu.nameIndex=4;menu.savedSpeed=.25f;menu.frontAnm=&other;
 menu.cursor.current=2;menu.keyboard.current=3;std::strcpy(menu.name,"TEST");
 std::memset(profile,0x5a,sizeof(profile));std::memset(profileOther,0x5a,sizeof(profileOther));g_FrontEndProfileData=profile;
 std::memset(sound.commandQueue,0,sizeof(sound.commandQueue));
 for(auto& command:sound.commandQueue) { command.argument=901;command.step=902;std::strcpy(command.path,"untouched"); }
 g_GameOwner477830=reinterpret_cast<GameChainOwnerView*>(&menu);
 g_GuiDifficulty=4;g_GuiShotType=0;g_GuiCharacter=0;g_GuiStage=1;g_GuiPersistentData=profile;g_GuiGameFlags=0;
 modeOwner.state=0;g_GuiCounter4C4C=0;g_GuiPowerLevel=0;g_GuiPower=0;
 std::memcpy(menuBefore.data(),&menu,sizeof(menu));
}
void queueAt(int index,int opcode,int argument,const char* path,const char* id) {
 const auto& command=sound.commandQueue[index];
 CHECK(command.opcode==opcode && command.argument==argument && command.step==0 && std::strcmp(command.path,path)==0,id);
}
void checkUntouchedMenu() {
 checkMenuBytes(false);
 CHECK(menu.state==9 && menu.unknown000==0x1234,"early-menu-state");
 CHECK(menu.timer.previous==34 && menu.timer.current==35 && menu.timer.subframe==35.5f && menu.timer.scale==&privateScale && menu.timer.flags==0xa4u,"early-timer-unchanged");
 CHECK(menu.mainVm.value==11 && menu.secondaryVm.value==22 && menu.auxiliaryVm.value==33,"early-vm-ids");
 CHECK(menu.completed==7 && menu.scoreUnavailable==8 && menu.nameIndex==4 && menu.savedSpeed==.25f && menu.frontAnm==&other,"early-other-menu-fields");
}
void checkCompleted(bool active) {
 checkMenuBytes(true);
 CHECK(menu.state==6,"result-state-six");
 CHECK(menu.timer.previous==-1 && menu.timer.current==0 && menu.timer.subframe==0,"result-timer-zero");
 CHECK(menu.timer.flags==0xa5u && menu.timer.scale==(active?&privateScale:&g_AnmGameSpeed),"timer-initialization-preserves-active-scale");
 CHECK(game.flags58==0x95 && otherGame.flags58==0x44,"game-pause-flag-preserves-other-bits");
 CHECK(menu.completed==1,"result-completed-one");
 CHECK(menu.auxiliaryVm.value==33 && menu.scoreUnavailable==8 && menu.nameIndex==4 && menu.cursor.current==2 && menu.keyboard.current==3 && std::strcmp(menu.name,"TEST")==0,"unrelated-menu-fields-preserved");
 CHECK(g_AnmGameSpeed==1.0f,"game-speed-reset-one");
 CHECK(g_GameTransitionState==77,"normal-transition-state-unchanged");
}
void testEarly() {
 for(unsigned flags:{0u,0x1000u,0x800u,0x1800u,0xffffffffu}) {
  reset(1);g_GameConfigurationFlags=flags;g_GameRuntimeFlags=~flags;
  GameCompleteStageTransition(&menu);
  CHECK(g_GameTransitionState==((flags&0x1000u)?2:4),"early-config-flag-selects-mode");
  CHECK(events.empty(),"early-no-dependency-calls");checkUntouchedMenu();
  CHECK(game.flags58==0x85 && g_AnmGameSpeed==.5f,"early-game-state-unchanged");
  CHECK(std::all_of(std::begin(profile),std::end(profile),[](unsigned char x){return x==0x5a;}),"early-profile-unchanged");
  CHECK(sound.commandQueue[0].opcode==0,"early-no-queued-command");
 }
}
void testNormal() {
 for(int mode:{0,2,-1}) for(bool active:{false,true}) for(unsigned supervisor:{0u,0x10u,0x20u}) {
  reset(mode,active);g_FrontEndSupervisorFlags=supervisor;
  callback=[&](const string& s) {
   if(s=="a:allocate") {
    CHECK(menu.state==6 && menu.timer.current==0 && game.flags58==0x95,"state-timer-flags-before-allocation");
    ascii.captureAnm=&other;g_ResultGameManager=&otherGame;
   }
   if(s=="capture:initialize:0") g_AnmRenderManagerView=&b;
   if(s=="b:add") g_AnmRenderManagerView=&c;
   if(s=="c:find:202") CHECK(menu.secondaryVm.value==202,"secondary-id-before-capture-lookup");
   if(s=="b:allocate") { CHECK(menu.frontAnm==&front,"front-resource-published-before-allocation");gui.frontAnm=&other; }
   if(s=="queue:bgm/th10_17.wav:1:0") {
    CHECK(menu.mainVm.value==303,"main-id-before-music-load");CHECK(menu.completed==7,"completion-after-music");
    g_AnmGameSpeed=.75f;
   }
   if(s=="queue:dummy:2:0") {
    CHECK(menu.completed==7 && profile[0x1d8a3]==0x5a,"publication-after-play-queue");
    g_AnmGameSpeed=.875f;g_FrontEndProfileData=profileOther;
   }
  };
  GameCompleteStageTransition(&menu);
  std::vector<string> wanted={"a:allocate","capture:initialize:0","b:add","c:find:202","b:allocate","front:initialize:129","c:add","queue:bgm/th10_17.wav:1:0"};
  if(supervisor&0x10) wanted.push_back("queue:dummy:4:0");
  wanted.push_back("queue:dummy:2:0");
  CHECK(events==wanted,"normal-dependency-order-and-reloads");checkCompleted(active);
  CHECK(menu.secondaryVm.value==202 && menu.mainVm.value==303,"published-vm-ids");
  CHECK(menu.frontAnm==&front && va.anmFile==&capture && vb.anmFile==&front,"resource-caching");
  CHECK(b.lastAdded==&va && c.lastAdded==&vb && a.lastAdded==nullptr,"manager-publication-reload");
  CHECK(menu.savedSpeed==.875f,"save-latest-speed-after-queue");
  CHECK(b.captureSurfaceIndex==73 && b.captureDestinationX==32 && b.captureDestinationY==16 && b.captureDestinationWidth==384 && b.captureDestinationHeight==448,"capture-receiver-and-destination");
  CHECK(b.captureSourceX==11 && b.captureSourceY==-13 && b.captureSourceWidth==69 && b.captureSourceHeight==83 && b.captureRequestState==0,"capture-live-global-lookup-and-truncation");
  CHECK(a.captureSurfaceIndex==-1 && c.captureSurfaceIndex==-1,"only-cached-capture-receiver-changed");
  CHECK(profile[0x1d8a3]==0x5a && profileOther[0x1d8a3]==1 && profileOther[0x1d8a2]==0x5a && profileOther[0x1d8a4]==0x5a,"profile-reload-and-exact-index");
  queueAt(0,1,0,"bgm/th10_17.wav","load-command");
  if(supervisor&0x10) {queueAt(1,4,0,"dummy","optional-release-command");queueAt(2,2,0,"dummy","play-command");}
  else queueAt(1,2,0,"dummy","play-command");
 }
}
void testSupervisorReload() {
 for(unsigned initial:{0u,0x10u}) {
  reset();g_FrontEndSupervisorFlags=initial;
  callback=[&](const string& text) {
   if(text=="queue:bgm/th10_17.wav:1:0") g_FrontEndSupervisorFlags=initial^0x10u;
  };
  GameCompleteStageTransition(&menu);checkCompleted(false);
  queueAt(0,1,0,"bgm/th10_17.wav","reload-flags-load");
  if(initial==0) {queueAt(1,4,0,"dummy","supervisor-reload-enables-release");queueAt(2,2,0,"dummy","reload-flags-play");}
  else {queueAt(1,2,0,"dummy","supervisor-reload-disables-release");CHECK(sound.commandQueue[2].opcode==0,"supervisor-no-extra-command");}
 }
}
void testCaptureBusy() {
 reset();a.captureSurfaceIndex=0;g_AnmRenderManagerView=&b;
 CHECK(a.RequestCaptureRectangle(88,1,2,3,4)==-1,"busy-capture-return");
 expect({"b:find:88"},"busy-capture-still-lookups");
 CHECK(a.captureSurfaceIndex==0 && a.captureDestinationX==901 && a.captureSourceX==902 && a.captureDestinationY==903 && a.captureDestinationWidth==904 && a.captureSourceHeight==905 && a.captureSourceY==906 && a.captureDestinationHeight==907 && a.captureSourceWidth==908 && a.captureRequestState==909,"busy-capture-fields-unchanged");
 reset();a.captureSurfaceIndex=0;GameCompleteStageTransition(&menu);checkCompleted(false);
 CHECK(menu.mainVm.value==101 && menu.secondaryVm.value==101,"busy-capture-owner-continues");
 queueAt(0,1,0,"bgm/th10_17.wav","busy-capture-still-loads");
}
void testQueueCapacity() {
 for(int occupied:{0,29,30,31}) for(bool release:{false,true}) {
  reset();g_FrontEndSupervisorFlags=release?0x10u:0u;
  for(int i=0;i<occupied;++i) sound.commandQueue[i].opcode=9;
  GameCompleteStageTransition(&menu);checkCompleted(false);
  CHECK(profile[0x1d8a3]==1 && menu.savedSpeed==.5f,"full-queue-still-publishes-completion");
  for(int i=0;i<occupied;++i) CHECK(sound.commandQueue[i].opcode==9 && sound.commandQueue[i].step==902,"occupied-queue-slots-unchanged");
  if(occupied<31) queueAt(occupied,1,0,"bgm/th10_17.wav","capacity-load-command");
  if(occupied+1<31) queueAt(occupied+1,release?4:2,0,"dummy","capacity-second-command");
  if(release && occupied+2<31) queueAt(occupied+2,2,0,"dummy","capacity-third-command");
  CHECK(sound.commandQueue[31].opcode==0 && sound.commandQueue[31].step==902 && std::strcmp(sound.commandQueue[31].path,"untouched")==0,"reserved-queue-slot31-unchanged");
 }
 reset();sound.commandQueue[0].opcode=9;sound.commandQueue[2].opcode=8;
 sound.QueueCommand("hole",5,-17);queueAt(1,5,-17,"hole","queue-first-free-hole");
 CHECK(sound.commandQueue[2].opcode==8,"queue-after-hole-unchanged");
}
void testMusicHelpers() {
 reset();CHECK(FrontEndLoadMusic(3,"bgm/test.mid")==1,"load-helper-result");queueAt(0,1,3,"bgm/test.wav","real-extension-replacement");
 for(unsigned flags:{0u,0x10u}) {
  reset();g_FrontEndSupervisorFlags=flags;
  CHECK(FrontEndPlayMusic(4,17)==0,"play-helper-result");
  if(flags) {queueAt(0,4,0,"dummy","helper-release");queueAt(1,2,4,"dummy","helper-play");}
  else queueAt(0,2,4,"dummy","helper-play");
  CHECK(profile[0x1d8a3]==1,"play-helper-profile");
 }
}
void testScaledSound() {
 struct Case { float speed,magnitude;int expected; };
 const Case cases[]={{0.0f,2.0f,2},{0.0f,8.0f,8},{.5f,2.0f,4},{.5f,8.0f,16},{1.0f,2.0f,2},{1.0f,8.0f,8},{2.0f,2.0f,2},{2.0f,8.0f,8},{0.0f,8.75f,8},{-0.0f,-8.75f,-8},{.5f,8.75f,17},{.75f,8.75f,11},{1.0f,-8.75f,-8},{1.0001f,8.75f,8},{2.0f,8.75f,8},{-.5f,8.75f,-17},{-2.0f,-8.75f,4},{.75f,16777216.0f,22369621},{.5f,0.0f,0},{.5f,-.75f,-1}};
 for(const auto& item:cases) {
  reset();g_AnmGameSpeed=item.speed;
  CHECK(GameQueueScaledSoundCommand(item.magnitude)==0,"scaled-sound-return-zero");
  queueAt(0,5,item.expected,"","scaled-sound-value-opcode-empty-path");
  CHECK(events==std::vector<string>({"queue::5:"+std::to_string(item.expected)}),"scaled-sound-one-command");
  CHECK(g_AnmGameSpeed==item.speed && menu.state==9,"scaled-sound-no-other-state");
 }
 reset();for(int i=0;i<31;++i)sound.commandQueue[i].opcode=9;
 CHECK(GameQueueScaledSoundCommand(2)==0,"scaled-full-queue-return");
 CHECK(sound.commandQueue[31].opcode==0,"scaled-full-queue-drop");
}
void testActualGuiCaller() {
 reset();g_GuiGameFlags=0x10;CompleteStageFromMessage();checkCompleted(false);
 CHECK(menu.mainVm.value==101 && menu.secondaryVm.value==101,"gui-real-live-menu-receiver");
 CHECK(std::find(events.begin(),events.end(),"clear-count")==events.end(),"gui-early-return-after-owner");
 reset(1);g_GuiGameFlags=0x10;g_GameConfigurationFlags=0x1000;CompleteStageFromMessage();
 checkUntouchedMenu();CHECK(g_GameTransitionState==2 && events.empty(),"gui-owner-preserves-game-early-mode");
 reset();g_GuiStage=7;CompleteStageFromMessage();checkCompleted(false);
 CHECK(gui.guiFlags==0x20 && events.back()=="clear-count","gui-stage-seven-calls-owner-then-count");
 CHECK(events[0]=="score:0" && events[1]=="score:0" && events[2]=="score:0","gui-stage-seven-score-precedes-owner");
}
int main() {
 testEarly();testNormal();testSupervisorReload();testCaptureBusy();testQueueCapacity();testMusicHelpers();testScaledSound();testActualGuiCaller();
 std::cout << "PASS: actual-body result-transition/scaled-sound host contracts; native ABI/storage/x87 remain unresolved\n";
}
