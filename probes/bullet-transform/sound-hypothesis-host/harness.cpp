#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>
#include <new>
#include "extracted_sound.hpp"
alignas(EnemySoundQueueView) unsigned char g_MainSoundOwner[0x52d0];
alignas(EnemySoundQueueView) unsigned char g_TestSoundDecoy[sizeof(EnemySoundQueueView)];
static EnemySoundQueueView *soundOwner;
static EnemySoundQueueView *decoyOwner;
static_assert(sizeof(EnemySoundQueueView)<=sizeof(g_MainSoundOwner),"host sound buffer capacity");
struct Player {};
struct PlayerFloat3 { float x,y,z; };
struct AnmFloat3View {float x,y,z; void FromAngleMagnitude(float a,float m){x=std::cos(a)*m;y=std::sin(a)*m;}};
struct AnmVmTimerView {int current, calls; void SetCurrent(int n){current=n;++calls;}};
struct AnmLoadedView {};
struct AnmVmView { int calls, script; AnmLoadedView *loaded; void InitializeForLoadedScript(AnmLoadedView *a,int n){++calls;loaded=a;script=n;} };
Player player; Player *g_Player=&player;
#include "extracted_header.hpp"
#include "extracted_source.cpp"
BulletManagerView manager;
void *g_EnemyBulletManager=&manager;
int g_BulletSpriteScriptBase[16]={};
static BulletRuntimeView *parent;
static BulletSpawnDescriptorView captured;
static int spawns,cancels,callbackMode,aimCalls;
static std::vector<int> events;
struct SoundEvent { bool sample; EnemySoundQueueView *receiver; int id, integerSample; float x; BulletRuntimeView state; };
static std::vector<SoundEvent> sounds;
float EnemyAngleFromPlayer(Player*,const PlayerFloat3*) {++aimCalls;return 0.75f;}
void EnemySoundQueueView::QueueSoundSample(int n,int sample) {sounds.push_back({true,this,n,sample,0,*parent});events.push_back(n);}
void EnemySoundQueueView::QueueSoundCue(int n,float x) {sounds.push_back({false,this,n,0,x,*parent});events.push_back(n);}
unsigned int EnemyCancelBulletRecord(unsigned char*) {++cancels;events.push_back(-99);return 0;}
int BulletManagerView::SpawnBulletPattern(BulletSpawnDescriptorView *p) {
 captured=*p;++spawns;events.push_back(-88);
 if(callbackMode==1){parent->transformIndex=7;parent->transforms[8].kind=0x00020000;parent->transforms[8].payload.int0=888;}
 if(callbackMode==2){parent->transforms[2].kind=0x01000000;parent->transforms[2].payload.int0=222; parent->transforms[17].payload.int1=999;}
 return 0;
}
static void check(bool b,const char *s){if(!b){std::fprintf(stderr,"FAIL: %s\n",s);std::exit(31);}}
static BulletRuntimeView b;
static void reset(){std::memset(&b,0,sizeof(b));parent=&b;spawns=cancels=callbackMode=aimCalls=0;events.clear();sounds.clear();b.angle=0.25f;b.speed=4.5f;b.transformSound=-1;}
static void rec(int i,unsigned k,int a=0,int c=0,float f=0,float g=0,int allow=1){b.transforms[i]={{f,g,a,c},k,allow};}
static void child(int mode,bool fade){
 reset();callbackMode=mode;b.position={11,22,33};
 rec(0,0x00400000,static_cast<int>((fade?0x80000000u:0u)|0x0000fe05u),65534,1.25f,2.5f);
 // The second record is data, regardless of kind and gating fields.
 rec(1,0x00010000,65533,0x12345678,0.5f,0.125f,0);
 rec(2,0x01000000,123);rec(3,0);b.transforms[17].payload.int1=17017;
 BulletTransformRecordView before[18];std::memcpy(before,b.transforms,sizeof(before));
 b.AdvanceTransformProgram();
 check(spawns==1,"one child spawn");check(captured.bulletType==0,"child bullet type zero");
 check(captured.color==-2,"packed color sign extends 8 bits");check(captured.transformStartIndex==5,"packed low-byte index");
 check(captured.count1==-2 && captured.count2==-3,"signed low-16 counts");
 check(captured.position.x==11 && captured.position.y==22 && captured.position.z==33,"position copy");
 check(captured.speed1==1.25f&&captured.speed2==2.5f&&captured.angle==0.5f&&captured.angleStep==0.125f,"two-record floats");
 check(captured.transformFlags==0x12345678 && captured.transformSound==-1,"child flags and sound");
 check(captured.aimMode==0&&captured.spawnSound==0&&captured.templateSprites==nullptr&&captured.laserWidth==0,"zero initialized fields");
 check(std::memcmp(before,captured.transforms,sizeof(before))==0,"all 18 records copied before callback");
 check(cancels==(fade?1:0),"fade cancellation only; data record not executed");
 if(mode==1){check(b.transformIndex==9&&b.ownerTag==0,"postspawn increments live index");check(events.back()==888,"resume from callback-mutated cursor");check(sounds.size()==1&&!sounds[0].sample&&sounds[0].receiver==soundOwner&&sounds[0].id==888,"child callback continuation uses typed Cue");}
 else {check(b.transformIndex==3,"child consumes two records");check(b.ownerTag==(mode==2?222:123),"resume reloads callback-mutated records");}
 check(events[0]==-88,"spawn precedes cancellation");if(fade)check(events[1]==-99,"fade immediately after spawn");
}
static void controls(){
 reset();rec(0,0x80000000);rec(1,0x00000002);rec(2,0x00002000,33);rec(3,0x01000000,44);rec(4,0x02000000,6);rec(5,0x00010000);rec(6,0x00020000,77);b.AdvanceTransformProgram();
 check(b.transformIndex==7&&b.offscreenCullDelayFrames==33&&b.ownerTag==44&&cancels==0&&events==std::vector<int>{77},"finite control chain");
 reset();b.activeTransformFlags=1;rec(0,0x80000000,0,0,0,0,0);b.AdvanceTransformProgram();check(b.transformIndex==0,"SKIP is gated");
 reset();b.activeTransformFlags=1;rec(0,0x2,0,0,0,0,0);b.AdvanceTransformProgram();check(b.transformIndex==0,"unknown is gated");
 reset();b.activeTransformFlags=1;rec(0,0,0,0,0,0,1);rec(1,0x00010000);b.AdvanceTransformProgram();check(b.transformIndex==0&&cancels==0,"NONE terminates");
 reset();b.transformIndex=18;b.AdvanceTransformProgram();check(b.transformIndex==18,"upper bound returns");
 reset();rec(0,0x00010000);rec(1,0x00020000,55);b.AdvanceTransformProgram();check(b.transformIndex==2&&events==std::vector<int>({-99,55}),"despawn continues");
}
static void timers(){
 const unsigned kinds[]={1,0x10,0x20,0x40,0x80,0x100,0x8000,0x100000,0x200000,0x4000000};
 const int slots[]={0,1,2,3,3,3,5,6,7,8};
 const int starts[]={0,0,0,0,0,0,13,13,13,0};
 for(int i=0;i<10;++i){reset();for(auto &s:b.exStates){s.timer.current=98;s.timer.calls=0;s.int2=82;s.vector.z=9;}
 rec(0,kinds[i],13,14,2,3);rec(1,0x00020000,66,0,0,0,0);b.AdvanceTransformProgram();
 check(b.transformIndex==1&&b.activeTransformFlags==kinds[i]&&events.empty(),"active startup blocks next gated record");
 for(int j=0;j<9;++j){check(b.exStates[j].timer.calls==(j==slots[i]?1:0),"only selected timer reset");check(b.exStates[j].timer.current==(j==slots[i]?starts[i]:98),"timer startup argument");}
 if(i==0)check(b.exStates[0].vector.z==0,"decelerate zero vector z");
 if(i>=3&&i<=5)check(b.exStates[3].int2==0&&b.exStates[3].int0==13&&b.exStates[3].int1==14,"direction counters");
 }
}
static void angles(){
 const float inf=std::numeric_limits<float>::infinity();
 const float nan=std::numeric_limits<float>::quiet_NaN();
 const float values[]={-inf,-991,-990,-989,989,990,991,inf,nan};
 const float expected[]={.25f,.25f,.25f,-989,989,.75f,.75f,.75f,.75f};
 for(int i=0;i<9;++i){
  reset();rec(0,0x10,2,0,3,values[i]);b.AdvanceTransformProgram();
  if(i==8)check(std::isnan(b.exStates[1].value1),"vector NaN angle remains NaN");
  else check(b.exStates[1].value1==expected[i],"vector finite and infinite angle sentinels");
  check(aimCalls==((i>=5&&i<=7)?1:0),"vector aim is lazy; NaN never calls aim");
 }
 for(unsigned kind:{0x10u,0x40u,0x80u,0x100u})for(float zero:{0.0f,-0.0f}){
  reset();
  if(kind==0x10)rec(0,kind,2,3,2,zero);
  else rec(0,kind,2,3,zero,2);
  b.AdvanceTransformProgram();
  const float actual=b.exStates[kind==0x10?1:3].value1;
  check(actual==0&&std::signbit(actual)==std::signbit(zero)&&aimCalls==0,"literal signed zero preserved without aim");
 }
 for(float angle:{inf,nan}){
  reset();b.activeTransformFlags=1;b.exStates[1].value1=17;
  BulletRuntimeView before;std::memcpy(&before,&b,sizeof(before));
  rec(0,0x10,2,3,2,angle,0);std::memcpy(before.transforms,b.transforms,sizeof(b.transforms));
  b.AdvanceTransformProgram();
  check(aimCalls==0&&std::memcmp(&before,&b,sizeof(b))==0,"gated vector neither calls aim nor writes state");
 }
 for(unsigned kind:{0x40u,0x80u,0x100u}){
  for(int i=0;i<9;++i){
   reset();rec(0,kind,2,3,values[i],2.0f);b.AdvanceTransformProgram();
   check(b.activeTransformFlags==kind&&b.transformIndex==1,"direction matrix executes selected action");
   if(i==8)check(std::isnan(b.exStates[3].value1),"direction NaN angle remains NaN");
   else check(b.exStates[3].value1==expected[i],"direction finite and infinite angle sentinels");
   check(aimCalls==((i>=5&&i<=7)?1:0),"direction aim is lazy; NaN never calls aim");
  }
  reset();rec(0,kind,2,3,0,nan);b.AdvanceTransformProgram();
  check(b.exStates[3].value0==4.5f&&aimCalls==0,"NaN direction speed preserves speed without aim");
  reset();b.activeTransformFlags=1;b.exStates[3].value1=17;
  BulletRuntimeView before;std::memcpy(&before,&b,sizeof(before));
  rec(0,kind,2,3,inf,2,0);std::memcpy(before.transforms,b.transforms,sizeof(b.transforms));
  b.AdvanceTransformProgram();
  check(aimCalls==0&&std::memcmp(&before,&b,sizeof(b))==0,"gated direction neither calls aim nor writes state");
 }
}
static void bounceAndCancel(){
 const unsigned kinds[]={0x400,0x800,0x08000000};
 for(unsigned kind:kinds){
  reset();b.activeTransformFlags=0x10;
  for(auto &state:b.exStates){state.timer.current=73;state.timer.calls=4;state.value0=12;state.value1=13;state.int0=17;state.int1=18;state.int2=19;}
  BulletExStateView expected[9];std::memcpy(expected,b.exStates,sizeof(expected));
  expected[4].value0=1.25f;expected[4].int1=-7;expected[4].int0=0;
  rec(0,kind,-7,61,1.25f,2.75f);rec(1,0x00020000,91,0,0,0,0);
  b.AdvanceTransformProgram();
  check(b.activeTransformFlags==(0x10|kind),"bounce preserves existing flags and adds selected kind");
  check(std::memcmp(expected,b.exStates,sizeof(expected))==0,"bounce writes slot4 value0/int1/int0 only");
  check(b.transformIndex==1&&events.empty(),"bounce startup gates following action");
 }
 for(int value:{-7,4}){
  reset();b.cancelBehavior=99;rec(0,0x1000,value,61);rec(1,0x00020000,92,0,0,0,0);
  b.AdvanceTransformProgram();
  check(b.cancelBehavior==value,"cancel behavior uses payload int0");
  check(b.activeTransformFlags==0&&b.transformIndex==2&&events==std::vector<int>{92}&&cancels==0,"cancel behavior only assigns and continues");
 }
}
static void spriteArguments(){
 for(int offset:{-4,7}){
  reset();AnmLoadedView loaded;manager.bulletAnm=&loaded;g_BulletSpriteScriptBase[3]=120;
  rec(0,0x4000,3,offset);rec(1,0x01000000,49);
  b.AdvanceTransformProgram();
  check(b.vm.calls==1&&b.vm.loaded==&loaded,"sprite initializes parent vm with manager anm");
  check(b.vm.script==120+offset,"sprite script is indexed base plus signed offset");
  check(b.transformIndex==2&&b.ownerTag==49&&b.activeTransformFlags==0,"sprite continues without active flag");
  manager.bulletAnm=nullptr;
 }
}
static void transformSounds(){
 for(unsigned kind:{0x10u,0x20u})for(int index:{0,1})for(int sound:{-1,0,27}){
  reset();b.position.x=123;b.transformSound=sound;b.transformIndex=index;
  rec(index,kind,7,8,2,0.5f);b.AdvanceTransformProgram();
  const bool shouldPlay=index==1&&sound>=0;
  check(b.transformIndex==index+1&&b.activeTransformFlags==kind,"acceleration sound fixture executes action");
  check(events.size()==(shouldPlay?1u:0u)&&sounds.size()==(shouldPlay?1u:0u),"vector and polar sound require later index and nonnegative id");
  if(shouldPlay){check(events[0]==sound,"transform sound uses configured id");check(sounds[0].sample&&sounds[0].integerSample==0,"transform sound uses zero position");}
 }
 reset();b.position.x=123;rec(0,0x20000,41);b.AdvanceTransformProgram();
 check(events==std::vector<int>{41}&&sounds.size()==1&&!sounds[0].sample&&sounds[0].x==123,"explicit sound uses bullet position");
}
static void typedSamples(){
 for(unsigned kind:{0x10u,0x20u})for(int index:{0,1,17})for(int sound:{-1,0,27})for(unsigned active:{0u,1u}){
  reset();b.position={123.75f,-44,0};b.transformIndex=index;b.transformSound=sound;b.activeTransformFlags=active;b.ownerTag=91;
  for(auto &s:b.exStates){s.timer={73,4};s.value0=11;s.value1=12;s.vector={13,14,15};s.int0=16;s.int1=17;s.int2=18;}
  BulletExStateView expected[9];std::memcpy(expected,b.exStates,sizeof(expected));
  const int slot=kind==0x10?1:2;expected[slot].timer={0,5};expected[slot].value0=2;expected[slot].value1=0;expected[slot].int0=7;
  if(kind==0x10)expected[slot].vector={2,0,15};
  rec(index,kind,7,8,2,0);if(index<17)rec(index+1,0x01000000,99);
  b.AdvanceTransformProgram();const bool play=index!=0&&sound>=0;
  check(sounds.size()==(play?1u:0u),"typed Sample gate");
  if(play){const auto &e=sounds[0];
   check(e.sample,"typed Sample member identity");check(e.receiver==soundOwner,"typed Sample receiver identity");
   check(e.id==sound,"typed Sample sound id");check(e.integerSample==0,"typed Sample integer zero");
   check(e.state.transformIndex==index,"typed Sample before cursor increment");
   check(e.state.ownerTag==91,"typed Sample before continuation");
   check(e.state.activeTransformFlags==(active|kind)&&std::memcmp(expected,e.state.exStates,sizeof(expected))==0,"typed Sample after state initialization");
   check(e.state.transformSound==sound,"typed Sample configured sound snapshot");
  }
  check(b.transformIndex==(index==17?18:index+2)&&b.ownerTag==(index==17?91:99),"typed Sample continuation");
 }
 for(unsigned kind:{0x10u,0x20u}){
  reset();b.activeTransformFlags=1;b.transformSound=27;b.transformIndex=1;rec(1,kind,7,8,2,0,0);
  BulletRuntimeView before=b;b.AdvanceTransformProgram();
  check(sounds.empty()&&std::memcmp(&before,&b,sizeof(b))==0,"typed Sample common admission");
 }
}
static void oneTypedCue(int index,int configured,float x,int id,unsigned active){
 reset();b.position={x,44,0};b.transformIndex=index;b.transformSound=configured;b.ownerTag=91;b.activeTransformFlags=active;
 rec(index,0x20000,id,72);if(index<17)rec(index+1,0x01000000,99);
 b.AdvanceTransformProgram();
 check(sounds.size()==1,"typed Cue no extra index or sign gate");const auto &e=sounds[0];
 check(!e.sample,"typed Cue member identity");check(e.receiver==soundOwner,"typed Cue receiver identity");
 check(e.id==id,"typed Cue payload int0");
 if(std::isnan(x))check(std::isnan(e.x),"typed Cue X classification");
 else check(e.x==x&&(x!=0||std::signbit(e.x)==std::signbit(x)),"typed Cue exact X");
 check(e.state.transformIndex==index,"typed Cue before cursor increment");
 check(e.state.ownerTag==91,"typed Cue before continuation");
 check(e.state.activeTransformFlags==active&&e.state.transformSound==configured,"typed Cue state snapshot");
 check(b.transformIndex==(index==17?18:index+2)&&b.ownerTag==(index==17?91:99),"typed Cue continuation");
}
static void typedCues(){
 for(int index:{0,1,17})for(int configured:{-1,0,27})for(unsigned active:{0u,1u})
  for(float x:{-123.75f,0.0f,-0.0f,12.25f,std::numeric_limits<float>::quiet_NaN()})oneTypedCue(index,configured,x,-9,active);
 reset();b.activeTransformFlags=1;rec(0,0x20000,53,72,0,0,0);BulletRuntimeView before=b;b.AdvanceTransformProgram();
 check(sounds.empty()&&std::memcmp(&before,&b,sizeof(b))==0,"typed Cue common admission");
}
int main(int argc,char **argv){soundOwner=::new (static_cast<void *>(g_MainSoundOwner)) EnemySoundQueueView{};decoyOwner=::new (static_cast<void *>(g_TestSoundDecoy)) EnemySoundQueueView{};check(soundOwner!=decoyOwner,"distinct live sound receivers");if(argc==2&&std::strcmp(argv[1],"finite-cue")==0){oneTypedCue(0,-1,-123.75f,-9,0);return 0;}typedSamples();typedCues();child(0,false);child(0,true);child(1,true);child(2,false);controls();timers();angles();bounceAndCancel();spriteArguments();transformSounds();std::puts("PASS authored-source protocol checks");}
