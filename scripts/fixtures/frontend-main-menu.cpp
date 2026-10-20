#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#include "FrontEnd.hpp"
#include "InputState.hpp"
#include "EnemySoundQueue.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>
#include <vector>
struct Failure { const char *label; };
#define CHECK(test,label) do { if(!(test)) throw Failure{label}; } while(0)
static std::vector<std::string> events;
static std::function<void(const std::string&)> after;
static void event(const std::string&s) { events.push_back(s); if(after) after(s); }
#include "helpers.inc"
static EnemySoundQueueView soundA;
// HOST typed object for the fixed native sound-owner array.
#define g_MainSoundOwner reinterpret_cast<unsigned char*>(&soundA)
static void HostObserveQueue(EnemySoundQueueView *owner,int id,int sample) {
 CHECK(owner==reinterpret_cast<EnemySoundQueueView*>(g_MainSoundOwner),"sound-owner");
 CHECK(sample==0,"sound-sample");event("sound:"+std::to_string(id));
}
#include "sound.inc"
FrontEndInputView g_FrontEndInput;
unsigned int g_FrontEndFlags;
int g_ReplayDifficulty;
static unsigned char profile[0x1da00];
unsigned char *g_FrontEndProfileData=profile;
AnmRenderManagerView::AnmRenderManagerView() {}
AnmRenderManagerView::~AnmRenderManagerView() {}
AnmLoadedView::AnmLoadedView() {}
AnmVmView::AnmVmView() {}
AnmVmView::~AnmVmView() {}
static AnmRenderManagerView manager;
AnmRenderManagerView *g_AnmRenderManagerView=&manager;
static AnmLoadedView resource;
static AnmVmView found;
static bool backdropPresent;
AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id) {
 CHECK(this==&manager&&id.value==88,"backdrop-lookup");event("find:88");
 return backdropPresent?&found:NULL;
}
AnmVmIdView AnmLoadedView::CreateVmVariant0(int script,unsigned int layer) {
 CHECK(this==&resource&&script==0&&layer==15,"intro-resource");event("aux-create");return AnmVmIdView(777);
}
static void CreateVm(FrontEndControllerView *,int slot) {event("create:"+std::to_string(slot));}
static void DeleteVm(FrontEndControllerView *c,int slot) {event("delete:"+std::to_string(slot));c->vmIds[slot].value=0;}
static void InterruptVmNow(FrontEndControllerView *,int slot,short interrupt) {event("run:"+std::to_string(slot)+":"+std::to_string(interrupt));}
void FrontEndMarkVmInterrupt(FrontEndControllerView *,int slot,short interrupt) {event("interrupt:"+std::to_string(slot)+":"+std::to_string(interrupt));}
static int *auxiliaryAddress;
void EnemyMarkPendingInterrupt(int *id) {CHECK(id==auxiliaryAddress,"intro-auxiliary");event("aux-retire");*id=0;}
void FrontEndControllerView::SetScreenStateTarget(int state) {screenState=state;stateTimer.current=0;event("phase:"+std::to_string(state));}
void FrontEndControllerView::SetScreenTarget(int value) {screen=value;screenState=0;stateTimer.current=0;event("screen:"+std::to_string(value));}
#include "owner.inc"
static FrontEndControllerView c;
static void reset(int state=2) {
 after={};std::memset(&c,0,sizeof(c));std::memset(profile,0,sizeof(profile));
 c.screen=2;c.screenState=state;c.stateTimer.current=20;c.cursor.count=8;c.cursor.wraps=1;
 c.vmIds[88].value=88;c.titleAnm=&resource;c.titleAuxVmId.value=777;
 auxiliaryAddress=&c.titleAuxVmId.value;g_FrontEndInput={};g_FrontEndFlags=0;g_ReplayDifficulty=1;
 soundA={};
 std::fill(soundA.activeSoundIds,soundA.activeSoundIds+12,-1);
 for(int i=0;i<134;++i)g_EnemySoundCueMetadata[i].cueValue=i+500;
 backdropPresent=true;events.clear();
}
static void menu() {CHECK(FrontEndUpdateMainMenuReturn(&c)==1,"menu-return");}
static void intro() {CHECK(FrontEndUpdateMainMenu(&c)==1,"intro-return");}
static bool has(const std::string&s) {return std::find(events.begin(),events.end(),s)!=events.end();}
static int count(const std::string&s) {return int(std::count(events.begin(),events.end(),s));}
static void suite() {
 reset();CHECK(FrontEndHasAnyClear(profile)==0,"six-clear-flags");
 for(int i=0;i<6;++i)for(int value:{1,2,128,255}) {std::memset(profile,0,sizeof(profile));profile[0x1d888+i]=value;CHECK(FrontEndHasAnyClear(profile)==1,"six-clear-flags");}
 std::memset(profile,0,sizeof(profile));profile[0x1d887]=profile[0x1d88e]=255;CHECK(FrontEndHasAnyClear(profile)==0,"six-clear-flags");
 for(int state:{-1,3,5,100}) {reset(state);menu();intro();CHECK(events.empty(),"unused-phases");}
 reset(0);backdropPresent=false;g_FrontEndFlags=0x30;c.cursor.disabledEntryCount=1;c.cursor.disabledEntries[0]=4;menu();
 CHECK(c.cursor.count==8&&c.cursor.current==2&&c.cursor.disabledEntryCount==2&&c.cursor.disabledEntries[1]==1&&g_FrontEndFlags==0x20,"menu-initialize");
 CHECK(events==std::vector<std::string>({"find:88","create:88","create:0","phase:1"}),"menu-initialize-order");
 reset(0);profile[0x1d888]=1;menu();CHECK(c.cursor.disabledEntryCount==0&&!has("create:88"),"extra-available");
 reset(1);c.stateTimer.current=10;menu();CHECK(events.empty()&&c.screenState==1,"open-eleven");
 c.stateTimer.current=11;c.cursor.current=3;menu();CHECK(events==std::vector<std::string>({"phase:2","run:0:3","interrupt:0:20"}),"opening-animation");
 reset();g_FrontEndInput.repeated=0x10;menu();CHECK(c.cursor.previous==0&&c.cursor.current==7&&has("sound:12")&&has("interrupt:0:14"),"move-wrap");
 reset();c.cursor.disabledEntries[0]=1;c.cursor.disabledEntryCount=1;g_FrontEndInput.repeated=0x20;menu();CHECK(c.cursor.current==2,"skip-disabled");
 reset();g_FrontEndInput.repeated=0x30;menu();CHECK(c.cursor.current==0&&!has("sound:12"),"both-directions");
 reset();c.cursor.current=7;g_FrontEndInput.pressed=0x1009;menu();
 CHECK(events==std::vector<std::string>({"sound:11","phase:4"}),"quit-cancel-priority");
 reset();g_FrontEndInput.pressed=0x1009;menu();CHECK(c.cursor.current==7&&has("interrupt:0:6")&&count("sound:11")==2&&c.screenState==4,"cancel-then-confirm");
 for(int selected=0;selected<8;++selected) {
  reset();c.cursor.current=selected;g_FrontEndInput.current=0x1001;menu();CHECK(events.empty(),"pressed-confirm");
  g_FrontEndInput.current=0;g_FrontEndInput.pressed=1;menu();CHECK(c.screenState==4&&has("interrupt:0:6"),"pressed-confirm");
  if(selected==5)CHECK(count("sound:10")==2&&events.back()=="sound:10","music-double-sound");
  else CHECK(count("sound:10")==int(selected!=7),"selection-sound");
  if(selected<=5)CHECK(has("interrupt:90:7")&&has("interrupt:91:7"),"leaving-animation");
  if(selected==6)CHECK(has("run:90:3")&&has("run:91:3")&&!has("interrupt:90:7"),"options-animation");
 }
 reset();c.cursor.current=5;g_FrontEndInput.pressed=1;menu();
 CHECK(soundA.activeSoundIds[0]==10&&soundA.sampleCounts[0]==2&&soundA.samples[0][0]==0&&soundA.samples[0][1]==0,"music-queue-twice");
 reset();g_FrontEndInput.repeated=0x20;after=[](const std::string&s){if(s=="run:0:3")c.cursor.current=4;};menu();CHECK(has("interrupt:0:11"),"live-animation-selection");
 const int screens[]={6,6,6,12,11,14,4,3};
 for(int selected=0;selected<8;++selected) {
  reset(4);c.cursor.current=selected;c.stateTimer.current=19;menu();CHECK(events.empty(),"close-twenty");
  c.stateTimer.current=20;g_FrontEndFlags=0x30;g_ReplayDifficulty=3;menu();
  CHECK(c.screen==screens[selected]&&c.screenState==0,"closing-screen");
  CHECK(c.cursor.saveDepth==int(selected!=7),"closing-stack");
  if(selected!=7)CHECK(c.cursor.savedCurrent[0]==selected&&c.cursor.savedCount[0]==8,"closing-saved-cursor");
  CHECK(has("delete:88")==bool(selected<6),"closing-backdrop");
  if(selected==1)CHECK(g_ReplayDifficulty==4&&c.savedDifficulty==3&&c.cursor.current==0,"extra-saved-difficulty");
  if(selected==0||selected==1)CHECK(g_FrontEndFlags==0x20,"clear-practice-flag");
  if(selected==2)CHECK(g_FrontEndFlags==0x30,"set-practice-flag");
 }
 for(int difficulty:{-1,0,1,3,4,5})for(int selected:{0,2}) {reset(4);c.cursor.current=selected;g_ReplayDifficulty=difficulty;menu();CHECK(g_ReplayDifficulty==(difficulty>=4?1:difficulty)&&c.cursor.current==(difficulty<0?0:difficulty>=4?1:difficulty),"difficulty-selection");}
 reset(0);backdropPresent=false;intro();CHECK(c.cursor.current==0&&c.titleAuxVmId.value==777&&events==std::vector<std::string>({"find:88","create:88","create:90","aux-create","phase:1"}),"intro-create-order");
 reset(0);intro();CHECK(events==std::vector<std::string>({"find:88","phase:1"}),"intro-existing-backdrop");
 reset(1);c.stateTimer.current=10;intro();CHECK(events.empty(),"open-eleven");c.stateTimer.current=11;intro();CHECK(c.screenState==2,"intro-open");
 reset();g_FrontEndInput.current=0x160b;intro();CHECK(events.empty(),"intro-edge-only");
 g_FrontEndInput.pressed=0x160b;intro();CHECK(events==std::vector<std::string>({"interrupt:90:6","phase:4","sound:32","aux-retire"})&&c.titleAuxVmId.value==0,"intro-exit-order");
 reset(4);c.stateTimer.current=29;intro();CHECK(events.empty(),"intro-thirty");c.stateTimer.current=30;intro();CHECK(c.screen==2&&has("screen:2"),"intro-thirty");
}
int main() {try {suite();}catch(const Failure&f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
