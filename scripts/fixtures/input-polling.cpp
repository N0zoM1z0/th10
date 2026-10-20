#define __stdcall
#define __fastcall
#define __declspec(x)
#include "InputPolling.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <vector>
struct Failure { const char *label; };
#define CHECK(x,label) do { if (!(x)) throw Failure{label}; } while (0)
static InputBindingSetView inputStates[3];
unsigned int g_FrontEndSupervisorFlags;
int g_InputEnabled;
InputDeviceView *g_InputKeyboardDevice, *g_InputPadDevices[3];
InputJoystickCapsView g_InputJoystickCaps[3];
short g_InputJoystickXThreshold, g_InputJoystickYThreshold;
signed char g_InputJoystickButtons[128];
static unsigned char keyboard[256];
static InputJoystickInfoView joy;
static InputDirectJoystickStateView direct;
static unsigned int joyResult, joyId;
static int keyboardResult, pollResult, stateResult, acquireResult, acquireCalls;
static bool requireZero;
static std::function<void()> onJoy, onKeyboard, onPoll, onAcquire, onState;
struct Call { char method; InputDeviceView *device; };
static std::vector<Call> calls;
extern "C" int GetKeyboardState(unsigned char *out) {
    std::memcpy(out,keyboard,sizeof keyboard); if(onKeyboard)onKeyboard(); return keyboardResult;
}
extern "C" unsigned int joyGetPosEx(unsigned int id, InputJoystickInfoView *out) {
    CHECK(out->size==0x34&&out->flags==0xff,"winmm-header");
    const unsigned char *bytes=reinterpret_cast<const unsigned char*>(out);
    CHECK(std::all_of(bytes+8,bytes+sizeof(*out),[](unsigned char b){return b==0;}),"winmm-zero");
    joyId=id;*out=joy;if(onJoy)onJoy();return joyResult;
}
static int acquire(InputDeviceView *d) { calls.push_back({'A',d});++acquireCalls;if(onAcquire)onAcquire();return acquireResult; }
static int poll(InputDeviceView *d) { calls.push_back({'P',d});if(onPoll)onPoll();return pollResult; }
static int state(InputDeviceView *d,unsigned int size,void *out) {
    calls.push_back({'S',d});
    if(size==256)std::memcpy(out,keyboard,sizeof keyboard);
    else {
        CHECK(size==0x110,"direct-size");
        if(requireZero){const unsigned char *b=static_cast<const unsigned char*>(out);CHECK(std::all_of(b,b+size,[](unsigned char x){return x==0;}),"merge-state-zero");}
        std::memcpy(out,&direct,sizeof direct);
    }
    if(onState)onState();return stateResult;
}
static InputDeviceVtableView vtable;
static InputDeviceView deviceA={&vtable},deviceB={&vtable},deviceC={&vtable};
#define g_FrontEndInput inputStates[0]
#include "owner.inc"
#undef g_FrontEndInput
static void reset() {
    std::memset(inputStates,0,sizeof inputStates);std::memset(keyboard,0,sizeof keyboard);
    std::memset(&joy,0,sizeof joy);std::memset(&direct,0,sizeof direct);
    for(auto &s:inputStates)std::fill(std::begin(s.controllerMappings),std::end(s.controllerMappings),-1);
    for(auto &c:g_InputJoystickCaps){c={};c.xMax=c.yMax=400;}
    joy.x=joy.y=200;joyId=999;joyResult=1;keyboardResult=1;pollResult=stateResult=acquireResult=acquireCalls=0;
    onJoy=onKeyboard=onPoll=onAcquire=onState={};requireZero=false;calls.clear();
    g_FrontEndSupervisorFlags=0;g_InputEnabled=1;g_InputKeyboardDevice=&deviceA;
    for(auto &p:g_InputPadDevices)p=&deviceA;
    g_InputJoystickXThreshold=g_InputJoystickYThreshold=100;
    vtable.Acquire=acquire;vtable.Poll=poll;vtable.GetDeviceState=state;
    std::memset(g_InputJoystickButtons,0x55,sizeof g_InputJoystickButtons);
}
static unsigned short keyMask(int key,bool di) {
    static const int vk[][2]={{0x5a,1},{0x58,2},{0x10,4},{0x1b,8},{0x26,16},{0x68,16},{0x28,32},{0x62,32},{0x25,64},{0x64,64},{0x27,128},{0x66,128},{0x11,256},{0x51,512},{0x53,1024},{0x50,2048},{0x24,2048},{0x0d,4096},{0x44,8192},{0x52,16384},{0x61,0x60},{0x63,0xa0},{0x67,0x50},{0x69,0x90}};
    static const int scan[][2]={{0x2c,1},{0x2d,2},{0x2a,4},{1,8},{0xc8,16},{0x48,16},{0xd0,32},{0x50,32},{0xcb,64},{0x4b,64},{0xcd,128},{0x4d,128},{0x1d,256},{0x10,512},{0x1f,1024},{0x19,2048},{0xc7,2048},{0x1c,4096},{0x20,8192},{0x13,16384},{0x4f,0x60},{0x51,0xa0},{0x47,0x50},{0x49,0x90}};
    unsigned short mask=0;
    if(di){for(auto &p:scan)if(p[0]==key)mask|=p[1];}
    else{for(auto &p:vk)if(p[0]==key)mask|=p[1];}
    return mask;
}
static void cases() {
    reset();joyResult=0;joy.buttons=0xffffffff;CHECK(MergeJoystickInput(0,0,0x200)==0x200,"unbound-buttons");
    reset();joyResult=0;joy.buttons=1;inputStates[1].controllerMappings[0]=0;PollInputFrame(1);CHECK(inputStates[1].current==1&&inputStates[0].current==0,"record-stride");
    for(bool di:{false,true})for(int key=0;key<256;++key)for(unsigned char value:{1,0x80,0xff}) {
        reset();g_FrontEndSupervisorFlags=di?0x200:0;keyboard[key]=value;PollInputFrame(0);
        unsigned short expected=(value&0x80)?keyMask(key,di):0;
        CHECK(inputStates[0].current==expected&&inputStates[0].pressed==expected,"keyboard-bits");
    }
    reset();g_InputEnabled=0;keyboard[0x5a]=0x80;PollInputFrame(0);CHECK(inputStates[0].current==0,"keyboard-enabled");
    reset();keyboardResult=0;keyboard[0x5a]=0x80;PollInputFrame(0);CHECK(inputStates[0].current==1,"keyboard-ignored-bool");
    for(int result:{static_cast<int>(0x8007001e),-1,1}){reset();g_FrontEndSupervisorFlags=0x200;stateResult=result;keyboard[0x2c]=0x80;PollInputFrame(0);CHECK(acquireCalls==1,"keyboard-acquire");CHECK(inputStates[0].current==0,"keyboard-failure-mask");}
    reset();g_InputEnabled=0;joyResult=0;joy.buttons=1;inputStates[0].controllerMappings[0]=0;PollInputFrame(0);CHECK(inputStates[0].current==1,"joystick-when-disabled");
    reset();joyResult=0;joy.buttons=0x80000000;inputStates[1].controllerMappings[8]=31;CHECK(MergeJoystickInput(1,2,0x200)==0x300,"winmm-shift");CHECK(joyId==1,"winmm-index");
    reset();joyResult=0;joy.buttons=0xffffffff;CHECK(MergeJoystickInput(0,0,0x200)==0x200,"unbound-buttons");
    for(unsigned int x:{99u,100u,101u,299u,300u,301u})for(unsigned int y:{99u,100u,101u,299u,300u,301u}){reset();joyResult=0;joy.x=x;joy.y=y;unsigned short expected=(x<100?0x40:0)|(x>300?0x80:0)|(y<100?0x10:0)|(y>300?0x20:0);CHECK(MergeJoystickInput(0,0,0)==expected,"winmm-axes");}
    reset();joyResult=0;g_InputJoystickCaps[2].xMax=800;joy.x=500;CHECK(MergeJoystickInput(0,2,0)==0,"caps-original-index");
    reset();joyResult=0;joy.buttons=0x80000001;auto *b=FrontEndGetControllerState(2);CHECK(b==g_InputJoystickButtons&&joyId==0,"button-query-index");for(int i=0;i<128;++i)CHECK(static_cast<unsigned char>(b[i])==((i==0||i==31)?0x80:0),"button-query-bits");
    reset();g_FrontEndSupervisorFlags=0x400;requireZero=true;direct.buttons[127]=static_cast<signed char>(0x80);inputStates[1].controllerMappings[8]=127;stateResult=1;CHECK(MergeJoystickInput(1,1,0x4000)==0x4100,"merge-positive-result");
    reset();g_FrontEndSupervisorFlags=0x400;direct.buttons[0]=static_cast<signed char>(0x80);inputStates[0].controllerMappings[0]=0;stateResult=-1;CHECK(MergeJoystickInput(0,0,0x4000)==0x4000,"merge-failed-result");
    reset();g_FrontEndSupervisorFlags=0x400;stateResult=-1;direct.buttons[7]=static_cast<signed char>(0x80);CHECK(static_cast<unsigned char>(FrontEndGetControllerState(0)[7])==0x80,"query-unchecked-result");
    for(int x:{-101,-100,0,100,101})for(int y:{-101,-100,0,100,101}){reset();g_FrontEndSupervisorFlags=0x400;direct.x=x;direct.y=y;unsigned short expected=(x< -100?0x40:0)|(x>100?0x80:0)|(y< -100?0x10:0)|(y>100?0x20:0);CHECK(MergeJoystickInput(0,0,0)==expected,"direct-axes");}
    reset();g_FrontEndSupervisorFlags=0x400;pollResult=-1;acquireResult=static_cast<int>(0x8007001e);CHECK(MergeJoystickInput(0,0,0x1234)==0x1234&&acquireCalls==401&&calls.size()==402,"acquire-limit");
    reset();g_FrontEndSupervisorFlags=0x400;pollResult=-1;acquireResult=static_cast<int>(0x8007001e);FrontEndGetControllerState(0);CHECK(acquireCalls==401&&calls.size()==402,"query-acquire-limit");
    reset();g_FrontEndSupervisorFlags=0x400;pollResult=-1;onPoll=[](){g_InputPadDevices[0]=&deviceB;};onAcquire=[](){g_InputPadDevices[0]=&deviceC;acquireResult=acquireCalls<2?static_cast<int>(0x8007001e):0;};MergeJoystickInput(0,0,0);CHECK(calls.size()==3&&calls[1].device==&deviceB&&calls[2].device==&deviceC,"fresh-acquire-owner");
    reset();g_FrontEndSupervisorFlags=0x400;onPoll=[](){g_InputPadDevices[0]=&deviceB;};MergeJoystickInput(0,0,0);CHECK(calls.size()==2&&calls[1].device==&deviceB,"fresh-state-owner");
    reset();g_FrontEndSupervisorFlags=0x400;onPoll=[](){g_InputPadDevices[0]=&deviceB;};FrontEndGetControllerState(0);CHECK(calls.size()==2&&calls[1].device==&deviceB,"query-fresh-state-owner");
    reset();joyResult=0;joy.buttons=2;inputStates[0].controllerMappings[0]=0;onJoy=[](){inputStates[0].controllerMappings[0]=1;};CHECK(MergeJoystickInput(0,0,0)==1,"post-poll-mapping");
    reset();joyResult=0;joy.buttons=1;inputStates[0].controllerMappings[0]=0;onJoy=[](){inputStates[0].current=8;};PollInputFrame(0);CHECK(inputStates[0].previous==8,"late-publication");CHECK(inputStates[0].pressed==1&&inputStates[0].released==8,"frame-edges");
    reset();onKeyboard=[](){g_FrontEndSupervisorFlags=0x400;};direct.buttons[0]=static_cast<signed char>(0x80);inputStates[0].controllerMappings[0]=0;PollInputFrame(0);CHECK(inputStates[0].current==1&&calls.size()==2,"live-backend-flags");
    const unsigned short masks[]={0,1,2,0x8000,0xffff,0xa5a5},before[]={0,24,25,26,65534,65535};
    for(unsigned short held:masks)for(unsigned short previous:masks)for(int seed=0;seed<6;++seed){
        reset();joyResult=0;joy.buttons=held;inputStates[1].controllerMappings[0]=0;inputStates[1].controllerMappings[1]=1;inputStates[1].controllerMappings[2]=2;inputStates[1].controllerMappings[3]=3;inputStates[1].controllerMappings[8]=8;
        // Use keyboard role to supply every gameplay bit, independently of joystick mapping.
        for(int key=0;key<256;++key)if(keyMask(key,false)&&!(keyMask(key,false)&(keyMask(key,false)-1))&&(held&keyMask(key,false)))keyboard[key]=0x80;
        unsigned short actualHeld=static_cast<unsigned short>(held&0x7fff);joyResult=1;
        auto &s=inputStates[1];s.current=previous;s.repeated=0xffff;s.previousRaw=0x5432;s.repeatFlags=0x8811;
        unsigned short expected[16],repeatMask=0;
        for(int bit=0;bit<16;++bit){unsigned int old=before[(seed+bit)%6];s.holdCounters[bit]=old;unsigned int next=(actualHeld&(1u<<bit))?((old+1)&0xffffu):0;if(next>=26){next-=8;repeatMask|=1u<<bit;}expected[bit]=next;}
        auto first=inputStates[0];PollInputFrame(1);
        CHECK(s.current==actualHeld&&s.previous==previous,"record-stride");
        CHECK(s.pressed==static_cast<unsigned short>(actualHeld&~previous)&&s.released==static_cast<unsigned short>(previous&~actualHeld),"frame-edges");
        CHECK(s.repeated==repeatMask,"repeat-mask");for(int bit=0;bit<16;++bit)CHECK(s.holdCounters[bit]==expected[bit],"repeat-counters");
        CHECK(s.previousRaw==0x5432&&s.repeatFlags==0x8811&&std::memcmp(&first,&inputStates[0],sizeof first)==0,"debug-fields-preserved");
    }
    for(short button=0;button<32;++button){unsigned short out=0x4000;CHECK(MergeWinMmButton(&out,button,0x1234,1u<<button)==0x1234&&out==0x5234,"helper-return");out=0x4000;CHECK(MergeWinMmButton(&out,button,0x1234,0)==0&&out==0x4000,"helper-return");}
    signed char buttons[128]={};for(short button=0;button<128;++button){unsigned short out=0x4000;buttons[button]=static_cast<signed char>(0x80);CHECK(MergeDirectButton(&out,button,0x1234,buttons)==0x1234&&out==0x5234,"helper-return");buttons[button]=0;out=0x4000;CHECK(MergeDirectButton(&out,button,0x1234,buttons)==0&&out==0x4000,"helper-return");}
    unsigned short out=0x4000;CHECK(MergeWinMmButton(&out,-1,0xffff,0xffffffff)==0&&MergeDirectButton(&out,-1,0xffff,buttons)==0&&out==0x4000,"helper-return");
}
int main(){try{cases();}catch(const Failure &f){std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
