#define __stdcall
#define __fastcall
#define __cdecl
#define __declspec(x)
#include "ResultMenu.hpp"
#include "ReplayManager.hpp"
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <functional>
#include <string>
#include "types.inc"
struct Failure { const char *label; };
#define CHECK(x,label) do { if (!(x)) throw Failure{label}; } while (0)
// HOST constructors provide defined prefill, excluding native construction.
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
AsciiManagerView::AsciiManagerView(){numStrings=0;numGuiStrings=0;color=0;}
AsciiManagerView::~AsciiManagerView(){}
size_t AsciiManagerView::GetSize(){return sizeof(*this);}
ReplayStageState::ReplayStageState(){std::memset(this,0,sizeof(*this));}
ReplayStageState::~ReplayStageState(){}
ReplayManager::ReplayManager(){std::memset(this,0,sizeof(*this));}
ReplayManager::~ReplayManager(){}
static AsciiManagerView asciiA,asciiB;
AsciiManagerView *g_AsciiManagerView=&asciiA;
static ResultMenuView menu;
static ReplayManager files[25];
static ReplayDataHeader data[25];
ReplayManager *g_ReplayManager=&files[0];
// HOST byte-storage convenience. Actual owner code uses its extracted helper.
struct HostScoreProfile {
 unsigned char prefix[0x18]; FrontEndScoreRecordView records[5][10];
 unsigned char tail[0x437c-0x4c8];
};
static_assert(sizeof(HostScoreProfile)==0x437c,"host int32 score backing");
static HostScoreProfile profileA[6],profileB[6];
unsigned char *g_FrontEndProfileData=reinterpret_cast<unsigned char *>(profileA);
int g_ReplayCharacter,g_ReplayShotType,g_ReplayDifficulty,g_FrontEndSelectedStage;
const char *g_ReplayCharacterNames[]={"S0","S1","S2","S3","S4","S5"};
const char *g_ReplayDifficultyNames[]={"D0","D1","D2","D3","D4"};
const char *g_ReplayCompletionNames[]={"St0","St1","St2","St3","St4","St5","St6","St7","St8"};
const char *g_FrontEndStageNames[]={"Stage0","Stage1","Stage2","Stage3","Stage4","Stage5","Stage6","Stage7","Stage8"};
static std::string alphabet;
const char *g_FrontEndScoreEntryAlphabet;
static int timeCalls;
static std::function<void(std::int32_t *)> afterTime;
static std::tm *HostLocaltime(std::int32_t *stamp){
 int captured=*stamp;++timeCalls;if(afterTime)afterTime(stamp);
 static std::tm result;result={};result.tm_year=100+captured%70;
 result.tm_mon=captured%12;result.tm_mday=1+captured%28;return &result;
}
#include "helpers.inc"
#define localtime HostLocaltime
#include "owners.inc"
#undef localtime
static void reset(int state=10){
 std::memset(&menu,0,sizeof(menu));std::memset(data,0,sizeof(data));
 std::memset(profileA,0,sizeof(profileA));std::memset(profileB,0,sizeof(profileB));
 for(int i=0;i<25;++i){files[i].replayData=&data[i];std::snprintf(data[i].replayName,9,"R%d",i);}
 for(auto *a:{&asciiA,&asciiB}){a->numStrings=0;a->numGuiStrings=0;a->color=0xff123456;a->drawShadow=7;a->scaleX=1;a->scaleY=1;a->viewportIndex=2;}
 menu.state=state;menu.cursor.current=3;menu.keyboard.current=14;menu.nameIndex=3;std::strcpy(menu.name,"NAME");
 alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabc?<!";g_FrontEndScoreEntryAlphabet=alphabet.c_str();
 g_AsciiManagerView=&asciiA;g_ReplayManager=&files[0];g_ReplayCharacter=0;g_ReplayShotType=0;g_ReplayDifficulty=0;g_FrontEndSelectedStage=7;
 g_FrontEndProfileData=reinterpret_cast<unsigned char *>(profileA);timeCalls=0;afterTime={};
}
static bool near(float a,float b){return std::fabs(a-b)<.001f;}
static void draw(){CHECK(ResultMenuDrawRowsCallback(&menu)==1,"callback-result");}
static void nameCases(){
 for(int length:{0,1,2,3,13,14,27})for(int cursor:{0,7,8}){
  reset();alphabet.assign(length,'A');g_FrontEndScoreEntryAlphabet=alphabet.c_str();menu.nameIndex=cursor;AnmFloat3View position(7,8,9);
  ResultMenuDrawNameEntry(&menu,position);
  CHECK(position.x==7&&position.y==8&&position.z==9,"name-position-value");
  CHECK(asciiA.numStrings==length+2&&std::string(asciiA.strings[0].text)=="NAME","name-count");
  CHECK(near(asciiA.strings[1].position.x,7+(cursor==8?7:cursor)*9)&&asciiA.strings[1].color==0xffffff00u,"full-name-underline");
  for(int i=0;i<length;++i){auto &s=asciiA.strings[i+2];
   CHECK(near(s.position.x,112+(i%13)*18)&&near(s.position.y,320+(i/13)*16)&&s.position.z==0,"grid-wrap");
   unsigned char expected=i<length-3?'A':i==length-3?0x81:i==length-2?0x7f:0x80;
   CHECK(static_cast<unsigned char>(s.text[0])==expected&&s.text[1]==0,"final-control-glyphs");
   CHECK(s.color==(i==14?0xffffff00u:0xff808080u),"grid-highlight");
  }
  CHECK(asciiA.color==0xffffffffu,"name-final-color");
 }
}
static void replayCases(){
 for(int state:{-1,0,9,13,16,20}){reset(state);unsigned color=asciiA.color;draw();CHECK(asciiA.numStrings==0&&asciiA.color==color&&asciiA.drawShadow==0,"default-phase");}
 for(int state:{10,17}){
  reset(state);draw();CHECK(asciiA.numStrings==25,"replay-row-count");
  for(int i=0;i<25;++i){auto &s=asciiA.strings[i];
   CHECK(s.position.x==48&&s.position.y==64+i*15&&s.position.z==0,"replay-row-position");
   CHECK(s.drawShadow==1,"shadow-queued");CHECK(s.color==(i==3?0xffffff00u:0xff808080u),"replay-highlight");
  }
  CHECK(asciiA.color==0xffffffffu&&asciiA.drawShadow==0,"replay-final-state");
 }
 reset();menu.replayFiles[0]=&files[0];data[0].timestamp=17;data[0].character=1;data[0].shotType=2;data[0].difficulty=2;data[0].completionState=3;
 draw();CHECK(std::string(asciiA.strings[0].text)=="No.01 R0 17/06/18 S5 D2 St3"&&timeCalls==1,"replay-format");
 reset();menu.replayFiles[0]=&files[0];data[0].timestamp=17;
 afterTime=[](std::int32_t *){if(timeCalls==1){files[0].replayData=&data[24];g_AsciiManagerView=&asciiB;menu.cursor.current=1;}};
 draw();CHECK(asciiA.numStrings==0&&asciiB.numStrings==25,"replay-live-ascii");
 CHECK(std::string(asciiB.strings[0].text).find(" R0 ")!=std::string::npos,"replay-retained-header");
 CHECK(asciiB.strings[1].color==0xffffff00u,"replay-live-cursor");
 CHECK(asciiB.strings[0].drawShadow==7&&asciiA.drawShadow==1&&asciiB.drawShadow==0,"shadow-live-owner");
 reset();asciiA.numStrings=250;draw();CHECK(asciiA.numStrings==256&&asciiA.drawShadow==0,"queue-capacity");
}
static void overlayCases(){
 for(int state:{11,18})for(int frame:{0,9,10,11}){
  reset(state);menu.timer.current=frame;menu.cursor.current=4;data[0].timestamp=17;data[0].completionState=3;draw();
  int count=int(alphabet.size());CHECK(asciiA.numStrings==count+3,"overlay-count");float y=frame<10?124+(224-124)*frame*.1f:224;
  CHECK(asciiA.strings[0].position.x==102&&near(asciiA.strings[0].position.y,y)&&asciiA.strings[count+2].position.x==48&&near(asciiA.strings[count+2].position.y,y),"overlay-position");
  CHECK(std::string(asciiA.strings[count+2].text).find(" St7")!=std::string::npos,"overlay-stage");
 }
}
static void scoreCases(){
 for(int state:{12,19}){
  reset(state);menu.scoreUnavailable=1;g_ReplayCharacter=1;g_ReplayShotType=2;g_ReplayDifficulty=4;
  auto &r=profileA[5].records[4][0];r.score=1234;r.scoreSuffix=-5;std::strcpy(r.name,"ZERO");
  auto &s=profileA[5].records[4][1];s.score=5678;s.timestamp=17;s.stageNameIndex=2;std::strcpy(s.name,"DATED");
  draw();CHECK(asciiA.numStrings==11,"score-unavailable");
  CHECK(std::string(asciiA.strings[1].text).find("000001234-5 --/--/-- Stage -")!=std::string::npos,"score-signed-suffix");
  CHECK(std::string(asciiA.strings[2].text).find("17/06/18 Stage2")!=std::string::npos,"score-date");
  for(int i=0;i<10;++i)CHECK(asciiA.strings[i+1].color==0xff808080u&&asciiA.strings[i+1].position.y==96+i*18,"score-unavailable-highlight");
  CHECK(asciiA.drawShadow==0&&asciiA.color==0xffffffffu,"score-final-state");
 }
 reset(12);draw();int offset=3+int(alphabet.size());CHECK(asciiA.numStrings==offset+10,"score-name-count");
 CHECK(asciiA.strings[1].position.x==75&&asciiA.strings[1].position.y==150&&asciiA.strings[offset+3].color==0xffffff00u,"score-name-position");
 reset(12);menu.scoreUnavailable=1;auto &old=profileA[0].records[0][0];old.timestamp=17;std::strcpy(old.name,"OLD");
 auto &fresh=profileB[5].records[4][0];fresh.score=9999;fresh.scoreSuffix=-7;fresh.stageNameIndex=4;std::strcpy(fresh.name,"NEW");
 afterTime=[](std::int32_t *){if(timeCalls==1){g_FrontEndProfileData=reinterpret_cast<unsigned char *>(profileB);g_ReplayCharacter=1;g_ReplayShotType=2;g_ReplayDifficulty=4;g_AsciiManagerView=&asciiB;}};
 draw();CHECK(asciiB.numStrings==10&&std::string(asciiB.strings[0].text).find("NEW 000009999-7 17/06/18 Stage4")!=std::string::npos,"score-live-record");
}
int main(){try{nameCases();replayCases();overlayCases();scoreCases();}catch(const Failure &f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
