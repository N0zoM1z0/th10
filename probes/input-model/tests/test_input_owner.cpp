#include "input_owner.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
using namespace input_owner_candidate;
static unsigned int cases = 0;
static void require(bool ok, const char* message) {
    if (!ok) { std::printf("MISMATCH %s case=%u\n",message,cases); std::exit(10); }
}
struct Trace { std::vector<std::string> events; void add(const char* s) { events.push_back(s); } };
struct W : Win32Input {
    Trace& trace; int keyboardStatus; unsigned int joyStatus, id;
    KeyboardState keys; WinMMPosition sampled;
    W(Trace& t) : trace(t), keyboardStatus(1), joyStatus(1), id(99) {
        std::memset(&keys,0,sizeof(keys)); std::memset(&sampled,0,sizeof(sampled));
    }
    int GetKeyboardState(KeyboardState& output) { trace.add("win32-key"); output=keys; return keyboardStatus; }
    unsigned int joyGetPosEx(unsigned int selected, WinMMPosition& output) {
        trace.add("winmm"); id=selected;
        require(output.size==52 && output.flags==255,"WinMM header");
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&output);
        for (unsigned int i=8; i<sizeof(output); ++i) require(bytes[i]==0,"WinMM initialization");
        output=sampled; return joyStatus;
    }
};
struct K : KeyboardDevice {
    Trace& trace; int status, acquireStatus; unsigned int acquires; KeyboardState keys;
    K(Trace& t) : trace(t),status(0),acquireStatus(0),acquires(0) { std::memset(&keys,0,sizeof(keys)); }
    int GetDeviceState(unsigned int size, KeyboardState& output) { trace.add("di-key"); require(size==256,"DI key size"); output=keys; return status; }
    int Acquire() { trace.add("key-acquire"); ++acquires; return acquireStatus; }
};
struct J : JoystickDevice {
    Trace& trace; int pollStatus,stateStatus,terminal; unsigned int lostCount,acquires,gets;
    DIJoystickState sampled;
    J(Trace& t) : trace(t),pollStatus(0),stateStatus(-1),terminal(0),lostCount(0),acquires(0),gets(0) { std::memset(&sampled,0,sizeof(sampled)); }
    int Poll() { trace.add("poll"); return pollStatus; }
    int Acquire() {
        trace.add("joy-acquire"); ++acquires;
        return acquires <= lostCount ? (-2147483647-1)+0x7001e : terminal;
    }
    int GetDeviceState(unsigned int size, DIJoystickState& output) {
        trace.add("di-joy"); ++gets; require(size==272,"DI joy size");
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&output);
        for (unsigned int i=0; i<sizeof(output); ++i) require(bytes[i]==0,"DI joy initialization");
        output=sampled; return stateStatus;
    }
};
struct Fixture {
    Trace trace; W win32; K keyboard; J joystick; JoystickDevice* devices[3];
    AxisCapabilities caps[3]; InputRecord records[2]; Environment env;
    Fixture() : win32(trace), keyboard(trace), joystick(trace), env(win32,keyboard,devices,caps,records,0,true,0,0) {
        for (unsigned int i=0;i<3;++i) {devices[i]=&joystick; caps[i].xMin=100+i; caps[i].xMax=200+i; caps[i].yMin=300+i; caps[i].yMax=400+i;}
        for (unsigned int i=0;i<2;++i) {
            std::memset(&records[i].transition,0,sizeof(records[i].transition));
            records[i].bindings.action1=0; records[i].bindings.action2=1; records[i].bindings.action4=2; records[i].bindings.action8=3; records[i].bindings.action256=4;
        }
    }
};
int main() {
    const int statusValues[4]={0,1,-1,(-2147483647-1)+0x7001e};
    // Keyboard modes, exact nonzero failures, ignored Win32 BOOL, same state index.
    for (unsigned int enabled=0;enabled<2;++enabled)
    for (unsigned int mode=0;mode<2;++mode)
    for (unsigned int s=0;s<4;++s) {
        Fixture f; f.env.keyboardEnabled=enabled!=0; f.env.modeFlags=mode ? 0x200u:0;
        f.win32.keyboardStatus=statusValues[s]; f.keyboard.status=statusValues[s];
        f.win32.keys.keys[0x5a]=128; f.keyboard.keys.keys[0x2c]=128;
        f.records[1].transition.current=2; f.records[1].transition.repeated=65535;
        f.records[1].transition.counters[0]=25;
        const unsigned short expected=enabled && (!mode || !statusValues[s]) ? 1:0;
        require(UpdateInput(f.env,1)==expected,"keyboard/mapper failure result");
        const input_candidate::InputPrefix& state=f.records[1].transition;
        require(state.current==expected && state.previous==2 && state.pressed==expected && state.released==2,"transition fields");
        require(state.repeated==expected && state.counters[0]==(expected?18:0),"transition repeat");
        require(f.records[0].transition.current==0,"record selection");
        require(f.keyboard.acquires==(enabled && mode && statusValues[s]!=0 ? 1u:0u),"key Acquire count");
        const unsigned int calls=enabled ? (mode && statusValues[s]!=0 ? 3u:2u):1u;
        require(f.trace.events.size()==calls && f.trace.events.back()=="winmm","keyboard order and disabled mapper call");
        if(enabled) require(f.trace.events.front()==(mode?"di-key":"win32-key"),"keyboard mode event");
        require(f.win32.id==0,"keyboard raw pad and failure skip axes"); ++cases;
    }
    // Poll failures: exact InputLost continuation and maximum 401 total calls.
    const unsigned int lostCounts[6]={0,1,2,399,400,401};
    for(unsigned int n=0;n<6;++n) for(unsigned int terminal=0;terminal<3;++terminal) {
        Fixture f; f.env.modeFlags=0x400; f.joystick.pollStatus=-1;
        f.joystick.lostCount=lostCounts[n]; f.joystick.terminal=statusValues[terminal];
        require(MapJoystick(f.env,0xcafe1234u,0,2)==0x1234,"Poll failure preserves low16");
        const unsigned int expected=lostCounts[n]>=400 ? 401:lostCounts[n]+1;
        require(f.joystick.acquires==expected && f.joystick.gets==0,"retry count and immediate return");
        require(f.trace.events.size()==expected+1 && f.trace.events.front()=="poll","retry ordering"); ++cases;
    }
    // Failure mask words, both paths. Positive Poll proceeds to GetState.
    for(unsigned int mask=0;mask<65536u;++mask) {
        Fixture f; const unsigned int input=(mask<<16)|(65535u-mask);
        require(MapJoystick(f.env,input,0,2)==65535u-mask,"WinMM failure mask");
        require(f.win32.id==1,"boolean joystick ID");
        f.env.modeFlags=0x400;f.joystick.pollStatus=1;f.joystick.stateStatus=-1;
        require(MapJoystick(f.env,input,0,2)==65535u-mask,"DI failure mask");
        require(f.joystick.gets==1 && f.joystick.acquires==0,"DI state failure path"); ++cases;
    }
    // Both success routes use actual reviewed button and axis helpers.
    for(unsigned int mode=0;mode<2;++mode) for(unsigned int status=0;status<2;++status) {
        Fixture f; f.env.modeFlags=mode?0x400:0; f.win32.joyStatus=0;
        f.win32.sampled.buttons=31;f.win32.sampled.x=126;f.win32.sampled.y=350;
        f.joystick.pollStatus=static_cast<int>(status);f.joystick.stateStatus=static_cast<int>(status);
        for(unsigned int i=0;i<5;++i)f.joystick.sampled.buttons[i]=128;
        f.joystick.sampled.position.x=-12345;f.env.thresholdWord0=-32768;f.env.thresholdWord1=-1;
        const unsigned int combined=0xdead8000u|0x10fu;
        require(MapJoystick(f.env,0xdead8000u,0,2)==static_cast<unsigned short>(combined|(mode?0xf0u:0x40u)),"axis result forwarding");
        if(!mode)require(f.win32.id==1,"boolean ID with raw capability selection");
        ++cases;
    }
    for(unsigned int mapperMode=0;mapperMode<2;++mapperMode)
    for(unsigned int keyMode=0;keyMode<2;++keyMode) {
        Fixture f;f.env.modeFlags=(mapperMode?0x400u:0u)|(keyMode?0x200u:0u);f.joystick.stateStatus=0;
        f.win32.joyStatus=0;f.win32.sampled.buttons=2;f.win32.sampled.x=0;f.win32.sampled.y=350;
        f.win32.keys.keys[0x5a]=128;f.keyboard.keys.keys[0x2c]=128;
        f.joystick.sampled.buttons[1]=128;f.joystick.sampled.position.x=-100;
        f.env.thresholdWord0=10;f.env.thresholdWord1=10;
        f.records[1].transition.current=0x20;f.records[1].transition.counters[0]=25;
        require(UpdateInput(f.env,1)==0x43,"keyboard buttons axes end-to-end");
        const input_candidate::InputPrefix& s=f.records[1].transition;
        require(s.current==0x43 && s.previous==0x20 && s.pressed==0x43 && s.released==0x20 && s.repeated==1,"end-to-end transition");
        require(f.joystick.gets==(mapperMode?1u:0u) && f.trace.events.back()==(mapperMode?"di-joy":"winmm"),"end-to-end real API sequence");++cases;
    }
    std::printf("PASS orchestration_cases=%u all_axes=IMPLEMENTED\n",cases);
}
