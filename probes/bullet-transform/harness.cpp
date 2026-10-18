#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>
struct Player {};
struct PlayerFloat3 { float x,y,z; };
struct AnmFloat3View {float x,y,z; void FromAngleMagnitude(float a,float m){x=std::cos(a)*m;y=std::sin(a)*m;}};
struct AnmVmTimerView {int current, calls; void SetCurrent(int n){current=n;++calls;}};
struct AnmLoadedView {};
struct AnmVmView { void InitializeForLoadedScript(AnmLoadedView*,int){} };
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
float EnemyAngleFromPlayer(Player*,const PlayerFloat3*) {++aimCalls;return 0.75f;}
void BulletQueueSoundAtPosition(int n,float) {events.push_back(n);}
unsigned int EnemyCancelBulletRecord(unsigned char*) {++cancels;events.push_back(-99);return 0;}
int BulletManagerView::SpawnBulletPattern(BulletSpawnDescriptorView *p) {
 captured=*p;++spawns;events.push_back(-88);
 if(callbackMode==1){parent->transformIndex=7;parent->transforms[8].kind=0x00020000;parent->transforms[8].payload.int0=888;}
 if(callbackMode==2){parent->transforms[2].kind=0x01000000;parent->transforms[2].payload.int0=222; parent->transforms[17].payload.int1=999;}
 return 0;
}
static void check(bool b,const char *s){if(!b){std::fprintf(stderr,"FAIL: %s\n",s);std::exit(31);}}
static BulletRuntimeView b;
static void reset(){std::memset(&b,0,sizeof(b));parent=&b;spawns=cancels=callbackMode=aimCalls=0;events.clear();b.angle=0.25f;b.speed=4.5f;b.transformSound=-1;}
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
 if(mode==1){check(b.transformIndex==9&&b.ownerTag==0,"postspawn increments live index");check(events.back()==888,"resume from callback-mutated cursor");}
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
 const float values[]={-991,-990,-989,989,990,std::numeric_limits<float>::quiet_NaN()};
 const float expected[]={.25f,.25f,-989,989,.75f,.75f};
 for(int i=0;i<6;++i){reset();rec(0,0x10,2,0,3,values[i]);b.AdvanceTransformProgram();check(b.exStates[1].value1==expected[i],"angle sentinels including NaN aim");check(aimCalls==(i>=4?1:0),"aim helper branch");}
 reset();rec(0,0x40,2,3,0,std::numeric_limits<float>::quiet_NaN());b.AdvanceTransformProgram();check(b.exStates[3].value0==4.5f,"NaN direction speed preserves speed");
}
int main(){child(0,false);child(0,true);child(1,true);child(2,false);controls();timers();angles();std::puts("PASS authored-source protocol checks");}
